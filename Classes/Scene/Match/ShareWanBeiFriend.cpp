//
//  ShareWanBeiFriend.cpp
//  ZJH
//
//  Created by apple on 16/8/6.
//
//

#include "ShareWanBeiFriend.hpp"

bool ShareWanBeiFriend::init()
{
    if (!BaseLayer::init()) {
        return false;
    }

    setTitle("分享给好友");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x14, 0x14, 0x15, 255));

    Size winSize = Director::getInstance()->getWinSize();
    
    Layout *inviteBg = Layout::create();
    inviteBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    inviteBg->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    inviteBg->setContentSize(Size(winSize.width, 210));
    this->addChild(inviteBg);
    
    auto line = Layout::create();
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->ignoreAnchorPointForPosition(false);
    line->setAnchorPoint(Vec2(0, 1));
    line->setBackGroundColor(Color3B(0, 0, 0));
    line->setPosition(Vec2(0, 210));
    line->setContentSize(Size(winSize.width, 3));
    inviteBg->addChild(line);
    

    Button *inviteBtn = Button::create("Public/btn_yellow.png","Public/btn_yellow.png");
    inviteBtn->setScale9Enabled(true);
    inviteBtn->setContentSize(Size(960, 115));
    inviteBtn->setPosition(Vec2(winSize.width/2, 105));
    inviteBg->addChild(inviteBtn);
    inviteBtn->setName("invite_button");
    inviteBtn->setTitleText("分享");
    inviteBtn->setTitleFontSize(40);
    inviteBtn->setTitleColor(Color3B(0x65, 0x41, 0x00));
    inviteBtn->setTitleFontName(".SFUIDisplay-Semibold");
    inviteBtn->addClickEventListener(CC_CALLBACK_1(ShareWanBeiFriend::buttonClicked, this));
    
    Json::Value json;
    json["pageNo"] = 1;
    json["pageSize"] = 50;
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        CCHttpAgent::getInstance()->packets.erase(tag);
        CCHttpAgent::getInstance()->callbacks.erase(tag);
        if (loginPacket->status != 3)
        {
            delete loginPacket;
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        loginPacket->status = 0;
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"];
            myInit(data);
            
        }
        
        delete loginPacket;
        
    },"friend/listFriends",json.toStyledString(),"myFriendList");
    
    return true;
}

void ShareWanBeiFriend::myInit(Json::Value &data)
{
    Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = Size(winSize.width, winSize.height - getHeadH() -  210);
    Size itemSize = Size(winSize.width, 180);
    
    ListView *resultList = ListView::create();
    resultList->setDirection(ScrollView::Direction::VERTICAL);
    resultList->setBounceEnabled(false);
    resultList->setContentSize(contentSize);
    resultList->setAnchorPoint(Vec2(0, 0));
    resultList->setPosition(Vec2(0, 210));
    resultList->setItemsMargin(0);
    resultList->setScrollBarEnabled(false);
    this->addChild(resultList);
    
    for (int i = 0; i < data["list"].size(); i++)
    {
        Widget *item = Widget::create();
        item->setContentSize(itemSize);
        item->setColor(Color3B(0x4d, 0x4d, 0x4e));
        item->setTouchEnabled(true);
        item->addClickEventListener(CC_CALLBACK_1(ShareWanBeiFriend::listItemClicked, this));
        item->setTag(data["list"][i]["user"]["id"].asInt());
        
        Sprite *userAvatar = Utils::createCircleAvatar(data["list"][i]["user"]["image"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(120, 120));
        userAvatar->setPosition(Vec2(100, itemSize.height / 2));
        item->addChild(userAvatar);
        
        Text* text = Text::create(data["list"][i]["user"]["nickName"].asString(), ".SFUIDisplay-Semibold", 36);
        text->setColor(Color3B(0xcc, 0xcc, 0xcc));
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setPosition(Vec2(190, itemSize.height/2));
        item->addChild(text);
        
        Sprite *unselected = Sprite::create("Mine/choice_icon.png");
        unselected->setPosition(Vec2(itemSize.width - 65, itemSize.height / 2));
        item->addChild(unselected);
        
        Sprite *selected = Sprite::create("Mine/choice_icon_check.png");
        selected->setPosition(unselected->getPosition());
        selected->setName("selected_sprite");
        item->addChild(selected);
        selected->setVisible(false);
        
        auto line = Layout::create();
        line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        line->setBackGroundColor(Color3B(0x30, 0x30, 0x31));
        line->setContentSize(Size(winSize.width - 40, 3));
        line->setPosition(Vec2(40, 0));
        item->addChild(line);
        
        resultList->addChild(item);
    }
    resultList->refreshView();
}

void ShareWanBeiFriend::buttonClicked(cocos2d::Ref *ref)
{
    Widget *sender = dynamic_cast<Widget *>(ref);
    if (sender) {
        std::string senderName = sender->getName();
        if (senderName == "invite_button")
        {
            if (m_selection.size() == 0)
            {
                PlatformHelper::showToast("请选择好友");
                return;
            }
            std::string userList = "";
            char buff[64];
            for (std::set<int>::const_iterator i = m_selection.begin(); i != m_selection.end(); i++) {
                snprintf(buff, sizeof(buff), "%d,", *i);
                userList = userList + buff;
            }
            // http://188.188.1.111:20006/chess/chess_img/gameroom/101370/1608081249084951.png
            Json::Value jsonPost;
            jsonPost["userIds"] = userList;
            jsonPost["imgUrl"] = jsonDate["imagePath"];
            //当前对局中的列表
            CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                                     {
                                                         CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                         if (packet) {
                                                             if (getReferenceCount() == 0 || getReferenceCount() > 10)
                                                             {
                                                                 //nothing
                                                             }
                                                             else
                                                             {
                                                                 if (packet->status == 3)
                                                                 {
                                                                     packet->status = 0;
                                                                     if (packet->resultIsOK())
                                                                     {
                                                                         PlatformHelper::showToast("分享成功！");
                                                                         this->disappear();
                                                                     }
                                                                     else
                                                                     {
                                                                         PlatformHelper::showToast(packet->recvVal["message"].asCString());
                                                                     }
                                                                     
                                                                 }
                                                             }
                                                             
                                                             //删除packet
                                                             CCHttpAgent::getInstance()->packets.erase(tag);
                                                             delete packet;
                                                         }
                                                     },
                                                     "duiju/integral/shareToUsers",jsonPost.toStyledString(), "integral");
            

        }
    }
}

void ShareWanBeiFriend::getInfoJson(Json::Value json)
{
    jsonDate = json;
}
void ShareWanBeiFriend::listItemClicked(cocos2d::Ref *ref)
{
    Widget *item = dynamic_cast<Widget *>(ref);
    Sprite *selectedSp = dynamic_cast<Sprite *>(item->getChildByName("selected_sprite"));
    selectedSp->setVisible(!selectedSp->isVisible());
    int tag = item->getTag();
    
    if (selectedSp->isVisible()) {
        m_selection.insert(tag);
    } else {
        m_selection.erase(tag);
    }
}
