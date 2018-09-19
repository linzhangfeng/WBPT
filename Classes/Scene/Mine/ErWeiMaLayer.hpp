//
//  ErWeiMaLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/19.
//
//

#ifndef ErWeiMaLayer_hpp
#define ErWeiMaLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJH.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  ErWeiMaLayer: public BaseLayer
{
public:
    
    virtual bool init();
    CREATE_FUNC(ErWeiMaLayer);
    void getUserData(string URL);
};

#endif /* ErWeiMaLayer_hpp */
