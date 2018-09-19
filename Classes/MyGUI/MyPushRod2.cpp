//
//  MyPushRod2.cpp
//  ZJH
//
//  Created by wuyu on 16/1/27.
//
//

#include "MyPushRod2.hpp"
#include "ZJH.h"
MyPushRod2* MyPushRod2::createWithFile(std::string ball_pic, std::string up_pic, std::string down_pic,bool swallowTouches)
{
    MyPushRod2 * pCommonButton = new MyPushRod2();
    if(pCommonButton && pCommonButton->_initWithFile(ball_pic, up_pic, down_pic,swallowTouches))
    {
        pCommonButton->autorelease();
        return pCommonButton;
    }
    CC_SAFE_DELETE(pCommonButton);
    return NULL;
}

bool MyPushRod2::_initWithFile(std::string ball_pic, std::string up_pic, std::string down_pic,bool swallowTouches)
{
    if (!Layer::init())
    {
        return false;
    }
    
    isStop = 0;
    
//    ignoreAnchorPointForPosition(false);
//    setAnchorPoint(Vec2(0.0f, 0.0f));
    
    UpSprite = Sprite::create(up_pic.c_str());
    UpSprite->setAnchorPoint(Vec2(0.5,0.0));
    UpSprite->setPosition(Vec2(0.0,0.0));
    this->addChild(UpSprite);
    
    BallSprite = Sprite::create(ball_pic.c_str());
    BallSprite->setAnchorPoint(Vec2(0.5,0.5));
    BallSprite->setPosition(Vec2(0, UpSprite->getContentSize().height));
    this->addChild(BallSprite,2);
    
    effectSprite = Sprite::create(down_pic.c_str());
    effectSprite->setVisible(false);
    effectSprite->setColor(Color3B::RED);
    effectSprite->setAnchorPoint(Vec2(0.5,0.5));
    effectSprite->setPosition(Vec2(0, UpSprite->getContentSize().height));

    this->addChild(effectSprite,1);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(swallowTouches);
    listener->onTouchBegan = CC_CALLBACK_2(MyPushRod2::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MyPushRod2::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MyPushRod2::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

bool MyPushRod2::_isTouchInside(Touch* touch)
{
    Point touchPoint = touch->getLocation();
    Point localPos = this->convertToNodeSpace(touchPoint);
    
    {
        Rect rc = BallSprite->boundingBox();
        rc.origin.x -= 20;
        rc.origin.y -= 20;
        rc.size.width += 40;
        rc.size.height += 40;
        
        bool isTouched = rc.containsPoint(localPos);
        return isTouched;
    }
    
    return false;
}

bool MyPushRod2::onTouchBegan(Touch* touch, Event* event)
{
    Point touchPoint = touch->getLocation();
    if (!_isTouchInside(touch))
    {
        return false;
    }
    
    curPos = touchPoint;
    return true;
}

void MyPushRod2::onTouchMoved(Touch* touch, Event* event)
{
    if (isStop == 1)return;
    Point touchPoint = touch->getLocation();
    
    updateBall(touchPoint.y - curPos.y);
    
    curPos = touchPoint;
}

void MyPushRod2::onTouchEnded(Touch* touch, Event* event)
{
    if(isStop == 0)RestPoint();
}

void MyPushRod2::setCallback(const ccBtnCallback& callback)
{
    _callback = callback;
}

void MyPushRod2::updateBall(int dy)
{
    float by = BallSprite->getPositionY() + dy;
    
    if (by > UpSprite->getContentSize().height)
    {
        by = UpSprite->getContentSize().height;
    }
    else if (by < -UpSprite->getContentSize().height)
    {
        by = -UpSprite->getContentSize().height;
    }
    
    BallSprite->setPositionY(by);
    effectSprite->setPositionY(by);
    
    float sy = by / UpSprite->getContentSize().height;
    
    UpSprite->setScaleY(sy);
    
    if (UpSprite->getContentSize().height - by >= UpSprite->getContentSize().height)
    {
        if (isStop == 0)
        {
            runToDown();
        }
    }
}

void MyPushRod2::runToDown()
{
    isStop = 1;
    BallSprite->stopAllActions();
    effectSprite->stopAllActions();
    UpSprite->stopAllActions();
    
    auto callback = CallFunc::create([=](){
        if( _callback != NULL)
        {
            _callback(this->getTag());
        }
    });
    BallSprite->runAction(Sequence::create(MoveTo::create(0.17, Vec2(0, -UpSprite->getContentSize().height)),callback, NULL));
    effectSprite->runAction(MoveTo::create(0.17, Vec2(0, -UpSprite->getContentSize().height)));
    UpSprite->runAction(ScaleTo::create(0.17, 1, -1));
    effectSprite->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1.0),FadeIn::create(0.3),DelayTime::create(0.5), NULL)));
    Sound::getInstance()->playEffect("Sound/tigger_push.mp3");
}

void MyPushRod2::RestPoint()
{
    BallSprite->stopAllActions();
    effectSprite->stopAllActions();
    UpSprite->stopAllActions();
    
    isStop = 0;
//    UpSprite->setScaleY(1.0);
//    BallSprite ->setPosition(Vec2(0,UpSprite->getContentSize().height));
//    effectSprite->setPositionY(BallSprite->getPositionY());
    BallSprite->runAction(MoveTo::create(0.17, Vec2(0, UpSprite->getContentSize().height)));
    effectSprite->runAction(MoveTo::create(0.17, Vec2(0, UpSprite->getContentSize().height)));
    UpSprite->runAction(ScaleTo::create(0.17, 1, 1));
    effectSprite->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1.0),FadeIn::create(0.3),DelayTime::create(0.5), NULL)));
}

void MyPushRod2::showEffect(bool v)
{
    effectSprite->stopAllActions();
    effectSprite->setPositionY(BallSprite->getPositionY());
    effectSprite->setVisible(v);
    effectSprite->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1.0),FadeIn::create(0.3),DelayTime::create(0.5), NULL)));
}