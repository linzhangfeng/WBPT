//
//  MatchResultLayer.cpp
//  ZJH
//
//  Created by apple on 16/8/17.
//
//

#include "MatchResultLayer.hpp"
#include "MatchItem.hpp"

bool MatchResultLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("战绩");
    setMore_img("Match/search_other_video.png","Match/search_other_video.png", [=](){
        showVideoInput();
    });
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    
    auto moreImg = getChildByName("more_img_btn");
    moreImg->setPosition(Vec2(moreImg->getPosition().x - 75,moreImg->getPosition().y));

    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    Size winSize = Director::getInstance()->getWinSize();
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MatchResultLayer::updateMatchPlayerNum), "player_Num_change", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MatchResultLayer::updatePlayAllAddNum), "addAllFriends_refesh", NULL);
    
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"duiju/integral/index", "", "record");
    this->schedule(schedule_selector(MatchResultLayer::postRecord), 0.0f);

    loading = Loading::startLoading(this, winSize / 2);
    loading->maskSwllowTouch();
    
    ImageView *playerBg = ImageView::create("Match/match_bg_players.png");
    playerBg->setAnchorPoint(Vec2(0, 1));
    playerBg->setPosition(Vec2(10, winSize.height - getHeadH() - 20));
    playerBg->setContentSize(Size(Size(winSize.width - 20, 140)));
    playerBg->setScale9Enabled(true);
    this->addChild(playerBg);
    playerBg->setTouchEnabled(true);
    playerBg->addClickEventListener([=](Ref*ref){
        
        ZJHModel::getInstance()->gotoView(VIEW_MATCH_PLAYER);
        
    });
    
    ImageView *playerIcon = ImageView::create("Match/match_player_icon.png");
    playerIcon->setAnchorPoint(Vec2(0, 0.5));
    playerIcon->setPosition(Vec2(30, playerBg->getContentSize().height/2));
    playerBg->addChild(playerIcon);
    
    numT = Text::create("", "", 40);
    numT->setAnchorPoint(Vec2(0, 0.5));
    numT->setPosition(Vec2(130, playerBg->getContentSize().height/2));
    numT->setColor(Color3B(0x80, 0x80, 0x80));
    playerBg->addChild(numT);
    
    Text *intoT = Text::create("点击查看 > ", "", 40);
    intoT->setAnchorPoint(Vec2(1, 0.5));
    intoT->setTextHorizontalAlignment(TextHAlignment::RIGHT);
    intoT->setPosition(Vec2(playerBg->getContentSize().width - 30, playerBg->getContentSize().height/2));
    intoT->setColor(Color3B(0xe2, 0xce, 0xae));
    playerBg->addChild(intoT);
    
    return true;
}


void MatchResultLayer::updatePlayAllAddNum(Ref*ref)
{
    __Integer *count = (__Integer*)ref;
    int numRef = count->getValue();
    
    __String *buf = __String::createWithFormat("我打过牌的人有%d位", numRef);
    numT->setString(buf->getCString());

}
void MatchResultLayer::updateMatchPlayerNum(Ref*ref)
{
    num--;
    __String *buf = __String::createWithFormat("我打过牌的人有%d位", num);
    numT->setString(buf->getCString());
}

void MatchResultLayer::postRecord(float dt)
{
    //请求战绩
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["record"];
    if (packet && packet->status != 0)
    {
        loading->removeFromParent();
        unschedule(schedule_selector(MatchResultLayer::postRecord));
        if (packet->status != 3)
        {
            return;
        }
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
        
        CCHttpAgent::getInstance()->packets["record"] = NULL;
        delete packet;
    }
}

void MatchResultLayer::showView(Json:: Value json)
{
    num = json["playerNum"].asInt();
    __String *buf = __String::createWithFormat("我打过牌的人有%d位", json["playerNum"].asInt());
    numT->setString(buf->getCString());
    
    Size winSize = Director::getInstance()->getWinSize();
    float H = this->getContentSize().height;
    
    listRecord = NULL;
    
    //请求战绩记录的分页消息
    isFirstPost = false;
    pageIndex = 1;
    Json::Value json3;
    json3["pageIndex"] = pageIndex;
    json3["pageSize"] = 10;
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"duiju/historyList", json3.toStyledString(), "historyList");
    this->schedule(schedule_selector(MatchResultLayer::postRecordDetail), 0.0f);
    
    Size size = Size(winSize.width, H - 600);
    loading2 = Loading::startLoading(this, size / 2);
}

void MatchResultLayer::postRecordDetail(float dt)
{
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["historyList"];
    if (packet && packet->status != 0)
    {
        loading2->removeFromParent();
        unschedule(schedule_selector(MatchResultLayer::postRecordDetail));
        
        if (packet->status != 3)
        {
            return;
        }
        packet->status = 0;
        
        if (packet->resultIsOK())
        {
            Json::Value data = packet->recvVal["resultMap"]["historyList"];
            isFirstPost = true;
            //战绩记录
            recordIndex = 800;
            recordJson.clear();
            showRecordList(data);
        }
        else
        {
            PlatformHelper::showToast(packet->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets["historyList"] = NULL;
        delete packet;
    }

}
void MatchResultLayer::showRecordList(Json::Value json)
{
    Size winSize = Director::getInstance()->getWinSize();
    float H = this->getContentSize().height - getHeadH() - 185;
    
    Json::Value listData = json["result"];
    
    int nums = listData.size();
    if (nums == 0 && isFirstPost)
    {
        Text *text = Text::create("暂无对战纪录", "", 36);
        text->setPosition(Vec2(winSize.width/2, H/2));
        text->setTextColor(Color4B::GRAY);
        this->addChild(text);
        return;
    }
    
    if(listRecord == NULL)
    {
        listRecord = ListView::create();
        listRecord->setDirection(ScrollView::Direction::VERTICAL);
        listRecord->setBounceEnabled(false);
        listRecord->setContentSize(Size(winSize.width, H));
        listRecord->setItemsMargin(25);
        listRecord->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
        this->addChild(listRecord);
        
        listRecord->addEventListener([=](Ref *sendre, ScrollView::EventType eventType){
            if (eventType == ScrollView::EventType::SCROLL_TO_BOTTOM)
            {
                pageIndex++;
                Json::Value json5;
                json5["pageIndex"] = pageIndex;
                json5["pageSize"] = 10;
                
                CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
                    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["historyListMore"];
                    if (packet->status != 3)
                    {
                        return;
                    }
                    packet->status = 0;
                    
                    if (packet->resultIsOK())
                    {
                        isFirstPost = false;
                        Json::Value data = packet->recvVal["resultMap"]["historyList"];
                        addDataInList(data);
                    }
                    else
                    {
                        PlatformHelper::showToast(packet->recvVal["message"].asCString());
                    }
                    
                    CCHttpAgent::getInstance()->packets["historyListMore"] = NULL;
                    delete packet;
                    
                    
                },"duiju/historyList", json5.toStyledString(), "historyListMore");
            }
            
        });
    }
    
    addDataInList(json);
}

void MatchResultLayer::addDataInList(Json::Value json)
{
    Json::Value listData = json["result"];
    int nums = listData.size();
    
    for (int i = 0; i < nums; i++)
    {
        recordeItem *item = recordeItem::createWithData(listData[i]);
        item->setTag(recordIndex++);
        item->setTouchEnabled(true);
        item->addClickEventListener(CC_CALLBACK_1(MatchResultLayer::gotoRecordDetail, this));
        listRecord->addChild(item);
        
        recordJson.push_back(listData[i]);
    }
    if(nums > 0)listRecord->forceDoLayout();
}

void MatchResultLayer::gotoRecordDetail(Ref *pSender)
{
    recordeItem *item = (recordeItem *)pSender;
    int Index = item->getTag()-800;
    Json::Value data;
    data["roomID"] = recordJson[Index]["id"];
    data["gameId"] = recordJson[Index]["gameId"];
    data["code"] = recordJson[Index]["code"];
    printf("ggg == %s\n", recordJson[Index]["code"].asString().c_str());
    //战绩详情
    ZJHModel::getInstance()->gotoView(VIEW_RECORD_DETIL, data);
}

void MatchResultLayer::gotoWinRate(Ref *pSender)
{

    ZJHModel::getInstance()->gotoView(VIEW_WINRATE);
   
}

//对局玩家
void MatchResultLayer::gotoMatchPlayer(Ref *pSender)
{
    ZJHModel::getInstance()->gotoView(VIEW_MATCH_PLAYER);
}

void MatchResultLayer::requestVideoInfo(Json::Value value){

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
            Json::Value data = loginPacket->recvVal["resultMap"];
            data["roomID"] = value["roomId"];
            data["gameId"] = value["gameId"];
            data["pkgId"] = value["pkgId"];
            ZJHModel::getInstance()->gotoView(VIEW_LUXIANG_REVIEW, data);
            if (ZJHModel::getInstance()->curGameID == -1)
            {
                ZJHModel::getInstance()->setCurGameID(value["gameId"].asInt(), 1);
            }
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    },"duiju/integral/detail", value.toStyledString(), "detail");
}

void MatchResultLayer::showVideoInput(){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout *bgLayout = Layout::create();
    bgLayout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    bgLayout->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    bgLayout->setBackGroundColorOpacity(150);
    bgLayout->setTouchEnabled(true);
    bgLayout->setSwallowTouches(true);
    bgLayout->setContentSize(visibleSize);
    bgLayout->addClickEventListener([=](Ref*ref){
        bgLayout->removeFromParent();
    });
    
    Director::getInstance()->getRunningScene()->addChild(bgLayout, 1000);
    

    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(visibleSize.width - 300, 500));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    bgLayout->addChild(bg);
    
    float H = bg->getContentSize().height;
    float bgW = bg->getContentSize().width;
    
    Button *cancel = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    cancel->setPosition(Vec2(bgW, H));
    cancel->addClickEventListener([=](Ref*ref){
        bgLayout->removeFromParent();
    });
    bg->addChild(cancel);
    
    Text *title = Text::create("请输入录像码", "", 40);
    title->setAnchorPoint(Vec2(0, 0.5));
    title->setTextColor(Color4B(0xf1, 0xcb, 0x5b, 255));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(50, H - 100));
    bg->addChild(title);
    
    EditBox* edit = EditBox::create(Size(bg->getContentSize().width - 100, 80), "Mine/bar_sendtxt.png");
    edit->setPlaceholderFontSize(42);
    edit->setAnchorPoint(Vec2(0.5, 0.5));
    edit->setPosition(Vec2(bg->getContentSize().width / 2, H - 200));
    edit->setFontColor(Color3B(0x99, 0x99, 0x99));
    edit->setFontSize(40);
    bg->addChild(edit);
    edit->setInputMode(EditBox::InputMode::NUMERIC);
    
    Button *but = Button::create("Mine/btn_yellow_big.png","Mine/btn_yellow_big_check.png");
    but->setScale9Enabled(true);
    but->setTitleText("查看");
    but->setContentSize(Size(600, 80));
    but->setAnchorPoint(Vec2(0.5, 0));
    but->setPosition(Vec2(bgW/2, 40));
    bg->addChild(but);
    but->setTitleFontSize(40);
    but->setTitleColor(Color3B(0x65, 0x40, 0x02));
    but->setTitleFontName("");
    but->addClickEventListener([=](Ref* sender)
                               {
                                   Json::Value json;
                                   json["code"] = edit->getText();
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
                                           bgLayout->removeFromParent();
                                           Json::Value data = loginPacket->recvVal["resultMap"];
                                           requestVideoInfo(data);
                                       }else
                                       {
                                           PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                                       }
                                   },"room/dz/video/get", json.toStyledString(), "videoget");
                                   
                               });
    
}




