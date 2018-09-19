//
//  JiaoCheng.cpp
//  ZJH
//
//  Created by apple on 16/8/17.
//
//

#include "JiaoCheng.hpp"
bool JiaoCheng::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    setAnchorPoint(Vec2(0.0, 0.0));
    setPosition(Vec2(0.0, 0.0));
    ignoreAnchorPointForPosition(false);
    int firstWelcome = UserDefault::getInstance()->getIntegerForKey("jiaocheng_ok",0);
    int secondWelcome = UserDefault::getInstance()->getIntegerForKey("jiaocheng_second",0);
    if (firstWelcome == 0)
    {
        jiaocheng_jionRoom();
    }else if (secondWelcome == 0){
        jiaoCheng_clickWithoutRoom();
    }else{
        removeFromParent();
    }
    return true;
}

void JiaoCheng::jiaocheng_jionRoom()
{
    removeAllChildren();
    Size size = Director::getInstance()->getWinSize();
    
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setAnchorPoint(Vec2(0,0));
    layout->setPosition(Vec2(0,0));
    layout->setTouchEnabled(true);
    layout->setSwallowTouches(true);
    layout->addClickEventListener([=](Ref*){
        jiaoCheng_clickGame();
    });
    addChild(layout,-1000);
    
    auto clipper = ClippingNode::create();
    clipper->setContentSize(size);
    clipper->setAnchorPoint(Vec2(0.0, 0.0));
    clipper->setPosition(Vec2(0.0, 0.0));
    clipper->setInverted(true);
    
    auto clp = Sprite::create("JiaoCheng/11.png");
    clp->setAnchorPoint( Vec2(0.5, 0.5) );
    clp->setPosition(Vec2(size.width/2, 320 + 30 + 110 / 2));
//    clp->setScaleY(0.8);
    clipper->setStencil(clp);
    addChild(clp);

    auto bg = LayerColor::create(Color4B(0, 0, 0, 255 * 0.7));
    bg->setContentSize(size);
    bg->setAnchorPoint(Vec2(0.0, 0.0));
    bg->setPosition(0,0);
    clipper->addChild(bg);
    addChild(clipper);
    
    auto tip = Sprite::create("JiaoCheng/1.png");
    tip->setAnchorPoint( Vec2(0.5, 0.5) );
    tip->setPosition(Vec2(size.width / 2, 665));
    addChild(tip);
    
    auto ok = Sprite::create("JiaoCheng/ok.png");
    ok->setAnchorPoint( Vec2(0.5, 0.5) );
    ok->setPosition(Vec2(size.width / 2, 203));
    addChild(ok);
}

void JiaoCheng::jiaoCheng_clickGame()
{
    removeAllChildren();
    Size size = Director::getInstance()->getWinSize();
    
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setAnchorPoint(Vec2(0,0));
    layout->setPosition(Vec2(0,0));
    layout->setTouchEnabled(true);
    layout->setSwallowTouches(true);
    layout->addClickEventListener([=](Ref*){
        jiaoCheng_clickDiqu();
    });
    addChild(layout,-1000);
    
    auto clipper = ClippingNode::create();
    clipper->setContentSize(size);
    clipper->setAnchorPoint(Vec2(0.0, 0.0));
    clipper->setPosition(Vec2(0.0, 0.0));
    clipper->setInverted(true);
    
    auto clp = Sprite::create("JiaoCheng/22.png");
    clp->setAnchorPoint( Vec2(0.5, 0.5) );
    clp->setPosition(Vec2(size.width / 2 - 500 + 1000 / 3 / 2 + 2, size.height - 284 - 900 / 3 / 2));
    clipper->setStencil(clp);
    addChild(clp);
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 255 * 0.88));
    bg->setContentSize(size);
    bg->setAnchorPoint(Vec2(0.0, 0.0));
    bg->setPosition(0,0);
    clipper->addChild(bg);
    addChild(clipper);
    
    auto tip = Sprite::create("JiaoCheng/2.png");
    tip->setAnchorPoint( Vec2(0.5, 0.5) );
    tip->setPosition(Vec2(clp->getPositionX() + 500 - 20, clp->getPositionY() - 0));
    addChild(tip);
    
    auto ok = Sprite::create("JiaoCheng/ok.png");
    ok->setAnchorPoint( Vec2(0.5, 0.5) );
    ok->setPosition(Vec2(clp->getPositionX() + 500 - 20, clp->getPositionY() - 200));
    addChild(ok);
}

void JiaoCheng::jiaoCheng_clickDiqu()
{
    removeAllChildren();
    Size size = Director::getInstance()->getWinSize();
    
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setAnchorPoint(Vec2(0,0));
    layout->setPosition(Vec2(0,0));
    layout->setTouchEnabled(true);
    layout->setSwallowTouches(true);
    layout->addClickEventListener([=](Ref*){
        UserDefault::getInstance()->setIntegerForKey("jiaocheng_ok",1);
        UserDefault::getInstance()->flush();
        jiaoCheng_clickWithoutRoom();
    });
    addChild(layout,-1000);
    
    auto clipper = ClippingNode::create();
    clipper->setContentSize(size);
    clipper->setAnchorPoint(Vec2(0.0, 0.0));
    clipper->setPosition(Vec2(0.0, 0.0));
    clipper->setInverted(true);
    
    auto clp = Sprite::create("JiaoCheng/33.png");
    clp->setAnchorPoint( Vec2(0.0, 0.5) );
    clp->setPosition(Vec2(87,size.height - 248));
    clipper->setStencil(clp);
    addChild(clp);
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 255 * 0.88));
    bg->setContentSize(size);
    bg->setAnchorPoint(Vec2(0.0, 0.0));
    bg->setPosition(0,0);
    clipper->addChild(bg);
    addChild(clipper);
    
    auto tip = Sprite::create("JiaoCheng/3.png");
    tip->setAnchorPoint( Vec2(0.5, 0.5) );
    tip->setPosition(Vec2(clp->getPositionX() + 80 + 145, clp->getPositionY() - 159));
    addChild(tip);
    
    auto ok = Sprite::create("JiaoCheng/ok.png");
    ok->setAnchorPoint( Vec2(0.5, 0.5) );
    ok->setPosition(Vec2(clp->getPositionX() + 543 + 145, clp->getPositionY() - 231));
    addChild(ok);
}

void JiaoCheng::jiaoCheng_clickWithoutRoom(){
    removeFromParent();
    return;
    removeAllChildren();
    Size size = Director::getInstance()->getWinSize();
    
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setAnchorPoint(Vec2(0,0));
    layout->setPosition(Vec2(0,0));
    layout->setTouchEnabled(true);
    layout->setSwallowTouches(true);
    layout->addClickEventListener([=](Ref*){
        UserDefault::getInstance()->setIntegerForKey("jiaocheng_second",1);
        UserDefault::getInstance()->flush();
        removeFromParent();
    });
    addChild(layout,-1000);
    
    auto clipper = ClippingNode::create();
    clipper->setContentSize(size);
    clipper->setAnchorPoint(Vec2(0.0, 0.0));
    clipper->setPosition(Vec2(0.0, 0.0));
    clipper->setInverted(true);
    
    auto clp = Sprite::create("JiaoCheng/44.png");
    clp->setAnchorPoint(Vec2(0.5, 0.0) );
    clp->setPosition(Vec2(size.width / 2, 135));
    clipper->setStencil(clp);
    addChild(clp);
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 255 * 0.88));
    bg->setContentSize(size);
    bg->setAnchorPoint(Vec2(0.0, 0.0));
    bg->setPosition(0,0);
    clipper->addChild(bg);
    addChild(clipper);
    
    auto tip = Sprite::create("JiaoCheng/4.png");
    tip->setAnchorPoint(Vec2(0.5, 0) );
    tip->setPosition(Vec2(size.width / 2, 170));
    addChild(tip);
}
