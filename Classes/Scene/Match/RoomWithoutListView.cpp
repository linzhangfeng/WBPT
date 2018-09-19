//
//  RoomWithoutListView.cpp
//  ZJH
//
//  Created by niklaus on 2017/6/24.
//
//

#include "RoomWithoutListView.hpp"
#include "../../MyGUI/ImageByUrl.h"

bool RoomWithoutListView::init(){
    if (!Layout::init()) {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    setContentSize(Size(winSize.width, winSize.height - 335));
    setSwallowTouches(true);
    setTouchEnabled(true);
    setBackGroundImage("Match/Open_BG.png");
    
    initUI();
    loadData();
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomWithoutListView::doRefresh), "refresh_room_state", NULL);
    
    return true;
}

void RoomWithoutListView::loadData(){
    pageIndex = 1;
    Json::Value json;
    json["pageIndex"] = pageIndex;
    json["pageSize"] = pageSize;
    CCHttpAgent::getInstance()->sendHttpPost(NULL, "club/clubRoom/joinRoomList", json.toStyledString(), "joinRoomList2");
    this->schedule(schedule_selector(RoomWithoutListView::postData), 0.0f);
}

void RoomWithoutListView::postData(float dt){
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["joinRoomList2"];
    if (packet && packet->status != 0)
    {
        unschedule(schedule_selector(RoomWithoutListView::postData));
        if (packet->status != 3)
        {
            return;
        }
        packet->status = 0;
        if (packet->resultIsOK())
        {
            Json::Value data = packet->recvVal["resultMap"];
            updateList(data);
        }else
        {
            PlatformHelper::showToast(packet->recvVal["message"].asCString());
            unschedule(schedule_selector(RoomWithoutListView::postData));
        }
        
        CCHttpAgent::getInstance()->packets["joinRoomList2"] = NULL;
        delete packet;
    }
}

void RoomWithoutListView::doRefresh(Ref *arg){
    loadData();
}

void RoomWithoutListView::updateList(Json::Value json){
    hasMore = json["page"]["hasMore"].asBool();
    Json::Value roomList = json["page"]["result"];
    Text* headView = (Text*)this->getChildByName("headText");
    if (headView) {
        string roomSize = StringUtils::format("免房间号(%d)", json["page"]["totalCount"].asInt());
        headView->setString(roomSize);
    }
    mListView->removeAllItems();
    if (roomList.size() > 0) {
        for (int i = 0; i < roomList.size(); i++) {
            Layout* item = createItem(json["page"]["result"][i]);
            mListView->addChild(item);
        }
        mListView->forceDoLayout();
    }
}

void RoomWithoutListView::addMoreList(Json::Value json){
    hasMore = json["page"]["hasMore"].asBool();
    Json::Value roomList = json["page"]["result"];
    if (roomList.size() > 0) {
        for (int i = 0; i < roomList.size(); i++) {
            Layout* item = createItem(json["page"]["result"][i]);
            mListView->addChild(item);
        }
        mListView->forceDoLayout();
    }
}

Layout* RoomWithoutListView::createItem(Json::Value json){
    Layout* bigBg = Layout::create();
    bigBg->setSwallowTouches(true);
    bigBg->setTouchEnabled(true);
    bigBg->setContentSize(Size(960, 240));
    
    ImageView* bg = ImageView::create("Match/match_room_bg.png");
    bg->setAnchorPoint(Vec2(0.5, 1));
    bg->setPosition(Vec2(bigBg->getContentSize().width / 2, bigBg->getContentSize().height));
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(960, 220));
    bigBg->addChild(bg);
    
    string imageUrl = json["gameLog"].asString();
    ImageByUrl* icon = ImageByUrl::create();
    icon->load(150, 150, imageUrl);
    icon->setAnchorPoint(Vec2(0, 0.5));
    icon->setPosition(Vec2(20, bg->getContentSize().height / 2));
    bg->addChild(icon);
    
    char buf[128];
    string text_long = json["gameName"].asString() + "(" + json["clubPlayName"].asString();
    Utils::parseName(9, buf, text_long.c_str());
    string nameStr = StringUtils::format("%s):", buf);
    Text* name = Text::create(nameStr, "", 40);
    name->setAnchorPoint(Vec2(0, 1));
    name->setPosition(Vec2(190, 180));
    name->setTextColor(Color4B(0xfd, 0xe6, 0x46, 255));
    bg->addChild(name);
    
    string codeText = json["clubRoomCode"].asString();
    Text* roomCode = Text::create(codeText, "", 40);
    roomCode->setAnchorPoint(Vec2(0, 1));
    roomCode->setPosition(Vec2(name->getPositionX() + name->getContentSize().width, 180));
    roomCode->setTextColor(Color4B(0xfd, 0x61, 0x08, 255));
    bg->addChild(roomCode);
    
    string numberStr = json["roomJuDesc"].asString() + " " +json["roomCountDesc"].asString();
    Text* number = Text::create(numberStr, "", 36);
    number->setAnchorPoint(Vec2(0, 1));
    number->setPosition(Vec2(190, 130));
    number->setTextColor(Color4B(0xcc, 0xcc, 0xcc, 255));
    bg->addChild(number);
    
    if (!json["clubId"].isNull() || json["clubId"].asInt() != 0) {
        char buf2[128];
        string text_2 = json["clubName"].asString();
        Utils::parseName(11, buf2, text_2.c_str());
        string club_nameStr = StringUtils::format("[俱乐部]%s", buf2);
        Text* club_name = Text::create(club_nameStr, "", 36);
        club_name->setAnchorPoint(Vec2(0, 1));
        club_name->setPosition(Vec2(190, 84));
        club_name->setTextColor(Color4B(0xcc, 0xcc, 0xcc, 255));
        bg->addChild(club_name);
    }
    
    Button* addButton;
    if (json["clubId"].asInt() == 0 || json["clubId"].isNull()) {
        addButton = Button::create("Match/match_back_room_btn.png","Match/match_back_room_btn_press.png", "Match/match_back_room_btn_press.png");
    }else{
        addButton = Button::create("Match/match_add_room_btn.png","Match/match_add_room_btn_press.png", "Match/match_add_room_btn_press.png");
    }
    addButton->setAnchorPoint(Vec2(1, 0.5));
    addButton->setPosition(Vec2(bg->getContentSize().width - 25, bg->getContentSize().height / 2));
    bg->addChild(addButton);
    addButton->addClickEventListener([=](Ref*){
        ZJHModel::getInstance()->gotoRoomWithCode(json["clubRoomCode"].asString());
    });
    
    return bigBg;
}

void RoomWithoutListView::initUI(){
    Size winSize = Director::getInstance()->getWinSize();
    float height = this->getContentSize().height;
    
    ImageView* downImage = ImageView::create("Match/match_list_down.png");
    downImage->setTouchEnabled(true);
    downImage->setAnchorPoint(Vec2(0.5, 1));
    downImage->setPosition(Vec2(winSize.width / 2, height - 75));
    downImage->addClickEventListener([=](Ref*ref){
        CallFunc* func1 = CallFunc::create([=]{
            __NotificationCenter::getInstance()->postNotification("down_refresh_room", __String::create(""));
            this->removeFromParent();
        });
        runAction(Sequence::create(MoveBy::create(0.15, Vec2(0, 160 - winSize.height)),func1, NULL));
    });
    this->addChild(downImage);
    
    ImageView* headLine = ImageView::create("Match/match_head_line.png");
    headLine->setAnchorPoint(Vec2(0.5, 1));
    headLine->setPosition(Vec2(winSize.width / 2, height - 250));
    this->addChild(headLine);
    
    Text* headText = Text::create("免房间号(0)", "", 39);
    headText->setAnchorPoint(Vec2(0.5, 1));
    headText->setPosition(Vec2(winSize.width / 2, height - 230));
    headText->setTextColor(Color4B(0xee, 0xdc, 0x73, 255));
    headText->setName("headText");
    this->addChild(headText);
    
    mListView = ListView::create();
    mListView->setDirection(ScrollView::Direction::VERTICAL);
    mListView->setBounceEnabled(false);
    mListView->setContentSize(Size(960, 1200));
    mListView->setAnchorPoint(Vec2(0.5, 1));
    mListView->setPosition(Vec2(winSize.width / 2, height - 325));
    mListView->setItemsMargin(0);
    mListView->setScrollBarEnabled(false);
    mListView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType){
        if (eventType == ScrollView::EventType::SCROLL_TO_BOTTOM)
        {
            if (!hasMore) {
                return;
            }
            mListView->setTouchEnabled(false);
            pageIndex++;
            Json::Value json;
            json["pageIndex"] = pageIndex;
            json["pageSize"] = pageSize;
            
            CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
                mListView->setTouchEnabled(true);
                CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["joinRoomList3"];
                CCHttpAgent::getInstance()->packets.erase("joinRoomList3");
                if (packet->status != 3)
                {
                    return;
                }
                packet->status = 0;
                if (packet->resultIsOK())
                {
                    Json::Value data = packet->recvVal["resultMap"];
                    addMoreList(data);
                }
                else
                {
                    PlatformHelper::showToast(packet->recvVal["message"].asCString());
                }
                delete packet;
            },"club/clubRoom/joinRoomList", json.toStyledString(), "joinRoomList3");
        }
    });
    this->addChild(mListView);
}

void RoomWithoutListView::onExit()
{
    Layout::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}
