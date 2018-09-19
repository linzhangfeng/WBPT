#include "JsUtils.h"

#include "cocos2d.h"
//js
#include "scripting/js-bindings/auto/jsb_cocos2dx_3d_auto.hpp"
#include "scripting/js-bindings/auto/jsb_cocos2dx_3d_extension_auto.hpp"
#include "scripting/js-bindings/auto/jsb_cocos2dx_auto.hpp"
#include "scripting/js-bindings/auto/jsb_cocos2dx_builder_auto.hpp"
#include "scripting/js-bindings/auto/jsb_cocos2dx_extension_auto.hpp"
#include "scripting/js-bindings/auto/jsb_cocos2dx_navmesh_auto.hpp"
#include "scripting/js-bindings/auto/jsb_cocos2dx_physics3d_auto.hpp"
#include "scripting/js-bindings/auto/jsb_cocos2dx_spine_auto.hpp"
#include "scripting/js-bindings/auto/jsb_cocos2dx_studio_auto.hpp"
#include "scripting/js-bindings/auto/jsb_cocos2dx_ui_auto.hpp"
#include "scripting/js-bindings/manual/3d/jsb_cocos2dx_3d_manual.h"
#include "scripting/js-bindings/manual/chipmunk/js_bindings_chipmunk_registration.h"
#include "scripting/js-bindings/manual/cocosbuilder/js_bindings_ccbreader.h"
#include "scripting/js-bindings/manual/cocostudio/jsb_cocos2dx_studio_manual.h"
#include "scripting/js-bindings/manual/extension/jsb_cocos2dx_extension_manual.h"
#include "scripting/js-bindings/manual/jsb_opengl_registration.h"
#include "scripting/js-bindings/manual/localstorage/js_bindings_system_registration.h"
#include "scripting/js-bindings/manual/navmesh/jsb_cocos2dx_navmesh_manual.h"
#include "scripting/js-bindings/manual/network/XMLHTTPRequest.h"
#include "scripting/js-bindings/manual/network/jsb_socketio.h"
#include "scripting/js-bindings/manual/network/jsb_websocket.h"
#include "scripting/js-bindings/manual/physics3d/jsb_cocos2dx_physics3d_manual.h"
#include "scripting/js-bindings/manual/spine/jsb_cocos2dx_spine_manual.h"
#include "scripting/js-bindings/manual/ui/jsb_cocos2dx_ui_manual.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "scripting/js-bindings/auto/jsb_cocos2dx_experimental_video_auto.hpp"
#include "scripting/js-bindings/auto/jsb_cocos2dx_experimental_webView_auto.hpp"
#include "scripting/js-bindings/manual/experimental/jsb_cocos2dx_experimental_video_manual.h"
#include "scripting/js-bindings/manual/experimental/jsb_cocos2dx_experimental_webView_manual.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "scripting/js-bindings/auto/jsb_cocos2dx_audioengine_auto.hpp"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "cocos/scripting/js-bindings/manual/platform/android/CCJavascriptJavaBridge.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "cocos/scripting/js-bindings/manual/platform/ios/JavaScriptObjCBridge.h"
#endif
#include "../jsbind/js_wb_auto.hpp"

USING_NS_CC;
#include "base/base64.h"
#include "../Utils/Utils.h"
#include "../Model/ZJHModel.h"
#include "../Scene/Load/LoadTable.h"
#include "editor-support/cocostudio/CCArmatureDataManager.h"
JsUtils *jsUtils=nullptr;
JsUtils::JsUtils()
{
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(JsUtils::onChatEvent), "notify_room_chat", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(JsUtils::onChatStartEvent), "chat_sound_play_start_notify", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(JsUtils::onChatStopEvent), "chat_sound_play_over_notify", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(JsUtils::onIMEvent), "im_notify", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(JsUtils::onJSModuleEvent), "js_module_notify", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(JsUtils::onGameChangeEvent), "notify_game_count_change", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(JsUtils::onPowerChangeEvent), "notify_game_power_change", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(JsUtils::onImageNotify), "image_notify", NULL);
    m_jsError = EventListenerCustom::create("js_error", [=](EventCustom* event){

        std::string str =*(std::string*)event->getUserData();
       
//#if COCOS2D_DEBUG == 1
        CCLOG("js error: %s",str.c_str());
        return;
//#endif
        auto isBack = false;
        if(str.find("bad script XDR magic number")!= string::npos || str.find("too much recursion")!= string::npos || str.find("uncaught exception: Invalid parameter")!= string::npos)
        {
            if (str.find("bad script XDR magic number")!= string::npos)
            {
                FileUtils::getInstance()->removeDirectory(Utils::getSavePath()+ZJHModel::getInstance()->packName+"/");
                FileUtils::getInstance()->removeDirectory(Utils::getSavePath()+"public/");
            }

            isBack = true;
        }
        str =StringUtils::format("ErrorInfo:%s  UserID:%d  RoomID:%d PhoneName:%s PhoneOS:%s Ver:%s",str.c_str(),ZJHModel::getInstance()->uid
                                 ,ZJHModel::getInstance()->curRoomID,ZJHModel::getInstance()->phonename.c_str(),ZJHModel::getInstance()->phoneOs.c_str(),ZJHModel::getInstance()->vname.c_str());
        
        std::stringstream name;
        name.clear();
        name << ZJHModel::getInstance()->packName << ":" << ZJHModel::getInstance()->curGameType;

        if(isBack)
        {
            Director::getInstance()->getScheduler()->schedule([=](float){
                FileUtils::getInstance()->purgeCachedEntries();
                cocostudio::ArmatureDataManager::getInstance()->destroyInstance();
                SpriteFrameCache::getInstance()->removeSpriteFrames();
                Director::getInstance()->getTextureCache()->removeAllTextures();

                ZJHModel::getInstance()->roomToHall(true);
                PlatformHelper::showToast("游戏脚本发生错误,请重试！");
            }, this, 0, 0, 0.0, false, "js_error");

        }
        
        Json::Value ary;
        Json::Value dat;
      
        dat["httpCode"] = 100000;
        dat["codeDesc"] = name.str();
        dat["content"] = str;
        dat["produceTime"] = (int)(Utils::getCurrentTime()/1000-8 * 60 * 60 );
        dat["requestUrl"] = "";
        dat["requestParams"] = "";
        ary["logInfo"]="["+dat.toStyledString()+"]";
        CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                                 {
                                                     CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                     if (packet) {
                                                         if (getReferenceCount() == 0 || getReferenceCount() > 10)
                                                         {
                                                             //nothing
                                                         }
                                                         else
                                                         {
                                                             if (packet->status == 3) {
                                                                 packet->status = 0;
                                                                 if (packet->resultIsOK())
                                                                 {
                                                                     cocos2d::log("js_error up succ");
                                                                 }
                                                                 else
                                                                 {
                                                                 }
                                                                 
                                                             }
                                                         }
                                                         //删除packet
                                                         CCHttpAgent::getInstance()->packets.erase(tag);
                                                         delete packet;
                                                     }
                                                 },
                                                 "clientLog/upload",ary.toStyledString(), "js_error");

    });
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(m_jsError, 1);
}
JsUtils::~JsUtils()
{
    __NotificationCenter::getInstance()->removeObserver(this, "notify_room_chat");
     Director::getInstance()->getEventDispatcher()->removeEventListener(m_jsError);
}
JsUtils *JsUtils::getInstance()
{
    
    if (jsUtils == nullptr) {
        jsUtils = new JsUtils();
        jsUtils->retain();
    }
    
    return jsUtils;

}


std::string JsUtils::encodeBase64(const unsigned char *in, unsigned int inLength)
{
    char * encodedData;
    int len =  cocos2d::base64Encode((unsigned char*)in, inLength, &encodedData);

    return std::string(( char *)encodedData,len);
}
std::string JsUtils::decodeBase64(const std::string& dat)
{
    const char * codedData = dat.c_str();
    unsigned char * decodedData;
  int len =  cocos2d::base64Decode((unsigned char*)codedData, (unsigned int)strlen(codedData), &decodedData);
    
    return std::string(( char *)decodedData,len);
}

std::string JsUtils::decodeVideoData(const std::string& dat)
{
    const char * codedData = dat.c_str();
    unsigned char * decodedData;
    int len =  cocos2d::base64Decode((unsigned char*)codedData, (unsigned int)strlen(codedData), &decodedData);
        unsigned char * deflated;
    ssize_t deflatedLen = ZipUtils::inflateMemory(decodedData,len, &deflated);
    
    
    return std::string(( char *)deflated,deflatedLen);
}

void JsUtils::onChatEvent(Ref* arg)
{
    __String *str = dynamic_cast<__String *>(arg);
    const char * codedData = (*str).getCString();
    std::string msg =encodeBase64((unsigned char*)codedData, (unsigned int)strlen(codedData));
    ScriptingCore::getInstance()->evalString(StringUtils::format("cc.eventManager.dispatchCustomEvent(\"notify_room_chat1\",\"%s\");"
                                                                 ,msg.c_str()).c_str());
}

void JsUtils::onChatStartEvent(Ref* arg)
{
    __String *str = dynamic_cast<__String *>(arg);
    const char * codedData = (*str).getCString();
    std::string msg =encodeBase64((unsigned char*)codedData, (unsigned int)strlen(codedData));
    ScriptingCore::getInstance()->evalString(StringUtils::format("cc.eventManager.dispatchCustomEvent(\"chat_sound_play_start_notify1\",\"%s\");"
                                                                 ,msg.c_str()).c_str());
}

void JsUtils::onChatStopEvent(Ref* arg)
{
    __String *str = dynamic_cast<__String *>(arg);
    const char * codedData = (*str).getCString();
    std::string msg =encodeBase64((unsigned char*)codedData, (unsigned int)strlen(codedData));
    ScriptingCore::getInstance()->evalString(StringUtils::format("cc.eventManager.dispatchCustomEvent(\"chat_sound_play_over_notify1\",\"%s\");"
                                                                 ,msg.c_str()).c_str());
}

void JsUtils::onIMEvent(Ref* arg)
{
    __String *str = dynamic_cast<__String *>(arg);
    const char * codedData = (*str).getCString();
    std::string msg =encodeBase64((unsigned char*)codedData, (unsigned int)strlen(codedData));
    ScriptingCore::getInstance()->evalString(StringUtils::format("cc.eventManager.dispatchCustomEvent(\"im_notify1\",\"%s\");"
                                                                 ,msg.c_str()).c_str());
}

void JsUtils::onJSModuleEvent(Ref* arg)
{
    __String *str = dynamic_cast<__String *>(arg);
    const char * codedData = (*str).getCString();
    std::string msg =encodeBase64((unsigned char*)codedData, (unsigned int)strlen(codedData));
    ScriptingCore::getInstance()->evalString(StringUtils::format("cc.eventManager.dispatchCustomEvent(\"js_module_notify1\",\"%s\");"
                                                                 ,msg.c_str()).c_str());
}

void JsUtils::onGameChangeEvent(Ref* arg)
{
    __String *str = dynamic_cast<__String *>(arg);
    const char * codedData = (*str).getCString();
    std::string msg =encodeBase64((unsigned char*)codedData, (unsigned int)strlen(codedData));
    ScriptingCore::getInstance()->evalString(StringUtils::format("cc.eventManager.dispatchCustomEvent(\"notify_game_count_change1\",\"%s\");"
                                                                 ,msg.c_str()).c_str());
}

void JsUtils::onPowerChangeEvent(cocos2d::Ref *arg){
    __String *str = dynamic_cast<__String *>(arg);
    const char * codedData = (*str).getCString();
    std::string msg =encodeBase64((unsigned char*)codedData, (unsigned int)strlen(codedData));
    ScriptingCore::getInstance()->evalString(StringUtils::format("cc.eventManager.dispatchCustomEvent(\"notify_power_change1\",\"%s\");"
                                                                 ,msg.c_str()).c_str());
}

void JsUtils::onImageNotify(cocos2d::Ref *arg){
    __String *str = dynamic_cast<__String *>(arg);
    const char * codedData = (*str).getCString();
    std::string msg =encodeBase64((unsigned char*)codedData, (unsigned int)strlen(codedData));
    ScriptingCore::getInstance()->evalString(StringUtils::format("cc.eventManager.dispatchCustomEvent(\"image_notify1\",\"%s\");"
                                                                 ,msg.c_str()).c_str());
}

//发送事件
void JsUtils::postNotifi(const std::string& key)
{
    if (key == "save_setting"){
        UserDefault::getInstance()->setDoubleForKey("sound", ZJHModel::getInstance()->sound);
        UserDefault::getInstance()->setDoubleForKey("bgMusic", ZJHModel::getInstance()->bgMusic);
        UserDefault::getInstance()->setBoolForKey("vibrate", ZJHModel::getInstance()->vibrate);
        UserDefault::getInstance()->flush();
        return;
    }
    __NotificationCenter::getInstance()->postNotification(key);
}
void JsUtils::postNotifiInt(const std::string& key,int d)
{
    __NotificationCenter::getInstance()->postNotification(key,__Integer::create(d));
}
void JsUtils::postNotifiStr(const std::string& key,const std::string& d)
{
    __NotificationCenter::getInstance()->postNotification(key,__String::create(d));
}


//清空js虚拟机
void JsUtils::resetJsVM()
{
    ScriptingCore::getInstance()->cleanup();
    ScriptingCore::getInstance()->initRegister();
    
}

void JsUtils::runScript(const std::string& file)
{
    ScriptingCore::getInstance()->runScript(file.c_str());
}
void JsUtils::initLoad()
{
    auto b1 =Utils::getCurrentTime();
    //js
    resetJsVM();
    
    ScriptingCore* sc = ScriptingCore::getInstance();
    sc->addRegisterCallback(register_all_cocos2dx);
    sc->addRegisterCallback(register_cocos2dx_js_core);
    sc->addRegisterCallback(jsb_register_system);
    
    
    // extension can be commented out to reduce the package
    sc->addRegisterCallback(register_all_cocos2dx_extension);
    sc->addRegisterCallback(register_all_cocos2dx_extension_manual);
    
    // chipmunk can be commented out to reduce the package
    sc->addRegisterCallback(jsb_register_chipmunk);
    // opengl can be commented out to reduce the package
    sc->addRegisterCallback(JSB_register_opengl);
    
    // builder can be commented out to reduce the package
    sc->addRegisterCallback(register_all_cocos2dx_builder);
    sc->addRegisterCallback(register_CCBuilderReader);
    
    // ui can be commented out to reduce the package, attension studio need ui module
    sc->addRegisterCallback(register_all_cocos2dx_ui);
    sc->addRegisterCallback(register_all_cocos2dx_ui_manual);
    
    // studio can be commented out to reduce the package,
    sc->addRegisterCallback(register_all_cocos2dx_studio);
    sc->addRegisterCallback(register_all_cocos2dx_studio_manual);
    
    // spine can be commented out to reduce the package
    sc->addRegisterCallback(register_all_cocos2dx_spine);
    sc->addRegisterCallback(register_all_cocos2dx_spine_manual);
    
    // XmlHttpRequest can be commented out to reduce the package
    sc->addRegisterCallback(MinXmlHttpRequest::_js_register);
    // websocket can be commented out to reduce the package
    sc->addRegisterCallback(register_jsb_websocket);
    // sokcet io can be commented out to reduce the package
    sc->addRegisterCallback(register_jsb_socketio);
    
    // 3d can be commented out to reduce the package
    sc->addRegisterCallback(register_all_cocos2dx_3d);
    sc->addRegisterCallback(register_all_cocos2dx_3d_manual);
    
    // 3d extension can be commented out to reduce the package
    sc->addRegisterCallback(register_all_cocos2dx_3d_extension);
    
#if CC_USE_3D_PHYSICS && CC_ENABLE_BULLET_INTEGRATION
    // Physics 3d can be commented out to reduce the package
    sc->addRegisterCallback(register_all_cocos2dx_physics3d);
    sc->addRegisterCallback(register_all_cocos2dx_physics3d_manual);
#endif
    
#if CC_USE_NAVMESH
    sc->addRegisterCallback(register_all_cocos2dx_navmesh);
    sc->addRegisterCallback(register_all_cocos2dx_navmesh_manual);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    sc->addRegisterCallback(register_all_cocos2dx_experimental_video);
    sc->addRegisterCallback(register_all_cocos2dx_experimental_video_manual);
    sc->addRegisterCallback(register_all_cocos2dx_experimental_webView);
    sc->addRegisterCallback(register_all_cocos2dx_experimental_webView_manual);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    sc->addRegisterCallback(register_all_cocos2dx_audioengine);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sc->addRegisterCallback(JavascriptJavaBridge::_js_register);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    sc->addRegisterCallback(JavaScriptObjCBridge::_js_register);
#endif
    
    //for wb
    
    sc->addRegisterCallback(register_all_wb);
    
    CCLOG("t1:%ld",Utils::getCurrentTime()-b1);
    b1 =Utils::getCurrentTime();

    sc->start();
    CCLOG("t2:%ld",Utils::getCurrentTime()-b1);
    b1 =Utils::getCurrentTime();

    sc->runScript("script/jsb_boot.js");
    CCLOG("t3:%ld",Utils::getCurrentTime()-b1);
    b1 =Utils::getCurrentTime();

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
   sc->enableDebugger();
#endif
    //sc->enableDebugger();

    ScriptEngineProtocol *engine = ScriptingCore::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
    //  ScriptingCore::getInstance()->runScript("main.js");
    //js end
    CCLOG("t4:%ld",Utils::getCurrentTime()-b1);
    b1 =Utils::getCurrentTime();


}

