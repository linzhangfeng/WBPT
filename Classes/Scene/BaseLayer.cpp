//
//  BaseLayer.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "BaseLayer.hpp"

BaseLayer::~BaseLayer()
{
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

bool BaseLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    return_callback = NULL;
    
    baseHeadH = 150;
    lastTouchBackKeyTime = 0;
    setHeadH = baseHeadH;
    bg = NULL;
    
    pre_view = NULL;
    bg = NULL;
    
    isRuning = false;
    
    canReturn = true;
    
    setAnchorPoint(Vec2(0, 0));
    ignoreAnchorPointForPosition(false);
    
    setHead(setHeadH);
    
    Size size = Director::getInstance()->getWinSize();
    
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setAnchorPoint(Vec2(0,0));
    layout->setPosition(Vec2(0,0));
    layout->setTouchEnabled(true);
    layout->setSwallowTouches(true);
    layout->addClickEventListener([=](Ref*){
        log("click base layer [%s]",this->getName().c_str());
    });
    addChild(layout,-1000);
    
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(BaseLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    return true;
}

void BaseLayer::setHead(int h)
{
    setHeadH = h;
    if (bg)bg->removeFromParentAndCleanup(true);
    
    Size size = Director::getInstance()->getWinSize();
    
    bg = LayerColor::create(Color4B(14, 14, 15, 255));
    bg->setContentSize(Size(size.width,getHeadH()));
    bg->setAnchorPoint(Vec2(0.0f, 0.0f));
    bg->setPosition(0,size.height - getHeadH());
    addChild(bg, 100);
    
    Layout *line = Layout::create();
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->setBackGroundColor(Color3B(0, 0, 0));
    line->setPosition(Vec2(0, 0));
    line->setContentSize(Size(size.width, 3));
    bg->addChild(line);
}

int BaseLayer::getSpriteHeight(){
    return getHeadH();
}

BaseLayer* BaseLayer::setBackground(Color4B color)
{
    Size size = Director::getInstance()->getWinSize();
    LayerColor* bg = LayerColor::create(color);
    bg->setContentSize(size);
    bg->setAnchorPoint(Vec2(0.0f, 0.0f));
    bg->setPosition(0,0);
    addChild(bg, -100);
    return this;
}

BaseLayer* BaseLayer::setBackground(string strpath){
    Size size = Director::getInstance()->getWinSize();
    ImageView* _sprite = ImageView::create(strpath);
    _sprite->setContentSize(Size(size.width,size.height));
    _sprite->ignoreAnchorPointForPosition(false);
    _sprite->setAnchorPoint(Vec2(0,0));
    _sprite->setPosition(Vec2(0,0));
    addChild(_sprite,-100);
    return this;
}

BaseLayer* BaseLayer::setReturn_txt(std::string return_str,const MyCallBack_Null &callback)
{
    Size size = Director::getInstance()->getWinSize();
    int by = size.height - baseHeadH / 2;
    Text* text = Text::create(return_str, "AmericanTypewriter", 50);
    text->setColor(Color3B(194, 174, 150));
    text->setPosition(Vec2(size.width - 100,by));
    text->setTouchEnabled(true);
    text->addClickEventListener([=](Ref*){
        if(callback)
        {
            callback();
        }
        else
        {
            this->disappear();
        }
    });
    addChild(text,200);
    
    return_callback = callback;
    return this;
}

BaseLayer* BaseLayer::setReturn_img(std::string return_up,std::string return_down,const MyCallBack_Null &callback)
{
    Size size = Director::getInstance()->getWinSize();
    int by = size.height - baseHeadH / 2;
    
    Button* button = Button::create(return_up,
                                    return_down);
    button->setPosition(Vec2(80,by));
    button->setContentSize(Size(160,getHeadH()));
    button->ignoreContentAdaptWithSize(false);
    button->setFillContentSizeEnabled(false);
    button->addClickEventListener([=](Ref*){
        if(callback)
        {
            callback();
        }
        else
        {
            this->disappear();
        }
    });
    addChild(button,200);
    
    return_callback = callback;
    return this;
}

BaseLayer* BaseLayer::setPreView(BaseLayer* pre)
{
    this->pre_view = pre;
    return this;
}

BaseLayer* BaseLayer::setTitle(std::string title)
{
    removeChildByName("title");
    Size size = Director::getInstance()->getWinSize();
    int by = size.height - baseHeadH / 2 + 10;
    Text* text = Text::create(title, "AmericanTypewriter", 50);
    text->setColor(Color3B(194, 174, 150));
    text->setPosition(Vec2(size.width / 2.0f,by));
    text->setName("title");
    addChild(text,200);
    return this;
}

BaseLayer* BaseLayer::setMore_txt(std::string more,const MyCallBack_Null &callback)
{
    Size size = Director::getInstance()->getWinSize();
    int by = size.height - baseHeadH / 2;
    
    auto m = Button::create();
    m->setContentSize(Size(240,getHeadH()));
    m->ignoreContentAdaptWithSize(false);
    m->setTitleText(more);
    m->setTitleFontSize(45);
    m->setTitleColor(Color3B(194, 174, 150));
    m->setZoomScale(0.1f);
    m->setPosition(Vec2(size.width - 120,by));
    m->setPressedActionEnabled(true);
    m->addClickEventListener([=](Ref*){
        if(callback)callback();
    });
    addChild(m,200);
    
    return this;
}

BaseLayer* BaseLayer::setMore_img(std::string more_up,std::string more_down,const MyCallBack_Null &callback)
{
    Size size = Director::getInstance()->getWinSize();
    int by = size.height - baseHeadH / 2;
    Button* button = Button::create(more_up,
                                    more_down);
    button->setPosition(Vec2(size.width - 80,by));
    
    button->setContentSize(Size(160,getHeadH()));
    button->setName("more_img_btn");
    button->ignoreContentAdaptWithSize(false);
    button->setFillContentSizeEnabled(false);
    button->addClickEventListener([=](Ref*){if(callback)callback();});
    addChild(button,200);
    
    return this;
}

void BaseLayer::appear()
{
    Size size = Director::getInstance()->getWinSize();
    runAction(MoveBy::create(0.17, Vec2(-size.width, 0)));
    
    if (pre_view)pre_view->runAction(MoveBy::create(0.17, Vec2(-size.width, 0)));
    ZJHModel::getInstance()->curView = this;
}

void BaseLayer::disappear()
{
    Size size = Director::getInstance()->getWinSize();
    runAction(Sequence::create(MoveBy::create(0.17,Vec2(size.width, 0)),RemoveSelf::create(), NULL));
    
    if (pre_view)pre_view->runAction(MoveBy::create(0.17, Vec2(size.width, 0)));
    if (pre_view)
    {
        ZJHModel::getInstance()->curView = pre_view;
    }
    else
    {
        ZJHModel::getInstance()->curView = NULL;
    }
    
    isRuning = true;
}

int BaseLayer::getHeadH()
{
    return setHeadH;
}

void BaseLayer::onExit()
{
    Layer::onExit();
}

BaseLayer* BaseLayer::setPreByName(std::string name)
{
    BaseLayer* pre = pre_view;
    BaseLayer* tagetPre = NULL;
    while (true)
    {
        if (!pre)break;
        
        if (pre->getName() == name)
        {
            tagetPre = pre;
            break;
        }
        pre = pre->pre_view;
    }
    
    if (tagetPre && tagetPre != pre_view)
    {
        removePre(pre_view,tagetPre);
        pre_view = tagetPre;
    }
    return tagetPre;
}

void BaseLayer::removeToPreByName(std::string name)
{
    BaseLayer* pre = pre_view;
    BaseLayer* tagetPre = NULL;
    while (true)
    {
        if (!pre)break;
        
        if (pre->getName() == name)
        {
            tagetPre = pre;
            break;
        }
        pre = pre->pre_view;
    }
    
    if (tagetPre && tagetPre != pre_view)
    {
        removePre(pre_view,tagetPre);
        pre_view = tagetPre;
    }
    pre_view = tagetPre->pre_view;
    tagetPre->removeFromParentAndCleanup(true);
}

void BaseLayer::removePre(BaseLayer* pre, BaseLayer* toPreView)
{
    if (pre == NULL || pre == toPreView)return;
    removePre(pre->pre_view,toPreView);
    pre->removeFromParentAndCleanup(true);
}

void BaseLayer::onKeyReleased(EventKeyboard::KeyCode keycode,Event* event)
{
    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
    {
        if (isRuning == false)
        {
            if (return_callback)
            {
                return_callback();
            }
            else
            {
                if (pre_view == NULL || canReturn == false)
                {
                    //监听返回键  两次退出
                    long long currentTime = Utils::getCurrentTime();
                    if(currentTime - lastTouchBackKeyTime > 2000){
                        lastTouchBackKeyTime = currentTime;
                        PlatformHelper::showToast("再按一次退出玩呗斗牌");
                    }else{
                        PlatformHelper::exitWithoutParam();
                    }
                }
                else if (pre_view)
                {
                    this->disappear();
                }
            }
        }
    }
    event->stopPropagation();//
}
