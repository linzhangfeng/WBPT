//
//  BlackFriends.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "BlackFriends.hpp"
#include "../Loading/Loading.h"

bool BlackFriends::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setName("BlackFriends");
    
    Size size = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(20, 20, 22, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    
    setTitle("黑名单");
    curLayout = NULL;
    
    count = 0;
    this->schedule(schedule_selector(BlackFriends::getData), 0.0f);
    pageNo = 1;
    Json::Value json;
    json["pageNo"] = pageNo;
    json["pageSize"] = 20;
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"friend/findBlackUserList",json.toStyledString(),"BlackUserList");
    
    loading = Loading::startLoading(this,size / 2);
    return true;
}

void BlackFriends::getData(float dt){
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["BlackUserList"];
    if (loginPacket && loginPacket->status != 0)
    {
        loading->removeFromParent();
        
        unschedule(schedule_selector(BlackFriends::getData));
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        loginPacket->status = 0;
        
        if (loginPacket->resultIsOK())
        {
            listData = loginPacket->recvVal["resultMap"];
            initList();
        }
        else
        {
            //PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets["BlackUserList"] = NULL;
        delete loginPacket;
    }
}

void BlackFriends::initList()
{
    Size size = Director::getInstance()->getWinSize();
    
    if (listData["list"].size() == 0)
    {
        Text* text = Text::create("没有黑名单好友", "AmericanTypewriter", 45);
        text->setColor(Color3B(150, 150, 150));
        text->setPosition(Vec2(size.width / 2,size.height / 2));
        addChild(text);
        return;
    }
    
    if(listView == NULL){
        listView = ListView::create();
        listView->setDirection(ui::ScrollView::Direction::VERTICAL);
        listView->setBounceEnabled(false);
        listView->setScrollBarEnabled(false);
        listView->setContentSize(Size(size.width, size.height - getHeadH()));
        listView->setAnchorPoint(Vec2(0, 0));
        listView->setPosition(Vec2(0, 0));
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
    }
    
    for (int i = 0; i < listData["list"].size(); i ++)
    {
        BlackFriends_Item* item = BlackFriends_Item::create();
        item->initData(listData["list"][i],count,[=](int index){
            if(listView){
                listView->removeChildByTag(index);
                listView->refreshView();
                curLayout = NULL;
            }
        });
        
        auto layout = Layout::create();
        layout->setTouchEnabled(true);
        layout->setSwallowTouches(true);
        layout->setTag(count);
        layout->setContentSize(item->getContentSize());
        layout->addChild(item);
        listView->pushBackCustomItem(layout);
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
                                              if (curLayout == n)return;
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
                                      });
        count++;
    }
    
    listView->setItemsMargin(5);
    listView->refreshView();
    
    if (listData["hasMore"].asBool()) {
        listView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType)
                                   {
                                       if (eventType == ScrollView::EventType::SCROLL_TO_BOTTOM) {
                                           listView->ScrollView::addEventListener(NULL);
                                           
                                           pageNo = pageNo +1;
                                           
                                           Json::Value json;
                                           json["pageNo"] = pageNo;
                                           json["pageSize"] = 20;
                                           
                                           char buff[64];
                                           snprintf(buff, sizeof(buff), "searchList_%s", "BlackUserList");
                                           loading = Loading::startLoading(this,size / 2);
                                           
                                           CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                                                                    {
                                                                                        if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
                                                                                        
                                                                                        loading->removeFromParent();
                                                                                        
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
                                                                                                        listData = packet->recvVal["resultMap"];
                                                                                                        initList();
                                                                                                    }
                                                                                                    else
                                                                                                    {
                                                                                                        PlatformHelper::showToast(packet->recvVal["message"].asCString());
                                                                                                    }
                                                                                                    
                                                                                                }
                                                                                                //
                                                                                            }
                                                                                            
                                                                                            //删除packet
                                                                                            CCHttpAgent::getInstance()->packets.erase(tag);
                                                                                            delete packet;
                                                                                        }
                                                                                    },
                                                                                    "friend/findBlackUserList", json.toStyledString(), buff);
                                       }
                                   });
    }
}

bool BlackFriends_Item::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    return true;
}


void BlackFriends_Item::initData(Json::Value &json,int index,const MyCallBack_Int &callback)
{
    Size size = Director::getInstance()->getWinSize();
    
    setTag(index);
    
    setContentSize(Size(size.width, 222));
    
    Size itemSize = getContentSize();
    
    LayerColor* bg = LayerColor::create(Color4B(50, 50, 50, 255));
    bg->setContentSize(Size(itemSize.width - 50,2));
    bg->setAnchorPoint(Vec2(0.0f, 0.0f));
    bg->setPosition(50,0);
    addChild(bg, -100);
    
    int id = json["user"]["id"].asInt();
    
    Sprite* userAvatar = Utils::createCircleAvatar(json["user"]["image"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(144,144));
    userAvatar->setPosition(Vec2(100, itemSize.height / 2));
    this->addChild(userAvatar);
    
    std::string name = json["user"]["nickName"].asString();
    char buf[128];
    Utils::parseName(10, buf, name.c_str());
    
    Text* text = Text::create(buf, "AmericanTypewriter", 45);
    text->setColor(Color3B(208, 208, 208));
    text->setAnchorPoint(Vec2(0, 0.5));
    text->setPosition(Vec2(200,itemSize.height * 0.5));
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
                                                                                                 
                                                                                             },"friend/deleteBlackUser",json_s.toStyledString(),"blackFriend_removeFriend");
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
