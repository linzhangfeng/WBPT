//
//  RoomShopLayer.cpp
//  ZJH
//
//  Created by hyhy on 16/8/5.
//
//

#include "RoomShopLayer.h"

RoomShopLayer::RoomShopLayer()
    :m_listView(NULL)
{
    
}

void RoomShopLayer::show()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(Vec2(0, -winSize.height));
    this->runAction(MoveTo::create(0.17, Vec2(0, 0)));
    this->setVisible(true);
}

void RoomShopLayer::hide()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->runAction(Sequence::create(MoveTo::create(0.17, Vec2(0, -winSize.height)),
                                     RemoveSelf::create(),
                                     NULL));
}

bool RoomShopLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    this->myInit();
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomShopLayer::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void RoomShopLayer::myInit()
{
    Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = Size(winSize.width, 790);
    
    LayerColor *contentBg = LayerColor::create(Color4B(32, 32, 32, 255));
    contentBg->setContentSize(contentSize);
    contentBg->setPosition(Vec2(0, 0));
    contentBg->setName("content_bg");
    this->addChild(contentBg);
    
    LayerColor *titleBg = LayerColor::create(Color4B(32, 32, 32, 255));
    titleBg->setContentSize(Size(winSize.width, 75));
    titleBg->setPosition(Vec2(0, contentBg->getPositionY() + contentBg->getContentSize().height));
    titleBg->setName("title_bg");
    this->addChild(titleBg);
    
    LayerColor *line = LayerColor::create(Color4B(0, 0, 0, 255));
    line->setContentSize(Size(winSize.width, 2));
    line->setPosition(titleBg->getPosition());
    this->addChild(line);
    
    Text* titlelabel = Text::create("商城", "AmericanTypewriter", 36);
    titlelabel->setPosition(titleBg->getContentSize() / 2);
    titleBg->addChild(titlelabel);
    
    Button *closeBtn = Button::create("Public/icon_cancel.png");
    closeBtn->setZoomScale(0);
    closeBtn->setPosition(Vec2(winSize.width - 45, titleBg->getContentSize().height / 2));
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->hide();
                                    });
    titleBg->addChild(closeBtn);
    
    m_listView = ListView::create();
    m_listView->setDirection(ScrollView::Direction::VERTICAL);
    m_listView->setBounceEnabled(false);
    m_listView->setContentSize(contentSize);
    m_listView->setAnchorPoint(Vec2(0, 0));
    m_listView->setPosition(Vec2(0, 0));
    m_listView->setItemsMargin(0);
    m_listView->setScrollBarEnabled(false);
    contentBg->addChild(m_listView);
    
    //124
    Size itemSize = Size(winSize.width, 124);
    Layout *wealthLayout = Layout::create();
    wealthLayout->setContentSize(itemSize);
    wealthLayout->setName("wealth_layout");
    m_listView->addChild(wealthLayout);
    
    Text *currentText = Text::create("现有", "AmericanTypewriter", 32);
    currentText->setPosition(Vec2(80, itemSize.height / 2));
    wealthLayout->addChild(currentText);
    
    Sprite *goldIcon = Sprite::create("RoomBottom/icon_money_small.png");
    goldIcon->setPosition(Vec2(170, itemSize.height / 2));
    wealthLayout->addChild(goldIcon);
    
    Text *goldText = Text::create("", "AmericanTypewriter", 28);
    goldText->setColor(Color3B(255, 232, 147));
    goldText->setAnchorPoint(Vec2(0, 0.5));
    goldText->setPosition(Vec2(200, itemSize.height / 2));
    goldText->setName("gold_text");
    wealthLayout->addChild(goldText);
    
    Sprite *wanbeiIcon = Sprite::create("RoomBottom/icon_wanbei_small.png");
    wanbeiIcon->setPosition(Vec2(itemSize.width / 2 + 50, itemSize.height / 2));
    wealthLayout->addChild(wanbeiIcon);
    
    Text *wanbeiText = Text::create("", "AmericanTypewriter", 28);
    wanbeiText->setColor(Color3B(255, 232, 147));
    wanbeiText->setAnchorPoint(Vec2(0, 0.5));
    wanbeiText->setPosition(Vec2(itemSize.width / 2 + 90, itemSize.height / 2));
    wanbeiText->setName("wanbei_text");
    wealthLayout->addChild(wanbeiText);
}

void RoomShopLayer::myInitWealth(Json::Value &data)
{
    int goldNum = data["play"]["goldenNum"].asInt();
    int wanbei = data["play"]["wanbei"].asInt();
    char buff[128];
    
    Layout *wealthLayout = dynamic_cast<Layout *>(m_listView->getChildByName("wealth_layout"));
    Text *goldText = dynamic_cast<Text *>(wealthLayout->getChildByName("gold_text"));
    Text *wanbeiText = dynamic_cast<Text *>(wealthLayout->getChildByName("wanbei_text"));
    
    Utils::parseCutNums(3, buff, goldNum);
    goldText->setString(buff);
    
    Utils::parseCutNums(3, buff, wanbei);
    snprintf(buff, sizeof(buff), "%d", wanbei);
    wanbeiText->setString(buff);
}

void RoomShopLayer::myInitShop(Json::Value &data)
{
    Size winSize = Director::getInstance()->getWinSize();
    Size itemSize = Size(winSize.width, 124);
    int nums = data["info"]["elements"].size();
    char buff[128];
    
    for (int i = 0; i < nums; i++) {
        Layout *layout = Layout::create();
        layout->setContentSize(itemSize);
        if (i%2 == 0) {
            LayerColor *bg = LayerColor::create(Color4B(38, 38, 38, 255));
            bg->setContentSize(layout->getContentSize());
            bg->setPosition(Vec2(0, 0));
            layout->addChild(bg);
        }
        
        int goodsNum = data["info"]["elements"][i]["goodsNum"].asInt();
        int currentPrice = data["info"]["elements"][i]["currentPrice"].asInt();
        
        Sprite *goldIcon = Sprite::create("RoomBottom/icon_money_small.png");
        goldIcon->setPosition(Vec2(70, itemSize.height / 2));
        layout->addChild(goldIcon);
        
        Utils::parseCutNums(3, buff, goodsNum);
        Text *goldText = Text::create(buff, "AmericanTypewriter", 28);
        goldText->setAnchorPoint(Vec2(0, 0.5));
        goldText->setPosition(Vec2(105, itemSize.height / 2));
        goldText->setColor(Color3B(255, 232, 147));
        layout->addChild(goldText);
        
        Button *priceBtn = Button::create("RoomBottom/my_coin_btn.9.png", "RoomBottom/my_coin_btn_press.9.png");
        priceBtn->setPosition(Vec2(itemSize.width - 135, itemSize.height / 2));
        priceBtn->setScale(1, 0.75);
        priceBtn->addClickEventListener([=](Ref *ref)
                                        {
                                            PlatformHelper::showToast("待添加");
                                        });
        layout->addChild(priceBtn);
        
        Sprite *wanbeiIcon = Sprite::create("RoomBottom/icon_wanbei_small.png");
        wanbeiIcon->setPosition(Vec2(itemSize.width - 190, itemSize.height / 2));
        layout->addChild(wanbeiIcon);
        
        Utils::parseCutNums(3, buff, currentPrice);
        Text *wanbeiText = Text::create(buff, "AmericanTypewriter", 28);
        wanbeiText->setColor(Color3B(172, 92, 0));
        wanbeiText->setAnchorPoint(Vec2(0, 0.5));
        wanbeiText->setPosition(Vec2(itemSize.width - 155, itemSize.height / 2));
        layout->addChild(wanbeiText);
        
        m_listView->addChild(layout);
    }
    
    m_listView->refreshView();
}

bool RoomShopLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    LayerColor *contentBg = dynamic_cast<LayerColor *>(this->getChildByName("content_bg"));
    LayerColor *titleBg = dynamic_cast<LayerColor *>(this->getChildByName("title_bg"));
    if (contentBg->boundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())) || titleBg->boundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())))
    {
        //
    }
    else
    {
        this->hide();
        event->stopPropagation();
    }
    return true;
}