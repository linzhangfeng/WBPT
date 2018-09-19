//
//  Moreoptions.h
//  ZJH
//
//  Created by zhangzhenhua on 16/7/13.
//
//

#ifndef Moreoptions_h
#define Moreoptions_h

#include "ZJH.h"

class Moreoptions : public Layout
{
public:
    CREATE_FUNC(Moreoptions);
    virtual bool init();
    void initSelectoptions();
    void callFun(const MyCallBack_Null &callback);
private:
    MyCallBack_Null _callfun;
    Layout* listLayout;
};

class optionsListItem : public cocos2d::ui::Widget
{
public:
    static optionsListItem *createWithData(int index,const MyCallBack_Null &callback);
    bool initWithData(int index,const MyCallBack_Null &callback);
};

#endif /* Moreoptions_h */
