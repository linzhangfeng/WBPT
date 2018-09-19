//
//  HallScene.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef HallScene_hpp
#define HallScene_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"
#include "DailyReward.h"
#include "Notice.h"

USING_NS_CC;

class HallScene : public BaseLayer
{
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(HallScene);
    bool init();
    
    void sendSignreward(bool flag);
    void initRewardView(Json::Value mValue);
    void initNoticeView(float dt);
    
    void reSignList(Ref* ref);
    void noticeCloseDaily(Ref* ref);
    void notifyClubReady(Ref* ref);

    void noticeShowShopItem(Ref*ref);
    void noticeDailyRecive(Ref*ref);
    
    void reflashOpenMatch(Ref*ref);
    
    void acountChangeReflash(Ref*ref);
    
    void querySystemMsg();
    
    void runHotUpdate(float dt);
    
    void onExit();
    
    void onEnter();
    
    void revoceryHall(string param);
    
    void addClubView();
    
    void downloadBootMusic();//下载开机音乐
    
    void downloading(float dt);
    
private:
    std::map<std::string, Node*> itemMap;
    std::map<std::string,bool> modules;
    EventListenerCustom* _listener;
    bool isHaveNotice;
};

#endif /* HallScene_hpp */
