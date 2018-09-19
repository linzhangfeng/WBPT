//
//  CreateRoomFinishMask.hpp
//  ZJH
//
//  Created by apple on 16/7/12.
//
//

#ifndef CreateRoomFinishMask_hpp
#define CreateRoomFinishMask_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  CreateRoomFinishMask: public Layer
{
    
public:
    virtual bool init();
    CREATE_FUNC(CreateRoomFinishMask);
private:
    void sureOn(Ref *pSender, Widget::TouchEventType type);
    void exitOn(Ref *pSender, Widget::TouchEventType type);
    
};

#endif /* CreateRoomFinishMask_hpp */
