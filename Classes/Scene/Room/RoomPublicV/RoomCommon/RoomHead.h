//
// RoomHead.h
//  ZJH
//
//  Created by Apple on 16-3-11.
//
//

#ifndef ZJH_RoomHead_h
#define ZJH_RoomHead_h

#include "cocos2d.h"
#include "ZJH.h"
#include "RoomCMConfig.h"

using namespace cocos2d;

class RoomHead : public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(RoomHead);
    void btnCallback(int tag);
    void RoomBtnCallBack(int tag);
    void setCallback(const MyCallBack_Int &callback);
private:
    MyCallBack_Int callback;
    bool    isMenuDown;
};


#endif
