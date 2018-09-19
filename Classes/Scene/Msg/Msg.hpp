//
//  Msg.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef Msg_hpp
#define Msg_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"
#include "../../Model/MsgModel.h"

USING_NS_CC;

class Msg : public BaseLayer
{
public:
    CREATE_FUNC(Msg);
    bool init();
    void btnCallback(Ref* ref);
private:
    std::map<std::string, Node*> itemMap;
};

#endif /* Msg_hpp */
