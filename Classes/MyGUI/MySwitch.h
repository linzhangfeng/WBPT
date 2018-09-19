//
//  MySwitch.h
//  ZJH
//
//  Created by 陈家勇 on 13-12-17.
//
//

#ifndef ZJH_MySwitch_h
#define ZJH_MySwitch_h


#include "cocos2d.h"
#include "MyButton.h"

using namespace std;
using namespace cocos2d;

class MySwitch : public cocos2d::Layer
{
    enum MySwitch_Type
    {
        Type_frame = 0,
        Type_file = 1,
    };
    
public:
    
    static MySwitch* createWithSpriteFrame(std::string normal, std::string selected, std::string disabled = "",bool swallowTouches = true,bool on = true);
    static MySwitch* createWithFile(std::string normal, std::string selected, std::string disabled = "",bool swallowTouches = true,bool on = true);
    
    void setEnabled(bool enabled);
    void setCallback(const ccBtnCallback& callback);
    
    bool _initWithSpriteFrame(std::string normal, std::string selected, std::string disabled = "",bool swallowTouches = true,bool on = true);
    
    bool _initWithFile(std::string normal, std::string selected, std::string disabled = "",bool swallowTouches = true,bool on = true);
    
    bool _isTouchInside(Touch* touch);
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    void setOn(bool on);
    
public:
    std::string normal;
    std::string selected;
    std::string disabled;
    Sprite* pSprite;
    bool enable;
    bool isDown;
    bool on;
    int type;
    
    float preTouchx;
    float preTouchy;
    bool isSwallowTouches;
    // callback
	ccBtnCallback _callback;
};


#endif
