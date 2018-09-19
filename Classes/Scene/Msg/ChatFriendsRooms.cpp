//
//  ChatFriendsRooms.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "ChatFriendsRooms.hpp"
#include "../../Model/MsgModel.h"
#include "proto.h"
#include "../Hall/HallRecommend.h"

bool ChatFriendsRooms::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setName("ChatFriendsRooms");
    
    return true;
}

void ChatFriendsRooms::myInit(Json::Value &json)
{
    _cuid = json["uid"].asInt();
    
    Size size = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(20, 20, 20, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", NULL);
    
    setTitle("好友房间");
    
    this->schedule(schedule_selector(ChatFriendsRooms::getData), 0.0f);
    
    pageNo = 1;
    
    Json::Value s_json;
    s_json["userId"] = _cuid;
    s_json["pageIndex"] = pageNo;
    s_json["pageSize"] = 16;
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"game/userRoomList",s_json.toStyledString(),"myFriendRoomList");
}

void ChatFriendsRooms::initList()
{
    pageNo = 1;
    myCount = 0;
    
    if(getChildByName("list"))removeChildByName("list");
    Size size = Director::getInstance()->getWinSize();
    if (dataList.size() == 0)
    {
        Text* text = Text::create("该好友还没有创建房间", "AmericanTypewriter", 35);
        text->setColor(Color3B(150, 150, 150));
        text->setPosition(Vec2(size.width / 2,size.height / 2));
        text->setName("tip");
        addChild(text);
        return;
    }
    
    removeChildByName("tip");
    
    listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(size.width, size.height - getHeadH()));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 0));
    listView->setScrollBarEnabled(true);
    listView->setName("list");
    addChild(listView);
    
    addDataInList();
    
    listView->setItemsMargin(20);
    listView->refreshView();
    
    listView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType)
                               {
                                   if (eventType == ScrollView::EventType::SCROLL_TO_BOTTOM) {
                                       if (hasMore)
                                       {
                                           pageNo ++;
                                           
                                           Json::Value json;
                                           json["userId"] = _cuid;
                                           json["pageIndex"] = pageNo;
                                           json["pageSize"] = 16;
                                           CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
                                               
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
                                                   dataList = loginPacket->recvVal["resultMap"]["roomList"]["result"];
                                                   hasMore = loginPacket->recvVal["resultMap"]["roomList"]["hasMore"].asBool();
                                                   addDataInList();
                                               }
                                               
                                               delete loginPacket;
                                               
                                           },"game/userRoomList",json.toStyledString(),"myFriendRoomList");
                                           
                                       }
                                   }
                               });
}

void ChatFriendsRooms::getData(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["myFriendRoomList"];
    if (loginPacket && loginPacket->status != 0)
    {
        unschedule(schedule_selector(ChatFriendsRooms::getData));
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        loginPacket->status = 0;
        
        if (loginPacket->resultIsOK())
        {
            dataList = loginPacket->recvVal["resultMap"]["roomList"]["result"];
            hasMore = loginPacket->recvVal["resultMap"]["roomList"]["hasMore"].asBool();
            initList();
        }
        else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets["myFriendRoomList"] = NULL;
        delete loginPacket;
    }
}

void ChatFriendsRooms::addDataInList()
{
    if(!listView)return;
    Size size = Director::getInstance()->getWinSize();
    
    float itemHeight = 430;
    
    for (int i = 0; i < dataList.size(); i ++)
    {
        int di = myCount % 2;
        int dj = myCount / 2;
        Layout* layout = (Layout*)listView->getChildByTag(dj);
        if (!layout)
        {
            layout = Layout::create();
            layout->setContentSize(Size(size.width, itemHeight));
            layout->setTag(dj);
            listView->pushBackCustomItem(layout);
        }
        
        float posX = (di ? 0.75 : 0.25) * size.width;
        float posY = itemHeight / 2;
        
        HotListItem *item = HotListItem::createWithData(dataList[i]);
        item->setAnchorPoint(Vec2(0.5, 0.5));
        item->setPosition(Vec2(posX, posY));
        layout->addChild(item);
        
        myCount++;
    }
}