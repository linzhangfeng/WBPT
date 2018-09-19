//
//  Loading.cpp
//  ZJH
//
//  Created by luochuanting on 13-5-3.
//
//

#include "Loading.h"
#include "ZJH.h"

using namespace cocos2d;

Loading* Loading::startLoading(Node *pNode,Vec2 p)
{
    Loading *loading = Loading::create();
    if(pNode)
    {
        pNode->addChild(loading,1000);
        loading->setPosition(p);
    }
    else
    {
        Size size = Director::getInstance()->getWinSize();
        loading->setPosition(size / 2);
        Director::getInstance()->getRunningScene()->addChild(loading,100000);
    }
    
    return loading;
}

bool Loading::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Sprite *p = Sprite::create("Loading/loading.png");
    p->setPosition(Vec2(0, 0));
    addChild(p);
    
    ActionInterval* actionBy = RotateBy::create(0.5f, 380);
    p->runAction(RepeatForever::create(actionBy));
    
    return true;
}

void Loading::maskSwllowTouch()
{
    // 创建一个事件监听器类型为 OneByOne 的单点触摸
    auto listener1 = EventListenerTouchOneByOne::create();
    // 设置是否吞没事件，在 onTouchBegan 方法返回 true 时吞没
    listener1->setSwallowTouches(true);
    
    // 使用 lambda 实现 onTouchBegan 事件回调函数
    listener1->onTouchBegan = [](Touch* touch, Event* event) -> bool {
        // 获取事件所绑定的 target
        return true;
    };
    
    EventDispatcher * eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
}