// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#import "keyboard_input_view.h"

#include "core/os/keyboard.h"
#include "os_iphone.h"

@interface GodotKeyboardInputView () <UITextViewDelegate>

@property(nonatomic, copy) NSString* previousText;
@property(nonatomic, assign) NSRange previousSelectedRange;

@end

@implementation GodotKeyboardInputView

- (instancetype)initWithCoder:(NSCoder*)coder {
    self = [super initWithCoder:coder];

    if (self) {
        [self godot_commonInit];
    }

    return self;
}

- (instancetype)initWithFrame:(CGRect)frame
                textContainer:(NSTextContainer*)textContainer {
    self = [super initWithFrame:frame textContainer:textContainer];

    if (self) {
        [self godot_commonInit];
    }

    return self;
}

- (void)godot_commonInit {
    self.hidden   = YES;
    self.delegate = self;

    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(observeTextChange:)
               name:UITextViewTextDidChangeNotification
             object:self];
}

- (void)dealloc {
    self.delegate = nil;
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

// MARK: Keyboard

- (BOOL)canBecomeFirstResponder {
    return YES;
}

- (BOOL)becomeFirstResponderWithString:(NSString*)existingString
                             multiline:(BOOL)flag
                           cursorStart:(NSInteger)start
                             cursorEnd:(NSInteger)end {
    self.text         = existingString;
    self.previousText = existingString;

    NSInteger safeStartIndex = MAX(start, 0);

    NSRange textRange;

    // Either a simple cursor or a selection.
    if (end > 0) {
        textRange = NSMakeRange(safeStartIndex, end - start);
    } else {
        textRange = NSMakeRange(safeStartIndex, 0);
    }

    self.selectedRange         = textRange;
    self.previousSelectedRange = textRange;

    return [self becomeFirstResponder];
}

- (BOOL)resignFirstResponder {
    self.text         = nil;
    self.previousText = nil;
    return [super resignFirstResponder];
}

// MARK: OS Messages

- (void)deleteText:(NSInteger)charactersToDelete {
    for (int i = 0; i < charactersToDelete; i++) {
        OSIPhone::get_singleton()->key(KEY_BACKSPACE, true);
        OSIPhone::get_singleton()->key(KEY_BACKSPACE, false);
    }
}

- (void)enterText:(NSString*)substring {
    String characters;
    characters.parse_utf8([substring UTF8String]);

    for (int i = 0; i < characters.size(); i++) {
        int character = characters[i];

        switch (character) {
            case 10:
                character = KEY_ENTER;
                break;
            case 8198:
                character = KEY_SPACE;
                break;
            default:
                break;
        }

        OSIPhone::get_singleton()->key(character, true);
        OSIPhone::get_singleton()->key(character, false);
    }
}

// MARK: Observer

- (void)observeTextChange:(NSNotification*)notification {
    if (notification.object != self) {
        return;
    }

    if (self.previousSelectedRange.length == 0) {
        // We are deleting all text before cursor if no range was selected.
        // This way any inserted or changed text will be updated.
        NSString* substringToDelete = [self.previousText
            substringToIndex:self.previousSelectedRange.location];
        [self deleteText:substringToDelete.length];
    } else {
        // If text was previously selected
        // we are sending only one `backspace`.
        // It will remove all text from text input.
        [self deleteText:1];
    }

    NSString* substringToEnter;

    if (self.selectedRange.length == 0) {
        // If previous cursor had a selection
        // we have to calculate an inserted text.
        if (self.previousSelectedRange.length != 0) {
            NSInteger rangeEnd =
                self.selectedRange.location + self.selectedRange.length;
            NSInteger rangeStart =
                MIN(self.previousSelectedRange.location,
                    self.selectedRange.location);
            NSInteger rangeLength = MAX(0, rangeEnd - rangeStart);

            NSRange calculatedRange;

            if (rangeLength >= 0) {
                calculatedRange = NSMakeRange(rangeStart, rangeLength);
            } else {
                calculatedRange = NSMakeRange(rangeStart, 0);
            }

            substringToEnter = [self.text substringWithRange:calculatedRange];
        } else {
            substringToEnter =
                [self.text substringToIndex:self.selectedRange.location];
        }
    } else {
        substringToEnter = [self.text substringWithRange:self.selectedRange];
    }

    [self enterText:substringToEnter];

    self.previousText          = self.text;
    self.previousSelectedRange = self.selectedRange;
}

@end
