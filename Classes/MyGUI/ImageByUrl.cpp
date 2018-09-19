//
//  ImageByUrl.cpp
//  MyLuaGame
//
//  Created by 陈家勇 on 14-11-4.
//
//

#include "ImageByUrl.h"
#include "ZJH.h"

bool ImageByUrl::init()
{
    return Layout::init();
}

void ImageByUrl::load(int w, int h, std::string imageURL,string defaultImg)
{
    setContentSize(Size(w,h));
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2(0.5,0.5));
    this->imageURL = imageURL;
    image = ImageView::create();
    image->ignoreContentAdaptWithSize(false);
    image->setAnchorPoint(Vec2(0.5,0.5));
    image->setContentSize(Size(w,h));
    image->setPosition(Vec2(w / 2, h / 2));
    addChild(image);
    
    if (Utils::hasImageFile(imageURL)) {
        image->loadTexture(Utils::getImageSavePath(imageURL));
    } else {
        image->loadTexture(defaultImg);
        tag = Utils::getImageSavePath(imageURL);
        CCHttpAgent::getInstance()->sendHttpImage(imageURL, Utils::getImageSavePath(imageURL), tag, NULL);
        this->schedule(schedule_selector(ImageByUrl::loading), 0);
    }
}

void ImageByUrl::loading(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
    if (loginPacket && loginPacket->status != 0)
    {
        unschedule(schedule_selector(ImageByUrl::loading));
        
        CCHttpAgent::getInstance()->packets.erase(tag);
        CCHttpAgent::getInstance()->callbacks.erase(tag);
        
        if (loginPacket->status != 3)
        {
            return;
        }
        
        loginPacket->status = 0;
        
        if(Utils::hasImageFile(imageURL))
        {
            image->loadTexture(Utils::getImageSavePath(imageURL));
        }
        
        delete loginPacket;
    }
}
