//
//  GamePlayingErrorDialog.cpp
//  ZJH
//
//  Created by mac on 2016/12/9.
//
//

#include "GamePlayingErrorDialog.hpp"
void GamePlayingErrorDialog::show(string c,string name,string logo)
{
    this->setOpacity(0);
    this->runAction(FadeIn::create(0.17));
    ZJHModel::getInstance()->loadingImageResource(gameLogo, logo);
    gameName->setString(name);
    code->setString("房间号:"+c);
}

bool GamePlayingErrorDialog::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    this->setCascadeOpacityEnabled(true);
    
    Size winSize = Director::getInstance()->getWinSize();
    Size bgSize = Size(721, 549);
    
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
    
    Label *title = Label::createWithSystemFont("提示", "Arial", 46);
    title->setColor(Color3B(0xee,0xcb,0x5c));
    title->setAnchorPoint(Vec2(0.5,1));
    title->setPosition(Vec2(bgSize.width / 2, bgSize.height - 35));
    dialog->addChild(title);
    
    Label *tip = Label::createWithSystemFont("你中途退出游戏,请从新加入", "Arial", 44);
    tip->setAnchorPoint(Vec2(0.5, 1));
    tip->setPosition(Vec2(bgSize.width / 2, bgSize.height - 135));
    dialog->addChild(tip);
    
    gameLogo = ImageView::create();
    gameLogo->setAnchorPoint(Vec2(0,0));
    gameLogo->setContentSize(Size(130,130));
    gameLogo->ignoreContentAdaptWithSize(false);
    gameLogo->setPosition(Vec2(96,183));
    dialog->addChild(gameLogo);
    
    gameName = Label::createWithSystemFont("", "Arial", 42);
    gameName->setColor(Color3B(0xee,0xcb,0x5c));
    gameName->setAnchorPoint(Vec2(0,0));
    gameName->setPosition(Vec2(250, 260));
    dialog->addChild(gameName);
    
    code = Label::createWithSystemFont("", "Arial", 42);
    code->setColor(Color3B(0xee,0xcb,0x5c));
    code->setAnchorPoint(Vec2(0,0));
    code->setPosition(Vec2(250, 195));
    dialog->addChild(code);
    
    
    ImageView *confirmBtn = ImageView::create("Room/btn_yellow_small.9.png");
    confirmBtn->setScale9Enabled(true);
    confirmBtn->setCascadeOpacityEnabled(true);
    confirmBtn->setContentSize(Size(610, 88));
    confirmBtn->setPosition(Vec2(bgSize.width/2, 25));
    confirmBtn->setName("enter");
    confirmBtn->setTouchEnabled(true);
    confirmBtn->setAnchorPoint(Vec2(0.5,0));
    confirmBtn->ignoreContentAdaptWithSize(false);
    confirmBtn->addClickEventListener(CC_CALLBACK_1(GamePlayingErrorDialog::buttonClicked, this));
    dialog->addChild(confirmBtn);
    
    Label *confirmLabel = Label::createWithSystemFont("快速加入", "Arial", 36);
    confirmLabel->setColor(Color3B(0x66,0x41,0x05));
    confirmLabel->setPosition(confirmBtn->getContentSize() / 2);
    confirmBtn->addChild(confirmLabel);
    
    Button *closeBtn = Button::create("Room/cancel_icon_yellow.png", "Room/cancel_icon_yellow_check.png");
    closeBtn->setPosition(Vec2(bgSize.width - 20, bgSize.height - 20));
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->stopAllActions();
                                        this->removeFromParentAndCleanup(true);
                                    });
    dialog->addChild(closeBtn);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(GamePlayingErrorDialog::onTouchBegan,this);
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

bool GamePlayingErrorDialog::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
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

void GamePlayingErrorDialog::buttonClicked(cocos2d::Ref *ref)
{
    Node *sender = dynamic_cast<Node *>(ref);
    if (sender) {
        std::string name = sender->getName();
        if (name == "enter")
        {
            if (m_pCallback) {
                m_pCallback("enter");
            }
            this->removeFromParentAndCleanup(true);
        }
        else if (name == "exit_cancel")
        {
            this->removeFromParentAndCleanup(true);
        }
    }
}
