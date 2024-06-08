// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

const RebelDisplayVK = {

	$RebelDisplayVK__deps: ['$RebelRuntime', '$RebelConfig', '$RebelEventListeners'],
	$RebelDisplayVK__postset: 'RebelOS.atexit(function(resolve, reject) { RebelDisplayVK.clear(); resolve(); });',
	$RebelDisplayVK: {
		textinput: null,
		textarea: null,

		available: function () {
			return RebelConfig.virtual_keyboard && 'ontouchstart' in window;
		},

		init: function (input_cb) {
			function create(what) {
				const elem = document.createElement(what);
				elem.style.display = 'none';
				elem.style.position = 'absolute';
				elem.style.zIndex = '-1';
				elem.style.background = 'transparent';
				elem.style.padding = '0px';
				elem.style.margin = '0px';
				elem.style.overflow = 'hidden';
				elem.style.width = '0px';
				elem.style.height = '0px';
				elem.style.border = '0px';
				elem.style.outline = 'none';
				elem.readonly = true;
				elem.disabled = true;
				RebelEventListeners.add(elem, 'input', function (evt) {
					const c_str = RebelRuntime.allocString(elem.value);
					input_cb(c_str, elem.selectionEnd);
					RebelRuntime.free(c_str);
				}, false);
				RebelEventListeners.add(elem, 'blur', function (evt) {
					elem.style.display = 'none';
					elem.readonly = true;
					elem.disabled = true;
				}, false);
				RebelConfig.canvas.insertAdjacentElement('beforebegin', elem);
				return elem;
			}
			RebelDisplayVK.textinput = create('input');
			RebelDisplayVK.textarea = create('textarea');
			RebelDisplayVK.updateSize();
		},
		show: function (text, multiline, start, end) {
			if (!RebelDisplayVK.textinput || !RebelDisplayVK.textarea) {
				return;
			}
			if (RebelDisplayVK.textinput.style.display !== '' || RebelDisplayVK.textarea.style.display !== '') {
				RebelDisplayVK.hide();
			}
			RebelDisplayVK.updateSize();
			const elem = multiline ? RebelDisplayVK.textarea : RebelDisplayVK.textinput;
			elem.readonly = false;
			elem.disabled = false;
			elem.value = text;
			elem.style.display = 'block';
			elem.focus();
			elem.setSelectionRange(start, end);
		},
		hide: function () {
			if (!RebelDisplayVK.textinput || !RebelDisplayVK.textarea) {
				return;
			}
			[RebelDisplayVK.textinput, RebelDisplayVK.textarea].forEach(function (elem) {
				elem.blur();
				elem.style.display = 'none';
				elem.value = '';
			});
		},
		updateSize: function () {
			if (!RebelDisplayVK.textinput || !RebelDisplayVK.textarea) {
				return;
			}
			const rect = RebelConfig.canvas.getBoundingClientRect();
			function update(elem) {
				elem.style.left = `${rect.left}px`;
				elem.style.top = `${rect.top}px`;
				elem.style.width = `${rect.width}px`;
				elem.style.height = `${rect.height}px`;
			}
			update(RebelDisplayVK.textinput);
			update(RebelDisplayVK.textarea);
		},
		clear: function () {
			if (RebelDisplayVK.textinput) {
				RebelDisplayVK.textinput.remove();
				RebelDisplayVK.textinput = null;
			}
			if (RebelDisplayVK.textarea) {
				RebelDisplayVK.textarea.remove();
				RebelDisplayVK.textarea = null;
			}
		},
	},
};
mergeInto(LibraryManager.library, RebelDisplayVK);

/*
 * Display server cursor helper.
 * Keeps track of cursor status and custom shapes.
 */
const RebelDisplayCursor = {
	$RebelDisplayCursor__deps: ['$RebelOS', '$RebelConfig'],
	$RebelDisplayCursor__postset: 'RebelOS.atexit(function(resolve, reject) { RebelDisplayCursor.clear(); resolve(); });',
	$RebelDisplayCursor: {
		shape: 'auto',
		visible: true,
		cursors: {},
		set_style: function (style) {
			RebelConfig.canvas.style.cursor = style;
		},
		set_shape: function (shape) {
			RebelDisplayCursor.shape = shape;
			let css = shape;
			if (shape in RebelDisplayCursor.cursors) {
				const c = RebelDisplayCursor.cursors[shape];
				css = `url("${c.url}") ${c.x} ${c.y}, auto`;
			}
			if (RebelDisplayCursor.visible) {
				RebelDisplayCursor.set_style(css);
			}
		},
		clear: function () {
			RebelDisplayCursor.set_style('');
			RebelDisplayCursor.shape = 'auto';
			RebelDisplayCursor.visible = true;
			Object.keys(RebelDisplayCursor.cursors).forEach(function (key) {
				URL.revokeObjectURL(RebelDisplayCursor.cursors[key]);
				delete RebelDisplayCursor.cursors[key];
			});
		},
		lockPointer: function () {
			const canvas = RebelConfig.canvas;
			if (canvas.requestPointerLock) {
				canvas.requestPointerLock();
			}
		},
		releasePointer: function () {
			if (document.exitPointerLock) {
				document.exitPointerLock();
			}
		},
		isPointerLocked: function () {
			return document.pointerLockElement === RebelConfig.canvas;
		},
	},
};
mergeInto(LibraryManager.library, RebelDisplayCursor);

const RebelDisplayScreen = {
	$RebelDisplayScreen__deps: ['$RebelConfig', '$RebelOS', '$GL', 'emscripten_webgl_get_current_context'],
	$RebelDisplayScreen: {
		desired_size: [0, 0],
		hidpi: true,
		getPixelRatio: function () {
			return RebelDisplayScreen.hidpi ? window.devicePixelRatio || 1 : 1;
		},
		isFullscreen: function () {
			const elem = document.fullscreenElement || document.mozFullscreenElement
				|| document.webkitFullscreenElement || document.msFullscreenElement;
			if (elem) {
				return elem === RebelConfig.canvas;
			}
			// But maybe knowing the element is not supported.
			return document.fullscreen || document.mozFullScreen
				|| document.webkitIsFullscreen;
		},
		hasFullscreen: function () {
			return document.fullscreenEnabled || document.mozFullScreenEnabled
				|| document.webkitFullscreenEnabled;
		},
		requestFullscreen: function () {
			if (!RebelDisplayScreen.hasFullscreen()) {
				return 1;
			}
			const canvas = RebelConfig.canvas;
			try {
				const promise = (canvas.requestFullscreen || canvas.msRequestFullscreen
					|| canvas.mozRequestFullScreen || canvas.mozRequestFullscreen
					|| canvas.webkitRequestFullscreen
				).call(canvas);
				// Some browsers (Safari) return undefined.
				// For the standard ones, we need to catch it.
				if (promise) {
					promise.catch(function () {
						// nothing to do.
					});
				}
			} catch (e) {
				return 1;
			}
			return 0;
		},
		exitFullscreen: function () {
			if (!RebelDisplayScreen.isFullscreen()) {
				return 0;
			}
			try {
				const promise = document.exitFullscreen();
				if (promise) {
					promise.catch(function () {
						// nothing to do.
					});
				}
			} catch (e) {
				return 1;
			}
			return 0;
		},
		_updateGL: function () {
			const gl_context_handle = _emscripten_webgl_get_current_context(); // eslint-disable-line no-undef
			const gl = GL.getContext(gl_context_handle);
			if (gl) {
				GL.resizeOffscreenFramebuffer(gl);
			}
		},
		updateSize: function () {
			const isFullscreen = RebelDisplayScreen.isFullscreen();
			const wantsFullWindow = RebelConfig.canvas_resize_policy === 2;
			const noResize = RebelConfig.canvas_resize_policy === 0;
			const wwidth = RebelDisplayScreen.desired_size[0];
			const wheight = RebelDisplayScreen.desired_size[1];
			const canvas = RebelConfig.canvas;
			let width = wwidth;
			let height = wheight;
			if (noResize) {
				// Don't resize canvas, just update GL if needed.
				if (canvas.width !== width || canvas.height !== height) {
					RebelDisplayScreen.desired_size = [canvas.width, canvas.height];
					RebelDisplayScreen._updateGL();
					return 1;
				}
				return 0;
			}
			const scale = RebelDisplayScreen.getPixelRatio();
			if (isFullscreen || wantsFullWindow) {
				// We need to match screen size.
				width = window.innerWidth * scale;
				height = window.innerHeight * scale;
			}
			const csw = `${width / scale}px`;
			const csh = `${height / scale}px`;
			if (canvas.style.width !== csw || canvas.style.height !== csh || canvas.width !== width || canvas.height !== height) {
				// Size doesn't match.
				// Resize canvas, set correct CSS pixel size, update GL.
				canvas.width = width;
				canvas.height = height;
				canvas.style.width = csw;
				canvas.style.height = csh;
				RebelDisplayScreen._updateGL();
				return 1;
			}
			return 0;
		},
	},
};
mergeInto(LibraryManager.library, RebelDisplayScreen);

/**
 * Display server interface.
 *
 * Exposes all the functions needed by DisplayServer implementation.
 */
const RebelDisplay = {
	$RebelDisplay__deps: ['$RebelConfig', '$RebelRuntime', '$RebelDisplayCursor', '$RebelEventListeners', '$RebelDisplayScreen', '$RebelDisplayVK'],
	$RebelDisplay: {
		window_icon: '',
		findDPI: function () {
			function testDPI(dpi) {
				return window.matchMedia(`(max-resolution: ${dpi}dpi)`).matches;
			}
			function bisect(low, high, func) {
				const mid = parseInt(((high - low) / 2) + low, 10);
				if (high - low <= 1) {
					return func(high) ? high : low;
				}
				if (func(mid)) {
					return bisect(low, mid, func);
				}
				return bisect(mid, high, func);
			}
			try {
				const dpi = bisect(0, 800, testDPI);
				return dpi >= 96 ? dpi : 96;
			} catch (e) {
				return 96;
			}
		},
	},

	// This is implemented as "glGetBufferSubData" in new emscripten versions.
	// Since we have to support older (pre 2.0.17) emscripten versions, we add this wrapper function instead.
	rebel_js_display_glGetBufferSubData__sig: 'viiii',
	rebel_js_display_glGetBufferSubData__deps: ['$GL', 'emscripten_webgl_get_current_context'],
	rebel_js_display_glGetBufferSubData: function (target, offset, size, data) {
		const gl_context_handle = _emscripten_webgl_get_current_context(); // eslint-disable-line no-undef
		const gl = GL.getContext(gl_context_handle);
		if (gl) {
			gl.GLctx['getBufferSubData'](target, offset, HEAPU8, data, size);
		}
	},

	rebel_js_display_is_swap_ok_cancel__sig: 'i',
	rebel_js_display_is_swap_ok_cancel: function () {
		const win = (['Windows', 'Win64', 'Win32', 'WinCE']);
		const plat = navigator.platform || '';
		if (win.indexOf(plat) !== -1) {
			return 1;
		}
		return 0;
	},

	rebel_js_display_alert__sig: 'vi',
	rebel_js_display_alert: function (p_text) {
		window.alert(RebelRuntime.parseString(p_text)); // eslint-disable-line no-alert
	},

	rebel_js_display_screen_dpi_get__sig: 'i',
	rebel_js_display_screen_dpi_get: function () {
		return RebelDisplay.findDPI();
	},

	rebel_js_display_pixel_ratio_get__sig: 'f',
	rebel_js_display_pixel_ratio_get: function () {
		return RebelDisplayScreen.getPixelRatio();
	},

	rebel_js_display_fullscreen_request__sig: 'i',
	rebel_js_display_fullscreen_request: function () {
		return RebelDisplayScreen.requestFullscreen();
	},

	rebel_js_display_fullscreen_exit__sig: 'i',
	rebel_js_display_fullscreen_exit: function () {
		return RebelDisplayScreen.exitFullscreen();
	},

	rebel_js_display_desired_size_set__sig: 'vii',
	rebel_js_display_desired_size_set: function (width, height) {
		RebelDisplayScreen.desired_size = [width, height];
		RebelDisplayScreen.updateSize();
	},

	rebel_js_display_size_update__sig: 'i',
	rebel_js_display_size_update: function () {
		const updated = RebelDisplayScreen.updateSize();
		if (updated) {
			RebelDisplayVK.updateSize();
		}
		return updated;
	},

	rebel_js_display_screen_size_get__sig: 'vii',
	rebel_js_display_screen_size_get: function (width, height) {
		const scale = RebelDisplayScreen.getPixelRatio();
		RebelRuntime.setHeapValue(width, window.screen.width * scale, 'i32');
		RebelRuntime.setHeapValue(height, window.screen.height * scale, 'i32');
	},

	rebel_js_display_window_size_get__sig: 'vii',
	rebel_js_display_window_size_get: function (p_width, p_height) {
		RebelRuntime.setHeapValue(p_width, RebelConfig.canvas.width, 'i32');
		RebelRuntime.setHeapValue(p_height, RebelConfig.canvas.height, 'i32');
	},

	rebel_js_display_has_webgl__sig: 'ii',
	rebel_js_display_has_webgl: function (p_version) {
		if (p_version !== 1 && p_version !== 2) {
			return false;
		}
		try {
			return !!document.createElement('canvas').getContext(p_version === 2 ? 'webgl2' : 'webgl');
		} catch (e) { /* Not available */ }
		return false;
	},

	/*
	 * Canvas
	 */
	rebel_js_display_canvas_focus__sig: 'v',
	rebel_js_display_canvas_focus: function () {
		RebelConfig.canvas.focus();
	},

	rebel_js_display_canvas_is_focused__sig: 'i',
	rebel_js_display_canvas_is_focused: function () {
		return document.activeElement === RebelConfig.canvas;
	},

	/*
	 * Touchscreen
	 */
	rebel_js_display_touchscreen_is_available__sig: 'i',
	rebel_js_display_touchscreen_is_available: function () {
		return 'ontouchstart' in window;
	},

	/*
	 * Clipboard
	 */
	rebel_js_display_clipboard_set__sig: 'ii',
	rebel_js_display_clipboard_set: function (p_text) {
		const text = RebelRuntime.parseString(p_text);
		if (!navigator.clipboard || !navigator.clipboard.writeText) {
			return 1;
		}
		navigator.clipboard.writeText(text).catch(function (e) {
			// Setting OS clipboard is only possible from an input callback.
			RebelRuntime.error('Setting OS clipboard is only possible from an input callback for the Web plafrom. Exception:', e);
		});
		return 0;
	},

	rebel_js_display_clipboard_get__sig: 'ii',
	rebel_js_display_clipboard_get: function (callback) {
		const func = RebelRuntime.get_func(callback);
		try {
			navigator.clipboard.readText().then(function (result) {
				const ptr = RebelRuntime.allocString(result);
				func(ptr);
				RebelRuntime.free(ptr);
			}).catch(function (e) {
				// Fail graciously.
			});
		} catch (e) {
			// Fail graciously.
		}
	},

	/*
	 * Window
	 */
	rebel_js_display_window_title_set__sig: 'vi',
	rebel_js_display_window_title_set: function (p_data) {
		document.title = RebelRuntime.parseString(p_data);
	},

	rebel_js_display_window_icon_set__sig: 'vii',
	rebel_js_display_window_icon_set: function (p_ptr, p_len) {
		let link = document.getElementById('-gd-engine-icon');
		if (link === null) {
			link = document.createElement('link');
			link.rel = 'icon';
			link.id = '-gd-engine-icon';
			document.head.appendChild(link);
		}
		const old_icon = RebelDisplay.window_icon;
		const png = new Blob([RebelRuntime.heapSlice(HEAPU8, p_ptr, p_len)], { type: 'image/png' });
		RebelDisplay.window_icon = URL.createObjectURL(png);
		link.href = RebelDisplay.window_icon;
		if (old_icon) {
			URL.revokeObjectURL(old_icon);
		}
	},

	/*
	 * Cursor
	 */
	rebel_js_display_cursor_set_visible__sig: 'vi',
	rebel_js_display_cursor_set_visible: function (p_visible) {
		const visible = p_visible !== 0;
		if (visible === RebelDisplayCursor.visible) {
			return;
		}
		RebelDisplayCursor.visible = visible;
		if (visible) {
			RebelDisplayCursor.set_shape(RebelDisplayCursor.shape);
		} else {
			RebelDisplayCursor.set_style('none');
		}
	},

	rebel_js_display_cursor_is_hidden__sig: 'i',
	rebel_js_display_cursor_is_hidden: function () {
		return !RebelDisplayCursor.visible;
	},

	rebel_js_display_cursor_set_shape__sig: 'vi',
	rebel_js_display_cursor_set_shape: function (p_string) {
		RebelDisplayCursor.set_shape(RebelRuntime.parseString(p_string));
	},

	rebel_js_display_cursor_set_custom_shape__sig: 'viiiii',
	rebel_js_display_cursor_set_custom_shape: function (p_shape, p_ptr, p_len, p_hotspot_x, p_hotspot_y) {
		const shape = RebelRuntime.parseString(p_shape);
		const old_shape = RebelDisplayCursor.cursors[shape];
		if (p_len > 0) {
			const png = new Blob([RebelRuntime.heapSlice(HEAPU8, p_ptr, p_len)], { type: 'image/png' });
			const url = URL.createObjectURL(png);
			RebelDisplayCursor.cursors[shape] = {
				url: url,
				x: p_hotspot_x,
				y: p_hotspot_y,
			};
		} else {
			delete RebelDisplayCursor.cursors[shape];
		}
		if (shape === RebelDisplayCursor.shape) {
			RebelDisplayCursor.set_shape(RebelDisplayCursor.shape);
		}
		if (old_shape) {
			URL.revokeObjectURL(old_shape.url);
		}
	},

	rebel_js_display_cursor_lock_set__sig: 'vi',
	rebel_js_display_cursor_lock_set: function (p_lock) {
		if (p_lock) {
			RebelDisplayCursor.lockPointer();
		} else {
			RebelDisplayCursor.releasePointer();
		}
	},

	rebel_js_display_cursor_is_locked__sig: 'i',
	rebel_js_display_cursor_is_locked: function () {
		return RebelDisplayCursor.isPointerLocked() ? 1 : 0;
	},

	/*
	 * Listeners
	 */
	rebel_js_display_fullscreen_cb__sig: 'vi',
	rebel_js_display_fullscreen_cb: function (callback) {
		const canvas = RebelConfig.canvas;
		const func = RebelRuntime.get_func(callback);
		function change_cb(evt) {
			if (evt.target === canvas) {
				func(RebelDisplayScreen.isFullscreen());
			}
		}
		RebelEventListeners.add(document, 'fullscreenchange', change_cb, false);
		RebelEventListeners.add(document, 'mozfullscreenchange', change_cb, false);
		RebelEventListeners.add(document, 'webkitfullscreenchange', change_cb, false);
	},

	rebel_js_display_window_blur_cb__sig: 'vi',
	rebel_js_display_window_blur_cb: function (callback) {
		const func = RebelRuntime.get_func(callback);
		RebelEventListeners.add(window, 'blur', function () {
			func();
		}, false);
	},

	rebel_js_display_notification_cb__sig: 'viiiii',
	rebel_js_display_notification_cb: function (callback, p_enter, p_exit, p_in, p_out) {
		const canvas = RebelConfig.canvas;
		const func = RebelRuntime.get_func(callback);
		const notif = [p_enter, p_exit, p_in, p_out];
		['mouseover', 'mouseleave', 'focus', 'blur'].forEach(function (evt_name, idx) {
			RebelEventListeners.add(canvas, evt_name, function () {
				func(notif[idx]);
			}, true);
		});
	},

	rebel_js_display_setup_canvas__sig: 'viiii',
	rebel_js_display_setup_canvas: function (p_width, p_height, p_fullscreen, p_hidpi) {
		const canvas = RebelConfig.canvas;
		RebelEventListeners.add(canvas, 'contextmenu', function (ev) {
			ev.preventDefault();
		}, false);
		RebelEventListeners.add(canvas, 'webglcontextlost', function (ev) {
			alert('WebGL context lost, please reload the page'); // eslint-disable-line no-alert
			ev.preventDefault();
		}, false);
		RebelDisplayScreen.hidpi = !!p_hidpi;
		switch (RebelConfig.canvas_resize_policy) {
		case 0: // None
			RebelDisplayScreen.desired_size = [canvas.width, canvas.height];
			break;
		case 1: // Project
			RebelDisplayScreen.desired_size = [p_width, p_height];
			break;
		default: // Full window
			// Ensure we display in the right place, the size will be handled by updateSize
			canvas.style.position = 'absolute';
			canvas.style.top = 0;
			canvas.style.left = 0;
			break;
		}
		RebelDisplayScreen.updateSize();
		if (p_fullscreen) {
			RebelDisplayScreen.requestFullscreen();
		}
	},

	/*
	 * Virtual Keyboard
	 */
	rebel_js_display_vk_show__sig: 'viiii',
	rebel_js_display_vk_show: function (p_text, p_multiline, p_start, p_end) {
		const text = RebelRuntime.parseString(p_text);
		const start = p_start > 0 ? p_start : 0;
		const end = p_end > 0 ? p_end : start;
		RebelDisplayVK.show(text, p_multiline, start, end);
	},

	rebel_js_display_vk_hide__sig: 'v',
	rebel_js_display_vk_hide: function () {
		RebelDisplayVK.hide();
	},

	rebel_js_display_vk_available__sig: 'i',
	rebel_js_display_vk_available: function () {
		return RebelDisplayVK.available();
	},

	rebel_js_display_vk_cb__sig: 'vi',
	rebel_js_display_vk_cb: function (p_input_cb) {
		const input_cb = RebelRuntime.get_func(p_input_cb);
		if (RebelDisplayVK.available()) {
			RebelDisplayVK.init(input_cb);
		}
	},
};

autoAddDeps(RebelDisplay, '$RebelDisplay');
mergeInto(LibraryManager.library, RebelDisplay);
