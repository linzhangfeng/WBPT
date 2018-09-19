//
//  MyFriends.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef MyFriends_hpp
#define MyFriends_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"
USING_NS_CC;

class MyFriends : public BaseLayer
{
public:
    CREATE_FUNC(MyFriends);
    bool init();
    void getData(float dt);
    void initList();
    void addDataInList();
    
    void notificationReflash(Ref* msg);
private:
    Json::Value friendList;
    int pageNo;
    bool hasMore;
    Node* loading;
    Layout* curLayout;
};

class MyFriends_Item : public Layer
{
public:
    CREATE_FUNC(MyFriends_Item);
    bool init();
    void initData(Json::Value &json,int index,const MyCallBack_Int &callback);
private:
};
#endif /* MyFriends_hpp */
