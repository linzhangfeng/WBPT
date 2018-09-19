//
//  AgainstNotify.hpp
//  ZJH
//
//  Created by Jack on 16/9/23.
//
//

#ifndef AgainstNotify_hpp
#define AgainstNotify_hpp

#include <stdio.h>
#include "ZJH.h"
#include "../BaseLayer.hpp"
#include "../../Model/MsgModel.h"
class AgainstNotify : public BaseLayer
{
public:
    CREATE_FUNC(AgainstNotify);
    bool init();
    void initView();
    void initData(Json::Value &json);
    void getRecordData();
    
private:
    Size size;
    long long lastTime;
    long long newTime;
    ListView* listView;
    int curPage;
    int pageSize;
    int lastId;
    bool hasMore;
    int _cuid;
    
};

class AgainstNotifyItem :public Layer{

public:
    CREATE_FUNC(AgainstNotifyItem);
    bool init();
    void initView(MsgModel::Chat* chat);
    void initTimeView(long long timeLong);
};


#endif /* AgainstNotify_hpp */
