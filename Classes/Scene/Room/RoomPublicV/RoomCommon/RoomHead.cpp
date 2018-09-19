//
//  RoomHead.cpp
//  ZJH
//
//  Created by Apple on 16-3-11.
//
//

#include "RoomHead.h"
#include "RoomButton.h"
#include "ZJH.h"

using namespace cocos2d;



bool RoomHead::init()
{
    Size size = Director::getInstance()->getWinSize();
    isMenuDown = false;
    Sprite* bg = Sprite::create("Hall/top-bg.png");
    bg->setAnchorPoint(Vec2(0.5f, 1.0f));
    bg->setScaleX(size.width / bg->getContentSize().width);
    bg->setPosition(size.width / 2,size.height);
    this->addChild(bg, -50);
    
    bg = Sprite::create("Hall/laba-bg.png");
    bg->setAnchorPoint(Vec2(0.0f, 0.5f));
    bg->setScaleX(Utils::getScaleX() * 0.95);
    bg->setPosition(Vec2(63 - 51 + 50, size.height - 24));
    this->addChild(bg, -50);
    
//    auto help = MyButton::createWithFile("Room/room_btn_help.png", "Room/room_btn_help.png");
//    help->setPosition(Vec2(30, size.height - 24));
//    help->setCallback([](int tag) {
//        AlertDialog* dl = AlertDialog::createDialog();
//        dl->AlertRoomHelp();
//    });
//    addChild(help);
    
//    Marquee* m = Marquee::create();
//    m->initWithSize(Size(850 * Utils::getScaleX(),24));
//    m->setPosition(Vec2(63 + 50, size.height - 24));
//    addChild(m);
    
//    MyButton* laba_btn = MyButton::createWithSize(m->getContentSize());
//    laba_btn->setTag(RoomCMConfig::ROOMCOMMON_LABA);
//    laba_btn->setAnchorPoint(Vec2(0.0, 0.5f));
//    laba_btn->setPosition(m->getPosition());
//    laba_btn->setCallback(CC_CALLBACK_1(RoomHead::btnCallback, this));
//    addChild(laba_btn);
    MyButton* btn;
    {
        btn = MyButton::createWithFile("Hall/chat.png", "Hall/chat1.png");
        btn->setPosition(Vec2(size.width - 190,size.height - 32));
        btn->setTag(RoomCMConfig::ROOMCOMMON_CHAT);
        btn->setCallback(CC_CALLBACK_1(RoomHead::btnCallback, this));
        addChild(btn);
    }
    
    {
        btn = MyButton::createWithFile("Hall/msg.png", "Hall/msg1.png");
        btn->setPosition(Vec2(size.width - 118 + 5,size.height - 32));
        btn->setTag(RoomCMConfig::ROOMCOMMON_MSG);
        btn->setCallback(CC_CALLBACK_1(RoomHead::btnCallback, this));
        addChild(btn);
    }
    {
        btn = MyButton::createWithFile("Room/room_menu_btn.png", "Room/room_menu_btn_h.png");
        btn->setPosition(Vec2(size.width - 46,size.height - 32));
        btn->setTag(RoomCMConfig::ROOMCOMMON_MENU);
        btn->setCallback(CC_CALLBACK_1(RoomHead::btnCallback, this));
        addChild(btn);
    }
    {
        //帮助按钮
//        auto help = ui::Button::create("Common/common_help01.png");
//        help->setPosition(Vec2(30, size.height - 25));
//        help->addClickEventListener([=](Ref* sender){
//            AlertDialog* dl = AlertDialog::createDialog();
//            //        //        dl->AlertHtml5ByURL("http:://www.baidu.com", Size(0,0));
//            dl->AlertRoomHelp();
//        });
//        addChild(help);
    }
    return true;
}



void RoomHead::RoomBtnCallBack(int tag)
{
    if (callback) {
        btnCallback(tag);
    }
}

void RoomHead::btnCallback(int tag)
{
    
    if (tag == RoomCMConfig::ROOMCOMMON_MSG)
    {
//        Msg::showMsg();
    }
    else if (tag == RoomCMConfig::ROOMCOMMON_LABA)
    {
//        ChatLayer::show();
    }
    else{
        if (callback) {
            callback(tag);
        }
    }
}

void RoomHead::setCallback(const MyCallBack_Int &_callback)
{
    callback = _callback;
}

