//
//  RoomExitDialog.cpp
//  ZJH
//
//  Created by hyhy on 16/8/4.
//
//

#include "RoomExitDialog.h"

RoomExitDialog::RoomExitDialog()
    :m_pCallback(NULL)
{
    
}

void RoomExitDialog::show()
{
    this->setOpacity(0);
    this->runAction(FadeIn::create(0.17));
}

bool RoomExitDialog::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    this->setCascadeOpacityEnabled(true);
    
    Size winSize = Director::getInstance()->getWinSize();
    Size bgSize = Size(640, 260);
    
    LayerColor *mask = LayerColor::create(Color4B(0, 0, 0, 100));
    this->addChild(mask);
    
    Sprite *dialog = Sprite::create();
    dialog->setCascadeOpacityEnabled(true);
    dialog->setContentSize(bgSize);
    dialog->setPosition(winSize / 2);
    dialog->setName("dialog");
    this->addChild(dialog);
    
    Scale9Sprite *bg = Scale9Sprite::create("Room/bar_window_yellow.9.png");
    bg->setContentSize(bgSize);
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    bg->setPosition(bgSize / 2);
    dialog->addChild(bg);
    
    Label *title = Label::createWithSystemFont("提示", "Arial", 32);
    title->setPosition(Vec2(bgSize.width / 2, bgSize.height - 35));
    dialog->addChild(title);
    
    Label *tip = Label::createWithSystemFont("正在对战中，是否退出房间？", "Arial", 28);
    tip->setAnchorPoint(Vec2(0, 0.5));
    tip->setPosition(Vec2(48, 160));
    dialog->addChild(tip);
    
    ImageView *confirmBtn = ImageView::create("Room/btn_yellow_small.9.png");
    confirmBtn->setScale9Enabled(true);
    confirmBtn->setCascadeOpacityEnabled(true);
    confirmBtn->setContentSize(Size(200, 60));
    confirmBtn->setPosition(Vec2(bgSize.width * 0.25, 75));
    confirmBtn->setName("exit_confirm");
    confirmBtn->setTouchEnabled(true);
    confirmBtn->ignoreContentAdaptWithSize(false);
    confirmBtn->addClickEventListener(CC_CALLBACK_1(RoomExitDialog::buttonClicked, this));
    dialog->addChild(confirmBtn);
    
    Label *confirmLabel = Label::createWithSystemFont("退出房间", "Arial", 28);
    confirmLabel->setColor(Color3B::BLACK);
    confirmLabel->setPosition(confirmBtn->getContentSize()  /2);
    confirmBtn->addChild(confirmLabel);
    
    ImageView *cancelBtn = ImageView::create("Room/btn_yellow_small.9.png");
    cancelBtn->setCascadeOpacityEnabled(true);
    cancelBtn->setScale9Enabled(true);
    cancelBtn->setContentSize(Size(200, 60));
    cancelBtn->setPosition(Vec2(bgSize.width * 0.75, 75));
    cancelBtn->setName("exit_cancel");
    cancelBtn->setTouchEnabled(true);
    cancelBtn->ignoreContentAdaptWithSize(false);
    cancelBtn->addClickEventListener(CC_CALLBACK_1(RoomExitDialog::buttonClicked, this));
    dialog->addChild(cancelBtn);
    
    Label *cancelLabel = Label::createWithSystemFont("继续游戏", "Arial", 28);
    cancelLabel->setColor(Color3B::BLACK);
    cancelLabel->setPosition(cancelBtn->getContentSize()  /2);
    cancelBtn->addChild(cancelLabel);
    
    Button *closeBtn = Button::create("Room/cancel_icon_yellow.png", "Room/cancel_icon_yellow_check.png");
    closeBtn->setPosition(Vec2(bgSize.width - 20, bgSize.height - 20));
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->stopAllActions();
                                        this->removeFromParentAndCleanup(true);
                                    });
    dialog->addChild(closeBtn);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomExitDialog::onTouchBegan,this);
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

bool RoomExitDialog::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Node *dialog = dynamic_cast<Sprite *>(this->getChildByName("dialog"));
    if (dialog && dialog->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())))
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

void RoomExitDialog::buttonClicked(cocos2d::Ref *ref)
{
    Node *sender = dynamic_cast<Node *>(ref);
    if (sender) {
        std::string name = sender->getName();
        if (name == "exit_confirm")
        {
            if (m_pCallback) {
                m_pCallback("exit_confirm");
            }
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


