//
//  SelectDiqu.cpp
//  ZJH
//
//  Created by apple on 16/8/17.
//
//

#include "SelectDiqu.hpp"
bool SelectDiqu::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    curIndex = 0;
    
    setName("SelectDiqu");
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
    
    Text* text = Text::create("选择城市", "Thonburi", 50);
    text->setColor(Color3B(194, 174, 150));
    text->setPosition(Vec2(size.width / 2,size.height - 62));
    text->setName("title");
    addChild(text);
    
    auto cancel = Button::create("SelectGame/yellow_cancel_btn.png","SelectGame/yellow_cancel_btn.png");
    cancel->setPosition(Vec2(size.width - 62, size.height - 62));
    cancel->setContentSize(Size(160,120));
    cancel->ignoreContentAdaptWithSize(false);
    cancel->setFillContentSizeEnabled(false);
    cancel->addClickEventListener([=](Ref*ref){
        
        this->removeFromParent();
    });
    
    addChild(cancel);
    
    Size editBoxSize = Size(754, 100);
    _editName = EditBox::create(editBoxSize, Scale9Sprite::create("Chat/bar_txt.png"));
    _editName->setAnchorPoint(Vec2(0.5,0.5));
    _editName->setPosition(Vec2(414, size.height - 223));
    _editName->setFontSize(50);
    _editName->setName("_editName");
    _editName->setFontColor(Color3B(139,139,139));
    _editName->setPlaceHolder("输入城市名");
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
    
    loading = Loading::startLoading(this, Director::getInstance()->getWinSize() / 2 + Size(0,180));
    this->schedule(schedule_selector(SelectDiqu::gotoInitView), 0.1);
    return true;
}

void SelectDiqu::gotoInitView(float dt)
{
    this->unschedule(schedule_selector(SelectDiqu::gotoInitView));
    //如果有缓存则不重新请求服务器
    Json::Value gameCache = ZJHModel::getInstance()->diquList;
    if(gameCache.empty()){
        requestData();
    }else{
        if(loading != NULL){
            loading->removeFromParent();
            loading = NULL;
        }
        showListView(gameCache);
    }
}

void SelectDiqu::requestData()
{
    this->schedule(schedule_selector(SelectDiqu::getData), 0);
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"sCity/search","","SelectDiqu_listdata");
}

void SelectDiqu::getData(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["SelectDiqu_listdata"];
    if (loginPacket && loginPacket->status != 0)
    {
        if (loginPacket->status != 3)
        {
            CCHttpAgent::getInstance()->packets["SelectDiqu_listdata"] = NULL;
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
            ZJHModel::getInstance()->diquList = data;
        }
        this->unschedule(schedule_selector(SelectDiqu::getData));
        CCHttpAgent::getInstance()->packets["SelectDiqu_listdata"] = NULL;
        if(loading != NULL){
            loading->removeFromParent();
        }
        loading = NULL;
        delete loginPacket;
    }
}

void SelectDiqu::showLoadErrorDialog(){
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

void SelectDiqu::showListView(Json::Value json)
{
    initOverSelect();
    
    Json::Value duijuList = json["cityNameList"];
    configList = duijuList;
    
    this->removeChildByName("tabitem_listview");
    Size winSize = Director::getInstance()->getWinSize();
    auto listview = ListView::create();
    listview->setDirection(ScrollView::Direction::VERTICAL);
    listview->setBounceEnabled(false);
    listview->setContentSize(Size(winSize.width, winSize.height - 300 - 236));
    listview->setAnchorPoint(Vec2(0, 1));
    listview->setPosition(Vec2(0, winSize.height - 300 - 236));
    listview->setItemsMargin(0);
    listview->setName("tabitem_listview");
    this->addChild(listview);
    listview->addEventListener([=](Ref *sendre, ScrollView::EventType eventType){
        if (eventType == ScrollView::EventType::SCROLL_TO_BOTTOM)
        {
            showmore();
        }
    });
    
    cur_key = "";
    int all = configList.size();//MIN(20,configList.size() - curIndex);
    for (int i = 0; i < all; i++)
    {
        string gameName = configList[curIndex]["cityName"].asString();
        string key = configList[curIndex]["letter"].asString();
        if (key != cur_key)
        {
            cur_key = key;
            Layout* iconText = createKey(cur_key);
            listview->addChild(iconText);
        }
        //添加icon图片 计算icon坐标
        Layout* iconText = createItem(gameName);
        iconText->setTag(curIndex);
        listview->addChild(iconText);
        curIndex ++;
    }
}

void SelectDiqu::showmore()
{
    if (curIndex >= configList.size() -1)
    {
        return;
    }
    auto listview = (ListView*)getChildByName("tabitem_listview");
    auto pos = listview->getInnerContainerPosition();
    
    int all = MIN(20,configList.size() - curIndex);
    float h =0;
    for (int i = 0; i < all; i ++)
    {
        string gameName = configList[curIndex]["cityName"].asString();
        string key = configList[curIndex]["letter"].asString();
        if (key != cur_key)
        {
            cur_key = key;
            Layout* iconText = createKey(cur_key);
            listview->addChild(iconText);
        }
        //添加icon图片 计算icon坐标
        Layout* iconText = createItem(gameName);
        iconText->setTag(curIndex);
        listview->addChild(iconText);
        h+=iconText->getContentSize().height;
        curIndex ++;
    }
    
    pos.y-=h;
    listview->forceDoLayout();
    listview->setInnerContainerPosition(pos);
    listview->stopAutoScroll();
}

void SelectDiqu::initOverSelect()
{
    Size winSize = Director::getInstance()->getWinSize();
    
    Size item_size = Size(winSize.width, 236);
    
    Layout* iconText = Layout::create();
    iconText->setContentSize(item_size);
    iconText->setAnchorPoint(Vec2(0.5,1.0));
    iconText->setPosition(Vec2(winSize.width / 2,winSize.height - 300));
    addChild(iconText);
    
    auto bg = LayerColor::create(Color4B(29, 29, 31, 255));
    bg->setContentSize(item_size);
    bg->setAnchorPoint(Vec2(0.0, 0.0));
    bg->setPosition(0,0);
    iconText->addChild(bg);
    
//    bg = LayerColor::create(Color4B(35, 35, 38, 255));
//    bg->setContentSize(Size(item_size.width,2));
//    bg->setAnchorPoint(Vec2(0.0, 0.0));
//    bg->setPosition(0,0);
//    iconText->addChild(bg);
    int ddx = 30;
    int dx = (winSize.width - ddx * 4) / 3;
    int bx = ddx + dx / 2;
    int by = 75;
    
    Text* title = Text::create("最近选择", "Thonburi", 45);
    title->setColor(Color3B(239,239, 239));
    title->setAnchorPoint(Vec2(0.0, 0.5));
    title->setPosition(Vec2(ddx, 180));
    iconText->addChild(title);
    
    Json::Value json = ZJHModel::getInstance()->diquSelfCach;
    int cur_size = json.size();
    for (int i = 0; i < cur_size ; i ++)
    {
        Button *lianBut = Button::create("SelectGame/btn4.png", "SelectGame/btn4.png");
        lianBut->setScale9Enabled(true);
        lianBut->setAnchorPoint(Vec2(0.5, 0.5));
        lianBut->setContentSize(Size(dx , 100));
        lianBut->setPosition(Vec2(bx + (dx + ddx) * i, by));
        lianBut->setTag(i);
        bg->addChild(lianBut);
        lianBut->addClickEventListener([=](Ref*ref){
            auto l = (Button*)ref;
            int tag = l->getTag();
            int diqu_id = json[tag]["cityId"].asInt();
            string diqu_name = json[tag]["cityName"].asString();
            ZJHModel::getInstance()->setDiquCach(diqu_id,diqu_name);
            __NotificationCenter::getInstance()->postNotification("GameListView_updateview_notify");
            this->removeFromParent();
        });
        
        char buf[128];
        Utils::parseName(6, buf, json[i]["cityName"].asString().c_str());
        Text *titleBtn = Text::create(buf, "Thonburi", 35);
        titleBtn->setTextColor(Color4B(200, 179, 154, 255));
        titleBtn->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        titleBtn->setPosition(Vec2(lianBut->getContentSize().width/2, lianBut->getContentSize().height/2));
        lianBut->addChild(titleBtn);
    }
}

Layout* SelectDiqu::createSearchTitle()
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

Layout* SelectDiqu::createItem(std::string gamename)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    Size item_size = Size(winSize.width, 120);
    
    Layout* iconText = Layout::create();
    iconText->setContentSize(item_size);
    iconText->setTouchEnabled(true);
    iconText->setAnchorPoint(Vec2(0.5,0.5));
    iconText->addClickEventListener([=](Ref*ref){
        auto l = (Layout*)ref;
        int tag = l->getTag();
        int diqu_id = configList[tag]["cityId"].asInt();
        string diqu_name = configList[tag]["cityName"].asString();
        ZJHModel::getInstance()->setDiquCach(diqu_id,diqu_name);
        __NotificationCenter::getInstance()->postNotification("GameListView_updateview_notify");
        this->removeFromParent();
    });
    
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
    
    Text* title = Text::create(gamename.c_str(), "Thonburi", 45);
    title->setColor(Color3B(125,125, 125));
    title->setAnchorPoint(Vec2(0.0, 0.5));
    title->setPosition(Vec2(50, item_size.height / 2));
    iconText->addChild(title);
    
    return iconText;
}

Layout* SelectDiqu::createKey(std::string gamename)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    Size item_size = Size(winSize.width, 120);
    
    Layout* iconText = Layout::create();
    iconText->setContentSize(item_size);
    iconText->setAnchorPoint(Vec2(0.5,0.5));
    
    auto bg = LayerColor::create(Color4B(38, 38, 42, 255));
    bg->setContentSize(item_size);
    bg->setAnchorPoint(Vec2(0.0, 0.0));
    bg->setPosition(0,0);
    iconText->addChild(bg);
    
    bg = LayerColor::create(Color4B(21, 21, 21, 255));
    bg->setContentSize(Size(item_size.width,2));
    bg->setAnchorPoint(Vec2(0.0, 0.0));
    bg->setPosition(0,0);
    iconText->addChild(bg);
    
    Text* title = Text::create(gamename.c_str(), "Thonburi", 55);
    title->setColor(Color3B(239,239, 239));
    title->setAnchorPoint(Vec2(0.0, 0.5));
    title->setPosition(Vec2(50, item_size.height / 2));
    iconText->addChild(title);
    
    return iconText;
}

void SelectDiqu::requestSearchData()
{
    std::string cur_text = _editName->getText();
    if (cur_text.empty())
    {
        this->removeChildByName("search_listview");
        return;
    }
    
    std::vector<int> allIndex;
    allIndex.clear();
    for (int i = 0; i < configList.size(); i++)
    {
        string gameName = configList[i]["cityName"].asString();
        if (gameName.find(cur_text) != -1)
        {
            allIndex.push_back(i);
        }
    }
    showSearchListView(allIndex);
}

void SelectDiqu::showSearchListView(std::vector<int> allIndex)
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
        Text* title = Text::create("没有搜索到该地区", "Thonburi", 55);
        title->setColor(Color3B(126,126, 126));
        title->setAnchorPoint(Vec2(0.5, 0.5));
        title->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height - 206));
        bg->addChild(title);
        return;
    }
    
    for (int i = 0; i < allIndex.size(); i ++)
    {
        string gameName = configList[allIndex[i]]["cityName"].asString();
        
        //添加icon图片 计算icon坐标
        Layout* iconText = createItem(gameName);
        iconText->setTag(allIndex[i]);
        listview->addChild(iconText);
    }
}

void SelectDiqu::onExit()
{
    Layer::onExit();
}

void SelectDiqu::editBoxEditingDidBegin(EditBox* editBox)
{
}

void SelectDiqu::editBoxEditingDidEnd(EditBox* editBox)
{
}

void SelectDiqu::editBoxTextChanged(EditBox* editBox, const std::string &text)
{
    requestSearchData();
}

void SelectDiqu::editBoxReturn(EditBox *editBox)
{
}
