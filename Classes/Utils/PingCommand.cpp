//
//  PingCommand.cpp
//  ZJH
//
//  Created by mac on 2016/12/20.
//
//

#include "PingCommand.hpp"
#include "cocos2d.h"
#include "ZJh.h"
#include "cocostudio/CocoStudio.h"
using namespace std ;

static PingCommand* command = nullptr;

PingCommand* PingCommand::getInstance(){
    if (!command)
    {
        command = new (std::nothrow) PingCommand();
        command->baiduRespTime = -1;
        command->lastTime = 0;
        command->interval = 0;
    }
    return command;
}

void PingCommand::getData(float dt){
    lastTime = Utils::getCurrentSecend();
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        if(loginPacket->httpCode == 200){
            baiduRespTime = Utils::getCurrentSecend() - lastTime;
        }else{
            baiduRespTime = -1;
        }
    },"http://www.baidu.com","","PingCommand");
}

void  PingCommand::start() {
    if(interval < 10){
        interval = 10;
    }
//    cocos2d::CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(PingCommand::getData),this,interval,CC_REPEAT_FOREVER,0,false);
}


