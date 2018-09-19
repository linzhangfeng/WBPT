//
//  RoomManagement2.hpp
//  ZJH
//
//  Created by pg on 16/9/8.
//
//

#ifndef RoomManagement2_hpp
#define RoomManagement2_hpp

#include "ZJH.h"

class RoomManagement2 : public Layer
{
public:
    CREATE_FUNC(RoomManagement2);
    virtual bool init();
    void myInit(Json::Value &data);
    
    void show();
    void hide();
    
    bool onTouchBegan(Touch* touch, Event* event);
    Vec2 m_nPosition;
};

#endif /* RoomManagement2_hpp */
