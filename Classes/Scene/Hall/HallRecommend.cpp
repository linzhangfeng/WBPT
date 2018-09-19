//
//  HallRecommend.cpp
//  ZJH
//
//  Created by hyhy on 16/7/4.
//
//

#include "HallRecommend.h"
#include "ui/CocosGUI.h"
#include "ZJH.h"
#include "Marquee.h"
#include "../Load/LoadTable.h"
#include "../Mine/TaskLayer.hpp"
#include "../Loading/Loading.h"


USING_NS_CC;

#pragma mark [ HallRecommend ]

bool HallRecommend::init()
{
    if (Layer::init() == false) {
        return  false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
    setContentSize(Size(winSize.width, winSize.height - 410));
    
    LayerColor* bg = LayerColor::create(Color4B(30, 30, 30, 255));
    bg->setContentSize(this->getContentSize());
    bg->setAnchorPoint(Vec2(0.0f, 0.0f));
    bg->setPosition(0,0);
    addChild(bg, -100);
    
    Marquee *marquee = Marquee::createWithSize(Size(winSize.width - 300, 50));
    marquee->setPosition(Vec2(winSize.width / 2 + 50, winSize.height - 500));
    //this->addChild(marquee, 10);
    
    ImageView* newsBg = ImageView::create("Hall/notice_bg.9.png");
    newsBg->setScale9Enabled(true);
    newsBg->setContentSize(Size(winSize.width - 200, 80));
    newsBg->setPosition(Vec2(marquee->getContentSize().width / 2 - 40 ,marquee->getContentSize().height / 2));
    
    Sprite *lba = Sprite::create("Hall/room_detail_back_notice.png");
    lba->setAnchorPoint(Vec2(0.5,0.5));
    lba->setPosition(Vec2(50,40));
    newsBg->addChild(lba,5);
    

    marquee->addChild(newsBg, -1);
    
    m_jsonData = CCHttpAgent::getInstance()->packets["hallinit"]->recvVal["resultMap"];
    
    _pay = ZJHModel::getInstance()->gameConfigJson["list"]["payTypeFilter"];

    adpageviewCount = 0;
    refreshing = true;
    
    initList();
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(HallRecommend::RefreshDaTing), "refresh_dating", NULL);
    
    return true;
}

void HallRecommend::RefreshDaTing(Ref* ref){
    callFun(1);
    listView->setPosition(Vec2(0,-100));
    Json::Value json;
    
    char buff[64];
    snprintf(buff, sizeof(buff), "game_hall");
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
                                                 if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
                                                 CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                 if (packet) {
                                                     
                                                     callFun(0);
                                                     listView->setPosition(Vec2(0,0));
                                                     
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
                                                                 m_jsonData = packet->recvVal["resultMap"];
                                                                 RefreshView();
                                                             }
                                                             else
                                                             {
                                                                 PlatformHelper::showToast(packet->recvVal["message"].asCString());
                                                             }
                                                             
                                                         }
                                                         //
                                                     }
                                                     
                                                     //删除packet
                                                     CCHttpAgent::getInstance()->packets.erase(tag);
                                                     delete packet;
                                                 }
                                             },
                                             "game/hall", json.toStyledString(), buff);
}

void HallRecommend::setCallFun(const MyCallBack_Int &callback){
    callFun = callback;
}


Layout*  getTextlayout(string name){
    Size winSize = Director::getInstance()->getWinSize();
    Layout* lay = Layout::create();
    lay->setAnchorPoint(Vec2(0,0));
    lay->setContentSize(Size(winSize.width,80));
    
    ImageView* mView = ImageView::create("Hall/icon_3.png");
    mView->setAnchorPoint(Vec2(0,0.5));
    mView->setPosition(Vec2(50, 40));
    lay->addChild(mView);
    
    Text *title = Text::create(name, "AmericanTypewriter", 45);
    title->setColor(Color3B::GRAY);
    mView->setAnchorPoint(Vec2(0,0.5));
    title->setPosition(Vec2(200, 40));
    lay->addChild(title);
    return lay;
}

void HallRecommend::initList()
{
    Size winSize = Director::getInstance()->getWinSize();
    
    listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(true);
    listView->setContentSize(this->getContentSize());
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 0));
    listView->setScrollBarEnabled(false);
    listView->setItemsMargin(0);
    listView->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    addChild(listView);
    
    float scalex = Utils::getScaleX();
    adimage_height = 300*scalex;
    
    RefreshView();
    
    listView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType)
                               {
                                   if (eventType == ScrollView::EventType::SCROLL_TO_TOP && refreshing){
//                                       refreshing = false;
//                                       RefreshDaTing(NULL);
                                   }
                               });
    
}

void HallRecommend::RefreshView(){
    
    if(listView->getChildrenCount() > 0){
        listView->removeAllChildren();
    }
    if (m_jsonData["advertisementList"].size() > 0) {
        Widget *advertisementList = this->getAdList(m_jsonData["advertisementList"]);
        listView->addChild(advertisementList);
    }
    
    if(m_jsonData["navigationList"].size() > 0){
        Layout* lay = Layout::create();
        lay->setContentSize(Size(this->getContentSize().width,280));
        
        LayerColor* bg = LayerColor::create(Color4B(20, 20, 20, 255));
        bg->setContentSize(Size(this->getContentSize().width,280));
        bg->setAnchorPoint(Vec2(0.0f, 0.0f));
        bg->setPosition(0,0);
        lay->addChild(bg);
        
        Layout* selType = getHorizeListView(m_jsonData["navigationList"]);
        lay->addChild(selType);
        listView->addChild(lay);
    }
    
   
    if (m_jsonData["activityList"].size() > 0) {
        Layout* activityText = getTextlayout("活动房");
        listView->addChild(activityText);
        
        Widget *activityList = this->getActivityList(m_jsonData["activityList"]);
        listView->addChild(activityList);
    }
    
    if (m_jsonData["liveList"].size() > 0) {
        Layout* hotText = getTextlayout("直播房");
        listView->addChild(hotText);
        
        Widget *liveList = this->getActivityList(m_jsonData["liveList"]);
        listView->addChild(liveList);
    }
    
    if (m_jsonData["publicList"].size() > 0) {
        Layout* publicText = getTextlayout("公开房");
        listView->addChild(publicText);
        
        Widget *publicList = this->getHotList(m_jsonData["publicList"]);
        listView->addChild(publicList);
    }
    
    if (m_jsonData["privateList"].size() > 0) {
        Layout* privateText = getTextlayout("私密房");
        listView->addChild(privateText);
        
        Widget *privateList = this->getHotList(m_jsonData["privateList"]);
        listView->addChild(privateList);
    }
    
    refreshing = true;
}


Layout* getPointview(int number){
    Layout* pointLay = Layout::create();
    pointLay->setName("pintview");
    pointLay->setContentSize(Size(50*number+10*(number-1),30));
    for(int i = 0;i<number;i++){
        ImageView* view = ImageView::create("Hall/number_bg2.png");
        view->setAnchorPoint(Vec2(0,0.5));
        view->setScale(0.5, 0.5);
        view->setPosition(Vec2(i*25+i*10,15));
        view->setTag(i);
        view->setScale9Enabled(true);
        pointLay->addChild(view);
    }
    return pointLay;
}


Widget* HallRecommend::getLayViewPoint(int num){
    
    Size winSize = Director::getInstance()->getWinSize();
    Widget *viewPointWidget = Widget::create();
    viewPointWidget->setContentSize(Size(winSize.width,30));

    Layout* lay = getPointview(num);
    lay->setPosition(Vec2((winSize.width-lay->getContentSize().width)/2,0));
    viewPointWidget->addChild(lay);
    
    return viewPointWidget;
}

void HallRecommend::changePointView(int index){
    for(int i =0;i<adpageviewCount;i++){
        ImageView* view =  (ImageView*)_widgetPoint->getChildByName("pintview")->getChildByTag(i);
        if(view != NULL){
            if(i == index){
                view->loadTexture("Hall/number_bg.png");
            }else{
                view->loadTexture("Hall/number_bg2.png");
            }
        }
    }
}

Widget *HallRecommend::getActivityList(Json::Value &data)
{
    Size winSize = Director::getInstance()->getWinSize();
    Widget *activityWidget = Widget::create();
    int num = data.size();
    float itemHeight = 397*Utils::getScaleX();
    if (num > 0) {
        Size widgetSize = Size(winSize.width, 40 + (num + 1) / 2 * (itemHeight+40));
        activityWidget->setContentSize(widgetSize);
        
        LayerColor *bg = LayerColor::create(Color4B(20, 20, 20, 255), widgetSize.width, widgetSize.height);
        bg->setPosition(Vec2(0, 0));
        activityWidget->addChild(bg, -10);
        
        for (int i = 0; i < num; i++) {
            float posX = (i%2 ? 0.75 : 0.25) * winSize.width;
            float posY = widgetSize.height - 40 - (itemHeight +40) * (i / 2);
            
            ActivityListItem *item = ActivityListItem::createWithData(data[i]);
            item->setAnchorPoint(Vec2(0.5, 1.0));
            item->setPosition(Vec2(posX, posY));
            activityWidget->addChild(item);
        }
    }
    
    return activityWidget;
}

Widget *HallRecommend::getHotList(Json::Value &data)
{
    Size winSize = Director::getInstance()->getWinSize();
    Widget *hotWidget = Widget::create();
    int num = data.size();
    float itemHeight = 397*Utils::getScaleX();
    if (num > 0) {
        Size widgetSize = Size(winSize.width, 40 + (num + 1) / 2 * (itemHeight+40));
        hotWidget->setContentSize(widgetSize);
        
        LayerColor *bg = LayerColor::create(Color4B(20, 20, 20, 255), widgetSize.width, widgetSize.height);
        bg->setPosition(Vec2(0, 0));
        hotWidget->addChild(bg, -10);
        
        
        for (int i = 0; i < num; i++) {
            float posX = (i%2 ? 0.75 : 0.25) * winSize.width;
            float posY = widgetSize.height - 40 - (itemHeight+40) * (i / 2);
            
            HotListItem *item = HotListItem::createWithData(data[i]);
            item->setAnchorPoint(Vec2(0.5, 1.0));
            item->setPosition(Vec2(posX, posY));
            hotWidget->addChild(item);
        }
    }
    
    return hotWidget;
}

Widget *HallRecommend::getAdList(Json::Value &data)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    Widget *adWidget = Widget::create();
    adWidget->setContentSize(Size(winSize.width, adimage_height));
    
    int num = data.size();
    PageView *adList = PageView::create();
    adList->setContentSize(Size(winSize.width, adimage_height));
    adList->removeAllPages();
    adpageviewCount = num;
    for (int i = 0; i < num; i++) {
      std::string imageUrl = data[i]["imageUrl"].asString();
        Layout *layout = Layout::create();
        layout->setContentSize(Size(winSize.width, adimage_height));
        
        ImageView *image = ImageView::create();
        image->loadTexture("Hall/Adtop.png");
        image->setContentSize(adList->getContentSize());
        image->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height / 2));
        image->setTouchEnabled(true);
        image->ignoreContentAdaptWithSize(false);
        layout->addChild(image);
        
        // Json::Value &typeInfo = ZJHModel::getInstance()->gameConfigJson["list"]["gameconfig"];
        bool flagPay = false;
        if(_pay["alipay"].asBool() || _pay["union"].asBool() || _pay["wechat"].asBool()){
            flagPay = true;
        }
        int id = data[i]["id"].asInt();
        if(id == 3 || id == 4){
            if(flagPay){
                if (Utils::hasImageFile(imageUrl)) {
                    image->loadTexture(Utils::getImageSavePath(imageUrl));
                } else {
                    image->loadTexture("Hall/pic_default2.png");
                    CCHttpAgent::getInstance()->sendHttpImage(imageUrl, Utils::getImageSavePath(imageUrl), Utils::getImageSavePath(imageUrl), [=](std::string tag)
                                                              {
                                                                  CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                                  if (packet) {
                                                                      CCHttpAgent::getInstance()->packets.erase(tag);
                                                                      delete packet;
                                                                  }
                                                                  
                                                                  if (image == NULL || image->getReferenceCount() <= 0 || image->getReferenceCount() > 10) return;
                                                                  
                                                                  if(Utils::hasImageFile(imageUrl))
                                                                  {
                                                                      image->loadTexture(Utils::getImageSavePath(imageUrl));
                                                                      
                                                                  }
                                                              });
                }
            }else{
//                image->setTouchEnabled(false);
//                image->loadTexture("Hall/pic_default2.png");
                continue;
            }
        }else{
            if (Utils::hasImageFile(imageUrl)) {
                image->loadTexture(Utils::getImageSavePath(imageUrl));
            } else {
                image->loadTexture("Hall/pic_default2.png");
                CCHttpAgent::getInstance()->sendHttpImage(imageUrl, Utils::getImageSavePath(imageUrl), Utils::getImageSavePath(imageUrl), [=](std::string tag)
                                                          {
                                                              CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                              if (packet) {
                                                                  CCHttpAgent::getInstance()->packets.erase(tag);
                                                                  delete packet;
                                                              }
                                                              
                                                              if (image == NULL || image->getReferenceCount() <= 0 || image->getReferenceCount() > 10) return;
                                                              
                                                              if(Utils::hasImageFile(imageUrl))
                                                              {
                                                                  image->loadTexture(Utils::getImageSavePath(imageUrl));
                                                                  
                                                              }
                                                          });
            }
        }
        
        
        
        
        image->addClickEventListener([=](Ref *ref)
                                     {
                                         ZJHModel::getInstance()->_adPayData = data[i];
                                         int type = data[i]["type"].asInt();
                                         if(type == 3){
//                                             callFun(2);
                                             PlatformHelper::showToast("暂未开放,敬请期待");
                                         }else if(type == 4){
                                             callFun(3);
                                         }
                                     });
        
        adList->insertPage(layout, i);
    }
    
    adList->scrollToPage(0);
    
    //添加viewpoint页面
    _widgetPoint = getLayViewPoint(adpageviewCount);
    _widgetPoint->setPosition(Vec2(winSize.width/2,35));
    adWidget->addChild(adList);
//    adWidget->addChild(_widgetPoint);
    
    adList->addEventListener(CC_CALLBACK_2(HallRecommend::pageViewEvent, this));
    return adWidget;
}

void HallRecommend::pageViewEvent(cocos2d::Ref *pSender, PageView::EventType type)
{
    switch (type) {
        case PageView::EventType::TURNING:
        {
            PageView *pageView = dynamic_cast<PageView *>(pSender);
            int index = (int)pageView->getCurPageIndex();
//            changePointView(index);
        }
            break;
            
        default:
            break;
    }
}

#pragma mark [ ActivityListItem ]

ActivityListItem *ActivityListItem::createWithData(Json::Value &info)
{
    ActivityListItem *pRet = new ActivityListItem();
    if (pRet && pRet->initWithData(info)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool ActivityListItem::initWithData(Json::Value &info)
{
    if (Widget::init() == false) {
        return false;
    }
    
    setTouchEnabled(true);
    
    std::string roomLogImage = info["roomLogImage"].asString();
    Size itemSize = Size(492, 397)*Utils::getScaleX();
    Size imageSize = Size(475, 310)*Utils::getScaleX();
    this->setContentSize(itemSize);
    
    ImageView* sp = ImageView::create("Hall/room.png");
    sp->setContentSize(itemSize);
    sp->setPosition(Vec2(itemSize.width / 2, itemSize.height));
    sp->setAnchorPoint(Vec2(0.5, 1.0));
    sp->ignoreContentAdaptWithSize(false);
    this->addChild(sp);
    
    //图片
    ImageView *image = ImageView::create();
    image->setContentSize(imageSize);
    image->setAnchorPoint(Vec2(0.5, 1.0));
    image->setPosition(Vec2(itemSize.width / 2, itemSize.height-8*Utils::getScaleX()));
    image->setTouchEnabled(true);
    image->ignoreContentAdaptWithSize(false);
    sp->addChild(image);
    
    if (Utils::hasImageFile(roomLogImage)) {
        image->loadTexture(Utils::getImageSavePath(roomLogImage));
    } else {
        image->loadTexture("Hall/pic_default2.png");
        CCHttpAgent::getInstance()->sendHttpImage(roomLogImage, Utils::getImageSavePath(roomLogImage), Utils::getImageSavePath(roomLogImage), [image, roomLogImage](std::string tag)
                                                  {
                                                      CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                      if (packet) {
                                                          CCHttpAgent::getInstance()->packets.erase(tag);
                                                          delete packet;
                                                      }
                                                      
                                                      if (image == NULL || image->getReferenceCount() <= 0 || image->getReferenceCount() > 10) return;
                                                      
                                                      if(Utils::hasImageFile(roomLogImage))
                                                      {
                                                          image->loadTexture(Utils::getImageSavePath(roomLogImage));
                                                      }
                                                  });
    }
    
    image->addClickEventListener([=](Ref *ref)
                                 {
//                                     char buff[128];
//                                     snprintf(buff, sizeof(buff), "click item : %d", info["relatedId"].asInt());
                                     PlatformHelper::showToast("暂未开放,敬请期待");
                                 });

    //房间
    Label* name = Label::createWithSystemFont(info["roomName"].asString(), "AmericanTypewriter", 35,Size(380,45));
    name->setAnchorPoint(Vec2(0, 0.5));
    Vec2 namescale = Vec2(20*Utils::getScaleX(), 40*Utils::getScaleX());
    name->setPosition(namescale);
    name->setColor(Color3B(152,145,122));
    sp->addChild(name);
    
    //游戏名
    Label* gameName = Label::createWithSystemFont(info["gameName"].asString(), "AmericanTypewriter", 35,Size(380,45));
    gameName->setAnchorPoint(Vec2(0, 0.5));
    gameName->setVisible(false);
    Vec2 _gameName = Vec2(20*Utils::getScaleX(), (sp->getContentSize().height-50)*Utils::getScaleX());
    gameName->setPosition(_gameName);
    gameName->setColor(Color3B::WHITE);
    sp->addChild(gameName);
    
    //玩家名字
    __String*  _userName = __String::createWithFormat("%s%s%s","[",info["user"]["nickName"].asCString(),"]");
    Label* userName = Label::createWithSystemFont(_userName->getCString(), "AmericanTypewriter", 35,Size(380,35));
    userName->setAnchorPoint(Vec2(0, 0.5));
    Vec2 usernamescale = Vec2(20*Utils::getScaleX(), 120*Utils::getScaleX());
    userName->setPosition(usernamescale);
    userName->setColor(Color3B::WHITE);
    sp->addChild(userName);
    
    //人数
    Text *number = Text::create(__String::createWithFormat("%d%s",info["onlineCount"].asInt(),"人")->getCString(), "AmericanTypewriter", 35);
    number->setVisible(false);
    number->setAnchorPoint(Vec2(0, 0.5));
    Vec2 numberscale = Vec2(itemSize.width - 80*Utils::getScaleX(), 40*Utils::getScaleX());
    number->setPosition(numberscale);
    number->setColor(Color3B(152,145,122));
    sp->addChild(number);
    
    
    return true;
}

#pragma mark [ HotListItem ]

HotListItem *HotListItem::createWithData(Json::Value &info)
{
    HotListItem *pRet = new HotListItem();
    if (pRet && pRet->initWithData(info)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool HotListItem::initWithData(Json::Value &info)
{
    if (Widget::init() == false) {
        return false;
    }
    
    setTouchEnabled(true);
    
    std::string imageName = info["roomLogImage"].asString();
    Size itemSize = Size(492, 397)* Utils::getScaleX();
    Size imageSize = Size(475, 310)* Utils::getScaleX();
    this->setContentSize(itemSize);
    
    
    ImageView* sp = ImageView::create("Hall/room.png");
    sp->setContentSize(itemSize);
    sp->ignoreContentAdaptWithSize(false);
    sp->setPosition(Vec2(itemSize.width / 2, itemSize.height));
    sp->setAnchorPoint(Vec2(0.5, 1.0));
    sp->setName("HotListItemImagebg");
    this->addChild(sp);
    
    //图片
    ImageView *image = ImageView::create();
    image->setContentSize(imageSize);
    image->setAnchorPoint(Vec2(0.5, 1.0));
    image->setPosition(Vec2(itemSize.width / 2, itemSize.height - 8));
    image->setTouchEnabled(true);
    image->ignoreContentAdaptWithSize(false);
    image->setName("HotListItemImage");
    sp->addChild(image);
    
    if (Utils::hasImageFile(imageName)) {
        image->loadTexture(Utils::getImageSavePath(imageName));
    } else {
        image->loadTexture("Hall/pic_default2.png");
        CCHttpAgent::getInstance()->sendHttpImage(imageName, Utils::getImageSavePath(imageName), Utils::getImageSavePath(imageName), [=](std::string tag)
                                                  {
                                                      if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
                                                      
                                                      CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                      if (packet) {
                                                          CCHttpAgent::getInstance()->packets.erase(tag);
                                                          delete packet;
                                                      }
                                                      
                                                      if (image == NULL || image->getReferenceCount() <= 0 || image->getReferenceCount() > 10) return;
                                                      
                                                      if(Utils::hasImageFile(imageName))
                                                      {
                                                          image->loadTexture(Utils::getImageSavePath(imageName));
                                                      }
                                                  });
    }
    
    image->addClickEventListener([=](Ref *ref)
                                 {
//                                     char buff[128];
//                                     snprintf(buff, sizeof(buff), "default click item : %d", info["id"].asInt());
                                     PlatformHelper::showToast("暂未开放,敬请期待");
                                 });
    
    //玩家信息
    std::string userImage = info["user"]["image"].asString();
    std::string userName = info["user"]["nickName"].asString();
    int userNumber =  info["onlineCount"].asInt();
    
    if (!userImage.empty()) {
        Sprite *userAvatar = Utils::createCircleAvatar(userImage, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(96,96));
        userAvatar->setPosition(Vec2(65* Utils::getScaleX(), 150* Utils::getScaleX()));
        sp->addChild(userAvatar);
    }
    
    Label* userText = Label::createWithSystemFont(userName, "AmericanTypewriter", 35,Size(300,90));
   //Text *userText = Text::create(userName, "AmericanTypewriter", 35);
    userText->setAnchorPoint(Vec2(0, 0.5));
    userText->setPosition(Vec2(135* Utils::getScaleX(), 130* Utils::getScaleX()));
    userText->setColor(Color3B::WHITE);
    sp->addChild(userText, 1);
    
    Text *userNum = Text::create(__String::createWithFormat("%d%s",userNumber,"人")->getCString(), "AmericanTypewriter", 35);
    userNum->setAnchorPoint(Vec2(0, 0.5));
    userNum->setPosition(Vec2(itemSize.width - 80* Utils::getScaleX(), 40* Utils::getScaleX()));
    userNum->setColor(Color3B(152,145,122));
    sp->addChild(userNum, 1);

    Label* roomName = Label::createWithSystemFont(info["roomName"].asString(), "AmericanTypewriter", 35,Size(380,40));
    roomName->setAnchorPoint(Vec2(0, 0.5));
    roomName->setPosition(Vec2(20* Utils::getScaleX(), 40* Utils::getScaleX()));
    roomName->setColor(Color3B(152,145,122));
    sp->addChild(roomName);
    
    Text *gameName = Text::create(info["gameName"].asString(), "AmericanTypewriter", 35);
    gameName->setAnchorPoint(Vec2(0, 0.5));
    gameName->setPosition(Vec2((roomName->getPositionX())* Utils::getScaleX(), (itemSize.height - 50)* Utils::getScaleX()));
    gameName->setColor(Color3B::WHITE);
    sp->addChild(gameName);
    
    return true;
}

ImageView *HotListItem::getImageView()
{
    return dynamic_cast<ImageView *>(this->getChildByName("HotListItemImagebg")->getChildByName("HotListItemImage"));
}

Layout* getLayoutItem(Size _size,Json::Value _json){
    
    Layout* lay = Layout::create();
    int _width = _size.width/4;
    lay->setContentSize(Size(_width,_size.height));
    lay->setAnchorPoint(Vec2(0,0));
    int idlay = _json["jumpType"].asInt();
    string names = _json["name"].asString();
    lay->setTag(idlay);
    lay->setName(names);
    //图片
    string imgaeurl = _json["logoUrl"].asString();
    
    if (!imgaeurl.empty()) {
        
        Sprite *userAvatar = Utils::createCircleAvatar(imgaeurl, "Hall/notification.png", "Hall/notification.png", Size(120,120));
        userAvatar->setAnchorPoint(Vec2(0.5, 0.5));
        userAvatar->setPosition(Vec2(lay->getContentSize().width/2, lay->getContentSize().height/2 +20));
        lay->addChild(userAvatar);
    }
    
    string tiptex = _json["name"].asString() ;
    //文字
    Text *name = Text::create(tiptex, "AmericanTypewriter", 40);
    name->setAnchorPoint(Vec2(0.5, 0.5));
    name->setTextColor(Color4B::WHITE);
    name->setPosition(Vec2(lay->getContentSize().width/2, lay->getContentSize().height/2 - 70));
    lay->addChild(name);
    
    return lay;
}

ListView* HallRecommend::getHorizeListView(Json::Value &data){
    
    ListView *listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    listView->setBounceEnabled(false);
    Size _size = Size(this->getContentSize().width,280);
    listView->setContentSize(_size);
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 0));
    listView->setScrollBarEnabled(false);
    listView->setGravity(ListView::Gravity::CENTER_VERTICAL);
 
    int number = data.size();
    for(int i = 0;i< number;i++){
        Layout* itemLay = getLayoutItem(_size,data[i]);
        itemLay->setTouchEnabled(true);
        itemLay->addClickEventListener([=](Ref* ref){
            Layout* _lay = dynamic_cast<Layout *>(ref);
            int tag = _lay->getTag();
            RichListView(tag);

        });
        listView->addChild(itemLay);
    }
    return listView;
}

void HallRecommend::RichListView(int sel){
    switch(sel){
        case 3:  //商城
            ZJHModel::getInstance()->gotoView(VIEW_WAN_SHOP);
            break;
        case 4:
            ZJHModel::getInstance()->gotoView(VIEW_RICH_LIST);
            break;
        case 1:
        {
             PlatformHelper::showToast("暂未开放");
        }
            break;
        case 2:
        {
            Size winSize = Director::getInstance()->getWinSize();
            TaskLayer* task = TaskLayer::create();
            task->setPosition(Vec2(0,-winSize.height));
            this->getParent()->getParent()->addChild(task, 100);
            task->runAction(MoveTo::create(0.5, Vec2(0,0)));
        }
            break;
        case 5:
        {
            PlatformHelper::showToast("暂未开放");
        }
            break;
        default:
            PlatformHelper::showToast("未知的类型");
            break;
    }
}
