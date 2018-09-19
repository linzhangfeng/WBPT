////
////  FriendLayer.cpp
////  wanMain
////
////  Created by apple on 16/6/23.
////
////
//
//#include "ClubHome.h"
//#include "JsUtils.h"
//
//ClubHome::ClubHome()
//{
//
//}
//
//bool ClubHome::init()
//{
//    if (!BaseLayer::init())
//    {
//        return false;
//    }
//    
//    setTitle("俱乐部");
//    setHead(130);
//    setMore_txt("更多", [=](){
//        
//        ZJHModel::getInstance()->gotoView(VIEW_MINE_MORE);
//    });
//    
//    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
//    initView();
//    return true;
//}
//
//void ClubHome::initView(){
//    Size winSize = Director::getInstance()->getWinSize();
//    //顶部三个按钮区
//    Layout* topBtnLayout = Layout::create();
//    topBtnLayout->setBackGroundColorType(LAYOUT_COLOR_SOLID);
//    topBtnLayout->setAnchorPoint(Vec2(0, 1));
//    topBtnLayout->setContentSize(Size(winSize.width,245));
//    topBtnLayout->setPosition(Vec2(0,winSize.height-getHeadH()-22));
//    topBtnLayout->setBackGroundColor(Color3B(0x40,0x40,0x40));
//    addChild(topBtnLayout);
//    
//    Button* joinClub = Button::create("Club/join_club_btn.png","Club/join_club_btn.png","Club/join_club_btn.png");
//    joinClub->setAnchorPoint(Vec2(0.5,0.5));
//    joinClub->setPosition(Vec2(211,topBtnLayout->getContentSize().height / 2));
//    joinClub->addClickEventListener(CC_CALLBACK_1(ClubHome::btnCallback, this));
//    joinClub->setName("joinClub");
//    topBtnLayout->addChild(joinClub);
//    
//    Button* createClub = Button::create("Club/create_club_btn.png","Club/create_club_btn.png","Club/create_club_btn.png");
//    createClub->setAnchorPoint(Vec2(0.5,0.5));
//    createClub->setPosition(Vec2(topBtnLayout->getContentSize().width / 2,topBtnLayout->getContentSize().height / 2));
//    createClub->setName("createClub");
//    createClub->addClickEventListener(CC_CALLBACK_1(ClubHome::btnCallback, this));
//    topBtnLayout->addChild(createClub);
//    
//    Button* rank = Button::create("Club/ran_btn.png","Club/ran_btn.png","Club/ran_btn.png");
//    rank->setAnchorPoint(Vec2(0.5,0.5));
//    rank->setPosition(Vec2(topBtnLayout->getContentSize().width - 211,topBtnLayout->getContentSize().height / 2));
//    rank->setName("rank");
//    rank->addClickEventListener(CC_CALLBACK_1(ClubHome::btnCallback, this));
//    topBtnLayout->addChild(rank);
//    
//    Layout* middleLayout = Layout::create();
//    middleLayout->setBackGroundColorType(LAYOUT_COLOR_SOLID);
//    middleLayout->setAnchorPoint(Vec2(0, 1));
//    middleLayout->setContentSize(Size(winSize.width,108));
//    middleLayout->setPosition(Vec2(0,winSize.height-getHeadH()-290));
//    middleLayout->setBackGroundColor(Color3B(0x29,0x29,0x29));
//    addChild(middleLayout);
//
//    tips = Text::create("已加入0个俱乐部", "", 42);
//    tips->setColor(Color3B(0xff,0xff, 0xff));
//    tips->setAnchorPoint(Vec2(0.0, 0.5));
//    tips->setPosition(Vec2(50, middleLayout->getContentSize().height / 2));
//    middleLayout->addChild(tips);
//
//    
//    listView = ListView::create();
//    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
//    listView->setBounceEnabled(false);
//    listView->setContentSize(Size(winSize.width - 40, 1208));
//    listView->setAnchorPoint(Vec2(0, 1));
//    listView->setPosition(Vec2(40, winSize.height - getHeadH() - 388));
//    listView->setScrollBarEnabled(true);
//    listView->setName("list");
//    listView->setBackGroundColor(Color3B(0x2f,0x2f,0x30));
//    listView->setItemsMargin(0);
//    addChild(listView);
//    
//    listView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType)
//                               {
//                                   if (eventType == ScrollView::EventType::SCROLL_TO_TOP) {
//                                       initHttpRequest();
//                                   }
//                               });
//}
//
//void ClubHome::btnCallback(Ref* ref)
//{
//    Node* n = (Node*)ref;
//    string name = n->getName();
//    if(name == "createClub"){
//    }else if(name == "joinClub"){
//    }
//}
//
//void ClubHome::initListView(Json::Value data){
//    listView->removeAllItems();
//    Size winSize = Director::getInstance()->getWinSize();
//    
//    string number = __String::createWithFormat("已加入%d个俱乐部",data.size())->getCString();
//    if(data.size() == 0){
//        number = "暂时没有已经加入的俱乐部";
//    }
//    tips->setString(number);
//    
//    for (int i = 0; i < data.size(); i++) {
//        ClubHomeListItem* item = ClubHomeListItem::create();
//        item->initView(data[i]);
//        listView->addChild(item);
//    }
//    listView->forceDoLayout();
//}
//
//void ClubHome::initHttpRequest(){
//    Json::Value jsonDate;
//    //获取已经加入的俱乐部数据
//    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
//        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
//        
//        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
//        
//        if (loginPacket->status != 3)
//        {
//            PlatformHelper::showToast("获取俱乐部数据失败");
//            return;
//        }
//        
//        if (loginPacket->resultIsOK())
//        {
//            Json::Value data = loginPacket->recvVal["resultMap"]["data"];
//            initListView(data);
//        }
//    },"club/clubMembers/userInClubList",jsonDate.toStyledString(),"userInClubList");
//
//}
//
//void ClubHome::onExit()
//{
//    BaseLayer::onExit();
//    CCHttpAgent::getInstance()->callbacks.erase("userInClubList");
//}
//
//bool ClubHomeListItem::init(){
//    if (!Layout::init()) {
//        return false;
//    }
//    Size winSize = Director::getInstance()->getWinSize();
//    setContentSize(Size(winSize.width - 40,245));
//    setAnchorPoint(Vec2(0,0));
//    setPosition(Vec2(0,0));
//    return true;
//}
//
//void ClubHomeListItem::initView(Json::Value data){
//    
//    Layout* bg = Layout::create();
//    bg->setBackGroundColorType(LAYOUT_COLOR_SOLID);
//    bg->setAnchorPoint(Vec2(0, 0));
//    bg->setContentSize(Size(this->getContentSize().width,this->getContentSize().height));
//    bg->setPosition(Vec2(0,0));
//    bg->setBackGroundColor(Color3B(0x1d,0x1d,0x1e));
//    bg->setTouchEnabled(true);
//    bg->addClickEventListener([=](Ref *ref){
//        Json::Value json;
//        json["id"] = data["id"].asInt();
//    });
//    this->addChild(bg);
//    
//
//    Sprite* clubPortrit = Utils::createCircleAvatar(data["logo"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(140,140));
//    clubPortrit->setAnchorPoint(Vec2(0,0.5));
//    clubPortrit->setPosition(Vec2(0, bg->getContentSize().height/2));
//    bg->addChild(clubPortrit);
//    
//    int level = data["level"].asInt();
//    if(level == 2){
//        ImageView* huangguanTag = ImageView::create("Club/huangguan_tag.png"); //1普通俱乐部   2皇冠俱乐部
//        huangguanTag->setAnchorPoint(Vec2(1,1));
//        huangguanTag->setPosition(Vec2(clubPortrit->getContentSize().width,clubPortrit->getContentSize().height));
//        clubPortrit->addChild(huangguanTag);
//    }
//    Layout* line = Layout::create();
//    line->setBackGroundColorType(LAYOUT_COLOR_SOLID);
//    line->setAnchorPoint(Vec2(0, 0));
//    line->setContentSize(Size(this->getContentSize().width,1));
//    line->setPosition(Vec2(0,0));
//    line->setBackGroundColor(Color3B(0x2f,0x2f,0x30));
//    bg->addChild(line);
//    
//    char buf[500];
//    Utils::parseName(10, buf, data["clubName"].asString().c_str());
//    
//    Text* clubName = Text::create(buf, "", 42);
//    clubName->setColor(Color3B(0xf3,0xf2, 0xcf));
//    clubName->setAnchorPoint(Vec2(0.0, 1));
//    clubName->setPosition(Vec2(180, bg->getContentSize().height - 50));
//    bg->addChild(clubName);
//    
//    Utils::parseName(14, buf, data["notice"].asString().c_str());
//    Text* clubNotice = Text::create(buf, "", 42);
//    clubNotice->setColor(Color3B(0x82,0x82, 0x83));
//    clubNotice->setAnchorPoint(Vec2(0.0, 1));
//    clubNotice->setPosition(Vec2(180, bg->getContentSize().height - 110));
//    bg->addChild(clubNotice);
//
//    Utils::parseName(14, buf, data["playName"].asString().c_str());
//    Text* clubGame = Text::create(buf, "", 42);
//    clubGame->setColor(Color3B(0x82,0x82, 0x83));
//    clubGame->setAnchorPoint(Vec2(0.0, 1));
//    clubGame->setPosition(Vec2(180, bg->getContentSize().height - 162));
//    bg->addChild(clubGame);
//
//    string number = __String::createWithFormat("成员:%d人",data["membersCount"].asInt())->getCString();
//    Text* peopleNum = Text::create(number, "", 42);
//    peopleNum->setColor(Color3B(0x82,0x82, 0x83));
//    peopleNum->setAnchorPoint(Vec2(0.0, 1));
//    peopleNum->setPosition(Vec2(805, bg->getContentSize().height / 2));
//    bg->addChild(peopleNum);
//    
//    
//}
//
//
//
//
