//
//  LoadTable.cpp
//  ZJH
//
//  Created by hyhy on 16/7/12.
//
//

#include "LoadTable.h"
#include "../Hall/HallScene.hpp"
#include "../../Model/MsgModel.h"
#include "../../JsUtils/HotUpdateLayer.hpp"
#include "../../JsUtils/JsUtils.h"
#include "extensions/cocos-ext.h"


USING_NS_CC;
USING_NS_CC_EXT;

Scene *LoadTable::createScene()
{
    Scene *scene = Scene::create();
    auto layer = LoadTable::createWithData();

    //此处保存是否有正在玩的房间
    int playingCount = UserDefault::getInstance()->getIntegerForKey("lastRoomPlaying");
    UserDefault::getInstance()->setIntegerForKey("lastRoomPlaying", ++playingCount);

    scene->addChild(layer);
    return scene;
}

LoadTable *LoadTable::createWithData()
{
    LoadTable *pRet = new LoadTable();
    if (pRet && pRet->initWithData()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool LoadTable::initWithData()
{
    if (!Layer::init()) {
        return false;
    }
    
    ZJHModel::getInstance()->gameUseDefaultSetting = true;
    ZJHModel::getInstance()->gameUseDefaultChat = true;
    
    //群聊需要
    MsgModel::getInstance()->join_group(ZJHModel::getInstance()->roomData["gameRoom"]["groupId"].asInt());
    
    ZJHModel::getInstance()->roomUserID = ZJHModel::getInstance()->roomData["gameRoom"]["userId"].asInt();
    ZJHModel::getInstance()->curGameType = ZJHModel::getInstance()->roomData["gameRoom"]["gameId"].asInt();
    ZJHModel::getInstance()->curRoomID = ZJHModel::getInstance()->roomData["gameRoom"]["id"].asInt();
    ZJHModel::getInstance()->isOnVideo = false;
    MsgModel::getInstance()->curChatUID = ZJHModel::getInstance()->roomData["clubGroupId"].asInt();
    if (ZJHModel::getInstance()->curGameID == -1)
    {
        ZJHModel::getInstance()->setCurGameID(ZJHModel::getInstance()->roomData["gameRoom"]["gameId"].asInt(), 1);
    }
    
    
    Size winSize = Director::getInstance()->getWinSize();
    
    Sprite *loadText = Sprite::create("Loading/loading_txt.png");
    loadText->setPosition(Vec2(winSize.width / 2, winSize.height / 2 ));
    this->addChild(loadText);
    
    Sprite *dots = Sprite::create("Loading/loading_icon.png");
    dots->setPosition(Vec2(winSize.width / 2, winSize.height / 2 +140));
    this->addChild(dots);
    dots->runAction(RepeatForever::create(RotateBy::create(3, 360)));

    //触摸
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch *t,Event *e){return true;};
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //返回键
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = [=](EventKeyboard::KeyCode keycode, cocos2d::Event *event){
        //
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad,this);
    
    //移除ios更新提示
    PlatformHelper::removeIosUpdateTip();
    
    schedule(schedule_selector(LoadTable::updateGame), 0.0);
    return true;
}

void LoadTable::updateGame(float dt)
{
    unschedule(schedule_selector(LoadTable::updateGame));
#if COCOS2D_DEBUG == 1
    JsUtils::initLoad();
//    JsUtils::runScript("tianchangxiaohua/scripts/main.js");
//    JsUtils::runScript("mj_project/scripts/main.js");
//    JsUtils::runScript("xiangqi_project/scripts/main.js");
//    JsUtils::runScript("mj_new/scripts/main.js");
//    JsUtils::runScript("mj2.5_new/scripts/main.js");
//    JsUtils::runScript("mj2d_2.5/scripts/main.js");
//    JsUtils::runScript("niuniu_project/scripts/main.js");
//    JsUtils::runScript("mj_chengdu/scripts/main.js");
//    JsUtils::runScript("card_project/scripts/main.js");
    JsUtils::runScript("poker_project/scripts/main.js");
//    JsUtils::runScript("slwh/scripts/main.js");
//    Json::Value f = Utils::ReadFile("runScript.txt");
//    JsUtils::runScript(f["run"].asString());
    return;
#endif
    
    Json::Value json;
    json["gameId"] = ZJHModel::getInstance()->roomData["gameRoom"]["gameId"].asInt();
    
    //加入对局的请求
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            this->cancelCallback();
            
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data;
            
            Json::Value& publicList = loginPacket->recvVal["resultMap"]["publicList"];
            Json::Value& list = loginPacket->recvVal["resultMap"]["list"];
            for(int i =0;i<publicList.size();++i)
            {
                string url =publicList[i]["jsurl"].asString();
                string pName = publicList[i]["entrance"].asString();
                //                        ZJHModel::getInstance()->packName =pName;
                
                Json::Value dat;
                dat["packageUrl"]="";
                dat["remoteManifestUrl"]=pName+"/version/project.manifest";
                dat["remoteVersionUrl"]=pName+"/version/version.manifest";
                dat["version"]="0";
                dat["assets"].append("");
                dat["assets"].clear();
                dat["searchPaths"].append("");
                dat["searchPaths"].clear();
                
                Json::Value dat1;
                dat1["baseData"]=dat.toStyledString();
                dat1["baseUrl"]=(url+"/");
                dat1["versionPath"]=(pName+"/version/");
                
                data["list"].append(dat1);
            }
            
            for(int i =0;i<list.size();++i)
            {
                if(list[i]["id"] == ZJHModel::getInstance()->roomData["gameRoom"]["gameId"].asInt())
                {
                    string url =list[i]["jsurl"].asString();
                    string pName = list[i]["entrance"].asString();
                    ZJHModel::getInstance()->packName =pName;
                    Json::Value dat;
                    dat["packageUrl"]="";
                    dat["remoteManifestUrl"]=pName+"/version/project.manifest";
                    dat["remoteVersionUrl"]=pName+"/version/version.manifest";
                    dat["version"]="0";
                    dat["assets"].append("");
                    dat["assets"].clear();
                    dat["searchPaths"].append("");
                    dat["searchPaths"].clear();
                    
                    Json::Value dat1;
                    dat1["baseData"]=dat.toStyledString();
                    dat1["baseUrl"]=(url+"/");
                    dat1["versionPath"]=(pName+"/version/");
                    
                    data["list"].append(dat1);
                    
                    data["entry"]=pName+"/scripts/main.js";
                    Director::getInstance()->replaceScene(HotUpdateLayer::createScene(data));
                    return;
                }
            }
            this->cancelCallback();
            
        }else
        {
            this->cancelCallback();
        }
    },"version/zipVersionList",json.toStyledString(),"check");
}

void LoadTable::cancelCallback()
{
    ZJHModel::getInstance()->roomToHall(false);
}
