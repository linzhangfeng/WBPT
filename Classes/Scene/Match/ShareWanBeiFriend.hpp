//
//  ShareWanBeiFriend.hpp
//  ZJH
//
//  Created by apple on 16/8/6.
//
//

#ifndef ShareWanBeiFriend_hpp
#define ShareWanBeiFriend_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJH.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class ShareWanBeiFriend : public BaseLayer
{
public:
    
    CREATE_FUNC(ShareWanBeiFriend);
    virtual bool init();
    void myInit(Json::Value &data);
    
    void buttonClicked(Ref *ref);
    void listItemClicked(Ref *ref);

    Json::Value jsonDate;
    
    void getInfoJson(Json::Value json);
    
    
private:
    std::set<int> m_selection;
};

#endif /* ShareWanBeiFriend_hpp */
