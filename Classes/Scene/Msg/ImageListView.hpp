//
//  ImageListView.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef ImageListView_hpp
#define ImageListView_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"
USING_NS_CC;

class ImageListView : public BaseLayer
{
public:
    CREATE_FUNC(ImageListView);
    bool init();
    void myInit(Json::Value &json);
private:
};

#endif /* ImageListView_hpp */
