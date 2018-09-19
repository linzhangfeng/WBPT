//
//  TipChangeEditLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/23.
//
//

#ifndef TipChangeEditLayer_hpp
#define TipChangeEditLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  TipChangeEditLayer: public Layer
{
    
public:
    
    //index == 1 修改昵称 2: 修改签名 3:设置房间名称
    static TipChangeEditLayer* create(int index, string text);
    virtual bool init(int index, string text);
    
    string replace(string str);
    
    void closeFun(Ref*ref);
    void qianMingFun(Ref*ref);
    void buluoNameFun(Ref*ref);
    void sureFun(Ref*ref);
    
private:
    
    
    
    string nameStr;
    
    EditBox *textField1;
    
};

#endif /* TipChangeEditLayer_hpp */
