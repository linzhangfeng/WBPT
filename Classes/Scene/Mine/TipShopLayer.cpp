//
//  TipChangeEditLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/23.
//
//

#include "TipShopLayer.hpp"
#include "ShopAllLayer.hpp"

TipShopLayer* TipShopLayer::create()
{
    TipShopLayer *pRet = new(std::nothrow) TipShopLayer();
    if (pRet && pRet->init())
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

bool TipShopLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout *mask = Layout::create();
    mask->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    mask->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    mask->setBackGroundColorOpacity(200);
    mask->setContentSize(visibleSize);
    this->addChild(mask);
    
    // 创建一个事件监听器类型为 OneByOne 的单点触摸
    auto listener1 = EventListenerTouchOneByOne::create();
    // 设置是否吞没事件，在 onTouchBegan 方法返回 true 时吞没
    listener1->setSwallowTouches(true);
    
    // 使用 lambda 实现 onTouchBegan 事件回调函数
    listener1->onTouchBegan = [](Touch* touch, Event* event) -> bool {
        // 获取事件所绑定的 target
        return true;
    };
    
    // 点击事件结束处理
    listener1->onTouchEnded = [=](Touch* touch, Event* event)
    {
        this->removeFromParent();
        
    };
    EventDispatcher * eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

    return true;
}

void TipShopLayer::buyWanPayType(Json::Value json, int index, int FromType)
{
    //0 默认商店的玩呗购买 1:首充的购买
    payFromType = FromType;
    
    wanList = json;
    wanIndex = index;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(720, 660-120));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(bg);

    Text *title = Text::create("支付", ".SFUIDisplay-Semibold", 50);
    title->setTextColor(Color4B(0xf0, 0xcb, 0x5c, 255));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 70));
    bg->addChild(title);
    
    Button *cancel = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    cancel->setAnchorPoint(Vec2(0, 0));
    cancel->setPosition(Vec2(bg->getContentSize().width - cancel->getContentSize().width/2, bg->getContentSize().height - cancel->getContentSize().height/2));
    cancel->addClickEventListener([=](Ref*ref){
        
        this->removeFromParent();
        
    });
    bg->addChild(cancel);
    
    char buf[128];
    
    if (payFromType == 0)
    {
        for (int i = 0; i < 2; i++)
        {
            Text *title = Text::create("", ".SFUIDisplay-Semibold", 36);
            title->setTextColor(Color4B(0xff, 0x79, 0x00, 255));
            bg->addChild(title);
            
            if (i == 0 )
            {
                sprintf(buf, "%d钻石", json["elements"][index]["goodsNum"].asInt());
                title->setString(buf);
                title->setAnchorPoint(Vec2(0, 0));
                title->setPosition(Vec2(50, bg->getContentSize().height - 150));
                
            }else if(i == 1)
            {
                sprintf(buf, "支付%d元", json["elements"][index]["currentPrice"].asInt());
                title->setString(buf);
                title->setAnchorPoint(Vec2(1, 0));
                title->setPosition(Vec2(bg->getContentSize().width - 50, bg->getContentSize().height - 150));
            }
           
        }

        
    }else if (payFromType == 1)
    {
        for (int i = 0; i < 2; i++)
        {
            Text *title = Text::create("", ".SFUIDisplay-Semibold", 36);
            title->setTextColor(Color4B(0xff, 0x79, 0x00, 255));
            bg->addChild(title);
            
            if (i == 0 )
            {
                title->setString(json["name"].asString());
                title->setAnchorPoint(Vec2(0, 0));
                title->setPosition(Vec2(50, bg->getContentSize().height - 200));
                
            }else if(i == 1)
            {
                sprintf(buf, "支付%d元", json["price"].asInt());
                title->setString(buf);
                title->setAnchorPoint(Vec2(1, 0));
                title->setPosition(Vec2(bg->getContentSize().width - 50, bg->getContentSize().height - 200));
            }
            
        }
    }
    
    Layout *line3 = Layout::create();
    line3->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line3->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
    line3->setContentSize(Size(bg->getContentSize().width - 80, 1));
    line3->setPosition(Vec2(40, bg->getContentSize().height - 210));
    bg->addChild(line3);

    for (int i = 0; i < 2; i++)
    {
        Layout *cell = Layout::create();
        cell->setContentSize(Size(bg->getContentSize().width - 80, 120));
        cell->setPosition(Vec2(40, 30+(cell->getContentSize().height+25)*i));
        bg->addChild(cell);
        
        Button *buyBut = Button::create("Mine/bar_window_gray_check.png", "Mine/bar_window_gray_check.png","Mine/bar_window_gray_check.png");
        buyBut->setScale9Enabled(true);
        buyBut->setContentSize(cell->getContentSize());
        buyBut->setPosition(Vec2(cell->getContentSize().width/2, cell->getContentSize().height/2));
        cell->addChild(buyBut);
        buyBut->addClickEventListener(CC_CALLBACK_1(TipShopLayer::buyWanFun, this));
        
        Text *title4 = Text::create("", ".SFUIDisplay-Semibold", 40);
        title4->setTextColor(Color4B(0xe6, 0xe6, 0xe6, 255));
        title4->setAnchorPoint(Vec2(0, 0.5));
        title4->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        title4->setPosition(Vec2(130, cell->getContentSize().height/2));
        cell->addChild(title4, 10);
        
        ImageView *icon;
        switch (i)
        {
            case 0:
                buyBut->setName("weiXin");
                title4->setString("微信");
                icon = ImageView::create("Mine/pay_icon1.png");
                break;
            case 1:
                title4->setString("支付宝");
                buyBut->setName("zhifubao");
                icon = ImageView::create("Mine/pay_icon2.png");
                break;
//            case 0:
//                title4->setString("appStore");
//                buyBut->setName("appstore");
//                icon = ImageView::create("Mine/pay_icon4.png");
//
//                break;
        }
        icon->setAnchorPoint(Vec2(0, 0.5));
        icon->setPosition(Vec2(40, cell->getContentSize().height/2));
        cell->addChild(icon);
        
        ImageView *arrow = ImageView::create("Mine/room_detail_view_arrow.png");
        arrow->setPosition(Vec2(cell->getContentSize().width - 40, cell->getContentSize().height/2));
        cell->addChild(arrow);
        
    }
    
}

void TipShopLayer::buyCardGold(int index, Json::Value json)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(720, 350));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(bg);
    
    Button *cacelBut = Button::create("Mine/btn_yellow_small.png", "Mine/btn_yellow_small_check.png");
    cacelBut->setScale9Enabled(true);
    cacelBut->setAnchorPoint(Vec2(0, 0));
    cacelBut->setContentSize(Size(300 , 85));
    cacelBut->setPosition(Vec2(40, 40));
    bg->addChild(cacelBut);
    cacelBut->addClickEventListener([=](Ref*ref){
        this->removeFromParent();
    });

    Text *titleCancel = Text::create("取消", ".SFUIDisplay-Semibold", 40);
    titleCancel->setTextColor(Color4B(0x60,0x41,0x10, 255));
    titleCancel->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleCancel->setPosition(Vec2(cacelBut->getContentSize().width/2, cacelBut->getContentSize().height/2));
    cacelBut->addChild(titleCancel);

    Text *title = Text::create("确认兑换", ".SFUIDisplay-Semibold", 40);
    title->setTextColor(Color4B(0xd9, 0xd9, 0xd9, 255));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2+80));
    bg->addChild(title);
    
    Text *title2;
    char buf[128];
    string tempS = ZJHModel::getInstance()->goldCovertStr2(json["buyGoldNum"].asInt());;
                                                           
    if (index == 1)
    {
        sprintf(buf, "%s", tempS.c_str());
        title2 = Text::create(buf, ".SFUIDisplay-Semibold", 40);
        
    }else if (index == 2)
    {
        sprintf(buf, "房卡＊%d", json["buyGoldNum"].asInt());
        title2 = Text::create(buf, ".SFUIDisplay-Semibold", 40);
    }
    
    title2->setTextColor(Color4B(0xd9, 0xd9, 0xd9, 255));
    title2->setAnchorPoint(Vec2(0.5, 0.5));
    title2->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title2->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2+10));
    bg->addChild(title2);
    
    Button *sureBut = Button::create("Mine/btn_yellow_small.png", "Mine/btn_yellow_small_check.png");
    sureBut->setScale9Enabled(true);
    sureBut->setAnchorPoint(Vec2(1, 0));
    sureBut->setContentSize(Size(300 , 85));
    sureBut->setPosition(Vec2(bg->getContentSize().width - 40, 40));
    bg->addChild(sureBut);
    sureBut->addClickEventListener([=](Ref*ref){
        
        Json::Value jsonT;
        jsonT["id"] = json["id"];
        jsonT["num"] = json["buyGoldNum"];
        jsonT["type"] = 0;
        
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
                if (index == 1)
                {
                    PlatformHelper::showToast("兑换金币成功");
                    
                }else if (index == 2)
                {
                    PlatformHelper::showToast("兑换房卡成功");
                }
                
                __NotificationCenter::getInstance()->postNotification("money_change");
                this->removeFromParent();
            }else
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
        },"store/buy/more", jsonT.toStyledString(),"buy");
        
    });
    
    Text *titleSure = Text::create("确认", ".SFUIDisplay-Semibold", 40);
    titleSure->setTextColor(Color4B(0x60,0x41,0x10, 255));
    titleSure->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleSure->setPosition(Vec2(sureBut->getContentSize().width/2, sureBut->getContentSize().height/2));
    sureBut->addChild(titleSure);

}



void TipShopLayer::buyCardGoldWanRechange(int type)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(720, 350));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(bg);
    
    Button *cacelBut = Button::create("Mine/btn_yellow_small.png", "Mine/btn_yellow_small_check.png");
    cacelBut->setScale9Enabled(true);
    cacelBut->setAnchorPoint(Vec2(0, 0));
    cacelBut->setContentSize(Size(300 , 85));
    cacelBut->setPosition(Vec2(40, 40));
    bg->addChild(cacelBut);
    cacelBut->addClickEventListener([=](Ref*ref){
        this->removeFromParent();
    });
    
    Text *titleCancel = Text::create("取消", ".SFUIDisplay-Semibold", 40);
    titleCancel->setTextColor(Color4B(0x60,0x41,0x10, 255));
    titleCancel->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleCancel->setPosition(Vec2(cacelBut->getContentSize().width/2, cacelBut->getContentSize().height/2));
    cacelBut->addChild(titleCancel);
    
    Text*titleText = Text::create("", ".SFUIDisplay-Semibold", 50);
    titleText->setTextColor(Color4B(0xd9, 0xd9, 0xd9, 255));
    titleText->setAnchorPoint(Vec2(0.5, 0.5));
    titleText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleText->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2 + 80));
    bg->addChild(titleText);
    
    Button *sureBut = Button::create("Mine/btn_yellow_small.png", "Mine/btn_yellow_small_check.png");
    sureBut->setScale9Enabled(true);
    sureBut->setAnchorPoint(Vec2(1, 0));
    sureBut->setContentSize(Size(300 , 85));
    sureBut->setPosition(Vec2(bg->getContentSize().width - 40, 40));
    bg->addChild(sureBut);
    
    Text*titleSure = Text::create("", ".SFUIDisplay-Semibold", 40);
    titleSure->setTextColor(Color4B(0x60,0x41,0x10, 255));
    titleSure->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleSure->setPosition(Vec2(sureBut->getContentSize().width/2, sureBut->getContentSize().height/2));
    sureBut->addChild(titleSure);
    
    sureBut->addClickEventListener([=](Ref*ref){
        this->removeFromParent();
        if (type == 1)
        {
            //1 ：跳转频道
            __NotificationCenter::getInstance()->postNotification("update_rechange_wan");
            
        }else if(type == 2 || type == 3)
        {
            //商店弹窗
            ShopAllLayer *shopTip = ShopAllLayer::create();
            
            Director::getInstance()->getRunningScene()->addChild(shopTip, 100);
            
            Button* moreBtn = Button::create("Mine/yellow_cancel_btn.png", "Mine/yellow_cancel_btn.png");
            moreBtn->setPosition(Vec2(visibleSize.width - 80, visibleSize.height - 80));
            moreBtn->setContentSize(Size(150,150));
            moreBtn->ignoreContentAdaptWithSize(false);
            moreBtn->setFillContentSizeEnabled(false);
            shopTip->addChild(moreBtn,200);
            moreBtn->addClickEventListener([=](Ref*ref){
                shopTip->removeFromParent();
            });
        }
    });
    
    if (type == 1)
    {
        titleText->setString("钻石不足!");
        titleSure->setString("充值钻石");
        
    }else if (type == 2)
    {
        titleText->setString("钻石不足!");
        titleSure->setString("充值钻石");
    
    }else if (type == 3)
    {
        titleText->setString("金币不足！");
        titleSure->setString("充值金币");
    }

}

void TipShopLayer::buyWanFun(Ref*ref)
{
    Button *btn = (Button*)ref;
    string name = btn->getName();
    int payType;
    
    if (name == "zhifubao")
    {
        payType = 2;
        
    }else if (name == "weiXin")
    {
        payType = 1;
        
    }else if (name == "appstore")
    {
        payType = 3;
    }
    
    //0 默认商店的玩呗购买 1:首充的购买
    if (payFromType == 0)
    {
        ZJHModel::getInstance()->appleId = wanList["elements"][wanIndex]["appleId"].asString();
        ZJHModel::getInstance()->requestPay(wanList["id"].asInt(), wanList["elements"][wanIndex]["goodsNum"].asInt(), payType);
        
    }else if (payFromType == 1)
    {
        ZJHModel::getInstance()->requestPay(wanList["id"].asInt(), 1, payType);
    }
    
    if (name == "appstore")
    {
        __NotificationCenter::getInstance()->postNotification("showLoading_appstore");
    }
    this->removeFromParent();
}
