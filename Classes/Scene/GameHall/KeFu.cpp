
//
//  TaskLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/6.
//


#include "KeFu.hpp"
#include "../Loading/Loading.h"

bool KeFu::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size size = Director::getInstance()->getWinSize();
    
    auto cbg = LayerColor::create(Color4B(0, 0, 0, 200));
    cbg->setContentSize(size);
    cbg->setAnchorPoint(Vec2(0.0, 0.0));
    cbg->setPosition(0,0);
    addChild(cbg);
    
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setAnchorPoint(Vec2(0,0));
    layout->setPosition(Vec2(0,0));
    layout->setTouchEnabled(true);
    layout->setSwallowTouches(true);
    layout->addClickEventListener([=](Ref*){
    });
    addChild(layout,-1000);
    
    bg = Sprite::create("GameHall/kefu/b.png");
    bg->setPosition(Vec2(size.width/2, size.height/2));
    addChild(bg);
    
    auto title = Sprite::create("GameHall/kefu/t.png");
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 20));
    bg->addChild(title);
    
    auto closeBtn = Button::create("GameHall/kefu/record_btn_003.png", "GameHall/kefu/record_btn_003c.png");
    closeBtn->setPosition(Vec2(bg->getContentSize().width - 30, bg->getContentSize().height - 30));
    closeBtn->addClickEventListener([=](Ref*){
        this->removeFromParent();
    });
    bg->addChild(closeBtn,10);
    
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

void KeFu::requestData()
{
    this->schedule(schedule_selector(KeFu::getData), 0);
    loading = Loading::startLoading(this, Director::getInstance()->getWinSize() / 2);
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"cuservice/list","","KeFuCenter_listdata");
}

void KeFu::getData(float dt)
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
        this->unschedule(schedule_selector(KeFu::getData));
        CCHttpAgent::getInstance()->packets["KeFuCenter_listdata"] = NULL;
        if(loading != NULL){
            loading->removeFromParent();
        }
        loading = NULL;
        delete loginPacket;
    }
}

void KeFu::showListView(Json::Value json)
{
    Json::Value duijuList = json["list"];
    configList = duijuList;
    
    this->removeChildByName("tabitem_listview");
    Size winSize = bg->getContentSize();
    Size listSize = Size(1428,504);
    
    auto send_bg = ui::Scale9Sprite::create("GameHall/kefu/9.png");
    send_bg->setPreferredSize(listSize);
    send_bg->setAnchorPoint(Vec2(0.5, 0.0));
    send_bg->setPosition(Vec2(winSize.width / 2, 171));
    bg->addChild(send_bg);
    
    auto listview = ListView::create();
    listview->setDirection(ScrollView::Direction::HORIZONTAL);
    listview->setBounceEnabled(false);
    listview->setContentSize(listSize);
    listview->setAnchorPoint(Vec2(0.5, 0.0));
    listview->setPosition(Vec2(winSize.width / 2, 171));
    listview->setItemsMargin(0);
    listview->setName("tabitem_listview");
    bg->addChild(listview);
    
    for (int i = 0; i < configList.size(); i++)
    {
        //添加icon图片 计算icon坐标
        Layout* iconText = createItem(configList[i]);
        iconText->setTag(i);
        listview->addChild(iconText);
    }
    
    createCallPhone();
}

void KeFu::showLoadErrorDialog(){
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

Layout* KeFu::createItem(Json::Value json)
{
    auto mbg = Sprite::create("GameHall/kefu/b1.png");
    mbg->setScaleX(1.3);
    Size item_size = Size(1428 / 3,504);
    
    Layout* iconText = Layout::create();
    iconText->setContentSize(item_size);
    iconText->setAnchorPoint(Vec2(0.5,0.5));
    
    mbg->setPosition(Vec2(item_size.width / 2, item_size.height / 2));
    iconText->addChild(mbg);
    
    string miniLogo = json["headUrl"].asString();
    string gameName = json["name"].asString();
    string id = json["number"].asString();
    string ps = json["watchword"].asString();
    string qrcode = json["qrcode"].asString();
    
    ImageView* icon = ImageView::create();
    icon->ignoreContentAdaptWithSize(false);
    icon->setAnchorPoint(Vec2(0.5,0.5));
    icon->setContentSize(Size(110,110));
    icon->setPosition(Vec2(item_size.width / 2 - 93 - 45, 383));
    ZJHModel::getInstance()->loadingImageResource(icon, miniLogo);
    iconText->addChild(icon);
    
    char buf[128];
    Utils::parseName(10, buf, gameName.c_str());
    
    Text* title = Text::create(buf, "Thonburi", 30);
    title->setColor(Color3B(108,28, 0));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setPosition(Vec2(item_size.width / 2 + 50 + 5, 420));
    iconText->addChild(title);
    
    title = Text::create(__String::createWithFormat("微信号 %s",id.c_str())->getCString(), "Thonburi", 30);
    title->setColor(Color3B(83,0, 0));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setPosition(Vec2(item_size.width / 2 + 50, 363));
    iconText->addChild(title);
    
    Utils::parseName(18, buf, ps.c_str());
    title = Text::create(buf, "Thonburi", 24);
    title->setColor(Color3B(106,0, 0));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setPosition(Vec2(item_size.width / 2, 280));
    iconText->addChild(title);
    
    icon = ImageView::create();
    icon->ignoreContentAdaptWithSize(false);
    icon->setAnchorPoint(Vec2(0.5,0.5));
    icon->setContentSize(Size(190,190));
    icon->setPosition(Vec2(item_size.width / 2, 143));
    ZJHModel::getInstance()->loadingImageResource(icon, qrcode);
    iconText->addChild(icon);
    return iconText;
}

void KeFu::createTitle()
{
    Size winSize = bg->getContentSize();
    
    Sprite* userPortrit = Utils::createCircleAvatar(ZJHModel::getInstance()->portrit, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(140,140));
    userPortrit->setAnchorPoint(Vec2(0.5,0.5));
    userPortrit->setPosition(Vec2(225, winSize.height - 168));
    bg->addChild(userPortrit);
    
    char buf[128];
    Utils::parseName(6, buf, ZJHModel::getInstance()->name.c_str());
    
    Label* nameStr = Label::createWithSystemFont(buf, "Thonburi", 45);
    nameStr->setAnchorPoint(Vec2(0.0, 0.5));
    nameStr->setColor(Color3B(90, 0, 0));
    nameStr->setPosition(Vec2(322, winSize.height - 168));
    bg->addChild(nameStr);
    
    nameStr = Label::createWithSystemFont(__String::createWithFormat("斗牌ID %d",ZJHModel::getInstance()->uid)->getCString(), "Thonburi", 45);
    nameStr->setAnchorPoint(Vec2(1.0, 0.5));
    nameStr->setColor(Color3B(77, 0, 0));
    nameStr->setPosition(Vec2(winSize.width - 175, winSize.height - 141));
    bg->addChild(nameStr);
    
    nameStr = Label::createWithSystemFont("寻求客服帮助时，请告之斗牌ID", "Thonburi", 35);
    nameStr->setAnchorPoint(Vec2(1.0, 0.5));
    nameStr->setColor(Color3B(105, 27, 0));
    nameStr->setPosition(Vec2(winSize.width - 175, winSize.height - 203));
    bg->addChild(nameStr);
}

void KeFu::createCallPhone()
{
    Size winSize = bg->getContentSize();

    Button* button = Button::create("GameHall/kefu/c1.png","GameHall/kefu/c2.png");
    button->setPosition(Vec2(winSize.width / 2, 108));
    button->addClickEventListener([=](Ref*ref){
        PlatformHelper::callPhone(ZJHModel::getInstance()->kefuDataCach["phoneNumber"].asString());
    });
    bg->addChild(button,200);
    
    auto t = Sprite::create("GameHall/kefu/c.png");
    t->setPosition(Vec2(78, button->getContentSize().height / 2 + 3));
    button->addChild(t);
    
    Label* nameStr = Label::createWithSystemFont("<-点击拨打", "Thonburi", 45);
    nameStr->setAnchorPoint(Vec2(0.0, 0.5));
    nameStr->setColor(Color3B(138, 82, 43));
    nameStr->setPosition(Vec2(winSize.width / 2 + 275 + 20, 112));
    bg->addChild(nameStr);
    
    auto label = LabelAtlas::create(ZJHModel::getInstance()->kefuDataCach["phoneNumber"].asString().c_str(), "GameHall/kefu/n.png", 39, 50, '0');
    label->setAnchorPoint(Vec2(0.0, 0.5));
    label->setPosition(Vec2(141, button->getContentSize().height / 2 + 3));
    button->addChild(label);
}
