//
//  HallScene.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "HallScene.hpp"
#include "HallBottom.hpp"
#include "Hall.hpp"
#include "../Msg/Msg.hpp"
#include "../Mine/MineLayer.h"
#include "../Match/OpenMatchLayer.hpp"
#include "../Mine/ShopAllLayer.hpp"
#include "../../Model/MsgModel.h"
#include "../Msg/PlayWebLayerMsg.hpp"
#include "../Club/ClubHomeEmptyView.hpp"
#include "../../JsUtils/HotUpdateWithNormalView.hpp"
#include "../../JsUtils/JsUtils.h"

Scene* HallScene::createScene()
{
    auto scene = Scene::create();
    auto layer = HallScene::create();
    layer->setName("hall");
    scene->addChild(layer);
    return scene;
}

void HallScene::acountChangeReflash(Ref*ref)
{
    ZJHModel::getInstance()->diquGameList.clear();
    //1、账号改过后刷新开局界面
    if (itemMap["found"])
    {
        bool isvis = itemMap["found"]->isVisible();
        itemMap["found"]->removeFromParentAndCleanup(true);
        
        OpenMatchLayer* match = OpenMatchLayer::create();
        match->setVisible(isvis);
        addChild(match);
        match->showKeyBordView();
        itemMap["found"] = match;
    }
    
    //每日登陆奖励暂时无用屏蔽
    //noticeDailyRecive(NULL);
}

void HallScene::reflashOpenMatch(Ref*ref)
{
    //部落信息的改变刷新主界面
    bool isvis = itemMap["found"]->isVisible();
    itemMap["found"]->removeFromParentAndCleanup(true);
    
    OpenMatchLayer* match = OpenMatchLayer::create();
    match->setVisible(isvis);
    addChild(match);
    match->showKeyBordView();
    itemMap["found"] = match;
    
}

void HallScene::noticeDailyRecive(Ref*ref)
{
    Json::Value json;
    char buff[64];
    snprintf(buff, sizeof(buff), "signList");
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
                                                 if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
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
                                                                 Json::Value _Value = packet->recvVal["resultMap"];
                                                                 ZJHModel::getInstance()->loginJson = _Value;
                                                                 
                                                             }
                                                             else
                                                             {
                                                                PlatformHelper::showToast(packet->recvVal["message"].asCString());
                                                             }
                                                             
                                                         }
                                                         //
                                                     }
                                                     
                                                     //删除packet
                                                     CCHttpAgent::getInstance()->packets.erase(tag);
                                                     delete packet;
                                                 }
                                             },
                                             "task/signList", json.toStyledString(), buff);
    
}

void HallScene::noticeShowShopItem(Ref* ref)
{
    if(itemMap["myinfo"])itemMap["myinfo"]->setVisible(false);
    
    if(itemMap["dating"])
    {
        itemMap["dating"]->setVisible(true);
    }
    else
    {
        ShopAllLayer* shop = ShopAllLayer::create();
        addChild(shop);
        itemMap["dating"] = shop;
    }
}

bool HallScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    ZJHModel::getInstance()->enterGame = false;
    ZJHModel::getInstance()->gamePuase = false;
    Sound::getInstance()->StopBgSound();
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(HallScene::noticeShowShopItem), "show_shop_item", NULL);
    
    //加入、退出部落账号时发送的主界面的消息，暂时主界面的部落信息已被屏蔽
    //__NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(HallScene::reflashOpenMatch), "buLuo_InfoChange", NULL);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(HallScene::acountChangeReflash), "account_change", NULL);
    
    OpenMatchLayer* match = OpenMatchLayer::create();
    addChild(match);
    match->showKeyBordView();
    itemMap["found"] = match;
    this->setTag(9857);
    HallBottom* bottom = HallBottom::create();
    bottom->setCallback([=](std::string tag){
        
        if(itemMap["dating"])itemMap["dating"]->setVisible(false);
        if(itemMap["msg"])itemMap["msg"]->setVisible(false);
        if(itemMap["found"])itemMap["found"]->setVisible(false);
        if(itemMap["myinfo"])itemMap["myinfo"]->setVisible(false);
        if(itemMap["play"])itemMap["play"]->setVisible(false);
        
        //商店
        if (tag == "dating")
        {
            if(itemMap["dating"])
            {
                itemMap["dating"]->setVisible(true);
            }
            else
            {
                ShopAllLayer* shop = ShopAllLayer::create();
                addChild(shop);
                itemMap["dating"] = shop;
            }
        }
        else if (tag == "msg")
        {
            //社区
            if(itemMap["msg"])
            {
                itemMap["msg"]->setVisible(true);
            }
            else
            {
                Msg* msg = Msg::create();
                addChild(msg);
                itemMap["msg"] = msg;
            }
//            if(!modules["module_club"]){
//                runHotUpdate(0);
//            }
        }
        else if (tag == "found")
        {
            //大厅
            if(itemMap["found"])
            {
                itemMap["found"]->setVisible(true);
            }
            else
            {
                OpenMatchLayer* match = OpenMatchLayer::create();
                addChild(match);
                match->showKeyBordView();
                itemMap["found"] = match;
            }
        }
        else if (tag == "myinfo")
        {
            //我的
            if(itemMap["myinfo"])
            {
                itemMap["myinfo"]->setVisible(true);
            }
            else
            {
                MineLayer* msg = MineLayer::create();
                addChild(msg);
                itemMap["myinfo"] = msg;
            }
        }else if (tag == "play")
        {
            //俱乐部
            if(itemMap["play"])
            {
                ClubHomeEmptyView* club = (ClubHomeEmptyView*)itemMap["play"];
                club->setVisible(true);
                Json::Value eventjson;
                eventjson["event"] = 102;
                eventjson["content"] = "";
                __NotificationCenter::getInstance()->postNotification("js_module_notify",__String::create(eventjson.toStyledString()));
                //判断俱乐部模块是否已经加载好 如果没有加载好则要重新走加载流程
                if(!modules["module_club"]){
                    runHotUpdate(0);
                }else{
                    addClubView();
                }
            }
            else
            {
                /**** 要添加俱乐部 *******/
                ClubHomeEmptyView* club = ClubHomeEmptyView::create();
                club->setName("club_home");
                addChild(club);
                itemMap["play"] = club;
                if(!modules["module_club"]){
                    runHotUpdate(0);
                }else{
                    addClubView();
                }
                
            }
        }
    });
    addChild(bottom,100);
    
    ZJHModel::getInstance()->curView = this;
    
    
    Json::Value _Value;
    //每日登陆奖励暂时无用屏蔽
//    initRewardView(_Value);
    
    isHaveNotice = false;
    this->schedule(schedule_selector(HallScene::initNoticeView), 0.0);
    //initNoticeView();
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(HallScene::noticeCloseDaily), "notice_close", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(HallScene::reSignList), "refresh_signList", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(HallScene::notifyClubReady), "notify_club_ready", NULL);

    Sound::getInstance()->StopBgSound();
    //初始化俱乐部js组件监听
    _listener = EventListenerCustom::create("custom_module_success_event", [=](EventCustom* event){
        __String str1 =*(__String*)event->getUserData();
//        log("======custom_module_success_event=======%s",str1.getCString());
        //组件初始化 成功后读取需要恢复的场景参数
//        log("========recoveryHallParam=========%s",ZJHModel::getInstance()->recoveryHallParam.c_str());
          CallFunc* func = CallFunc::create([=]
                                          {
                                              string param = ZJHModel::getInstance()->recoveryHallParam.c_str();
                                              modules[str1.getCString()] = true;
                                              ZJHModel::getInstance()->recoveryHallParam = "";
                                              revoceryHall(param);
                                              addClubView();
                                          });
        runAction(Sequence::create(DelayTime::create(0),func, NULL));
    });
    _eventDispatcher->addEventListenerWithFixedPriority(_listener, 1);
    querySystemMsg();
    return true;
}

void HallScene::runHotUpdate(float dt){
//    unschedule(schedule_selector(HallScene::runHotUpdate));
//    HotUpdateWithNormalView::runTestModule();
//    HotUpdateWithNormalView::hotUpdateModule(2130,this->getParent(),false);
//    HotUpdateWithNormalView::hotUpdateModule(ZJHModel::getInstance()->club_id,this->getParent(),false);
}

void HallScene::addClubView(){
        CallFunc* func = CallFunc::create([=]
                                          {
                                              auto layout = this->getChildByName("club_home");
                                              if(layout != NULL && layout->getChildByName("club_view") == NULL){
                                                  Json::Value json;
                                                  json["event"] = 100;
                                                  json["content"] = "";
                                                  __NotificationCenter::getInstance()->postNotification("js_module_notify",__String::create(json.toStyledString()));
                                              }
                                          });
        runAction(Sequence::create(DelayTime::create(0.0),func, NULL));

}

void HallScene::revoceryHall(string param){
    if(param != ""){
        Json::Value jsonParam = Utils::ParseJsonStr(param);
        string view = jsonParam["view"].asString();
        if(view == "club_chat_view"){
            //跳转到聊天界面
            int uid = jsonParam["content"]["uid"].asInt();
            Json::Value json;
            json["uid"] = uid;
            Json::Value eventJsos;
            eventJsos["event"] = 101;
            eventJsos["content"] = json;
            __NotificationCenter::getInstance()->postNotification("js_module_notify",__String::create(eventJsos.toStyledString()));
        }else if(view == "club_create_view"){
            //跳转到创建俱乐部界面
            Json::Value json;
            Json::Value eventJsos;
            eventJsos["event"] = 104;
            eventJsos["content"] = json;
            __NotificationCenter::getInstance()->postNotification("js_module_notify",__String::create(eventJsos.toStyledString()));
        }
    }
};

void HallScene::noticeCloseDaily(Ref* ref)
{
    sendSignreward(true);
}

void HallScene::reSignList(Ref* ref){
    sendSignreward(false);
}

void HallScene::notifyClubReady(Ref* ref){
    if(!modules["module_club"]){
        runHotUpdate(0);
    }
}

void HallScene::sendSignreward(bool flag){
    
    //屏蔽掉每日登录的接口
    /*
    Json::Value json;
    char buff[64];
    snprintf(buff, sizeof(buff), "signList");
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
                                                 if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
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
                                                                 Json::Value _Value = packet->recvVal["resultMap"];
                                                                 ZJHModel::getInstance()->loginJson = _Value;
                                                                 if(!ZJHModel::getInstance()->isYoukeLogin() && flag){
                                                                     this->initRewardView(_Value);
                                                                 }
                                                                 
                                                             }
                                                             else
                                                             {
//                                                                 PlatformHelper::showToast(packet->recvVal["message"].asCString());
                                                             }
                                                             
                                                         }
                                                         //
                                                     }
                                                     
                                                     //删除packet
                                                     CCHttpAgent::getInstance()->packets.erase(tag);
                                                     delete packet;
                                                 }
                                             },
                                             "task/signList", json.toStyledString(), buff);
*/
    
}

void HallScene::initRewardView(Json::Value mValue){
    
   long long Atime = Utils::getCurrentTime();
   int days = (int)(Atime/1000/86400);
    int sayDays = UserDefault::getInstance()->getIntegerForKey("days");
    bool _flag = mValue["signToday"].asBool();
    if(days > sayDays && !_flag){
        Size _size = Director::getInstance()->getWinSize();
        UserDefault::getInstance()->setIntegerForKey("days", days);
        DailyReward* rew = DailyReward::create();
        rew->addJsonData(mValue);
        rew->setPosition(Vec2(0,-_size.height));
        this->addChild(rew);
        rew->runAction(MoveTo::create(0.5, Vec2(0,0)));
    }
}

void HallScene::initNoticeView(float dt){
    CCHttpPacket *packet = CCHttpAgent::getInstance()->packets["notice"];
    if(packet)
    {
        this->unschedule(schedule_selector(HallScene::initNoticeView));
        Json::Value _json = packet->recvVal["resultMap"];
        if(_json.size() > 0)
        {
            double time = _json["notice"]["time"].asDouble();
            UserDefault::getInstance()->setDoubleForKey("noticetime", time);
            
            isHaveNotice = true;
            Notice* notice = Notice::create();
            notice->addMessage(_json["notice"]);
            this->addChild(notice);
        }
        
    }else
    {
        this->unschedule(schedule_selector(HallScene::initNoticeView));
    }
    if (isHaveNotice == false)
    {
        sendSignreward(true);
    }
    
}

void HallScene::onEnter(){
    BaseLayer::onEnter();
    CallFunc* func = CallFunc::create([=]
                                      {
                                          runHotUpdate(0);
                                      });
    runAction(Sequence::create(DelayTime::create(0.0),func, NULL));
    downloadBootMusic();
}

void HallScene::onExit(){
    BaseLayer::onExit();
    _eventDispatcher->removeEventListener(_listener);
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

void HallScene::downloadBootMusic(){
    if(ZJHModel::getInstance()->bootMusicDownload){
        return;
    }
    ZJHModel::getInstance()->bootMusicDownload = true;
    //进入大厅3s后启动下载 先判断当前有没有此文件
    string url = UserDefault::getInstance()->getStringForKey("bootMusicUrl");
    string savePath = Utils::getTmpFoldPath()+Utils::getFileName(url);
    if(!Utils::hasImageFile(savePath)){
        CallFunc* func = CallFunc::create([=]
                                          {
                                              CCHttpAgent::getInstance()->sendHttpImage(url, savePath, url, NULL);
                                              this->schedule(schedule_selector(HallScene::downloading), 0);
                                          });
        runAction(Sequence::create(DelayTime::create(3),func, NULL));
    }
}

void HallScene::downloading(float dt){
    string tag = UserDefault::getInstance()->getStringForKey("bootMusicUrl");
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
    if (loginPacket && loginPacket->status != 0)
    {
        unschedule(schedule_selector(HallScene::downloading));
        CCHttpAgent::getInstance()->packets.erase(tag);
        CCHttpAgent::getInstance()->callbacks.erase(tag);
        if (loginPacket->status != 3)
        {
            return;
        }
        loginPacket->status = 0;
        string savePath = Utils::getTmpFoldPath()+Utils::getFileName(tag);
        bool hasFile = Utils::hasImageFile(savePath);
        if(hasFile){
            UserDefault::getInstance()->setStringForKey("boot_music",Utils::getFileName(tag));
        }
        log("%s========下载成功=======%d",savePath.c_str(),hasFile);
        delete loginPacket;
    }
}

void HallScene::querySystemMsg(){

    if(ZJHModel::getInstance()->isOpenIPV6())
    {
        return;
    }
    if (!ZJHModel::getInstance()->isNoticeFirstShow){
        return;
    }
    Json::Value json;
    json["time"] = UserDefault::getInstance()->getStringForKey("web_system_time","0") ;
//    json["time"] = 0;
    char buff[64];
    snprintf(buff, sizeof(buff), "querySystemMsg");
    CCHttpAgent::getInstance()->packets.erase("querySystemMsg");
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
                                                 if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
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
                                                                 Json::Value _Value = packet->recvVal["resultMap"]["notice"];
                                                                 if(!_Value){
                                                                     return;
                                                                 }
                                                                 string url = _Value["url"].asString();
                                                                 long long time = _Value["time"].asDouble();
                                                                 
                                                                 int showType = _Value["type"].asInt();//0 首页弹出  2消息列表
                                                                 
                                                                 ostringstream os;
                                                                 os << time;
                                                                 UserDefault::getInstance()->setStringForKey("web_system_time", os.str());
                                                                 
                                                                 MsgModel::FriendInfo* fi = new MsgModel::FriendInfo;
                                                                 fi->mid = SESSION_SYSTEM_WEB_MESSAGE;
                                                                 fi->unReadNums = 1;
                                                                 MsgModel::getInstance()->addFriend(fi);
                                                                 
                                                                 MsgModel::Chat* chat = new MsgModel::Chat();
                                                                 chat->mid = SESSION_SYSTEM_WEB_MESSAGE;
                                                                 chat->fromId = SESSION_SYSTEM_WEB_MESSAGE;
                                                                 chat->toId = ZJHModel::getInstance()->uid;
                                                                 chat->msgType = MESSAGE_TYPE::MSG_TYPE_TEXT;
                                                                 chat->content = url;
                                                                 chat->createTime = Utils::getCurrentTime();
                                                                 chat->status = MSG_STATE::MSG_STATE_SUCC;
                                                                 MsgModel::getInstance()->addChat(chat);
                                                                 
                                                                 MsgModel::Session* s = new MsgModel::Session;
                                                                 s->mid = chat->mid;
                                                                 s->fromId = chat->fromId;
                                                                 s->type = MESSAGE_TYPE::SESSION_SYSTEM_WEB_MESSAGE;
                                                                 s->msgType = chat->msgType;
                                                                 s->content = url;
                                                                 s->createTime = chat->createTime;
                                                                 MsgModel::getInstance()->addSession(s);
                                                                 
                                                                 if(showType == 0 || showType == 4){
                                                                     PlayWebLayerMsg *web = PlayWebLayerMsg::create();
                                                                     Director::getInstance()->getRunningScene()->addChild(web, 1000);
                                                                     web->showView("系统消息", CCHttpAgent::getInstance()->url+url);
                                                                     
                                                                     //将未读数 清零
                                                                     MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[SESSION_SYSTEM_WEB_MESSAGE];
                                                                     fi->unReadNums = 0;
                                                                     MsgModel::getInstance()->updateFriendInfo(fi);
                                                                 }else{
                                                                 __NotificationCenter::getInstance()->postNotification("hasNewMsg_notify");//notify msg has new msg
                                                                 }
                                                                 
                                                                 __NotificationCenter::getInstance()->postNotification("Msg_reflash");//notify msg has new msg
                                                                 
                                                                 __NotificationCenter::getInstance()->postNotification("ChatFriends_reflash");//notify chat view
                                                             }
                                                             else
                                                             {
                                                                 PlatformHelper::showToast(packet->recvVal["message"].asCString());
                                                             }

                                                         }
                                                     }

                                                     //删除packet
                                                     CCHttpAgent::getInstance()->packets.erase(tag);
                                                     delete packet;
                                                 }
                                             },
                                             "notice/info", json.toStyledString(), buff);
    ZJHModel::getInstance()->isNoticeFirstShow = false;

}

