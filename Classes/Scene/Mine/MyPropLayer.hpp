//
//  MyPropLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/21.
//
//

#ifndef MyPropLayer_hpp
#define MyPropLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"

using namespace cocos2d::ui;
using namespace cocos2d;


class  MyPropLayer: public BaseLayer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(MyPropLayer);
private:
    
    ImageView* createItem(Json::Value json);
    void initView(Json::Value jsonData);
    ListView* listView1;
    
};

#endif /* MyPropLayer_hpp */
