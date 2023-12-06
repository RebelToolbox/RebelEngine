// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#import "view_controller.h"

#include "core/project_settings.h"
#import "godot_view.h"
#import "godot_view_renderer.h"
#import "keyboard_input_view.h"
#import "native_video_view.h"
#include "os_iphone.h"

@interface ViewController () <GodotViewDelegate>

@property(strong, nonatomic) GodotViewRenderer* renderer;
@property(strong, nonatomic) GodotNativeVideoView* videoView;
@property(strong, nonatomic) GodotKeyboardInputView* keyboardView;

@property(strong, nonatomic) UIView* godotLoadingOverlay;

@end

@implementation ViewController

- (GodotView*)godotView {
    return (GodotView*)self.view;
}

- (void)loadView {
    GodotView* view = [[GodotView alloc] init];
    [view initializeRendering];

    GodotViewRenderer* renderer = [[GodotViewRenderer alloc] init];

    self.renderer = renderer;
    self.view     = view;

    view.renderer = self.renderer;
    view.delegate = self;
}

- (instancetype)initWithNibName:(NSString*)nibNameOrNil
                         bundle:(NSBundle*)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];

    if (self) {
        [self godot_commonInit];
    }

    return self;
}

- (instancetype)initWithCoder:(NSCoder*)coder {
    self = [super initWithCoder:coder];

    if (self) {
        [self godot_commonInit];
    }

    return self;
}

- (void)godot_commonInit {
    // Initialize view controller values.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    printf("*********** did receive memory warning!\n");
};

- (void)viewDidLoad {
    [super viewDidLoad];

    [self observeKeyboard];
    [self displayLoadingOverlay];

    if (@available(iOS 11.0, *)) {
        [self setNeedsUpdateOfScreenEdgesDeferringSystemGestures];
    }
}

- (void)observeKeyboard {
    printf("******** setting up keyboard input view\n");
    self.keyboardView = [GodotKeyboardInputView new];
    [self.view addSubview:self.keyboardView];

    printf("******** adding observer for keyboard show/hide\n");
    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(keyboardOnScreen:)
               name:UIKeyboardDidShowNotification
             object:nil];
    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(keyboardHidden:)
               name:UIKeyboardDidHideNotification
             object:nil];
}

- (void)displayLoadingOverlay {
    NSBundle* bundle         = [NSBundle mainBundle];
    NSString* storyboardName = @"Launch Screen";

    if ([bundle pathForResource:storyboardName ofType:@"storyboardc"] == nil) {
        return;
    }

    UIStoryboard* launchStoryboard =
        [UIStoryboard storyboardWithName:storyboardName bundle:bundle];

    UIViewController* controller =
        [launchStoryboard instantiateInitialViewController];
    self.godotLoadingOverlay       = controller.view;
    self.godotLoadingOverlay.frame = self.view.bounds;
    self.godotLoadingOverlay.autoresizingMask =
        UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;

    [self.view addSubview:self.godotLoadingOverlay];
}

- (BOOL)godotViewFinishedSetup:(GodotView*)view {
    [self.godotLoadingOverlay removeFromSuperview];
    self.godotLoadingOverlay = nil;

    return YES;
}

- (void)dealloc {
    [self.videoView stopVideo];
    self.videoView = nil;

    self.keyboardView = nil;

    self.renderer = nil;

    if (self.godotLoadingOverlay) {
        [self.godotLoadingOverlay removeFromSuperview];
        self.godotLoadingOverlay = nil;
    }

    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

// MARK: Orientation

- (UIRectEdge)preferredScreenEdgesDeferringSystemGestures {
    return UIRectEdgeAll;
}

- (BOOL)shouldAutorotate {
    if (!OSIPhone::get_singleton()) {
        return NO;
    }

    switch (OS::get_singleton()->get_screen_orientation()) {
        case OS::SCREEN_SENSOR:
        case OS::SCREEN_SENSOR_LANDSCAPE:
        case OS::SCREEN_SENSOR_PORTRAIT:
            return YES;
        default:
            return NO;
    }
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
    if (!OSIPhone::get_singleton()) {
        return UIInterfaceOrientationMaskAll;
    }

    switch (OS::get_singleton()->get_screen_orientation()) {
        case OS::SCREEN_PORTRAIT:
            return UIInterfaceOrientationMaskPortrait;
        case OS::SCREEN_REVERSE_LANDSCAPE:
            return UIInterfaceOrientationMaskLandscapeRight;
        case OS::SCREEN_REVERSE_PORTRAIT:
            return UIInterfaceOrientationMaskPortraitUpsideDown;
        case OS::SCREEN_SENSOR_LANDSCAPE:
            return UIInterfaceOrientationMaskLandscape;
        case OS::SCREEN_SENSOR_PORTRAIT:
            return UIInterfaceOrientationMaskPortrait
                 | UIInterfaceOrientationMaskPortraitUpsideDown;
        case OS::SCREEN_SENSOR:
            return UIInterfaceOrientationMaskAll;
        case OS::SCREEN_LANDSCAPE:
            return UIInterfaceOrientationMaskLandscapeLeft;
    }
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (BOOL)prefersHomeIndicatorAutoHidden {
    if (GLOBAL_GET("display/window/ios/hide_home_indicator")) {
        return YES;
    } else {
        return NO;
    }
}

// MARK: Keyboard

- (void)keyboardOnScreen:(NSNotification*)notification {
    NSDictionary* info = notification.userInfo;
    NSValue* value     = info[UIKeyboardFrameEndUserInfoKey];

    CGRect rawFrame      = [value CGRectValue];
    CGRect keyboardFrame = [self.view convertRect:rawFrame fromView:nil];

    if (OSIPhone::get_singleton()) {
        OSIPhone::get_singleton()->set_virtual_keyboard_height(
            keyboardFrame.size.height
        );
    }
}

- (void)keyboardHidden:(NSNotification*)notification {
    if (OSIPhone::get_singleton()) {
        OSIPhone::get_singleton()->set_virtual_keyboard_height(0);
    }
}

// MARK: Native Video Player

- (BOOL)playVideoAtPath:(NSString*)filePath
                 volume:(float)videoVolume
                  audio:(NSString*)audioTrack
               subtitle:(NSString*)subtitleTrack {
    // If we are showing some video already, reuse existing view for new video.
    if (self.videoView) {
        return [self.videoView playVideoAtPath:filePath
                                        volume:videoVolume
                                         audio:audioTrack
                                      subtitle:subtitleTrack];
    } else {
        // Create autoresizing view for video playback.
        GodotNativeVideoView* videoView =
            [[GodotNativeVideoView alloc] initWithFrame:self.view.bounds];
        videoView.autoresizingMask =
            UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        [self.view addSubview:videoView];

        self.videoView = videoView;

        return [self.videoView playVideoAtPath:filePath
                                        volume:videoVolume
                                         audio:audioTrack
                                      subtitle:subtitleTrack];
    }
}

@end
