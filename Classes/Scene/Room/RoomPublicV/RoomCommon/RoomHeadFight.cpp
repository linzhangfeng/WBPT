//
//  RoomHeadFight.cpp
//  ZJH
//
//  Created by hyhy on 16/7/23.
//
//

#include "RoomHeadFight.h"
#include "RoomCMConfig.h"
#include "RoomButton.h"
#include "RoomHelp.h"
#include "RoomSetting.h"
#include "RoomManagement.h"
#include "RoomInformationLayer2.h"
#include "RoomExitDialog.h"
#include "EndRoomDialog.h"
#include "../RoomBottom/RoomInviteLayer.h"
#include "../RoomBottom/RoomAllPlayerLayer2.h"
#include "RoomChatLayer.h"

bool RoomHeadFight::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    m_pCallback = NULL;
    needRequestExist = true;
    scaleRate = 0.67;
    
    Size winSize = Director::getInstance()->getWinSize();
    float posY = winSize.height - 45;
    char buff[128];
    
    Json::Value data = ZJHModel::getInstance()->roomData;
    
    m_againstNumber = data["gameRoom"]["againstNumber"].asInt();
    m_aginstStatus = data["gameRoom"]["aginstStatus"].asInt();

    m_remain = 0;
    m_jsonData = data;
    
    //顶部状态栏背景图
    ImageView *infoBg = ImageView::create("RoomPublicV/top_bar_bg.png");
    infoBg->setAnchorPoint(Vec2(0,1));
    infoBg->setPosition(Vec2(0, winSize.height));
    infoBg->setName("infoBg");
    infoBg->setTouchEnabled(true);
    infoBg->setScale(winSize.width / 720);
    this->addChild(infoBg);
    
    //电量图标
    powIcon = ImageView::create("RoomPublicV/power_icon3.png");
    powIcon->setAnchorPoint(Vec2(0,1));
    powIcon->setPosition(Vec2(20,winSize.height - infoBg->getContentSize().height - 10));
    this->addChild(powIcon);
//    //信号图标
//    phoneSigIcon = ImageView::create("RoomPublicV/wifi3.png");
//    phoneSigIcon->setAnchorPoint(Vec2(0,1));
//    phoneSigIcon->setPosition(Vec2(40 + powIcon->getContentSize().width,winSize.height - infoBg->getContentSize().height - 10));
//    this->addChild(phoneSigIcon);

    
    Layout* leftTopLayout = Layout::create();
    leftTopLayout->setAnchorPoint(Vec2(0,1));
    leftTopLayout->setPosition(Vec2(0,infoBg->getContentSize().height));
    leftTopLayout->setContentSize(Size(250,100));
    leftTopLayout->setTouchEnabled(true);
    leftTopLayout->setName("leftTopLayout");
    leftTopLayout->addClickEventListener(CC_CALLBACK_1(RoomHeadFight::buttonCallback, this));
    infoBg->addChild(leftTopLayout);
    
    //右边下拉菜单 按钮
    Button *menuBtn = Button::create("RoomPublicV/room_bar_more.png", "RoomPublicV/room_bar_more_press.png");
    menuBtn->setName("menu_button");
    menuBtn->setAnchorPoint(Vec2(1,0.5));
    menuBtn->setPosition(Vec2(infoBg->getContentSize().width - 11, infoBg->getContentSize().height / 2 - 5));
    menuBtn->addClickEventListener(CC_CALLBACK_1(RoomHeadFight::buttonCallback, this));
    infoBg->addChild(menuBtn);
    
    //邀请按钮
    auto btn = Button::create("RoomPublicV/room_invite_icon.png","RoomPublicV/room_invite_icon_press.png");
    btn->setPosition(Vec2(475, infoBg->getContentSize().height / 2 - 5));
    btn->setAnchorPoint(Vec2(0,0.5));
    btn->addClickEventListener(CC_CALLBACK_1(RoomHeadFight::buttonCallback, this));
    infoBg->addChild(btn);
    btn->setName("weixin_invite");
    
    //排名按钮
    auto rankBtn = Button::create("RoomPublicV/rank_btn.png","RoomPublicV/rank_btn_press.png");
    rankBtn->setPosition(Vec2(341, infoBg->getContentSize().height / 2 - 5));
    rankBtn->setAnchorPoint(Vec2(0,0.5));
    infoBg->addChild(rankBtn);
    rankBtn->addClickEventListener(CC_CALLBACK_1(RoomHeadFight::buttonCallback, this));
    rankBtn->setName("watch_list");
    
    //邀请码
    Text *roomdesc = Text::create("房间号", "", 20);
    roomdesc->setAnchorPoint(Vec2(0, 1));
    roomdesc->setColor(Color3B(0xff,0xff,0xff));
    roomdesc->setPosition(Vec2(25,infoBg->getContentSize().height - 9));
    infoBg->addChild(roomdesc);

    
    auto ma = Label::createWithSystemFont(__String::createWithFormat("%s",data["gameRoom"]["code"].asCString())->getCString(), "Arial", 26);
    ma->setAnchorPoint(Vec2(0, 1));
    ma->setColor(Color3B(0xff,0xff,0xff));
    ma->setPosition(Vec2(25,infoBg->getContentSize().height - 36));
    infoBg->addChild(ma);
    
    Text *gameTypedesc = Text::create("局数", "", 20);
    gameTypedesc->setAnchorPoint(Vec2(0.5, 1));
    gameTypedesc->setColor(Color3B(0xff,0xff,0xff));
    gameTypedesc->setPosition(Vec2(197,infoBg->getContentSize().height - 9));
    gameTypedesc->setName("gameTypedesc");
    infoBg->addChild(gameTypedesc);

    
    
    snprintf(buff, sizeof(buff), "(%d/%d人)", data["gameRoom"]["onlineCount"].asInt(), m_againstNumber);
    Label *numLabel = Label::createWithSystemFont(buff, "Arial", 22);
    numLabel->setColor(Color3B(0xff,0xff,0xff));
    numLabel->setAnchorPoint(Vec2(0, 1));
    numLabel->setPosition(Vec2(234, infoBg->getContentSize().height - 11));
    numLabel->setName("num_label");
    numLabel->setVisible(false);
    infoBg->addChild(numLabel);
    
    Label *timeLabel = Label::createWithSystemFont("", "Arial", 26);
    timeLabel->setAnchorPoint(Vec2(0.5,1));
    timeLabel->setColor(Color3B(0xff,0xff,0xff));
    timeLabel->setPosition(197,infoBg->getContentSize().height - 36);
    timeLabel->setName("time_label");
    infoBg->addChild(timeLabel);
    
    ImageView* gameHelpTips = ImageView::create("RoomPublicV/game_help_icon.png");
    gameHelpTips->setAnchorPoint(Vec2(0,0));
    gameHelpTips->setPosition(Vec2(16,8));
    gameHelpTips->setName("gameHelpTips");
    gameHelpTips->setTouchEnabled(true);
    gameHelpTips->addClickEventListener(CC_CALLBACK_1(RoomHeadFight::buttonCallback, this));
    infoBg->addChild(gameHelpTips);
    
    RoomChatLayer* danmuLayout = RoomChatLayer::create();
    danmuLayout->setAnchorPoint(Vec2(0,0));
    danmuLayout->setPosition(Vec2(0,0));
    addChild(danmuLayout);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomHeadFight::notificationRoomStart), "notify_against_room_start", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomHeadFight::notificationPlayerNumber), "notify_room_player_number", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomHeadFight::notificationGameResume), "notify_game_resume", NULL);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomHeadFight::notificationGameStart), "notify_game_check_end", NULL);
     __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomHeadFight::removeShareButton), "notify_remove_share_btn", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomHeadFight::checkEndRoomTip), "notify_room_check_ip", NULL);
    this->schedule(schedule_selector(RoomHeadFight::powerSigChange), 5.0);
    checkEndRoomTip();
    checkGameEnd();
    
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = [=](EventKeyboard::KeyCode keycode,Event* event)
    {
        if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
        {
//            RoomExitDialog *exitDialog= RoomExitDialog::create();
//            exitDialog->show();
//            exitDialog->setCallback([=](std::string str)
//                                    {
//                                        if (str == "exit_confirm") {
//                                            if(m_pCallback)m_pCallback("exit");
//                                            ZJHModel::getInstance()->roomToHall();
//                                        }
//                                    });
//            this->addChild(exitDialog, 100);
        }
        event->stopPropagation();//
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    return true;
}

void RoomHeadFight::buttonCallback(cocos2d::Ref *sender)
{
    ui::Widget *btn = dynamic_cast<Widget *>(sender);
    if (btn) {
        std::string name = btn->getName();
        if (name == "menu_button")
        {
            int uid = m_jsonData["gameRoom"]["userId"].asInt();
            int type = ZJHModel::getInstance()->uid == uid ? 1 : 2;
            
            RoomButton *roomButton = RoomButton::create();
            roomButton->setName("RoomButton");
            roomButton->MyInit(type,m_aginstStatus == 0);
            roomButton->setMenuCallback(CC_CALLBACK_1(RoomHeadFight::menuCallback, this));
            this->addChild(roomButton, 100);
        }
        else if (name == "avatar_image")
        {
            RoomInformationLayer2 *roomInfo = RoomInformationLayer2::create();
            roomInfo->myInit(m_jsonData);
            this->getParent()->addChild(roomInfo,130000);
            roomInfo->show();
        }
        else if (name == "to_about")
        {
        }
        else if(name == "weixin_invite" || name == "to_invoice"){
            Json::Value json;
            json["roomId"] = m_jsonData["gameRoom"]["id"].asInt();
            CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
                CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
                if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
                
                if (loginPacket->status != 3)
                {
                    return;
                }
                if (loginPacket->resultIsOK())
                {
                    Json::Value data = loginPacket->recvVal["resultMap"];
                    PlatformHelper::WeChatShare(data["gameRoom"]["shareTitle"].asString().c_str(), data["gameRoom"]["shareContent"].asString().c_str(), data["gameRoom"]["shareUrl"].asString().c_str(), "");
                }
                CCHttpAgent::getInstance()->packets.erase(tag);
                delete loginPacket;
            },"duiju/shareView",json.toStyledString(),"shareVertical");
//            PlatformHelper::WeChatShare(m_jsonData["gameRoom"]["shareTitle"].asString().c_str(), m_jsonData["gameRoom"]["shareContent"].asString().c_str(), m_jsonData["gameRoom"]["shareUrl"].asString().c_str(), "");
        }
        else if (name == "share" )
        {
            RoomInviteLayer *invite = RoomInviteLayer::create();
            invite->myInit(m_jsonData);
            this->getParent()->addChild(invite,130000);
            invite->show();
        }
        else if(name == "watch_list"){
            RoomAllPlayerLayer2 *layer = RoomAllPlayerLayer2::create();
            layer->myInit(ZJHModel::getInstance()->roomData["gameRoom"]["userId"].asInt(), ZJHModel::getInstance()->roomData["gameRoom"]["id"].asInt());
            Director::getInstance()->getRunningScene()->addChild(layer, Z_ORDER_IN_GAME_LAYER);
            layer->show();
        }else if (name == "gameHelpTips" || name == "leftTopLayout")
        {
            RoomHelp *help = RoomHelp::create();
            this->addChild(help, 100);
            help->initHelp(m_jsonData);
            help->setPosition(Vec2(0, 0));
        }
    }
}

void RoomHeadFight::showPlayerList(){
    RoomAllPlayerLayer2 *layer = RoomAllPlayerLayer2::create();
    layer->myInit(ZJHModel::getInstance()->roomData["gameRoom"]["userId"].asInt(), ZJHModel::getInstance()->roomData["gameRoom"]["id"].asInt());
    Director::getInstance()->getRunningScene()->addChild(layer, Z_ORDER_IN_GAME_LAYER);
    layer->show();
}

void RoomHeadFight::setOnlineNumber(int value)
{
    Label *numLabel = dynamic_cast<Label *>(getChildByName("num_label"));
    if (numLabel) {
        char buff[64];
        snprintf(buff, sizeof(buff), "%d/%d人", value, m_againstNumber);
        numLabel->setString(buff);
    }
}

void RoomHeadFight::setGameStatus(int status)
{
    if (status <= m_aginstStatus) {
        return;
    }
    if (m_aginstStatus == 0) {
        if (status == 1)
        {
            m_remain = m_duration * 60;
            this->unschedule(schedule_selector(RoomHeadFight::updateTimeLabel));
            this->schedule(schedule_selector(RoomHeadFight::updateTimeLabel), 1.0);
        }
        else if (status == 2)
        {
            this->unschedule(schedule_selector(RoomHeadFight::updateTimeLabel));
            Label *timeLabel = dynamic_cast<Label *>(this->getChildByName("infoBg")->getChildByName("time_label"));
            if (timeLabel) {
                timeLabel->setString("00:00:00");
            }
            
        }
    }
    else if (m_aginstStatus == 1)
    {
        this->unschedule(schedule_selector(RoomHeadFight::updateTimeLabel));
        Label *timeLabel = dynamic_cast<Label *>(this->getChildByName("infoBg")->getChildByName("time_label"));
        if (timeLabel) {
            timeLabel->setString("00:00:00");
        }
    }
    m_aginstStatus = status;
}

void RoomHeadFight::updateTimeLabel(float dt)
{
    m_remain--;
    Label *timeLabel = dynamic_cast<Label *>(this->getChildByName("infoBg")->getChildByName("time_label"));
    
    if (m_remain < 0)
    {
//        if (timeLabel)
//        {
//            timeLabel->setString(Utils::getFormatedTime(0));
//        }
        m_remain = 0;
        this->unschedule(schedule_selector(RoomHeadFight::updateTimeLabel));
        this->checkGameEnd();
        return;
    }
    
    if (timeLabel)
    {
        timeLabel->setString(Utils::getFormatedTime(m_remain));
    }
}

void RoomHeadFight::checkGameEnd()
{
    Json::Value json;
    json["roomId"] = m_jsonData["gameRoom"]["id"].asInt();
    
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
                                                                 //房间信息里面局数或者时间的提醒
                                                                 int status  = packet->recvVal["resultMap"]["status"].asInt();
                                                                 m_aginstStatus = status;
                                                                 refreshMenu();
                                                                 int type = packet->recvVal["resultMap"]["way"].asInt();
                                                                 Label *timeLabel = dynamic_cast<Label *>(this->getChildByName("infoBg")->getChildByName("time_label"));
                                                                 Text* gameTypeTips = dynamic_cast<Text *>(this->getChildByName("infoBg")->getChildByName("gameTypedesc"));
                                                                 if (type == 1)
                                                                 {
                                                                     int curCount = packet->recvVal["resultMap"]["currentCount"].asInt();
                                                                     int allCount = packet->recvVal["resultMap"]["count"].asInt();
                                                                     gameTypeTips->setText("局数");
                                                                     string gameTips = StringUtils::format("%d",allCount);
                                                                     m_jsonData["gameTips"] = gameTips;
                                                                     if(timeLabel)timeLabel->setString(__String::createWithFormat("%d/%d",curCount,allCount)->getCString());
                                                                     
                                                                     
                                                                     if (curCount == allCount)
                                                                     {
                                                                         this->schedule(schedule_selector(RoomHeadFight::checkGameEndRepeat), 5.0);
                                                                     }
                                                                     
                                                                     if (status == 2 || status == 3)
                                                                     {
                                                                         __NotificationCenter::getInstance()->postNotification("notify_against_room_end");
                                                                     }
                                                                     else if (status != 0)
                                                                     {
                                                                         
                                                                     }
                                                                 }
                                                                 else
                                                                 {
                                                                     int allTime = packet->recvVal["resultMap"]["time"].asInt();
                                                                     double leftTime = packet->recvVal["resultMap"]["leftTime"].asDouble();
                                                                     gameTypeTips->setText("时长");
                                                                     string gameTips = StringUtils::format("%d分钟",allTime);
                                                                     m_jsonData["gameTips"] = gameTips;

                                                                     if (leftTime < 0) leftTime = 0;
                                                                     
                                                                     if (leftTime == 0)
                                                                     {
                                                                         this->schedule(schedule_selector(RoomHeadFight::checkGameEndRepeat), 5.0);
                                                                     }
                                                                     
                                                                     if (status == 2 || status == 3)
                                                                     {
                                                                         if(timeLabel)timeLabel->setString("00:00:00");
                                                                         __NotificationCenter::getInstance()->postNotification("notify_against_room_end");
                                                                     }
                                                                     else if (status != 0)
                                                                     {
                                                                         m_remain = leftTime / 1000;
                                                                         this->unschedule(schedule_selector(RoomHeadFight::updateTimeLabel));
                                                                         this->schedule(schedule_selector(RoomHeadFight::updateTimeLabel), 1.0);
                                                                         
                                                                         this->setGameStatus(status);
                                                                     }
                                                                     else
                                                                     {
                                                                         if(timeLabel)timeLabel->setString(Utils::getFormatedTime(allTime * 60));
                                                                     }
                                                                 }
                                                             }
                                                             else
                                                             {
//                                                                 Json::Value _json = packet->recvVal["resultMap"];
//                                                                 if(!_json.isNull()){
                                                                     PlatformHelper::showToast(packet->recvVal["message"].asCString());
//                                                                 }
                                                             }
                                                             
                                                         }
                                                         //
                                                         
                                                     }
                                                     
                                                     //删除packet
                                                     CCHttpAgent::getInstance()->packets.erase(tag);
                                                     delete packet;
                                                 }
                                             },
                                             "room/dz/status", json.toStyledString(), "room_status");
}

void RoomHeadFight::menuCallback(std::string name)
{
    if (name == "room_manage")
    {
        RoomManagement *management = RoomManagement::create();
        management->myInit(m_jsonData);
        this->addChild(management, 100);
        
        management->show();
    }
    else if (name == "game_setting")
    {
        RoomSetting *setting = RoomSetting::create();
        setting->initSetting();
        this->addChild(setting, 100);
    }
    else if (name == "game_help")
    {
        RoomHelp *help = RoomHelp::create();
        help->initHelp(m_jsonData);
        help->setPosition(Vec2(0, 0));
        this->addChild(help, 100);
    }
    else if(name == "out_seat_room"){
        //下桌
        //离开座位对游戏进行回调
        if (m_pCallback)m_pCallback("outSeatBtn");
    }
    else if (name == "end_room")
    {
        EndRoomDialog *exitDialog= EndRoomDialog::create();
        exitDialog->init1();
        exitDialog->setCallback([=](std::string str)
                                {
                                    if (str == "end_confirm")
                                    {
                                        if(m_pCallback)m_pCallback("end_room");
                                    }
                                });
        this->addChild(exitDialog, 100);
    }
    else if (name == "exit_room")
    {
        if (ZJHModel::getInstance()->mySeatid == -1)
        {
            //确认退出时调用退出房间的接口
            requestExit();
            if(m_pCallback)m_pCallback("exit");
            CallFunc* func1 = CallFunc::create([=]
                                               {
                                                   ZJHModel::getInstance()->roomToHall();
                                               });
            runAction(Sequence::create(DelayTime::create(0.5),func1, NULL));
        }
        else
        {
            RoomExitDialog *exitDialog= RoomExitDialog::create();
            exitDialog->show();
            exitDialog->setCallback([=](std::string str)
                                    {
                                        if (str == "exit_confirm") {
                                            //确认退出时调用退出房间的接口
                                            requestExit();
                                            if(m_pCallback)m_pCallback("exit");
                                            CallFunc* func1 = CallFunc::create([=]
                                                                               {
                                                                                   ZJHModel::getInstance()->roomToHall();
                                                                               });
                                            runAction(Sequence::create(DelayTime::create(0.5),func1, NULL));
                                        }
                                    });
            this->addChild(exitDialog, 100);
        }
    }
}

void RoomHeadFight::notificationRoomStart(cocos2d::Ref *msg)
{
    if (msg == NULL) {
        return;
    }
    
    __Integer *data = dynamic_cast<__Integer *>(msg);
    int roomId = data->getValue();
    
    if (roomId == m_jsonData["gameRoom"]["id"].asInt()) {
        this->setGameStatus(1);
    }
}

void RoomHeadFight::notificationPlayerNumber(cocos2d::Ref *msg)
{
    if (msg == NULL) {
        return;
    }
    
    __Integer *data = dynamic_cast<__Integer *>(msg);
    int num = data->getValue();
    
    this->setOnlineNumber(num);
}

void RoomHeadFight::notificationGameStart(Ref *msg)
{
    removeChildByName("share");
    this->checkGameEnd();
}

void RoomHeadFight::refreshMenu(){
    auto roomButton = getChildByName("RoomButton");
    if(roomButton != NULL){
        roomButton->removeFromParent();
        int uid = m_jsonData["gameRoom"]["userId"].asInt();
        int type = ZJHModel::getInstance()->uid == uid ? 1 : 2;
        RoomButton *roomButton = RoomButton::create();
        roomButton->setName("RoomButton");
        roomButton->MyInit(type,m_aginstStatus == 0);
        roomButton->setMenuCallback(CC_CALLBACK_1(RoomHeadFight::menuCallback, this));
        this->addChild(roomButton, 100);
    }
}

void RoomHeadFight::removeShareButton(Ref *msg)
{
    removeChildByName("share");
}

void RoomHeadFight::notificationGameResume(cocos2d::Ref *msg)
{
    this->checkGameEnd();
}

void RoomHeadFight::onExit()
{
    Layer::onExit();
    this->unschedule(schedule_selector(RoomHeadFight::powerSigChange));
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

void RoomHeadFight::checkGameEndRepeat(float dt)
{
    checkGameEnd();
}

void RoomHeadFight::requestExit(){
    if(!needRequestExist){
        return;
    }
    Json::Value json;
    json["roomId"] = m_jsonData["gameRoom"]["id"].asInt();
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"duiju/invite/out",json.toStyledString(),"requestExit");
}

void RoomHeadFight::setRequestFlag(bool flag){
    needRequestExist = flag;
}

void RoomHeadFight::checkEndRoomTip(Ref *msg)
{
    //封装获取玩家的请求
    Json::Value json;
    json["roomId"] = m_jsonData["gameRoom"]["id"].asInt();
    json["UserKey"] = ZJHModel::getInstance()->UserKey;
    
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"duiju/invite/lookList", json.toStyledString(), "room_all_player");
    
    //获取返回结果
    this->schedule(schedule_selector(RoomHeadFight::checkEndRoomTip_getData), 0.0f);
}

//获取电量和wifi手机信号强度
void RoomHeadFight::powerSigChange(float d){
    //手机信号强度
//    string phoneState = PlatformHelper::getPhoneStateSig();
//    if(!phoneState.empty()){
//        Json::Value json = Utils::ParseJsonStr(phoneState);
//        string type = json["netType"].asString();
//        int level = json["level"].asInt();
//        if(type == "WIFI"){
//            if(level > 80){
//                phoneSigIcon->loadTexture("RoomPublicV/wifi3.png");
//            }else if(level > 40  && level <= 80){
//                phoneSigIcon->loadTexture("RoomPublicV/wifi2.png");
//            }else{
//                phoneSigIcon->loadTexture("RoomPublicV/wifi1.png");
//            }
//        }else{
//            if(level > 80){
//                phoneSigIcon->loadTexture("RoomPublicV/phone3.png");
//            }else if(level > 40  && level <= 80){
//                phoneSigIcon->loadTexture("RoomPublicV/phone2.png");
//            }else{
//                phoneSigIcon->loadTexture("RoomPublicV/phone1.png");
//            }
//
//        }
//    }
    //电量
    string powerState = PlatformHelper::getPowStateSig();
    if(!powerState.empty()){
        Json::Value json = Utils::ParseJsonStr(powerState);
        int level = json["level"].asInt();
        log("======power_state==json===%d==",level);
        if(level > 80){
            powIcon->loadTexture("RoomPublicV/power_icon3.png");
        }else if(level > 40  && level <= 80){
            powIcon->loadTexture("RoomPublicV/power_icon2.png");
        }else{
            powIcon->loadTexture("RoomPublicV/power_icon1.png");
        }
    }

}

//获取返回结果
void RoomHeadFight::checkEndRoomTip_getData(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["room_all_player"];
    if (loginPacket && loginPacket->status != 0)
    {
        unschedule(schedule_selector(RoomHeadFight::checkEndRoomTip_getData));
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        else
        {
            if (loginPacket->resultIsOK())
            {
                //load数据
                Json::Value realData = loginPacket->recvVal["resultMap"]["lookList"]["result"];
                
                //计算是否有在同一个IP段的人
                map<std::string,vector<std::string>> sameGroup = Utils::searchIpSameName(realData);
                
                std::map<std::string,vector<std::string>>::iterator it;
                std::stringstream tip;
                tip.clear();
                
                char buf[128];
                bool isIpTogether = false;
                for (it = sameGroup.begin(); it != sameGroup.end(); it++) {
                    std::string ip = it->first;
                    vector<string> childV = it->second;
                    //如果有两个以上则是处于同一IP段
                    if (childV.size() > 1)
                    {
                        string ipTipsStr = "";
                        for (int i = 0; i < childV.size(); i++)
                        {
                            
                            Utils::parseName(6, buf, childV[i].c_str());
                            std::string name = buf;
                            tip<<ip<<" "<<name << "\n";
                        }
                        isIpTogether = true;
                    }
                }
                
                if (isIpTogether && getChildByName("EndRoomDialog") == NULL)
                {
                    EndRoomDialog *exitDialog= EndRoomDialog::create();
                    exitDialog->init2(tip.str());
                    exitDialog->setName("EndRoomDialog");
                    exitDialog->setCallback([=](std::string str)
                                            {
                                                if (str == "end_confirm")
                                                {
                                                    if(m_pCallback)m_pCallback("end_room");
                                                }
                                            });
                    this->addChild(exitDialog, 100);
                }
            }
            else
            {
                
            }
        }
        loginPacket->status = 0;
        CCHttpAgent::getInstance()->packets["room_all_player"] = NULL;
        delete loginPacket;
    }
}
