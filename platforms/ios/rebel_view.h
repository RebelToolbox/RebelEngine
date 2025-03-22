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

@class RebelView;
@protocol DisplayLayer;
@protocol RebelViewRendererProtocol;

@protocol RebelViewDelegate

- (BOOL)rebelViewFinishedSetup:(RebelView*)view;

@end

@interface RebelView : UIView

@property(assign, nonatomic) id<RebelViewRendererProtocol> renderer;
@property(assign, nonatomic) id<RebelViewDelegate> delegate;

@property(assign, readonly, nonatomic) BOOL isActive;

@property(strong, readonly, nonatomic) CALayer<DisplayLayer>* renderingLayer;
@property(assign, readonly, nonatomic) BOOL canRender;

@property(assign, nonatomic) NSTimeInterval renderingInterval;

- (CALayer<DisplayLayer>*)initializeRendering;
- (void)stopRendering;
- (void)startRendering;

@property(nonatomic, assign) BOOL useCADisplayLink;

- (void)rebelTouchesBegan:(NSSet*)touches withEvent:(UIEvent*)event;
- (void)rebelTouchesMoved:(NSSet*)touches withEvent:(UIEvent*)event;
- (void)rebelTouchesEnded:(NSSet*)touches withEvent:(UIEvent*)event;
- (void)rebelTouchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event;

@end
