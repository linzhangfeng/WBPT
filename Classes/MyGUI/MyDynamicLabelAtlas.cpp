//
//  MyDynamicLabelAtlasAtlas.cpp
//  ZJH
//
//  Created by admin on 16/3/31.
//
//

#include "MyDynamicLabelAtlas.hpp"

MyDynamicLabelAtlas* MyDynamicLabelAtlas::create(const std::string& string, const std::string& charMapFile, int itemWidth, int itemHeight, int startCharMap)
{
    auto ret = new MyDynamicLabelAtlas();
    
    if (ret && ret->initWithString(string, charMapFile, itemWidth, itemHeight, startCharMap))
    {
        ret->curValue = 0;
        
        ret->autorelease();
        
        return ret;
    }
    
    delete ret;
    return nullptr;
}

void MyDynamicLabelAtlas::setValue(int value,int step, bool isNow)
{
    myValue = value;
    
    int dv = myValue - curValue;
    if(step == 0)step = 1;
    
    addValue = dv / step;
    
    if (addValue == 0 || isNow)
    {
        curValue = myValue;
        state = state_idle;
        unschedule(schedule_selector(MyDynamicLabelAtlas::update));
        updateValue();
    }
    else
    {
        if(state == state_idle)
        {
            state = state_run;
            schedule(schedule_selector(MyDynamicLabelAtlas::update),0.2);
        }
    }
}

void MyDynamicLabelAtlas::update(float dt)
{
    curValue += addValue;
    if (addValue > 0 && curValue > myValue)
    {
        curValue = myValue;
        state = state_idle;
        unschedule(schedule_selector(MyDynamicLabelAtlas::update));
    }
    updateValue();
}

void MyDynamicLabelAtlas::updateValue()
{
    if (callback)
    {
        this->callback(curValue);
    }
    else
    {
        setString(__String::createWithFormat("%d",curValue)->getCString());
    }
}