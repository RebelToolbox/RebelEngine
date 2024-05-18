// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#import <UIKit/UIKit.h>

@class RebelView;
@class RebelNativeVideoView;
@class RebelKeyboardInputView;

@interface ViewController : UIViewController

@property(nonatomic, readonly, strong) RebelView* rebelView;
@property(nonatomic, readonly, strong) RebelNativeVideoView* videoView;
@property(nonatomic, readonly, strong) RebelKeyboardInputView* keyboardView;

// MARK: Native Video Player

- (BOOL)playVideoAtPath:(NSString*)filePath
                 volume:(float)videoVolume
                  audio:(NSString*)audioTrack
               subtitle:(NSString*)subtitleTrack;

@end
