
#ifndef ZJH_MyButton_h
#define ZJH_MyButton_h

#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

typedef std::function<void(int tag)> ccBtnCallback;
class MyButton : public cocos2d::Layer
{
    enum MyButton_Type
    {
        Type_frame = 0,
        Type_file = 1,
        Type_size = 2,
    };
public:
    
    static MyButton* createWithSpriteFrame(std::string normal, std::string selected, std::string disabled = "",bool swallowTouches = true);
    static MyButton* createWithFile(std::string normal, std::string selected, std::string disabled = "",bool swallowTouches = true);
    
    static MyButton* createWithFileUseTitle(std::string normal, std::string selected, std::string disabled = "",std::string titleFile = "", bool swallowTouches = true);
    
    static MyButton* createWithSize(cocos2d::Size size,bool swallowTouches = true);
    void setEnabled(bool enabled);
    void setClick(bool IsClick);
    void setCallback(const ccBtnCallback& callback);
    
    bool _initWithSpriteFrame(std::string normal, std::string selected, std::string disabled = "",bool swallowTouches = true);
    bool _initWithFile(std::string normal, std::string selected, std::string disabled = "",bool swallowTouches = true);
    
    bool _initWithFileUseTitle(std::string normal, std::string selected, std::string disabled = "",std::string titleFile = "",bool swallowTouches = true);
    
    bool _initWithSize(cocos2d::Size size,bool swallowTouches = true);
    
    bool _isTouchInside(Touch* touch);
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);

public:
    std::string normal;
    std::string selected;
    std::string disabled;
    Sprite* pSprite;
    bool enable;
    bool isDown;
    int type;
    
    float preTouchx;
    float preTouchy;
    bool isSwallowTouches;
    // callback
	ccBtnCallback _callback;
};

#endif
