//
//  MyNotification.cpp
//  ZJH
//
//  Created by 陈家勇 on 13-12-16.
//
//

/*
 主要为了解决跨线程之间的问题
 */
#include "MyNotification.h"

MyNotification *_MyNotification;
std::mutex _ContainerMutex;
MyNotification* MyNotification::getInstance()
{
    if (_MyNotification == NULL) {
        _MyNotification = new MyNotification();
        _MyNotification->init();
        _MyNotification->retain();
    }
    
    return _MyNotification;
}

int MyNotification::init()
{
    cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(MyNotification::post),this,0.0,false);
    return 0;
}

void MyNotification::postNotify(std::string file,Ref* data)
{
    std::lock_guard<std::mutex> ul(_ContainerMutex);
    auto msg = new XMessage(file,data);
    msgs.push_back(msg);
    log("MyNotification::postNotify[%s]",file.c_str());
}

void MyNotification::post(float dt)
{
    if (msgs.size() <= 0)return;
    std::lock_guard<std::mutex> ul(_ContainerMutex);
    auto msg = msgs.front();
    msg->handerMessage();
    
    vector< XMessage* >::iterator k = msgs.begin();
    msgs.erase(k);
    
    delete msg;
    log("MyNotification::post");
}

XMessage::XMessage( const std::string &msgName,cocos2d::Ref* msgContent )
:_msgName(msgName),_msgContent(msgContent)
{
    if(_msgContent)_msgContent->retain();
}

XMessage::~XMessage()
{
    if(_msgContent)_msgContent->release();
}

void XMessage::handerMessage()
{
    __NotificationCenter::getInstance()->postNotification(_msgName, _msgContent);
}