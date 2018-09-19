//
//  RoomPublic.hpp
//  ZJH
//
//  Created by mac on 16/9/8.
//
//

#ifndef RoomPublic_hpp
#define RoomPublic_hpp

#include "cocos2d.h"
#include "ZJH.h"

USING_NS_CC;

/**
 @brief 对战场的顶部
 */
class RoomPublic : public Layer
{
public:
    CREATE_FUNC(RoomPublic);
    virtual bool init();
    void buttonCallback(Ref *sender);
    
    void checkGameEndRepeat(float dt);
    
    void setOnlineNumber(int value);
    void setGameStatus(int status);//status 0：未开始 1：进行中 2：已结束
    void updateTimeLabel(float dt);
    void menuCallback(std::string name);
    void checkGameEnd();
    void notificationRoomStart(Ref *msg);
    void notificationPlayerNumber(Ref *msg);
    void notificationGameResume(Ref *msg);
    void notificationGameStart(Ref *msg);
    virtual void onExit();
    void setMyVisible(string name);
    void refreshMenu();
    void requestExit();
    void setRequestFlag(bool flag);
    void showPlayerList();
    CC_SYNTHESIZE(MyCallBack_Str, m_pCallback, Callback);
    void checkEndRoomTip(Ref *msg = NULL);
    void checkEndRoomTip_getData(float dt);
    void powerSigChange(float d);
    void getCurrentTime(float d);
    void inviteShare();//邀请好友分享到微信
private:
    float scaleRate;
    int m_aginstStatus;//游戏状态 0：未开始，1：正在进行，2：已结束
    int m_duration;//游戏总时间，单位为分钟
    int m_againstNumber;//总人数
    long m_remain;//剩余游戏时间，单位为秒
    int m_type;
    
    Json::Value m_jsonData;
    
    Button *menuMessageBtn;
    Button *menuZhanjiBtn;
    Button *menuShowBt;
    Button *menuMoreBtn;
    ImageView* powIcon;
    Text* cTime;//当前时间
    bool needRequestExist;//退出时是否要上报业务服务器

//    ImageView* phoneSigIcon;
};
#endif /* RoomPublic_hpp */
