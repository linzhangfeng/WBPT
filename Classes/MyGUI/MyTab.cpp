//
//  MyTab.cpp
//  MyLuaGame
//
//  Created by 陈家勇 on 14-11-4.
//
//

#include "MyTab.h"
using namespace cocos2d;

MyTab* MyTab::create()
{
    MyTab* list = new MyTab();
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

bool MyTab::init()
{
    if (!Layer::init())
    {
        return false;
    }
    curIndex = -1;
    IsCanEnable =false;
    allIndex.clear();
    return true;
}

void MyTab::setCur(int index)
{
    if(curIndex == index)
    {
        return;
    }

    if(curIndex != index && curIndex != -1)
    {
        MyButton* btn = (MyButton*)getChildByTag(curIndex);
        if(btn != NULL)
        {
            if(IsCanEnable)
            {
                btn->setClick(true);
            }
            else{
                btn->setEnabled(true);
            }
        }
    }
    curIndex = index;
    MyButton* btn = (MyButton*)getChildByTag(curIndex);
    if(btn != NULL)
    {
        if(IsCanEnable)
        {
            btn->setClick(false);
        }
        else{
            btn->setEnabled(false);
        }
    }
}


void MyTab::setEnabled(bool _enable)
{
    if (!IsCanEnable) {
        return;
    }
    for (int i = 0; i< allIndex.size(); i++)
    {
        MyButton* btn = (MyButton*)getChildByTag(allIndex[i]);
        if (btn) {
            
            if (curIndex == allIndex[i])
            {
                if (_enable) {
                    btn->setClick(false);
                    continue;
                }
            }
            btn->setEnabled(_enable);
        }
    }
}

void MyTab::SetCanEnable(bool IsCan)
{
    IsCanEnable = IsCan;
}

Node * MyTab::addByFrames(std::string normal, std::string selected, std::string cur, Vec2 pos, int index)
{
    MyButton* btn = MyButton::createWithSpriteFrame(normal,selected,cur);
    btn->setTag(index);
    btn->setPosition(pos);
    btn->setCallback(CC_CALLBACK_1(MyTab::onItemCallback, this));
    addChild(btn);
    allIndex.push_back(index);
    return btn;
}

Node * MyTab::addByFiles(std::string normal, std::string selected, std::string cur, Vec2 pos, int index)
{
    MyButton* btn = MyButton::createWithFile(normal,selected,cur);
    btn->setTag(index);
    btn->setPosition(pos);
    btn->setCallback(CC_CALLBACK_1(MyTab::onItemCallback, this));
    addChild(btn);
    allIndex.push_back(index);
    return btn;
}

void MyTab::onItemCallback(int tag)
{
    setCur(tag);
    if( _callback != NULL)
    {
        _callback(tag);
    }
}

void MyTab::setCallback(const ccBtnCallback& callback)
{
	_callback = callback;
}