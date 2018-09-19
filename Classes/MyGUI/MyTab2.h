//
//  MyTab2.h
//  MyLuaGame
//
//  Created by 陈家勇 on 14-11-4.
//
//

#ifndef __MyLuaGame__MyTab2__
#define __MyLuaGame__MyTab2__

#include "cocos2d.h"
#include "MyButton.h"

using namespace std;
using namespace cocos2d;

class MyTab2 : public cocos2d::Layer
{
public:
    
    static MyTab2* create();

    bool init();
    void setCur(int index);
    
    void addByFiles(string normal, string selected, string cur, cocos2d::Point pos, int index);

    int curIndex;
    void onItemCallback(int tag);
    
    void setCallback(const ccBtnCallback& callback);
    
    // callback
	ccBtnCallback _callback;
};

#endif /* defined(__MyLuaGame__MyTab2__) */
