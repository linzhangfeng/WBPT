//
//  TeQuanLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/6.
//
//

#include "TeQuanLayer.hpp"

bool TeQuanlayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("会员特权");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    auto visibleSize = Director::getInstance()->getWinSizeInPixels();
    
    cocos2d::experimental::ui::WebView * webView = cocos2d::experimental::ui::WebView::create();
    webView->setColor(Color3B(0x14, 0x14, 0x15));
    webView->setAnchorPoint(Vec2(0, 0));
    webView->setPosition(Vec2(0, 0));
    webView->setContentSize(Size(visibleSize.width, visibleSize.height- getHeadH()));
    webView->loadURL(TE_QUAN_WEB);
    webView->setColor(Color3B(0x14, 0x14, 0x15));
    webView->setScalesPageToFit(true);
    this->addChild(webView);

    
    return true;
    
}