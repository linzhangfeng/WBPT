//
//  Play.cpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/23.
//
//

#include "Play.hpp"

bool Play::init(){
    if(!BaseLayer::init()){
        return false;
    }
    
    setName("play");
    
    Size size = Director::getInstance()->getWinSize();
    
    setHead(270);
    
    setBackground("Play/bg.jpg");
    
    setTitle("玩呗");
    
    levelFlag = true;
    
    decount = 0;
    flag = false;
    initTopLay();
    initBottomLay();
    initListView();
    
    running = false;
    runningCount = 0;
    
    postMoney();
    RequestData();
    
     this->schedule(schedule_selector(Play::countScorll), 1.0f);
    return true;
}

void Play::countScorll(float ft){
    if(running){
        CalculationPosition();
    }
}


void Play::initTopLay(){
    Size size = Director::getInstance()->getWinSize();
    int dx = size.height - getHeadH() + 90;
    
    Sprite* userAvatar = Utils::createCircleAvatar(ZJHModel::getInstance()->portrit, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(144,144));
    userAvatar->setAnchorPoint(Vec2(0.5,0.5));
    userAvatar->setPosition(Vec2(120,dx));
    this->addChild(userAvatar,200);
    
    
    char buf[128];
    Utils::parseName(5, buf, ZJHModel::getInstance()->nick.c_str());
     Text* userName = Text::create("游客", "AmericanTypewriter", 45);
    userName->setString(buf);
    userName->setTextColor(Color4B::WHITE);
    userName->setAnchorPoint(Vec2(0,0.5));
    userName->setPosition(Vec2(220,dx));
    this->addChild(userName,200);
    
    Text* userMoney = Text::create("金币", "AmericanTypewriter", 40);
    userMoney->setTextColor(Color4B::WHITE);
    userMoney->setAnchorPoint(Vec2(0,0.5));
    userMoney->setPosition(Vec2(size.width/2 - 20,dx));
    this->addChild(userMoney,200);
    
    userMoneyNumber = Text::create("0", "AmericanTypewriter", 40);
    userMoneyNumber->setTextColor(Color4B(255,103,0,255));
    userMoneyNumber->setAnchorPoint(Vec2(0,0.5));
    userMoneyNumber->setPosition(Vec2(size.width/2 + 80,dx));
    this->addChild(userMoneyNumber,200);
    
    ImageView* _view = ImageView::create("Head/tab_bg.png");
    _view->setContentSize(Size(5,50));
    _view->setAnchorPoint(Vec2(0.5,0.5));
    _view->setPosition(Vec2(size.width/2 + 200,dx));
    _view->ignoreContentAdaptWithSize(false);
    this->addChild(_view,200);
    
    Text* userWanbi = Text::create("玩贝", "AmericanTypewriter", 40);
    userWanbi->setTextColor(Color4B::WHITE);
    userWanbi->setAnchorPoint(Vec2(0,0.5));
    userWanbi->setPosition(Vec2(size.width/2 + 250,dx));
    this->addChild(userWanbi,200);
    
    userWanbiNumber = Text::create("0", "AmericanTypewriter", 40);
    userWanbiNumber->setTextColor(Color4B(255,103,0,255));
    userWanbiNumber->setAnchorPoint(Vec2(0,0.5));
    userWanbiNumber->setPosition(Vec2(size.width/2 + 350,dx));
    this->addChild(userWanbiNumber,200);
}

void Play::RequestData(){

    Json::Value json;
    char buff[64];
    snprintf(buff, sizeof(buff), "quickStart");
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
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
                                                                 _jsonData = packet->recvVal["resultMap"]["quickStartList"];
                                                                 initListData(_jsonData);
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
                                             "game/classify/quickStart", json.toStyledString(), buff);
}

void Play::initBottomLay(){
    Size size = Director::getInstance()->getWinSize();
    
     _play = Button::create("Play/btn_b.png","Play/btn_b_2.png","Play/btn_b_3.png");
    _play->setEnabled(false);
    _play->setAnchorPoint(Vec2(0.5,0.5));
    _play->setPosition(Vec2(size.width/2,420));
    this->addChild(_play);
    _play->addClickEventListener([=](Ref* ref){
        PlatformHelper::showToast("暂未开放");
    });
    
    _froom = Button::create("Play/btn_c.png","Play/btn_c_check.png","Play/btn_c_check.png");
    _froom->setEnabled(false);
    _froom->setAnchorPoint(Vec2(0.5,0.5));
    _froom->setPosition(Vec2(160,420));
    this->addChild(_froom);
    _froom->addClickEventListener([=](Ref* ref){
        PlatformHelper::showToast("暂未开放");
    });
    
    _fon = Button::create("Play/C.png","Play/C2.png","Play/C3.png");
    _fon->setEnabled(false);
    _fon->setAnchorPoint(Vec2(0.5,0.5));
    _fon->setPosition(Vec2(size.width-160,420));
    this->addChild(_fon);
    _fon->addClickEventListener([=](Ref* ref){
        PlatformHelper::showToast("暂未开放");
    });
    
    Button* _close = Button::create("Play/btn_d.png","Play/btn_d_2.png","Play/btn_d_2.png");
    _close->setAnchorPoint(Vec2(0.5,0.5));
    _close->setPosition(Vec2(size.width/2,120));
    this->addChild(_close);
    _close->addClickEventListener([=](Ref* ref){
        unschedule(schedule_selector(Play::countScorll));
        this->disappear();
    });
    
}

void Play::initListView(){
    Size size = Director::getInstance()->getWinSize();
    
    imgBack = Button::create("Hall/icon_nav_04.png","Hall/icon_nav_04.png");
    imgBack->setAnchorPoint(Vec2(0.5,0.5));
    imgBack->setContentSize(Size(60,100));
    imgBack->setPosition(Vec2(100,size.height-400));
    imgBack->ignoreContentAdaptWithSize(false);
    this->addChild(imgBack,10000);
    imgBack->setVisible(false);
    imgBack->setTouchEnabled(false);
    imgBack->addClickEventListener([=](Ref *ref)
                               {
                                _play->setEnabled(false);
                                _froom->setEnabled(false);
                                _fon->setEnabled(false);
                                initListData(_jsonData);
                               });
    
    
    listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(size.width,1000));
    listView->setAnchorPoint(Vec2(0.5, 0.5));
    listView->setPosition(Vec2(size.width/2, size.height/2+100));
    listView->setScrollBarEnabled(false);
    listView->setItemsMargin(50);
    listView->setGravity(ListView::Gravity::CENTER_VERTICAL);
    this->addChild(listView);
    
    firstx = size.width/2-260;
    endx = size.width/2+260;
 
    listView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType){
        if(eventType == ScrollView::EventType::CONTAINER_MOVED){
            if(!flag){
                flag = true;
                if(decount > 1){
                    CalculationPosition();
                }
                flag = false;
                decount++;
            }
        }
    });
}


void Play::initListData(Json::Value _json){
    running = true;
    int number = _json.size();
    imgBack->setVisible(false);
    imgBack->setTouchEnabled(false);
    if(listView){
        if(listView->getChildrenCount() > 0){
            listView->removeAllChildren();
            listView->jumpToLeft();
        }
    }
    
    Widget *LMargin = Widget::create();
    LMargin->setAnchorPoint(Vec2(0,0));
    LMargin->setPosition(Vec2(0,0));
    LMargin->setContentSize(Size(202, 720));
    listView->addChild(LMargin);
    
    for(int i = 0;i< number;i++){
        Layout* itemLay = playItem::getItemLayout(i,[=](int index){
            
//            Layout* lays = (Layout*)listView->getItem(index+1);
//            ImageView* img = (ImageView*)lays->getChildByName("imageV");
//            Size size = Director::getInstance()->getWinSize();
//            Size _size = img->getContentSize();
//            int x = lays->getWorldPosition().x + _size.width/2;
//            if(x>size.width/2-80 && x<size.width/2+80){
//                initListSecData(_json[index]["gameTypeList"]);
//            }else{
//                if(x > size.width/2+80){
//                     listView->getInnerContainer()->setPosition(Vec2(-(lays->getPosition().x - 360),0));
//                    CalculationPosition();
//                }else{
//                    listView->getInnerContainer()->setPosition(Vec2(-(lays->getPosition().x - 260),0));
//                    CalculationPosition();
//                }
//               
//            }
            initListSecData(_json[index]["gameTypeList"]);
            
        },_json[i]);
        listView->addChild(itemLay);
    }
    
    Widget *RMargin = Widget::create();
    RMargin->setAnchorPoint(Vec2(0,0));
    RMargin->setPosition(Vec2(0,0));
    RMargin->setContentSize(Size(202, 720));
    listView->addChild(RMargin);

    listView->refreshView();
}

void Play::initListSecData(Json::Value _json){
    int numbers = _json.size();
    imgBack->setVisible(true);
    imgBack->setTouchEnabled(true);
    if(listView){
        if(listView->getChildrenCount() > 0){
            listView->removeAllChildren();
            listView->jumpToLeft();
        }
    }
    
    Widget *LMargin = Widget::create();
    LMargin->setContentSize(Size(202, 720));
    listView->addChild(LMargin);
    
    for(int i = 0;i< numbers;i++){
        Layout* itemLay = playSecItem::getItemLayout(i,_json[i],[=](int index){
            
//            Layout* lays = (Layout*)listView->getItem(index+1);
//            ImageView* img = (ImageView*)lays->getChildByName("imageV");
//            Size size = Director::getInstance()->getWinSize();
//            Size _size = img->getContentSize();
//            int x = lays->getWorldPosition().x + _size.width/2;
//            if(x>size.width/2-80 && x<size.width/2+80){
//                itemMCallBack(_json[index]);
//            }else{
//                listView->getInnerContainer()->setPosition(Vec2(-(lays->getPosition().x-_size.width/2),0));
//            }
            itemMCallBack(_json[index]);
            
        });
        listView->addChild(itemLay);
    }
    
    Widget *RMargin = Widget::create();
    RMargin->setContentSize(Size(202, 720));
    listView->addChild(RMargin);
    
    listView->refreshView();

}

void Play::itemMCallBack(Json::Value _json){
    if(!_json["quickStart"].asBool()){
        _play->setEnabled(true);
    }else{
        _play->setEnabled(false);
    }
    
    if(!_json["isDuiju"].asBool()){
        _fon->setEnabled(true);
    }else{
        _fon->setEnabled(false);
    }
    
    if(!_json["roomId"].isNull()){
        int roomid = _json["roomId"].asInt();
        if(roomid != 0){
            _froom->setEnabled(true);
        }else{
            _froom->setEnabled(false);
        }
    }
}


void Play::CalculationPosition(){

        Size size = Director::getInstance()->getWinSize();
        int count = (int)listView->getChildrenCount();
        for(int i = 1;i<count-1;i++){
            Layout* lays = (Layout*)listView->getItem(i);
            
            ImageView* imgV = (ImageView*)lays->getChildByName("image");
            ImageView* img = (ImageView*)lays->getChildByName("imageV");
            
            Size _size = img->getContentSize();
            int x = lays->getWorldPosition().x + _size.width/2;
            float scale = 0.0;
            if(x > firstx && x < endx){
                if(x > size.width/2){
//                    if(x <= size.width/2+130){
//                        scale = 1.2;
//                    }else if(x <= size.width/2+260){
//                        scale = 1.1;
//                    }
                    scale = 1+(260-(x-size.width/2))*0.3/260.0;
                }else if(x < size.width/2){
//                    if(x >= size.width/2-130){
//                        scale = 1.2;
//                    }else if(x >= size.width/2-260){
//                        scale = 1.1;
//                    }
                    
                    scale = 1+(260-(size.width/2-x))*0.3/260.0;
                }else{
                    scale= 1.3;
                }
                log("====%f====",scale);
                Size _msize = Size(520*scale,720*scale);
                Size _msizeV = Size(490*scale,680*scale);
                lays->setContentSize(_msize);
                img->setContentSize(_msizeV);
                img->setPosition(Vec2(15*scale,25*scale));
                imgV->setContentSize(_msize);
            }else{
                Size _msize = Size(520,720);
                Size _msizeV = Size(490,680);
                lays->setContentSize(_msize);
                img->setContentSize(_msizeV);
                img->setPosition(Vec2(15,25));
                imgV->setContentSize(_msize);
            }
        }
        listView->refreshView();
 
}


Layout* playItem::getItemLayout(int index,const MyCallBack_Int &callback,Json::Value _json){
    
    float scale = 1.0;
    if(index == 0){
        scale = 1.3;
    }
    
    Layout* lay = Layout::create();
    lay->setName("layname");
    lay->setAnchorPoint(Vec2(0,0));
    Size _size = Size(520*scale,720*scale);
    lay->setContentSize(_size);
    
    ImageView* view = ImageView::create("Play/play_itembg.png");
    view->setName("image");
    view->setTouchEnabled(true);
    view->ignoreContentAdaptWithSize(false);
    view->setAnchorPoint(Vec2(0,0));
    view->setContentSize(_size);
    view->setPosition(Vec2(0,0));
    
    ImageView* imageV = ImageView::create("Match/Open_BG.png");
    imageV->setName("imageV");
    imageV->ignoreContentAdaptWithSize(false);
    imageV->setAnchorPoint(Vec2(0,0));
    imageV->setContentSize(Size(_size.width-30*scale,_size.height-40*scale));
    imageV->setPosition(Vec2(15*scale,25*scale));
    string imageUrl = _json["bigLogo"].asString();
//    string imageUrl = "";
    if (Utils::hasImageFile(imageUrl)) {
        imageV->loadTexture(Utils::getImageSavePath(imageUrl));
    } else {
        imageV->loadTexture("Hall/pic_default2.png");
        CCHttpAgent::getInstance()->sendHttpImage(imageUrl, Utils::getImageSavePath(imageUrl), Utils::getImageSavePath(imageUrl), [=](std::string tag)
                                                  {
                                                      CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                      if (packet) {
                                                          CCHttpAgent::getInstance()->packets.erase(tag);
                                                          delete packet;
                                                      }
                                                      
                                                      if (imageV == NULL || imageV->getReferenceCount() <= 0 || imageV->getReferenceCount() > 10) return;
                                                      
                                                      if(Utils::hasImageFile(imageUrl))
                                                      {
                                                          imageV->loadTexture(Utils::getImageSavePath(imageUrl));
                                                          
                                                      }
                                                  });
    }
    
    view->addClickEventListener([=](Ref* ref){
        callback(index);
    });
    lay->addChild(imageV);
    lay->addChild(view);
    return lay;
}

Layout* playSecItem::getItemLayout(int index,Json::Value _json,const MyCallBack_Int &callback){
    
    float scale = 1.0;
    if(index == 0){
        scale = 1.3;
    }
    
    Layout* lay = Layout::create();
    lay->setName("layname");
    lay->setAnchorPoint(Vec2(0,0));
    Size _size = Size(520*scale,720*scale);
    lay->setContentSize(_size);
    
    ImageView* view = ImageView::create("Play/play_itembg.png");
    view->setName("image");
    view->setTouchEnabled(true);
    view->ignoreContentAdaptWithSize(false);
    view->setAnchorPoint(Vec2(0,0));
    view->setContentSize(_size);
    view->setPosition(Vec2(0,0));
    
    ImageView* imageV = ImageView::create("Match/Open_BG.png");
    imageV->setName("imageV");
    imageV->ignoreContentAdaptWithSize(false);
    imageV->setAnchorPoint(Vec2(0,0));
    imageV->setContentSize(Size(_size.width-30*scale,_size.height-40*scale));
    imageV->setPosition(Vec2(15*scale,25*scale));
    string imageUrl = _json["logUrl"].asString();
    if (Utils::hasImageFile(imageUrl)) {
        imageV->loadTexture(Utils::getImageSavePath(imageUrl));
    } else {
        imageV->loadTexture("Hall/pic_default2.png");
        CCHttpAgent::getInstance()->sendHttpImage(imageUrl, Utils::getImageSavePath(imageUrl), Utils::getImageSavePath(imageUrl), [=](std::string tag)
                                                  {
                                                      CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                      if (packet) {
                                                          CCHttpAgent::getInstance()->packets.erase(tag);
                                                          delete packet;
                                                      }
                                                      
                                                      if (imageV == NULL || imageV->getReferenceCount() <= 0 || imageV->getReferenceCount() > 10) return;
                                                      
                                                      if(Utils::hasImageFile(imageUrl))
                                                      {
                                                          imageV->loadTexture(Utils::getImageSavePath(imageUrl));
                                                          
                                                      }
                                                  });
    }
    
    view->addClickEventListener([=](Ref* ref){
        if(callback){
            callback(index);
        }
    });
    lay->addChild(imageV);
    lay->addChild(view);
    return lay;
}


void Play::postMoney()
{
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
            Json::Value data = loginPacket->recvVal["resultMap"]["play"];
            log("钱信息：＝＝＝ [%s]",data.toStyledString().c_str());
            //刷新玩家金钱数据
            
            int gold = data["goldenNum"].asInt();
            
            userMoneyNumber->setString(ZJHModel::getInstance()->moneyShowDeal(gold));
            
            
            int wan = data["wanbei"].asInt();
            userWanbiNumber->setString(ZJHModel::getInstance()->moneyShowDeal(wan));
        
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
        
    },"golden/userWealth","","wealth");
    
}

