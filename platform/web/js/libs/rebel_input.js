// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

/*
 * Gamepad API helper.
 */
const RebelInputGamepads = {
	$RebelInputGamepads__deps: ['$RebelRuntime', '$RebelEventListeners'],
	$RebelInputGamepads: {
		samples: [],

		get_pads: function () {
			try {
				// Will throw in iframe when permission is denied.
				// Will throw/warn in the future for insecure contexts.
				// See https://github.com/w3c/gamepad/pull/120
				const pads = navigator.getGamepads();
				if (pads) {
					return pads;
				}
				return [];
			} catch (e) {
				return [];
			}
		},

		get_samples: function () {
			return RebelInputGamepads.samples;
		},

		get_sample: function (index) {
			const samples = RebelInputGamepads.samples;
			return index < samples.length ? samples[index] : null;
		},

		sample: function () {
			const pads = RebelInputGamepads.get_pads();
			const samples = [];
			for (let i = 0; i < pads.length; i++) {
				const pad = pads[i];
				if (!pad) {
					samples.push(null);
					continue;
				}
				const s = {
					standard: pad.mapping === 'standard',
					buttons: [],
					axes: [],
					connected: pad.connected,
				};
				for (let b = 0; b < pad.buttons.length; b++) {
					s.buttons.push(pad.buttons[b].value);
				}
				for (let a = 0; a < pad.axes.length; a++) {
					s.axes.push(pad.axes[a]);
				}
				samples.push(s);
			}
			RebelInputGamepads.samples = samples;
		},

		init: function (onchange) {
			RebelInputGamepads.samples = [];
			function add(pad) {
				const guid = RebelInputGamepads.get_guid(pad);
				const c_id = RebelRuntime.allocString(pad.id);
				const c_guid = RebelRuntime.allocString(guid);
				onchange(pad.index, 1, c_id, c_guid);
				RebelRuntime.free(c_id);
				RebelRuntime.free(c_guid);
			}
			const pads = RebelInputGamepads.get_pads();
			for (let i = 0; i < pads.length; i++) {
				// Might be reserved space.
				if (pads[i]) {
					add(pads[i]);
				}
			}
			RebelEventListeners.add(window, 'gamepadconnected', function (evt) {
				if (evt.gamepad) {
					add(evt.gamepad);
				}
			}, false);
			RebelEventListeners.add(window, 'gamepaddisconnected', function (evt) {
				if (evt.gamepad) {
					onchange(evt.gamepad.index, 0);
				}
			}, false);
		},

		get_guid: function (pad) {
			if (pad.mapping) {
				return pad.mapping;
			}
			const ua = navigator.userAgent;
			let os = 'Unknown';
			if (ua.indexOf('Android') >= 0) {
				os = 'Android';
			} else if (ua.indexOf('Linux') >= 0) {
				os = 'Linux';
			} else if (ua.indexOf('iPhone') >= 0) {
				os = 'iOS';
			} else if (ua.indexOf('Macintosh') >= 0) {
				// Updated iPads will fall into this category.
				os = 'MacOS';
			} else if (ua.indexOf('Windows') >= 0) {
				os = 'Windows';
			}

			const id = pad.id;
			// Chrom* style: NAME (Vendor: xxxx Product: xxxx)
			const exp1 = /vendor: ([0-9a-f]{4}) product: ([0-9a-f]{4})/i;
			// Firefox/Safari style (safari may remove leading zeores)
			const exp2 = /^([0-9a-f]+)-([0-9a-f]+)-/i;
			let vendor = '';
			let product = '';
			if (exp1.test(id)) {
				const match = exp1.exec(id);
				vendor = match[1].padStart(4, '0');
				product = match[2].padStart(4, '0');
			} else if (exp2.test(id)) {
				const match = exp2.exec(id);
				vendor = match[1].padStart(4, '0');
				product = match[2].padStart(4, '0');
			}
			if (!vendor || !product) {
				return `${os}Unknown`;
			}
			return os + vendor + product;
		},
	},
};
mergeInto(LibraryManager.library, RebelInputGamepads);

/*
 * Drag and drop helper.
 * This is pretty big, but basically detect dropped files on RebelConfig.canvas,
 * process them one by one (recursively for directories), and copies them to
 * the temporary FS path '/tmp/drop-[random]/' so it can be emitted as a Rebel
 * event (that requires a string array of paths).
 *
 * NOTE: The temporary files are removed after the callback. This means that
 * deferred callbacks won't be able to access the files.
 */
const RebelInputDragDrop = {
	$RebelInputDragDrop__deps: ['$FS', '$RebelFS'],
	$RebelInputDragDrop: {
		promises: [],
		pending_files: [],

		add_entry: function (entry) {
			if (entry.isDirectory) {
				RebelInputDragDrop.add_dir(entry);
			} else if (entry.isFile) {
				RebelInputDragDrop.add_file(entry);
			} else {
				RebelRuntime.error('Unrecognized entry...', entry);
			}
		},

		add_dir: function (entry) {
			RebelInputDragDrop.promises.push(new Promise(function (resolve, reject) {
				const reader = entry.createReader();
				reader.readEntries(function (entries) {
					for (let i = 0; i < entries.length; i++) {
						RebelInputDragDrop.add_entry(entries[i]);
					}
					resolve();
				});
			}));
		},

		add_file: function (entry) {
			RebelInputDragDrop.promises.push(new Promise(function (resolve, reject) {
				entry.file(function (file) {
					const reader = new FileReader();
					reader.onload = function () {
						const f = {
							'path': file.relativePath || file.webkitRelativePath,
							'name': file.name,
							'type': file.type,
							'size': file.size,
							'data': reader.result,
						};
						if (!f['path']) {
							f['path'] = f['name'];
						}
						RebelInputDragDrop.pending_files.push(f);
						resolve();
					};
					reader.onerror = function () {
						RebelRuntime.print('Error reading file');
						reject();
					};
					reader.readAsArrayBuffer(file);
				}, function (err) {
					RebelRuntime.print('Error!');
					reject();
				});
			}));
		},

		process: function (resolve, reject) {
			if (RebelInputDragDrop.promises.length === 0) {
				resolve();
				return;
			}
			RebelInputDragDrop.promises.pop().then(function () {
				setTimeout(function () {
					RebelInputDragDrop.process(resolve, reject);
				}, 0);
			});
		},

		_process_event: function (ev, callback) {
			ev.preventDefault();
			if (ev.dataTransfer.items) {
				// Use DataTransferItemList interface to access the file(s)
				for (let i = 0; i < ev.dataTransfer.items.length; i++) {
					const item = ev.dataTransfer.items[i];
					let entry = null;
					if ('getAsEntry' in item) {
						entry = item.getAsEntry();
					} else if ('webkitGetAsEntry' in item) {
						entry = item.webkitGetAsEntry();
					}
					if (entry) {
						RebelInputDragDrop.add_entry(entry);
					}
				}
			} else {
				RebelRuntime.error('File upload not supported');
			}
			new Promise(RebelInputDragDrop.process).then(function () {
				const DROP = `/tmp/drop-${parseInt(Math.random() * (1 << 30), 10)}/`;
				const drops = [];
				const files = [];
				FS.mkdir(DROP.slice(0, -1)); // Without trailing slash
				RebelInputDragDrop.pending_files.forEach((elem) => {
					const path = elem['path'];
					RebelFS.copy_to_fs(DROP + path, elem['data']);
					let idx = path.indexOf('/');
					if (idx === -1) {
						// Root file
						drops.push(DROP + path);
					} else {
						// Subdir
						const sub = path.substr(0, idx);
						idx = sub.indexOf('/');
						if (idx < 0 && drops.indexOf(DROP + sub) === -1) {
							drops.push(DROP + sub);
						}
					}
					files.push(DROP + path);
				});
				RebelInputDragDrop.promises = [];
				RebelInputDragDrop.pending_files = [];
				callback(drops);
				if (RebelConfig.persistent_drops) {
					// Delay removal at exit.
					RebelOS.atexit(function (resolve, reject) {
						RebelInputDragDrop.remove_drop(files, DROP);
						resolve();
					});
				} else {
					RebelInputDragDrop.remove_drop(files, DROP);
				}
			});
		},

		remove_drop: function (files, drop_path) {
			const dirs = [drop_path.substr(0, drop_path.length - 1)];
			// Remove temporary files
			files.forEach(function (file) {
				FS.unlink(file);
				let dir = file.replace(drop_path, '');
				let idx = dir.lastIndexOf('/');
				while (idx > 0) {
					dir = dir.substr(0, idx);
					if (dirs.indexOf(drop_path + dir) === -1) {
						dirs.push(drop_path + dir);
					}
					idx = dir.lastIndexOf('/');
				}
			});
			// Remove dirs.
			dirs.sort(function (a, b) {
				const al = (a.match(/\//g) || []).length;
				const bl = (b.match(/\//g) || []).length;
				if (al > bl) {
					return -1;
				} else if (al < bl) {
					return 1;
				}
				return 0;
			}).forEach(function (dir) {
				FS.rmdir(dir);
			});
		},

		handler: function (callback) {
			return function (ev) {
				RebelInputDragDrop._process_event(ev, callback);
			};
		},
	},
};
mergeInto(LibraryManager.library, RebelInputDragDrop);

/*
 * Rebel exposed input functions.
 */
const RebelInput = {
	$RebelInput__deps: ['$RebelRuntime', '$RebelConfig', '$RebelEventListeners', '$RebelInputGamepads', '$RebelInputDragDrop'],
	$RebelInput: {
		getModifiers: function (evt) {
			return (evt.shiftKey + 0) + ((evt.altKey + 0) << 1) + ((evt.ctrlKey + 0) << 2) + ((evt.metaKey + 0) << 3);
		},
		computePosition: function (evt, rect) {
			const canvas = RebelConfig.canvas;
			const rw = canvas.width / rect.width;
			const rh = canvas.height / rect.height;
			const x = (evt.clientX - rect.x) * rw;
			const y = (evt.clientY - rect.y) * rh;
			return [x, y];
		},
	},

	/*
	 * Mouse API
	 */
	rebel_js_input_mouse_move_cb__sig: 'vi',
	rebel_js_input_mouse_move_cb: function (callback) {
		const func = RebelRuntime.get_func(callback);
		const canvas = RebelConfig.canvas;
		function move_cb(evt) {
			const rect = canvas.getBoundingClientRect();
			const pos = RebelInput.computePosition(evt, rect);
			// Scale movement
			const rw = canvas.width / rect.width;
			const rh = canvas.height / rect.height;
			const rel_pos_x = evt.movementX * rw;
			const rel_pos_y = evt.movementY * rh;
			const modifiers = RebelInput.getModifiers(evt);
			func(pos[0], pos[1], rel_pos_x, rel_pos_y, modifiers);
		}
		RebelEventListeners.add(window, 'mousemove', move_cb, false);
	},

	rebel_js_input_mouse_wheel_cb__sig: 'vi',
	rebel_js_input_mouse_wheel_cb: function (callback) {
		const func = RebelRuntime.get_func(callback);
		function wheel_cb(evt) {
			if (func(evt['deltaX'] || 0, evt['deltaY'] || 0)) {
				evt.preventDefault();
			}
		}
		RebelEventListeners.add(RebelConfig.canvas, 'wheel', wheel_cb, false);
	},

	rebel_js_input_mouse_button_cb__sig: 'vi',
	rebel_js_input_mouse_button_cb: function (callback) {
		const func = RebelRuntime.get_func(callback);
		const canvas = RebelConfig.canvas;
		function button_cb(p_pressed, evt) {
			const rect = canvas.getBoundingClientRect();
			const pos = RebelInput.computePosition(evt, rect);
			const modifiers = RebelInput.getModifiers(evt);
			// Since the event is consumed, focus manually.
			// NOTE: The iframe container may not have focus yet, so focus even when already active.
			if (p_pressed) {
				RebelConfig.canvas.focus();
			}
			if (func(p_pressed, evt.button, pos[0], pos[1], modifiers)) {
				evt.preventDefault();
			}
		}
		RebelEventListeners.add(canvas, 'mousedown', button_cb.bind(null, 1), false);
		RebelEventListeners.add(window, 'mouseup', button_cb.bind(null, 0), false);
	},

	/*
	 * Touch API
	 */
	rebel_js_input_touch_cb__sig: 'viii',
	rebel_js_input_touch_cb: function (callback, ids, coords) {
		const func = RebelRuntime.get_func(callback);
		const canvas = RebelConfig.canvas;
		function touch_cb(type, evt) {
			// Since the event is consumed, focus manually.
			// NOTE: The iframe container may not have focus yet, so focus even when already active.
			if (type === 0) {
				RebelConfig.canvas.focus();
			}
			const rect = canvas.getBoundingClientRect();
			const touches = evt.changedTouches;
			for (let i = 0; i < touches.length; i++) {
				const touch = touches[i];
				const pos = RebelInput.computePosition(touch, rect);
				RebelRuntime.setHeapValue(coords + (i * 2) * 8, pos[0], 'double');
				RebelRuntime.setHeapValue(coords + (i * 2 + 1) * 8, pos[1], 'double');
				RebelRuntime.setHeapValue(ids + i * 4, touch.identifier, 'i32');
			}
			func(type, touches.length);
			if (evt.cancelable) {
				evt.preventDefault();
			}
		}
		RebelEventListeners.add(canvas, 'touchstart', touch_cb.bind(null, 0), false);
		RebelEventListeners.add(canvas, 'touchend', touch_cb.bind(null, 1), false);
		RebelEventListeners.add(canvas, 'touchcancel', touch_cb.bind(null, 1), false);
		RebelEventListeners.add(canvas, 'touchmove', touch_cb.bind(null, 2), false);
	},

	/*
	 * Key API
	 */
	rebel_js_input_key_cb__sig: 'viii',
	rebel_js_input_key_cb: function (callback, code, key) {
		const func = RebelRuntime.get_func(callback);
		function key_cb(pressed, evt) {
			const modifiers = RebelInput.getModifiers(evt);
			RebelRuntime.stringToHeap(evt.code, code, 32);
			RebelRuntime.stringToHeap(evt.key, key, 32);
			func(pressed, evt.repeat, modifiers);
			evt.preventDefault();
		}
		RebelEventListeners.add(RebelConfig.canvas, 'keydown', key_cb.bind(null, 1), false);
		RebelEventListeners.add(RebelConfig.canvas, 'keyup', key_cb.bind(null, 0), false);
	},

	/*
	 * Gamepad API
	 */
	rebel_js_input_gamepad_cb__sig: 'vi',
	rebel_js_input_gamepad_cb: function (change_cb) {
		const onchange = RebelRuntime.get_func(change_cb);
		RebelInputGamepads.init(onchange);
	},

	rebel_js_input_gamepad_sample_count__sig: 'i',
	rebel_js_input_gamepad_sample_count: function () {
		return RebelInputGamepads.get_samples().length;
	},

	rebel_js_input_gamepad_sample__sig: 'i',
	rebel_js_input_gamepad_sample: function () {
		RebelInputGamepads.sample();
		return 0;
	},

	rebel_js_input_gamepad_sample_get__sig: 'iiiiiii',
	rebel_js_input_gamepad_sample_get: function (p_index, r_btns, r_btns_num, r_axes, r_axes_num, r_standard) {
		const sample = RebelInputGamepads.get_sample(p_index);
		if (!sample || !sample.connected) {
			return 1;
		}
		const btns = sample.buttons;
		const btns_len = btns.length < 16 ? btns.length : 16;
		for (let i = 0; i < btns_len; i++) {
			RebelRuntime.setHeapValue(r_btns + (i << 2), btns[i], 'float');
		}
		RebelRuntime.setHeapValue(r_btns_num, btns_len, 'i32');
		const axes = sample.axes;
		const axes_len = axes.length < 10 ? axes.length : 10;
		for (let i = 0; i < axes_len; i++) {
			RebelRuntime.setHeapValue(r_axes + (i << 2), axes[i], 'float');
		}
		RebelRuntime.setHeapValue(r_axes_num, axes_len, 'i32');
		const is_standard = sample.standard ? 1 : 0;
		RebelRuntime.setHeapValue(r_standard, is_standard, 'i32');
		return 0;
	},

	/*
	 * Drag/Drop API
	 */
	rebel_js_input_drop_files_cb__sig: 'vi',
	rebel_js_input_drop_files_cb: function (callback) {
		const func = RebelRuntime.get_func(callback);
		const dropFiles = function (files) {
			const args = files || [];
			if (!args.length) {
				return;
			}
			const argc = args.length;
			const argv = RebelRuntime.allocStringArray(args);
			func(argv, argc);
			RebelRuntime.freeStringArray(argv, argc);
		};
		const canvas = RebelConfig.canvas;
		RebelEventListeners.add(canvas, 'dragover', function (ev) {
			// Prevent default behavior (which would try to open the file(s))
			ev.preventDefault();
		}, false);
		RebelEventListeners.add(canvas, 'drop', RebelInputDragDrop.handler(dropFiles));
	},

	/* Paste API */
	rebel_js_input_paste_cb__sig: 'vi',
	rebel_js_input_paste_cb: function (callback) {
		const func = RebelRuntime.get_func(callback);
		RebelEventListeners.add(window, 'paste', function (evt) {
			const text = evt.clipboardData.getData('text');
			const ptr = RebelRuntime.allocString(text);
			func(ptr);
			RebelRuntime.free(ptr);
		}, false);
	},
};

autoAddDeps(RebelInput, '$RebelInput');
mergeInto(LibraryManager.library, RebelInput);
