//
//  RegistLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/8.
//
//

#ifndef RegistLayer_hpp
#define RegistLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  RegistLayer: public BaseLayer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(RegistLayer);
    
    //注册下一步
    void nextOn(Ref *pSender, Widget::TouchEventType type);
    void timeDown(float dt);
    //发送验证码
    void postCheckNumOn(Ref *pSender, Widget::TouchEventType type);
private:
    
    Button* msgBtn;
    EditBox *edit1;
    EditBox *edit2;
    int secend;
};


#endif /* RegistLayer_hpp */
