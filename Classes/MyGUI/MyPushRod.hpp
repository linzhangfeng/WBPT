//
//  MyPushRod.hpp
//  ZJH
//
//  Created by wuyu on 16/1/27.
//
//

#ifndef MyPushRod_hpp
#define MyPushRod_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "MyButton.h"

using namespace std;
using namespace cocos2d;

class MyPushRod : public cocos2d::Layer
{
public:
    
    static MyPushRod* createWithSpriteFrame(std::string ball_pic, std::string up_pic, std::string down_pic,float isangle=0,bool swallowTouches = true);
    static MyPushRod* createWithFile(std::string ball_pic, std::string up_pic, std::string down_pic,float isangle=0,bool swallowTouches = true);
    
    bool _initWithSpriteFrame(std::string ball_pic, std::string up_pic, std::string down_pic,float isangle,bool swallowTouches = true);
    bool _initWithFile(std::string ball_pic, std::string up_pic, std::string down_pic,float isangle,bool swallowTouches = true);
    void setCallback(const ccBtnCallback& callback);
    
    bool _isTouchInside(Touch* touch);
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    void RestPoint();
    void RunAction();
    // callback
    ccBtnCallback _callback;
public:
    Sprite* BallSprite;//球炳
    Sprite* UpSprite;//上杆
    Sprite* DownSprite;//上杆
    cocos2d::Point InitPoint;
    float height;//高度
    float ball_dx;//
    float _y;//球与杆相差高度
    bool isEnd;
    float angle;
    bool isRun;//
};
#endif /* MyPushRod_hpp */
