//
//  RoomAllPlayerLayer.cpp
//  ZJH
//
//  Created by hyhy on 16/7/27.
//
//

#include "RoomAllPlayerLayer.h"
#include "RoomInviteFriendLayer.h"
#include "ZJH.h"

void RoomAllPlayerLayer::show()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(Vec2(0, -winSize.height));
    this->runAction(MoveTo::create(0.17, Vec2(0, 0)));
    this->setVisible(true);
}

void RoomAllPlayerLayer::hide()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->runAction(Sequence::create(MoveTo::create(0.17, Vec2(0, -winSize.height)),
                                     RemoveSelf::create(),
                                     NULL));
}

bool RoomAllPlayerLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomAllPlayerLayer::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void RoomAllPlayerLayer::myInit(int hostUid, int roomId)
{
    Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = Size(winSize.width, 710);
    m_hostUid = hostUid;
    m_roomId = roomId;
    
    LayerColor *contentBg = LayerColor::create(Color4B(32, 32, 32, 255));
    contentBg->setContentSize(contentSize);
    contentBg->setPosition(Vec2(0, 0));
    contentBg->setName("content_bg");
    this->addChild(contentBg);
    
    LayerColor *titleBg = LayerColor::create(Color4B(32, 32, 32, 255));
    titleBg->setContentSize(Size(winSize.width, 75));
    titleBg->setPosition(Vec2(0, contentBg->getPositionY() + contentBg->getContentSize().height-75));
    titleBg->setName("title_bg");
    this->addChild(titleBg);
    
    LayerColor *line = LayerColor::create(Color4B(0, 0, 0, 255));
    line->setContentSize(Size(winSize.width, 2));
    line->setPosition(titleBg->getPosition());
    this->addChild(line);
    
    Text* titlelabel = Text::create("玩家", "AmericanTypewriter", 36);
    titlelabel->setPosition(titleBg->getContentSize() / 2);
    titleBg->addChild(titlelabel);
    
    Button *closeBtn = Button::create("Public/icon_cancel.png");
    closeBtn->setZoomScale(0);
    closeBtn->setPosition(Vec2(winSize.width - 45, titleBg->getContentSize().height / 2));
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->hide();
                                    });
    titleBg->addChild(closeBtn);
    
    Json::Value json;
    json["roomId"] = m_roomId;
    json["UserKey"] = ZJHModel::getInstance()->UserKey;
    json["pageSize"] = 100;
    
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"duiju/invite/onlookersList", json.toStyledString(), "room_all_player");
    
    this->schedule(schedule_selector(RoomAllPlayerLayer::getData), 0.0f);
}

void RoomAllPlayerLayer::getData(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["room_all_player"];
    if (loginPacket && loginPacket->status != 0)
    {
        unschedule(schedule_selector(RoomAllPlayerLayer::getData));
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        else
        {
            if (loginPacket->resultIsOK())
            {
                initListView(loginPacket->recvVal["resultMap"]);
            }
            else
            {
                
            }
        }
        loginPacket->status = 0;
        CCHttpAgent::getInstance()->packets["room_all_player"] = NULL;
        delete loginPacket;
    }
}

void RoomAllPlayerLayer::initListView(Json::Value &data)
{
    Size winSize = Director::getInstance()->getWinSize();
    Size viewSize = Size(winSize.width - 10,630);
    
    bool isHost = ZJHModel::getInstance()->uid == m_hostUid;
    Json::Value realData = data["onlookersList"]["result"];
    
    Json::Value tempData;
    int a = 1;
    for (int i = 0; i < realData.size(); i++) {
        Json::Value temp = realData[i];
        if (!temp["latitude"].empty() && !temp["longitude"].empty()) {
            tempData[a-1] = temp;
        }
        a++;
    }
    
    auto *tishi = Text::create("提示","AmericanTypewriter",26);
    tishi->setTextColor(Color4B(223, 186, 100, 255));
    tishi->setAnchorPoint(Vec2(0, 1.0f));
    
    map<std::string,vector<std::string>> ddd = Utils::searchIpSameName(realData);
    
    auto *tipLayer = LayerColor::create(Color4B(51, 51, 51, 255));
    auto *scrollView = ScrollView::create();
    scrollView->setContentSize(viewSize);
    scrollView->setPosition(Vec2(2, 0));
    scrollView->setTouchEnabled(true);
    scrollView->setDirection(ScrollView::Direction::VERTICAL);
    
    vector<LabelTTF *> tempvec;
    int tempHeight = tishi->getContentSize().height + 10;
    std::map<std::string,vector<std::string>>::iterator it;
    string mm = "";
    int m = 0;
    for (it = ddd.begin(); it != ddd.end(); it++) {
        string key = it->first;
        vector<string> cc = it->second;
        if (cc.size() > 1) {
            string nn = "";
            for (int i = 0; i < cc.size(); i++) {
                if (i != cc.size() - 1) {
                    nn += cc[i] + "、";
                } else {
                    nn += cc[i];
                }
            }
            nn += "位于同一IP段";
            auto tishiTex = LabelTTF::create(nn, ".SFUIDisplay-Semibold", 26,Size(viewSize.width, 0), TextHAlignment::LEFT);
            tishiTex->setAnchorPoint(Vec2(0, 1.0f));
            tempHeight += tishiTex->getContentSize().height+10;
            tempvec.push_back(tishiTex);
            tipLayer->addChild(tishiTex);
        }
    }
    
    vector<vector<int>> retlist;
    Utils::searchGather(retlist, tempData);
    for (int i = 0; i < retlist.size(); i++) {
        vector<int> temp = retlist[i];
        if (temp.size() > 1) {
            string nn = "";
            for (int j = 0; j < temp.size(); j++) {
                Json::Value val = realData[temp[j]];
                char buff[20];
                memset(buff, 0, sizeof(buff));
                Utils::parseName(4,buff,val["nickName"].asCString());
                if (j != temp.size() - 1) {
                    snprintf(buff, sizeof(buff), "%s、",buff);
                } else {
                    snprintf(buff, sizeof(buff), "%s",buff);
                }
                nn += buff;
            }
            nn += "相距小于500米";
            auto tishiTex = LabelTTF::create(nn, ".SFUIDisplay-Semibold", 26,Size(viewSize.width, 0), TextHAlignment::LEFT);
            tishiTex->setAnchorPoint(Vec2(0, 1.0f));
            tempHeight += tishiTex->getContentSize().height+10;
            tempvec.push_back(tishiTex);
            tipLayer->addChild(tishiTex);
        }
    }
    
    int aa = tempHeight + ceil((realData.size() + 2)/2) * 125 + 15;
    Size scrollSize = Size(viewSize.width, aa > viewSize.height ? aa : viewSize.height);
    int height = tishi->getContentSize().height + 10;
    for (int i = 0; i < tempvec.size(); i++) {
        auto tex = tempvec[i];
        tex->setPosition(Vec2(10, tempHeight - height));
        height += tex->getContentSize().height + 10;
    }
    height += 10;
    
    
    scrollView->setInnerContainerSize(scrollSize);
    //    scrollView->setScrollBarEnabled(false);
    scrollView->setBackGroundColor(Color3B(255, 100, 0));
    getChildByName("content_bg")->addChild(scrollView);
    
    tipLayer->setContentSize(Size(scrollSize.width,height));
    scrollView->addChild(tipLayer);
    
    tishi->setPosition(Vec2(10, height - 10));
    tipLayer->addChild(tishi);
    tipLayer->setPosition(Vec2(0, scrollSize.height-height));
    
    Size contentSize = Size(scrollSize.width, scrollSize.height-height-15);
    
    
//    map<std::string,vector<std::string>> ddd = Utils::searchIpSameName(realData);
//    std::map<std::string,vector<std::string>>::iterator it;
//    string mm = "";
//    for (it = ddd.begin(); it != ddd.end(); it++) {
//        string key = it->first;
//        vector<string> cc = it->second;
//        
//        if (cc.size() > 1) {
//            string nn = "";
//            for (int i = 0; i < cc.size(); i++) {
//                if (i != cc.size() - 1) {
//                    nn += cc[i] + "、";
//                } else {
//                    nn += cc[i];
//                }
//            }
//            nn += "位于同一IP段";
//            mm += nn + " ";
//        }
//    }
    
//    Json::Value tempData;
//    int a = 1;
//    for (int i = 0; i < realData.size(); i++) {
//        Json::Value temp = realData[i];
//        if (!temp["latitude"].empty() && !temp["longitude"].empty()) {
//            tempData[a-1] = temp;
//        }
//        a++;
//    }
//    
//    vector<vector<int>> retlist;
//    Utils::searchGather(retlist, tempData);
//    for (int i = 0; i < retlist.size(); i++) {
//        vector<int> temp = retlist[i];
//        if (temp.size() > 1) {
//            string nn = "";
//            for (int j = 0; j < temp.size(); j++) {
//                Json::Value val = realData[temp[j]];
//                char buff[20];
//                memset(buff, 0, sizeof(buff));
//                Utils::parseName(4,buff,val["nickName"].asCString());
//                if (j != temp.size() - 1) {
//                    snprintf(buff, sizeof(buff), "%s、",buff);
//                } else {
//                    snprintf(buff, sizeof(buff), "%s",buff);
//                }
//                nn += buff;
//            }
//            nn += "相距低于500米";
//            mm += nn + " ";
//        }
//    }
    
//    auto *tishi = Text::create("提示","AmericanTypewriter",26);
//    tishi->setTextColor(Color4B(223, 186, 100, 255));
//    tishi->setAnchorPoint(Vec2(0, 1.0f));
    
//    auto tishiTex = LabelTTF::create(mm, ".SFUIDisplay-Semibold", 26,
//                                  Size(winSize.width-20, 0), TextHAlignment::LEFT);
//    tishiTex->setAnchorPoint(Vec2(0, 0.0f));
    
//    int height = tishi->getContentSize().height + tishiTex->getContentSize().height + 25;
//    auto *tipLayer = LayerColor::create(Color4B(51, 51, 51, 255));
//    tipLayer->setContentSize(Size(winSize.width,height));
//    getChildByName("content_bg")->addChild(tipLayer);
//    
//    tishi->setPosition(Vec2(10, height - 10));
//    tishiTex->setPosition(Vec2(10, 10));
//    tipLayer->addChild(tishi);
//    tipLayer->addChild(tishiTex);
//    tipLayer->setPosition(Vec2(2, 790-height));
//    
//    Size contentSize = Size(winSize.width, 790 - tipLayer->getContentSize().height - 15);
    ListView *resultList = ListView::create();
    resultList->setDirection(ScrollView::Direction::VERTICAL);
    resultList->setBounceEnabled(false);
    resultList->setContentSize(contentSize);
    resultList->setAnchorPoint(Vec2(0, 0));
    resultList->setPosition(Vec2(0, 0));
    resultList->setItemsMargin(15);
    resultList->setScrollBarEnabled(false);
    resultList->setTouchEnabled(false);
//    getChildByName("content_bg")->addChild(resultList);
    scrollView->addChild(resultList);
    
    int num = data["onlookersList"]["result"].size();
    Size layoutSize = Size(winSize.width, 110);
    Size itemSize = Size(winSize.width / 2 - 10, layoutSize.height);
    if (isHost) {
        Layout *layout = Layout::create();
        layout->setContentSize(layoutSize);
        
        Widget *item = Widget::create();
        item->setContentSize(itemSize);
        item->setPosition(Vec2(layoutSize.width * 0.25, layoutSize.height / 2));
        item->setTouchEnabled(true);
        item->addClickEventListener(CC_CALLBACK_1(RoomAllPlayerLayer::inviteFriend, this));
        layout->addChild(item);
        
        LayerColor *grayBg = LayerColor::create(Color4B(51, 51, 51, 255));
        grayBg->setContentSize(itemSize);
        grayBg->setPosition(Vec2(2, 2));
        item->addChild(grayBg);

        auto *btn = Sprite::create("Room/bar_02.png");
        btn->setPosition(itemSize/2);
        item->addChild(btn);
        
        if (num > 0) {
            int index = 0;
            Widget *item = this->createItem(data["onlookersList"]["result"][index]);
            item->setPosition(Vec2(layoutSize.width * 0.75, layoutSize.height / 2));
            layout->addChild(item);
        }
        
        resultList->addChild(layout);
    }
    
    for (int i = isHost ? 1 : 0; i < num; i += 2) {
        Layout *layout = Layout::create();
        layout->setContentSize(layoutSize);
        
        Widget *item = this->createItem(data["onlookersList"]["result"][i]);
        item->setPosition(Vec2(layoutSize.width * 0.25, layoutSize.height / 2));
        layout->addChild(item);
        
        if (i+1 < num) {
            Widget *item = this->createItem(data["onlookersList"]["result"][i+1]);
            item->setPosition(Vec2(layoutSize.width * 0.75, layoutSize.height / 2));
            layout->addChild(item);
        }
        resultList->addChild(layout);
    }
    resultList->refreshView();
}

Widget *RoomAllPlayerLayer::createItem(Json::Value &itemData)
{
    Size itemSize = Size(Director::getInstance()->getWinSize().width / 2 - 10, 110);
    Widget *item = Widget::create();
    item->setContentSize(itemSize);
    
    std::string nickName = itemData["nickName"].asString();
    std::string image = itemData["thumbUrl"].asString();
    int gold = itemData["golden"].asInt();
    int uid = itemData["userId"].asInt();
    float latitude = itemData["latitude"].asDouble();
    float longitude = itemData["longitude"].asDouble();
    string city = itemData["city"].asString();
    string ip = itemData["ip"].asString();
    char buff[128];
    
    LayerColor *bg = LayerColor::create(Color4B(51, 51, 51, 255));
    bg->setContentSize(itemSize);
    item->addChild(bg);
    
    Sprite *userAvatar = Utils::getImageSavePath(image) == "" ? NULL : Sprite::create(Utils::getImageSavePath(image));
    if (userAvatar == NULL) {
        userAvatar = Sprite::create("Avatars/user4_unlogin.png");
    }
    userAvatar = Utils::CirCleSprite(userAvatar, "Avatars/StencilBg.png");
    userAvatar->getTexture()->setAntiAliasTexParameters();
    userAvatar->setPosition(Vec2(50, itemSize.height / 2));
    userAvatar->setScale(0.82);
    item->addChild(userAvatar);
    
    if (uid == m_hostUid) {
        Sprite *sp = Sprite::create("Room/btn_yellow_small.9.png");
        sp->setScale(70 / sp->getContentSize().width, 30 / sp->getContentSize().height);
        sp->setPosition(Vec2(userAvatar->getContentSize().width / 2, 5));
        userAvatar->addChild(sp);
        
        Text *host = Text::create("房主", "AmericanTypewriter", 20);
        host->setPosition(Vec2(userAvatar->getContentSize().width / 2, 5));
        host->setColor(Color3B::BLACK);
        userAvatar->addChild(host);
    }
    Utils::parseName(4, buff, nickName.c_str());
    
    Text* name = Text::create(buff, "AmericanTypewriter", 22);
    name->setAnchorPoint(Vec2(0, 0.5));
    name->setPosition(Vec2(100, 80));
    name->setTextColor(Color4B(225,205,173,255));
    item->addChild(name);

    Text *ipTex = Text::create("ip:"+ip,"AmericanTypewriter", 16);
    ipTex->setAnchorPoint(Vec2(0, 0.5f));
    ipTex->setPosition(Vec2(100, 52));
    ipTex->setTextColor(Color4B(153,153,153,255));
    item->addChild(ipTex);

    Text *addressTex = Text::create("","AmericanTypewriter", 16);
    addressTex->setAnchorPoint(Vec2(0, 0.5f));
    addressTex->setPosition(Vec2(100, 18));
    if (ip == "") {
        addressTex->setString("地址: 检测不到玩家位置");
        addressTex->setTextColor(Color4B(218,45,21, 255));
    } else {
        addressTex->setString("地址: "+city);
        addressTex->setTextColor(Color4B(153,153,153,255));
    }
    item->addChild(addressTex);
    
//    char goldStr[64];
//    Utils::parseNums2(goldStr, gold);
//    snprintf(buff, sizeof(buff), "%s金币", goldStr);
//    Text* goldLabel = Text::create(buff, "AmericanTypewriter", 26);
//    goldLabel->setAnchorPoint(Vec2(1, 0.5));
//    goldLabel->setPosition(Vec2(itemSize.width - 15, 20));
//    item->addChild(goldLabel);
    
    return item;
}

void RoomAllPlayerLayer::inviteFriend(cocos2d::Ref *sender)
{
    Json::Value json;
    json["pageNo"] = 1;
    json["pageSize"] = 50;
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
                                                                 RoomInviteFriendLayer *layer = RoomInviteFriendLayer::create();
                                                                 layer->myInit(m_roomId, packet->recvVal["resultMap"]);
                                                                 Director::getInstance()->getRunningScene()->addChild(layer, Z_ORDER_IN_GAME_LAYER);
                                                                 layer->show();
                                                             }
                                                             else
                                                             {
                                                                 PlatformHelper::showToast(packet->recvVal["message"].asString().c_str());
                                                             }
                                                             
                                                         }
                                                         //
                                                     }
                                                     
                                                     //删除packet
                                                     CCHttpAgent::getInstance()->packets.erase(tag);
                                                     delete packet;
                                                 }
                                             },
                                             "friend/listFriends", json.toStyledString(), "invite_friend");
}

bool RoomAllPlayerLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    LayerColor *contentBg = dynamic_cast<LayerColor *>(this->getChildByName("content_bg"));
    LayerColor *titleBg = dynamic_cast<LayerColor *>(this->getChildByName("title_bg"));
    if (contentBg->boundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())) || titleBg->boundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())))
    {
        //
    }
    else
    {
        this->hide();
        event->stopPropagation();
    }
    return true;
}
