//
//  ChatRecordEffect.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "ChatRecordEffect.hpp"
#include "../../Model/MsgModel.h"

bool ChatRecordEffect::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    setName("ChatRecordEffect");
    
    Size size = Director::getInstance()->getWinSize();
    
    recordbg = Sprite::create("Chat/record/tt_sound_volume_default_bk.png");
    recordbg->setPosition(size / 2);
    recordbg->setVisible(false);
    addChild(recordbg);
    
    recording = Sprite::create("Chat/record/tt_sound_volume_01.png");
    recording->setAnchorPoint(Vec2(0, 0));
    recording->setPosition(Vec2(257,142));
    recordbg->addChild(recording);
    
    Animate* disappear = NULL;
    int disappearf[7] = {1,2,3,4,5,6,7};
    disappear = Utils::getCCAnimate("Chat/record/tt_sound_volume_0%d.png", 7, disappearf,0.2);
    recording->runAction(RepeatForever::create(disappear));
    
    error_tooshort = Sprite::create("Chat/record/tt_sound_volume_short_tip_bk.png");
    error_tooshort->setPosition(size / 2);
    error_tooshort->setVisible(false);
    addChild(error_tooshort);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatRecordEffect::recordStart), "chat_sound_record_start_notify", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatRecordEffect::recordIng), "chat_sound_record_ing_notify", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatRecordEffect::recordOver), "chat_sound_record_over_notify", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatRecordEffect::recordStop), "chat_sound_record_stop_notify", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatRecordEffect::recordError), "chat_sound_record_error_notify", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatRecordEffect::recordCancel), "chat_sound_record_cancel_notify", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatRecordEffect::recordTimeOut), "chat_sound_record_timeout_notify", NULL);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatRecordEffect::playNoFile), "chat_sound_play_nofile_notify", NULL);
    contentType = CONTENT_POINT_TO_POINT;
    return true;
}

void ChatRecordEffect::playNoFile(Ref* r)
{
    PlatformHelper::showToast("文件已经不存在");
}

void ChatRecordEffect::recordStart(Ref* r)
{
    recordbg->setVisible(true);
    error_tooshort->setVisible(false);
    log("ChatRecordEffect::recordStart");
    ZJHModel::getInstance()->isPause = 1;
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void ChatRecordEffect::recordIng(Ref* r)
{
    __Integer* v = (__Integer*)r;
    int value = v->getValue();
    log("ChatRecordEffect::recordIng[%d]",value);
}

void ChatRecordEffect::recordOver(Ref* r)
{
    ZJHModel::getInstance()->isPause = 0;
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    recordbg->setVisible(false);
    __String* str = (__String*)r;
    Json::Value json = Utils::ParseJsonStr(str->getCString());
    int time = json["time"].asInt();
    std::string filename = json["filename"].asString();
    log("ChatRecordEffect::recordOver[%d]",time);
    if (time > 0)
    {
        if (time < 1) time = 1;
        CallFunc* cl = CallFunc::create([=](){
            if (ZJHModel::getInstance()->enterGame)
            {
                MsgModel::getInstance()->sendSound_group(filename, time);
            }
            else
            {
                MsgModel::getInstance()->sendSound(filename, time,contentType);
            }
        });
        runAction(cl);
    }
    else
    {
        error_tooshort->setVisible(true);
        error_tooshort->runAction(Sequence::create(DelayTime::create(1),Hide::create(),NULL));
    }
}

void ChatRecordEffect::recordStop(Ref* r)
{
    recordbg->setVisible(false);
    log("ChatRecordEffect::recordStop");
}

void ChatRecordEffect::recordError(Ref* r)
{
    recordbg->setVisible(false);
    log("ChatRecordEffect::recordError");
}

void ChatRecordEffect::recordCancel(Ref* r)
{
    recordbg->setVisible(false);
    log("ChatRecordEffect::recordCancel");
}

void ChatRecordEffect::recordTimeOut(Ref* r)
{
    recordbg->setVisible(false);
    PlatformHelper::stopRecord();
    log("ChatRecordEffect::recordTimeOut");
}

void ChatRecordEffect::onExit()
{
    Layer::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}
