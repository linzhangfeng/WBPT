//
//  ShareAppInviteLayer.hpp
//  ZJH
//
//  Created by apple on 16/8/4.
//
//

#ifndef ShareAppInviteLayer_hpp
#define ShareAppInviteLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  ShareAppInviteLayer: public Layer
{
    
public:
    virtual bool init();
    
    CREATE_FUNC(ShareAppInviteLayer);
    
    void setShareTypeView(int type, Json::Value json); //1:invite 2: app
    
    int typeIndex;
    Json::Value shareJson;
    
    void btnClick(Ref*ref);
    
private:
    
    Text *titleText;

};

#endif /* ShareAppInviteLayer_hpp */
