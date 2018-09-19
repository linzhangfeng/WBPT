//
//  MyControlSwitch.cpp
//  ZJH
//
//  Created by wuyu on 16/2/20.
//
//

#include "MyControlSwitch.hpp"
using namespace cocos2d;

MyControlSwitch* MyControlSwitch::createWithSpriteFrame(std::string switch_bg, std::string switch_on, std::string switch_off,std::string switch_thumb,bool swallowTouches,bool ison )
{
    MyControlSwitch * pCommonButton = new MyControlSwitch();
    if(pCommonButton && pCommonButton->_initWithSpriteFrame(switch_bg, switch_on, switch_off,switch_thumb,swallowTouches,ison))
    {
        pCommonButton->autorelease();
        return pCommonButton;
    }
    CC_SAFE_DELETE(pCommonButton);
    return NULL;
}

bool MyControlSwitch::_initWithSpriteFrame(std::string switch_bg, std::string switch_on, std::string switch_off,std::string switch_thumb,bool swallowTouches,bool ison)
{
    if (!Layer::init())
    {
        return false;
    }
    
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2(0.5f, 0.5f));
    _callback = NULL;
    isDown = false;
    enable = true;
    isSwallowTouches = swallowTouches;
    on = ison;
    type = Type_frame;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(swallowTouches);
    listener->onTouchBegan = CC_CALLBACK_2(MyControlSwitch::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MyControlSwitch::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MyControlSwitch::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->normal = switch_on;
    this->selected = switch_off;
    this->disabled = disabled == "" ? selected : disabled;
    pSprite = Sprite::createWithSpriteFrameName(switch_bg.c_str());
    setContentSize(pSprite->boundingBox().size);
    pSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    pSprite->setPosition(Vec2(boundingBox().size.width / 2, boundingBox().size.height / 2));
    addChild(pSprite);
    pSprite = Sprite::createWithSpriteFrameName(switch_on.c_str());
    if (pSprite) {
        pSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        pSprite->setPosition(Vec2(pSprite->getBoundingBox().size.width / 2, pSprite->getBoundingBox().size.height / 2));
        addChild(pSprite);
    }
    

    pSprite = Sprite::createWithSpriteFrameName(switch_off.c_str());
    if (pSprite)
    {
        pSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        pSprite->setPosition(Vec2(boundingBox().size.width - pSprite->boundingBox().size.width / 2, boundingBox().size.height / 2));
        addChild(pSprite);
    }
    
    pSprite = Sprite::createWithSpriteFrameName(switch_thumb.c_str());
    pSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    pSprite->setPosition(Vec2(boundingBox().size.width - pSprite->boundingBox().size.width / 2, boundingBox().size.height / 2));
    addChild(pSprite);
    
    setOn(on);
    
    return true;
}

MyControlSwitch* MyControlSwitch::createWithFile(std::string switch_bg, std::string switch_on, std::string switch_off,std::string switch_thumb,bool swallowTouches,bool ison)
{
    MyControlSwitch * pCommonButton = new MyControlSwitch();
    if(pCommonButton && pCommonButton->_initWithFile(switch_bg, switch_on, switch_off,switch_thumb,swallowTouches,ison))
    {
        pCommonButton->autorelease();
        return pCommonButton;
    }
    CC_SAFE_DELETE(pCommonButton);
    return NULL;
}

bool MyControlSwitch::_initWithFile(std::string switch_bg, std::string switch_on, std::string switch_off,std::string switch_thumb,bool swallowTouches,bool ison)
{
    if (!Layer::init())
    {
        return false;
    }
    
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2(0.5f, 0.5f));
    _callback = NULL;
    isDown = false;
    enable = true;
    isSwallowTouches = swallowTouches;
    on = ison;
    type = Type_frame;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(swallowTouches);
    listener->onTouchBegan = CC_CALLBACK_2(MyControlSwitch::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MyControlSwitch::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MyControlSwitch::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->normal = switch_on;
    this->selected = switch_off;
    this->disabled = disabled == "" ? selected : disabled;
    pSprite = Sprite::create(switch_bg.c_str());
    setContentSize(pSprite->boundingBox().size);
    pSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    pSprite->setPosition(Vec2(boundingBox().size.width / 2, boundingBox().size.height / 2));
    addChild(pSprite);
    pSprite = Sprite::create(switch_on.c_str());
    if (pSprite)
    {
        pSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        pSprite->setPosition(Vec2(pSprite->getBoundingBox().size.width / 2+15,boundingBox().size.height / 2));
        addChild(pSprite);
    }
    
    
    pSprite = Sprite::create(switch_off.c_str());
    if (pSprite)
    {
        pSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        pSprite->setPosition(Vec2(boundingBox().size.width - pSprite->boundingBox().size.width / 2-15, boundingBox().size.height / 2));
        addChild(pSprite);
    }
    
    pSprite = Sprite::create(switch_thumb.c_str());
    pSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    pSprite->setPosition(Vec2(boundingBox().size.width - pSprite->boundingBox().size.width / 2+5, boundingBox().size.height / 2));
    addChild(pSprite);
    
    if (on)
    {
        if(pSprite)pSprite->setPosition(Vec2(pSprite->boundingBox().size.width / 2-5, boundingBox().size.height / 2));
    }
    else
    {
        if(pSprite)pSprite->setPosition(Vec2(boundingBox().size.width - pSprite->boundingBox().size.width / 2+5, boundingBox().size.height / 2));
    }
    return true;
}

void MyControlSwitch::setOn(bool on)
{
    this->on = on;
    
    if (on)
    {
        if(pSprite)pSprite->runAction(Sequence::create(MoveTo::create(0.05, Vec2(pSprite->boundingBox().size.width / 2-5, boundingBox().size.height / 2)),NULL));
    }
    else
    {
        if(pSprite)pSprite->runAction(Sequence::create(MoveTo::create(0.05, Vec2(boundingBox().size.width - pSprite->boundingBox().size.width / 2+5, boundingBox().size.height / 2)),NULL));
    }
}

void MyControlSwitch::setEnabled(bool enabled)
{
    if(pSprite != NULL)
    {
        if(type == Type_frame)
        {
            pSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(enabled ? (!on ? normal.c_str(): selected.c_str()): disabled.c_str()));
        }
        else if(type == Type_file)
        {
            pSprite->setTexture(enabled ? (!on ? normal.c_str(): selected.c_str()): disabled.c_str());
        }
    }
    this->enable = enabled;
}

bool MyControlSwitch::_isTouchInside(Touch* touch)
{
    if (!getParent())
    {
        return false;
    }
    
    Point touchPoint = touch->getLocation();
    Point localPos = getParent()->convertToNodeSpace(touchPoint);
    if(Rect(0, 0, getParent()->getContentSize().width, getParent()->getContentSize().height).containsPoint(localPos))
    {
        Rect rc = this->boundingBox();
        bool isTouched = rc.containsPoint(localPos);
        return isTouched;
    }
    
    return false;
}

bool MyControlSwitch::onTouchBegan(Touch* touch, Event* event)
{
    if (!enable || !isVisible())
    {
        return false;
    }
    
    if (_isTouchInside(touch))
    {
        setOn(!on);
        
        if( _callback != NULL)
        {
            _callback(this->getTag());
        }
    }
    return false;
}

void MyControlSwitch::onTouchMoved(Touch* touch, Event* event)
{
//    if (!enable || !isVisible())
//    {
//        return;
//    }
//    
//    if(isSwallowTouches == false)
//    {
//        Point touchPoint = touch->getLocation();
//        int dx = (touchPoint.x - preTouchx) * 1;
//        int dy = (touchPoint.y - preTouchy) * 1;
//        if (abs(dy) > 5 || abs(dx) > 5)
//        {
//            isDown = false;
//            return;
//        }
//    }
//    
//    if (!_isTouchInside(touch))
//    {
//        isDown = false;
//    }
//    else if (!isDown)
//    {
//        isDown = true;
//    }
}

void MyControlSwitch::onTouchEnded(Touch* touch, Event* event)
{
//    if (!enable || !isVisible())
//    {
//        return;
//    }
//    
//    if (_isTouchInside(touch))
//    {
//        setOn(!on);
//        
//        if (isDown)
//        {
//            if( _callback != NULL)
//            {
//                _callback(this->getTag());
//            }
//        }
//        isDown = false;
//    }
}

void MyControlSwitch::setCallback(const ccBtnCallback& callback)
{
    _callback = callback;
}