//
//  RoomShareLayer.cpp
//  ZJH
//
//  Created by hyhy on 16/7/26.
//
//

#include "RoomShareLayer.h"

void RoomShareLayer::show()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(Vec2(0, -winSize.height));
    this->runAction(MoveTo::create(0.17, Vec2(0, 0)));
    this->setVisible(true);
}

void RoomShareLayer::hide()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->runAction(Sequence::create(MoveTo::create(0.17, Vec2(0, -winSize.height)),
                                     RemoveSelf::create(),
                                     NULL));
}

bool RoomShareLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = Size(winSize.width, 630);
    
    LayerColor *mask = LayerColor::create(Color4B(0, 0, 0, 50));
    mask->setContentSize(Size(winSize.width, winSize.height * 2));
    this->addChild(mask);
    
    LayerColor *contentBg = LayerColor::create(Color4B(255, 255, 255, 255));
    contentBg->setContentSize(contentSize);
    contentBg->setName("content_bg");
    this->addChild(contentBg);
    
    Text* titlelabel = Text::create("分享推广到", "AmericanTypewriter", 32);
    titlelabel->setPosition(Vec2(contentSize.width / 2, contentSize.height - 44));
    titlelabel->setColor(Color3B::BLACK);
    contentBg->addChild(titlelabel);
    
    Text *cancelLabel = Text::create("取消", "AmericanTypewriter", 40);
    cancelLabel->setColor(Color3B::BLACK);
    cancelLabel->setPosition(Vec2(contentSize.width / 2, 54));
    cancelLabel->setTouchScaleChangeEnabled(false);
    cancelLabel->setTouchEnabled(true);
    cancelLabel->addClickEventListener([=](Ref *ref)
                                       {
                                           this->hide();
                                       });
    contentBg->addChild(cancelLabel);
    
    char buff[256];
    Vec2 btnPos[6] = {
        Vec2(contentSize.width * 0.191, 434),
        Vec2(contentSize.width * 0.5, 434),
        Vec2(contentSize.width * 0.809, 434),
        Vec2(contentSize.width * 0.191, 242),
        Vec2(contentSize.width * 0.5, 242),
        Vec2(contentSize.width * 0.809, 242),
    };
    std::string nameStrings[6] = {
        "朋友圈",
        "微信",
        "QQ空间",
        "QQ好友",
        "新浪微博",
        "短信",
    };
    for (int i = 0; i < 6; i++) {
        snprintf(buff, sizeof(buff), "RoomHead/windows_share_icon%d.png", i+1);
        Button *btn = Button::create(buff);
        btn->setPosition(btnPos[i]);
        btn->addClickEventListener(CC_CALLBACK_1(RoomShareLayer::buttonClicked, this));
        btn->setTag(i);
        btn->setScale(0.8);
        contentBg->addChild(btn);
        
        Text *name = Text::create(nameStrings[i], "AmericanTypewriter", 30);
        name->setPosition(Vec2(btn->getContentSize().width / 2, -35));
        name->setColor(Color3B::BLACK);
        btn->addChild(name);
    }
    
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomShareLayer::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

bool RoomShareLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    LayerColor *contentBg = dynamic_cast<LayerColor *>(this->getChildByName("content_bg"));
    if (contentBg->boundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())))
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

void RoomShareLayer::buttonClicked(cocos2d::Ref *sender)
{
    Button *btn = dynamic_cast<Button *>(sender);
    int tag = btn->getTag();
    if (tag == 1)//微信
    {
        Json::Value &data = ZJHModel::getInstance()->roomData;
        
        PlatformHelper::WeChatShare(data["gameRoom"]["shareTitle"].asString().c_str(), data["gameRoom"]["shareContent"].asString().c_str(), data["gameRoom"]["shareUrl"].asString().c_str(), "");
    }
    else
    {
        PlatformHelper::showToast("暂未开放");
    }
    this->hide();
}