//
//解散房间提示
//

#include "EndRoomTipsDialog.h"

EndRoomTipsDialog::EndRoomTipsDialog()
    :m_pCallback(NULL)
{
    
}

bool EndRoomTipsDialog::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(EndRoomTipsDialog::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    
    Size winSize = Director::getInstance()->getWinSize();
    
//    auto layout = Layout::create();
//    layout->setContentSize(winSize);
//    layout->setAnchorPoint(Vec2(0,0));
//    layout->setPosition(Vec2(0,0));
//    layout->setTouchEnabled(true);
//    layout->setSwallowTouches(true);
//    layout->addClickEventListener([=](Ref*){
//    });
//    addChild(layout,0);
    
    LayerColor *mask = LayerColor::create(Color4B(0, 0, 0, 100));
    this->addChild(mask);
    
    
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
    
    return true;
}

void EndRoomTipsDialog::init1()
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
        ok->addClickEventListener(CC_CALLBACK_1(EndRoomTipsDialog::buttonClicked, this));
        
        Button* no = Button::create("RoomPublicH/end/no_apply_bt.png","RoomPublicH/end/no_apply_bt_press.png");
        no->setScale9Enabled(true);
        no->setAnchorPoint(Vec2(1,0));
        no->setPosition(Vec2(size.width-40,135));
        no->setVisible(true);
        no->setName("no");
        bg->addChild(no,1000);
        no->addClickEventListener(CC_CALLBACK_1(EndRoomTipsDialog::buttonClicked, this));
    }
}

void EndRoomTipsDialog::init2(std::string titleTips,std::string content,bool isHome)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    auto title = Label::createWithSystemFont(titleTips, "Thonburi", 30);
   
    auto content_label = Label::createWithSystemFont(content, "Thonburi", 30);
    
    float minWidth = title->getContentSize().width > 570 ? title->getContentSize().width : 570;
    
    float minHeight = content_label->getContentSize().height > 150 ? content_label->getContentSize().height : 150;
    Size size = Size(120 + minWidth, 320 + minHeight);
    
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
    title->setPosition(Vec2(size.width / 2, size.height - 110));
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
    
    {   if(isHome)
        {
            Button* ok = Button::create("RoomPublicH/end/end01.png","RoomPublicH/end/end02.png");
            ok->setScale9Enabled(true);
            ok->setAnchorPoint(Vec2(0,0));
            ok->setPosition(Vec2(40,70));
            ok->setVisible(true);
            ok->setName("ok");
            bg->addChild(ok,1000);
            ok->addClickEventListener(CC_CALLBACK_1(EndRoomTipsDialog::buttonClicked, this));
        }else{
            Button* ok = Button::create("RoomPublicH/end/tuichu01.png","RoomPublicH/end/tuichu02.png");
            ok->setScale9Enabled(true);
            ok->setAnchorPoint(Vec2(0,0));
            ok->setPosition(Vec2(40,70));
            ok->setVisible(true);
            ok->setName("exit");
            bg->addChild(ok,1000);
            ok->addClickEventListener(CC_CALLBACK_1(EndRoomTipsDialog::buttonClicked, this));
        }

        
        Button* no = Button::create("RoomPublicH/end/continu01.png","RoomPublicH/end/continu02.png");
        no->setScale9Enabled(true);
        no->setAnchorPoint(Vec2(1,0));
        no->setPosition(Vec2(size.width-40,70));
        no->setVisible(true);
        no->setName("no");
        bg->addChild(no,1000);
        no->addClickEventListener(CC_CALLBACK_1(EndRoomTipsDialog::buttonClicked, this));
    }
}

void EndRoomTipsDialog::buttonClicked(cocos2d::Ref *ref)
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
        } else if(name == "exit"){
            if (m_pCallback)
            {
                m_pCallback("exit");
            }
            this->removeFromParentAndCleanup(true);
        }
        else if (name == "no")
        {
            this->removeFromParentAndCleanup(true);
        }
    }
}

bool EndRoomTipsDialog::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
//    Scale9Sprite *dialog = dynamic_cast<Scale9Sprite *>(this->getChildByName("dialog"));
//    if (!dialog->getBoundingBox().containsPoint(touch->getLocation()))
//    {
//        this->removeFromParentAndCleanup(true);
//        event->stopPropagation();
//    }
    
    return true;
}
