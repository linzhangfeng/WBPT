//
//  InviteFriendListLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/13.
//
//

#ifndef InviteFriendListLayer_hpp
#define InviteFriendListLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJH.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  InviteFriendListLayer: public BaseLayer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(InviteFriendListLayer);

private:
    void showNoListView();
    void showView(Json::Value json);
   
    
    Layout* cellView(Json::Value json);
    int pageIndex;
    ListView *listView;
    bool isFirstPost;
    
};

#endif /* InviteFriendListLayer_hpp */
