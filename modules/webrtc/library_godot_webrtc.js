// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

const RebelRTCDataChannel = {
	// Our socket implementation that forwards events to C++.
	$RebelRTCDataChannel__deps: ['$IDHandler', '$RebelRuntime'],
	$RebelRTCDataChannel: {
		connect: function (p_id, p_on_open, p_on_message, p_on_error, p_on_close) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return;
			}

			ref.binaryType = 'arraybuffer';
			ref.onopen = function (event) {
				p_on_open();
			};
			ref.onclose = function (event) {
				p_on_close();
			};
			ref.onerror = function (event) {
				p_on_error();
			};
			ref.onmessage = function (event) {
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
				p_on_message(out, len, is_string);
				RebelRuntime.free(out);
			};
		},

		close: function (p_id) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return;
			}
			ref.onopen = null;
			ref.onmessage = null;
			ref.onerror = null;
			ref.onclose = null;
			ref.close();
		},

		get_prop: function (p_id, p_prop, p_def) {
			const ref = IDHandler.get(p_id);
			return (ref && ref[p_prop] !== undefined) ? ref[p_prop] : p_def;
		},
	},

	rebel_js_rtc_datachannel_ready_state_get__sig: 'ii',
	rebel_js_rtc_datachannel_ready_state_get: function (p_id) {
		const ref = IDHandler.get(p_id);
		if (!ref) {
			return 3; // CLOSED
		}

		switch (ref.readyState) {
		case 'connecting':
			return 0;
		case 'open':
			return 1;
		case 'closing':
			return 2;
		case 'closed':
		default:
			return 3;
		}
	},

	rebel_js_rtc_datachannel_send__sig: 'iiiii',
	rebel_js_rtc_datachannel_send: function (p_id, p_buffer, p_length, p_raw) {
		const ref = IDHandler.get(p_id);
		if (!ref) {
			return 1;
		}

		const bytes_array = new Uint8Array(p_length);
		for (let i = 0; i < p_length; i++) {
			bytes_array[i] = RebelRuntime.getHeapValue(p_buffer + i, 'i8');
		}

		if (p_raw) {
			ref.send(bytes_array.buffer);
		} else {
			const string = new TextDecoder('utf-8').decode(bytes_array);
			ref.send(string);
		}
		return 0;
	},

	rebel_js_rtc_datachannel_is_ordered__sig: 'ii',
	rebel_js_rtc_datachannel_is_ordered: function (p_id) {
		return RebelRTCDataChannel.get_prop(p_id, 'ordered', true);
	},

	rebel_js_rtc_datachannel_id_get__sig: 'ii',
	rebel_js_rtc_datachannel_id_get: function (p_id) {
		return RebelRTCDataChannel.get_prop(p_id, 'id', 65535);
	},

	rebel_js_rtc_datachannel_max_packet_lifetime_get__sig: 'ii',
	rebel_js_rtc_datachannel_max_packet_lifetime_get: function (p_id) {
		const ref = IDHandler.get(p_id);
		if (!ref) {
			return 65535;
		}
		if (ref['maxPacketLifeTime'] !== undefined) {
			return ref['maxPacketLifeTime'];
		} else if (ref['maxRetransmitTime'] !== undefined) {
			// Guess someone didn't appreciate the standardization process.
			return ref['maxRetransmitTime'];
		}
		return 65535;
	},

	rebel_js_rtc_datachannel_max_retransmits_get__sig: 'ii',
	rebel_js_rtc_datachannel_max_retransmits_get: function (p_id) {
		return RebelRTCDataChannel.get_prop(p_id, 'maxRetransmits', 65535);
	},

	rebel_js_rtc_datachannel_is_negotiated__sig: 'ii',
	rebel_js_rtc_datachannel_is_negotiated: function (p_id) {
		return RebelRTCDataChannel.get_prop(p_id, 'negotiated', 65535);
	},

	rebel_js_rtc_datachannel_get_buffered_amount__sig: 'ii',
	rebel_js_rtc_datachannel_get_buffered_amount: function (p_id) {
		return RebelRTCDataChannel.get_prop(p_id, 'bufferedAmount', 0);
	},

	rebel_js_rtc_datachannel_label_get__sig: 'ii',
	rebel_js_rtc_datachannel_label_get: function (p_id) {
		const ref = IDHandler.get(p_id);
		if (!ref || !ref.label) {
			return 0;
		}
		return RebelRuntime.allocString(ref.label);
	},

	rebel_js_rtc_datachannel_protocol_get__sig: 'ii',
	rebel_js_rtc_datachannel_protocol_get: function (p_id) {
		const ref = IDHandler.get(p_id);
		if (!ref || !ref.protocol) {
			return 0;
		}
		return RebelRuntime.allocString(ref.protocol);
	},

	rebel_js_rtc_datachannel_destroy__sig: 'vi',
	rebel_js_rtc_datachannel_destroy: function (p_id) {
		RebelRTCDataChannel.close(p_id);
		IDHandler.remove(p_id);
	},

	rebel_js_rtc_datachannel_connect__sig: 'viiiiii',
	rebel_js_rtc_datachannel_connect: function (p_id, p_ref, p_on_open, p_on_message, p_on_error, p_on_close) {
		const onopen = RebelRuntime.get_func(p_on_open).bind(null, p_ref);
		const onmessage = RebelRuntime.get_func(p_on_message).bind(null, p_ref);
		const onerror = RebelRuntime.get_func(p_on_error).bind(null, p_ref);
		const onclose = RebelRuntime.get_func(p_on_close).bind(null, p_ref);
		RebelRTCDataChannel.connect(p_id, onopen, onmessage, onerror, onclose);
	},

	rebel_js_rtc_datachannel_close__sig: 'vi',
	rebel_js_rtc_datachannel_close: function (p_id) {
		const ref = IDHandler.get(p_id);
		if (!ref) {
			return;
		}
		RebelRTCDataChannel.close(p_id);
	},
};

autoAddDeps(RebelRTCDataChannel, '$RebelRTCDataChannel');
mergeInto(LibraryManager.library, RebelRTCDataChannel);

const RebelRTCPeerConnection = {
	$RebelRTCPeerConnection__deps: ['$IDHandler', '$RebelRuntime', '$RebelRTCDataChannel'],
	$RebelRTCPeerConnection: {
		onstatechange: function (p_id, p_conn, callback, event) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return;
			}
			let state;
			switch (p_conn.iceConnectionState) {
			case 'new':
				state = 0;
				break;
			case 'checking':
				state = 1;
				break;
			case 'connected':
			case 'completed':
				state = 2;
				break;
			case 'disconnected':
				state = 3;
				break;
			case 'failed':
				state = 4;
				break;
			case 'closed':
			default:
				state = 5;
				break;
			}
			callback(state);
		},

		onicecandidate: function (p_id, callback, event) {
			const ref = IDHandler.get(p_id);
			if (!ref || !event.candidate) {
				return;
			}

			const c = event.candidate;
			const candidate_str = RebelRuntime.allocString(c.candidate);
			const mid_str = RebelRuntime.allocString(c.sdpMid);
			callback(mid_str, c.sdpMLineIndex, candidate_str);
			RebelRuntime.free(candidate_str);
			RebelRuntime.free(mid_str);
		},

		ondatachannel: function (p_id, callback, event) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return;
			}

			const cid = IDHandler.add(event.channel);
			callback(cid);
		},

		onsession: function (p_id, callback, session) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return;
			}
			const type_str = RebelRuntime.allocString(session.type);
			const sdp_str = RebelRuntime.allocString(session.sdp);
			callback(type_str, sdp_str);
			RebelRuntime.free(type_str);
			RebelRuntime.free(sdp_str);
		},

		onerror: function (p_id, callback, error) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return;
			}
			RebelRuntime.error(error);
			callback();
		},
	},

	rebel_js_rtc_pc_create__sig: 'iiiiii',
	rebel_js_rtc_pc_create: function (p_config, p_ref, p_on_state_change, p_on_candidate, p_on_datachannel) {
		const onstatechange = RebelRuntime.get_func(p_on_state_change).bind(null, p_ref);
		const oncandidate = RebelRuntime.get_func(p_on_candidate).bind(null, p_ref);
		const ondatachannel = RebelRuntime.get_func(p_on_datachannel).bind(null, p_ref);

		const config = JSON.parse(RebelRuntime.parseString(p_config));
		let conn = null;
		try {
			conn = new RTCPeerConnection(config);
		} catch (e) {
			RebelRuntime.error(e);
			return 0;
		}

		const base = RebelRTCPeerConnection;
		const id = IDHandler.add(conn);
		conn.oniceconnectionstatechange = base.onstatechange.bind(null, id, conn, onstatechange);
		conn.onicecandidate = base.onicecandidate.bind(null, id, oncandidate);
		conn.ondatachannel = base.ondatachannel.bind(null, id, ondatachannel);
		return id;
	},

	rebel_js_rtc_pc_close__sig: 'vi',
	rebel_js_rtc_pc_close: function (p_id) {
		const ref = IDHandler.get(p_id);
		if (!ref) {
			return;
		}
		ref.close();
	},

	rebel_js_rtc_pc_destroy__sig: 'vi',
	rebel_js_rtc_pc_destroy: function (p_id) {
		const ref = IDHandler.get(p_id);
		if (!ref) {
			return;
		}
		ref.oniceconnectionstatechange = null;
		ref.onicecandidate = null;
		ref.ondatachannel = null;
		IDHandler.remove(p_id);
	},

	rebel_js_rtc_pc_offer_create__sig: 'viiii',
	rebel_js_rtc_pc_offer_create: function (p_id, p_obj, p_on_session, p_on_error) {
		const ref = IDHandler.get(p_id);
		if (!ref) {
			return;
		}
		const onsession = RebelRuntime.get_func(p_on_session).bind(null, p_obj);
		const onerror = RebelRuntime.get_func(p_on_error).bind(null, p_obj);
		ref.createOffer().then(function (session) {
			RebelRTCPeerConnection.onsession(p_id, onsession, session);
		}).catch(function (error) {
			RebelRTCPeerConnection.onerror(p_id, onerror, error);
		});
	},

	rebel_js_rtc_pc_local_description_set__sig: 'viiiii',
	rebel_js_rtc_pc_local_description_set: function (p_id, p_type, p_sdp, p_obj, p_on_error) {
		const ref = IDHandler.get(p_id);
		if (!ref) {
			return;
		}
		const type = RebelRuntime.parseString(p_type);
		const sdp = RebelRuntime.parseString(p_sdp);
		const onerror = RebelRuntime.get_func(p_on_error).bind(null, p_obj);
		ref.setLocalDescription({
			'sdp': sdp,
			'type': type,
		}).catch(function (error) {
			RebelRTCPeerConnection.onerror(p_id, onerror, error);
		});
	},

	rebel_js_rtc_pc_remote_description_set__sig: 'viiiiii',
	rebel_js_rtc_pc_remote_description_set: function (p_id, p_type, p_sdp, p_obj, p_session_created, p_on_error) {
		const ref = IDHandler.get(p_id);
		if (!ref) {
			return;
		}
		const type = RebelRuntime.parseString(p_type);
		const sdp = RebelRuntime.parseString(p_sdp);
		const onerror = RebelRuntime.get_func(p_on_error).bind(null, p_obj);
		const onsession = RebelRuntime.get_func(p_session_created).bind(null, p_obj);
		ref.setRemoteDescription({
			'sdp': sdp,
			'type': type,
		}).then(function () {
			if (type !== 'offer') {
				return Promise.resolve();
			}
			return ref.createAnswer().then(function (session) {
				RebelRTCPeerConnection.onsession(p_id, onsession, session);
			});
		}).catch(function (error) {
			RebelRTCPeerConnection.onerror(p_id, onerror, error);
		});
	},

	rebel_js_rtc_pc_ice_candidate_add__sig: 'viiii',
	rebel_js_rtc_pc_ice_candidate_add: function (p_id, p_mid_name, p_mline_idx, p_sdp) {
		const ref = IDHandler.get(p_id);
		if (!ref) {
			return;
		}
		const sdpMidName = RebelRuntime.parseString(p_mid_name);
		const sdpName = RebelRuntime.parseString(p_sdp);
		ref.addIceCandidate(new RTCIceCandidate({
			'candidate': sdpName,
			'sdpMid': sdpMidName,
			'sdpMlineIndex': p_mline_idx,
		}));
	},

	rebel_js_rtc_pc_datachannel_create__deps: ['$RebelRTCDataChannel'],
	rebel_js_rtc_pc_datachannel_create__sig: 'iiii',
	rebel_js_rtc_pc_datachannel_create: function (p_id, p_label, p_config) {
		try {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return 0;
			}

			const label = RebelRuntime.parseString(p_label);
			const config = JSON.parse(RebelRuntime.parseString(p_config));

			const channel = ref.createDataChannel(label, config);
			return IDHandler.add(channel);
		} catch (e) {
			RebelRuntime.error(e);
			return 0;
		}
	},
};

autoAddDeps(RebelRTCPeerConnection, '$RebelRTCPeerConnection');
mergeInto(LibraryManager.library, RebelRTCPeerConnection);
