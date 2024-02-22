// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

const RebelAudio = {
	$RebelAudio__deps: ['$RebelRuntime', '$RebelOS'],
	$RebelAudio: {
		ctx: null,
		input: null,
		driver: null,
		interval: 0,

		init: function (mix_rate, latency, onstatechange, onlatencyupdate) {
			const opts = {};
			// If mix_rate is 0, let the browser choose.
			if (mix_rate) {
				opts['sampleRate'] = mix_rate;
			}
			// Do not specify, leave 'interactive' for good performance.
			// opts['latencyHint'] = latency / 1000;
			const ctx = new (window.AudioContext || window.webkitAudioContext)(opts);
			RebelAudio.ctx = ctx;
			ctx.onstatechange = function () {
				let state = 0;
				switch (ctx.state) {
				case 'suspended':
					state = 0;
					break;
				case 'running':
					state = 1;
					break;
				case 'closed':
					state = 2;
					break;

					// no default
				}
				onstatechange(state);
			};
			ctx.onstatechange(); // Immediately notify state.
			// Update computed latency
			RebelAudio.interval = setInterval(function () {
				let computed_latency = 0;
				if (ctx.baseLatency) {
					computed_latency += RebelAudio.ctx.baseLatency;
				}
				if (ctx.outputLatency) {
					computed_latency += RebelAudio.ctx.outputLatency;
				}
				onlatencyupdate(computed_latency);
			}, 1000);
			RebelOS.atexit(RebelAudio.close_async);
			return ctx.destination.channelCount;
		},

		create_input: function (callback) {
			if (RebelAudio.input) {
				return 0; // Already started.
			}
			function gotMediaInput(stream) {
				try {
					RebelAudio.input = RebelAudio.ctx.createMediaStreamSource(stream);
					callback(RebelAudio.input);
				} catch (e) {
					RebelRuntime.error('Failed creaating input.', e);
				}
			}
			if (navigator.mediaDevices && navigator.mediaDevices.getUserMedia) {
				navigator.mediaDevices.getUserMedia({
					'audio': true,
				}).then(gotMediaInput, function (e) {
					RebelRuntime.error('Error getting user media.', e);
				});
			} else {
				if (!navigator.getUserMedia) {
					navigator.getUserMedia = navigator.webkitGetUserMedia || navigator.mozGetUserMedia;
				}
				if (!navigator.getUserMedia) {
					RebelRuntime.error('getUserMedia not available.');
					return 1;
				}
				navigator.getUserMedia({
					'audio': true,
				}, gotMediaInput, function (e) {
					RebelRuntime.print(e);
				});
			}
			return 0;
		},

		close_async: function (resolve, reject) {
			const ctx = RebelAudio.ctx;
			RebelAudio.ctx = null;
			// Audio was not initialized.
			if (!ctx) {
				resolve();
				return;
			}
			// Remove latency callback
			if (RebelAudio.interval) {
				clearInterval(RebelAudio.interval);
				RebelAudio.interval = 0;
			}
			// Disconnect input, if it was started.
			if (RebelAudio.input) {
				RebelAudio.input.disconnect();
				RebelAudio.input = null;
			}
			// Disconnect output
			let closed = Promise.resolve();
			if (RebelAudio.driver) {
				closed = RebelAudio.driver.close();
			}
			closed.then(function () {
				return ctx.close();
			}).then(function () {
				ctx.onstatechange = null;
				resolve();
			}).catch(function (e) {
				ctx.onstatechange = null;
				RebelRuntime.error('Error closing AudioContext', e);
				resolve();
			});
		},
	},

	rebel_audio_is_available__sig: 'i',
	rebel_audio_is_available__proxy: 'sync',
	rebel_audio_is_available: function () {
		if (!(window.AudioContext || window.webkitAudioContext)) {
			return 0;
		}
		return 1;
	},

	rebel_audio_has_worklet__sig: 'i',
	rebel_audio_has_worklet: function () {
		return (RebelAudio.ctx && RebelAudio.ctx.audioWorklet) ? 1 : 0;
	},

	rebel_audio_has_script_processor__sig: 'i',
	rebel_audio_has_script_processor: function () {
		return (RebelAudio.ctx && RebelAudio.ctx.createScriptProcessor) ? 1 : 0;
	},

	rebel_audio_init__sig: 'iiiii',
	rebel_audio_init: function (p_mix_rate, p_latency, p_state_change, p_latency_update) {
		const statechange = RebelRuntime.get_func(p_state_change);
		const latencyupdate = RebelRuntime.get_func(p_latency_update);
		const mix_rate = RebelRuntime.getHeapValue(p_mix_rate, 'i32');
		const channels = RebelAudio.init(mix_rate, p_latency, statechange, latencyupdate);
		RebelRuntime.setHeapValue(p_mix_rate, RebelAudio.ctx.sampleRate, 'i32');
		return channels;
	},

	rebel_audio_resume__sig: 'v',
	rebel_audio_resume: function () {
		if (RebelAudio.ctx && RebelAudio.ctx.state !== 'running') {
			RebelAudio.ctx.resume();
		}
	},

	rebel_audio_capture_start__proxy: 'sync',
	rebel_audio_capture_start__sig: 'i',
	rebel_audio_capture_start: function () {
		return RebelAudio.create_input(function (input) {
			input.connect(RebelAudio.driver.get_node());
		});
	},

	rebel_audio_capture_stop__proxy: 'sync',
	rebel_audio_capture_stop__sig: 'v',
	rebel_audio_capture_stop: function () {
		if (RebelAudio.input) {
			const tracks = RebelAudio.input['mediaStream']['getTracks']();
			for (let i = 0; i < tracks.length; i++) {
				tracks[i]['stop']();
			}
			RebelAudio.input.disconnect();
			RebelAudio.input = null;
		}
	},
};

autoAddDeps(RebelAudio, '$RebelAudio');
mergeInto(LibraryManager.library, RebelAudio);

/**
 * The AudioWorklet API driver, used when threads are available.
 */
const RebelAudioWorklet = {
	$RebelAudioWorklet__deps: ['$RebelAudio', '$RebelConfig'],
	$RebelAudioWorklet: {
		promise: null,
		worklet: null,
		ring_buffer: null,

		create: function (channels) {
			const path = RebelConfig.locate_file('rebel.audio.worklet.js');
			RebelAudioWorklet.promise = RebelAudio.ctx.audioWorklet.addModule(path).then(function () {
				RebelAudioWorklet.worklet = new AudioWorkletNode(
					RebelAudio.ctx,
					'rebel-processor',
					{
						'outputChannelCount': [channels],
					}
				);
				return Promise.resolve();
			});
			RebelAudio.driver = RebelAudioWorklet;
		},

		start: function (in_buf, out_buf, state) {
			RebelAudioWorklet.promise.then(function () {
				const node = RebelAudioWorklet.worklet;
				node.connect(RebelAudio.ctx.destination);
				node.port.postMessage({
					'cmd': 'start',
					'data': [state, in_buf, out_buf],
				});
				node.port.onmessage = function (event) {
					RebelRuntime.error(event.data);
				};
			});
		},

		start_no_threads: function (p_out_buf, p_out_size, out_callback, p_in_buf, p_in_size, in_callback) {
			function RingBuffer() {
				let wpos = 0;
				let rpos = 0;
				let pending_samples = 0;
				const wbuf = new Float32Array(p_out_size);

				function send(port) {
					if (pending_samples === 0) {
						return;
					}
					const buffer = RebelRuntime.heapSub(HEAPF32, p_out_buf, p_out_size);
					const size = buffer.length;
					const tot_sent = pending_samples;
					out_callback(wpos, pending_samples);
					if (wpos + pending_samples >= size) {
						const high = size - wpos;
						wbuf.set(buffer.subarray(wpos, size));
						pending_samples -= high;
						wpos = 0;
					}
					if (pending_samples > 0) {
						wbuf.set(buffer.subarray(wpos, wpos + pending_samples), tot_sent - pending_samples);
					}
					port.postMessage({ 'cmd': 'chunk', 'data': wbuf.subarray(0, tot_sent) });
					wpos += pending_samples;
					pending_samples = 0;
				}
				this.receive = function (recv_buf) {
					const buffer = RebelRuntime.heapSub(HEAPF32, p_in_buf, p_in_size);
					const from = rpos;
					let to_write = recv_buf.length;
					let high = 0;
					if (rpos + to_write >= p_in_size) {
						high = p_in_size - rpos;
						buffer.set(recv_buf.subarray(0, high), rpos);
						to_write -= high;
						rpos = 0;
					}
					if (to_write) {
						buffer.set(recv_buf.subarray(high, to_write), rpos);
					}
					in_callback(from, recv_buf.length);
					rpos += to_write;
				};
				this.consumed = function (size, port) {
					pending_samples += size;
					send(port);
				};
			}
			RebelAudioWorklet.ring_buffer = new RingBuffer();
			RebelAudioWorklet.promise.then(function () {
				const node = RebelAudioWorklet.worklet;
				const buffer = RebelRuntime.heapSlice(HEAPF32, p_out_buf, p_out_size);
				node.connect(RebelAudio.ctx.destination);
				node.port.postMessage({
					'cmd': 'start_nothreads',
					'data': [buffer, p_in_size],
				});
				node.port.onmessage = function (event) {
					if (!RebelAudioWorklet.worklet) {
						return;
					}
					if (event.data['cmd'] === 'read') {
						const read = event.data['data'];
						RebelAudioWorklet.ring_buffer.consumed(read, RebelAudioWorklet.worklet.port);
					} else if (event.data['cmd'] === 'input') {
						const buf = event.data['data'];
						if (buf.length > p_in_size) {
							RebelRuntime.error('Input chunk is too big');
							return;
						}
						RebelAudioWorklet.ring_buffer.receive(buf);
					} else {
						RebelRuntime.error(event.data);
					}
				};
			});
		},

		get_node: function () {
			return RebelAudioWorklet.worklet;
		},

		close: function () {
			return new Promise(function (resolve, reject) {
				if (RebelAudioWorklet.promise === null) {
					return;
				}
				RebelAudioWorklet.promise.then(function () {
					RebelAudioWorklet.worklet.port.postMessage({
						'cmd': 'stop',
						'data': null,
					});
					RebelAudioWorklet.worklet.disconnect();
					RebelAudioWorklet.worklet = null;
					RebelAudioWorklet.promise = null;
					resolve();
				}).catch(function (err) { /* aborted? */ });
			});
		},
	},

	rebel_audio_worklet_create__sig: 'ii',
	rebel_audio_worklet_create: function (channels) {
		try {
			RebelAudioWorklet.create(channels);
		} catch (e) {
			RebelRuntime.error('Error starting AudioDriverWorklet', e);
			return 1;
		}
		return 0;
	},

	rebel_audio_worklet_start__sig: 'viiiii',
	rebel_audio_worklet_start: function (p_in_buf, p_in_size, p_out_buf, p_out_size, p_state) {
		const out_buffer = RebelRuntime.heapSub(HEAPF32, p_out_buf, p_out_size);
		const in_buffer = RebelRuntime.heapSub(HEAPF32, p_in_buf, p_in_size);
		const state = RebelRuntime.heapSub(HEAP32, p_state, 4);
		RebelAudioWorklet.start(in_buffer, out_buffer, state);
	},

	rebel_audio_worklet_start_no_threads__sig: 'viiiiii',
	rebel_audio_worklet_start_no_threads: function (p_out_buf, p_out_size, p_out_callback, p_in_buf, p_in_size, p_in_callback) {
		const out_callback = RebelRuntime.get_func(p_out_callback);
		const in_callback = RebelRuntime.get_func(p_in_callback);
		RebelAudioWorklet.start_no_threads(p_out_buf, p_out_size, out_callback, p_in_buf, p_in_size, in_callback);
	},

	rebel_audio_worklet_state_wait__sig: 'iiii',
	rebel_audio_worklet_state_wait: function (p_state, p_idx, p_expected, p_timeout) {
		Atomics.wait(HEAP32, (p_state >> 2) + p_idx, p_expected, p_timeout);
		return Atomics.load(HEAP32, (p_state >> 2) + p_idx);
	},

	rebel_audio_worklet_state_add__sig: 'iiii',
	rebel_audio_worklet_state_add: function (p_state, p_idx, p_value) {
		return Atomics.add(HEAP32, (p_state >> 2) + p_idx, p_value);
	},

	rebel_audio_worklet_state_get__sig: 'iii',
	rebel_audio_worklet_state_get: function (p_state, p_idx) {
		return Atomics.load(HEAP32, (p_state >> 2) + p_idx);
	},
};

autoAddDeps(RebelAudioWorklet, '$RebelAudioWorklet');
mergeInto(LibraryManager.library, RebelAudioWorklet);

/*
 * The deprecated ScriptProcessorNode API, used when threads are disabled.
 */
const RebelAudioScript = {
	$RebelAudioScript__deps: ['$RebelAudio'],
	$RebelAudioScript: {
		script: null,

		create: function (buffer_length, channel_count) {
			RebelAudioScript.script = RebelAudio.ctx.createScriptProcessor(buffer_length, 2, channel_count);
			RebelAudio.driver = RebelAudioScript;
			return RebelAudioScript.script.bufferSize;
		},

		start: function (p_in_buf, p_in_size, p_out_buf, p_out_size, onprocess) {
			RebelAudioScript.script.onaudioprocess = function (event) {
				// Read input
				const inb = RebelRuntime.heapSub(HEAPF32, p_in_buf, p_in_size);
				const input = event.inputBuffer;
				if (RebelAudio.input) {
					const inlen = input.getChannelData(0).length;
					for (let ch = 0; ch < 2; ch++) {
						const data = input.getChannelData(ch);
						for (let s = 0; s < inlen; s++) {
							inb[s * 2 + ch] = data[s];
						}
					}
				}

				// Let Rebel process the input/output.
				onprocess();

				// Write the output.
				const outb = RebelRuntime.heapSub(HEAPF32, p_out_buf, p_out_size);
				const output = event.outputBuffer;
				const channels = output.numberOfChannels;
				for (let ch = 0; ch < channels; ch++) {
					const data = output.getChannelData(ch);
					// Loop through samples and assign computed values.
					for (let sample = 0; sample < data.length; sample++) {
						data[sample] = outb[sample * channels + ch];
					}
				}
			};
			RebelAudioScript.script.connect(RebelAudio.ctx.destination);
		},

		get_node: function () {
			return RebelAudioScript.script;
		},

		close: function () {
			return new Promise(function (resolve, reject) {
				RebelAudioScript.script.disconnect();
				RebelAudioScript.script.onaudioprocess = null;
				RebelAudioScript.script = null;
				resolve();
			});
		},
	},

	rebel_audio_script_create__sig: 'iii',
	rebel_audio_script_create: function (buffer_length, channel_count) {
		const buf_len = RebelRuntime.getHeapValue(buffer_length, 'i32');
		try {
			const out_len = RebelAudioScript.create(buf_len, channel_count);
			RebelRuntime.setHeapValue(buffer_length, out_len, 'i32');
		} catch (e) {
			RebelRuntime.error('Error starting AudioDriverScriptProcessor', e);
			return 1;
		}
		return 0;
	},

	rebel_audio_script_start__sig: 'viiiii',
	rebel_audio_script_start: function (p_in_buf, p_in_size, p_out_buf, p_out_size, p_cb) {
		const onprocess = RebelRuntime.get_func(p_cb);
		RebelAudioScript.start(p_in_buf, p_in_size, p_out_buf, p_out_size, onprocess);
	},
};

autoAddDeps(RebelAudioScript, '$RebelAudioScript');
mergeInto(LibraryManager.library, RebelAudioScript);
