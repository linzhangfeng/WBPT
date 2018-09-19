//
//  MySwitch.cpp
//  ZJH
//
//  Created by 陈家勇 on 13-12-17.
//
//

#include "MySwitch.h"

using namespace cocos2d;

MySwitch* MySwitch::createWithSpriteFrame(std::string normal, std::string selected, std::string disabled,bool swallowTouches, bool on)
{
    MySwitch * pCommonButton = new MySwitch();
    if(pCommonButton && pCommonButton->_initWithSpriteFrame(normal, selected, disabled,swallowTouches,on))
	{
        pCommonButton->autorelease();
        return pCommonButton;
    }
    CC_SAFE_DELETE(pCommonButton);
    return NULL;
}

bool MySwitch::_initWithSpriteFrame(std::string normal, std::string selected, std::string disabled,bool swallowTouches,bool ison)
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
    listener->onTouchBegan = CC_CALLBACK_2(MySwitch::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MySwitch::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MySwitch::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->normal = normal;
    this->selected = selected;
    this->disabled = disabled == "" ? selected : disabled;
    
    pSprite = Sprite::createWithSpriteFrameName(normal.c_str());
    if(pSprite != NULL)
    {
        setContentSize(pSprite->boundingBox().size);
        pSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        pSprite->setPosition(Vec2(boundingBox().size.width / 2, boundingBox().size.height / 2));
        addChild(pSprite);
    }
    
    setOn(on);
        
    return true;
}

MySwitch* MySwitch::createWithFile(std::string normal, std::string selected, std::string disabled,bool swallowTouches,bool on)
{
    MySwitch * pCommonButton = new MySwitch();
    if(pCommonButton && pCommonButton->_initWithFile(normal, selected, disabled,swallowTouches,on))
	{
        pCommonButton->autorelease();
        return pCommonButton;
    }
    CC_SAFE_DELETE(pCommonButton);
    return NULL;
}

bool MySwitch::_initWithFile(std::string normal, std::string selected, std::string disabled,bool swallowTouches,bool ison)
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
    type = Type_file;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(swallowTouches);
    listener->onTouchBegan = CC_CALLBACK_2(MySwitch::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MySwitch::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MySwitch::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->normal = normal;
    this->selected = selected;
    this->disabled =  disabled == "" ? selected : disabled;
    
    pSprite = Sprite::create(normal.c_str());
    if(pSprite != NULL)
    {
        setContentSize(pSprite->boundingBox().size);
        pSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        pSprite->setPosition(Vec2(boundingBox().size.width / 2, boundingBox().size.height / 2));
        addChild(pSprite);
    }
    
    setOn(on);
    return true;
}

void MySwitch::setOn(bool on)
{
    this->on = on;
    
    if(pSprite != NULL)
    {
        if(type == Type_frame)
        {
            pSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(!on ? normal.c_str(): selected.c_str()));
        }
        else if(type == Type_file)
        {
            pSprite->setTexture(!on ? normal.c_str(): selected.c_str());
        }
    }
}

void MySwitch::setEnabled(bool enabled)
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

bool MySwitch::_isTouchInside(Touch* touch)
{
    if (!getParent())
    {
        return false;
    }
    
    Vec2 touchPoint = touch->getLocation();
    Vec2 localPos = getParent()->convertToNodeSpace(touchPoint);
    Rect rc = this->boundingBox();
    bool isTouched = rc.containsPoint(localPos);
    return isTouched;
}

bool MySwitch::onTouchBegan(Touch* touch, Event* event)
{
    if (_isTouchInside(touch))
    {
        if(!isVisible())
        {
            return false;
        }
        if (!enable)
        {
            return true;
        }
        
        Point touchPoint = touch->getLocation();
        preTouchx = touchPoint.x;
        preTouchy = touchPoint.y;
        
        isDown = true;
        return true;
    }
    return false;
}

void MySwitch::onTouchMoved(Touch* touch, Event* event)
{
    if (!enable || !isVisible())
    {
        return;
    }
    
    if(isSwallowTouches == false)
    {
        Point touchPoint = touch->getLocation();
        int dx = (touchPoint.x - preTouchx) * 1;
        int dy = (touchPoint.y - preTouchy) * 1;
        if (abs(dy) > 5 || abs(dx) > 5)
        {
            isDown = false;
            return;
        }
    }
    
    if (!_isTouchInside(touch))
    {
        isDown = false;
    }
    else if (!isDown)
    {
        isDown = true;
    }
}

void MySwitch::onTouchEnded(Touch* touch, Event* event)
{
    if (!enable || !isVisible())
    {
        return;
    }
    
    if (_isTouchInside(touch))
    {
        setOn(!on);
        
        if (isDown)
        {
            if( _callback != NULL)
            {
                _callback(this->getTag());
            }
        }
        isDown = false;
    }
}

void MySwitch::setCallback(const ccBtnCallback& callback)
{
	_callback = callback;
}