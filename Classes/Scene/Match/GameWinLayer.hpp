//
//  GameWinLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/15.
//
//

#ifndef GameWinLayer_hpp
#define GameWinLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJH.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  GameWinLayer: public BaseLayer
{
    
public:
    virtual bool init();
    CREATE_FUNC(GameWinLayer);

    
private:
    
    void  postData();
    
    void  showView(Json::Value json);
    ListView *viewList;
};

#endif /* GameWinLayer_hpp */
