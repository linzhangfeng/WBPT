//
//  VIPBuyTipLayer.hpp
//  ZJH
//
//  Created by apple on 16/8/4.
//
//

#ifndef VIPBuyTipLayer_hpp
#define VIPBuyTipLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  VIPBuyTipLayer: public Layer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(VIPBuyTipLayer);
    
    virtual bool TouchBegan(Touch * touch, Event * pEvent);
    virtual void TouchMoved(Touch * touch, Event * pEvent);
    virtual void TouchEnded(Touch * touch, Event * pEvent);
    
private:
    
    int pageNum;
    
    int lastNum;
    
    //将CCScrollView作为自己的层添加进来
    ScrollView * m_scrollView;
    
    //触摸点的位置
    Point m_startPoint;
    
    //CCScrollView的便宜量
    Point m_offsetPoint;
    
    //当前为第几个关卡
    int m_nCurPage;
    
    void showView(Json::Value json);
    
    ImageView* createItem(Json::Value json, int type);
    
    virtual void scrollViewDidScroll(float offset);
    
    int countX;
};


#endif /* VIPBuyTipLayer_hpp */
