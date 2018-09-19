//
//  MatchBottomView.cpp
//  ZJH
//
//  Created by niklaus on 2017/6/23.
//
//

#include "MatchBottomView.hpp"
#include "../../MyGUI/MatchEnterIconByUrl.h"
#include "../../MyGUI/ImageByUrl.h"

bool MatchBottomView::init(){
    if (!Layout::init()) {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    setContentSize(Size(winSize.width, 420));
    
    initUI();
    loadData();
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MatchBottomView::doRefresh), "refresh_room_state", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MatchBottomView::doErrorRefresh), "refresh_room_error", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MatchBottomView::doRefresh), "down_refresh_room", NULL);
    
    return true;
}

void MatchBottomView::loadData(){
    CCHttpAgent::getInstance()->sendHttpPost(NULL, "club/clubRoom/joinRoomList", "", "joinRoomList");
    this->schedule(schedule_selector(MatchBottomView::postData), 0.0f);
}

void MatchBottomView::postData(float dt){
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["joinRoomList"];
    if (packet && packet->status != 0)
    {
        unschedule(schedule_selector(MatchBottomView::postData));
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
            unschedule(schedule_selector(MatchBottomView::postData));
        }
        
        CCHttpAgent::getInstance()->packets["joinRoomList"] = NULL;
        delete packet;
    }
}

void MatchBottomView::doRefresh(Ref* arg){
    //log("======doRefresh======");
    loadData();
}

void MatchBottomView::doErrorRefresh(Ref *arg){
    //log("======doErrorRefresh======");
    __String *str = (__String*)arg;
    string tempStr = str->getCString();
    if (tempStr.compare(mRoomCode) == 0) {
        loadData();
    }
}

void MatchBottomView::updateList(Json::Value json){
    Json::Value roomList = json["page"]["result"];
    Text* headView = (Text*)freeRoomView->getChildByName("headText");
    if (headView) {
        string roomSize = StringUtils::format("免房间号(%d)", json["page"]["totalCount"].asInt());
        headView->setString(roomSize);
    }
    ImageView* roomView = (ImageView*)freeRoomView->getChildByName("roomBg");
    Text* emptyView = (Text*)freeRoomView->getChildByName("emptyText");
    if (roomList.size() > 0) {
        if (roomView) {
            roomView->setVisible(true);
            createItem(json);
        }
        if (emptyView) {
            emptyView->setVisible(false);
        }
        int welcome = UserDefault::getInstance()->getIntegerForKey("jiaocheng_second",0);
        if (welcome != 0) {
            pageView->scrollToPage(1);
        }
    }else{
        if (roomView) {
            roomView->setVisible(false);
            createItem(json);
        }
        if (emptyView) {
            emptyView->setVisible(true);
        }
        pageView->scrollToPage(0);
    }
}

void MatchBottomView::quickAddRoom(cocos2d::Ref *pSender, Widget::TouchEventType type){
    if (type == Widget::TouchEventType::ENDED)
    {
        m_pCallback("openView");
    }
}

void MatchBottomView::moreView(cocos2d::Ref *pSender, Widget::TouchEventType type){
    if (type == Widget::TouchEventType::ENDED)
    {
        m_pCallback("moreView");
    }
}

void MatchBottomView::createItem(Json::Value json){
    int position = 0;
    Json::Value roomList = json["page"]["result"];
    
    ImageView* roomView = (ImageView*)freeRoomView->getChildByName("roomBg");
    if (roomView) {
        mRoomCode = roomList[position]["clubRoomCode"].asString();
        string imageUrl = roomList[position]["gameLog"].asString();
        ImageByUrl* icon_last = (ImageByUrl*)roomView->getChildByName("icon");
        if (icon_last) {
            roomView->removeChildByName("icon");
        }
        ImageByUrl* icon = ImageByUrl::create();
        icon->load(150, 150, imageUrl);
        icon->setName("icon");
        icon->setAnchorPoint(Vec2(0, 0.5));
        icon->setPosition(Vec2(20, roomView->getContentSize().height / 2));
        roomView->addChild(icon);
        
        char buf[128];
        string text_long = roomList[position]["gameName"].asString() + "(" + roomList[position]["clubPlayName"].asString();
        Utils::parseName(8, buf, text_long.c_str());
        string nameStr = StringUtils::format("%s):%s", buf, roomList[position]["clubRoomCode"].asString().c_str());
        Text* name_last = (Text*)roomView->getChildByName("name");
        if (!name_last) {
            Text* name = Text::create(nameStr, "", 40);
            name->setName("name");
            name->setAnchorPoint(Vec2(0, 1));
            name->setPosition(Vec2(190, 180));
            name->setTextColor(Color4B(0xfd, 0xe6, 0x46, 255));
            roomView->addChild(name);
        }else{
            name_last->setString(nameStr);
        }
        
        string numberStr = roomList[position]["roomJuDesc"].asString() + " " + roomList[position]["roomCountDesc"].asString();
        Text* number_last = (Text*)roomView->getChildByName("number");
        if (!number_last) {
            Text* number = Text::create(numberStr, "", 36);
            number->setName("number");
            number->setAnchorPoint(Vec2(0, 1));
            number->setPosition(Vec2(190, 130));
            number->setTextColor(Color4B(0xcc, 0xcc, 0xcc, 255));
            roomView->addChild(number);
        }else{
            number_last->setString(numberStr);
        }
        
        if (roomList[position]["clubId"].asInt() == 0 || roomList[position]["clubId"].isNull()) {
            roomView->removeChildByName("club_name");
        }else{
            char buf2[128];
            string text_2 = roomList[position]["clubName"].asString();
            Utils::parseName(8, buf2, text_2.c_str());
            string club_nameStr = StringUtils::format("[俱乐部]%s", buf2);
            Text* club_name_last = (Text*)roomView->getChildByName("club_name");
            if (!club_name_last) {
                Text* club_name = Text::create(club_nameStr, "", 36);
                club_name->setName("club_name");
                club_name->setAnchorPoint(Vec2(0, 1));
                club_name->setPosition(Vec2(190, 84));
                club_name->setTextColor(Color4B(0xcc, 0xcc, 0xcc, 255));
                roomView->addChild(club_name);
            }else{
                club_name_last->setString(club_nameStr);
            }
        }
        
        if (roomList[position]["clubId"].asInt() == 0 || roomList[position]["clubId"].isNull()) {
            addButton->loadTextures("Match/match_back_room_btn.png","Match/match_back_room_btn_press.png");
        }else{
            addButton->loadTextures("Match/match_add_room_btn.png","Match/match_add_room_btn_press.png");}
        addButton->setName(roomList[position]["clubRoomCode"].asString());
    }
}

void MatchBottomView::initUI(){
    Size winSize = Director::getInstance()->getWinSize();
    
    pageView = PageView::create();
    pageView->setContentSize(Size(winSize.width, 420));
    pageView->setAnchorPoint(Vec2(0, 0));
    pageView->setPosition(Vec2(0,0));
    pageView->setUsingCustomScrollThreshold(true);
    pageView->setCustomScrollThreshold(winSize.width / 5);
    pageView->removeAllPages();
    pageView->setName("GameListView_pageView");
    this->addChild(pageView);
    
    //快速加入房间View
    Layout *addRoomView = Layout::create();
    pageView->addPage(addRoomView);
    
    ImageView* spliteLine = ImageView::create("Match/match_head_line.png");
    spliteLine->setAnchorPoint(Vec2(0.5, 1));
    spliteLine->setPosition(Vec2(winSize.width / 2, 410));
    addRoomView->addChild(spliteLine);
    
    Text* spliteText = Text::create("房间号加入", "", 39);
    spliteText->setAnchorPoint(Vec2(0.5, 1));
    spliteText->setPosition(Vec2(winSize.width / 2, 420));
    spliteText->setTextColor(Color4B(0xee, 0xdc, 0x73, 255));
    addRoomView->addChild(spliteText);
    
    //加入对战btn
    Button* join = Button::create("Match/btn_against_enter.png","Match/btn_against_enter_press.png", "Match/btn_against_enter.png");
    join->addTouchEventListener(CC_CALLBACK_2(MatchBottomView::quickAddRoom, this));
    join->setAnchorPoint(Vec2(0.5, 1));
    join->setPosition(Vec2(winSize.width/2, 280));
    addRoomView->addChild(join);
    
    std::string bottomImg = UserDefault::getInstance()->getStringForKey("bottomImg","");
    if (bottomImg != ""){
        MatchEnterIconByUrl *guoJia = MatchEnterIconByUrl::create();
        guoJia->load(bottomImg, "");
        guoJia->setAnchorPoint(Vec2(0.5, 1));
        guoJia->setPosition(Vec2(winSize.width/2, 140));
        addRoomView->addChild(guoJia);
    }
    
    //免房间号View
    freeRoomView = Layout::create();
    pageView->addPage(freeRoomView);
    
    ImageView* headLine = ImageView::create("Match/match_head_line.png");
    headLine->setAnchorPoint(Vec2(0.5, 1));
    headLine->setPosition(Vec2(winSize.width / 2, 410));
    freeRoomView->addChild(headLine);
    
    Text* headText = Text::create("免房间号()", "", 39);
    headText->setAnchorPoint(Vec2(0.5, 1));
    headText->setPosition(Vec2(winSize.width / 2, 420));
    headText->setTextColor(Color4B(0xee, 0xdc, 0x73, 255));
    headText->setName("headText");
    freeRoomView->addChild(headText);
    
    ImageView* roomBg = ImageView::create("Match/match_room_bg.png");
    roomBg->setScale9Enabled(true);
    roomBg->setContentSize(Size(900, 220));
    roomBg->setAnchorPoint(Vec2(0.5, 0));
    roomBg->setPosition(Vec2(winSize.width / 2, 130));
    roomBg->setName("roomBg");
    roomBg->setVisible(false);
    freeRoomView->addChild(roomBg);
    
    addButton = Button::create("Match/match_add_room_btn.png","Match/match_add_room_btn_press.png", "Match/match_add_room_btn_press.png");
    addButton->setAnchorPoint(Vec2(1, 0.5));
    addButton->setPosition(Vec2(roomBg->getContentSize().width - 25, roomBg->getContentSize().height / 2));
    addButton->addClickEventListener([=](Ref*){
        ZJHModel::getInstance()->gotoRoomWithCode(addButton->getName());
    });
    roomBg->addChild(addButton);
    
    Text* emptyText = Text::create("暂无可加入房间\n加入俱乐部免输房间号游戏", "", 44);
    emptyText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    emptyText->setAnchorPoint(Vec2(0.5, 0.5));
    emptyText->setPosition(Vec2(freeRoomView->getContentSize().width / 2, freeRoomView->getContentSize().height / 2 + 40));
    emptyText->setTextColor(Color4B(0xff, 0xff, 0xff, 255));
    emptyText->setName("emptyText");
    freeRoomView->addChild(emptyText);
    
    ImageView* moreImage = ImageView::create("Match/match_list_pull.png");
    moreImage->setTouchEnabled(true);
    moreImage->setAnchorPoint(Vec2(0.5, 0));
    moreImage->setPosition(Vec2(winSize.width / 2, 20));
    moreImage->addTouchEventListener(CC_CALLBACK_2(MatchBottomView::moreView, this));
    freeRoomView->addChild(moreImage);
}

void MatchBottomView::onExit()
{
    Layout::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}
