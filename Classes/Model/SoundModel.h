//
//  SoundModel.h
//  ZJH
//
//  Created by 陈家勇 on 13-12-16.
//
//

#ifndef ZJH_SoundModel_h
#define ZJH_SoundModel_h

#include <string>
#include <stdint.h>
#include "ZJH.h"

class SoundModel : public cocos2d::Ref
{
public:
    /** Return the shared instance **/
    static SoundModel *getInstance();
    int init();
    void reset();
    void addSound(std::string file){playFile.push_back(file);};
    void checkNext(float dt);
    void clear();
    void onChatStopEvent(Ref* arg);
    int can_play;
private:
    std::deque<std::string> playFile;
    std::string cur_playfile;
    
    bool playIng;
};

#endif
