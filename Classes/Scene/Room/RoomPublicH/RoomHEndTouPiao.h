//
//
//

#ifndef RoomHEndTouPiao_h
#define RoomHEndTouPiao_h

#include "cocos2d.h"
#include "ZJH.h"

USING_NS_CC;

/**
 @brief 对战场的顶部
 */
class RoomHEndTouPiao : public Layer
{
public:
    CREATE_FUNC(RoomHEndTouPiao);
    virtual bool init();
    void buttonCallback(Ref *sender);
    virtual void onExit();
    void initData(Json::Value data);
    void initWithJsonStr(const std::string& d);
    void updateState(int uid,int state);
    
    CC_SYNTHESIZE(MyCallBack_Str, m_pCallback, Callback);
    
    void updateTime(float dt);
    void setDefaultTips(const std::string tips);

private:
    void initOwner(Json::Value data);
    void initOther(Json::Value data);
    
    void updateOwner();
    void updateOther();
    
    Json::Value jsonData;
    int owner_id;
    int time_count;
    Label* timeLabel;
    std::map<int,Node*> no_Map;
    std::map<int,Node*> ok_Map;
    std::map<int,Node*> wait_Map;
    
    LabelAtlas* okLabel;
    LabelAtlas* noLabel;
    
    LabelTTF* okNameLabel;
    LabelTTF* noNameLabel;
    
    Button* ok;
    Button* no;
    
    Label* myTip;
    ImageView* myResult;
    
    Sprite *otherTimeBG;
    Label *timeout_tip;
};
#endif /* RoomHEndTouPiao_h */
