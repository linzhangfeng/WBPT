//
//  NearFriends.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "NearFriends.hpp"
#include "ZJH.h"
#include "../Loading/Loading.h"

bool NearFriends::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setName("NearFriends");
    
    Size size = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(20, 20, 22, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    
    setTitle("附近的好友");
    
    this->schedule(schedule_selector(NearFriends::getData), 0.0f);
    
    pageNo = 1;
    hasMore = true;
    
    std::map<std::string, double> locationMap = PlatformHelper::getLocation();
    
    Json::Value json;
    json["pageNo"] = pageNo;
    json["pageSize"] = 30;
    json["latitude"] = locationMap["latitude"];//4.9E-324;
    json["longitude"] = locationMap["longitude"];//4.9E-324;
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"account/selectNearUser",json.toStyledString(),"nearFriendList");
    
    loading = Loading::startLoading(this,size / 2);
    return true;
}

void NearFriends::getData(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["nearFriendList"];
    if (loginPacket && loginPacket->status != 0)
    {
        loading->removeFromParent();
        
        unschedule(schedule_selector(NearFriends::getData));
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        loginPacket->status = 0;
        
        if (loginPacket->resultIsOK())
        {
            listData = loginPacket->recvVal["resultMap"];
            hasMore = loginPacket->recvVal["resultMap"]["hasMore"].asBool();
            initList();
        }
        else
        {
//            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets["nearFriendList"] = NULL;
        delete loginPacket;
    }
}

void NearFriends::initList()
{
    Size size = Director::getInstance()->getWinSize();
    
    if (listData["list"].size() == 0)
    {
        Text* text = Text::create("附近没有好友", "AmericanTypewriter", 45);
        text->setColor(Color3B(208, 208, 208));
        text->setPosition(Vec2(size.width / 2,size.height / 2));
        addChild(text);
        return;
    }
    
    listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setScrollBarEnabled(true);
    listView->setContentSize(Size(size.width, size.height - getHeadH()));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 0));
    listView->setName("list");
    addChild(listView);
    
    addDataInList();
    
    listView->setItemsMargin(5);
    listView->refreshView();
    
    listView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType)
                               {
                                   if (eventType == ScrollView::EventType::SCROLL_TO_BOTTOM) {
                                       if (hasMore)
                                       {
                                           pageNo ++;
                                           
                                           std::map<std::string, double> locationMap = PlatformHelper::getLocation();
                                           
                                           Json::Value json;
                                           json["pageNo"] = pageNo;
                                           json["pageSize"] = 30;
                                           json["latitude"] = locationMap["latitude"];//4.9E-324;
                                           json["longitude"] = locationMap["longitude"];//4.9E-324;
                                           CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
                                               if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
                                               CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
                                               CCHttpAgent::getInstance()->packets.erase(tag);
                                               CCHttpAgent::getInstance()->callbacks.erase(tag);
                                               if (loginPacket->status != 3)
                                               {
                                                   PlatformHelper::showToast("网络链接失败，请稍后再试");
                                                   delete loginPacket;
                                                   return;
                                               }
                                               
                                               loginPacket->status = 0;
                                               
                                               if (loginPacket->resultIsOK())
                                               {
                                                   listData = loginPacket->recvVal["resultMap"];
                                                   hasMore = loginPacket->recvVal["resultMap"]["hasMore"].asBool();
                                                   addDataInList();
                                               }
                                               
                                               delete loginPacket;
                                               
                                           },"account/selectNearUser",json.toStyledString(),"nearFriendList");
                                       }
                                   }
                               });
}

void NearFriends::addDataInList(bool init)
{
    if(!listView)return;
    for (int i = 0; i < listData["list"].size(); i ++)
    {
        NearFriends_Item* item = NearFriends_Item::create();
        item->initData(listData["list"][i]);
        
        auto layout = Layout::create();
        layout->setContentSize(item->getContentSize());
        layout->addChild(item);
        listView->pushBackCustomItem(layout);
    }
}

//------------------------NearFriendItem---------------------------
bool NearFriends_Item::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    return true;
}

/*
 {
 "orderIndex": null,
 "user": {
     "id": 101365,
     "nickName": "玩呗101365",
     "image": "http://188.188.0.111:20084/chess/chess_img/head/101365/1607011613113289_120x120.jpg",
     "phone": "15309865370",
     "remarkName": "",
     "signature": null,
     "sex": 1,
     "vipLv": 0
     },
 "distance": 0,
 "lastUpdate": null,
 "isFriend": true
 }
 */
void NearFriends_Item::initData(Json::Value &json)
{
    int uid = json["user"]["id"].asInt();
    
    Size size = Director::getInstance()->getWinSize();
    
    setContentSize(Size(size.width, 222));
    
    Size itemSize = getContentSize();
    
    LayerColor* bg = LayerColor::create(Color4B(50, 50, 50, 255));
    bg->setContentSize(Size(itemSize.width - 50,2));
    bg->setAnchorPoint(Vec2(0.0f, 0.0f));
    bg->setPosition(50,0);
    addChild(bg, -100);
    
    Sprite* userAvatar = Utils::createCircleAvatar(json["user"]["image"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(144,144));
    userAvatar->setPosition(Vec2(100, itemSize.height / 2));
    this->addChild(userAvatar);
    
    char buf[128];
    Utils::parseName(10, buf, json["user"]["nickName"].asString().c_str());
    
    Text* text = Text::create(buf, "AmericanTypewriter", 45);
    text->setColor(Color3B(208, 208, 208));
    text->setAnchorPoint(Vec2(0, 0.5));
    text->setPosition(Vec2(200,144));
    addChild(text,1000);
    
    float dis = json["distance"].asInt();
    __String* str = NULL;
    if (dis < 100)
    {
        str = __String::createWithFormat("100米内");
    }
    else if (dis < 300)
    {
        str = __String::createWithFormat("300米内");
    }
    else if (dis < 500)
    {
        str = __String::createWithFormat("500米内");
    }
    else if (dis < 800)
    {
        str = __String::createWithFormat("800米内");
    }
    else if (dis < 800)
    {
        str = __String::createWithFormat("1000米内");
    }
    else
    {
        str = __String::createWithFormat("1000米以上");
    }
    text = Text::create(str->getCString(), "AmericanTypewriter", 40);
    
    text->setColor(Color3B(112, 112, 112));
    text->setAnchorPoint(Vec2(0, 0.5));
    text->setPosition(Vec2(200,68));
    addChild(text,1000);
    
    Text* add = Text::create("", "AmericanTypewriter", 40);
    
    add->setAnchorPoint(Vec2(0.5, 0.5));
    add->setPosition(Vec2(size.width - 150,itemSize.height * 0.5));
    addChild(add,1000);
    
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
        m->setPosition(Vec2(size.width - 150,itemSize.height * 0.5));
        m->setPressedActionEnabled(true);
        addChild(m,1000);
        
        auto send_bg = ui::Scale9Sprite::create("Msg/btn_gray_small.png");
        send_bg->setPreferredSize(Size(Size(200, 85)));
        send_bg->setAnchorPoint(Vec2(0.5, 0.5));
        send_bg->setPosition(m->getContentSize() / 2);
        m->addChild(send_bg,-100);

        m->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
                                    {
            if (type == Widget::TouchEventType::ENDED)
            {
                Json::Value json_s;
                json_s["friendId"] = uid;
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
                       PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                    }
                    
                    CCHttpAgent::getInstance()->packets[tag] = NULL;
                    delete loginPacket;
                    
                },"friend/addFriend",json_s.toStyledString(),"nearFriend_addFriend");
            }
        });
    }
}