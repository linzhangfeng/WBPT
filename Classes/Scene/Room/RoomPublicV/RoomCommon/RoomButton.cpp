//
//  RoomButton.cpp
//  ZJH
//
//  Created by wuyu on 16/3/15.
//
//

#include "RoomButton.h"
#include "RoomCMConfig.h"
#include "ZJH.h"

using namespace cocos2d;

static const float scaleRate = 0.67;

RoomButton::RoomButton()
    :m_menuCallback(NULL)
{
    
}

bool RoomButton::init()
{
    if (!Layer::init())
    {
        return false;
    }
   
    auto touchListener =EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomButton::onTouchBegan,this);
    touchListener->onTouchMoved =CC_CALLBACK_2(RoomButton::onTouchMoved, this);
    touchListener->onTouchEnded =CC_CALLBACK_2(RoomButton::onTouchEnded, this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(RoomButton::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    return true;
}

void RoomButton::MyInit(int type ,bool isCMRoom)
{
    //type 1：房主（有管理房间功能  只有解散房间功能  没有退出功能） 2：房客  游戏开始后没有退出房间功能
    Size winSize = Director::getInstance()->getWinSize();
    int itemNums = 2;
    
    Size dialogSize = Size(430 * scaleRate, (126 * itemNums + 30) * scaleRate);
    
    Sprite *dialog = Sprite::create();
    dialog->setContentSize(dialogSize);
    dialog->setAnchorPoint(Vec2(1, 1));
    dialog->setName("dialog");
    dialog->setPosition(Vec2(winSize.width - 25, winSize.height - 98));
    this->addChild(dialog);
    
    Sprite *bgSp = Sprite::create("RoomHead/bar_bg.png");
    bgSp->setScale(dialogSize.width / bgSp->getContentSize().width, dialogSize.height / bgSp->getContentSize().height);
    bgSp->setPosition(dialogSize / 2);
    dialog->addChild(bgSp);
    
    char buff[128];
    float posX = dialogSize.width / 2;
    float posY = 63 * scaleRate;
    float deltaY = posY * 2;
    //在没有开始游戏的时候  解散房间只有房住才有此功能   开始后就所有人都有解散功能
    //房主不能退出房间，只能解散房间
    //普通玩家游戏开始前可以退出房间  开始后不能退出  无退出按钮
    //去掉所有下桌按钮  都没有下桌功能
    std::string names[4] = {
        "game_setting",
        "end_room",
//        "out_seat_room",
        "exit_room",
    };
    for (int i = 3; i >= 1; i--)
    {
        if((!(type == 1 || !isCMRoom)) && (names[i - 1] == "end_room")){
            continue;
            
        }
        if((names[i - 1] == "exit_room") && !isCMRoom){
            continue;
        }
        if((names[i - 1] == "exit_room") && type == 1){
            continue;
        }
        
        
        Button *btn = Button::create("RoomHead/bar_btn_check2_void.png", "RoomHead/bar_btn_check2.png");
        btn->setPosition(Vec2(posX, posY));
        btn->addClickEventListener(CC_CALLBACK_1(RoomButton::buttonClicked, this));
        btn->setName(names[i - 1]);
        btn->setScale(scaleRate);
        dialog->addChild(btn);
        
        snprintf(buff, sizeof(buff), "RoomHead/room_menu_%s.png", names[i - 1].c_str());
        Sprite *wordSp = Sprite::create(buff);
        wordSp->setPosition(btn->getContentSize() / 2);
        btn->addChild(wordSp, 1);
        
        if (i != 5)
        {
            Sprite *line = Sprite::create("RoomHead/bar_line.png");
            line->setScale(scaleRate);
            line->setPosition(Vec2(posX, posY - deltaY * 0.5));
            dialog->addChild(line);
        }
        
        posY += deltaY;
    }
}

void RoomButton::buttonClicked(cocos2d::Ref *ref)
{
    Button *btn = dynamic_cast<Button *>(ref);
    if (btn) {
        std::string name = btn->getName();
        
        this->closeCallBack();
        
        if (m_menuCallback) {
            m_menuCallback(name);
        }
    }
}

void RoomButton::closeCallBack()
{
    runAction(RemoveSelf::create());
}

bool RoomButton::isTouchInside(Touch* touch)
{
    Point touchPoint = touch->getLocation();
    Node * dialog = dynamic_cast<Node *>(this->getChildByName("dialog"));
    
    return dialog->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation()));
}

bool RoomButton::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void RoomButton::onTouchMoved(Touch* touch, Event* event)
{
    
}

void RoomButton::onTouchEnded(Touch* touch, Event* event)
{
    if (!isTouchInside(touch)) {
        this->closeCallBack();
    }
}

void RoomButton::onKeyReleased(EventKeyboard::KeyCode keycode,Event* event)
{
    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
    {
        this->closeCallBack();
    }
    event->stopPropagation();
}
