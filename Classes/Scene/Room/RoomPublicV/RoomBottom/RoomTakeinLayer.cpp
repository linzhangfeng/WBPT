//
//  RoomTakeinLayer.cpp
//  ZJH
//
//  Created by hyhy on 16/7/26.
//
//

#include "RoomTakeinLayer.h"

void RoomTakeinLayer::show()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(Vec2(0, -winSize.height));
    this->runAction(MoveTo::create(0.17, Vec2(0, 0)));
    this->setVisible(true);
}

void RoomTakeinLayer::hide()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->runAction(Sequence::create(MoveTo::create(0.17, Vec2(0, -winSize.height)),
                                     RemoveSelf::create(),
                                     NULL));
}

bool RoomTakeinLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomTakeinLayer::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = Size(winSize.width, 790);
    
    LayerColor *contentBg = LayerColor::create(Color4B(32, 32, 32, 255));
    contentBg->setContentSize(contentSize);
    contentBg->setPosition(Vec2(0, 0));
    contentBg->setName("content_bg");
    this->addChild(contentBg);
    
    LayerColor *titleBg = LayerColor::create(Color4B(32, 32, 32, 255));
    titleBg->setContentSize(Size(winSize.width, 75));
    titleBg->setPosition(Vec2(0, contentBg->getPositionY() + contentBg->getContentSize().height));
    titleBg->setName("title_bg");
    this->addChild(titleBg);
    
    LayerColor *line = LayerColor::create(Color4B(0, 0, 0, 255));
    line->setContentSize(Size(winSize.width, 2));
    line->setPosition(titleBg->getPosition());
    this->addChild(line);
    
    Text* titlelabel = Text::create("总带入", "AmericanTypewriter", 36);
    titlelabel->setPosition(titleBg->getContentSize() / 2);
    titleBg->addChild(titlelabel);
    
    Button *closeBtn = Button::create("Public/icon_cancel.png");
    closeBtn->setZoomScale(0);
    closeBtn->setPosition(Vec2(winSize.width - 45, titleBg->getContentSize().height / 2));
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->hide();
                                    });
    titleBg->addChild(closeBtn);
    
    return true;
}

void RoomTakeinLayer::myInit(int rid)
{
    Json::Value json;
    json["roomId"] = rid;
    json["UserKey"] = ZJHModel::getInstance()->UserKey;
    
    CCHttpAgent::getInstance()->sendHttpPost(NULL,
                                             "duiju/integral/takeIn", json.toStyledString(), "room_takein");
    this->schedule(schedule_selector(RoomTakeinLayer::getData), 0.0f);
}

void RoomTakeinLayer::getData(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["room_takein"];
    if (loginPacket && loginPacket->status != 0)
    {
        unschedule(schedule_selector(RoomTakeinLayer::getData));
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        else
        {
            if (loginPacket->resultIsOK())
            {
                initListView(loginPacket->recvVal["resultMap"]);
            }
            else
            {
                
            }
        }
        loginPacket->status = 0;
        CCHttpAgent::getInstance()->packets["room_takein"] = NULL;
        delete loginPacket;
    }
}

void RoomTakeinLayer::initListView(Json::Value &data)
{
    Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = Size(winSize.width, 790);
    
    ListView *resultList = ListView::create();
    resultList->setDirection(ScrollView::Direction::VERTICAL);
    resultList->setBounceEnabled(false);
    resultList->setContentSize(contentSize);
    resultList->setAnchorPoint(Vec2(0, 0));
    resultList->setPosition(Vec2(0, 0));
    resultList->setItemsMargin(0);
    resultList->setScrollBarEnabled(false);
    getChildByName("content_bg")->addChild(resultList);
    
    float itemHeight = 100;
    char buff[128];
    Color3B numColor = Color3B(254, 232, 153);
    
    //我的带入
    {
        Layout *myResult = Layout::create();
        myResult->setContentSize(Size(contentSize.width, itemHeight));
        
        LayerColor *myBg = LayerColor::create(Color4B(38, 38, 38, 255));
        myBg->setContentSize(myResult->getContentSize());
        myBg->setPosition(Vec2(0, 0));
        myResult->addChild(myBg);
        
        Text *myTitle = Text::create("我的带入", "AmericanTypewriter", 32);
        myTitle->setColor(numColor);
        myTitle->setPosition(Vec2(90, itemHeight * 0.5));
        myResult->addChild(myTitle);
        
        snprintf(buff, sizeof(buff), "%d×%d", data["myTakeIn"].asInt(), data["times"].asInt());
        Text *myNum = Text::create(buff, "AmericanTypewriter", 32);
        myNum->setColor(numColor);
        myNum->setAnchorPoint(Vec2(1, 0.5));
        myNum->setPosition(Vec2(contentSize.width - 40, itemHeight * 0.5));
        myResult->addChild(myNum);
        
        resultList->addChild(myResult);
    }
    
    for (int i = 0; i < data["takeInRank"].size(); i++) {
        Layout *layout = Layout::create();
        layout->setContentSize(Size(contentSize.width, itemHeight));
        
        if (i%2) {
            LayerColor *bg = LayerColor::create(Color4B(38, 38, 38, 255));
            bg->setContentSize(layout->getContentSize());
            bg->setPosition(Vec2(0, 0));
            layout->addChild(bg);
        }
        
        int userId = data["takeInRank"][i]["userId"].asInt();
        int takein = data["takeInRank"][i]["takeIn"].asInt();
        int takeInTimes = data["takeInRank"][i]["times"].asInt();
        std::string userName = data["takeInRank"][i]["userName"].asString();
        std::string image = data["takeInRank"][i]["image"].asString();
        
        Sprite *userAvatar = image == "" ? NULL : Sprite::create(Utils::getImageSavePath(image));
        if (userAvatar == NULL) {
            userAvatar = Sprite::create("Avatars/user4_unlogin.png");
        }
        userAvatar = Utils::CirCleSprite(userAvatar, "Avatars/StencilBg.png");
        userAvatar->getTexture()->setAntiAliasTexParameters();
        userAvatar->setPosition(Vec2(60, itemHeight / 2));
        userAvatar->setScale(0.8);
        layout->addChild(userAvatar);
        
        Text *name = Text::create(userName, "AmericanTypewriter", 28);
        name->setAnchorPoint(Vec2(0, 0.5));
        name->setPosition(Vec2(120, 70));
        layout->addChild(name);
        
        snprintf(buff, sizeof(buff), "(ID:%d)", userId);
        Text *identify = Text::create(buff, "AmericanTypewriter", 28);
        identify->setAnchorPoint(Vec2(0, 0.5));
        identify->setPosition(Vec2(name->getPositionX(), 30));
        layout->addChild(identify);
        
        snprintf(buff, sizeof(buff), "%d×%d", takein, takeInTimes);
        Text *numText = Text::create(buff, "AmericanTypewriter", 30);
        numText->setColor(numColor);
        numText->setAnchorPoint(Vec2(1, 0.5));
        numText->setPosition(Vec2(contentSize.width - 40, itemHeight * 0.5));
        layout->addChild(numText);
        
        resultList->addChild(layout);
    }
    
    resultList->refreshView();
}

bool RoomTakeinLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    LayerColor *contentBg = dynamic_cast<LayerColor *>(this->getChildByName("content_bg"));
    LayerColor *titleBg = dynamic_cast<LayerColor *>(this->getChildByName("title_bg"));
    if (contentBg->boundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())) || titleBg->boundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())))
    {
        //
    }
    else
    {
        this->hide();
        event->stopPropagation();
    }
    return true;
}