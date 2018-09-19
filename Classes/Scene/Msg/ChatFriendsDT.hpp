//
//  ChatFriendsDT.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef ChatFriendsDT_hpp
#define ChatFriendsDT_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"
USING_NS_CC;

class ChatFriendsDT : public BaseLayer
{
public:
    CREATE_FUNC(ChatFriendsDT);
    bool init();
    void myInit(Json::Value &json);
private:
};

#endif /* ChatFriendsDT_hpp */
