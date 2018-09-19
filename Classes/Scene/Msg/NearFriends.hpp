//
//  NearFriends.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef NearFriends_hpp
#define NearFriends_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"
USING_NS_CC;

class NearFriends : public BaseLayer
{
public:
    CREATE_FUNC(NearFriends);
    bool init();
    void getData(float dt);
    void initList();
    void addDataInList(bool init = true);
private:
    int pageNo;
    bool hasMore;
    Json::Value listData;
    Node* loading;
    ListView* listView;
    int test;
};

class NearFriends_Item : public Layer
{
public:
    CREATE_FUNC(NearFriends_Item);
    bool init();
    void initData(Json::Value &json);
private:

};
#endif /* NearFriends_hpp */
