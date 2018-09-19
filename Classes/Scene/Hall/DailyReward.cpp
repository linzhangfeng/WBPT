//
//  DailyReward.cpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/14.
//
//

#include "DailyReward.h"
#include "../Loading/Loading.h"

bool DailyReward::init()
{
    if (!Layout::init()) {
        return false;
    }

    inde = 0;
    Size winSize = Director::getInstance()->getWinSize();
    this->setTouchEnabled(true);
    this->setContentSize(winSize);
    
    LayerColor* clo = LayerColor::create(Color4B(122,122,122,0));
    clo->setContentSize(winSize);
    
    this->addChild(clo);
    this->addClickEventListener([=](Ref *ref){
        this->removeFromParentAndCleanup(true);
    });
    
    midLaySize = Size(winSize.width-200,winSize.height-500);
    midlay = Layout::create();
    midlay->setTouchEnabled(true);
    midlay->setContentSize(midLaySize);
    midlay->setAnchorPoint(Vec2(0,0));
    midlay->setPosition(Vec2(100,200));
    
    ImageView*  _sprite =  ImageView::create("Play/play_itembg.png");
    _sprite->setContentSize(midLaySize);
    _sprite->ignoreContentAdaptWithSize(false);
    _sprite->setAnchorPoint(Vec2(0,0));
    _sprite->setPosition(Vec2(0,0));
    
    LayerColor* clolay = LayerColor::create(Color4B(26,26,26,255));
    clolay->setContentSize(Size(midLaySize.width-40,midLaySize.height-60));
    clolay->setPosition(Vec2(20,40));
    _sprite->addChild(clolay);
    

    midlay->addChild(_sprite);
    
    this->addChild(midlay);
    
    initView();
    
    return true;
}

void DailyReward::cgettitleColor(int newindex){
    
    Layout* tday = (Layout*)laytop->getChildByTag(newindex);
    
    int fixPos = newindex - 1;
    Json::Value _json =  ZJHModel::getInstance()->loginJson;
    _json["list"][fixPos]["status"] = 1;
    _json["signToday"] = true;
    
       ImageView*  main_num = (ImageView*)tday->getChildByName("mmain");
        if(main_num){
            ImageView *selt = ImageView::create("Hall/reward/jb_check_icon.png");
            selt->setContentSize(Size(102,88));
            selt->setAnchorPoint(Vec2(0.5, 0.5));
            selt->setPosition(Vec2(90,90));
            selt->setTouchEnabled(false);
            selt->setColor(Color3B::GRAY);
            selt->ignoreContentAdaptWithSize(false);
            main_num->addChild(selt);
        }
    button->setTitleText("已领取");
    button->setEnabled(false);
}


void DailyReward::sendSignreward(){
    
    Json::Value json;
    char buff[64];
    snprintf(buff, sizeof(buff), "signList");
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
                                                 if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
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
                                                                 Json::Value _Value = packet->recvVal["resultMap"];
                                                                 ZJHModel::getInstance()->loginJson = _Value;
                                                                 
                                                                 __NotificationCenter::getInstance()->postNotification("daily_receive");
                                                                 
                                                                 this->removeFromParentAndCleanup(true);
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
                                             "task/signList", json.toStyledString(), buff);
}


Layout* getitemLayout(int index){
    
    Layout* layItem = Layout::create();
    layItem->setAnchorPoint(Vec2(0.5, 0.5));
    layItem->setContentSize(Size(200,200));
    layItem->setTag(index);
    
    ImageView *nobleimage = ImageView::create("Hall/reward/money.png");
    nobleimage->setContentSize(Size(185,185));
    nobleimage->setAnchorPoint(Vec2(0.5, 0.5));
    nobleimage->setPosition(Vec2(100,100));
    nobleimage->setTouchEnabled(false);
    nobleimage->setName("mmain");
    nobleimage->ignoreContentAdaptWithSize(false);
    layItem->addChild(nobleimage);
    
    
    ImageView *bgttp = ImageView::create("Hall/reward/bar_jb_send.png");
    bgttp->setContentSize(Size(106,40));
    bgttp->setAnchorPoint(Vec2(0.5, 0.5));
    bgttp->setPosition(Vec2(100,185));
    bgttp->setTouchEnabled(false);
    bgttp->setName("topmain");
    bgttp->ignoreContentAdaptWithSize(false);
    layItem->addChild(bgttp);
    
    Text* days = Text::create("123", "AmericanTypewriter", 30);
    days->setAnchorPoint(Vec2(0.5,0.5));
    days->setPosition(Vec2(bgttp->getContentSize().width/2 ,20));
    days->setName("days");
    bgttp->addChild(days);
    
   Text* money = Text::create("123", "AmericanTypewriter", 40);
    money->setAnchorPoint(Vec2(1, 0));
    money->setPosition(Vec2(nobleimage->getContentSize().width - 20,0));
    money->setName("moneynum");
    nobleimage->addChild(money);
    
    return layItem;
}


void DailyReward::initView(){
    
    Size _Size = Director::getInstance()->getWinSize();
    Button* butclose = Button::create("Hall/button/close.png",
                                    "Hall/button/closex.png");
    butclose->setAnchorPoint(Vec2(0.5,0.5));
    butclose->setScale(2.0);
    butclose->setPosition(Vec2(_Size.width-210,_Size.height-510));
    butclose->addClickEventListener([=](Ref* ref){
        this->removeFromParentAndCleanup(true);
    });
    midlay->addChild(butclose);
    
    Text* text = Text::create("每日登录","AmericanTypewriter", 55);
    text->setColor(Color3B::YELLOW);
    text->setAnchorPoint(Vec2(0.5,0.5));
    text->setPosition(Vec2(midLaySize.width/2,midLaySize.height - text->getContentSize().height/2 - 30));
    midlay->addChild(text);
    
    
    laytop = Layout::create();
    laytop->setContentSize(Size(midLaySize.width-100,600));
    LayerColor* laytop_bg = LayerColor::create(Color4B(0,0,0,255));
    laytop_bg->setAnchorPoint(Vec2(0.5,0.5));
    laytop_bg->setContentSize(Size(midLaySize.width-100,600));
    laytop->addChild(laytop_bg);
    laytop->setAnchorPoint(Vec2(0.5,0.5));
    laytop->setPosition(Vec2(midLaySize.width/2, midLaySize.height - text->getContentSize().height - 350));
    midlay->addChild(laytop);
    
    
    Layout*  lay =  getitemLayout(1);
    lay->setPosition(Vec2((midLaySize.width-100)/8,450));
    laytop->addChild(lay);

    lay =  getitemLayout(2);
    lay->setPosition(Vec2((midLaySize.width-100)*3/8,450));
    laytop->addChild(lay);
    
    lay =  getitemLayout(3);
    lay->setPosition(Vec2((midLaySize.width-100)*5/8,450));
    laytop->addChild(lay);
    
    lay =  getitemLayout(4);
    lay->setPosition(Vec2((midLaySize.width-100)*7/8,450));
    laytop->addChild(lay);
    
    lay =  getitemLayout(5);
    lay->setPosition(Vec2((midLaySize.width-100)/4,150));
    laytop->addChild(lay);
    
    lay =  getitemLayout(6);
    lay->setPosition(Vec2((midLaySize.width-100)/2,150));
    laytop->addChild(lay);
    
    lay =  getitemLayout(7);
    lay->setPosition(Vec2((midLaySize.width-100)*3/4,150));
    laytop->addChild(lay);
    
    
//    dayListView = ListView::create();
//    dayListView->setDirection(ui::ScrollView::Direction::VERTICAL);
//    dayListView->setBounceEnabled(false);
//    dayListView->setContentSize(Size(midLaySize.width,500));
//    dayListView->setAnchorPoint(Vec2(0.5, 1));
//    dayListView->setPosition(Vec2(midLaySize.width/2, midLaySize.height - text->getContentSize().height - 50));
//    dayListView->setScrollBarEnabled(false);
//    dayListView->setItemsMargin(20);
//    midlay->addChild(dayListView,100);

//--------------------------------------会员每日额外赠送-----------------------------------
    
    Text *texttip = Text::create("会员每日额外赠送", "AmericanTypewriter", 55);
    texttip->setAnchorPoint(Vec2(0.5, 1));
    texttip->setPosition(Vec2(midLaySize.width/2, midLaySize.height - text->getContentSize().height - 700));
    texttip->setColor(Color3B::YELLOW);
    midlay->addChild(texttip);
    
    
    Layout* laybottom = Layout::create();
    laybottom->setContentSize(Size(midLaySize.width-100,300));
    LayerColor* laybottom_bg = LayerColor::create(Color4B(0,0,0,255));
    laybottom_bg->setAnchorPoint(Vec2(0.5,0.5));
    laybottom_bg->setContentSize(Size(midLaySize.width-100,300));
    laybottom->addChild(laybottom_bg);
    laybottom->setAnchorPoint(Vec2(0.5,0.5));
    laybottom->setPosition(Vec2(midLaySize.width/2, midLaySize.height - text->getContentSize().height - 950));
    midlay->addChild(laybottom);
    
    
    
    
    ImageView *vipimage = ImageView::create("Hall/reward/noble.png");
    vipimage->setContentSize(Size(185,185));
    vipimage->setAnchorPoint(Vec2(0.5, 0.5));
    vipimage->setPosition(Vec2(laybottom->getContentSize().width/2, 150));
    vipimage->setTouchEnabled(false);
    vipimage->ignoreContentAdaptWithSize(false);
    laybottom->addChild(vipimage);

    nobleMoney = Text::create("1200", "fonts/arialbd.ttf", 40);
    nobleMoney->setAnchorPoint(Vec2(1, 0));
    nobleMoney->setPosition(Vec2(vipimage->getContentSize().width - 20,0));
    vipimage->addChild(nobleMoney);
    
    
    
    ImageView *nobleimage = ImageView::create("Hall/reward/patriarch.png");
    nobleimage->setContentSize(Size(185,185));
    nobleimage->setAnchorPoint(Vec2(0.5, 0.5));
    nobleimage->setPosition(Vec2(laybottom->getContentSize().width*5/6, 150));
    nobleimage->setTouchEnabled(false);
    nobleimage->ignoreContentAdaptWithSize(false);
    laybottom->addChild(nobleimage);
    
    patriarchimageMoney = Text::create("1200", "fonts/arialbd.ttf", 40);
    patriarchimageMoney->setAnchorPoint(Vec2(1, 0));
    patriarchimageMoney->setPosition(Vec2(vipimage->getContentSize().width - 20,0));
    nobleimage->addChild(patriarchimageMoney);
    
    
    ImageView *patriarchimage = ImageView::create("Hall/reward/VIP.png");
    patriarchimage->setContentSize(Size(185,185));
    patriarchimage->setAnchorPoint(Vec2(0.5, 0.5));
    patriarchimage->setPosition(Vec2(laybottom->getContentSize().width/6, 150));
    patriarchimage->setTouchEnabled(false);
    patriarchimage->ignoreContentAdaptWithSize(false);
    laybottom->addChild(patriarchimage);

    
    vipMoney = Text::create("1200", "fonts/arialbd.ttf", 40);
    vipMoney->setAnchorPoint(Vec2(1, 0));
    vipMoney->setPosition(Vec2(vipimage->getContentSize().width - 20,0));
    patriarchimage->addChild(vipMoney);

    
    button = Button::create("Hall/button/btn_yellow_small.png","Hall/button/btn_yellow_big.png","Hall/button/btn_yellow_small_check.png");
    button->setContentSize(Size(midLaySize.width-200,120));
    button->setAnchorPoint(Vec2(0.5, 0.5));
    button->setScale9Enabled(true);
    button->setTitleText("领取奖励");
    button->setTitleFontSize(50);
    button->setPosition(Vec2(midLaySize.width/2,120));
    button->addClickEventListener([=](Ref*){
        Size z_size = Director::getInstance()->getWinSize();
       Loading::startLoading(this, z_size / 2)->setName("loading");
       sendGetSignreward();
    });
    midlay->addChild(button);
}

void DailyReward::addJsonData(Json::Value m_value)
{
    int num = m_value["list"].size();
//    rewardListItem* item = NULL;
    inde = 0;
    for(int index= 0;index<num;index++){
        int state = m_value["list"][index]["status"].asInt();
        Layout* tday = (Layout*)laytop->getChildByTag(index+1);
        ImageView* main_num = NULL;
        if(tday){
            main_num = (ImageView*)tday->getChildByName("mmain");
            ImageView* dtext = (ImageView*)tday->getChildByName("topmain");
            if(dtext){
                Text* _text = (Text*)dtext->getChildByName("days");
                _text->setString(m_value["list"][index]["taskDesc"].asCString());
            }
            if(main_num){
                Text* _text = (Text*)main_num->getChildByName("moneynum");
               __String*  ns = __String::createWithFormat("%d",m_value["list"][index]["recieveCount"].asInt());
                _text->setString(ns->getCString());
            }
        }
        
        if(state == 1){
            inde++;
            ImageView *selt = ImageView::create("Hall/reward/jb_check_icon.png");
            selt->setContentSize(Size(102,88));
            selt->setAnchorPoint(Vec2(0.5, 0.5));
            selt->setPosition(Vec2(90,90));
            selt->setTouchEnabled(false);
            selt->setColor(Color3B::GRAY);
            selt->ignoreContentAdaptWithSize(false);
            main_num->addChild(selt);
        }
//        item = rewardListItem::createWithData(index,m_value["list"][index],midLaySize.width,[=](int index){cgettitleColor(index);});
//        dayListView->addChild(item);
    }
    if(m_value["signToday"].asBool()){
        button->setTitleText("已领取");
        button->setEnabled(false);
    }else{
        button->setTitleText("领取奖励");
    }
    __String* ns = __String::createWithFormat("%d %s",m_value["vipTask"]["recieveCount"].asInt(),"");
    vipMoney->setString(ns->getCString());
    
    __String* no = __String::createWithFormat("%d %s",m_value["svipTask"]["recieveCount"].asInt(),"");
    nobleMoney->setString(no->getCString());
    
    __String* pa = __String::createWithFormat("%d %s",m_value["patriarchTask"]["recieveCount"].asInt(),"");
    patriarchimageMoney->setString(pa->getCString());
}


rewardListItem *rewardListItem::createWithData(int index,Json::Value m_info,int width,const MyCallBack_Int &callback)
{
    rewardListItem *pRet = new rewardListItem();
    if (pRet && pRet->initWithData(index,m_info,width,callback)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool rewardListItem::initWithData(int index,Json::Value m_info,int width,const MyCallBack_Int &callback)
{
    if (Layout::init() == false) {
        return false;
    }
    
    setTouchEnabled(true);
    
    Size itemSize = Size(width-40, 120);
    
    this->setContentSize(itemSize);
    this->setAnchorPoint(Vec2(0,0));
    this->setPosition(Vec2(20,0));
    
    int state = m_info["status"].asInt();
    Color4B image_show ;
    string pointimage ;
    if(state == 0){
        image_show = Color4B(64,64,64,255);
        pointimage = "Hall/check_point_out.png";
    }else if(state == 1){
        image_show = Color4B(39,39,39,255);
        pointimage = "Hall/check_point.png";
    }
    LayerColor* clo = LayerColor::create(image_show);
    clo->setContentSize(itemSize);
    clo->setPosition(Vec2(20,0));
    clo->setName("lq_image");
    this->addChild(clo);

    if(index == 0){
        Layout* lay = getLineLayout(5,60);
        lay->setAnchorPoint(Vec2(0.5, 1));
        lay->setPosition(Vec2(100,60));
        this->addChild(lay);
    }else if(index == 6){
        Layout* lay = getLineLayout(5,60);
        lay->setAnchorPoint(Vec2(0.5, 0));
        lay->setPosition(Vec2(100,60));
        this->addChild(lay);
    }else{
         Layout* lay = getLineLayout(5,120);
        lay->setAnchorPoint(Vec2(0.5, 0.5));
        lay->setPosition(Vec2(100,60));
        this->addChild(lay);
    }
    
    //图片的
    ImageView *image_point = ImageView::create(pointimage);
    image_point->setContentSize(Size(40,40));
    image_point->setName("pointname");
    image_point->setAnchorPoint(Vec2(0.5, 0.5));
    image_point->setPosition(Vec2(100, 60));
    image_point->ignoreContentAdaptWithSize(false);
    this->addChild(image_point);
    
    //文字
    Text *name = Text::create(m_info["taskDesc"].asCString(), "AmericanTypewriter", 40);
    name->setAnchorPoint(Vec2(0, 0.5));
    name->setPosition(Vec2(250, 60));
    name->setColor(Color3B::BLACK);
    this->addChild(name);
    

    //图片的
    ImageView *image_lv = ImageView::create("Hall/icon_money.png");
    image_lv->setContentSize(Size(80,80));
    image_lv->setAnchorPoint(Vec2(0.5, 0.5));
    image_lv->setPosition(Vec2((width- 300), 60));
    image_lv->setTouchEnabled(false);
    image_lv->ignoreContentAdaptWithSize(false);
    this->addChild(image_lv);
    
    
    //金币的
    __String* ns = NULL;
    if(m_info["moneyType"].asInt() == 1){
        ns = __String::createWithFormat("%d %s",m_info["recieveCount"].asInt(),"金币");
    }else{
        ns = __String::createWithFormat("%d %s",m_info["recieveCount"].asInt(),"玩币");
    }
    string value = ns->getCString();
    Text *money = Text::create(value, "AmericanTypewriter", 40);
    money->setAnchorPoint(Vec2(0, 0.5));
    money->setPosition(Vec2((width- 250), 60));
    money->setColor(Color3B::YELLOW);
    this->addChild(money);

    
    this->addClickEventListener([=](Ref* ref){
        if(callback != NULL){
            //callback(index);
        }
    });
    
    return true;
}

Layout* rewardListItem::getLineLayout(int width,int height)
{
    Layout* linelay = Layout::create();
    linelay->setContentSize(Size(width,height));
    linelay->setAnchorPoint(Vec2(0,0));
    LayerColor* clo = LayerColor::create(Color4B(69,69,69,255));
    clo->setContentSize(Size(width,height));
    linelay->addChild(clo);
    return linelay;
}


void DailyReward::sendGetSignreward(){
    Json::Value json;
    char buff[64];
    snprintf(buff, sizeof(buff), "takesign");
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
                                                 if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
                                                 this->removeChildByName("loading");
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
                                                                
                                                                 Json::Value _Value = packet->recvVal["resultMap"];
                                                                 if(packet->recvVal["success"].asBool()){
                                                                     inde++;
                                                                     PlatformHelper::showToast("领取成功");
                                                                     cgettitleColor(inde);
                                                                     __NotificationCenter::getInstance()->postNotification("money_change");
                                                                     sendSignreward();
                                                                 }else{
                                                                     PlatformHelper::showToast(packet->recvVal["message"].asCString());
                                                                 }
                                                             }
                                                             else
                                                             {
                                                             }
                                                             
                                                         }
                                                         //
                                                     }
                                                     //删除packet
                                                     CCHttpAgent::getInstance()->packets.erase(tag);
                                                     delete packet;
                                                 }
                                             },
                                             "task/sign", json.toStyledString(), buff);
}

Layout* DailyReward::getLineLayout(int height)
{
    Layout* linelay = Layout::create();
    linelay->setContentSize(Size(midLaySize.width,height));
    linelay->setAnchorPoint(Vec2(0.5,1));
    LayerColor* clo = LayerColor::create(Color4B(122,122,122,122));
    clo->setContentSize(Size(midLaySize.width,height));
    linelay->addChild(clo);
    return linelay;
}

