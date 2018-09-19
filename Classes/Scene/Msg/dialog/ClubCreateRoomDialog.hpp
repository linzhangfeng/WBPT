//
//  ClubCreateRoomDialog.hpp
//  ZJH
//
//  Created by mac on 2017/4/26.
//
//

#ifndef ClubCreateRoomDialog_hpp
#define ClubCreateRoomDialog_hpp
#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"


using namespace cocos2d::ui;
using namespace cocos2d;


class ClubCreateRoomDialog: public Layer
{
public:
    ListView* gameList;
    
    void initView(int clubId);
    
    void showView(Json::Value data);
    
    CREATE_FUNC(ClubCreateRoomDialog);
    
private:
    
    
};

class Game_Item : public Layout
{
public:
    CREATE_FUNC(Game_Item);
    bool init();
    void initView(Json::Value data);
    void createRoom(int clubId,int clubPlayId,int gameId);
};


#endif /* ClubCreateRoomDialog_hpp */
