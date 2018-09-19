//
//  AlertDialogDownTable.cpp
//  ZJH
//
//  Created by hyhy on 16/7/21.
//
//

#include "AlertDialogDownTable.h"

USING_NS_CC;

bool AlertDialogDownTable::init()
{
    if (Layer::init() == false) {
        return  false;
    }
    
    m_pCallback = NULL;
    
    LayerColor *mask = LayerColor::create(Color4B(0, 0, 0, 100));
    this->addChild(mask);
    
    Size winSize = Director::getInstance()->getWinSize();
    
    Size bgSize = Size(598 + 34, 450);
    Sprite *dialog =  Utils::createDialogBG(bgSize.width, bgSize.height);
    dialog->setPosition(winSize / 2);
    dialog->setName("dialog");
    this->addChild(dialog, 100);
    
    Sprite *titleSp = Sprite::create("Room/dialog/v_dialog_title_bg.png");
    titleSp->setPosition(Vec2(bgSize.width  /2, bgSize.height - 60));
    dialog->addChild(titleSp);
    
    Sprite *bottomSp = Sprite::create("Room/dialog/v_alert_dialog_bottom_bg.png");
    bottomSp->setPosition(Vec2(bgSize.width / 2, 54));
    dialog->addChild(bottomSp);
    
    Label *titleLabel = Label::createWithSystemFont("退座确认", "Arial", 36);
    titleLabel->setPosition(titleSp->getPosition());
    dialog->addChild(titleLabel);
    
    Label *contentLabel = Label::createWithSystemFont("您是想要离开这个座位？", "Arial", 32);
    contentLabel->setPosition(Vec2(bgSize.width / 2, 280));
    dialog->addChild(contentLabel);
    
    Button *confirmBtn = Button::create("Room/dialog/v_alert_dialog_btn_green.png", "Room/dialog/v_alert_dialog_btn_green_press.png");
    confirmBtn->setPosition(Vec2(bgSize.width * 0.25, 140));
    confirmBtn->setTag(ALERT_DIALOG_BUTTON_CONFIRM);
    confirmBtn->addClickEventListener(CC_CALLBACK_1(AlertDialogDownTable::buttonClicked, this));
    confirmBtn->setTitleText("确认");
    confirmBtn->setTitleFontSize(32);
    dialog->addChild(confirmBtn);
    
    Button *cancelBtn = Button::create("Room/dialog/v_alert_dialog_btn_red.png", "Room/dialog/v_alert_dialog_btn_red_press.png");
    cancelBtn->setPosition(Vec2(bgSize.width * 0.75, confirmBtn->getPositionY()));
    cancelBtn->setTag(ALERT_DIALOG_BUTTON_CANCEL);
    cancelBtn->addClickEventListener(CC_CALLBACK_1(AlertDialogDownTable::buttonClicked, this));
    cancelBtn->setTitleText("取消");
    cancelBtn->setTitleFontSize(32);
    dialog->addChild(cancelBtn);
    
    Button *closeBtn = Button::create("Room/dialog/v_dialog_close.png", "Room/dialog/v_dialog_close_press.png");
    closeBtn->setPosition(Vec2(bgSize.width - 55, bgSize.height - 55));
    closeBtn->setTag(ALERT_DIALOG_BUTTON_CLOSE);
    closeBtn->addClickEventListener(CC_CALLBACK_1(AlertDialogDownTable::buttonClicked, this));
    dialog->addChild(closeBtn);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(AlertDialogDownTable::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void AlertDialogDownTable::buttonClicked(cocos2d::Ref *sender)
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

bool AlertDialogDownTable::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Sprite *dialog = dynamic_cast<Sprite *>(this->getChildByName("dialog"));
    if (!dialog->getBoundingBox().containsPoint(touch->getLocation())) {
        this->removeFromParentAndCleanup(true);
        event->stopPropagation();
    }
    
    return true;
}
