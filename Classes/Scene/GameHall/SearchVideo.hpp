//
//  SearchVideo.hpp
//  ZJH
//
//  Created by mac on 2017/5/17.
//
//

#ifndef SearchVideo_hpp
#define SearchVideo_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  SearchVideo : public Layout
{
public:
    virtual bool init();
    CREATE_FUNC(SearchVideo);
    void requestVideoInfo(Json::Value data);
private:
};

#endif /* SearchVideo_hpp */
