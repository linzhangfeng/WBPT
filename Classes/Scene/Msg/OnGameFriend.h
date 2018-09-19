//
//  OnGameFriend.hpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/29.
//
//

#ifndef OnGameFriend_hpp
#define OnGameFriend_hpp

#include <stdio.h>
#include "ZJH.h"

class OnGameFriend : public Layout
{
public:
    CREATE_FUNC(OnGameFriend);
    bool init();
    
    void setHead(int h);
    Layout* setBackground(Color4B color);
    void  initView();
    void addListViewData(Json::Value _value);
    void requestMyOnGame(int friendId);
private:
    ListView *listView;
};

class OnGameFriend_Item : public Layer
{
public:
    static Layout* initData(Json::Value _value);
private:
    
};

#endif /* OnGameFriend_hpp */
