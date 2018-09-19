//
//  RoomHeadFight.h
//  ZJH
//
//  Created by hyhy on 16/7/23.
//
//

#ifndef __ZJH__RoomHeadFight__
#define __ZJH__RoomHeadFight__

#include "cocos2d.h"
#include "ZJH.h"

USING_NS_CC;

/**
 @brief 对战场的顶部
 */
class RoomHeadFight : public Layer
{
public:
    CREATE_FUNC(RoomHeadFight);
    virtual bool init();
    void buttonCallback(Ref *sender);
    
    void setOnlineNumber(int value);
    void setGameStatus(int status);//status 0：未开始 1：进行中 2：已结束
    void updateTimeLabel(float dt);
    void menuCallback(std::string name);
    void checkGameEnd();
    void notificationRoomStart(Ref *msg);
    void notificationPlayerNumber(Ref *msg);
    void notificationGameResume(Ref *msg);
    void notificationGameStart(Ref *msg);
    void removeShareButton(Ref *msg);
    void powerSigChange(float d);
    void showPlayerList();
    virtual void onExit();
    void checkGameEndRepeat(float dt);
    void refreshMenu();
    void requestExit();
    void setRequestFlag(bool flag);
    
    CC_SYNTHESIZE(MyCallBack_Str, m_pCallback, Callback);
    
    void checkEndRoomTip(Ref *msg = NULL);
    void checkEndRoomTip_getData(float dt);
private:
    float scaleRate;
    int m_aginstStatus;//游戏状态 0：未开始，1：正在进行，2：已结束
    int m_duration;//游戏总时间，单位为分钟
    float m_startTime;//游戏开始时间
    int m_againstNumber;//总人数
    long m_remain;//剩余游戏时间，单位为秒
    bool needRequestExist;//退出时是否要上报业务服务器
    ImageView* powIcon;
//    ImageView* phoneSigIcon;
    Json::Value m_jsonData;
};

#endif /* defined(__ZJH__RoomHeadFight__) */
