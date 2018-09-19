//
//  RoomVerticalLayout.cpp
//  ZJH
//
//  Created by mac on 2016/11/14.
//

#include "RoomVerticalLayout.hpp"
#include "RoomCommon/RoomHeadFight.h"
#include "RoomBottom/RoomBottom.hpp"
bool RoomVerticalLayout::init()
{
    if (!Layer::init()) {
        return false;
    }
    roomBottom = RoomBottom::create();
//    roomBottom->setCallback([=](std::string tag)
//                             {
//                                 if (m_pCallback)m_pCallback(tag);
//                             });
//    roomBottom->setCallback(m_pCallback);
    roomHead = RoomHeadFight::create();
//    roomHead->setCallback([=](std::string tag)
//                          {
//                              if(m_pCallback)m_pCallback(tag);
//                          });
//    roomHead->setCallback(m_pCallback);
    setContentSize(Size(Director::getInstance()->getWinSize()));
    addChild(roomBottom);
    addChild(roomHead);
    return true;
}

void RoomVerticalLayout::setRequestFlag(bool flag){
    roomHead->setRequestFlag(flag);
}

void RoomVerticalLayout::setCallback(MyCallBack_Str var){
     roomHead->setCallback(var);
     roomBottom->setCallback(var);
}

MyCallBack_Str RoomVerticalLayout::getCallback(){
    return NULL;
}

void RoomVerticalLayout::setGamePlayStatu(bool  statu){
    roomBottom->setGamePlayStatu(statu);
}

void RoomVerticalLayout::setFastSoundVis(bool statu){
    roomBottom->setFastSoundVis(statu);
}

void RoomVerticalLayout::showPlayerList(){
    roomHead->showPlayerList();
}
