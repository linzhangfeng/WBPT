#pragma once

#include "cocos2d.h"

class JsUtils: public cocos2d::Ref
{
public:
    JsUtils();
    virtual ~JsUtils();
    static  JsUtils *getInstance();
    cocos2d::EventListenerCustom* m_jsError;
    
    void onChatEvent(Ref* arg);
    void onChatStartEvent(Ref* arg);
    void onChatStopEvent(Ref* arg);
    void onIMEvent(Ref* arg);
    void onGameChangeEvent(Ref* arg);
    void onPowerChangeEvent(Ref* arg);
    void onJSModuleEvent(Ref* arg);
    void onImageNotify(Ref* arg);

    static std::string encodeBase64(const unsigned char *in, unsigned int inLength);
    static std::string decodeBase64(const std::string& dat);
    
     static std::string decodeVideoData(const std::string& dat);
    //重新加载js代码
    static void initLoad();
    
    //清空js虚拟机
    static void resetJsVM();
    
    //执行js脚本
    static void runScript(const std::string& file);    
    //发送事件
    static void postNotifi(const std::string& key);
    static void postNotifiInt(const std::string& key,int d);
    static void postNotifiStr(const std::string& key,const std::string& d);
};
