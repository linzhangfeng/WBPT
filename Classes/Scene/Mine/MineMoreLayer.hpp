//
//  MineMoreLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/27.
//
//

#ifndef MineMoreLayer_hpp
#define MineMoreLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"
using namespace cocos2d::ui;
using namespace cocos2d;


class  MineMoreLayer: public BaseLayer
{
public:
    virtual bool init();
    void selectFun(Ref *pSender, Widget::TouchEventType type);
    void postCheckVern();
    
    CREATE_FUNC(MineMoreLayer);
    
private:
       
};

#endif /* MineMoreLayer_hpp */
