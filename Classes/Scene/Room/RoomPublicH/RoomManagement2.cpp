//
//  RoomManagement2.cpp
//  ZJH
//
//  Created by pg on 16/9/8.
//
//

#include "RoomManagement2.hpp"

static const float scaleRate = 0.67;

void RoomManagement2::show()
{
//    Size winSize = Director::getInstance()->getWinSize();
//    this->setPosition(Vec2(0, -winSize.height));
//    this->runAction(MoveTo::create(0.17, Vec2(0, 0)));
//    this->setVisible(true);
    stopAllActions();
    Size size = Director::getInstance()->getWinSize();
    runAction(MoveTo::create(0.20, m_nPosition));
    setVisible(true);
}

void RoomManagement2::hide()
{
//    this->setVisible(false);
//    this->removeFromParentAndCleanup(true);
    stopAllActions();
    Size winSize = Director::getInstance()->getWinSize();
    this->runAction(Sequence::create(MoveTo::create(0.20, Vec2(getPositionX() + 1000,getPositionY())),
                                     RemoveSelf::create(true),
                                     NULL));
}

bool RoomManagement2::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomManagement2::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
//    Json::Value xxx;
//    myInit(xxx);
    
    return true;
}

void RoomManagement2::myInit(Json::Value &data)
{
    Size size = Director::getInstance()->getWinSize();
    
    //背景
    Scale9Sprite* ChatBG= Scale9Sprite::create("RoomPublic/bar_a_l.png");
    ChatBG->setInsetTop(9);
    ChatBG->setInsetLeft(9);
    ChatBG->setInsetRight(16);
    ChatBG->setInsetBottom(9);
    ChatBG->setContentSize(Size(size.width*0.4,size.height*0.85));
    ChatBG->setPosition(Vec2(ChatBG->getContentSize().width/2,ChatBG->getContentSize().height/2));
    setContentSize(ChatBG->getContentSize());
    
    ChatBG->setFlippedX(true);
    
    //标题背景
    Scale9Sprite* TitleBG= Scale9Sprite::create("RoomPublic/bar_b_l.png");
    TitleBG->setInsetTop(10);
    TitleBG->setInsetLeft(0);
    TitleBG->setInsetRight(16);
    TitleBG->setInsetBottom(6);
    TitleBG->setContentSize(Size(ChatBG->getContentSize().width,ChatBG->getContentSize().height*0.15));
    TitleBG->setPosition(Vec2(ChatBG->getContentSize().width/2-6,ChatBG->getContentSize().height-TitleBG->getContentSize().height/2-6));
    ChatBG->addChild(TitleBG);
    TitleBG->setName("title_bg");
    
    //标题文字
    Text* text = Text::create("房间管理", "AmericanTypewriter", 40);
    text->setColor(Color3B(194, 174, 150));
    text->setPosition(Vec2(TitleBG->getContentSize().width/2,TitleBG->getContentSize().height/2));
    text->setFlippedX(true);
    TitleBG->addChild(text);
    
    //closebtn
    Button *pCloseBtn = Button::create("RoomPublic/room_public_close.png");
    pCloseBtn->setFlippedX(true);
    pCloseBtn->setPosition(Vec2(ChatBG->getContentSize().width + pCloseBtn->getContentSize().width/2, ChatBG->getContentSize().height / 2 + pCloseBtn->getContentSize().height/2));
    pCloseBtn->addClickEventListener([=](Ref *ref)
                                     {
                                         this->hide();
                                     });
    ChatBG->addChild(pCloseBtn);
    
    Node* pBGNode = Node::create();
    pBGNode->setName("content_bg");
    
    pBGNode->addChild(ChatBG);
    pBGNode->setPosition(Vec2(size.width - ChatBG->getContentSize().width,0));
    addChild(pBGNode);
    pBGNode->setContentSize(getContentSize());
    
    m_nPosition = getPosition();
    setPositionX(getPositionX() - 0);
    
    //-------------
    
    Size winSize = getContentSize();
    
    
    
    LayerColor *typeBg = LayerColor::create(Color4B(38, 38, 38, 255));
    typeBg->setContentSize(Size(winSize.width, 80));
    typeBg->ignoreAnchorPointForPosition(false);
    typeBg->setAnchorPoint(Vec2(0, 1));
    typeBg->setPosition(Vec2(6, getContentSize().height - typeBg->getContentSize().height));
    pBGNode->addChild(typeBg);
    
    Text *typeTitle = Text::create("娱乐类型", "AmericanTypewriter", 28);
    typeTitle->setColor(Color3B(194, 174, 150));
    typeTitle->setAnchorPoint(Vec2(0, 0.5));
    typeTitle->setPosition(Vec2(30, typeBg->getContentSize().height / 2));
    typeBg->addChild(typeTitle);
    
    char buff[256];
    std::string gameName = data["gameRoom"]["gameName"].asString();
    int bottomNote = data["gameRoom"]["bottomNote"].asInt();
    int takeIn = data["gameRoom"]["takeIn"].asInt();
    int status = data["gameRoom"]["aginstStatus"].asInt();
    
//        std::string gameName = "1233";
//        int bottomNote = 1;
//        int takeIn = 56789;
//        int status = 2;
    
    snprintf(buff, sizeof(buff), "%s\n(底注%d初始带入%d)", gameName.c_str(), bottomNote, takeIn);
    Text *typeLabel = Text::create(buff, "AmericanTypewriter", 28);
    typeLabel->setTextHorizontalAlignment(TextHAlignment::RIGHT);
    typeLabel->setAnchorPoint(Vec2(1, 0.5));
    typeLabel->setPosition(Vec2(winSize.width - 15, typeTitle->getPositionY()));
    typeBg->addChild(typeLabel);
    
    if (status == 0)
    {
        Text *text = Text::create("未开始", "AmericanTypewriter", 36);
        text->setColor(Color3B(200, 14, 12));
        text->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2 + 50));
        pBGNode->addChild(text);
        
        Text *tip = Text::create("超过12小时未开始游戏，房间将自动解散", "AmericanTypewriter", 26);
        tip->setColor(Color3B(105, 105, 105));
        tip->setPosition(Vec2(text->getPositionX(), text->getPositionY() - 40));
        pBGNode->addChild(tip);
    }
    else if (status == 1)
    {
        Text *text = Text::create("进行中", "AmericanTypewriter", 36);
        text->setColor(Color3B(200, 14, 12));
        text->setPosition(Vec2(getContentSize() / 2));
        pBGNode->addChild(text);
    }
    else if (status == 2)
    {
        Text *text = Text::create("已结束", "AmericanTypewriter", 36);
        text->setColor(Color3B(200, 14, 12));
        text->setPosition(Vec2(getContentSize() / 2));
        pBGNode->addChild(text);
    }
}

bool RoomManagement2::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto *contentBg = getChildByName("content_bg");
    if (!contentBg->boundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())))
    {
        this->hide();
        event->stopPropagation();
    }
    return true;
}