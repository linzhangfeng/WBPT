//
//  HallFriend.h
//  ZJH
//
//  Created by hyhy on 16/7/7.
//
//

#ifndef __ZJH__HallFriend__
#define __ZJH__HallFriend__

#include "ZJH.h"

class HallFriend : public Layer
{
public:
    CREATE_FUNC(HallFriend);
    virtual bool init();
    
    void initList();
    Widget *getFriendList(Json::Value &data);
private:
    Json::Value m_jsonData;
};

#endif /* defined(__ZJH__HallFriend__) */
