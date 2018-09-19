//
//  FeedBacklayer.hpp
//  ZJH
//
//  Created by apple on 16/7/27.
//
//

#ifndef FeedBacklayer_hpp
#define FeedBacklayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"
using namespace cocos2d::ui;
using namespace cocos2d;


class  FeedBacklayer: public BaseLayer
{
public:
    virtual bool init();
    void clickImageFun(Ref *pSender);
    void sureFun(Ref *pSender);
    
    void addImageFun(Ref*ref);
    
    CREATE_FUNC(FeedBacklayer);
    
private:
    
    TextField *opption;
    string imageStr;
    EditBox *qq;
    EditBox *email;
    
    ImageView *image1;
    ImageView *image2;
    ImageView *image3;
    
    int tagIndex;
    
    std::string imagepath;
    
};

#endif /* FeedBacklayer_hpp */
