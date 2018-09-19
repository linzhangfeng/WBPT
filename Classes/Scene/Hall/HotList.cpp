//
//  HotList.cpp
//  ZJH
//
//  Created by hyhy on 16/7/7.
//
//

#include "HotList.h"
#include "HallRecommend.h"
#include "../Loading/Loading.h"

const int TAG_ALL_GAME = 0;

bool HotList::init()
{
    if (!BaseLayer::init()) {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(255, 255, 255, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    
    setTitle("热门房间");
    
    
//    
//    this->schedule(schedule_selector(HotList::getData), 0.0f);
//    
//    Json::Value json;
//    json["pageIndex"] = 1;
//    json["pageSize"] = 10;
//    CCHttpAgent::getInstance()->sendHttpPost(NULL, "game/hotList", json.toStyledString(), "hotList");
    
    m_pLoading = Loading::startLoading(this, winSize / 2);
    m_pLoading->setVisible(false);
    
    m_pNoData = Text::create("还没有数据", "AmericanTypewriter", 60);
    m_pNoData->setPosition(winSize / 2);
    m_pNoData->setVisible(false);
    m_pNoData->setColor(Color3B(182, 182, 182));
    this->addChild(m_pNoData, 10);
    
    m_curType = -1;
    this->initTable();
    this->selectGameType(TAG_ALL_GAME);
    
    return true;
}

void HotList::getData(float dt)
{
//    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["hotList"];
//    if (loginPacket && loginPacket->status != 0)
//    {
//        m_pLoading->removeFromParent();
//        
//        unschedule(schedule_selector(HotList::getData));
//        
//        if (loginPacket->status != 3)
//        {
//            return;
//        }
//        
//        loginPacket->status = 0;
//        
//        if (loginPacket->resultIsOK())
//        {
//            m_jsonData = loginPacket->recvVal["resultMap"];
//            initList();
//        }
//        else
//        {
//            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
//        }
//        
//        CCHttpAgent::getInstance()->packets["hotList"] = NULL;
//        delete loginPacket;
//    }
}

void HotList::initTable()
{
    Json::Value &typeInfo = ZJHModel::getInstance()->gameConfigJson["list"]["gameTypeList"];
    int num = typeInfo.size();
//    num += 1;   //多出来的那个是全部游戏
    Size winSize = Director::getInstance()->getWinSize();
    Size imageSize = Size(100, 100);
    
    ListView *typeList = ListView::create();
    typeList->setDirection(ScrollView::Direction::HORIZONTAL);
    typeList->setBounceEnabled(false);
    typeList->setContentSize(Size(winSize.width, 200));
    typeList->setAnchorPoint(Vec2(0, 1));
    typeList->setPosition(Vec2(0, winSize.height - 150));
    typeList->setScrollBarEnabled(true);
    typeList->setItemsMargin(50);
    typeList->setGravity(ListView::Gravity::CENTER_VERTICAL);
    addChild(typeList);
    
    Widget *leftMargin = Widget::create();
    leftMargin->setContentSize(Size(35, 50));
    typeList->addChild(leftMargin);
    
    ImageView *allTypeImage = ImageView::create();
    allTypeImage->loadTexture("res/Head/title_btn.png");
    allTypeImage->setContentSize(imageSize);
    allTypeImage->setTouchEnabled(true);
    allTypeImage->ignoreContentAdaptWithSize(false);
    allTypeImage->setTag(TAG_ALL_GAME);
    allTypeImage->addClickEventListener(CC_CALLBACK_1(HotList::clickImage, this));
    typeList->addChild(allTypeImage);
    
    for (int i = 0; i < num; i++) {
        int gameId = typeInfo[i]["gameId"].asInt();
        std::string logUrl = typeInfo[i]["logUrl"].asString();
        
        ImageView *typeImage = ImageView::create();
//        typeImage->loadTexture("res/Head/title_btn.png");
        typeImage->setContentSize(imageSize);
        typeImage->setTouchEnabled(true);
        typeImage->ignoreContentAdaptWithSize(false);
        typeImage->setTag(gameId);
        typeImage->addClickEventListener(CC_CALLBACK_1(HotList::clickImage, this));
        typeList->addChild(typeImage);
        
        if (Utils::hasImageFile(logUrl)) {
            typeImage->loadTexture(Utils::getImageSavePath(logUrl));
        } else {
            CCHttpAgent::getInstance()->sendHttpImage(logUrl, Utils::getImageSavePath(logUrl), Utils::getImageSavePath(logUrl), [typeImage, logUrl](std::string tag)
                                                      {
                                                          CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                          if (packet) {
                                                              CCHttpAgent::getInstance()->packets.erase(tag);
                                                              delete packet;
                                                          }
                                                          
                                                          if (typeImage == NULL || typeImage->getReferenceCount() <= 0 || typeImage->getReferenceCount() > 10) return;
                                                          
                                                          if(Utils::hasImageFile(logUrl))
                                                          {
                                                              typeImage->loadTexture(Utils::getImageSavePath(logUrl));
                                                          }
                                                      });
        }
    }
    
    Widget *rightMargin = Widget::create();
    rightMargin->setContentSize(Size(35, 50));
    typeList->addChild(rightMargin);
    
    m_pSelectedFlag = Sprite::create("res/Head/title_btn.png");
    m_pSelectedFlag->setPosition(Vec2(130, 10));
    typeList->addChild(m_pSelectedFlag);

}

void HotList::selectGameType(int type)
{
    if (m_curType == type) {
        return;
    }
    
    m_pLoading->setVisible(false);
    m_pNoData->setVisible(false);
    
    if (listViewMap.find(m_curType) != listViewMap.end()) {
        listViewMap[m_curType]->setVisible(false);
    }
    m_curType = type;
    
    if (listViewMap.find(type) != listViewMap.end()) {
        listViewMap[type]->setVisible(true);
    } else {//没初始化过
        m_pLoading->setVisible(true);
        Json::Value json;
        json["pageIndex"] = 1;
        json["pageSize"] = 10;
        if (type != 0) {
            json["gameTypeId"] = type;
        }
        
        char buff[64];
        snprintf(buff, sizeof(buff), "hotlist_%d", type);
        CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                                 {
                                                     if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
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
                                                                     listViewData[type] = packet->recvVal["resultMap"]["hotList"];
                                                                     this->initList(type);
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
}

void HotList::initList(int type)
{
    Size winSize = Director::getInstance()->getWinSize();
    Json::Value &listData = listViewData[type];
    int nums = listData["result"].size();
    char buff[128];
    
    ListView *listView = NULL;
    if (listViewMap.find(type) != listViewMap.end()) {
        listView = listViewMap[type];
    } else {
        if (nums == 0) {
            m_pNoData->setVisible(true);
            return;
        }
        
        snprintf(buff, sizeof(buff), "listView_%d", type);
        listView = ListView::create();
        listView->setDirection(ScrollView::Direction::VERTICAL);
        listView->setBounceEnabled(false);
        listView->setContentSize(Size(winSize.width, winSize.height - 450));
        listView->setAnchorPoint(Vec2(0, 0));
        listView->setPosition(Vec2(0, 0));
        listView->setItemsMargin(50);
        listView->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
        listView->setName(buff);
        this->addChild(listView);
        listViewMap.insert(pair<int, ListView *>(type, listView));
    }
    
    Size rowSize = Size(winSize.width, 430);
    for (int i = 0; i < nums; i += 2) {//每排两个
        Layout *layout = Layout::create();
        layout->setContentSize(rowSize);
        
        int gameId = listData["result"][i]["id"].asInt();
        HotListItem *item = HotListItem::createWithData(listData["result"][i]);
        item->setPosition(Vec2(rowSize.width * 0.25, rowSize.height / 2));
        item->getImageView()->addClickEventListener([gameId](Ref *sender)
                                                    {
//                                                        char msg[128];
//                                                        snprintf(msg, sizeof(msg), "click item %d", gameId);
                                                        PlatformHelper::showToast("暂未开放,敬请期待");
                                                    });
        layout->addChild(item);
        
        if (i + 1 < nums) {
            gameId = listData["result"][i + 1]["id"].asInt();
            item = HotListItem::createWithData(listData["result"][i + 1]);
            item->setPosition(Vec2(rowSize.width * 0.75, rowSize.height / 2));
            item->getImageView()->addClickEventListener([gameId](Ref *sender)
                                                        {
//                                                            char msg[128];
//                                                            snprintf(msg, sizeof(msg), "click item %d", gameId);
                                                            PlatformHelper::showToast("暂未开放,敬请期待");
                                                        });
            layout->addChild(item);
        }
        
        listView->addChild(layout);
    }
    
    listView->refreshView();
    
    listView->setVisible(m_curType == type);//生成表的时候有可能已经切换到其他类型了
    
    if (listData["hasMore"].asBool())
    {
        listView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType)
                                   {
                                       if (eventType == ScrollView::EventType::SCROLL_TO_BOTTOM) {
                                           listView->ScrollView::addEventListener(NULL);
                                           PlatformHelper::showToast("到达底部");
                                           
                                           if (listViewPage.find(type) != listViewPage.end()) {
                                               listViewPage[type] = listViewPage[type] + 1;
                                           } else {
                                               listViewPage.insert(pair<int, int>(type, 1 + 1));
                                           }
                                           
                                           Json::Value json;
                                           json["pageIndex"] = listViewPage[type];
                                           json["pageSize"] = 10;
                                           if (type != 0) {
                                               json["gameTypeId"] = type;
                                           }
                                           
                                           char buff[64];
                                           snprintf(buff, sizeof(buff), "hotlist_%d", type);
                                           CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                                                                    {
                                                                                        if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
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
                                                                                                        listViewData[type] = packet->recvVal["resultMap"]["hotList"];
                                                                                                        this->initList(type);
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

void HotList::clickImage(cocos2d::Ref *sender)
{
    ImageView *typeImage = dynamic_cast<ImageView *>(sender);
    if (m_pSelectedFlag) {
        m_pSelectedFlag->setPositionX(typeImage->getPositionX());
    }
    this->selectGameType(typeImage->getTag());
}