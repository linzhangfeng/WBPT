//
//  RoomBottom.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "RoomBottom.hpp"
#include "../RoomChat/RoomChat.hpp"
#include "../../../../Model/MsgModel.h"
#include "RoomResultLayer.h"
#include "RoomTakeinLayer.h"
#include "RoomInviteLayer.h"
#include "RoomShopLayer.h"
#include "../../../Msg/ChatRecordEffect.hpp"
#include "../RoomCommon/EndRoomDialog.h"
bool RoomBottom::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    addChild(ChatRecordEffect::create(),100000);
    
    ss = 0.67;
    
    Size size = Director::getInstance()->getWinSize();
    
    m_jsonData = ZJHModel::getInstance()->roomData;
    
    Json::Value json;
    json["uid"] = 1000;
    
    auto rc = RoomChat::create();
    rc->myInit(json);
    addChild(rc,100);
    /////////////////////新布局样式//////////////////////////
    
    {
        auto shareBtn = Button::create("RoomPublicV/room_invite.png","RoomPublicV/room_invite_press.png");
        shareBtn->setPosition(Vec2(size.width / 2, size.height / 2 - 50));
        shareBtn->setName("share");
        shareBtn->addClickEventListener(CC_CALLBACK_1(RoomBottom::btnCallback, this));
        if(m_jsonData["aginstStatus"].asInt() != 0)shareBtn->setVisible(false);
        addChild(shareBtn);
        
        //当是房主时才显示解散按钮
        int uid = m_jsonData["gameRoom"]["userId"].asInt();
        auto dissmissRoomBtn = Button::create("RoomPublicV/diss_room.png","RoomPublicV/diss_room_press.png");
        dissmissRoomBtn->setPosition(Vec2(size.width / 2, size.height / 2 - 170));
        dissmissRoomBtn->setName("diss_room");
        dissmissRoomBtn->addClickEventListener(CC_CALLBACK_1(RoomBottom::btnCallback, this));
        if(m_jsonData["aginstStatus"].asInt() != 0 || ZJHModel::getInstance()->uid != uid)dissmissRoomBtn->setVisible(false);
        addChild(dissmissRoomBtn);
    }
    
    //底部工具栏背景
    Layout* bottomBarBg = Layout::create();
    bottomBarBg->setContentSize(Size(size.width,100));
    bottomBarBg->setAnchorPoint(Vec2(0,0));
    bottomBarBg->setPosition(Vec2(0,0));
    addChild(bottomBarBg,-100);
    
    //聊天按钮
    auto chatBtn = Button::create("RoomPublicV/room_bottom_chat.png", "RoomPublicV/room_bottom_chat_press.png");
    chatBtn->setAnchorPoint(Vec2(0,0));
    chatBtn->setPosition(Vec2(0,0));
    chatBtn->addClickEventListener([=](Ref* rf){
                    rc->show();
                    rc->showFastChatWindow();
                });
    bottomBarBg->addChild(chatBtn);
    
    //入座按钮
    seatBtn = Button::create("RoomPublicV/seat_down.png", "RoomPublicV/seat_down_press.png");
    seatBtn->setAnchorPoint(Vec2(0.5,0));
    seatBtn->setPosition(Vec2(bottomBarBg->getContentSize().width/2,12));
    seatBtn->setName("seatBtn");
    bottomBarBg->addChild(seatBtn);
    
    seatBtn->addClickEventListener(CC_CALLBACK_1(RoomBottom::btnCallback, this));
    
    //语音按钮
    auto voiceBtn = Button::create("RoomPublicV/room_bottom_voice.png", "RoomPublicV/room_bottom_voice_press.png");
    voiceBtn->setAnchorPoint(Vec2(1,0));
    voiceBtn->setPosition(Vec2(bottomBarBg->getContentSize().width,0));
    bottomBarBg->addChild(voiceBtn);
    voiceBtn->addTouchEventListener([=](Ref* r,Widget::TouchEventType type){
        if (type == Widget::TouchEventType::BEGAN)
        {
            PlatformHelper::startRecord();
        }
        else if (type == Widget::TouchEventType::ENDED || type == Widget::TouchEventType::CANCELED)
        {
            PlatformHelper::stopRecord();
            __NotificationCenter::getInstance()->postNotification("chat_sound_record_stop_notify");
        }
     });
    
    auto bottomStype = ImageView::create("RoomPublicV/bottom_stype.png");
    Size bSize = bottomStype->getContentSize();
    bottomStype->setContentSize(Size(bottomBarBg->getContentSize().width - voiceBtn->getContentSize().width - chatBtn->getContentSize().width,bSize.height));
    bottomStype->setScale9Enabled(true);
    bottomStype->setAnchorPoint(Vec2(0.5,0));
    bottomStype->setPosition(Vec2(bottomBarBg->getContentSize().width/2,0));
    bottomBarBg->addChild(bottomStype);
    
     __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomBottom::notificationGameStart), "notify_game_check_end", NULL);
    return true;
}

void RoomBottom::setFastSoundVis(bool v)
{
//    if (v)
//    {
//        fastSound->setVisible(true);
//        fastChat->setVisible(false);
//    }
//    else
//    {
//        fastSound->setVisible(false);
//        fastChat->setVisible(true);
//    }
}

void RoomBottom::setGamePlayStatu(bool s){
    if(s){
        //在桌上
        seatBtn->setVisible(false);
    }else{
        seatBtn->setVisible(true);
    }
}

void RoomBottom::notificationGameStart(Ref *msg)
{
    removeChildByName("share");
    removeChildByName("diss_room");
}

void RoomBottom::btnCallback(Ref* ref)
{
    Node* n = (Node*)ref;
    string name = n->getName();
    if (name == "to_fastchat")
    {
        Size size = Director::getInstance()->getWinSize();
        auto bg = Layout::create();
        bg->setContentSize(size);
        bg->setTouchEnabled(true);
        bg->setSwallowTouches(true);
        bg->addClickEventListener([=](Ref* rf){
            bg->removeFromParentAndCleanup(true);
        });
        addChild(bg,10000);
        
        Size itemSize = Size(size.width - 100,5 * 100);
        
        auto send_bg = ui::Scale9Sprite::create("RoomBottom/btn_gray_small.png");
        send_bg->setPreferredSize(itemSize + Size(0,50));
        send_bg->setAnchorPoint(Vec2(0.5, 0.0));
        send_bg->setPosition(Vec2(size.width / 2, 130 - 25));
        bg->addChild(send_bg,-100);
        
        ListView* listView = ListView::create();
        listView->setDirection(ui::ScrollView::Direction::VERTICAL);
        listView->setBounceEnabled(false);
        listView->setContentSize(itemSize);
        listView->setAnchorPoint(Vec2(0.5, 0));
        listView->setPosition(Vec2(size.width / 2, 130));
        listView->setScrollBarEnabled(false);
        listView->setName("list");
        bg->addChild(listView);
        listView->setItemsMargin(0);
        std::vector<std::string> strs;
        strs.clear();
        for (int i = 0; i < strs.size(); i ++)
        {
            Text* text = Text::create(strs[i], "AmericanTypewriter", 30);
            text->setAnchorPoint(Vec2(0, 0.5));
            text->setColor(Color3B(169,169,169));
            text->setPosition(Vec2(50, 100 / 2));
            
            auto layout = Layout::create();
            layout->setContentSize(Size(itemSize.width,100));
            layout->addChild(text);
            listView->addChild(layout);
            layout->setTouchEnabled(true);
            layout->addClickEventListener([=](Ref* rf){
                MsgModel::getInstance()->sendText_group(strs[i]);
                bg->removeAllChildrenWithCleanup(true);
            });
            
            LayerColor* line = LayerColor::create(Color4B(50, 50, 50, 255));
            line->setContentSize(Size(itemSize.width - 100,2));
            line->setAnchorPoint(Vec2(0.0f, 0.0f));
            line->setPosition(50,0);
            layout->addChild(line, -100);
        }
    }
    else if (name == "to_result")
    {
        RoomResultLayer *result = RoomResultLayer::create();
        result->myInit(m_jsonData["gameRoom"]["id"].asInt());
        this->addChild(result);
        result->show();
    }
    else if (name == "to_takein")
    {
        RoomTakeinLayer *takein = RoomTakeinLayer::create();
        takein->myInit(m_jsonData["gameRoom"]["id"].asInt());
        this->addChild(takein);
        takein->show();
    }
    else if (name == "share" || name == "to_invoice")
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
    else if(name == "outSeatBtn"){
        //离开座位对游戏进行回调
        if (m_pCallback)m_pCallback("outSeatBtn");
    }
    else if(name == "seatBtn"){
        //入座对游戏进行回调
        if (m_pCallback)m_pCallback("seatBtn");
    }else if (name == "diss_room")
    {
        EndRoomDialog *exitDialog= EndRoomDialog::create();
        exitDialog->init1();
//                    exitDialog->init2("188.188.1.80 熊猫大侠\n188.188.1.80 熊猫大侠\n188.188.1.80 熊猫大侠\n188.188.1.80 熊猫大侠\n");
        exitDialog->setCallback([=](std::string str)
                                {
                                    if (str == "end_confirm")
                                    {
                                        if(m_pCallback)m_pCallback("end_room");
                                    }
                                });
        this->addChild(exitDialog, 100);
    }
    else if (name == "to_shop")
    {
        PlatformHelper::showToast("暂未开启，敬请期待");
        return;
        RoomShopLayer *shop = RoomShopLayer::create();
        this->addChild(shop);
        shop->show();
        
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
                                                                     shop->myInitWealth(packet->recvVal["resultMap"]);
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
                                                 "golden/userWealth", "", "wealth3");
        
        Json::Value json;
        json["type"] = "JB";
        
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
                                                                     shop->myInitShop(packet->recvVal["resultMap"]);
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
                                                 "store/find.action", json.toStyledString(), "store_find");
    }
}

void RoomBottom::onExit()
{
    Layer::onExit();
    
    __NotificationCenter::getInstance()->removeAllObservers(this);
}
