//
//  MyDynamicLabel.cpp
//  ZJH
//
//  Created by admin on 16/3/31.
//
//

#include "MyDynamicLabel.hpp"

MyDynamicLabel* MyDynamicLabel::createWithSystemFont(const std::string& text, const std::string& font, float fontSize, const Size& dimensions /* = Size::ZERO */, TextHAlignment hAlignment /* = TextHAlignment::LEFT */, TextVAlignment vAlignment /* = TextVAlignment::TOP */)
{
    auto ret = new MyDynamicLabel();
    
    if (ret)
    {
        ret->curValue = 0;
        
        ret->setAlignment(hAlignment,vAlignment);
        ret->setSystemFontName(font);
        ret->setSystemFontSize(fontSize);
        ret->setDimensions(dimensions.width, dimensions.height);
        ret->setString(text);
        
        ret->autorelease();
        
        return ret;
    }
    
    delete ret;
    return nullptr;
}

void MyDynamicLabel::setValue(int value,int step, bool isNow)
{
    myValue = value;
    
    int dv = myValue - curValue;
    if(step == 0)step = 1;
    
    addValue = dv / step;
    
    if (addValue == 0 || isNow)
    {
        curValue = myValue;
        state = state_idle;
        unschedule(schedule_selector(MyDynamicLabel::update));
        updateValue();
    }
    else
    {
        if(state == state_idle)
        {
            state = state_run;
            schedule(schedule_selector(MyDynamicLabel::update),0.0);
        }
    }
}

void MyDynamicLabel::update(float dt)
{
    curValue += addValue;
    if (addValue > 0 && curValue > myValue)
    {
        curValue = myValue;
        state = state_idle;
        unschedule(schedule_selector(MyDynamicLabel::update));
    }
    updateValue();
}

void MyDynamicLabel::updateValue()
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