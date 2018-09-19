//
//  AlertDialogFold.cpp
//  ZJH
//
//  Created by hyhy on 16/8/8.
//
//

#include "AlertDialogFold.h"

USING_NS_CC;

bool AlertDialogFold::init()
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
    
    Label *titleLabel = Label::createWithSystemFont("弃牌提示", "Arial", 36);
    titleLabel->setPosition(titleSp->getPosition());
    dialog->addChild(titleLabel);
    
    Label *contentLabel = Label::createWithSystemFont("建议先看牌再弃牌", "Arial", 32);
    contentLabel->setPosition(Vec2(bgSize.width / 2, 280));
    
    bool foldTip = UserDefault::getInstance()->getBoolForKey("fold_tip", true);
    CheckBox *switchBox = CheckBox::create("Room/dialog/dialog_checkbox_normal.png", "Room/dialog/dialog_checkbox_selected.png");
    switchBox->setPosition(Vec2(200, 215));
    switchBox->setSelected(!foldTip);
    switchBox->addEventListener([=](Ref* ref,CheckBox::EventType type)
                                {
                                    if (type == CheckBox::EventType::SELECTED)
                                    {
                                        UserDefault::getInstance()->setBoolForKey("fold_tip", false);
                                        UserDefault::getInstance()->flush();
                                    }
                                    else if (type == CheckBox::EventType::UNSELECTED)
                                    {
                                        UserDefault::getInstance()->setBoolForKey("fold_tip", true);
                                        UserDefault::getInstance()->flush();
                                    }
                                });
    dialog->addChild(switchBox);
    
    Label *tipLabel = Label::createWithSystemFont("以后不需要这个提示", "Arial", 24);
    tipLabel->setAnchorPoint(Vec2(0, 0.5));
    tipLabel->setPosition(Vec2(switchBox->getPositionX() + 35, switchBox->getPositionY()));
    dialog->addChild(tipLabel);
    
    Button *confirmBtn = Button::create("Room/dialog/v_alert_dialog_btn_green.png", "Room/dialog/v_alert_dialog_btn_green_press.png");
    confirmBtn->setPosition(Vec2(bgSize.width * 0.25, 130));
    confirmBtn->setName("fold_confirm");
    confirmBtn->addClickEventListener(CC_CALLBACK_1(AlertDialogFold::buttonClicked, this));
    confirmBtn->setTitleText("确认弃牌");
    confirmBtn->setTitleFontSize(32);
    dialog->addChild(confirmBtn);
    
    Button *cancelBtn = Button::create("Room/dialog/v_alert_dialog_btn_red.png", "Room/dialog/v_alert_dialog_btn_red_press.png");
    cancelBtn->setPosition(Vec2(bgSize.width * 0.75, confirmBtn->getPositionY()));
    cancelBtn->setName("fold_cancel");
    cancelBtn->addClickEventListener(CC_CALLBACK_1(AlertDialogFold::buttonClicked, this));
    cancelBtn->setTitleText("取消弃牌");
    cancelBtn->setTitleFontSize(32);
    dialog->addChild(cancelBtn);
    
    Button *closeBtn = Button::create("Room/dialog/v_dialog_close.png", "Room/dialog/v_dialog_close_press.png");
    closeBtn->setPosition(Vec2(bgSize.width - 55, bgSize.height - 55));
    closeBtn->setTag(ALERT_DIALOG_BUTTON_CLOSE);
    closeBtn->addClickEventListener(CC_CALLBACK_1(AlertDialogFold::buttonClicked, this));
    dialog->addChild(closeBtn);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(AlertDialogFold::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void AlertDialogFold::buttonClicked(cocos2d::Ref *sender)
{
    Button *btn = dynamic_cast<Button *>(sender);
    if (btn) {
        std::string name = btn->getName();
        
        if (m_pCallback) {
            m_pCallback(name);
        }
        this->removeFromParentAndCleanup(true);
    }
}

bool AlertDialogFold::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Sprite *dialog = dynamic_cast<Sprite *>(this->getChildByName("dialog"));
    if (!dialog->getBoundingBox().containsPoint(touch->getLocation())) {
        this->removeFromParentAndCleanup(true);
        event->stopPropagation();
    }
    
    return true;
}
