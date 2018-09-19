//
//  RecordDegitalLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/16.
//
//

#include "RecordDegitalLayer.hpp"
#include "MatchItem.hpp"
#include "ShareMatchLayer.hpp"
#include "../Loading/Loading.h"

#define XI_SHU 0.5

void RecordDegitalLayer::showShareWaitView()
{
    auto winSize = Director::getInstance()->getVisibleSize();
    
    Layout *wait = Layout::create();
    wait->setName("wait");
    wait->setTouchEnabled(true);
    wait->setSwallowTouches(true);
    wait->setContentSize(winSize);
    Director::getInstance()->getRunningScene()->addChild(wait, 1000);
    
    ImageView *waitBg = ImageView::create("Public/bar_window_yellow.png");
    waitBg->setScale9Enabled(true);
    waitBg->setContentSize(Size(450, 450));
    waitBg->setPosition(Vec2(winSize.width/2, winSize.height/2));
    wait->addChild(waitBg);

    
    ImageView *loading = ImageView::create("Loading/loading.png");
    loading->setPosition(Vec2(waitBg->getContentSize().width/2, waitBg->getContentSize().height/2 + 100));
    waitBg->addChild(loading);
    
    Text *text = Text::create("正在获取数据...", "", 40);
    text->setPosition(Vec2(waitBg->getContentSize().width/2, waitBg->getContentSize().height/2 - 100));
    text->setColor(Color3B(0xff, 0xff, 0xff));
    waitBg->addChild(text);
}

void RecordDegitalLayer::removeSaveImage(Ref*ref)

{
    if(pathImage != "")
    {
        FileUtils::getInstance()->removeFile(pathImage);
    }
    
}

void RecordDegitalLayer::createNewView()
{
    auto winSize = Director::getInstance()->getVisibleSize();
    
    newList = Layout::create();
    newList->setVisible(false);
    this->addChild(newList);
    
    //layout1
    Layout *layout1 = Layout::create();
    layout1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    layout1->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    layout1->setContentSize(Size(winSize.width*XI_SHU, 150*XI_SHU));
    
    //geme logo
    Size imageSize = Size(110*XI_SHU, 110*XI_SHU);
    ImageView *logo = ImageView::create("Match/icon_wanbei.png");
    logo->setContentSize(imageSize);
    logo->setScale(0.5);
    logo->setAnchorPoint(Vec2(0, 0.5));
    logo->ignoreContentAdaptWithSize(false);
    logo->setPosition(Vec2(40*XI_SHU, layout1->getContentSize().height/2));
    layout1->addChild(logo);
    
    ZJHModel::getInstance()->loadingImageResource(logo, jsonData["gameUrl"].asString());
    
    //房间名字
    char buf1[256];
    char buf2[256];
    Utils::parseName(12, buf1, jsonData["gameTypeName"].asString().c_str());
    Utils::parseName(15, buf2, jsonData["pkgName"].asString().c_str());
    __String *nameStr = __String::createWithFormat("%s(%s)", buf1, buf2);
    
    Text *gameName = Text::create(nameStr->getCString(), "", 32);
    gameName->setAnchorPoint(Vec2(0, 0.5));
    gameName->setScale(0.5);
    gameName->setPosition(Vec2(logo->getPositionX() + imageSize.width*XI_SHU+15*XI_SHU, layout1->getContentSize().height/2));
    gameName->setColor(Color3B(0xcc, 0xcc, 0xcc));
    layout1->addChild(gameName);
    
    newList->addChild(layout1);
    temp_list.push_back(layout1);
    

    //cell2
    Layout *hideCell = Layout::create();
    hideCell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    hideCell->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    hideCell->setContentSize(Size(winSize.width*XI_SHU, 150*XI_SHU));

    //房间ID
    Text *roomT1 = Text::create("房间号：" , "", 36);
    roomT1->setAnchorPoint(Vec2(0.5, 1));
    roomT1->setScale(0.5);
    roomT1->setTextHorizontalAlignment(TextHAlignment::CENTER);
    roomT1->setPosition(Vec2(hideCell->getContentSize().width - 200*XI_SHU, hideCell->getContentSize().height- 20*XI_SHU));
    roomT1->setColor(Color3B(0x80, 0x80, 0x80));
    hideCell->addChild(roomT1);
    
    Text *roomT2 = Text::create(jsonData["code"].asString(), "", 40);
    roomT2->setAnchorPoint(Vec2(0.5, 0));
    roomT2->setScale(0.5);
    roomT2->setTextHorizontalAlignment(TextHAlignment::CENTER);
    roomT2->setPosition(Vec2(hideCell->getContentSize().width - 200*XI_SHU, 20*XI_SHU));
    roomT2->setColor(Color3B(0xe6, 0xe6, 0xe6));
    hideCell->addChild(roomT2);
    
    //
    Utils::parseName(8, buf1, jsonData["userName"].asString().c_str());
    string temp = buf1;
    Text *text1 = Text::create("房主：" + temp, ".SFUIDisplay-Semibold", 32);
    text1->setAnchorPoint(Vec2(0, 1));
    text1->setScale(0.5);
    text1->setPosition(Vec2(40*XI_SHU, hideCell->getContentSize().height - 20*XI_SHU));
    text1->setColor(Color3B(0xa6, 0xa6, 0xa6));
    hideCell->addChild(text1);
    
    
    if(jsonData["timeWay"].asInt() == 0)
    {
        if (jsonData["duration"].asInt()%60 == 0)
        {
            sprintf(buf1, "时长：%d小时", jsonData["duration"].asInt()/60);
        }else
        {
            string str = ZJHModel::getInstance()->converPointTwoBit(jsonData["duration"].asInt()/60.0);
            sprintf(buf1, "时长：%s时", str.c_str());
        }
        
    }else if (jsonData["timeWay"].asInt() == 1)
    {
        sprintf(buf1, "局数：%d局", jsonData["count"].asInt());
    }
    
    
    Text *text3 = Text::create(buf1, ".SFUIDisplay-Semibold", 32);
    text3->setAnchorPoint(Vec2(0, 1));
    text3->setScale(0.5);
    text3->setPosition(Vec2(winSize.width/2*XI_SHU - 50*XI_SHU, text1->getPositionY()));
    text3->setColor(Color3B(0xa6, 0xa6, 0xa6));
    hideCell->addChild(text3);
    
    //剩余时间的计算
    time_t starTime = jsonData["startTime"].asDouble()/1000;
    time_t endTime = jsonData["endTime"].asDouble()/1000;
    
    struct tm * starTm = localtime(&starTime);
    int year = starTm->tm_year + 1900;
    int mon = starTm->tm_mon+1;
    int day = starTm->tm_mday;
    int hour = starTm->tm_hour;
    int min = starTm->tm_min;
    
    struct tm * endTm = localtime(&endTime);
    int yearEnd = endTm->tm_year + 1900;
    int monEnd = endTm->tm_mon+1;
    int dayEnd = endTm->tm_mday;
    int hourEnd = endTm->tm_hour;
    int minEnd = endTm->tm_min;
    
    char star[100];
    
    sprintf(star, "(%04d/%02d/%02d  %02d:%02d -- %04d/%02d/%02d  %02d:%02d)", year, mon, day, hour, min, yearEnd, monEnd, dayEnd, hourEnd, minEnd);
    
    Text *text3_3 = Text::create(star, ".SFUIDisplay-Semibold", 32);
    text3_3->setAnchorPoint(Vec2(0, 0));
    text3_3->setScale(0.5);
    text3_3->setPosition(Vec2(text1->getPositionX(), 20*XI_SHU));
    text3_3->setColor(Color3B(0xa6, 0xa6, 0xa6));
    hideCell->addChild(text3_3);
    
    newList->addChild(hideCell);
    temp_list.push_back(hideCell);
    
    Layout *cell5 = Layout::create();
    cell5->setContentSize(Size(winSize.width*XI_SHU, 90*XI_SHU));
    cell5->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    cell5->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    
    auto line1 = Layout::create();
    line1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line1->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4e));
    line1->setContentSize(Size(winSize.width*XI_SHU, 3*XI_SHU));
    cell5->addChild(line1);
    
    Text *cell5Text = Text::create("详情" , ".SFUIDisplay-Semibold", 36);
    cell5Text->setAnchorPoint(Vec2(0, 0.5));
    cell5Text->setScale(0.5);
    cell5Text->setPosition(Vec2(40*XI_SHU, cell5->getContentSize().height/2));
    cell5Text->setColor(Color3B(0xcc, 0xcc, 0xcc));
    cell5->addChild(cell5Text);
    
    Text *cell5Text2 = Text::create("战绩" , ".SFUIDisplay-Semibold", 36);
    cell5Text2->setScale(0.5);
    cell5Text2->setPosition(Vec2(winSize.width*XI_SHU - 250*XI_SHU, cell5->getContentSize().height/2));
    cell5Text2->setColor(Color3B(0xcc, 0xcc, 0xcc));
    cell5->addChild(cell5Text2, 10);
    
    Text *cell5Text3 = Text::create("喜钱" , ".SFUIDisplay-Semibold", 36);
    cell5Text3->setScale(0.5);
    cell5Text3->setPosition(Vec2(winSize.width*XI_SHU - 80*XI_SHU, cell5->getContentSize().height/2));
    cell5Text3->setColor(Color3B(0xcc, 0xcc, 0xcc));
    cell5->addChild(cell5Text3, 10);
    
    newList->addChild(cell5);
    
    temp_list.push_back(cell5);
    
    Json::Value list = jsonData["playRank"];
    for (int i = 0; i < list.size(); i++)
    {
        auto cellItem = Layout::create();
        cellItem->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        cellItem->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
        cellItem->setContentSize(Size(winSize.width*XI_SHU, 180*XI_SHU));
        
        newList->addChild(cellItem);

        RecordDetailItem *item = RecordDetailItem::createWithData(list[i], i+1);
        item->setScale(0.5);
        cellItem ->addChild(item);
        
        temp_list.push_back(cellItem);
        
        bool hasXiqian = list[i]["withReward"].asBool();
        cell5Text3->setVisible(hasXiqian);
        item->getChildByName("text4")->setVisible(hasXiqian);
        if(!hasXiqian){
            cell5Text2->setPosition(Vec2(winSize.width*XI_SHU - 80*XI_SHU, cell5->getContentSize().height/2));
            item->getChildByName("text3")->setPosition(Vec2(item->getContentSize().width - 80,item->getContentSize().height / 2));
        }
    }
    
    float screenH = 0;
    for (int i = 0; i < temp_list.size(); i++)
    {
        Widget*item = temp_list.at(i);
        screenH += item->getContentSize().height;
    }
    
    newList->setContentSize(Size(winSize.width*0.5, screenH));
    
    for (int i = 0; i < temp_list.size(); i++)
    {
        Widget* item = temp_list.at(i);
        item->setPosition(Vec2(0, screenH - item->getContentSize().height));
        screenH -= item->getContentSize().height;
    }
}

bool RecordDegitalLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RecordDegitalLayer::removeSaveImage), "removeShareImage", NULL);
    
    isShow = true;
    setTitle("战绩详情");
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    auto winSize = Director::getInstance()->getVisibleSize();
    
    Layout *createBg = Layout::create();
    createBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    createBg->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    createBg->setContentSize(Size(winSize.width, 210));
    this->addChild(createBg, 2);
    
    auto line = Layout::create();
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->ignoreAnchorPointForPosition(false);
    line->setAnchorPoint(Vec2(0, 1));
    line->setBackGroundColor(Color3B(0, 0, 0));
    line->setPosition(Vec2(0, 210));
    line->setContentSize(Size(winSize.width, 3));
    createBg->addChild(line);
    
    shareBtn = Button::create("Public/btn_yellow.png","Public/btn_yellow.png");
    shareBtn->setScale9Enabled(true);
    shareBtn->setContentSize(Size(960, 115));
    shareBtn->setPosition(Vec2(winSize.width/2, 105));
    createBg->addChild(shareBtn, 2);
    shareBtn->setTitleText("分享");
    shareBtn->setTitleFontSize(40);
    shareBtn->setTitleColor(Color3B(0x65, 0x41, 0x00));
    shareBtn->setTitleFontName(".SFUIDisplay-Semibold");
    shareBtn->addClickEventListener(CC_CALLBACK_1(RecordDegitalLayer::shareBtnOn, this));
    shareBtn->setVisible(false);
    return true;
}

void RecordDegitalLayer::shareBtnOn(Ref*ref)
{
    //分享延时等待对象
    //showShareWaitView();
    
    shareBtn->setEnabled(false);
    
    newList->setVisible(true);
    RenderTexture* renderTexture = RenderTexture::create(newList->getContentSize().width, newList->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
    renderTexture->begin();
    
    for (int i = 0; i < temp_list.size(); i++)
    {
        Widget* item = temp_list.at(i);
        item->cocos2d::Node::visit();
    }
    
    renderTexture->end();
    newList->setVisible(false);
    
    renderTexture->saveToFile("record.jpg", Image::Format::JPG, true, [=](RenderTexture* o,std::string path){
//        Layout *wait = (Layout*)Director::getInstance()->getRunningScene()->getChildByName("wait");
//        wait->removeFromParent();
        
        pathImage = path;
        log("%s",path.c_str());
        ShareMatchLayer *share = ShareMatchLayer::create();
        share->setPosition(Vec2::ZERO);
        Director::getInstance()->getRunningScene()->addChild(share, 2000);
        share->setPath(path, jsonData);
        
        shareBtn->setEnabled(true);
        
    });
}

void RecordDegitalLayer::show(Json::Value jsonTemp)
{
    auto winSize = Director::getInstance()->getVisibleSize();
    roomID = jsonTemp["roomID"].asInt();
    gameType = jsonTemp["gameId"].asInt();
    
    Json::Value json;
    json["roomId"] = roomID;
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"duiju/integral/detail", json.toStyledString(), "detail");
    auto loading = Loading::startLoading(this, winSize / 2);
    loading->setName("loading");
    schedule(schedule_selector(RecordDegitalLayer::getData), 0.0f);
}

void RecordDegitalLayer::getData(float dt)
{
    CCHttpPacket *packet = CCHttpAgent::getInstance()->packets["detail"];
    if (packet && packet->status != 0)
    {
        removeChildByName("loading");
        if (packet->status != 3)
        {
            auto winSize = Director::getInstance()->getVisibleSize();
            auto shareBtn = Button::create("Public/btn_yellow.png","Public/btn_yellow.png");
            shareBtn->setScale9Enabled(true);
            shareBtn->setContentSize(Size(300, 100));
            shareBtn->setPosition(Vec2(winSize.width/2, 105));
            addChild(shareBtn, 2);
            shareBtn->setTitleText("刷新");
            shareBtn->setTitleFontSize(40);
            shareBtn->setTitleColor(Color3B(0x65, 0x41, 0x00));
            shareBtn->addClickEventListener([=](Ref* r)
            {
                Json::Value json;
                json["roomId"] = roomID;
                CCHttpAgent::getInstance()->sendHttpPost(NULL,"duiju/integral/detail", json.toStyledString(), "detail");
                auto loading = Loading::startLoading(this, winSize / 2);
                loading->setName("loading");
                schedule(schedule_selector(RecordDegitalLayer::getData), 1.0f);
                shareBtn->removeFromParentAndCleanup(true);

            });
        }
        else
        {
            packet->status = 0;
            if (packet->resultIsOK())
            {
                Json::Value data = packet->recvVal["resultMap"];
                showView(data);
            }
            else
            {
                PlatformHelper::showToast(packet->recvVal["message"].asCString());
            }
        }
        
        //删除packet
        CCHttpAgent::getInstance()->packets.erase("detail");
        delete packet;
    }
}

void RecordDegitalLayer::showView(Json::Value json)
{
    jsonData = json;
    
    jsonData["roomID"] = roomID;
    jsonData["gameId"] = gameType;
    
    shareBtn->setVisible(true);
    
    auto winSize = Director::getInstance()->getVisibleSize();
    float H = winSize.height - getHeadH() - 210;
    
    viewList = ListView::create();
    viewList->ignoreAnchorPointForPosition(false);
    viewList->setDirection(ScrollView::Direction::VERTICAL);
    viewList->setBounceEnabled(false);
    viewList->setContentSize(Size(winSize.width, H));
    viewList->setAnchorPoint(Vec2(0, 0));
    viewList->setPosition(Vec2(0, 210));
    viewList->setItemsMargin(0);
    viewList->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(viewList, 5);
    
    
    //layout1
    Layout *layout1 = Layout::create();
    layout1->setTouchEnabled(true);
    layout1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    layout1->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    layout1->setContentSize(Size(winSize.width, 150));
    layout1->addTouchEventListener(CC_CALLBACK_2(RecordDegitalLayer::hideOn, this));
    
    //geme logo
    Size imageSize = Size(110, 110);
    ImageView *logo = ImageView::create("Match/icon_wanbei.png");
    logo->setContentSize(imageSize);
    logo->setAnchorPoint(Vec2(0, 0.5));
    logo->ignoreContentAdaptWithSize(false);
    logo->setPosition(Vec2(40, layout1->getContentSize().height/2));
    layout1->addChild(logo);
    
    ZJHModel::getInstance()->loadingImageResource(logo, json["gameUrl"].asString());
    
    char buf1[256];
    char buf2[256];
    Utils::parseName(12, buf1, json["gameTypeName"].asString().c_str());
    Utils::parseName(15, buf2, json["pkgName"].asString().c_str());
    
    __String *nameStr = __String::createWithFormat("%s(%s)", buf1, buf2);
    Text *gameName = Text::create(nameStr->getCString(), ".SFUIDisplay-Semibold", 36);
    gameName->setAnchorPoint(Vec2(0, 0.5));
    gameName->setPosition(Vec2(logo->getPositionX() + imageSize.width+15, layout1->getContentSize().height/2));
    gameName->setColor(Color3B(0xcc, 0xcc, 0xcc));
    layout1->addChild(gameName);

    viewList->addChild(layout1);

    if (isShow)
    {
        //隐藏显示的cell
        showHidenView(json);
    }else
    {
        if(layout2Cell != NULL)
        {
            layout2Cell->removeFromParent();
        }
        
    }
    showDownView(json);
}

void RecordDegitalLayer::showHidenView(Json::Value json)
{
    auto winSize = Director::getInstance()->getVisibleSize();
    
    //layout1
    layout2Cell = Layout::create();
    layout2Cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    layout2Cell->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    layout2Cell->setContentSize(Size(winSize.width, 150));
    layout2Cell->setTouchEnabled(true);
    layout2Cell->addTouchEventListener(CC_CALLBACK_2(RecordDegitalLayer::hideOn, this));
    //
    char buf[256];
    Utils::parseName(8, buf, json["userName"].asString().c_str());
    string temp = buf;
    Text *text1 = Text::create("房主：" + temp, ".SFUIDisplay-Semibold", 32);
    text1->setAnchorPoint(Vec2(0, 1));
    text1->setPosition(Vec2(40, layout2Cell->getContentSize().height - 20));
    text1->setColor(Color3B(0xa6, 0xa6, 0xa6));
    layout2Cell->addChild(text1);


    if(json["timeWay"].asInt() == 0)
    {
        if (json["duration"].asInt()%60 == 0)
        {
            sprintf(buf, "时长：%d小时", json["duration"].asInt()/60);
        }else
        {
            string str = ZJHModel::getInstance()->converPointTwoBit(json["duration"].asInt()/60.0);
            sprintf(buf, "时长：%s时", str.c_str());
        }
        
    }else if (json["timeWay"].asInt() == 1)
    {
        sprintf(buf, "局数：%d局", json["count"].asInt());
    }
    
    Text *text3 = Text::create(buf, ".SFUIDisplay-Semibold", 32);
    text3->setAnchorPoint(Vec2(0, 1));
    text3->setPosition(Vec2(winSize.width/2 - 50, text1->getPositionY()));
    text3->setColor(Color3B(0xa6, 0xa6, 0xa6));
    layout2Cell->addChild(text3);
    
    
    //剩余时间的计算
    time_t starTime = json["startTime"].asDouble()/1000;
    time_t endTime = json["endTime"].asDouble()/1000;
    
    struct tm * starTm = localtime(&starTime);
    int year = starTm->tm_year + 1900;
    int mon = starTm->tm_mon+1;
    int day = starTm->tm_mday;
    int hour = starTm->tm_hour;
    int min = starTm->tm_min;
    
    struct tm * endTm = localtime(&endTime);
    int yearEnd = endTm->tm_year + 1900;
    int monEnd = endTm->tm_mon+1;
    int dayEnd = endTm->tm_mday;
    int hourEnd = endTm->tm_hour;
    int minEnd = endTm->tm_min;
    
    char star[100];
    
    sprintf(star, "(%04d/%02d/%02d  %02d:%02d -- %04d/%02d/%02d  %02d:%02d)", year, mon, day, hour, min, yearEnd, monEnd, dayEnd, hourEnd, minEnd);

    Text *text3_3 = Text::create(star, ".SFUIDisplay-Semibold", 32);
    text3_3->setAnchorPoint(Vec2(0, 0));
    text3_3->setPosition(Vec2(text1->getPositionX(), 20));
    text3_3->setColor(Color3B(0xa6, 0xa6, 0xa6));
    layout2Cell->addChild(text3_3);
    
    //房间ID
    Text *roomT1 = Text::create("房间号：" , "", 36);
    roomT1->setAnchorPoint(Vec2(0.5, 1));
    roomT1->setTextHorizontalAlignment(TextHAlignment::CENTER);
    roomT1->setPosition(Vec2(layout2Cell->getContentSize().width - 200, layout2Cell->getContentSize().height - 20));
    roomT1->setColor(Color3B(0x80, 0x80, 0x80));
    layout2Cell->addChild(roomT1);
    
    Text *roomT2 = Text::create(jsonData["code"].asString(), "", 40);
    roomT2->setAnchorPoint(Vec2(0.5, 0));
    roomT2->setTextHorizontalAlignment(TextHAlignment::CENTER);
    roomT2->setPosition(Vec2(layout2Cell->getContentSize().width - 200, 20));
    roomT2->setColor(Color3B(0xe6, 0xe6, 0xe6));
    layout2Cell->addChild(roomT2);

    viewList->addChild(layout2Cell);
}

void RecordDegitalLayer::showDownView(Json::Value json)
{
    auto winSize = Director::getInstance()->getVisibleSize();
    
    //点击隐藏的cell
    Layout *cell1 = Layout::create();
    cell1->setTouchEnabled(true);
    cell1->addTouchEventListener(CC_CALLBACK_2(RecordDegitalLayer::hideOn, this));
    cell1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    cell1->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    cell1->setContentSize(Size(winSize.width, 25));
    viewList->addChild(cell1);
    hideCell = cell1;
    
    ImageView *hideMark = ImageView::create("Match/more_icon_down.png");
    hideMark->setTag(100);
    hideMark->setPosition(Vec2(winSize.width/2, cell1->getContentSize().height/2));
    cell1->addChild(hideMark);
    hideMark->setFlippedY(false);
    
    Layout *bgCell1 = Layout::create();
    bgCell1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    bgCell1->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    bgCell1->setContentSize(Size(winSize.width, 25));
    viewList->addChild(bgCell1);
    
    /********************回顾 栏的显示状态 start ****************************/
    //
    Layout *cell2 = Layout::create();
    cell2->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    cell2->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    cell2->setContentSize(Size(winSize.width, 140));
    viewList->addChild(cell2);
    
    Text *cell2Text = Text::create("录像回放" , ".SFUIDisplay-Semibold", 36);
    cell2Text->setAnchorPoint(Vec2(0, 0.5));
    cell2Text->setPosition(Vec2(40, cell2->getContentSize().height/2));
    cell2Text->setColor(Color3B(0xcc, 0xcc, 0xcc));
    cell2->addChild(cell2Text);
    if (json["videoNum"].asInt() > 0)
    {
        cell2->setTouchEnabled(true);
        cell2->addTouchEventListener(CC_CALLBACK_2(RecordDegitalLayer::reviewOn, this));
        
        ImageView *reviewIcon = ImageView::create("Match/luxiang_icon.png");
        reviewIcon->setAnchorPoint(Vec2(1, 0.5));
        reviewIcon->setPosition(Vec2(winSize.width - 80, cell2->getContentSize().height/2));
        cell2->addChild(reviewIcon);
        
        ImageView *rightMark = ImageView::create("Public/detail_arrow.png");
        rightMark->setPosition(Vec2(winSize.width - 40, cell2->getContentSize().height/2));
        cell2->addChild(rightMark);
        setMore_txt("分享录像", [=](){
            shareVideo();
        });
    }else if(json["videoNum"].asInt() == 0)
    {
        Text *right = Text::create("暂无录像", ".SFUIDisplay-Semibold", 40);
        right->setAnchorPoint(Vec2(1, 0.5));
        right->setTextColor(Color4B(0x5a, 0x5a, 0x5a, 255));
        right->setTextHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
        right->setPosition(Vec2(winSize.width - 80, cell2->getContentSize().height/2));
        cell2->addChild(right);
    }

    /********************回顾 栏的显示状态 end ****************************/
    
    Layout *bgCell2 = Layout::create();
    bgCell2->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    bgCell2->setBackGroundColor(Color3B(0x14, 0x14, 0x15));
    bgCell2->setContentSize(Size(winSize.width, 25));
    viewList->addChild(bgCell2);

    //总带入
    Layout *cell3 = Layout::create();
    cell3->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    cell3->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    cell3->setContentSize(Size(winSize.width, 420));
    
    
    Text *cell3Text = Text::create("总带入" , ".SFUIDisplay-Semibold", 36);
    cell3Text->setAnchorPoint(Vec2(0, 0.5));
    cell3Text->setPosition(Vec2(40, cell3->getContentSize().height - 80));
    cell3Text->setColor(Color3B(0xcc, 0xcc, 0xcc));
    cell3->addChild(cell3Text);
    
    char buf[200];
    sprintf(buf, "%d", json["totalTakeIn"].asInt());
    Text *cell3Text2 = Text::create(buf , ".SFUIDisplay-Semibold", 36);
    cell3Text2->setAnchorPoint(Vec2(1, 0.5));
    cell3Text2->setPosition(Vec2(winSize.width - 40, cell3->getContentSize().height - 80));
    cell3Text2->setColor(Color3B(0xfd, 0x61, 0x00));
    cell3->addChild(cell3Text2);

    for (int i = 0; i < 3; i++)
    {
        string imageName;
        Text *name;
        int k;
        
        Sprite *portrait;
        ImageView *rankMark;
        switch (i) {
            case 0:
                //2
                if (json["playRank"].size()>=2)
                {
                    k = 1;
                    Utils::parseName(10, buf, json["playRank"][k]["userName"].asString().c_str());
                    name = Text::create(buf, ".SFUIDisplay-Semibold", 36);
                    name->setAnchorPoint(Vec2(0.5, 0));
                    name->setPosition(Vec2(240, 40));
                    name->setColor(Color3B(0xcc, 0xcc, 0xcc));
                    cell3->addChild(name);
                    
                    portrait = Utils::createCircleAvatar(json["playRank"][k]["image"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(120, 120));
                }else
                {
                    portrait = Sprite::create("Match/icon_wanbei.png");
                    portrait->setScale(0.8);
                }
                rankMark = ImageView::create("Match/user_no2.png");
                rankMark->setPosition(Vec2(240, 105));
                portrait->setPosition(Vec2(rankMark->getContentSize().width/2, rankMark->getContentSize().height/2+10));
                
                break;
            case 1:
                //1
                if (json["playRank"].size()>=1)
                {
                    k = 0;
                    Utils::parseName(10, buf, json["playRank"][k]["userName"].asString().c_str());
                    name = Text::create(buf, ".SFUIDisplay-Semibold", 36);
                    
                    name->setAnchorPoint(Vec2(0.5, 0));
                    name->setPosition(Vec2(winSize.width/2, 40));
                    name->setColor(Color3B(0xcc, 0xcc, 0xcc));
                    cell3->addChild(name);
                    
                    portrait = Utils::createCircleAvatar(json["playRank"][k]["image"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(147, 147));
                }else
                {
                    portrait = Sprite::create("Match/icon_wanbei.png");
                    portrait->setScale(0.8);
                }
                rankMark = ImageView::create("Match/user_no1.png");
                rankMark->setPosition(Vec2(winSize.width/2, 105));
                portrait->setPosition(Vec2(rankMark->getContentSize().width/2, rankMark->getContentSize().height/2+10));
                
                break;
            case 2:
                //3
                if (json["playRank"].size()>=3)
                {
                    k = 2;
                    Utils::parseName(10, buf, json["playRank"][k]["userName"].asString().c_str());
                    name = Text::create(buf, ".SFUIDisplay-Semibold", 36);
                    
                    name->setAnchorPoint(Vec2(0.5, 0));
                    name->setPosition(Vec2(winSize.width-240, 40));
                    name->setColor(Color3B(0xcc, 0xcc, 0xcc));
                    cell3->addChild(name);
                    
                    portrait = Utils::createCircleAvatar(json["playRank"][k]["image"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(120, 120));
                }else
                {
                    portrait = Sprite::create("Match/icon_wanbei.png");
                    portrait->setScale(0.8);
                }
                rankMark = ImageView::create("Match/user_no3.png");
                rankMark->setPosition(Vec2(winSize.width - 240, 105));
                portrait->setPosition(Vec2(rankMark->getContentSize().width/2, rankMark->getContentSize().height/2+10));
                
                break;
        }
        rankMark->setAnchorPoint(Vec2(0.5, 0));
        cell3->addChild(rankMark);
        rankMark->addChild(portrait);
        
    }
    
    //viewList->addChild(cell3);
    
    Layout *cell5 = Layout::create();
    cell5->setContentSize(Size(winSize.width, 90));
    cell5->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    cell5->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    
    auto line1 = Layout::create();
    line1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line1->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4e));
    line1->setContentSize(Size(winSize.width, 3));
    cell5->addChild(line1);
    
    Text *cell5Text = Text::create("详情" , ".SFUIDisplay-Semibold", 36);
    cell5Text->setAnchorPoint(Vec2(0, 0.5));
    cell5Text->setPosition(Vec2(40, cell5->getContentSize().height/2));
    cell5Text->setColor(Color3B(0xcc, 0xcc, 0xcc));
    cell5->addChild(cell5Text);

    Text *cell5Text2 = Text::create("战绩" , ".SFUIDisplay-Semibold", 36);
    cell5Text2->setAnchorPoint(Vec2(0.5,0.5));
    cell5Text2->setPosition(Vec2(winSize.width - cell5Text2->getContentSize().width / 2 - 250, cell5->getContentSize().height/2));
    cell5Text2->setColor(Color3B(0xcc, 0xcc, 0xcc));
    cell5->addChild(cell5Text2);
    
    Text *cell5Text3 = Text::create("喜钱" , ".SFUIDisplay-Semibold", 36);
    cell5Text3->setPosition(Vec2(winSize.width - 80, cell5->getContentSize().height/2));
    cell5Text3->setColor(Color3B(0xcc, 0xcc, 0xcc));
    cell5->addChild(cell5Text3);
    
    viewList->addChild(cell5);
    
    Json::Value list = json["playRank"];
    int temp = 0;
    int winValue = 0;
    
    if(list[temp]["integral"].asInt() > 0)
    {
        winValue = list[temp]["integral"].asInt();
    }
    
    for (int i = 0; i < list.size(); i++)
    {
        list[i]["winValue"] = winValue;
        
        auto cellItem = Layout::create();
        cellItem->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        cellItem->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
        cellItem->setContentSize(Size(winSize.width, 180));
        viewList->addChild(cellItem);
        
        RecordDetailItem *item = RecordDetailItem::createWithData(list[i], i+1);
        cellItem ->addChild(item);
        bool hasXiqian = list[i]["withReward"].asBool();
        cell5Text3->setVisible(hasXiqian);
        item->getChildByName("text4")->setVisible(hasXiqian);
        if(!hasXiqian){
            auto texr3 = item->getChildByName("text3");
            texr3->setPosition(Vec2(item->getContentSize().width - texr3->getContentSize().width / 2 - 80,item->getContentSize().height / 2));
            cell5Text2->setPosition(Vec2(winSize.width - 80 - texr3->getContentSize().width / 2, cell5->getContentSize().height/2));
        }else{
            auto texr3 = item->getChildByName("text3");
            texr3->setPosition(Vec2(item->getContentSize().width - texr3->getContentSize().width / 2 - 250,item->getContentSize().height / 2));
            cell5Text2->setPosition(Vec2(winSize.width - 250 - texr3->getContentSize().width / 2, cell5->getContentSize().height/2));
        }
    }
    
    createNewView();
}

void RecordDegitalLayer::hideOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        ImageView *hideMark = (ImageView *)hideCell->getChildByTag(100);
        isShow = !isShow;
        if (!isShow)
        {
            hideMark->loadTexture("Match/more_icon_down.png");
        }
        if (isShow)
        {
            hideMark->loadTexture("Match/more_icon_up.png");
            //隐藏显示的cell
            showHidenView(jsonData);
        }else
        {
            if(layout2Cell != NULL)
            {
                layout2Cell->removeFromParent();
            }
        }
        
    }
}

void RecordDegitalLayer::reviewOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        ZJHModel::getInstance()->gotoView(VIEW_LUXIANG_REVIEW, jsonData);
    }
}

void RecordDegitalLayer::shareVideo(){
    Json::Value json;
    json["roomId"] = roomID;
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            return;
        }
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"];
            string codestr = "录像码:"+data["code"].asString();
            PlatformHelper::WeChatShareText("录像分享", codestr.c_str());
        }
        CCHttpAgent::getInstance()->packets.erase(tag);
        delete loginPacket;
    },"room/dz/video/share",json.toStyledString(),"videoshare");

}

