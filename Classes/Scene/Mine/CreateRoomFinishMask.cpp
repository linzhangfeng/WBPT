//
//  CreateRoomFinishMask.cpp
//  ZJH
//
//  Created by apple on 16/7/12.
//
//

#include "CreateRoomFinishMask.hpp"
#define CELLH      400
#define INTER_MAX        30

bool CreateRoomFinishMask::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    // 创建一个事件监听器类型为 OneByOne 的单点触摸
    auto listener1 = EventListenerTouchOneByOne::create();
    // 设置是否吞没事件，在 onTouchBegan 方法返回 true 时吞没
    listener1->setSwallowTouches(true);
    
    // 使用 lambda 实现 onTouchBegan 事件回调函数
    listener1->onTouchBegan = [](Touch* touch, Event* event) -> bool {
        // 获取事件所绑定的 target
        return true;
    };
    
    // 点击事件结束处理
    listener1->onTouchEnded = [=](Touch* touch, Event* event)
    {
        this->removeFromParent();
        
    };
    EventDispatcher * eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
    auto layout = Layout::create();
    layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    layout->setBackGroundColor(Color3B(255, 255, 255));
    layout->ignoreAnchorPointForPosition(false);
    //layout->setAnchorPoint(Vec2(0, 1));
    layout->setContentSize(Size(visibleSize.width, CELLH));
    layout->setPosition(Vec2(0, 0));
    this->addChild(layout);
    
    
    Text *text = Text::create("创建成功，是否立即进入房间", "AmericanTypewriter", 35);
    text->setAnchorPoint(Vec2(0, 1));
    text->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    text->setPosition(Vec2(INTER_MAX, CELLH - 50));
    text->setTextColor(Color4B::BLACK);
    layout->addChild(text);
    
    auto sure = Layout::create();
    sure->setTouchEnabled(true);
    sure->addTouchEventListener(CC_CALLBACK_2(CreateRoomFinishMask::sureOn, this));
    sure->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    sure->setBackGroundColor(Color3B(255, 255, 255));
    sure->ignoreAnchorPointForPosition(false);
    sure->setAnchorPoint(Vec2(0, 1));
    sure->setContentSize(Size(visibleSize.width/2, 120));
    sure->setPosition(Vec2(INTER_MAX, CELLH/2));
    layout->addChild(sure);
    
    Text *text1 = Text::create("确 认", "AmericanTypewriter", 35);
    text1->setTextColor(Color4B::BLACK);
    text1->setPosition(Vec2(sure->getContentSize().width/2, sure->getContentSize().height/2));
    text1->setTextColor(Color4B::BLACK);
    sure->addChild(text1);
    
    auto exit = Layout::create();
    exit->setTouchEnabled(true);
    exit->addTouchEventListener(CC_CALLBACK_2(CreateRoomFinishMask::exitOn, this));
    exit->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    exit->setBackGroundColor(Color3B(255, 255, 255));
    exit->ignoreAnchorPointForPosition(false);
    exit->setAnchorPoint(Vec2(1, 1));
    exit->setContentSize(Size(visibleSize.width/2, 120));
    exit->setPosition(Vec2(visibleSize.width - INTER_MAX, CELLH/2));
    layout->addChild(exit);
    
    Text *text2 = Text::create("取 消", "AmericanTypewriter", 35);
    text2->setTextColor(Color4B::BLACK);
    text2->setPosition(Vec2(exit->getContentSize().width/2, exit->getContentSize().height/2));
    text2->setTextColor(Color4B::BLACK);
    exit->addChild(text2);
    
    return true;
}

void CreateRoomFinishMask::sureOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        printf("进入对应的房间");
    }
}
void CreateRoomFinishMask::exitOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        this->removeFromParent();
    }
}
