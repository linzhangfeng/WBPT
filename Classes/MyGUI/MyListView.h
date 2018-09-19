//
//  MyListView.h
//  ZJH
//
//  Created by 陈家勇 on 14-6-19.
//
//

#ifndef __ZJH__MyListView__
#define __ZJH__MyListView__

#include <iostream>

#include "cocos2d.h"

using namespace std;
using namespace cocos2d;
class MyListView : public cocos2d::Layer
{
private:
    enum MyListView_State
    {
        State_idle = 0,
        State_waitmove,
        State_moverow,
        State_movecol,
        State_rowautomove,
        State_colautomove
    };
    
public:
    enum MyListView_Type
    {
        Type_row = 0,//竖直滚动
        Type_col,//水平滚动
        Type_row_col,//竖直＋水平 其中水平分页滚动
    };
    
public:
    static MyListView* create();
    
    //设置参数
    void setEnabled(bool enable){m_enable = enable;};
    void setScollBar(const char* buf);
    
    static Layer* createItem();

    //初始化
    void initView(int viewW, int viewH, int type);
    
    int addItem(Layer *item);
    
    float reflash(bool isResever);
    
    void removeItemByTag(int tag);
    void removeItemByIndex(int tag);
    void removeAllItem();
    
    Layer* getItemByTag(int tag);
    Layer* getItemByIndex(int tag);
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    
    void setScollPercent(float p);
    
    float getCurPositionX();
    float getCurPositionY();
    void initPositionX(float x);
    void initPositionY(float y);

    void onDownCallback(const ccMenuCallback& callback);
    void onUpCallback(const ccMenuCallback& callback);
    void onClickCallback(const ccMenuCallback& callback);
    void moveXY(float x, float y);
    float scollpercent;
private:
    bool init();
    void scollbarbegin();
    void scollbarmove();
    void scollbarend();
    
    void setScollbarVisi(bool value);
    bool isTouchInside(Touch* touch);
    void curItemDown(float dt);
    void checkItemClick(Touch* touch, bool isDown);
    
    void update(float dt);
    void updateOpirity(bool isColMove,int value);
    void checkoparity();
    
private:
    Layer* movinglayer;
    Layer* curItem;
    std::map<int, Layer*> allItem;

    int viewW;
    int viewH;
    int itemW;
    int itemH;
    int row;
    int col;
    int type;
    int nums;
    
    float firstTouchx;
    float firstTouchy;
    int touchTime;
    
    float preTouchx;
    float preTouchy;
    int state;
    int virtualDX;
    int virtualDY;
    float vx;
    float vy;
    
    bool isMove;
    bool m_enable;
    
    Sprite* mscollbar;
    float scollmaxy;
    int m_level;
    
    // callback
	ccMenuCallback _downcallback;
    
    // callback
	ccMenuCallback _upcallback;
    
    // callback
	ccMenuCallback _clickcallback;
};


#endif /* defined(__ZJH__MyListView__) */
