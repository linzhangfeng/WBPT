#include "MyButton.h"
#include "ZJH.h"
using namespace cocos2d;

MyButton* MyButton::createWithSpriteFrame(std::string normal, std::string selected, std::string disabled,bool swallowTouches)
{
    MyButton * pCommonButton = new MyButton();
    if(pCommonButton && pCommonButton->_initWithSpriteFrame(normal, selected, disabled,swallowTouches))
	{
        pCommonButton->autorelease();
        return pCommonButton;
    }
    CC_SAFE_DELETE(pCommonButton);
    return NULL;
}

bool MyButton::_initWithSpriteFrame(std::string normal, std::string selected, std::string disabled,bool swallowTouches)
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
    type = Type_frame;
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(swallowTouches);
    listener->onTouchBegan = CC_CALLBACK_2(MyButton::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MyButton::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MyButton::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->normal = normal;
    this->selected = selected;
    this->disabled = disabled == "" ? selected : disabled;
    
    pSprite = Sprite::createWithSpriteFrameName(normal.c_str());
    if(pSprite != NULL)
    {
        setContentSize(pSprite->getContentSize());
        pSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        pSprite->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2));
        addChild(pSprite);
    }
    
    return true;
}

MyButton* MyButton::createWithFile(std::string normal, std::string selected, std::string disabled,bool swallowTouches)
{
    MyButton * pCommonButton = new MyButton();
    if(pCommonButton && pCommonButton->_initWithFile(normal, selected, disabled,swallowTouches))
	{
        pCommonButton->autorelease();
        return pCommonButton;
    }
    CC_SAFE_DELETE(pCommonButton);
    return NULL;
}

bool MyButton::_initWithFile(std::string normal, std::string selected, std::string disabled,bool swallowTouches)
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
    
    type = Type_file;
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(swallowTouches);
    listener->onTouchBegan = CC_CALLBACK_2(MyButton::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MyButton::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MyButton::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->normal = normal;
    this->selected = selected;
    this->disabled = disabled == "" ? selected : disabled;
    
    pSprite = Sprite::create(normal.c_str());
    if(pSprite != NULL)
    {
        setContentSize(pSprite->getContentSize());
        pSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        pSprite->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2));
        addChild(pSprite);
    }
    
    return true;
}

MyButton* MyButton::createWithFileUseTitle(std::string normal, std::string selected, std::string disabled,std::string title, bool swallowTouches)
{
    MyButton * pCommonButton = new MyButton();
    if(pCommonButton && pCommonButton->_initWithFileUseTitle(normal, selected, disabled,title, swallowTouches))
    {
        pCommonButton->autorelease();
        return pCommonButton;
    }
    CC_SAFE_DELETE(pCommonButton);
    return NULL;
}

bool MyButton::_initWithFileUseTitle(std::string normal, std::string selected, std::string disabled,std::string title, bool swallowTouches)
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
    
    type = Type_file;
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(swallowTouches);
    listener->onTouchBegan = CC_CALLBACK_2(MyButton::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MyButton::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MyButton::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->normal = normal;
    this->selected = selected;
    this->disabled = disabled == "" ? selected : disabled;
    
    pSprite = Sprite::create(normal.c_str());
    if(pSprite != NULL)
    {
        setContentSize(pSprite->getContentSize());
        pSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        pSprite->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2));
        addChild(pSprite);
    }
    
    Sprite* title_sp = Sprite::create(title.c_str());
    if(title_sp)
    {
        title_sp->setAnchorPoint(Vec2(0.5f, 0.5f));
        title_sp->setPosition(Vec2(pSprite->getContentSize().width / 2, pSprite->getContentSize().height / 2));
        pSprite->addChild(title_sp);
    }
    return true;
}

MyButton* MyButton::createWithSize(Size size,bool swallowTouches)
{
    MyButton * pCommonButton = new MyButton();
    if(pCommonButton && pCommonButton->_initWithSize(size,swallowTouches))
	{
        pCommonButton->autorelease();
        return pCommonButton;
    }
    CC_SAFE_DELETE(pCommonButton);
    return NULL;
}

bool MyButton::_initWithSize(Size size,bool swallowTouches)
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
    
    type = Type_size;
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(swallowTouches);
    listener->onTouchBegan = CC_CALLBACK_2(MyButton::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MyButton::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MyButton::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->normal = "";
    this->selected = "";
    this->disabled = "";

    pSprite = NULL;
    setContentSize(size);
    return true;
}

void MyButton::setEnabled(bool enabled)
{
    if(pSprite != NULL)
    {
        if(type == Type_frame)
        {
            pSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(enabled ? normal.c_str(): disabled.c_str()));
        }
        else if(type == Type_file)
        {
            pSprite->setTexture(enabled ? normal.c_str(): disabled.c_str());
        }
    }
    this->enable = enabled;
}

void MyButton::setClick(bool IsClick)
{
    if(pSprite != NULL)
    {
        if(type == Type_frame)
        {
            pSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(IsClick ? normal.c_str(): selected.c_str()));
        }
        else if(type == Type_file)
        {
            pSprite->setTexture(IsClick ? normal.c_str():selected.c_str());
        }
    }
    this->enable = IsClick;
}

bool MyButton::_isTouchInside(Touch* touch)
{
    if (getParent() == NULL)
    {
        return false;
    }
    
    Vec2 touchPoint = touch->getLocation();
    Vec2 localPos = getParent()->convertToNodeSpace(touchPoint);
    Rect rc = this->boundingBox();
    bool isTouched = rc.containsPoint(localPos);
    return isTouched;
}

bool MyButton::onTouchBegan(Touch* touch, Event* event)
{
    if (_isTouchInside(touch))
    {
        if(!isVisible())
        {
            return false;
        }
        CCLOG("MyButton::onTouchBegan");
        if (!enable)
        {
            return true;
        }
        
        Vec2 touchPoint = touch->getLocation();
        preTouchx = touchPoint.x;
        preTouchy = touchPoint.y;
        
        if(pSprite != NULL)
        {
            if(type == Type_frame)
            {
                pSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(selected.c_str()));
            }
            else if(type == Type_file)
            {
                pSprite->setTexture(selected.c_str());
            }
        }
        
        isDown = true;
        return true;
    }
    return false;
}

void MyButton::onTouchMoved(Touch* touch, Event* event)
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
            if(pSprite != NULL)
            {
                if(type == Type_frame)
                {
                    pSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(normal.c_str()));
                }
                else if(type == Type_file)
                {
                    pSprite->setTexture(normal.c_str());
                }
            }
            isDown = false;
            return;
        }
    }
    
    if (!_isTouchInside(touch))
    {
        if(pSprite != NULL)
        {
            if(type == Type_frame)
            {
                pSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(normal.c_str()));
            }
            else if(type == Type_file)
            {
                pSprite->setTexture(normal.c_str());
            }
        }
        isDown = false;
    }
    else if (!isDown)
    {
        if(pSprite != NULL)
        {
            if(type == Type_frame)
            {
                pSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(selected.c_str()));
            }
            else if(type == Type_file)
            {
                pSprite->setTexture(selected.c_str());
            }
        }
        isDown = true;
    }
}

void MyButton::onTouchEnded(Touch* touch, Event* event)
{
    if (!enable || !isVisible())
    {
        return;
    }
    
    if (_isTouchInside(touch))
    {
        if(pSprite != NULL)
        {
            if(type == Type_frame)
            {
                pSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(normal.c_str()));
            }
            else if(type == Type_file)
            {
                pSprite->setTexture(normal.c_str());
            }
        }
        if (isDown)
        {
            Sound::getInstance()->playEffect("Sound/zjh/click.mp3", false,true);
            if( _callback != NULL)
            {
                _callback(this->getTag());
            }
        }
        isDown = false;
    }
}

void MyButton::setCallback(const ccBtnCallback& callback)
{
	_callback = callback;
}