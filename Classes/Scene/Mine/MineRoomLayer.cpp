//
//  MIneRoomLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/11.
//
//

#include "MineRoomLayer.hpp"
#include "../Hall/HallRecommend.h"

MineRoomLayer::MineRoomLayer()
:listView1(NULL)
,listView2(NULL)
,listView3(NULL)
{
    log("MatchLayer new");
}
bool  MineRoomLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("我的房间");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    
    setHead(270);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();
    
    currentPage = 0;
    Button* btn1 = Button::create("Mine/roomMsg_n.png","Mine/roomMsg_s.png", "Mine/roomMsg_s.png");
    btn1->setTag(200);
    btn1->setPosition(Vec2(visibleSize.width/2 - btn1->getContentSize().width - 10, visibleSize.height - getHeadH() + 90));
    btn1->addClickEventListener(CC_CALLBACK_1(MineRoomLayer::tabClickFun, this));
    this->addChild(btn1, 200);
    btn1->setEnabled(false);
    
    Button* btn2 = Button::create("Mine/roomCreate_n.png","Mine/roomCreate_s.png","Mine/roomCreate_s.png");
    btn2->setTag(201);
    btn2->setPosition(Vec2(visibleSize.width/2, btn1->getPositionY()));
    btn2->addClickEventListener(CC_CALLBACK_1(MineRoomLayer::tabClickFun, this));
    this->addChild(btn2, 200);
    
    
    Button* btn3 = Button::create("Mine/roomJoin_n.png","Mine/roomJoin_s.png", "Mine/roomJoin_s.png");
    btn3->setTag(202);
    btn3->setPosition(Vec2(visibleSize.width/2 + btn1->getContentSize().width + 10, btn1->getPositionY()));
    btn3->addClickEventListener(CC_CALLBACK_1(MineRoomLayer::tabClickFun, this));
    this->addChild(btn3, 200);
    
    Layout* layout = Layout::create();
    layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    layout->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    layout->setContentSize(Size(visibleSize.width, 80));
    this->addChild(layout);
    layout->setPosition(Vec2(0, H - 80));
    
    //pageView的创建
    Size size = Size(visibleSize.width, visibleSize.height - 370);
    pageView = PageView::create();
    pageView->setTouchEnabled(false);
    pageView->setDirection(PageView::Direction::HORIZONTAL);
    pageView->setContentSize(size);
    pageView->setAnchorPoint(Vec2(0, 0));
    pageView->setPosition(Vec2(0, 0));
    this->addChild(pageView);
    
    for (int i = 0; i < 3; i++)
    {
        Layout* layout = Layout::create();
        layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
        layout->setBackGroundColor(Color3B(0x14, 0x14, 0x15));
        pageView->addPage(layout);
    }
    
    postLove();

    return true;
}

void MineRoomLayer::postLove()
{
    pageIndexLove = 1;
    Json::Value json;
    json["pageIndex"] = pageIndexLove;
    json["pageSize"] = 10;
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
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
                                                                 isFirstLove = true;
                                                                 Layout *page1 = pageView->getPage(0);
                                                                 page1->removeAllChildren();
                                                                 showLove(data);
                                                                 postCreate();
                                                                 postJoin();
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
                                             "game/collectList", json.toStyledString(), "collectList");
}
void MineRoomLayer::postCreate()
{
    pageIndexCreate = 1;
    Json::Value json;
    json["pageIndex"] = pageIndexCreate;
    json["pageSize"] = 10;
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
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
                                                                 isFirstCreate = true;
                                                                 Layout *page2 = pageView->getPage(1);
                                                                 page2->removeAllChildren();
                                                                 showCreate(data);
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
                                             "game/myList", json.toStyledString(), "myList");
}
void MineRoomLayer::postJoin()
{
    pageIndexJoin = 1;
    Json::Value json;
    json["pageIndex"] = pageIndexJoin;
    json["pageSize"] = 10;
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
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
                                                                 isFirstJoin = true;
                                                                 Layout *page3 = pageView->getPage(2);
                                                                 page3->removeAllChildren();
                                                                 showJoin(data);
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
                                             "game/joinList", json.toStyledString(), "joinList");
}

void MineRoomLayer::showLove(Json::Value json)
{
    Layout *page1 = pageView->getPage(0);
    
    Size winSize = Director::getInstance()->getWinSize();
    Json::Value listData = json["myList"]["result"];
    int nums = listData.size();
    if (nums == 0 && isFirstLove == true)
    {
        Text *text = Text::create("暂无关注的房间", "", 36);
        text->setPosition(Vec2(winSize.width/2, page1->getContentSize().height/2));
        text->setTextColor(Color4B::GRAY);
        page1->addChild(text);
        return;
    }
    
    if (listView1 == NULL)
    {
        listView1 = ListView::create();
        listView1->setDirection(ScrollView::Direction::VERTICAL);
        listView1->setBounceEnabled(false);
        listView1->setContentSize(Size(winSize.width, winSize.height - 370 ));
        listView1->setAnchorPoint(Vec2(0, 0));
        listView1->setPosition(Vec2(0, 0));
        listView1->setItemsMargin(50);
        listView1->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
        page1->addChild(listView1);
    }
    
    Size rowSize = Size(winSize.width, 397);
    for (int i = 0; i < nums; i += 2)
    {
        Layout *layout = Layout::create();
        layout->setContentSize(rowSize);
        
        int gameId = listData[i]["id"].asInt();
        HotListItem *item = HotListItem::createWithData(listData[i]);
        item->setPosition(Vec2(rowSize.width * 0.25, rowSize.height/2));
        item->getImageView()->addClickEventListener([gameId](Ref *sender)
                                                    {
                                                        char msg[128];
                                                        snprintf(msg, sizeof(msg), "click item %d", gameId);
                                                        PlatformHelper::showToast(msg);
                                                    });
        layout->addChild(item);
        
        if (i + 1 < nums)
        {
            gameId = listData[i + 1]["id"].asInt();
            item = HotListItem::createWithData(listData[i + 1]);
            item->setPosition(Vec2(rowSize.width * 0.75, rowSize.height / 2));
            item->getImageView()->addClickEventListener([gameId](Ref *sender)
                                                        {
                                                            char msg[128];
                                                            snprintf(msg, sizeof(msg), "click item %d", gameId);
                                                            PlatformHelper::showToast(msg);
                                                        });
            layout->addChild(item);
        }
        
        listView1->addChild(layout);
    }
    
    listView1->refreshView();
    if (json["myList"]["hasMore"].asBool())
    {
        listView1->addEventListener([=](Ref *sendre, ScrollView::EventType eventType){
            if (eventType == ScrollView::EventType::SCROLL_TO_BOTTOM)
            {
                pageIndexLove++;
                PlatformHelper::showToast("到达底部");
                Json::Value json;
                json["pageIndex"] = pageIndexLove;
                json["pageSize"] = 10;
                
                CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                                         {
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
                                                                             isFirstLove = false;
                                                                             showLove(data);
                                                                             
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
                                                         "game/collectList", json.toStyledString(), "collectList");
                
            }
            
        });
    }

}
void MineRoomLayer::showCreate(Json::Value json)
{
    Layout *page2 = pageView->getPage(1);
    
    Size winSize = Director::getInstance()->getWinSize();
    Json::Value listData = json["myList"]["result"];
    int nums = listData.size();
    if (nums == 0 && isFirstCreate == true)
    {
        Text *text = Text::create("暂无创建的房间", "", 36);
        text->setPosition(Vec2(winSize.width/2, page2->getContentSize().height/2));
        text->setTextColor(Color4B::GRAY);
        page2->addChild(text);
        return;
    }
    
    if (listView2 == NULL)
    {
        listView2 = ListView::create();
        listView2->setDirection(ScrollView::Direction::VERTICAL);
        listView2->setBounceEnabled(false);
        listView2->setContentSize(Size(winSize.width, winSize.height - 370));
        listView2->setAnchorPoint(Vec2(0, 0));
        listView2->setPosition(Vec2(0, 0));
        listView2->setItemsMargin(50);
        listView2->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
        page2->addChild(listView2);
    }
    
    Size rowSize = Size(winSize.width, 397);
    for (int i = 0; i < nums; i += 2)
    {
        Layout *layout = Layout::create();
        layout->setContentSize(rowSize);
        
        int gameId = listData[i]["id"].asInt();
        HotListItem *item = HotListItem::createWithData(listData[i]);
        item->setPosition(Vec2(rowSize.width * 0.25, rowSize.height/2));
        item->getImageView()->addClickEventListener([gameId](Ref *sender)
                                                    {
                                                        char msg[128];
                                                        snprintf(msg, sizeof(msg), "click item %d", gameId);
                                                        PlatformHelper::showToast(msg);
                                                    });
        layout->addChild(item);
        
        if (i + 1 < nums)
        {
            gameId = listData[i + 1]["id"].asInt();
            item = HotListItem::createWithData(listData[i + 1]);
            item->setPosition(Vec2(rowSize.width * 0.75, rowSize.height / 2));
            item->getImageView()->addClickEventListener([gameId](Ref *sender)
                                                        {
                                                            char msg[128];
                                                            snprintf(msg, sizeof(msg), "click item %d", gameId);
                                                            PlatformHelper::showToast(msg);
                                                        });
            layout->addChild(item);
        }
        
        listView2->addChild(layout);
    }
    
    listView2->refreshView();
    if (json["myList"]["hasMore"].asBool())
    {
        listView2->addEventListener([=](Ref *sendre, ScrollView::EventType eventType){
            if (eventType == ScrollView::EventType::SCROLL_TO_BOTTOM)
            {
                pageIndexCreate++;
                PlatformHelper::showToast("到达底部");
                Json::Value json;
                json["pageIndex"] = pageIndexCreate;
                json["pageSize"] = 10;
                
                CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                                         {
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
                                                                             isFirstCreate = false;
                                                                             showCreate(data);
                                                                            
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
                                                         "game/myList", json.toStyledString(), "myList");
                
            }
            
        });
    }

}

void MineRoomLayer::showJoin(Json::Value json)
{
    Layout *page3 = pageView->getPage(2);

    Size winSize = Director::getInstance()->getWinSize();
    Json::Value listData = json["myList"]["result"];
    int nums = listData.size();
    
    if (nums == 0 && isFirstJoin == true)
    {
        Text *text = Text::create("暂无参与的房间", "", 36);
        text->setPosition(Vec2(winSize.width/2, page3->getContentSize().height/2));
        text->setTextColor(Color4B::GRAY);
        page3->addChild(text);
        return;
        
    }
    
    if (listView3 == NULL)
    {
        listView3 = ListView::create();
        listView3->setDirection(ScrollView::Direction::VERTICAL);
        listView3->setBounceEnabled(false);
        listView3->setContentSize(Size(winSize.width, winSize.height - 370));
        listView3->setAnchorPoint(Vec2(0, 0));
        listView3->setPosition(Vec2(0, 0));
        listView3->setItemsMargin(50);
        listView3->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
        page3->addChild(listView3);
    }
    
    Size rowSize = Size(winSize.width, 397);
    for (int i = 0; i < nums; i += 2)
    {
        Layout *layout = Layout::create();
        layout->setContentSize(rowSize);
        
        int gameId = listData[i]["id"].asInt();
        HotListItem *item = HotListItem::createWithData(listData[i]);
        item->setPosition(Vec2(rowSize.width * 0.25, rowSize.height/2));
        item->getImageView()->addClickEventListener([gameId](Ref *sender)
                                                    {
                                                        char msg[128];
                                                        snprintf(msg, sizeof(msg), "click item %d", gameId);
                                                        PlatformHelper::showToast(msg);
                                                    });
        layout->addChild(item);
        
        if (i + 1 < nums)
        {
            gameId = listData[i + 1]["id"].asInt();
            item = HotListItem::createWithData(listData[i + 1]);
            item->setPosition(Vec2(rowSize.width * 0.75, rowSize.height / 2));
            item->getImageView()->addClickEventListener([gameId](Ref *sender)
                                                        {
                                                            char msg[128];
                                                            snprintf(msg, sizeof(msg), "click item %d", gameId);
                                                            PlatformHelper::showToast(msg);
                                                        });
            layout->addChild(item);
        }
        
        listView3->addChild(layout);
    }
    
    listView3->refreshView();
    if (json["myList"]["hasMore"].asBool())
    {
        listView3->addEventListener([=](Ref *sendre, ScrollView::EventType eventType){
            if (eventType == ScrollView::EventType::SCROLL_TO_BOTTOM)
            {
                pageIndexJoin++;
                PlatformHelper::showToast("到达底部");
                Json::Value json;
                json["pageIndex"] = pageIndexJoin;
                json["pageSize"] = 10;
                
                CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                                         {
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
                                                                        
                                                                             isFirstJoin = false;
                                                                             showJoin(data);
                                                                             
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
                                                         "game/joinList", json.toStyledString(), "joinList");
                
            }
            
        });
    }
    
}

void MineRoomLayer::tabClickFun(Ref* ref)
{
    Button *tab = (Button*)ref;
    Button *tabLast = (Button*)this->getChildByTag(200+currentPage);
    if (tab==tabLast)
    {
        return;
    }
    tab->setEnabled(false);
    tabLast->setEnabled(true);

    currentPage = tab->getTag()-200;
    pageView->setCurPageIndex(currentPage);
    
    }
