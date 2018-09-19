//
//  RoomHelp.cpp
//  ZJH
//
//  Created by hyhy on 16/7/25.
//
//

#include "RoomHelp.h"

USING_NS_CC;

bool RoomHelp::init()
{
    if (!Layer::init()) {
        return false;
    }
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

bool RoomHelp::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Node *dialog = dynamic_cast<Sprite *>(this->getChildByName("dialog"));
    if (dialog && dialog->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())))
    {
        //
    }
    else
    {
        this->removeFromParentAndCleanup(true);
        event->stopPropagation();
    }
    
    return true;
}

void RoomHelp::initHelp(Json::Value &json)
{
    
    initHelpWeb();
}


void RoomHelp::initHelpWeb()
{
    
    Size winSize = Director::getInstance()->getWinSize();
    Size bgSize = Size(630,926);
    
    Layout *mask = Layout::create();
    mask->setContentSize(winSize);
    mask->setTouchEnabled(true);
    mask->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    mask->setBackGroundColor(Color3B(0, 0, 0));
    mask->setOpacity(150);
    mask->addClickEventListener([=](Ref *ref)
                                    {
                                        this->removeFromParentAndCleanup(true);
                                    });
    this->addChild(mask);
    
    ImageView *dialog = ImageView::create("RoomPublicV/game_help_dialog.png");
    dialog->setScale9Enabled(true);
    dialog->setTouchEnabled(true);
    dialog->ignoreContentAdaptWithSize(false);
    dialog->setAnchorPoint(Vec2(0.5,0.5));
    dialog->setSize(bgSize);
    dialog->setPosition(winSize / 2);
    dialog->setName("dialog");
    this->addChild(dialog);
    
    Button *ruleBtn = Button::create("RoomPublicV/rule.png", "RoomPublicV/rule.png","RoomPublicV/rule_select.png");
    ruleBtn->setAnchorPoint(Vec2(1,1));
    ruleBtn->setPosition(Vec2(bgSize.width / 2,bgSize.height - 38));
    ruleBtn->setEnabled(false);
    dialog->addChild(ruleBtn);
    
    Button *configBtn = Button::create("RoomPublicV/config_btn.png", "RoomPublicV/config_btn.png","RoomPublicV/config_btn_press.png");
    configBtn->setAnchorPoint(Vec2(0,1));
    configBtn->setPosition(Vec2(bgSize.width / 2,bgSize.height - 38));
    configBtn->setEnabled(true);
    dialog->addChild(configBtn);
    
    ImageView *middle = ImageView::create("RoomPublicV/middle_bg.png");
    middle->setAnchorPoint(Vec2(0.5, 0));
    middle->setScale9Enabled(true);
    middle->ignoreContentAdaptWithSize(true);
    middle->setPosition(Vec2(bgSize.width / 2, 25));
    middle->setContentSize(Size(585,795));

    
    cocos2d::experimental::ui::WebView* webView = cocos2d::experimental::ui::WebView::create();
    webView->setAnchorPoint(Vec2(0, 0));
    webView->setPosition(Vec2(0,0));
    webView->setContentSize(middle->getContentSize());
    webView->setColor(Color3B(0x14, 0x14, 0x15));
    webView->setScalesPageToFit(true);
    webView->loadURL(ZJHModel::getInstance()->gameHelpUrl);
    webView->setName("webview");
    middle->addChild(webView);
    
    
    dialog->addChild(middle);
    
    
    ListView* listView = createConfigView(middle->getContentSize());
    listView->setVisible(false);
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0,0));
    listView->setBounceEnabled(false);
    middle->addChild(listView);

    
    ruleBtn->addClickEventListener([=](Ref *ref)
                                   {
                                       auto btn = (Button*)ref;
                                       if(btn->isEnabled()){
                                           btn->setEnabled(false);
                                           configBtn->setEnabled(true);
                                           middle->getChildByName("webview")->setVisible(true);
                                           middle->getChildByName("listview")->setVisible(false);
                                       }
                                   });
    
    configBtn->addClickEventListener([=](Ref *ref)
                                     {
                                         auto btn = (Button*)ref;
                                         if(btn->isEnabled()){
                                             ruleBtn->setEnabled(true);
                                             btn->setEnabled(false);
                                             middle->getChildByName("webview")->setVisible(false);
                                             middle->getChildByName("listview")->setVisible(true);
                                         }
                                     });
    Button *closeBtn = Button::create("RoomPublicV/cancel.png", "RoomPublicV/cancel2.png");
    closeBtn->setPosition(Vec2(bgSize.width, bgSize.height));
    closeBtn->setTag(ALERT_DIALOG_BUTTON_CLOSE);
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->removeFromParentAndCleanup(true);
                                    });
    dialog->addChild(closeBtn);
}


ListView* RoomHelp::createConfigView(Size size){
    ListView *listV = ListView::create();
    listV->setBounceEnabled(true);
    listV->setScrollBarEnabled(false);
    listV->setDirection(ScrollView::Direction::VERTICAL);
    listV->setContentSize(Size(size.width, size.height));
    listV->setPosition(Vec2(10, 10));
    listV->setName("listview");
    listV->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    
    std::vector<int> IDList;
    std::vector<string> classIDList;
    int intK = 0;
    Json::Value roomData = ZJHModel::getInstance()->roomData["gameRoom"]["innerWayRecommend"][intK];
    
    Json::Value fistBao = roomData["gameCustomClassifyVOList"];
    for (int i = 0; i < fistBao.size(); i++)
    {
        if (fistBao[i]["platformVisible"].asBool() == false)
        {
            continue;
        }
        
        Json::Value pointJson = fistBao[i]["gameCustomPlayValInfoList"];
        
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
        cell->setContentSize(Size(size.width-40, 100 + (pointJson.size()-1)/3*60));
        listV->pushBackCustomItem(cell);
        
        
        Text *title = Text::create(roomData["gameCustomClassifyVOList"][i]["classifyName"].asString(), ".SFUIDisplay-Semibold", 24);
        title->setColor(Color3B(0xec, 0xd1, 0xa3));
        title->setAnchorPoint(Vec2(0, 0.5));
        title->setPosition(Vec2(20,  cell->getContentSize().height - 50));
        cell->addChild(title, 2);
    
        
        for (int j = 0; j < pointJson.size(); j++)
        {
            bool isSelect = true;
            if (pointJson[j]["iptValue"] == Json::Value::null)
            {
                isSelect = false;
            }
            
            Layout *selecBtn;
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
            selecBtn->setPosition(Vec2(20 + j%3*(165+15), cell->getContentSize().height - 60 - j/3*(50+15)));
            cell->addChild(selecBtn, 2);
            
        }
    }

    return listV;
}


Layout *RoomHelp::selectBtnDraw(string titleStr, bool selected, int cellType)
{
    Layout *btn = Layout::create();
    btn->setContentSize(Size(180, 60));
    
    Text *title = Text::create(titleStr, ".SFUIDisplay-Semibold", 20);
    title->setContentSize(Size(110, 50));
    title->setTextColor(Color4B(0x99, 0x99, 0x99, 255));
    title->setAnchorPoint(Vec2(0, 0.5));
    title->setPosition(Vec2(40, btn->getContentSize().height/2));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    btn->addChild(title);
    
    ImageView *kongImg = ImageView::create();
    kongImg->setAnchorPoint(Vec2(0, 0.5));
    kongImg->setPosition(Vec2(0, btn->getContentSize().height/2));
    btn->addChild(kongImg);
    
    ImageView *selelctImg = ImageView::create();
    selelctImg->setName("selecIcon");
    selelctImg->setAnchorPoint(Vec2(0, 0.5));
    selelctImg->setPosition(kongImg->getPosition());
    btn->addChild(selelctImg, 2);
    selelctImg->setVisible(selected);
    
    if(cellType == 2)
    {
        kongImg->loadTexture("RoomPublicV/room_point_2.png");
        selelctImg->loadTexture("RoomPublicV/room_point_2_check.png");
    }else
    {
        kongImg->loadTexture("RoomPublicV/room_config_check_press.png");
        selelctImg->loadTexture("RoomPublicV/room_config_check.png");
    }
    
    return  btn;
}
