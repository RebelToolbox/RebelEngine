// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#import <UIKit/UIKit.h>

@class GodotView;
@class GodotNativeVideoView;
@class GodotKeyboardInputView;

@interface ViewController : UIViewController

@property(nonatomic, readonly, strong) GodotView* godotView;
@property(nonatomic, readonly, strong) GodotNativeVideoView* videoView;
@property(nonatomic, readonly, strong) GodotKeyboardInputView* keyboardView;

// MARK: Native Video Player

- (BOOL)playVideoAtPath:(NSString*)filePath
                 volume:(float)videoVolume
                  audio:(NSString*)audioTrack
               subtitle:(NSString*)subtitleTrack;

@end
