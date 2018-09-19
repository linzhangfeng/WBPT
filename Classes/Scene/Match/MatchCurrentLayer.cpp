//
//  MatchCurrentLayer.cpp
//  ZJH
//
//  Created by apple on 16/8/17.
//
//

#include "MatchCurrentLayer.hpp"
#include "MatchItem.hpp"
#include "../Load/LoadTable.h"

bool MatchCurrentLayer::init()
{
    if (!Widget::init())
    {
        return false;
    }
    Size winSize = Director::getInstance()->getWinSize();
    this->setContentSize(Size(winSize.width, winSize.height - 160 - 150));
    
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"duiju/currentList", "", "currentList");
    this->schedule(schedule_selector(MatchCurrentLayer::postCurrent), 0.0f);
    
    loading = Loading::startLoading(this, winSize / 2);
    loading->maskSwllowTouch();
    
    return true;
}

void MatchCurrentLayer::postCurrent(float dt)
{
    //当前对局中的列表
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["currentList"];
    if (packet && packet->status != 0)
    {
        loading->removeFromParent();
        unschedule(schedule_selector(MatchCurrentLayer::postCurrent));
        if (packet->status != 3)
        {
            return;
        }
        packet->status = 0;
        
        if (packet->resultIsOK())
        {
            Json::Value data = packet->recvVal["resultMap"];
            showView(data);
        }
        else
        {
            PlatformHelper::showToast(packet->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets["currentList"] = NULL;
        delete packet;
    }
}

void MatchCurrentLayer::showView(Json::Value json)
{
    this->removeAllChildren();
    Size winSize = Director::getInstance()->getWinSize();
    float H = this->getContentSize().height;
    
    Json::Value listData = json["currentList"];
    jsonlist = listData;
    int nums = listData.size();
    if (nums <= 0)
    {
        Text *text = Text::create("还没有进行中的房间哦", "", 36);
        text->setPosition(Vec2(winSize.width/2, H/2 + 100));
        text->setTextColor(Color4B::GRAY);
        this->addChild(text);
        
        Text *textCreate = Text::create("去创建一个房间邀请好友一起打牌吧！", "", 36);
        textCreate->setPosition(Vec2(winSize.width/2, H/2));
        textCreate->setTextColor(Color4B::GRAY);
        this->addChild(textCreate);
        
        return;
    }
    

    ListView*listView = ListView::create();
    listView->setDirection(ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(true);
    listView->setContentSize(Size(winSize.width, H));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 0));
    listView->setItemsMargin(25);
    listView->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(listView);
    
    for (int i = 0; i < nums; i++)
    {
        Layout *layoutItem = Layout::create();
        Size itemSize = Size(winSize.width, 420);
        layoutItem->setContentSize(itemSize);
        listView->addChild(layoutItem);
        
        MatchItem *item = MatchItem::createWithData(listData[i]);
        item->setTag(i);
        item->setTouchEnabled(true);
        item->addClickEventListener(CC_CALLBACK_1(MatchCurrentLayer::currentItemJoinRoom, this));
        layoutItem->addChild(item);
    }
}

void MatchCurrentLayer::currentItemJoinRoom(Ref *pSender)
{
    MatchItem *layoutItem = (MatchItem*)pSender;
    int index = layoutItem->getTag();
    Json::Value json;
    json["code"] = jsonlist[index]["code"];
    json["userJoin"] = 0;
    
    //加入对局的请求
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"];
            log("进入房间：＝＝＝ [%s]",data.toStyledString().c_str());
            
            ZJHModel::getInstance()->roomData = data;
            
            ZJHModel::getInstance()->gameHelpUrl = CCHttpAgent::getInstance()->url+data["gameRoom"]["gameHelpUrl"].asString();
            Director::getInstance()->replaceScene(LoadTable::createScene());
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    },"duiju/view",json.toStyledString(),"duijuCode");
}
