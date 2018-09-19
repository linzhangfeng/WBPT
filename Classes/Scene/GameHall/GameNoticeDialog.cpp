//
//  GameNoticeDialog.cpp
//  ZJH
//
//  Created by mac on 2017/3/17.
//
//

#include "GameNoticeDialog.hpp"
bool GameNoticeDialog::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size size = Director::getInstance()->getWinSize();
    
    auto cbg = LayerColor::create(Color4B(0, 0, 0, 200));
    cbg->setContentSize(size);
    cbg->setAnchorPoint(Vec2(0.0, 0.0));
    cbg->setPosition(0,0);
    addChild(cbg);
    
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setAnchorPoint(Vec2(0,0));
    layout->setPosition(Vec2(0,0));
    layout->setTouchEnabled(true);
    layout->setSwallowTouches(true);
    layout->addClickEventListener([=](Ref*){
    });
    addChild(layout,-1000);
    
    Sprite *bg = Sprite::create("GameHall/game_notice_big_bg.png");
    bg->setPosition(Vec2(size.width/2, size.height/2));
    addChild(bg);
    
    auto title = Sprite::create("GameHall/game_notice_title.png");
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 20));
    bg->addChild(title);
    
    auto closeBtn = Button::create("GameHall/kefu/record_btn_003.png", "GameHall/kefu/record_btn_003c.png");
    closeBtn->setPosition(Vec2(bg->getContentSize().width - 30, bg->getContentSize().height - 30));
    closeBtn->addClickEventListener([=](Ref*){
        this->removeFromParent();
    });
    bg->addChild(closeBtn,10);
    
    return true;
}

void GameNoticeDialog::show(string content){
    Layout* noticeLayout = Layout::create();
    noticeLayout->setTouchEnabled(true);
    
    notice = LabelTTF::create(content, "", 42,Size(1500, 0), TextHAlignment::LEFT);
    notice->setAnchorPoint(Vec2(0,0));
    notice->setColor(Color3B(0xc1, 0x62, 0x24));
    notice->setPosition(Vec2(0,0));
    
    noticeLayout->addChild(notice);
    noticeLayout->setContentSize(notice->getContentSize());

    ListView *resultList = ListView::create();
    resultList->setDirection(ScrollView::Direction::VERTICAL);
    resultList->setBounceEnabled(false);
    resultList->setContentSize(Size(1500, 790));
    resultList->setAnchorPoint(Vec2(0, 1));
    resultList->setPosition(Vec2(200, this->getContentSize().height - 160));
    resultList->setItemsMargin(10);
    resultList->addChild(noticeLayout);
    this->addChild(resultList);
}


