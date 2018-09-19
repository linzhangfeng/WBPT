//
//  GameHallScene.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "GameHallScene.hpp"
#include "JoinRoom.hpp"
#include "RecordBox.hpp"
#include "CreateRoom.hpp"
#include "CreateRoomHelp.hpp"
#include "editor-support/cocostudio/CocoStudio.h"
#include "../Load/LoadTable.h"
#include "SettingGame.hpp"
#include "../../Model/MsgModel.h"
#include "HorizenSystemMsg.hpp"
#include "GameNoticeDialog.hpp"
#include "SearchVideo.hpp"
#include "../../JsUtils/JsUtils.h"
#include "KeFu.hpp"
Scene* GameHallScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameHallScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameHallScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    ZJHModel::getInstance()->enterGame = false;
    ZJHModel::getInstance()->gamePuase = false;
    noticeText = "";
    Sound::getInstance()->StopBgSound();
    cocostudio::ArmatureDataManager::getInstance()->destroyInstance();
    //    cocostudio::SceneReader::getInstance()->destroyInstance();//此行会导致音效缓存失效
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    Director::getInstance()->getTextureCache()->removeAllTextures();
    titleBg = NULL;
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameHallScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    initCsbBView();
    initHttpParam();
    Sound::getInstance()->StopBgSound();
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(GameHallScene::gameLoadWithWebCode), "game_reload_new",NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(GameHallScene::gameReload), "game_reload",NULL);
    querySystemMsg();
    return true;
}

void GameHallScene::initHttpParam(){
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("请求用户数据失败");
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"]["myInfoVO"];
            
            char buf[128];
            Utils::parseName(7, buf, data ["nickName"].asString().c_str());
            nameUser->setString(buf);
            
            std::string userImage = data["thumbImage"].asString();
            ZJHModel::getInstance()->portrit = userImage;
            
            __String *bufStr = __String::createWithFormat("斗牌ID:%d", data["id"].asInt());
            userID->setString(bufStr->getCString());
            
            portritBg->removeAllChildren();
            Sprite* userPortrit = Utils::createCircleAvatar(userImage, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(140,140));
            userPortrit->setAnchorPoint(Vec2(0.5,0.5));
            userPortrit->setPosition(Vec2(portritBg->getContentSize().width/2 - 5, portritBg->getContentSize().height/2 + 12));
            portritBg->addChild(userPortrit);
            CCHttpAgent::getInstance()->packets.erase(tag);
            delete loginPacket;
        }
    },"account/myInfo","","myInfoHall");
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("请求用户数据失败");
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"]["play"];
            int wan = data["wanbei"].asInt();
            int card = loginPacket->recvVal["resultMap"]["roomCard"].asInt();
            
            printf("card == %d\n", card);
            roomCards->setString(ZJHModel::getInstance()->moneyShowDeal(card));
            zhuanshiText->setString(ZJHModel::getInstance()->moneyShowDeal(wan));
            //删除packet
            CCHttpAgent::getInstance()->packets.erase(tag);
            delete loginPacket;
        }else
        {
            if (!loginPacket->recvVal["message"].isNull() && loginPacket->recvVal["message"].isString())
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
        }
        
    },"golden/userWealth","","wealth1Hall");
    
    //请求单款游戏详细数据
    Json::Value jsonDate;
    int gameId = ZJHModel::getInstance()->curGameID;
//    gameId = 2001;
    if(gameId == 0){
        ZJHModel::getInstance()->roomToHall(false);
        return;
    }
    jsonDate["gameId"] = gameId;
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* packet = CCHttpAgent::getInstance()->packets[tag];
        
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;

        if (packet->status != 3)
        {
            PlatformHelper::showToast("请求游戏数据失败");
            return;
        }
        
        if (packet->resultIsOK())
        {
            Json::Value data = packet->recvVal["resultMap"];
            //中间游戏logo区域
            std::string titleLogo = data["gameConfig"]["titleLogo"].asString();
            ImageView* logo = Utils::createNormalImage(titleLogo, "Avatars/wanbei_default.png", Size(454,142));
            if(titleBg == NULL){
                logo->setAnchorPoint(Vec2(0.5,1));
                logo->setPosition(Vec2(1022,topBg->getContentSize().height - 10));
                topBg->addChild(logo);
            }else{
                logo->setAnchorPoint(Vec2(0.5,0.5));
                logo->setPosition(titleBg->getContentSize() / 2);
                titleBg->addChild(logo);
            }
            //删除packet
            CCHttpAgent::getInstance()->packets.erase(tag);
            delete packet;
        }
    }, "gameClasli/gameConfig", jsonDate.toStyledString(), "requestDataByGameId");
    
    //请求公告数据
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* packet = CCHttpAgent::getInstance()->packets[tag];
        
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (packet->status != 3)
        {
            PlatformHelper::showToast("请求公告数据失败");
            return;
        }
        
        if (packet->resultIsOK())
        {
            Json::Value data = packet->recvVal["resultMap"];
            //公告区域
            noticeText = data["notice"]["content"].asString();
            notice->setString(noticeText);
            
            if(notice->getContentSize().height > clip->getContentSize().height){
                int time = 13 * (notice->getContentSize().height / 251);
                MoveTo* to2 = MoveTo::create(time, Vec2(0, notice->getContentSize().height + clip->getContentSize().height));
                MoveTo* back = MoveTo::create(0, Vec2(0, 0));
                notice->runAction(RepeatForever::create(Sequence::create(to2,back, NULL)));
            }
            //删除packet
            CCHttpAgent::getInstance()->packets.erase(tag);
            delete packet;
        }
    }, "notice/typeinfo?notictype=5&gameId="+StringUtils::format("%d",gameId), "", "requestNotice");
    
    
    UserDefault::getInstance()->setIntegerForKey("lastRoomPlaying", 1);
    CallFunc* func = CallFunc::create([=]
                                      {
                                          ZJHModel::getInstance()->checkGamePlayingError([=](std::string str){
                                              if(str == "normal"){
                                                  gameReload(NULL);
                                                  gameLoadWithWebCode(NULL);
                                              }else{
                                                  ZJHModel::getInstance()->gotoRoomWithCode(str);
                                              }
                                          });
                                      });
    if (ZJHModel::getInstance()->firtEnterApp)runAction(Sequence::create(DelayTime::create(0.5),func, NULL));
    
    ZJHModel::getInstance()->firtEnterApp = false;
}

void GameHallScene::gameLoadWithWebCode(Ref* f)
{
    if(ZJHModel::getInstance()->enterGame)
    {
        return;
    }
    string webCode = PlatformHelper::loadCodeNew();
    if(webCode != "")
    {
        PlatformHelper::showToast("正在进入房间");
        ZJHModel::getInstance()->gotoRoomWithCode(webCode);
    }
}

void GameHallScene::gameReload(Ref* r)
{
    string webCode = PlatformHelper::loadCode();
    
    if (webCode.size() != 0)
    {
        const static std::string findFlag = "=";
        int cutPos = webCode.find(findFlag);
        if (cutPos == std::string::npos)
        {
            
        }
        else
        {
            string code = webCode.substr(cutPos + findFlag.length());
            if (webCode.find("webCode") != std::string::npos)
            {
                ZJHModel::getInstance()->gotoRoomWithCode(code);
            }
            else
            {
                PlatformHelper::sdkDoForVoid_C("copyToPasteboard",code);
                PlatformHelper::showToast("邀请码已复制到剪贴板");
            }
        }
    }
}

//加载csb布局文件
void GameHallScene::initCsbBView(){
    this->removeAllChildren();
    Size screenSize = Director::getInstance()->getWinSize();
    root = CSLoader::createNode("GameHall/img/GameHall.csb");  //注意csb文件已经copy到Resources目录了。
    this->addChild(root);
    //初始化 背景
    ImageView* backBg = (ImageView *)Utils::findNode(root,"hall_image_bg");
    
    //特效雪花
//    CallFunc* func1 = CallFunc::create([=]
//                                       {
//                                           cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("GameHall/xuehua/effects_phdtxuehua.ExportJson");
//                                           auto effect = cocostudio::Armature::create("effects_phdtxuehua");
//                                           effect->setPosition(Vec2(screenSize.width / 2,screenSize.height));
//                                           effect->setAnchorPoint(Vec2(0.5,1));
//                                           backBg->addChild(effect,1);
//                                           effect->getAnimation()->playByIndex(0,-1,2);
//                                       });
//    runAction(Sequence::create(DelayTime::create(0.5),func1, NULL));
    
    
    //初始化顶部UI
    topBg = (ImageView *)Utils::findNode(root,"top_bg");
    
    titleBg = (ImageView *)Utils::findNode(root,"game_icon_title");
    
    portritBg = (ImageView *)Utils::findNode(root,"my_head_icon_bg");
    
    //name
    nameUser = (Text *)Utils::findNode(root,"nick_name");
    
    userID = (Text *)Utils::findNode(root,"my_doupai_id");
    
    zhuanshiText = (Text *)Utils::findNode(root,"zuanshi_number");
    
    roomCards = (Text *)Utils::findNode(root,"room_card");
    
    //右边功能区域
    //客服
    Button* kefuBtn = (Button *)Utils::findNode(root,"kefu");
    kefuBtn->addClickEventListener(CC_CALLBACK_1(GameHallScene::menuCallback, this));
    
    //战绩
    Button* zhanjiBtn = (Button *)Utils::findNode(root,"zhanjiBtn");
    zhanjiBtn->addClickEventListener(CC_CALLBACK_1(GameHallScene::menuCallback, this));
    //玩法
    Button* wanfaBtn = (Button *)Utils::findNode(root,"wanfaBtn");
    wanfaBtn->addClickEventListener(CC_CALLBACK_1(GameHallScene::menuCallback, this));
    
    //设置
    Button* settingBtn = (Button *)Utils::findNode(root,"settingBtn");
    settingBtn->addClickEventListener(CC_CALLBACK_1(GameHallScene::menuCallback, this));
    
    Button* searchVideoBtn = (Button *)Utils::findNode(root,"searchVideoBtn");
    searchVideoBtn->addClickEventListener(CC_CALLBACK_1(GameHallScene::menuCallback, this));
    
    //下面大厅功能区域
    //返回竖版大厅按钮
    Button* backVertical = (Button *)Utils::findNode(root,"backVertical");
    backVertical->addClickEventListener(CC_CALLBACK_1(GameHallScene::menuCallback, this));
    
    //公告
    ImageView* gameNotice = (ImageView *)Utils::findNode(root,"game_notice_bg");
    gameNotice->addTouchEventListener(CC_CALLBACK_2(GameHallScene::touchCallback, this));
    gameNotice->setTouchEnabled(true);
    
    //公告裁剪区域
    clip = ClippingNode::create();
    clip->setAnchorPoint(Vec2(0.5, 0));
    clip->setPosition(Vec2(gameNotice->getContentSize().width / 2,45));
    clip->setInverted(false);    //设置裁剪区域可见还是非裁剪区域可见  这里为裁剪区域可见
    //    clip->setAlphaThreshold(0);
    clip->setContentSize(Size(253, 270));
    gameNotice->addChild(clip);
    
    auto stencil = DrawNode::create();
    Vec2 rectangle[4];
    rectangle[0] = Vec2(0, 0);
    rectangle[1] = Vec2(clip->getContentSize().width, 0);
    rectangle[2] = Vec2(clip->getContentSize().width, clip->getContentSize().height);
    rectangle[3] = Vec2(0, clip->getContentSize().height);
    
    Color4F white(1, 1, 1, 1);
    stencil->drawPolygon(rectangle, 4, white, 1, white);
    clip->setStencil(stencil);
    
    
    notice = LabelTTF::create("", "", 30,Size(253, 0), TextHAlignment::LEFT);
    notice->setAnchorPoint(Vec2(0,1));
    notice->setColor(Color3B(0xf3, 0xf4, 0xef));
    notice->setPosition(Vec2(0,clip->getContentSize().height));
    clip->addChild(notice);
    
    //创建房间
    ImageView* createRoomIcon = (ImageView *)Utils::findNode(root,"createRoomIcon");
    createRoomIcon->setTouchEnabled(true);
    createRoomIcon->addClickEventListener(CC_CALLBACK_1(GameHallScene::menuCallback, this));
    
    Button* createRoom = (Button *)Utils::findNode(root,"createRoom");
    createRoom->addClickEventListener(CC_CALLBACK_1(GameHallScene::menuCallback, this));
    
    //加入房间
    ImageView* joinRoomIcon = (ImageView *)Utils::findNode(root,"joinRoomIcon");
    joinRoomIcon->setTouchEnabled(true);
    joinRoomIcon->addClickEventListener(CC_CALLBACK_1(GameHallScene::menuCallback, this));
    
    Button* joinRoom = (Button *)Utils::findNode(root,"joinRoom");
    joinRoom->addClickEventListener(CC_CALLBACK_1(GameHallScene::menuCallback, this));
}

void GameHallScene::touchCallback(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        ui::Widget *btn = dynamic_cast<Widget *>(pSender);
        if (btn) {
            std::string name = btn->getName();
            if(name == "game_notice_bg"){
                if(noticeText == ""){
                    return;
                }
                auto dialog = GameNoticeDialog::create();
                addChild(dialog);
                dialog->show(noticeText);
            }
        }
    }
    
}

void GameHallScene::menuCallback(cocos2d::Ref *sender){
    ui::Widget *btn = dynamic_cast<Widget *>(sender);
    if (btn) {
        std::string name = btn->getName();
        if(name == "joinRoom" || name == "joinRoomIcon"){
            ZJHModel::getInstance()->checkGamePlayingError([=](std::string str){
                if(str == "normal"){
                    addChild(JoinRoom::create());
                }else{
                    ZJHModel::getInstance()->gotoRoomWithCode(str);
                }
            });
        }else if(name == "createRoom" || name =="createRoomIcon"){
            ZJHModel::getInstance()->checkGamePlayingError([=](std::string str){
                if(str == "normal"){
                    addChild(CreateRoom::create());
                }else{
                    ZJHModel::getInstance()->gotoRoomWithCode(str);
                }
            });
        }else if(name == "backVertical"){
            ZJHModel::getInstance()->toHall();
        }else if(name == "settingBtn"){
            addChild(SettingGame::create());
        }else if(name == "wanfaBtn"){
            addChild(CreateRoomHelp::create());
        }else if(name == "zhanjiBtn"){
            addChild(RecordBox::create(),10000);
        }
        else if(name == "kefu"){
            addChild(KeFu::create(),10000);
        }else if(name == "searchVideoBtn"){
            addChild(SearchVideo::create(),10000);
        }else if(name == "addCards"){
//            if(ZJHModel::getInstance()->isOpenIPV6())
//            {
//                return;
//            }
//            PlatformHelper::showToast("请联系你的代理");
        }
    }
}


void GameHallScene::onKeyReleased(EventKeyboard::KeyCode keycode,Event* event)
{
    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
    {
//        //监听返回键  两次退出
//        long long currentTime = Utils::getCurrentTime();
//        if(currentTime - lastTouchBackKeyTime > 2000){
//            lastTouchBackKeyTime = currentTime;
//            PlatformHelper::showToast("再按一次退出玩呗斗牌");
//        }else{
//            PlatformHelper::exitWithoutParam();
//        }
        ZJHModel::getInstance()->toHall();
    }
    event->stopPropagation();
}


void GameHallScene::querySystemMsg(){
    
    if(ZJHModel::getInstance()->isOpenIPV6())
    {
        return;
    }
//    if (!ZJHModel::getInstance()->isNoticeFirstShow){
//        return;
//    }
    Json::Value json;
    json["time"] = UserDefault::getInstance()->getStringForKey("web_system_time","0") ;
    json["gameNoticeTime"] = UserDefault::getInstance()->getStringForKey("web_system_time_game","0") ;
    json["gameId"] = ZJHModel::getInstance()->curGameID;
    char buff[64];
    snprintf(buff, sizeof(buff), "queryHorizenMsg");
    CCHttpAgent::getInstance()->packets.erase("queryHorizenMsg");
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
                                                                 int noticeGameId = _Value["gameId"].asInt();
                                                                 if(noticeGameId == -1){
                                                                     ZJHModel::getInstance()->isNoticeFirstShow = false;
                                                                     //返回的是默认公告
                                                                     string url = _Value["url"].asString();
                                                                     string transverse_url = _Value["transverse_url"].asString();
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
                                                                         HorizenSystemMsg *web = HorizenSystemMsg::create();
                                                                         Director::getInstance()->getRunningScene()->addChild(web, 1000);
                                                                         web->showView("系统消息", CCHttpAgent::getInstance()->url+transverse_url);
                                                                         
                                                                         //将未读数 清零
                                                                         MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[SESSION_SYSTEM_WEB_MESSAGE];
                                                                         fi->unReadNums = 0;
                                                                         MsgModel::getInstance()->updateFriendInfo(fi);
                                                                     }else{
                                                                         __NotificationCenter::getInstance()->postNotification("hasNewMsg_notify");//notify msg has new msg
                                                                     }
                                                                     
                                                                     __NotificationCenter::getInstance()->postNotification("Msg_reflash");//notify msg has new msg
                                                                     
                                                                     __NotificationCenter::getInstance()->postNotification("ChatFriends_reflash");//notify chat view
                                                                 }else {
                                                                     //返回的是游戏特定公告
                                                                     string url = _Value["url"].asString();
                                                                     string transverse_url = _Value["transverse_url"].asString();
                                                                     long long time = _Value["time"].asDouble();
                                                                     
                                                                     ostringstream os;
                                                                     os << time;
                                                                     UserDefault::getInstance()->setStringForKey("web_system_time_game", os.str());
                                                                     
                                                                     HorizenSystemMsg *web = HorizenSystemMsg::create();
                                                                     Director::getInstance()->getRunningScene()->addChild(web, 1000);
                                                                     web->showView("系统消息", CCHttpAgent::getInstance()->url+transverse_url);
                                                                 }
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
    
}

void GameHallScene::onEnter()
{
    Layer::onEnter();
    
    //显示定位提示框
    PlatformHelper::showOpenLocationDialog();
    
}

void GameHallScene::onExit()
{
    Layer::onExit();
    
    //移除定位提示框
    PlatformHelper::removeIosLocationTip();

    CCHttpAgent::getInstance()->callbacks.erase("myInfoHall");
    CCHttpAgent::getInstance()->callbacks.erase("wealth1Hall");
    CCHttpAgent::getInstance()->callbacks.erase("requestDataByGameId");
    CCHttpAgent::getInstance()->callbacks.erase("requestNotice");
    __NotificationCenter::getInstance()->removeAllObservers(this);
}


