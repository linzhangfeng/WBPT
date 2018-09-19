//
//  AlertDialogDownTable.h
//  ZJH
//
//  Created by hyhy on 16/7/21.
//
//

#ifndef __ZJH__AlertDialogDownTable__
#define __ZJH__AlertDialogDownTable__

#include "cocos2d.h"
#include "ZJH.h"

class AlertDialogDownTable : public cocos2d::Layer
{
public:
    CREATE_FUNC(AlertDialogDownTable);
    virtual bool init();
    CC_SYNTHESIZE(MyCallBack_Int, m_pCallback, CallBack);
    void buttonClicked(cocos2d::Ref *sender);
    
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif /* defined(__ZJH__AlertDialogDownTable__) */
