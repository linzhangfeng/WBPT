//
//  HallBottom.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "HallBottom.hpp"
#include "../Play/Play.hpp"

bool HallBottom::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size size = Director::getInstance()->getWinSize();
    
//    Sprite* bg_top = Sprite::create("Hall/bottom/bottom_tab_bar.png");
//    bg_top->setAnchorPoint(Vec2(0.5, 0));
//    bg_top->setPosition(size.width / 2, 0);
//    addChild(bg_top);
    
    ImageView *bg = ImageView::create("Hall/bottom/bottom_bar.png");
    bg->setContentSize(Size(size.width,163));
    bg->setScale9Enabled(true);
    bg->ignoreContentAdaptWithSize(false);
    bg->setAnchorPoint(Vec2(0.0f, 0.0f));
    bg->setPosition(Vec2(0, 0));
    addChild(bg, -100);
    
//    Sprite* bg_top = Sprite::create("Hall/bottom/bar_under_top.png");
//    bg_top->setAnchorPoint(Vec2(0.5, 0));
//    bg_top->setPosition(size.width / 2, 160);
//    addChild(bg_top);
    
    int dx = size.width / 5;
    int bx = dx / 2;
    int by = 76;
    
    int clickH = 160;
    int clickW = dx;
    
    {
        //1:社区
        Button* button = Button::create("Hall/bottom/icon_title_03.png",
                                        "Hall/bottom/icon_title_03_check.png",
                                        "Hall/bottom/icon_title_03_check.png");
        button->setPosition(Vec2(bx,by));
        button->setContentSize(Size(clickW,clickH));
        button->ignoreContentAdaptWithSize(false);
        button->setFillContentSizeEnabled(false);
        button->setName("msg");
        button->addClickEventListener(CC_CALLBACK_1(HallBottom::btnCallback, this));
        addChild(button);
        
        auto tip = Sprite::create("Msg/news.png");
        tip->setScale(0.5);
        tip->setVisible(false);
        tip->setPosition(Vec2(clickW / 2 + 45, clickH - 25));
        tip->setName("tip");
        button->addChild(tip);
        
        bx += dx;
    }
    
    {
        //2:房间
        Button* button = Button::create("Hall/bottom/icon_title_02.png",
                                        "Hall/bottom/icon_title_02_check.png",
                                        "Hall/bottom/icon_title_02_check.png");

        button->setPosition(Vec2(bx,by));
        button->setContentSize(Size(clickW,clickH));
        button->ignoreContentAdaptWithSize(false);
        button->setFillContentSizeEnabled(false);
        button->setName("play");
        button->addClickEventListener(CC_CALLBACK_1(HallBottom::btnCallback, this));
        
        addChild(button);
        
        auto tipNew = Sprite::create("Msg/news.png");
        tipNew->setScale(0.5);
        tipNew->setVisible(false);
        tipNew->setPosition(Vec2(clickW / 2 + 45, clickH - 25));
        tipNew->setName("tipNewRoom");
        button->addChild(tipNew);
        
        bx += dx;
    }
    
    {
        //3:对战
        Button* button = Button::create("Hall/bottom/icon_title_play.png",
                                        "Hall/bottom/icon_title_play_check.png",
                                        "Hall/bottom/icon_title_play_check.png");
        button->setPosition(Vec2(bx,by));
        button->addClickEventListener(CC_CALLBACK_1(HallBottom::btnCallback, this));
        button->setName("found");
        button->setEnabled(false);
        addChild(button);
        
        bx += dx;
    }
    
    {
        //4:商店
        Button* button = Button::create("Hall/bottom/icon_title_01.png",
                                        "Hall/bottom/icon_title_01_check.png",
                                        "Hall/bottom/icon_title_01_check.png");
        button->setPosition(Vec2(bx,by));
        button->setContentSize(Size(clickW,clickH));
        button->ignoreContentAdaptWithSize(false);
        button->setFillContentSizeEnabled(false);
        button->addClickEventListener(CC_CALLBACK_1(HallBottom::btnCallback, this));
        button->setName("dating");
        addChild(button);
        
        bx += dx;
    }
    
    {
        //5:我的
        Button* button = Button::create("Hall/bottom/icon_title_04.png",
                                        "Hall/bottom/icon_title_04_check.png",
                                        "Hall/bottom/icon_title_04_check.png");
        button->setPosition(Vec2(bx,by));
        button->setContentSize(Size(clickW,clickH));
        button->ignoreContentAdaptWithSize(false);
        button->setFillContentSizeEnabled(false);
        button->addClickEventListener(CC_CALLBACK_1(HallBottom::btnCallback, this));
        button->setName("myinfo");
        addChild(button);
        
        bx += dx;
    }
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(HallBottom::hasNewMsgNotify), "hasNewMsg_notify", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(HallBottom::noticeShowShopItem), "show_shop_item", NULL);
    
    
    return true;
}


void HallBottom::noticeShowShopItem(Ref* ref)
{
    Button* dating = (Button*)getChildByName("dating");
    Button* myinfo = (Button*)getChildByName("myinfo");
    if (dating != NULL) {
        dating->setEnabled(false);
    }
    
    if(myinfo != NULL) {
       myinfo->setEnabled(true);
    }
}

void HallBottom::btnCallback(Ref* ref)
{
    Button* btn = (Button*)ref;
    
//    if(ZJHModel::getInstance()->isCanPay() == false)
//    {
//        if (btn->getName() == "dating")
//        {
//            PlatformHelper::showToast("暂未开放，敬请期待");
//            return;
//        }
//    }
    
    
    if (callback)callback(btn->getName());
    
    Button* dating = (Button*)getChildByName("dating");
    Button* msg = (Button*)getChildByName("msg");
    Button* play = (Button*)getChildByName("play");
    Button* found = (Button*)getChildByName("found");
    Button* myinfo = (Button*)getChildByName("myinfo");
    
    if(dating)dating->setEnabled(true);
    if(msg)msg->setEnabled(true);
    if(play)play->setEnabled(true);
    if(found)found->setEnabled(true);
    if(myinfo)myinfo->setEnabled(true);
    
    
    if(btn)
    {
//        if(ZJHModel::getInstance()->isCanPay() == false)
//        {
//            if (btn->getName() != "dating")
//            {
//                btn->setEnabled(false);
//            }
//        }else
//        {
            btn->setEnabled(false);
        //}
        
    }
    
    if (btn->getName() == "msg")
    {
        msg->getChildByName("tip")->setVisible(false);
    }
    if (btn->getName() == "play")
    {
        play->getChildByName("tipNewRoom")->setVisible(false);
    }
    if (btn->getName() == "found") {
        //刷新免房间号房间
        __NotificationCenter::getInstance()->postNotification("refresh_room_state", __String::create(""));
    }
    
}

void HallBottom::setCallback(const MyCallBack_Str &callback)
{
    this->callback = callback;
}

void HallBottom::hasIngRoomNotice(Ref*ref)
{
    Button* room_btn = (Button*)getChildByName("play");
    if (room_btn->isEnabled() == false)return;
    
    room_btn->getChildByName("tipNewRoom")->setVisible(true);
}

void HallBottom::hasNewMsgNotify(Ref* msg)
{    
    Button* msg_btn = (Button*)getChildByName("msg");
    if (msg_btn->isEnabled() == false)return;
    
    msg_btn->getChildByName("tip")->setVisible(true);
}

void HallBottom::onExit()
{
    Layer::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}
