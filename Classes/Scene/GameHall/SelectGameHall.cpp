//
//  SelectGameHall.cpp
//  ZJH
//  游戏第一次启动的时候 需要玩家选择一个默认的游戏 根据游戏类型不同进入不同的游戏大厅
//  Created by admin on 16/3/6.
//
//
#include "SelectGameHall.hpp"
#include "GameHallScene.hpp"
#include "../Hall/HallScene.hpp"

Scene* SelectGameHall::createScene()
{
    auto scene = Scene::create();
    auto layer = SelectGameHall::create();
    scene->addChild(layer);
    return scene;
}

bool SelectGameHall::init()
{
    if (!Layer::init())
    {
        return false;
    }
    Size size = Director::getInstance()->getWinSize();
    //初始化背景图片
    ImageView* bg = ImageView::create("Match/Open_BG.png");
    bg->ignoreContentAdaptWithSize(false);
    bg->setContentSize(size);
    bg->setAnchorPoint(Vec2(0.5,0.5));
    bg->setPosition(Vec2(size.width/2, size.height/2));
    this->addChild(bg);
    
    loading = Loading::startLoading(this, size / 2);
    
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"gameClasli/gameList","","selectGameList");
    this->schedule(schedule_selector(SelectGameHall::getData), 0);
    return true;
}

void SelectGameHall::getData(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["selectGameList"];
    if (loginPacket && loginPacket->status != 0)
    {
        if (loginPacket->status != 3)
        {
            CCHttpAgent::getInstance()->packets["selectGameList"] = NULL;
            if(loading != NULL){
                loading->removeFromParent();
            }
            showLoadErrorDialog();
            return;
        }

        loginPacket->status = 0;
        
        if (loginPacket->resultIsOK())
        {
            initData(loginPacket->recvVal["resultMap"]);
        }
        this->unschedule(schedule_selector(SelectGameHall::getData));
        CCHttpAgent::getInstance()->packets["selectGameList"] = NULL;
        if(loading != NULL){
            loading->removeFromParent();
        }
        loading = NULL;
        delete loginPacket;
    }
}

void SelectGameHall::showLoadErrorDialog(){
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
        loading = Loading::startLoading(this, size / 2);
        CCHttpAgent::getInstance()->sendHttpPost(NULL,"gameClasli/gameList","","selectGameList");
    });
    
    Text *titleBtn = Text::create("重试", ".SFUIDisplay-Semibold", 40);
    titleBtn->setTextColor(Color4B(0x60, 0x41, 0x10, 255));
    titleBtn->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleBtn->setPosition(Vec2(lianBut->getContentSize().width/2, lianBut->getContentSize().height/2));
    lianBut->addChild(titleBtn);
}

void SelectGameHall::initData(Json::Value data)
{
    Json::Value duijuList = data["classifyList"];
    configList = duijuList;
    
    Size size = Director::getInstance()->getWinSize();

    ImageView* title = ImageView::create("GameHall/select_game_text.png");
    title->setAnchorPoint(Vec2(0.5,1));
    title->setPosition(Vec2(size.width / 2,size.height - 110));
    addChild(title);
    
    Size pageSize = Size(size.width,1300);
    PageView* pageView = PageView::create();
    pageView->setContentSize(pageSize);
    pageView->setAnchorPoint(Vec2(0.5, 0.5));
    pageView->setPosition(size / 2);
    pageView->setUsingCustomScrollThreshold(true);
    pageView->setCustomScrollThreshold(pageSize.width / 5);
    pageView->removeAllPages();
    addChild(pageView);
    
    int numsCount = 0;
    int hangNums = 3;
    int lieNums = 3;
    int pageNums = hangNums * lieNums;
    int dx = pageSize.width / hangNums;
    int dy = (pageSize.height - 200) / lieNums;
    int bx = pageSize.width / hangNums - dx / 2;
    int by = pageSize.height - dy / 2;
    int allPageNums = 0;
    for(int i = 0; i < duijuList.size(); i++)
    {
        for (int j = 0; j < duijuList[i]["gameClasliConfigList"].size(); j++)
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
            
            //添加icon图片 计算icon坐标
            Layout* iconText = Layout::create();
            iconText->setTouchEnabled(true);
            iconText->setTag(i * 100 + j);
            iconText->setContentSize(Size(dx,dy));
            iconText->setAnchorPoint(Vec2(0.5,0.5));
            iconText->setPosition(Vec2(bx + dx * di, by - dy * dj));
            iconText->addClickEventListener([=](Ref*ref){
                auto l = (Layout*)ref;
                int tag = l->getTag();
                int mi = tag / 100;
                int mj = tag % 100;
                int gameid = duijuList[mi]["gameClasliConfigList"][mj]["gameId"].asInt();
                int hallType = duijuList[mi]["gameClasliConfigList"][mj]["displayMode"].asInt();
                ZJHModel::getInstance()->setCurGameID(gameid, hallType == 2 ? 1 : 0);
                if (hallType == 2){
                    ZJHModel::getInstance()->toGameHall();
                }else{
                    ZJHModel::getInstance()->roomToHall(false);
                }
            });
            layout->addChild(iconText);
            
            string miniLogo = duijuList[i]["gameClasliConfigList"][j]["logo"].asString();
            string gameName = duijuList[i]["gameClasliConfigList"][j]["gameName"].asString();
            
            ImageView* icon = ImageView::create();
            icon->ignoreContentAdaptWithSize(false);
            icon->setAnchorPoint(Vec2(0.5,0.5));
            icon->setContentSize(Size(255,255));
            icon->setPosition(Vec2(dx / 2, dy / 2));
            ZJHModel::getInstance()->loadingImageResource(icon, miniLogo);
            
            char buf[128];
            Utils::parseName(6, buf, gameName.c_str());
            Text* title = Text::create(buf, "", 38);
            title->setColor(Color3B(0xff,0xff, 0xff));
            title->setAnchorPoint(Vec2(0.5, 0));
            title->setPosition(Vec2(iconText->getContentSize().width / 2, 0));
            
            iconText->addChild(icon);
            iconText->addChild(title);
            
            numsCount ++;
        }
    }
    
    Layout* bottomPageTips = Layout::create();
    bottomPageTips->setAnchorPoint(Vec2(0.0,0));
    bottomPageTips->setPosition(Vec2(0,360));
    addChild(bottomPageTips);
    
    dx = 50;
    by = 50;
    bx = size.width / 2 - (allPageNums - 1) / 2 * dx;
    for (int i = 0; i < allPageNums; i ++){
        Button *bottomIcon = Button::create("Match/bottom_tips.png", "Match/bottom_tips_press.png", "Match/bottom_tips_press.png");
        bottomIcon->setAnchorPoint(Vec2(0.5,0.5));
        bottomIcon->setTag(i);
        bottomIcon->setPosition(Vec2(bx + dx * i, by));
        bottomPageTips->addChild(bottomIcon,300);
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
}
