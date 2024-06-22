// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#import "display_layer.h"

#include "core/os/keyboard.h"
#include "core/project_settings.h"
#include "ios_os.h"
#include "main/main.h"
#include "servers/audio_server.h"

#import <AudioToolbox/AudioServices.h>
#import <GameController/GameController.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>

int gl_view_base_fb;
bool gles3_available = true;

@implementation RebelOpenGLLayer {
    // The pixel dimensions of the backbuffer
    GLint backingWidth;
    GLint backingHeight;

    EAGLContext* context;
    GLuint viewRenderbuffer, viewFramebuffer;
    GLuint depthRenderbuffer;
}

- (void)initializeDisplayLayer {
    // Configure it so that it is opaque, does not retain the contents of the
    // backbuffer when displayed, and uses RGBA8888 color.
    self.opaque             = YES;
    self.drawableProperties = [NSDictionary
        dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:FALSE],
                                     kEAGLDrawablePropertyRetainedBacking,
                                     kEAGLColorFormatRGBA8,
                                     kEAGLDrawablePropertyColorFormat,
                                     nil];
    bool fallback_gl2       = false;
    // Create a GL ES 3 context based on the gl driver from project settings
    if (GLOBAL_GET("rendering/quality/driver/driver_name") == "GLES3") {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        NSLog(
            @"Setting up an OpenGL ES 3.0 context. Based on Project Settings "
            @"\"rendering/quality/driver/driver_name\""
        );
        if (!context
            && GLOBAL_GET("rendering/quality/driver/fallback_to_gles2")) {
            gles3_available = false;
            fallback_gl2    = true;
            NSLog(
                @"Failed to create OpenGL ES 3.0 context. Falling back to "
                @"OpenGL ES 2.0"
            );
        }
    }

    // Create GL ES 2 context
    if (GLOBAL_GET("rendering/quality/driver/driver_name") == "GLES2"
        || fallback_gl2) {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        NSLog(@"Setting up an OpenGL ES 2.0 context.");
        if (!context) {
            NSLog(@"Failed to create OpenGL ES 2.0 context!");
            return;
        }
    }

    if (![EAGLContext setCurrentContext:context]) {
        NSLog(@"Failed to set EAGLContext!");
        return;
    }
    if (![self createFramebuffer]) {
        NSLog(@"Failed to create frame buffer!");
        return;
    }
}

- (void)layoutDisplayLayer {
    [EAGLContext setCurrentContext:context];
    [self destroyFramebuffer];
    [self createFramebuffer];
}

- (void)startRenderDisplayLayer {
    [EAGLContext setCurrentContext:context];

    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
}

- (void)stopRenderDisplayLayer {
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];

#ifdef DEBUG_ENABLED
    GLenum err = glGetError();
    if (err) {
        NSLog(@"DrawView: %x error", err);
    }
#endif
}

- (void)dealloc {
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }

    if (context) {
        context = nil;
    }
}

- (BOOL)createFramebuffer {
    // Generate IDs for a framebuffer object and a color renderbuffer
    glGenFramebuffersOES(1, &viewFramebuffer);
    glGenRenderbuffersOES(1, &viewRenderbuffer);

    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    // This call associates the storage for the current render buffer with the
    // EAGLDrawable (our CAEAGLLayer) allowing us to draw into a buffer that
    // will later be rendered to screen wherever the layer is (which corresponds
    // with our view).
    [context renderbufferStorage:GL_RENDERBUFFER_OES
                    fromDrawable:(id<EAGLDrawable>)self];
    glFramebufferRenderbufferOES(
        GL_FRAMEBUFFER_OES,
        GL_COLOR_ATTACHMENT0_OES,
        GL_RENDERBUFFER_OES,
        viewRenderbuffer
    );

    glGetRenderbufferParameterivOES(
        GL_RENDERBUFFER_OES,
        GL_RENDERBUFFER_WIDTH_OES,
        &backingWidth
    );
    glGetRenderbufferParameterivOES(
        GL_RENDERBUFFER_OES,
        GL_RENDERBUFFER_HEIGHT_OES,
        &backingHeight
    );

    // For this sample, we also need a depth buffer, so we'll create and attach
    // one via another renderbuffer.
    glGenRenderbuffersOES(1, &depthRenderbuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
    glRenderbufferStorageOES(
        GL_RENDERBUFFER_OES,
        GL_DEPTH_COMPONENT16_OES,
        backingWidth,
        backingHeight
    );
    glFramebufferRenderbufferOES(
        GL_FRAMEBUFFER_OES,
        GL_DEPTH_ATTACHMENT_OES,
        GL_RENDERBUFFER_OES,
        depthRenderbuffer
    );

    if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES)
        != GL_FRAMEBUFFER_COMPLETE_OES) {
        NSLog(
            @"failed to make complete framebuffer object %x",
            glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES)
        );
        return NO;
    }

    if (OS::get_singleton()) {
        OS::VideoMode vm;
        vm.fullscreen = true;
        vm.width      = backingWidth;
        vm.height     = backingHeight;
        vm.resizable  = false;
        OS::get_singleton()->set_video_mode(vm);
        IosOS::get_singleton()->set_base_framebuffer(viewFramebuffer);
    }

    gl_view_base_fb = viewFramebuffer;

    return YES;
}

// Clean up any buffers we have allocated.
- (void)destroyFramebuffer {
    glDeleteFramebuffersOES(1, &viewFramebuffer);
    viewFramebuffer = 0;
    glDeleteRenderbuffersOES(1, &viewRenderbuffer);
    viewRenderbuffer = 0;

    if (depthRenderbuffer) {
        glDeleteRenderbuffersOES(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
}

@end
