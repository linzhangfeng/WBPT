//
//  ImageListView.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "ImageListView.hpp"
#include "../../Model/MsgModel.h"
#include "proto.h"

bool ImageListView::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setName("ImageListView");
    
    return true;
}

void ImageListView::myInit(Json::Value &json)
{
    Size size = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(20, 20, 22, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", NULL);
    
    setTitle("图片");
    
//    for (int i = 0; i < json["list"].size(); i ++)
//    {
//        
//    }
    std::string content = json["img"].asString();
    ImageView *image = ImageView::create();
    image->setContentSize(size);
    image->setAnchorPoint(Vec2(0.5, 0.5));
    image->setPosition(size / 2);
    image->ignoreContentAdaptWithSize(false);
    image->setFillContentSizeEnabled(false);
    addChild(image);
    std::string img_save_path = Utils::getImageSavePath(content);
    if (Utils::hasImageFile(content)) {
        image->loadTexture(img_save_path);
    } else {
        CCHttpAgent::getInstance()->sendHttpImage(content, img_save_path, img_save_path, [=](std::string tag)
                                                  {
                                                      CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                      if (packet) {
                                                          CCHttpAgent::getInstance()->packets.erase(tag);
                                                          delete packet;
                                                      }
                                                      
                                                      if (image == NULL || image->getReferenceCount() <= 0 || image->getReferenceCount() > 10) return;
                                                      
                                                      if(Utils::hasImageFile(content))
                                                      {
                                                          image->loadTexture(img_save_path);
                                                      }
                                                  });
    }

}