//
//  RoomSetting.cpp
//  ZJH
//
//  Created by hyhy on 16/7/25.
//
//

#include "RoomSettingH.h"

USING_NS_CC;

bool RoomSettingH::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomSettingH::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void RoomSettingH::initSetting()
{
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
    
    auto titleSp = ImageView::create("RoomPublicH/dialog/dialog_setting_title.png");
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
                                        this->removeFromParentAndCleanup(true);
                                    });
    dialog->addChild(closeBtn);
    
    Label *soundLabel = Label::createWithSystemFont("游戏音效", "Arial", 32);
    soundLabel->setTextColor(Color4B(0x4d, 0x2d, 0x1e, 255));
    soundLabel->setAnchorPoint(Vec2(0, 1));
    soundLabel->setPosition(Vec2(74, bgSize.height-145));
    dialog->addChild(soundLabel);
    
    
    Slider *soundSlider = Slider::create("RoomPublicH/dialog/progress_bg.png", "RoomPublicH/dialog/progress_point.png");
    soundSlider->loadProgressBarTexture("RoomPublicH/dialog/progress_fg.png");
    soundSlider->setZoomScale(0);
    soundSlider->setMaxPercent(100);
    soundSlider->setAnchorPoint(Vec2(0, 0.5));
    soundSlider->setPercent(ZJHModel::getInstance()->sound * 100);
    soundSlider->setPosition(Vec2(soundLabel->getContentSize().width +16, soundLabel->getContentSize().height/2));
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
    soundLabel->addChild(soundSlider);
    
    Label *musicLabel = Label::createWithSystemFont("背景音乐", "Arial", 32);
    musicLabel->setTextColor(Color4B(0x4d, 0x2d, 0x1e, 255));
    musicLabel->setAnchorPoint(Vec2(0,1));
    musicLabel->setPosition(Vec2(0, -45));
    soundLabel->addChild(musicLabel);
    
    Slider *musicSlider = Slider::create("RoomPublicH/dialog/progress_bg.png", "RoomPublicH/dialog/progress_point.png");
    musicSlider->setZoomScale(0);
    musicSlider->loadProgressBarTexture("RoomPublicH/dialog/progress_fg.png");
    musicSlider->setMaxPercent(100);
    musicSlider->setAnchorPoint(Vec2(0, 0.5));
    musicSlider->setPercent(ZJHModel::getInstance()->bgMusic * 100);
    musicSlider->setPosition(Vec2(musicLabel->getContentSize().width+16, musicLabel->getContentSize().height/2));
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

    musicLabel->addChild(musicSlider);
    

    Label *vibrateLabel = Label::createWithSystemFont("震动反馈", "Arial", 32);
    vibrateLabel->setTextColor(Color4B(0x4d, 0x2d, 0x1e, 255));
    vibrateLabel->setAnchorPoint(Vec2(0,1));
    vibrateLabel->setPosition(Vec2(0, -60));
    musicLabel->addChild(vibrateLabel);
    
    CheckBox *vibrateSwitch = CheckBox::create();
    vibrateSwitch->setZoomScale(0);
    vibrateSwitch->setAnchorPoint(Vec2(0, 0.5));
    vibrateSwitch->loadTextureBackGround("RoomPublicH/dialog/dialog_bt_check_bg.png");
    vibrateSwitch->setPosition(Vec2(vibrateLabel->getContentSize().width+16, vibrateLabel->getContentSize().height/2));
    vibrateSwitch->setSelected(ZJHModel::getInstance()->vibrate);
    
    Sprite *vibrateBtn = Sprite::create("RoomPublicH/dialog/dialog_check_slider.png");
    vibrateBtn->setPosition(Vec2(ZJHModel::getInstance()->vibrate ? 117 : 39, 24));
    vibrateBtn->setName("check_button");
    vibrateSwitch->addChild(vibrateBtn, 1);
    
    vibrateSwitch->addEventListener([](Ref* pSender,CheckBox::EventType type)
                                 {
                                     CheckBox *checkBox = dynamic_cast<CheckBox *>(pSender);
                                     Sprite *sp = dynamic_cast<Sprite *>(checkBox->getChildByName("check_button"));
                                     if (type == CheckBox::EventType::SELECTED)
                                     {
                                         sp->runAction(MoveTo::create(0.05, Vec2(117, 24)));
                                         ZJHModel::getInstance()->vibrate = true;
                                         PlatformHelper::sdkDoForVoid_C("setVibrate");
                                         UserDefault::getInstance()->setBoolForKey("vibrate", true);
                                         UserDefault::getInstance()->flush();
                                     }
                                     else if (type == CheckBox::EventType::UNSELECTED)
                                     {
                                         sp->runAction(MoveTo::create(0.05, Vec2(39, 24)));
                                         ZJHModel::getInstance()->vibrate = false;
                                         UserDefault::getInstance()->setBoolForKey("vibrate", false);
                                         UserDefault::getInstance()->flush();
                                     }
                                 });
    vibrateLabel->addChild(vibrateSwitch);
}

bool RoomSettingH::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Scale9Sprite *dialog = dynamic_cast<Scale9Sprite *>(this->getChildByName("dialog"));
    if (!dialog->getBoundingBox().containsPoint(touch->getLocation()))
    {
        this->removeFromParentAndCleanup(true);
        event->stopPropagation();
    }
    
    return true;
}

void RoomSettingH::buttonClicked(cocos2d::Ref *sender)
{
    //
}
