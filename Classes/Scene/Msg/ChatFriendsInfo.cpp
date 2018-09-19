//
//  ChatFriendsInfo.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "ChatFriendsInfo.hpp"
#include "../../Model/MsgModel.h"
#include "proto.h"
#include "OnGameFriend.h"

bool ChatFriendsInfo::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setName("ChatFriendsInfo");
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatFriendsInfo::notificationReflash), "ChatFriendsInfo_reflash", NULL);
    
    return true;
}

void ChatFriendsInfo::notificationReflash(Ref* msg)
{
    MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[uid];
    
    if (fi->friendType == FRIEND_TYPE::FRIEND_TYPE_NULL_TO_NULL || fi->friendType == FRIEND_TYPE::FRIEND_TYPE_NULL_TO_FRIEND)
    {
        getChildByName("send_msg")->setVisible(false);
        getChildByName("add_friend")->setVisible(true);
    }
    else
    {
        getChildByName("send_msg")->setVisible(true);
        getChildByName("add_friend")->setVisible(false);
    }
    
    if (fi->rmkName != "")
    {
        rmkname->setString(__String::createWithFormat("%s",fi->rmkName.c_str())->getCString());
    }
    else
    {
        rmkname->setString(__String::createWithFormat("%s",fi->nickName.c_str())->getCString());
    }
}

ImageView* getImageViewItem(std::string icon,std::string title,bool hasArrow = true)
{
    Size size = Director::getInstance()->getWinSize();
    
    Size itemSize = Size(size.width,136);
    ImageView *image = ImageView::create();
    image->setContentSize(itemSize);
    image->setTouchEnabled(true);
    image->ignoreContentAdaptWithSize(false);
    
    LayerColor* info_bg = LayerColor::create(Color4B(29,29,31,255));
    info_bg->setContentSize(itemSize);
    info_bg->setAnchorPoint(Vec2(0.0f, 0.0f));
    info_bg->setPosition(0,0);
    image->addChild(info_bg,-100);
    
    if (!icon.empty())
    {
        Sprite* icon_sp = Sprite::create(icon);
        icon_sp->setPosition(Vec2(70, itemSize.height / 2));
        image->addChild(icon_sp);
    }
    
    Text* name = Text::create(title, "AmericanTypewriter", 45);
    name->setColor(Color3B(208, 208, 208));
    name->setAnchorPoint(Vec2(0, 0.5));
    name->setPosition(Vec2(123,itemSize.height * 0.5));
    image->addChild(name,1000);
    
    Text* tipText = Text::create("", "AmericanTypewriter", 45);
    tipText->setColor(Color3B(112, 112, 112));
    tipText->setAnchorPoint(Vec2(1, 0.5));
    tipText->setName("tiptext");
    tipText->setPosition(Vec2(hasArrow ? itemSize.width - 80 : itemSize.width - 42,itemSize.height * 0.5));
    image->addChild(tipText,1000);

    
    if (hasArrow)
    {
        Sprite* avatar = Sprite::create("Msg/icon_more.png");
        avatar->setPosition(size.width - 58, itemSize.height * 0.5);
        image->addChild(avatar);
    }

    return image;
}

void ChatFriendsInfo::myInit(Json::Value &json)
{
    size = Director::getInstance()->getWinSize();
    
    uid = json["uid"].asInt();
    
    setBackground(Color4B(20, 20, 22, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", NULL);
    
    setTitle("个人资料");
    
    setMore_txt("更多", [=](){
        
        Json::Value json;
        json["uid"] = uid;
        ZJHModel::getInstance()->gotoView(VIEW_FRIENDS_MORE,json);
    });
    
    MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[uid];
    
    Size itemSize = Size(size.width,634);
    LayerColor* info_bg = LayerColor::create(Color4B(7, 7, 7, 255));
    info_bg->setContentSize(itemSize);
    info_bg->setAnchorPoint(Vec2(0.0f, 0.0f));
    info_bg->setPosition(0,size.height - getHeadH() - info_bg->getContentSize().height);
    addChild(info_bg);
    
    Sprite* userAvatar = Utils::createCircleAvatar(fi->photo, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(240,240));
    userAvatar->setPosition(Vec2(itemSize.width / 2, 426));
    info_bg->addChild(userAvatar);
    
    std::string name = fi->rmkName != "" ? fi->rmkName : fi->nickName;
    char buf[128];
    Utils::parseName(15, buf, name.c_str());
    
    rmkname = Text::create(buf, "AmericanTypewriter", 40);
    rmkname->setColor(Color3B(255, 255, 255));
    rmkname->setAnchorPoint(Vec2(0.5, 0.5));
    rmkname->setName("username");
    rmkname->setPosition(Vec2(itemSize.width / 2,277));
    info_bg->addChild(rmkname,1000);
    
//    rmkname = Text::create("", "AmericanTypewriter", 40);
//    rmkname->setColor(Color3B(112, 112, 112));
//    rmkname->setAnchorPoint(Vec2(0.5, 0.5));
//    rmkname->setName("rmkname");
//    rmkname->setPosition(Vec2(itemSize.width / 2,277 - 65));
//    info_bg->addChild(rmkname,1000);
    
    Text* id = Text::create(__String::createWithFormat("ID:%d",uid)->getCString(), "AmericanTypewriter", 40);
    id->setColor(Color3B(112, 112, 112));
    id->setAnchorPoint(Vec2(0.5, 0.5));
    id->setName("userid");
    id->setPosition(Vec2(itemSize.width / 2,147));
    info_bg->addChild(id,1000);
    
    
    int bx = size.width / 2;
    int dy = 28 + 136;
    int by = size.height - getHeadH() - 634 - 136 / 2;
    ImageView *signview = getImageViewItem("Msg/ps.png","签名",false);
    signview->setPosition(Vec2(bx, by));
    addChild(signview);
    
    ImageView *address = getImageViewItem("Msg/city.png","地址",false);
    address->setPosition(Vec2(bx, by - dy * 1));
    addChild(address);
    
    ImageView *love = getImageViewItem("Msg/love.png","爱好",false);
    love->setPosition(Vec2(bx, by - dy * 2));
    addChild(love);
    
    ImageView *fangjian = getImageViewItem("Msg/room.png","房间", true);
    fangjian->setPosition(Vec2(bx, by - dy * 3));
    fangjian->addClickEventListener([=](Ref *ref)
                                 {
                                     Json::Value json;
                                     json["uid"] = uid;
                                     ZJHModel::getInstance()->gotoView(VIEW_FRIENDS_ROOMS,json);
                                 });
    addChild(fangjian);
    
    ImageView *battle = getImageViewItem("Msg/battle.png","对战", true);
    battle->setPosition(Vec2(bx, by - dy * 4));
    battle->addClickEventListener([=](Ref *ref)
                                    {
                                        OnGameFriend* View = OnGameFriend::create();
                                        this->addChild(View,10000);
                                        View->requestMyOnGame(uid);
                                    });
    addChild(battle);
    
    {
        Size itemSize = Size(size.width,215);

        LayerColor* info_bg = LayerColor::create(Color4B(29,29,31,255));
        info_bg->setContentSize(itemSize);
        info_bg->setAnchorPoint(Vec2(0.0f, 0.0f));
        info_bg->setPosition(0,0);
        addChild(info_bg,-100);
        
        auto btn = Button::create("Msg/btn_yellow.png");
        btn->setScale9Enabled(true);
        btn->setContentSize(Size(Size(960, 120)));
        btn->setTitleText("发消息");
        btn->setTitleFontSize(40);
        btn->setTitleColor(Color3B(102,69,5));
        btn->setPosition(itemSize / 2);
        btn->setVisible(true);
        btn->setName("send_msg");
        addChild(btn,100);
        btn->addClickEventListener([=](Ref *ref)
                                   {
                                       this->disappear();
                                   });
        
        btn = Button::create("Msg/btn_yellow.png");
        btn->setScale9Enabled(true);
        btn->setContentSize(Size(Size(960, 120)));
        btn->setTitleText("加为好友");
        btn->setTitleFontSize(40);
        btn->setTitleColor(Color3B(102,69,5));
        btn->setPosition(itemSize / 2);
        btn->setName("add_friend");
        btn->setVisible(false);
        addChild(btn,100);
        btn->addClickEventListener([=](Ref *ref)
                                   {
                                       Json::Value json_s;
                                       json_s["friendId"] = uid;
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
                                               PlatformHelper::showToast("添加成功");
                                               fi->friendType = 3;
                                               notificationReflash(NULL);
                                               __NotificationCenter::getInstance()->postNotification("MyFriend_reflash");
                                           }
                                           else
                                           {
                                               PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                                           }
                                           
                                           CCHttpAgent::getInstance()->packets[tag] = NULL;
                                           delete loginPacket;
                                           
                                       },"friend/addFriend",json_s.toStyledString());
                                   });
    }
    
    Json::Value json_s;
    json_s["uid"] = uid;
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
            /*
             {"errorType":"SUCCESS","resultMap":{"blackType":0,"friendType":2,"friendRmkName":null,"otherUser":{"id":100014,"nickName":"IT黄贯中","userType":5,"phone":"15986759237","image":"http://188.188.1.179:20084/chess/chess_img/head/100014/1510301726222933.jpg","sex":0,"signature":"2\nwv3x。","blackList":1,"friends":23,"articleNumber":2,"shareNumber":0,"age":0,"birthday":null,"starSign":null,"hobby":null,"province":"辽宁省","city":"鞍山市","area":"海城市","address":"u","lastLoginTime":1468808457000,"createTime":1445912720000,"updateTime":1468808464000,"qrCode":"http://188.188.1.179:20084/chess/chess_img/qrcode/100014/15120415523762481000147344.png","channelId":86011001,"user_number":0,"adminId":0,"orderIndex":"I","vipInfo":{"userId":100014,"goldBonusCount":60000,"wanbeiBonus":30000,"goldBonusNum":2,"wanbeiBonusNum":3,"type":1,"startTime":1458093452000,"endTime":2066026582000,"expireDay":6913,"expire":false,"vipLvId":3,"vipName":"PATRIARCH"},"thumbImage":"http://188.188.1.179:20084/chess/chess_img/head/100014/1510301726222933_120x120.jpg","pwdOK":true,"phoneOK":true}},"success":true,"message":"操作成功","status":200}
             */
            
            fi->blackType = loginPacket->recvVal["resultMap"]["blackType"].asInt();
            fi->friendType = loginPacket->recvVal["resultMap"]["friendType"].asInt();
            if (!loginPacket->recvVal["resultMap"]["friendRmkName"].isNull())
            {
                fi->rmkName = loginPacket->recvVal["resultMap"]["friendRmkName"].asString();
            }
            
            notificationReflash(NULL);
            
            Json::Value &sJson = loginPacket->recvVal["resultMap"]["otherUser"];
            
            int id_s = sJson["id"].asInt();
            
            int index = 0;
            
            if (!sJson["signature"].isNull())
            {
                Text* show = (Text*)signview->getChildByName("tiptext");
                
                char buf[128];
                Utils::parseName(15, buf, sJson["signature"].asCString());
                show->setString(buf);
                signview->setPosition(Vec2(bx, by - dy * index));
                index++;
            }
            else
            {
                signview->removeFromParentAndCleanup(true);
            }
            
            if (!sJson["province"].isNull())
            {
                Text* show = (Text*)address->getChildByName("tiptext");
                show->setString(__String::createWithFormat("%s %s %s",sJson["province"].asCString(),sJson["city"].asCString(),sJson["area"].asCString())->getCString());
                address->setPosition(Vec2(bx, by - dy * index));
                index++;
            }
            else
            {
                address->removeFromParentAndCleanup(true);
            }
            
            if (!sJson["hobby"].isNull())
            {
                Text* show = (Text*)love->getChildByName("tiptext");
                char buf[128];
                Utils::parseName(15, buf, sJson["hobby"].asCString());
                show->setString(buf);
                love->setPosition(Vec2(bx, by - dy * index));
                index++;
            }
            else
            {
                love->removeFromParentAndCleanup(true);
            }

            fangjian->setPosition(Vec2(bx, by - dy * index));
            index++;
            
            battle->setPosition(Vec2(bx,  by - dy * index));
            
        }
        else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets[tag] = NULL;
        delete loginPacket;
        
    },"account/otherUserInfo",json_s.toStyledString());
}