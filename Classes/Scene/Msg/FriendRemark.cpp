//
//  FriendRemark.cpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/29.
//
//

#include "FriendRemark.h"
#include "../../Model/MsgModel.h"

bool FriendRemark::init(){
    if(!Layout::init()){
        return false;
    }
    Size size = Director::getInstance()->getWinSize();
    setContentSize(size);
    setTouchEnabled(true);
    
    laymain = Layout::create();
    laymain->setContentSize(Size(600,400));
    laymain->setAnchorPoint(Vec2(0.5,0.5));
    laymain->setPosition(Vec2(size.width/2,size.height/2));
    
    LayerColor* bg_color = LayerColor::create(Color4B(29,29,31, 255));
    bg_color->setContentSize(Size(600,400));
    laymain->addChild(bg_color);
    
    this->addChild(laymain);
    initView();
    return true;
}

void FriendRemark::initView(){
    Text* gameName = Text::create("设置备注名", "AmericanTypewriter", 45);
    gameName->setAnchorPoint(Vec2(0.5, 0.5));
    gameName->setPosition(Vec2(300,laymain->getContentSize().height-50));
    gameName->setColor(Color3B::WHITE);
    laymain->addChild(gameName);
    
    Button* btn_close = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    btn_close->setAnchorPoint(Vec2(0.5,0.5));
    btn_close->setPosition(Vec2(550,350));
    btn_close->setContentSize(Size(70,70));
    btn_close->ignoreContentAdaptWithSize(false);
    btn_close->addClickEventListener([=](Ref* ref){
        this->removeFromParentAndCleanup(true);
    });
    laymain->addChild(btn_close);
    
    
    Size editBoxSize = Size(500, 100);
    EditBox* _editName = EditBox::create(editBoxSize, Scale9Sprite::create("Chat/bar_txt.png"));
    _editName->setAnchorPoint(Vec2(0.5,0.5));
    _editName->setPosition(Vec2(300,200));
    _editName->setFontSize(40);
    _editName->setFontColor(Color3B::RED);
    _editName->setPlaceHolder("输入备注");
    _editName->setPlaceholderFontSize(40);
    _editName->setPlaceholderFontColor(Color3B::BLACK);
    _editName->setMaxLength(20);
    _editName->setInputMode(EditBox::InputMode::SINGLE_LINE);
    _editName->setReturnType(EditBox::KeyboardReturnType::DONE);
    laymain->addChild(_editName,1200);
    
    
    Button* button = Button::create("Hall/button/btn_yellow_small.png","Hall/button/btn_yellow_big.png","Hall/button/btn_yellow_small_check.png");
    button->setContentSize(Size(300,80));
    button->setAnchorPoint(Vec2(0.5, 0.5));
    button->setScale9Enabled(true);
    button->setTitleText("确  认");
    button->setTitleFontSize(50);
    button->setPosition(Vec2(300,80));
    button->addClickEventListener([=](Ref*){
        sendMesFixNick(_editName->getText());
    });
    laymain->addChild(button);
    
}

void FriendRemark::sendMesFixNick(string nick){
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
            MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[_cuid];
            if (nick == "")
            {
                fi->nickName = loginPacket->recvVal["resultMap"]["freindNickName"].asCString();
            }
            else
            {
                fi->nickName = nick;
            }
            
            fi->rmkName = nick;
            MsgModel::getInstance()->addFriend(fi);
            
            PlatformHelper::showToast("修改备注成功");
            __NotificationCenter::getInstance()->postNotification("ChatFriendsInfo_reflash");
            __NotificationCenter::getInstance()->postNotification("MyFriend_reflash");
            __NotificationCenter::getInstance()->postNotification("Msg_reflash");
            __NotificationCenter::getInstance()->postNotification("ChatFriends_namereflash");
            this->removeFromParentAndCleanup(true);
        }
        else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets[tag] = NULL;
        delete loginPacket;
        
    },"friend/addRemark",json_s.toStyledString());
}

void FriendRemark::onExit(){
    Layout::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}