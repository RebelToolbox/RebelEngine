// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

const IDHandler = {
	$IDHandler: {
		_last_id: 0,
		_references: {},

		get: function (p_id) {
			return IDHandler._references[p_id];
		},

		add: function (p_data) {
			const id = ++IDHandler._last_id;
			IDHandler._references[id] = p_data;
			return id;
		},

		remove: function (p_id) {
			delete IDHandler._references[p_id];
		},
	},
};

autoAddDeps(IDHandler, '$IDHandler');
mergeInto(LibraryManager.library, IDHandler);

const RebelConfig = {
	$RebelConfig__postset: 'Module["initConfig"] = RebelConfig.init_config;',
	$RebelConfig__deps: ['$RebelRuntime'],
	$RebelConfig: {
		canvas: null,
		locale: 'en',
		canvas_resize_policy: 2, // Adaptive
		virtual_keyboard: false,
		persistent_drops: false,
		on_execute: null,
		on_exit: null,

		init_config: function (p_opts) {
			RebelConfig.canvas_resize_policy = p_opts['canvasResizePolicy'];
			RebelConfig.canvas = p_opts['canvas'];
			RebelConfig.locale = p_opts['locale'] || RebelConfig.locale;
			RebelConfig.virtual_keyboard = p_opts['virtualKeyboard'];
			RebelConfig.persistent_drops = !!p_opts['persistentDrops'];
			RebelConfig.on_execute = p_opts['onExecute'];
			RebelConfig.on_exit = p_opts['onExit'];
			if (p_opts['focusCanvas']) {
				RebelConfig.canvas.focus();
			}
		},

		locate_file: function (file) {
			return Module['locateFile'](file); // eslint-disable-line no-undef
		},
		clear: function () {
			RebelConfig.canvas = null;
			RebelConfig.locale = 'en';
			RebelConfig.canvas_resize_policy = 2;
			RebelConfig.virtual_keyboard = false;
			RebelConfig.persistent_drops = false;
			RebelConfig.on_execute = null;
			RebelConfig.on_exit = null;
		},
	},

	rebel_js_config_canvas_id_get__sig: 'vii',
	rebel_js_config_canvas_id_get: function (p_ptr, p_ptr_max) {
		RebelRuntime.stringToHeap(`#${RebelConfig.canvas.id}`, p_ptr, p_ptr_max);
	},

	rebel_js_config_locale_get__sig: 'vii',
	rebel_js_config_locale_get: function (p_ptr, p_ptr_max) {
		RebelRuntime.stringToHeap(RebelConfig.locale, p_ptr, p_ptr_max);
	},
};

autoAddDeps(RebelConfig, '$RebelConfig');
mergeInto(LibraryManager.library, RebelConfig);

const RebelFS = {
	$RebelFS__deps: ['$ERRNO_CODES', '$FS', '$IDBFS', '$RebelRuntime'],
	$RebelFS__postset: [
		'Module["initFS"] = RebelFS.init;',
		'Module["copyToFS"] = RebelFS.copy_to_fs;',
	].join(''),
	$RebelFS: {
		_idbfs: false,
		_syncing: false,
		_mount_points: [],

		is_persistent: function () {
			return RebelFS._idbfs ? 1 : 0;
		},

		// Initialize Rebel file system, setting up persistent paths.
		// Returns a promise that resolves when the FS is ready.
		// We keep track of mount_points, so that we can properly close the IDBFS
		// since emscripten is not doing it by itself. (emscripten GH#12516).
		init: function (persistentPaths) {
			RebelFS._idbfs = false;
			if (!Array.isArray(persistentPaths)) {
				return Promise.reject(new Error('Persistent paths must be an array'));
			}
			if (!persistentPaths.length) {
				return Promise.resolve();
			}
			RebelFS._mount_points = persistentPaths.slice();

			function createRecursive(dir) {
				try {
					FS.stat(dir);
				} catch (e) {
					if (e.errno !== ERRNO_CODES.ENOENT) {
						throw e;
					}
					FS.mkdirTree(dir);
				}
			}

			RebelFS._mount_points.forEach(function (path) {
				createRecursive(path);
				FS.mount(IDBFS, {}, path);
			});
			return new Promise(function (resolve, reject) {
				FS.syncfs(true, function (err) {
					if (err) {
						RebelFS._mount_points = [];
						RebelFS._idbfs = false;
						RebelRuntime.print(`IndexedDB not available: ${err.message}`);
					} else {
						RebelFS._idbfs = true;
					}
					resolve(err);
				});
			});
		},

		// Deinit Rebel file system, making sure to unmount file systems, and close IDBFS(s).
		deinit: function () {
			RebelFS._mount_points.forEach(function (path) {
				try {
					FS.unmount(path);
				} catch (e) {
					RebelRuntime.print('Already unmounted', e);
				}
				if (RebelFS._idbfs && IDBFS.dbs[path]) {
					IDBFS.dbs[path].close();
					delete IDBFS.dbs[path];
				}
			});
			RebelFS._mount_points = [];
			RebelFS._idbfs = false;
			RebelFS._syncing = false;
		},

		sync: function () {
			if (RebelFS._syncing) {
				RebelRuntime.error('Already syncing!');
				return Promise.resolve();
			}
			RebelFS._syncing = true;
			return new Promise(function (resolve, reject) {
				FS.syncfs(false, function (error) {
					if (error) {
						RebelRuntime.error(`Failed to save IDB file system: ${error.message}`);
					}
					RebelFS._syncing = false;
					resolve(error);
				});
			});
		},

		// Copies a buffer to the internal file system. Creating directories recursively.
		copy_to_fs: function (path, buffer) {
			const idx = path.lastIndexOf('/');
			let dir = '/';
			if (idx > 0) {
				dir = path.slice(0, idx);
			}
			try {
				FS.stat(dir);
			} catch (e) {
				if (e.errno !== ERRNO_CODES.ENOENT) {
					throw e;
				}
				FS.mkdirTree(dir);
			}
			FS.writeFile(path, new Uint8Array(buffer));
		},
	},
};
mergeInto(LibraryManager.library, RebelFS);

const RebelOS = {
	$RebelOS__deps: ['$RebelRuntime', '$RebelConfig', '$RebelFS'],
	$RebelOS__postset: [
		'Module["request_quit"] = function() { RebelOS.request_quit() };',
		'Module["onExit"] = RebelOS.cleanup;',
		'RebelOS._fs_sync_promise = Promise.resolve();',
	].join(''),
	$RebelOS: {
		request_quit: function () {},
		_async_cbs: [],
		_fs_sync_promise: null,

		atexit: function (p_promise_cb) {
			RebelOS._async_cbs.push(p_promise_cb);
		},

		cleanup: function (exit_code) {
			const cb = RebelConfig.on_exit;
			RebelFS.deinit();
			RebelConfig.clear();
			if (cb) {
				cb(exit_code);
			}
		},

		finish_async: function (callback) {
			RebelOS._fs_sync_promise.then(function (err) {
				const promises = [];
				RebelOS._async_cbs.forEach(function (cb) {
					promises.push(new Promise(cb));
				});
				return Promise.all(promises);
			}).then(function () {
				return RebelFS.sync(); // Final FS sync.
			}).then(function (err) {
				// Always deferred.
				setTimeout(function () {
					callback();
				}, 0);
			});
		},
	},

	rebel_js_os_finish_async__sig: 'vi',
	rebel_js_os_finish_async: function (p_callback) {
		const func = RebelRuntime.get_func(p_callback);
		RebelOS.finish_async(func);
	},

	rebel_js_os_request_quit_cb__sig: 'vi',
	rebel_js_os_request_quit_cb: function (p_callback) {
		RebelOS.request_quit = RebelRuntime.get_func(p_callback);
	},

	rebel_js_os_fs_is_persistent__sig: 'i',
	rebel_js_os_fs_is_persistent: function () {
		return RebelFS.is_persistent();
	},

	rebel_js_os_fs_sync__sig: 'vi',
	rebel_js_os_fs_sync: function (callback) {
		const func = RebelRuntime.get_func(callback);
		RebelOS._fs_sync_promise = RebelFS.sync();
		RebelOS._fs_sync_promise.then(function (err) {
			func();
		});
	},

	rebel_js_os_execute__sig: 'ii',
	rebel_js_os_execute: function (p_json) {
		const json_args = RebelRuntime.parseString(p_json);
		const args = JSON.parse(json_args);
		if (RebelConfig.on_execute) {
			RebelConfig.on_execute(args);
			return 0;
		}
		return 1;
	},

	rebel_js_os_shell_open__sig: 'vi',
	rebel_js_os_shell_open: function (p_uri) {
		window.open(RebelRuntime.parseString(p_uri), '_blank');
	},

	rebel_js_os_hw_concurrency_get__sig: 'i',
	rebel_js_os_hw_concurrency_get: function () {
		return navigator.hardwareConcurrency || 1;
	},

	rebel_js_os_download_buffer__sig: 'viiii',
	rebel_js_os_download_buffer: function (p_ptr, p_size, p_name, p_mime) {
		const buf = RebelRuntime.heapSlice(HEAP8, p_ptr, p_size);
		const name = RebelRuntime.parseString(p_name);
		const mime = RebelRuntime.parseString(p_mime);
		const blob = new Blob([buf], { type: mime });
		const url = window.URL.createObjectURL(blob);
		const a = document.createElement('a');
		a.href = url;
		a.download = name;
		a.style.display = 'none';
		document.body.appendChild(a);
		a.click();
		a.remove();
		window.URL.revokeObjectURL(url);
	},
};

autoAddDeps(RebelOS, '$RebelOS');
mergeInto(LibraryManager.library, RebelOS);

/*
 * Rebel event listeners.
 * Keeps track of registered event listeners so it can remove them on shutdown.
 */
const RebelEventListeners = {
	$RebelEventListeners__deps: ['$RebelOS'],
	$RebelEventListeners__postset: 'RebelOS.atexit(function(resolve, reject) { RebelEventListeners.clear(); resolve(); });',
	$RebelEventListeners: {
		handlers: [],

		has: function (target, event, method, capture) {
			return RebelEventListeners.handlers.findIndex(function (e) {
				return e.target === target && e.event === event && e.method === method && e.capture === capture;
			}) !== -1;
		},

		add: function (target, event, method, capture) {
			if (RebelEventListeners.has(target, event, method, capture)) {
				return;
			}
			function Handler(p_target, p_event, p_method, p_capture) {
				this.target = p_target;
				this.event = p_event;
				this.method = p_method;
				this.capture = p_capture;
			}
			RebelEventListeners.handlers.push(new Handler(target, event, method, capture));
			target.addEventListener(event, method, capture);
		},

		clear: function () {
			RebelEventListeners.handlers.forEach(function (h) {
				h.target.removeEventListener(h.event, h.method, h.capture);
			});
			RebelEventListeners.handlers.length = 0;
		},
	},
};
mergeInto(LibraryManager.library, RebelEventListeners);
