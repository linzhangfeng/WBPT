//
//  TipChangeEditLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/23.
//
//

#include "TipLoadLayer.hpp"

TipLoadLayer* TipLoadLayer::create(int type)
{
    TipLoadLayer *pRet = new(std::nothrow) TipLoadLayer();
    if (pRet && pRet->init(type))
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

bool TipLoadLayer::init(int type)
{
    if (!Layer::init())
    {
        return false;
    }
    
    connectType = type;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout *mask = Layout::create();
    mask->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    mask->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    mask->setBackGroundColorOpacity(150);
    mask->setContentSize(visibleSize);
    this->addChild(mask);
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(720, 470));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(bg);
    
    Text *title = Text::create("提示", ".SFUIDisplay-Semibold", 50);
    title->setTextColor(Color4B(0xf0, 0xcb, 0x5c, 255));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 70));
    bg->addChild(title);
    
    Button *cancel = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    cancel->setAnchorPoint(Vec2(0.5,0.5));
    cancel->setPosition(Vec2(bg->getContentSize().width, bg->getContentSize().height));
    cancel->addClickEventListener([=](Ref*ref){
        PlatformHelper::exitWithoutParam();
    });
    bg->addChild(cancel);
    
    Text *title2 = Text::create("网络未连接！", ".SFUIDisplay-Semibold", 45);
    title2->setTextColor(Color4B(0xd9, 0xd9, 0xd9, 255));
    title2->setAnchorPoint(Vec2(0.5, 0.5));
    title2->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title2->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2));
    bg->addChild(title2);

    
    Button *lianBut = Button::create("Mine/btn_yellow_small.png", "Mine/btn_yellow_small_check.png");
    lianBut->setScale9Enabled(true);
    lianBut->setAnchorPoint(Vec2(0, 0));
    lianBut->setContentSize(Size(600 , 90));
    lianBut->setPosition(Vec2(40, 40));
    bg->addChild(lianBut);
    lianBut->addClickEventListener(CC_CALLBACK_1(TipLoadLayer::reLianOn, this));
    
    Text *titleBtn = Text::create("重新连接", ".SFUIDisplay-Semibold", 40);
    titleBtn->setTextColor(Color4B(0x60, 0x41, 0x10, 255));
    titleBtn->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleBtn->setPosition(Vec2(lianBut->getContentSize().width/2, lianBut->getContentSize().height/2));
    lianBut->addChild(titleBtn);
    
    return true;
}

void TipLoadLayer::reLianOn(Ref*ref)
{
    //type ＝ 1:启动界面，之前有账号存储的时候，断网连接
    //type ＝ 2:登录界面，没有账号存储的时候，断网连接

    
    if (connectType == 1)
    {
        __NotificationCenter::getInstance()->postNotification("re_connect_GameLoad");
        
    }else if (connectType == 2)
    {
        __NotificationCenter::getInstance()->postNotification("re_connect_LoadLayer");
    }
    
    this->removeFromParentAndCleanup(true);
}
