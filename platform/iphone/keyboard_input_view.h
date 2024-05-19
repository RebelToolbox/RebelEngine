// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#import <UIKit/UIKit.h>

@interface RebelKeyboardInputView : UITextView

- (BOOL)becomeFirstResponderWithString:(NSString*)existingString
                             multiline:(BOOL)flag
                           cursorStart:(NSInteger)start
                             cursorEnd:(NSInteger)end;

@end
