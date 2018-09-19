//
//  TaskLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/6.
//
//

#ifndef PlayWebLayer_hpp
#define PlayWebLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"


using namespace cocos2d::ui;
using namespace cocos2d;


class  PlayWebLayer: public Layer
{
public:
    virtual bool init();
    void createView();
    void createData(Json::Value Data,int index);
    CREATE_FUNC(PlayWebLayer);
    
    void onBaoBtnResponse(Ref*ref);
    void onMoreResponse(Ref*ref);
    void onShouResponse(Ref*ref);
    
private:
    ImageView * bg;
    float H;
    float bgW;
    
    Button *lastBaoBtn;
    int baoIndex;
    Json::Value setData;
    cocos2d::experimental::ui::WebView *webView;
};


#endif /* TaskLayer_hpp */
