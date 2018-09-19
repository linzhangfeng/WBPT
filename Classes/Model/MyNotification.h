//
//  MyNotification.h
//  ZJH
//
//  Created by 陈家勇 on 13-12-16.
//
//

#ifndef ZJH_MyNotification_h
#define ZJH_MyNotification_h

#include <string>
#include <stdint.h>
#include "ZJH.h"

class XMessage
{
public:
    XMessage(const std::string &msgName,cocos2d::Ref* msgContent);
    ~XMessage();
    void handerMessage();
public:
    std::string _msgName;
    cocos2d::Ref* _msgContent;
};

class MyNotification : public cocos2d::Ref
{
public:
    /** Return the shared instance **/
    static MyNotification *getInstance();
    int init();
    void postNotify(std::string file,Ref* data = NULL);
    void post(float dt);
private:
    std::vector<XMessage*> msgs;
};

#endif
