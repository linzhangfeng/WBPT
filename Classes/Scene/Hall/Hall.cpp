//
//  Hall.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "Hall.hpp"
#include "HallFriend.h"
#include "../Loading/Loading.h"

const int TAG_ALL_GAME = 0;

bool Hall::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }

    Size size = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(29, 29, 31, 255));
    
    setReturn_img("Head/icon_nav_05.png", "Head/icon_nav_05.png", [=](){
        ZJHModel::getInstance()->gotoView(VIEW_MINE_ROOM);
    });
    setName("dating");
    
    setTitle("大厅");
    
    setMore_img("Head/title-icon2.png", "Head/title-icon2_press.png", [=](){
        addoptions();
    });
    
    m_pContainer = NULL;
    oldTag = -1;
    
    CCHttpPacket *hallInitPacket = CCHttpAgent::getInstance()->packets["hallinit"];
    if (hallInitPacket && hallInitPacket->status != 0) {
        this->initHall();
    } else {
        Loading::startLoading(this, size / 2)->setName("loading");
        this->schedule(schedule_selector(Hall::getData), 0.0);
    }
    
    m_pLoading = Loading::startLoading(this, Vec2(size.width/4,size.height - 750));
    m_pLoading->setScale(0.5f);
    m_pLoading->setVisible(false);
    
    return true;
}

void Hall::showTopLoading(){
    m_pLoading->setVisible(true);
}
void Hall::cahcelTopLoading(){
    m_pLoading->setVisible(false);
}


void Hall::addoptions(){
    if(mp == NULL){
        mp = Moreoptions::create();
        mp->callFun([=](){
            addoptions();
        });
        this->addChild(mp,1000);
    }else{
        this->removeChild(mp);
        mp = NULL;
    }
}


void Hall::btnCallback(Ref* ref)
{
    Layout *typeImage = dynamic_cast<Layout *>(ref);
    int tag = typeImage->getTag();

    if (tag == 0)
    {
        if (m_curIndex != kHallRecommend) {
            m_curIndex = kHallRecommend;
            m_pContainer->runAction(MoveTo::create(0.2, Vec2(0, 160)));
        }
        if(hr != NULL){
            hr->RefreshDaTing(NULL);
        }
    }
    else
    {
        if (m_curIndex != kHallFriend) {
            m_curIndex = kHallFriend;
            m_pContainer->runAction(MoveTo::create(0.2, Vec2(-Director::getInstance()->getWinSize().width, 160)));
        }
        if(hf != NULL){
            hf->initSwitchType(tag);
        }
    }
}

void Hall::initHall()
{
    
    Size winSize = Director::getInstance()->getWinSize();
    
    m_pContainer = Widget::create();
    m_pContainer->setContentSize(Size(winSize.width * 2, winSize.height - this->getSpriteHeight()-260));
    m_pContainer->setAnchorPoint(Vec2(0, 0));
    m_pContainer->setPosition(Vec2(0, 160));
    this->addChild(m_pContainer,10);
    
    m_curIndex = kHallRecommend;
    
    hr = HallRecommend::create();
    hr->setPosition(0, 1);
    hr->setCallFun([=](int inx){
        if(inx == 0){
            cahcelTopLoading();
        }else if(inx == 1){
            showTopLoading();
        }else if(inx == 2){
            showPayMent();
        }else if(inx == 3){
            ZJHModel::getInstance()->gotoView(VIEW_REGIST);
        }
    });
    m_pContainer->addChild(hr);

    hf = OtherGameType::create();
    hf->setPosition(winSize.width, 0);
    m_pContainer->addChild(hf);
    
    initTable();
    
}

void Hall::showPayMent(){
    Size winSize = Director::getInstance()->getWinSize();
    PaymentView* pay = PaymentView::create();
    Json::Value _json = ZJHModel::getInstance()->_adPayData;
    pay->initDataMessage(_json);
    pay->setPosition(Vec2(0,-winSize.height));
    this->getParent()->addChild(pay,10000);
    pay->runAction(MoveTo::create(0.5, Vec2(0,0)));
}

void Hall::getData(float dt)
{
    CCHttpPacket *hallInitPacket = CCHttpAgent::getInstance()->packets["hallinit"];
    if (hallInitPacket && hallInitPacket->status != 0)
    {
        this->removeChildByName("loading");
        unschedule(schedule_selector(Hall::getData));
        
        if (hallInitPacket->status != 3)
        {
            return;
        }
        
        hallInitPacket->status = 0;
        
        if (hallInitPacket->resultIsOK())
        {
            this->initHall();
        }
        else
        {
            PlatformHelper::showToast(hallInitPacket->recvVal["message"].asCString());
        }
    }
}


//--------------------------------------------------Top Table--------------------------------------------------


Layout* Hall::getTopTextLay(string tipname,int gametag){
    Layout* lay = Layout::create();
    lay->setAnchorPoint(Vec2(0,0));
    lay->setTag(gametag);
    
    Text *hotname = Text::create(tipname, "AmericanTypewriter", 45);
    hotname->setColor(Color3B(152,145,122));
    hotname->setTouchEnabled(false);
    hotname->setTag(gametag);
    hotname->setAnchorPoint(Vec2(0.5,0.5));
    
    lay->setContentSize(Size(hotname->getContentSize().width + 100, 100));
    lay->setTouchEnabled(true);
    lay->addClickEventListener(CC_CALLBACK_1(Hall::clickImage, this));
    
    LayerColor* bg = LayerColor::create(Color4B(32, 32, 32, 255));
    bg->setAnchorPoint(Vec2(0,0));
    bg->setContentSize(Size(hotname->getContentSize().width + 100, 100));
    lay->addChild(bg);
    hotname->setPosition(Vec2(lay->getContentSize().width/2,lay->getContentSize().height/2));
    lay->addChild(hotname);
    
    return lay;
}

ImageView* getImageLine(){
    ImageView *m_pSelectedFlag = ImageView::create("res/Hall/icon_check_line.png");
    m_pSelectedFlag->setName("pSelected");
    m_pSelectedFlag->setAnchorPoint(Vec2(0.5,0.5));
    m_pSelectedFlag->setScale9Enabled(true);
    m_pSelectedFlag->ignoreAnchorPointForPosition(false);
    return m_pSelectedFlag;
}


void Hall::initTable()
{
    Json::Value &typeInfo = ZJHModel::getInstance()->gameConfigJson["list"]["hallList"];
    int num = typeInfo.size();
    Size winSize = Director::getInstance()->getWinSize();
    
    typeList = ListView::create();
    typeList->setTouchEnabled(true);
    typeList->setDirection(ScrollView::Direction::HORIZONTAL);
    typeList->setBounceEnabled(false);
    typeList->setContentSize(Size(winSize.width - 100, 100));
    typeList->setAnchorPoint(Vec2(0, 1));
    typeList->setPosition(Vec2(0, winSize.height - this->getSpriteHeight()));
    typeList->setScrollBarEnabled(false);
    typeList->setGravity(ListView::Gravity::CENTER_VERTICAL);
    addChild(typeList,100);
    
    Layout* hotlay = getTopTextLay("热门",TAG_ALL_GAME);
    typeList->addChild(hotlay);

    for (int i = 0; i < num; i++) {
        int gameId = typeInfo[i]["id"].asInt();
        string gamename = typeInfo[i]["classifyName"].asString();
        Layout* hotlay = getTopTextLay(gamename,gameId);
        typeList->addChild(hotlay);
    }
    
    ImageView* m_pSelectedFlag = getImageLine();
    Layout* layout = (Layout*)typeList->getChildByTag(TAG_ALL_GAME);
    oldTag = TAG_ALL_GAME;
    if(layout != NULL){
        m_pSelectedFlag->setContentSize(Size(layout->getContentSize().width,1));
        m_pSelectedFlag->setPosition(Vec2(layout->getContentSize().width/2, 10));
        layout->addChild(m_pSelectedFlag);
    }
    
    Layout* lay = Layout::create();
    lay->setTouchEnabled(true);
    lay->setAnchorPoint(Vec2(0,1));
    lay->setContentSize(Size(100,100));
    LayerColor* bg = LayerColor::create(Color4B(32, 32, 32, 255));
    bg->setAnchorPoint(Vec2(0,1));
    bg->setContentSize(Size(100,100));
    lay->addChild(bg);
    
    Button *img = Button::create("Hall/button/btn_upmenu_check.png","Hall/button/btn_upmenu.png");
    img->setAnchorPoint(Vec2(0,1));
    img->setPosition(Vec2((100-img->getContentSize().width)/2,100-((100-img->getContentSize().height)/2)));
    lay->addChild(img);
    
    lay->setPosition(Vec2(winSize.width-100, winSize.height - this->getSpriteHeight()));
    addChild(lay,100);
    
    img->addClickEventListener([=](Ref *ref)
                               {
                                   OtherGameTypeView* typeView = OtherGameTypeView::create();
                                   typeView->setTouchEnabled(true);
                                   typeView->setCallBack([=](int gameId){
                                       callBackToIndex(gameId);
                                   });
                                   typeView->setPosition(Vec2(0,160));
                                   Json::Value &typeInfo = ZJHModel::getInstance()->gameConfigJson["list"]["hallList"];
                                   typeView->initData(typeInfo);
                                   this->addChild(typeView,120);
                               });
}

void Hall::callBackToIndex(int gameid){
    int count = (int)typeList->getChildrenCount();
    for(int i = 0;i<count;i++){
        Layout* lay = (Layout*)typeList->getItem(i);
        int tag = lay->getTag();
        if(tag == gameid){
            clickImage(lay);
            scrollListViewToPosition(lay);
        }
    }
}

void Hall::scrollListViewToPosition(Layout* _lay){
    int leng = typeList->getInnerContainerSize().width;
    int h_width = (int)Director::getInstance()->getWinSize().width/2;
    int posx = _lay->getPositionX();
    int h_layleng = _lay->getContentSize().width;
    if(posx<h_width){
        typeList->getInnerContainer()->setPosition(Vec2(0,0));
    }else if(posx>leng-h_width){
        typeList->getInnerContainer()->setPosition(Vec2(h_width*2-leng-100,0));
    }else{
        int x = posx - h_layleng;
        typeList->getInnerContainer()->setPosition(Vec2(-x,0));
    }
}


void Hall::clickImage(cocos2d::Ref *sender)
{
    Layout *typeImage = dynamic_cast<Layout *>(sender);
    int tag = typeImage->getTag();
    if(oldTag != -1 && oldTag!=tag){
        Layout* layout = (Layout*)typeList->getChildByTag(oldTag);
        if(layout != NULL){
            layout->removeChildByName("pSelected");
        }
    }
    ImageView* m_pSelectedFlag = getImageLine();
    Layout* layout = (Layout*)typeList->getChildByTag(tag);
    if(layout != NULL && oldTag!=tag){
        m_pSelectedFlag->setContentSize(Size(layout->getContentSize().width,1));
        m_pSelectedFlag->setPosition(Vec2(layout->getContentSize().width/2, 10));
        layout->addChild(m_pSelectedFlag);
        oldTag = tag;
    }
    this->btnCallback(sender);
}
