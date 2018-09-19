//
//  MyTab2.cpp
//  MyLuaGame
//
//  Created by 陈家勇 on 14-11-4.
//
//

#include "MyTab2.h"
using namespace cocos2d;

MyTab2* MyTab2::create()
{
    MyTab2* list = new MyTab2();
    if(list && list->init())
    {
        list->autorelease();
        return list;
    }
    else
    {
        delete list;
        list = NULL;
        return NULL;
    }
}

bool MyTab2::init()
{
    if (!Layer::init())
    {
        return false;
    }
    curIndex = -1;
    return true;
}

void MyTab2::setCur(int index)
{
    if(curIndex == index)
    {
        return;
    }

    if(curIndex != index && curIndex != -1)
    {
        MyButton* btn = (MyButton*)getChildByTag(curIndex);
        reorderChild(btn, 0);
        if(btn != NULL)btn->setEnabled(true);
    }
    curIndex = index;
    MyButton* btn = (MyButton*)getChildByTag(curIndex);
    reorderChild(btn, 10);
    if(btn != NULL)btn->setEnabled(false);
}

void MyTab2::addByFiles(std::string normal, std::string selected, std::string cur, Vec2 pos, int index)
{
    MyButton* btn = MyButton::createWithFile(normal,selected,cur);
    btn->setTag(index);
    btn->setPosition(pos);
    btn->setCallback(CC_CALLBACK_1(MyTab2::onItemCallback, this));
    addChild(btn);
}

void MyTab2::onItemCallback(int tag)
{
    setCur(tag);
    if( _callback != NULL)
    {
        _callback(tag);
    }
}

void MyTab2::setCallback(const ccBtnCallback& callback)
{
	_callback = callback;
}