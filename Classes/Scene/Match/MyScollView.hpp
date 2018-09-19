//
//  MyScollView.hpp
//  ZJH
//
//  Created by admin on 16/3/24.
//
//

#ifndef MyScollView_hpp
#define MyScollView_hpp

#include "ZJH.h"

class ScollItem : public Layer
{
public:
    
    CREATE_FUNC(ScollItem);
    bool init();
    ScollItem* preItem;
    ScollItem* nextItem;
};

class MyScollView : public Layer
{
public:
    enum MyListView_State
    {
        State_idle = 0,
        State_move,
        State_automove,
        State_autoretri,
        State_moveToIndex,
    };
    
    enum MyListView_Type
    {
        Type_Repeat = 1,
        Type_Normal = 2,
    };
    
    CREATE_FUNC(MyScollView);
    bool init();
    void initView(int viewW, int viewH, int itemW, int itemH, int scollType);
    
    ScollItem* getItemByIndex(int index){return allItem[index];};
    
    int addItem(ScollItem *item);
    void reflash(int index);
    
    bool isTouchInside(Touch* touch);
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    
    void update(float dt);
    
    void move(int dy);
    
    bool isStop(){return state == State_idle;};
    void setRunCallback(const MyCallBack_Int &callback){this->runCallback = callback;};
    void setStopCallback(const MyCallBack_Int &callback){this->stopCallback = callback;};
    void setClickCallback(const MyCallBack_Int &callback){this->clickCallback = callback;};
    void toIndex(int index);
    int getNext(ScollItem* curItem,ScollItem* toItem);
    int getPre(ScollItem* curItem,ScollItem* toIte);
    
    int getCurIndex(){return curIndex;};
public:
    std::map<int, ScollItem*> allItem;
    int itemNums;
    int viewW;
    int viewH;
    int itemW;
    int itemH;
    
    cocos2d::Point curPoint;
    
    int state;
    ClippingNode* clp;
    float retriY;
    int retriP;
    int retriV;
    
    int curIndex;
    int tIndex;
    MyCallBack_Int runCallback;
    MyCallBack_Int stopCallback;
    MyCallBack_Int clickCallback;
    bool waitMove;
    long long beginTime;
    cocos2d::Point beginPoint;
    
    int type;
};

#endif /* MyScollView_hpp */
