//
//  OtherGameType.hpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/20.
//
//

#ifndef OtherGameType_hpp
#define OtherGameType_hpp

#include <stdio.h>
#include "ZJH.h"
#include "../Loading/Loading.h"

class OtherGameType : public Layer
{
public:
    CREATE_FUNC(OtherGameType);
    virtual bool init();
    
    void initList();
    Widget *getOtherGameTypeList(Json::Value &data);
    void initData(int typeId);
    void initSwitchType(int type);
private:
    Json::Value m_jsonData;
    Node *m_pLoading;
    Text *m_pNoData;
    ListView* listView;
    Json::Value _value;
    int index = 1;
};

#endif /* OtherGameType_hpp */
