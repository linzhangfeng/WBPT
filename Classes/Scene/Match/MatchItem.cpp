//
//  MatchItem.cpp
//  ZJH
//
//  Created by apple on 16/7/14.
//
//

#include "MatchItem.hpp"


#define LEFT_W   30
#define LEFT_W2   150

#pragma mark [ MatchItem ]

MatchItem *MatchItem::createWithData(Json::Value &info)
{
    MatchItem *pRet = new MatchItem();
    if (pRet && pRet->initWithData(info)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
    
}

bool MatchItem::initWithData(Json::Value &info)
{
    if (Widget::init() == false) {
        return false;
    }
    setTouchEnabled(true);
    
    Size winSize = Director::getInstance()->getWinSize();
    Size itemSize = Size(winSize.width, 420);
    this->setAnchorPoint(Vec2(0, 0));
    this->setContentSize(itemSize);

    ImageView *bg1 = ImageView::create("Match/match_item_bgup.png");
    bg1->setAnchorPoint(Vec2(0, 1));
    bg1->setPosition(Vec2(10, itemSize.height));
    bg1->setContentSize(Size(Size(itemSize.width - 20, 120)));
    bg1->setScale9Enabled(true);
    this->addChild(bg1);
    
    //图片
    ImageView *gameLogo = ImageView::create();
    gameLogo->setContentSize(Size(80, 80));
    gameLogo->setAnchorPoint(Vec2(0, 0.5));
    gameLogo->ignoreContentAdaptWithSize(false);
    gameLogo->setPosition(Vec2(30, bg1->getContentSize().height/2));
    gameLogo->setName("gameLogo");
    bg1->addChild(gameLogo);
    
    string imageName = info["gameLogo"].asString();
    ZJHModel::getInstance()->loadingImageResource(gameLogo, imageName);

    //gameName
    Text *gameName = Text::create(info["gameName"].asString(), "", 45);
    gameName->setAnchorPoint(Vec2(0, 0.5));
    gameName->setPosition(Vec2(130, bg1->getContentSize().height/2));
    gameName->setColor(Color3B(0xe2, 0xce, 0xae));
    bg1->addChild(gameName);
    
    Text *create1 = Text::create("创建时间" , "", 40);
    create1->setAnchorPoint(Vec2(1, 1));
    create1->setTextHorizontalAlignment(TextHAlignment::RIGHT);
    create1->setPosition(Vec2(bg1->getContentSize().width - 30, bg1->getContentSize().height - 10));
    create1->setColor(Color3B(0x80, 0x80, 0x80));
    bg1->addChild(create1);
    
    //剩余时间的计算
    struct timeval nowTimeval;
    gettimeofday(&nowTimeval, NULL);
    time_t time_sec ;
    time_sec = nowTimeval.tv_sec;
    time_t time_sec1 = info["createTime"].asDouble()/1000;
    
    struct tm * test = localtime(&time_sec1);
    int year = test->tm_year + 1900;
    int mon = test->tm_mon+1;
    int day = test->tm_mday;
    int hour = test->tm_hour;
    int min = test->tm_min;
    
    struct tm * curTest = localtime(&time_sec);
    int curYear = curTest->tm_year + 1900;
    int curMon = curTest->tm_mon+1;
    int curDay = curTest->tm_mday;
    int curHour = curTest->tm_hour;
    int curMin = curTest->tm_min;
    
    //秒数转分钟
    long int intervalDay = (time_sec - time_sec1)*(1.0/60.0);
    
    __String *bufStr = NULL;
    if (intervalDay >= 0 && intervalDay < 60)
    {
        //60分钟之内
        if (intervalDay < 1)
        {
            intervalDay = 1;
        }
        bufStr = __String::createWithFormat("%ld分钟之前", intervalDay);
    }else if (intervalDay >=60 && intervalDay < 24*60)
    {
        //60分钟--24小时
        if (day == curDay)
        {
            bufStr = __String::createWithFormat("今天%02d:%02d", hour, min);
        }else
        {
            bufStr = __String::createWithFormat("昨天%02d:%02d", hour, min);
        }
        
    }else if (intervalDay >=24*60 && intervalDay < 48*60)
    {
        //24小时－－48小时
        if (day == curDay)
        {
            bufStr = __String::createWithFormat("今天%02d:%02d", hour, min);
        }else
        {
            bufStr = __String::createWithFormat("昨天%02d:%02d", hour, min);
        }
    }else if (intervalDay >= 48*60)
    {
        bufStr = __String::createWithFormat("%04d/%d/%d %02d:%02d", year, mon, day, hour, min);
    }
    
    Text *create2 = Text::create(bufStr->getCString(), "", 40);
    create2->setAnchorPoint(Vec2(1, 0));
    create2->setTextHorizontalAlignment(TextHAlignment::RIGHT);
    create2->setPosition(Vec2(bg1->getContentSize().width - 30, 10));
    create2->setColor(Color3B(0x80, 0x80, 0x80));
    bg1->addChild(create2);
    
    ImageView *bg2 = ImageView::create("Match/match_item_bgdown.png");
    bg2->setAnchorPoint(Vec2(0, 1));
    bg2->setPosition(Vec2(10, itemSize.height - 120));
    bg2->setContentSize(Size(Size(itemSize.width - 20, 300)));
    bg2->setScale9Enabled(true);
    this->addChild(bg2);
    
    Text *roomT1 = Text::create("房间号：" , "", 45);
    roomT1->setAnchorPoint(Vec2(0.5, 1));
    roomT1->setTextHorizontalAlignment(TextHAlignment::CENTER);
    roomT1->setPosition(Vec2(bg2->getContentSize().width/2, bg2->getContentSize().height - 25));
    roomT1->setColor(Color3B(0x80, 0x80, 0x80));
    bg2->addChild(roomT1);
    
    Text *roomT2 = Text::create(info["code"].asString(), "", 80);
    roomT2->setAnchorPoint(Vec2(0.5, 0.5));
    roomT2->setTextHorizontalAlignment(TextHAlignment::CENTER);
    roomT2->setPosition(Vec2(bg2->getContentSize().width/2, bg2->getContentSize().height/2+10));
    roomT2->setColor(Color3B(0xe6, 0xe6, 0xe6));
    bg2->addChild(roomT2);
    
    auto line = Layout::create();
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
    line->setContentSize(Size(bg2->getContentSize().width, 2));
    line->setPosition(Vec2(0, 95));
    bg2->addChild(line);
    
    __String *buf = NULL;
    Text *statesT = Text::create("", "", 40);
    statesT->setAnchorPoint(Vec2(0, 0));
    statesT->setTextHorizontalAlignment(TextHAlignment::LEFT);
    statesT->setPosition(Vec2(30, 25));
    bg2->addChild(statesT);
    
    if (info["status"].asInt() == 0)
    {
        buf = __String::createWithFormat("未开始");
        statesT->setColor(Color3B(0x80, 0x80, 0x80));
        
    }else if(info["status"].asInt() == 1)
    {
        
        buf = __String::createWithFormat("%d个人，正在游戏...", info["onlineCount"].asInt());
        statesT->setColor(Color3B(0x66, 0xb3, 0x38));
    }
    statesT->setString(buf->getCString());
    
    Text *intoT = Text::create("点击进入游戏 > ", "", 40);
    intoT->setAnchorPoint(Vec2(1, 0));
    intoT->setTextHorizontalAlignment(TextHAlignment::RIGHT);
    intoT->setPosition(Vec2(bg2->getContentSize().width - 30, 25));
    intoT->setColor(Color3B(0xe2, 0xce, 0xae));
    bg2->addChild(intoT);
   
    return true;
}

#pragma mark [ recordeItem ]

recordeItem *recordeItem::createWithData(Json::Value &info)
{
    recordeItem *pRet = new recordeItem();
    if (pRet && pRet->initWithData(info))
    {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool recordeItem::initWithData(Json::Value &info)
{
    if (Widget::init() == false) {
        return false;
    }
    
    
    Size winSize = Director::getInstance()->getWinSize();
    Size itemSize = Size(winSize.width, 420);
    this->setAnchorPoint(Vec2(0, 0));
    this->setContentSize(itemSize);
    
    ImageView *bg1 = ImageView::create("Match/match_item_bgup.png");
    bg1->setAnchorPoint(Vec2(0, 1));
    bg1->setPosition(Vec2(10, itemSize.height));
    bg1->setContentSize(Size(Size(itemSize.width - 20, 120)));
    bg1->setScale9Enabled(true);
    this->addChild(bg1);
    
    //图片
    ImageView *gameLogo = ImageView::create();
    gameLogo->setContentSize(Size(80, 80));
    gameLogo->setAnchorPoint(Vec2(0, 0.5));
    gameLogo->ignoreContentAdaptWithSize(false);
    gameLogo->setPosition(Vec2(30, bg1->getContentSize().height/2));
    bg1->addChild(gameLogo);
    
    string imageName = info["gameLogo"].asString();
    ZJHModel::getInstance()->loadingImageResource(gameLogo, imageName);
    
    //gameName
    Text *gameName = Text::create(info["gameName"].asString(), "", 45);
    gameName->setAnchorPoint(Vec2(0, 0.5));
    gameName->setPosition(Vec2(130, bg1->getContentSize().height/2));
    gameName->setColor(Color3B(0xe2, 0xce, 0xae));
    bg1->addChild(gameName);
    
    Text *create1 = Text::create("" , "", 40);
    create1->setAnchorPoint(Vec2(1, 1));
    create1->setTextHorizontalAlignment(TextHAlignment::RIGHT);
    create1->setPosition(Vec2(bg1->getContentSize().width - 30, bg1->getContentSize().height - 10));
    create1->setColor(Color3B(0x80, 0x80, 0x80));
    bg1->addChild(create1);
    
    Text *create2 = Text::create("" , "", 40);
    create2->setAnchorPoint(Vec2(1, 0));
    create2->setTextHorizontalAlignment(TextHAlignment::RIGHT);
    create2->setPosition(Vec2(bg1->getContentSize().width - 30, 10));
    create2->setColor(Color3B(0x80, 0x80, 0x80));
    bg1->addChild(create2);
    
    ImageView *bg2 = ImageView::create("Match/match_item_bgdown.png");
    bg2->setAnchorPoint(Vec2(0, 1));
    bg2->setPosition(Vec2(10, itemSize.height - 120));
    bg2->setContentSize(Size(Size(itemSize.width - 20, 300)));
    bg2->setScale9Enabled(true);
    this->addChild(bg2);
    
    Text *roomT1 = Text::create("房间号：" , "", 45);
    roomT1->setAnchorPoint(Vec2(0.5, 1));
    roomT1->setTextHorizontalAlignment(TextHAlignment::CENTER);
    roomT1->setPosition(Vec2(265, bg2->getContentSize().height - 25));
    roomT1->setColor(Color3B(0x80, 0x80, 0x80));
    bg2->addChild(roomT1);
    
    Text *roomT2 = Text::create(info["code"].asString(), "", 80);
    roomT2->setAnchorPoint(Vec2(0.5, 0.5));
    roomT2->setTextHorizontalAlignment(TextHAlignment::CENTER);
    roomT2->setPosition(Vec2(265, bg2->getContentSize().height/2+10));
    roomT2->setColor(Color3B(0xe6, 0xe6, 0xe6));
    bg2->addChild(roomT2);
    
    Text *winT1 = Text::create("我的战绩：" , "", 45);
    winT1->setAnchorPoint(Vec2(0.5, 1));
    winT1->setTextHorizontalAlignment(TextHAlignment::CENTER);
    winT1->setPosition(Vec2(bg2->getContentSize().width - 265, bg2->getContentSize().height - 25));
    winT1->setColor(Color3B(0x80, 0x80, 0x80));
    bg2->addChild(winT1);
    
    Text *winT2 = Text::create("", "", 80);
    winT2->setAnchorPoint(Vec2(0.5, 0.5));
    winT2->setTextHorizontalAlignment(TextHAlignment::CENTER);
    winT2->setPosition(Vec2(bg2->getContentSize().width - 265, bg2->getContentSize().height/2+10));
    winT2->setColor(Color3B(0xe6, 0xe6, 0xe6));
    bg2->addChild(winT2);
    
    __String *buf = NULL;
    if (info["integral"].asInt() > 0)
    {
        winT2->setColor(Color3B(0x66, 0xb3, 0x38));
        buf = __String::createWithFormat("+%d", info["integral"].asInt());
        
    }else if(info["integral"].asInt() < 0)
    {
        winT2->setColor(Color3B(0xff, 0x00, 0x00));
        buf = __String::createWithFormat("%d", info["integral"].asInt());
        
    }else
    {
        winT2->setColor(Color3B(0xe6, 0xe6, 0xe6));
        buf = __String::createWithFormat("%d", info["integral"].asInt());
        
    }
    winT2->setString(buf->getCString());
    
    auto line = Layout::create();
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
    line->setContentSize(Size(bg2->getContentSize().width, 2));
    line->setPosition(Vec2(0, 95));
    bg2->addChild(line);
    
    Text *statesT = Text::create("", "", 40);
    statesT->setAnchorPoint(Vec2(0, 0));
    statesT->setTextHorizontalAlignment(TextHAlignment::LEFT);
    statesT->setPosition(Vec2(30, 25));
    bg2->addChild(statesT);
    
    if (info["timeWay"].asInt() == 1)
    {
        buf = __String::createWithFormat("%d局", info["count"].asInt());
        statesT->setColor(Color3B(0x80, 0x80, 0x80));
        statesT->setString(buf->getCString());
    }

    Text *intoT = Text::create("点击查看详情 > ", "", 40);
    intoT->setAnchorPoint(Vec2(1, 0));
    intoT->setTextHorizontalAlignment(TextHAlignment::RIGHT);
    intoT->setPosition(Vec2(bg2->getContentSize().width - 30, 25));
    intoT->setColor(Color3B(0xe2, 0xce, 0xae));
    bg2->addChild(intoT);
    

    dealTimeFormate(info["endTime"].asDouble()/1000, create1, create2);
    
    return true;
}

void recordeItem::dealTimeFormate(long long sec, Text *create1, Text *create2)
{
    //房间结束时间计算
    struct timeval nowTimeval;
    gettimeofday(&nowTimeval, NULL);
    time_t time_sec ;
    time_sec = nowTimeval.tv_sec;
    time_t time_sec1 = sec;
    
    struct tm * test = localtime(&time_sec1);
    int year = test->tm_year + 1900;
    int mon = test->tm_mon+1;
    int day = test->tm_mday;
    int hour = test->tm_hour;
    int min = test->tm_min;
    
    struct tm * curTest = localtime(&time_sec);
    int curDay = curTest->tm_mday;

    //秒数转分钟
    long int intervalDay = (time_sec - time_sec1)*(1.0/60.0);
    
    __String *bufStr = NULL;
    if (intervalDay >= 0 && intervalDay < 60*24)
    {
        //24小时之内
        if (day == curDay)
        {
            bufStr = __String::createWithFormat("今天");
        }else
        {
            bufStr = __String::createWithFormat("昨天");
        }
    }else if (intervalDay >= 60*24 && intervalDay < 48*60)
    {
        //24小时－－48小时
        if (day == curDay-1)
        {
            bufStr = __String::createWithFormat("昨天");
        }else
        {
            bufStr = __String::createWithFormat("前天");
        }
    }else if (intervalDay >= 48*60)
    {
        bufStr = __String::createWithFormat("%04d年%d月%d日", year, mon, day);
    }

    create1->setString(bufStr->getCString());
    bufStr = __String::createWithFormat("%02d:%02d", hour, min);
    create2->setString(bufStr->getCString());
}

#pragma mark [ WinRateItem ]

WinRateItem *WinRateItem ::createWithData(Json::Value &info)
{
    WinRateItem *pRet = new WinRateItem();
    if (pRet && pRet->initWithData(info)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool WinRateItem::initWithData(Json::Value &info)
{
    if (Widget::init() == false) {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Size itemSize = Size(visibleSize.width, 180);
    Size imageSize = Size(150, 150);
    this->setAnchorPoint(Vec2(0, 0));
    this->setContentSize(itemSize);
    
    auto line = Layout::create();
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->setBackGroundColor(Color3B(0x30, 0x30, 0x31));
    line->setContentSize(Size(visibleSize.width - 40, 3));
    line->setPosition(Vec2(40, 0));
    this->addChild(line);
    
    //图片
    ImageView *image = ImageView::create();
    image->setContentSize(imageSize);
    image->setAnchorPoint(Vec2(0, 0.5));
    image->ignoreContentAdaptWithSize(false);
    image->setPosition(Vec2(30, itemSize.height/2));
    image->setName("gameLogo");
    this->addChild(image);
    
    string imageName = info["gameUrl"].asString();
    ZJHModel::getInstance()->loadingImageResource(image, imageName);
    
    RichText *gameName = RichText::create();
    gameName->setAnchorPoint(Vec2(0, 1));
    gameName->ignoreContentAdaptWithSize(true);
    gameName->setContentSize(Size(itemSize.width/2, 40));
    
    RichElementText* re1 = RichElementText::create(1, Color3B(0xcc, 0xcc, 0xcc), 255, info["gameTypeName"].asString(), "", 36);
    gameName->pushBackElement(re1);
    gameName->setPosition(Vec2(220, itemSize.height- 30));
    this->addChild(gameName);
    
    __String *bufStr = __String::createWithFormat("%d局", info["total"].asInt());
    
    //总局
    Text *total = Text::create(bufStr->getCString(), "", 32);
    total->setAnchorPoint(Vec2(0, 0.5));
    total->setPosition(Vec2(gameName->getPositionX(), itemSize.height/2));
    total->setColor(Color3B(0x99, 0x99, 0x99));
    this->addChild(total);
    
    
    //win num
    bufStr = __String::createWithFormat("胜%d局", info["winNum"].asInt());
    
    RichText *win = RichText::create();
    win->setAnchorPoint(Vec2(0, 0));
    win->ignoreContentAdaptWithSize(true);
    win->setContentSize(Size(itemSize.width/2, 40));
    
    RichElementText* re3 = RichElementText::create(1, Color3B(0x99, 0x99, 0x99), 255, bufStr->getCString(), "", 32);
    win->pushBackElement(re3);
    win->setPosition(Vec2(gameName->getPositionX(), 30));
    this->addChild(win);
    
    bufStr = __String::createWithFormat("胜率%s", info["winRate"].asString().c_str());
    Text *rate = Text::create(bufStr->getCString(), "", 40);
    rate->setAnchorPoint(Vec2(1, 0.5));
    rate->setTextHorizontalAlignment(TextHAlignment::RIGHT);
    rate->setPosition(Vec2(itemSize.width -  40, itemSize.height/2));
    rate->setColor(Color3B(0x62, 0xb4, 0x37));
    this->addChild(rate);
    
    return true;
}

#pragma mark [ MatchPlayerItem ]

MatchPlayerItem *MatchPlayerItem ::createWithData(Json::Value &info)
{
    MatchPlayerItem *pRet = new MatchPlayerItem();
    if (pRet && pRet->initWithData(info)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool MatchPlayerItem::initWithData(Json::Value &info)
{
    if (Widget::init() == false) {
        return false;
    }
    setTouchEnabled(true);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Size itemSize = Size(visibleSize.width+180, 180);
    
    this->setAnchorPoint(Vec2(0, 0));
    this->setContentSize(itemSize);
    
    auto line = Layout::create();
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->setBackGroundColor(Color3B(0x30, 0x30, 0x31));
    line->setContentSize(Size(visibleSize.width - 40, 3));
    line->setPosition(Vec2(40, 0));
    this->addChild(line);
    
    Sprite *userPortrit = Utils::createCircleAvatar(info["image"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(120,120));
    userPortrit->setPosition(Vec2(40+60, itemSize.height/2));
    this->addChild(userPortrit);
    
    RichText *nick= RichText::create();
    nick->setAnchorPoint(Vec2(0, 0.5));
    nick->ignoreContentAdaptWithSize(true);
    nick->setContentSize(Size(itemSize.width/2, 40));
    
    RichElementText* re1 = RichElementText::create(1, Color3B(0xcc, 0xcc, 0xcc), 255, info["userName"].asString(), "", 36);
    nick->pushBackElement(re1);
    nick->setPosition(Vec2(180, itemSize.height/2 + 50));
    this->addChild(nick);
    
    __String *bufStr = __String::createWithFormat("总胜率 %s", info["winRate"].asString().c_str());
    
    //总胜率
    Text *total = Text::create(bufStr->getCString(), "", 36);
    total->setAnchorPoint(Vec2(0, 0.5));
    total->setPosition(Vec2(nick->getPositionX(), itemSize.height/2 - 20));
    total->setColor(Color3B(0xcc, 0xcc, 0xcc));
    this->addChild(total);
    
    //加好友
    //添加状态
    Text* add = Text::create("", "", 40);
    add->setAnchorPoint(Vec2(0.5, 0.5));
    add->setPosition(Vec2(visibleSize.width - 150, 90));
    this->addChild(add,1000);
    
    auto m = Button::create();
    m->setContentSize(Size(200,200));
    m->ignoreContentAdaptWithSize(false);
    m->setTitleText("加为好友");
    m->setTitleFontSize(45);
    m->setTitleColor(Color3B(208, 208, 208));
    m->setZoomScale(0.1f);
    m->setAnchorPoint(Vec2(0.5, 0.5));
    m->setPosition(Vec2(visibleSize.width - 150, 90));
    m->setPressedActionEnabled(true);
    this->addChild(m,1000);
    m->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
                             {
                                 if (type == Widget::TouchEventType::ENDED)
                                 {
                                     Json::Value json_s;
                                     json_s["friendId"] = info["userId"];
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
                                             PlatformHelper::showToast("添加成功");
                                             __NotificationCenter::getInstance()->postNotification("player_Num_change");
                                             __NotificationCenter::getInstance()->postNotification("player_add_finish", __Integer::create(info["userId"].asInt()));
                                             
                                         }
                                         else
                                         {
                                             PlatformHelper::showToast(loginPacket->recvVal["message"].asString().c_str());
                                         }
                                         
                                         CCHttpAgent::getInstance()->packets[tag] = NULL;
                                         delete loginPacket;
                                         
                                     },"friend/addFriend",json_s.toStyledString(),"addFriend");
                                 }
                             });
    
    
    //delete
    deleteLayout = Layout::create();
    deleteLayout->setTouchEnabled(true);
    deleteLayout->setAnchorPoint(Vec2(1, 0.5));
    deleteLayout->setPosition(Vec2(itemSize.width, itemSize.height/2));
    deleteLayout->setContentSize(Size(180, itemSize.height));
    deleteLayout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    deleteLayout->setBackGroundColor(Color3B(255, 0, 0));
    deleteLayout->ignoreAnchorPointForPosition(false);
    this->addChild(deleteLayout);
    
    Text *deleteText = Text::create("忽略", "", 36);
    deleteText->setPosition(Vec2(90, itemSize.height/2));
    deleteLayout->addChild(deleteText);
    
    return true;
}

Layout *MatchPlayerItem::getDeleteLayout()
{
    return deleteLayout;
}

void MatchPlayerItem::addFriend(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        PlatformHelper::showToast("添加好友");
    }
}

#pragma mark [ RecordDetailItem ]
RecordDetailItem *RecordDetailItem ::createWithData(Json::Value &info, int rate)
{
    RecordDetailItem *pRet = new RecordDetailItem();
    if (pRet && pRet->initWithData(info, rate)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool RecordDetailItem::initWithData(Json::Value &info, int rate)
{
    if (Widget::init() == false) {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Size itemSize = Size(visibleSize.width, 180);
    this->setAnchorPoint(Vec2(0, 0));
    this->setContentSize(itemSize);
    
    auto line1 = Layout::create();
    line1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line1->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4e));
    line1->setPosition(Vec2(40, 0));
    line1->setContentSize(Size(visibleSize.width-40, 3));
    this->addChild(line1);
    
    //图片
    Sprite* portrait = Utils::createCircleAvatar(info["image"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(120, 120));
    portrait->setPosition(Vec2(155+20, itemSize.height/2));
    this->addChild(portrait);
    
    if(rate == 1 && info["integral"].asInt() > 0)
    {
        ImageView *win = ImageView::create("Match/win_first_icon.png");
        win->setPosition(Vec2(portrait->getPositionX(), 30));
        this->addChild(win);
    }else
    {
        if(info["winValue"].asInt() == info["integral"].asInt() && info["winValue"].asInt() > 0)
        {
            ImageView *win = ImageView::create("Match/win_first_icon.png");
            win->setPosition(Vec2(portrait->getPositionX(), 30));
            this->addChild(win);
        }
    }
    
    char buf[200];
    Utils::parseName(12, buf, info["userName"].asString().c_str());
    
    Text *text1 = Text::create(buf, "", 36);
    text1->setAnchorPoint(Vec2(0, 0.5));
    text1->setTextColor(Color4B(0xa6, 0xa6, 0xa6, 255));
    text1->setPosition(Vec2(280, this->getContentSize().height/2 + 25));
    this->addChild(text1);
    
    __String *bufStr = __String::createWithFormat("ID:%d", info["userId"].asInt());

    Text *text2 = Text::create(bufStr->getCString(), "", 36);
    text2->setAnchorPoint(Vec2(0, 0.5));
    text2->setTextColor(Color4B(0xa6, 0xa6, 0xa6, 255));
    text2->setPosition(Vec2(280, this->getContentSize().height/2 - 25));
    this->addChild(text2);
    
    Text *text3 = Text::create("", "", 60);
    text3->setAnchorPoint(Vec2(0.5,0.5));
    text3->setPosition(Vec2(itemSize.width - 250, itemSize.height/2));
    text3->setName("text3");
    this->addChild(text3);
    
    if (info["integral"].asInt() > 0)
    {
        text3->setColor(Color3B(0x35, 0xb1, 0x0e));
        bufStr = __String::createWithFormat("+%d", info["integral"].asInt());
        
    }else if (info["integral"].asInt() == 0)
    {
        text3->setColor(Color3B(0x35, 0xb1, 0x0e));
        bufStr = __String::createWithFormat("%d", info["integral"].asInt());
    }else
    {
        text3->setColor(Color3B(0xc8, 0x01, 0x00));
        bufStr = __String::createWithFormat("%d", info["integral"].asInt());
    }
    text3->setString(bufStr->getCString());
    text3->setPosition(Vec2(itemSize.width - text3->getContentSize().width / 2 - 250, itemSize.height/2));
    
    Text *text4 = Text::create("", "", 60);
    text4->setPosition(Vec2(itemSize.width - 80, itemSize.height/2));
    text4->setName("text4");
    this->addChild(text4);
    
    if (info["reward"].asDouble() > 0)
    {
        text4->setColor(Color3B(0x35, 0xb1, 0x0e));
        bufStr = __String::createWithFormat("+%.02f", info["reward"].asDouble());
    }else if (info["reward"].asInt() == 0)
    {
        text4->setColor(Color3B(0x35, 0xb1, 0x0e));
        bufStr = __String::createWithFormat("+%.02f", info["reward"].asDouble());
    }else
    {
        text4->setColor(Color3B(0xc8, 0x01, 0x00));
        bufStr = __String::createWithFormat("+%.02f", info["reward"].asDouble());
    }
    text4->setString(bufStr->getCString());
    
    bufStr = __String::createWithFormat("%d", rate);
    Text *rateName = Text::create(bufStr->getCString(), "", 36);
    rateName->setAnchorPoint(Vec2(0, 0.5));
    rateName->setColor(Color3B(0xcc, 0xcc, 0xcc));
    rateName->setPosition(Vec2(55, itemSize.height/2));
    rateName->setName("rateName");
    this->addChild(rateName);
    
    return true;
}

#pragma mark [ LuXiangItem ]
LuXiangItem *LuXiangItem ::createWithData(Json::Value &info)
{
    LuXiangItem *pRet = new LuXiangItem();
    if (pRet && pRet->initWithData(info)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool LuXiangItem::initWithData(Json::Value &info)
{
    if (Layout::init() == false) {
        return false;
    }
    
    auto winSize = Director::getInstance()->getVisibleSize();
    Size itemSize = Size(winSize.width, 275);
    
    this->setAnchorPoint(Vec2(0, 0));
    this->setContentSize(itemSize);
    this->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    this->setBackGroundColor(Color3B(0x26, 0x26, 0x29));

    auto line1 = Layout::create();
    line1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line1->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    line1->setContentSize(Size(winSize.width, 3));
    this->addChild(line1);
    
    __String *bufStr = __String::createWithFormat("第%d局", info["index"].asInt());
    
    Text *countText = Text::create(bufStr->getCString(), "", 36);
    countText->setAnchorPoint(Vec2(0, 0.5));
    countText->setColor(Color3B(0xcc, 0xcc, 0xcc));
    countText->setPosition(Vec2(95 , itemSize.height/2));
    this->addChild(countText);
    
    Button* icon2 = Button::create("Match/PIC2.png","Match/PIC2_check.png", "Match/PIC2_check.png");
    icon2->setAnchorPoint(Vec2(1, 0.5));
    icon2->setPosition(Vec2(winSize.width - 40, this->getContentSize().height/2 + 10));
    this->addChild(icon2);
    icon2->addClickEventListener([=](Ref*ref){
     
        postReviewData(info);
        
    });
    
    return true;
}

void LuXiangItem::postReviewData(Json::Value json1)
{
    Json::Value json2;
    json2["roomId"] = json1["roomID"];
    json2["num"] = json1["index"];
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            int index = 0;
            Json::Value data = loginPacket->recvVal["resultMap"]["data"][index];
            //新增code vidioList curIndex count(总局数)
            ZJHModel::getInstance()->roomData["videoData"]["code"] = json1["code"];
            ZJHModel::getInstance()->roomData["videoData"]["videoNumList"] = json1["videoNumList"];
            ZJHModel::getInstance()->roomData["videoData"]["curCount"] = json1["index"];
            ZJHModel::getInstance()->roomData["videoData"]["count"] = json1["count"];
            ZJHModel::getInstance()->roomData["videoData"]["roomID"] = json1["roomID"];
             ZJHModel::getInstance()->roomData["videoData"]["pkgId"] = json1["pkgId"];
             ZJHModel::getInstance()->roomData["videoData"]["gameId"] = json1["gameId"];
            
            ZJHModel::getInstance()->gotoVideo(json1["gameId"].asInt(), json1["roomID"].asInt(), data["content"].asString(), json1["pkgId"].asInt());
            
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            
        }
    }, "room/dz/video", json2.toStyledString(), "videoPost");
}
