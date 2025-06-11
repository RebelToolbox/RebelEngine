// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

const RebelJSWrapper = {

	$RebelJSWrapper__deps: ['$RebelRuntime', '$IDHandler'],
	$RebelJSWrapper__postset: 'RebelJSWrapper.proxies = new Map();',
	$RebelJSWrapper: {
		proxies: null,

		MyProxy: function (val) {
			const id = IDHandler.add(this);
			RebelJSWrapper.proxies.set(val, id);
			let refs = 1;
			this.ref = function () {
				refs++;
			};
			this.unref = function () {
				refs--;
				if (refs === 0) {
					IDHandler.remove(id);
					RebelJSWrapper.proxies.delete(val);
				}
			};
			this.get_val = function () {
				return val;
			};
			this.get_id = function () {
				return id;
			};
		},

		get_proxied: function (val) {
			const id = RebelJSWrapper.proxies.get(val);
			if (id === undefined) {
				const proxy = new RebelJSWrapper.MyProxy(val);
				return proxy.get_id();
			}
			IDHandler.get(id).ref();
			return id;
		},

		get_proxied_value: function (id) {
			const proxy = IDHandler.get(id);
			if (proxy === undefined) {
				return undefined;
			}
			return proxy.get_val();
		},

		variant2js: function (type, val) {
			switch (type) {
			case 0:
				return null;
			case 1:
				return !!RebelRuntime.getHeapValue(val, 'i64');
			case 2:
				return RebelRuntime.getHeapValue(val, 'i64');
			case 3:
				return RebelRuntime.getHeapValue(val, 'double');
			case 4:
				return RebelRuntime.parseString(RebelRuntime.getHeapValue(val, '*'));
			case 17: // OBJECT
				return RebelJSWrapper.get_proxied_value(RebelRuntime.getHeapValue(val, 'i64'));
			default:
				return undefined;
			}
		},

		js2variant: function (p_val, p_exchange) {
			if (p_val === undefined || p_val === null) {
				return 0; // NIL
			}
			const type = typeof (p_val);
			if (type === 'boolean') {
				RebelRuntime.setHeapValue(p_exchange, p_val, 'i64');
				return 1; // BOOL
			} else if (type === 'number') {
				if (Number.isInteger(p_val)) {
					RebelRuntime.setHeapValue(p_exchange, p_val, 'i64');
					return 2; // INT
				}
				RebelRuntime.setHeapValue(p_exchange, p_val, 'double');
				return 3; // REAL
			} else if (type === 'string') {
				const c_str = RebelRuntime.allocString(p_val);
				RebelRuntime.setHeapValue(p_exchange, c_str, '*');
				return 4; // STRING
			}
			const id = RebelJSWrapper.get_proxied(p_val);
			RebelRuntime.setHeapValue(p_exchange, id, 'i64');
			return 17;
		},
	},

	rebel_js_wrapper_interface_get__sig: 'ii',
	rebel_js_wrapper_interface_get: function (p_name) {
		const name = RebelRuntime.parseString(p_name);
		if (typeof (window[name]) !== 'undefined') {
			return RebelJSWrapper.get_proxied(window[name]);
		}
		return 0;
	},

	rebel_js_wrapper_object_get__sig: 'iiii',
	rebel_js_wrapper_object_get: function (p_id, p_exchange, p_prop) {
		const obj = RebelJSWrapper.get_proxied_value(p_id);
		if (obj === undefined) {
			return 0;
		}
		if (p_prop) {
			const prop = RebelRuntime.parseString(p_prop);
			try {
				return RebelJSWrapper.js2variant(obj[prop], p_exchange);
			} catch (e) {
				RebelRuntime.error(`Error getting variable ${prop} on object`, obj);
				return 0; // NIL
			}
		}
		return RebelJSWrapper.js2variant(obj, p_exchange);
	},

	rebel_js_wrapper_object_set__sig: 'viiii',
	rebel_js_wrapper_object_set: function (p_id, p_name, p_type, p_exchange) {
		const obj = RebelJSWrapper.get_proxied_value(p_id);
		if (obj === undefined) {
			return;
		}
		const name = RebelRuntime.parseString(p_name);
		try {
			obj[name] = RebelJSWrapper.variant2js(p_type, p_exchange);
		} catch (e) {
			RebelRuntime.error(`Error setting variable ${name} on object`, obj);
		}
	},

	rebel_js_wrapper_object_call__sig: 'iiiiiiiii',
	rebel_js_wrapper_object_call: function (p_id, p_method, p_args, p_argc, p_convert_callback, p_exchange, p_lock, p_free_lock_callback) {
		const obj = RebelJSWrapper.get_proxied_value(p_id);
		if (obj === undefined) {
			return -1;
		}
		const method = RebelRuntime.parseString(p_method);
		const convert = RebelRuntime.get_func(p_convert_callback);
		const freeLock = RebelRuntime.get_func(p_free_lock_callback);
		const args = new Array(p_argc);
		for (let i = 0; i < p_argc; i++) {
			const type = convert(p_args, i, p_exchange, p_lock);
			const lock = RebelRuntime.getHeapValue(p_lock, '*');
			args[i] = RebelJSWrapper.variant2js(type, p_exchange);
			if (lock) {
				freeLock(p_lock, type);
			}
		}
		try {
			const res = obj[method](...args);
			return RebelJSWrapper.js2variant(res, p_exchange);
		} catch (e) {
			RebelRuntime.error(`Error calling method ${method} on:`, obj, 'error:', e);
			return -1;
		}
	},

	rebel_js_wrapper_object_unref__sig: 'vi',
	rebel_js_wrapper_object_unref: function (p_id) {
		const proxy = IDHandler.get(p_id);
		if (proxy !== undefined) {
			proxy.unref();
		}
	},

	rebel_js_wrapper_create_cb__sig: 'iii',
	rebel_js_wrapper_create_cb: function (p_ref, p_func) {
		const func = RebelRuntime.get_func(p_func);
		let id = 0;
		const cb = function () {
			if (!RebelJSWrapper.get_proxied_value(id)) {
				return;
			}
			const args = Array.from(arguments);
			func(p_ref, RebelJSWrapper.get_proxied(args), args.length);
		};
		id = RebelJSWrapper.get_proxied(cb);
		return id;
	},

	rebel_js_wrapper_object_getvar__sig: 'iiii',
	rebel_js_wrapper_object_getvar: function (p_id, p_type, p_exchange) {
		const obj = RebelJSWrapper.get_proxied_value(p_id);
		if (obj === undefined) {
			return -1;
		}
		const prop = RebelJSWrapper.variant2js(p_type, p_exchange);
		if (prop === undefined || prop === null) {
			return -1;
		}
		try {
			return RebelJSWrapper.js2variant(obj[prop], p_exchange);
		} catch (e) {
			RebelRuntime.error(`Error getting variable ${prop} on object`, obj, e);
			return -1;
		}
	},

	rebel_js_wrapper_object_setvar__sig: 'iiiiii',
	rebel_js_wrapper_object_setvar: function (p_id, p_key_type, p_key_ex, p_val_type, p_val_ex) {
		const obj = RebelJSWrapper.get_proxied_value(p_id);
		if (obj === undefined) {
			return -1;
		}
		const key = RebelJSWrapper.variant2js(p_key_type, p_key_ex);
		try {
			obj[key] = RebelJSWrapper.variant2js(p_val_type, p_val_ex);
			return 0;
		} catch (e) {
			RebelRuntime.error(`Error setting variable ${key} on object`, obj);
			return -1;
		}
	},

	rebel_js_wrapper_create_object__sig: 'iiiiiiii',
	rebel_js_wrapper_create_object: function (p_object, p_args, p_argc, p_convert_callback, p_exchange, p_lock, p_free_lock_callback) {
		const name = RebelRuntime.parseString(p_object);
		if (typeof (window[name]) === 'undefined') {
			return -1;
		}
		const convert = RebelRuntime.get_func(p_convert_callback);
		const freeLock = RebelRuntime.get_func(p_free_lock_callback);
		const args = new Array(p_argc);
		for (let i = 0; i < p_argc; i++) {
			const type = convert(p_args, i, p_exchange, p_lock);
			const lock = RebelRuntime.getHeapValue(p_lock, '*');
			args[i] = RebelJSWrapper.variant2js(type, p_exchange);
			if (lock) {
				freeLock(p_lock, type);
			}
		}
		try {
			const res = new window[name](...args);
			return RebelJSWrapper.js2variant(res, p_exchange);
		} catch (e) {
			RebelRuntime.error(`Error calling constructor ${name} with args:`, args, 'error:', e);
			return -1;
		}
	},
};

autoAddDeps(RebelJSWrapper, '$RebelJSWrapper');
mergeInto(LibraryManager.library, RebelJSWrapper);

const RebelEval = {
	rebel_js_eval__deps: ['$RebelRuntime'],
	rebel_js_eval__sig: 'iiiiiii',
	rebel_js_eval: function (p_js, p_use_global_ctx, p_union_ptr, p_byte_arr, p_byte_arr_write, p_callback) {
		const js_code = RebelRuntime.parseString(p_js);
		let eval_ret = null;
		try {
			if (p_use_global_ctx) {
				// indirect eval call grants global execution context
				const global_eval = eval;
				eval_ret = global_eval(js_code);
			} else {
				eval_ret = eval(js_code);
			}
		} catch (e) {
			RebelRuntime.error(e);
		}

		switch (typeof eval_ret) {
		case 'boolean':
			RebelRuntime.setHeapValue(p_union_ptr, eval_ret, 'i32');
			return 1; // BOOL

		case 'number':
			RebelRuntime.setHeapValue(p_union_ptr, eval_ret, 'double');
			return 3; // REAL

		case 'string':
			RebelRuntime.setHeapValue(p_union_ptr, RebelRuntime.allocString(eval_ret), '*');
			return 4; // STRING

		case 'object':
			if (eval_ret === null) {
				break;
			}

			if (ArrayBuffer.isView(eval_ret) && !(eval_ret instanceof Uint8Array)) {
				eval_ret = new Uint8Array(eval_ret.buffer);
			} else if (eval_ret instanceof ArrayBuffer) {
				eval_ret = new Uint8Array(eval_ret);
			}
			if (eval_ret instanceof Uint8Array) {
				const func = RebelRuntime.get_func(p_callback);
				const bytes_ptr = func(p_byte_arr, p_byte_arr_write, eval_ret.length);
				HEAPU8.set(eval_ret, bytes_ptr);
				return 20; // POOL_BYTE_ARRAY
			}
			break;

			// no default
		}
		return 0; // NIL
	},
};

mergeInto(LibraryManager.library, RebelEval);
