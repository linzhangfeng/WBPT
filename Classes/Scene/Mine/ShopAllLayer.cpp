//
//  ShopAllLayer.cpp
//  ZJH
//
//  Created by apple on 16/8/29.
//
//

#include "ShopAllLayer.hpp"
#include "WanBeiShop.hpp"
#include "GoldShopLayer.hpp"
#include "CardShopLayer.hpp"


void ShopAllLayer::createLoading(Ref*ref)
{
    loading = Loading::startLoading(this,  Director::getInstance()->getVisibleSize() / 2);
    loading->maskSwllowTouch();
}

void ShopAllLayer::deleteLoading(Ref*ref)
{
    if (loading != NULL)
    {
        loading->removeFromParent();
        loading = NULL;
    }
}

void ShopAllLayer::postMoney(Ref* ref)
{
    CallFunc* func1 = CallFunc::create([=]
                                       {
                                           CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
                                               CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
                                               
                                               if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
                                               
                                               if (loginPacket->status != 3)
                                               {
                                                   PlatformHelper::showToast("网络链接失败，请稍后再试");
                                                   return;
                                               }
                                               
                                               if (loginPacket->resultIsOK())
                                               {
                                                   Json::Value data = loginPacket->recvVal["resultMap"]["play"];
                                                   tipJson = data;
                                                   //刷新玩家金钱数据
                                                   log("钱信息：＝＝＝ [%s]",data.toStyledString().c_str());
                                                   //刷新玩家金钱数据
                                                   int gold = data["goldenNum"].asInt();
                                                   goldText->setString(ZJHModel::getInstance()->moneyShowDeal(gold));
                                                   
                                                   int wan = data["wanbei"].asInt();
                                                   __String *bufStr = __String::createWithFormat("%d", wan);
                                                   wanText->setString(bufStr->getCString());
                                                   
                                                   int card = loginPacket->recvVal["resultMap"]["roomCard"].asInt();
                                                   
                                                   cardText->setString(ZJHModel::getInstance()->moneyShowDeal(card));
                                                   
                                               }else
                                               {
                                                   PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                                               }
                                               
                                           },"golden/userWealth","","wealth5");
                                       });
    
    runAction(Sequence::create(DelayTime::create(1.0),func1, NULL));
}

void ShopAllLayer::onEnter()
{
    BaseLayer::onEnter();
    addShopObserver();
}

void ShopAllLayer::addShopObserver()
{
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ShopAllLayer::postMoney), "cz_succ_notify", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ShopAllLayer::postMoney), "account_change", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ShopAllLayer::postMoney), "money_change", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ShopAllLayer::updateShowWan), "update_rechange_wan", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ShopAllLayer::postMoney), "exit_Room_refere", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ShopAllLayer::deleteLoading), "appstore_deleteLoad",NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ShopAllLayer::createLoading), "showLoading_appstore",NULL);
    

}

bool ShopAllLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("商城");

    addShopObserver();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    setHead(270);
    float H = visibleSize.height - getHeadH();
    
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    currentPage = 0;
    Button* btn1 = Button::create("Mine/shop_gold_n.png","Mine/shop_gold_s.png", "Mine/shop_gold_s.png");
    btn1->setTag(202);
    btn1->setPosition(Vec2(visibleSize.width/2 + btn1->getContentSize().width + 10, visibleSize.height - getHeadH() + 90));
    btn1->addTouchEventListener(CC_CALLBACK_2(ShopAllLayer::tabClickFun, this));
    this->addChild(btn1, 200);
    btn1->setVisible(false);
    
    Button* btn2 = Button::create("Mine/shop_shell_n.png","Mine/shop_shell_s.png","Mine/shop_shell_s.png");
    btn2->setTag(200);
    btn2->setAnchorPoint(Vec2(1,0.5));
    btn2->setPosition(Vec2(visibleSize.width/2  - 10 , btn1->getPositionY()));
    btn2->addTouchEventListener(CC_CALLBACK_2(ShopAllLayer::tabClickFun, this));
    this->addChild(btn2, 200);
    btn2->setEnabled(false);
    
    Button* btn3 = Button::create("Mine/shop_prop_n.png","Mine/shop_prop_s.png", "Mine/shop_prop_s.png");
    btn3->setTag(201);
    btn3->setAnchorPoint(Vec2(0,0.5));
    btn3->setPosition(Vec2(visibleSize.width/2  + 10 , btn1->getPositionY()));
    btn3->setPosition(Vec2(visibleSize.width/2 , btn1->getPositionY()));
    btn3->addTouchEventListener(CC_CALLBACK_2(ShopAllLayer::tabClickFun, this));
    
    this->addChild(btn3, 200);
    
    Layout *moneyBg = Layout::create();
    moneyBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    moneyBg->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    moneyBg->ignoreAnchorPointForPosition(false);
    moneyBg->ignoreContentAdaptWithSize(false);
    moneyBg->setAnchorPoint(Vec2(0, 1));
    moneyBg->setPosition(Vec2(0, H));
    moneyBg->setContentSize(Size(visibleSize.width, 135));
    this->addChild(moneyBg);
    
    for (int i = 0; i < 2; i++)
    {
        Layout *line = Layout::create();
        line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        line->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
        line->ignoreAnchorPointForPosition(false);
        line->ignoreContentAdaptWithSize(false);
        if (i == 0)
        {
            line->setAnchorPoint(Vec2(0, 1));
            line->setPosition(Vec2(0, 135));
        }else if(i == 1)
        {
            line->setAnchorPoint(Vec2(0, 0));
            line->setPosition(Vec2(0, 0));

        }
        
        line->setContentSize(Size(visibleSize.width, 2));
        moneyBg->addChild(line);
    }
    
    for (int i = 0; i < 3; i++)
    {
        ImageView *bgImage = ImageView::create("Mine/bar_money.png");
        bgImage->setScale9Enabled(true);
        bgImage->setContentSize(Size(335, 70));
        moneyBg->addChild(bgImage);
        
        Text *text = Text::create("--", ".SFUIDisplay-Semibold", 40);
        text->setPosition(Vec2(50, bgImage->getContentSize().height/2));
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        text->setTextColor(Color4B(0xff, 0xeb, 0xb3, 255));
        bgImage->addChild(text);
        
        ImageView *icon;
        switch (i)
        {
            case 0:
                wanText = text;
                icon = ImageView::create("Mine/icon_shop_02.png");
                bgImage->setPosition(Vec2(visibleSize.width/2 - 50, 70));
                bgImage->setAnchorPoint(Vec2(1,0.5));
                break;
            case 1:
                 cardText = text;
                text->setPositionX(70);
                icon = ImageView::create("Mine/icon_shop_03.png");
                bgImage->setPosition(Vec2(visibleSize.width/2 + 50, 70));
                bgImage->setAnchorPoint(Vec2(0,0.5));
                break;
            case 2:
                goldText = text;
                icon = ImageView::create("Mine/icon_shop_01.png");
                bgImage->setPosition(Vec2(visibleSize.width/2 + bgImage->getContentSize().width + 15, 70));
                text->setVisible(false);
                icon->setVisible(false);
                bgImage->setVisible(false);
                break;
        }
        icon->setPosition(Vec2(0, bgImage->getContentSize().height/2));
        bgImage->addChild(icon);
    }
    
    Size size = Size(visibleSize.width, H - 135-160);
    bgLayout = Layout::create();
    bgLayout->ignoreAnchorPointForPosition(false);
    bgLayout->setContentSize(size);
    this->addChild(bgLayout);
    bgLayout->setPosition(Vec2(0, 160));
    
    postMoney(NULL);
    
    wanView();
    
    return true;
}

void ShopAllLayer::updateShowWan(Ref* ref)
{
    Button *tabLast = (Button*)this->getChildByTag(200+currentPage);
    tabLast->setEnabled(true);
    
    Button *wanBtn = (Button*)this->getChildByTag(200);
    wanBtn->setEnabled(false);
    
    currentPage = 0;
    
    wanView();
}

void ShopAllLayer::wanView()
{
    bgLayout->removeAllChildren();
    WanBeiShop *wan = WanBeiShop::create();
    wan->setAnchorPoint(Vec2(0, 0));
    wan->setPosition(Vec2::ZERO);
    bgLayout->addChild(wan);
}


void ShopAllLayer::goldView()
{
    bgLayout->removeAllChildren();
    GoldShopLayer *gold = GoldShopLayer::create();
    gold->setAnchorPoint(Vec2(0, 0));
    gold->getMoney(tipJson);
    gold->setPosition(Vec2::ZERO);
    bgLayout->addChild(gold);
}

void ShopAllLayer::propView()
{
    bgLayout->removeAllChildren();
    CardShopLayer *card = CardShopLayer::create();
    card->setAnchorPoint(Vec2(0, 0));
    card->getMoney(tipJson);
    card->setPosition(Vec2::ZERO);
    bgLayout->addChild(card);
}

void ShopAllLayer::tabClickFun(Ref *pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED == type)
    {
        Button *tab = (Button*)pSender;
        Button *tabLast = (Button*)this->getChildByTag(200+currentPage);
        if (tab==tabLast)
        {
            return;
        }
        tab->setEnabled(false);
        tabLast->setEnabled(true);
        currentPage = tab->getTag()-200;
        switch (currentPage)
        {
            case 0:
                wanView();
                break;
            case 1:
                propView();
                break;
            case 2:
                
                goldView();
                break;
        }

    }
}


