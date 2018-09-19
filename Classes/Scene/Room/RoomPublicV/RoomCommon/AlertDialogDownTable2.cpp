//
//  AlertDialogDownTable2.cpp
//  ZJH
//
//  Created by mac on 2016/11/7.
//
//

#include "AlertDialogDownTable2.hpp"

USING_NS_CC;

bool AlertDialogDownTable2::init()
{
    if (Layer::init() == false) {
        return  false;
    }
    
    m_pCallback = NULL;
    
    LayerColor *mask = LayerColor::create(Color4B(0, 0, 0, 100));
    this->addChild(mask);
    
    Size winSize = Director::getInstance()->getWinSize();
    
    
    Sprite *dialog =  Sprite::create("Room/dialog/dt_bg.png");
    Size bgSize = dialog->getContentSize();
    dialog->setPosition(winSize / 2);
    dialog->setName("dialog");
    this->addChild(dialog, 100);
    
    Sprite *titleSp = Sprite::create("Room/dialog/dt_title.png");
    titleSp->setPosition(Vec2(bgSize.width  /2, bgSize.height - 50));
    dialog->addChild(titleSp);
    
    
    Label *contentLabel = Label::createWithSystemFont("您是想要离开这个座位？", "Arial", 32);
    contentLabel->setPosition(Vec2(bgSize.width / 2, 280));
    contentLabel->setTextColor(Color4B(0x7c, 0x5e, 0x4a, 0xff));
    dialog->addChild(contentLabel);
    
    Button *confirmBtn = Button::create("Room/dialog/dt_sure1.png", "Room/dialog/dt_sure2.png");
    confirmBtn->setPosition(Vec2(40 + confirmBtn->getContentSize().width/2, 88 + confirmBtn->getContentSize().height/2));
    confirmBtn->setTag(ALERT_DIALOG_BUTTON_CONFIRM);
    confirmBtn->addClickEventListener(CC_CALLBACK_1(AlertDialogDownTable2::buttonClicked, this));
    dialog->addChild(confirmBtn);
    
    Button *cancelBtn = Button::create("Room/dialog/dt_cancel1.png", "Room/dialog/dt_cancel2.png");
    cancelBtn->setPosition(Vec2(bgSize.width - 40 - cancelBtn->getContentSize().width/2, confirmBtn->getPositionY()));
    cancelBtn->setTag(ALERT_DIALOG_BUTTON_CANCEL);
    cancelBtn->addClickEventListener(CC_CALLBACK_1(AlertDialogDownTable2::buttonClicked, this));
    dialog->addChild(cancelBtn);
    
    Button *closeBtn = Button::create("Room/dialog/dt_close1.png", "Room/dialog/dt_close2.png");
    closeBtn->setPosition(Vec2(bgSize.width - 18 - closeBtn->getContentSize().width/2, bgSize.height - 63 - closeBtn->getContentSize().height/2));
    closeBtn->setTag(ALERT_DIALOG_BUTTON_CLOSE);
    closeBtn->addClickEventListener(CC_CALLBACK_1(AlertDialogDownTable2::buttonClicked, this));
    dialog->addChild(closeBtn);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(AlertDialogDownTable2::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void AlertDialogDownTable2::buttonClicked(cocos2d::Ref *sender)
{
    Button *btn = dynamic_cast<Button *>(sender);
    if (btn) {
        int tag = btn->getTag();
        
        if (m_pCallback) {
            m_pCallback(tag);
        }
        this->removeFromParentAndCleanup(true);
    }
}

bool AlertDialogDownTable2::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Sprite *dialog = dynamic_cast<Sprite *>(this->getChildByName("dialog"));
    if (!dialog->getBoundingBox().containsPoint(touch->getLocation())) {
        this->removeFromParentAndCleanup(true);
        event->stopPropagation();
    }
    
    return true;
}