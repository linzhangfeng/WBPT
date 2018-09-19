//
//  HallFriend.cpp
//  ZJH
//
//  Created by hyhy on 16/7/7.
//
//

#include "HallFriend.h"
#include "HallRecommend.h"

bool HallFriend::init()
{
    if (Layer::init() == false) {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
    setContentSize(Size(winSize.width, winSize.height - 310));
    
   initList();
    
    return true;
}

void HallFriend::initList()
{
    Size winSize = Director::getInstance()->getWinSize();
    
    ListView *listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setTouchEnabled(false);
    listView->setContentSize(this->getContentSize());
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 0));
    listView->setScrollBarEnabled(true);
    listView->setItemsMargin(30);
    listView->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    addChild(listView);
    
    ///@todo test 暂时没收到朋友列表的信息，先用热门房间代替
    Json::Value &data = CCHttpAgent::getInstance()->packets["hallinit"]->recvVal["resultMap"]["hotList"];
    Widget *friendList = this->getFriendList(data);
    
    listView->addChild(friendList);
}

Widget *HallFriend::getFriendList(Json::Value &data)
{
    Size winSize = Director::getInstance()->getWinSize();
    Widget *friendWidget = Widget::create();
    int num = data.size();
    float itemHeight = 430;
    float topMargin = 30;
    
    ///@todo test
    num = 5;
    
    
    if (num > 0) {
        Size widgetSize = Size(winSize.width, topMargin + (num + 1) / 2 * itemHeight);
        friendWidget->setContentSize(widgetSize);
        
//        LayerColor *bg = LayerColor::create(Color4B(255, 255, 255, 255), widgetSize.width, widgetSize.height);
//        bg->setPosition(Vec2(0, 0));
//        friendWidget->addChild(bg, -10);
        
//        Text *title = Text::create("热门房间", "AmericanTypewriter", 50);
//        title->setColor(Color3B::GRAY);
//        title->setPosition(Vec2(100, widgetSize.height - 50));
//        friendWidget->addChild(title);
        
//        Button *moreBtn = Button::create("Head/title-icon8_press.png");
//        moreBtn->setContentSize(Size(150, 60));
//        moreBtn->setAnchorPoint(Vec2(1, 0.5));
//        moreBtn->setPosition(Vec2(widgetSize.width - 20, title->getPositionY()));
//        friendWidget->addChild(moreBtn);
//        moreBtn->addClickEventListener([](Ref *ref)
//                                       {
//                                           PlatformHelper::showToast("显示更多热门房间");
//                                       });
//        
//        Text *moreText = Text::create("更多", "AmericanTypewriter", 50);
//        moreText->setPosition(Vec2(widgetSize.width - 150, title->getPositionY()));
//        moreText->setColor(Color3B::GRAY);
//        friendWidget->addChild(moreText);
//        
//        Sprite *moreIcon = Sprite::create("Head/title-icon2_press.png");
//        moreIcon->setRotation(180);
//        moreIcon->setPosition(Vec2(widgetSize.width - 60, title->getPositionY()));
//        friendWidget->addChild(moreIcon);
        
        
        for (int i = 0; i < num; i++) {
            float posX = (i%2 ? 0.75 : 0.25) * winSize.width;
            float posY = widgetSize.height - topMargin - itemHeight * (i / 2);
            
            HotListItem *item = HotListItem::createWithData(data[i]);
            item->setAnchorPoint(Vec2(0.5, 1.0));
            item->setPosition(Vec2(posX, posY));
            friendWidget->addChild(item);
        }
    }
    
    return friendWidget;
}
