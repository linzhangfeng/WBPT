//
//  TipChangeEditLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/23.
//
//

#ifndef TipLoadLayer_hpp
#define TipLoadLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  TipLoadLayer: public Layer
{
    
public:

    static TipLoadLayer* create(int type);
    virtual bool init(int type);
    
    void reLianOn(Ref*ref);
private:
    
    int connectType;
};

#endif /* TipChangeEditLayer_hpp */
