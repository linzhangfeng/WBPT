//
//  RoomEndTouPiao.cpp
//  ZJH
//
//  Created by mac on 16/9/8.
//
//

#include "RoomEndTouPiao.h"

bool RoomEndTouPiao::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    m_pCallback = NULL;
    timeout_tip = NULL;
    
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
    
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = [=](EventKeyboard::KeyCode keycode,Event* event)
    {
        if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
        {
        }
        event->stopPropagation();//
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    return true;
}

/*
 json
 {
    ownerid:220,
    ownername:"agag",
    time_left:20,
    players
    [
        {uid:111,name:"name",avatar:"avatar",state:1},//state: 0:等待 1：同意 2:反对
    ],
 }
 */
void RoomEndTouPiao::initData(Json::Value data)
{
    jsonData = data;
    owner_id = jsonData["ownerid"].asInt();
    if (ZJHModel::getInstance()->uid == owner_id)
    {
        initOwner(data);
    }
    else
    {
        initOther(data);
    }
}

void RoomEndTouPiao::initWithJsonStr(const std::string& d)
{
    Json::Value val;
    Json::Reader reader;
    reader.parse(d, val);
    initData(val);
}
void RoomEndTouPiao::initOwner(Json::Value data)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    Size size = Size(629,491);
    
    auto bg = ui::Scale9Sprite::create("RoomPublic/common/popup_window.png");
    bg->setPreferredSize(size);
    bg->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    addChild(bg);
    
    auto line = Sprite::create("RoomPublic/common/line.png");
    line->setScaleX(size.width / line->getContentSize().width);
    line->setPosition(Vec2(size.width / 2, size.height - 76));
    bg->addChild(line);
    
    auto title = Label::createWithSystemFont("正在申请解散房间", "Thonburi", 40);
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setPosition(Vec2(size.width / 2, size.height - 76 / 2));
    title->setColor(Color3B(240, 203, 91));
    bg->addChild(title);
    
    Size itemSize = Size(116,140);
    
    int nums = jsonData["players"].size();
    int bx = 0;
    int dx = 0;
    int by = 0;
    int dy = 0;
    int lineNums = 0;
    if (nums <= 4)
    {
        lineNums = 4;
        dx = 20;
        bx = size.width / 2 - (nums - 1) * (itemSize.width / 2 + dx / 2);
        by = 285;
        dy = 0;
        dx += itemSize.width;
    }
    else if (nums == 5)
    {
        lineNums = 5;
        dx = 6;
        bx = size.width / 2 - (nums - 1) * (itemSize.width / 2 + dx / 2);
        by = 285;
        dy = 0;
        dx += itemSize.width;
    }
    else
    {
        lineNums = 5;
        dx = 6;
        bx = size.width / 2 - (lineNums - 1) * (itemSize.width / 2 + dx / 2);
        by = 324;
        dy = 149;
        dx += itemSize.width;
    }
    
    char buf[128];
    for (int i = 0; i < nums; i ++)
    {
        int uid = jsonData["players"][i]["uid"].asInt();
        int di = i % lineNums;
        int dj = i / lineNums;
        auto player_bg = Sprite::create("RoomPublic/jiesan/player_bg.png");
        player_bg->setPosition(Vec2(bx + dx * di, by - dy * dj));
        bg->addChild(player_bg);
        
        Size bg_size = player_bg->getContentSize();
        
        Utils::parseName(4, buf, jsonData["players"][i]["name"].asCString());
        
        auto name = Label::createWithSystemFont(buf, "Thonburi", 20);
        name->setPosition(Vec2(bg_size.width / 2, bg_size.height - 22));
        name->setColor(Color3B(255,255,255));
        player_bg->addChild(name);
        
        auto avatar = Utils::createCircleImage(jsonData["players"][i]["avatar"].asCString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(60,60));
        avatar->setPosition(bg_size / 2);
        player_bg->addChild(avatar);
        
        if (uid == owner_id)
        {
            auto tip = Label::createWithSystemFont("发起人", "Thonburi", 20);
            tip->setPosition(Vec2(bg_size.width / 2, 22));
            tip->setColor(Color3B(240,203,91));
            player_bg->addChild(tip);
        }
        else
        {
            auto no = Sprite::create("RoomPublic/jiesan/no.png");
            no_Map[uid] = no;
            no->setPosition(Vec2(19, 22));
            player_bg->addChild(no);
            no->setVisible(false);
            
            auto noText = Label::createWithSystemFont("反对解散", "Thonburi", 18);
            noText->setAnchorPoint(Vec2(0, 0.5));
            noText->setPosition(Vec2(27, no->getContentSize().height / 2));
            noText->setColor(Color3B(218,45,21));
            no->addChild(noText);
            
            auto ok = Sprite::create("RoomPublic/jiesan/ok.png");
            ok_Map[uid] = ok;
            ok->setPosition(Vec2(19, 22));
            player_bg->addChild(ok);
            ok->setVisible(false);
            
            auto okText = Label::createWithSystemFont("同意解散", "Thonburi", 18);
            okText->setAnchorPoint(Vec2(0, 0.5));
            okText->setPosition(Vec2(27, ok->getContentSize().height / 2));
            okText->setColor(Color3B(240,203,91));
            ok->addChild(okText);
            
            auto wait = Label::createWithSystemFont("等待响应", "Thonburi", 20);
            wait_Map[uid] = wait;
            wait->setPosition(Vec2(bg_size.width / 2, 22));
            wait->setColor(Color3B(153,153,153));
            player_bg->addChild(wait);
            wait->setVisible(false);
            
            int state = jsonData["players"][i]["state"].asInt();
            if (state == 0)
            {
                wait->setVisible(true);
            }
            else if (state == 1)
            {
                ok->setVisible(true);
            }
            else if (state == 2)
            {
                no->setVisible(true);
            }
        }
    }
    
    //time
    {
        auto timeBG = Sprite::create("RoomPublic/jiesan/yuan.png");
        timeBG->setPosition(Vec2(size.width / 2, 50));
        if (nums <= 5)
        {
            timeBG->setPosition(Vec2(size.width / 2, 95));
        }
        bg->addChild(timeBG);
        
        time_count = jsonData["time_left"].asInt();
        timeLabel = Label::createWithSystemFont(__String::createWithFormat("%02d",time_count)->getCString(), "Thonburi", 22);
        timeLabel->setPosition(timeBG->getContentSize() / 2);
        timeLabel->setColor(Color3B(240,203,91));
        timeBG->addChild(timeLabel);
        
        if (time_count > 0)
        {
            schedule(schedule_selector(RoomEndTouPiao::updateTime), 1.0f);
        }
    }
}

void RoomEndTouPiao::initOther(Json::Value data)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    Size size = Size(629,491);
    
    auto bg = ui::Scale9Sprite::create("RoomPublic/common/popup_window.png");
    bg->setPreferredSize(size);
    bg->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    addChild(bg);
    
    auto line = Sprite::create("RoomPublic/common/line.png");
    line->setScaleX(size.width / line->getContentSize().width);
    line->setPosition(Vec2(size.width / 2, size.height - 76));
    bg->addChild(line);
    
    char buf[128];
    
    Utils::parseName(4, buf, jsonData["ownername"].asCString());
    
    auto title = Label::createWithSystemFont(__String::createWithFormat("玩家 %s 申请解散房间",buf)->getCString(), "Thonburi", 40);
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setPosition(Vec2(size.width / 2, size.height - 76 / 2));
    title->setColor(Color3B(240, 203, 91));
    bg->addChild(title);
    
    //time
    {
        auto timeBG = Sprite::create("RoomPublic/jiesan/yuan.png");
        timeBG->setPosition(Vec2(size.width / 2, 238));
        bg->addChild(timeBG);
        
        time_count = jsonData["time_left"].asInt();
        timeLabel = Label::createWithSystemFont(__String::createWithFormat("%02d",time_count)->getCString(), "Thonburi", 22);
        timeLabel->setPosition(timeBG->getContentSize() / 2);
        timeLabel->setColor(Color3B(240,203,91));
        timeBG->addChild(timeLabel);
        
        if (time_count > 0)
        {
            schedule(schedule_selector(RoomEndTouPiao::updateTime), 1.0f);
        }
    }
    
    {
        timeout_tip = Label::createWithSystemFont("若您超时未选择，则系统默认为\"反对解散\"", "Thonburi", 30);
        timeout_tip->setPosition(Vec2(size.width / 2, 166));
        timeout_tip->setColor(Color3B(225,205,173));
        bg->addChild(timeout_tip);
    }
    
    {
        auto ok_tip_bg = Sprite::create("RoomPublic/jiesan/tip_bg.png");
        ok_tip_bg->setPosition(Vec2(91, 109));
        bg->addChild(ok_tip_bg);
        
        auto ok = Sprite::create("RoomPublic/jiesan/ok.png");
        ok->setPosition(Vec2(19, 22));
        ok_tip_bg->addChild(ok);
        
        auto okText = Label::createWithSystemFont("同意解散", "Thonburi", 18);
        okText->setAnchorPoint(Vec2(0, 0.5));
        okText->setPosition(Vec2(27, ok->getContentSize().height / 2));
        okText->setColor(Color3B(240,203,91));
        ok->addChild(okText);
        
        okLabel = Label::createWithSystemFont("0/0", "Thonburi", 18);
        okLabel->setAnchorPoint(Vec2(0.5, 0.5));
        okLabel->setPosition(Vec2(130, ok_tip_bg->getContentSize().height / 2));
        okLabel->setColor(Color3B(240,203,91));
        ok_tip_bg->addChild(okLabel);
        
        okNameLabel = LabelTTF::create("", ".SFUIDisplay-Semibold", 20, Size(420, 0), TextHAlignment::LEFT);
        okNameLabel->setColor(Color3B(255,255, 255));
        okNameLabel->setAnchorPoint(Vec2(0, 0.5));
        okNameLabel->setPosition(Vec2(184, 109));
        bg->addChild(okNameLabel);
    }
    
    {
        auto no_tip_bg = Sprite::create("RoomPublic/jiesan/tip_bg.png");
        no_tip_bg->setPosition(Vec2(91, 41));
        bg->addChild(no_tip_bg);
        
        auto no = Sprite::create("RoomPublic/jiesan/no.png");
        no->setPosition(Vec2(19, 22));
        no_tip_bg->addChild(no);
        
        auto noText = Label::createWithSystemFont("反对解散", "Thonburi", 18);
        noText->setAnchorPoint(Vec2(0, 0.5));
        noText->setPosition(Vec2(27, no->getContentSize().height / 2));
        noText->setColor(Color3B(218,45,21));
        no->addChild(noText);
        
        noLabel = Label::createWithSystemFont("0/0", "Thonburi", 18);
        noLabel->setAnchorPoint(Vec2(0.5, 0.5));
        noLabel->setPosition(Vec2(130, no_tip_bg->getContentSize().height / 2));
        noLabel->setColor(Color3B(218,45,21));
        no_tip_bg->addChild(noLabel);
        
        noNameLabel = LabelTTF::create("", ".SFUIDisplay-Semibold", 20, Size(420, 0), TextHAlignment::LEFT);
        noNameLabel->setColor(Color3B(255,255, 255));
        noNameLabel->setAnchorPoint(Vec2(0, 0.5));
        noNameLabel->setPosition(Vec2(184, 41));
        bg->addChild(noNameLabel);
    }
    
    {
        ok = Button::create("RoomPublic/jiesan/btn_yellow.png");
        ok->setScale9Enabled(true);
        ok->setContentSize(Size(Size(273, 53)));
        ok->setTitleText("同意解散");
        ok->setTitleFontSize(30);
        ok->setTitleColor(Color3B(0,0,0));
        ok->setPosition(Vec2(size.width / 2 - 151,321));
        ok->setVisible(true);
        ok->setName("ok");
        bg->addChild(ok,1000);
        ok->addClickEventListener(CC_CALLBACK_1(RoomEndTouPiao::buttonCallback, this));
        
        no = Button::create("RoomPublic/jiesan/btn_red_big.png");
        no->setScale9Enabled(true);
        no->setContentSize(Size(Size(273, 53)));
        no->setTitleText("反对解散");
        no->setTitleFontSize(30);
        no->setTitleColor(Color3B(255,255,255));
        no->setPosition(Vec2(size.width / 2 + 151,321));
        no->setVisible(true);
        no->setName("no");
        bg->addChild(no,1000);
        no->addClickEventListener(CC_CALLBACK_1(RoomEndTouPiao::buttonCallback, this));
    }
    
    {
        myTip = Label::createWithSystemFont("您选择了", "Thonburi", 30);
        myTip->setAnchorPoint(Vec2(1, 0.5));
        myTip->setPosition(Vec2(size.width / 2 - 5, 321));
        myTip->setColor(Color3B(255,255,255));
        bg->addChild(myTip);
        
        myResult = Label::createWithSystemFont("", "Thonburi", 30);
        myResult->setAnchorPoint(Vec2(0.0, 0.5));
        myResult->setPosition(Vec2(size.width / 2 + 5, 321));
        bg->addChild(myResult);
    }
    updateOther();
}

void RoomEndTouPiao::updateTime(float dt)
{
    time_count--;
    timeLabel->setString(__String::createWithFormat("%02d",time_count)->getCString());
    
    if (time_count <= 0)
    {
        unschedule(schedule_selector(RoomEndTouPiao::updateTime));
    }
}

void RoomEndTouPiao::updateState(int uid,int state)
{
    int nums = jsonData["players"].size();
    for (int i = 0; i < nums ; i ++)
    {
        if (jsonData["players"][i]["uid"].asInt() == uid)
        {
            jsonData["players"][i]["state"] = state;
            break;
        }
    }
    
    if (ZJHModel::getInstance()->uid == owner_id)
    {
        updateOwner();
    }
    else
    {
        updateOther();
    }
}

void RoomEndTouPiao::updateOwner()
{
    int nums = jsonData["players"].size();
    for (int i = 0; i < nums; i ++)
    {
        int state = jsonData["players"][i]["state"].asInt();
        int uid = jsonData["players"][i]["uid"].asInt();
        if (uid == owner_id)
        {
            continue;
        }
        
        if (state == 1)
        {
            wait_Map[uid]->setVisible(false);
            no_Map[uid]->setVisible(false);
            ok_Map[uid]->setVisible(true);
        }
        else if (state == 2)
        {
            wait_Map[uid]->setVisible(false);
            no_Map[uid]->setVisible(true);
            ok_Map[uid]->setVisible(false);
        }
    }
}

void RoomEndTouPiao::updateOther()
{
    int my_state = 0;
    int nums = jsonData["players"].size();
    int ok_count = 0;
    int no_count = 0;
    vector<std::string> ok_names;
    vector<std::string> no_names;
    char buf[128];
    
    for (int i = 0; i < nums; i ++)
    {
        int state = jsonData["players"][i]["state"].asInt();
        int uid = jsonData["players"][i]["uid"].asInt();
        if (state == 0)
        {
            
        }
        else if (state == 1)
        {
            ok_count ++;
            Utils::parseName(4, buf, jsonData["players"][i]["name"].asCString());
            ok_names.push_back(buf);
        }
        else if (state == 2)
        {
            no_count ++;
            Utils::parseName(4, buf, jsonData["players"][i]["name"].asCString());
            no_names.push_back(buf);
        }
        
        if (uid == ZJHModel::getInstance()->uid)
        {
            my_state = state;
        }
    }
    
    if (my_state == 0)
    {
        myTip->setVisible(false);
        myResult->setVisible(false);
        no->setVisible(true);
        ok->setVisible(true);
    }
    else
    {
        myTip->setVisible(true);
        myResult->setVisible(true);
        no->setVisible(false);
        ok->setVisible(false);
        
        if (my_state == 1)
        {
            myResult->setString("同意解散");
            myResult->setColor(Color3B(240,203,91));
        }
        else if (my_state == 2)
        {
            myResult->setString("反对解散");
            myResult->setColor(Color3B(218,45,21));
        }
    }
    
    noLabel->setString(__String::createWithFormat("%d/%d",no_count,nums)->getCString());
    okLabel->setString(__String::createWithFormat("%d/%d",ok_count,nums)->getCString());
    
    std::string okname = "";
    for (int i = 0; i < ok_names.size(); i ++)
    {
        okname += ok_names[i];
        
        if (i != ok_names.size() - 1)
        {
            okname += "、";
        }
    }
    okNameLabel->setString(okname);
    
    std::string noname = "";
    for (int i = 0; i < no_names.size(); i ++)
    {
        noname += no_names[i];
        
        if (i != no_names.size() - 1)
        {
            noname += "、";
        }
    }
    noNameLabel->setString(noname);
}

void RoomEndTouPiao::buttonCallback(cocos2d::Ref *sender)
{
    ui::Widget *btn = dynamic_cast<Widget *>(sender);
    if (btn)
    {
        std::string name = btn->getName();
//        if (name == "ok")
//        {
//
//        }
//        else if (name == "no")
//        {
//
//        }
        if (m_pCallback)m_pCallback(name);
    }
}

void RoomEndTouPiao::setDefaultTips(const std::string tips){
    if(timeout_tip != NULL){
        timeout_tip->setString(tips);
    }
}

void RoomEndTouPiao::onExit()
{
    Layer::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}
