//
//  RecordDegitalLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/16.
//
//

#include "LuXiangReviewLayer.hpp"
#include "MatchItem.hpp"

bool LuXiangReviewLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("战绩回顾");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    auto winSize = Director::getInstance()->getVisibleSize();
    
//    Text *titleName = Text::create("系统仅保存您最近一次参与对战的录像，请及时查看", ".SFUIDisplay-Semibold", 40);
//    titleName->setPosition(Vec2(winSize.width/2, winSize.height - getHeadH() - 80));
//    titleName->setColor(Color3B(194, 174, 150));
//    this->addChild(titleName);

    return true;
}


void LuXiangReviewLayer::showView(Json::Value json)
{
    
    auto winSize = Director::getInstance()->getVisibleSize();
    
    Layout *layout1 = Layout::create();
    layout1->setTouchEnabled(true);
    layout1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    layout1->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    layout1->setAnchorPoint(Vec2(0, 1));
    layout1->ignoreAnchorPointForPosition(false);
    layout1->setContentSize(Size(winSize.width, 180));
    layout1->setPosition(Vec2(0, winSize.height - getHeadH() - 10));
    this->addChild(layout1);
    
    auto line1 = Layout::create();
    line1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line1->ignoreAnchorPointForPosition(false);
    line1->setBackGroundColor(Color3B(0, 0, 0));
    line1->setContentSize(Size(winSize.width, 3));
    layout1->addChild(line1);

    char buf[128];
    
    //房间名字
    Utils::parseName(15, buf, json["duijuName"].asString().c_str());
    Text *roomName = Text::create(buf , "", 40);
    roomName->setAnchorPoint(Vec2(0, 0.5));
    roomName->setPosition(Vec2(40, layout1->getContentSize().height/2));
    roomName->setColor(Color3B(0xcc, 0xcc, 0xcc));
    layout1->addChild(roomName);
    
    //geme logo
    Size imageSize = Size(110, 110);
    ImageView *logo = ImageView::create("Match/icon_wanbei.png");
    logo->setContentSize(imageSize);
    logo->setAnchorPoint(Vec2(1, 0.5));
    logo->ignoreContentAdaptWithSize(false);
    logo->setPosition(Vec2(winSize.width/2 + 200, layout1->getContentSize().height/2));
    layout1->addChild(logo);
    
    ZJHModel::getInstance()->loadingImageResource(logo, json["gameUrl"].asString());
    
    Utils::parseName(10, buf, json["gameTypeName"].asString().c_str());
    Text *gameName = Text::create(buf , "", 40);
    gameName->setAnchorPoint(Vec2(0, 0.5));
    gameName->setPosition(Vec2(logo->getPositionX() + 30, layout1->getContentSize().height/2));
    gameName->setColor(Color3B(194, 174, 150));
    layout1->addChild(gameName);
    
    ListView *viewList = ListView::create();
    viewList->ignoreAnchorPointForPosition(false);
    viewList->setDirection(ScrollView::Direction::VERTICAL);
    viewList->setBounceEnabled(false);
    viewList->setContentSize(Size(winSize.width, winSize.height - getHeadH()- 10 - 180 - 25));
    viewList->setItemsMargin(25);
    viewList->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(viewList);
    
    for (int i = 0; i < json["videoNumList"].size(); i++)
    {
        json["index"] = json["videoNumList"][i];
        LuXiangItem *item = LuXiangItem::createWithData(json);
        viewList->addChild(item);
    }
}
