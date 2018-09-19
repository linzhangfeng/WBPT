//
//  TaskLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/6.
//
//

#ifndef PlayWebLayerMsg_hpp
#define PlayWebLayerMsg_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"


using namespace cocos2d::ui;
using namespace cocos2d;


class  PlayWebLayerMsg: public Layer
{
public:
    void createData(Json::Value data);
    
    void showView(string titleStr, string urlStr);
    
    CREATE_FUNC(PlayWebLayerMsg);
    
private:
    

};


#endif /* TaskLayer_hpp */
