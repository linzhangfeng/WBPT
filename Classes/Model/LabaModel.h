#ifndef __LABA_MODEL_H__
#define __LABA_MODEL_H__

#include <string>
#include <stdint.h>
#include "ZJH.h"

class LabaModel : public Ref
{
public:
    static LabaModel *getInstance();
    
    void init();
    void start();
    
    void getData(float dt);
    void reflashData(float dt);
    
public:
    Json::Value msgData;
    float timeCount;
    
};

#endif