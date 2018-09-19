//
//  RoomVerticalLayout.hpp
//  ZJH
//
//  Created by mac on 2016/11/14.
//
//

#ifndef RoomVerticalLayout_hpp
#define RoomVerticalLayout_hpp


#include "cocos2d.h"
#include "ZJH.h"
class RoomBottom;
class RoomHeadFight;

USING_NS_CC;

//竖版UI统一入口
class RoomVerticalLayout : public Layer
{
public:
     MyCallBack_Str m_pCallback;
    CREATE_FUNC(RoomVerticalLayout);
    virtual bool init();
    void setGamePlayStatu(bool statu);
    void setFastSoundVis(bool satatu);
    void showPlayerList();
    void setRequestFlag(bool flag);
    void setCallback(const MyCallBack_Str callback);
    MyCallBack_Str getCallback();
private:
    RoomHeadFight *roomHead;
    RoomBottom* roomBottom;
   
};


#endif /* RoomVerticalLayout_hpp */
