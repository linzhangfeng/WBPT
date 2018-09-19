//
//  MineMoreLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/27.
//
//

#include "BuLuoInfoLayer.hpp"
bool BuLuoInfoLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("俱乐部信息");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));

    return true;
}

void BuLuoInfoLayer::showView(Json::Value jsonData)
{
    auto visibleSize = Director::getInstance()->getWinSizeInPixels();
    float H = visibleSize.height - getHeadH();

    Sprite *userPortrit = Utils::createCircleAvatar(jsonData["logo"].asString() , "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(260, 260));
    userPortrit->setPosition(Vec2(visibleSize.width/2, H - 190));
    this->addChild(userPortrit);
    
    //name
    Text *name = Text::create(jsonData["name"].asString(), "", 44);
    name->setAnchorPoint(Vec2(0.5, 1));
    name->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    name->setPosition(Vec2(visibleSize.width/2, H - 320));
    name->setTextColor(Color4B(0xe2, 0xce, 0xb0, 255));
    this->addChild(name);
    
    char buf[128];
    sprintf(buf, "等级%d级", jsonData["level"].asInt());
    Text *LV = Text::create(buf, "", 40);
    LV->setAnchorPoint(Vec2(0.5, 1));
    LV->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    LV->setPosition(Vec2(visibleSize.width/2, H - 385));
    LV->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
    this->addChild(LV);

    ListView *listView1 = ListView::create();
    listView1->setInertiaScrollEnabled(false);
    listView1->setDirection(ScrollView::Direction::VERTICAL);
    listView1->setBounceEnabled(false);
    listView1->setContentSize(Size(visibleSize.width, H - 455));
    listView1->setAnchorPoint(Vec2(0, 0));
    listView1->setPosition(Vec2(0, 0));
    listView1->setItemsMargin(0);
    listView1->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(listView1);
    
    Size cellSize = Size(visibleSize.width, 130);
    for (int i = 0; i < 7; i++)
    {
        auto cell = Layout::create();
        cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        cell->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
        cell->setContentSize(cellSize);
        listView1->addChild(cell);
        
        Text *left = Text::create("", "", 40);
        left->setAnchorPoint(Vec2(0, 0.5));
        left->setTextColor(Color4B(0xe6, 0xe6, 0xe6, 255));
        left->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        left->setPosition(Vec2(40, cellSize.height/2));
        cell->addChild(left);
        
        Layout *line = Layout::create();
        line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        line->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
        line->setPosition(Vec2(40, 0));
        line->setContentSize(Size(cellSize.width - 40, 1));
        cell->addChild(line);
        
        Text *right = Text::create("", "", 40);
        right->setAnchorPoint(Vec2(1, 0.5));
        right->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
        right->setTextHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
        right->setPosition(Vec2(cellSize.width - 40, cellSize.height/2));
        cell->addChild(right);
        
        char buf[128];
        
        switch (i)
        {
            case 0:
                left->setTextColor(Color4B(0xe2, 0xce, 0xb0, 255));
                left->setString("基本信息");
                
                break;
            case 1:
                left->setString("俱乐部名称");
                right->setString(jsonData["name"].asString());
                break;
                
            case 2:
                left->setString("俱乐部ID");
                sprintf(buf, "%d", jsonData["id"].asInt());
                right->setString(buf);
                break;
                
            case 3:
                left->setString("开通日期");
                right->setString(createTimeBecomeString(jsonData["createDatetime"].asDouble()/1000));
                break;
            case 4:
                
                left->setString("俱乐部位置");
                right->setString(jsonData["address"].asString());
                break;
            case 5:
                
                left->setString("俱乐部积分");
                right->setString("敬请期待");
                break;
            case 6:
                
                left->setString("俱乐部等级");
                sprintf(buf, "等级%d级", jsonData["level"].asInt());
                right->setString(buf);
                line->setBackGroundColor(Color3B(0, 0, 0));
                line->setPosition(Vec2(0, 0));
                line->setContentSize(Size(cellSize.width, 2));
        }
        
    }
    
    
}

string BuLuoInfoLayer::createTimeBecomeString(long long sec)
{
    time_t time_sec1 = sec;

    struct tm * test = localtime(&time_sec1);
    int year = test->tm_year + 1900;
    int mon = test->tm_mon+1;
    int day = test->tm_mday;
    
    char buf[128];
    sprintf(buf, "%d.%d.%d", year, mon, day);
    
    return buf;
}
