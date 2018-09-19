//
//  RoomSetting.cpp
//  ZJH
//
//  Created by hyhy on 16/7/25.
//
//

#include "RoomSetting.h"

USING_NS_CC;

bool RoomSetting::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomSetting::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void RoomSetting::initSetting()
{
    Size winSize = Director::getInstance()->getWinSize();
    Size bgSize = Size(598 + 34, 600);
    
    LayerColor *mask = LayerColor::create(Color4B(0, 0, 0, 100));
    this->addChild(mask);
    
    Sprite *dialog = Utils::createDialogBG(bgSize.width, bgSize.height);
    dialog->setPosition(winSize / 2);
    dialog->setName("dialog");
    this->addChild(dialog);
    
    Sprite *titleSp = Sprite::create("Room/dialog/v_dialog_title_bg.png");
    titleSp->setPosition(Vec2(bgSize.width  /2, bgSize.height - 60));
    dialog->addChild(titleSp);
    
    Sprite *bottomSp = Sprite::create("Room/dialog/v_alert_dialog_bottom_bg.png");
    bottomSp->setPosition(Vec2(bgSize.width / 2, 54));
    dialog->addChild(bottomSp);
    
    Button *closeBtn = Button::create("Room/dialog/v_dialog_close.png", "Room/dialog/v_dialog_close_press.png");
    closeBtn->setPosition(Vec2(bgSize.width - 55, bgSize.height - 55));
    closeBtn->setTag(ALERT_DIALOG_BUTTON_CLOSE);
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->removeFromParentAndCleanup(true);
                                    });
    dialog->addChild(closeBtn);
    
    Label *titleLabel = Label::createWithSystemFont("游戏设置", "Arial", 36);
    titleLabel->setPosition(titleSp->getPosition());
    dialog->addChild(titleLabel);
    
    float posX = 115;
    float posY = 440;
    float deltaY = 95;
    Label *soundLabel = Label::createWithSystemFont("游戏音效", "Arial", 32);
    soundLabel->setPosition(Vec2(posX, posY));
    dialog->addChild(soundLabel);
    posY -= deltaY;
    
    Slider *soundSlider = Slider::create("Public/v_progress_bg.png", "Public/v_progress_point.png");
    soundSlider->loadProgressBarTexture("Public/v_progress_fg.png");
    soundSlider->setZoomScale(0);
    soundSlider->setMaxPercent(100);
    soundSlider->setPercent(ZJHModel::getInstance()->sound * 100);
    soundSlider->setPosition(Vec2(370, soundLabel->getPositionY()));
    soundSlider->addEventListener([=](Ref *pSender, ui::Slider::EventType type)
                                  {
                                      if (type == ui::Slider::EventType::ON_SLIDEBALL_UP) {
                                          ui::Slider *slider = dynamic_cast<Slider *>(pSender);
                                          
                                          int slidTemp;
                                          if (slider->getPercent() >= 25 && slider->getPercent() < 50){
                                              
                                              slidTemp = 25;
                                              
                                          }else if (slider->getPercent() >= 50 && slider->getPercent() < 75){
                                              
                                              slidTemp = 75;
                                              
                                          }else if (slider->getPercent() >= 75 && slider->getPercent() <= 100){
                                              
                                              slidTemp = 100;
                                              
                                          }else{
                                              
                                              slidTemp = slider->getPercent();
                                          }
                                          
                                          ZJHModel::getInstance()->sound = slidTemp / 100.0;
                                          SimpleAudioEngine::getInstance()->setEffectsVolume(ZJHModel::getInstance()->sound);
                                          UserDefault::getInstance()->setDoubleForKey("sound", ZJHModel::getInstance()->sound);
                                          UserDefault::getInstance()->flush();
                                          
                                      }
                                  });
    dialog->addChild(soundSlider);
    
    Label *musicLabel = Label::createWithSystemFont("背景音乐", "Arial", 32);
    musicLabel->setPosition(Vec2(posX, posY));
    dialog->addChild(musicLabel);
    posY -= deltaY;
    
    Slider *musicSlider = Slider::create("Public/v_progress_bg.png", "Public/v_progress_point.png");
    musicSlider->setZoomScale(0);
    musicSlider->loadProgressBarTexture("Public/v_progress_fg.png");
    musicSlider->setMaxPercent(100);
    musicSlider->setPercent(ZJHModel::getInstance()->bgMusic * 100);
    musicSlider->setPosition(Vec2(soundSlider->getPositionX(), musicLabel->getPositionY()));
    musicSlider->addEventListener([=](Ref *pSender, ui::Slider::EventType type)
                                  {
                                      if (type == ui::Slider::EventType::ON_SLIDEBALL_UP) {
                                          ui::Slider *slider = dynamic_cast<Slider *>(pSender);
                                          ZJHModel::getInstance()->bgMusic = slider->getPercent() / 100.0;
                                          log("%0.2f",ZJHModel::getInstance()->bgMusic);
                                          SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(ZJHModel::getInstance()->bgMusic);
                                          UserDefault::getInstance()->setDoubleForKey("bgMusic", ZJHModel::getInstance()->bgMusic);
                                          UserDefault::getInstance()->flush();
                                          
                                      }
                                  });
    dialog->addChild(musicSlider);
    
    
    int roomType = ZJHModel::getInstance()->curGameType;
    if (roomType == ROOM_TYPE::ROOM_TYPE_ZJH_DZ || roomType == ROOM_TYPE_ZJH_DZ_SET)
    {
        Label *foldLabel = Label::createWithSystemFont("弃牌提示", "Arial", 32);
        foldLabel->setPosition(Vec2(posX, posY));
        dialog->addChild(foldLabel);
        posY -= deltaY;
        
        bool foldTip = UserDefault::getInstance()->getBoolForKey("fold_tip", true);
        CheckBox *foldSwitch = CheckBox::create();
        foldSwitch->setZoomScale(0);
        foldSwitch->loadTextureBackGround("Public/v_check_bg.png");
        foldSwitch->setPosition(Vec2(370, foldLabel->getPositionY()));
        foldSwitch->setSelected(foldTip);
        
        Sprite *checkBtn = Sprite::create("Public/v_check_btn.png");
        checkBtn->setPosition(Vec2(foldTip ? 110 : 37, 26.5));
        checkBtn->setName("check_button");
        foldSwitch->addChild(checkBtn, 1);
        
        foldSwitch->addEventListener([](Ref* pSender,CheckBox::EventType type)
                                     {
                                         CheckBox *checkBox = dynamic_cast<CheckBox *>(pSender);
                                         Sprite *sp = dynamic_cast<Sprite *>(checkBox->getChildByName("check_button"));
                                         if (type == CheckBox::EventType::SELECTED)
                                         {
                                             sp->runAction(MoveTo::create(0.05, Vec2(110, 26.5)));
                                             UserDefault::getInstance()->setBoolForKey("fold_tip", true);
                                             UserDefault::getInstance()->flush();
                                         }
                                         else if (type == CheckBox::EventType::UNSELECTED)
                                         {
                                             sp->runAction(MoveTo::create(0.05, Vec2(37, 26.5)));
                                             UserDefault::getInstance()->setBoolForKey("fold_tip", false);
                                             UserDefault::getInstance()->flush();
                                         }
                                     });
        dialog->addChild(foldSwitch);
    }

    Label *vibrateLabel = Label::createWithSystemFont("震动反馈", "Arial", 32);
    vibrateLabel->setPosition(Vec2(posX, posY));
    dialog->addChild(vibrateLabel);
    posY -= deltaY;
    
    CheckBox *vibrateSwitch = CheckBox::create();
    vibrateSwitch->setZoomScale(0);
    vibrateSwitch->loadTextureBackGround("Public/v_check_bg.png");
    vibrateSwitch->setPosition(Vec2(370, vibrateLabel->getPositionY()));
    vibrateSwitch->setSelected(ZJHModel::getInstance()->vibrate);
    
    Sprite *vibrateBtn = Sprite::create("Public/v_check_btn.png");
    vibrateBtn->setPosition(Vec2(ZJHModel::getInstance()->vibrate ? 110 : 37, 26.5));
    vibrateBtn->setName("check_button");
    vibrateSwitch->addChild(vibrateBtn, 1);
    
    vibrateSwitch->addEventListener([](Ref* pSender,CheckBox::EventType type)
                                 {
                                     CheckBox *checkBox = dynamic_cast<CheckBox *>(pSender);
                                     Sprite *sp = dynamic_cast<Sprite *>(checkBox->getChildByName("check_button"));
                                     if (type == CheckBox::EventType::SELECTED)
                                     {
                                         sp->runAction(MoveTo::create(0.05, Vec2(110, 26.5)));
                                         ZJHModel::getInstance()->vibrate = true;
                                         PlatformHelper::sdkDoForVoid_C("setVibrate");
                                         UserDefault::getInstance()->setBoolForKey("vibrate", true);
                                         UserDefault::getInstance()->flush();
                                     }
                                     else if (type == CheckBox::EventType::UNSELECTED)
                                     {
                                         sp->runAction(MoveTo::create(0.05, Vec2(37, 26.5)));
                                         ZJHModel::getInstance()->vibrate = false;
                                         UserDefault::getInstance()->setBoolForKey("vibrate", false);
                                         UserDefault::getInstance()->flush();
                                     }
                                 });
    dialog->addChild(vibrateSwitch);
}

bool RoomSetting::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
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

void RoomSetting::buttonClicked(cocos2d::Ref *sender)
{
    //
}
