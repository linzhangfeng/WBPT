//
//  RoomManagement.h
//  ZJH
//
//  Created by hyhy on 16/7/26.
//
//

#ifndef __ZJH__RoomManagement__
#define __ZJH__RoomManagement__

#include "ZJH.h"

class RoomManagement : public Layer
{
public:
    CREATE_FUNC(RoomManagement);
    virtual bool init();
    void myInit(Json::Value &data);
    
    void show();
    void hide();
    
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif /* defined(__ZJH__RoomManagement__) */
