//
//  ShareMatchLayer.hpp
//  ZJH
//
//  Created by apple on 16/8/1.
//
//

#ifndef ShareMatchLayer_hpp
#define ShareMatchLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  ShareMatchLayer: public Layer
{
    
public:
    virtual bool init();
    CREATE_FUNC(ShareMatchLayer);
    
    void setPath(string path, Json::Value json);
    
    string imagePath;
    Json::Value jsonDate;
    
    void shareCkick(Ref*ref);
};

#endif /* ShareMatchLayer_hpp */
