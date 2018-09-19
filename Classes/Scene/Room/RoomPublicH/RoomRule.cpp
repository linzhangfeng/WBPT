//
//
// 横版游戏帮助
//
//

#include "RoomRule.h"

USING_NS_CC;

bool RoomRule::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomRule::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = [=](EventKeyboard::KeyCode keycode,Event* event)
    {
        if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
        {
            this->removeFromParent();
        }
        event->stopPropagation();//
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    
    return true;
}

bool RoomRule::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Node *dialog = dynamic_cast<Sprite *>(this->getChildByName("dialog"));
    if (dialog && dialog->getBoundingBox().containsPoint(touch->getLocation()))
    {
        //
    }
    else
    {   webView->setVisible(false);
        this->removeFromParentAndCleanup(true);
        event->stopPropagation();
    }
    
    return true;
}

void RoomRule::initHelp()
{
    initHelpWeb();
}

void RoomRule::initHelpWeb()
{
    
    Size winSize = Director::getInstance()->getWinSize();
    Size bgSize = Size(963,601);
    
    
    LayerColor *mask = LayerColor::create(Color4B(0, 0, 0, 100));
    mask->setContentSize(winSize);
    this->addChild(mask);
    
    auto *dialog = Scale9Sprite::create("RoomPublicH/dialog/dialog_rule_bg.png");
    dialog->setAnchorPoint(Vec2(0.5, 0.5));
    dialog->setPosition(winSize / 2);
    dialog->setContentSize(Size(bgSize.width, bgSize.height));
    dialog->setName("dialog");
    mask->addChild(dialog);
    
    //游戏帮助按钮
    helpBtn = Button::create("RoomPublicH/dialog/dialog_rule_help_check.png", "RoomPublicH/dialog/dialog_rule_help_check.png");
    helpBtn->setAnchorPoint(Vec2(1,1));
    helpBtn->setPosition(Vec2(bgSize.width/2, bgSize.height-2));
    helpBtn->addClickEventListener(CC_CALLBACK_1(RoomRule::buttonCallback, this));
    helpBtn->setName("rule_help");
    dialog->addChild(helpBtn);
    
    //游戏规则按钮
    ruleBtn = Button::create("RoomPublicH/dialog/dialog_rule_rule.png","RoomPublicH/dialog/dialog_rule_rule.png");
    ruleBtn->setAnchorPoint(Vec2(0,1));
    ruleBtn->setPosition(Vec2(bgSize.width/2, bgSize.height-2));
    ruleBtn->addClickEventListener(CC_CALLBACK_1(RoomRule::buttonCallback, this));
    ruleBtn->setName("rule_rule");
    dialog->addChild(ruleBtn);

    
    webView = cocos2d::experimental::ui::WebView::create();
    webView->setAnchorPoint(Vec2(0, 0));
    webView->setPosition(Vec2(35, 33));
    webView->setContentSize(Size(bgSize.width-70,484));
    webView->setScalesPageToFit(true);
    log("============url==%s====",(ZJHModel::getInstance()->gameHelpUrl).c_str());
    webView->loadURL(ZJHModel::getInstance()->gameHelpUrl);
    dialog->addChild(webView);
    
    //牌局设置
    Size listViewSize = Size(893, 450);
    
//    auto listViewBg = ui::Scale9Sprite::create("RoomPublicH/dialog/dialog_invite_item_bg.png");
//    listViewBg->setScale9Enabled(true);
//    listViewBg->setContentSize(Size(799, 355));
//    listViewBg->setAnchorPoint(Vec2(0, 0));
//    listViewBg->setPosition(Vec2(47, 40));
//    dialog->addChild(listViewBg);
    
    
    ruleListView = ListView::create();
    ruleListView->setAnchorPoint(Vec2(0, 0));
    ruleListView->setBounceEnabled(false);
    ruleListView->setScrollBarEnabled(true);
    ruleListView->setDirection(ScrollView::Direction::VERTICAL);
    ruleListView->setContentSize(listViewSize);
    ruleListView->setPosition(Vec2(47, 44));
    ruleListView->setItemsMargin(2);
    ruleListView->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    dialog->addChild(ruleListView);
    ruleListView->setVisible(false);
    
    std::vector<int> IDList;
    std::vector<string> classIDList;
    int intK = 0;
    Json::Value roomData = ZJHModel::getInstance()->roomData["gameRoom"]["innerWayRecommend"][intK];
    
    Json::Value fistBao = roomData["gameCustomClassifyVOList"];
    for (int i = 0; i < fistBao.size(); i++)
    {
        Json::Value pointJson = fistBao[i]["gameCustomPlayValInfoList"];
        if (fistBao[i]["platformVisible"].asBool() == false)
        {
            continue;
        }
        bool isHave = false;
        for (int k = 0; k < pointJson.size(); k++)
        {
            if (pointJson[k]["iptValue"] != Json::Value::null)
            {
                isHave = true;
                break;
            }
        }
        
        
        if (isHave == false)
        {
            continue;
        }
        Layout* cell = Layout::create();
        cell->setContentSize(Size(listViewSize.width, 100 + (pointJson.size()-1)/3*50));
        ruleListView->pushBackCustomItem(cell);
        
        LabelTTF *title =  LabelTTF::create(roomData["gameCustomClassifyVOList"][i]["classifyName"].asString(), ".SFUIDisplay-Semibold", 30, Size(cell->getContentSize().width, 32), TextHAlignment::LEFT);
        
        title->setColor(Color3B(0x4d, 0x2d, 0x1e));
        title->setAnchorPoint(Vec2(0, 0.5));
        title->setPosition(Vec2(20,cell->getContentSize().height - 15));
        cell->addChild(title, 2);
        
        //auto cellBg = ui::Scale9Sprite::create("RoomPublic/common/set_txt_bg.png");
        //cellBg->setPreferredSize(cell->getContentSize());
        //cellBg->setAnchorPoint(Vec2(0, 0));
        //cellBg->setPosition(Vec2(0, 0));
        //cell->addChild(cellBg);
        
        for (int j = 0; j < pointJson.size(); j++)
        {
            bool isSelect = true;
            if (pointJson[j]["iptValue"] == Json::Value::null)
            {
                isSelect = false;
            }
            
            Button *selecBtn;
            if (pointJson[j]["classifyId"].asInt() == 29)
            {
                char buff[128];
                
                int tempV = atoi(pointJson[j]["playName"].asString().c_str());
                tempV *= ZJHModel::getInstance()->roomData["gameRoom"]["bottomNote"].asInt();
                sprintf(buff, "%d", tempV);
                
                selecBtn = selectBtnDraw(buff, isSelect, roomData["gameCustomClassifyVOList"][i]["choiceType"].asInt());
                
            }else
            {
                if (roomData["gameCustomClassifyVOList"][i]["choiceType"].asInt() == 4)
                {
                    char buff[128];
                    sprintf(buff, "%d", pointJson[j]["iptValue"].asInt());
                    
                    selecBtn = selectBtnDraw(buff, isSelect, roomData["gameCustomClassifyVOList"][i]["choiceType"].asInt());
                    
                }else
                {
                    selecBtn = selectBtnDraw(pointJson[j]["playName"].asString(), isSelect, roomData["gameCustomClassifyVOList"][i]["choiceType"].asInt());
                }
            }
            
            selecBtn->setAnchorPoint(Vec2(0, 1));
            selecBtn->setPosition(Vec2(10+ j%3*(300), cell->getContentSize().height - 60 - j/3*50));
            cell->addChild(selecBtn, 2);
            
        }
        
    }

    Button *closeBtn = Button::create("RoomPublicH/dialog/dialog_close.png", "RoomPublicH/dialog/dialog_close_press.png");
    closeBtn->ignoreContentAdaptWithSize(false);
    closeBtn->setScale9Enabled(true);
    closeBtn->setPosition(Vec2(bgSize.width - 61, bgSize.height - 58));
    closeBtn->setTag(ALERT_DIALOG_BUTTON_CLOSE);
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->removeFromParentAndCleanup(true);
                                    });
    dialog->addChild(closeBtn);
    buttonCallback(ruleBtn);
}


//生成对应的富文本控件
RichText * RoomRule::createRichText(const std::string &label, const std::string &content){
    auto richText = RichText::create();
    auto richLabel = RichElementText::create(1, Color3B(80, 0, 0), 255, label, "AmericanTypewriter", 43);
    auto richContent = RichElementText::create(2, Color3B(80, 0, 0), 255, content, "AmericanTypewriter", 43);
    richText->pushBackElement(richLabel);
    richText->pushBackElement(richContent);
    return richText;
}

//生成对应的checkButton
Button * RoomRule::selectBtnDraw(const std::string &titleStr, bool selected, int cellType){
    
    Button *btn = Button::create();
    
    Text *title = Text::create(titleStr, "", 28);
    title->setContentSize(Size(110, 36));
    title->setTextColor(Color4B(0xaf, 0x5a, 0x22, 255));
    title->setAnchorPoint(Vec2(0, 0.5));
    title->setPosition(Vec2(60, btn->getContentSize().height/2));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    btn->addChild(title);
    
    ImageView *kongImg = ImageView::create();
    kongImg->setScale(0.68);
    kongImg->setAnchorPoint(Vec2(0, 0.5));
    kongImg->setPosition(Vec2(10, btn->getContentSize().height/2));
    btn->addChild(kongImg);
    
    ImageView *selelctImg = ImageView::create();
    selelctImg->ignoreAnchorPointForPosition(true);
    selelctImg->setScale(0.92);
    selelctImg->setName("selecIcon");
    selelctImg->setAnchorPoint(Vec2(0.5, 0.5));
    selelctImg->setPosition(Vec2(kongImg->getContentSize().width / 2 - 7,kongImg->getContentSize().height / 2 - 7));
    kongImg->addChild(selelctImg, 2);
    selelctImg->setVisible(selected);
    
    if(cellType == 2)
    {
        kongImg->loadTexture("GameHall/createRoom/point_hold_kong1.png");
        selelctImg->loadTexture("GameHall/createRoom/play_point_check_can.png");
    }else
    {
        kongImg->loadTexture("GameHall/createRoom/point_hold_kong2.png");
        selelctImg->loadTexture("GameHall/createRoom/play_point_check_sigle_can.png");
    }
    return  btn;
}

void RoomRule::buttonCallback(cocos2d::Ref *sender)
{
    ui::Widget *btn = dynamic_cast<Widget *>(sender);
    if (btn) {
        std::string name = btn->getName();
        if (name == "rule_help")
        {
            //显示帮助
            webView->setVisible(true);
            ruleListView->setVisible(false);
            helpBtn->loadTextureNormal("RoomPublicH/dialog/dialog_rule_help_check.png");
            helpBtn->loadTexturePressed("RoomPublicH/dialog/dialog_rule_help_check.png");
            ruleBtn->loadTextureNormal("RoomPublicH/dialog/dialog_rule_rule.png");
            ruleBtn->loadTexturePressed("RoomPublicH/dialog/dialog_rule_rule.png");
        }
        else if(name == "rule_rule"){
            //显示规则
            webView->setVisible(false);
            ruleListView->setVisible(true);
            helpBtn->loadTextureNormal("RoomPublicH/dialog/dialog_rule_help.png");
            helpBtn->loadTexturePressed("RoomPublicH/dialog/dialog_rule_help.png");
            ruleBtn->loadTextureNormal("RoomPublicH/dialog/dialog_rule_rule_check.png");
            ruleBtn->loadTexturePressed("RoomPublicH/dialog/dialog_rule_rule_check.png");
        }
    }
}

