// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#import <UIKit/UIKit.h>

@interface RebelNativeVideoView : UIView

- (BOOL)playVideoAtPath:(NSString*)filePath
                 volume:(float)videoVolume
                  audio:(NSString*)audioTrack
               subtitle:(NSString*)subtitleTrack;
- (BOOL)isVideoPlaying;
- (void)pauseVideo;
- (void)unfocusVideo;
- (void)unpauseVideo;
- (void)stopVideo;

@end
