//
//  ClubCreateRoomDialog.cpp
//  ZJH
//
//  Created by mac on 2017/4/26.
//
//

#include "ClubCreateRoomDialog.hpp"
#include "../../../MyGUI/ImageByUrl.h"

void ClubCreateRoomDialog::initView(int clubId)
{
    Json::Value jsonDate;
    jsonDate["clubId"] = clubId;
    //    jsonDate["groupId"] = 1;
    
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
            Json::Value data = loginPacket->recvVal["resultMap"]["list"];//payRoomCardType  1,群主支付2，开房者支付',
            showView(data);
        }
    },"club/clubGamePlay/clubPlayList",jsonDate.toStyledString(),"clubPlayList");

}



void ClubCreateRoomDialog::showView(Json::Value data)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout *bgLayout = Layout::create();
    bgLayout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    bgLayout->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    bgLayout->setBackGroundColorOpacity(150);
    bgLayout->setTouchEnabled(true);
    bgLayout->setSwallowTouches(true);
    bgLayout->setContentSize(visibleSize);
    bgLayout->addClickEventListener([=](Ref*ref){
        this->removeFromParent();
        
    });
    
    this->addChild(bgLayout);
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(visibleSize.width - 120, 1232));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    bgLayout->addChild(bg);
    
    float H = bg->getContentSize().height;
    float bgW = bg->getContentSize().width;
    
    Button *cancel = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    cancel->setPosition(Vec2(bgW, H));
    cancel->addClickEventListener([=](Ref*ref){
        this->removeFromParent();
    });
    bg->addChild(cancel);
    
    auto titleCard = Text::create("扣房卡:扣群主房卡/扣开房者房卡", "", 44);
    titleCard->setColor(Color3B(0xf0,0xcb,0x5b));
    titleCard->setAnchorPoint(Vec2(0,1));
    titleCard->setPosition(Vec2(75,H - 60));
    bg->addChild(titleCard);
    
    auto titleHome = Text::create("群房间:群房非俱乐部成员不可加入", "", 44);
    titleHome->setColor(Color3B(0xff,0xff,0xff));
    titleHome->setAnchorPoint(Vec2(0,1));
    titleHome->setPosition(Vec2(75,H - 125));
    bg->addChild(titleHome);
    
    gameList = ListView::create();
    gameList->setDirection(ui::ScrollView::Direction::VERTICAL);
    gameList->setBounceEnabled(false);
    gameList->setContentSize(Size(bgW - 25 * 2, 1010));
    gameList->setAnchorPoint(Vec2(0, 0));
    gameList->setPosition(Vec2(25, 0));
    gameList->setScrollBarEnabled(true);
    gameList->setItemsMargin(20);
    
    bg->addChild(gameList);
    
    for (int i = 0; i < data.size(); i++) {
        Game_Item* item = Game_Item::create();
        item->initView(data[i]);
        item->setPosition(Vec2(0,0));
        item->setAnchorPoint(Vec2(0,0));
        gameList->addChild(item);
    }
    
}



bool Game_Item::init()
{
    if (!Layout::init())
    {
        return false;
    }
    return true;
}

void Game_Item::initView(Json::Value data)
{
    ImageView* bg = ImageView::create("Msg/trible_btn.png");
    bg->setScale9Enabled(true);
    bg->ignoreContentAdaptWithSize(false);
    bg->setContentSize(Size(900,250));
    bg->setAnchorPoint(Vec2(0,0));
    bg->setPosition(Vec2(0,0));
    this->addChild(bg);
    
    this->setContentSize(bg->getContentSize());
    
    ImageByUrl* icon = ImageByUrl::create();
    icon->load(180, 180, data["gameLog"].asString());
    icon->setAnchorPoint(Vec2(0.5,0.5));
    icon->setPosition(Vec2(110,this->getContentSize().height / 2));
    bg->addChild(icon);
    
    char buf[500];
    Utils::parseName(6, buf, data["playName"].asString().c_str());
    string gameNameStr = __String::createWithFormat("%s(%s)",data["gameName"].asString().c_str(),buf)->getCString();
    auto gameName = Text::create(gameNameStr, "", 44);
    gameName->setColor(Color3B(0xf0,0xcb,0x5b));
    gameName->setAnchorPoint(Vec2(0,0));
    gameName->setPosition(Vec2(228,145));
    bg->addChild(gameName);

    string jushuStr = __String::createWithFormat("局数:%d局",data["gameCount"].asInt())->getCString();
    
    auto gameCount = Text::create(jushuStr, "", 44);
    gameCount->setColor(Color3B(0xa4,0xa4,0xa4));
    gameCount->setAnchorPoint(Vec2(0,0));
    gameCount->setPosition(Vec2(228,80));
    bg->addChild(gameCount);

    int cardType = data["payRoomCardType"].asInt(); //        1,群主支付2，开房者支付'
    string format = "";
    if(cardType == 1){
         format = "(群主支付)";
    }else{
        int cardNo = data["cardNum"].asInt();
        if(cardNo == 0){
           format = "(免房卡)";
        }else{
           format = __String::createWithFormat("(%d房卡)",data["cardNum"].asInt())->getCString();
        }
    }
    
    auto gameCard = Text::create(format, "", 44);
    gameCard->setColor(Color3B(0xe3,0xd0,0x63));
    gameCard->setAnchorPoint(Vec2(0,0));
    gameCard->setPosition(Vec2(gameCount->getPositionX()+gameCount->getContentSize().width + 10,80));
    bg->addChild(gameCard);
    
    Button* createRoom = Button::create("Msg/create_room.png","Msg/create_room_press.png");
    createRoom->setScale9Enabled(true);
    createRoom->ignoreContentAdaptWithSize(false);
    createRoom->setContentSize(Size(210,bg->getContentSize().height));
    createRoom->setAnchorPoint(Vec2(1,0));
    createRoom->setPosition(Vec2(bg->getContentSize().width,0));
    createRoom->addClickEventListener([=](Ref *ref)
                                      {
                                          this->createRoom(data["clubId"].asInt(),data["clubPlayId"].asInt(),data["gameId"].asInt());
                                      });
    bg->addChild(createRoom);
    
    auto createText = Text::create("开房", "", 52);
    createText->setColor(Color3B(0x6c,0x46,0x03));
    createText->setAnchorPoint(Vec2(0.5,0.5));
    createText->setPosition(Vec2(createRoom->getContentSize().width / 2,createRoom->getContentSize().height / 2));
    createRoom->addChild(createText);
}

//创建俱乐部房间
void Game_Item::createRoom(int clubId, int clubPlayId,int gameId){
    Json::Value jsonDate;
    jsonDate["clubId"] = clubId;
    jsonDate["clubPlayId"] = clubPlayId;
    jsonDate["gameId"] = gameId;
    
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
            PlatformHelper::showToast("创建成功");
        }
    },"club/clubRoom/clubOpenRoom",jsonDate.toStyledString(),"clubOpenRoom");
}
