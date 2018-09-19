#ifndef __ZJH__IOSToast__
#define __ZJH__IOSToast__

#include "cocos2d.h"

class IOSToast {
public:
    typedef enum {
        ToastPosTop,
        ToastPosBottom,
        ToastPosCenter
    } ToastPos;
    
    typedef enum {
        ToastDurationLong,
        ToastDurationShort,
        ToastDurationNormal
    } ToastDuration;
    static void showToast(const char *str, ToastDuration duration = ToastDurationNormal, ToastPos pos = ToastPosBottom);
    
};

#endif /* defined(__ZJH__IOSToast__) */
