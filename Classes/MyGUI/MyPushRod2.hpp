//
//  MyPushRod2.hpp
//  ZJH
//
//  Created by wuyu on 16/1/27.
//
//

#ifndef MyPushRod2_hpp
#define MyPushRod2_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "MyButton.h"

using namespace std;
using namespace cocos2d;

class MyPushRod2 : public cocos2d::Layer
{
public:
    static MyPushRod2* createWithFile(std::string ball_pic, std::string up_pic, std::string down_pic,bool swallowTouches = true);

    bool _initWithFile(std::string ball_pic, std::string up_pic, std::string down_pic,bool swallowTouches = true);
    
    void setCallback(const ccBtnCallback& callback);
    
    bool _isTouchInside(Touch* touch);
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    // callback
    ccBtnCallback _callback;
    
    void updateBall(int dy);
    void RestPoint();
    void showEffect(bool v);
    
    void runToDown();
    
public:
    Sprite* BallSprite;//球炳
    Sprite* UpSprite;//上杆
    Sprite* effectSprite;//上杆
    
    cocos2d::Point curPos;
    int isStop;
};
#endif /* MyPushRod2_hpp */
