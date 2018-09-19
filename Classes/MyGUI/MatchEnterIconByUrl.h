//
//  ImageByUrl.h
//  MyLuaGame
//
//  Created by  on 14-11-4.
//
//

#ifndef __MyLuaGame__MatchEnterIconByUrl__
#define __MyLuaGame__MatchEnterIconByUrl__

#include "cocos2d.h"
#include "ZJh.h"
using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

class MatchEnterIconByUrl : public ImageView
{
public:
    virtual bool init();
    CREATE_FUNC(MatchEnterIconByUrl);
    void load(std::string imageURL, string defaultImg="Match/match_enter.png");
    void loading(float dt);
public:
    std::string tag;
    std::string imageURL;
};

#endif /* defined(__MyLuaGame__ImageByUrl__) */
