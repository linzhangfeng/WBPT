//
//  MyTab.h
//  MyLuaGame
//
//  Created by 陈家勇 on 14-11-4.
//
//

#ifndef __MyLuaGame__MyTab__
#define __MyLuaGame__MyTab__

#include "cocos2d.h"
#include "MyButton.h"

using namespace std;
using namespace cocos2d;

class MyTab : public cocos2d::Layer
{
public:
    
    static MyTab* create();

    bool init();
    void setCur(int index);
    cocos2d::Node* addByFrames(string normal, string selected, string cur, cocos2d::Point pos, int index);
    
    cocos2d::Node* addByFiles(string normal, string selected, string cur, cocos2d::Point pos, int index);

    int curIndex;
    void onItemCallback(int tag);
    void setEnabled(bool enable);//设置禁用
    void SetCanEnable(bool IsCan);//设置是否可以禁用
    void setCallback(const ccBtnCallback& callback);
    // callback
	ccBtnCallback _callback;
private:
    bool IsCanEnable;//是否可以禁用
    vector<int >allIndex;//所有按钮的下标
};

#endif /* defined(__MyLuaGame__MyTab__) */
