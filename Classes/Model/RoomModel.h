//
//  RoomModel.h
//  ZJH
//
//  Created by 陈家勇 on 13-12-16.
//
//

#ifndef ZJH_RoomModel_h
#define ZJH_RoomModel_h

#include <string>
#include <stdint.h>

class RoomModel
{
public:
    RoomModel();
    void reset();
	int					mid;
	std::string			ip;
    int					port;
    int live;
};

#endif
