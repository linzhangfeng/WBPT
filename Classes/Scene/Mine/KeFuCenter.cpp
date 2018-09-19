//
//  KeFuCenter.cpp
//  ZJH
//
//  Created by apple on 16/7/1.
//
//

#include "KeFuCenter.hpp"
bool KeFuCenter::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setName("NearFriends");
    
    Size size = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(29, 29, 31, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    
    setTitle("客服中心");
    
    createTitle();
    
    //如果有缓存则不重新请求服务器
    Json::Value gameCache = ZJHModel::getInstance()->kefuDataCach;
    if(gameCache.empty()){
        requestData();
    }else{
        showListView(gameCache);
    }
    
    return true;
}

void KeFuCenter::requestData()
{
    this->schedule(schedule_selector(KeFuCenter::getData), 0);
    loading = Loading::startLoading(this, Director::getInstance()->getWinSize() / 2);
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"cuservice/list","","KeFuCenter_listdata");
}

void KeFuCenter::getData(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["KeFuCenter_listdata"];
    if (loginPacket && loginPacket->status != 0)
    {
        if (loginPacket->status != 3)
        {
            CCHttpAgent::getInstance()->packets["KeFuCenter_listdata"] = NULL;
            if(loading != NULL){
                loading->removeFromParent();
            }
            showLoadErrorDialog();
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"];
            ZJHModel::getInstance()->kefuDataCach = data;
            showListView(data);
        }
        this->unschedule(schedule_selector(KeFuCenter::getData));
        CCHttpAgent::getInstance()->packets["KeFuCenter_listdata"] = NULL;
        if(loading != NULL){
            loading->removeFromParent();
        }
        loading = NULL;
        delete loginPacket;
    }
}

void KeFuCenter::showListView(Json::Value json)
{
    Json::Value duijuList = json["list"];
    configList = duijuList;
    
    this->removeChildByName("tabitem_listview");
    Size winSize = Director::getInstance()->getWinSize();
    auto listview = ListView::create();
    listview->setDirection(ScrollView::Direction::VERTICAL);
    listview->setBounceEnabled(false);
    listview->setContentSize(Size(winSize.width, winSize.height - getHeadH() - 200 - 491 - 15));
    listview->setAnchorPoint(Vec2(0, 1));
    listview->setPosition(Vec2(0, winSize.height - getHeadH() - 200 - 15));
    listview->setItemsMargin(15);
    listview->setName("tabitem_listview");
    this->addChild(listview);
    
    for (int i = 0; i < configList.size(); i++)
    {
        //添加icon图片 计算icon坐标
        Layout* iconText = createItem(configList[i]);
        iconText->setTag(i);
        listview->addChild(iconText);
    }
    
    createCallPhone();
}

void KeFuCenter::showLoadErrorDialog(){
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

Layout* KeFuCenter::createItem(Json::Value json)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    Size item_size = Size(winSize.width, 240);
    
    Layout* iconText = Layout::create();
    iconText->setContentSize(item_size);
    iconText->setAnchorPoint(Vec2(0.5,0.5));
    
    auto bg = LayerColor::create(Color4B(38, 37, 42, 255));
    bg->setContentSize(item_size);
    bg->setAnchorPoint(Vec2(0.0, 0.0));
    bg->setPosition(0,0);
    iconText->addChild(bg);
    
//    bg = LayerColor::create(Color4B(0, 0, 0, 255));
//    bg->setContentSize(Size(item_size.width,2));
//    bg->setAnchorPoint(Vec2(0.0, 0.0));
//    bg->setPosition(0,0);
//    iconText->addChild(bg);
    
    string miniLogo = json["headUrl"].asString();
    string gameName = json["name"].asString();
    string id = json["number"].asString();
    string ps = json["watchword"].asString();
    string qrcode = json["qrcode"].asString();
    
    ImageView* icon = ImageView::create();
    icon->ignoreContentAdaptWithSize(false);
    icon->setAnchorPoint(Vec2(0.5,0.5));
    icon->setContentSize(Size(157,157));
    icon->setPosition(Vec2(100, item_size.height / 2));
    ZJHModel::getInstance()->loadingImageResource(icon, miniLogo);
    iconText->addChild(icon);
    
    Text* title = Text::create(gameName.c_str(), "Thonburi", 45);
    title->setColor(Color3B(227,227, 227));
    title->setAnchorPoint(Vec2(0.0, 0.5));
    title->setPosition(Vec2(icon->getPositionX() + icon->getContentSize().width / 2 + 20, 174));
    iconText->addChild(title);
    
    title = Text::create(__String::createWithFormat("微信号 %s",id.c_str())->getCString(), "Thonburi", 40);
    title->setColor(Color3B(35,134, 68));
    title->setAnchorPoint(Vec2(0.0, 0.5));
    title->setPosition(Vec2(icon->getPositionX() + icon->getContentSize().width / 2 + 20, 113));
    iconText->addChild(title);
    
    title = Text::create(ps.c_str(), "Thonburi", 35);
    title->setColor(Color3B(124,124, 124));
    title->setAnchorPoint(Vec2(0.0, 1.0));
    title->setPosition(Vec2(icon->getPositionX() + icon->getContentSize().width / 2 + 20, 80));
    iconText->addChild(title);
    
    icon = ImageView::create();
    icon->ignoreContentAdaptWithSize(false);
    icon->setAnchorPoint(Vec2(0.5,0.5));
    icon->setContentSize(Size(190,190));
    icon->setPosition(Vec2(item_size.width - 110, item_size.height / 2));
    ZJHModel::getInstance()->loadingImageResource(icon, qrcode);
    iconText->addChild(icon);
    return iconText;
}

void KeFuCenter::createTitle()
{
    Size winSize = Director::getInstance()->getWinSize();
    
    Size item_size = Size(winSize.width, 200);
    
    auto bg = LayerColor::create(Color4B(38, 37, 42, 255));
    bg->setContentSize(item_size);
    bg->setAnchorPoint(Vec2(0.0, 1.0));
    bg->setPosition(0,winSize.height - getHeadH() - 200);
    addChild(bg,1000);
    
    Sprite* userPortrit = Utils::createCircleAvatar(ZJHModel::getInstance()->portrit, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(140,140));
    userPortrit->setAnchorPoint(Vec2(0.5,0.5));
    userPortrit->setPosition(Vec2(100, item_size.height / 2));
    bg->addChild(userPortrit);
    
    char buf[128];
    Utils::parseName(6, buf, ZJHModel::getInstance()->name.c_str());
    
    Label* nameStr = Label::createWithSystemFont(buf, "Thonburi", 45);
    nameStr->setAnchorPoint(Vec2(0.0, 0.5));
    nameStr->setColor(Color3B(255, 255, 255));
    nameStr->setPosition(Vec2(192, item_size.height / 2));
    bg->addChild(nameStr);
    
    nameStr = Label::createWithSystemFont(__String::createWithFormat("斗牌ID %d",ZJHModel::getInstance()->uid)->getCString(), "Thonburi", 45);
    nameStr->setAnchorPoint(Vec2(1.0, 0.0));
    nameStr->setColor(Color3B(255, 255, 255));
    nameStr->setPosition(Vec2(winSize.width - 45, item_size.height / 2));
    bg->addChild(nameStr);
    
    nameStr = Label::createWithSystemFont("寻求客服帮助时，请告之斗牌ID", "Thonburi", 35);
    nameStr->setAnchorPoint(Vec2(1.0, 1.0));
    nameStr->setColor(Color3B(161, 161, 161));
    nameStr->setPosition(Vec2(winSize.width - 45, item_size.height / 2));
    bg->addChild(nameStr);
}

void KeFuCenter::createCallPhone()
{
    Size winSize = Director::getInstance()->getWinSize();
    
    Size item_size = Size(winSize.width, 491);
    
    Layout* iconText = Layout::create();
    iconText->setContentSize(item_size);
    iconText->setAnchorPoint(Vec2(0.0,0.0));
    iconText->setPosition(Vec2(0,0));
    addChild(iconText);
    
    auto bg = LayerColor::create(Color4B(38, 37, 42, 255));
    bg->setContentSize(item_size);
    bg->setAnchorPoint(Vec2(0.0, 0.0));
    bg->setPosition(0,0);
    addChild(bg);
    
    Label* nameStr = Label::createWithSystemFont("客服电话", "Thonburi", 45);
    nameStr->setAnchorPoint(Vec2(0.5, 0.5));
    nameStr->setColor(Color3B(255, 255, 255));
    nameStr->setPosition(Vec2(winSize.width / 2, 432));
    addChild(nameStr);
    
    nameStr = Label::createWithSystemFont(ZJHModel::getInstance()->kefuDataCach["phoneNumber"].asString().c_str(), "Thonburi", 45);
    nameStr->setAnchorPoint(Vec2(0.5, 0.5));
    nameStr->setColor(Color3B(255, 255, 255));
    nameStr->setPosition(Vec2(winSize.width / 2, 367));
    addChild(nameStr);
    
    Button* button = Button::create("Mine/call.png");
    button->setZoomScale(0.1);
    button->setPressedActionEnabled(true);
    button->setPosition(Vec2(winSize.width / 2, 178));
    button->addClickEventListener([=](Ref*ref){
        PlatformHelper::callPhone(ZJHModel::getInstance()->kefuDataCach["phoneNumber"].asString());
    });
    addChild(button,200);
}
