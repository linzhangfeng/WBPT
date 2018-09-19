//
//  AgainstNotify.cpp
//  ZJH
//
//  Created by Jack on 16/9/23.
//
//

#include "AgainstNotify.hpp"
#include "../../Model/MsgModel.h"
#include "../../sqlite3/DataBase.h"
#include "cocostudio/CocoStudio.h"

bool AgainstNotify::init(){
    if (!BaseLayer::init()) {
        return false;
    }
    
    size = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(0x14, 0x14, 0x15, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    
    setName("AgainstNotify");
    
    setTitle("对局通知");
    
    setMore_txt("通知设置", [=](){
        ZJHModel::getInstance()->gotoView(VIEW_MINE_SETTING);
    });
    

    initView();
    return true;
}

void AgainstNotify::initView(){
    listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(size.width, size.height - getHeadH()));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 0));
    listView->setScrollBarEnabled(false);
    listView->setName("list");
    addChild(listView);
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
}

void AgainstNotify::initData(Json::Value &json){
    curPage = 1;
    lastTime = 0;
    newTime = 0;
    hasMore = true;
    pageSize = 10;
    lastId = 0;
    _cuid = json["uid"].asInt();
    getRecordData();
    
}


void AgainstNotify::getRecordData(){
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
        
        AgainstNotifyItem* item = AgainstNotifyItem::create();
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
    CallFunc* func = CallFunc::create([=]
                                      {
                                          listView->jumpToBottom();
                                      });
    listView->refreshView();
    
    runAction(Sequence::create(DelayTime::create(0.0),func, NULL));
    listView->setInnerContainerPosition(p);
    listView->jumpToBottom();
}


bool AgainstNotifyItem::init(){
    if (!Layer::init()) {
        return false;
    }
    
//    setBackground(Color4B(0x14, 0x14, 0x15, 255));

    return true;
}

void AgainstNotifyItem::initView(MsgModel::Chat* chat){
    Layout* layout = Layout::create();
    Size size = Director::getInstance()->getWinSize();
    float width = size.width - 20;
    layout->setSize(Size(width, 632));
    layout->setAnchorPoint(Vec2(0,0));
    layout->setPosition(Point(0,0));Size(width, 632);
    this->setContentSize(Size(width, 632));
    addChild(layout);

    initTimeView(chat->createTime);
    
//    ImageView *contentTopBg = ImageView::create("Msg/mes_room_up.9.png");
//    contentTopBg->setContentSize(Size(size.width-20,120));
//    contentTopBg->setScale9Enabled(true);
//    contentTopBg->setAnchorPoint(Vec2(0,0));
//    contentTopBg->setPosition(Vec2(10,400));
//    addChild(contentTopBg);
    Json::Value jsContent = Utils::ParseJsonStr(chat->content);
    if(jsContent["type"].asInt() == 1){
        Layout* content = Layout::create();
        content->setBackGroundImage("Msg/mes_room_up.9.png");
        content->setContentSize(Size(size.width-20,120));
        content->setAnchorPoint(Vec2(0,0));
        content->setPosition(Vec2(10,400));
        addChild(content);
        
        auto avatar = Utils::createCircleImage(jsContent["gameLogo"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(70,70));
        avatar->setAnchorPoint(Vec2(0,0.5));
        avatar->setPosition(30, content->getContentSize().height/2);
        content->addChild(avatar);
        
        
        char buf[128];
        Utils::parseName(10, buf, jsContent["gameName"].asString().c_str());
        Text* textGameName = Text::create(buf, "AmericanTypewriter", 40);
        textGameName->setColor(Color3B(0xe0,0xcf, 0xae));
        textGameName->setAnchorPoint(Vec2(0, 0.5));
        textGameName->setPosition(Vec2(117, content->getContentSize().height/2));
        content->addChild(textGameName);
        
        Utils::parseName(10, buf, "房间回收通知");
        Text* textRoomName = Text::create(buf, "AmericanTypewriter", 40);
        textRoomName->setColor(Color3B(0xe6,0xe6, 0xe6));
        textRoomName->setAnchorPoint(Vec2(1, 0.5));
        textRoomName->setPosition(Vec2(content->getContentSize().width - 30, content->getContentSize().height/2));
        content->addChild(textRoomName);
        
        Layout* topClearance = Layout::create();
        topClearance->setContentSize(Size(size.width-20,1));
        topClearance->setAnchorPoint(Vec2(0,0));
        topClearance->setPosition(Vec2(10,399));
        LayerColor* topBg = LayerColor::create(Color4B(0x05,0x05,0x05, 255));
        topBg->setContentSize(Size(size.width-20,1));
        topBg->setAnchorPoint(Vec2(0,0));
        topBg->setPosition(Vec2(0,0));
        topClearance->addChild(topBg);
        addChild(topClearance);
        
        Layout* centerContent = Layout::create();
        centerContent->setBackGroundImage("Msg/dongtai_bg_middle.png");
        centerContent->setContentSize(Size(size.width-20,292));
        centerContent->setAnchorPoint(Vec2(0,0));
        centerContent->setPosition(Vec2(10,107));
//        Utils::parseName(30, buf, jsContent["content"].asString().c_str());
        
        auto textContent = LabelTTF::create(jsContent["content"].asString(), "", 40,Size(955, 200), TextHAlignment::LEFT);
        textContent->setColor(Color3B(0xe6,0xe6, 0xe6));
        textContent->setAnchorPoint(Vec2(0.5, 0.5));
        textContent->setPosition(Vec2(centerContent->getContentSize().width /2, centerContent->getContentSize().height  / 2));
        
//        Text* textContent = Text::create(buf, "AmericanTypewriter", 40);
//        textContent->setColor(Color3B(0xe6,0xe6, 0xe6));
//        textContent->setAnchorPoint(Vec2(0.5, 0.5));
//        textContent->setPosition(Vec2(centerContent->getContentSize().width /2, centerContent->getContentSize().height/2));
        centerContent->addChild(textContent);
        addChild(centerContent);
        
      
        Layout* bottom = Layout::create();
        bottom->setBackGroundImage("Msg/mes_room_down.9.png");
        bottom->setContentSize(Size(size.width-20,107));
        bottom->setAnchorPoint(Vec2(0,0));
        bottom->setPosition(Vec2(10,0));

        string roomEnter = "被收回的房间号:"+jsContent["roomName"].asString();
//        Utils::parseName(6, buf, roomEnter.c_str());
        Text* textEnter = Text::create(roomEnter, "AmericanTypewriter", 40);
        textEnter->setColor(Color3B(0xe0,0xcf, 0xae));
        textEnter->setAnchorPoint(Vec2(0.5, 0.5));
        textEnter->setPosition(Vec2(bottom->getContentSize().width / 2, bottom->getContentSize().height/2));
        bottom->addChild(textEnter);
        
        addChild(bottom);
    }else{
        Layout* content = Layout::create();
        content->setBackGroundImage("Msg/mes_room_up.9.png");
        content->setContentSize(Size(size.width-20,120));
        content->setAnchorPoint(Vec2(0,0));
        content->setPosition(Vec2(10,400));
        addChild(content);
        
        auto avatar = Utils::createCircleImage(jsContent["gameLogo"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(70,70));
        avatar->setAnchorPoint(Vec2(0,0.5));
        avatar->setPosition(30, content->getContentSize().height/2);
        content->addChild(avatar);
        
        
        char buf[128];
        Utils::parseName(10, buf, jsContent["gameName"].asString().c_str());
        Text* textGameName = Text::create(buf, "AmericanTypewriter", 40);
        textGameName->setColor(Color3B(0xe0,0xcf, 0xae));
        textGameName->setAnchorPoint(Vec2(0, 0.5));
        textGameName->setPosition(Vec2(117, content->getContentSize().height/2));
        content->addChild(textGameName);
        
        Utils::parseName(10, buf, jsContent["roomName"].asString().c_str());
        Text* textRoomName = Text::create(buf, "AmericanTypewriter", 40);
        textRoomName->setColor(Color3B(0xe6,0xe6, 0xe6));
        textRoomName->setAnchorPoint(Vec2(1, 0.5));
        textRoomName->setPosition(Vec2(content->getContentSize().width - 30, content->getContentSize().height/2));
        content->addChild(textRoomName);
        
        Layout* topClearance = Layout::create();
        topClearance->setContentSize(Size(size.width-20,1));
        topClearance->setAnchorPoint(Vec2(0,0));
        topClearance->setPosition(Vec2(10,399));
        LayerColor* topBg = LayerColor::create(Color4B(0x05,0x05,0x05, 255));
        topBg->setContentSize(Size(size.width-20,1));
        topBg->setAnchorPoint(Vec2(0,0));
        topBg->setPosition(Vec2(0,0));
        topClearance->addChild(topBg);
        addChild(topClearance);
        
        Layout* centerContent = Layout::create();
        centerContent->setBackGroundImage("Msg/dongtai_bg_middle.png");
        centerContent->setContentSize(Size(size.width-20,292));
        centerContent->setAnchorPoint(Vec2(0,0));
        centerContent->setPosition(Vec2(10,107));
        stringstream stream;
        stream<<jsContent["number"].asInt();
        string contentStr = "对局房间已加入"+stream.str()+"个玩家";
        Utils::parseName(30, buf, contentStr.c_str());
        Text* textContent = Text::create(buf, "AmericanTypewriter", 40);
        textContent->setColor(Color3B(0xe6,0xe6, 0xe6));
        textContent->setAnchorPoint(Vec2(0.5, 0.5));
        textContent->setPosition(Vec2(centerContent->getContentSize().width /2, centerContent->getContentSize().height/2));
        centerContent->addChild(textContent);
        addChild(centerContent);
        
        Layout* bottom = Layout::create();
        bottom->setBackGroundImage("Msg/mes_room_down.9.png");
        bottom->setContentSize(Size(size.width-20,107));
        bottom->setAnchorPoint(Vec2(0,0));
        bottom->setPosition(Vec2(10,0));
        bottom->setTouchEnabled(true);
        bottom->addClickEventListener([=](Ref* sender)
                                      {
                                          //进入房间
                                          ZJHModel::getInstance()->gotoRoom(jsContent["roomId"].asInt());
                                      });
        
        string roomEnter = "进入房间";
        Utils::parseName(4, buf, roomEnter.c_str());
        Text* textEnter = Text::create(buf, "AmericanTypewriter", 40);
        textEnter->setColor(Color3B(0xe0,0xcf, 0xae));
        textEnter->setAnchorPoint(Vec2(0, 0.5));
        textEnter->setPosition(Vec2(30, bottom->getContentSize().height/2));
        bottom->addChild(textEnter);
        
        ImageView* image = ImageView::create("Msg/mes_room_icon.png");
        image->setContentSize(Size(18,32));
        image->setAnchorPoint(Vec2(1,0.5));
        image->setPosition(Vec2(bottom->getContentSize().width-30,bottom->getContentSize().height/2));
        bottom->addChild(image);
        addChild(bottom);
    }
}

void AgainstNotifyItem::initTimeView(long long timeLong){
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
    text->setAnchorPoint(Vec2(0.5, 0.5));
    addChild(text,1000);
    
    ui::Scale9Sprite *normalSprite2 = ui::Scale9Sprite::create("Chat/time_window.9.png");
    
    Size time_size = text->getContentSize() + Size(50,10);
    time_size.width = MAX(time_size.width,180);
    time_size.height = MAX(time_size.height,50);
    normalSprite2->setPreferredSize(time_size);
    normalSprite2->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(normalSprite2);
    float width = size.width / 2;
    float height = getContentSize().height - 33 - normalSprite2->getContentSize().height / 2;
    text->setPosition(Vec2(width, height));
    
    normalSprite2->setPosition(width, height);
    
}




