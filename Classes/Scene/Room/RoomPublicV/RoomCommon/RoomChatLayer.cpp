//
//  RoomChatLayer.cpp
//  ZJH
//
//  Created by hyhy on 16/7/25.
//
//

#include "RoomChatLayer.h"
#include "../Utils/ChatUtils.hpp"
USING_NS_CC;

bool RoomChatLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomChatLayer::notificationCallback), "notify_room_chat", NULL);
    return true;
}

void RoomChatLayer::notificationCallback(cocos2d::Ref *msg)
{
    if (msg == NULL) {
        return;
    }
    
    __String *str = dynamic_cast<__String *>(msg);
    Json::Value json = Utils::ParseJsonStr(str->getCString());
    int uid = json["uid"].asInt();
    std::string name = json["name"].asString();
    std::string content = json["content"].asString();
    
    ShowMoveChat(uid, name, content);
}

void RoomChatLayer::ShowMoveChat(int uid, std::string name, std::string content)
{
    Size winSize = Director::getInstance()->getWinSize();
    Sprite *container = Sprite::create();
    this->addChild(container, 100);
    float totalWidth = 0;
    
    ui::RichText* contentLabel = ui::RichText::create();
    contentLabel->setAnchorPoint(Vec2(0, 0));
    contentLabel->setFontSize(24);
    contentLabel->setColor(Color3B::WHITE);
    ChatUtils::componentsSeparatedByString_roomchat(content,contentLabel,24, Color3B::WHITE);
    contentLabel->formatText();
    container->addChild(contentLabel);
    totalWidth += contentLabel->getContentSize().width;
    
    Label *nameLabel = Label::createWithSystemFont(name + ":", "Arial", 24);
    nameLabel->setColor(Color3B(181, 243, 255));
    nameLabel->setAnchorPoint(Vec2(0, 0));
    nameLabel->setPosition(Vec2(0, 0));
    container->addChild(nameLabel);
    totalWidth += nameLabel->getContentSize().width;
    
    contentLabel->setPosition(Vec2(nameLabel->getContentSize().width, 0));
    
    if (ZJHModel::getInstance()->uid == uid)
    {
        nameLabel->setColor(Color3B::GREEN);
    }
    else
    {
        nameLabel->setColor(Color3B(181, 243, 255));
    }
    
    Vec2 begin = Vec2(winSize.width, random(winSize.height * 0.7, winSize.height * 0.9));
    container->setPosition(begin);
    container->runAction(Sequence::create(MoveBy::create(6, Vec2(-winSize.width - totalWidth, 0)),
                                          RemoveSelf::create(),
                                          NULL));
}

Point RoomChatLayer::GetChatPos(int nums)
{
    
    Size size = Director::getInstance()->getWinSize();
    Point p=Vec2(size.width,size.height-50);
    if (nums ==1) {
        p=Vec2(size.width,size.height-130);
    }
    else if (nums ==2)
    {
        p=Vec2(size.width,size.height/2);
    }
    else if (nums ==3)
    {
        p=Vec2(size.width,200);
    }
    return p;
}

void RoomChatLayer::onExit()
{
    __NotificationCenter::getInstance()->removeAllObservers(this);
    Layer::onExit();
}
