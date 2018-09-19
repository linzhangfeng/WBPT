//
//  ForgetPassLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/11.
//
//

#ifndef ForgetPassLayer_hpp
#define ForgetPassLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  ForgetPassLayer: public BaseLayer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(ForgetPassLayer);
    
    //下一步
    void nextOn(Ref *pSender, Widget::TouchEventType type);
    void timeDown(float dt);
    //发送验证码
    void postCheckNumOn(Ref *pSender, Widget::TouchEventType type);
private:
    
    EditBox *edit1;
    EditBox *edit2;
    Button *msgBtn;
    int secend;
};


#endif /* ForgetPassLayer_hpp */
