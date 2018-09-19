//
//  EndRoomDialog.h
//  ZJH
//
//  Created by hyhy on 16/8/4.
//
//

#ifndef __ZJH__EndRoomDialog__
#define __ZJH__EndRoomDialog__

#include "cocos2d.h"
#include "ZJH.h"

class EndRoomDialog : public cocos2d::Layer
{
public:
    CREATE_FUNC(EndRoomDialog);
    virtual bool init();
    void init1();
    void init2(std::string content);
    EndRoomDialog();
    
    void buttonClicked(Ref *ref);
    CC_SYNTHESIZE(MyCallBack_Str, m_pCallback, Callback);
};

#endif /* defined(__ZJH__EndRoomDialog__) */
