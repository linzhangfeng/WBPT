//
//  MyDynamicLabelAtlas.hpp
//  ZJH
//
//  Created by admin on 16/3/31.
//
//

#ifndef MyDynamicLabelAtlas_hpp
#define MyDynamicLabelAtlas_hpp

#include "cocos2d.h"
#include "ZJH.h"

using namespace std;
using namespace cocos2d;
class MyDynamicLabelAtlas : public cocos2d::LabelAtlas
{
    enum
    {
        state_idle = 0,
        state_run,
    };
    
public:
    
    static MyDynamicLabelAtlas* create(const std::string& string, const std::string& charMapFile, int itemWidth, int itemHeight, int startCharMap);
    
    void setValue(int value,int step = 30, bool isNow = false);
    void update(float dt);
    void setUpdateCallback(const MyCallBack_Int &callback){this->callback = callback;};
private:
    void updateValue();
    
    int myValue;
    int curValue;
    int addValue;
    int state;
    MyCallBack_Int callback;
};

#endif /* MyDynamicLabelAtlas_hpp */
