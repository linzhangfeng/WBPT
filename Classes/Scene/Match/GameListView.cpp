//
//  GameListView.cpp
//  ZJH
//
//  Created by apple on 16/8/17.
//
//

#include "GameListView.hpp"
#include "SelectGame.hpp"
#include "SelectDiqu.hpp"
#include "JiaoCheng.hpp"

#include "../../MyGUI/ImageByUrl.h"
#include "../../MyGUI/MatchEnterIconByUrl.h"

bool GameListView::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    requestData();
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(GameListView::updateViewNotify), "GameListView_updateview_notify", NULL);
    return true;
}

void GameListView::updateViewNotify(Ref* r)
{
    requestData();
}

void GameListView::requestData()
{
    this->schedule(schedule_selector(GameListView::getData), 0);
    if(loading != NULL){
        loading->removeFromParent();
        loading = NULL;
    }
    loading = Loading::startLoading(this, Director::getInstance()->getWinSize() / 2 + Size(0,180));
    
    int diqu_cityId = UserDefault::getInstance()->getIntegerForKey("diqu_cityId", -1);
    std::string diqu_cityName = UserDefault::getInstance()->getStringForKey("diqu_cityName", "");
    
    Json::Value json;
    json["cityId"] = diqu_cityId;
    
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"gameClasli/ciytList",json.toStyledString(),"GameListview_listdata");
}

void GameListView::getData(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["GameListview_listdata"];
    if (loginPacket && loginPacket->status != 0)
    {
        if (loginPacket->status != 3)
        {
            CCHttpAgent::getInstance()->packets["GameListview_listdata"] = NULL;
            if(loading != NULL){
                loading->removeFromParent();
                loading = NULL;
            }
            showLoadErrorDialog();
            return;
        }
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"];
            showListView(data);
            ZJHModel::getInstance()->diquGameList = data;
            
            //int newGuidTag = UserDefault::getInstance()->getIntegerForKey("jiaocheng_ok",0);
            int newGuidTag = UserDefault::getInstance()->getIntegerForKey("jiaocheng_second",0);
            if (newGuidTag == 0)
            {
                Director::getInstance()->getRunningScene()->removeChildByName("JiaoCheng");
                Director::getInstance()->getRunningScene()->addChild(JiaoCheng::create());
            }
        }
        this->unschedule(schedule_selector(GameListView::getData));
        CCHttpAgent::getInstance()->packets["GameListview_listdata"] = NULL;
        if(loading != NULL){
            loading->removeFromParent();
        }
        loading = NULL;
        delete loginPacket;
    }
}

void GameListView::showLoadErrorDialog(){
    Size size = Director::getInstance()->getWinSize();
    Layout *mask = Layout::create();
    mask->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    mask->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    mask->setBackGroundColorOpacity(150);
    mask->setContentSize(size);
    this->addChild(mask);
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(720, 470));
    bg->setPosition(Vec2(size.width/2, size.height/2));
    mask->addChild(bg);
    
    Text *title = Text::create("提示", ".SFUIDisplay-Semibold", 50);
    title->setTextColor(Color4B(0xf0, 0xcb, 0x5c, 255));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 70));
    bg->addChild(title);
    
    Text *title2 = Text::create("请求游戏数据失败,请重试", ".SFUIDisplay-Semibold", 45);
    title2->setTextColor(Color4B(0xd9, 0xd9, 0xd9, 255));
    title2->setAnchorPoint(Vec2(0.5, 0.5));
    title2->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title2->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2));
    bg->addChild(title2);
    
    
    Button *lianBut = Button::create("Mine/btn_yellow_small.png", "Mine/btn_yellow_small_check.png");
    lianBut->setScale9Enabled(true);
    lianBut->setAnchorPoint(Vec2(0, 0));
    lianBut->setContentSize(Size(600 , 90));
    lianBut->setPosition(Vec2(40, 40));
    bg->addChild(lianBut);
    lianBut->addClickEventListener([=](Ref*ref){
        mask->removeFromParent();
        requestData();
    });
    
    Text *titleBtn = Text::create("重试", ".SFUIDisplay-Semibold", 40);
    titleBtn->setTextColor(Color4B(0x60, 0x41, 0x10, 255));
    titleBtn->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleBtn->setPosition(Vec2(lianBut->getContentSize().width/2, lianBut->getContentSize().height/2));
    lianBut->addChild(titleBtn);
}

void GameListView::createDiqu()
{
    removeChildByName("GameListView_diqu");
    
    int diqu_cityId = UserDefault::getInstance()->getIntegerForKey("diqu_cityId", -1);
    std::string diqu_cityName = UserDefault::getInstance()->getStringForKey("diqu_cityName", "");
    if (diqu_cityName == "")
    {
        //第一次没有本地缓存的时候 使用服务器定位到的 如果服务器没有定位到 默认是推荐
        diqu_cityId = configList["cityId"].asInt();
        diqu_cityName = configList["cityName"].asString();
        ZJHModel::getInstance()->setDiquCach(0,diqu_cityName);
    }
    
    Size size = Director::getInstance()->getWinSize();
    
    auto bg = Sprite::create("SelectGame/diqu_bg.png");
    
    Layout* diqu_layout = Layout::create();
    diqu_layout->setContentSize(bg->getContentSize());
    diqu_layout->setAnchorPoint(Vec2(0.0,0.5));
    diqu_layout->setPosition(Vec2(100,size.height - 248));
    diqu_layout->setName("GameListView_diqu");
    diqu_layout->setTouchEnabled(true);
    diqu_layout->addClickEventListener([=](Ref*ref){
        Director::getInstance()->getRunningScene()->removeChildByName("SelectDiqu");
        Director::getInstance()->getRunningScene()->addChild(SelectDiqu::create(),1000);
    });
    
    addChild(diqu_layout,1000);
    
    
    bg->setPosition(diqu_layout->getContentSize() / 2);
    diqu_layout->addChild(bg);
    
    auto bg2 = Sprite::create("SelectGame/diqu.png");
    bg2->setAnchorPoint(Vec2(1.0, 0.5));
    bg2->setPosition(Vec2(bg->getContentSize().width,bg->getContentSize().height / 2 + 3));
    bg->addChild(bg2);
    
    char buf[128];
    Utils::parseName(4, buf, diqu_cityName.c_str());
    Label* nameStr = Label::createWithSystemFont(buf, "Thonburi", 45);
    nameStr->setAnchorPoint(Vec2(0.0, 0.5));
    nameStr->setColor(Color3B(255, 255, 255));
    nameStr->setPosition(Vec2(20, diqu_layout->getContentSize().height / 2));
    diqu_layout->addChild(nameStr);
}

void GameListView::showListView(Json::Value json)
{
    removeChildByName("GameListView_bottomPageTips");
    removeChildByName("GameListView_pageView");
    
    Json::Value duijuList = json["gameList"];
    configList = json;
    
    Size size = Director::getInstance()->getWinSize();

    Size pageSize = Size(1000,900);
    PageView* pageView = PageView::create();
    pageView->setContentSize(pageSize);
    pageView->setAnchorPoint(Vec2(0.5, 1));
    pageView->setPosition(Vec2(size.width / 2,size.height - 284));
    pageView->setUsingCustomScrollThreshold(true);
    pageView->setCustomScrollThreshold(pageSize.width / 5);
    pageView->removeAllPages();
    pageView->setName("GameListView_pageView");
    addChild(pageView);
    
    int numsCount = 0;
    int hangNums = 3;
    int lieNums = 3;
    int pageNums = hangNums * lieNums;
    int dx = pageSize.width / hangNums;
    int dy = (pageSize.height) / lieNums;
    int bx = pageSize.width / hangNums - dx / 2;
    int by = pageSize.height - dy / 2;
    int allPageNums = 0;
    int array_size = duijuList.size();
    
//    bool isShowMatch = ZJHModel::getInstance()->gameConfigJson["list"]["matchInfo"]["matchStatus"].asInt();
    bool isShowMatch = true;//坤哥说写死比赛大厅都能出来  不做服务器判断
    if(isShowMatch){
    
        array_size += 1;
        
    }
        
    for(int i = 0; i <= array_size; i++)
    {
        int rindex = numsCount % pageNums;
        int page = numsCount / pageNums;
        int di = rindex % hangNums;
        int dj = rindex / hangNums;
        auto layout = pageView->getPage(page);
        if (layout == NULL){
            layout = Layout::create();
            pageView->addPage(layout);
            allPageNums ++;
        }
        
        string miniLogo = "";
        string gameName = "";
        int tag = i;
        
        if(!isShowMatch)
        {
        
            if (i == array_size)
            {
                bool isClicked = UserDefault::getInstance()->getBoolForKey("is_more_game_clicked");
                if(isClicked){
                    miniLogo = "Match/all_games.png";
                }else{
                    miniLogo = "Match/all_games2.png";
                }
                
                gameName = "更多游戏";
                tag = -1;
            }
            else
            {
                miniLogo = duijuList[i]["logo"].asString();
                gameName = duijuList[i]["gameName"].asString();
                tag = i;
            }
            
        }else
        {
            //有比赛入口时
            if (i == array_size)
            {
                bool isClicked = UserDefault::getInstance()->getBoolForKey("is_more_game_clicked");
                if(isClicked){
                    miniLogo = "Match/all_games.png";
                }else{
                    miniLogo = "Match/all_games2.png";
                }
                gameName = "更多游戏";
                tag = -1;
            }
            else
            {
                if(duijuList.size() >= pageNums)
                {
                    //大于等于一页游戏时
                    if(i == pageNums-1)
                    {
                        miniLogo = ZJHModel::getInstance()->gameConfigJson["list"]["matchInfo"]["matchLogo"].asString();
                        gameName = ZJHModel::getInstance()->matchEnterName();
                        tag = -2;
                    }else if (i > pageNums-1)
                    {
                        int temp = i;
                        temp -= 1;
                        miniLogo = duijuList[temp]["logo"].asString();
                        gameName = duijuList[temp]["gameName"].asString();
                        tag = temp;
                    }else
                    {
                        miniLogo = duijuList[i]["logo"].asString();
                        gameName = duijuList[i]["gameName"].asString();
                        tag = i;
                    }
                }else{
                    
                    //小于一页游戏时
                    if (i == array_size - 1)
                    {
                        miniLogo = ZJHModel::getInstance()->gameConfigJson["list"]["matchInfo"]["matchLogo"].asString();
                        gameName = ZJHModel::getInstance()->matchEnterName();
                        tag = -2;
                    }else
                    {
                        miniLogo = duijuList[i]["logo"].asString();
                        gameName = duijuList[i]["gameName"].asString();
                        tag = i;
                    }
                }
            }
        }
        
        //添加icon图片 计算icon坐标
        Layout* iconText = Layout::create();
                iconText->setContentSize(Size(dx,dy));
        iconText->setAnchorPoint(Vec2(0.5,0.5));
        iconText->setPosition(Vec2(bx + dx * di, by - dy * dj));
        layout->addChild(iconText);
        
        if (tag == -1)
        {
            ImageView* icon = ImageView::create();
            icon->setTouchEnabled(true);
            icon->setTag(tag);
            icon->ignoreContentAdaptWithSize(false);
            icon->setAnchorPoint(Vec2(0.5,0.5));
            icon->setContentSize(Size(200,200));
            icon->setPosition(Vec2(dx / 2, dy / 2));
            icon->addClickEventListener([=](Ref*ref){
                auto l = (ImageView*)ref;
                int tag = l->getTag();
                if (tag != -1 && tag != -2)
                {
                    int gameid = duijuList[tag]["gameId"].asInt();
                    int hallType = 2;//duijuList[tag]["displayMode"].asInt();
                    ZJHModel::getInstance()->setCurGameID(gameid, hallType == 2 ? 1 : 0);
                    if (hallType == 2){
                        ZJHModel::getInstance()->toGameHall();
                    }else{
                        ZJHModel::getInstance()->roomToHall(false);
                    }
                }
                else if(tag == -1)
                {
                    Director::getInstance()->getRunningScene()->removeChildByName("SelectGame");
                    Director::getInstance()->getRunningScene()->addChild(SelectGame::create(),1000);
                    if(!UserDefault::getInstance()->getBoolForKey("is_more_game_clicked")){
                        UserDefault::getInstance()->setBoolForKey("is_more_game_clicked",true);
                        l->loadTexture("Match/all_games.png");
                    }
                    
                }
            });
            icon->loadTexture(miniLogo);
            iconText->addChild(icon);
            
        }else if (tag == -2)
        {
            MatchEnterIconByUrl* icon = MatchEnterIconByUrl::create();
            icon->setTouchEnabled(true);
            icon->setTag(tag);
            icon->ignoreContentAdaptWithSize(false);
            icon->setAnchorPoint(Vec2(0.5,0.5));
            icon->setContentSize(Size(200,200));
            icon->setPosition(Vec2(dx / 2, dy / 2));
            icon->load(miniLogo);
            
            icon->addClickEventListener([=](Ref*ref){
                
                //进入竞技二打一大厅
                Json::Value param;
                param["view"] = "match_view";
                ZJHModel::getInstance()->recoveryHallParam = param.toStyledString();
                ZJHModel::getInstance()->toMatchHall();
                
            });
            icon->loadTexture(miniLogo);
            iconText->addChild(icon);
        }
        else
        {
            ImageByUrl* icon = ImageByUrl::create();
            icon->load(200, 200, miniLogo);
            icon->image->setTouchEnabled(true);
            icon->image->setTag(tag);
            icon->setPosition(Vec2(dx / 2, dy / 2));
            icon->image->addClickEventListener([=](Ref*ref){
                auto l = (Layout*)ref;
                int tag = l->getTag();
                if (tag != -1 && tag != -2)
                {
                    int gameid = duijuList[tag]["gameId"].asInt();
                    int hallType = 2;//duijuList[tag]["displayMode"].asInt();
                    ZJHModel::getInstance()->setCurGameID(gameid, hallType == 2 ? 1 : 0);
                    if (hallType == 2){
                        ZJHModel::getInstance()->toGameHall();
                    }else{
                        ZJHModel::getInstance()->roomToHall(false);
                    }
                }
                else if(tag == -1)
                {
                    Director::getInstance()->getRunningScene()->removeChildByName("SelectGame");
                    Director::getInstance()->getRunningScene()->addChild(SelectGame::create(),1000);
                }
            });
            iconText->addChild(icon);
        }
        
        char buf[128];
        Utils::parseName(6, buf, gameName.c_str());
        Text* title = Text::create(buf, "", 38);
        title->setColor(Color3B(0xff,0xff, 0xff));
        title->setAnchorPoint(Vec2(0.5, 1));
        title->setPosition(Vec2(iconText->getContentSize().width / 2, dy / 2 - 100 - 10));
        
        
        iconText->addChild(title);
        
        numsCount ++;
    }
    
    Layout* bottomPageTips = Layout::create();
    bottomPageTips->setAnchorPoint(Vec2(0.0,0.0));
    bottomPageTips->setPosition(Vec2(0,0));
    bottomPageTips->setName("GameListView_bottomPageTips");
    addChild(bottomPageTips);
    
    dx = 50;
    by = size.height - 1264 - 10;
    bx = size.width / 2 - (allPageNums - 1) / 2 * dx;
    for (int i = 0; i < allPageNums; i ++){
        Button *bottomIcon = Button::create("Match/bottom_tips.png", "Match/bottom_tips.png", "Match/bottom_tips_press.png");
        bottomIcon->setAnchorPoint(Vec2(0.5,0.5));
        bottomIcon->setTag(i);
        bottomIcon->setPosition(Vec2(bx + dx * i, by));
        bottomPageTips->addChild(bottomIcon,300);
        bottomIcon->addClickEventListener([=](Ref*ref){
//            Vector<Node*> children = bottomPageTips->getChildren();
//            for(Node* child : children)
//            {
//                Button* bt = (Button*)child;
//                bt->setEnabled(true);
//            }
            pageView->scrollToPage(i);
        });
    }
    
    pageView->addEventListener([=](Ref *pSender, PageView::EventType type){
        switch (type)
        {
            case PageView::EventType::TURNING:
            {
                PageView* pageView = dynamic_cast<PageView*>(pSender);
                int curPage = pageView->getCurrentPageIndex();
                
                Vector<Node*> children = bottomPageTips->getChildren();
                for(Node* child : children)
                {
                    Button* bt = (Button*)child;
                    bt->setEnabled(true);
                }
                Button* button = (Button*)bottomPageTips->getChildByTag(curPage);
                button->setEnabled(false);
            }
                break;
                
            default:
                break;
        }
    });
    pageView->scrollToPage(0);
    
    createDiqu();
}

void GameListView::onExit()
{
    Layer::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}


