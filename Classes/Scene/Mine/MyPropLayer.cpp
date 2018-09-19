//
//  MyPropLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/21.
//
//

#include "MyPropLayer.hpp"

bool MyPropLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("我的道具");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    
    setBackground(Color4B(0x14, 0x14, 0x15, 255));
    
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
            Json::Value data = loginPacket->recvVal["resultMap"]["info"];
            initView(data);
            
            log("我的道具：＝＝＝ [%s]", loginPacket->recvVal["resultMap"].toStyledString().c_str());
        }
    },"store/findmygoods", "" ,"mygoods");
    

    return true;
}

void MyPropLayer::initView(Json::Value jsonData)
{
    auto winSize = Director::getInstance()->getVisibleSize();
    float H = winSize.height - getHeadH() - 3;

    listView1 = ListView::create();
    listView1->setDirection(ScrollView::Direction::VERTICAL);
    listView1->setBounceEnabled(false);
    listView1->setContentSize(Size(winSize.width, H));
    listView1->setItemsMargin(20);
    listView1->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(listView1);
    
    
    float itemW = 1000;
    float itemH = 395;
    Size rowSize = Size(itemW, itemH);
    int nums = jsonData.size();
    
    bool isNull = true;
    for (int i = 0; i < nums; i++)
    {
        if(jsonData[i]["goodsNum"].asInt() != 0)
        {
            isNull = false;
            break;
        }
    }
    
    if (isNull)
    {
        Text *text = Text::create("你还没有购买道具", "", 36);
        text->setPosition(Vec2(winSize.width/2, H/2));
        text->setTextColor(Color4B::GRAY);
        this->addChild(text);
        return;
    }
    
    for (int i = 0; i < nums; i+=2)
    {
        Layout *layout = Layout::create();
        layout->setContentSize(rowSize);
        
        ImageView *item = createItem(jsonData[i]);
        item->setPosition(Vec2(0, 0));
        
        layout->addChild(item);
        
        if (i + 1 < nums)
        {
            item = createItem(jsonData[i+1]);
            item->setPosition(Vec2(20+495, 0));
            layout->addChild(item);
        }
        listView1->addChild(layout);
    }

}

ImageView* MyPropLayer::createItem(Json::Value json)
{
    ImageView* item = ImageView::create("Mine/shop_prop_Bg.png");
    item->setAnchorPoint(Vec2(0, 0));

    float itemW = item->getContentSize().width;
    float itemH = item->getContentSize().height;
    
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
    

    char buf[20];
    ImageView *numBg = ImageView::create("Mine/shop_my_prop_numBg.png");
    numBg->setAnchorPoint(Vec2(1, 1));
    numBg->setPosition(Vec2(itemW, itemH - 20));
    item->addChild(numBg);
    
    
    sprintf(buf, "%d", json["goodsNum"].asInt());
    Text *num = Text::create(buf, "", 36);
    num->setTextColor(Color4B(0x65, 0x40, 0x02, 255));
    num->setPosition(Vec2(numBg->getContentSize().width/2, numBg->getContentSize().height/2));
    numBg->addChild(num);
    
    return item;
}
