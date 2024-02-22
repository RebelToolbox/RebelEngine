// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT
const RebelWebXR = {

	$RebelWebXR__deps: ['$Browser', '$GL', '$RebelRuntime'],
	$RebelWebXR: {
		gl: null,

		texture_ids: [null, null],
		textures: [null, null],

		session: null,
		space: null,
		frame: null,
		pose: null,

		// Monkey-patch the requestAnimationFrame() used by Emscripten for the main
		// loop, so that we can swap it out for XRSession.requestAnimationFrame()
		// when an XR session is started.
		orig_requestAnimationFrame: null,
		requestAnimationFrame: (callback) => {
			if (RebelWebXR.session && RebelWebXR.space) {
				const onFrame = function (time, frame) {
					RebelWebXR.frame = frame;
					RebelWebXR.pose = frame.getViewerPose(RebelWebXR.space);
					callback(time);
					RebelWebXR.frame = null;
					RebelWebXR.pose = null;
				};
				RebelWebXR.session.requestAnimationFrame(onFrame);
			} else {
				RebelWebXR.orig_requestAnimationFrame(callback);
			}
		},
		monkeyPatchRequestAnimationFrame: (enable) => {
			if (RebelWebXR.orig_requestAnimationFrame === null) {
				RebelWebXR.orig_requestAnimationFrame = Browser.requestAnimationFrame;
			}
			Browser.requestAnimationFrame = enable
				? RebelWebXR.requestAnimationFrame : RebelWebXR.orig_requestAnimationFrame;
		},
		pauseResumeMainLoop: () => {
			// Once both RebelWebXR.session and RebelWebXR.space are set or
			// unset, our monkey-patched requestAnimationFrame() should be
			// enabled or disabled. When using the WebXR API Emulator, this
			// gets picked up automatically, however, in the Oculus Browser
			// on the Quest, we need to pause and resume the main loop.
			Browser.mainLoop.pause();
			window.setTimeout(function () {
				Browser.mainLoop.resume();
			}, 0);
		},

		// Some custom WebGL code for blitting our eye textures to the
		// framebuffer we get from WebXR.
		shaderProgram: null,
		programInfo: null,
		buffer: null,
		// Vertex shader source.
		vsSource: `
			const vec2 scale = vec2(0.5, 0.5);
			attribute vec4 aVertexPosition;

			varying highp vec2 vTextureCoord;

			void main () {
				gl_Position = aVertexPosition;
				vTextureCoord = aVertexPosition.xy * scale + scale;
			}
		`,
		// Fragment shader source.
		fsSource: `
			varying highp vec2 vTextureCoord;

			uniform sampler2D uSampler;

			void main() {
				gl_FragColor = texture2D(uSampler, vTextureCoord);
			}
		`,

		initShaderProgram: (gl, vsSource, fsSource) => {
			const vertexShader = RebelWebXR.loadShader(gl, gl.VERTEX_SHADER, vsSource);
			const fragmentShader = RebelWebXR.loadShader(gl, gl.FRAGMENT_SHADER, fsSource);

			const shaderProgram = gl.createProgram();
			gl.attachShader(shaderProgram, vertexShader);
			gl.attachShader(shaderProgram, fragmentShader);
			gl.linkProgram(shaderProgram);

			if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
				RebelRuntime.error(`Unable to initialize the shader program: ${gl.getProgramInfoLog(shaderProgram)}`);
				return null;
			}

			return shaderProgram;
		},
		loadShader: (gl, type, source) => {
			const shader = gl.createShader(type);
			gl.shaderSource(shader, source);
			gl.compileShader(shader);

			if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
				RebelRuntime.error(`An error occurred compiling the shader: ${gl.getShaderInfoLog(shader)}`);
				gl.deleteShader(shader);
				return null;
			}

			return shader;
		},
		initBuffer: (gl) => {
			const positionBuffer = gl.createBuffer();
			gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
			const positions = [
				-1.0, -1.0,
				1.0, -1.0,
				-1.0, 1.0,
				1.0, 1.0,
			];
			gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);
			return positionBuffer;
		},
		blitTexture: (gl, texture) => {
			if (RebelWebXR.shaderProgram === null) {
				RebelWebXR.shaderProgram = RebelWebXR.initShaderProgram(gl, RebelWebXR.vsSource, RebelWebXR.fsSource);
				RebelWebXR.programInfo = {
					program: RebelWebXR.shaderProgram,
					attribLocations: {
						vertexPosition: gl.getAttribLocation(RebelWebXR.shaderProgram, 'aVertexPosition'),
					},
					uniformLocations: {
						uSampler: gl.getUniformLocation(RebelWebXR.shaderProgram, 'uSampler'),
					},
				};
				RebelWebXR.buffer = RebelWebXR.initBuffer(gl);
			}

			const orig_program = gl.getParameter(gl.CURRENT_PROGRAM);
			gl.useProgram(RebelWebXR.shaderProgram);

			gl.bindBuffer(gl.ARRAY_BUFFER, RebelWebXR.buffer);
			gl.vertexAttribPointer(RebelWebXR.programInfo.attribLocations.vertexPosition, 2, gl.FLOAT, false, 0, 0);
			gl.enableVertexAttribArray(RebelWebXR.programInfo.attribLocations.vertexPosition);

			gl.activeTexture(gl.TEXTURE0);
			gl.bindTexture(gl.TEXTURE_2D, texture);
			gl.uniform1i(RebelWebXR.programInfo.uniformLocations.uSampler, 0);

			gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);

			// Restore state.
			gl.bindTexture(gl.TEXTURE_2D, null);
			gl.disableVertexAttribArray(RebelWebXR.programInfo.attribLocations.vertexPosition);
			gl.bindBuffer(gl.ARRAY_BUFFER, null);
			gl.useProgram(orig_program);
		},

		// Holds the controllers list between function calls.
		controllers: [],

		// Updates controllers array, where the left hand (or sole tracker) is
		// the first element, and the right hand is the second element, and any
		// others placed at the 3rd position and up.
		sampleControllers: () => {
			if (!RebelWebXR.session || !RebelWebXR.frame) {
				return;
			}

			let other_index = 2;
			const controllers = [];
			RebelWebXR.session.inputSources.forEach((input_source) => {
				if (input_source.targetRayMode === 'tracked-pointer') {
					if (input_source.handedness === 'right') {
						controllers[1] = input_source;
					} else if (input_source.handedness === 'left' || !controllers[0]) {
						controllers[0] = input_source;
					}
				} else {
					controllers[other_index++] = input_source;
				}
			});
			RebelWebXR.controllers = controllers;
		},

		getControllerId: (input_source) => RebelWebXR.controllers.indexOf(input_source),
	},

	rebel_webxr_is_supported__proxy: 'sync',
	rebel_webxr_is_supported__sig: 'i',
	rebel_webxr_is_supported: function () {
		return !!navigator.xr;
	},

	rebel_webxr_is_session_supported__proxy: 'sync',
	rebel_webxr_is_session_supported__sig: 'vii',
	rebel_webxr_is_session_supported: function (p_session_mode, p_callback) {
		const session_mode = RebelRuntime.parseString(p_session_mode);
		const cb = RebelRuntime.get_func(p_callback);
		if (navigator.xr) {
			navigator.xr.isSessionSupported(session_mode).then(function (supported) {
				const c_str = RebelRuntime.allocString(session_mode);
				cb(c_str, supported ? 1 : 0);
				RebelRuntime.free(c_str);
			});
		} else {
			const c_str = RebelRuntime.allocString(session_mode);
			cb(c_str, 0);
			RebelRuntime.free(c_str);
		}
	},

	rebel_webxr_initialize__deps: ['emscripten_webgl_get_current_context'],
	rebel_webxr_initialize__proxy: 'sync',
	rebel_webxr_initialize__sig: 'viiiiiiiiii',
	rebel_webxr_initialize: function (p_session_mode, p_required_features, p_optional_features, p_requested_reference_spaces, p_on_session_started, p_on_session_ended, p_on_session_failed, p_on_controller_changed, p_on_input_event, p_on_simple_event) {
		RebelWebXR.monkeyPatchRequestAnimationFrame(true);

		const session_mode = RebelRuntime.parseString(p_session_mode);
		const required_features = RebelRuntime.parseString(p_required_features).split(',').map((s) => s.trim()).filter((s) => s !== '');
		const optional_features = RebelRuntime.parseString(p_optional_features).split(',').map((s) => s.trim()).filter((s) => s !== '');
		const requested_reference_space_types = RebelRuntime.parseString(p_requested_reference_spaces).split(',').map((s) => s.trim());
		const onstarted = RebelRuntime.get_func(p_on_session_started);
		const onended = RebelRuntime.get_func(p_on_session_ended);
		const onfailed = RebelRuntime.get_func(p_on_session_failed);
		const oncontroller = RebelRuntime.get_func(p_on_controller_changed);
		const oninputevent = RebelRuntime.get_func(p_on_input_event);
		const onsimpleevent = RebelRuntime.get_func(p_on_simple_event);

		const session_init = {};
		if (required_features.length > 0) {
			session_init['requiredFeatures'] = required_features;
		}
		if (optional_features.length > 0) {
			session_init['optionalFeatures'] = optional_features;
		}

		navigator.xr.requestSession(session_mode, session_init).then(function (session) {
			RebelWebXR.session = session;

			session.addEventListener('end', function (evt) {
				onended();
			});

			session.addEventListener('inputsourceschange', function (evt) {
				let controller_changed = false;
				[evt.added, evt.removed].forEach((lst) => {
					lst.forEach((input_source) => {
						if (input_source.targetRayMode === 'tracked-pointer') {
							controller_changed = true;
						}
					});
				});
				if (controller_changed) {
					oncontroller();
				}
			});

			['selectstart', 'select', 'selectend', 'squeezestart', 'squeeze', 'squeezeend'].forEach((input_event) => {
				session.addEventListener(input_event, function (evt) {
					const c_str = RebelRuntime.allocString(input_event);
					oninputevent(c_str, RebelWebXR.getControllerId(evt.inputSource));
					RebelRuntime.free(c_str);
				});
			});

			session.addEventListener('visibilitychange', function (evt) {
				const c_str = RebelRuntime.allocString('visibility_state_changed');
				onsimpleevent(c_str);
				RebelRuntime.free(c_str);
			});

			const gl_context_handle = _emscripten_webgl_get_current_context(); // eslint-disable-line no-undef
			const gl = GL.getContext(gl_context_handle).GLctx;
			RebelWebXR.gl = gl;

			gl.makeXRCompatible().then(function () {
				session.updateRenderState({
					baseLayer: new XRWebGLLayer(session, gl),
				});

				function onReferenceSpaceSuccess(reference_space, reference_space_type) {
					RebelWebXR.space = reference_space;

					// Using reference_space.addEventListener() crashes when
					// using the polyfill with the WebXR Emulator extension,
					// so we set the event property instead.
					reference_space.onreset = function (evt) {
						const c_str = RebelRuntime.allocString('reference_space_reset');
						onsimpleevent(c_str);
						RebelRuntime.free(c_str);
					};

					// Now that both RebelWebXR.session and RebelWebXR.space are
					// set, we need to pause and resume the main loop for the XR
					// main loop to kick in.
					RebelWebXR.pauseResumeMainLoop();

					// Call in setTimeout() so that errors in the onstarted()
					// callback don't bubble up here and cause Rebel to try the
					// next reference space.
					window.setTimeout(function () {
						const c_str = RebelRuntime.allocString(reference_space_type);
						onstarted(c_str);
						RebelRuntime.free(c_str);
					}, 0);
				}

				function requestReferenceSpace() {
					const reference_space_type = requested_reference_space_types.shift();
					session.requestReferenceSpace(reference_space_type)
						.then((refSpace) => {
							onReferenceSpaceSuccess(refSpace, reference_space_type);
						})
						.catch(() => {
							if (requested_reference_space_types.length === 0) {
								const c_str = RebelRuntime.allocString('Unable to get any of the requested reference space types');
								onfailed(c_str);
								RebelRuntime.free(c_str);
							} else {
								requestReferenceSpace();
							}
						});
				}

				requestReferenceSpace();
			}).catch(function (error) {
				const c_str = RebelRuntime.allocString(`Unable to make WebGL context compatible with WebXR: ${error}`);
				onfailed(c_str);
				RebelRuntime.free(c_str);
			});
		}).catch(function (error) {
			const c_str = RebelRuntime.allocString(`Unable to start session: ${error}`);
			onfailed(c_str);
			RebelRuntime.free(c_str);
		});
	},

	rebel_webxr_uninitialize__proxy: 'sync',
	rebel_webxr_uninitialize__sig: 'v',
	rebel_webxr_uninitialize: function () {
		if (RebelWebXR.session) {
			RebelWebXR.session.end()
				// Prevent exception when session has already ended.
				.catch((e) => { });
		}

		// Clean-up the textures we allocated for each view.
		const gl = RebelWebXR.gl;
		for (let i = 0; i < RebelWebXR.textures.length; i++) {
			const texture = RebelWebXR.textures[i];
			if (texture !== null) {
				gl.deleteTexture(texture);
			}
			RebelWebXR.textures[i] = null;

			const texture_id = RebelWebXR.texture_ids[i];
			if (texture_id !== null) {
				GL.textures[texture_id] = null;
			}
			RebelWebXR.texture_ids[i] = null;
		}

		RebelWebXR.session = null;
		RebelWebXR.space = null;
		RebelWebXR.frame = null;
		RebelWebXR.pose = null;

		// Disable the monkey-patched window.requestAnimationFrame() and
		// pause/restart the main loop to activate it on all platforms.
		RebelWebXR.monkeyPatchRequestAnimationFrame(false);
		RebelWebXR.pauseResumeMainLoop();
	},

	rebel_webxr_get_view_count__proxy: 'sync',
	rebel_webxr_get_view_count__sig: 'i',
	rebel_webxr_get_view_count: function () {
		if (!RebelWebXR.session || !RebelWebXR.pose) {
			return 0;
		}
		return RebelWebXR.pose.views.length;
	},

	rebel_webxr_get_render_targetsize__proxy: 'sync',
	rebel_webxr_get_render_targetsize__sig: 'i',
	rebel_webxr_get_render_targetsize: function () {
		if (!RebelWebXR.session || !RebelWebXR.pose) {
			return 0;
		}

		const glLayer = RebelWebXR.session.renderState.baseLayer;
		const view = RebelWebXR.pose.views[0];
		const viewport = glLayer.getViewport(view);

		const buf = RebelRuntime.malloc(2 * 4);
		RebelRuntime.setHeapValue(buf + 0, viewport.width, 'i32');
		RebelRuntime.setHeapValue(buf + 4, viewport.height, 'i32');
		return buf;
	},

	rebel_webxr_get_transform_for_eye__proxy: 'sync',
	rebel_webxr_get_transform_for_eye__sig: 'ii',
	rebel_webxr_get_transform_for_eye: function (p_eye) {
		if (!RebelWebXR.session || !RebelWebXR.pose) {
			return 0;
		}

		const views = RebelWebXR.pose.views;
		let matrix;
		if (p_eye === 0) {
			matrix = RebelWebXR.pose.transform.matrix;
		} else {
			matrix = views[p_eye - 1].transform.matrix;
		}
		const buf = RebelRuntime.malloc(16 * 4);
		for (let i = 0; i < 16; i++) {
			RebelRuntime.setHeapValue(buf + (i * 4), matrix[i], 'float');
		}
		return buf;
	},

	rebel_webxr_get_projection_for_eye__proxy: 'sync',
	rebel_webxr_get_projection_for_eye__sig: 'ii',
	rebel_webxr_get_projection_for_eye: function (p_eye) {
		if (!RebelWebXR.session || !RebelWebXR.pose) {
			return 0;
		}

		const view_index = (p_eye === 2 /* ARVRInterface::EYE_RIGHT */) ? 1 : 0;
		const matrix = RebelWebXR.pose.views[view_index].projectionMatrix;
		const buf = RebelRuntime.malloc(16 * 4);
		for (let i = 0; i < 16; i++) {
			RebelRuntime.setHeapValue(buf + (i * 4), matrix[i], 'float');
		}
		return buf;
	},

	rebel_webxr_get_external_texture_for_eye__proxy: 'sync',
	rebel_webxr_get_external_texture_for_eye__sig: 'ii',
	rebel_webxr_get_external_texture_for_eye: function (p_eye) {
		if (!RebelWebXR.session) {
			return 0;
		}

		const view_index = (p_eye === 2 /* ARVRInterface::EYE_RIGHT */) ? 1 : 0;
		if (RebelWebXR.texture_ids[view_index]) {
			return RebelWebXR.texture_ids[view_index];
		}

		// Check pose separately and after returning the cached texture id,
		// because we won't get a pose in some cases if we lose tracking, and
		// we don't want to return 0 just because tracking was lost.
		if (!RebelWebXR.pose) {
			return 0;
		}

		const glLayer = RebelWebXR.session.renderState.baseLayer;
		const view = RebelWebXR.pose.views[view_index];
		const viewport = glLayer.getViewport(view);
		const gl = RebelWebXR.gl;

		const texture = gl.createTexture();
		gl.bindTexture(gl.TEXTURE_2D, texture);
		gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, viewport.width, viewport.height, 0, gl.RGBA, gl.UNSIGNED_BYTE, null);

		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
		gl.bindTexture(gl.TEXTURE_2D, null);

		const texture_id = GL.getNewId(GL.textures);
		GL.textures[texture_id] = texture;
		RebelWebXR.textures[view_index] = texture;
		RebelWebXR.texture_ids[view_index] = texture_id;
		return texture_id;
	},

	rebel_webxr_commit_for_eye__proxy: 'sync',
	rebel_webxr_commit_for_eye__sig: 'vi',
	rebel_webxr_commit_for_eye: function (p_eye) {
		if (!RebelWebXR.session || !RebelWebXR.pose) {
			return;
		}

		const view_index = (p_eye === 2 /* ARVRInterface::EYE_RIGHT */) ? 1 : 0;
		const glLayer = RebelWebXR.session.renderState.baseLayer;
		const view = RebelWebXR.pose.views[view_index];
		const viewport = glLayer.getViewport(view);
		const gl = RebelWebXR.gl;

		const orig_framebuffer = gl.getParameter(gl.FRAMEBUFFER_BINDING);
		const orig_viewport = gl.getParameter(gl.VIEWPORT);

		// Bind to WebXR's framebuffer.
		gl.bindFramebuffer(gl.FRAMEBUFFER, glLayer.framebuffer);
		gl.viewport(viewport.x, viewport.y, viewport.width, viewport.height);

		RebelWebXR.blitTexture(gl, RebelWebXR.textures[view_index]);

		// Restore state.
		gl.bindFramebuffer(gl.FRAMEBUFFER, orig_framebuffer);
		gl.viewport(orig_viewport[0], orig_viewport[1], orig_viewport[2], orig_viewport[3]);
	},

	rebel_webxr_sample_controller_data__proxy: 'sync',
	rebel_webxr_sample_controller_data__sig: 'v',
	rebel_webxr_sample_controller_data: function () {
		RebelWebXR.sampleControllers();
	},

	rebel_webxr_get_controller_count__proxy: 'sync',
	rebel_webxr_get_controller_count__sig: 'i',
	rebel_webxr_get_controller_count: function () {
		if (!RebelWebXR.session || !RebelWebXR.frame) {
			return 0;
		}
		return RebelWebXR.controllers.length;
	},

	rebel_webxr_is_controller_connected__proxy: 'sync',
	rebel_webxr_is_controller_connected__sig: 'ii',
	rebel_webxr_is_controller_connected: function (p_controller) {
		if (!RebelWebXR.session || !RebelWebXR.frame) {
			return false;
		}
		return !!RebelWebXR.controllers[p_controller];
	},

	rebel_webxr_get_controller_transform__proxy: 'sync',
	rebel_webxr_get_controller_transform__sig: 'ii',
	rebel_webxr_get_controller_transform: function (p_controller) {
		if (!RebelWebXR.session || !RebelWebXR.frame) {
			return 0;
		}

		const controller = RebelWebXR.controllers[p_controller];
		if (!controller) {
			return 0;
		}

		const frame = RebelWebXR.frame;
		const space = RebelWebXR.space;

		const pose = frame.getPose(controller.targetRaySpace, space);
		if (!pose) {
			// This can mean that the controller lost tracking.
			return 0;
		}
		const matrix = pose.transform.matrix;

		const buf = RebelRuntime.malloc(16 * 4);
		for (let i = 0; i < 16; i++) {
			RebelRuntime.setHeapValue(buf + (i * 4), matrix[i], 'float');
		}
		return buf;
	},

	rebel_webxr_get_controller_buttons__proxy: 'sync',
	rebel_webxr_get_controller_buttons__sig: 'ii',
	rebel_webxr_get_controller_buttons: function (p_controller) {
		if (RebelWebXR.controllers.length === 0) {
			return 0;
		}

		const controller = RebelWebXR.controllers[p_controller];
		if (!controller || !controller.gamepad) {
			return 0;
		}

		const button_count = controller.gamepad.buttons.length;

		const buf = RebelRuntime.malloc((button_count + 1) * 4);
		RebelRuntime.setHeapValue(buf, button_count, 'i32');
		for (let i = 0; i < button_count; i++) {
			RebelRuntime.setHeapValue(buf + 4 + (i * 4), controller.gamepad.buttons[i].value, 'float');
		}
		return buf;
	},

	rebel_webxr_get_controller_axes__proxy: 'sync',
	rebel_webxr_get_controller_axes__sig: 'ii',
	rebel_webxr_get_controller_axes: function (p_controller) {
		if (RebelWebXR.controllers.length === 0) {
			return 0;
		}

		const controller = RebelWebXR.controllers[p_controller];
		if (!controller || !controller.gamepad) {
			return 0;
		}

		const axes_count = controller.gamepad.axes.length;

		const buf = RebelRuntime.malloc((axes_count + 1) * 4);
		RebelRuntime.setHeapValue(buf, axes_count, 'i32');
		for (let i = 0; i < axes_count; i++) {
			let value = controller.gamepad.axes[i];
			if (i === 1 || i === 3) {
				// Invert the Y-axis on thumbsticks and trackpads, in order to
				// match OpenXR and other XR platform SDKs.
				value *= -1.0;
			}
			RebelRuntime.setHeapValue(buf + 4 + (i * 4), value, 'float');
		}
		return buf;
	},

	rebel_webxr_get_visibility_state__proxy: 'sync',
	rebel_webxr_get_visibility_state__sig: 'i',
	rebel_webxr_get_visibility_state: function () {
		if (!RebelWebXR.session || !RebelWebXR.session.visibilityState) {
			return 0;
		}

		return RebelRuntime.allocString(RebelWebXR.session.visibilityState);
	},

	rebel_webxr_get_bounds_geometry__proxy: 'sync',
	rebel_webxr_get_bounds_geometry__sig: 'i',
	rebel_webxr_get_bounds_geometry: function () {
		if (!RebelWebXR.space || !RebelWebXR.space.boundsGeometry) {
			return 0;
		}

		const point_count = RebelWebXR.space.boundsGeometry.length;
		if (point_count === 0) {
			return 0;
		}

		const buf = RebelRuntime.malloc(((point_count * 3) + 1) * 4);
		RebelRuntime.setHeapValue(buf, point_count, 'i32');
		for (let i = 0; i < point_count; i++) {
			const point = RebelWebXR.space.boundsGeometry[i];
			RebelRuntime.setHeapValue(buf + ((i * 3) + 1) * 4, point.x, 'float');
			RebelRuntime.setHeapValue(buf + ((i * 3) + 2) * 4, point.y, 'float');
			RebelRuntime.setHeapValue(buf + ((i * 3) + 3) * 4, point.z, 'float');
		}

		return buf;
	},

};

autoAddDeps(RebelWebXR, '$RebelWebXR');
mergeInto(LibraryManager.library, RebelWebXR);
