//
//  RoomModel.cpp
//  ZJH
//
//  Created by 陈家勇 on 13-12-16.
//
//

#include "RoomModel.h"
RoomModel::RoomModel()
{
    reset();
}

void RoomModel::reset()
{
	mid = 0;
	ip.clear();
    port = 0;
    live = 0;
}