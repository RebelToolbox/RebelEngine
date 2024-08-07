// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "linux_gl_context.h"

#include "core/version.h"

#ifdef LINUX_ENABLED
#if defined(OPENGL_ENABLED)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GLX_GLXEXT_PROTOTYPES
#include <GL/glx.h>
#include <GL/glxext.h>

#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092

typedef GLXContext (*GLXCREATECONTEXTATTRIBSARBPROC)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

struct LinuxPrivateGLContext {
    ::GLXContext glx_context;
};

void LinuxGLContext::release_current() {
    glXMakeCurrent(x11_display, None, nullptr);
}

void LinuxGLContext::make_current() {
    glXMakeCurrent(x11_display, x11_window, p->glx_context);
}

void LinuxGLContext::swap_buffers() {
    glXSwapBuffers(x11_display, x11_window);
}

static bool ctxErrorOccurred = false;

static int ctxErrorHandler(Display* dpy, XErrorEvent* ev) {
    ctxErrorOccurred = true;
    return 0;
}

static void set_class_hint(Display* p_display, Window p_window) {
    XClassHint* classHint;

    /* set the name and class hints for the window manager to use */
    classHint = XAllocClassHint();
    if (classHint) {
        classHint->res_name  = (char*)"Rebel_Engine";
        classHint->res_class = (char*)"Rebel";
    }
    XSetClassHint(p_display, p_window, classHint);
    XFree(classHint);
}

Error LinuxGLContext::initialize() {
    // const char *extensions = glXQueryExtensionsString(x11_display,
    // DefaultScreen(x11_display));

    GLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB =
        (GLXCREATECONTEXTATTRIBSARBPROC
        )glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");

    ERR_FAIL_COND_V(!glXCreateContextAttribsARB, ERR_UNCONFIGURED);

    static int visual_attribs[] = {
        GLX_RENDER_TYPE,
        GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE,
        GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER,
        true,
        GLX_RED_SIZE,
        1,
        GLX_GREEN_SIZE,
        1,
        GLX_BLUE_SIZE,
        1,
        GLX_DEPTH_SIZE,
        24,
        None
    };

    static int visual_attribs_layered[] = {
        GLX_RENDER_TYPE,
        GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE,
        GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER,
        true,
        GLX_RED_SIZE,
        8,
        GLX_GREEN_SIZE,
        8,
        GLX_BLUE_SIZE,
        8,
        GLX_ALPHA_SIZE,
        8,
        GLX_DEPTH_SIZE,
        24,
        None
    };

    int fbcount;
    GLXFBConfig fbconfig = nullptr;
    XVisualInfo* vi      = nullptr;

    XSetWindowAttributes swa;
    swa.event_mask          = StructureNotifyMask;
    swa.border_pixel        = 0;
    unsigned long valuemask = CWBorderPixel | CWColormap | CWEventMask;

    if (OS::get_singleton()->is_layered_allowed()) {
        GLXFBConfig* fbc = glXChooseFBConfig(
            x11_display,
            DefaultScreen(x11_display),
            visual_attribs_layered,
            &fbcount
        );
        ERR_FAIL_COND_V(!fbc, ERR_UNCONFIGURED);

        for (int i = 0; i < fbcount; i++) {
            vi = (XVisualInfo*)glXGetVisualFromFBConfig(x11_display, fbc[i]);
            if (!vi) {
                continue;
            }

            XRenderPictFormat* pict_format =
                XRenderFindVisualFormat(x11_display, vi->visual);
            if (!pict_format) {
                XFree(vi);
                vi = nullptr;
                continue;
            }

            fbconfig = fbc[i];
            if (pict_format->direct.alphaMask > 0) {
                break;
            }
        }
        XFree(fbc);
        ERR_FAIL_COND_V(!fbconfig, ERR_UNCONFIGURED);

        swa.background_pixmap  = None;
        swa.background_pixel   = 0;
        swa.border_pixmap      = None;
        valuemask             |= CWBackPixel;

    } else {
        GLXFBConfig* fbc = glXChooseFBConfig(
            x11_display,
            DefaultScreen(x11_display),
            visual_attribs,
            &fbcount
        );
        ERR_FAIL_COND_V(!fbc, ERR_UNCONFIGURED);

        vi = glXGetVisualFromFBConfig(x11_display, fbc[0]);

        fbconfig = fbc[0];
        XFree(fbc);
    }

    int (*oldHandler)(Display*, XErrorEvent*) =
        XSetErrorHandler(&ctxErrorHandler);

    switch (context_type) {
        case OLDSTYLE: {
            p->glx_context =
                glXCreateContext(x11_display, vi, nullptr, GL_TRUE);
            ERR_FAIL_COND_V(!p->glx_context, ERR_UNCONFIGURED);
        } break;
        case GLES_2_0_COMPATIBLE: {
            p->glx_context = glXCreateNewContext(
                x11_display,
                fbconfig,
                GLX_RGBA_TYPE,
                nullptr,
                true
            );
            ERR_FAIL_COND_V(!p->glx_context, ERR_UNCONFIGURED);
        } break;
        case GLES_3_0_COMPATIBLE: {
            static int context_attribs[] = {
                GLX_CONTEXT_MAJOR_VERSION_ARB,
                3,
                GLX_CONTEXT_MINOR_VERSION_ARB,
                3,
                GLX_CONTEXT_PROFILE_MASK_ARB,
                GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
                GLX_CONTEXT_FLAGS_ARB,
                GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB /*|GLX_CONTEXT_DEBUG_BIT_ARB*/
                ,
                None
            };

            p->glx_context = glXCreateContextAttribsARB(
                x11_display,
                fbconfig,
                nullptr,
                true,
                context_attribs
            );
            ERR_FAIL_COND_V(
                ctxErrorOccurred || !p->glx_context,
                ERR_UNCONFIGURED
            );
        } break;
    }

    swa.colormap = XCreateColormap(
        x11_display,
        RootWindow(x11_display, vi->screen),
        vi->visual,
        AllocNone
    );
    x11_window = XCreateWindow(
        x11_display,
        RootWindow(x11_display, vi->screen),
        0,
        0,
        OS::get_singleton()->get_video_mode().width,
        OS::get_singleton()->get_video_mode().height,
        0,
        vi->depth,
        InputOutput,
        vi->visual,
        valuemask,
        &swa
    );
    XStoreName(x11_display, x11_window, VERSION_NAME);

    ERR_FAIL_COND_V(!x11_window, ERR_UNCONFIGURED);
    set_class_hint(x11_display, x11_window);

    if (!OS::get_singleton()->is_no_window_mode_enabled()) {
        XMapWindow(x11_display, x11_window);
    }

    XSync(x11_display, False);
    XSetErrorHandler(oldHandler);

    glXMakeCurrent(x11_display, x11_window, p->glx_context);

    XFree(vi);

    return OK;
}

int LinuxGLContext::get_window_width() {
    XWindowAttributes xwa;
    XGetWindowAttributes(x11_display, x11_window, &xwa);

    return xwa.width;
}

int LinuxGLContext::get_window_height() {
    XWindowAttributes xwa;
    XGetWindowAttributes(x11_display, x11_window, &xwa);

    return xwa.height;
}

void* LinuxGLContext::get_glx_context() {
    if (p != nullptr) {
        return p->glx_context;
    } else {
        return nullptr;
    }
}

void LinuxGLContext::set_use_vsync(bool p_use) {
    static bool setup                                    = false;
    static PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT  = nullptr;
    static PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalMESA = nullptr;
    static PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI  = nullptr;

    if (!setup) {
        setup = true;
        String extensions =
            glXQueryExtensionsString(x11_display, DefaultScreen(x11_display));
        if (extensions.find("GLX_EXT_swap_control") != -1) {
            glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC
            )glXGetProcAddressARB((const GLubyte*)"glXSwapIntervalEXT");
        }
        if (extensions.find("GLX_MESA_swap_control") != -1) {
            glXSwapIntervalMESA = (PFNGLXSWAPINTERVALSGIPROC
            )glXGetProcAddressARB((const GLubyte*)"glXSwapIntervalMESA");
        }
        if (extensions.find("GLX_SGI_swap_control") != -1) {
            glXSwapIntervalSGI = (PFNGLXSWAPINTERVALSGIPROC
            )glXGetProcAddressARB((const GLubyte*)"glXSwapIntervalSGI");
        }
    }
    int val = p_use ? 1 : 0;
    if (glXSwapIntervalMESA) {
        glXSwapIntervalMESA(val);
    } else if (glXSwapIntervalSGI) {
        glXSwapIntervalSGI(val);
    } else if (glXSwapIntervalEXT) {
        GLXDrawable drawable = glXGetCurrentDrawable();
        glXSwapIntervalEXT(x11_display, drawable, val);
    } else {
        return;
    }
    use_vsync = p_use;
}

bool LinuxGLContext::is_using_vsync() const {
    return use_vsync;
}

LinuxGLContext::LinuxGLContext(
    ::Display* p_x11_display,
    ::Window& p_x11_window,
    const OS::VideoMode& p_default_video_mode,
    ContextType p_context_type
) :
    x11_window(p_x11_window) {
    default_video_mode = p_default_video_mode;
    x11_display        = p_x11_display;

    context_type = p_context_type;

    double_buffer = false;
    direct_render = false;
    glx_minor = glx_major = 0;
    p                     = memnew(LinuxPrivateGLContext);
    p->glx_context        = nullptr;
    use_vsync             = false;
}

LinuxGLContext::~LinuxGLContext() {
    release_current();
    glXDestroyContext(x11_display, p->glx_context);
    memdelete(p);
}

#endif
#endif
