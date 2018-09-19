//
//  Notice.cpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/14.
//
//

#include "Notice.h"
bool Notice::init(){
    if (!Layout::init()) {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();
    this->setTouchEnabled(true);
    this->setContentSize(winSize);
    
    LayerColor* clo = LayerColor::create(Color4B(122,122,122,0));
    clo->setContentSize(winSize);
    
    this->addChild(clo);
    this->addClickEventListener([=](Ref *ref){
        this->removeFromParentAndCleanup(true);
    });
    
    midSize = Size(800,600);
    midlay = Layout::create();
    
    midlay->setTouchEnabled(true);
    midlay->setContentSize(midSize);
    midlay->setAnchorPoint(Vec2(0.5,0.5));
    LayerColor* bgWhite = LayerColor::create(Color4B(26,26,26,255));
    bgWhite->setContentSize(midSize);
    midlay->addChild(bgWhite);
    midlay->setPosition(Vec2(winSize.width/2,winSize.height/2));
    this->addChild(midlay);
    
    initview();
    
    return true;
}


void Notice::initview(){
    Text* text = Text::create("公  告","AmericanTypewriter", 60);
    text->setColor(Color3B::WHITE);
    text->setAnchorPoint(Vec2(0.5,0.5));
    text->setPosition(Vec2(midSize.width/2,midSize.height - text->getContentSize().height/2));
    midlay->addChild(text);

    
    content = Text::create("测试 测试 测试 测试!!!!!!", "AmericanTypewriter", 40);
    content->setContentSize(Size(700,400));
    content->setTextAreaSize(Size(700,400));
    content->setAnchorPoint(Vec2(0.5, 0.5));
    content->ignoreContentAdaptWithSize(false);
    content->setPosition(Vec2(midSize.width/2, midSize.height/2));
    content->setColor(Color3B::WHITE);
    midlay->addChild(content);

    
    Button* button = Button::create("Hall/button/btn_yellow_small.png","Hall/button/btn_yellow_big.png","Hall/button/btn_yellow_small_check.png");
    button->setContentSize(Size(midSize.width-200,120));
    button->setAnchorPoint(Vec2(0.5, 0.5));
    button->setScale9Enabled(true);
    button->setTitleText("关  闭");
    button->setTitleFontSize(50);
    button->setPosition(Vec2(midSize.width/2,100));
    button->addClickEventListener([=](Ref*){
        this->removeFromParentAndCleanup(true);
        __NotificationCenter::getInstance()->postNotification("notice_close");
    });
    midlay->addChild(button);
}


void Notice::addMessage(Json::Value _mvalue){
    string _content = _mvalue["content"].asString();
    content->setString(_content);
}
