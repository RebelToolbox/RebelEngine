// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#import <AVFoundation/AVFoundation.h>
#import <MediaPlayer/MediaPlayer.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <UIKit/UIKit.h>

class String;

@class GodotView;
@protocol DisplayLayer;
@protocol GodotViewRendererProtocol;

@protocol GodotViewDelegate

- (BOOL)godotViewFinishedSetup:(GodotView*)view;

@end

@interface GodotView : UIView

@property(assign, nonatomic) id<GodotViewRendererProtocol> renderer;
@property(assign, nonatomic) id<GodotViewDelegate> delegate;

@property(assign, readonly, nonatomic) BOOL isActive;

@property(strong, readonly, nonatomic) CALayer<DisplayLayer>* renderingLayer;
@property(assign, readonly, nonatomic) BOOL canRender;

@property(assign, nonatomic) NSTimeInterval renderingInterval;

- (CALayer<DisplayLayer>*)initializeRendering;
- (void)stopRendering;
- (void)startRendering;

@property(nonatomic, assign) BOOL useCADisplayLink;

- (void)godotTouchesBegan:(NSSet*)touches withEvent:(UIEvent*)event;
- (void)godotTouchesMoved:(NSSet*)touches withEvent:(UIEvent*)event;
- (void)godotTouchesEnded:(NSSet*)touches withEvent:(UIEvent*)event;
- (void)godotTouchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event;

@end
