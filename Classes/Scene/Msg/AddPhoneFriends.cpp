//
//  AddPhoneFriends.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "AddPhoneFriends.hpp"
#include "ZJH.h"

bool AddPhoneFriends::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setName("AddPhoneFriends");
    
    Size size = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(255, 255, 255, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    
    setTitle("添加手机联系人");
    
    return true;
}