//
//  OtherGameTypeView.hpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/21.
//
//

#ifndef OtherGameTypeView_hpp
#define OtherGameTypeView_hpp

#include <stdio.h>
#include "ZJH.h"

class OtherGameTypeView : public Layout
{
public:
    CREATE_FUNC(OtherGameTypeView);
    virtual bool init();
    
    void initData(Json::Value _value);
    void initView();
    void setCallBack(const MyCallBack_Int &callback);
private:
    ListView* _mListView;
    MyCallBack_Int _callback;
};

class TypeItem : public Layout
{
public:
    static Layout* createWithData(Json::Value info,const MyCallBack_Int &callback);
};

#endif /* OtherGameTypeView_hpp */
