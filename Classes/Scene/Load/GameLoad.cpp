//
//  GameLoad.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include <sstream>
#include "GameLoad.hpp"
#include "../Hall/HallScene.hpp"
#include "../GameHall/GameHallScene.hpp"
#include "../GameHall/SelectGameHall.hpp"
#include "HelloWorldScene.h"
#include "../../Model/LabaModel.h"
#include "../../Model/MsgModel.h"
#include "TipLoadLayer.hpp"
#include "../Mine/LoadLayer.hpp"
#include "../../Utils/PingCommand.hpp"
#include "ServerErrorTips.hpp"

#include "extensions/cocos-ext.h"

USING_NS_CC_EXT;

Scene* GameLoad::createScene()
{
    auto scene = Scene::create();
    auto layer = GameLoad::create();
    scene->addChild(layer);
    return scene;
}

bool GameLoad::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    ZJHModel::getInstance()->diquGameList.clear();
    
    //界面进入时间
    intoTime = Utils::getCurrentSecend()/1000;
    
    ZJHModel::getInstance()->bgMusic = UserDefault::getInstance()->getDoubleForKey("bgMusic", 0.1f);
    ZJHModel::getInstance()->sound = UserDefault::getInstance()->getDoubleForKey("sound", 0.8f);
    
    ZJHModel::getInstance()->soundOn = UserDefault::getInstance()->getIntegerForKey("soundon", 1);
    ZJHModel::getInstance()->musicOn = UserDefault::getInstance()->getIntegerForKey("musicon", 1);
    ZJHModel::getInstance()->vibrate = UserDefault::getInstance()->getIntegerForKey("vibrate", 1);
    
    std::string startimg = UserDefault::getInstance()->getStringForKey("startImageUrl","");
    
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(ZJHModel::getInstance()->bgMusic);
    SimpleAudioEngine::getInstance()->setEffectsVolume(ZJHModel::getInstance()->sound);
    
    Size size = Director::getInstance()->getWinSize();
    
    if (startimg != "" && Utils::hasImageFile(startimg))
    {
        Sprite* bg = Sprite::create(Utils::getImageSavePath(startimg));
        float sx = size.width / bg->getContentSize().width;
        float sy = size.height / bg->getContentSize().height;
        bg->setScale(MAX(sx,sy));
        bg->setAnchorPoint(Vec2(0.5, 1));
        bg->setPosition(Vec2(size.width / 2, size.height));
        addChild(bg);
    }
    else
    {
        Sprite* bg = Sprite::create("Load/loading_page.jpg");
        float sx = size.width / bg->getContentSize().width;
        float sy = size.height / bg->getContentSize().height;
        bg->setScale(MAX(sx,sy));
        bg->setAnchorPoint(Vec2(0.5, 1));
        bg->setPosition(Vec2(size.width / 2, size.height));
        addChild(bg);
    }
    
    Sprite* logo = Sprite::create("Load/loading_page_logo.png");
    logo->setAnchorPoint(Vec2(0.5, 0));
    logo->setPosition(size.width / 2, 0);
    logo->setScale(Utils::getMaxScale());
    addChild(logo);
   
    Sprite* guojia = Sprite::create("Load/guojia_load.png");
    guojia->setAnchorPoint(Vec2(0.5, 0));
    guojia->setPosition(size.width / 2, 25);
    guojia->setScale(Utils::getMaxScale());
    addChild(guojia);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(GameLoad::loadConnect), "re_connect_GameLoad", NULL);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(GameLoad::loadConnect), "notForceUpdateNext", NULL);
    
    
    //调用登录
    this->schedule(schedule_selector(GameLoad::threePostLogin), 0, false, 0.1);
    //判断缓存的音频文件是否存在
    string savePath = Utils::getTmpFoldPath()+UserDefault::getInstance()->getStringForKey("boot_music");
    bool hasFile = Utils::hasImageFile(savePath);
    if(hasFile)
    {
        Sound::getInstance()->playEffect(savePath.c_str());
    }else{
        Sound::getInstance()->playEffect("Sound/boot_music.mp3");
    }

    
    return true;
}

void GameLoad::dealAppUpdateIos()
{
    //处理ios平台的升级
    if (getReferenceCount() == 0 || getReferenceCount() > 10)
    {
        //登录请求
        loadConnect(NULL);
        return;
    }
    CCHttpPacket *checkUpdatePacket = CCHttpAgent::getInstance()->packets["check_updateIOS"];
    if (checkUpdatePacket && checkUpdatePacket->status != 0)
    {
        if (checkUpdatePacket->status != 3)
        {
            //网络出错
            loadConnect(NULL);
            return;
        }
        
        checkUpdatePacket->status = 0;
        if (checkUpdatePacket->resultIsOK())
        {
            
            bool result = checkUpdatePacket->recvVal["resultMap"].isNull();
            if(result){
                loadConnect(NULL);
                return;
            }
            int isForce = checkUpdatePacket->recvVal["resultMap"]["version"]["force"].asInt();
            int curCode = atoi(checkUpdatePacket->recvVal["resultMap"]["version"]["code"].asString().c_str());
            if (curCode > ZJHModel::getInstance()->vcode)
            {
                //提示升级需要增加限制,一天提示一次
                if (isForce == 0)
                {
                    PlatformHelper::showUpdateDialog(checkUpdatePacket->recvVal["resultMap"]["version"]);
                }
                else
                {
                    //继续游戏  2、要升级不强制
                    double lastTime = UserDefault::getInstance()->getDoubleForKey("update_tips",0.0);
                    long long currentTime = Utils::getCurrentSecend();
                    double interval = (currentTime-lastTime)*0.001/60/60;
                    
                    log("＝＝＝ lastTime[%f]==currentTime[%lld]==interval[%f]",lastTime,currentTime, interval);
                    if (interval > 24)
                    {
                        UserDefault::getInstance()->setDoubleForKey("update_tips", currentTime);
                        PlatformHelper::showUpdateDialog(checkUpdatePacket->recvVal["resultMap"]["version"]);
                    }else{
                    
                        //登录请求
                        loadConnect(NULL);
                    }
                }
            }else
            {
                //登录请求
                loadConnect(NULL);
                
            }
        }else if(checkUpdatePacket->recvVal["status"].asInt() == 999)
        {
            //tomcat没有连上
            unschedule(schedule_selector(GameLoad::loading));
            string content = checkUpdatePacket->recvVal["message"].asString();
            ServerErrorTips* tips = ServerErrorTips::create();
            tips->setContent(content);
            addChild(tips,1000);
        }else{
        
            //登录请求
            loadConnect(NULL);
        }
        
        CCHttpAgent::getInstance()->packets["check_updateIOS"] = NULL;
        delete checkUpdatePacket;
    }
    
}

void GameLoad::threePostLogin(float dt)
{
    PlatformHelper::sdkDoForVoid_C("initSDK");
    ZJHModel::getInstance()->initPhoneData();
    unschedule(schedule_selector(GameLoad::threePostLogin));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        dealAppUpdateIos();
    },"version/clientGameVersion", "","check_updateIOS");
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        ZJHModel::getInstance()->dealAppUpdate();
    },"version/clientGameVersion", "","check_update");
    
    //登录请求
    loadConnect(NULL);
    
#endif
}

void GameLoad::loadConnect(Ref*ref)
{
    this->schedule(schedule_selector(GameLoad::loading), 0.0f);
    setState(State_login);
}

//微信登陆3天重新登录
void GameLoad::WXLoadThreeDaysReLoad()
{
    double lastTime = UserDefault::getInstance()->getDoubleForKey("WXThreeDay_ReLoad", 0.0);
    long long currentTime = Utils::getCurrentSecend();
    double interval = (currentTime-lastTime)*0.001/60/60;
    
    if (interval > 24*3)
    {
        UserDefault::getInstance()->setDoubleForKey("WXThreeDay_ReLoad", 0.0);
        
        /******************* 存储还原 start(相当于退出帐号)********************/
        UserDefault::getInstance()->setIntegerForKey("thirdtype", -1);
        UserDefault::getInstance()->setStringForKey("wx_login_openid", "");
        
        ZJHModel::getInstance()->setPhoneAndPwd("", "");
        
        ZJHModel::getInstance()->UserKey = "";
        ZJHModel::getInstance()->uid = 0;
        
        ZJHModel::getInstance()->nick = "";
        ZJHModel::getInstance()->portrit = "";
        ZJHModel::getInstance()->setUserType(0);
        ZJHModel::getInstance()->pwdOK = 0;
        
        MsgModel::getInstance()->ip = "";
        MsgModel::getInstance()->port = 0;
        
        ZJHModel::getInstance()->setUserType(6);
        
        __NotificationCenter::getInstance()->postNotification("account_logout");
        
        // 登录天数
        UserDefault::getInstance()->setIntegerForKey("days", 0);
        
        if (PlatformHelper::sdkDoForInt_C("hasThirdChannel"))
        {
            ZJHModel::getInstance()->UserKey = "";
            PlatformHelper::sdkDoForVoid_C("logout");
        }
        /******************* 存储还原 end (相当于退出帐号)********************/
        
        //重新进入登录界面
        auto scene = Scene::create();
        Director::getInstance()->replaceScene(scene);
        
        LoadLayer *load = LoadLayer::create();
        scene->addChild(load, 100);
        ZJHModel::getInstance()->curView = load;
        ZJHModel::getInstance()->isGameLoad = true;
        
    }
    
}

void GameLoad::setState(int mstate)
{
    state = mstate;
    if (state == State_login)
    {
        
        if (ZJHModel::getInstance()->UserKey != "")
        {
            setState(State_GameConfig);
            return;
        }
        string phone = ZJHModel::getInstance()->phone;
        string password = ZJHModel::getInstance()->pwd;
        
        if (PlatformHelper::sdkDoForInt_C("hasThirdChannel"))
        {
            Json::Value json;
            json["userFrom"] = 1;
            json["platFrom"] = 1;
            json["loginType"] = 3;
            
            json["thirdId"] = PlatformHelper::sdkDoForStr_C("thirdID");
            json["image"] = PlatformHelper::sdkDoForStr_C("thirdUserHead");
            json["nickName"] = PlatformHelper::sdkDoForStr_C("thirdUserName");
            json["thirdType"] = PlatformHelper::sdkDoForInt_C("thirdAccountType");
            
            CCHttpAgent::getInstance()->sendHttpPost(NULL,"user/thirdLogin",json.toStyledString(),"login");
        }
        else if ((int)UserDefault::getInstance()->getIntegerForKey("thirdtype") == 3)
        {
            //微信登陆3天后重新登录处理
            WXLoadThreeDaysReLoad();
            
            Json::Value json;
            json["platFrom"] = 1;
            json["thirdType"] = 3; //微信登陆类型
            json["loginType"] = 3;
            json["userFrom"] = 1;
            json["thirdId"] = UserDefault::getInstance()->getStringForKey("wx_login_openid");
            CCHttpAgent::getInstance()->sendHttpPost(NULL,"user/thirdLogin",json.toStyledString(),"login");
            
        }else
        {
            Json::Value json_s;
            json_s["phone"] = phone;
            json_s["loginType"] = 1;  //0:unknown 1:normal 2:fast 3:third
            json_s["password"] = password;
            
            json_s["platFrom"] = 1; //1:app 2:wap 网页
            json_s["userFrom"] = 1; //1:app 2:wap 网页
            
            //登录请求
            CCHttpAgent::getInstance()->sendHttpPost(NULL,"user/login", json_s.toStyledString(),"login");
        }
    }
    else if (state == State_GameConfig)
    {
        Json::Value json;
        
        //load game config
        CCHttpAgent::getInstance()->sendHttpPost(NULL,"config/configList","","gameconfig");
        
        //pre load hall init data   //game/index.action  之前大厅的接口已经不用了
       // CCHttpAgent::getInstance()->sendHttpPost(NULL,"game/hall","","hallinit");
        
        json.clear();
        double time = UserDefault::getInstance()->getDoubleForKey("noticetime");
        if(time == 0){
            json["time"] = "0";
        }else{
            string m_time;
            stringstream ss;
            ss<<time;
            m_time = ss.str();
            ss.clear();
            json["time"] = m_time;
        }
        
        //pre load notice data
//        CCHttpAgent::getInstance()->sendHttpPost(NULL,"notice/snotice",json.toStyledString(),"notice");
        
        json.clear();
        json["createTime"] = (int)Utils::getCurrentTime();
        
        //pre load msg data 之前用的接口已经不用了
        //CCHttpAgent::getInstance()->sendHttpPost(NULL,"msgTask/msgData",json.toStyledString(),"msgdata");
        
        ZJHModel::getInstance()->checkUploadMoblieContacts();
    }
    else if (state == State_loadres)
    {
        
    }
    else if (state == State_loadover)
    {
        
    }
    else if (state == State_error)
    {
        
        if(this->loginRetryTimes == 3){
            this->loginRetryTimes = 0;
            PlatformHelper::showToast("网络异常，请重试!");
            TipLoadLayer *tip = TipLoadLayer::create(1);
            this->addChild(tip, 100);
        }else{
            log("=======登陆失败重试第%d次=======%s",this->loginRetryTimes,CCHttpAgent::getInstance()->url.c_str());
            this->loginRetryTimes++;
            loadConnect(NULL);
        }
        
    }
    else if (state == State_checkUpdate)
    {
//        CCHttpAgent::getInstance()->sendHttpPost(NULL,"version/clientGameVersion", "","check_update");
    }
}

void GameLoad::loading(float dt)
{
    if (state == State_login)
    {
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["login"];
        if (loginPacket && loginPacket->status != 0)
        {
            if (loginPacket->status != 3)
            {
                unschedule(schedule_selector(GameLoad::loading));
                CCHttpAgent::getInstance()->packets["login"] = NULL;
                delete loginPacket;
                setState(State_error);
                return;
            }
            
            loginPacket->status = 0;
            
            if (loginPacket->resultIsOK())
            {
                //在json解析失败时要重新切换域名登录
                bool result = loginPacket->recvVal["resultMap"].isNull();
                if(result){
                    //登录接口特殊处理一下 接收数据有问题时手动变更一次url
                    unschedule(schedule_selector(GameLoad::loading));
                    CCHttpAgent::getInstance()->packets["login"] = NULL;
                    delete loginPacket;
                    CCHttpAgent::getInstance()->checkChangeURL(CCHttpAgent::getInstance()->url);
                    setState(State_error);
                    return;
                }
                //注册完成记录本地存储账号的信息
                Json::Value data = loginPacket->recvVal["resultMap"];
                ZJHModel::getInstance()->UserKey = data["userKey"].asString();
                ZJHModel::getInstance()->uid = data["id"].asInt();
                ZJHModel::getInstance()->vipLv = data["vipLv"].asInt();
                ZJHModel::getInstance()->nick = data["nickName"].asString();
                ZJHModel::getInstance()->userType = data["userType"].asInt();
                ZJHModel::getInstance()->portrit = data["thumbImage"].asString();
                ZJHModel::getInstance()->name = data["nickName"].asString();
                ZJHModel::getInstance()->pwdOK = data["pwdOK"].asBool();
                
                MsgModel::getInstance()->ip = data["im"]["ipAddress"].asString();
                MsgModel::getInstance()->port = data["im"]["port"].asInt();
                
                setState(State_GameConfig);
                
                
                if (PlatformHelper::sdkDoForInt_C("hasThirdChannel")) {
                    UserDefault::getInstance()->setIntegerForKey("thirdtype", PlatformHelper::sdkDoForInt_C("thirdAccountType"));
                }
                //#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                //                setState(State_GameConfig);
                //#else
                //                setState(State_checkUpdate);
                //#endif
                Utils::uploadGameCommandRecord();
            }else if(loginPacket->recvVal["status"].asInt() == 999)
            {
                //tomcat没有连上
                unschedule(schedule_selector(GameLoad::loading));
                string content = loginPacket->recvVal["message"].asString();
                ServerErrorTips* tips = ServerErrorTips::create();
                tips->setContent(content);
                addChild(tips,1000);
            }else if(loginPacket->recvVal["status"].asInt() == 97)
            {
                
                //账号被封（存储还原）
                UserDefault::getInstance()->setIntegerForKey("thirdtype", -1);
                UserDefault::getInstance()->setStringForKey("wx_login_openid", "");
                
                ZJHModel::getInstance()->setPhoneAndPwd("", "");
                
                ZJHModel::getInstance()->UserKey = "";
                ZJHModel::getInstance()->uid = 0;
                
                ZJHModel::getInstance()->nick = "";
                ZJHModel::getInstance()->portrit = "";
                ZJHModel::getInstance()->setUserType(0);
                ZJHModel::getInstance()->pwdOK = false;
                
                MsgModel::getInstance()->ip = "";
                MsgModel::getInstance()->port = 0;
                
                ZJHModel::getInstance()->setUserType(6);
                
                __NotificationCenter::getInstance()->postNotification("account_logout");
                // 登录天数
                UserDefault::getInstance()->setIntegerForKey("days", 0);
                
                if (PlatformHelper::sdkDoForInt_C("hasThirdChannel"))
                {
                    ZJHModel::getInstance()->UserKey = "";
                    PlatformHelper::sdkDoForVoid_C("logout");
                }else
                {
                    //进入登陆界面
                    auto scene = Scene::create();
                    Director::getInstance()->replaceScene(scene);
                    
                    LoadLayer *load = LoadLayer::create();
                    scene->addChild(load, 100);
                    ZJHModel::getInstance()->curView = load;
                    ZJHModel::getInstance()->isGameLoad = true;
                }
                
            }
            else
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                unschedule(schedule_selector(GameLoad::loading));
                
                if (PlatformHelper::sdkDoForInt_C("hasThirdChannel"))
                {
                    
                }
                else
                {
                    endTime = Utils::getCurrentSecend()/1000;
                    CallFunc* func1 = CallFunc::create([=]
                                                       {
                                                           auto scene = Scene::create();
                                                           Director::getInstance()->replaceScene(scene);
                                                           
                                                           LoadLayer *load = LoadLayer::create();
                                                           scene->addChild(load, 100);
                                                           ZJHModel::getInstance()->curView = load;
                                                           ZJHModel::getInstance()->isGameLoad = true;
                                                           
                                                       });
                    
                    //控制延迟的时间
                    long long delayTime = (endTime-intoTime) >= 3? 0: 3-(endTime-intoTime);
                    printf("111111111111 = %lld\nend = %lld\nint = %lld", delayTime, endTime, intoTime);
                    runAction(Sequence::create(DelayTime::create(delayTime),func1, NULL));
                }
            }
            
            CCHttpAgent::getInstance()->packets["login"] = NULL;
            delete loginPacket;
        }
    }
    else if (state == State_checkUpdate)
    {
//        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["check_update"];
//        if (loginPacket && loginPacket->status != 0)
//        {
//            if (loginPacket->status != 3)
//            {
//                setState(State_error);
//                return;
//            }
//            
//            loginPacket->status = 0;
//            
//            if (loginPacket->resultIsOK())
//            {
//                int isForce = loginPacket->recvVal["resultMap"]["version"]["force"].asInt();
//                int curCode = atoi(loginPacket->recvVal["resultMap"]["version"]["code"].asCString());
//                if (curCode > ZJHModel::getInstance()->vcode)
//                {
//                    //提示升级需要增加限制,一天提示一次
//                    if (isForce == 0)
//                    {
//                        PlatformHelper::showUpdateDialog(loginPacket->recvVal["resultMap"]["version"]);
//                    }
//                    else
//                    {
//                        double lastTime = UserDefault::getInstance()->getDoubleForKey("update_tips",0.0);
//                        double currentTime = Utils::getCurrentTime();
//                        double interval = (currentTime-lastTime)*0.001/60/60;
//                        if (interval > 24)
//                        {
//                            UserDefault::getInstance()->setDoubleForKey("update_tips", currentTime);
//                            PlatformHelper::showUpdateDialog(loginPacket->recvVal["resultMap"]["version"]);
//                        }
//                        
//                        setState(State_GameConfig);
//                    }
//                }
//                else
//                {
//                    setState(State_GameConfig);
//                }
//            }
//            else
//            {
//                unschedule(schedule_selector(GameLoad::loading));
//            }
//            
//            CCHttpAgent::getInstance()->packets["check_update"] = NULL;
//            delete loginPacket;
//        }
    }
    else if (state == State_GameConfig)
    {
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["gameconfig"];
        if (loginPacket && loginPacket->status != 0)
        {
            if (loginPacket->status != 3)
            {
                setState(State_error);
                return;
            }
            
            loginPacket->status = 0;
            
            if (loginPacket->resultIsOK())
            {
                ZJHModel::getInstance()->gameConfigJson = loginPacket->recvVal["resultMap"];
                CCHttpAgent::getInstance()->setImageDownloadUrl(loginPacket->recvVal["resultMap"]["list"]["imgIp"].asString());
                
                //动态修改访问域名
                {
                    if (!loginPacket->recvVal["resultMap"]["list"]["ipAddress"].isNull() && loginPacket->recvVal["resultMap"]["list"]["ipAddress"].isArray())
                    {
                        Json::Value json;
                        json["urls"] = loginPacket->recvVal["resultMap"]["list"]["ipAddress"];
                        Utils::WriteFile("urls_tmp.data", json.toStyledString());
                        
                        int index = 0;
                        std::string default_use_url = loginPacket->recvVal["resultMap"]["list"]["ipAddress"][index]["ip"].asString() + "/";
                        
                        UserDefault::getInstance()->setStringForKey("default_use_url",default_use_url);
                        UserDefault::getInstance()->flush();
//                        
//                        Manifest::_baseUrl = default_use_url;
//                        CCHttpAgent::getInstance()->setUrl(default_use_url);
//                        CCHttpAgent::getInstance()->setImageUploadUrl(default_use_url);
                    }
                }
                
                std::string startimg = loginPacket->recvVal["resultMap"]["list"]["startImageUrl"].asString();
                UserDefault::getInstance()->setStringForKey("startImageUrl",startimg);
                
                std::string bottomImg = loginPacket->recvVal["resultMap"]["list"]["bottom_img"]["bottomImg"].asString();
                UserDefault::getInstance()->setStringForKey("bottomImg", bottomImg);
                
                int openType = loginPacket->recvVal["resultMap"]["list"]["open_type"]["openType"].asInt();
                UserDefault::getInstance()->setIntegerForKey("openType", openType);
                
                string bootMusicUrl = loginPacket->recvVal["resultMap"]["list"]["app_open_Music"]["openMusic"].asString();
                UserDefault::getInstance()->setStringForKey("bootMusicUrl",bootMusicUrl);

                
                long long serverDate = loginPacket->recvVal["resultMap"]["list"]["serviceDate"].asDouble();
                
                ZJHModel::getInstance()->serverDiffrentTime = serverDate - time(NULL) * 1000;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                //是否支持ipv6
                if (ZJHModel::getInstance()->isOpenIPV6()) {
                    Network::setIpv6Enable(true);
                }
#endif
                //是否开启日志上报
//                int logIntvalTime = loginPacket->recvVal["resultMap"]["list"]["logIntvalTime"].asInt();
//                int clientLogOpen = loginPacket->recvVal["resultMap"]["list"]["clientLogOpen"].asInt(); // 1开启  -1关闭
                
                int logIntvalTime = atoi(loginPacket->recvVal["resultMap"]["list"]["openLog"]["logIntvalTime"].asString().c_str());
                int clientLogOpen = atoi(loginPacket->recvVal["resultMap"]["list"]["openLog"]["clientLogOpen"].asString().c_str()); // 1开启  -1关闭
                if(clientLogOpen == 1){
                    PingCommand::getInstance()->interval = logIntvalTime;
                    PingCommand::getInstance()->start();
                }
                if(!Utils::hasImageFile(startimg))CCHttpAgent::getInstance()->sendHttpImage(startimg, Utils::getImageSavePath(startimg), Utils::getImageSavePath(startimg));
                setState(State_loadover);
            }else if(loginPacket->recvVal["status"].asInt() == 999)
            {
                //tomcat没有连上
                unschedule(schedule_selector(GameLoad::loading));
                string content = loginPacket->recvVal["message"].asString();
                ServerErrorTips* tips = ServerErrorTips::create();
                tips->setContent(content);
                addChild(tips,1000);
            }
            else
            {
                unschedule(schedule_selector(GameLoad::loading));
            }
            
            CCHttpAgent::getInstance()->packets["gameconfig"] = NULL;
            delete loginPacket;
        }
    }
    else if (state == State_loadres)
    {
        
    }
    else if (state == State_loadover)
    {
        //        auto scene = HelloWorld::createScene();
        unschedule(schedule_selector(GameLoad::loading));
        
        endTime = Utils::getCurrentSecend()/1000;
        
        CallFunc* func1 = CallFunc::create([=]
                                           {
                                               ZJHModel::getInstance()->isGameLoad = false;
                                               int openType = UserDefault::getInstance()->getIntegerForKey("openType", 1);//1竖屏  2横屏
                                               ZJHModel::getInstance()->curGameID = 2257;
                                               ZJHModel::getInstance()->toGameHall();
//                                               if (ZJHModel::getInstance()->curGameHasHall == 1 && openType == 2){
//                                                   
//                                                   ZJHModel::getInstance()->toGameHall();
////                                               }else if (ZJHModel::getInstance()->curGameID <= 0){
////                                                   auto scene = SelectGameHall::createScene();
////                                                   Director::getInstance()->replaceScene(TransitionFade::create(0.5,scene));
//                                               }else{
//                                                   ZJHModel::getInstance()->toHall();
//                                               }
                                               
                                               //        LabaModel::getInstance()->start();
                                               MsgModel::getInstance()->start();
                                               ZJHModel::getInstance()->startRoomHeart();
                                               //        Sound::getInstance()->PlayBgSound("Sound/hall_bg.mp3");
                                               {
                                                   ZJHModel::getInstance()->uploadMyLocation();
                                               }
                                               
                                               //预加载音乐
                                               Sound::getInstance()->preLoadSound();
                                               
                                               //新加的上报一个接口
                                               shagnBaoActivated();
                                           });
        
        //控制延迟的时间
        long long delayTime = (endTime-intoTime) >= 1.5? 0: 1.5-(endTime-intoTime);
        
        printf("11111122222 = %lld\nend = %lld\nint = %lld", delayTime, endTime, intoTime);
        
        runAction(Sequence::create(DelayTime::create(delayTime),func1, NULL));
        
        
    }
    else if (state == State_error)
    {
        
    }
}

void GameLoad::doLoginSucc()
{

}

void GameLoad::onExit(){
    Layer::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

void GameLoad::shagnBaoActivated()
{
    CCHttpAgent::getInstance()->sendHttpPost(NULL, "account/uploadActivated", "", "uploadActivated");
}
