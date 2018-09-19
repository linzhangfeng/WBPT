//
//  EndRoomDialog.cpp
//  ZJH
//
//  Created by hyhy on 16/8/4.
//
//

#include "EndRoomDialog.h"

EndRoomDialog::EndRoomDialog()
    :m_pCallback(NULL)
{
    
}

bool EndRoomDialog::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
    auto layout = Layout::create();
    layout->setContentSize(winSize);
    layout->setAnchorPoint(Vec2(0,0));
    layout->setPosition(Vec2(0,0));
    layout->setTouchEnabled(true);
    layout->setSwallowTouches(true);
    layout->addClickEventListener([=](Ref*){
    });
    addChild(layout,0);
    
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = [=](EventKeyboard::KeyCode keycode,Event* event)
    {
        if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
        {
            this->removeFromParent();
        }
        event->stopPropagation();//
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    
//    Size size = Size(629,319);
//    
//    auto bg = ui::Scale9Sprite::create("RoomPublic/common/popup_window.png");
//    bg->setPreferredSize(size);
//    bg->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
//    addChild(bg);
//    
//    auto line = Sprite::create("RoomPublic/common/line.png");
//    line->setScaleX(size.width / line->getContentSize().width);
//    line->setPosition(Vec2(size.width / 2, size.height - 76));
//    bg->addChild(line);
//    
//    auto title = Label::createWithSystemFont("申请解散房间确认", "Thonburi", 40);
//    title->setAnchorPoint(Vec2(0.5, 0.5));
//    title->setPosition(Vec2(size.width / 2, size.height - 76 / 2));
//    title->setColor(Color3B(240, 203, 91));
//    bg->addChild(title);
//    
//    Button *closeBtn = Button::create("Room/cancel_icon_yellow.png", "Room/cancel_icon_yellow_check.png");
//    closeBtn->setPosition(Vec2(size.width - 20, size.height - 15));
//    closeBtn->setScale(0.67);
//    closeBtn->addClickEventListener([=](Ref *ref)
//                                    {
//                                        this->stopAllActions();
//                                        this->removeFromParentAndCleanup(true);
//                                    });
//    bg->addChild(closeBtn);
//    
//    {
//        auto timeout_tip = Label::createWithSystemFont("*解散房间需要征集所有游戏玩家同意", "Thonburi", 30);
//        timeout_tip->setPosition(Vec2(size.width / 2, 46));
//        timeout_tip->setColor(Color3B(225,205,173));
//        bg->addChild(timeout_tip);
//    }
//    
//    {
//        Button* ok = Button::create("RoomPublic/jiesan/btn_yellow.png");
//        ok->setScale9Enabled(true);
//        ok->setContentSize(Size(Size(273, 53)));
//        ok->setTitleText("确认申请");
//        ok->setTitleFontSize(30);
//        ok->setTitleColor(Color3B(0,0,0));
//        ok->setPosition(Vec2(size.width / 2 - 151,size.height / 2));
//        ok->setVisible(true);
//        ok->setName("ok");
//        bg->addChild(ok,1000);
//        ok->addClickEventListener(CC_CALLBACK_1(EndRoomDialog::buttonClicked, this));
//        
//        Button* no = Button::create("RoomPublic/jiesan/btn_red_big.png");
//        no->setScale9Enabled(true);
//        no->setContentSize(Size(Size(273, 53)));
//        no->setTitleText("不申请");
//        no->setTitleFontSize(30);
//        no->setTitleColor(Color3B(255,255,255));
//        no->setPosition(Vec2(size.width / 2 + 151,size.height / 2));
//        no->setVisible(true);
//        no->setName("no");
//        bg->addChild(no,1000);
//        no->addClickEventListener(CC_CALLBACK_1(EndRoomDialog::buttonClicked, this));
//    }
    return true;
}

void EndRoomDialog::init1()
{
    Size winSize = Director::getInstance()->getWinSize();
    
    Size size = Size(613, 480);
    
    auto bg = Scale9Sprite::create("RoomPublicH/dialog/room_dialog_small.png");
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    bg->setPosition(winSize / 2);
    bg->setContentSize(Size(size.width, size.height));
    bg->setName("dialog");
    this->addChild(bg);
    
    auto titleSp = ImageView::create("RoomPublicH/end/dialog_tips_title.png");
    titleSp->setScale9Enabled(true);
    titleSp->setAnchorPoint(Vec2(0.5, 1));
    titleSp->setPosition(Vec2(size.width/2, size.height-10));
    bg->addChild(titleSp);
    
    Button *closeBtn = Button::create("RoomPublicH/dialog/dialog_close.png", "RoomPublicH/dialog/dialog_close_press.png");
    closeBtn->setAnchorPoint(Vec2(1, 1));
    closeBtn->setPosition(Vec2(size.width - 18, size.height - 60));
    closeBtn->setTag(ALERT_DIALOG_BUTTON_CLOSE);
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->removeFromParentAndCleanup(true);
                                    });
    bg->addChild(closeBtn);
    
    auto title = Label::createWithSystemFont("申请解散房间确认", "Thonburi", 30);
    title->setAnchorPoint(Vec2(0.5, 1));
    title->setPosition(Vec2(size.width / 2, size.height - 150-10));
    title->setColor(Color3B(0x4d, 0x2d, 0x1e));
    bg->addChild(title);
    
    {
        auto timeout_tip = Label::createWithSystemFont("*解散房间需要征集所有游戏玩家同意", "Thonburi", 24);
        timeout_tip->setAnchorPoint(Vec2(0.5,0));
        timeout_tip->setPosition(Vec2(size.width / 2, 44+35));
        timeout_tip->setColor(Color3B(0xaf,0x5a,0x22));
        bg->addChild(timeout_tip);
    }
    
    {
        Button* ok = Button::create("RoomPublicH/end/apply_bt.png","RoomPublicH/end/apply_bt_press.png");
        ok->setScale9Enabled(true);
        ok->setAnchorPoint(Vec2(0,0));
        ok->setPosition(Vec2(40,135));
        ok->setVisible(true);
        ok->setName("ok");
        bg->addChild(ok,1000);
        ok->addClickEventListener(CC_CALLBACK_1(EndRoomDialog::buttonClicked, this));
        
        Button* no = Button::create("RoomPublicH/end/no_apply_bt.png","RoomPublicH/end/no_apply_bt_press.png");
        no->setScale9Enabled(true);
        no->setAnchorPoint(Vec2(1,0));
        no->setPosition(Vec2(size.width-40,135));
        no->setVisible(true);
        no->setName("no");
        bg->addChild(no,1000);
        no->addClickEventListener(CC_CALLBACK_1(EndRoomDialog::buttonClicked, this));
    }
}

void EndRoomDialog::init2(std::string content)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    auto title = Label::createWithSystemFont("发现相同IP玩家，是否解散房间?", "Thonburi", 30);
    auto content_label = Label::createWithSystemFont(content, "Thonburi", 30);
    
    Size size = Size(613, 320 + content_label->getContentSize().height);
    
    auto bg = Scale9Sprite::create("RoomPublicH/dialog/room_dialog_small.png");
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    bg->setPosition(winSize / 2);
    bg->setContentSize(Size(size.width, size.height));
    bg->setName("dialog");
    this->addChild(bg);
    
    auto titleSp = ImageView::create("RoomPublicH/end/dialog_tips_title.png");
    titleSp->setScale9Enabled(true);
    titleSp->setAnchorPoint(Vec2(0.5, 1));
    titleSp->setPosition(Vec2(size.width/2, size.height-10));
    bg->addChild(titleSp);
    
    content_label->setAnchorPoint(Vec2(0.5, 0.5));
    content_label->setPosition(Vec2(size.width / 2, size.height / 2));
    content_label->setColor(Color3B(0xaf,0x5a,0x22));
    bg->addChild(content_label);
    
    title->setAnchorPoint(Vec2(0.5, 1));
    title->setPosition(Vec2(size.width / 2, size.height - 100));
    title->setColor(Color3B(0x4d, 0x2d, 0x1e));
    bg->addChild(title);
    
    {
        Button *closeBtn = Button::create("RoomPublicH/dialog/dialog_close.png", "RoomPublicH/dialog/dialog_close_press.png");
        closeBtn->setAnchorPoint(Vec2(1, 1));
        closeBtn->setPosition(Vec2(size.width - 18, size.height - 60));
        closeBtn->setTag(ALERT_DIALOG_BUTTON_CLOSE);
        closeBtn->addClickEventListener([=](Ref *ref)
                                        {
                                            this->removeFromParentAndCleanup(true);
                                        });
        bg->addChild(closeBtn);
    }
    
    {
        Button* ok = Button::create("RoomPublicH/end/end01.png","RoomPublicH/end/end02.png");
        ok->setScale9Enabled(true);
        ok->setAnchorPoint(Vec2(0,0));
        ok->setPosition(Vec2(40,70));
        ok->setVisible(true);
        ok->setName("ok");
        bg->addChild(ok,1000);
        ok->addClickEventListener(CC_CALLBACK_1(EndRoomDialog::buttonClicked, this));
        
        Button* no = Button::create("RoomPublicH/end/continu01.png","RoomPublicH/end/continu02.png");
        no->setScale9Enabled(true);
        no->setAnchorPoint(Vec2(1,0));
        no->setPosition(Vec2(size.width-40,70));
        no->setVisible(true);
        no->setName("no");
        bg->addChild(no,1000);
        no->addClickEventListener(CC_CALLBACK_1(EndRoomDialog::buttonClicked, this));
    }
}

void EndRoomDialog::buttonClicked(cocos2d::Ref *ref)
{
    Node *sender = dynamic_cast<Node *>(ref);
    if (sender)
    {
        std::string name = sender->getName();
        if (name == "ok")
        {
            if (m_pCallback)
            {
                m_pCallback("end_confirm");
            }
            this->removeFromParentAndCleanup(true);
        }
        else if (name == "no")
        {
            this->removeFromParentAndCleanup(true);
        }
    }
}
