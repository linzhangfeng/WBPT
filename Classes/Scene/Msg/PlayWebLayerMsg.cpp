
//
//  TaskLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/6.
//


#include "PlayWebLayerMsg.hpp"
#include "../Loading/Loading.h"

void PlayWebLayerMsg::showView(string titleStr, string urlStr)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout *bgLayout = Layout::create();
    bgLayout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    bgLayout->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    bgLayout->setBackGroundColorOpacity(150);
    bgLayout->setTouchEnabled(true);
    bgLayout->setSwallowTouches(true);
    bgLayout->setContentSize(visibleSize);
    bgLayout->addClickEventListener([=](Ref*ref){
        this->removeFromParent();
        
    });
    
    this->addChild(bgLayout);
    
    ImageView * bg = ImageView::create("Public/bar_system_window.png");
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(visibleSize.width - 100, visibleSize.height - 400));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    bgLayout->addChild(bg);
    
    float H = bg->getContentSize().height;
    float bgW = bg->getContentSize().width;
    
    Button *cancel = Button::create("Public/bar_window_cancel.png","Public/bar_window_cancel.png");
    cancel->setPosition(Vec2(bgW - 70, H - 70));
    cancel->addClickEventListener([=](Ref*ref){
        this->removeFromParent();
    });
    bg->addChild(cancel);
    
    //"玩法说明"
    Text *title = Text::create(titleStr, "", 58);
    title->setAnchorPoint(Vec2(0.5, 1));
    title->setTextColor(Color4B(0xf1, 0xcb, 0x5b, 255));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(bgW/2, H - 35));
    bg->addChild(title);
    
    //
    cocos2d::experimental::ui::WebView * webView = cocos2d::experimental::ui::WebView::create();
    webView->setColor(Color3B(0xc7, 0x95, 0x5e));
    webView->setAnchorPoint(Vec2(0.5, 0));
    webView->setPosition(Vec2(bg->getContentSize().width / 2, 20));
    webView->setContentSize(Size(bgW - 60, H - 150));
    webView->loadURL(urlStr);
    webView->setScalesPageToFit(true);
    bg->addChild(webView);
}
