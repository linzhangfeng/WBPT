//
//  GoldShopLayer.cpp
//  ZJH
//
//  Created by apple on 16/8/29.
//
//

#include "CardShopLayer.hpp"
#include "TipShopLayer.hpp"

bool CardShopLayer::init()
{
    if (!Widget::init())
    {
        return false;
    }
    auto winSize = Director::getInstance()->getVisibleSize();
    float H = winSize.height - 270 - 135 - 160;
    this->setContentSize(Size(winSize.width, H));
    
    //3:请求
    Json::Value json2;
    json2["type"] = "PROP_ROOMCARD";
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"store/find.action",json2.toStyledString(),"cardShop");
    this->schedule(schedule_selector(CardShopLayer::postCard), 0.0f);
    
    return true;
}

void CardShopLayer::postCard(float dt)
{
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["cardShop"];
    if (packet && packet->status != 0)
    {
        if (packet->status != 3)
        {
            return;
        }
        packet->status = 0;
        
        if (packet->resultIsOK())
        {
            Json::Value data = packet->recvVal["resultMap"]["info"];
            cardList = data;
            data = data["elements"];
            cardView(data);
            
            log("钱数：＝＝＝ [%s]",data.toStyledString().c_str());
        }
        else
        {
            PlatformHelper::showToast(packet->recvVal["message"].asCString());
            
        }
        unschedule(schedule_selector(CardShopLayer::postCard));
        CCHttpAgent::getInstance()->packets["cardShop"] = NULL;
        delete packet;
    }
}

void CardShopLayer::cardView(Json::Value jsonData)
{
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
        
        __String *bufStr = __String::createWithFormat("Mine/card%d.png", i+1);
        ImageView *gold = ImageView::create(bufStr->getCString());
        gold->setAnchorPoint(Vec2(0, 0.5));
        gold->setPosition(Vec2(40, list2->getContentSize().height/2));
        list2->addChild(gold);
        
//        ImageView *gold = ImageView::create();
//        gold->setAnchorPoint(Vec2(0, 0.5));
//        gold->setPosition(Vec2(40, list2->getContentSize().height/2));
//        list2->addChild(gold);
//        ZJHModel::getInstance()->loadingImageResource(gold, jsonData[i]["logo"].asString());
        
        char priceStr[128];
        sprintf(priceStr, "%d房卡", jsonData[i]["goodsNum"].asInt());
        
        Text *text = Text::create(priceStr, "", 40);
        text->setPosition(Vec2(220, gold->getPositionY()));
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        text->setTextColor(Color4B(0xf1, 0xcd, 0x5c, 255));
        list2->addChild(text);
        
        int price = jsonData[i]["currentPrice"].asInt();
        sprintf(priceStr, "%d钻石", price);
        
        Button *buyBut = Button::create("Mine/btn_yellow_small.png", "Mine/btn_yellow_small_check.png");
        buyBut->setScale9Enabled(true);
        buyBut->setAnchorPoint(Vec2(1, 0.5));
        buyBut->setContentSize(Size(220 , 90));
        buyBut->setPosition(Vec2(winSize.width - 40, list2->getContentSize().height/2));
        list2->addChild(buyBut);
        buyBut->setTag(100+i);
        buyBut->addTouchEventListener(CC_CALLBACK_2(CardShopLayer::buyGoldFun, this));
        
        Text *wanBtn = Text::create(priceStr, "",40);
        wanBtn->setPosition(Vec2(buyBut->getContentSize().width/2, buyBut->getContentSize().height/2));
        wanBtn->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        wanBtn->setTextColor(Color4B(0x60,0x41,0x10,255));
        buyBut->addChild(wanBtn);
        
        /*
        if (jsonData[i]["originalPrice"].asInt() != jsonData[i]["currentPrice"].asInt())
        {
            sprintf(priceStr, "原价＝%d玩贝", jsonData[i]["originalPrice"].asInt());
            Text *yuanValue = Text::create(priceStr, "",30);
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
            Text *wanValue = Text::create(priceStr, "",30);
            wanValue->setPosition(Vec2(winSize.width - 145, 15));
            wanValue->setAnchorPoint(Vec2(0.5, 0));
            wanValue->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
            wanValue->setTextColor(Color4B(0xfe,0x7a,0x00, 255));
            list2->addChild(wanValue);
        }
        */
    }
}


void CardShopLayer::buyGoldFun(Ref *pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED == type)
    {
        Button *but = (Button *)pSender;
        //用玩呗买房卡
        int id = cardList["id"].asInt();
        int num = cardList["elements"][but->getTag()-100]["goodsNum"].asInt();
        
        char idC[128];
        sprintf(idC, "%d", id);
        
        tipJson["buyGoldNum"] = num;
        tipJson["currentPrice"] = cardList["elements"][but->getTag()-100]["currentPrice"];
        tipJson["id"] = idC;
        
        TipShopLayer *tip = TipShopLayer::create();
        tip->setPosition(Vec2::ZERO);
        if (tipJson["wanbei"].asInt() < tipJson["currentPrice"].asInt())
        {
            tip->buyCardGoldWanRechange();
        }else
        {
            tip->buyCardGold(2, tipJson);
        }
        
        this->addChild(tip);
    }
}

void CardShopLayer::getMoney(Json::Value json)
{
    tipJson = json;
}