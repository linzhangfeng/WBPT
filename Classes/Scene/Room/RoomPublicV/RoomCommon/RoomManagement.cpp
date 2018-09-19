//
//  RoomManagement.cpp
//  ZJH
//
//  Created by hyhy on 16/7/26.
//
//

#include "RoomManagement.h"

static const float scaleRate = 0.67;

void RoomManagement::show()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(Vec2(0, -winSize.height));
    this->runAction(MoveTo::create(0.17, Vec2(0, 0)));
    this->setVisible(true);
}

void RoomManagement::hide()
{
    this->setVisible(false);
    this->removeFromParentAndCleanup(true);
}

bool RoomManagement::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomManagement::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void RoomManagement::myInit(Json::Value &data)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    LayerColor *contentBg = LayerColor::create(Color4B(32, 32, 32, 255));
    contentBg->setContentSize(Size(winSize.width, 650));
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
    
    Text* titlelabel = Text::create("房间管理", "AmericanTypewriter", 50 * scaleRate);
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
    
    
    LayerColor *typeBg = LayerColor::create(Color4B(38, 38, 38, 255));
    typeBg->setContentSize(Size(winSize.width, 80));
    typeBg->ignoreAnchorPointForPosition(false);
    typeBg->setAnchorPoint(Vec2(0, 1));
    typeBg->setPosition(Vec2(0, contentBg->getContentSize().height));
    contentBg->addChild(typeBg);
    
    Text *typeTitle = Text::create("娱乐类型", "AmericanTypewriter", 28);
    typeTitle->setAnchorPoint(Vec2(0, 0.5));
    typeTitle->setPosition(Vec2(30, typeBg->getContentSize().height / 2));
    typeBg->addChild(typeTitle);
    
    char buff[256];
    std::string gameName = data["gameRoom"]["gameName"].asString();
    int bottomNote = data["gameRoom"]["bottomNote"].asInt();
    int takeIn = data["gameRoom"]["takeIn"].asInt();
    int status = data["gameRoom"]["aginstStatus"].asInt();
    snprintf(buff, sizeof(buff), "%s(底注%d初始带入%d)", gameName.c_str(), bottomNote, takeIn);
    Text *typeLabel = Text::create(buff, "AmericanTypewriter", 28);
    typeLabel->setAnchorPoint(Vec2(1, 0.5));
    typeLabel->setPosition(Vec2(winSize.width - 30, typeTitle->getPositionY()));
    typeBg->addChild(typeLabel);
    
    if (status == 0)
    {
        Text *text = Text::create("未开始", "AmericanTypewriter", 36);
        text->setColor(Color3B(200, 14, 12));
        text->setPosition(Vec2(contentBg->getContentSize().width / 2, contentBg->getContentSize().height / 2 + 50));
        contentBg->addChild(text);
        
        Text *tip = Text::create("超过12小时未开始游戏，房间将自动解散", "AmericanTypewriter", 26);
        tip->setColor(Color3B(105, 105, 105));
        tip->setPosition(Vec2(text->getPositionX(), text->getPositionY() - 40));
        contentBg->addChild(tip);
    }
    else if (status == 1)
    {
        Text *text = Text::create("进行中", "AmericanTypewriter", 36);
        text->setColor(Color3B(200, 14, 12));
        text->setPosition(Vec2(contentBg->getContentSize() / 2));
        contentBg->addChild(text);
    }
    else if (status == 2)
    {
        Text *text = Text::create("已结束", "AmericanTypewriter", 36);
        text->setColor(Color3B(200, 14, 12));
        text->setPosition(Vec2(contentBg->getContentSize() / 2));
        contentBg->addChild(text);
    }
}

bool RoomManagement::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
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