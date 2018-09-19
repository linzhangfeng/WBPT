//
//  FastLoadLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/11.
//
//

#ifndef FastLoadLayer_hpp
#define FastLoadLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  FastLoadLayer: public BaseLayer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(FastLoadLayer);
    
    //注册下一步
    void load(Ref *pSender, Widget::TouchEventType type);
    void timeDown(float dt);
    //发送验证码
    void postCheckNumOn(Ref *pSender, Widget::TouchEventType type);
private:
    EditBox *edit1;
    EditBox *edit2;
    Button *msgBtn;
    
    int secend;
};


#endif /* FastLoadLayer_hpp */
