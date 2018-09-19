//
//  ChatFriendsInfo.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef ChatFriendsInfo_hpp
#define ChatFriendsInfo_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"

USING_NS_CC;

class ChatFriendsInfo : public BaseLayer
{
public:
    CREATE_FUNC(ChatFriendsInfo);
    bool init();
    void myInit(Json::Value &json);
    
    void notificationReflash(Ref* msg);
private:
    Size size;
    int uid;
    
    Text* rmkname;
};

#endif /* ChatFriendsInfo_hpp */
