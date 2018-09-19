//
//  ReciveEffectTipLayer.cpp
//  ZJH
//
//  Created by apple on 16/8/17.
//
//

#include "ReciveEffectTipLayer.hpp"

bool ReciveEffectTipLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    

    auto winSize = Director::getInstance()->getVisibleSize();
    bg = Layout::create();
    bg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    bg->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    bg->setBackGroundColorOpacity(150);
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setContentSize(winSize);
    bg->addClickEventListener([=](Ref*ref){
        this->removeFromParent();
    });
    this->addChild(bg);

    return true;
}

void ReciveEffectTipLayer::showView(Json::Value json)
{
    auto winSize = Director::getInstance()->getVisibleSize();

    ImageView *lightBg = ImageView::create("Mine/game_light2.png");
    lightBg->setPosition(Vec2(winSize.width/2, winSize.height/2));
    bg->addChild(lightBg);
    
    auto pRotate1 = RotateBy::create(2.5, 180);
    auto pRotate2 = RotateBy::create(2.5, -180);
    auto pCallback = CallFunc::create(CC_CALLBACK_0(ReciveEffectTipLayer::stopAction,this));
    auto se = Sequence::create(pRotate1, pRotate2, pCallback, NULL);
    auto pRepeatRotate = Repeat::create(se, 5);
    lightBg->runAction(pRepeatRotate);
    
    auto blink = Blink::create(25, 60);
    ImageView *starBg = ImageView::create("Mine/game_light3.png");
    starBg->setPosition(Vec2(winSize.width/2, lightBg->getPositionY() + 120));
    bg->addChild(starBg, 3);
    starBg->runAction(blink);
    
    ImageView *iconBg = ImageView::create();
    iconBg->setPosition(Vec2(winSize.width/2, winSize.height/2));
    bg->addChild(iconBg, 2);
    
    //任务奖励描述
    char moneyS[100];
    if (json["moneyType"].asInt() == 1)
    {
        iconBg->loadTexture("Mine/game_windows_coin2.png");
        //1:金币奖励
        sprintf(moneyS, "金币 *%d",  json["recieveCount"].asInt());
    }else if (json["moneyType"].asInt() == 2)
    {
        //2:玩贝奖励
        iconBg->loadTexture("Mine/game_windows_coin4.png");
        sprintf(moneyS, "钻石 *%d",  json["recieveCount"].asInt());
    }else
    {
        //3:房卡
        iconBg->loadTexture("Mine/game_windows_coin5.png");
        sprintf(moneyS, "房卡 *%d",  json["recieveCount"].asInt());
    }

    string temp = moneyS;
    Text *title = Text::create(moneyS, "", 40);
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setTextColor(Color4B(0xf1, 0xcb, 0x5b, 255));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(winSize.width/2, winSize.height/2 - 150));
    bg->addChild(title, 2);
}

void ReciveEffectTipLayer::stopAction()
{
    this->removeFromParent();
}