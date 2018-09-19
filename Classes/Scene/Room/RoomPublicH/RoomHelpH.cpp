//
//
// 横版游戏帮助
//
// 2016-11-01
//
//

#include "RoomHelpH.h"

USING_NS_CC;

bool RoomHelpH::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomHelpH::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

bool RoomHelpH::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
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

void RoomHelpH::initHelp(Json::Value &json)
{
    initHelpWeb();
}

void RoomHelpH::initHelpWeb()
{
    
    Size winSize = Director::getInstance()->getWinSize();
    Size bgSize = Size(890,630);
    
    
    LayerColor *mask = LayerColor::create(Color4B(0, 0, 0, 100));
    this->addChild(mask);
    
    auto *dialog = Scale9Sprite::create("RoomPublicH/dialog/dialog_help_bg.png");
    dialog->setAnchorPoint(Vec2(0.5, 0.5));
    dialog->setPosition(winSize / 2);
    dialog->setContentSize(Size(bgSize.width, bgSize.height));
    dialog->setName("dialog");
    this->addChild(dialog);
    
    
    auto *titleSp = ImageView::create("RoomPublicH/dialog/dialog_help_title.png");
    titleSp->setScale9Enabled(true);
    titleSp->setAnchorPoint(Vec2(0.5, 1));
    titleSp->setPosition(Vec2(bgSize.width/2, bgSize.height));
    dialog->addChild(titleSp);
    
    cocos2d::experimental::ui::WebView* webView = cocos2d::experimental::ui::WebView::create();
    webView->setAnchorPoint(Vec2(0, 0));
    webView->setPosition(Vec2(35, 33));
    webView->setContentSize(Size(bgSize.width-70,500));
    webView->setScalesPageToFit(true);
    log("============url==%s====",(ZJHModel::getInstance()->gameHelpUrl).c_str());
    webView->loadURL(ZJHModel::getInstance()->gameHelpUrl);
    dialog->addChild(webView);
    
    
    Button *closeBtn = Button::create("RoomPublicH/dialog/dialog_close.png", "RoomPublicH/dialog/dialog_close_press.png");
    closeBtn->ignoreContentAdaptWithSize(false);
    closeBtn->setScale9Enabled(true);
    closeBtn->setPosition(Vec2(bgSize.width - 65, bgSize.height - 68));
    closeBtn->setTag(ALERT_DIALOG_BUTTON_CLOSE);
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->removeFromParentAndCleanup(true);
                                    });
    dialog->addChild(closeBtn);
}

