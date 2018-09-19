//
//  ErWeiMaLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/19.
//
//

#include "ErWeiMaLayer.hpp"
bool ErWeiMaLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("我的二维码");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    return true;
}

void ErWeiMaLayer::getUserData(string url)
{
    //图片
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    ImageView *image = ImageView::create();
    image->ignoreContentAdaptWithSize(false);
    image->setContentSize(Size(800, 800));
    image->setPosition(Vec2(visibleSize.width/2, (visibleSize.height - getHeadH())/2));
    this->addChild(image);
    
    string imageName = url;
    if (Utils::hasImageFile(imageName)) {
        image->loadTexture(Utils::getImageSavePath(imageName));
    } else {
        CCHttpAgent::getInstance()->sendHttpImage(imageName, Utils::getImageSavePath(imageName), Utils::getImageSavePath(imageName), [=](std::string tag)
                                                  {
                                                      if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
                                                      
                                                      CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                      if (packet) {
                                                          CCHttpAgent::getInstance()->packets.erase(tag);
                                                          delete packet;
                                                      }
                                                      
                                                      if (image == NULL || image->getReferenceCount() <= 0 || image->getReferenceCount() > 10) return;
                                                      
                                                      if(Utils::hasImageFile(imageName))
                                                      {
                                                          image->loadTexture(Utils::getImageSavePath(imageName));
                                                      }
                                                  });
    }

}