//
//  HorizenSystemMsg.cpp
//  ZJH
//
//  Created by mac on 2017/1/16.
//
//

#include "HorizenSystemMsg.hpp"
#include "../Loading/Loading.h"

void HorizenSystemMsg::showView(string titleStr, string urlStr)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout *bgLayout = Layout::create();
    bgLayout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    bgLayout->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    bgLayout->setBackGroundColorOpacity(150);
    bgLayout->setTouchEnabled(true);
    bgLayout->setSwallowTouches(false);
    bgLayout->setContentSize(visibleSize);
    bgLayout->addClickEventListener([=](Ref*ref){
    });
    
    this->addChild(bgLayout);
    
    ImageView * bg = ImageView::create("GameHall/system_msg_bg.png");
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(false);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(visibleSize.width - 100, visibleSize.height - 180));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    bg->addClickEventListener([=](Ref*ref){
        
    });
    bgLayout->addChild(bg);
    
    float H = bg->getContentSize().height;
    float bgW = bg->getContentSize().width;
    
    Button *cancel = Button::create("GameHall/createRoom/c1.png","GameHall/createRoom/c2.png");
    cancel->setPosition(Vec2(bgW - 10, H - 10));
    cancel->addClickEventListener([=](Ref*ref){
        this->removeFromParent();
    });
    bg->addChild(cancel);
    
    //"玩法说明"
    ImageView *title = ImageView::create("GameHall/system_msg2.png");
    title->setAnchorPoint(Vec2(0.5, 1));
    title->setPosition(Vec2(bgW/2, H - 50));
    bg->addChild(title);
    
    //
    cocos2d::experimental::ui::WebView * webView = cocos2d::experimental::ui::WebView::create();
    webView->setColor(Color3B(0xff, 0xef, 0xbf));
    webView->setAnchorPoint(Vec2(0.5, 0));
    webView->setPosition(Vec2(bg->getContentSize().width / 2, 50));
    webView->setContentSize(Size(bgW - 80, H - 200));
    webView->loadURL(urlStr);
    webView->setScalesPageToFit(true);
    bg->addChild(webView);
}
