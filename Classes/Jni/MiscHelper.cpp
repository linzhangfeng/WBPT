
#include "cocos2d.h"
#include "MiscHelper.h"
#include "Config.h"
#include "ZJH.h"

using namespace cocos2d;

extern "C"
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void sdkDoForVoid(const char *str,const char *data)
    {
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t, (string(CLASS_NAME)+string("MiscHelper")).c_str(), "sdkDoForVoid", "(Ljava/lang/String;Ljava/lang/String;)V"))
        {
            jstring jarg0 =  t.env->NewStringUTF(str);
            jstring jarg1 =  t.env->NewStringUTF(data);
            t.env->CallStaticVoidMethod(t.classID, t.methodID, jarg0, jarg1);
            t.env->DeleteLocalRef(jarg0);
            t.env->DeleteLocalRef(jarg1);
            t.env->DeleteLocalRef(t.classID);
        }
    }
    
    std::string sdkDoForStr(const char *str,const char *data)
    {
        std::string ret;
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t, (string(CLASS_NAME)+string("MiscHelper")).c_str(), "sdkDoForStr", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"))
        {
            jstring jarg0 =  t.env->NewStringUTF(str);
            jstring jarg1 =  t.env->NewStringUTF(data);
            jstring retFromJava = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID, jarg0, jarg1);
            const char* rstr = t.env->GetStringUTFChars(retFromJava, 0);
            ret = rstr;
            t.env->ReleaseStringUTFChars(retFromJava, rstr);
            t.env->DeleteLocalRef(jarg0);
            t.env->DeleteLocalRef(jarg1);
            t.env->DeleteLocalRef(t.classID);
        }
        return ret;
    }
    
    int sdkDoForInt(const char *str,const char *data)
    {
        int ret;
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t, (string(CLASS_NAME)+string("MiscHelper")).c_str(), "sdkDoForInt", "(Ljava/lang/String;Ljava/lang/String;)I"))
        {
            jstring jarg0 =  t.env->NewStringUTF(str);
            jstring jarg1 =  t.env->NewStringUTF(data);
            ret = t.env->CallStaticIntMethod(t.classID, t.methodID, jarg0, jarg1);
            t.env->DeleteLocalRef(jarg0);
            t.env->DeleteLocalRef(jarg1);
            t.env->DeleteLocalRef(t.classID);
        }
        
        return ret;
    }
    
    JNIEXPORT void JNICALL define_native_function(PACKAGE_NAME, MiscHelper_nativeJava2C)
    (JNIEnv *env, jobject thiz,jstring jstr_cmd,jstring jstr_data)
    {
        const char* str_cmd = env->GetStringUTFChars(jstr_cmd, 0);
        std::string ret_cmd = str_cmd;
        env->ReleaseStringUTFChars(jstr_cmd, str_cmd);
        
        const char* str_data = env->GetStringUTFChars(jstr_data, 0);
        std::string ret_data = str_data;
        env->ReleaseStringUTFChars(jstr_data, str_data);
        
//        __NotificationCenter::getInstance()->postNotification(ret_cmd,__String::create(ret_data));
        MyNotification::getInstance()->postNotify(ret_cmd,__String::create(ret_data));
    }
#endif
}