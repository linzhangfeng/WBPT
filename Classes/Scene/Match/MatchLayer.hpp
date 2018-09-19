//
//  MatchLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/13.
//
//

#ifndef MatchLayer_hpp
#define MatchLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJH.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  MatchLayer: public BaseLayer
{
    
public:
    MatchLayer();
    virtual bool init();
    CREATE_FUNC(MatchLayer);
    
private:
    
    Layout* bgLayout;
    
    void tabClickFun(Ref* ref);
    void showOpenMatch();
    void showCurrent(Ref*ref);
    void showRecord();
};

#endif /* MatchLayer_hpp */
