//
//  RealNameAuthLayer.hpp
//  ZJH
//
//  Created by mac on 2016/11/24.
//
//

#ifndef RealNameAuthLayer_hpp
#define RealNameAuthLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"

using namespace cocos2d::ui;
using namespace cocos2d;

class RealNameAuthLayer: public BaseLayer
{
public:
    
    virtual bool init();
    CREATE_FUNC(RealNameAuthLayer);
    
private:
    cocos2d::ui::EditBox* _editText;
    cocos2d::ui::EditBox* _editTextName;
};

#endif /* RealNameAuthLayer_hpp */
