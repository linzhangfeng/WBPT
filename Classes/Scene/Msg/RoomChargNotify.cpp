//
//  RoomChargNotify.cpp
//  ZJH
//
//  Created by mac on 2016/12/14.
//
//

#include "RoomChargNotify.hpp"
#include "../../Model/MsgModel.h"
#include "../../sqlite3/DataBase.h"
#include "cocostudio/CocoStudio.h"

bool RoomChargNotify::init(){
    if (!BaseLayer::init()) {
        return false;
    }
    
    size = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(0x14, 0x14, 0x15, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    
    setName("AgainstNotify");
    
    setTitle("充值记录");
    
    initView();
    return true;
}

void RoomChargNotify::initView(){
    listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(size.width, size.height - getHeadH()));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 0));
    listView->setScrollBarEnabled(false);
    listView->setName("list");
    listView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType)
                               {
                                   if (eventType == ScrollView::EventType::SCROLL_TO_TOP) {
                                       if (hasMore)
                                       {
                                           curPage ++;
                                           getRecordData(false);
                                       }
                                   }
                               });
    addChild(listView);
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
}

void RoomChargNotify::initData(Json::Value &json){
    curPage = 1;
    lastTime = 0;
    newTime = 0;
    hasMore = true;
    pageSize = 30;
    lastId = 0;
    _cuid = json["uid"].asInt();
    getRecordData(true);
    
}


void RoomChargNotify::getRecordData(bool init){
    MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[_cuid];
    std::vector<MsgModel::Chat*> chatList = DataBase::getInstance(ZJHModel::getInstance()->uid)->getChat(_cuid, curPage, pageSize, lastId);
    
    if (fi == NULL)
    {
        log("error fi is NULL id[%d]",_cuid);
        return;
    }
    Vec2 p = listView->getInnerContainerPosition();
    if (chatList.size() < pageSize)hasMore = false;
    for (int i = 0; i < chatList.size() ; i ++)
    {
        MsgModel::Chat* chat = chatList[i];
        if (!chat)continue;
        
        RoomChargNotifyItem* item = RoomChargNotifyItem::create();
        item->initView(chat);
        auto layout = Layout::create();
        layout->setContentSize(item->getContentSize());
        layout->addChild(item,10000);
        listView->insertCustomItem(layout, 0);
        
        fi->chatList.push_back(chat);//for delete this object in the view onExit
        
        if (i == chatList.size() - 1)
        {
            lastId = chat->id;
        }
    }
    
    if (!init)
    {
        listView->refreshView();
        listView->setInnerContainerPosition(p);
    }else{
        CallFunc* func = CallFunc::create([=]
                                          {
                                              listView->jumpToBottom();
                                          });
        listView->refreshView();
        runAction(Sequence::create(DelayTime::create(0.0),func, NULL));
        listView->setInnerContainerPosition(p);
        listView->jumpToBottom();
    }
}


bool RoomChargNotifyItem::init(){
    if (!Layer::init()) {
        return false;
    }
    
    //    setBackground(Color4B(0x14, 0x14, 0x15, 255));
    
    return true;
}

void RoomChargNotifyItem::initView(MsgModel::Chat* chat){
    Layout* layout = Layout::create();
    Size size = Director::getInstance()->getWinSize();
    float width = size.width - 20;
    layout->setSize(Size(width, 500));
    layout->setAnchorPoint(Vec2(0,0));
    layout->setPosition(Point(0,0));
    this->setContentSize(Size(width, 500));
    addChild(layout);
    
        Text* timeView = initTimeView(chat->createTime);
    
        Json::Value jsContent = Utils::ParseJsonStr(chat->content);
        Layout* content = Layout::create();
        content->setBackGroundImage("Msg/mes_room_up.9.png");
        content->setContentSize(Size(size.width-20,120));
        content->setAnchorPoint(Vec2(0,0));
        content->setPosition(Vec2(10,342));
        addChild(content);
    
        Text* textGameName = Text::create("房卡代充", "AmericanTypewriter", 40);
        textGameName->setColor(Color3B(0xe0,0xcf, 0xae));
        textGameName->setAnchorPoint(Vec2(0, 0.5));
        textGameName->setPosition(Vec2(30, content->getContentSize().height/2));
        content->addChild(textGameName);
    
        timeView->setPosition(Vec2(content->getContentSize().width-10, content->getContentSize().height/2));
        content->addChild(timeView);
    
        Layout* centerContent = Layout::create();
        centerContent->setBackGroundImage("Msg/dongtai_bg_middle.png");
        centerContent->setContentSize(Size(size.width-20,292));
        centerContent->setAnchorPoint(Vec2(0,0));
        centerContent->setPosition(Vec2(10,50));
    
        auto textContent = LabelTTF::create(jsContent["content"].asString(), "", 40,Size(955, 200), TextHAlignment::LEFT);
        textContent->setColor(Color3B(0xe6,0xe6, 0xe6));
        textContent->setAnchorPoint(Vec2(0.5, 0.5));
        textContent->setPosition(Vec2(centerContent->getContentSize().width /2, centerContent->getContentSize().height  / 2));
    
        centerContent->addChild(textContent);
        addChild(centerContent);
        
        
        Layout* bottom = Layout::create();
        bottom->setBackGroundImage("Msg/mes_room_down.9.png");
        bottom->setContentSize(Size(size.width-20,50));
        bottom->setAnchorPoint(Vec2(0,0));
        bottom->setPosition(Vec2(10,0));
    
        addChild(bottom);
}

Text* RoomChargNotifyItem::initTimeView(long long timeLong){
    Size size = Director::getInstance()->getWinSize();
    
    time_t t;
    struct tm *p;
    char buf[128];
    
    t = Utils::getCurrentTime() * 0.001;
    p = localtime(&t);
    strftime(buf, sizeof(buf), "%Y-%m-%d", p);
    std::string now_Day = buf;
    
    t = timeLong * 0.001;
    p = localtime(&t);
    strftime(buf, sizeof(buf), "%Y-%m-%d", p);
    std::string to_Day = buf;
    
    if (to_Day == now_Day)
    {
        strftime(buf, sizeof(buf), "%H:%M", p);
    }
    else
    {
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", p);
    }
    
    Text* text = Text::create(buf, "AmericanTypewriter", 40);
    text->setColor(Color3B(190, 190, 190));
    text->setAnchorPoint(Vec2(1, 0.5));
    
    return text;
//    addChild(text,1000);
    
//    ui::Scale9Sprite *normalSprite2 = ui::Scale9Sprite::create("Chat/time_window.9.png");
//    
//    Size time_size = text->getContentSize() + Size(50,10);
//    time_size.width = MAX(time_size.width,180);
//    time_size.height = MAX(time_size.height,50);
//    normalSprite2->setPreferredSize(time_size);
//    normalSprite2->setAnchorPoint(Vec2(0.5,0.5));
//    this->addChild(normalSprite2);
//    float width = size.width / 2;
//    float height = getContentSize().height - 33 - normalSprite2->getContentSize().height / 2;
//    text->setPosition(Vec2(width, height));
//    
//    normalSprite2->setPosition(width, height);
    
}


