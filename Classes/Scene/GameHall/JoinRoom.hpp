//
//  JoinRoom.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef JoinRoom_hpp
#define JoinRoom_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"
#include "../Loading/Loading.h"

class JoinRoom : public Layer
{
public:
    CREATE_FUNC(JoinRoom);
    bool init();
    void numsOnClick(Ref* r);
    
private:
    void setCodeText(string code);
    vector<LabelAtlas*> allNumsLabel;
    string codeStr;
};

#endif /* JoinRoom_hpp */
