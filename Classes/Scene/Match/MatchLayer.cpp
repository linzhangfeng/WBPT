//
//  MatchLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/13.
//
//

#include "MatchLayer.hpp"
#include "MatchCurrentLayer.hpp"
#include "MatchResultLayer.hpp"

MatchLayer::MatchLayer()
{

}

bool MatchLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MatchLayer::showCurrent), "exit_Room_refere", NULL);
    
    
    setTitle("房间列表");
    setName("MatchLayer");
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    getHeadH();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Size size = Size(visibleSize.width, visibleSize.height -150-160);
    bgLayout = Layout::create();
    bgLayout->setAnchorPoint(Vec2(0, 0));
    bgLayout->ignoreAnchorPointForPosition(false);
    bgLayout->ignoreContentAdaptWithSize(false);
    bgLayout->setPosition(Vec2(0, 160));
    bgLayout->setContentSize(size);
    this->addChild(bgLayout);
    
    showCurrent(NULL);
    
    int by = visibleSize.height - getHeadH()/ 2;
    Button* resultBtn = Button::create("Match/match_reslutBtn1.png", "Match/match_reslutBtn2.png");
    resultBtn->setPosition(Vec2(visibleSize.width - 40,by));
    resultBtn->setName("resultBtn");
    resultBtn->setAnchorPoint(Vec2(1, 0.5));
    resultBtn->setContentSize(Size(160, getHeadH()));
    resultBtn->ignoreContentAdaptWithSize(false);
    resultBtn->setFillContentSizeEnabled(false);
    resultBtn->addClickEventListener([=](Ref*ref){
        
        ZJHModel::getInstance()->gotoView(VIEW_MATCH_RESULT);
        
            });
    addChild(resultBtn, 200);
    
    Text *resultTitle = Text::create("战绩", "", 40);
    resultTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    resultTitle->setPosition(Vec2(resultBtn->getContentSize().width/2, resultBtn->getContentSize().height/2));
    resultTitle->setColor(Color3B(0x14, 0x14, 0x14));
    resultBtn->addChild(resultTitle);
    
    return true;
}

void MatchLayer::showCurrent(Ref* ref)
{
    bgLayout->removeAllChildren();
    MatchCurrentLayer *currentLayout = MatchCurrentLayer::create();
    currentLayout->setAnchorPoint(Vec2(0, 0));
    currentLayout->setPosition(Vec2::ZERO);
    bgLayout->addChild(currentLayout);
}

//战绩view1
void MatchLayer::showRecord()
{
    bgLayout->removeAllChildren();
    MatchResultLayer *resultLayout = MatchResultLayer::create();
    resultLayout->setAnchorPoint(Vec2(0, 0));
    resultLayout->setPosition(Vec2::ZERO);
    bgLayout->addChild(resultLayout);
}
