//
//  OtherGameType.cpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/20.
//
//

#include "OtherGameType.hpp"
#include "HallRecommend.h"

bool OtherGameType::init()
{
    if (Layer::init() == false) {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
    setContentSize(Size(winSize.width, winSize.height - 410));
    
    m_pLoading = Loading::startLoading(this, winSize / 2);
    m_pLoading->setVisible(false);
    
    LayerColor *bg = LayerColor::create(Color4B(20, 20, 20, 255), winSize.width, winSize.height - 410);
    bg->setPosition(Vec2(0, 0));
    this->addChild(bg, -10);
    
    m_pNoData = Text::create("还没有数据", "AmericanTypewriter", 60);
    m_pNoData->setPosition(winSize / 2);
    m_pNoData->setVisible(false);
    m_pNoData->setColor(Color3B(182, 182, 182));
    this->addChild(m_pNoData, 10);
    
    initList();
    
    return true;
}

void OtherGameType::initList()
{
     Size winSize = Director::getInstance()->getWinSize();
    
    listView = ListView::create();
    listView->setDirection(ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(winSize.width, winSize.height - 410));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 0));
    listView->setItemsMargin(40);
    listView->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(listView);

}

void OtherGameType::initSwitchType(int type){
    Json::Value json;
    index = 1;
     m_pNoData->setVisible(false);
    if(listView->getChildrenCount() > 0){
        listView->removeAllChildren();
    }
    json["pageIndex"] = index;
    json["pageSize"] = 10;
    if (type != 0) {
        json["classifyId"] = type;
    }
    m_pLoading->setVisible(true);
    char buff[64];
    snprintf(buff, sizeof(buff), "hotlist_%d", type);
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
                                                         m_pLoading->setVisible(false);
                                                         if (packet->status == 3) {
                                                             packet->status = 0;
                                                             if (packet->resultIsOK())
                                                             {
                                                                 _value = packet->recvVal["resultMap"]["hotList"];
                                                                 initData(type);
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
                                             "game/hall/hotList", json.toStyledString(), buff);
}


void OtherGameType::initData(int typeId)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    Json::Value &listData = _value;
    int nums = listData["result"].size();
    m_pLoading->setVisible(false);
    if(nums == 0 && listView->getChildrenCount() <= 0) {
        m_pNoData->setVisible(true);
    }else{
        Size rowSize = Size(winSize.width, 397);
        for (int i = 0; i < nums; i += 2) {//每排两个
            Layout *layout = Layout::create();
            layout->setContentSize(rowSize);
            
            int gameId = listData["result"][i]["id"].asInt();
            HotListItem *item = HotListItem::createWithData(listData["result"][i]);
            item->setPosition(Vec2(rowSize.width * 0.25, rowSize.height / 2));
            item->getImageView()->addClickEventListener([gameId](Ref *sender)
                                                        {
//                                                            char msg[128];
//                                                            snprintf(msg, sizeof(msg), "click item %d", gameId);
                                                            PlatformHelper::showToast("暂未开放,敬请期待");
                                                        });
            layout->addChild(item);
            
            if (i + 1 < nums) {
                gameId = listData["result"][i + 1]["id"].asInt();
                item = HotListItem::createWithData(listData["result"][i + 1]);
                item->setPosition(Vec2(rowSize.width * 0.75, rowSize.height / 2));
                item->getImageView()->addClickEventListener([gameId](Ref *sender)
                                                            {
//                                                                char msg[128];
//                                                                snprintf(msg, sizeof(msg), "click item %d", gameId);
                                                                PlatformHelper::showToast("暂未开放,敬请期待");
                                                            });
                layout->addChild(item);
            }
            
            listView->addChild(layout);
        }
        
        listView->refreshView();
        
        if (listData["hasMore"].asBool())
        {
            listView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType)
                                       {
                                           if (eventType == ScrollView::EventType::SCROLL_TO_BOTTOM) {
                                               listView->ScrollView::addEventListener(NULL);
                                               PlatformHelper::showToast("到达底部");
                                               
                                               index++;
                                               Json::Value json;
                                               json["pageIndex"] = index;
                                               json["pageSize"] = 10;
                                              json["gameTypeId"] = typeId;
                                               
                                               char buff[64];
                                               snprintf(buff, sizeof(buff), "hotlist_%d", typeId);
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
                                                                                                    m_pLoading->setVisible(false);
                                                                                                    if (packet->status == 3) {
                                                                                                        packet->status = 0;
                                                                                                        if (packet->resultIsOK())
                                                                                                        {
                                                                                                            _value = packet->recvVal["resultMap"]["hotList"];
                                                                                                            initData(typeId);
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
                                                                                        "game/hotList", json.toStyledString(), buff);
                                           }
                                       });
        }
    }
}