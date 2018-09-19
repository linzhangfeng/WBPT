//
//  SettingGame.cpp
//  ZJH
//
//  Created by mac on 2017/1/2.
//
//

#include "SettingGame.hpp"

USING_NS_CC;

bool SettingGame::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(SettingGame::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    initSetting();
    return true;
}

void SettingGame::initSetting()
{
    Size winSize = Director::getInstance()->getWinSize();
    Size bgSize = Size(598 + 34, 600);
    
    ImageView *mask = ImageView::create("GameHall/setting_bg.png");
    mask->setTouchEnabled(true);
    mask->addClickEventListener([=](Ref *ref)
                                {
                                    
                                });
    mask->setPosition(winSize/2);
    this->addChild(mask);
    
    ImageView* bgTitle = ImageView::create("GameHall/bg_title.png");
    bgTitle->setAnchorPoint(Vec2(0.5,0.5));
    bgTitle->setPosition(Vec2(mask->getContentSize().width / 2,mask->getContentSize().height));
    mask->addChild(bgTitle);
    
    auto closeBtn = Button::create("GameHall/joinRoom/c1.png", "GameHall/joinRoom/c2.png");
    closeBtn->setAnchorPoint(Vec2(0.5,0.5));
    closeBtn->setPosition(Vec2(mask->getContentSize().width - 10, mask->getContentSize().height - 10));
    closeBtn->addClickEventListener([=](Ref*){
        this->removeFromParentAndCleanup(true);
    });
    mask->addChild(closeBtn);
    
    ImageView* gameVoiceIcon = ImageView::create("GameHall/game_voice_icon.png");
    gameVoiceIcon->setAnchorPoint(Vec2(0.5,0.5));
    gameVoiceIcon->setPosition(Vec2(178,mask->getContentSize().height - 170));
    mask->addChild(gameVoiceIcon);
    
    ImageView* gameVoiceText = ImageView::create("GameHall/game_voice_text.png");
    gameVoiceText->setAnchorPoint(Vec2(0.5,0.5));
    gameVoiceText->setPosition(Vec2(352,mask->getContentSize().height - 170));
    mask->addChild(gameVoiceText);
    
    Slider *soundSlider = Slider::create("GameHall/progress_bg.png", "GameHall/progress_point.png");
    soundSlider->loadProgressBarTexture("GameHall/progress_fg.png");
    soundSlider->setZoomScale(0);
    soundSlider->setMaxPercent(100);
    soundSlider->setPercent(ZJHModel::getInstance()->sound * 100);
    soundSlider->setAnchorPoint(Vec2(0,0.5));
    soundSlider->setPosition(Vec2(510, mask->getContentSize().height - 170));
    soundSlider->addEventListener([=](Ref *pSender, ui::Slider::EventType type)
                                  {
                                      if (type == ui::Slider::EventType::ON_SLIDEBALL_UP) {
                                          ui::Slider *slider = dynamic_cast<Slider *>(pSender);
                                          ZJHModel::getInstance()->sound = slider->getPercent() / 100.0;
                                          SimpleAudioEngine::getInstance()->setEffectsVolume(ZJHModel::getInstance()->sound);
                                          UserDefault::getInstance()->setDoubleForKey("sound", ZJHModel::getInstance()->sound);
                                          UserDefault::getInstance()->flush();
                                          
                                      }
                                  });
    mask->addChild(soundSlider);
    
    ImageView* gameMusicIcon = ImageView::create("GameHall/game_music_icon.png");
    gameMusicIcon->setAnchorPoint(Vec2(0.5,0.5));
    gameMusicIcon->setPosition(Vec2(173,mask->getContentSize().height - 304));
    mask->addChild(gameMusicIcon);
    
    ImageView* gameMusicText = ImageView::create("GameHall/game_music_text.png");
    gameMusicText->setAnchorPoint(Vec2(0.5,0.5));
    gameMusicText->setPosition(Vec2(352,mask->getContentSize().height - 304));
    mask->addChild(gameMusicText);
    
    Slider *musicSlider = Slider::create("GameHall/progress_bg.png", "GameHall/progress_point.png");
    musicSlider->setZoomScale(0);
    musicSlider->loadProgressBarTexture("GameHall/progress_fg.png");
    musicSlider->setMaxPercent(100);
    musicSlider->setPercent(ZJHModel::getInstance()->bgMusic * 100);
    musicSlider->setAnchorPoint(Vec2(0,0.5));
    musicSlider->setPosition(Vec2(510, 304));
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
    mask->addChild(musicSlider);
    ImageView* gameQuikIcon = ImageView::create("GameHall/game_zhengdong_icon.png");
    gameQuikIcon->setAnchorPoint(Vec2(0.5,0.5));
    gameQuikIcon->setPosition(Vec2(178,mask->getContentSize().height - 418));
    mask->addChild(gameQuikIcon);
    
    ImageView* gameQuikText = ImageView::create("GameHall/game_zhengdong_text.png");
    gameQuikText->setAnchorPoint(Vec2(0.5,0.5));
    gameQuikText->setPosition(Vec2(352,mask->getContentSize().height - 418));
    mask->addChild(gameQuikText);
    
    CheckBox *vibrateSwitch = CheckBox::create();
    vibrateSwitch->setZoomScale(0);
    if(ZJHModel::getInstance()->vibrate){
        vibrateSwitch->loadTextureBackGround("GameHall/turn_on.png");
    }else{
        vibrateSwitch->loadTextureBackGround("GameHall/turn_off.png");
    }
    
    vibrateSwitch->setAnchorPoint(Vec2(0.5,0.5));
    vibrateSwitch->setPosition(Vec2(870, mask->getContentSize().height - 418));
    vibrateSwitch->setSelected(ZJHModel::getInstance()->vibrate);
    
    vibrateSwitch->addEventListener([](Ref* pSender,CheckBox::EventType type)
                                    {
                                        CheckBox *checkBox = dynamic_cast<CheckBox *>(pSender);
                                        if (type == CheckBox::EventType::SELECTED)
                                        {
                                            ZJHModel::getInstance()->vibrate = true;
                                            checkBox->loadTextureBackGround("GameHall/turn_on.png");
                                            PlatformHelper::sdkDoForVoid_C("setVibrate");
                                            UserDefault::getInstance()->setBoolForKey("vibrate", true);
                                            UserDefault::getInstance()->flush();
                                        }
                                        else if (type == CheckBox::EventType::UNSELECTED)
                                        {
                                            ZJHModel::getInstance()->vibrate = false;
                                            checkBox->loadTextureBackGround("GameHall/turn_off.png");
                                            UserDefault::getInstance()->setBoolForKey("vibrate", false);
                                            UserDefault::getInstance()->flush();
                                        }
                                    });
    mask->addChild(vibrateSwitch);
}

bool SettingGame::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
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
