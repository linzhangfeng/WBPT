//
//  MyControlSwitch.hpp
//  ZJH
//
//  Created by wuyu on 16/2/20.
//
//

#ifndef MyControlSwitch_hpp
#define MyControlSwitch_hpp
#include "cocos2d.h"
#include "MyButton.h"

using namespace std;
using namespace cocos2d;

class MyControlSwitch : public cocos2d::Layer
{
    enum MySwitch_Type
    {
        Type_frame = 0,
        Type_file = 1,
    };
    
public:
    
    static MyControlSwitch* createWithSpriteFrame(std::string switch_bg, std::string switch_on, std::string switch_off,std::string switch_thumb,bool swallowTouches = true,bool on = true);
    static MyControlSwitch* createWithFile(std::string switch_bg, std::string switch_on, std::string switch_off,std::string switch_thumb,bool swallowTouches = true,bool on = true);
    
    void setEnabled(bool enabled);
    void setCallback(const ccBtnCallback& callback);
    
    bool _initWithSpriteFrame(std::string switch_bg, std::string switch_on, std::string switch_off,std::string switch_thumb,bool swallowTouches = true,bool on = true);
    
    bool _initWithFile(std::string switch_bg, std::string switch_on, std::string switch_off,std::string switch_thumb,bool swallowTouches = true,bool on = true);
    
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



#endif /* MyControlSwitch_hpp */
