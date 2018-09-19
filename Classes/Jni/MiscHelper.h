#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif
#include "Config.h"
#include "cocos2d.h"
#include "ZJH.h"

#ifndef MiscHelper
#define MiscHelper

#ifdef __cplusplus
extern "C" {
#endif
    
    void sdkDoForVoid(const char *message,const char *data);
    std::string sdkDoForStr(const char *message,const char *data);
    int sdkDoForInt(const char *message,const char *data);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JNIEXPORT void JNICALL define_native_function(PACKAGE_NAME, MiscHelper_nativeJava2C)(JNIEnv *, jobject, jstring,jstring);
#endif
    
#ifdef __cplusplus
}
#endif

#endif
