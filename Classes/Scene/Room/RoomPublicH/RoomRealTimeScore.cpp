//
//  实时战绩，包括玩家位置信息
//
//

#include "RoomRealTimeScore.h"
#include "RoomInviteFriend.h"
#include "../../../Utils/Utils.h"


//初始操作
bool RoomRealTimeScore::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    return true;
}

//显示战绩对话框
void RoomRealTimeScore::show()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(Vec2(0, 0));
    this->setVisible(true);
    this->addLayerTouchEvent();
}

//隐藏战绩对话框
void RoomRealTimeScore::hide()
{
    touchListener->setSwallowTouches(false);
    Director::getInstance()->getEventDispatcher()->removeEventListener(touchListener);
    this->setVisible(false);
}


//增加Layer触摸事件
void RoomRealTimeScore::addLayerTouchEvent() {
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomRealTimeScore::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

//初始绘图，数据请求
void RoomRealTimeScore::myInit()
{
    Size winSize = Director::getInstance()->getWinSize();
    Size bgSize = Size(890,599);
    Size scSize = Size(779, 500);
    m_hostUid = ZJHModel::getInstance()->roomData["gameRoom"]["userId"].asInt();
    m_roomId = ZJHModel::getInstance()->roomData["gameRoom"]["id"].asInt();
    
    //蒙层
    auto mask = LayerColor::create(Color4B(0, 0, 0, 50));
    mask->setContentSize(Size(winSize.width*2, winSize.height));
    mask->setPositionX(-winSize.width);
    this->addChild(mask);
    
    //背景框
    auto contentBg = Scale9Sprite::create("RoomPublicH/dialog/room_dialog_big.png");
    contentBg->setAnchorPoint(Vec2(0.5, 0.5));
    contentBg->setPosition(winSize / 2);
    contentBg->setContentSize(Size(bgSize.width, bgSize.height));
    contentBg->setName("content_bg");
    this->addChild(contentBg);
    
    //标题图片
    auto titleSp = ImageView::create("RoomPublicH/dialog/dialog_realtime_title.png");
    titleSp->setScale9Enabled(true);
    titleSp->setAnchorPoint(Vec2(0.5, 1));
    titleSp->setPosition(Vec2(bgSize.width/2, bgSize.height));
    contentBg->addChild(titleSp);
   
    //滑动嵌套ListView
    scrollView = Layout::create();
    scrollView->setAnchorPoint(Vec2(0, 0));
    scrollView->setContentSize(scSize);
    scrollView->setPosition(Vec2(64, 25));
    scrollView->setTouchEnabled(true);
//    scrollView->setDirection(ScrollView::Direction::VERTICAL);
    contentBg->addChild(scrollView);

    //关闭按钮
    auto closeBtn = Button::create("RoomPublicH/dialog/dialog_close.png", "RoomPublicH/dialog/dialog_close_press.png");
    closeBtn->ignoreContentAdaptWithSize(false);
    closeBtn->setScale9Enabled(true);
    closeBtn->setPosition(Vec2(bgSize.width - 65, bgSize.height - 69));
    closeBtn->setTag(ALERT_DIALOG_BUTTON_CLOSE);
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->removeFromParentAndCleanup(true);
                                    });
    contentBg->addChild(closeBtn);

    
    //封装获取玩家的请求
    Json::Value json;
    json["roomId"] = m_roomId;
    json["UserKey"] = ZJHModel::getInstance()->UserKey;
    
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"duiju/invite/lookList", json.toStyledString(), "room_all_player");
    //获取返回结果
    this->schedule(schedule_selector(RoomRealTimeScore::getData), 0.0f);
}

//获取返回结果
void RoomRealTimeScore::getData(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["room_all_player"];
    if (loginPacket && loginPacket->status != 0)
    {
        unschedule(schedule_selector(RoomRealTimeScore::getData));
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        else
        {
            if (loginPacket->resultIsOK())
            {
                //load数据
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


//加载是否处于同一个IP段，是否有距离小于500米
LayerColor* RoomRealTimeScore::initTips(Json::Value &data){
    vector<Text *> tempvec;
    auto tipLayer = LayerColor::create();
    //提示
    auto tishi = Text::create("提示:","AmericanTypewriter",24);
    tishi->setTextColor(Color4B(0x4d, 0x2d, 0x1e, 255));
    tishi->setAnchorPoint(Vec2(0, 1.0f));
    tipLayer->addChild(tishi);
    tempvec.push_back(tishi);
    cursorHeight = tishi->getContentSize().height + 10;
    Json::Value realData = data["lookList"]["result"];

    //计算是否有在同一个IP段的人
    map<std::string,vector<std::string>> sameGroup = Utils::searchIpSameName(realData);
    
    std::map<std::string,vector<std::string>>::iterator it;
    string mm = "";

    bool isIpTogether = false;
    for (it = sameGroup.begin(); it != sameGroup.end(); it++) {
        vector<string> childV = it->second;
        //如果有两个以上则是处于同一IP段
        if (childV.size() > 1) {
            string ipTipsStr = "";
            for (int i = 0; i < childV.size(); i++) {
                if (i != childV.size() - 1) {
                    ipTipsStr += childV[i] + "、";
                } else {
                    ipTipsStr += childV[i];
                }
            }
            ipTipsStr += "位于同一IP段";
            auto ipTips = Text::create(ipTipsStr, ".SFUIDisplay-Semibold", 24);
            ipTips->setTextColor(Color4B(0xaf, 0x5a, 0x22,255));
            ipTips->setAnchorPoint(Vec2(0, 1.0f));
            tempvec.push_back(ipTips);
            tipLayer->addChild(ipTips);
            cursorHeight += ipTips->getContentSize().height + 10;
            tishi->setVisible(true);
            isIpTogether = true;
        }
    }
    if(!isIpTogether){
        auto ipTips = Text::create("未检测到有玩家在同一IP", ".SFUIDisplay-Semibold", 24);
        ipTips->setTextColor(Color4B(0xaf, 0x5a, 0x22,255));
        ipTips->setAnchorPoint(Vec2(0, 1.0f));
        cursorHeight += tipsItemHeight;
        tempvec.push_back(ipTips);
        tipLayer->addChild(ipTips);
        cursorHeight += ipTips->getContentSize().height + 10;
        tishi->setVisible(true);

    }
    
    //临时存储经纬度不为空的用户数据（计算距离小于500米）
    Json::Value tempData;
    int a = 1;
    for (int i = 0; i < realData.size(); i++) {
        Json::Value temp = realData[i];
        if (!temp["latitude"].empty() && !temp["longitude"].empty() && temp["city"].asString() != "") {
            tempData[a-1] = temp;
        }
        a++;
    }
    
    //获取到距离小于500米的用户
    vector<map<vector<int>,int>> mapGather;
    Utils::searchGatherWithPath(mapGather, tempData,500);//mapGather有多少组小于500的 tempData服务器有多少个人
    if(mapGather.size() == 0){
        auto limitTips = Text::create("未检测到玩家位置较近", ".SFUIDisplay-Semibold", 24);
        limitTips->setTextColor(Color4B(0xaf, 0x5a, 0x22,255));
        limitTips->setAnchorPoint(Vec2(0, 1.0f));
        cursorHeight += tipsItemHeight;
        tempvec.push_back(limitTips);
        tipLayer->addChild(limitTips);
        cursorHeight += limitTips->getContentSize().height + 10;
        tishi->setVisible(true);
    }else{
        for (int i = 0; i < mapGather.size(); i++) {
            std::map<vector<int>,int> temp = mapGather[i];
            std::map<vector<int>,int>::iterator it;
            for(it=temp.begin();it!=temp.end();it++)
            {
                string nn = "";
                for (int j = 0; j < it->first.size(); j++) {
                    Json::Value val = realData[it->first[j]];
                    char buff[512];
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
                auto limitTips = Text::create(nn, ".SFUIDisplay-Semibold", 24);
                limitTips->setTextColor(Color4B(0xaf, 0x5a, 0x22,255));
                limitTips->setAnchorPoint(Vec2(0, 1.0f));
                tempvec.push_back(limitTips);
                tipLayer->addChild(limitTips);
                cursorHeight += limitTips->getContentSize().height + 10;
                tishi->setVisible(true);
        }
      }
    }
    tipLayer->setContentSize(Size(scrollView->getContentSize().width,cursorHeight));
    int height = 0;
    for (int i = 0; i < tempvec.size(); i++) {
        auto tex = tempvec[i];
        tex->setPosition(Vec2(10, tipLayer->getContentSize().height - height));
        height += tex->getContentSize().height + 10;
    }
    return tipLayer;
}


//加载数据
void RoomRealTimeScore::initListView(Json::Value &data)
{
    //初始化tips
    auto* layerTips = initTips(data);
    
    Size winSize = Director::getInstance()->getWinSize();
    Size scrollSize = scrollView->getContentSize();
    
    //是否是房主查看
    bool isHost = ZJHModel::getInstance()->uid == m_hostUid;
    Json::Value realData = data["lookList"]["result"];
    
    //==============测试提示=========================
//    std::string limitTipsStr[5] =
//    {"金毛狮王、张无忌位于同一IP段",
//    "杨康、郭靖位于同一IP段",
//    "小龙女、杨过相距小于500米",
//    "郭靖、黄蓉相距小于500米",
//    "二逼、杨过相距小于500米",};
//    
//    for (int i=0; i<5; i++){
//        auto limitTips = LabelTTF::create(limitTipsStr[i], ".SFUIDisplay-Semibold", 24,Size(scrollView->getContentSize().width, tipsItemHeight), TextHAlignment::LEFT);
//        limitTips->setFontFillColor(Color3B(0xaf, 0x5a, 0x22));
//        limitTips->setAnchorPoint(Vec2(0, 1.0f));
//        limitTips->setPosition(Vec2(0, scrollView->getContentSize().height - cursorHeight));
//        scrollView->addChild(limitTips);
//        cursorHeight += tipsItemHeight;
//    }
    
    //=============测试提示==========================
    Size contentSize = Size(scrollSize.width, scrollSize.height - 30);
    
    ListView *resultList = ListView::create();
    resultList->setDirection(ScrollView::Direction::VERTICAL);
    resultList->setBounceEnabled(false);
    resultList->setContentSize(contentSize);
    resultList->setAnchorPoint(Vec2(0, 0));
    resultList->setPosition(Vec2(0, 20));
    resultList->setItemsMargin(10);
//    resultList->setScrollBarEnabled(false);
//    resultList->setTouchEnabled(false);
    scrollView->addChild(resultList);
    Widget *top = Widget::create();
    top->setContentSize(layerTips->getContentSize());
    top->setAnchorPoint(Vec2(0,0));
    top->setPosition(Vec2(0,0));
    top->addChild(layerTips);
    resultList->addChild(top);

    int num = data["lookList"]["result"].size();
    
//    for (int j=0 ; j<20; j++){
        for (int i=0; i < num; i++) {
            Widget *item = this->createItem(data["lookList"]["result"][i]);
            resultList->addChild(item);
        }
//    }
}


//创建ListView的Item
Widget *RoomRealTimeScore::createItem(Json::Value &itemData)
{
    Size itemSize = Size(744, 70);
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
    int integral = itemData["integral"].asInt();
    char buff[128];
    
    //item背景
    auto itemBg = ui::Scale9Sprite::create(uid == m_hostUid ? "RoomPublicH/dialog/dialog_item_me_bg.png":"RoomPublicH/dialog/dialog_item_other_bg.png");
    itemBg->setPreferredSize(Size(744, 70));
    itemBg->setPosition(itemSize/2);
    item->addChild(itemBg);
    
    //用户头像
    Sprite *userAvatar = Utils::getImageSavePath(image) == "" ? NULL : Sprite::create(Utils::getImageSavePath(image));
    if (userAvatar == NULL) {
        userAvatar = Sprite::create("Avatars/user4_unlogin.png");
    }
    userAvatar = Utils::CirCleSprite(userAvatar, "Avatars/StencilBg.png");
    userAvatar->getTexture()->setAntiAliasTexParameters();
    userAvatar->setPosition(Vec2(80, itemSize.height / 2));
    userAvatar->setScale(0.7);
    item->addChild(userAvatar);
    
    if (uid == m_hostUid) {
        Sprite *sp = Sprite::create("RoomPublic/common/icon_friend_roommaster.png");
        sp->setPosition(Vec2(userAvatar->getPositionX(), 20));
        item->addChild(sp);
    }
    
    //用户名称
    Utils::parseName(7, buff, nickName.c_str());
    Text* name = Text::create(buff, "AmericanTypewriter", 22);
    name->setAnchorPoint(Vec2(0, 1));
    name->setPosition(Vec2(152, itemSize.height - 10));
    name->setTextColor(Color4B(0x4d, 0x2d, 0x1e, 255));
    item->addChild(name);
    
    //用户ID
    snprintf(buff, sizeof(buff), "ID: %d", uid);
    Text* idText= Text::create(buff, "AmericanTypewriter", 22);
    idText->setAnchorPoint(Vec2(0, 0));
    idText->setPosition(Vec2(152, 10));
    idText->setTextColor(Color4B(0x4d, 0x2d, 0x1e, 255));
    item->addChild(idText);
    
    //用户IP
    Text *ipTex = Text::create("ip:"+ip,"AmericanTypewriter", 22);
    ipTex->setAnchorPoint(Vec2(0, 1));
    ipTex->setPosition(Vec2(334, itemSize.height - 10));
    ipTex->setTextColor(Color4B(0x4d, 0x2d, 0x1e, 255));
    item->addChild(ipTex);
    
    //用户地址
    Text *addressTex = Text::create("","AmericanTypewriter", 22);
    addressTex->setAnchorPoint(Vec2(0, 0));
    addressTex->setPosition(Vec2(334, 10));
    if (city == "") {
        addressTex->setString("地址: 检测不到玩家位置");
        addressTex->setTextColor(Color4B(218,45,21, 255));
    } else {
        addressTex->setString("地址: "+city);
        addressTex->setTextColor(Color4B(0x4d, 0x2d, 0x1e, 255));
    }
    item->addChild(addressTex);
    
    //用户ID
   
//    snprintf(buff, sizeof(buff), ":%d", integral);
//    TextAtlas* integralText= TextAtlas::create(buff, integral >= 0 ? "RoomPublicH/dialog/score_item_win.png":"RoomPublicH/dialog/score_item_lose.png", 22, 32, "0");
//    integralText->setAnchorPoint(Vec2(1, 0.5));
//    integralText->setPosition(Vec2(itemSize.width-36, itemSize.height/2));
//    item->addChild(integralText);
    return item;
}

void RoomRealTimeScore::inviteFriend(cocos2d::Ref *sender)
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
                                                                 auto layer = RoomInviteFriend::create();
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

bool RoomRealTimeScore::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Node *contentBg = dynamic_cast<Node *>(this->getChildByName("content_bg"));
    if (contentBg->boundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())) )
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
