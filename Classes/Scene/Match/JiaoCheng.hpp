//
//  JiaoCheng.hpp
//  ZJH
//
//  Created by apple on 16/8/17.
//
//

#ifndef JiaoCheng_hpp
#define JiaoCheng_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ZJh.h"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class JiaoCheng: public Layer
{
    
public:
    virtual bool init();
    CREATE_FUNC(JiaoCheng);
    void jiaocheng_jionRoom();
    void jiaoCheng_clickGame();
    void jiaoCheng_clickDiqu();
    void jiaoCheng_clickWithoutRoom();
private:
};
#endif /* JiaoCheng_hpp */
