//
// 实时战绩
//
//

#ifndef RoomRealTimeScore_h
#define RoomRealTimeScore_h

#include "ZJH.h"

class RoomRealTimeScore : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomRealTimeScore);
    virtual bool init();
    void myInit();
    
    void show();
    void hide();
    ui::Widget *createItem(Json::Value &itemData);
    void inviteFriend(Ref *sender);
    
    void getData(float dt);
    LayerColor* initTips(Json::Value &data);
    void initListView(Json::Value &data);
    
    bool onTouchBegan(Touch* touch, Event* event);
    void addLayerTouchEvent();
    
private:
    int m_hostUid;
    int m_roomId;
    EventListenerTouchOneByOne *touchListener;
    
    Layout *scrollView;
    //单个提示的总高度
    int tipsItemHeight = 28;
    //绘制第一个提示的离scrollview顶部的偏移量
    int cursorHeight = 0;
};

#endif /* defined(RoomRealTimeScore_h) */
