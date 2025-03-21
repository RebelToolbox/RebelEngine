// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

const RebelFetch = {
	$RebelFetch__deps: ['$IDHandler', '$RebelRuntime'],
	$RebelFetch: {

		onread: function (id, result) {
			const obj = IDHandler.get(id);
			if (!obj) {
				return;
			}
			if (result.value) {
				obj.chunks.push(result.value);
			}
			obj.reading = false;
			obj.done = result.done;
		},

		onresponse: function (id, response) {
			const obj = IDHandler.get(id);
			if (!obj) {
				return;
			}
			let chunked = false;
			response.headers.forEach(function (value, header) {
				const v = value.toLowerCase().trim();
				const h = header.toLowerCase().trim();
				if (h === 'transfer-encoding' && v === 'chunked') {
					chunked = true;
				}
			});
			obj.status = response.status;
			obj.response = response;
			obj.reader = response.body.getReader();
			obj.chunked = chunked;
		},

		onerror: function (id, err) {
			RebelRuntime.error(err);
			const obj = IDHandler.get(id);
			if (!obj) {
				return;
			}
			obj.error = err;
		},

		create: function (method, url, headers, body) {
			const obj = {
				request: null,
				response: null,
				reader: null,
				error: null,
				done: false,
				reading: false,
				status: 0,
				chunks: [],
				bodySize: -1,
			};
			const id = IDHandler.add(obj);
			const init = {
				method: method,
				headers: headers,
				body: body,
			};
			obj.request = fetch(url, init);
			obj.request.then(RebelFetch.onresponse.bind(null, id)).catch(RebelFetch.onerror.bind(null, id));
			return id;
		},

		free: function (id) {
			const obj = IDHandler.get(id);
			if (!obj) {
				return;
			}
			IDHandler.remove(id);
			if (!obj.request) {
				return;
			}
			// Try to abort
			obj.request.then(function (response) {
				response.abort();
			}).catch(function (e) { /* nothing to do */ });
		},

		read: function (id) {
			const obj = IDHandler.get(id);
			if (!obj) {
				return;
			}
			if (obj.reader && !obj.reading) {
				if (obj.done) {
					obj.reader = null;
					return;
				}
				obj.reading = true;
				obj.reader.read().then(RebelFetch.onread.bind(null, id)).catch(RebelFetch.onerror.bind(null, id));
			}
		},
	},

	rebel_js_fetch_create__sig: 'iiiiiii',
	rebel_js_fetch_create: function (p_method, p_url, p_headers, p_headers_size, p_body, p_body_size) {
		const method = RebelRuntime.parseString(p_method);
		const url = RebelRuntime.parseString(p_url);
		const headers = RebelRuntime.parseStringArray(p_headers, p_headers_size);
		const body = p_body_size ? RebelRuntime.heapSlice(HEAP8, p_body, p_body_size) : null;
		return RebelFetch.create(method, url, headers.map(function (hv) {
			const idx = hv.indexOf(':');
			if (idx <= 0) {
				return [];
			}
			return [
				hv.slice(0, idx).trim(),
				hv.slice(idx + 1).trim(),
			];
		}).filter(function (v) {
			return v.length === 2;
		}), body);
	},

	rebel_js_fetch_state_get__sig: 'ii',
	rebel_js_fetch_state_get: function (p_id) {
		const obj = IDHandler.get(p_id);
		if (!obj) {
			return -1;
		}
		if (obj.error) {
			return -1;
		}
		if (!obj.response) {
			return 0;
		}
		if (obj.reader) {
			return 1;
		}
		if (obj.done) {
			return 2;
		}
		return -1;
	},

	rebel_js_fetch_http_status_get__sig: 'ii',
	rebel_js_fetch_http_status_get: function (p_id) {
		const obj = IDHandler.get(p_id);
		if (!obj || !obj.response) {
			return 0;
		}
		return obj.status;
	},

	rebel_js_fetch_read_headers__sig: 'iiii',
	rebel_js_fetch_read_headers: function (p_id, p_parse_cb, p_ref) {
		const obj = IDHandler.get(p_id);
		if (!obj || !obj.response) {
			return 1;
		}
		const cb = RebelRuntime.get_func(p_parse_cb);
		const arr = [];
		obj.response.headers.forEach(function (v, h) {
			arr.push(`${h}:${v}`);
		});
		const c_ptr = RebelRuntime.allocStringArray(arr);
		cb(arr.length, c_ptr, p_ref);
		RebelRuntime.freeStringArray(c_ptr, arr.length);
		return 0;
	},

	rebel_js_fetch_read_chunk__sig: 'iiii',
	rebel_js_fetch_read_chunk: function (p_id, p_buf, p_buf_size) {
		const obj = IDHandler.get(p_id);
		if (!obj || !obj.response) {
			return 0;
		}
		let to_read = p_buf_size;
		const chunks = obj.chunks;
		while (to_read && chunks.length) {
			const chunk = obj.chunks[0];
			if (chunk.length > to_read) {
				RebelRuntime.heapCopy(HEAP8, chunk.slice(0, to_read), p_buf);
				chunks[0] = chunk.slice(to_read);
				to_read = 0;
			} else {
				RebelRuntime.heapCopy(HEAP8, chunk, p_buf);
				to_read -= chunk.length;
				chunks.pop();
			}
		}
		if (!chunks.length) {
			RebelFetch.read(p_id);
		}
		return p_buf_size - to_read;
	},

	rebel_js_fetch_body_length_get__sig: 'ii',
	rebel_js_fetch_body_length_get: function (p_id) {
		const obj = IDHandler.get(p_id);
		if (!obj || !obj.response) {
			return -1;
		}
		return obj.bodySize;
	},

	rebel_js_fetch_is_chunked__sig: 'ii',
	rebel_js_fetch_is_chunked: function (p_id) {
		const obj = IDHandler.get(p_id);
		if (!obj || !obj.response) {
			return -1;
		}
		return obj.chunked ? 1 : 0;
	},

	rebel_js_fetch_free__sig: 'vi',
	rebel_js_fetch_free: function (id) {
		RebelFetch.free(id);
	},
};

autoAddDeps(RebelFetch, '$RebelFetch');
mergeInto(LibraryManager.library, RebelFetch);
