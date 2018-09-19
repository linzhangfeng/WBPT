//
//  AlertDialogFold.h
//  ZJH
//
//  Created by hyhy on 16/8/8.
//
//

#ifndef __ZJH__AlertDialogFold__
#define __ZJH__AlertDialogFold__

#include "cocos2d.h"
#include "ZJH.h"

class AlertDialogFold: public cocos2d::Layer
{
public:
    CREATE_FUNC(AlertDialogFold);
    virtual bool init();
    CC_SYNTHESIZE(MyCallBack_Str, m_pCallback, CallBack);
    void buttonClicked(cocos2d::Ref *sender);
    
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif /* defined(__ZJH__AlertDialogFold__) */
