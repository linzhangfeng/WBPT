//
//  IOSToast.cpp
//  ZJH
//
//  Created by hyhy on 7/15/14.
//
//

#include "IOSToast.h"
#include "iToast.h"

void IOSToast::showToast(const char *str, ToastDuration duration, ToastPos pos)
{
    iToast *toast = [iToast makeText: [NSString stringWithUTF8String: str]];
    iToastSettings *setting = [toast theSettings];
    if (pos == ToastPosTop) {
        setting.gravity = iToastGravityTop;
    } else if (pos == ToastPosBottom) {
        setting.gravity = iToastGravityBottom;
    } else if (pos == ToastPosCenter) {
        setting.gravity = iToastGravityCenter;
    }
    
    if (duration == ToastDurationLong) {
        setting.duration = iToastDurationLong;
    } else if (duration == ToastDurationShort) {
        setting.duration = iToastDurationShort;
    } else if (duration == ToastDurationNormal) {
        setting.duration = iToastDurationNormal;
    }
    
    [toast show];
}