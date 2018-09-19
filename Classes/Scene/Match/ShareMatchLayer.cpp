//
//  ShareMatchLayer.cpp
//  ZJH
//
//  Created by apple on 16/8/1.
//
//

#include "ShareMatchLayer.hpp"

bool ShareMatchLayer::init()
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
        __NotificationCenter::getInstance()->postNotification("removeShareImage");
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
    
    Text *title = Text::create("分享战绩至" , "", 45);
    title->setAnchorPoint(Vec2(0.5, 0));
    title->setPosition(Vec2(winSize.width/2, 445));
    title->setColor(Color3B(0xe1, 0xcd, 0xae));
    bg->addChild(title);
    
    for (int i = 0; i < 3; i++)
    {
        Button *but;
        Text *name;
        switch (i)
        {
            case 2:
                but = Button::create("Match/icon_wanbei.png","Match/icon_wanbei.png");
                but->setName("wanBei");
                but->setPosition(Vec2(winSize.width - 150, 250));
                name = Text::create("玩呗好友" , "", 36);
                break;
            case 0:
                but = Button::create("Match/icon_weiXin.png", "Match/icon_weiXin.png");
                but->setName("weiXin");
                but->setPosition(Vec2(150, 250));
                name = Text::create("微信好友" , "", 36);
                break;
            case 1:
                but = Button::create("Public/share_pengyouquan_icon.png", "Public/share_pengyouquan_icon.png");
                but->setName("FrindQuan");
                but->setPosition(Vec2(winSize.width/2, 250));
                name = Text::create("朋友圈" , "", 36);
                break;
        }
        but->addClickEventListener(CC_CALLBACK_1(ShareMatchLayer::shareCkick, this));
        
        bg->addChild(but);
        
        name->setTextHorizontalAlignment(TextHAlignment::CENTER);
        name->setPosition(Vec2(but->getPositionX(), 130));
        name->setTextColor(Color4B(0xff, 0xff, 0xff, 255));
        bg->addChild(name);
        
    }
    
    Button *cancel = Button::create("Match/icon_cancel.png", "Match/icon_cancel.png");
    cancel->setScale9Enabled(true);
    cancel->setAnchorPoint(Vec2(1, 0));
    cancel->setPosition(Vec2(winSize.width - 50, title->getPositionY()));
    bg->addChild(cancel);
    
    cancel->addClickEventListener([=](Ref*ref){
        
        this->removeFromParent();
    });
    
    return true;
}

void ShareMatchLayer::setPath(string path, Json::Value json)
{
    imagePath = path;
    jsonDate = json;
}

void ShareMatchLayer::shareCkick(Ref*ref)
{
    Button *btn = (Button*)ref;
    
    if (btn->getName() == "weiXin")
    {
        PlatformHelper::WeChatShareImage("", imagePath.c_str());
        
    }else if (btn->getName() == "wanBei")
    {
        //post image to server
        CCHttpAgent::getInstance()->sendHttpImagePost("/img/upload/temp", imagePath, "shareWanBei",[=](std::string tag){
            
            CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
            
            CCHttpAgent::getInstance()->packets.erase(tag);
            CCHttpAgent::getInstance()->callbacks.erase(tag);
            
            if (loginPacket->status != 3)
            {
                PlatformHelper::showToast("网络链接失败，请稍后再试");
                return;
            }
            
            loginPacket->status = 0;
            
            if (loginPacket->resultIsOK())
            {
                Json::Value json;
                json["imagePath"] = loginPacket->recvVal["resultMap"]["imageUrl"];
                
                ZJHModel::getInstance()->gotoView(VIEW_SHARE_WANBEI_FRIEND, json);
            }
            else
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
            
            delete loginPacket;
        });

    
    }else if (btn->getName() == "FrindQuan")
    {
        PlatformHelper::WeChatFrendsCircleShareImage("", imagePath.c_str());
    }
    
    this->removeFromParent();
}

