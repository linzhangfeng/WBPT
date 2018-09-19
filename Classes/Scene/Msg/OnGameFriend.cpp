//
//  OnGameFriend.cpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/29.
//
//

#include "OnGameFriend.h"
bool OnGameFriend::init(){
    if(!Layout::init()){
        return false;
    }
    
    setName("OnGameFriend");
    Size size = Director::getInstance()->getWinSize();

    setContentSize(size);
    setTouchEnabled(true);
    
    setHead(150);
    setBackground(Color4B(20, 20, 22, 255));
    
    Text* Texttitle = Text::create("对战", "AmericanTypewriter", 55);
    Texttitle->setAnchorPoint(Vec2(0.5,0.5));
    Texttitle->setPosition(Vec2(size.width/2,size.height-75));
    Texttitle->setColor(Color3B(152,145,122));
    this->addChild(Texttitle,1000);
    
    Button* btn_close = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    btn_close->setAnchorPoint(Vec2(0.5,0.5));
    btn_close->setPosition(Vec2(size.width-100,size.height-75));
    btn_close->setContentSize(Size(70,70));
    btn_close->ignoreContentAdaptWithSize(false);
    btn_close->addClickEventListener([=](Ref* ref){
        this->removeFromParentAndCleanup(true);
    });
    this->addChild(btn_close,1000);
    initView();
    return true;
}


void OnGameFriend::setHead(int h)
{
    Size size = Director::getInstance()->getWinSize();
    
    LayerColor* bg = LayerColor::create(Color4B(14, 14, 15, 255));
    bg->setContentSize(Size(size.width,h));
    bg->setAnchorPoint(Vec2(0.0f, 0.0f));
    bg->setPosition(0,size.height - h);
    addChild(bg, 100);
}

Layout* OnGameFriend::setBackground(Color4B color)
{
    Size size = Director::getInstance()->getWinSize();
    LayerColor* bg = LayerColor::create(color);
    bg->setContentSize(size);
    bg->setAnchorPoint(Vec2(0.0f, 0.0f));
    bg->setPosition(0,0);
    addChild(bg, -100);
    return this;
}

void OnGameFriend::initView(){
    
    Size size = Director::getInstance()->getWinSize();
    listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(size.width,size.height-150));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 0));
    listView->setScrollBarEnabled(true);
    listView->setItemsMargin(0);
    addChild(listView);
}

void OnGameFriend::addListViewData(Json::Value _value){
    int num = _value.size();
    for(int i =0;i<num;i++){
        Layout* item = OnGameFriend_Item::initData(_value[i]);
        listView->addChild(item);
    }
}


Layout* OnGameFriend_Item::initData(Json::Value _value){
    Size size = Director::getInstance()->getWinSize();
    Layout* lay = Layout::create();
    lay->setContentSize(Size(size.width,200));
    lay->setAnchorPoint(Vec2(0,0));
    
    ImageView *onGameImage = ImageView::create();
    onGameImage->setContentSize(Size(150,150));
    onGameImage->setTouchEnabled(false);
    onGameImage->ignoreContentAdaptWithSize(false);
    onGameImage->setAnchorPoint(Vec2(0.5,0.5));
    onGameImage->setPosition(Vec2(120, 100));
    lay->addChild(onGameImage);
    
    string logUrl = _value["gameUrl"].asString();
    if (Utils::hasImageFile(logUrl)) {
        onGameImage->loadTexture(Utils::getImageSavePath(logUrl));
    } else {
        onGameImage->loadTexture("Hall/notification.png");
        CCHttpAgent::getInstance()->sendHttpImage(logUrl, Utils::getImageSavePath(logUrl), Utils::getImageSavePath(logUrl), [onGameImage, logUrl](std::string tag)
                                                  {
                                                      CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                      if (packet) {
                                                          CCHttpAgent::getInstance()->packets.erase(tag);
                                                          delete packet;
                                                      }
                                                      
                                                      if (onGameImage == NULL || onGameImage->getReferenceCount() <= 0 || onGameImage->getReferenceCount() > 10) return;
                                                      
                                                      if(Utils::hasImageFile(logUrl))
                                                      {
                                                              onGameImage->loadTexture(Utils::getImageSavePath(logUrl));                                                    
                                                      }
                                                  });
    }
    
    string gameN = _value["gameName"].asString();
    Text* gameName = Text::create(gameN, "AmericanTypewriter", 45);
    gameName->setColor(Color3B(208, 208, 208));
    gameName->setAnchorPoint(Vec2(0, 0.5));
    gameName->setPosition(Vec2(220,100));
    gameName->setColor(Color3B::YELLOW);
    lay->addChild(gameName);
    
    int oNum = _value["total"].asInt();
    Text* text = Text::create(__String::createWithFormat("%d%s",oNum,"场")->getCString(), "AmericanTypewriter", 45);
    text->setColor(Color3B(208, 208, 208));
    text->setAnchorPoint(Vec2(0.5, 0.5));
    text->setPosition(Vec2(size.width-200,100));
    text->setColor(Color3B::YELLOW);
    lay->addChild(text);

    return lay;
}


void OnGameFriend::requestMyOnGame(int friendId){
    
    Json::Value json;

    json["friendId"] = friendId;
    
    char buff[64];
    snprintf(buff, sizeof(buff), "integral_%s", "friendDuijus");
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
                                                                 Json::Value serValue = packet->recvVal["resultMap"]["playGames"];
                                                                 addListViewData(serValue);
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
                                             "duiju/integral/friendDuijus", json.toStyledString(), buff);
}
