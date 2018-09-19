//
//  wanBeiShop.cpp
//  ZJH
//
//  Created by apple on 16/6/30.
//
//

#include "WanBeiShop.hpp"
#include "TipShopLayer.hpp"

bool WanBeiShop::init()
{
    if (!Widget::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();

    float H = visibleSize.height - 270 - 135 - 160;
    
    this->setContentSize(Size(visibleSize.width, H));
    
    //2:玩呗请求
    Json::Value json2;
    json2["type"] = "WB";
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"store/find.action",json2.toStyledString(),"wanShop");
    this->schedule(schedule_selector(WanBeiShop::postWan), 0.0f);

    return true;
}

void WanBeiShop::postWan(float dt)
{
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["wanShop"];
    if (packet && packet->status != 0)
    {
        if (packet->status != 3)
        {
            return;
        }
        packet->status = 0;
        
        if (packet->resultIsOK())
        {
            Json::Value data = packet->recvVal["resultMap"]["info"]["elements"];
            wanList = packet->recvVal["resultMap"]["info"];
            wanView(data);
            
            log("玩呗：＝＝＝ [%s]",packet->recvVal["resultMap"]["info"].toStyledString().c_str());
        }
        else
        {
            PlatformHelper::showToast(packet->recvVal["message"].asCString());
            
        }
        
        unschedule(schedule_selector(WanBeiShop::postWan));
        CCHttpAgent::getInstance()->packets["wanShop"] = NULL;
        delete packet;
    }
}

void WanBeiShop::wanView(Json::Value jsonData)
{
    this->removeAllChildren();
    auto winSize = Director::getInstance()->getVisibleSize();
    float H = this->getContentSize().height;
    
    ListView* listView1 = ListView::create();
    listView1->setBounceEnabled(true);
    listView1->setDirection(ScrollView::Direction::VERTICAL);
    listView1->setContentSize(Size(winSize.width, H));
    listView1->setAnchorPoint(Vec2(0, 0));
    listView1->setPosition(Vec2(0, 0));
    listView1->setItemsMargin(0);
    listView1->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(listView1);
    
    for (int i = 0; i < jsonData.size(); i++)
    {
        auto list2 = Layout::create();
        list2->setContentSize(Size(winSize.width, 195));
        listView1->addChild(list2);
        
        Layout *line3 = Layout::create();
        line3->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        line3->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
        line3->setContentSize(Size(winSize.width, 1));
        line3->setPosition(Vec2(40, 0));
        list2->addChild(line3);
        

        char priceStr[128];
        sprintf(priceStr, "%d钻石", jsonData[i]["goodsNum"].asInt());
        
        ImageView *gold = ImageView::create();
        gold->setAnchorPoint(Vec2(0, 0.5));
        gold->setPosition(Vec2(40, list2->getContentSize().height/2));
        list2->addChild(gold);
        
        ZJHModel::getInstance()->loadingImageResource(gold, jsonData[i]["logo"].asString());
        
        //暂时横版本地取
//        ImageView *gold = ImageView::create();
//        gold->setAnchorPoint(Vec2(0, 0.5));
//        gold->setPosition(Vec2(40, list2->getContentSize().height/2));
//        list2->addChild(gold);
//        ZJHModel::getInstance()->loadingImageResource(gold, jsonData[i]["logo"].asString());
        
        Text *text = Text::create(priceStr, ".SFUIDisplay-Semibold", 40);
        text->setPosition(Vec2(220, gold->getPositionY()));
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        text->setTextColor(Color4B(0xf1, 0xcd, 0x5c, 255));
        list2->addChild(text);
        
        int price = jsonData[i]["currentPrice"].asInt();
        
        sprintf(priceStr, "￥%d元", price);
        
        Button *buyBut = Button::create("Mine/btn_yellow_small.png", "Mine/btn_yellow_small_check.png");
        buyBut->setScale9Enabled(true);
        buyBut->setAnchorPoint(Vec2(1, 0.5));
        buyBut->setContentSize(Size(220 , 90));
        buyBut->setPosition(Vec2(winSize.width - 40, list2->getContentSize().height/2));
        list2->addChild(buyBut);
        buyBut->setTag(100+i);
        buyBut->addClickEventListener(CC_CALLBACK_1(WanBeiShop::buyWanFun, this));
        
        Text *wanBtn = Text::create(priceStr, ".SFUIDisplay-Semibold",40);
        wanBtn->setPosition(Vec2(buyBut->getContentSize().width/2, buyBut->getContentSize().height/2));
        wanBtn->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        wanBtn->setTextColor(Color4B(0x60,0x41,0x10,255));
        buyBut->addChild(wanBtn);
        
        /*
        if (jsonData[i]["originalPrice"].asInt() != jsonData[i]["currentPrice"].asInt())
        {
            sprintf(priceStr, "原价＝￥%d元", jsonData[i]["originalPrice"].asInt());
            Text *yuanValue = Text::create(priceStr, ".SFUIDisplay-Semibold",30);
            yuanValue->setPosition(Vec2(225, 20));
            yuanValue->setAnchorPoint(Vec2(0, 0));
            yuanValue->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
            yuanValue->setTextColor(Color4B(0xfe,0x7a,0x00, 255));
            list2->addChild(yuanValue);
            
            Layout *line4 = Layout::create();
            line4->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
            line4->setBackGroundColor(Color3B(0xfe,0x7a,0x00));
            line4->setContentSize(Size(195, 3));
            line4->setPosition(Vec2(0, yuanValue->getContentSize().height/2));
            yuanValue->addChild(line4);
            
        }
        
        if(jsonData[i]["reduceRMB"].asInt() > 0)
        {
            sprintf(priceStr, "优惠%d元", jsonData[i]["reduceRMB"].asInt());
            Text *wanValue = Text::create(priceStr, ".SFUIDisplay-Semibold",30);
            wanValue->setPosition(Vec2(winSize.width - 145, 10));
            wanValue->setAnchorPoint(Vec2(0.5, 0));
            wanValue->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
            wanValue->setTextColor(Color4B(0xfe,0x7a,0x00, 255));
            list2->addChild(wanValue);
        }
        */
    }
}

void WanBeiShop::buyWanFun(Ref*ref)
{
    Button *btn = (Button*)ref;
    int index = btn->getTag()-100;
    
    if (PlatformHelper::sdkDoForInt_C("hasThirdChannel"))
    {
        ZJHModel::getInstance()->requestThirdPay(wanList["id"].asInt(),wanList["elements"][index]["goodsNum"].asInt());
    }
    else
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
#if (ENABLE_IOS_PAY == 1)
        // 3: "appstore"
        
        __NotificationCenter::getInstance()->postNotification("showLoading_appstore");
        ZJHModel::getInstance()->appleId = wanList["elements"][index]["appleId"].asString();
        ZJHModel::getInstance()->requestPay(wanList["id"].asInt(), wanList["elements"][index]["goodsNum"].asInt(), 3);

#endif
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        TipShopLayer *tip = TipShopLayer::create();
        tip->buyWanPayType(wanList, index);
        Director::getInstance()->getRunningScene()->addChild(tip, 100);
        
#endif
    }
    
}
void WanBeiShop::selectPayFun(Ref *pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED == type)
    {
        Layout *layout = (Layout *)pSender;
        if (layout != selectPay)
        {
            ImageView * image1 = (ImageView*)selectPay->getChildByTag(100);
            ImageView * image2 = (ImageView*)selectPay->getChildByTag(200);
            image1->setVisible(true);
            image2->setVisible(false);
        
            selectPay = layout;
            image1 = (ImageView*)selectPay->getChildByTag(100);
            image2 = (ImageView*)selectPay->getChildByTag(200);
            image1->setVisible(false);
            image2->setVisible(true);
        }
    }
}

void WanBeiShop::touchIndexFun(Ref *pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED == type)
    {
         Layout *layout = (Layout *)pSender;
        
        if (lastItem == layout)
        {
            return;
        }
       
        Text *text1 = (Text *)layout->getChildByTag(100);
        int index = layout->getTag()- 500;

        layout->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
        text1->setTextColor(Color4B(0xfe,0x66,0x00,255));
        
        Text *text2 = (Text *)lastItem->getChildByTag(100);
        lastItem->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
        text2->setTextColor(Color4B(255,255,255,255));
        
        Json::Value list = wanList["elements"];
        
        char renMinS[100];
        sprintf(renMinS, "%d元", list[index]["rmb"].asInt());
        renMinValue->setString(renMinS);
        
        sprintf(renMinS, "%d钻石", list[index]["goodsNum"].asInt());
        wanValue->setString(renMinS);
        
        sprintf(renMinS, "立即支付:%d元", list[index]["rmb"].asInt());
        payTitle->setString(renMinS);
        
        lastItem = layout;
        
    }
}



