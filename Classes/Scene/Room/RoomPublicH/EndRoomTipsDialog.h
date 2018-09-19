//
//解散房间提示
//

#ifndef EndRoomTipsDialog_H
#define EndRoomTipsDialog_H

#include "cocos2d.h"
#include "ZJH.h"

class EndRoomTipsDialog : public cocos2d::Layer
{
public:
    CREATE_FUNC(EndRoomTipsDialog);
    virtual bool init();
    void init1();
    void init2(std::string title,std::string content,bool isHome = false);//是否为房主
    EndRoomTipsDialog();
    
    void buttonClicked(Ref *ref);
    CC_SYNTHESIZE(MyCallBack_Str, m_pCallback, Callback);
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
};

#endif /* defined(EndRoomTipsDialog_H) */
