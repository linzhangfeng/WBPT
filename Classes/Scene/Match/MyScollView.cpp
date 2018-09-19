//
//  MyScollView.cpp
//  ZJH
//
//  Created by admin on 16/3/24.
//
//
#include "MyScollView.hpp"

bool ScollItem::init()
{
    if (!Layer::init())return false;
    preItem = NULL;
    nextItem = NULL;
    return true;
}

bool MyScollView::init()
{
    if (!Layer::init())return false;
    
    return true;
}

void MyScollView::initView(int viewW, int viewH, int itemW, int itemH, int scollType)
{
    type = scollType;
    
    this->viewW = viewW;
    this->viewH = viewH;
    this->itemW = itemW;
    this->itemH = itemH;
    
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2(0.0, 0.0));
    setContentSize(Size(viewW, viewH));
    
    clp = ClippingNode::create();
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
    
    schedule(schedule_selector(MyScollView::update), 0);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(MyScollView::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MyScollView::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MyScollView::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    state = State_idle;
    move(0);
    
    auto next = Layout::create();
    next->setTouchEnabled(true);
    next->setSwallowTouches(false);
    next->setAnchorPoint(Vec2(0.5, 0.5));
    next->setContentSize(Size(itemW,itemH));
    next->setPosition(Vec2(viewW / 2 + itemW, viewH / 2));
    next->addClickEventListener([=](Ref* sender)
                                {
                                    Button* n = (Button*)sender;
                                    Vec2 dvec2 = n->getTouchBeganPosition() - n->getTouchEndPosition();
                                    if (abs(dvec2.x) < 20)
                                    {
                                        toIndex(curIndex + 1);
                                    }
                                });
    addChild(next);
    
    auto pre = Layout::create();
    pre->setTouchEnabled(true);
    pre->setSwallowTouches(false);
    pre->setAnchorPoint(Vec2(0.5, 0.5));
    pre->setContentSize(Size(itemW,itemH));
    pre->setPosition(Vec2(viewW / 2 - itemW, viewH / 2));
    pre->addClickEventListener([=](Ref* sender)
                               {
                                   Button* n = (Button*)sender;
                                   Vec2 dvec2 = n->getTouchBeganPosition() - n->getTouchEndPosition();
                                   if (abs(dvec2.x) < 20)
                                   {
                                       toIndex(curIndex - 1);
                                   }
                               });
    addChild(pre);
}

int MyScollView::addItem(ScollItem *item)
{
    allItem[itemNums] = item;
    itemNums++;
    item->ignoreAnchorPointForPosition(false);
    item->setAnchorPoint(Vec2(0.5, 0.5));
    clp->addChild(item);
    return itemNums;
}

void MyScollView::reflash(int index)
{
    if (index >= itemNums) index = 0;
    
    if (type == Type_Repeat)
    {
        for (int i = 0; i < itemNums; i ++)
        {
            ScollItem* item = allItem[i];
            item->setPosition(-10000,itemH / 2);
        }
        
        for (int i = 1; i < itemNums - 1; i ++)
        {
            ScollItem* item = allItem[i];
            item->preItem = allItem[i - 1];
            item->nextItem = allItem[i + 1];
        }
        
        ScollItem* item = allItem[0];
        item->preItem = allItem[itemNums - 1];
        item->nextItem = allItem[1];
        
        item = allItem[itemNums - 1];
        item->preItem = allItem[itemNums - 2];
        item->nextItem = allItem[0];
        
        float bx = viewW / 2;
        
        auto curItem = allItem[index];
        auto preItem = curItem->preItem;
        auto nextItem = curItem->nextItem;
        
        curItem->setPosition(Vec2(bx, itemH / 2));
        preItem->setPosition(Vec2(bx - itemW, itemH / 2));
        nextItem->setPosition(Vec2(bx + itemW, itemH / 2));
        
        move(0);
    }
    else if (type == Type_Normal)
    {
        for (int i = 0; i < itemNums; i ++)
        {
            ScollItem* item = allItem[i];
            item->setPosition(viewW / 2 + i * itemW,itemH / 2);
        }
        
        move(-index * itemW);
    }
    
    curIndex = index;
    state = State_idle;
    if(stopCallback)stopCallback(curIndex);
}

bool MyScollView::isTouchInside(Touch* touch)
{
    Point touchPoint = touch->getLocation();
    Point localPos = getParent()->convertToNodeSpace(touchPoint);
    Rect rc = this->boundingBox();
    bool isTouched = rc.containsPoint(localPos);
    
    return isTouched;
}

bool MyScollView::onTouchBegan(Touch* touch, Event* event)
{
    if (!isVisible())
    {
        return false;
    }
    
    if(!isTouchInside(touch))
    {
        return false;
    }
    
//    retriP = 0;
    curPoint = touch->getLocation();
    waitMove = true;
    beginTime = Utils::getCurrentTime();
    beginPoint = curPoint;
    return true;
}

void MyScollView::onTouchMoved(Touch* touch, Event* event)
{
//    if(!isTouchInside(touch))
//    {
//        onTouchEnded(touch, event);
//        return;
//    }
    
    Point localPoint = touch->getLocation();
    float dx = localPoint.x - curPoint.x;
    
    if (waitMove && abs(dx) < 5)
    {
        return;
    }
    
    waitMove = false;
    
    if (state == State_idle)
    {
        state = State_move;
        if(runCallback)runCallback(0);
    }
    
    retriP = dx > 0 ? 1 : -1;
    if (abs(dx) > itemW / 2) dx = retriP * itemW / 2;
//    if(abs(dx) > 30)dx = retriP * 30;
    move(dx);
    retriV = abs(dx);
    curPoint = localPoint;
}

void MyScollView::onTouchEnded(Touch* touch, Event* event)
{
    if (waitMove)return;
    log("MyScollView::onTouchEnded");
    Point localPoint = touch->getLocation();
    state = State_automove;
    
    float dx = localPoint.x - beginPoint.x;
    int time = (Utils::getCurrentTime() - beginTime) * 30 / 1000;
    if (time == 0) time = 1;
    retriV = abs(dx) / time;
    if(retriV > itemW / 2)retriV = itemW / 2;
//    if(retriV > 30)30;
    if(retriV <= 0)retriV = 1;
}

void MyScollView::update(float dt)
{
    if (state == State_autoretri)
    {
        if (retriY <= retriV)
        {
            move(retriY * retriP);
            state = State_idle;
            for(int i = 0; i < itemNums; i ++)
            {
                auto item = allItem[i];
                float newY = item->getPositionX();
                if (newY < viewW / 2 + itemW / 2 && newY > viewW / 2 - itemW / 2)
                {
                    curIndex = i;
                    if(stopCallback)stopCallback(curIndex);
                    break;
                }
            }
        }
        else
        {
            move(retriP * retriV);
            retriY -= retriV;
        }
        retriV *= 5;
        if (retriV > itemW / 2)
        {
            retriV = itemW / 2;
        }
    }
    else if (state == State_automove)
    {
        if (retriV <= 10)
        {
            state = State_autoretri;
            log("MyScollView::onTouchEnded  state = State_autoretri");
            for(int i = 0; i < itemNums; i ++)
            {
                auto item = allItem[i];
                float nowY = item->getPositionX();
                float absY = abs(nowY - viewW / 2);
                if (absY < itemW)
                {
                    if (retriP >= 0 && nowY - viewW / 2 < 0)
                    {
                        state = State_autoretri;
                        retriY = viewW / 2 - nowY;
                        curIndex = i;
//                        log("MyScollView::onTouchEnded  retriP >= 0 && nowY - viewH / 2 < 0");
                        break;
                    }
                    else if (retriP <= 0 && nowY - viewW / 2 > 0)
                    {
                        state = State_autoretri;
                        retriY = nowY - viewW / 2;
                        curIndex = i;
//                        log("MyScollView::onTouchEnded  retriP <= 0 && nowY - viewH / 2 > 0");
                        break;
                    }
                }
            }
        }
        else
        {
            move(retriP * retriV);
            retriV *= 0.50;
        }
    }
    else if (state == State_moveToIndex)
    {
        if (retriY <= retriV)
        {
            move(retriY * retriP);
            state = State_idle;
            for(int i = 0; i < itemNums; i ++)
            {
                auto item = allItem[i];
                float newY = item->getPositionX();
                if (newY < viewW / 2 + itemW / 2 && newY > viewW / 2 - itemW / 2)
                {
                    curIndex = i;
                    if(stopCallback)stopCallback(curIndex);
                    break;
                }
            }
        }
        else
        {
            move(retriP * retriV);
            retriY -= retriV;
        }
        retriV *= 5;
        if (retriV > itemW / 2)
        {
            retriV = itemW / 2;
        }
    }
}

void MyScollView::move(int dy)
{
    if (type == Type_Normal)
    {
        if (dy > 0)
        {
            if (allItem[0]->getPositionX() + dy > viewW / 2)
            {
                dy = viewW / 2 - allItem[0]->getPositionX();
            }
        }
        else if (dy < 0)
        {
            if (allItem[itemNums - 1]->getPositionX() + dy < viewW / 2)
            {
                dy = viewW / 2 - allItem[itemNums - 1]->getPositionX();
            }
        }
    }
    
    for(int i = 0; i < itemNums; i ++)
    {
        auto item = allItem[i];
        float oldY = item->getPositionX();
        item->setPositionX(item->getPositionX() + dy);

        float nowY = item->getPositionX();
        
        float absY = abs(nowY - viewW / 2);
        if (absY <= itemW)
        {
            item->setScale(1.0 + 0.4 * ( 1 - absY / itemW));
            clp->reorderChild(item, item->getScale() * 10000);
        }
        else
        {
            item->setScale(1.0);
        }
    }
    
    if (type == Type_Repeat)
    {
        for(int i = 0; i < itemNums; i ++)
        {
            auto item = allItem[i];
            float newY = item->getPositionX();
            if (dy > 0 && newY <= viewW / 2 && newY >= itemW / 2)
            {
                item->preItem->setPositionX(newY - itemW);
            }
            else if (dy < 0 && newY >= viewW / 2 && newY <= viewW / 2 + itemW)
            {
                item->nextItem->setPositionX(newY + itemW);
            }
        }
    }
}

void MyScollView::toIndex(int index)
{
    if (state == State_idle)
    {
        if (type == Type_Repeat)
        {
            if (index < 0)index = itemNums - 1;
            if (index >= itemNums)index = 0;
            tIndex = index;
            
            ScollItem* curItem = allItem[curIndex];
            ScollItem* toItem = allItem[index];
            if(!toItem)return;
            int allNext = getNext(curItem,toItem);
            int allPre = getPre(curItem,toItem);
            if (allNext <= allPre && allNext > 0)
            {
                retriY = allNext * itemW;
                retriP = -1;
                retriV = 1;
                state = State_moveToIndex;
            }
            else if (allNext >= allPre && allPre > 0)
            {
                retriY = allPre * itemW;
                retriP = 1;
                retriV = 1;
                state = State_moveToIndex;
            }
        }
        else if (type == Type_Normal)
        {
            if (index < 0)return;
            if (index >= itemNums)return;
            tIndex = index;
            int dindex = index - curIndex;
            if (dindex > 0)
            {
                retriY = dindex * itemW;
                retriP = -1;
                retriV = 1;
                state = State_moveToIndex;
            }
            else if (dindex < 0)
            {
                retriY = abs(dindex) * itemW;
                retriP = 1;
                retriV = 1;
                state = State_moveToIndex;
            }
        }
    }
}

int MyScollView::getNext(ScollItem* curItem,ScollItem* toItem)
{
    if (curItem == toItem)return 0;
    if (curItem->nextItem == toItem)
    {
        return 1;
    }
    else
    {
        return 1 + getNext(curItem->nextItem,toItem);
    }
}

int MyScollView::getPre(ScollItem* curItem,ScollItem* toItem)
{
    if (curItem == toItem)return 0;
    if (curItem->preItem == toItem)
    {
        return 1;
    }
    else
    {
        return 1 + getPre(curItem->preItem,toItem);
    }
}