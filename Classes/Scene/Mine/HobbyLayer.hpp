//
//  HobbyLayer.hpp
//  wanMain
//
//  Created by apple on 16/6/30.
//
//

#ifndef HobbyLayer_hpp
#define HobbyLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"
using namespace cocos2d::ui;
using namespace cocos2d;


class  HobbyLayer: public BaseLayer
{
public:
    virtual bool init();
    
    void selectFun(Ref *pSender, Widget::TouchEventType type);
    void backFun(Ref *pSender, Widget::TouchEventType type);
    
    void showView(Json::Value data);
    CREATE_FUNC(HobbyLayer);
    
private:
    Json::Value hobbyList;
    std::string _countStr;
    ListView* listView1;
    
};


#endif /* HobbyLayer_hpp */
