//
//  ChatFriendsRooms.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef ChatFriendsRooms_hpp
#define ChatFriendsRooms_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"
USING_NS_CC;

class ChatFriendsRooms : public BaseLayer
{
public:
    CREATE_FUNC(ChatFriendsRooms);
    bool init();
    void myInit(Json::Value &json);
    void initList();
    
    void getData(float dt);
    void addDataInList();
private:
    ListView* listView;
    Json::Value dataList;
    int pageNo;
    bool hasMore;
    int _cuid;
    int myCount;
};

#endif /* ChatFriendsRooms_hpp */
