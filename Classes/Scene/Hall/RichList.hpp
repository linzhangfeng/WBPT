//
//  RichList.hpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/22.
//
//

#ifndef RichList_hpp
#define RichList_hpp

#include <stdio.h>
#include "ZJH.h"
#include "../BaseLayer.hpp"
#include "../Loading/Loading.h"

class RichList :public BaseLayer
{
public:
    CREATE_FUNC(RichList);
    bool init();
    void btnCallback(Ref* ref);
    void initview();
    
    void initData(int type);
    void initRequestType();
private:
    ListView* _mListView;
    Button* _btn;
    Node *m_pLoading;
    Json::Value _value;
    Layout* layLineName;
    int currentType;
};

class RichListItem : public Layout
{
public:
   static Layout* getItemLayout(Json::Value _json,int index,int type);
};
#endif /* RichList_hpp */
