//
//  RoomAllPlayerLayer.cpp
//  ZJH
//
//  Created by hyhy on 16/7/27.
//
//

#include "RoomAllPlayerLayer2.h"
#include "RoomInviteFriendLayer.h"
#include "ZJH.h"

void RoomAllPlayerLayer2::show()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(Vec2(0, -winSize.height));
    this->runAction(MoveTo::create(0.17, Vec2(0, 0)));
    this->setVisible(true);
}

void RoomAllPlayerLayer2::hide()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->runAction(Sequence::create(MoveTo::create(0.17, Vec2(0, -winSize.height)),
                                     RemoveSelf::create(),
                                     NULL));
}

bool RoomAllPlayerLayer2::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomAllPlayerLayer2::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void RoomAllPlayerLayer2::myInit(int hostUid, int roomId)
{
    Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = Size(winSize.width, 710);
    m_hostUid = hostUid;
    m_roomId = roomId;
    
    LayerColor *contentBg = LayerColor::create(Color4B(0x1e,0x1e, 0x1f, 255));
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
    
    Text* titlelabel = Text::create("玩家列表", "AmericanTypewriter", 36);
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
    
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"duiju/invite/lookList", json.toStyledString(), "room_all_player");
    
    this->schedule(schedule_selector(RoomAllPlayerLayer2::getData), 0.0f);
}

void RoomAllPlayerLayer2::getData(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["room_all_player"];
    if (loginPacket && loginPacket->status != 0)
    {
        unschedule(schedule_selector(RoomAllPlayerLayer2::getData));
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

void RoomAllPlayerLayer2::initListView(Json::Value &data)
{
    Size winSize = Director::getInstance()->getWinSize();
    Size viewSize = Size(winSize.width - 15 * 2,630);
    
    Json::Value realData = data["lookList"]["result"];
    
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
    tishi->setTextColor(Color4B(0xf2, 0xcc, 0x5a, 255));
    tishi->setAnchorPoint(Vec2(0, 1.0f));
    
    map<std::string,vector<std::string>> ipCheck = Utils::searchIpSameName(realData);
    
    auto *tipLayer = LayerColor::create(Color4B(0x1e, 0x1e, 0x1f, 255));
    
    vector<Text *> tempvec;
    int tempHeight = tishi->getContentSize().height + 10;
    std::map<std::string,vector<std::string>>::iterator it;
    string mm = "";
    int m = 0;
    for (it = ipCheck.begin(); it != ipCheck.end(); it++) {
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
            auto tishiTex = Text::create(nn, "AmericanTypewriter",26);
            tishiTex->setAnchorPoint(Vec2(0, 1.0f));
            tishiTex->setTextColor(Color4B(0xf2, 0xcc, 0x5a, 255));

            tempHeight += tishiTex->getContentSize().height+10;
            tempvec.push_back(tishiTex);
            tipLayer->addChild(tishiTex);
        }
    }
    if(tempvec.size() == 0){
        auto tishiTex = Text::create("未检测到有玩家在同一IP", "AmericanTypewriter",26);
        tishiTex->setAnchorPoint(Vec2(0, 1.0f));
        tishiTex->setTextColor(Color4B(0xf2, 0xcc, 0x5a, 255));
        
        tempHeight += tishiTex->getContentSize().height+10;
        tempvec.push_back(tishiTex);
        tipLayer->addChild(tishiTex);
    }
    
    vector<map<vector<int>,int>> mapGather;
    Utils::searchGatherWithPath(mapGather, tempData,500);//mapGather有多少组小于500的 tempData服务器有多少个人
    if(mapGather.size() == 0){
        auto tishiTex = Text::create("未检测到玩家位置较近", "AmericanTypewriter",26);
        tishiTex->setAnchorPoint(Vec2(0, 1.0f));
        tishiTex->setTextColor(Color4B(0xf2, 0xcc, 0x5a, 255));
        
        tempHeight += tishiTex->getContentSize().height+10;
        tempvec.push_back(tishiTex);
        tipLayer->addChild(tishiTex);
    }else{
        for (int i = 0; i < mapGather.size(); i++) {
            std::map<vector<int>,int> temp = mapGather[i];
            std::map<vector<int>,int>::iterator it;
            for(it=temp.begin();it!=temp.end();it++)
            {
                string nn = "";
                for (int j = 0; j < it->first.size(); j++) {
                    Json::Value val = realData[it->first[j]];
                    char buff[128];
                    memset(buff, 0, sizeof(buff));
                    Utils::parseName(10,buff,val["nickName"].asString().c_str());
                    if (j != it->first.size() - 1) {
                        snprintf(buff, sizeof(buff), "%s、",buff);
                    } else {
                        snprintf(buff, sizeof(buff), "%s",buff);
                    }
                    nn += buff;
                }
                stringstream stream;
                stream<<it->second;
                nn += "相距"+stream.str()+"米";
                auto tishiTex = Text::create(nn, "AmericanTypewriter",26);
                tishiTex->setAnchorPoint(Vec2(0, 1.0f));
                tishiTex->setTextColor(Color4B(0xf2, 0xcc, 0x5a, 255));
                
                tempHeight += tishiTex->getContentSize().height+10;
                tempvec.push_back(tishiTex);
                tipLayer->addChild(tishiTex);
            }
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
    
    tipLayer->setContentSize(Size(scrollSize.width,height));
//    getChildByName("content_bg")->addChild(tipLayer);
    
    tishi->setPosition(Vec2(0, height - 10));
    tipLayer->addChild(tishi);
    tipLayer->setPosition(Vec2(20, 0));
    
    Size contentSize = Size(scrollSize.width, getChildByName("content_bg")->getContentSize().height -95);
    

    ListView *resultList = ListView::create();
    resultList->setDirection(ScrollView::Direction::VERTICAL);
    resultList->setBounceEnabled(false);
    resultList->setContentSize(contentSize);
    resultList->setAnchorPoint(Vec2(0.5, 1));
    resultList->setPosition(Vec2(winSize.width / 2, getChildByName("content_bg")->getContentSize().height -95));
    resultList->setItemsMargin(15);
    resultList->setScrollBarEnabled(false);
    resultList->setTouchEnabled(true);
    getChildByName("content_bg")->addChild(resultList);
    
    int num = data["lookList"]["result"].size();
    Size itemSize = Size(resultList->getContentSize().width, 107);
    
    if(tempvec.size() == 0){
        tipLayer->setVisible(false);
    }else{
        Widget *top = Widget::create();
        top->setContentSize(tipLayer->getContentSize());
        top->setAnchorPoint(Vec2(0,0));
        top->setPosition(Vec2(0,0));
        top->addChild(tipLayer);
        resultList->addChild(top);
    }

    for (int i = 0; i < num; i ++) {
        Widget *item = this->createItem(data["lookList"]["result"][i],itemSize,i);
        item->setAnchorPoint(Vec2(0,0));
        item->setPosition(Vec2(0, 0));
        resultList->addChild(item);
    }
    resultList->refreshView();
    
//    Widget *top = Widget::create();
//    top->setContentSize(tipLayer->getContentSize());
//    top->setAnchorPoint(Vec2(0,0));
//    top->setPosition(Vec2(0,0));
//    top->addChild(tipLayer);
//    resultList->insertCustomItem(top, 0);
}

Widget *RoomAllPlayerLayer2::createItem(Json::Value &itemData,Size &itemSize,int i)
{
    Widget *item = Widget::create();
    item->setContentSize(itemSize);
    item->setPosition(Vec2(0,0));
    item->setAnchorPoint(Vec2(0,0));
    
    std::string nickName = itemData["nickName"].asString();
    std::string image = itemData["thumbUrl"].asString();
    int gold = itemData["golden"].asInt();
    int uid = itemData["userId"].asInt();
    float latitude = itemData["latitude"].asDouble();
    float longitude = itemData["longitude"].asDouble();
    int integral = itemData["integral"].asInt();
    int userId = itemData["userId"].asInt();
    string city = itemData["city"].asString();
    string ip = itemData["ip"].asString();
    char buff[128];
    
    LayerColor *bg = LayerColor::create(Color4B(0x26, 0x26, 0x29, 255));
    bg->setContentSize(itemSize);
    bg->setPosition(Vec2(0,0));
    bg->setAnchorPoint(Vec2(0,0));

    item->addChild(bg);
    
    stringstream positionS;
    positionS<<(i+1);
    string p = positionS.str() + ".";
    
//    Text *position = Text::create(p,"AmericanTypewriter", 36);
//    position->setAnchorPoint(Vec2(0, 0.5));
//    position->setPosition(Vec2(30, itemSize.height / 2));
//    if(i < 3){
//        position->setTextColor(Color4B(0xf0,0xcc,0x5b,255));
//    }
//    item->addChild(position);

    
    auto userAvatar = Utils::createCircleImage(image, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(75,75));
    userAvatar->setAnchorPoint(Vec2(0.5,0.5));
    userAvatar->setPosition(Vec2(63, itemSize.height / 2));
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
    Utils::parseName(5, buff, nickName.c_str());
    
    Text* name = Text::create(buff, "AmericanTypewriter", 22);
    name->setAnchorPoint(Vec2(0, 1));
    name->setPosition(Vec2(140, itemSize.height - 27));
    name->setTextColor(Color4B(0xbe,0xae,0x94,255));
    item->addChild(name);
    
    stringstream stream;
    stream<<userId;
    string contentStr = "ID:"+stream.str();
    
    Text *idText = Text::create(contentStr,"AmericanTypewriter", 22);
    idText->setAnchorPoint(Vec2(0, 1));
    idText->setPosition(Vec2(140, itemSize.height - 66));
    idText->setTextColor(Color4B(0x82,0x82,0x82,255));
    item->addChild(idText);

    Text *ipTex = Text::create("ip:"+ip,"AmericanTypewriter", 22);
    ipTex->setAnchorPoint(Vec2(0, 1));
    ipTex->setPosition(Vec2(300, itemSize.height - 27));
    ipTex->setTextColor(Color4B(0x82,0x82,0x82,255));
    item->addChild(ipTex);

    Text *addressTex = Text::create("","AmericanTypewriter", 22);
    addressTex->setAnchorPoint(Vec2(0, 1));
    addressTex->setPosition(Vec2(300, itemSize.height - 66));
    if (ip == "") {
        addressTex->setString("地址: 检测不到玩家位置");
        addressTex->setTextColor(Color4B(0x82,0x82,0x82,255));
    } else {
        addressTex->setString("地址: "+city);
        addressTex->setTextColor(Color4B(0x82,0x82,0x82,255));
    }
    item->addChild(addressTex);
    
//    stringstream intvStream;
//    intvStream<<integral;
//    string integralStr = intvStream.str();
//    
//    Text *integralText = Text::create("","AmericanTypewriter", 38);
//    integralText->setAnchorPoint(Vec2(0, 0.5));
//    integralText->setPosition(Vec2(550, itemSize.height / 2));
//    if(integral >= 0){
//      integralText->setTextColor(Color4B(0x38,0xae,0x12,255));
//      integralText->setString("+"+integralStr);
//    }else{
//      integralText->setString(integralStr);
//      integralText->setTextColor(Color4B(0xca,0x02,0x00,255));
//    }
    
//    item->addChild(integralText);

    
    
    return item;
}

void RoomAllPlayerLayer2::inviteFriend(cocos2d::Ref *sender)
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

bool RoomAllPlayerLayer2::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
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
