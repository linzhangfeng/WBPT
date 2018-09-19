//
//  ChatFriendsMore_Nick.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "ChatFriendsMore_Nick.hpp"
#include "../../Model/MsgModel.h"
#include "proto.h"

bool ChatFriendsMore_Nick::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    size = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(255, 255, 255, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    
    setTitle("修改备注");
    
    setMore_txt("确定", [=](){
        string remark = _editName->getText();
        //确定提交修改备注
        sendMesFixNick(remark);
    });
    
    return true;

}

void ChatFriendsMore_Nick::myInit(Json::Value &json)
{
    _cuid = json["uid"].asInt();
    
    //MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[uid];
//    Size size = Director::getInstance()->getWinSize();
//    setBackground(Color4B(200, 200, 200, 255));
//    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", NULL);
    //setTitle(fi->nickName);
    
    
    Size editBoxSize = Size(size.width-20, 150);
    _editName = EditBox::create(editBoxSize, Scale9Sprite::create());
    _editName->setAnchorPoint(Vec2(0,0));
    _editName->setPosition(Vec2(10,size.height - getSpriteHeight()- 180));
    _editName->setFontSize(40);
    _editName->setFontColor(Color3B::RED);
    _editName->setPlaceHolder("输入TA的备注");
    _editName->setPlaceholderFontSize(40);
    _editName->setPlaceholderFontColor(Color3B::BLACK);
    _editName->setMaxLength(20);
    _editName->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    _editName->setReturnType(EditBox::KeyboardReturnType::DONE);
    _editName->setDelegate(this);
    this->addChild(_editName,1200);

}


void ChatFriendsMore_Nick::sendMesFixNick(string nick){
    Json::Value json_s;
    json_s["friendId"] = _cuid;
    json_s["remarkName"] = nick;
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
            PlatformHelper::showToast("修改备注成功");
        }
        else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets[tag] = NULL;
        delete loginPacket;
        
    },"friend/addRemark",json_s.toStyledString());
}


void ChatFriendsMore_Nick::editBoxEditingDidBegin(EditBox* editBox)
{
    log("editBox %p DidBegin.", editBox);
}

void ChatFriendsMore_Nick::editBoxEditingDidEnd(EditBox* editBox)
{
    log("editBox %p DidEnd.", editBox);
}

void ChatFriendsMore_Nick::editBoxTextChanged(EditBox* editBox, const std::string &text)
{
    
    log("editBox %p TextChanged, text: %s", editBox, text.c_str());
}

void ChatFriendsMore_Nick::editBoxReturn(EditBox *editBox)
{
    log("editBox %p was returned.", editBox);
}