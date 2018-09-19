//
//  ShareAppInviteLayer.cpp
//  ZJH
//
//  Created by apple on 16/8/4.
//
//

#include "ShareAppInviteLayer.hpp"

bool ShareAppInviteLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    // 创建一个事件监听器类型为 OneByOne 的单点触摸
    auto listener1 = EventListenerTouchOneByOne::create();
    // 设置是否吞没事件，在 onTouchBegan 方法返回 true 时吞没
    listener1->setSwallowTouches(true);
    
    // 使用 lambda 实现 onTouchBegan 事件回调函数
    listener1->onTouchBegan = [](Touch* touch, Event* event) -> bool {
        // 获取事件所绑定的 target
        return true;
    };
    
    
    // 点击事件结束处理
    listener1->onTouchEnded = [=](Touch* touch, Event* event)
    {
        this->removeFromParent();
        
    };
    EventDispatcher * eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
    auto winSize = Director::getInstance()->getVisibleSize();
    
    Layout *bg = Layout::create();
    bg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    bg->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1f));
    bg->setContentSize(Size(winSize.width, 525));
    this->addChild(bg);
    
    auto line1 = Layout::create();
    line1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line1->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    line1->setContentSize(Size(winSize.width, 3));
    line1->setAnchorPoint(Vec2(0, 1));
    line1->setPosition(Vec2(0, bg->getContentSize().height));
    bg->addChild(line1);
    
    auto line2 = Layout::create();
    line2->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line2->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    line2->setContentSize(Size(winSize.width, 3));
    line2->setAnchorPoint(Vec2(0, 1));
    line2->setPosition(Vec2(0, bg->getContentSize().height - 120));
    bg->addChild(line2);
    
    titleText = Text::create("分享" , "", 45);
    titleText->setAnchorPoint(Vec2(0.5, 0));
    titleText->setPosition(Vec2(winSize.width/2, 445));
    titleText->setColor(Color3B(0xe1, 0xcd, 0xae));
    bg->addChild(titleText);
    
    for (int i = 0; i < 2; i++)
    {
        Button *but;
        Text *name;
        switch (i)
        {
            case 0:
                but = Button::create("Match/icon_weiXin.png", "Match/icon_weiXin.png");
                but->setName("weiXin");
                but->setPosition(Vec2(winSize.width/2 - 250, 250));
                name = Text::create("微信好友" , "", 36);
                break;
            case 1:
                but = Button::create("Public/share_pengyouquan_icon.png", "Public/share_pengyouquan_icon.png");
                but->setName("FrindQuan");
                but->setPosition(Vec2(winSize.width/2 + 250, 250));
                name = Text::create("朋友圈" , "", 36);
                break;
        }
        but->addClickEventListener(CC_CALLBACK_1(ShareAppInviteLayer::btnClick, this));
        bg->addChild(but);
        
        name->setTextHorizontalAlignment(TextHAlignment::CENTER);
        name->setPosition(Vec2(but->getPositionX(), 130));
        name->setTextColor(Color4B(0xff, 0xff, 0xff, 255));
        bg->addChild(name);
        
    }

    Button *cancel = Button::create("Match/icon_cancel.png", "Match/icon_cancel.png");
    cancel->setScale9Enabled(true);
    cancel->setAnchorPoint(Vec2(1, 0));
    cancel->setPosition(Vec2(winSize.width - 50, titleText->getPositionY()));
    bg->addChild(cancel);
    
    cancel->addClickEventListener([=](Ref*ref){
        
        this->removeFromParent();
    });
    
    return true;
}

void ShareAppInviteLayer::btnClick(Ref*ref)
{
    Button *btn = (Button*)ref;
    if (btn->getName() == "weiXin")
    {
        switch (typeIndex)
        {
            case 1:
            {
                //invite
                std::string imageUrl = shareJson["url"].asString();
                if (Utils::hasImageFile(imageUrl)) {
                    PlatformHelper::WeChatShare(shareJson["inviteTitle"].asString().c_str(), shareJson["inviteContent"].asString().c_str(), shareJson["inviteUrl"].asString().c_str(), Utils::getImageSavePath(shareJson["url"].asString()).c_str());
                } else {
                    CCHttpAgent::getInstance()->sendHttpImageWithUrl(imageUrl, Utils::getImageSavePath(imageUrl), Utils::getImageSavePath(imageUrl), [=](std::string tag)
                                                              {
                                                                  CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                                  if (packet) {
                                                                      CCHttpAgent::getInstance()->packets.erase(tag);
                                                                      delete packet;
                                                                  }
                                                                  
                                                                  PlatformHelper::WeChatShare(shareJson["inviteTitle"].asString().c_str(), shareJson["inviteContent"].asString().c_str(), shareJson["inviteUrl"].asString().c_str(), Utils::getImageSavePath(shareJson["url"].asString()).c_str());
                                                              });
                }
                
            break;
            }
            case 2:
                //app
                PlatformHelper::WeChatShare(shareJson["title"].asString().c_str(), "", shareJson["url"].asString().c_str(), "");
                break;
                
            case 3:
            {
                __String *bufStr = __String::createWithFormat("邀请你加入我的玩呗俱乐部，使用邀请码[%s]可加入我的俱乐部。", shareJson["inviteCode"].asString().c_str());
                //俱乐部推广
                PlatformHelper::WeChatShare("加入棋牌俱乐部一起对战", bufStr->getCString(), shareJson["shareUrl"].asString().c_str(), "");
            }
            break;
        }
        
    }else if (btn->getName() == "FrindQuan")
    {
        switch (typeIndex)
        {
            case 1:
                //invite
            {
                std::string imageUrl = shareJson["url"].asString();
                if (Utils::hasImageFile(imageUrl)) {
                    PlatformHelper::WeFriendsCircleShare(shareJson["inviteTitle"].asString().c_str(), shareJson["inviteContent"].asString().c_str(), shareJson["inviteUrl"].asString().c_str(), Utils::getImageSavePath(shareJson["url"].asString()).c_str());
                    
                } else {
                    CCHttpAgent::getInstance()->sendHttpImageWithUrl(imageUrl, Utils::getImageSavePath(imageUrl), Utils::getImageSavePath(imageUrl), [=](std::string tag)
                                                                     {
                                                                         CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                                         if (packet) {
                                                                             CCHttpAgent::getInstance()->packets.erase(tag);
                                                                             delete packet;
                                                                         }
                                                                         
                                                                         PlatformHelper::WeFriendsCircleShare(shareJson["inviteTitle"].asString().c_str(), shareJson["inviteContent"].asString().c_str(), shareJson["inviteUrl"].asString().c_str(), Utils::getImageSavePath(shareJson["url"].asString()).c_str());
                                                                     });
                }

                
                break;
            }
            case 2:
                //app
                PlatformHelper::WeFriendsCircleShare(shareJson["title"].asString().c_str(), "", shareJson["url"].asString().c_str(), "");
                break;
            case 3:
                //俱乐部推广
                __String *bufStr = __String::createWithFormat("邀请你加入我的玩呗俱乐部，使用邀请码[%s]可加入我的俱乐部。", shareJson["inviteCode"].asString().c_str());
                //俱乐部推广
                PlatformHelper::WeFriendsCircleShare("加入棋牌俱乐部一起对战", bufStr->getCString(), shareJson["shareUrl"].asString().c_str(), "");

                break;
        }
    }
}
void ShareAppInviteLayer::setShareTypeView(int type, Json::Value json)
{
    typeIndex = type;
    shareJson = json;
    
    if (type == 3)
    {
        titleText->setString("推广至");
    }
}
