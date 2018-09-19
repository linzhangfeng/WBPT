//
//  ReciveEffectTipLayer.hpp
//  ZJH
//
//  Created by apple on 16/8/17.
//
//

#ifndef ReciveEffectTipLayer_hpp
#define ReciveEffectTipLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  ReciveEffectTipLayer: public Layer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(ReciveEffectTipLayer);
    
    void showView(Json::Value json);

private:
    
    void stopAction();
    
    Layout *bg;
    
};

#endif /* ReciveEffectTipLayer_hpp */
