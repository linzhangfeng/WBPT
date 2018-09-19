//
//  ChatRecordEffect.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef ChatRecordEffect_hpp
#define ChatRecordEffect_hpp

#include "ZJH.h"
USING_NS_CC;

class ChatRecordEffect : public Layer
{
public:
    CREATE_FUNC(ChatRecordEffect);
    bool init();
    void recordStart(Ref* r);
    void recordIng(Ref* r);
    void recordOver(Ref* r);
    void recordStop(Ref* r);
    void recordError(Ref* r);
    void recordCancel(Ref* r);
    void recordTimeOut(Ref* r);
    void playNoFile(Ref* r);
    void onExit();
    int contentType;
private:
    Sprite* recordbg;
    Sprite* recording;
    Sprite* error_tooshort;
    
};

#endif /* ChatRecordEffect_hpp */
