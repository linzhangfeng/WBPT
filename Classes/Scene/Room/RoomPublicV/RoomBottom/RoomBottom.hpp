//
//  RoomBottom.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef RoomBottom_hpp
#define RoomBottom_hpp

#include "ZJH.h"
USING_NS_CC;

class RoomBottom : public Layer
{
public:
    CREATE_FUNC(RoomBottom);
    bool init();
    void btnCallback(Ref* ref);
    void setFastSoundVis(bool v);
    //设置当前上桌状态 true 在桌上   false没有上桌
    void setGamePlayStatu(bool s);
    CC_SYNTHESIZE(MyCallBack_Str, m_pCallback, Callback);
    void notificationGameStart(Ref *msg);
    virtual void onExit();

private:
    float ss;
    Json::Value m_jsonData;
    Button* fastChat;
    Button* fastSound;
    Button* seatBtn;
};

#endif /* RoomBottom_hpp */
