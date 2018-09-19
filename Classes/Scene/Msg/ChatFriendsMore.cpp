//
//  ChatFriendsMore.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "ChatFriendsMore.hpp"
#include "../../Model/MsgModel.h"
#include "proto.h"
#include "../../sqlite3/DataBase.h"
#include "FriendRemark.h"

#define CELL_SWITCH_TAG_1   100 //开关背景－－通知开关tag
#define CELL_SWITCH_ON      103 //开关on 图片
#define CELL_SWITCH_OFF     104 //开关off 图片

bool ChatFriendsMore::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setName("ChatFriendsMore");
    
    Size size = Director::getInstance()->getWinSize();
    return true;
}

ImageView* getMoreItem(Size size,Color4B color,std::string title,bool hasArrow = true)
{
    ImageView *image = ImageView::create();
    image->setContentSize(size);
    image->setTouchEnabled(true);
    image->ignoreContentAdaptWithSize(false);
    
    LayerColor* info_bg = LayerColor::create(color);
    info_bg->setContentSize(size);
    info_bg->setAnchorPoint(Vec2(0.0f, 1.0f));
    info_bg->setPosition(0,0);
    image->addChild(info_bg,-100);
    
    Text* name = Text::create(title, "AmericanTypewriter", 45);
    name->setColor(Color3B(208, 208, 208));
    name->setAnchorPoint(Vec2(0, 0.5));
    name->setPosition(Vec2(100,size.height * 0.5));
    image->addChild(name,1000);
    
    if (hasArrow)
    {
        Sprite* avatar = Sprite::create("Msg/icon_more.png");
        avatar->setPosition(size.width - 58, size.height * 0.5);
        image->addChild(avatar);
    }
    
    return image;
}

ImageView* ChatFriendsMore::getMoreItemswitch(Size size,Color4B color,std::string title,bool hasArrow){
    
    
    ImageView *image = ImageView::create();
    image->setContentSize(size);
    image->setTouchEnabled(true);
    image->ignoreContentAdaptWithSize(false);
    
    LayerColor* info_bg = LayerColor::create(color);
    info_bg->setContentSize(size);
    info_bg->setAnchorPoint(Vec2(0.0f, 1.0f));
    info_bg->setPosition(0,0);
    image->addChild(info_bg,-100);
    
    Text* name = Text::create(title, "AmericanTypewriter", 45);
    name->setColor(Color3B(208, 208, 208));
    name->setAnchorPoint(Vec2(0, 0.5));
    name->setPosition(Vec2(100,size.height * 0.5));
    image->addChild(name,1000);
    
    rightOn = ImageView::create("Msg/switch_gray_on.png");
    rightOff = ImageView::create("Msg/switch_gray.png");
    
    Layout* layoutSwitch = Layout::create();
    layoutSwitch->setContentSize(Size(rightOn->getContentSize().width, image->getContentSize().height));
    layoutSwitch->setTouchEnabled(true);
    layoutSwitch->addTouchEventListener(CC_CALLBACK_2(ChatFriendsMore::swithClickFun, this));
    
    layoutSwitch->setTag(CELL_SWITCH_TAG_1);
//    layoutSwitch->ignoreAnchorPointForPosition(false);
    layoutSwitch->setAnchorPoint(Vec2(0, 0.5));
    layoutSwitch->setPosition(Vec2(image->getContentSize().width - rightOn->getContentSize().width - 20,size.height * 0.5));
    image->addChild(layoutSwitch);
    
    
    rightOn->setAnchorPoint(Vec2(0.5, 0.5));
    rightOn->setTag(CELL_SWITCH_ON);
    rightOn->setTouchEnabled(false);
    rightOn->setPosition(Vec2(rightOn->getContentSize().width/2, image->getContentSize().height/2));
    layoutSwitch->addChild(rightOn);

    
    rightOff->setAnchorPoint(Vec2(0.5, 0.5));
    rightOff->setTag(CELL_SWITCH_OFF);
    rightOff->setPosition(Vec2(rightOn->getPosition()));
    rightOff->setTouchEnabled(false);
    layoutSwitch->addChild(rightOff);
    
    updateBlackFlag();
    return image;
}

void ChatFriendsMore::swithClickFun(Ref *pSender, Widget::TouchEventType type){
    if(type ==  Widget::TouchEventType::ENDED){
        MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[_cuid];
        
        bool flagblack = true;
        if (fi->blackType == FRIEND_TYPE::FRIEND_TYPE_NULL_TO_NULL || fi->blackType == FRIEND_TYPE::FRIEND_TYPE_NULL_TO_FRIEND)
        {
            flagblack = false;
        }
        
        if (flagblack)
        {
            delBlckFriend();
        }
        else
        {
            addBlckFriend();
        }
    }
}

void ChatFriendsMore::updateBlackFlag()
{
    MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[_cuid];
    
    bool flagblack = true;
    if (fi->blackType == FRIEND_TYPE::FRIEND_TYPE_NULL_TO_NULL || fi->blackType == FRIEND_TYPE::FRIEND_TYPE_NULL_TO_FRIEND)
    {
        flagblack = false;
    }
    
    if (flagblack)
    {
        rightOff->setVisible(true);
        rightOn->setVisible(false);
    }
    else
    {
        rightOff->setVisible(false);
        rightOn->setVisible(true);
    }
}

void ChatFriendsMore::updateFriendFlag()
{
    MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[_cuid];
    
    bool flagfriend = true;
    
    if (fi->friendType == FRIEND_TYPE::FRIEND_TYPE_NULL_TO_NULL || fi->friendType == FRIEND_TYPE::FRIEND_TYPE_NULL_TO_FRIEND)
    {
        flagfriend = false;
    }
    
    if (flagfriend)
    {
        //默认开
        getChildByName("del_friend")->setVisible(true);
        getChildByName("add_friend")->setVisible(false);
    }
    else
    {
        getChildByName("del_friend")->setVisible(false);
        getChildByName("add_friend")->setVisible(true);
    }
    
    __NotificationCenter::getInstance()->postNotification("ChatFriendsInfo_reflash");
}

void ChatFriendsMore::myInit(Json::Value &json)
{
    _cuid = json["uid"].asInt();
    
    MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[_cuid];
    
    Size size = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(20, 20, 22, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", NULL);
    
    setTitle("更多");
    
    int bx = size.width / 2;
    int dy = 28 + 136;
    int by = size.height - getHeadH() - dy;
    
    ImageView *Remarks = getMoreItem(Size(size.width,136),Color4B(29,29,31, 255),"备注");
    Remarks->setName("remarks");
    Remarks->setAnchorPoint(Vec2(0.5, 0));
    Remarks->setPosition(Vec2(bx, by));
    Remarks->addClickEventListener([=](Ref *ref)
                                        {
                                            //PlatformHelper::showToast("尚未完成");
                                            FriendRemark* remark = FriendRemark::create();
                                            remark->setuid(_cuid);
                                            this->addChild(remark,10000);
                                        });
    addChild(Remarks);
    
    Blackname = getMoreItemswitch(Size(size.width,136),Color4B(29,29,31, 255),"加入黑名单");
    Blackname->setName("blackname");
    Blackname->setAnchorPoint(Vec2(0.5, 0));
    Blackname->setPosition(Vec2(bx, by - dy));
    addChild(Blackname);
    
    {
        Size itemSize = Size(size.width,389);
        
        LayerColor* info_bg = LayerColor::create(Color4B(29,29,31,255));
        info_bg->setContentSize(itemSize);
        info_bg->setAnchorPoint(Vec2(0.0f, 0.0f));
        info_bg->setPosition(0,0);
        addChild(info_bg,-100);
        
        auto btn = Button::create("Msg/btn_gray_small.png");
        btn->setScale9Enabled(true);
        btn->setContentSize(Size(Size(960, 120)));
        btn->setTitleText("清除私聊消息");
        btn->setTitleFontSize(50);
        btn->setTitleColor(Color3B(231,231,231));
        btn->setPosition(Vec2(itemSize.width / 2,272));
        btn->setVisible(true);
        btn->setName("clear_msg");
        addChild(btn,100);
        btn->addClickEventListener([=](Ref *ref)
                                            {
                                                MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[_cuid];
                                                fi->chatList.clear();
                                                DataBase::getInstance(ZJHModel::getInstance()->uid)->deleteChats(_cuid);
                                                __NotificationCenter::getInstance()->postNotification("ChatFriends_reflash",__String::create("all"));//notify chat view
                                                PlatformHelper::showToast("清除成功");
                                            });
        
        btn = Button::create("Msg/btn_red_big.png");
        btn->setScale9Enabled(true);
        btn->setContentSize(Size(Size(960, 120)));
        btn->setTitleText("删除好友");
        btn->setTitleFontSize(50);
        btn->setTitleColor(Color3B(248,221,219));
        btn->setPosition(Vec2(itemSize.width / 2,105));
        btn->setName("del_friend");
        btn->setVisible(true);
        addChild(btn,100);
        btn->addClickEventListener([=](Ref *ref)
                                   {
                                       this->DeletemyFriend();
                                   });
        
        btn = Button::create("Msg/btn_red_big.png");
        btn->setScale9Enabled(true);
        btn->setContentSize(Size(Size(960, 120)));
        btn->setTitleText("加为好友");
        btn->setTitleFontSize(50);
        btn->setTitleColor(Color3B(248,221,219));
        btn->setPosition(Vec2(itemSize.width / 2,105));
        btn->setName("add_friend");
        btn->setVisible(false);
        addChild(btn,100);
        btn->addClickEventListener([=](Ref *ref)
                                   {
                                       this->addFriend();
                                   });
        
        updateFriendFlag();
    }
}

void ChatFriendsMore::delBlckFriend(){
    Json::Value json_s;
    json_s["friendId"] = _cuid;
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
        
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        loginPacket->status = 0;
        
        if (loginPacket->resultIsOK())
        {
            MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[_cuid];
            fi->blackType = FRIEND_TYPE::FRIEND_TYPE_NULL_TO_NULL;
            
            this->updateBlackFlag();
            PlatformHelper::showToast("删除黑名单成功");
        }
        else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets[tag] = NULL;
        delete loginPacket;
        
    },"friend/deleteBlackUser",json_s.toStyledString());
}

void ChatFriendsMore::addBlckFriend(){
    Json::Value json_s;
    json_s["friendId"] = _cuid;
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
        
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        loginPacket->status = 0;
        
        if (loginPacket->resultIsOK())
        {
            __NotificationCenter::getInstance()->postNotification("MyFriend_reflash");
            MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[_cuid];
            fi->blackType = FRIEND_TYPE::FRIEND_TYPE_FRIEND_TO_NULL;
            fi->friendType = FRIEND_TYPE::FRIEND_TYPE_NULL_TO_FRIEND;
            
            this->updateBlackFlag();
            this->updateFriendFlag();
            
            PlatformHelper::showToast("添加黑名单成功");
        }
        else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets[tag] = NULL;
        delete loginPacket;
        
    },"friend/addBlackUser",json_s.toStyledString());
}

void ChatFriendsMore::DeletemyFriend(){
    Json::Value json_s;
    json_s["friendId"] = _cuid;
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
        
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        loginPacket->status = 0;
        
        if (loginPacket->resultIsOK())
        {
            PlatformHelper::showToast("删除好友成功");
            MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[_cuid];
            fi->friendType = FRIEND_TYPE::FRIEND_TYPE_NULL_TO_FRIEND;
            this->updateFriendFlag();
            __NotificationCenter::getInstance()->postNotification("MyFriend_reflash");
        }
        else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets[tag] = NULL;
        delete loginPacket;
        
    },"friend/deleteFriend",json_s.toStyledString());
}

void ChatFriendsMore::addFriend(){
    Json::Value json_s;
    json_s["friendId"] = _cuid;
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
        
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        loginPacket->status = 0;
        
        if (loginPacket->resultIsOK())
        {
            PlatformHelper::showToast("添加好友成功");
            MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[_cuid];
            fi->blackType = FRIEND_TYPE::FRIEND_TYPE_NULL_TO_FRIEND;
            fi->friendType = FRIEND_TYPE::FRIEND_TYPE_FRIEND_TO_NULL;
            
            this->updateBlackFlag();
            this->updateFriendFlag();
            __NotificationCenter::getInstance()->postNotification("MyFriend_reflash");
        }
        else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets[tag] = NULL;
        delete loginPacket;
        
    },"friend/addFriend",json_s.toStyledString());
}