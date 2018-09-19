//
//横版公共模块
//1.左上角房间信息
//2.右上角邀请，菜单，聊天，排名
//3.左下快速语音
//4.右下自动胡牌，上桌
//

#include "RoomPublic.h"
#include "RoomMenu.h"
#include "RoomRule.h"
#include "RoomSettingH.h"
#include "RoomInformation.h"
#include "RoomHExitDialog.h"

#include "RoomInvite.h"

#include "RoomManagement2.hpp"
#include "RoomRealTimeScore.h"
#include "RoomChat2.hpp"
//#include "RoomTakeinLayer2.hpp"

#include "../../Msg/ChatRecordEffect.hpp"

#include "EndRoomTipsDialog.h"
#include "../RoomPublicV/RoomCommon/AlertDialogDownTable2.hpp"
#include "../RoomPublicV/RoomCommon/RoomChatLayer.h"
bool RoomPublic::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    addChild(ChatRecordEffect::create(),100000);
    
    m_pCallback = NULL;
    needRequestExist = true;
    scaleRate = 0.67;
    
    Size winSize = Director::getInstance()->getWinSize();
    float posY = winSize.height - 43;
    char buff[128];
    
    Json::Value data = ZJHModel::getInstance()->roomData;
    
    m_againstNumber = data["gameRoom"]["againstNumber"].asInt();
    m_aginstStatus = data["gameRoom"]["aginstStatus"].asInt();
    
    m_remain = 0;
    m_jsonData = data;
    
    //房间信息展示
    
    {
        //房间信息button背景
        auto gameHelp = Button::create("RoomPublicH/game_rule_tips.png","RoomPublicH/game_rule_tips_press.png");
        gameHelp->setAnchorPoint(Vec2(0, 1));
        gameHelp->setPosition(Vec2(190, winSize.height - 11));
        gameHelp->setName("game_help");
        gameHelp->addClickEventListener(CC_CALLBACK_1(RoomPublic::buttonCallback, this));
        this->addChild(gameHelp);
        //电量图标
        powIcon = ImageView::create("RoomPublicV/power_icon3.png");
        powIcon->setName("pow_icon");
        powIcon->setAnchorPoint(Vec2(0,1));
        powIcon->setPosition(Vec2(17 ,winSize.height - 9));
        this->addChild(powIcon);
        
        cTime = Text::create("", "AmericanTypewriter", 22);
        cTime->setName("current_time");
        cTime->setAnchorPoint(Vec2(0,1));
        cTime->setPosition(Vec2(75 ,winSize.height - 9));
        this->addChild(cTime);
        getCurrentTime(0);
        
        //信号图标
//        phoneSigIcon = ImageView::create("RoomPublicV/wifi3.png");
//        phoneSigIcon->setAnchorPoint(Vec2(0,1));
//        phoneSigIcon->setPosition(Vec2(roomInfoBg->getContentSize().width + powIcon->getContentSize().width + 10,winSize.height - 13));
//        this->addChild(phoneSigIcon);
        //房主信息按钮
//        auto roomInfoBt = Button::create("RoomPublicH/room_info_bt.png","RoomPublicH/room_info_bt_press.png");
//        roomInfoBt->setScale9Enabled(true);
//        roomInfoBt->setName("avatar_image");
//        roomInfoBt->setAnchorPoint(Vec2(0, 0.5));
//        roomInfoBt->setPosition(Vec2(5,roomInfoBg->getContentSize().height/2));
//        roomInfoBt->addClickEventListener(CC_CALLBACK_1(RoomPublic::buttonCallback, this));
//        roomInfoBg->addChild(roomInfoBt);
        
        //邀请码
        auto inviteCodeLabel = Label::createWithSystemFont(__String::createWithFormat("房间号 : %s",data["gameRoom"]["code"].asCString())->getCString(), "Arial", 22);
        inviteCodeLabel->setName("invite_code");
        inviteCodeLabel->setAnchorPoint(Vec2(0, 1));
        inviteCodeLabel->setColor(Color3B(0xdc,0xff,0x92));
        inviteCodeLabel->setPosition(Vec2(18,winSize.height - 39));
        addChild(inviteCodeLabel);
        
        //对局人数
//        snprintf(buff, sizeof(buff), "(%d/%d人)", data["gameRoom"]["onlineCount"].asInt(), m_againstNumber);
//        Label *numLabel = Label::createWithSystemFont(buff, "Arial", 16);
//        numLabel->setAnchorPoint(Vec2(0, 0));
//        numLabel->setPosition(Vec2(78,17));
//        numLabel->setName("num_label");
//        roomInfoBg->addChild(numLabel);

        
        //局数
        Label *timeLabel = Label::createWithSystemFont("局数 : 0/0", "Arial", 22);
        timeLabel->setAnchorPoint(Vec2(0, 1));
        timeLabel->setPosition(Vec2(18,inviteCodeLabel->getPositionY() - inviteCodeLabel->getContentSize().height - 2));
        timeLabel->setName("time_label");
        addChild(timeLabel);
        
    }
    
    //菜单区域
    {
        
        //菜单more
        menuMoreBtn = Button::create("RoomPublicH/menu_more.png", "RoomPublicH/menu_more_press.png");
        menuMoreBtn->setName("menu_button");
        menuMoreBtn->setAnchorPoint(Vec2(1,1));
        menuMoreBtn->setPosition(Vec2(winSize.width-16, winSize.height - 7));
        menuMoreBtn->addClickEventListener(CC_CALLBACK_1(RoomPublic::buttonCallback, this));
        addChild(menuMoreBtn);
        
        //战绩
        menuZhanjiBtn = Button::create("RoomPublicH/menu_zhanji.png", "RoomPublicH/menu_zhanji_press.png");
        menuZhanjiBtn->setName("to_result");
        menuZhanjiBtn->setAnchorPoint(Vec2(1,0.5));
        menuZhanjiBtn->setPosition(Vec2(-10, menuMoreBtn->getContentSize().height/2));
        menuZhanjiBtn->addClickEventListener(CC_CALLBACK_1(RoomPublic::buttonCallback, this));
        menuMoreBtn->addChild(menuZhanjiBtn);
//        menuZhanjiBtn->setVisible(false);
        
        
        //消息
        menuMessageBtn = Button::create("RoomPublicH/menu_message.png", "RoomPublicH/menu_message_press.png");
        menuMessageBtn->setName("to_chat");
        menuMessageBtn->setAnchorPoint(Vec2(1,0.5));
        menuMessageBtn->setPosition(Vec2(-10, menuZhanjiBtn->getContentSize().height/2));
        menuMessageBtn->addClickEventListener(CC_CALLBACK_1(RoomPublic::buttonCallback, this));
        menuZhanjiBtn->addChild(menuMessageBtn);
//        menuMessageBtn->setVisible(false);
        
        auto shareBtn = Button::create("RoomPublicV/room_invite.png","RoomPublicV/room_invite_press.png");
        shareBtn->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
        shareBtn->setName("share");
        shareBtn->addClickEventListener(CC_CALLBACK_1(RoomPublic::buttonCallback, this));
        if(m_jsonData["gameRoom"]["aginstStatus"].asInt() != 0)shareBtn->setVisible(false);
        addChild(shareBtn,-100000);
        
        
        auto endRoomBtn = Button::create("RoomPublicV/diss_room.png","RoomPublicV/diss_room_press.png");
        endRoomBtn->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 105));
        endRoomBtn->setName("end_room");
        endRoomBtn->addClickEventListener(CC_CALLBACK_1(RoomPublic::buttonCallback, this));
        if(ZJHModel::getInstance()->uid == m_jsonData["gameRoom"]["user"]["id"].asInt()
           && m_jsonData["gameRoom"]["aginstStatus"].asInt() == 0){
            endRoomBtn->setVisible(true);
        }else{
            endRoomBtn->setVisible(false);
        }
        addChild(endRoomBtn,-100000);
        
        RoomChat2 *pRoomChat2 = RoomChat2::create();
        pRoomChat2->setName("room_chat");
        this->addChild(pRoomChat2);
    
        
//        RoomChatLayer* danmuLayout = RoomChatLayer::create();
//        danmuLayout->setAnchorPoint(Vec2(0,0));
//        danmuLayout->setPosition(Vec2(0,0));
//        danmuLayout->setContentSize(Size(winSize.width,winSize.height - 300));
//        addChild(danmuLayout);
    }
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomPublic::notificationRoomStart), "notify_against_room_start", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomPublic::notificationPlayerNumber), "notify_room_player_number", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomPublic::notificationGameResume), "notify_game_resume", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomPublic::notificationGameStart), "notify_game_check_end", NULL);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomPublic::checkEndRoomTip), "notify_room_check_ip", NULL);
    this->schedule(schedule_selector(RoomPublic::powerSigChange), 5.0);
    checkGameEnd();
    checkEndRoomTip();
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = [=](EventKeyboard::KeyCode keycode,Event* event)
    {
        if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
        {
//            RoomHExitDialog *exitDialog= RoomHExitDialog::create();
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
    this->schedule(schedule_selector(RoomPublic::getCurrentTime), 10);
    return true;
}

void RoomPublic::buttonCallback(cocos2d::Ref *sender)
{
    ui::Widget *btn = dynamic_cast<Widget *>(sender);
    if (btn) {
        std::string name = btn->getName();
        if (name == "menu_button")
        {
            int uid = m_jsonData["gameRoom"]["userId"].asInt();
            int type = ZJHModel::getInstance()->uid == uid ? 1 : 2;
            
            RoomMenu *roomMenu = RoomMenu::create();
            roomMenu->setName("RoomMenu");
            roomMenu->MyInit(type, m_aginstStatus == 0);
            roomMenu->setName("room_menu");
            roomMenu->setMenuCallback(CC_CALLBACK_1(RoomPublic::menuCallback, this));
            this->addChild(roomMenu, 100);
        }
        else if (name == "avatar_image")
        {
           auto roomInfo = RoomInformation::create();
            roomInfo->myInit(m_jsonData);
            this->addChild(roomInfo);
            roomInfo->show();
        }
        else if (name == "to_about")
        {
        }
        else if (name == "to_chat")
        {
            RoomChat2 *pRoomChat2 = getChildByName<RoomChat2 *>("room_chat");
            if (pRoomChat2 == NULL){
                pRoomChat2 = RoomChat2::create();
                pRoomChat2->setName("room_chat");
                this->addChild(pRoomChat2);
            }
            pRoomChat2->show();
        }
        else if (name == "to_result")
        {
            auto result = RoomRealTimeScore::create();
            result->myInit();
//            result->myInit(m_jsonData["gameRoom"]["id"].asInt());
            this->addChild(result);
            result->show();
        }
        else if (name == "to_invoice" || name == "share")
        {
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
            
        }
        else if (name == "menu_show"){
            auto menuAreaBg = getChildByName<ui::Scale9Sprite *>("menu_area");
            if (menuAreaBg->getContentSize().width == 190){
                menuMessageBtn->setVisible(true);
                menuZhanjiBtn->setVisible(true);
                menuShowBt->loadTextureNormal("RoomPublicH/menu_close.png");
                menuAreaBg->setContentSize(Size(365, 100));
            }else{
                menuMessageBtn->setVisible(false);
                menuZhanjiBtn->setVisible(false);
                menuShowBt->loadTextureNormal("RoomPublicH/menu_open.png");
                menuAreaBg->setContentSize(Size(190, 100));
            }
            menuMoreBtn->setPosition(Vec2(menuAreaBg->getContentSize().width-9, menuAreaBg->getContentSize().height/2));
        }
        else if (name == "game_help")
        {
            RoomRule *help = RoomRule::create();
            help->initHelp();
            help->setPosition(Vec2(0, 0));
            Director::getInstance()->getRunningScene()->addChild(help, 10000000);
        }
        
        else if (name == "end_room")
        {
            EndRoomTipsDialog *exitDialog= EndRoomTipsDialog::create();
                    exitDialog->init1();
//            exitDialog->init2("188.188.1.80 熊猫大侠\n188.188.1.80 熊猫大侠\n188.188.1.80 熊猫大侠\n188.188.1.80 熊猫大侠\n");
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
}

void RoomPublic::setOnlineNumber(int value)
{
    Label *numLabel = dynamic_cast<Label *>(getChildByName("num_label"));
    if (numLabel) {
        char buff[64];
        snprintf(buff, sizeof(buff), "%d/%d人", value, m_againstNumber);
        numLabel->setString(buff);
    }
}

void RoomPublic::setGameStatus(int status)
{
    if (status <= m_aginstStatus)
    {
        return;
    }
    if (m_aginstStatus == 0)
    {
        if (status == 1)
        {
            m_remain = m_duration * 60;
            this->unschedule(schedule_selector(RoomPublic::updateTimeLabel));
            this->schedule(schedule_selector(RoomPublic::updateTimeLabel), 1.0);
        }
        else if (status == 2)
        {
            this->unschedule(schedule_selector(RoomPublic::updateTimeLabel));
            Label *timeLabel = dynamic_cast<Label *>(this->getChildByName("time_label"));
            if (timeLabel)
            {
                timeLabel->setString("00:00:00");
            }
            
        }
    }
    else if (m_aginstStatus == 1)
    {
        this->unschedule(schedule_selector(RoomPublic::updateTimeLabel));
        Label *timeLabel = dynamic_cast<Label *>(this->getChildByName("time_label"));
        if (timeLabel)
        {
            timeLabel->setString("00:00:00");
        }
    }
    m_aginstStatus = status;
}

void RoomPublic::updateTimeLabel(float dt)
{
    m_remain--;
    Label *timeLabel = dynamic_cast<Label *>(this->getChildByName("time_label"));
    
    if (m_remain < 0)
    {
        if (timeLabel)
        {
            timeLabel->setString(Utils::getFormatedTime(0));
        }
        m_remain = 0;
        this->unschedule(schedule_selector(RoomPublic::updateTimeLabel));
        this->checkGameEnd();
        return;
    }
    
    if (timeLabel)
    {
        timeLabel->setString(Utils::getFormatedTime(m_remain));
    }
}

void RoomPublic::checkGameEnd()
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
                                                                 int status  = packet->recvVal["resultMap"]["status"].asInt();
                                                                 m_aginstStatus = status;
                                                                 refreshMenu();
                                                                 int type = packet->recvVal["resultMap"]["way"].asInt();
                                                                 m_type = type;
                                                                 Label *timeLabel = dynamic_cast<Label *>(this->getChildByName("time_label"));
                                                                 
                                                                 if (type == 1)
                                                                 {
                                                                     int curCount = packet->recvVal["resultMap"]["currentCount"].asInt();
                                                                     int allCount = packet->recvVal["resultMap"]["count"].asInt();
                                                                     
                                                                     if(timeLabel)timeLabel->setString(__String::createWithFormat(
                                                                        "局数 : %d/%d",curCount,allCount)->getCString());
                                                                     //发局数改变的通知出去
                                                                     __NotificationCenter::getInstance()->postNotification("notify_game_count_change",__String::create(packet->recvVal["resultMap"].toStyledString()));
                                                                     
                                                                     if (curCount == allCount)
                                                                     {
                                                                         this->schedule(schedule_selector(RoomPublic::checkGameEndRepeat), 5.0);
                                                                     }
                                                                     
                                                                     if (status == 2 || status == 3)
                                                                     {
                                                                         __NotificationCenter::getInstance()->postNotification("notify_against_room_end");
                                                                         this->unschedule(schedule_selector(RoomPublic::checkGameEndRepeat));
                                                                     }
                                                                     else if (status != 0)
                                                                     {
                                                                         
                                                                     }
                                                                 }
                                                                 else
                                                                 {
                                                                     int allTime = packet->recvVal["resultMap"]["time"].asInt();
                                                                     double leftTime = packet->recvVal["resultMap"]["leftTime"].asDouble();
                                                                     
                                                                     if (leftTime < 0) leftTime = 0;
                                                                     
                                                                     if (leftTime == 0)
                                                                     {
                                                                         this->schedule(schedule_selector(RoomPublic::checkGameEndRepeat), 5.0);
                                                                     }
                                                                     
                                                                     if (status == 2 || status == 3)
                                                                     {
                                                                         if(timeLabel)timeLabel->setString("00:00:00");
                                                                         __NotificationCenter::getInstance()->postNotification("notify_against_room_end");
                                                                         
                                                                         this->unschedule(schedule_selector(RoomPublic::checkGameEndRepeat));
                                                                     }
                                                                     else if (status != 0)
                                                                     {
                                                                         m_remain = leftTime / 1000;
                                                                         this->unschedule(schedule_selector(RoomPublic::updateTimeLabel));
                                                                         this->schedule(schedule_selector(RoomPublic::updateTimeLabel), 1.0);
                                                                         
                                                                         this->setGameStatus(status);
                                                                     }
                                                                     else
                                                                     {
                                                                         if(timeLabel)timeLabel->setString(Utils::getFormatedTime(allTime * 60));
                                                                     }
                                                                 }
                                                                 
                                                             }
                                                         }
                                                     }
                                                     
                                                     //删除packet
                                                     CCHttpAgent::getInstance()->packets.erase(tag);
                                                     delete packet;
                                                 }
                                             },
                                             "room/dz/status", json.toStyledString(), "room_status");
}


void RoomPublic::menuCallback(std::string name)
{
    if (name == "room_manage")
    {
        RoomManagement2 *management = RoomManagement2::create();
        management->myInit(m_jsonData);
        this->addChild(management, 100);
        
        management->show();
    }
    else if (name == "game_setting")
    {
        if (ZJHModel::getInstance()->gameUseDefaultSetting)
        {
            RoomSettingH *setting = RoomSettingH::create();
            setting->initSetting();
            this->addChild(setting, 100);
            return;
        }
        if(m_pCallback)m_pCallback("game_setting");
    }
    else if (name == "game_help")
    {
        RoomRule *help = RoomRule::create();
        help->initHelp();
        help->setPosition(Vec2(0, 0));
        this->addChild(help, 100);
    }
    else if (name == "down_table")
    {
        AlertDialogDownTable2 *alert = AlertDialogDownTable2::create();
        alert->setCallBack([=](int tag)
                           {
                               if (tag == ALERT_DIALOG_BUTTON_CONFIRM)
                               {
                                   if(m_pCallback)m_pCallback("down_table");
                               }
                           });
        this->addChild(alert, 1000000);
    }
    else if (name == "end_room")
    {
        EndRoomTipsDialog *exitDialog= EndRoomTipsDialog::create();
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
            if(m_pCallback)m_pCallback("exit");
            requestExit();
            CallFunc* func1 = CallFunc::create([=]
                                               {
                                                   ZJHModel::getInstance()->roomToHall();
                                               });
            runAction(Sequence::create(DelayTime::create(0.5),func1, NULL));
        }
        else
        {
            RoomHExitDialog *exitDialog= RoomHExitDialog::create();
            exitDialog->show();
            exitDialog->setCallback([=](std::string str)
                                    {
                                        if (str == "exit_confirm") {
                                            requestExit();
                                            if(m_pCallback)m_pCallback("exit");
                                            }
                                    });
            this->addChild(exitDialog, 100);
        }
    }
}

void RoomPublic::notificationRoomStart(cocos2d::Ref *msg)
{
    if (msg == NULL) {
        return;
    }
    
    __Integer *data = dynamic_cast<__Integer *>(msg);
    int roomId = data->getValue();
    
    if (roomId == m_jsonData["gameRoom"]["id"].asInt()) {
        checkGameEnd();
    }
}

void RoomPublic::refreshMenu(){
    auto roomButton = getChildByName("room_menu");
    if(roomButton != NULL){
        roomButton->removeFromParent();
        int uid = m_jsonData["gameRoom"]["userId"].asInt();
        int type = ZJHModel::getInstance()->uid == uid ? 1 : 2;
        RoomMenu *roomMenu = RoomMenu::create();
        roomMenu->MyInit(type, m_aginstStatus == 0);
        roomMenu->setName("room_menu");
        roomMenu->setMenuCallback(CC_CALLBACK_1(RoomPublic::menuCallback, this));
        this->addChild(roomMenu, 100);
    }
}

void RoomPublic::notificationPlayerNumber(cocos2d::Ref *msg)
{
    if (msg == NULL) {
        return;
    }
    
    __Integer *data = dynamic_cast<__Integer *>(msg);
    int num = data->getValue();
    
    this->setOnlineNumber(num);
}

void RoomPublic::notificationGameStart(Ref *msg)
{
    removeChildByName("share");
    removeChildByName("end_room");
    removeChildByName("roomHExitDialog");
//    RoomMenu * roomMenu = getChildByName<RoomMenu *>("room_menu");
//    if (roomMenu){
//        roomMenu->closeCallBack();
//
//    }
    this->checkGameEnd();
}

void RoomPublic::notificationGameResume(cocos2d::Ref *msg)
{
    this->checkGameEnd();
}

void RoomPublic::onExit()
{
    Layer::onExit();
    this->unschedule(schedule_selector(RoomPublic::powerSigChange));
    this->unschedule(schedule_selector(RoomPublic::getCurrentTime));
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

void RoomPublic::setMyVisible(string name){
    if(name == "code_layer"){
        auto child1 = this->getChildByName("game_help");
        auto child2 = this->getChildByName("pow_icon");
        auto child3 = this->getChildByName("current_time");
        auto child4 = this->getChildByName("invite_code");
        auto child5 = this->getChildByName("time_label");
        child1->setVisible(false);
        child2->setVisible(false);
        child3->setVisible(false);
        child4->setVisible(false);
        child5->setVisible(false);
    }
    if(name == "setting_layer"){
        auto child1 = this->getChildByName("menu_button");
        child1->setVisible(false);
    }
    if(name == "middle_layer"){
        auto child1 = this->getChildByName("share");
        auto child2 = this->getChildByName("end_room");
        child1->setVisible(false);
        child2->setVisible(false);
    }
    if(name == "to_chat"){
        auto child1 = menuZhanjiBtn->getChildByName("to_chat");
        child1->setVisible(false);
    }
    if(name == "to_result"){
        auto child1 = menuMoreBtn->getChildByName("to_result");
        child1->setVisible(false);
    }
    
}

void RoomPublic::checkGameEndRepeat(float dt)
{
    checkGameEnd();
}

void RoomPublic::showPlayerList(){
    auto result = RoomRealTimeScore::create();
    result->myInit();
    this->addChild(result);
    result->show();
}

void RoomPublic::requestExit(){
    if(!needRequestExist){
        return;
    }
    Json::Value json;
    json["roomId"] = m_jsonData["gameRoom"]["id"].asInt();
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"duiju/invite/out",json.toStyledString(),"requestExit");
}

void RoomPublic::setRequestFlag(bool flag){
    needRequestExist = flag;
}

void RoomPublic::checkEndRoomTip(Ref *msg)
{
    //封装获取玩家的请求
    Json::Value json;
    json["roomId"] = m_jsonData["gameRoom"]["id"].asInt();
    json["UserKey"] = ZJHModel::getInstance()->UserKey;
    
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"duiju/invite/lookList", json.toStyledString(), "room_all_player");
    
    //获取返回结果
    this->schedule(schedule_selector(RoomPublic::checkEndRoomTip_getData), 0.0f);
}

//获取电量和wifi手机信号强度
void RoomPublic::powerSigChange(float d){
//    //手机信号强度
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
        __NotificationCenter::getInstance()->postNotification("notify_game_power_change",__String::create(json.toStyledString()));
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
void RoomPublic::checkEndRoomTip_getData(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["room_all_player"];
    if (loginPacket && loginPacket->status != 0)
    {
        unschedule(schedule_selector(RoomPublic::checkEndRoomTip_getData));
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
                
                char buf[512];
                bool isIpTogether = false;
                bool isPathTogether = false;
                bool pathUnreach = false;
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
                
                //计算是否有小于50米的敌人
                Json::Value tempData;
                int a = 1;
                for (int i = 0; i < realData.size(); i++) {
                    Json::Value temp = realData[i];
                    if (!temp["latitude"].empty() && !temp["longitude"].empty() && temp["city"].asString() != "") {
                        tempData[a-1] = temp;
                    }
                    if(temp["city"].asString() == ""){
                        char bf[512];
                        memset(bf, 0, sizeof(bf));
                        Utils::parseName(6,bf,temp["nickName"].asString().c_str());
                        tip<<bf<<":检测不到该玩家位置\n";
                        pathUnreach = true;
                    }
                    a++;
                }
                
                vector<map<vector<int>,int>> mapGather;
                Utils::searchGatherWithPath(mapGather, tempData,5);
                if(mapGather.size() > 0){
                    for (int i = 0; i < mapGather.size(); i++) {
                        std::map<vector<int>,int> temp = mapGather[i];
                        std::map<vector<int>,int>::iterator it;
                        for(it=temp.begin();it!=temp.end();it++)
                        {
                            string nn = "";
                            for (int j = 0; j < it->first.size(); j++) {
                                Json::Value val = realData[it->first[j]];
                                char buff[512];
                                memset(buff, 0, sizeof(buff));
                                Utils::parseName(6,buff,val["nickName"].asString().c_str());
                                if (j != it->first.size() - 1) {
                                    snprintf(buff, sizeof(buff), "%s、",buff);
                                } else {
                                    snprintf(buff, sizeof(buff), "%s",buff);
                                }
                                nn += buff;
                            }
                            stringstream stream;
                            stream<<it->second;
                            tip<<nn<<"相距"<<stream.str()<< "米\n";
                        }
                    }
                    isPathTogether = true;
                }
                //查看是否有获取不到位置的玩家
                int uid = m_jsonData["gameRoom"]["userId"].asInt();
                string titleTips = "";
//                string jiesanTips = ZJHModel::getInstance()->uid == uid ? "解散" : "退出";
                string jiesanTips = "解散";
                if(isIpTogether && !isPathTogether){
                    titleTips = "发现相同IP玩家,是否"+jiesanTips+"房间?";
                }else if(isIpTogether && isPathTogether){
                    titleTips = "发现相同IP,距离较近的玩家,是否"+jiesanTips+"房间?";
                }else if(!isIpTogether && isPathTogether){
                    titleTips = "发现距离较近的玩家,是否"+jiesanTips+"房间?";
                }else if(pathUnreach){
                    titleTips = "有未检测到位置信息的玩家,是否"+jiesanTips+"房间?";
                }
                
                if ((isIpTogether || isPathTogether || pathUnreach) && getChildByName("EndRoomDialog") == NULL)
                {
                    EndRoomTipsDialog *exitDialog= EndRoomTipsDialog::create();
                    exitDialog->init2(titleTips,tip.str(),true);//如果是房主则显示解散房间  如果不是则显示退出房间
                    exitDialog->setName("EndRoomDialog");
                    exitDialog->setCallback([=](std::string str)
                                            {
                                                if (str == "end_confirm")
                                                {
                                                    if(m_pCallback)m_pCallback("end_room");
                                                }else if(str == "exit"){
                                                    if(m_pCallback)m_pCallback("exit");
                                                    requestExit();
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
            else
            {
                
            }
        }
        loginPacket->status = 0;
        CCHttpAgent::getInstance()->packets["room_all_player"] = NULL;
        delete loginPacket;
    }
}

void RoomPublic::getCurrentTime(float d){
    //获取当前时间
    time_t t;
    struct tm *p;
    char buf[128];
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    p = localtime(&tv.tv_sec);
    strftime(buf, sizeof(buf), "%H:%M", p);
    cTime->setString(buf);
}


void RoomPublic::inviteShare(){
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
}

