//
//  MyDynamicLabel.hpp
//  ZJH
//
//  Created by admin on 16/3/31.
//
//

#ifndef MyDynamicLabel_hpp
#define MyDynamicLabel_hpp

#include "cocos2d.h"
#include "ZJH.h"

using namespace std;
using namespace cocos2d;
class MyDynamicLabel : public cocos2d::Label
{
    enum
    {
        state_idle = 0,
        state_run,
    };
    
public:
    
    static MyDynamicLabel* createWithSystemFont(const std::string& text, const std::string& font, float fontSize,
                                       const cocos2d::Size& dimensions = cocos2d::Size::ZERO, TextHAlignment hAlignment = TextHAlignment::LEFT,
                                       TextVAlignment vAlignment = TextVAlignment::TOP);
    
    void setValue(int value,int step = 30, bool isNow = false);
    void update(float dt);
    void setUpdateCallback(const MyCallBack_Int &callback){this->callback = callback;};
private:
    void updateValue();
    
    int myValue;
    int curValue;
    int addValue;
    int state;
    MyCallBack_Int callback;
};

#endif /* MyDynamicLabel_hpp */
