//
//  PropShopLayer.cpp
//  ZJH
//
//  Created by apple on 16/8/29.
//
//

#include "PropShopLayer.hpp"
#include "../Match/JoinTipLayer.hpp"

#define  MIN1               3
#define LEFTW               30


bool PropShopLayer::init()
{
    if (!Widget::init())
    {
        return false;
    }
    
    auto winSize = Director::getInstance()->getVisibleSize();
    this->setContentSize(Size(winSize.width, winSize.height - 270 - 135));
    
    //2:道具请求
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"store/findbuy", "", "propShop");
    this->schedule(schedule_selector(PropShopLayer::postProp), 0.0f);

    return true;
}

void PropShopLayer::postProp(float dt)
{
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["propShop"];
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
            propJson = data;
            propView(data);
            log("道具：＝＝＝ [%s]",data.toStyledString().c_str());
        }
        else
        {
            PlatformHelper::showToast(packet->recvVal["message"].asCString());
            unschedule(schedule_selector(PropShopLayer::postProp));
        }
        
        CCHttpAgent::getInstance()->packets["propShop"] = NULL;
        delete packet;
    }

}

void PropShopLayer::propView(Json::Value jsonData)
{
    this->removeAllChildren();
    auto winSize = Director::getInstance()->getVisibleSize();
    float H = this->getContentSize().height;
    
    ListView* listView1 = ListView::create();
    listView1->setDirection(ScrollView::Direction::VERTICAL);
    listView1->setBounceEnabled(false);
    listView1->setContentSize(Size(winSize.width, H - 180));
    listView1->setAnchorPoint(Vec2(0, 0));
    listView1->setPosition(Vec2(0, 180));
    listView1->setItemsMargin(20);
    listView1->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(listView1);
    
    Size rowSize = Size(1000, 395);
    int nums = jsonData.size();
    for (int i = 0; i < nums; i+=2)
    {
        Layout *layout = Layout::create();
        layout->setTouchEnabled(true);
        layout->addTouchEventListener(CC_CALLBACK_2(PropShopLayer::buyPropOn, this));
        layout->setTag(100+i);
        layout->setContentSize(rowSize);
        
        ImageView *item = createItem(jsonData[i]);
        item->setPosition(Vec2(0, 0));
        
        layout->addChild(item);
        
        if (i + 1 < nums)
        {
            item = createItem(jsonData[i+1]);
            item->setPosition(Vec2(20+490, 0));
            layout->addChild(item);
        }
        listView1->addChild(layout);
    }
    
    //我的道具
    auto myCell = Layout::create();
    myCell->setTouchEnabled(true);
    myCell->addTouchEventListener(CC_CALLBACK_2(PropShopLayer::gotoMyPropOn, this));
    myCell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    myCell->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    myCell->setContentSize(Size(winSize.width, 180));
    this->addChild(myCell);
    
    Text *markName = Text::create("我的道具", "", 36);
    markName->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
    markName->setAnchorPoint(Vec2(0, 0.5));
    markName->setPosition(Vec2(30, myCell->getContentSize().height/2));
    myCell->addChild(markName);
    
    ImageView *left = ImageView::create("Public/detail_arrow.png");
    left->setPosition(Vec2(winSize.width - 30, markName->getPositionY()));
    myCell->addChild(left);
}

void PropShopLayer::gotoMyPropOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        ZJHModel::getInstance()->gotoView(VIEW_MY_PROP);
    }
}

void PropShopLayer::buyPropOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        Layout *prop = (Layout*)pSender;
        int index = prop->getTag()-100;
        Json::Value json = propJson[index];
        json["goldenNum"] = moneyJson["goldenNum"];
        json["wanbei"]= moneyJson["wanbei"];
       
        
        JoinTipLayer *tip = JoinTipLayer::create();
        tip->setPosition(Vec2::ZERO);
        tip->buyPropView(json);
        this->addChild(tip);
    }
}

void PropShopLayer::getMoney(Json::Value json)
{
    moneyJson = json;
}
ImageView*  PropShopLayer::createItem(Json::Value json)
{
    ImageView* item = ImageView::create("Mine/shop_prop_Bg.png");
    item->setAnchorPoint(Vec2(0, 0));
    
    float itemW = item->getContentSize().width;
    
    //图片
    ImageView *image = ImageView::create("Match/icon_wanbei.png");
    image->ignoreContentAdaptWithSize(false);
    image->setAnchorPoint(Vec2(0.5, 0));
    image->setContentSize(Size(300, 190));
    image->setPosition(Vec2(itemW/2, 135));
    item->addChild(image);
    
    ZJHModel::getInstance()->loadingImageResource(image, json["remark"].asString());
    
    auto titleBg = Layout::create();
    titleBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    titleBg->setBackGroundColor(Color3B(0x15, 0x15, 0x18));
    titleBg->setContentSize(Size(itemW, 60));
    titleBg->setPosition(Vec2(0, 75));
    item->addChild(titleBg);
    
    Text *name = Text::create(json["name"].asString(), "", 36);
    name->setAnchorPoint(Vec2(0, 0.5));
    name->setColor(Color3B(0xe5, 0xe5, 0xe5));
    name->setPosition(Vec2(20, titleBg->getContentSize().height/2));
    name->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    titleBg->addChild(name);
    
    //
    auto nameBg = Layout::create();
    nameBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    nameBg->setBackGroundColor(Color3B(0x29, 0x29, 0x2a));
    nameBg->setContentSize(Size(itemW, 75));
    item->addChild(nameBg);
    
    //
    Text *description = Text::create(json["description"].asString(), "", 36);
    description->setAnchorPoint(Vec2(0, 0.5));
    description->setTextColor(Color4B(0xd7, 0xc7, 0xa1, 255));
    description->setPosition(Vec2(20, nameBg->getContentSize().height/2));
    description->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    nameBg->addChild(description);
    
    char buf[10];
    Text *price;
    if(json["payType"].asInt() == 1)
    {
        //金币
        sprintf(buf, "%.f", json["price"].asDouble());
        price = Text::create((string)buf, "", 36);
        
    }else if(json["payType"].asInt() == 2)
    {
        //玩贝
        sprintf(buf, "%.f", json["price"].asDouble());
        price = Text::create((string)buf, "", 36);
    }
    price->setTextColor(Color4B(0xd7, 0xc7, 0xa1, 255));
    price->setAnchorPoint(Vec2(1, 0.5));
    price->setPosition(Vec2(itemW - 20, 37));
    price->setTextHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
    nameBg->addChild(price);
    
    ImageView *shell = ImageView::create("Mine/shop_shell_prop_small.png");
    shell->setAnchorPoint(Vec2(1, 0.5));
    shell->setPosition(Vec2(itemW - 100, 37));
    nameBg->addChild(shell);
    
    return item;
}
