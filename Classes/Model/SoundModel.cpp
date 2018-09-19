//
//  SoundModel.cpp
//  ZJH
//
//  Created by 陈家勇 on 13-12-16.
//
//

#include "SoundModel.h"

SoundModel *soundModel;

SoundModel* SoundModel::getInstance()
{
    if (soundModel == NULL) {
        soundModel = new SoundModel();
        soundModel->init();
        soundModel->retain();
    }
    
    return soundModel;
}

int SoundModel::init()
{
    reset();
    cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(SoundModel::checkNext),this,0.3,false);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(SoundModel::onChatStopEvent), "chat_sound_play_over_notify", NULL);
    return 0;
}

void SoundModel::reset()
{
    can_play = 1;
    playIng = false;
}

void SoundModel::clear()
{
    playFile.clear();
}

void SoundModel::checkNext(float dt)
{
    if (playFile.size() == 0)
    {
        if (playIng && !PlatformHelper::isPlaying())
        {
            playIng = false;
            ZJHModel::getInstance()->isPause = 0;
            SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
        }
        return;
    }
    if (PlatformHelper::isPlaying() || ZJHModel::getInstance()->isPause == 1)
    {
        return;
    }
    playIng = true;
    cur_playfile = playFile[0];
    playFile.pop_front();
    if (can_play == 1){
        PlatformHelper::playRecord(cur_playfile);
        ZJHModel::getInstance()->isPause = 1;
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
}

void SoundModel::onChatStopEvent(Ref* arg)
{
    ZJHModel::getInstance()->isPause = 0;
}


