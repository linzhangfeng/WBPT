//
//  ImageByUrl.cpp
//  MyLuaGame
//
//  Created by 陈家勇 on 14-11-4.
//
//

#include "MatchEnterIconByUrl.h"
#include "ZJH.h"

bool MatchEnterIconByUrl::init()
{
    return ImageView::init();
}

void MatchEnterIconByUrl::load(std::string imageURL, string defaultImg)
{
    this->imageURL = imageURL;
    
    if (Utils::hasImageFile(imageURL)) {
        this->loadTexture(Utils::getImageSavePath(imageURL));
    } else {
        this->loadTexture(defaultImg);
        tag = Utils::getImageSavePath(imageURL);
        CCHttpAgent::getInstance()->sendHttpImage(imageURL, Utils::getImageSavePath(imageURL), tag, NULL);
        this->schedule(schedule_selector(MatchEnterIconByUrl::loading), 0);
    }
}

void MatchEnterIconByUrl::loading(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
    
    printf("status = %d\n", loginPacket->status);
    
    if (loginPacket && loginPacket->status != 0)
    {
        unschedule(schedule_selector(MatchEnterIconByUrl::loading));
        
        CCHttpAgent::getInstance()->packets.erase(tag);
        CCHttpAgent::getInstance()->callbacks.erase(tag);
        
        if (loginPacket->status != 3)
        {
            return;
        }
        loginPacket->status = 0;
        
        if(Utils::hasImageFile(imageURL))
        {
            this->loadTexture(Utils::getImageSavePath(imageURL));
        }
        
        delete loginPacket;
    }
}
