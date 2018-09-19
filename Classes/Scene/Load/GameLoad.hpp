//
//  GameLoad.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef GameLoad_hpp
#define GameLoad_hpp

#include "ZJH.h"

USING_NS_CC;

class GameLoad : public Layer
{
    enum GameLoad_State
    {
        State_login,
        State_GameConfig,
        State_loadres,
        State_loadover,
        State_error,
        State_checkUpdate,
    };
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(GameLoad);
    void setState(int state);
    bool init();
    void loading(float dt);
    
    void doLoginSucc();
    
    void threePostLogin(float dt);
    
    //登录请求
    void loadConnect(Ref*ref);
    
    void dealAppUpdateIos();
    void onExit();
    
    void shagnBaoActivated();
    
    //微信登陆3天重新登录
    void WXLoadThreeDaysReLoad();
    
private:
    int state;
    
    long long intoTime;
    long long endTime;
    int loginRetryTimes = 0;//登陆重试次数
};

#endif /* GameLoad_hpp */
