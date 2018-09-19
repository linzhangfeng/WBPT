//
//  MyPushRod.cpp
//  ZJH
//
//  Created by wuyu on 16/1/27.
//
//

#include "MyPushRod.hpp"
#include "ZJH.h"
MyPushRod* MyPushRod::createWithSpriteFrame(std::string ball_pic, std::string up_pic, std::string down_pic,float isangle,bool swallowTouches)
{
    MyPushRod * pCommonButton = new MyPushRod();
    if(pCommonButton && pCommonButton->_initWithSpriteFrame(ball_pic, up_pic, down_pic,isangle,swallowTouches))
    {
        pCommonButton->autorelease();
        return pCommonButton;
    }
    CC_SAFE_DELETE(pCommonButton);
    return NULL;
}

bool MyPushRod::_initWithSpriteFrame(std::string ball_pic, std::string up_pic, std::string down_pic,float isangle,bool swallowTouches)
{
    if (!Layer::init())
    {
        return false;
    }
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2(0.5f, 0.5f));
    InitPoint =Vec2(getBoundingBox().size.width/2,getBoundingBox().size.height/2-67/2);
    angle =isangle;
    isEnd =false;
    isRun = false;
    UpSprite = Sprite::createWithSpriteFrameName(up_pic.c_str());
    UpSprite->setAnchorPoint(Vec2(0.5,0.0));
    UpSprite->setPosition(InitPoint);
    this->addChild(UpSprite);
    height =UpSprite->getBoundingBox().size.height;
    BallSprite = Sprite::createWithSpriteFrameName(ball_pic.c_str());
    BallSprite->setAnchorPoint(Vec2(0.5,0.5));
    
    _y =abs(BallSprite->getBoundingBox().size.height-height);
    ball_dx =height+5;
    BallSprite->setPosition(InitPoint+Vec2(0, ball_dx));
    this->addChild(BallSprite,2);
    DownSprite = Sprite::createWithSpriteFrameName(down_pic.c_str());
    DownSprite->setScaleY(0);
    DownSprite->setAnchorPoint(Vec2(0.5,1.0));
    DownSprite->setVisible(false);
    DownSprite->setRotation(-7);
    DownSprite->setPosition(Vec2(UpSprite->getPosition().x,UpSprite->getPosition().y+15));
    this->addChild(DownSprite);
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(swallowTouches);
    listener->onTouchBegan = CC_CALLBACK_2(MyPushRod::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MyPushRod::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MyPushRod::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    
    return true;
}

MyPushRod* MyPushRod::createWithFile(std::string ball_pic, std::string up_pic, std::string down_pic,float isangle,bool swallowTouches)
{
    MyPushRod * pCommonButton = new MyPushRod();
    if(pCommonButton && pCommonButton->_initWithFile(ball_pic, up_pic, down_pic,isangle,swallowTouches))
    {
        pCommonButton->autorelease();
        return pCommonButton;
    }
    CC_SAFE_DELETE(pCommonButton);
    return NULL;
}

bool MyPushRod::_initWithFile(std::string ball_pic, std::string up_pic, std::string down_pic,float isangle,bool swallowTouches)
{
    if (!Layer::init())
    {
        return false;
    }
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2(0.5f, 0.5f));
    InitPoint =Vec2(getBoundingBox().size.width/2,getBoundingBox().size.height/2-67/2);
    angle =isangle;
    isEnd =false;
    isRun = false;
    UpSprite = Sprite::create(up_pic.c_str());
    UpSprite->setAnchorPoint(Vec2(0.5,0.0));
    UpSprite->setPosition(InitPoint);
    this->addChild(UpSprite);
    height =UpSprite->getBoundingBox().size.height;
    BallSprite = Sprite::create(ball_pic.c_str());
    BallSprite->setAnchorPoint(Vec2(0.5,0.5));
    
    _y =abs(BallSprite->getBoundingBox().size.height-height);
    ball_dx =height+5;
    BallSprite->setPosition(InitPoint+Vec2(0, ball_dx));
    this->addChild(BallSprite,2);
    DownSprite = Sprite::create(down_pic.c_str());
    DownSprite->setScaleY(0);
    DownSprite->setAnchorPoint(Vec2(0.5,1.0));
    DownSprite->setVisible(false);
    DownSprite->setRotation(-angle);
    DownSprite->setPosition(Vec2(UpSprite->getPosition().x,UpSprite->getPosition().y+15));
    this->addChild(DownSprite);
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(swallowTouches);
    listener->onTouchBegan = CC_CALLBACK_2(MyPushRod::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MyPushRod::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MyPushRod::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void MyPushRod::RestPoint()
{
    BallSprite->setVisible(true);
    UpSprite->setVisible(true);
    UpSprite->setScaleY(1.0);
    DownSprite->setVisible(false);
    DownSprite->setScaleY(0);
    BallSprite ->setPosition(InitPoint+Vec2(0,ball_dx));
    isEnd =false;
    isRun = false;
}

bool MyPushRod::_isTouchInside(Touch* touch)
{
    
    Point touchPoint = touch->getLocation();
    Point localPos = this->convertToNodeSpace(touchPoint);
   
    {
        Rect rc = BallSprite->boundingBox();
        bool isTouched = rc.containsPoint(localPos);
        return isTouched;
    }
    
    return false;
}

bool MyPushRod::onTouchBegan(Touch* touch, Event* event)
{
    Point touchPoint = touch->getLocation();
    if (!_isTouchInside(touch))
    {
        return false;
    }
    CCLOG("--------onTouchBegan:%f",touchPoint.y);
    isRun = true;
    return true;
}

void MyPushRod::RunAction()
{
    float dele = 0;
    if (isRun) {
        return;
    }
    isRun =true;
    DownSprite->setScaleY(0);
    UpSprite->setScaleY(1.0);
    float x= height*sin(3.1414*7.0/180);
    MoveTo * move =MoveTo::create(0.5, Vec2(InitPoint.x+x,InitPoint.y-height));
    ScaleTo * down_s =ScaleTo::create(0.25, 1, 1);
    ScaleTo * up_s =ScaleTo::create(0.25, 1,0);
    CallFunc * func =CallFunc::create(CC_CALLBACK_0(MyPushRod::RestPoint, this));
    BallSprite->runAction(Sequence::create(move,DelayTime::create(0.5),func, NULL));
    DownSprite->runAction(Sequence::create(DelayTime::create(0.18),Show::create(),down_s, NULL));
    
    UpSprite->runAction(Sequence::create(up_s, NULL));
    Sound::getInstance()->playEffect("Sound/tigger_push1.mp3");
    
   
}

void MyPushRod::onTouchMoved(Touch* touch, Event* event)
{
    Point touchPoint = touch->getLocation();
    Point localPos = this->convertToNodeSpace(touchPoint);
    float dy =localPos.y-(InitPoint.y+ball_dx);
    DownSprite->setVisible(true);
    if (localPos.y <=InitPoint.y+ball_dx && localPos.y >=InitPoint.y-height)
    {
        if (abs(dy)>height)
        {
            DownSprite->setScaleY((abs(dy)-height+10)/height);
        }
        else{
            UpSprite->setScaleY((height-abs(dy))/height);
            DownSprite->setScaleY(0);
        }
        if (height-abs(dy)<=0) {
            UpSprite->setScaleY(0);
        }
        if (UpSprite->getBoundingBox().size.height<=0) {
            float c =DownSprite->getBoundingBox().size.height;
            float x= c*sin(3.1414*angle/180);
            BallSprite->setPosition(Vec2(InitPoint.x+x, localPos.y));
        
        }
        else{
            BallSprite->setPosition(Vec2(InitPoint.x, localPos.y));
        }
    }
    
    if(localPos.y >= InitPoint.y-height && localPos.y <= InitPoint.y)
    {
        if (UpSprite->getBoundingBox().size.height <= 0)
        {
            UpSprite->setScaleY(0);
            DownSprite->setScaleY(1.0);
            float c =DownSprite->getContentSize().height;
            float x= c*sin(3.1414*angle/180);
            BallSprite->setPosition(Vec2(InitPoint.x+x, InitPoint.y-height));
            if (!isEnd) {
                CCLOG("------end");
                if( _callback != NULL)
                {
                    _callback(this->getTag());
                }
                
                Sound::getInstance()->playEffect("Sound/tigger_push1.mp3");
                isEnd =true;
            }
           
        }
    }
//    if (localPos.y <=InitPoint.y-height/2)
//    {
//        UpSprite->setScaleY(0);
//        DownSprite->setScaleY(1.0);
//        BallSprite->setPosition(Vec2(BallSprite->getPositionX(), InitPoint.y-height));
//        if (!isEnd) {
//            CCLOG("------end");
//            if( _callback != NULL)
//            {
//                _callback(this->getTag());
//            }
//            
//            Sound::getInstance()->playEffect("Sound/tigger_push1.mp3");
//            isEnd =true;
//        }
//        
//    }
}

void MyPushRod::onTouchEnded(Touch* touch, Event* event)
{
    RestPoint();
    isRun = false;
}

void MyPushRod::setCallback(const ccBtnCallback& callback)
{
    _callback = callback;
}
