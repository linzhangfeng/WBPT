
//
//  TaskLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/6.
//


#include "CreateRoomHelp.hpp"
#include "../Loading/Loading.h"

bool CreateRoomHelp::init()
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
    
//    auto layer = LayerColor::create(Color4B(0, 0, 0, 100));
//    layer->setContentSize(size);
//    layer->setAnchorPoint(Vec2(0.0f, 0.0f));
//    layer->setPosition(0,0);
//    addChild(layer);
    
    bg = Sprite::create("GameHall/createRoom/bg3.png");
    bg->setPosition(size / 2);
    addChild(bg);
    
    Size csize = bg->getContentSize();
    
    auto closeBtn = Button::create("GameHall/createRoom/c1.png", "GameHall/createRoom/c2.png");
    closeBtn->setPosition(Vec2(csize.width - 50, csize.height - 50));
    closeBtn->addClickEventListener([=](Ref*){
        this->removeFromParent();
    });
    bg->addChild(closeBtn,10);
    
    baoIndex = 0;
    Json::Value json;
    json["gameId"] = ZJHModel::getInstance()->curGameID;
    showView(json);
    return true;
}

void CreateRoomHelp::showView(Json::Value json)
{
    loading1 = Loading::startLoading(this, Director::getInstance()->getWinSize() / 2);
//    loading1->maskSwllowTouch();
    postGameData(json);
}

void CreateRoomHelp::postGameData(Json::Value jsonDate)
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
            
            setData = loginPacket->recvVal["resultMap"]["innerWay"];
            createView();
            
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            this->removeFromParent();
        }
    }, "gameClasli/recommendConfig_v2", jsonDate.toStyledString(), "createRoomHelp_getData");
}

void CreateRoomHelp::createView()
{
//    bg->removeFromParent();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    int midX = 0;
    
    webView = cocos2d::experimental::ui::WebView::create();
//    webView->setColor(Color4B(255,255,255,0));
    webView->setAnchorPoint(Vec2(0, 0));
    webView->setContentSize(Size(1465 - 50, 719));
    webView->setScalesPageToFit(true);
    
    //包名显示
    if (setData.size() > 1 || true){
//        bg = Sprite::create("GameHall/createRoom/bg3.png");
//        addChild(bg);
        webView->setPosition(Vec2(390 + 25, 45));
//        bg->setPosition(visibleSize / 2);
        setBaoView();
        midX = 1093;
    }
    else{
        bg = Sprite::create("GameHall/createRoom/bg4.png");
        addChild(bg);
        webView->setPosition(Vec2(35 + 25, 55));
        bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        midX = bg->getContentSize().width / 2;
    }
    
    bg->addChild(webView);
    
    Size csize = bg->getContentSize();
    
//    auto closeBtn = Button::create("GameHall/createRoom/c1.png", "GameHall/createRoom/c2.png");
//    closeBtn->setPosition(Vec2(csize.width - 50, csize.height - 50));
//    closeBtn->addClickEventListener([=](Ref*){
//        this->removeFromParent();
//    });
//    bg->addChild(closeBtn,10);
    
    auto title = Sprite::create("GameHall/createRoom/help.png");
    title->setPosition(Vec2(midX,csize.height - 108));
    bg->addChild(title);
    
    auto createTitle = Sprite::create("GameHall/createRoom/line.png");
    createTitle->setPosition(Vec2(midX,csize.height - 179));
    bg->addChild(createTitle);
    
    string url = CCHttpAgent::getInstance()->url+setData[baoIndex]["gameHelpUrl"].asString();
    webView->loadURL(url);
}

void CreateRoomHelp::setBaoView()
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
    listView->setPosition(Vec2(0, 45));
    listView->setScrollBarEnabled(false);
    listView->setName("list");
    bg->addChild(listView);
    
    listView->setItemsMargin(10);
    
    //包名的tag取值：100+
    for (int i = 0; i < nums; i++)
    {
        auto layout = Layout::create();
        layout->setContentSize(item_size);
        listView->addChild(layout);
        
        Button* baoBtn = Button::create("GameHall/createRoom/title1.png","GameHall/createRoom/title1.png", "GameHall/createRoom/title2.png");
        baoBtn->setTag(100+i);
        baoBtn->addClickEventListener(CC_CALLBACK_1(CreateRoomHelp::onBaoBtnResponse, this));
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

void CreateRoomHelp::onBaoBtnResponse(Ref*ref)
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
    
//    string url = CCHttpAgent::getInstance()->url+setData[baoIndex]["helpUrl"].asString();
    string url = CCHttpAgent::getInstance()->url+setData[baoIndex]["gameHelpUrl"].asString();
    webView->loadURL(url);
    
}

void CreateRoomHelp::onExit()
{
    Layer::onExit();
    CCHttpAgent::getInstance()->callbacks.erase("createRoomHelp_getData");
}
