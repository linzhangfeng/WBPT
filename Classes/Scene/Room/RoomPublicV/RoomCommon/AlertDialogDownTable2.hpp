//
//  AlertDialogDownTable2.hpp
//  ZJH
//
//  Created by mac on 2016/11/7.
//
//

#ifndef AlertDialogDownTable2_hpp
#define AlertDialogDownTable2_hpp

#include "cocos2d.h"
#include "ZJH.h"

class AlertDialogDownTable2 : public cocos2d::Layer
{
public:
    CREATE_FUNC(AlertDialogDownTable2);
    virtual bool init();
    CC_SYNTHESIZE(MyCallBack_Int, m_pCallback, CallBack);
    void buttonClicked(cocos2d::Ref *sender);
    
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif /* AlertDialogDownTable2_hpp */
