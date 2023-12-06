// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "ios.h"

#import "app_delegate.h"
#import "view_controller.h"

#import <UIKit/UIKit.h>
#include <sys/sysctl.h>

void iOS::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("get_rate_url", "app_id"),
        &iOS::get_rate_url
    );
};

void iOS::alert(const char* p_alert, const char* p_title) {
    NSString* title   = [NSString stringWithUTF8String:p_title];
    NSString* message = [NSString stringWithUTF8String:p_alert];

    UIAlertController* alert = [UIAlertController
        alertControllerWithTitle:title
                         message:message
                  preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction* button =
        [UIAlertAction actionWithTitle:@"OK"
                                 style:UIAlertActionStyleCancel
                               handler:^(id){}];

    [alert addAction:button];

    [AppDelegate.viewController presentViewController:alert
                                             animated:YES
                                           completion:nil];
}

String iOS::get_model() const {
    // [[UIDevice currentDevice] model] only returns "iPad" or "iPhone".
    size_t size;
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
    char* model = (char*)malloc(size);
    if (model == NULL) {
        return "";
    }
    sysctlbyname("hw.machine", model, &size, NULL, 0);
    NSString* platform = [NSString stringWithCString:model
                                            encoding:NSUTF8StringEncoding];
    free(model);
    const char* str = [platform UTF8String];
    return String::utf8(str != nullptr ? str : "");
}

String iOS::get_rate_url(int p_app_id) const {
    String app_url_path = "itms-apps://itunes.apple.com/app/idAPP_ID";

    String ret = app_url_path.replace("APP_ID", String::num(p_app_id));

    printf("returning rate url %ls\n", ret.c_str());

    return ret;
};

iOS::iOS(){};
