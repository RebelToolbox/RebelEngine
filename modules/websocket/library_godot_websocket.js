// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

const RebelWebSocket = {
	// Our socket implementation that forwards events to C++.
	$RebelWebSocket__deps: ['$IDHandler', '$RebelRuntime'],
	$RebelWebSocket: {
		// Connection opened, report selected protocol
		_onopen: function (p_id, callback, event) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return; // Rebel object is gone.
			}
			const c_str = RebelRuntime.allocString(ref.protocol);
			callback(c_str);
			RebelRuntime.free(c_str);
		},

		// Message received, report content and type (UTF8 vs binary)
		_onmessage: function (p_id, callback, event) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return; // Rebel object is gone.
			}
			let buffer;
			let is_string = 0;
			if (event.data instanceof ArrayBuffer) {
				buffer = new Uint8Array(event.data);
			} else if (event.data instanceof Blob) {
				RebelRuntime.error('Blob type not supported');
				return;
			} else if (typeof event.data === 'string') {
				is_string = 1;
				const enc = new TextEncoder('utf-8');
				buffer = new Uint8Array(enc.encode(event.data));
			} else {
				RebelRuntime.error('Unknown message type');
				return;
			}
			const len = buffer.length * buffer.BYTES_PER_ELEMENT;
			const out = RebelRuntime.malloc(len);
			HEAPU8.set(buffer, out);
			callback(out, len, is_string);
			RebelRuntime.free(out);
		},

		// An error happened, 'onclose' will be called after this.
		_onerror: function (p_id, callback, event) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return; // Rebel object is gone.
			}
			callback();
		},

		// Connection is closed, this is always fired. Report close code, reason, and clean status.
		_onclose: function (p_id, callback, event) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return; // Rebel object is gone.
			}
			const c_str = RebelRuntime.allocString(event.reason);
			callback(event.code, c_str, event.wasClean ? 1 : 0);
			RebelRuntime.free(c_str);
		},

		// Send a message
		send: function (p_id, p_data) {
			const ref = IDHandler.get(p_id);
			if (!ref || ref.readyState !== ref.OPEN) {
				return 1; // Rebel object is gone or socket is not in a ready state.
			}
			ref.send(p_data);
			return 0;
		},

		// Get current bufferedAmount
		bufferedAmount: function (p_id) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return 0; // Rebel object is gone.
			}
			return ref.bufferedAmount;
		},

		create: function (socket, p_on_open, p_on_message, p_on_error, p_on_close) {
			const id = IDHandler.add(socket);
			socket.onopen = RebelWebSocket._onopen.bind(null, id, p_on_open);
			socket.onmessage = RebelWebSocket._onmessage.bind(null, id, p_on_message);
			socket.onerror = RebelWebSocket._onerror.bind(null, id, p_on_error);
			socket.onclose = RebelWebSocket._onclose.bind(null, id, p_on_close);
			return id;
		},

		// Closes the JavaScript WebSocket (if not already closing) associated to a given C++ object.
		close: function (p_id, p_code, p_reason) {
			const ref = IDHandler.get(p_id);
			if (ref && ref.readyState < ref.CLOSING) {
				const code = p_code;
				const reason = RebelRuntime.parseString(p_reason);
				ref.close(code, reason);
			}
		},

		// Deletes the reference to a C++ object (closing any connected socket if necessary).
		destroy: function (p_id) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return;
			}
			RebelWebSocket.close(p_id, 1001, '');
			IDHandler.remove(p_id);
			ref.onopen = null;
			ref.onmessage = null;
			ref.onerror = null;
			ref.onclose = null;
		},
	},

	rebel_js_websocket_create__sig: 'iiiiiiii',
	rebel_js_websocket_create: function (p_ref, p_url, p_proto, p_on_open, p_on_message, p_on_error, p_on_close) {
		const on_open = RebelRuntime.get_func(p_on_open).bind(null, p_ref);
		const on_message = RebelRuntime.get_func(p_on_message).bind(null, p_ref);
		const on_error = RebelRuntime.get_func(p_on_error).bind(null, p_ref);
		const on_close = RebelRuntime.get_func(p_on_close).bind(null, p_ref);
		const url = RebelRuntime.parseString(p_url);
		const protos = RebelRuntime.parseString(p_proto);
		let socket = null;
		try {
			if (protos) {
				socket = new WebSocket(url, protos.split(','));
			} else {
				socket = new WebSocket(url);
			}
		} catch (e) {
			return 0;
		}
		socket.binaryType = 'arraybuffer';
		return RebelWebSocket.create(socket, on_open, on_message, on_error, on_close);
	},

	rebel_js_websocket_send__sig: 'iiiii',
	rebel_js_websocket_send: function (p_id, p_buf, p_buf_len, p_raw) {
		const bytes_array = new Uint8Array(p_buf_len);
		let i = 0;
		for (i = 0; i < p_buf_len; i++) {
			bytes_array[i] = RebelRuntime.getHeapValue(p_buf + i, 'i8');
		}
		let out = bytes_array.buffer;
		if (!p_raw) {
			out = new TextDecoder('utf-8').decode(bytes_array);
		}
		return RebelWebSocket.send(p_id, out);
	},

	rebel_js_websocket_buffered_amount__sig: 'ii',
	rebel_js_websocket_buffered_amount: function (p_id) {
		return RebelWebSocket.bufferedAmount(p_id);
	},

	rebel_js_websocket_close__sig: 'viii',
	rebel_js_websocket_close: function (p_id, p_code, p_reason) {
		const code = p_code;
		const reason = RebelRuntime.parseString(p_reason);
		RebelWebSocket.close(p_id, code, reason);
	},

	rebel_js_websocket_destroy__sig: 'vi',
	rebel_js_websocket_destroy: function (p_id) {
		RebelWebSocket.destroy(p_id);
	},
};

autoAddDeps(RebelWebSocket, '$RebelWebSocket');
mergeInto(LibraryManager.library, RebelWebSocket);
