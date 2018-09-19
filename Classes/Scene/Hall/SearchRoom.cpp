//
//  SearchRoom.cpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/13.
//
//

#include "SearchRoom.h"
#include "HallRecommend.h"

bool SearchRoom::init()
{
    if (!BaseLayer::init()) {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(20, 20, 22, 255));
    
    setReturn_txt("取消", [=](){
        this->disappear();
    });
    
    countpage = 1;
    
    Size editBoxSize = Size(winSize.width-250, 100);
    EditBox* _editName = EditBox::create(editBoxSize, Scale9Sprite::create("Chat/bar_txt.png"));
    _editName->setAnchorPoint(Vec2(0,0));
    _editName->setPosition(Vec2(50,(winSize.height - getSpriteHeight()) + (getSpriteHeight()-editBoxSize.height)/2));
    _editName->setFontSize(40);
    _editName->setFontColor(Color3B::RED);
    _editName->setPlaceHolder("输入房间id/房主昵称");
    _editName->setPlaceholderFontSize(40);
    _editName->setPlaceholderFontColor(Color3B::BLACK);
    _editName->setMaxLength(20);
    _editName->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    _editName->setReturnType(EditBox::KeyboardReturnType::DONE);
    _editName->setDelegate(this);
    this->addChild(_editName,1200);
    
    
    tipText = Text::create("没有找到房间", "AmericanTypewriter", 35);
    tipText->setColor(Color3B(150, 150, 150));
    tipText->setPosition(Vec2(winSize.width / 2,winSize.height / 2));
    tipText->setName("tip");
    tipText->setVisible(false);
    addChild(tipText);
    
    return true;
}

void SearchRoom::editBoxEditingDidBegin(EditBox* editBox)
{
    log("editBox %p DidBegin.", editBox);
}

void SearchRoom::editBoxEditingDidEnd(EditBox* editBox)
{
    log("editBox %p DidEnd.", editBox);
}

void SearchRoom::editBoxTextChanged(EditBox* editBox, const std::string &text)
{
    if(serListView != NULL && serListView->getChildrenCount() > 0){
        serListView->removeAllChildren();
        countpage = 1;
    }
    roomId = text;
    if(roomId!= ""){
        searchGameRoom();
    }
    log("editBox %p TextChanged, text: %s", editBox, text.c_str());
}

void SearchRoom::editBoxReturn(EditBox *editBox)
{
    log("editBox %p was returned.", editBox);
}


void SearchRoom::searchGameRoom(){

    Json::Value json;
    json["keyword"] = roomId;
    json["pageIndex"] = countpage;
    json["pageSize"] = 10;
   
    char buff[64];
    snprintf(buff, sizeof(buff), "searchlist_%s", roomId.c_str());
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
                                                         if (packet->status == 3) {
                                                             packet->status = 0;
                                                             if (packet->resultIsOK())
                                                             {
                                                                 serValue = packet->recvVal["resultMap"]["searchList"];
                                                                 this->initListView(serValue);
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
                                             "game/searchList", json.toStyledString(), buff);
}

void SearchRoom::initListView(Json::Value mValue){
    Size winSize = Director::getInstance()->getWinSize();
    int nums = mValue["result"].size();
    char buff[128];
    
    if (nums == 0) {
        tipText->setVisible(true);
        return;
    }else{
        tipText->setVisible(false);
    }

    if(serListView == NULL){
        serListView = ListView::create();
        serListView->setDirection(ScrollView::Direction::VERTICAL);
        serListView->setBounceEnabled(false);
        serListView->setContentSize(Size(winSize.width, winSize.height - getSpriteHeight()));
        serListView->setAnchorPoint(Vec2(0, 0));
        serListView->setPosition(Vec2(0, 0));
        serListView->setItemsMargin(50);
        serListView->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
        serListView->setName(buff);
        this->addChild(serListView);

    }
    
    Size rowSize = Size(winSize.width, 430);
    for (int i = 0; i < nums; i += 2) {//每排两个
        Layout *layout = Layout::create();
        layout->setContentSize(rowSize);
        
        int gameId = mValue["result"][i]["id"].asInt();
        HotListItem *item = HotListItem::createWithData(mValue["result"][i]);
        item->setPosition(Vec2(rowSize.width * 0.25, rowSize.height / 2));
        item->getImageView()->addClickEventListener([gameId](Ref *sender)
                                                    {
                                                        char msg[128];
                                                        snprintf(msg, sizeof(msg), "click item %d", gameId);
                                                        PlatformHelper::showToast(msg);
                                                    });
        layout->addChild(item);
        
        if (i + 1 < nums) {
            gameId = mValue["result"][i + 1]["id"].asInt();
            item = HotListItem::createWithData(mValue["result"][i + 1]);
            item->setPosition(Vec2(rowSize.width * 0.75, rowSize.height / 2));
            item->getImageView()->addClickEventListener([gameId](Ref *sender)
                                                        {
                                                            char msg[128];
                                                            snprintf(msg, sizeof(msg), "click item %d", gameId);
                                                            PlatformHelper::showToast(msg);
                                                        });
            layout->addChild(item);
        }
        
        serListView->addChild(layout);
    }
    
    serListView->refreshView();

    
    if (mValue["hasMore"].asBool()) {
        serListView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType)
                                   {
                                       if (eventType == ScrollView::EventType::SCROLL_TO_BOTTOM) {
                                           serListView->ScrollView::addEventListener(NULL);
                                           
                                           CCLOG("hyhy_tese bottom");
                                           PlatformHelper::showToast("到达底部");
                                           
                                           countpage = countpage +1;
                                           
                                           Json::Value json;
                                           json["keyword"] = roomId;
                                           json["pageIndex"] = countpage;
                                           json["pageSize"] = 10;
                             
                                           
                                           char buff[64];
                                           snprintf(buff, sizeof(buff), "searchList_%s", roomId.c_str());
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
                                                                                                if (packet->status == 3) {
                                                                                                    packet->status = 0;
                                                                                                    if (packet->resultIsOK())
                                                                                                    {
                                                                                                        serValue = packet->recvVal["resultMap"]["searchList"];
                                                                                                        this->initListView(serValue);
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
                                                                                    "game/searchList", json.toStyledString(), buff);
                                       }
                                   });
    }

}
