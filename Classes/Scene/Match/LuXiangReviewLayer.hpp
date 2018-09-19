//
//  RecordDegitalLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/16.
//
//

#ifndef LuXiangReviewLayer_hpp
#define LuXiangReviewLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJH.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  LuXiangReviewLayer: public BaseLayer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(LuXiangReviewLayer);
    
    void showView(Json::Value json);
    
private:
    

};

#endif /* LuXiangReviewLayer_hpp */
