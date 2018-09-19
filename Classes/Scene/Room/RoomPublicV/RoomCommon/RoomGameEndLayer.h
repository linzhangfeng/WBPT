//
//  RoomGameEndLayer.h
//  ZJH
//
//  Created by hyhy on 16/7/28.
//
//

#ifndef __ZJH__RoomGameEndLayer__
#define __ZJH__RoomGameEndLayer__

#include "ZJH.h"

class RoomGameEndLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomGameEndLayer);
    virtual bool init();
    void myInit(int endTime, int roomId);
    
    void show();
    void hide();
    
    bool onTouchBegan(Touch* touch, Event* event);
    void buttonClicked(Ref *sender);
private:
    int m_roomId;
};

#endif /* defined(__ZJH__RoomGameEndLayer__) */
