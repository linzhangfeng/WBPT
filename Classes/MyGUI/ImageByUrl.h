//
//  ImageByUrl.h
//  MyLuaGame
//
//  Created by 陈家勇 on 14-11-4.
//
//

#ifndef __MyLuaGame__ImageByUrl__
#define __MyLuaGame__ImageByUrl__

#include "cocos2d.h"
#include "ZJh.h"
using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
class ImageByUrl : public Layout
{
public:
    virtual bool init();
    CREATE_FUNC(ImageByUrl);
    void load(int w, int h, std::string imageURL,string defaultImg="Match/icon_wanbei.png");
    void loading(float dt);
public:
    cocos2d::ui::ImageView* image;
    std::string tag;
    std::string imageURL;
};

#endif /* defined(__MyLuaGame__ImageByUrl__) */
