//
//  ChangePhone2Layer.hpp
//  ZJH
//
//  Created by apple on 16/7/11.
//
//

#ifndef ChangePhone2Layer_hpp
#define ChangePhone2Layer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  ChangePhone2Layer: public BaseLayer
{
public:
    
    virtual bool init();
    CREATE_FUNC(ChangePhone2Layer);
    
    //修改手机号
    void finishOn(Ref *pSender, Widget::TouchEventType type);
    
    void timeDown(float dt);
    //发送验证码
    void postCheckNumOn(Ref *pSender, Widget::TouchEventType type);
    
private:
    
    EditBox *edit1;
    EditBox *edit2;
    EditBox *edit3;
    
    Button *msgBtn;
    
    int secend;
};

#endif /* ChangePhone2Layer_hpp */
