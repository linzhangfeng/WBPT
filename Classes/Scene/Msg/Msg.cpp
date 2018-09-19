//
//  Msg.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "Msg.hpp"
#include "ZJH.h"
#include "ChatSession.hpp"
#include "MyFriends.hpp"

bool Msg::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setName("Msg");
    
    Size size = Director::getInstance()->getWinSize();
    
    setHead(270);
    
    setBackground(Color4B(29, 29, 31, 255));
    
    setTitle("社区");
    
    setMore_img("Msg/icon_nav_01.png","Msg/icon_nav_01.png", [=](){
        
        ZJHModel::getInstance()->gotoView(VIEW_ADD_FRIENDS);
    });
    
//    setReturn_img("Msg/trible_icon.png", "Msg/trible_icon.png", [=](){
//        ZJHModel::getInstance()->gotoView(VIEW_MY_BULUO);
//    });

    
    int dx = 334;
    int bx = size.width / 2;
    int by = size.height - getHeadH() + 90;
    
    {
        Button* button = Button::create("Msg/msg1.png",
                                        "Msg/msg2.png",
                                        "Msg/msg2.png");
        button->setPosition(Vec2(bx - 5,by));
        button->setAnchorPoint(Vec2(1,0.5));
        button->setEnabled(false);
        button->addClickEventListener(CC_CALLBACK_1(Msg::btnCallback, this));
        button->setName("msg");
        addChild(button,1000);
    }
    
    {
        Button* button = Button::create("Msg/friend1.png",
                                        "Msg/friend2.png",
                                        "Msg/friend2.png");
        button->setPosition(Vec2(bx + 5,by));
        button->setAnchorPoint(Vec2(0,0.5));
//        button->setContentSize(Size(clickW,clickH));
//        button->ignoreContentAdaptWithSize(false);
//        button->setFillContentSizeEnabled(false);
        button->addClickEventListener(CC_CALLBACK_1(Msg::btnCallback, this));
        button->setName("friend");
        addChild(button,1000);
        
        bx += dx;
    }
    
//    {
//        Button* button = Button::create("Msg/group1.png",
//                                        "Msg/group2.png",
//                                        "Msg/group2.png");
//        button->setPosition(Vec2(bx,by));
//        button->addClickEventListener(CC_CALLBACK_1(Msg::btnCallback, this));
//        button->setName("group");
//        addChild(button,1000);
//        
//        bx += dx;
//    }
    
    ChatSession* hall = ChatSession::create();
    addChild(hall);
    itemMap["msg"] = hall;
    return true;
}

void Msg::btnCallback(Ref* ref)
{
    Button* btn = (Button*)ref;
    
    if (btn->getName() == "group")
    {
        PlatformHelper::showToast("暂未开放，敬请期待");
        return;
    }
    
    if (btn->getName() == "friend" && ZJHModel::getInstance()->isYoukeLogin())
    {
        ZJHModel::getInstance()->gotoView(VIEW_LOAD);
        return;
    }
    
    Button* msg = (Button*)getChildByName("msg");
    Button* friends = (Button*)getChildByName("friend");
    Button* group = (Button*)getChildByName("group");
    
    if(msg)msg->setEnabled(true);
    if(friends)friends->setEnabled(true);
    if(group)group->setEnabled(true);
    
    if(btn)btn->setEnabled(false);
    
    if(itemMap["msg"])itemMap["msg"]->setVisible(false);
    if(itemMap["friend"])itemMap["friend"]->setVisible(false);
    if(itemMap["group"])itemMap["group"]->setVisible(false);
    
    string tag = btn->getName();
    
    if (tag == "msg")
    {
        if(itemMap["msg"])
        {
            itemMap["msg"]->setVisible(true);
            ChatSession* hall = (ChatSession*)itemMap["msg"];
            hall->hasUpdate = true;
        }
        else
        {
            ChatSession* hall = ChatSession::create();
            addChild(hall);
            itemMap["msg"] = hall;
        }
    }
    else if (tag == "friend")
    {
        if(itemMap["friend"])
        {
            itemMap["friend"]->setVisible(true);
        }
        else
        {
            MyFriends* msg = MyFriends::create();
            addChild(msg);
            itemMap["friend"] = msg;
        }
    }
}
