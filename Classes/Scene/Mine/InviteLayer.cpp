//
//  InviteLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/1.
//
//

#include "InviteLayer.hpp"
#include "MaskSelectLayer.hpp"
#include "InviteFriendLayer.hpp"
#include "TuiJianRenLayer.hpp"

bool InviteLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("推荐有奖");
    setName("InviteLayer");

    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x14, 0x14, 0x15, 255));
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    setHead(270);

    Button* btn1 = Button::create("Mine/invite_tab2_n.png","Mine/invite_tab2_s.png", "Mine/invite_tab2_s.png");
    btn1->setName("page1");
    btn1->setPosition(Vec2(visibleSize.width/2 -  btn1->getContentSize().width/2, visibleSize.height - getHeadH() + 90));
    btn1->addClickEventListener(CC_CALLBACK_1(InviteLayer::tabClickFun, this));
    this->addChild(btn1, 200);
    btn1->setEnabled(false);
    lastBut = btn1;
    
    Button* btn2 = Button::create("Mine/invite_tab1_n.png","Mine/invite_tab1_s.png","Mine/invite_tab1_s.png");
    btn2->setName("page2");
    btn2->setPosition(Vec2(visibleSize.width/2 + btn1->getContentSize().width/2 , btn1->getPositionY()));
    btn2->addClickEventListener(CC_CALLBACK_1(InviteLayer::tabClickFun, this));
    this->addChild(btn2, 200);
    receiveBtn2 = btn2;
    
    Size size = Size(visibleSize.width, visibleSize.height - getHeadH());
    
    Bglayout = Layout::create();
    Bglayout->setContentSize(size);
    this->addChild(Bglayout);

    showPage1();
    return true;
}

Button *InviteLayer::getReceiveBtn()
{
    return receiveBtn2;
}
void InviteLayer::showPage1()
{
    Bglayout->removeAllChildren();
    InviteFriendLayer *invite = InviteFriendLayer::create();
    invite->setAnchorPoint(Vec2(0, 0));
    invite->setPosition(Vec2::ZERO);
    Bglayout->addChild(invite);
}

void InviteLayer::showPage2()
{
    Bglayout->removeAllChildren();
    TuiJianRenLayer *tui = TuiJianRenLayer::create();
    tui->setAnchorPoint(Vec2(0, 0));
    tui->setPosition(Vec2::ZERO);
    Bglayout->addChild(tui);
}

void InviteLayer::tabClickFun(Ref* ref)
{
    Button *tab = (Button*)ref;
    if (tab==lastBut)
    {
        return;
    }
    Button *but1 = (Button*)this->getChildByName("page1");
    Button *but2 = (Button*)this->getChildByName("page2");
    
    if (tab == but1)
    {
        but1->setEnabled(false);
        but2->setEnabled(true);
        showPage1();
        
    }else if (tab == but2)
    {
        but1->setEnabled(true);
        but2->setEnabled(false);
        showPage2();
        
    }
    lastBut = tab;
}

void InviteLayer::inviteCodeInto(string code)
{
    Button *tab = receiveBtn2;
    
    Button *but1 = (Button*)this->getChildByName("page1");
    Button *but2 = (Button*)this->getChildByName("page2");
    
    if (tab == but2)
    {
        but1->setEnabled(true);
        but2->setEnabled(false);
        
        Bglayout->removeAllChildren();
        TuiJianRenLayer *tui = TuiJianRenLayer::create();
        tui->setAnchorPoint(Vec2(0, 0));
        tui->setPosition(Vec2::ZERO);
        Bglayout->addChild(tui);
        tui->inviteCodeInto(code);
        
    }
    lastBut = tab;
}


