//
//  CreateRoom.cpp
//  ZJH
//
//  Created by apple on 16/7/13.
//
//

#include "CreateRoom.hpp"
#include "../Mine/TipShopLayer.hpp"
#include "../Load/LoadTable.h"
#include "../Match/PlayWebLayer.hpp"
#include "../Loading/Loading.h"
#include "../Load/ServerErrorTips.hpp"

bool CreateRoom::init()
{
    
    if (!Layer::init())
    {
        return false;
    }
    Size size = Director::getInstance()->getWinSize();
    
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setAnchorPoint(Vec2(0,0));
    layout->setPosition(Vec2(0,0));
    layout->setTouchEnabled(true);
    layout->setSwallowTouches(true);
    layout->addClickEventListener([=](Ref*){
    });
    addChild(layout,-1000);
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 200));
    bg->setContentSize(size);
    bg->setAnchorPoint(Vec2(0.0f, 0.0f));
    bg->setPosition(0,0);
    addChild(bg);
    
    mbg = Sprite::create("GameHall/createRoom/bg2.png");
    mbg->setPosition(size / 2);
    addChild(mbg);
    
    Size csize = mbg->getContentSize();
    
    auto closeBtn = Button::create("GameHall/createRoom/c1.png", "GameHall/createRoom/c2.png");
    closeBtn->setPosition(Vec2(csize.width - 60, csize.height - 60));
    closeBtn->addClickEventListener([=](Ref*){
        this->removeFromParent();
    });
    mbg->addChild(closeBtn,10);
    
    baoIndex = 0;
    
    baoL = -1;
    classL = -1;
    smallL = -1;
    is4Name = false;
    countText = NULL;
    
    Json::Value json;
    json["gameId"] = ZJHModel::getInstance()->curGameID;//2201
    showView(json);
    return true;
}

void CreateRoom::LvMove(Ref *pSender, Widget::TouchEventType type)
{
    if(layInfo != NULL)
    {
        layInfo->setVisible(false);
    }
    
}

void CreateRoom::countNewView()
{
    layout3->removeAllChildren();
    
    countText  = Text::create("局数", "", 45);
    countText->setAnchorPoint(Vec2(0, 1));
    countText->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    countText->setPosition(Vec2(50, layout3->getContentSize().height - 45));
    countText->setTextColor(Color4B(197, 151, 70, 255));
    layout3->addChild(countText);
    
    if(is4Name)
    {
        countText->setFontSize(40);
        countText->setPositionX(30);
    }else
    {
        countText->setFontSize(45);
        countText->setPositionX(50);
    }
    
    float inter = (layout3->getContentSize().width - 170)/3;
    
    for (int i = 0; i < timeArray.size(); i++)
    {
        //局数显示
        Layout *selecBtn;
        if (i == countLastIndex)
        {
            selecBtn = juShuBtnDraw(true, i);
        }else
        {
            selecBtn = juShuBtnDraw(false, i);
        }
        selecBtn->setTouchEnabled(true);
        selecBtn->addTouchEventListener(CC_CALLBACK_2(CreateRoom::clickTime, this));
        selecBtn->setTag(300+i);
        selecBtn->setAnchorPoint(Vec2(0, 1));
        selecBtn->setPosition(Vec2(190 + i%3*inter, layout3->getContentSize().height - 30 - i/3*90));
        layout3->addChild(selecBtn);
    }
    
    auto createTitle = Sprite::create("GameHall/createRoom/line.png");
    createTitle->setPosition(Vec2(layout3->getContentSize().width / 2,0));
    layout3->addChild(createTitle);
    
}
void CreateRoom::showUpdateView()
{
    layout3->removeAllChildren();
    layout4->removeAllChildren();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    int k = 0;
    Text* out3text2  = Text::create("局数", "", 40);
    out3text2->setAnchorPoint(Vec2(0, 0.5));
    out3text2->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    out3text2->setPosition(Vec2(60, layout3->getContentSize().height - 50));
    out3text2->setTextColor(Color4B(0xa6, 0xa7, 0xa4, 255));
    layout3->addChild(out3text2);
    
    if (gameConfig["timeWay"].asInt() == 0)
    {
        timeArray = gameConfig["gameTimeList"];
        time = timeArray[k]["time"].asInt();
        
    }else if (gameConfig["timeWay"].asInt() == 1)
    {
        timeArray = gameConfig["gameRoundList"];
        time = timeArray[k]["round"].asInt();
    }
    
    
    int offInerval1;
    
    if (timeArray.size() <= 1)
    {
        offInerval1 = 770;
    }else
    {
        offInerval1 = 770/(timeArray.size()-1);
    }
    
    //时间线
    auto line = Layout::create();
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->setBackGroundColor(Color3B(0x3d, 0x3d, 0x3d));
    line->ignoreAnchorPointForPosition(false);
    line->setContentSize(Size(770, 5));
    line->setPosition(Vec2(130, 95+10));
    layout3->addChild(line);
    
    for (int i = 0; i < timeArray.size(); i++)
    {
        Layout* timeIcon= Layout::create();
        timeIcon->setAnchorPoint(Vec2(0.5, 0.5));
        timeIcon->cocos2d::ui::Widget::ignoreContentAdaptWithSize(true);
        timeIcon->ignoreAnchorPointForPosition(false);
        timeIcon->setTouchEnabled(true);
        timeIcon->setContentSize(Size(offInerval1, 200));
        timeIcon->addTouchEventListener(CC_CALLBACK_2(CreateRoom::clickTime, this));
        timeIcon->setPosition(Vec2(130 + offInerval1*i, 95+10));
        timeIcon->setTag(300+i);
        layout3->addChild(timeIcon);
        
        Sprite* sp = Sprite::create("Match/check_point_out.png");
        sp->setPosition(Vec2(140 + offInerval1*i, 98+10));
        layout3->addChild(sp);
        
        //时间显示
        char timeBuf[128];
        Text *timeNums;
        
        if (gameConfig["timeWay"].asInt() == 0)
        {
            if (timeArray[i]["time"].asInt()>=60)
            {
                if (timeArray[i]["time"].asInt()%60 == 0)
                {
                    sprintf(timeBuf, "%d时", timeArray[i]["time"].asInt()/60);
                }else
                {
                    string str = ZJHModel::getInstance()->converPointTwoBit(timeArray[i]["time"].asInt()/60.0);
                    sprintf(timeBuf, "%s时", str.c_str());
                }
                
            }else
            {
                sprintf(timeBuf, "%d分", timeArray[i]["time"].asInt());
            }
            
            timeNums  = Text::create(timeBuf, "", 36);
            
        }else if (gameConfig["timeWay"].asInt() == 1)
        {
            sprintf(timeBuf, "%d局", timeArray[i]["round"].asInt());
            
            timeNums  = Text::create(timeBuf, "", 36);
            
        }
        
        timeNums->setTag(350+i);
        timeNums->setPosition(Vec2(timeIcon->getPositionX(), 30));
        timeNums->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
        layout3->addChild(timeNums);
        
        if (i == 0)
        {
            lastTime = timeNums;
            timeSelect= ImageView::create("Match/check_point.png");
            timeSelect->setPosition(timeIcon->getPosition());
            layout3->addChild(timeSelect);
        }
    }
    
    
    //底注和初始带入
    Text *out4Text1 = Text::create("底注", "", 36);
    out4Text1->setAnchorPoint(Vec2(0.5, 0.5));
    out4Text1->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    out4Text1->setPosition(Vec2(275, layout4->getContentSize().height/2));
    out4Text1->setTextColor(Color4B(0xd9, 0xd9, 0xd9, 255));
    layout4->addChild(out4Text1);
    
    char baseBuf[10];
    sprintf(baseBuf, "%d", gameConfig["bottomInList"][k]["base"].asInt());
    
    base = Text::create(baseBuf, "", 55);
    base->setAnchorPoint(Vec2(0.5, 0.5));
    base->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    base->setPosition(Vec2(out4Text1->getPositionX(), out4Text1->getPositionY() + 65));
    base->setTextColor(Color4B(0xe0, 0xcf, 0xae, 255));
    layout4->addChild(base);
    
    Text *out4Text2 = Text::create("初始带入", "", 36);
    out4Text2->setAnchorPoint(Vec2(0.5, 0.5));
    out4Text2->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    out4Text2->setPosition(Vec2(visibleSize.width - 385, out4Text1->getPositionY()));
    out4Text2->setTextColor(Color4B(0xd9, 0xd9, 0xd9, 255));
    layout4->addChild(out4Text2);
    
    char bottomBuf[10];
    sprintf(bottomBuf, "%d", gameConfig["bottomInList"][k]["bottom"].asInt());
    bottomJson = gameConfig["bottomInList"][k];
    bottom = Text::create(bottomBuf, "", 55);
    bottom->setAnchorPoint(Vec2(0.5, 0.5));
    bottom->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    bottom->setPosition(Vec2(out4Text2->getPositionX(), out4Text1->getPositionY() + 65));
    bottom->setTextColor(Color4B(0xe0, 0xcf, 0xae, 255));
    layout4->addChild(bottom);
    
    Text *out4Text3 = Text::create("玩多大", "", 40);
    out4Text3->setAnchorPoint(Vec2(0, 0.5));
    out4Text3->setPosition(Vec2(60, layout4->getContentSize().height - 50));
    out4Text3->setColor(Color3B(0xa6, 0xa7, 0xa4));
    layout4->addChild(out4Text3);
    
    //底注轴 tag:400+
    bottomArray = gameConfig["bottomInList"];
    
    int offInerval2;
    if (bottomArray.size() <= 1)
    {
        offInerval2 = 770;
    }else
    {
        offInerval2 = 770/(bottomArray.size()-1);
    }
    
    //底注线
    auto line2 = Layout::create();
    line2->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line2->setBackGroundColor(Color3B(0x3d, 0x3d, 0x3d));
    line2->ignoreAnchorPointForPosition(false);
    line2->setContentSize(Size(770, 5));
    line2->setPosition(Vec2(130, 60));
    layout4->addChild(line2);
    
    for (int i = 0; i < bottomArray.size(); i++)
    {
        Layout* bottomIcon= Layout::create();
        bottomIcon->setAnchorPoint(Vec2(0.5, 0.5));
        bottomIcon->cocos2d::ui::Widget::ignoreContentAdaptWithSize(true);
        bottomIcon->ignoreAnchorPointForPosition(false);
        bottomIcon->setTouchEnabled(true);
        bottomIcon->setContentSize(Size(offInerval2, 200));
        bottomIcon->addTouchEventListener(CC_CALLBACK_2(CreateRoom::clickBottom, this));
        bottomIcon->setPosition(Vec2(130 + offInerval2*i, 60));
        bottomIcon->setTag(400+i);
        layout4->addChild(bottomIcon);
        
        Sprite* sp = Sprite::create("Match/check_point_out.png");
        sp->setPosition(Vec2(130 + offInerval2*i, 63));
        layout4->addChild(sp);
        
        if (i == 0)
        {
            bottomSelect= ImageView::create("Match/check_point.png");
            bottomSelect->setPosition(bottomIcon->getPosition());
            layout4->addChild(bottomSelect);
        }
    }
    
}

void CreateRoom::postData(Json::Value jsonDate)
{
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        loading->removeFromParent();
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            this->removeFromParent();
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            
            Json::Value data = loginPacket->recvVal["resultMap"]["innerWay"];
            serverData = data;
            //获取缓存的高级配置 对比本地的md5和服务器数据是否一致
            if (gameConfig["timeWay"].asInt() == 0)
            {
                timeArray = gameConfig["gameTimeList"];
            }else if (gameConfig["timeWay"].asInt() == 1)
            {
                timeArray = gameConfig["gameRoundList"];
            }
            string serverMd5Pre = timeArray.toStyledString() + serverData.toStyledString();
            MD5::MD5Str(serverMd5Pre, serverMd5);
            Json::Value cache = getConfigCacheByGameId();
            string localMd5 = cache["configMd5"].asString();
            if(localMd5 == serverMd5){
                //服务器版本和本地版本一致 直接使用本地缓存
                Json::Value cacheData = cache["setData"];
                setData = cacheData;
                std::cout<<"=======服务器版本和本地版本一致 直接使用本地缓存====="<<endl;
            }else{
                //服务器版本和本地版本不一致  使用服务器数据 并且清空缓存
                setData = data;
                clearConfigCache();
                std::cout<<"=======服务器版本和本地版本不一致  使用服务器数据 并且清空缓存====="<<endl;
            }
            
            showListView();
            
            log("set：＝＝＝ [%s]",data.toStyledString().c_str());
            
        }else if(loginPacket->recvVal["status"].asInt() == 999)
        {
            //tomcat没有连上
            string content = loginPacket->recvVal["message"].asString();
            ServerErrorTips* tips = ServerErrorTips::create();
            tips->setContent(content);
            getParent()->addChild(tips,1000);
            this->removeFromParent();
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            this->removeFromParent();
        }
    }, "gameClasli/recommendConfig_v2", jsonDate.toStyledString(), "createSet");
    
}

void CreateRoom::postGameData(Json::Value jsonDate)
{
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        loading1->removeFromParent();
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            this->removeFromParent();
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            
            Json::Value data = loginPacket->recvVal["resultMap"];
            countLastIndex = 0;
            config = data["config"];
            
            gameConfig = data["gameConfig"];
            gameConfig["gameId"] = jsonDate["gameId"];
            
            int k = 0;
            bottomArray = data["gameConfig"]["bottomInList"];
            bottomJson = bottomArray[k];
            
            //游戏帮助的wed网址
            ZJHModel::getInstance()->gameHelpUrl = CCHttpAgent::getInstance()->url+gameConfig["gameHelpUrl"].asString();
            
            Json::Value setJsonTemp;
            setJsonTemp["classifyId"] = data["gameConfig"]["id"];
            setJsonTemp["gameId"] = jsonDate["gameId"];
            
            if (ZJHModel::getInstance()->isTrait == false)
            {
                //1:平台
                setJsonTemp["type"] = 1;
            }
            else
            {
                //2:俱乐部
                setJsonTemp["type"] = 2;
            }
            //1.从缓存中读取并初始化包名索引
            Json::Value cache = getConfigCacheByGameId();
            baoIndex = cache["baoIndex"].asInt();
            countLastIndex = cache["countLastIndex"].asInt();
            string localMd5 = cache["configMd5"].asString();
            printf("localMd5================ = %s\n", localMd5.c_str());
            
            loading = Loading::startLoading(this, Director::getInstance()->getWinSize() / 2);
            loading->maskSwllowTouch();
            postData(setJsonTemp);
            
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            this->removeFromParent();
        }
    }, "gameClasli/gameConfig", jsonDate.toStyledString(), "postGameData");
}

void CreateRoom::showView(Json::Value json)
{
    loading1 = Loading::startLoading(this, Director::getInstance()->getWinSize() / 2);
//    loading1->maskSwllowTouch();
    postGameData(json);
}

void CreateRoom::setBaoView()
{
    //包名
    int nums = 0;
    auto cellBao = Layout::create();
    cellBao->setName("cellBag");
    nums = setData.size();
//    if (setData.size() > 0 && setData.size() <= 8)
//    {
//        cellBao->setContentSize(Size(visibleSize.width, 30 +100*ceil(setData.size()/4.0)));
//        listBg->addChild(cellBao);
//        
//        nums = setData.size();
//        
//    }else if (setData.size() > 8)
//    {
//        cellBao->setContentSize(Size(visibleSize.width, 30 +100*8/4.0));
//        listBg->addChild(cellBao);
//        
//        nums = 7;
//        
//        Button* moreBtn = Button::create("Match/set_more.png","Match/set_more_select.png", "Match/set_more_select.png");
//        moreBtn->addClickEventListener(CC_CALLBACK_1(CreateRoom::onMoreResponse, this));
//        moreBtn->setPosition(Vec2(30 + 3*(240+15) + 120, cellBao->getContentSize().height - 80 - 100 - 15));
//        cellBao->addChild(moreBtn);
//    }
    
    Size size = Size(387,856);
    Size item_size = Size(387,112);
    
    ListView* listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(size);
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 160));
    listView->setScrollBarEnabled(false);
    listView->setName("list");
    mbg->addChild(listView);
    
    listView->setItemsMargin(10);
    
    //包名的tag取值：100+
    for (int i = 0; i < nums; i++)
    {
        auto layout = Layout::create();
        layout->setContentSize(item_size);
        listView->addChild(layout);
        
        Button* baoBtn = Button::create("GameHall/createRoom/title1.png","GameHall/createRoom/title1.png", "GameHall/createRoom/title2.png");
        baoBtn->setTag(100+i);
        baoBtn->addClickEventListener(CC_CALLBACK_1(CreateRoom::onBaoBtnResponse, this));
        baoBtn->setAnchorPoint(Vec2(0.0, 0.5));
//        baoBtn->setTitleText(setData[i]["pkgName"].asString());
//        baoBtn->setTitleFontName("");
//        baoBtn->setTitleFontSize(45);
//        baoBtn->setTitleColor(Color3B(197, 151, 70));
        baoBtn->setPosition(Vec2(45,item_size.height / 2));
        layout->addChild(baoBtn);
        
        Text* out3text2  = Text::create(setData[i]["pkgName"].asString(), "", 45);
        out3text2->setTag(baoBtn->getTag());
        out3text2->setAnchorPoint(Vec2(0.5, 0.5));
        out3text2->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        out3text2->setPosition(Vec2(baoBtn->getContentSize().width / 2 - 21, baoBtn->getContentSize().height / 2));
        out3text2->setTextColor(Color4B(197, 151, 70, 255));
        baoBtn->addChild(out3text2);
        
        //此处要加一个缓存判断上一次选择的是哪个tab页 baoIndex
        if(baoIndex == i)
        {
            lastBaoBtn = baoBtn;
//            baoBtn->setTitleColor(Color3B(0xff, 0xff, 0xff));
            out3text2->setTextColor(Color4B(255, 255, 255, 255));
            baoBtn->setEnabled(false);
        }
    }
}

//收
void CreateRoom::onShouResponse(Ref*ref)
{
    //包名
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout * cellBao = (Layout*)listBg->getChildByName("cellBag");
    cellBao->removeAllChildren();
    cellBao->setContentSize(Size(visibleSize.width, 30 +100*2));
    listBg->refreshView();
    
    Button* moreBtn = Button::create("Match/set_more.png","Match/set_more_select.png", "Match/set_more_select.png");
    moreBtn->addClickEventListener(CC_CALLBACK_1(CreateRoom::onMoreResponse, this));
    moreBtn->setPosition(Vec2(30 + 3*(240+15) + 120, cellBao->getContentSize().height - 80 - 100 - 15));
    cellBao->addChild(moreBtn);
    
    //包名的tag取值：100+
    
    int i = 0;
    if (baoIndex >= 7)
    {
        Button* firstBtn = Button::create("Match/match_bao_n.png","Match/match_bao_s.png", "Match/match_bao_s.png");
        firstBtn->setTag(baoIndex);
        firstBtn->addClickEventListener(CC_CALLBACK_1(CreateRoom::onBaoBtnResponse, this));
        firstBtn->setAnchorPoint(Vec2(0, 0.5));
        firstBtn->setPosition(Vec2(30, cellBao->getContentSize().height - 80));
        firstBtn->setTitleText(setData[baoIndex]["pkgName"].asString());
        firstBtn->setTitleFontName("");
        firstBtn->setTitleFontSize(40);
        cellBao->addChild(firstBtn);
        
        lastBaoBtn = firstBtn;
        firstBtn->setTitleColor(Color3B(0xf0, 0xca, 0x58));
        firstBtn->setEnabled(false);
        
        i = 1;
        
    }
    for (int count = 0; i < 7; i++, count++)
    {
        Button* baoBtn = Button::create("Match/match_bao_n.png","Match/match_bao_s.png", "Match/match_bao_s.png");
        baoBtn->setTag(100+count);
        baoBtn->addClickEventListener(CC_CALLBACK_1(CreateRoom::onBaoBtnResponse, this));
        baoBtn->setAnchorPoint(Vec2(0, 0.5));
        baoBtn->setPosition(Vec2(30 + i%4*(240+15), cellBao->getContentSize().height - 80 - i/4*(100+15)));
        baoBtn->setTitleText(setData[count]["pkgName"].asString());
        baoBtn->setTitleFontName("");
        baoBtn->setTitleFontSize(40);
        cellBao->addChild(baoBtn);
        
        if(baoIndex == count)
        {
            lastBaoBtn = baoBtn;
            baoBtn->setTitleColor(Color3B(0xf0, 0xca, 0x58));
            baoBtn->setEnabled(false);
        }
    }
}

//更多展开
void CreateRoom::onMoreResponse(Ref*ref)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout * cellBao = (Layout*)listBg->getChildByName("cellBag");
    cellBao->removeAllChildren();
    cellBao->setContentSize(Size(visibleSize.width, 30 +115*ceil((setData.size()+1)/4.0)));
    listBg->refreshView();
    
    //包名的tag取值：100+
    for (int i = 0; i < setData.size(); i++)
    {
        Button* baoBtn = Button::create("Match/match_bao_n.png","Match/match_bao_s.png", "Match/match_bao_s.png");
        baoBtn->setTag(100+i);
        baoBtn->addClickEventListener(CC_CALLBACK_1(CreateRoom::onBaoBtnResponse, this));
        baoBtn->setAnchorPoint(Vec2(0, 0.5));
        baoBtn->setPosition(Vec2(30 + i%4*(240+15), cellBao->getContentSize().height - 80 - i/4*(100+15)));
        baoBtn->setTitleText(setData[i]["pkgName"].asString());
        baoBtn->setTitleFontName("");
        baoBtn->setTitleFontSize(40);
        cellBao->addChild(baoBtn);
        
        if(baoIndex == i)
        {
            lastBaoBtn = baoBtn;
            baoBtn->setTitleColor(Color3B(0xf0, 0xca, 0x58));
            baoBtn->setEnabled(false);
        }
    }
    
    Button* shouBtn = Button::create("Match/set_shou.png","Match/set_shou_select.png", "Match/set_shou_select.png");
    shouBtn->setName("shou");
    shouBtn->addClickEventListener(CC_CALLBACK_1(CreateRoom::onShouResponse, this));
    shouBtn->setPosition(Vec2(30 + setData.size()%4*(240+15) + 120, cellBao->getContentSize().height - 80 - setData.size()/4*(100+15)));
    cellBao->addChild(shouBtn);
    
}

void CreateRoom::showListView()
{
//    mbg->removeFromParent();
//    mbg = NULL;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    listBg = ListView::create();
    listBg->ignoreAnchorPointForPosition(false);
    listBg->setContentSize(Size(1469, 750));
    listBg->setAnchorPoint(Vec2(0, 0));
    listBg->setItemsMargin(10);
    listBg->setGravity(ListView::Gravity::CENTER_VERTICAL);
    listBg->addTouchEventListener(CC_CALLBACK_2(CreateRoom::LvMove, this));
    
    //包名显示
    if (setData.size() > 1 || true){
//        mbg = Sprite::create("GameHall/createRoom/bg2.png");
//        addChild(mbg);
        listBg->setPosition(Vec2(390, 253));
//        mbg->setPosition(visibleSize / 2);
        setBaoView();
        type = 1;
    }
    else {
        mbg = Sprite::create("GameHall/createRoom/bg1.png");
        addChild(mbg);
        listBg->setPosition(Vec2(30, 254));
        mbg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        type = 0;
    }
    
    mbg->addChild(listBg);
    
    Size csize = mbg->getContentSize();
    int midX = listBg->getPositionX() + listBg->getContentSize().width / 2;
    
//    auto closeBtn = Button::create("GameHall/createRoom/c1.png", "GameHall/createRoom/c2.png");
//    closeBtn->setPosition(Vec2(csize.width - 50, csize.height - 50));
//    closeBtn->addClickEventListener([=](Ref*){
//        this->removeFromParent();
//    });
//    mbg->addChild(closeBtn,10);
    
    //间距，屏蔽了房间名称
    auto itemInterVal = Layout::create();
    itemInterVal->setContentSize(Size(listBg->getContentSize().width, 15));
    listBg->addChild(itemInterVal);
    
    
    //人数和底注的层
    if (gameConfig["timeWay"].asInt() == 0)
    {
        timeArray = gameConfig["gameTimeList"];
        time = timeArray[countLastIndex]["time"].asInt();
        
    }else if (gameConfig["timeWay"].asInt() == 1)
    {
        timeArray = gameConfig["gameRoundList"];
        time = timeArray[countLastIndex]["round"].asInt();
    }
    
    printf("EEEE = %ld\n", timeArray.size());
    
    Layout *layBg3 = Layout::create();
    layBg3->setContentSize(Size(listBg->getContentSize().width, 40 + ceil(timeArray.size()/3.0)*90));
    listBg->addChild(layBg3);
    
//    ImageView *bg3 = ImageView::create("Match/set_item_bgbar.png");
//    bg3->setScale9Enabled(true);
//    bg3->setPosition(Vec2(layBg3->getContentSize().width/2, layBg3->getContentSize().height/2));
//    bg3->setContentSize(Size(layBg3->getContentSize().width-40, layBg3->getContentSize().height));
//    layBg3->addChild(bg3);
    
    layout3 = Layout::create();
    layout3->setContentSize(Size(listBg->getContentSize().width, layBg3->getContentSize().height));
    layBg3->addChild(layout3, 5);
    
//    Layout *layBg4 = Layout::create();
//    layBg4->setContentSize(Size(visibleSize.width, 240));
    //listBg->addChild(layBg4);
    
//    ImageView *bg4 = ImageView::create("Match/set_item_bgbar.png");
//    bg4->setScale9Enabled(true);
//    bg4->setPosition(Vec2(layBg4->getContentSize().width/2, layBg4->getContentSize().height/2));
//    bg4->setContentSize(Size(layBg4->getContentSize().width-40, layBg4->getContentSize().height));
//    layBg4->addChild(bg4);
    
//    layout4 = Layout::create();
//    layout4->setContentSize(Size(visibleSize.width, 240));
//    layBg4->addChild(layout4, 5);
    
    //showUpdateView();
    countNewView();
    
    setBg = Layout::create();
    listBg->addChild(setBg);
    
    showSetViewData(true);
    
    //友情提示
    {
        auto createTitle = Sprite::create("GameHall/createRoom/line.png");
        createTitle->setPosition(Vec2(midX,252));
        mbg->addChild(createTitle);
        
        Text *tip = Text::create("房间创建成功扣除房卡，提前解散房间返还房卡。", "", 40);
        tip->setAnchorPoint(Vec2(0.5, 0.5));
        tip->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        tip->setPosition(Vec2(midX, 207));
        tip->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
        mbg->addChild(tip);
    }
    
    
    //创建房间按钮
    {
        Button *createBtn = Button::create("GameHall/createRoom/create_btn1.png","GameHall/createRoom/create_btn2.png", "GameHall/createRoom/create_btn2.png");
        createBtn->setPosition(Vec2(csize.width / 2, 75));
        mbg->addChild(createBtn);
        createBtn->addClickEventListener(CC_CALLBACK_1(CreateRoom::sureOpen, this));
        
        auto createTitle = Sprite::create("GameHall/createRoom/create_title.png");
        createTitle->setPosition(Vec2(createBtn->getContentSize().width / 2,createBtn->getContentSize().height / 2 + 10));
        createBtn->addChild(createTitle);
    }
}

void CreateRoom::onBaoBtnResponse(Ref*ref)
{
    Button *onBtn = (Button *)ref;
    int tagBao = onBtn->getTag()-100;
    if(onBtn == lastBaoBtn)
    {
        return;
    }
    
    onBtn->setEnabled(false);
    Text* out3text2 = (Text*)onBtn->getChildByTag(onBtn->getTag());
    out3text2->setTextColor(Color4B(255, 255, 255, 255));
    
//    onBtn->setTitleColor(Color3B(0xff, 0xff, 0xff));
    lastBaoBtn->setEnabled(true);
    out3text2 = (Text*)lastBaoBtn->getChildByTag(lastBaoBtn->getTag());
    out3text2->setTextColor(Color4B(197, 151, 70, 255));
//    lastBaoBtn->setTitleColor(Color3B(197, 151, 70));
    
    lastBaoBtn = onBtn;
    baoIndex = tagBao;
    
    showSetViewData(true);
    
    
}
void CreateRoom::showSetViewData(bool isQie)
{
    setBg->removeAllChildren();
    
    editBoxList.clear();
    isDownShow.clear();
    downCell.clear();
    hideList.clear();
    butMap.clear();
    
    float setH = getSetViewH();
    //局数字大小的改变
    if(is4Name)
    {
        countText->setFontSize(40);
        countText->setPositionX(30);
    }else
    {
        countText->setFontSize(45);
        countText->setPositionX(50);
    }
    
    
    auto size = listBg->getContentSize();//Director::getInstance()->getVisibleSize();
    setBg->setContentSize(Size(size.width, setH));
    
//    ImageView *setImage = ImageView::create("Match/set_item_bgbar.png");
//    setImage->setScale9Enabled(true);
//    setImage->setPosition(Vec2(setBg->getContentSize().width/2, setBg->getContentSize().height/2));
//    setImage->setContentSize(Size(setBg->getContentSize().width-40, setBg->getContentSize().height));
//    setBg->addChild(setImage);
    
    listBg->forceDoLayout();
    
    lastCellH = 0;
    
    Json::Value fistBao = setData[baoIndex]["gameCustomClassifyVOList"];
    
    for (int i = 0; i < fistBao.size(); i++)
    {
        if (fistBao[i]["platformVisible"].asBool() == false)
        {
            hideList.push_back(fistBao[i]);
            continue;
        }
        Json::Value pointJson = fistBao[i]["gameCustomPlayValInfoList"];
        
        setData[baoIndex]["gameCustomClassifyVOList"][i]["isShow"] = true;
        setData[baoIndex]["gameCustomClassifyVOList"][i]["isKong"] = true;
        
        std::vector<Layout*> tempBtn;
        bool isHide = false;
        
        for (int j = 0; j < pointJson.size(); j++)
        {
            isHide = false;
            for(int k1 = 0; k1 < fistBao.size(); k1++)
            {
                Json::Value pointData = fistBao[k1]["gameCustomPlayValInfoList"];
                
                for(int k2 = 0; k2 < pointData.size(); k2++)
                {
                    // 前置条件的判断
                    if((pointJson[j]["premisePlayId"].asInt() == pointData[k2]["orderPlayId"].asInt()) && pointData[k2]["iptValue"] == Json::Value::null)
                    {
                        isHide = true;
                        setData[baoIndex]["gameCustomClassifyVOList"][i]["isShow"] = false;
                        break;
                    }
                }
            }
            
            if(isHide)
            {
                continue;
            }
            
            if (fistBao[i]["choiceType"].asInt() == 1 || fistBao[i]["choiceType"].asInt() == 2)
            {
                bool isSelect = true;
                if (pointJson[j]["iptValue"] == Json::Value::null)
                {
                    isSelect = false;
                }
                Layout *selecBtn;
                
                //飘：
                if (pointJson[j]["classifyId"].asInt() == 29)
                {
                    int tempV = atoi(pointJson[j]["playName"].asString().c_str());
                    tempV *= bottomJson["base"].asInt();
                    __String *buff = __String::createWithFormat("%d", tempV);
                    
                    selecBtn = selectBtnDraw(buff->getCString(), isSelect , fistBao[i]["choiceType"].asInt());
                    
                }else
                {
                    selecBtn = selectBtnDraw(pointJson[j]["playName"].asString(), isSelect , fistBao[i]["choiceType"].asInt());
                }
                
                selecBtn->setTouchEnabled(true);
                selecBtn->setTag(100*i+j);
                selecBtn->setAnchorPoint(Vec2(0, 1));
                selecBtn->addClickEventListener(CC_CALLBACK_1(CreateRoom::selectBtnOn, this));
                tempBtn.push_back(selecBtn);
                
                
            }else if (fistBao[i]["choiceType"].asInt() == 4)
            {
                auto cell = Layout::create();
                cell->setContentSize(Size(size.width, 90));
                cell->setAnchorPoint(Vec2(0, 1));
                cell->setPosition(Vec2(0, setH - lastCellH));
                setBg->addChild(cell, 5);
                
                lastCellH += 90+30;
                
                Text *title =  Text::create(fistBao[i]["classifyName"].asString(), "", 45);
                title->setColor(Color3B(197, 151, 70));
                title->setTextHorizontalAlignment(TextHAlignment::LEFT);
                title->setAnchorPoint(Vec2(0, 0.5));
                title->setPosition(Vec2(50,  cell->getContentSize().height/2));
                cell->addChild(title, 2);
                if(is4Name)
                {
                    title->setFontSize(40);
                    title->setPositionX(30);
                }else
                {
                    title->setFontSize(45);
                    title->setPositionX(50);
                }
                
                type4Edit(cell, pointJson[j], 100*i+j);
                
            }else if (fistBao[i]["choiceType"].asInt() == 3 && pointJson[j]["iptValue"] != Json::Value::null)
            {
                setData[baoIndex]["gameCustomClassifyVOList"][i]["isKong"] = false;
                auto cell = Layout::create();
                cell->setAnchorPoint(Vec2(0, 1));
                cell->setPosition(Vec2(0, setH - lastCellH));
                setBg->addChild(cell, 5);
                cell->setContentSize(Size(size.width, 90));
                
                lastCellH += 90 + 30;
                
            
                Text *title =  Text::create(fistBao[i]["classifyName"].asString(), "", 45);
                title->setColor(Color3B(197, 151, 70));
                title->setTextHorizontalAlignment(TextHAlignment::LEFT);
                title->setAnchorPoint(Vec2(0, 0.5));
                title->setPosition(Vec2(50,  cell->getContentSize().height/2));
                cell->addChild(title, 2);
                if(is4Name)
                {
                    title->setFontSize(40);
                    title->setPositionX(30);
                    
                }else
                {
                    title->setFontSize(45);
                    title->setPositionX(50);
                    
                }
                
                type3Down(cell, pointJson[j], 100*i);
            }
        }
        
        if(fistBao[i]["choiceType"].asInt() == 1 || fistBao[i]["choiceType"].asInt() == 2)
        {
            if(tempBtn.size() > 0)
            {
                //单选中没有一个选中的情况处理
                bool isHaveS = false;
                if (fistBao[i]["choiceType"].asInt() == 1)
                {
                    for (int s1 = 0; s1 < tempBtn.size(); s1++)
                    {
                        Layout *btn_s = tempBtn[s1];
                        ImageView *s_Icon = (ImageView*)btn_s->getChildByName("selecIcon");
                        if (s_Icon->isVisible())
                        {
                            isHaveS = true;
                            break;
                        }
                    }
                    if (isHaveS == false)
                    {
                        //没有选中的单选。默认选中第一个
                        int t = 0;
                        setData[baoIndex]["gameCustomClassifyVOList"][i]["gameCustomPlayValInfoList"][t]["iptValue"] = setData[baoIndex]["gameCustomClassifyVOList"][i]["gameCustomPlayValInfoList"][t]["playId"];
                        Layout *btn_s1 = tempBtn[t];
                        ImageView *s1_Icon = (ImageView*)btn_s1->getChildByName("selecIcon");
                        s1_Icon->setVisible(true);
                    }
                }
                
                auto cell = Layout::create();
                cell->setAnchorPoint(Vec2(0, 1));
                cell->setPosition(Vec2(0, setH - lastCellH));
                setBg->addChild(cell, 5);
                cell->setContentSize(Size(size.width, ceil(tempBtn.size()/3.0) *90));
                
                lastCellH += cell->getContentSize().height + 30;
                
        
                Text *title =  Text::create(fistBao[i]["classifyName"].asString(), "", 45);
                title->setColor(Color3B(197, 151, 70));
                title->setTextHorizontalAlignment(TextHAlignment::LEFT);
                title->setAnchorPoint(Vec2(0, 1));
                title->setPosition(Vec2(50,  cell->getContentSize().height - 50));
                cell->addChild(title, 2);
                if(is4Name)
                {
                    title->setFontSize(40);
                    title->setPositionX(30);
                    
                }else
                {
                    title->setFontSize(45);
                    title->setPositionX(50);
                    
                }
                
                butMap[i] = tempBtn;
                
                for (int k3 = 0; k3 < tempBtn.size(); k3++)
                {
                    Layout *temp = tempBtn.at(k3);
                    temp->setPosition(Vec2(190 + k3%3*((size.width-190)/3), cell->getContentSize().height - 30 - k3/3*90));
                    cell->addChild(temp, 2);
                }
            }
        }else if (setData[baoIndex]["gameCustomClassifyVOList"][i]["choiceType"].asInt() == 3 && setData[baoIndex]["gameCustomClassifyVOList"][i]["isShow"].asBool() && setData[baoIndex]["gameCustomClassifyVOList"][i]["isKong"].asBool())
        {
            auto cell = Layout::create();
            cell->setAnchorPoint(Vec2(0, 1));
            cell->setPosition(Vec2(0, setH - lastCellH));
            setBg->addChild(cell, 5);
            cell->setContentSize(Size(size.width, 90));
            
            lastCellH += 90+30;
            
        
            Text *title =  Text::create(fistBao[i]["classifyName"].asString(), "", 45);
            title->setColor(Color3B(197, 151, 70));
            title->setTextHorizontalAlignment(TextHAlignment::LEFT);
            title->setAnchorPoint(Vec2(0, 1));
            title->setPosition(Vec2(50,  cell->getContentSize().height - 50));
            cell->addChild(title, 2);
            if(is4Name)
            {
                title->setFontSize(40);
                title->setPositionX(30);
                
            }else
            {
                title->setFontSize(45);
                title->setPositionX(50);
                
            }
            
            int t = 0;
            setData[baoIndex]["gameCustomClassifyVOList"][i]["gameCustomPlayValInfoList"][t]["iptValue"] = setData[baoIndex]["gameCustomClassifyVOList"][i]["gameCustomPlayValInfoList"][t]["playId"];
            type3Down(cell, setData[baoIndex]["gameCustomClassifyVOList"][i]["gameCustomPlayValInfoList"][t], 100*i);
        }
    }
    
    if (isQie == false)
    {
        float currenY = listBg->getInnerContainer()->getContentSize().height - listBg->getContentSize().height - lastOffY;
        if (currenY < 0)
        {
            currenY = listBg->getInnerContainer()->getContentSize().height-listBg->getContentSize().height;
        }
        listBg->getInnerContainer()->setPosition(Vec2(0, -currenY));
    }
}


float CreateRoom::getSetViewH()
{
    float listSetH = 0;
    
    Json::Value fistBao = setData[baoIndex]["gameCustomClassifyVOList"];
    
    /*************   判断当前包是否含有4个字 start ************/
    
    for (int i = 0; i < fistBao.size(); i++)
    {
        if (fistBao[i]["platformVisible"].asBool() == false)
        {
            continue;
        }
        
        Json::Value pointJson = fistBao[i]["gameCustomPlayValInfoList"];
        setData[baoIndex]["gameCustomClassifyVOList"][i]["isBigShow"] = true;
        
        bool isBigHide = false;
        for (int j = 0; j < pointJson.size(); j++)
        {
            isBigHide = false;
            for(int k1 = 0; k1 < fistBao.size(); k1++)
            {
                Json::Value pointData = fistBao[k1]["gameCustomPlayValInfoList"];
                
                for(int k2 = 0; k2 < pointData.size(); k2++)
                {
                    if((pointJson[j]["premisePlayId"].asInt() == pointData[k2]["orderPlayId"].asInt()) && pointData[k2]["iptValue"] == Json::Value::null)
                    {
                        isBigHide = true;
                         setData[baoIndex]["gameCustomClassifyVOList"][i]["isBigShow"] = false;
                        break;
                    }
                }
            }
            
            if(isBigHide)
            {
                continue;
            }
        }
        

        //大项需要显示
        if(setData[baoIndex]["gameCustomClassifyVOList"][i]["isBigShow"].asBool() == true)
        {
            Text *tempText = Text::create(fistBao[i]["classifyName"].asString(), "", 45);
            if(tempText->getContentSize().width > 95)
            {
                //大于两个字
                is4Name = true;
                break;
            }
        }
        
    }
    
    /*************   判断当前包是否含有4个字 end ************/
    
    for (int i = 0; i < fistBao.size(); i++)
    {
        if (fistBao[i]["platformVisible"].asBool() == false)
        {
            continue;
        }
        
        Json::Value pointJson = fistBao[i]["gameCustomPlayValInfoList"];
        setData[baoIndex]["gameCustomClassifyVOList"][i]["isShow"] = true;
        setData[baoIndex]["gameCustomClassifyVOList"][i]["isKong"] = true;
        
        int btnSize = 0;
        bool isHide = false;
        
        for (int j = 0; j < pointJson.size(); j++)
        {
            isHide = false;
            for(int k1 = 0; k1 < fistBao.size(); k1++)
            {
                Json::Value pointData = fistBao[k1]["gameCustomPlayValInfoList"];
                
                for(int k2 = 0; k2 < pointData.size(); k2++)
                {
                    if((pointJson[j]["premisePlayId"].asInt() == pointData[k2]["orderPlayId"].asInt()) && pointData[k2]["iptValue"] == Json::Value::null)
                    {
                        isHide = true;
                        setData[baoIndex]["gameCustomClassifyVOList"][i]["isShow"] = false;
                        break;
                    }
                }
            }
            
            if(isHide)
            {
                continue;
            }
            
            if (fistBao[i]["choiceType"].asInt() == 1 || fistBao[i]["choiceType"].asInt() == 2)
            {
                btnSize++;
                
            }else if (fistBao[i]["choiceType"].asInt() == 4)
            {
                listSetH += 90+30;
                
            }else if (fistBao[i]["choiceType"].asInt() == 3 && pointJson[j]["iptValue"] != Json::Value::null)
            {
                listSetH += 90 + 30;
                setData[baoIndex]["gameCustomClassifyVOList"][i]["isKong"] = false;
            }
        }
        
        if(fistBao[i]["choiceType"].asInt() == 1 || fistBao[i]["choiceType"].asInt() == 2)
        {
            if(btnSize > 0)
            {
                listSetH += ceil(btnSize/3.0)*90;
                listSetH += 30;
            }
        }else if (fistBao[i]["choiceType"].asInt() == 3 && setData[baoIndex]["gameCustomClassifyVOList"][i]["isShow"].asBool() && setData[baoIndex]["gameCustomClassifyVOList"][i]["isKong"].asBool())
        {
            listSetH += 90+30;
        }
    }
    listSetH += 40;
    
    return listSetH;
}

void CreateRoom::getSureSetString()
{
    //清除数据
    std::vector<map<string, string>>itemVec;
    int btnTag;
    std::vector<Layout*> listBtn;
    Json::Value smallData;
    
    /**********************  1、选中按钮的选项  *******************************/
    char buf1[128];
    
    for (int i = 0; i < butMap.size(); i++)
    {
        listBtn = butMap[i];
        for (int j = 0; j < listBtn.size(); j++)
        {
            Layout *curBtn = listBtn.at(j);
            btnTag = curBtn->getTag();
            ImageView *seleTemp = (ImageView*)curBtn->getChildByName("selecIcon");
            smallData = setData[baoIndex]["gameCustomClassifyVOList"][btnTag/100]["gameCustomPlayValInfoList"][btnTag%100];
            if (seleTemp->isVisible())
            {
                std::map<string, string>itemMap;
                
                int baoId = setData[baoIndex]["gameCustomClassifyVOList"][btnTag/100]["pkgId"].asInt();
                sprintf(buf1, "%d", baoId);
                itemMap["pkgId"] = buf1;
                
                int playId = smallData["orderPlayId"].asInt();
                sprintf(buf1, "%d", playId);
                itemMap["orderPlayId"] = buf1;
                
                int classId = smallData["classifyId"].asInt();
                sprintf(buf1, "%d", classId);
                itemMap["classifyId"] = buf1;
                
                int ipValue = smallData["iptValue"].asInt();
                sprintf(buf1, "%d", ipValue);
                itemMap["iptValue"] = buf1;
                
                bool visible = smallData["visible"].asBool();
                if (visible)
                {
                    itemMap["isVisible"] = "true";
                }else
                {
                    itemMap["isVisible"] = "false";
                }
                
                itemVec.push_back(itemMap);
                
            }
        }
    }
    
    /**********************  2、输入框类型的值的提取选项  *******************************/
    for (int i = 0; i < editBoxList.size(); i++)
    {
        std::map<string, string>itemMap;
        
        EditBox *box = editBoxList[i];
        btnTag = box->getTag();
        smallData = setData[baoIndex]["gameCustomClassifyVOList"][btnTag/100]["gameCustomPlayValInfoList"][btnTag%100];
        
        int baoId = setData[baoIndex]["gameCustomClassifyVOList"][btnTag/100]["pkgId"].asInt();
        sprintf(buf1, "%d", baoId);
        itemMap["pkgId"] = buf1;
        
        int playId = smallData["orderPlayId"].asInt();
        sprintf(buf1, "%d", playId);
        itemMap["orderPlayId"] = buf1;
        
        int classId = smallData["classifyId"].asInt();
        sprintf(buf1, "%d", classId);
        itemMap["classifyId"] = buf1;
        
        int ipValue = smallData["iptValue"].asInt();
        sprintf(buf1, "%d", ipValue);
        itemMap["iptValue"] = buf1;
        
        bool visible = smallData["visible"].asBool();
        if (visible)
        {
            itemMap["isVisible"] = "true";
        }else
        {
            itemMap["isVisible"] = "false";
        }
        
        itemVec.push_back(itemMap);
    }
    
    /**********************  3、下拉框类型的值的提取选项  *******************************/
    std::map<int, Layout*>::iterator itDown;
    
    for (itDown = downCell.begin(); itDown != downCell.end(); itDown++)
    {
        int key = itDown->first;
        smallData = setData[baoIndex]["gameCustomClassifyVOList"][key/100]["gameCustomPlayValInfoList"];
        
        for (int j = 0; j < smallData.size(); j++)
        {
            if (!smallData[j]["iptValue"].isNull())
            {
                std::map<string, string>itemMap;
                int baoId = setData[baoIndex]["gameCustomClassifyVOList"][key/100]["pkgId"].asInt();
                sprintf(buf1, "%d", baoId);
                itemMap["pkgId"] = buf1;
                
                
                int playId = smallData[j]["orderPlayId"].asInt();
                sprintf(buf1, "%d", playId);
                itemMap["orderPlayId"] = buf1;
                
                int classId = smallData[j]["classifyId"].asInt();
                sprintf(buf1, "%d", classId);
                itemMap["classifyId"] = buf1;
                
                int ipValue = smallData[j]["iptValue"].asInt();
                sprintf(buf1, "%d", ipValue);
                itemMap["iptValue"] = buf1;
                
                bool visible = smallData[j]["visible"].asBool();
                if (visible)
                {
                    itemMap["isVisible"] = "true";
                }else
                {
                    itemMap["isVisible"] = "false";
                }
                
                itemVec.push_back(itemMap);
            }
        }
    }
    
    for (int i = 0; i < hideList.size(); i++)
    {
        for (int j = 0; j< hideList[i]["gameCustomPlayValInfoList"].size(); j++)
        {
            char buf1[128];
            string t = "";
            smallData = hideList[i]["gameCustomPlayValInfoList"][j];
            
            std::map<string, string>itemMap;
            bool isPost = false;
            if ((hideList[i]["choiceType"].asInt() == 4) && (smallData["iptValue"] != Json::Value::null))
            {
                isPost = true;
            }else
            {
                if (smallData["iptValue"] != Json::Value::null)
                {
                    isPost = true;
                }
            }
            
            if (isPost)
            {
                int baoId = hideList[i]["pkgId"].asInt();
                sprintf(buf1, "%d", baoId);
                itemMap["pkgId"] = buf1;
                
                int playId = smallData["orderPlayId"].asInt();
                sprintf(buf1, "%d", playId);
                itemMap["orderPlayId"] = buf1;
                
                int classId = smallData["classifyId"].asInt();
                sprintf(buf1, "%d", classId);
                itemMap["classifyId"] = buf1;
                
                int ipValue = smallData["iptValue"].asInt();
                sprintf(buf1, "%d", ipValue);
                itemMap["iptValue"] = buf1;
                
                bool visible = smallData["visible"].asBool();
                if (visible)
                {
                    itemMap["isVisible"] = "true";
                }else
                {
                    itemMap["isVisible"] = "false";
                }
                itemVec.push_back(itemMap);
            }
            
        }
    }
    
    std::map<string, string>itemMapT;
    std::vector<string> selectStrList;
    for (int i = 0; i < itemVec.size(); i++)
    {
        itemMapT = itemVec[i];
        string itemStr = "{";
        map<std::string, string>::iterator it;
        int k = 0;
        for (it = itemMapT.begin(); it != itemMapT.end(); it++, k++)
        {
            string key = it->first;
            string value = it->second;
            
            if (k == itemMapT.size()-1)
            {
                itemStr += key + ":" + value;
            }else
            {
                itemStr += key + ":" + value +",";
            }
        }
        itemStr += "}";
        selectStrList.push_back(itemStr);
        
    }
    
    if (selectStrList.size() <= 0)
    {
        playSetStr = "[]";
        return;
    }else
    {
        playSetStr = "[";
    }
    
    for (int k = 0; k < selectStrList.size(); k++)
    {
        if (k < selectStrList.size() -1)
        {
            playSetStr += selectStrList.at(k) + ",";
        }else if (k == selectStrList.size() -1)
        {
            playSetStr += selectStrList.at(k) + "]";
        }
    }
    
    
    
}

void CreateRoom::selectBtnOn(Ref*ref)
{
    Layout *btn = (Layout*)ref;
    ImageView *selecIcon = (ImageView*)btn->getChildByName("selecIcon");
    
    int tag = btn->getTag();
    
    //上一次点击的是一样选项直接跳出
    if (baoIndex == baoL &&  tag/100 == classL && smallL == tag%100 && setData[baoIndex]["gameCustomClassifyVOList"][tag/100]["choiceType"].asInt() == 1)
    {
        return;
    }
    
    if (setData[baoIndex]["gameCustomClassifyVOList"][tag/100]["choiceType"].asInt() == 1)
    {
        //单选
        selecIcon->setVisible(true);
        setData[baoIndex]["gameCustomClassifyVOList"][tag/100]["gameCustomPlayValInfoList"][tag%100]["iptValue"] = setData[baoIndex]["gameCustomClassifyVOList"][tag/100]["gameCustomPlayValInfoList"][tag%100]["playId"];
        
        std::vector<Layout *> btnList = butMap[tag/100];
        for (int i = 0; i < btnList.size(); i++)
        {
            Layout *btnTemp = btnList[i];
            if (btnTemp == btn)
            {
                continue;
            }
            ImageView *seleTemp = (ImageView*)btnTemp->getChildByName("selecIcon");
            seleTemp->setVisible(false);
            int selTag = btnTemp->getTag();
            
            setData[baoIndex]["gameCustomClassifyVOList"][selTag/100]["gameCustomPlayValInfoList"][selTag%100]["iptValue"] =  Json::Value::null;
        }
        
        
    }else if (setData[baoIndex]["gameCustomClassifyVOList"][tag/100]["choiceType"].asInt() == 2)
    {
        bool isSele = !(selecIcon->isVisible());
        //多选
        selecIcon->setVisible(isSele);
        if (isSele)
        {
            setData[baoIndex]["gameCustomClassifyVOList"][tag/100]["gameCustomPlayValInfoList"][tag%100]["iptValue"] = setData[baoIndex]["gameCustomClassifyVOList"][tag/100]["gameCustomPlayValInfoList"][tag%100]["playId"];
            
        }else
        {
            setData[baoIndex]["gameCustomClassifyVOList"][tag/100]["gameCustomPlayValInfoList"][tag%100]["iptValue"] = Json::Value::null;
        }
    }
    
    //初始高度位置
    lastOffY = listBg->getInnerContainer()->getContentSize().height - listBg->getContentSize().height - fabs(listBg->getInnerContainerPosition().y);
    
    //全部做刷新
    showSetViewData();
}

Layout *CreateRoom::juShuBtnDraw(bool selected, int index)
{
    Layout *btn = Layout::create();
    btn->setContentSize(Size(550/2, 70));
    
    RichText *jushu = RichText::create();
    jushu->setAnchorPoint(Vec2(0, 0.5));
    jushu->ignoreContentAdaptWithSize(true);
    jushu->setContentSize(Size(225, 70));
    
    __String *bufStr = __String::createWithFormat("%d局",  timeArray[index]["round"].asInt());
    RichElementText* re1 = RichElementText::create(1, Color3B(80, 0, 0), 255, bufStr->getCString(), "", 43);
    
    bufStr = __String::createWithFormat("(%d房卡)", timeArray[index]["card"].asInt());
    RichElementText* re2 = RichElementText::create(1, Color3B(80, 0, 0), 255, bufStr->getCString(), "", 38);
    
    jushu->pushBackElement(re1);
    jushu->pushBackElement(re2);
    jushu->setPosition(Vec2(70, 35));
    btn->addChild(jushu);
    
    ImageView *kongImg = ImageView::create();
    kongImg->setAnchorPoint(Vec2(0, 0.5));
    kongImg->setPosition(Vec2(0, btn->getContentSize().height/2));
    btn->addChild(kongImg);
    
    ImageView *selelctImg = ImageView::create();
    selelctImg->setName("selecIcon");
    selelctImg->setAnchorPoint(Vec2(0, 0.5));
//    selelctImg->setPosition(kongImg->getPosition());
    btn->addChild(selelctImg, 2);
    selelctImg->setVisible(selected);
    
    kongImg->loadTexture("GameHall/createRoom/point_hold_kong2.png");
    selelctImg->loadTexture("GameHall/createRoom/play_point_check_sigle_can.png");
    selelctImg->setPosition(kongImg->getPosition() + Vec2(-7,2));
    return  btn;
    
}
Layout *CreateRoom::selectBtnDraw(string titleStr, bool selected, int cellType)
{
    Layout *btn = Layout::create();
    btn->setContentSize(Size((550-50)/2, 90));
    
    Text *title = Text::create(titleStr, "", 43);
    title->setContentSize(Size(225-25, 90));
    title->setTextColor(Color4B(80, 0, 0, 255));
    title->setAnchorPoint(Vec2(0, 0.5));
    title->setPosition(Vec2(70, 45));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    btn->addChild(title);
    
    ImageView *kongImg = ImageView::create();
    kongImg->setAnchorPoint(Vec2(0, 0.5));
    kongImg->setPosition(Vec2(0, 45));
    btn->addChild(kongImg);
    
    ImageView *selelctImg = ImageView::create();
    selelctImg->setName("selecIcon");
    selelctImg->setAnchorPoint(Vec2(0, 0.5));
    selelctImg->setPosition(kongImg->getPosition());
    btn->addChild(selelctImg, 2);
    selelctImg->setVisible(selected);
    
    if (cellType == 1)
    {
        kongImg->loadTexture("GameHall/createRoom/point_hold_kong2.png");
        selelctImg->loadTexture("GameHall/createRoom/play_point_check_sigle_can.png");
        selelctImg->setPosition(kongImg->getPosition() + Vec2(-7,2));
        
    }else if (cellType == 2)
    {
        kongImg->loadTexture("GameHall/createRoom/point_hold_kong1.png");
        selelctImg->loadTexture("GameHall/createRoom/play_point_check_can.png");
        selelctImg->setPosition(kongImg->getPosition() + Vec2(-5,0));
    }
    return  btn;
}
void CreateRoom::type3Down(Layout *cell, Json::Value json, int tag)
{
    //下拉
    Size sizeM = Size(330, 75);
    ImageView *downBg = ImageView::create("Match/set_text_Bg.png");
    downBg->setScale9Enabled(true);
    downBg->setTag(150);
    downBg->setContentSize(sizeM);
    downBg->setAnchorPoint(Vec2(0, 0.5));
    downBg->setPosition(Vec2(180, cell->getContentSize().height/2));
    cell->addChild(downBg);
    
    Text *showText = Text::create("", "", 45);
    showText->setTextColor(Color4B(0x4c, 0x4c, 0x4c, 255));
    showText->setTag(250);
    showText->setPosition(Vec2((sizeM.width - 70)/2, sizeM.height/2));
    showText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    downBg->addChild(showText, 100);
    
    showText->setString(json["playName"].asString());
    
    isDownShow[tag] = false;
    CallFunc* func1 = CallFunc::create([=]
                                       {
                                           Size size = Size(330, 207);
                                           
                                           ImageView *downBg = ImageView::create("Match/set_down_listBg.png");
                                           downBg->setScale9Enabled(true);
                                           downBg->setTag(tag);
                                           downBg->setSwallowTouches(true);
                                           downBg->setContentSize(size);
                                           downBg->setAnchorPoint(Vec2(0, 1));
                                           downBg->setPosition(Vec2(cell->getWorldPosition().x+150, cell->getWorldPosition().y - 80));
                                           
                                           this->addChild(downBg, 20);
                                           downBg->setVisible(false);
                                           
                                           ListView *list_1 = ListView::create();
                                           list_1->setBounceEnabled(true);
                                           list_1->setScrollBarEnabled(false);
                                           list_1->setDirection(ScrollView::Direction::VERTICAL);
                                           list_1->setContentSize(Size(size.width - 10, size.height - 10));
                                           list_1->setPosition(Vec2(5, 5));
                                           list_1->setItemsMargin(0);
                                           list_1->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
                                           downBg->addChild(list_1);
                                           
                                           for (int i = 0; i < setData[baoIndex]["gameCustomClassifyVOList"][tag/100]["gameCustomPlayValInfoList"].size(); i++)
                                           {
                                               auto cellTemp = Layout::create();
                                               cellTemp->setTag(tag+i);
                                               cellTemp->setTouchEnabled(true);
                                               cellTemp->addClickEventListener(CC_CALLBACK_1(CreateRoom::onClickDownItem, this));
                                               cellTemp->setContentSize(Size(size.width, 70));
                                               list_1->addChild(cellTemp);
                                               
                                               auto line1 = Layout::create();
                                               line1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                                               line1->setBackGroundColor(Color3B(0xb3, 0xb3, 0xb3));
                                               line1->setPosition(Vec2(10, 0));
                                               line1->setContentSize(Size(size.width - 20, 2));
                                               cellTemp->addChild(line1);
                                               
                                               Text *showText = Text::create(setData[baoIndex]["gameCustomClassifyVOList"][tag/100]["gameCustomPlayValInfoList"][i]["playName"].asString(), "", 35);
                                               showText->setTextColor(Color4B(0x4c, 0x4c, 0x4c, 255));
                                               showText->setAnchorPoint(Vec2(0, 0));
                                               showText->setPosition(Vec2(15, 10));
                                               showText->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
                                               cellTemp->addChild(showText, 2);
                                           }
                                           
                                       });
    
    runAction(Sequence::create(DelayTime::create(0.0),func1, NULL));
    
    Button *downBtn = Button::create("Match/set_down_btn.png", "Match/set_down_btn.png", "Match/set_down_btn.png");
    downBtn->setAnchorPoint(Vec2(1, 0));
    downBtn->setPosition(Vec2(sizeM.width, 0));
    downBtn->setTag(tag);
    downBtn->addClickEventListener(CC_CALLBACK_1(CreateRoom::showDownListView, this));
    downBg->addChild(downBtn);
    
    downCell[tag] = cell;
    
}
void CreateRoom::onClickDownItem(Ref*ref)
{
    Layout *item = (Layout *)ref;
    int tag = item->getTag();
    Layout *cellTemp = downCell[tag/100*100];
    ImageView *bg = (ImageView *)cellTemp->getChildByTag(150);
    Text *showText = (Text *)bg->getChildByTag(250);
    
    showText->setString(setData[baoIndex]["gameCustomClassifyVOList"][tag/100]["gameCustomPlayValInfoList"][tag%100]["playName"].asString());
    
    for (int j = 0; j < setData[baoIndex]["gameCustomClassifyVOList"][tag/100]["gameCustomPlayValInfoList"].size(); j++)
    {
        setData[baoIndex]["gameCustomClassifyVOList"][tag/100]["gameCustomPlayValInfoList"][j]["iptValue"] = Json::Value::null;
    }
    
    setData[baoIndex]["gameCustomClassifyVOList"][tag/100]["gameCustomPlayValInfoList"][tag%100]["iptValue"] = setData[baoIndex]["gameCustomClassifyVOList"][tag/100]["gameCustomPlayValInfoList"][tag%100]["playId"];
    
    std::map<int, bool>::iterator it;
    
    for (it = isDownShow.begin(); it != isDownShow.end(); it++)
    {
        int key = it->first;
        ImageView *listBgT = (ImageView *)this->getChildByTag(key);
        listBgT->removeFromParent();
    }
    char buf[128];
    sprintf(buf, "%d", tag/100*100);
    Layout *lay = (Layout*)this->getChildByName(buf);
    lay->removeFromParent();
    
    lastOffY = listBg->getInnerContainer()->getContentSize().height - listBg->getContentSize().height - fabs(listBg->getInnerContainerPosition().y);
    
    showSetViewData();
}

void CreateRoom::closeSelectView(Ref*ref)
{
    Layout *lay = (Layout*)ref;
    int downTag = atoi(lay->getName().c_str());
    ImageView *listBg = (ImageView *)this->getChildByTag(downTag);
    listBg->setVisible(false);
    isDownShow[downTag] = false;
    lay->removeFromParent();
    
}
void CreateRoom::showDownListView(Ref*ref)
{
    Button *downBtn = (Button *)ref;
    int downTag = downBtn->getTag();
    isDownShow[downTag] = !isDownShow[downTag];
    bool isShow = isDownShow[downTag];
    ImageView *downBg = (ImageView *)this->getChildByTag(downTag);
    if (isShow)
    {
        Layout *cell = downCell[downTag];
        
        downBg->setAnchorPoint(Vec2(0, 1));
        downBg->setPosition(Vec2(cell->getWorldPosition().x+150, cell->getWorldPosition().y - 80));
        downBg->setVisible(true);
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Layout *layout = Layout::create();
        
        __String *bufStr = __String::createWithFormat("%d", downTag);
        layout->setName(bufStr->getCString());
        layout->setTouchEnabled(true);
        layout->addClickEventListener(CC_CALLBACK_1(CreateRoom::closeSelectView, this));
        layout->setContentSize(visibleSize);
        this->addChild(layout, 15);
    }else
    {
        downBg->setVisible(false);
    }
}
void CreateRoom::type4Edit(Layout *cell, Json::Value json, int tag)
{
    EditBox* edit = EditBox::create(Size(250, 80), "Match/set_text_Bg.png");
    edit->setPlaceholderFontSize(45);
    edit->setAnchorPoint(Vec2(0, 0.5));
    edit->setMaxLength(11);
    edit->setInputMode(EditBox::InputMode::NUMERIC);
    edit->setPosition(Vec2(190, cell->getContentSize().height/2));
    edit->setFontColor(Color3B(0x99, 0x99, 0x99));
    edit->setPlaceholderFontColor(Color3B(0x71, 0x64, 0x4d));
    edit->setFontSize(45);
    edit->setDelegate(this);
    edit->setTag(tag);
    cell->addChild(edit, 2);
    
    editBoxList.push_back(edit);
    
    if (json["iptValue"] == Json::Value::null)
    {
        json["iptValue"] = json["iptLower"];
        setData[baoIndex]["gameCustomClassifyVOList"][tag/100]["gameCustomPlayValInfoList"][tag%100]["iptValue"] = json["iptLower"];
    }
    __String *bufStr = __String::createWithFormat("%d", json["iptValue"].asInt());
    
    edit->setText(bufStr->getCString());
    
    bufStr = __String::createWithFormat("输入范围：%d~%d数字", json["iptLower"].asInt(), json["iptUpper"].asInt());
    
    Text *fanWei = Text::create(bufStr->getCString(), "", 38);
    fanWei->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
    fanWei->setAnchorPoint(Vec2(0, 0.5));
    fanWei->setPosition(Vec2(460, cell->getContentSize().height/2));
    fanWei->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    cell->addChild(fanWei, 2);
}

void CreateRoom::editBoxReturn(EditBox* editBox)
{
    
}

void CreateRoom::editBoxEditingDidBegin(EditBox* editBox)
{
    editBox->setText("");
}

void CreateRoom::editBoxEditingDidEnd(EditBox* editBox)
{
    int editTag = editBox->getTag();
    string valueStr = editBox->getText();
    int value = atoi(valueStr.c_str());
    Json::Value tempJson = setData[baoIndex]["gameCustomClassifyVOList"][editTag/100]["gameCustomPlayValInfoList"][editTag%100];
    
    if(value >= tempJson["iptLower"].asInt() && value <= tempJson["iptUpper"].asInt())
    {
        setData[baoIndex]["gameCustomClassifyVOList"][editTag/100]["gameCustomPlayValInfoList"][editTag%100]["iptValue"] = value;
    }else
    {
        char buf[128];
        sprintf(buf, "%d", tempJson["iptValue"].asInt());
        editBox->setText(buf);
        
        sprintf(buf, "请输入范围：%d~%d的数字", tempJson["iptLower"].asInt(), tempJson["iptUpper"].asInt());
        PlatformHelper::showToast(buf);
        
    }
    
}


void CreateRoom::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    
}

void CreateRoom::sureOpen(Ref *pSender)
{
    Button *createBtn = (Button*)pSender;
    //避免点击次数过多
    createBtn->setEnabled(false);
    Loading *loading = Loading::startLoading(Director::getInstance()->getRunningScene(), Director::getInstance()->getWinSize()/2);
    loading->maskSwllowTouch();
    
    getSureSetString();
    Json::Value json1;
    
    json1["innerPlayInfo"] = playSetStr;
    json1["playId"] = bottomJson["playId"];
    json1["roomName"] = "";
    json1["gameId"] =  gameConfig["gameId"];
    //json1["number"] = gameConfig["joinNum"]; //暂时不传人数
    
    json1["pkgId"] = setData[baoIndex]["id"];
    
    
    if (gameConfig["timeWay"].asInt() == 0)
    {
        //时间
        json1["duration"] = time;
        json1["count"] = 0;
        
    }else if (gameConfig["timeWay"].asInt() == 1)
    {
        //时局
        json1["count"] = time;
        json1["duration"] = 0;
    }
    json1["bottom"] = bottomJson["base"];
    json1["takeIn"] = bottomJson["bottom"];
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        loading->removeFromParent();
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            createBtn->setEnabled(true);
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data1 = loginPacket->recvVal["resultMap"];
            
            Json::Value json;
            json["code"] = data1["code"];
            json["userJoin"] = 0;
            
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
                    ZJHModel::getInstance()->gameHelpUrl = CCHttpAgent::getInstance()->url+data["gameRoom"]["gameHelpUrl"].asString();
                    
                    __NotificationCenter::getInstance()->postNotification("money_change");
                    
                    ZJHModel::getInstance()->roomData = data;
                    Director::getInstance()->replaceScene(LoadTable::createScene());
                    
                    //                        this->setPreByName("OpenMatchLayer");
                    //                        this->disappear();
                    
                    
                }else
                {
                    PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                }
            },"duiju/view",json.toStyledString(),"duijuCode");
            
        }else if(loginPacket->recvVal["status"].asInt() == 10002)
        {
            createBtn->setEnabled(true);
            
            //玩贝不足
            PlatformHelper::showToast("房卡不足,请联系您的代理");
        }else if(loginPacket->recvVal["status"].asInt() == 10001)
        {
            createBtn->setEnabled(true);
            //金币不足
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            
            TipShopLayer *tip = TipShopLayer::create();
            tip->setPosition(Vec2::ZERO);
            tip->buyCardGoldWanRechange(3);
            this->addChild(tip);
            
        }else if (loginPacket->recvVal["status"].asInt() == 20400){
            createBtn->setEnabled(true);
            
            //不能创建的提示
            showCannotCreateTip(loginPacket->recvVal["message"].asCString());
            
        }else if(loginPacket->recvVal["status"].asInt() == 999)
        {
            //tomcat没有连上
            string content = loginPacket->recvVal["message"].asString();
            ServerErrorTips* tips = ServerErrorTips::create();
            tips->setContent(content);
            getParent()->addChild(tips,1000);
        }else{
            createBtn->setEnabled(true);
            
            if(loginPacket->recvVal["message"].isString() && !loginPacket->recvVal["message"].isNull())
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
        }
    },"duiju/save_v2",json1.toStyledString(),"openMatch");
    
    setConfigCacheByGameId();
}

void CreateRoom::showCannotCreateTip(string tipString)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout *notTip = Layout::create();
    notTip->setTouchEnabled(true);
    notTip->setSwallowTouches(true);
    notTip->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    notTip->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    notTip->setBackGroundColorOpacity(150);
    notTip->setContentSize(visibleSize);
    notTip->addClickEventListener([=](Ref*ref){
        
        notTip->removeFromParentAndCleanup(true);
        
    });
    this->addChild(notTip, 100);
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    notTip->addChild(bg);
    
    LabelTTF *title2 = LabelTTF::create(tipString, "", 45 ,Size(visibleSize.height - 250 - 100, 0), TextHAlignment::LEFT);
    title2->setAnchorPoint(Vec2(0.5, 1));
    title2->setColor(Color3B(0xd9, 0xd9, 0xd9));
    
    bg->setContentSize(Size(visibleSize.height - 250, 300 + title2->getContentSize().height));
    title2->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 120));
    bg->addChild(title2);
    
    Text *title = Text::create("提示", "", 50);
    title->setTextColor(Color4B(0xf0, 0xcb, 0x5c, 255));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 65));
    bg->addChild(title);
    
    Button *cancel = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    cancel->setPosition(Vec2(bg->getContentSize().width-15, bg->getContentSize().height-15));
    cancel->addClickEventListener([=](Ref*ref){
        
        notTip->removeFromParentAndCleanup(true);
        
    });
    bg->addChild(cancel);
}

void CreateRoom::setConfigCacheByGameId(){
    //保存数据缓存
    int gameId = gameConfig["gameId"].asInt();
    stringstream stream;
    stream<<gameId;
    string cacheStr = UserDefault::getInstance()->getStringForKey(stream.str().c_str());
    Json::Value gameConfigCache = Utils::ParseJsonStr(cacheStr);
    //1.设置包名位置缓存
    gameConfigCache["baoIndex"] = baoIndex;
    //2.设置局数缓存
    gameConfigCache["countLastIndex"] = countLastIndex;
    //3.设置高级设置缓存
    gameConfigCache["setData"] = setData;
    //4.设置数据md5(所有包名+所有局数+所有高级设置) 用于对比服务器版本
    //    string md5Pre = timeArray.toStyledString() + serverData.toStyledString();
    //    std::string sign;
    //    MD5::MD5Str(md5Pre, sign);
    gameConfigCache["configMd5"] = serverMd5;
    //    std::cout<<"=======cacheStr======"<<gameConfigCache["configMd5"].toStyledString()<<endl;
    UserDefault::getInstance()->setStringForKey(stream.str().c_str(), gameConfigCache.toStyledString());
}

Json::Value CreateRoom::getConfigCacheByGameId(){
    int gameId = gameConfig["gameId"].asInt();
    stringstream stream;
    stream<<gameId;
    string cacheStr = UserDefault::getInstance()->getStringForKey(stream.str().c_str());
    Json::Value json = Utils::ParseJsonStr(cacheStr);
    return json;
}

void CreateRoom::clearConfigCache(){
    int gameId = gameConfig["gameId"].asInt();
    stringstream stream;
    stream<<gameId;
    UserDefault::getInstance()->setStringForKey(stream.str().c_str(),"");
    baoIndex = 0;
    countLastIndex = 0;
}

void CreateRoom::clickBottom(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        ImageView *bottomIcon = (ImageView*)pSender;
        Layout*layout =(Layout*)bottomIcon->getParent();
        //选中的底注
        int index = bottomIcon->getTag()-400;
        char baseBuf[10];
        sprintf(baseBuf, "%d", bottomArray[index]["base"].asInt());
        base->setString(baseBuf);
        
        char bottomBuf[10];
        sprintf(bottomBuf, "%d", bottomArray[index]["bottom"].asInt());
        bottom->setString(bottomBuf);
        
        bottomSelect->removeFromParent();
        
        bottomSelect= ImageView::create("Match/check_point.png");
        bottomSelect->setPosition(bottomIcon->getPosition());
        layout->addChild(bottomSelect);
        
        
        bottomJson = bottomArray[index];
        
        //如果为斗地主
        if(gameConfig["gameId"].asInt() == 2101)
        {
            lastOffY = listBg->getInnerContainer()->getContentSize().height - listBg->getContentSize().height - fabs(listBg->getInnerContainerPosition().y);
            showSetViewData();
        }
    }
}

void CreateRoom::clickTime(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        
        //        if(layInfo != NULL)
        //        {
        //            layInfo->setVisible(false);
        //        }
        //
        Layout *timeIcon = (Layout*)pSender;
        //选中的时间值
        int timeIndex = timeIcon->getTag()-300;
        
        if (timeIndex == countLastIndex)
        {
            return;
        }
        
        if (gameConfig["timeWay"].asInt() == 0)
        {
            time = timeArray[timeIndex]["time"].asInt();
        }else if (gameConfig["timeWay"].asInt() == 1)
        {
            time = timeArray[timeIndex]["round"].asInt();
        }
        
        countLastIndex  = timeIndex;
        
        countNewView();
        
    }
}

void CreateRoom::onExit()
{
    Layer::onExit();
    CCHttpAgent::getInstance()->callbacks.erase("createSet");
    CCHttpAgent::getInstance()->callbacks.erase("postGameData");
}
