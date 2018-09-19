//
//  横版退出房间提示
//
//

#include "RoomHExitDialog.h"

RoomHExitDialog::RoomHExitDialog()
    :m_pCallback(NULL)
{
    
}

void RoomHExitDialog::show()
{
//    this->setOpacity(0);
//    this->runAction(FadeIn::create(0.17));
//    this->setVisible(true);
}

bool RoomHExitDialog::init()
{
    if (!Layer::init()) {
        return false;
    }
    this->setName("roomHExitDialog");
    this->setCascadeOpacityEnabled(true);
    
    Size winSize = Director::getInstance()->getWinSize();
    Size bgSize = Size(613, 480);
    
    LayerColor *mask = LayerColor::create(Color4B(0, 0, 0, 100));
    this->addChild(mask);
    
    auto dialog = Scale9Sprite::create("RoomPublicH/dialog/room_dialog_small.png");
    dialog->setAnchorPoint(Vec2(0.5, 0.5));
    dialog->setPosition(winSize / 2);
    dialog->setContentSize(Size(bgSize.width, bgSize.height));
    dialog->setName("dialog");
    this->addChild(dialog);
    
    auto titleSp = ImageView::create("RoomPublicH/dialog/dialog_exit_title.png");
    titleSp->setScale9Enabled(true);
    titleSp->setAnchorPoint(Vec2(0.5, 1));
    titleSp->setPosition(Vec2(bgSize.width/2, bgSize.height-10));
    dialog->addChild(titleSp);
    
    Button *closeBtn = Button::create("RoomPublicH/dialog/dialog_close.png", "RoomPublicH/dialog/dialog_close_press.png");
    closeBtn->setAnchorPoint(Vec2(1, 1));
    closeBtn->setPosition(Vec2(bgSize.width - 18, bgSize.height - 60));
    closeBtn->setTag(ALERT_DIALOG_BUTTON_CLOSE);
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->stopAllActions();
                                        this->removeFromParentAndCleanup(true);
                                    });
    dialog->addChild(closeBtn);
    
    Label *tip = Label::createWithSystemFont("正在对战中，是否退出房间？", "Arial", 30);
    tip->setTextColor(Color4B(0x4d, 0x2d, 0x1e, 255));
    tip->setAnchorPoint(Vec2(0.5, 0));
    tip->setPosition(Vec2(bgSize.width/2, 236));
    dialog->addChild(tip);
    
    ImageView *confirmBtn = ImageView::create("RoomPublicH/dialog/exit_room_confirm.png");
    confirmBtn->setCascadeOpacityEnabled(true);
    confirmBtn->setAnchorPoint(Vec2(0,0));
    confirmBtn->setPosition(Vec2(40, 81));
    confirmBtn->setName("exit_confirm");
    confirmBtn->setTouchEnabled(true);
    confirmBtn->ignoreContentAdaptWithSize(false);
    confirmBtn->addClickEventListener(CC_CALLBACK_1(RoomHExitDialog::buttonClicked, this));
    dialog->addChild(confirmBtn);
    
    
    ImageView *cancelBtn = ImageView::create("RoomPublicH/dialog/exit_room_continue.png");
    cancelBtn->setCascadeOpacityEnabled(true);
    cancelBtn->setAnchorPoint(Vec2(1,0));
    cancelBtn->setPosition(Vec2(bgSize.width-40, 81));
    cancelBtn->setName("exit_cancel");
    cancelBtn->setTouchEnabled(true);
    cancelBtn->ignoreContentAdaptWithSize(false);
    cancelBtn->addClickEventListener(CC_CALLBACK_1(RoomHExitDialog::buttonClicked, this));
    dialog->addChild(cancelBtn);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomHExitDialog::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    
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

bool RoomHExitDialog::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Node *dialog = dynamic_cast<Sprite *>(this->getChildByName("dialog"));
    if (dialog && dialog->getBoundingBox().containsPoint(touch->getLocation()))
    {
        //
    }
    else
    {
        this->removeFromParentAndCleanup(true);
        event->stopPropagation();
    }
    
    return true;
}

void RoomHExitDialog::buttonClicked(cocos2d::Ref *ref)
{
    Node *sender = dynamic_cast<Node *>(ref);
    if (sender) {
        std::string name = sender->getName();
        if (name == "exit_confirm")
        {
            if (m_pCallback) {
                m_pCallback("exit_confirm");
            }
            CallFunc* func1 = CallFunc::create([=]
                                               {
                                                   ZJHModel::getInstance()->roomToHall();
                                               });
            runAction(Sequence::create(DelayTime::create(0.5),func1, NULL));
            Sound::getInstance()->StopBgSound();

        }
        else if (name == "exit_cancel")
        {
            if (m_pCallback) {
                m_pCallback("exit_cancel");
            }
            this->removeFromParentAndCleanup(true);
        }
    }
}


