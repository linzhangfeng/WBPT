//
// 横版投票
//
//

#include "RoomHEndTouPiao.h"

bool RoomHEndTouPiao::init()
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
void RoomHEndTouPiao::initData(Json::Value data)
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

void RoomHEndTouPiao::initWithJsonStr(const std::string& d)
{
    Json::Value val;
    Json::Reader reader;
    reader.parse(d, val);
    initData(val);
}


//正在申请解散房间(发起人的展示界面)
void RoomHEndTouPiao::initOwner(Json::Value data)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    Size size = Size(890,599);
    
    
    //背景框
    auto bg = Scale9Sprite::create("RoomPublicH/dialog/room_dialog_big.png");
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    bg->setPosition(winSize / 2);
    bg->setContentSize(Size(size.width, size.height));
    bg->setName("content_bg");
    this->addChild(bg);
    
    //标题图片
    auto titleSp = ImageView::create("RoomPublicH/dialog/dialog_end_title.png");
    titleSp->setScale9Enabled(true);
    titleSp->setAnchorPoint(Vec2(0.5, 1));
    titleSp->setPosition(Vec2(size.width/2, size.height));
    bg->addChild(titleSp);
    
    auto title = Label::createWithSystemFont("正在申请解散房间", "Thonburi", 34);
    title->setAnchorPoint(Vec2(0.5, 1));
    title->setPosition(Vec2(size.width / 2, size.height - 96-15));
    title->setColor(Color3B(0x4d, 0x2d, 0x1e));
    bg->addChild(title);
    
    Size itemSize = Size(132,155);
    
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
        auto player_bg = Sprite::create("RoomPublicH/end/end_item_bg.png");
        player_bg->setPosition(Vec2(bx + dx * di, by - dy * dj));
        bg->addChild(player_bg);
        
        Size bg_size = player_bg->getContentSize();
        
        Utils::parseName(4, buf, jsonData["players"][i]["name"].asString().c_str());
        
        auto name = Label::createWithSystemFont(buf, "Thonburi", 22);
        name->setPosition(Vec2(bg_size.width / 2, bg_size.height - 22));
        name->setColor(Color3B(0x4d,0x2d,0x1e));
        player_bg->addChild(name);
        
        auto avatar = Utils::createCircleImage(jsonData["players"][i]["avatar"].asString().c_str(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(76,76));
        avatar->setPosition(bg_size / 2);
        player_bg->addChild(avatar);
        
        if (uid == owner_id)
        {
            auto tip = Label::createWithSystemFont("发起人", "Thonburi", 22);
            tip->setPosition(Vec2(bg_size.width / 2, 22));
            tip->setColor(Color3B(0xaf,0x5a,0x22));
            player_bg->addChild(tip);
        }
        else
        {
            auto no = Sprite::create("RoomPublicH/end/end_item_against.png");
            no_Map[uid] = no;
            no->setPosition(Vec2(bg_size.width / 2, 22));
            player_bg->addChild(no);
            no->setVisible(false);
            
//            auto noText = Label::createWithSystemFont("反对解散", "Thonburi", 18);
//            noText->setAnchorPoint(Vec2(0, 0.5));
//            noText->setPosition(Vec2(27, no->getContentSize().height / 2));
//            noText->setColor(Color3B(218,45,21));
//            no->addChild(noText);
            
            auto ok = Sprite::create("RoomPublicH/end/end_item_agree.png");
            ok_Map[uid] = ok;
            ok->setPosition(Vec2(bg_size.width / 2, 22));
            player_bg->addChild(ok);
            ok->setVisible(false);
            
//            auto okText = Label::createWithSystemFont("同意解散", "Thonburi", 18);
//            okText->setAnchorPoint(Vec2(0, 0.5));
//            okText->setPosition(Vec2(27, ok->getContentSize().height / 2));
//            okText->setColor(Color3B(240,203,91));
//            ok->addChild(okText);
            
            auto wait = Label::createWithSystemFont("等待响应", "Thonburi", 22);
            wait_Map[uid] = wait;
            wait->setPosition(Vec2(bg_size.width / 2, 22));
            wait->setColor(Color3B(0xaf,0x5a,0x22));
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
        auto timeBG = Sprite::create("RoomPublicH/end/count_down_bg.png");
        timeBG->setPosition(Vec2(size.width / 2, 50));
        if (nums <= 5)
        {
            timeBG->setPosition(Vec2(size.width / 2, 95));
        }
        bg->addChild(timeBG);
        
        time_count = jsonData["time_left"].asInt();
        timeLabel = Label::createWithSystemFont(__String::createWithFormat("%02d",time_count)->getCString(), "Thonburi", 30);
        timeLabel->setPosition(timeBG->getContentSize() / 2);
        timeLabel->setColor(Color3B(0x4d,0x2d,0x1e));
        timeBG->addChild(timeLabel);
        
        if (time_count > 0)
        {
            schedule(schedule_selector(RoomHEndTouPiao::updateTime), 1.0f);
        }
    }
}

//初始化非发起人
void RoomHEndTouPiao::initOther(Json::Value data)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    Size size = Size(890,599);
    
    //背景框
    auto bg = Scale9Sprite::create("RoomPublicH/dialog/room_dialog_big.png");
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    bg->setPosition(winSize / 2);
    bg->setContentSize(Size(size.width, size.height));
    bg->setName("content_bg");
    this->addChild(bg);
    
    //标题图片
    auto titleSp = ImageView::create("RoomPublicH/dialog/dialog_end_title.png");
    titleSp->setScale9Enabled(true);
    titleSp->setAnchorPoint(Vec2(0.5, 1));
    titleSp->setPosition(Vec2(size.width/2, size.height));
    bg->addChild(titleSp);
    
    auto line = LayerColor::create(Color4B(0xf1, 0xce, 0x9f, 255));
    line->setIgnoreAnchorPointForPosition(false);
    line->setContentSize(Size(794,2));
    line->setAnchorPoint(Vec2(0.5,0.5));
    line->setPosition(Vec2(size.width/2, size.height-177));
    bg->addChild(line);
    
    char buf[128];
    
    Utils::parseName(4, buf, jsonData["ownername"].asString().c_str());
    
    auto title = Label::createWithSystemFont(__String::createWithFormat("玩家 %s 申请解散房间",buf)->getCString(), "Thonburi", 36);
    title->setAnchorPoint(Vec2(0.5, 1));
    title->setPosition(Vec2(size.width / 2, size.height- (100+13)));
    title->setColor(Color3B(0x4d, 0x2d, 0x1e));
    bg->addChild(title);
    
    //time
    {
        otherTimeBG = Sprite::create("RoomPublicH/end/count_down_bg.png");
        otherTimeBG->setAnchorPoint(Vec2(0.5, 1));
        otherTimeBG->setPosition(Vec2(size.width / 2, size.height-(233+13)));
        bg->addChild(otherTimeBG);
        
        time_count = jsonData["time_left"].asInt();
        timeLabel = Label::createWithSystemFont(__String::createWithFormat("%02d",time_count)->getCString(), "Thonburi", 22);
        timeLabel->setPosition(otherTimeBG->getContentSize() / 2);
        timeLabel->setColor(Color3B(0x4d,0x2d,0x1e));
        otherTimeBG->addChild(timeLabel);
        
        if (time_count > 0)
        {
            schedule(schedule_selector(RoomHEndTouPiao::updateTime), 1.0f);
        }
    }
    
    {
        timeout_tip = Label::createWithSystemFont("若您超时未选择，则系统默认为\"反对解散\"", "Thonburi", 24);
        timeout_tip->setAnchorPoint(Vec2(0.5, 0));
        timeout_tip->setPosition(Vec2(size.width / 2, 207+13));
        timeout_tip->setColor(Color3B(0xaf,0x5a,0x22));
        bg->addChild(timeout_tip);
    }
    
    Size tipsSize = Size(795, 72);
    
    //同意解散tips
    {
        auto agreeTipsBg = ui::Scale9Sprite::create("RoomPublicH/dialog/dialog_invite_item_bg.png");
        agreeTipsBg->setPreferredSize(tipsSize);
        agreeTipsBg->setAnchorPoint(Vec2(0, 0));
        agreeTipsBg->setPosition(Vec2(47, 109+13));
        bg->addChild(agreeTipsBg);
        
        auto ok_tip_bg = Sprite::create("RoomPublicH/end/agree_number_tips.png");
        ok_tip_bg->setAnchorPoint(Vec2(0, 0.5));
        ok_tip_bg->setPosition(Vec2(14, tipsSize.height/2));
        agreeTipsBg->addChild(ok_tip_bg);
        
        okLabel = LabelAtlas::create("0:0", "RoomPublicH/end/agree_number.png", 20, 23, '0');
        okLabel->setAnchorPoint(Vec2(0, 0.5));
        okLabel->setPosition(Vec2(178, tipsSize.height/2));
        agreeTipsBg->addChild(okLabel);
        
        okNameLabel = LabelTTF::create("", ".SFUIDisplay-Semibold", 20, Size(420, 0), TextHAlignment::LEFT);
        okNameLabel->setColor(Color3B(0x4d, 0x2d, 0x1e));
        okNameLabel->setAnchorPoint(Vec2(0, 0.5));
        okNameLabel->setPosition(Vec2(245, tipsSize.height/2));
        agreeTipsBg->addChild(okNameLabel);
    }
    
    //反对解散tips

    {
        auto againstTipsBg = ui::Scale9Sprite::create("RoomPublicH/dialog/dialog_invite_item_bg.png");
        againstTipsBg->setPreferredSize(tipsSize);
        againstTipsBg->setAnchorPoint(Vec2(0, 0));
        againstTipsBg->setPosition(Vec2(47, 27+13));
        bg->addChild(againstTipsBg);
        
        auto no_tip_bg = Sprite::create("RoomPublicH/end/against_number_tips.png");
        no_tip_bg->setAnchorPoint(Vec2(0, 0.5));
        no_tip_bg->setPosition(Vec2(14, tipsSize.height/2));
        againstTipsBg->addChild(no_tip_bg);
        
        noLabel = LabelAtlas::create("0:0", "RoomPublicH/end/against_number.png", 20, 23, '0');
        noLabel->setAnchorPoint(Vec2(0, 0.5));
        noLabel->setPosition(Vec2(178, tipsSize.height/2));
        againstTipsBg->addChild(noLabel);
        
        noNameLabel = LabelTTF::create("", ".SFUIDisplay-Semibold", 20, Size(420, 0), TextHAlignment::LEFT);
        noNameLabel->setColor(Color3B(0x4d, 0x2d, 0x1e));
        noNameLabel->setAnchorPoint(Vec2(0, 0.5));
        noNameLabel->setPosition(Vec2(245, tipsSize.height/2));
        againstTipsBg->addChild(noNameLabel);
    }
    
    {
        ok = Button::create("RoomPublicH/end/agree_end_bt.png", "RoomPublicH/end/agree_end_bt_press.png");
        ok->setScale9Enabled(true);
        ok->setAnchorPoint(Vec2(0, 0));
        ok->setPosition(Vec2(80+20, 273+13));
        ok->setVisible(true);
        ok->setName("ok");
        bg->addChild(ok,1000);
        ok->addClickEventListener(CC_CALLBACK_1(RoomHEndTouPiao::buttonCallback, this));
        
        no = Button::create("RoomPublicH/end/against_end_bt.png", "RoomPublicH/end/against_end_bt_press.png");
        no->setScale9Enabled(true);
        no->setAnchorPoint(Vec2(1, 0));
        no->setPosition(Vec2(size.width - (80+20), 273+13));
        no->setVisible(true);
        no->setName("no");
        bg->addChild(no,1000);
        no->addClickEventListener(CC_CALLBACK_1(RoomHEndTouPiao::buttonCallback, this));
    }
    
    {
        myTip = Label::createWithSystemFont("您选择了", "Thonburi", 30);
        myTip->setAnchorPoint(Vec2(1, 0));
        myTip->setPosition(Vec2(size.width / 2 - 5, 336+13));
        myTip->setColor(Color3B(0x4d,0x2d,0x1e));
        bg->addChild(myTip);
        
//        myResult = Label::createWithSystemFont("", "Thonburi", 30);
//        myResult->setAnchorPoint(Vec2(0.0, 0.5));
//        myResult->setPosition(Vec2(size.width / 2 + 5, 321));
//        bg->addChild(myResult);
        myResult = ImageView::create("");
        myResult->setAnchorPoint(Vec2(0.0, 0));
        myResult->setPosition(Vec2(size.width / 2 + 5, 336+13));
        bg->addChild(myResult);
        
    }
    updateOther();
}

void RoomHEndTouPiao::updateTime(float dt)
{
    time_count--;
    timeLabel->setString(__String::createWithFormat("%02d",time_count)->getCString());
    
    if (time_count <= 0)
    {
        unschedule(schedule_selector(RoomHEndTouPiao::updateTime));
    }
}

void RoomHEndTouPiao::updateState(int uid,int state)
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

void RoomHEndTouPiao::updateOwner()
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

void RoomHEndTouPiao::updateOther()
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
            Utils::parseName(4, buf, jsonData["players"][i]["name"].asString().c_str());
            ok_names.push_back(buf);
        }
        else if (state == 2)
        {
            no_count ++;
            Utils::parseName(4, buf, jsonData["players"][i]["name"].asString().c_str());
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
        otherTimeBG->setPosition(Vec2(890 / 2, 599-(264+13)));
        
        if (my_state == 1)
        {
            myResult->loadTexture("RoomPublicH/end/agree_end_tips.png");
        }
        else if (my_state == 2)
        {
            myResult->loadTexture("RoomPublicH/end/against_end_tips.png");
        }
    }
    
    noLabel->setString(__String::createWithFormat("%d:%d",no_count,nums)->getCString());
    okLabel->setString(__String::createWithFormat("%d:%d",ok_count,nums)->getCString());
    
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

void RoomHEndTouPiao::buttonCallback(cocos2d::Ref *sender)
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

void RoomHEndTouPiao::setDefaultTips(const std::string tips){
    if(timeout_tip != NULL){
        timeout_tip->setString(tips);
    }
}

void RoomHEndTouPiao::onExit()
{
    Layer::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}
