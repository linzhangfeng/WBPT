//
//  Notice.hpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/14.
//
//

#ifndef Notice_hpp
#define Notice_hpp

#include <stdio.h>
#include "ZJH.h"
#include "../BaseLayer.hpp"

class Notice : public Layout
{
public:
    CREATE_FUNC(Notice);
    bool init();
    void initview();
    void addMessage(Json::Value _mvalue);
private:
    Layout* midlay;
    Size midSize;
    Text *content;
};

#endif /* Notice_hpp */
