//
//  FoundFriends.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "FoundFriends.hpp"
#include "ZJH.h"

FoundFriends::FoundFriends()
{
    listView = NULL;
}

bool FoundFriends::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    winSize = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    setReturn_txt("取消", [=](){
        this->disappear();
    });
    
    Size editBoxSize = Size(winSize.width - 186 - 45, 100);
    _editName = EditBox::create(editBoxSize, Scale9Sprite::create("Chat/bar_txt.png"));
    _editName->setAnchorPoint(Vec2(0,0));
    _editName->setPosition(Vec2(45,(winSize.height - getHeadH()) + (getHeadH()-editBoxSize.height) / 2));
    _editName->setFontSize(50);
    _editName->setName("_editName");
    _editName->setFontColor(Color3B(139,139,139));
    _editName->setPlaceHolder("用户ID／用户昵称");
    _editName->setPlaceholderFontSize(40);
    _editName->setPlaceholderFontColor(Color3B(139,139,139));
    _editName->setMaxLength(20);
//    _editName->setInputFlag(cocos2d::ui::EditBox::InputFlag::)
    _editName->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    _editName->setReturnType(EditBox::KeyboardReturnType::DONE);
    _editName->setDelegate(this);
    _editName->requestFocus();
    this->addChild(_editName,1200);
    
    pageNo = 1;
    
    searchflag = false;
    isconneting = false;
    
    return true;
}


void FoundFriends::searchGameFriend(){
    
    Json::Value json;
    json["keyword"] = friendId;
    json["pageNo"] = pageNo;
    json["pageSize"] = 20;
    json["findType"] = 0;
    
    char buff[128];
    snprintf(buff, sizeof(buff), "searchlist_%s", friendId.c_str());
    searchID = buff;
    
    isconneting = true;
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
                                                 if (searchID != tag)
                                                 {
                                                     return;
                                                 }
                                                 if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
                                                 CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                 if (packet) {
                                                     if (getReferenceCount() == 0 || getReferenceCount() > 10)
                                                     {
                                                         //nothing
                                                     }
                                                     else
                                                     {
                                                         if (packet->status == 3) {
                                                             packet->status = 0;
                                                             if (packet->resultIsOK())
                                                             {
                                                                 listData = packet->recvVal["resultMap"];
                                                                 if(searchflag)
                                                                 {
                                                                     initList(true);
                                                                }
                                                             }
                                                             else
                                                             {
                                                                 PlatformHelper::showToast(packet->recvVal["message"].asCString());
                                                             }
                                                             
                                                         }
                                                         //
                                                     }
                                                     
                                                     //删除packet
                                                     CCHttpAgent::getInstance()->packets.erase(tag);
                                                     delete packet;
                                                 }
                                             },
                                             "account/findFriends", json.toStyledString(), buff);
}


void FoundFriends::initList(bool init)
{
    Size size = Director::getInstance()->getWinSize();
    
    if (init)
    {
        if(listView)listView->removeFromParentAndCleanup(true);
        listView = NULL;
    }
    
    removeChildByName("tip");
    
    if (listData["list"].size() == 0)
    {
        if (init)
        {
            Text* text = Text::create("没有找到玩家", "AmericanTypewriter", 35);
            text->setColor(Color3B(150, 150, 150));
            text->setPosition(Vec2(size.width / 2,size.height / 2));
            text->setName("tip");
            addChild(text);
        }

        return;
    }
    
    if(listView == NULL)
    {
        listView = ListView::create();
        listView->setDirection(ui::ScrollView::Direction::VERTICAL);
        listView->setBounceEnabled(false);
        listView->setScrollBarEnabled(false);
        listView->setContentSize(Size(size.width, size.height - 150));
        listView->setAnchorPoint(Vec2(0, 0));
        listView->setPosition(Vec2(0, 0));
        listView->setItemsMargin(5);
        addChild(listView);
        
        listView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType)
                                   {
                                       if (eventType == ScrollView::EventType::SCROLL_TO_BOTTOM) {
                                           pageNo++;
                                           
                                           Json::Value json;
                                           json["keyword"] = friendId;
                                           json["pageNo"] = pageNo;
                                           json["pageSize"] = 20;
                                           json["findType"] = 0;
                                           
                                           char buff[64];
                                           snprintf(buff, sizeof(buff), "searchList_%s", friendId.c_str());
                                           searchID = buff;
                                           
                                           CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                                                                    {
                                                                                        if (searchID != tag)
                                                                                        {
                                                                                            return;
                                                                                        }
                                                                                        if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
                                                                                        CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                                                        if (packet) {
                                                                                            if (getReferenceCount() == 0 || getReferenceCount() > 10)
                                                                                            {
                                                                                                //nothing
                                                                                            }
                                                                                            else
                                                                                            {
                                                                                                if (packet->status == 3) {
                                                                                                    packet->status = 0;
                                                                                                    if (packet->resultIsOK())
                                                                                                    {
                                                                                                        
                                                                        
                                                                                                        listData = packet->recvVal["resultMap"];
                                                                                                        addDataInList(listData);
                                                                                                    }
                                                                                                    else
                                                                                                    {
                                                                                                        PlatformHelper::showToast(packet->recvVal["message"].asCString());
                                                                                                    }
                                                                                                    
                                                                                                }
                                                                                                //
                                                                                            }
                                                                                            
                                                                                            //删除packet
                                                                                            CCHttpAgent::getInstance()->packets.erase(tag);
                                                                                            delete packet;
                                                                                        }
                                                                                    },
                                                                                    "account/findFriends", json.toStyledString(), buff);
                                           
                                       }
                                   });
        
        listView->setTouchEnabled(true);
    }
    
    addDataInList(listData);
}

void FoundFriends::addDataInList(Json::Value json)
{
    Json::Value listData = json["list"];
    int nums = listData.size();
    
    for (int i = 0; i < nums; i++)
    {
        FoundFriends_Item* item = FoundFriends_Item::create();
        item->initData(listData[i]);
        
        auto layout = Layout::create();
        //layout->setTouchEnabled(true);
        layout->setContentSize(item->getContentSize());
        layout->addChild(item);
        listView->pushBackCustomItem(layout);
    }
    if(nums > 0)listView->refreshView();
}

void FoundFriends::editBoxEditingDidBegin(EditBox* editBox)
{
    log("editBox %p DidBegin.", editBox);
}

void FoundFriends::editBoxEditingDidEnd(EditBox* editBox)
{
    log("editBox %p DidEnd.", editBox);
}

int count_space(string str_in){
    int count = 0;
    for(int i = 0; i < str_in.length(); i++){
        if(str_in[i] == ' '){
            count++;
        }
    }
    return count;
}

void FoundFriends::editBoxTextChanged(EditBox* editBox, const std::string &text)
{
    if(listView != NULL && listView->getChildrenCount() > 0){
        listView->removeAllChildren();
        pageNo = 1;
    }

    friendId = text;
    if(friendId!= ""){
        searchflag = true;
        CallFunc* func1 = CallFunc::create([=]
                                           {
                                               searchGameFriend();
                                           });
        
        runAction(Sequence::create(DelayTime::create(0.05),func1, NULL));
        
    }else{
        searchflag = false;
        if(listView != NULL && listView->getChildrenCount() > 0){
            listView->removeAllChildren();
            pageNo = 1;
        }
    }
    log("editBox %p TextChanged, text: %s", editBox, text.c_str());
}

void FoundFriends::editBoxReturn(EditBox *editBox)
{
    log("editBox %p was returned.", editBox);
}


bool FoundFriends_Item::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    return true;
}

void FoundFriends_Item::initData(Json::Value &json)
{
    Size size = Director::getInstance()->getWinSize();
    
    setContentSize(Size(size.width, 222));
    
    Size itemSize = getContentSize();
    
    LayerColor* bg = LayerColor::create(Color4B(50, 50, 50, 255));
    bg->setContentSize(Size(itemSize.width - 50,2));
    bg->setAnchorPoint(Vec2(0.0f, 0.0f));
    bg->setPosition(50,0);
    addChild(bg, -100);
    
    int uid = json["user"]["id"].asInt();
 
    Sprite* userAvatar = Utils::createCircleAvatar(json["user"]["image"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(144,144));
    userAvatar->setPosition(Vec2(100, itemSize.height / 2));
    this->addChild(userAvatar);
    
    Text* text = Text::create(json["user"]["nickName"].asString(), "AmericanTypewriter", 45);
    text->setColor(Color3B(208, 208, 208));
    text->setAnchorPoint(Vec2(0, 0.5));
    text->setPosition(Vec2(200,itemSize.height * 0.5));
    addChild(text,1000);
    
    Text* add = Text::create("", "AmericanTypewriter", 40);
    
    add->setAnchorPoint(Vec2(0.5, 0.5));
    add->setPosition(Vec2(size.width - 150,itemSize.height * 0.5));
    addChild(add,1000);
    
    if (json["isFriend"].asBool())
    {
        add->setString("已添加");
        add->setColor(Color3B(50, 50, 50));
    }
    else
    {
        auto m = Button::create();
        m->setContentSize(Size(200,200));
        m->ignoreContentAdaptWithSize(false);
        m->setTitleText("添加");
        m->setTitleFontSize(45);
        m->setTitleColor(Color3B(208, 208, 208));
        m->setZoomScale(0.1f);
        m->setAnchorPoint(Vec2(0.5, 0.5));
        m->setPosition(Vec2(size.width - 150,itemSize.height * 0.5));
        m->setPressedActionEnabled(true);
        addChild(m,1000);
        
        auto send_bg = ui::Scale9Sprite::create("Msg/btn_gray_small.png");
        send_bg->setPreferredSize(Size(Size(200, 85)));
        send_bg->setAnchorPoint(Vec2(0.5, 0.5));
        send_bg->setPosition(m->getContentSize() / 2);
        m->addChild(send_bg,-100);
        
        m->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
                                 {
                                     if (type == Widget::TouchEventType::ENDED)
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
                                                 m->setVisible(false);
                                                 add->setString("已添加");
                                                 add->setColor(Color3B(50, 50, 50));
                                                 __NotificationCenter::getInstance()->postNotification("MyFriend_reflash");
                                                 PlatformHelper::showToast("添加成功");
                                             }
                                             else
                                             {
                                                 PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                                             }
                                             
                                             CCHttpAgent::getInstance()->packets[tag] = NULL;
                                             delete loginPacket;
                                             
                                         },"friend/addFriend",json_s.toStyledString(),"foundFriend_addFriend");
                                     }
                                 });
    }
}