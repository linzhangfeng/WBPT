//
//  AboutOurLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/27.
//
//

#ifndef AboutOurLayer_hpp
#define AboutOurLayer_hpp

#include "cocos2d.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"

class  AboutOurLayer: public BaseLayer
{
public:
    
    virtual bool init();
    
    void selectFun(Ref *pSender, Widget::TouchEventType type);
    
    CREATE_FUNC(AboutOurLayer);
    
private:
    //用于记录点击次数 来显示版本信息对话框
    int clickCount;
    //上一次点击的时间
    long long lastClickTime;
    
    void showDialog(Ref* ref);
};


#endif /* AboutOurLayer_hpp */
