//
//  RoomInformationLayer.cpp
//  ZJH
//
//  Created by hyhy on 16/7/26.
//
//

#include "RoomInformationLayer.h"
#include "RoomShareLayer.h"

static const float scaleRate = 0.67;

void RoomInformationLayer::show()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(Vec2(0, -winSize.height));
    this->runAction(MoveTo::create(0.17, Vec2(0, 0)));
    this->setVisible(true);
}

void RoomInformationLayer::hide()
{
    this->setVisible(false);
    this->removeFromParentAndCleanup(true);
}

bool RoomInformationLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(RoomInformationLayer::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void RoomInformationLayer::myInit(Json::Value &data)
{
    Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = Size(winSize.width, 780);
    
    LayerColor *contentBg = LayerColor::create(Color4B(32, 32, 32, 255));
    contentBg->setContentSize(contentSize);
    contentBg->setPosition(Vec2(0, 0));
    contentBg->setName("content_bg");
    addChild(contentBg);
    
    LayerColor *titleBg = LayerColor::create(Color4B(32, 32, 32, 255));
    titleBg->setContentSize(Size(winSize.width, 111 * scaleRate));
    titleBg->setPosition(Vec2(0, contentBg->getPositionY() + contentBg->getContentSize().height));
    titleBg->setName("title_bg");
    this->addChild(titleBg);
    
    LayerColor *line = LayerColor::create(Color4B(0, 0, 0, 255));
    line->setContentSize(Size(winSize.width, 2));
    line->setPosition(titleBg->getPosition());
    this->addChild(line);
    
    Text* titlelabel = Text::create("房间信息", "AmericanTypewriter", 50 * scaleRate);
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
    
    //内容
    Vec2 avatarPos = Vec2(90, contentSize.height - 90);

    std::string userImage = data["gameRoom"]["user"]["image"].asString();
    Sprite *userAvatar = Utils::createCircleAvatar(userImage, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(90,90));
    userAvatar->setPosition(avatarPos);
    this->addChild(userAvatar);
    
    std::string nickName = data["gameRoom"]["user"]["nickName"].asString();
    std::string roomName = data["gameRoom"]["roomName"].asString();
    std::string code = data["gameRoom"]["code"].asString();
    std::string roomType = data["way"]["name"].asString();
    char buff[258];
    
    Utils::parseName(6, buff, nickName.c_str());
    Text *userName = Text::create(buff, "AmericanTypewriter", 32);
    userName->setAnchorPoint(Vec2(0, 0.5));
    userName->setPosition(Vec2(195, contentSize.height - 46));
    contentBg->addChild(userName);
    
    Text *roomNameTitle = Text::create("房名：", "AmericanTypewriter", 26);
    roomNameTitle->setAnchorPoint(Vec2(0, 0.5));
    roomNameTitle->setPosition(Vec2(userName->getPositionX(), contentSize.height - 86));
    roomNameTitle->setColor(Color3B(100, 100, 100));
    contentBg->addChild(roomNameTitle);
    
    Utils::parseName(6, buff, roomName.c_str());
    Text *roomNameLabel = Text::create(buff, "AmericanTypewriter", 26);
    roomNameLabel->setAnchorPoint(Vec2(0, 0.5));
    roomNameLabel->setPosition(Vec2(roomNameTitle->getPositionX() + roomNameTitle->getContentSize().width, roomNameTitle->getPositionY()));
    roomNameLabel->setColor(Color3B(118, 186, 0));
    contentBg->addChild(roomNameLabel);
    
    snprintf(buff, sizeof(buff), "房间号：%s", code.c_str());
    Text *codeText = Text::create(buff, "AmericanTypewriter", 26);
    codeText->setAnchorPoint(Vec2(0, 0.5));
    codeText->setPosition(Vec2(userName->getPositionX(), contentSize.height - 126));
    codeText->setColor(Color3B(100, 100, 100));
    contentBg->addChild(codeText);
    
    snprintf(buff, sizeof(buff), "类型：%s", roomType.c_str());
    Text *typeText = Text::create(buff, "AmericanTypewriter", 26);
    typeText->setAnchorPoint(Vec2(0, 0.5));
    typeText->setPosition(Vec2(userName->getPositionX(), contentSize.height - 166));
    typeText->setColor(Color3B(100, 100, 100));
    contentBg->addChild(typeText);
    
    //人数
    Sprite *numSp = Sprite::create("Public/game_db_detail_status_bg.9.png");
    numSp->setPosition(Vec2(winSize.width - 150, contentSize.height - 70));
    contentBg->addChild(numSp);
    
    Text *numTitle = Text::create("人数", "AmericanTypewriter", 24);
    numTitle->setColor(Color3B(100, 100, 100));
    numTitle->setPosition(Vec2(numSp->getContentSize().width / 2, numSp->getContentSize().height / 2 + 15));
    numSp->addChild(numTitle);
    
    int onlineCount = data["gameRoom"]["onlineCount"].asInt();
    int againstNumber = data["gameRoom"]["againstNumber"].asInt();
    snprintf(buff, sizeof(buff), "%d/%d人", onlineCount, againstNumber);
    Text *numText = Text::create(buff, "AmericanTypewriter", 24);
    numText->setColor(Color3B(100, 100, 100));
    numText->setPosition(Vec2(numSp->getContentSize().width / 2, numSp->getContentSize().height / 2 - 15));
    numSp->addChild(numText);
    
    //分享
    Button *shareBtn = Button::create("Public/game_db_detail_status_bg.9.png");
    shareBtn->setZoomScale(0);
    shareBtn->setPosition(Vec2(winSize.width - 150, contentSize.height - 160));
    shareBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        RoomShareLayer *shareLayer = RoomShareLayer::create();
                                        this->addChild(shareLayer, 100);
                                        shareLayer->show();
                                    });
    contentBg->addChild(shareBtn);
    
    Sprite *shareIcon = Sprite::create("Public/game_roomdetail_btn_share.png");
    shareIcon->setPosition(Vec2(70, shareBtn->getContentSize().height / 2));
    shareBtn->addChild(shareIcon);
    
    Text *shareText = Text::create("分享", "AmericanTypewriter", 24);
    shareText->setColor(Color3B(100, 100, 100));
    shareText->setPosition(Vec2(120, shareBtn->getContentSize().height / 2));
    shareBtn->addChild(shareText);
    
    //娱乐类型
    LayerColor *typeBg = LayerColor::create(Color4B(38, 38, 38, 255));
    typeBg->setContentSize(Size(winSize.width, 100));
    typeBg->ignoreAnchorPointForPosition(false);
    typeBg->setAnchorPoint(Vec2(0, 1));
    typeBg->setPosition(Vec2(0, contentSize.height - 224));
    contentBg->addChild(typeBg);
    
    Text *typeTitle = Text::create("娱乐类型", "AmericanTypewriter", 28);
    typeTitle->setAnchorPoint(Vec2(0, 0.5));
    typeTitle->setPosition(Vec2(40, typeBg->getContentSize().height / 2));
    typeBg->addChild(typeTitle);
    
    std::string gameName = data["gameRoom"]["gameName"].asString();
    int bottomNote = data["gameRoom"]["bottomNote"].asInt();
    int takeIn = data["gameRoom"]["takeIn"].asInt();
    snprintf(buff, sizeof(buff), "%s(底注%d初始带入%d)", gameName.c_str(), bottomNote, takeIn);
    Text *typeLabel = Text::create(buff, "AmericanTypewriter", 28);
    typeLabel->setAnchorPoint(Vec2(1, 0.5));
    typeLabel->setPosition(Vec2(winSize.width - 40, typeTitle->getPositionY()));
    typeLabel->setColor(Color3B(100, 100, 100));
    typeBg->addChild(typeLabel);
    
    //对局时长
    LayerColor *durationBg = LayerColor::create(Color4B(38, 38, 38, 255));
    durationBg->setContentSize(Size(winSize.width, 100));
    durationBg->ignoreAnchorPointForPosition(false);
    durationBg->setAnchorPoint(Vec2(0, 1));
    durationBg->setPosition(Vec2(0, contentSize.height - 390));
    contentBg->addChild(durationBg);
    
//    typeTitle = Text::create("对战时长", "AmericanTypewriter", 28);
//    typeTitle->setAnchorPoint(Vec2(0, 0.5));
//    typeTitle->setPosition(Vec2(40, durationBg->getContentSize().height / 2));
//    durationBg->addChild(typeTitle);
//    
//    int durationTime = data["gameRoom"]["duration"].asInt();
//    snprintf(buff, sizeof(buff), "%d分钟", durationTime);
//    Text *durationLabel = Text::create(buff, "AmericanTypewriter", 28);
//    durationLabel->setAnchorPoint(Vec2(1, 0.5));
//    durationLabel->setPosition(Vec2(winSize.width - 40, typeTitle->getPositionY()));
//    durationLabel->setColor(Color3B(100, 100, 100));
//    durationBg->addChild(durationLabel);
}

bool RoomInformationLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
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