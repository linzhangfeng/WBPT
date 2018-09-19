
//
//  TaskLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/6.
//


#include "TaskLayer.hpp"
#include "ReciveEffectTipLayer.hpp"
#include "OneTaskLayer.hpp"
#include "GrowTaskLayer.hpp"

bool TaskLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout *bgLayout = Layout::create();
    bgLayout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    bgLayout->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    bgLayout->setBackGroundColorOpacity(150);
    bgLayout->setTouchEnabled(true);
    bgLayout->setSwallowTouches(true);
    bgLayout->setContentSize(visibleSize);
    bgLayout->addClickEventListener([=](Ref*ref){
        this->removeFromParent();
    });
    
    this->addChild(bgLayout);

    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(810, 1200));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    bgLayout->addChild(bg);
    
    float H = bg->getContentSize().height;
    float bgW = bg->getContentSize().width;
    
    Button *cancel = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    cancel->setPosition(Vec2(bgW, H));
    cancel->addClickEventListener([=](Ref*ref){
        this->removeFromParent();
    });
    bg->addChild(cancel);
    
    Text *title = Text::create("平台任务", ".SFUIDisplay-Semibold", 40);
    title->setAnchorPoint(Vec2(0.5, 1));
    title->setTextColor(Color4B(0xf1, 0xcb, 0x5b, 255));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(bgW/2, H - 35));
    bg->addChild(title);
    
    ImageView *left1 = ImageView::create("Mine/task_head_left.9.png");
    Size butSize = Size(270, 80);
    
    left = Layout::create();
    left->setName("one");
    left->setAnchorPoint(Vec2(1, 1));
    left->setContentSize(butSize);
    left->ignoreAnchorPointForPosition(false);
    left->setTouchEnabled(true);
    left->addClickEventListener(CC_CALLBACK_1(TaskLayer::tabClickFun, this));
    left->setPosition(Vec2(bgW/2, H - 105));
    
    bg->addChild(left);
    lastBut = left;
    
    left1->setTag(200);
    left1->setPosition(Vec2(butSize.width/2, butSize.height/2));
    left->addChild(left1);
    left1->setVisible(false);
    
    ImageView *lefts = ImageView::create("Mine/task_head_left_check.9.png");
    lefts->setTag(201);
    lefts->setPosition(Vec2(butSize.width/2, butSize.height/2));
    left->addChild(lefts);
    
    Text *name = Text::create("一次", ".SFUIDisplay-Semibold", 36);
    name->setTag(202);
    name->setTextColor(Color4B(0x1a, 0x1a, 0x1a, 255));
    name->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    name->setPosition(Vec2(butSize.width/2, butSize.height/2));
    left->addChild(name, 100);
    
    right = Layout::create();
    right->setName("grow");
    right->setAnchorPoint(Vec2(0, 1));
    right->setContentSize(butSize);
    right->ignoreAnchorPointForPosition(false);
    right->setTouchEnabled(true);
    right->addClickEventListener(CC_CALLBACK_1(TaskLayer::tabClickFun, this));
    right->setPosition(Vec2(bgW/2, H - 105));
    bg->addChild(right);
    
    Text *name2 = Text::create("成长", ".SFUIDisplay-Semibold", 36);
    name2->setTag(202);
    name2->setTextColor(Color4B(0xf0, 0xcc, 0x5b, 255));
    name2->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    name2->setPosition(Vec2(butSize.width/2, butSize.height/2));
    right->addChild(name2, 100);
    
    ImageView *right1 = ImageView::create("Mine/task_head_right.9.png");
    right1->setTag(200);
    right1->setPosition(Vec2(butSize.width/2, butSize.height/2));
    right->addChild(right1);
    
    ImageView *rights = ImageView::create("Mine/task_head_right_check.9.png");
    rights->setTag(201);
    rights->setPosition(Vec2(butSize.width/2, butSize.height/2));
    right->addChild(rights);
    rights->setVisible(false);
    
    listBg = Layout::create();
    listBg->setContentSize(Size(bgW - 10, bg->getContentSize().height - 240));
    listBg->ignoreAnchorPointForPosition(false);
    listBg->setPosition(Vec2(5, 10));
    bg->addChild(listBg);
    
    showOne();
    return true;
}

void TaskLayer::showOne()
{
    listBg->removeAllChildren();
    OneTaskLayer *one = OneTaskLayer::create();
    one->setAnchorPoint(Vec2(0, 0));
    one->setPosition(Vec2::ZERO);
    listBg->addChild(one);
}

void TaskLayer::showGrow()
{
    listBg->removeAllChildren();
    GrowTaskLayer *grow = GrowTaskLayer::create();
    grow->setAnchorPoint(Vec2(0, 0));
    grow->setPosition(Vec2::ZERO);
    listBg->addChild(grow);
}

void TaskLayer::tabClickFun(Ref *pSender)
{
    Layout *but = (Layout*)pSender;
    if(but == lastBut)
    {
        return;
    }
    
    if (but == left)
    {
        ImageView *n1 = (ImageView*)left->getChildByTag(200);
        ImageView *s1 = (ImageView*)left->getChildByTag(201);
        Text *t1 = (Text*)left->getChildByTag(202);
        n1->setVisible(false);
        s1->setVisible(true);
        t1->setTextColor(Color4B(0x1a, 0x1a, 0x1a, 255));
        
        ImageView *n2 = (ImageView*)right->getChildByTag(200);
        ImageView *s2 = (ImageView*)right->getChildByTag(201);
        Text *t2 = (Text*)right->getChildByTag(202);
        
        n2->setVisible(true);
        s2->setVisible(false);
        t2->setTextColor(Color4B(0xf0, 0xcc, 0x5b, 255));
        
        showOne();
        
    }else if (but == right)
    {
        ImageView *n1 = (ImageView*)left->getChildByTag(200);
        ImageView *s1 = (ImageView*)left->getChildByTag(201);
        Text *t1 = (Text*)left->getChildByTag(202);
        n1->setVisible(true);
        s1->setVisible(false);
        t1->setTextColor(Color4B(0xf0, 0xcc, 0x5b, 255));
        
        ImageView *n2 = (ImageView*)right->getChildByTag(200);
        ImageView *s2 = (ImageView*)right->getChildByTag(201);
        Text *t2 = (Text*)right->getChildByTag(202);
        
        n2->setVisible(false);
        s2->setVisible(true);
        t2->setTextColor(Color4B(0x1a, 0x1a, 0x1a, 255));
        
        showGrow();
    }
    
    lastBut = but;
}