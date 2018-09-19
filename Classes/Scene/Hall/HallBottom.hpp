//
//  HallBottom.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef HallBottom_hpp
#define HallBottom_hpp

#include "ZJH.h"
USING_NS_CC;

class HallBottom : public Layer
{
public:
    CREATE_FUNC(HallBottom);
    bool init();
    void btnCallback(Ref* ref);
    void setCallback(const MyCallBack_Str &callback);
    virtual void onExit();
    
    void hasNewMsgNotify(Ref* msg);
    void noticeShowShopItem(Ref* ref);
    void hasIngRoomNotice(Ref*ref);
    
private:
    MyCallBack_Str callback;
};

#endif /* HallBottom_hpp */
