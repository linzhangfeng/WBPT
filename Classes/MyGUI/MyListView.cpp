//
//  MyListView.cpp
//  ZJH
//
//  Created by 陈家勇 on 14-6-19.
//
//

#include "MyListView.h"

using namespace cocos2d;

MyListView* MyListView::create()
{
    MyListView* list = new MyListView();
    if(list && list->init())
    {
        list->autorelease();
        return list;
    }
    else
    {
        delete list;
        list = NULL;
        return NULL;
    }
}

bool MyListView::init()
{
    curItem = NULL;
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(MyListView::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MyListView::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MyListView::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

Layer* MyListView::createItem()
{
    Layer* layer = Layer::create();
    layer->ignoreAnchorPointForPosition(false);
    layer->setAnchorPoint(Vec2(0.0f, 0.0f));
    return layer;
}

void MyListView::initView(int viewW, int viewH, int type)
{
    scollpercent = -1;
    nums = 0;
    m_enable = true;
    this->viewW = viewW;
    this->viewH = viewH;
    this->type = type;
    
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2(0, 0));
    setContentSize(Size(viewW, viewH));
    
    ClippingNode* clp = ClippingNode::create();
    clp->setTag(10);
    clp->setContentSize(  Size(viewW, viewH) );
    clp->setAnchorPoint(  Vec2(0.0, 0.0) );
    clp->setPosition( Vec2(0, 0) );
    this->addChild(clp);
    clp->setInverted(false);
    
    auto stencil = DrawNode::create();
    Vec2 rectangle[4];
    rectangle[0] = Vec2(0, 0);
    rectangle[1] = Vec2(clp->getContentSize().width, 0);
    rectangle[2] = Vec2(clp->getContentSize().width, clp->getContentSize().height);
    rectangle[3] = Vec2(0, clp->getContentSize().height);
    
    Color4F white(1, 1, 1, 1);
    stencil->drawPolygon(rectangle, 4, white, 1, white);
    clp->setStencil(stencil);
    
//    ClipLayer* clp = ClipLayer::create(Size(viewW, viewH));
//    clp->ignoreAnchorPointForPosition(false);
//    clp->setAnchorPoint(ccp(0.0f, 0.0f));
//    clp->setPosition(ccp(0, 0));
//    clp->setTag(10);
//    addChild(clp);
    
    movinglayer = Layer::create();
    movinglayer->ignoreAnchorPointForPosition(false);
    movinglayer->setAnchorPoint(Vec2(0.0f, 0.0f));
    movinglayer->setContentSize(Size(viewW, viewH));
    clp->addChild(movinglayer);
    
//    if(strcmp(buf, "") == 0)
//    {
//        Sprite* clg;
//        if (type == Type_row || type == Type_row_col)
//        {
//            clg = Sprite::create(buf);
//            clg->setScaleX(viewW / clg->boundingBox().size.width);
//            clg->setScaleY(disize / clg->boundingBox().size.height);
//            clg->setAnchorPoint(Vec2(0.5f, 1.0f));
//            clg->setPosition(Vec2(viewW / 2, viewH));
//            clg->setOpacity(0);
//            clg->setTag(10);
//            clp->addChild(clg);
//            
//            clg = Sprite::create(buf);
//            clg->setScaleX(viewW / clg->boundingBox().size.width);
//            clg->setScaleY(-disize / clg->boundingBox().size.height);
//            clg->setAnchorPoint(Vec2(0.5f, 1.0f));
//            clg->setPosition(Vec2(viewW / 2, 0));
//            clg->setOpacity(0);
//            clg->setTag(11);
//            clp->addChild(clg);
//        }
//        
//        if (type == Type_col || type == Type_row_col)
//        {
//            clg = Sprite::create(buf);
//            clg->setScaleX(viewH / clg->boundingBox().size.width);
//            clg->setScaleY(disize / clg->boundingBox().size.height);
//            clg->setAnchorPoint(Vec2(0.5f, 1.0f));
//            clg->setPosition(Vec2(0, viewH / 2));
//            clg->setOpacity(0);
//            clg->setTag(20);
//            clg->setRotation(-90);
//            clp->addChild(clg);
//            
//            clg = Sprite::create(buf);
//            clg->setScaleX(viewH / clg->boundingBox().size.width);
//            clg->setScaleY(disize / clg->boundingBox().size.height);
//            clg->setAnchorPoint(Vec2(0.5f, 1.0f));
//            clg->setPosition(Vec2(viewW, viewH / 2));
//            clg->setOpacity(0);
//            clg->setTag(21);
//            clg->setRotation(90);
//            clp->addChild(clg);
//        }
//    }
    
    state = State_idle;
    schedule(schedule_selector(MyListView::update), 0);
}

void MyListView::setScollBar(const char* buf)
{
//    if (type == Type_row && movinglayer->getContentSize().height <= viewH)
//    {
//        return;
//    }
//    
//    if (type == Type_col && movinglayer->getContentSize().width <= viewW)
//    {
//        return;
//    }
//    
    mscollbar = Sprite::create(buf);
    if (type == Type_row)
    {
        mscollbar->setAnchorPoint(Vec2(0.5, 0.0));
        float p = viewH * 1.0000f / itemH / nums;
        mscollbar->setScaleY(p * viewH / mscollbar->boundingBox().size.height);
        scollmaxy = viewH - p * viewH;
        mscollbar->setPosition(Vec2(viewW - mscollbar->boundingBox().size.width / 2, scollmaxy * 0));
        mscollbar->setOpacity(0);
        addChild(mscollbar,-1);
    }
    else if (type == Type_col)
    {
        mscollbar->setAnchorPoint(Vec2(0.0, 0.5));
        float p = viewW * 1.0000f / itemW / nums;
        mscollbar->setScaleX(p * viewW / mscollbar->boundingBox().size.width);
        scollmaxy = viewW - p * viewW;
        mscollbar->setPosition(Vec2(0, mscollbar->boundingBox().size.height / 2));
        mscollbar->setOpacity(0);
        addChild(mscollbar,-1);
    }
}

void MyListView::setScollbarVisi(bool value)
{
    if(value && mscollbar != NULL)mscollbar->setOpacity(255);
}

void MyListView::scollbarbegin()
{
    if (mscollbar)
    {
        mscollbar->stopAllActions();
        mscollbar->runAction(FadeTo::create(0.5,255));
    }
}

void MyListView::setScollPercent(float p)
{
    scollpercent = p;
    if (type == Type_row)
    {
        movinglayer->setPositionY((viewH - movinglayer->getContentSize().height) * (1 - p));
    }
    else if (type == Type_col)
    {
        movinglayer->setPositionY((viewW - movinglayer->getContentSize().width) * (1 - p));
    }
    scollbarmove();
}

void MyListView::scollbarmove()
{
    if (mscollbar && type == Type_row)
    {
        float p = -movinglayer->getPositionY() / (movinglayer->getContentSize().height - viewH);
        mscollbar->setPositionY(scollmaxy * p);
    }
    else if (mscollbar && type == Type_col)
    {
        float p = -movinglayer->getPositionX() / (movinglayer->getContentSize().width - viewW);
        //        CCLOG("%f",p);
        mscollbar->setPositionX(scollmaxy * p);
    }
}

void MyListView::scollbarend()
{
    if (mscollbar)
    {
        mscollbar->stopAllActions();
        mscollbar->runAction(FadeTo::create(2.0,0));
    }
}

int MyListView::addItem(Layer *item)
{
    allItem[nums] = item;
    nums++;
    movinglayer->addChild(item);
    return nums;
}

void MyListView::removeItemByTag(int tag)
{
    bool isFind = false;
    for(int i = 0; i < nums; i ++)
    {
        Layer* item = allItem[i];
        if(item->getTag() == tag)
        {
            isFind = true;
            if(i != nums - 1)swap(allItem[i], allItem[i + 1]);
        }
        else if (isFind)
        {
            if(i != nums - 1)swap(allItem[i], allItem[i + 1]);
        }
    }
    if(isFind)
    {
        nums --;
        movinglayer->removeChildByTag(tag, true);
    }
}

void MyListView::removeItemByIndex(int tag)
{
    bool isFind = false;
    for(int i = 0; i < nums; i ++)
    {
        if(i == tag)
        {
            isFind = true;
            allItem[i]->removeFromParentAndCleanup(true);
            if(i != nums - 1)swap(allItem[i], allItem[i + 1]);
            
        }
        else if (isFind)
        {
            if(i != nums - 1)swap(allItem[i], allItem[i + 1]);
        }
    }
    if(isFind)
    {
        nums --;
    }
}

void MyListView::removeAllItem()
{
    for(int i = nums - 1; i >= 0; i --)
    {
        allItem[i]->removeFromParentAndCleanup(true);
    }
    nums = 0;
    allItem.clear();
}

float MyListView::reflash(bool isResever)
{
    int bx = 0;
    int by = 0;
    float allValue = 0;
    if(isResever)
    {
        for(int i = 0; i < nums; i ++)
        {
            Layer* item = allItem[i];
            if(type == Type_row)
            {
                item->setPosition(Vec2(bx, by));
                by += item->getContentSize().height;
                allValue += item->getContentSize().height;
            }
            else if(type == Type_col)
            {
                item->setPosition(Vec2(bx, by));
                bx += item->getContentSize().width;
                allValue += item->getContentSize().width;
            }
        }
    }
    else
    {
        for(int i = nums - 1; i >= 0; i --)
        {
            Layer* item = allItem[i];
            if(type == Type_row)
            {
                item->setPosition(Vec2(bx, by));
                by += item->getContentSize().height;
                allValue += item->getContentSize().height;
            }
            else if(type == Type_col)
            {
                item->setPosition(Vec2(bx, by));
                bx += item->getContentSize().width;
                allValue += item->getContentSize().width;
            }
        }
    }
    
    if(type == Type_row)
    {
        movinglayer->setContentSize(Size(viewW, by));
    }
    else if(type == Type_col)
    {
        movinglayer->setContentSize(Size(bx, viewH));
    }
    
    return allValue;
}

Layer* MyListView::getItemByIndex(int index)
{
    return allItem[index];
}

Layer* MyListView::getItemByTag(int tag)
{
    for(int i = 0; i < nums; i ++)
    {
        Layer* item = allItem[i];
        if(item->getTag() == tag)
        {
            return item;
        }
    }
    return NULL;
}

bool MyListView::isTouchInside(Touch* touch)
{
    if (getParent() == NULL)
    {
        return false;
    }
    
    Point touchPoint = touch->getLocation();
    Point localPos = getParent()->convertToNodeSpace(touchPoint);
    Rect rc = this->boundingBox();
    bool isTouched = rc.containsPoint(localPos);
    
    return isTouched;
}

bool MyListView::onTouchBegan(Touch* touch, Event* event)
{
    if (!isVisible())
    {
        return false;
    }
    
    if (!m_enable)
    {
        return true;
    }
    
    if(!isTouchInside(touch))
    {
        return false;
    }
    
    Point touchPoint = touch->getLocation();
    Point localPos = movinglayer->convertToNodeSpace(touchPoint);
    
    preTouchx = touchPoint.x;
    preTouchy = touchPoint.y;
    state = State_waitmove;
    isMove = false;

    for(int i = 0; i < nums; i ++)
    {
        Layer* layer = allItem[i];
        Rect rc = layer->boundingBox();
        bool isTouched = rc.containsPoint(localPos);
        if (isTouched)
        {
            this->curItem = layer;
            schedule(schedule_selector(MyListView::curItemDown),0.2);
            return true;
        }
    }
    
    return true;
}

void MyListView::curItemDown(float dt)
{
    if (curItem)
    {
        if(_downcallback != NULL)_downcallback(curItem);
    }
    unschedule(schedule_selector(MyListView::curItemDown));
}

void MyListView::onTouchMoved(Touch* touch, Event* event)
{
    if (!m_enable)
    {
        return;
    }
    if(!isTouchInside(touch))
    {
        onTouchEnded(touch, event);
        return;
    }
    
    float minL = 10;
    Point touchPoint = touch->getLocation();
    int dx = (touchPoint.x - preTouchx) * 1;
    int dy = (touchPoint.y - preTouchy) * 1;
    if (abs(dy) > minL || abs(dx) > minL)
    {
        isMove = true;
    }
    
    if (state == State_waitmove)
    {
        if ((type == Type_row || type == Type_row_col) && abs(dy) > minL)
        {
            state = State_moverow;
            preTouchx = touchPoint.x;
            preTouchy = touchPoint.y;
            touchTime = 0;
            firstTouchx = preTouchx;
            firstTouchy = preTouchy;
            if(curItem)
            {
                if(_upcallback != NULL)_upcallback(curItem);
                curItem = NULL;
                unschedule(schedule_selector(MyListView::curItemDown));
            }
            scollbarbegin();
        }
        else if((type == Type_col || type == Type_row_col) && abs(dx) > minL)
        {
            state = State_movecol;
            preTouchx = touchPoint.x;
            preTouchy = touchPoint.y;
            touchTime = 0;
            firstTouchx = preTouchx;
            firstTouchy = preTouchy;
            if(curItem)
            {
                if(_upcallback != NULL)_upcallback(curItem);
                curItem = NULL;
                unschedule(schedule_selector(MyListView::curItemDown));
            }
            scollbarbegin();
        }
    }
    else if(state == State_movecol && abs(dx) > 3)
    {
        moveXY(dx,0);
        preTouchx = touchPoint.x;
        preTouchy = touchPoint.y;
    }
    else if(state == State_moverow && abs(dy) > 3)
    {
        moveXY(0,dy);
        preTouchx = touchPoint.x;
        preTouchy = touchPoint.y;
    }
    //    checkItemClick(touch,true);
}

void MyListView::onTouchEnded(Touch* touch, Event* event)
{
    if (!m_enable)
    {
        return;
    }
    Point touchPoint = touch->getLocation();
    float dx = (touchPoint.x - firstTouchx) * 1;
    float dy = (touchPoint.y - firstTouchy) * 1;
    
    float ex = movinglayer->getPositionX();
    float ey = movinglayer->getPositionY();
    
    if (state == State_movecol)
    {
        if (ex == 0)
        {
            state = State_idle;
            scollbarend();
        }
        else if (ex == viewW - movinglayer->getContentSize().width)
        {
            state = State_idle;
            scollbarend();
        }
        else
        {
            if (touchTime <= 0)
            {
                touchTime = 1;
            }
            vx = dx * 1 / touchTime;
            //            if (vx > 50)
            //            {
            //                vx = 50;
            //            }
            //            if (vx < -50)
            //            {
            //                vx = -50;
            //            }
            vy = 0;
            state = State_colautomove;
        }
    }
    else if (state == State_moverow)
    {
        if (ey == 0)
        {
            state = State_idle;
            scollbarend();
        }
        else if (ey == viewH - movinglayer->getContentSize().height)
        {
            state = State_idle;
            scollbarend();
        }
        else
        {
            if (touchTime <= 0)
            {
                touchTime = 1;
            }
            vx = 0;
            vy = dy * 1 / touchTime;
            state = State_rowautomove;
            //            if (vy > 50)
            //            {
            //                vy = 50;
            //            }
            //            if (vy < -50)
            //            {
            //                vy = -50;
            //            }
        }
    }
    else
    {
        state = State_idle;
        scollbarend();
    }
    //    checkItemClick(touch,false);
    if(curItem)
    {
        if(!isMove)
        {
            if(_clickcallback != NULL)_clickcallback(curItem);
        }
        else
        {
            if(_upcallback != NULL)_upcallback(curItem);
        }
        curItem = NULL;
    }
    //    isMove = false;
}

void MyListView::checkItemClick(Touch* touch, bool isDown)
{
    Point touchPoint = touch->getLocation();
    Point localPos = movinglayer->convertToNodeSpace(touchPoint);
    
    for(int i = 0; i < nums; i ++)
    {
        Layer* layer = allItem[i];
        Rect rc = layer->boundingBox();
        bool isTouched = rc.containsPoint(localPos);
        if (isTouched)
        {
            if (isDown)
            {
                if (curItem != layer)
                {
                    if(curItem)
                    {
                        if(_upcallback != NULL)_upcallback(curItem);
                    }
                    curItem = layer;
                   if(_downcallback != NULL)_downcallback(curItem);
                }
            }
            else
            {
                if(curItem == layer)
                {
                    if ((state == State_idle || state == State_waitmove) && !isMove)
                    {
                        if(_clickcallback != NULL)_clickcallback(curItem);
                        curItem = NULL;
                    }
                }
            }
            break;
        }
    }
    
    if(!isDown && curItem)
    {
        if(_upcallback != NULL)_upcallback(curItem);
        curItem = NULL;
    }
}

void MyListView::moveXY(float x, float y)
{
    //    if (type == Type_row && movinglayer->getContentSize().height <= viewH)
    //    {
    //        return;
    //    }
    //
    //    if (type == Type_col && movinglayer->getContentSize().width <= viewW)
    //    {
    //        return;
    //    }
    //
    x *= 1;
    y *= 1;
    float ex = movinglayer->getPositionX() + x;
    float ey = movinglayer->getPositionY() + y;
    if (x != 0)
    {
        if (ex > 0)
        {
            virtualDX += x * 4;
            ex = 0;
        }
        else if (ex < viewW - movinglayer->getContentSize().width)
        {
            virtualDX += x * 4;
            ex = viewW - movinglayer->getContentSize().width;
        }
        else
        {
            virtualDX = 0;
        }
        if (type == Type_col && movinglayer->getContentSize().width > viewW)
        {
            movinglayer->setPositionX(ex);
        }
    }
    
    if (y != 0)
    {
        if (ey > 0)
        {
            virtualDY += y * 4;
            ey = 0;
        }
        else if (ey < viewH - movinglayer->getContentSize().height)
        {
            virtualDY += y * 4;
            ey = viewH - movinglayer->getContentSize().height;
        }
        else
        {
            virtualDY = 0;
        }
        if (type == Type_row && movinglayer->getContentSize().height > viewH)
        {
            movinglayer->setPositionY(ey);
            //            return;
        }
    }
    
    if (virtualDX > 255)
    {
        virtualDX = 255;
    }
    else if (virtualDX < -255)
    {
        virtualDX = -255;
    }
    
    if (virtualDY > 255)
    {
        virtualDY = 255;
    }
    else if (virtualDY < -255)
    {
        virtualDY = -255;
    }
    
    if (state == State_movecol)
    {
        updateOpirity(true,virtualDX);
    }
    else if (state == State_moverow)
    {
        updateOpirity(false,virtualDY);
    }
    
    if (type == Type_row)
    {
        scollpercent = -movinglayer->getPositionY() / (movinglayer->getContentSize().height - viewH);
    }
    else if (type == Type_col)
    {
        scollpercent = -movinglayer->getPositionX() / (movinglayer->getContentSize().width - viewW);
    }
    
    scollbarmove();
}

void MyListView::update(float dt)
{
    if (state == State_idle)
    {
        checkoparity();
    }
    else if(state == State_colautomove)
    {
        float v = vx * 0.9;
        moveXY(v, 0);
        vx = vx * 0.9;
        checkoparity();
        if((vx >= 0 && vx <= 1.0f)||(vx <= 0 && vx >= -1.0f))
        {
            state = State_idle;
            scollbarend();
        }
    }
    else if (state == State_rowautomove)
    {
        //        CCLOG("%d",vy);
        float v = vy * 0.9;
        moveXY(0, v);
        vy = vy * 0.9;
        checkoparity();
        if((vy >= 0 && vy <= 1.0f)||(vy <= 0 && vy >= -1.0f))
        {
            state = State_idle;
            scollbarend();
        }
    }
    else if (state == State_movecol | state == State_moverow)
    {
        touchTime++;
    }
}

void MyListView::checkoparity()
{
    if (virtualDX > 0)
    {
        virtualDX -= 10;
        if (virtualDX < 0)virtualDX = 0;
        updateOpirity(true,virtualDX);
    }
    else if (virtualDX < 0)
    {
        virtualDX += 10;
        if (virtualDX > 0)virtualDX = 0;
        updateOpirity(true,virtualDX);
    }
    else if (virtualDY > 0)
    {
        virtualDY -= 10;
        if (virtualDY < 0)virtualDY = 0;
        updateOpirity(false,virtualDY);
    }
    else if (virtualDY < 0)
    {
        virtualDY += 10;
        if (virtualDY > 0)virtualDY = 0;
        updateOpirity(false,virtualDY);
    }
}

void MyListView::updateOpirity(bool isColMove,int value)
{
//    if (isColMove && value == 0)
//    {
//        ((Sprite*)getChildByTag(10)->getChildByTag(20))->setOpacity(abs(value));
//        ((Sprite*)getChildByTag(10)->getChildByTag(21))->setOpacity(abs(value));
//    }
//    else if (isColMove && value > 0)
//    {
//        ((Sprite*)getChildByTag(10)->getChildByTag(20))->setOpacity(abs(value));
//    }
//    else if (isColMove && value < 0)
//    {
//        ((Sprite*)getChildByTag(10)->getChildByTag(21))->setOpacity(abs(value));
//    }
//    else if (!isColMove && value == 0)
//    {
//        ((Sprite*)getChildByTag(10)->getChildByTag(10))->setOpacity(abs(value));
//        ((Sprite*)getChildByTag(10)->getChildByTag(11))->setOpacity(abs(value));
//    }
//    else if (!isColMove && value > 0)
//    {
//        ((Sprite*)getChildByTag(10)->getChildByTag(11))->setOpacity(abs(value));
//    }
//    else if (!isColMove && value < 0)
//    {
//        ((Sprite*)getChildByTag(10)->getChildByTag(10))->setOpacity(abs(value));
//    }
}

float MyListView::getCurPositionX()
{
    return movinglayer->getPositionX();
}

float MyListView::getCurPositionY()
{
    return movinglayer->getPositionY();
}

void MyListView::initPositionX(float x)
{
    movinglayer->setPositionX(x);
}

void MyListView::initPositionY(float y)
{
    movinglayer->setPositionY(y);
}

void MyListView::onDownCallback(const ccMenuCallback& callback)
{
    _downcallback = callback;
}

void MyListView::onUpCallback(const ccMenuCallback& callback)
{
    _upcallback = callback;
}

void MyListView::onClickCallback(const ccMenuCallback& callback)
{
    _clickcallback = callback;
}