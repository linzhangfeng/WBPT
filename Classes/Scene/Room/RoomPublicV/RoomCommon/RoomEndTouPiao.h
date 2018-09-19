//
//  RoomEndTouPiao.hpp
//  ZJH
//
//  Created by mac on 16/9/8.
//
//

#ifndef RoomEndTouPiao_hpp
#define RoomEndTouPiao_hpp

#include "cocos2d.h"
#include "ZJH.h"

USING_NS_CC;

/**
 @brief 对战场的顶部
 */
class RoomEndTouPiao : public Layer
{
public:
    CREATE_FUNC(RoomEndTouPiao);
    virtual bool init();
    void buttonCallback(Ref *sender);
    virtual void onExit();
    void initData(Json::Value data);
    void initWithJsonStr(const std::string& d);
    void updateState(int uid,int state);
    
    CC_SYNTHESIZE(MyCallBack_Str, m_pCallback, Callback);
    
    void updateTime(float dt);

private:
    void initOwner(Json::Value data);
    void initOther(Json::Value data);
    
    void updateOwner();
    void updateOther();
    void setDefaultTips(const std::string tips);
    
    Label *timeout_tip;
    Json::Value jsonData;
    int owner_id;
    int time_count;
    Label* timeLabel;
    std::map<int,Node*> no_Map;
    std::map<int,Node*> ok_Map;
    std::map<int,Node*> wait_Map;
    
    Label* okLabel;
    Label* noLabel;
    
    LabelTTF* okNameLabel;
    LabelTTF* noNameLabel;
    
    Button* ok;
    Button* no;
    
    Label* myTip;
    Label* myResult;
};
#endif /* RoomEndTouPiao_hpp */
