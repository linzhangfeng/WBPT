//
//  ServerErrorTips.cpp
//  ZJH
//
//  Created by mac on 2017/1/18.
//
//

#include "ServerErrorTips.hpp"

ServerErrorTips* ServerErrorTips::create()
{
    ServerErrorTips *pRet = new(std::nothrow) ServerErrorTips();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool ServerErrorTips::init()
{
    if (!Layer::init())
    {
        return false;
    }

    return true;
}

void ServerErrorTips::setContent(string content){
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout *mask = Layout::create();
    mask->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    mask->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    mask->setBackGroundColorOpacity(150);
    mask->setContentSize(visibleSize);
    this->addChild(mask);
    
    title2 = LabelTTF::create(content, "", 40,Size(600, 0), TextHAlignment::LEFT);
    title2->setAnchorPoint(Vec2(0.5, 1));
    title2->setColor(Color3B(0xd9, 0xd9, 0xd9));
    
    
    bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(bg);
    
    bg->setContentSize(Size(800,300+title2->getContentSize().height));
    
    Text *title = Text::create("系统消息", ".SFUIDisplay-Semibold", 50);
    title->setTextColor(Color4B(0xf0, 0xcb, 0x5c, 255));
    title->setAnchorPoint(Vec2(0.5, 1));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 40));
    bg->addChild(title);

    title2->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 140));
    bg->addChild(title2);
    
    
    Button *lianBut = Button::create("Mine/btn_yellow_small.png", "Mine/btn_yellow_small_check.png");
    lianBut->setScale9Enabled(true);
    lianBut->setAnchorPoint(Vec2(0.5, 0));
    lianBut->setContentSize(Size(600 , 90));
    lianBut->setPosition(Vec2(bg->getContentSize().width/2, 40));
    bg->addChild(lianBut);
    lianBut->addClickEventListener(CC_CALLBACK_1(ServerErrorTips::click, this));
    
    Text *titleBtn = Text::create("退出游戏", ".SFUIDisplay-Semibold", 40);
    titleBtn->setTextColor(Color4B(0x60, 0x41, 0x10, 255));
    titleBtn->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleBtn->setPosition(Vec2(lianBut->getContentSize().width/2, lianBut->getContentSize().height/2));
    lianBut->addChild(titleBtn);
    
}

void ServerErrorTips::click(Ref*ref)
{
    PlatformHelper::exitWithoutParam();
}
