//
//  ChatFriendsMore.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef ChatFriendsMore_hpp
#define ChatFriendsMore_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"
USING_NS_CC;

class ChatFriendsMore : public BaseLayer
{
public:
    CREATE_FUNC(ChatFriendsMore);
    bool init();
    void myInit(Json::Value &json);
    ImageView* getMoreItemswitch(Size size,Color4B color,std::string title,bool hasArrow = true);
    void swithClickFun(Ref *pSender, Widget::TouchEventType type);
    void delBlckFriend();   //加入黑名单
    void addBlckFriend();   //加入黑名单
    void DeletemyFriend(); //删除好友
    void addFriend(); //删除好友
    
    void updateBlackFlag();
    void updateFriendFlag();
private:
    ImageView* rightOn;
    ImageView* rightOff;
    
    ImageView *Blackname;
    int _cuid;  //选择的当前用户的id
};

#endif /* ChatFriendsMore_hpp */
