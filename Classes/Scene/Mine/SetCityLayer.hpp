//
//  SetCityLayer.hpp
//  ZJH
//
//  Created by apple on 16/8/7.
//
//

#ifndef SetCityLayer_hpp
#define SetCityLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  SetCityLayer: public Layer
{
    
public:

    virtual bool init();
    CREATE_FUNC(SetCityLayer)
private:
    
    ListView *listView1;
    ListView *listView2;
    ListView *listView3;
    
    int index1;
    int index2;
    int index3;
    
    std::vector<string> proviceArray;
    std::vector<vector<string>> cityArray;
    std::map<string, vector<string>>districtArray;
    
    void clickList1(Ref *pSender, Widget::TouchEventType type);
    void clickList2(Ref *pSender, Widget::TouchEventType type);
    void clickList3(Ref *pSender, Widget::TouchEventType type);
    
    void changeCity(Ref*ref);
    
    void updateViewList2();
    void updateViewList3();
    
    void showProvice();
    
};



#endif /* SetCityLayer_hpp */
