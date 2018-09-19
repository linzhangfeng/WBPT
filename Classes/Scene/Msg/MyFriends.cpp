//
//  MyFriends.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "MyFriends.hpp"
#include "ZJH.h"
#include "../Loading/Loading.h"
#include "../../Model/MsgModel.h"

bool MyFriends::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setName("MyFriends");
    
    Size size = Director::getInstance()->getWinSize();
    
//    setBackground(Color4B(255, 255, 255, 255));
//    
//    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
//        this->disappear();
//    });
//    
//    setTitle("我的好友");
//    
//    setMore_txt("添加", [=](){
//        
//        ZJHModel::getInstance()->gotoView(VIEW_ADD_FRIENDS);
//    });
    
    
    this->schedule(schedule_selector(MyFriends::getData), 0.0f);
    
    pageNo = 1;
    curLayout = NULL;
    
    Json::Value json;
    json["pageNo"] = pageNo;
    json["pageSize"] = 50;
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"friend/listFriends",json.toStyledString(),"myFriendList");
    
    loading = Loading::startLoading(this,size / 2);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MyFriends::notificationReflash), "MyFriend_reflash", NULL);
    return true;
}

void MyFriends::notificationReflash(Ref* msg)
{
    if (ZJHModel::getInstance()->isYoukeLogin())
    {
        return;
    }
    
    pageNo = 1;
    curLayout = NULL;
    
    removeChildByName("list");
    
    Json::Value json;
    json["pageNo"] = pageNo;
    json["pageSize"] = 30;
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"friend/listFriends",json.toStyledString(),"myFriendList");
    this->schedule(schedule_selector(MyFriends::getData), 0.0f);
}

void MyFriends::getData(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["myFriendList"];
    if (loginPacket && loginPacket->status != 0)
    {
        if(loading)loading->removeFromParent();
        loading = NULL;
        
        unschedule(schedule_selector(MyFriends::getData));
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        loginPacket->status = 0;
        
        if (loginPacket->resultIsOK())
        {
            friendList = loginPacket->recvVal["resultMap"];
            hasMore = loginPacket->recvVal["resultMap"]["hasMore"].asBool();
            initList();
        }
        else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets["myFriendList"] = NULL;
        delete loginPacket;
    }
}

void MyFriends::initList()
{
    pageNo = 1;
    curLayout = NULL;
    
    removeChildByName("list");
    Size size = Director::getInstance()->getWinSize();
    if (friendList["list"].size() == 0)
    {
        Text* text = Text::create("你还没有好友", "AmericanTypewriter", 35);
        text->setColor(Color3B(150, 150, 150));
        text->setPosition(Vec2(size.width / 2,size.height / 2));
        text->setName("tip");
        addChild(text);
        return;
    }
    
    removeChildByName("tip");
    
    ListView* listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(size.width, size.height - 270 - 160));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 160));
    listView->setScrollBarEnabled(false);
    listView->setName("list");
    addChild(listView);
    
    listView->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
                                    {
                                        if (type == Widget::TouchEventType::ENDED || type == Widget::TouchEventType::CANCELED)
                                        {
                                            if (curLayout)
                                            {
                                                curLayout->setPositionX(0);
                                                curLayout = NULL;
                                            }
                                        }
                                    });
    
    addDataInList();
    
    listView->setItemsMargin(5);
    listView->refreshView();
    
    listView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType)
                               {
                                   if (eventType == ScrollView::EventType::SCROLL_TO_BOTTOM) {
                                       if (hasMore)
                                       {
                                           pageNo ++;
                                           
                                           Json::Value json;
                                           json["pageNo"] = pageNo;
                                           json["pageSize"] = 30;
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
                                                   friendList = loginPacket->recvVal["resultMap"];
                                                   hasMore = loginPacket->recvVal["resultMap"]["hasMore"].asBool();
                                                   addDataInList();
                                               }

                                               delete loginPacket;
                                               
                                           },"friend/listFriends",json.toStyledString(),"myFriendList");
                                           
                                       }
                                   }
                               });
}

void MyFriends::addDataInList()
{
    ListView* listView = (ListView*)getChildByName("list");
    if(!listView)return;
    for (int i = 0; i < friendList["list"].size(); i ++)
    {
        MsgModel::FriendInfo* fi = new MsgModel::FriendInfo;
        fi->mid = friendList["list"][i]["user"]["id"].asInt();
        fi->nickName = friendList["list"][i]["user"]["nickName"].asString();
        fi->photo = friendList["list"][i]["user"]["image"].asString();
        if(!friendList["list"][i]["user"]["remarkName"].isNull())
        {
            fi->rmkName = friendList["list"][i]["user"]["remarkName"].asString();
//            if(fi->rmkName != "")fi->nickName = fi->rmkName;
        }
        fi = MsgModel::getInstance()->addFriend(fi);
        
        MyFriends_Item* item = MyFriends_Item::create();
        item->initData(friendList["list"][i],friendList["list"][i]["user"]["id"].asInt(),[=](int index){
            if(listView){
                listView->removeChildByTag(index);
                listView->refreshView();
                curLayout = NULL;
            }
        });
        
        auto layout = Layout::create();
        layout->setTouchEnabled(true);
        layout->setTag(friendList["list"][i]["user"]["id"].asInt());
        layout->setContentSize(item->getContentSize());
        layout->addChild(item);
        layout->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
                                      {
                                          if (type == Widget::TouchEventType::ENDED || type == Widget::TouchEventType::CANCELED)
                                          {
                                              Layout* n = (Layout*)sender;
                                              
                                              if (curLayout && n != curLayout)
                                              {
                                                  curLayout->setPositionX(0);
                                                  curLayout = NULL;
                                              }
                                              
                                              Vec2 dvec2 = n->getTouchBeganPosition() - n->getTouchEndPosition();
                                              if (dvec2.length() < 30 && type == Widget::TouchEventType::ENDED)
                                              {
                                                  if (curLayout)
                                                  {
                                                      curLayout->setPositionX(0);
                                                      curLayout = NULL;
                                                  }
                                                  
                                                  log("chat to id %d",n->getTag());
                                                  Json::Value json;
                                                  json["uid"] = n->getTag();
                                                  ZJHModel::getInstance()->gotoView(VIEW_CHAT_FRIENDS,json);
                                              }
                                              else
                                              {
                                                  if(dvec2.x > 0)
                                                  {
                                                      if (abs(dvec2.y) > abs(dvec2.x) || curLayout == n)return;
                                                      
                                                      n->setPositionX(-160);
                                                      
                                                      this->curLayout = n;
                                                  }
                                                  else
                                                  {
                                                      if (curLayout)
                                                      {
                                                          curLayout->setPositionX(0);
                                                          curLayout = NULL;
                                                      }
                                                  }
                                              }
                                          }
                                      });
        
        listView->pushBackCustomItem(layout);
    }
}

//------------------------MyFriendItem---------------------------
bool MyFriends_Item::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    return true;
}

/*
 {
 "orderIndex": "w",
 "user": {
 "id": 101365,
 "nickName": "玩呗101365",
 "image": "http://188.188.0.111:20084/chess/chess_img/head/101365/1607011613113289_120x120.jpg",
 "phone": "15309865370",
 "remarkName": "",
 "signature": null,
 "sex": 1,
 "vipLv": 0
 }
 },
 */
void MyFriends_Item::initData(Json::Value &json,int index,const MyCallBack_Int &callback)
{
    Size size = Director::getInstance()->getWinSize();
    
    setContentSize(Size(size.width, 222));
    
    Size itemSize = getContentSize();
    
    LayerColor* bg = LayerColor::create(Color4B(50, 50, 50, 255));
    bg->setContentSize(Size(itemSize.width - 50,2));
    bg->setAnchorPoint(Vec2(0.0f, 0.0f));
    bg->setPosition(50,0);
    addChild(bg, -100);
//    
    int id = json["user"]["id"].asInt();
//    Sprite* avatar = Sprite::create("Avatars/user2.png");
//    avatar->setPosition(100, itemSize.height / 2);
//    addChild(avatar);
    
//    Sprite* userAvatar = Utils::createCircleAvatar(json["user"]["image"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(144,144));
    
    auto userAvatar = Utils::createCircleImage(json["user"]["image"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(144,144));
    userAvatar->setPosition(Vec2(100, itemSize.height / 2));
    this->addChild(userAvatar);
    
    std::string name = (json["user"]["remarkName"].isNull() || json["user"]["remarkName"].asString() == "") ? json["user"]["nickName"].asString() : json["user"]["remarkName"].asString();
    char buf[128];
    Utils::parseName(15, buf, name.c_str());
    Text* text = Text::create(buf, "AmericanTypewriter", 45);
    text->setColor(Color3B(208, 208, 208));
    text->setAnchorPoint(Vec2(0, 0.5));
    text->setPosition(Vec2(200,itemSize.height * 0.5));
    addChild(text,1000);
    
    bool isOnline = json["user"]["isOnline"].asBool();
    
    text = Text::create(isOnline ? "在线" : "离线", "AmericanTypewriter", 45);
    if (isOnline)
    {
        text->setColor(Color3B(75, 127, 56));
    }
    else
    {
        text->setColor(Color3B(112, 112, 112));
    }
    text->setAnchorPoint(Vec2(1, 0.5));
    text->setPosition(Vec2(itemSize.width - 40,itemSize.height * 0.5));
    addChild(text,1000);
    
    {
        LayerColor* del_bg = LayerColor::create(Color4B(178, 0, 14, 255));
        del_bg->setContentSize(Size(162,220));
        
        auto layout = Layout::create();
        layout->setContentSize(del_bg->getContentSize());
        layout->setTouchEnabled(true);
        layout->setSwallowTouches(true);
        layout->setAnchorPoint(Vec2(0.0f, 0.0f));
        layout->setPosition(Vec2(size.width,1));
        layout->addClickEventListener([=](Ref* sender)
                                      {
                                          Layout* n = (Layout*)sender;
                                          Vec2 dvec2 = n->getTouchBeganPosition() - n->getTouchEndPosition();
                                          if (dvec2.length() < 10)
                                          {
                                              Json::Value json_s;
                                              json_s["friendId"] = id;
                                              CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                                                                       {
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
                                                                                               PlatformHelper::showToast("删除成功");
                                                                                               if(callback){
                                                                                                   callback(index);                                                      }
                                                                                           }
                                                                                           else
                                                                                           {
                                                                                               PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                                                                                           }
                                                                                           
                                                                                           CCHttpAgent::getInstance()->packets[tag] = NULL;
                                                                                           delete loginPacket;
                                                                                           
                                                                                       },"friend/deleteFriend",json_s.toStyledString(),"myfriends_removeFriend");
                                          }
                                      });
        addChild(layout);
        
        del_bg->setAnchorPoint(Vec2(0.0f, 0.0f));
        del_bg->setPosition(0,0);
        layout->addChild(del_bg, -100);
        
        Text* text = Text::create("删除", "AmericanTypewriter", 45);
        text->setColor(Color3B(208, 208, 208));
        text->setAnchorPoint(Vec2(0.5, 0.5));
        text->setPosition(del_bg->getContentSize() / 2);
        del_bg->addChild(text,1000);
    }
}