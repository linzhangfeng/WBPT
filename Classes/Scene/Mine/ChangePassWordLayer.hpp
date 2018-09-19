//
//  ChangePassWordLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/11.
//
//

#ifndef ChangePassWordLayer_hpp
#define ChangePassWordLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  ChangePassWordLayer: public BaseLayer
{
public:
    
    virtual bool init();
    CREATE_FUNC(ChangePassWordLayer);
    
    //修改密码
    void finishOn(Ref *pSender, Widget::TouchEventType type);
    
private:
    EditBox *edit1;
    EditBox *edit2;
    EditBox *edit3;
};


#endif /* ChangePassWordLayer_hpp */
