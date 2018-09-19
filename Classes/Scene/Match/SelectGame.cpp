//
//  SelectGame.cpp
//  ZJH
//
//  Created by apple on 16/8/17.
//
//

#include "SelectGame.hpp"
#include "../../MyGUI/ImageByUrl.h"

bool SelectGame::init()
{
    if (!Layer::init())
    {
        return false;
    }
    setName("SelectGame");
    curID = -1;
    
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
    
    auto bg = LayerColor::create(Color4B(14, 14, 15, 255));
    bg->setContentSize(size);
    bg->setAnchorPoint(Vec2(0.0, 0.0));
    bg->setPosition(0,0);
    addChild(bg);
    
    Text* text = Text::create("搜索更多游戏", "Thonburi", 50);
    text->setColor(Color3B(194, 174, 150));
    text->setPosition(Vec2(size.width / 2,size.height - 62));
    text->setName("title");
    addChild(text);
    
    auto cancel = Button::create("SelectGame/yellow_cancel_btn.png","SelectGame/yellow_cancel_btn.png");
    cancel->setPosition(Vec2(size.width - 62, size.height - 62));
    cancel->addClickEventListener([=](Ref*ref){
        
        this->removeFromParent();
    });
    cancel->setContentSize(Size(160,120));
    cancel->ignoreContentAdaptWithSize(false);
    cancel->setFillContentSizeEnabled(false);
    addChild(cancel);
    
    Size editBoxSize = Size(754, 100);
    _editName = EditBox::create(editBoxSize, Scale9Sprite::create("Chat/bar_txt.png"));
    _editName->setAnchorPoint(Vec2(0.5,0.5));
    _editName->setPosition(Vec2(414, size.height - 223));
    _editName->setFontSize(50);
    _editName->setName("_editName");
    _editName->setFontColor(Color3B(139,139,139));
    _editName->setPlaceHolder("输入游戏名");
    _editName->setPlaceholderFontSize(40);
    _editName->setPlaceholderFontColor(Color3B(139,139,139));
    _editName->setMaxLength(20);
    _editName->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    _editName->setReturnType(EditBox::KeyboardReturnType::DONE);
    _editName->setDelegate(this);
    this->addChild(_editName);
    
    auto search = Button::create("SelectGame/btn.png","SelectGame/btn.png");
    search->setPosition(Vec2(size.width - 149, size.height - 223));
    search->addClickEventListener([=](Ref*ref){
        requestSearchData();
    });
    search->setZoomScale(0.1);
    search->setPressedActionEnabled(true);
    addChild(search);
    
    auto title = Text::create("搜索", "Thonburi", 40);
    title->setColor(Color3B(122,86, 34));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setPosition(Vec2(search->getContentSize().width / 2, search->getContentSize().height / 2));
    search->addChild(title);
    
    //如果有缓存则不重新请求服务器
    Json::Value gameCache = ZJHModel::getInstance()->gameList;
    if(gameCache.empty()){
        requestData();
    }else{
        showListView(gameCache);
    }
    
    return true;
}

void SelectGame::requestData()
{
    this->schedule(schedule_selector(SelectGame::getData), 0);
    loading = Loading::startLoading(this, Director::getInstance()->getWinSize() / 2 + Size(0,180));
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"gameClasli/gameList","","SelectGame_listdata");
}

void SelectGame::getData(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["SelectGame_listdata"];
    if (loginPacket && loginPacket->status != 0)
    {
        if (loginPacket->status != 3)
        {
            CCHttpAgent::getInstance()->packets["SelectGame_listdata"] = NULL;
            if(loading != NULL){
                loading->removeFromParent();
            }
            showLoadErrorDialog();
            return;
        }

        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"];
            showListView(data);
            ZJHModel::getInstance()->gameList = data;
        }
        this->unschedule(schedule_selector(SelectGame::getData));
        CCHttpAgent::getInstance()->packets["SelectGame_listdata"] = NULL;
        if(loading != NULL){
            loading->removeFromParent();
        }
        loading = NULL;
        delete loginPacket;
    }
}

void SelectGame::showLoadErrorDialog(){
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
    
    Text *title = Text::create("提示", "Thonburi", 50);
    title->setTextColor(Color4B(0xf0, 0xcb, 0x5c, 255));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 70));
    bg->addChild(title);
    
    Text *title2 = Text::create("请求游戏数据失败,请重试", "Thonburi", 45);
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
    
    Text *titleBtn = Text::create("重试", "Thonburi", 40);
    titleBtn->setTextColor(Color4B(0x60, 0x41, 0x10, 255));
    titleBtn->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleBtn->setPosition(Vec2(lianBut->getContentSize().width/2, lianBut->getContentSize().height/2));
    lianBut->addChild(titleBtn);
}

void SelectGame::showListView(Json::Value json)
{
    Json::Value duijuList = json["classifyList"];
    configList = duijuList;
    initTab();
    showTab(0);
}

void SelectGame::initTab()
{
    this->removeChildByName("tab_list");
    Size winSize = Director::getInstance()->getWinSize();
    auto listview = ListView::create();
    listview->setDirection(ScrollView::Direction::HORIZONTAL);
    listview->setBounceEnabled(false);
    listview->setContentSize(Size(winSize.width - 100, 100));
    listview->setAnchorPoint(Vec2(0.5, 0.5));
    listview->setPosition(Vec2(winSize.width / 2, winSize.height - 341));
    listview->setItemsMargin(0);
    listview->setScrollBarEnabled(false);
    listview->setName("tab_list");
    this->addChild(listview);
    allTabs.clear();
    for(int i = 0; i <= configList.size(); i++)
    {
        string tab_name = "所有";
        if (i > 0)
        {
            tab_name = configList[i - 1]["classifyName"].asString();
        }
        
        //添加icon图片 计算icon坐标
        Layout* iconText = Layout::create();
        iconText->setTouchEnabled(true);
        iconText->setTag(i);
        iconText->setContentSize(Size(165,100));
        iconText->setAnchorPoint(Vec2(0.5,0.5));
        iconText->addClickEventListener([=](Ref*ref){
            auto l = (Layout*)ref;
            int tag = l->getTag();
            showTab(tag);
            
            if((l->getWorldPosition().x + iconText->getContentSize().width - 50) > listview->getContentSize().width){
                if(l->getTag() == configList.size() || (l->getTag() == (configList.size() - 1))){
                    listview->jumpToRight();
                    return;
                }
                Vec2 p = listview->getInnerContainerPosition();
                p -= Vec2(iconText->getContentSize().width,0);
                listview->setInnerContainerPosition(p);
            }else if((l->getWorldPosition().x - iconText->getContentSize().width - 50) < 0){
                if(l->getTag() == 0 || l->getTag() == 1){
                    listview->jumpToLeft();
                    return;
                }
                Vec2 p = listview->getInnerContainerPosition();
                p += Vec2(iconText->getContentSize().width,0);
                if(p.x > 0 && p.x <iconText->getContentSize().width){
                    listview->jumpToLeft();
                    return;
                }
                listview->setInnerContainerPosition(p);
            }
        });
        
        char buf[128];
        Utils::parseName(6, buf, tab_name.c_str());
        Text* title = Text::create(buf, "Thonburi", 40);
        title->setColor(Color3B(225,225, 225));
        title->setAnchorPoint(Vec2(0.5, 0.5));
        title->setName("title");
        title->setPosition(Vec2(iconText->getContentSize().width / 2, iconText->getContentSize().height / 2));
        
        iconText->addChild(title);
        
        auto line = Sprite::create("SelectGame/line.png");
        line->setName("line");
        line->setVisible(false);
        line->setAnchorPoint(Vec2(0.5,0));
        line->setPosition(Vec2(iconText->getContentSize().width / 2, 2));
        iconText->addChild(line);
        
        listview->addChild(iconText);
        allTabs.push_back(iconText);
    }
}
void SelectGame::showTab(int index)
{
    if (index == curID)return;
    curID = index;
    for (int i = 0; i < allTabs.size(); i ++)
    {
        Layout* l = allTabs[i];
        Text* title = (Text*)l->getChildByName("title");
        Sprite* line = (Sprite*)l->getChildByName("line");
        
        if (i == index)
        {
            title->setColor(Color3B(247,215, 122));
            line->setVisible(true);
        }
        else
        {
            title->setColor(Color3B(0xff,0xff, 0xff));
            line->setVisible(false);
        }
    }
    
    this->removeChildByName("tabitem_listview");
    Size winSize = Director::getInstance()->getWinSize();
    auto listview = ListView::create();
    listview->setDirection(ScrollView::Direction::VERTICAL);
    listview->setBounceEnabled(false);
    listview->setContentSize(Size(winSize.width, winSize.height - 400));
    listview->setAnchorPoint(Vec2(0, 1));
    listview->setPosition(Vec2(0, winSize.height - 400));
    listview->setItemsMargin(0);
    listview->setName("tabitem_listview");
    this->addChild(listview);
    
    
    int id = -1;
    if (index > 0)
    {
        id = configList[index - 1]["id"].asInt();
    }
    
    for(int i = 0; i < configList.size(); i++)
    {
        if (id != -1 && configList[i]["id"].asInt() != id)
        {
            continue;
        }
        
        std::string tabName = configList[i]["classifyName"].asString();
        
        for (int j = 0; j < configList[i]["gameClasliConfigList"].size(); j++)
        {
            string miniLogo = configList[i]["gameClasliConfigList"][j]["logo"].asString();
            string gameName = configList[i]["gameClasliConfigList"][j]["gameName"].asString();
            
            //添加icon图片 计算icon坐标
            Layout* iconText = createItem(miniLogo,gameName,tabName,i * 100 + j);
            listview->addChild(iconText);
        }
    }
}

Layout* SelectGame::createSearchTitle()
{
    Size winSize = Director::getInstance()->getWinSize();
    
    Size item_size = Size(winSize.width, 111);
    
    Layout* iconText = Layout::create();
    iconText->setContentSize(item_size);
    
    auto bg = LayerColor::create(Color4B(50, 50, 50, 255));
    bg->setContentSize(Size(item_size.width,2));
    bg->setAnchorPoint(Vec2(0.0, 0.0));
    bg->setPosition(0,2);
    iconText->addChild(bg);
    
    Text* title = Text::create("搜索结果", "Thonburi", 45);
    title->setColor(Color3B(220,220, 220));
    title->setAnchorPoint(Vec2(0.0, 0.5));
    title->setPosition(Vec2(42, item_size.height / 2));
    iconText->addChild(title);
    return iconText;
}

Layout* SelectGame::createItem(std::string img,std::string gamename,std::string tabname, int tag)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    Size item_size = Size(winSize.width, 214);
    
    Layout* iconText = Layout::create();
    iconText->setContentSize(item_size);
//    iconText->setTouchEnabled(true);
    iconText->setAnchorPoint(Vec2(0.5,0.5));

    auto bg = LayerColor::create(Color4B(29, 29, 31, 255));
    bg->setContentSize(item_size);
    bg->setAnchorPoint(Vec2(0.0, 0.0));
    bg->setPosition(0,0);
    iconText->addChild(bg);
    
    bg = LayerColor::create(Color4B(35, 35, 38, 255));
    bg->setContentSize(Size(item_size.width,2));
    bg->setAnchorPoint(Vec2(0.0, 0.0));
    bg->setPosition(0,0);
    iconText->addChild(bg);
    
    string miniLogo = img;
    string gameName = gamename;
    
    ImageByUrl* icon = ImageByUrl::create();
    icon->load(157, 157, miniLogo);
    
//    ImageView* icon = ImageView::create();
//    icon->ignoreContentAdaptWithSize(false);
//    icon->setAnchorPoint(Vec2(0.5,0.5));
//    icon->setContentSize(Size(157,157));
    icon->setPosition(Vec2(137, item_size.height / 2));
//    ZJHModel::getInstance()->loadingImageResource(icon, miniLogo);
    iconText->addChild(icon);
    
    Text* title = Text::create(gamename.c_str(), "Thonburi", 45);
    title->setColor(Color3B(227,205, 177));
    title->setAnchorPoint(Vec2(0.0, 0.0));
    title->setPosition(Vec2(icon->getPositionX() + icon->getContentSize().width / 2 + 20, item_size.height / 2));
    iconText->addChild(title);
    
    title = Text::create(tabname.c_str(), "Thonburi", 40);
    title->setColor(Color3B(124,124, 124));
    title->setAnchorPoint(Vec2(0.0, 1.0));
    title->setPosition(Vec2(icon->getPositionX() + icon->getContentSize().width / 2 + 20, item_size.height / 2 - 10));
    iconText->addChild(title);
    
    auto gotogame = Button::create("SelectGame/btn2.png","SelectGame/btn2.png");
    gotogame->setPosition(Vec2(winSize.width - 161, item_size.height / 2));
    gotogame->setZoomScale(0.1);
    gotogame->setPressedActionEnabled(true);
//    gotogame->setSwallowTouches(false);
//    gotogame->setEnbled(false);
    iconText->addChild(gotogame);
    
    gotogame->addClickEventListener([=](Ref*ref){
//        auto l = (Layout*)ref;
//        int tag = iconText->getTag();
        int mi = tag / 100;
        int mj = tag % 100;
        int gameid = configList[mi]["gameClasliConfigList"][mj]["gameId"].asInt();
        int hallType = 2;//duijuList[mi]["gameClasliConfigList"][mj]["displayMode"].asInt();
        ZJHModel::getInstance()->setCurGameID(gameid, hallType == 2 ? 1 : 0);
        if (hallType == 2){
            ZJHModel::getInstance()->toGameHall();
        }else{
            ZJHModel::getInstance()->roomToHall(false);
        }
    });
    
    iconText->addClickEventListener([=](Ref*ref){
        //        auto l = (Layout*)ref;
        //        int tag = iconText->getTag();
        int mi = tag / 100;
        int mj = tag % 100;
        int gameid = configList[mi]["gameClasliConfigList"][mj]["gameId"].asInt();
        int hallType = 2;//duijuList[mi]["gameClasliConfigList"][mj]["displayMode"].asInt();
        ZJHModel::getInstance()->setCurGameID(gameid, hallType == 2 ? 1 : 0);
        if (hallType == 2){
            ZJHModel::getInstance()->toGameHall();
        }else{
            ZJHModel::getInstance()->roomToHall(false);
        }
    });
    
    title = Text::create("进入游戏", "Thonburi", 40);
    title->setColor(Color3B(247,215, 122));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setPosition(Vec2(gotogame->getContentSize().width / 2, gotogame->getContentSize().height / 2));
    gotogame->addChild(title);
    
    return iconText;
}

void SelectGame::requestSearchData()
{
    std::string cur_text = _editName->getText();
    if (cur_text.empty())
    {
        this->removeChildByName("search_listview");
        return;
    }
    
    std::vector<int> allIndex;
    allIndex.clear();
    for(int i = 0; i < configList.size(); i++)
    {
        for (int j = 0; j < configList[i]["gameClasliConfigList"].size(); j++)
        {
            string gameName = configList[i]["gameClasliConfigList"][j]["gameName"].asString();
            if (gameName.find(cur_text) != -1)
            {
                allIndex.push_back(i * 100 + j);
            }
        }
    }
    showSearchListView(allIndex);
}

void SelectGame::showSearchListView(std::vector<int> allIndex)
{
    this->removeChildByName("search_listview");
    Size winSize = Director::getInstance()->getWinSize();
    
    auto bg = LayerColor::create(Color4B(29, 29, 31, 255));
    bg->setContentSize(Size(winSize.width,winSize.height - 300));
    bg->setAnchorPoint(Vec2(0.0, 0.0));
    bg->setPosition(Vec2(0,0));
    bg->setName("search_listview");
    addChild(bg);
    
    auto title = createSearchTitle();
    title->setAnchorPoint(Vec2(0.0, 1.0));
    title->setPosition(Vec2(0,bg->getContentSize().height));
    bg->addChild(title);
    
    auto listview = ListView::create();
    listview->setDirection(ScrollView::Direction::VERTICAL);
    listview->setBounceEnabled(false);
    listview->setContentSize(Size(winSize.width, winSize.height - 300 - 111));
    listview->setAnchorPoint(Vec2(0.0, 0.0));
    listview->setPosition(Vec2(0, 0));
    listview->setItemsMargin(0);
    listview->setScrollBarEnabled(false);
    bg->addChild(listview);
    
    if (allIndex.size() == 0)
    {
        Text* title = Text::create("没有搜索到该游戏", "Thonburi", 55);
        title->setColor(Color3B(126,126, 126));
        title->setAnchorPoint(Vec2(0.5, 0.5));
        title->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height - 206));
        bg->addChild(title);
        return;
    }
    
    for (int i = 0; i < allIndex.size(); i ++)
    {
        int mi = allIndex[i] / 100;
        int mj = allIndex[i] % 100;
        std::string tabName = configList[mi]["classifyName"].asString();
        string miniLogo = configList[mi]["gameClasliConfigList"][mj]["logo"].asString();
        string gameName = configList[mi]["gameClasliConfigList"][mj]["gameName"].asString();
        
        //添加icon图片 计算icon坐标
        Layout* iconText = createItem(miniLogo,gameName,tabName,mi * 100 + mj);
        listview->addChild(iconText);
    }
}

void SelectGame::onExit()
{
    Layer::onExit();
}

void SelectGame::editBoxEditingDidBegin(EditBox* editBox)
{
}

void SelectGame::editBoxEditingDidEnd(EditBox* editBox)
{
}

void SelectGame::editBoxTextChanged(EditBox* editBox, const std::string &text)
{
    requestSearchData();
}

void SelectGame::editBoxReturn(EditBox *editBox)
{
}
