//
//  HorizenSystemMsg.hpp
//  ZJH
//
//  Created by mac on 2017/1/16.
//
//

#ifndef HorizenSystemMsg_hpp
#define HorizenSystemMsg_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"


using namespace cocos2d::ui;
using namespace cocos2d;


class  HorizenSystemMsg: public Layer
{
public:
    void createData(Json::Value data);
    
    void showView(string titleStr, string urlStr);
    
    CREATE_FUNC(HorizenSystemMsg);
    
private:
    
    
};


#endif /* HorizenSystemMsg_hpp */
