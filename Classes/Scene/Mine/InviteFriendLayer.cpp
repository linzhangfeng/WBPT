//
//  wanBeiShop.cpp
//  ZJH
//
//  Created by apple on 16/6/30.
//
//

#include "InviteFriendLayer.hpp"
#include "ShareAppInviteLayer.hpp"
#include "MaskSelectLayer.hpp"
#include "../../MyGUI/ImageByUrl.h"

bool InviteFriendLayer::init()
{
    if (!Widget::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - 270;
    
    this->setContentSize(Size(visibleSize.width, H));
    
    //2:请求
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"invite/index", "" ,"inviteFriend");
    this->schedule(schedule_selector(InviteFriendLayer::postData), 0.0f);
    
    loading = Loading::startLoading(this, Director::getInstance()->getWinSize() / 2);
    loading->maskSwllowTouch();
    
    return true;
}

void InviteFriendLayer::postData(float dt)
{
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["inviteFriend"];
    if (packet && packet->status != 0)
    {
        loading->removeFromParent();
        unschedule(schedule_selector(InviteFriendLayer::postData));
        
        if (packet->status != 3)
        {
            return;
        }
        packet->status = 0;
        
        if (packet->resultIsOK())
        {
            Json::Value data = packet->recvVal["resultMap"];
            dataJson = data;
            showPage1(data);
        }else
        {
            PlatformHelper::showToast(packet->recvVal["message"].asCString());
            unschedule(schedule_selector(InviteFriendLayer::postData));
        }
        
        CCHttpAgent::getInstance()->packets["inviteFriend"] = NULL;
        delete packet;
    }
}

void InviteFriendLayer::showPage1(Json::Value json)
{
    this->removeAllChildren();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = this->getContentSize().height;
    
    for (int i = 0; i < 3; i++)
    {
        Button *but;
        Text *name;
        switch (i)
        {
            case 0:
                but = Button::create("Match/icon_weiXin.png", "Match/icon_weiXin.png");
                but->setName("weiXin");
                but->setPosition(Vec2(visibleSize.width/2 - 350, 230));
                name = Text::create("微信好友" , "", 36);
                break;
            case 1:
                but = Button::create("Public/share_pengyouquan_icon.png", "Public/share_pengyouquan_icon.png");
                but->setName("FrindQuan");
                but->setPosition(Vec2(visibleSize.width/2, 230));
                name = Text::create("朋友圈" , "", 36);
                break;
            case 2:
                but = Button::create("Mine/invite_mianduiMian.png", "Mine/invite_mianduiMian.png");
                but->setName("mianDuiMian");
                but->setPosition(Vec2(visibleSize.width/2 + 350, 230));
                name = Text::create("面对面" , "", 36);
                break;
        }
        but->addClickEventListener(CC_CALLBACK_1(InviteFriendLayer::btnClick, this));
        this->addChild(but);
        
        name->setTextHorizontalAlignment(TextHAlignment::CENTER);
        name->setPosition(Vec2(but->getPositionX(), 110));
        name->setTextColor(Color4B(0xff, 0xff, 0xff, 255));
        this->addChild(name);
        
    }
    
    auto line1 = Layout::create();
    line1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line1->setBackGroundColor(Color3B(0xe0, 0xcf, 0xae));
    line1->setAnchorPoint(Vec2(1, 0.5));
    line1->setPosition(Vec2(visibleSize.width/2 - 140, 400));
    line1->setContentSize(Size(110, 4));
    this->addChild(line1);
    
    auto line2 = Layout::create();
    line2->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line2->setBackGroundColor(Color3B(0xe0, 0xcf, 0xae));
    line2->setAnchorPoint(Vec2(0, 0.5));
    line2->setPosition(Vec2(visibleSize.width/2 + 140, 400));
    line2->setContentSize(Size(110, 4));
    this->addChild(line2);
    
    Text *shareT = Text::create("分享推荐", "", 45);
    shareT->setTextColor(Color4B(0xe0, 0xcf, 0xae, 255));
    shareT->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    shareT->setPosition(Vec2(visibleSize.width/2, 400));
    this->addChild(shareT);
    
    ListView *listView = ListView::create();
    listView->setDirection(ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(cocos2d::Size(visibleSize.width, H-450));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 450));
    listView->setItemsMargin(20);
    listView->setScrollBarEnabled(false);
    this->addChild(listView);
    
    string imageUrl = json["img"].asString();
    ImageByUrl* barBg = ImageByUrl::create();
    barBg->load(visibleSize.width, 385, imageUrl, "Mine/invite_banner.png");
    barBg->image->setTouchEnabled(true);
    //barBg->setAnchorPoint(Vec2(0, 1));
    //barBg->setPosition(Vec2(0, H));
    listView->addChild(barBg);
    
    Layout *textLayout = Layout::create();
    textLayout->setContentSize(Size(Vec2(visibleSize.width, 130)));
    listView->addChild(textLayout);
    
    string content = json["activeDesc"].asString();
    LabelTTF *text1 = LabelTTF::create(content, "", 36, Size(visibleSize.width - 20, 130), TextHAlignment::LEFT);
    text1->setAnchorPoint(Vec2(0, 0.5));
    text1->setPosition(Vec2(20, textLayout->getContentSize().height/2));
    text1->setColor(Color3B(0xe6, 0xe6, 0xe6));
    textLayout->addChild(text1);
    
    for (int i = 0; i < 3; i++)
    {
        Layout *LayoutBg = Layout::create();
        LayoutBg->setBackGroundColor(Color3B(0x14, 0x14, 0x15));
        LayoutBg->setContentSize(Size(Vec2(visibleSize.width, 200)));
        listView->addChild(LayoutBg);
        
        ImageView *list1 = ImageView::create("Match/set_item_bgbar.png");
        list1->setScale9Enabled(true);
        list1->setContentSize(Size(Vec2(LayoutBg->getContentSize().width-20, LayoutBg->getContentSize().height)));
        list1->setPosition(Vec2(LayoutBg->getContentSize().width/2, LayoutBg->getContentSize().height/2));
        list1->setAnchorPoint(Vec2(0.5, 0.5));
        LayoutBg->addChild(list1);
        
        Text *leftTitle = Text::create("", "", 50);
        leftTitle->setAnchorPoint(Vec2(0, 0.5));
        leftTitle->setPosition(Vec2(40, list1->getContentSize().height/2));
        leftTitle->setTextColor(Color4B(0xe6, 0xe6, 0xe6, 255));
        list1->addChild(leftTitle);
        
        Text *rightTitle = Text::create("", "", 50);
        rightTitle->setAnchorPoint(Vec2(1, 0.5));
        rightTitle->setPosition(Vec2(visibleSize.width - 40, list1->getContentSize().height/2));
        rightTitle->setTextColor(Color4B(0xfd, 0xe4, 0x5f, 255));
        list1->addChild(rightTitle);
        
        switch (i)
        {
            case 0:
                leftTitle->setString("你的推荐码");
                rightTitle->setString(json["inviteCode"].asString());

                break;
            case 1:
            {
                leftTitle->setString("已获得奖励");
                
                RichText *rightRich = RichText::create();
                rightRich->setAnchorPoint(Vec2(1, 0.5));
                rightRich->ignoreContentAdaptWithSize(true);
                rightRich->setContentSize(Size(list1->getContentSize().width, 200));
                rightRich->setPosition(Vec2(list1->getContentSize().width - 40, list1->getContentSize().height/2));
                list1->addChild(rightRich);
                
                __String *bufbuf = __String::createWithFormat("* %d", json["totalWanbei"].asInt());
                
                RichElementText* re1 = RichElementText::create(1, Color3B(0xfd, 0xe4, 0x5f), 255,bufbuf->getCString(), "", 50);
                
                RichElementImage *re2 = RichElementImage::create(2, Color3B::WHITE, 255, "Mine/invite_crad_small.png");
                re2->setWidth(130);
                re2->setHeight(100);

                rightRich->pushBackElement(re2);
                rightRich->pushBackElement(re1);
            }
                break;
            case 2:
            {
                __String *bufStr = NULL;
                if (json["inviteNum"].asInt() > 0)
                {
                    list1->addTouchEventListener(CC_CALLBACK_2(InviteFriendLayer::gotoFriendList, this));
                    list1->setTouchEnabled(true);
                    bufStr = __String::createWithFormat("%d >", json["inviteNum"].asInt());
                }else
                {
                    bufStr = __String::createWithFormat("%d", json["inviteNum"].asInt());
                }
                
                leftTitle->setString("共推荐好友");
                rightTitle->setString(bufStr->getCString());
                rightTitle->setPosition(Vec2(list1->getContentSize().width - 40, list1->getContentSize().height/2));
            }
                break;
        }
    }

}

void InviteFriendLayer::btnClick(Ref*ref)
{
    Button *btn = (Button*)ref;
    if (btn->getName() == "weiXin")
    {
        std::string imageUrl = dataJson["url"].asString();
        if (Utils::hasImageFile(imageUrl)) {
            PlatformHelper::WeChatShare(dataJson["inviteTitle"].asString().c_str(), dataJson["inviteContent"].asString().c_str(), dataJson["inviteUrl"].asString().c_str(), Utils::getImageSavePath(dataJson["url"].asString()).c_str());
        } else {
            CCHttpAgent::getInstance()->sendHttpImageWithUrl(imageUrl, Utils::getImageSavePath(imageUrl), Utils::getImageSavePath(imageUrl), [=](std::string tag)
                                                             {
                                                                 CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                                 if (packet) {
                                                                     CCHttpAgent::getInstance()->packets.erase(tag);
                                                                     delete packet;
                                                                 }
                                                                 
                                                                 PlatformHelper::WeChatShare(dataJson["inviteTitle"].asString().c_str(), dataJson["inviteContent"].asString().c_str(), dataJson["inviteUrl"].asString().c_str(), Utils::getImageSavePath(dataJson["url"].asString()).c_str());
                                                             });
        }
        
        
    }else if (btn->getName() == "FrindQuan")
    {
        std::string imageUrl = dataJson["url"].asString();
        if (Utils::hasImageFile(imageUrl))
        {
            PlatformHelper::WeFriendsCircleShareURL(dataJson["inviteTitle"].asString().c_str(), dataJson["inviteContent"].asString().c_str(), dataJson["inviteUrl"].asString().c_str(), Utils::getImageSavePath(dataJson["url"].asString()).c_str(), dataJson["url"].asString().c_str());
            
        } else {
            CCHttpAgent::getInstance()->sendHttpImageWithUrl(imageUrl, Utils::getImageSavePath(imageUrl), Utils::getImageSavePath(imageUrl), [=](std::string tag)
                                                             {
                                                                 CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                                 if (packet) {
                                                                     CCHttpAgent::getInstance()->packets.erase(tag);
                                                                     delete packet;
                                                                 }
                                                                 
                                                                 PlatformHelper::WeFriendsCircleShareURL(dataJson["inviteTitle"].asString().c_str(), dataJson["inviteContent"].asString().c_str(), dataJson["inviteUrl"].asString().c_str(), Utils::getImageSavePath(dataJson["url"].asString()).c_str(), dataJson["url"].asString().c_str());
                                                             });
        }

    }else if(btn->getName() == "mianDuiMian")
    {
        mianDuiMianErWeiMaView();
    }
}

void InviteFriendLayer::mianDuiMianErWeiMaView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout *erWeima = Layout::create();
    erWeima->setTouchEnabled(true);
    erWeima->setSwallowTouches(true);
    erWeima->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    erWeima->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    erWeima->setBackGroundColorOpacity(160);
    erWeima->setContentSize(visibleSize);
    erWeima->addClickEventListener([=](Ref*ref){
        
        erWeima->removeFromParentAndCleanup(true);
        
    });
    Director::getInstance()->getRunningScene()->addChild(erWeima, 10000);
    
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(720, 800));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    erWeima->addChild(bg);
    
    Text *title = Text::create("扫码下载玩呗斗牌 送房卡", "", 50);
    title->setTextColor(Color4B(0xf0, 0xcc, 0x5b, 255));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 75));
    bg->addChild(title);
    
    Button *cancel = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    cancel->setPosition(Vec2(bg->getContentSize().width-15, bg->getContentSize().height-15));
    cancel->addClickEventListener([=](Ref*ref){
        
        erWeima->removeFromParentAndCleanup(true);
        
    });
    bg->addChild(cancel);
    
    ImageView *image = ImageView::create();
    image->ignoreContentAdaptWithSize(false);
    image->setContentSize(Size(565, 565));
    image->setPosition(Vec2(bg->getContentSize().width/2 , bg->getContentSize().height/2-30));
    bg->addChild(image);
    
    //图片获得二维码图片
    Loading *erEriLoading = Loading::startLoading(bg, bg->getContentSize() / 2);
    erEriLoading->maskSwllowTouch();
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        erEriLoading->removeFromParent();
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"]["user"];
            
            string imageName = data["qrCode"].asString();
            
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

        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    }, "account/userInfo", "", "erWeima");

}
void InviteFriendLayer::gotoFriendList(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        ZJHModel::getInstance()->gotoView(VIEW_INVITE_FRIEND);
    }
}

void InviteFriendLayer::shareMaskLayerFun(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        MaskSelectLayer *shareMask = MaskSelectLayer::create(3);
        shareMask->setPosition(Vec2::ZERO);
        this->addChild(shareMask);
    }
}


