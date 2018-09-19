

//
//  InviteFriendListLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/13.
//
//

#include "InviteFriendListLayer.hpp"
#define CELLH2  200
#define CELLH1  150
#define ITERH_MAX   30
#define ITERH_MIN   3

#define TEXT_LEFT_W    30 //文字左边距

bool InviteFriendListLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("推荐的好友");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });

    setBackground(Color4B(0x14, 0x14, 0x15, 255));
    pageIndex = 0;
    isFirstPost = false;
    Json::Value json;
    json["pageIndex"] = pageIndex;
    json["pageSize"] = 10;
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
                                                 if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
                                                 CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                 if (packet) {
                                                     if (getReferenceCount() == 0 || getReferenceCount() > 10)
                                                     {
                                                         //nothing
                                                     }
                                                     else
                                                     {
                                                         if (packet->status == 3) {
                                                             packet->status = 0;
                                                             if (packet->resultIsOK())
                                                             {
                                                                 Json::Value data = packet->recvVal["resultMap"];
                                                                 printf("FFFFFFFFF = %s", data.toStyledString().c_str());
                                                                 isFirstPost = true;
                                                                 showView(data);
                                                             }
                                                             else
                                                             {
                                                                 PlatformHelper::showToast(packet->recvVal["message"].asCString());
                                                             }
                                                             
                                                         }
                                                     }
                                                     
                                                     //删除packet
                                                     CCHttpAgent::getInstance()->packets.erase(tag);
                                                     delete packet;
                                                 }
                                             },
                                             "invite/list", json.toStyledString(), "inviteList");
    
    
    return true;
}

void InviteFriendListLayer::showNoListView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float W = visibleSize.width;
    float H = visibleSize.height - getHeadH();
    Layout* layout = Layout::create();
    layout->setContentSize(Size(W, H));
    layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    layout->setBackGroundColor(Color3B(245, 245, 245));
    this->addChild(layout);
    
    Text *text = Text::create("没有邀请过好友", "", 36);
    text->setPosition(Vec2(W/2, H/2));
    text->setTextColor(Color4B::GRAY);
    layout->addChild(text);
    
}

void InviteFriendListLayer::showView(Json::Value json1)
{
    Size winSize = Director::getInstance()->getWinSize();
    Json::Value json = json1["list"]["result"];
    int nums = json.size();
    if (nums == 0 && isFirstPost == true)
    {
        showNoListView();
        return;
    }
    
    if (listView == NULL)
    {
        listView = ListView::create();
        listView->setDirection(ScrollView::Direction::VERTICAL);
        listView->setBounceEnabled(false);
        listView->setContentSize(Size(winSize.width, winSize.height - getHeadH()));
        listView->setAnchorPoint(Vec2(0, 0));
        listView->setPosition(Vec2(0, 0));
        listView->setItemsMargin(50);
        listView->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
        this->addChild(listView);
    }
    for (int i = 0; i < nums; i++)
    {
        Layout *item = cellView(json[i]);
        listView->addChild(item);
    }
    listView->refreshView();

    if (json1["list"]["hasMore"].asBool())
    {
        pageIndex++;
        listView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType){
            if (eventType == ScrollView::EventType::SCROLL_TO_BOTTOM)
            {
                PlatformHelper::showToast("到达底部");
                Json::Value json;
                json["pageIndex"] = pageIndex;
                json["pageSize"] = 10;
                
                CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                                         {
                                                             if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
                                                             CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                             if (packet) {
                                                                 if (getReferenceCount() == 0 || getReferenceCount() > 10)
                                                                 {
                                                                     //nothing
                                                                 }
                                                                 else
                                                                 {
                                                                     if (packet->status == 3)
                                                                     {
                                                                         packet->status = 0;
                                                                         if (packet->resultIsOK())
                                                                         {
                                                                             isFirstPost = false;
                                                                             Json::Value data = packet->recvVal["resultMap"];
                                                                             printf("DDDDDDDDDDDD = %s", data.toStyledString().c_str());
                                                                             showView(data);
                                                                         }
                                                                         else
                                                                         {
                                                                             PlatformHelper::showToast(packet->recvVal["message"].asCString());
                                                                         }
                                                                         
                                                                     }
                                                                  }
                                                                 
                                                                 //删除packet
                                                                 CCHttpAgent::getInstance()->packets.erase(tag);
                                                                 delete packet;
                                                             }
                                                         },
                                                         "invite/list", json.toStyledString(), "inviteList");
                
            }
        });
    }
}

Layout* InviteFriendListLayer::cellView(Json::Value json)
{
    Size winSize = Director::getInstance()->getWinSize();
    Layout* layout = Layout::create();
    layout->setContentSize(Size(winSize.width, 180));
    layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    layout->setBackGroundColor(Color3B(0x14, 0x14, 0x15));
    
    Layout* line = Layout::create();
    line->setContentSize(Size(winSize.width - 40, 3));
    line->setPosition(Vec2(40, 0));
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->setBackGroundColor(Color3B(0x30, 0x30, 0x31));
    layout->addChild(line);
    
    //头像
   Sprite* portrit = Utils::createCircleAvatar(Utils::getImageSavePath(json["user"]["image"].asString()) , "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(120,120));
    portrit->setPosition(Vec2(100, 90));
    layout->addChild(portrit);
    
    //name
    Text *name = Text::create(json["user"]["nickName"].asString(), "", 36);
    name->setAnchorPoint(Vec2(0, 0.5));
    name->setTextHorizontalAlignment(TextHAlignment::LEFT);
    name->setPosition(Vec2(185, 90));
    name->setTextColor(Color4B(0xcc, 0xcc, 0xcc, 255));
    layout->addChild(name);
    
    //添加状态
    Text* add = Text::create("", "", 40);
    add->setAnchorPoint(Vec2(0.5, 0.5));
    add->setPosition(Vec2(winSize.width - 150,90));
    layout->addChild(add,1000);
    
    if (json["isFriend"].asBool())
    {
        add->setString("已添加");
        add->setColor(Color3B(50, 50, 50));
    }
    else
    {
        auto m = Button::create();
        m->setContentSize(Size(200,200));
        m->ignoreContentAdaptWithSize(false);
        m->setTitleText("添加");
        m->setTitleFontSize(45);
        m->setTitleColor(Color3B(208, 208, 208));
        m->setZoomScale(0.1f);
        m->setAnchorPoint(Vec2(0.5, 0.5));
        m->setPosition(Vec2(winSize.width - 150, 90));
        m->setPressedActionEnabled(true);
        layout->addChild(m,1000);
        m->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
                                 {
                                     if (type == Widget::TouchEventType::ENDED)
                                     {
                                         Json::Value json_s;
                                         json_s["friendId"] = json["user"]["id"];
                                         CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
                                             if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
                                             
                                             CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
                                             
                                             if (loginPacket->status != 3)
                                             {
                                                 PlatformHelper::showToast("网络链接失败，请稍后再试");
                                                 return;
                                             }
                                             
                                             loginPacket->status = 0;
                                             
                                             if (loginPacket->resultIsOK())
                                             {
                                                 m->setVisible(false);
                                                 add->setString("已添加");
                                                 add->setColor(Color3B(50, 50, 50));
                                                 __NotificationCenter::getInstance()->postNotification("MyFriend_reflash");
                                                 PlatformHelper::showToast("添加成功");
                                             }
                                             else
                                             {
                                                 PlatformHelper::showToast(loginPacket->recvVal["message"].asString().c_str());
                                             }
                                             
                                             CCHttpAgent::getInstance()->packets[tag] = NULL;
                                             delete loginPacket;
                                             
                                         },"friend/addFriend",json_s.toStyledString(),"addFriend");
                                     }
                                 });

        
    }
    return layout;
}
