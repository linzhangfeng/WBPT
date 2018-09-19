


//
//  GameWinLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/15.
//
//

#include "GameWinLayer.hpp"
#include "MatchItem.hpp"

bool GameWinLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("游戏胜率");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
  
    setBackground(Color4B(0x14, 0x14, 0x15, 255));

    //发送请求数据
    postData();
    return true;
}

void  GameWinLayer::postData()
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
            Json::Value data = loginPacket->recvVal["resultMap"]["playWinRateList"];
            showView(data);
        }
        
    },"duiju/integral/gameWinRate","","gameWinRate");
    
}

void GameWinLayer::showView(Json::Value list)
{
    Size winSize = Director::getInstance()->getWinSize();
    float H = winSize.height - getHeadH();

    viewList = ListView::create();
    viewList->ignoreAnchorPointForPosition(false);
    viewList->setDirection(ScrollView::Direction::VERTICAL);
    viewList->setBounceEnabled(false);
    viewList->setContentSize(Size(winSize.width, H));
    viewList->setAnchorPoint(Vec2(0, 0));
    viewList->setPosition(Vec2(0, 0));
    viewList->setItemsMargin(30);
    viewList->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(viewList);
    
    int nums = list.size();
    if (nums == 0)
    {
        Text *text = Text::create("暂无纪录", "", 36);
        text->setPosition(Vec2(winSize.width/2, H/2));
        text->setTextColor(Color4B::GRAY);
        this->addChild(text);
        return;
    }
    //创建item
    for (int i = 0; i < nums; i++)
    {
        WinRateItem *item = WinRateItem::createWithData(list[i]);
        viewList->addChild(item);
    }
}

