//
//  TribleJoinNotify.cpp
//  ZJH
//
//  Created by Jack on 16/9/27.
//
//

#include "TribleJoinNotify.hpp"
#include "../../Model/MsgModel.h"
#include "../../sqlite3/DataBase.h"
#include "cocostudio/CocoStudio.h"

bool TribleJoinNotify::init(){
    if (!BaseLayer::init()) {
        return false;
    }
    
    size = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(0x14, 0x14, 0x15, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    
    setName("AgainstNotify");
    
    setTitle("俱乐部申请");
    
    
    initView();
    return true;
}

void TribleJoinNotify::initView(){
    listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(size.width, size.height - getHeadH()));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 0));
    listView->setScrollBarEnabled(false);
    listView->setName("list");
    addChild(listView);
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    listView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType)
                               {
                                   if (eventType == ScrollView::EventType::SCROLL_TO_TOP) {
                                       if (hasMore)
                                       {
                                           curPage ++;
                                           getRecordData(false);
                                       }
                                   }
                               });

}

void TribleJoinNotify::initData(Json::Value &json){
    curPage = 1;
    lastTime = 0;
    newTime = 0;
    hasMore = true;
    pageSize = 30;
    lastId = 0;
    _cuid = json["uid"].asInt();
    getRecordData(true);
    CallFunc* func = CallFunc::create([=]
                                      {
                                          listView->jumpToBottom();
                                      });
    
    listView->refreshView();
    
    runAction(Sequence::create(DelayTime::create(0.0),func, NULL));
    
}

void TribleJoinNotify::getRecordData(bool init){
    MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[_cuid];
    std::vector<MsgModel::Chat*> chatList = DataBase::getInstance(ZJHModel::getInstance()->uid)->getChat(_cuid, curPage, pageSize, lastId);
    
    if (fi == NULL)
    {
        log("error fi is NULL id[%d]",_cuid);
        return;
    }
    Vec2 p = listView->getInnerContainerPosition();
    if (chatList.size() < pageSize)hasMore = false;
    for (int i = 0; i < chatList.size() ; i ++)
    {
        MsgModel::Chat* chat = chatList[i];
        if (!chat)continue;
        
        TribleJoinItem* item = TribleJoinItem::create();
        item->initView(chat);
        auto layout = Layout::create();
        layout->setContentSize(item->getContentSize());
        layout->addChild(item,10000);
        listView->insertCustomItem(layout, 0);
        
        fi->chatList.push_back(chat);//for delete this object in the view onExit
        
        if (i == chatList.size() - 1)
        {
            lastId = chat->id;
        }
    }

    if (!init)
    {
        listView->refreshView();
        listView->setInnerContainerPosition(p);
    }
}

bool TribleJoinItem::init(){
    if (!Layout::init()) {
        return false;
    }
    Size size = Director::getInstance()->getWinSize();
    setContentSize(Size(size.width,221));
    
    return true;
}

void TribleJoinItem::initView(MsgModel::Chat* chat){
    this->chat = chat;
    Json::Value jsContent = Utils::ParseJsonStr(chat->content);
//    {\"applyTime\":1493211816332,\"clubName\":\"逗地主俱乐部\",\"content\":\"你好\",\"userId\":134226,\"userImage\":\"ess_img/head/134226/17042410062414990.jpg\",\"userName\":\"lemon\"}
    
    auto avatar = Utils::createCircleImage(jsContent["userImage"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(111,111));
    avatar->setAnchorPoint(Vec2(0,0.5));
    avatar->setPosition(40, getContentSize().height/2);
    addChild(avatar);
    
    char buf[128];
    Utils::parseName(10, buf, jsContent["userName"].asString().c_str());
    Text* textName = Text::create(buf, "AmericanTypewriter", 42);
    textName->setColor(Color3B(0xe6,0xe6, 0xe6));
    textName->setAnchorPoint(Vec2(0, 0));
    textName->setPosition(Vec2(181, 120));
    addChild(textName);

    Utils::parseName(10, buf, jsContent["content"].asString().c_str());
    Text* textContent = Text::create(buf, "AmericanTypewriter", 32);
    textContent->setColor(Color3B(0x66,0x66, 0x66));
    textContent->setAnchorPoint(Vec2(0, 0));
    textContent->setPosition(Vec2(181, 78));
    addChild(textContent);

    string timtStr = Utils::getMessageTimeFormat(chat->createTime);
    Text* textTime = Text::create(timtStr.c_str(), "AmericanTypewriter", 32);
    textTime->setColor(Color3B(0x66,0x66, 0x66));
    textTime->setAnchorPoint(Vec2(1, 0));
    textTime->setPosition(Vec2(getContentSize().width - 40, 135));
    addChild(textTime);

    LayerColor* topBg = LayerColor::create(Color4B(0x30,0x30,0x31, 255));
    topBg->setContentSize(Size(getContentSize().width-40,2));
    topBg->setAnchorPoint(Vec2(0,1));
    topBg->setPosition(Vec2(40,getContentSize().height-2));
    addChild(topBg);

    
    int result = jsContent["result"].asInt();
    if(result == 0){
        Button* btnOk = Button::create("Msg/trible_btn.png","Msg/trible_btn_check.png");
        btnOk->setContentSize(Size(168,69));
        btnOk->setAnchorPoint(Vec2(1, 0));
        btnOk->ignoreContentAdaptWithSize(false);
        btnOk->setTitleText("同意");
        btnOk->setTitleColor(Color3B(0xe6,0xe6,0xe6));
        btnOk->setTitleFontSize(30);
        btnOk->setScale9Enabled(true);
        btnOk->setPosition(Vec2(getContentSize().width - 220,35));
        btnOk->addClickEventListener([=](Ref* ref){
            sendHttpRequest(jsContent["clubId"].asInt(),1,jsContent["userId"].asInt());
        });
        btnOk->setTag(1);
        addChild(btnOk);
        
        Button* btnCancel = Button::create("Msg/trible_btn.png","Msg/trible_btn_check.png");
        btnCancel->setContentSize(Size(168,69));
        btnCancel->setAnchorPoint(Vec2(1, 0));
        btnCancel->ignoreContentAdaptWithSize(false);
        btnCancel->setTitleText("拒绝");
        btnCancel->setTitleColor(Color3B(0xe6,0xe6,0xe6));
        btnCancel->setTitleFontSize(30);
        btnCancel->setScale9Enabled(true);
        btnCancel->setPosition(Vec2(getContentSize().width - 41,35));
        btnCancel->addClickEventListener([=](Ref* ref){
            sendHttpRequest(jsContent["clubId"].asInt(),-1,jsContent["userId"].asInt());
        });
        btnCancel->setTag(2);
        addChild(btnCancel);
        
        }else{
        string resultStr = (result == 1 ? "已同意" : "已拒绝");
        Button* btnCancel = Button::create("Msg/trible_result.png","Msg/trible_result.png");
        btnCancel->setContentSize(Size(168,69));
        btnCancel->setAnchorPoint(Vec2(1, 0));
        btnCancel->ignoreContentAdaptWithSize(false);
        btnCancel->setTitleText(resultStr);
        btnCancel->setTitleColor(Color3B(0xe6,0xe6,0xe6));
        btnCancel->setTitleFontSize(30);
        btnCancel->setScale9Enabled(true);
        btnCancel->setPosition(Vec2(getContentSize().width - 41,35));
        addChild(btnCancel);
    }

}

void TribleJoinItem::sendHttpRequest(int clubId,int action,int userId){
    stringstream stream;
    stream<<clubId;
    tag = "trible" + stream.str();
    //发送申请处理请求
    this->schedule(schedule_selector(TribleJoinItem::getData), 0.0f);
    Json::Value json;
    json["clubId"] = clubId;
    json["applyStatus"] = action == 1 ? 2 : 3;
    json["requesterId"] = userId;

    this->action = action;
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"club/clubjoinRequest/check",json.toStyledString(),tag);
}

void TribleJoinItem::getData(float dt){
    CCHttpPacket* triblePacket = CCHttpAgent::getInstance()->packets[tag];
    if (triblePacket && triblePacket->status != 0)
    {
        unschedule(schedule_selector(TribleJoinItem::getData));
        
        if (triblePacket->status != 3)
        {
            return;
        }
        
        triblePacket->status = 0;
        if (triblePacket->resultIsOK())
        {
            Json::Value jsContent = Utils::ParseJsonStr(chat->content);
            jsContent["result"] = action;//0 未处理  1同意   -1拒绝
            chat->seqNo = -1;
            chat->content = jsContent.toStyledString();
            MsgModel::getInstance()->updateChat(chat);
            updateUI(action);
        }
        else
        {
            PlatformHelper::showToast(triblePacket->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets[tag] = NULL;
        delete triblePacket;
    }
}

void TribleJoinItem::updateUI(int result){
    string resultStr = (result == 1 ? "已同意" : "已拒绝");
    getChildByTag(1)->removeFromParentAndCleanup(true);
    getChildByTag(2)->removeFromParentAndCleanup(true);
    Button* btnCancel = Button::create("Msg/trible_result.png","Msg/trible_result.png");
    btnCancel->setContentSize(Size(168,69));
    btnCancel->setAnchorPoint(Vec2(1, 0));
    btnCancel->ignoreContentAdaptWithSize(false);
    btnCancel->setTitleText(resultStr);
    btnCancel->setTitleColor(Color3B(0xe6,0xe6,0xe6));
    btnCancel->setTitleFontSize(30);
    btnCancel->setScale9Enabled(true);
    btnCancel->setPosition(Vec2(getContentSize().width - 41,35));
    addChild(btnCancel);

}

TribleJoinItem::~TribleJoinItem(){
    unschedule(schedule_selector(TribleJoinItem::getData));
}

