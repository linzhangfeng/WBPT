//
//  ServerErrorTips.hpp
//  ZJH
//
//  Created by mac on 2017/1/18.
//
//

#ifndef ServerErrorTips_hpp
#define ServerErrorTips_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  ServerErrorTips: public Layer
{
    
public:
    
    static ServerErrorTips* create();
    virtual bool init();
    void click(Ref*ref);
    void setContent(string content);
private:
    LabelTTF* title2;
    ImageView* bg;
};

#endif /* ServerErrorTips_hpp */
