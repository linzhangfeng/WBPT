//
//  BlackFriends.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef BlackFriends_hpp
#define BlackFriends_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"
USING_NS_CC;

class BlackFriends : public BaseLayer
{
public:
    CREATE_FUNC(BlackFriends);
    bool init();
    void getData(float dt);
    void initList();
private:
    int pageNo;
    Json::Value listData;
    Node* loading;
    ListView* listView;
    int count;
    
    Layout* curLayout;
};

class BlackFriends_Item : public Layer
{
public:
    CREATE_FUNC(BlackFriends_Item);
    bool init();
    void initData(Json::Value &json,int index,const MyCallBack_Int &callback);
private:
};

#endif /* BlackFriends_hpp */
