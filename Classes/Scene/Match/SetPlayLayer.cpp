//
//  TipChangeEditLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/23.
//
//

#include "SetPlayLayer.hpp"
#include "PlayWebLayer.hpp"

SetPlayLayer::SetPlayLayer()
{
    listBg = NULL;
}
SetPlayLayer* SetPlayLayer::create()
{
    SetPlayLayer *pRet = new(std::nothrow) SetPlayLayer();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool SetPlayLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Size size = Size(900, visibleSize.height - 150);
    this->setContentSize(size);
    
    ImageView *bg = ImageView::create("Match/play_set_bg.png");
    bg->ignoreContentAdaptWithSize(false);
    bg->setContentSize(this->getContentSize());
    bg->setPosition(Vec2(size.width/2, size.height/2));
    this->addChild(bg);
    
    ImageView *headBg = ImageView::create("Match/match_play_setBg.png");
    headBg->setAnchorPoint(Vec2(0.5, 1));
    headBg->setPosition(Vec2(size.width/2, size.height - 40));
    this->addChild(headBg, 2);
    headBg->setTouchEnabled(true);
    headBg->addClickEventListener([=](Ref*ref){
        
        PlayWebLayer *web = PlayWebLayer::create();
        Director::getInstance()->getRunningScene()->addChild(web, 1000);
    });
    
    Button *sureBtn = Button::create("Public/btn_yellow.png","Public/btn_yellow.png");
    sureBtn->setScale9Enabled(true);
    sureBtn->setContentSize(Size(620, 100));
    sureBtn->setPosition(Vec2(size.width/2, 90));
    this->addChild(sureBtn, 10);
    sureBtn->addClickEventListener(CC_CALLBACK_1(SetPlayLayer::setFinishBtnOn, this));
    
    
    Text *sureTitle = Text::create("确认玩法", ".SFUIDisplay-Semibold", 40);
    sureTitle->setPosition(Vec2(sureBtn->getContentSize().width/2, sureBtn->getContentSize().height/2));
    sureTitle->setTextColor(Color4B(0x60, 0x41, 0x0a, 255));
    sureBtn->addChild(sureTitle);
    
    return true;
}

void SetPlayLayer::setFinishBtnOn(Ref*ref)
{
    string selecStr = "{";
    string selecShow = "#";
    int btnTag;
    std::vector<Button*> listBtn;
    Json::Value smallData;
    std::vector<string> selectStrList;
    
    map<std::string,vector<string>> hide_map;
    
    /**********************  1、选中按钮的选项  *******************************/
    char buf1[128];
    for (int i = 0; i < butMap.size(); i++)
    {
        listBtn = butMap[i];
        for (int j = 0; j < listBtn.size(); j++)
        {
            Button *curBtn = listBtn.at(j);
            btnTag = curBtn->getTag();
            ImageView *seleTemp = (ImageView*)curBtn->getChildByName("selecIcon");
            smallData = setData[btnTag/100]["gameCustomPlayInfoVOList"][btnTag%100];
            if (seleTemp->isVisible())
            {
                sprintf(buf1, "%d", smallData["playId"].asInt());
                string t = buf1;
                hide_map[smallData["classifyId"].asString()].push_back(t);
                //selecShow +=smallData["playName"].asString() + ", ";
            }
        }
    }
    
    /**********************  2、输入框类型的值的提取选项  *******************************/
    for (int i = 0; i < editBoxList.size(); i++)
    {
        EditBox *box = editBoxList[i];
        btnTag = box->getTag();
        smallData = setData[btnTag/100]["gameCustomPlayInfoVOList"][btnTag%100];
        sprintf(buf1, "%d_%d", smallData["playId"].asInt(), smallData["iptValue"].asInt());
        string t = buf1;
        hide_map[smallData["classifyId"].asString()].push_back(t);
        sprintf(buf1, "%s:%d", setData[btnTag/100]["classifyName"].asString().c_str(), smallData["iptValue"].asInt());
        t = buf1;
        //selecShow += t + ", ";
    }
    
    /**********************  3、下拉框类型的值的提取选项  *******************************/
    std::map<int, Layout*>::iterator itDown;
    for (itDown = downCell.begin(); itDown != downCell.end(); itDown++)
    {
        int key = itDown->first;
        smallData = setData[key/100]["gameCustomPlayInfoVOList"];
        for (int j = 0; j < smallData.size(); j++)
        {
            if (smallData[j]["selected"].asBool())
            {
                sprintf(buf1, "%d", smallData[j]["playId"].asInt());
                string t = buf1;
                hide_map[smallData[j]["classifyId"].asString()].push_back(t);
                sprintf(buf1, "%s:%s", setData[key/100]["classifyName"].asString().c_str(), smallData[j]["playName"].asString().c_str());
                t = buf1;
                //selecShow += t + ", ";
            }
        }
    }

    for (int k3 = 0; k3 < hideList.size(); k3++)
    {
        for (int k4 = 0; k4 < hideList[k3]["gameCustomPlayInfoVOList"].size(); k4++)
        {
            char buf1[128];
            string t = "";
            smallData = hideList[k3]["gameCustomPlayInfoVOList"][k4];
            
            if ((hideList[k3]["choiceType"].asInt() == 4) && (!smallData["iptValue"].isNull()))
            {
                sprintf(buf1, "%d_%d", smallData["playId"].asInt(), smallData["iptValue"].asInt());
                t = buf1;
                hide_map[smallData["classifyId"].asString()].push_back(t);
                
                if (!smallData["iptValue"].isNull())
                {
                    sprintf(buf1, "%s:%d", smallData["playName"].asString().c_str(), smallData["iptValue"].asInt());
                }
                t = buf1;
                //selecShow += t + ", ";
                
            }else
            {
                if (smallData["selected"].asBool())
                {
                    sprintf(buf1, "%d", smallData["playId"].asInt());
                    t = buf1;
                    hide_map[smallData["classifyId"].asString()].push_back(t);
                    
                    t = smallData["playName"].asString();
                    if (hideList[k3]["choiceType"].asInt() == 3)
                    {
                        //下拉框
                        sprintf(buf1, "%s:%s", hideList[k3]["classifyName"].asString().c_str(), smallData["playName"].asString().c_str());
                        t = buf1;
                    }
                    
                    //selecShow += t + ", ";
                }
                
            }

        }
    }
    
    map<std::string,vector<string>>::iterator it;
    for (it = hide_map.begin(); it != hide_map.end(); it ++)
    {
        string key = it->first;
        vector<string> v_list = it->second;
        if (v_list.size() == 0)
        {
            continue;
        }
        string value = "";
        for (int i = 0; i < v_list.size(); i++)
        {
            value += v_list[i];
            if (i != v_list.size() - 1)
            {
                value += ",";
            }
        }
        
        string final_str = "\"" +  key + "\":\"" + value + "\"";
        selectStrList.push_back(final_str);
    }
    
    for (int k = 0; k < selectStrList.size(); k++)
    {
        if (k < selectStrList.size() -1)
        {
            selecStr += selectStrList.at(k) + ",";
        }else if (k == selectStrList.size() -1)
        {
            selecStr += selectStrList.at(k) + "}";
        }
        
    }
    
    for (int i = 0; i < setData.size(); i++)
    {
        Json::Value pointJson = setData[i]["gameCustomPlayInfoVOList"];
        
        for (int j = 0; j < pointJson.size(); j++)
        {
            if(setData[i]["choiceType"] == 4 && !pointJson[j]["iptValue"].isNull())
            {
                sprintf(buf1, "%d_%d", pointJson[j]["playId"].asInt(), pointJson[j]["iptValue"].asInt());
            }else
            {
                sprintf(buf1, "%d", pointJson[j]["playId"].asInt());
            }
            
            map<std::string,vector<string>>::iterator itShow;
            for (itShow = hide_map.begin(); itShow != hide_map.end(); itShow++)
            {
                string key = itShow->first;
                vector<string> v_list = itShow->second;
                if (v_list.size() == 0)
                {
                    continue;
                }
                for (int k = 0; k < v_list.size(); k++)
                {
                    if (buf1 == v_list[k])
                    {
                        string t;
                        if(setData[i]["choiceType"] == 4 && !pointJson[j]["iptValue"].isNull())
                        {
                            sprintf(buf1, "%s:%d", setData[i]["classifyName"].asString().c_str(), pointJson[j]["iptValue"].asInt());
                            t = buf1;
                        }else if(setData[i]["choiceType"] == 3)
                        {
                            sprintf(buf1, "%s:%s", setData[i]["classifyName"].asString().c_str(), pointJson[j]["playName"].asString().c_str());
                            
                            t = buf1;
                        }
                        else
                        {
                            t = pointJson[j]["playName"].asString();
                        }
                        selecShow += t + ", ";
                    }
                    
                }
            }
            
        }
    }
    
    selecStr += selecShow;
    
    __NotificationCenter::getInstance()->postNotification("play_setString", __String::create(selecStr));
    this->setVisible(false);
}

void SetPlayLayer::refreshListView()
{
    listV->removeAllChildren();
    butMap.clear();
    hideList.clear();
    editBoxList.clear();
    isDownShow.clear();
    downCell.clear();
    
    Size size = Size(830, this->getContentSize().height - 250 - 140);
    
    for (int i = 0; i < setData.size(); i++)
    {
        if(setData[i]["tribeVisible"].asBool() || setData[i]["platformVisible"].asBool())
        {
            hideList.push_back(setData[i]);
            continue;
        }
        
        Json::Value pointJson = setData[i]["gameCustomPlayInfoVOList"];
        
        auto cell = Layout::create();
        
        std::vector<Button*> tempBtn;
        int countT;
        bool isHide = false;
        
        for (int j = 0; j < pointJson.size(); j++)
        {
            countT = 0;
            isHide = false;
            for(int k1 = 0; k1 < setData.size(); k1++)
            {
                Json::Value pointData = setData[k1]["gameCustomPlayInfoVOList"];
                
                for(int k2 = 0; k2 < pointData.size(); k2++)
                {
                    if(pointJson[j]["premisePlayId"].asInt() == pointData[k2]["playId"].asInt() && pointData[k2]["selected"].asBool() == false)
                    {
                        countT++;
                        isHide = true;
                        break;
                    }
                }
            }
            
            if(isHide)
            {
                continue;
            }
            
            if (setData[i]["choiceType"].asInt() == 1 || setData[i]["choiceType"].asInt() == 2)
            {
                Button *selecBtn = selectBtnDraw(pointJson[j]["playName"].asString(), pointJson[j]["selected"].asBool(), setData[i]["choiceType"].asInt());
                selecBtn->setTag(100*i+j);
                selecBtn->setAnchorPoint(Vec2(0, 1));
                selecBtn->addClickEventListener(CC_CALLBACK_1(SetPlayLayer::selectBtnOn, this));
                tempBtn.push_back(selecBtn);
                
            }else if (setData[i]["choiceType"].asInt() == 4)
            {
                listV->pushBackCustomItem(cell);
                cell->setContentSize(Size(size.width, 170));
                
                ImageView *cellBg = ImageView::create("Match/match_item_bg.png");
                cellBg->setScale9Enabled(true);
                cellBg->ignoreContentAdaptWithSize(false);
                cellBg->setContentSize(cell->getContentSize());
                cellBg->setAnchorPoint(Vec2(0, 0));
                cellBg->setPosition(Vec2(0, 0));
                cell->addChild(cellBg);
                
                LabelTTF *title =  LabelTTF::create(setData[i]["classifyName"].asString(), ".SFUIDisplay-Semibold", 40);
                title->setColor(Color3B(0xe0, 0xcf, 0xac));
                title->setAnchorPoint(Vec2(0, 1));
                title->setPosition(Vec2(30,  cell->getContentSize().height - 10));
                cell->addChild(title, 2);
                
                type4Edit(cell, pointJson[j], 100*i+j);
                
            }else if (setData[i]["choiceType"].asInt() == 3 && pointJson[j]["selected"].asBool())
            {
                listV->pushBackCustomItem(cell);
                cell->setContentSize(Size(size.width, 170));
                
                ImageView *cellBg = ImageView::create("Match/match_item_bg.png");
                cellBg->setScale9Enabled(true);
                cellBg->ignoreContentAdaptWithSize(false);
                cellBg->setContentSize(cell->getContentSize());
                cellBg->setAnchorPoint(Vec2(0, 0));
                cellBg->setPosition(Vec2(0, 0));
                cell->addChild(cellBg);
                
                LabelTTF *title =  LabelTTF::create(setData[i]["classifyName"].asString(), ".SFUIDisplay-Semibold", 40);
                title->setColor(Color3B(0xe0, 0xcf, 0xac));
                title->setAnchorPoint(Vec2(0, 1));
                title->setPosition(Vec2(30,  cell->getContentSize().height - 10));
                cell->addChild(title, 2);
                
                type3Down(cell, pointJson[j], 100*i);
            }

            
        }
        
        if (setData[i]["choiceType"].asInt() == 1 || setData[i]["choiceType"].asInt() == 2)
        {
            if(tempBtn.size() > 0)
            {
                listV->pushBackCustomItem(cell);
                cell->setContentSize(Size(size.width, 150 + (tempBtn.size()-1)/3*120));
                
                LabelTTF *title =  LabelTTF::create(setData[i]["classifyName"].asString(), ".SFUIDisplay-Semibold", 40);
                title->setColor(Color3B(0xe0, 0xcf, 0xac));
                title->setAnchorPoint(Vec2(0, 1));
                title->setPosition(Vec2(30,  cell->getContentSize().height - 10));
                cell->addChild(title, 2);
                
                butMap[i] = tempBtn;
                
                for (int k3 = 0; k3 < tempBtn.size(); k3++)
                {
                    Button *temp = tempBtn.at(k3);
                    temp->setPosition(Vec2(30 + k3%3*(250+15), cell->getContentSize().height - 70 - k3/3*(70+30)));
                    cell->addChild(temp, 2);
                }
                ImageView *cellBg = ImageView::create("Match/match_item_bg.png");
                cellBg->ignoreContentAdaptWithSize(false);
                cellBg->setContentSize(cell->getContentSize());
                cellBg->setAnchorPoint(Vec2(0, 0));
                cellBg->setPosition(Vec2(0, 0));
                cell->addChild(cellBg);
            }
        }
    }
    
}

void SetPlayLayer::showView(Json::Value wayJson)
{
    setData = wayJson;
    Size size = Size(830, this->getContentSize().height - 250 - 140);
    listV = ListView::create();
    listV->setBounceEnabled(true);
    listV->setScrollBarEnabled(false);
    listV->setDirection(ScrollView::Direction::VERTICAL);
    listV->setContentSize(size);
    listV->setAnchorPoint(Vec2(0, 1));
    listV->setPosition(Vec2(50, this->getContentSize().height - 250));
    listV->setItemsMargin(25);
    listV->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(listV);
    
    for (int i = 0; i < wayJson.size(); i++)
    {
        if(wayJson[i]["tribeVisible"].asBool() || wayJson[i]["platformVisible"].asBool())
        {
            hideList.push_back(wayJson[i]);
            continue;
        }
        Json::Value pointJson = wayJson[i]["gameCustomPlayInfoVOList"];
        
        std::vector<Button*> tempBtn;
        int countT;
        bool isHide = false;
        
        for (int j = 0; j < pointJson.size(); j++)
        {
            countT = 0;
            isHide = false;
            for(int k1 = 0; k1 < wayJson.size(); k1++)
            {
                Json::Value pointData = wayJson[k1]["gameCustomPlayInfoVOList"];
                
                for(int k2 = 0; k2 < pointData.size(); k2++)
                {
                    if(pointJson[j]["premisePlayId"].asInt() == pointData[k2]["playId"].asInt() && pointData[k2]["selected"].asBool() == false)
                    {
                        countT++;
                        isHide = true;
                        break;
                    }
                }
            }
            
            if(isHide)
            {
                continue;
            }
            if (wayJson[i]["choiceType"].asInt() == 1 || wayJson[i]["choiceType"].asInt() == 2)
            {
                Button *selecBtn = selectBtnDraw(pointJson[j]["playName"].asString(), pointJson[j]["selected"].asBool(), wayJson[i]["choiceType"].asInt());
                selecBtn->setTag(100*i+j);
                selecBtn->setAnchorPoint(Vec2(0, 1));
                selecBtn->addClickEventListener(CC_CALLBACK_1(SetPlayLayer::selectBtnOn, this));
                tempBtn.push_back(selecBtn);
                
            }else if (wayJson[i]["choiceType"].asInt() == 4)
            {
                auto cell = Layout::create();
                listV->pushBackCustomItem(cell);
            
                cell->setContentSize(Size(size.width, 170));
                
                ImageView *cellBg = ImageView::create("Match/match_item_bg.png");
                cellBg->setScale9Enabled(true);
                cellBg->ignoreContentAdaptWithSize(false);
                cellBg->setContentSize(cell->getContentSize());
                cellBg->setAnchorPoint(Vec2(0, 0));
                cellBg->setPosition(Vec2(0, 0));
                cell->addChild(cellBg);
                
                LabelTTF *title =  LabelTTF::create(setData[i]["classifyName"].asString(), ".SFUIDisplay-Semibold", 40);
                title->setColor(Color3B(0xe0, 0xcf, 0xac));
                title->setAnchorPoint(Vec2(0, 1));
                title->setPosition(Vec2(30,  cell->getContentSize().height - 10));
                cell->addChild(title, 2);
                
                type4Edit(cell, pointJson[j], 100*i+j);
                
            }else if (wayJson[i]["choiceType"].asInt() == 3 && pointJson[j]["selected"].asBool())
            {
                auto cell = Layout::create();
                listV->pushBackCustomItem(cell);
                cell->setContentSize(Size(size.width, 170));
                
                ImageView *cellBg = ImageView::create("Match/match_item_bg.png");
                cellBg->setScale9Enabled(true);
                cellBg->ignoreContentAdaptWithSize(false);
                cellBg->setContentSize(cell->getContentSize());
                cellBg->setAnchorPoint(Vec2(0, 0));
                cellBg->setPosition(Vec2(0, 0));
                cell->addChild(cellBg);
                
                LabelTTF *title =  LabelTTF::create(setData[i]["classifyName"].asString(), ".SFUIDisplay-Semibold", 40);
                title->setColor(Color3B(0xe0, 0xcf, 0xac));
                title->setAnchorPoint(Vec2(0, 1));
                title->setPosition(Vec2(30,  cell->getContentSize().height - 10));
                cell->addChild(title, 2);

                type3Down(cell, pointJson[j], 100*i);
            }
            
            
        }
        
        if(wayJson[i]["choiceType"].asInt() == 1 || wayJson[i]["choiceType"].asInt() == 2)
        {
            if(tempBtn.size() > 0)
            {
                auto cell = Layout::create();
                listV->pushBackCustomItem(cell);
                cell->setContentSize(Size(size.width, 150 + (tempBtn.size()-1)/3*120));
                
                LabelTTF *title =  LabelTTF::create(wayJson[i]["classifyName"].asString(), ".SFUIDisplay-Semibold", 40);
                title->setColor(Color3B(0xe0, 0xcf, 0xac));
                title->setAnchorPoint(Vec2(0, 1));
                title->setPosition(Vec2(30,  cell->getContentSize().height - 10));
                cell->addChild(title, 2);
                
                butMap[i] = tempBtn;
                
                for (int k3 = 0; k3 < tempBtn.size(); k3++)
                {
                    Button *temp = tempBtn.at(k3);
                    temp->setPosition(Vec2(30 + k3%3*(250+15), cell->getContentSize().height - 70 - k3/3*(70+30)));
                    cell->addChild(temp, 2);
                }
                ImageView *cellBg = ImageView::create("Match/match_item_bg.png");
                cellBg->setScale9Enabled(true);
                cellBg->ignoreContentAdaptWithSize(false);
                cellBg->setContentSize(cell->getContentSize());
                cellBg->setAnchorPoint(Vec2(0, 0));
                cellBg->setPosition(Vec2(0, 0));
                cell->addChild(cellBg);
            }
            
        }
        
    }
}

void SetPlayLayer::selectBtnOn(Ref*ref)
{
    Button *btn = (Button*)ref;
    ImageView *selecIcon = (ImageView*)btn->getChildByName("selecIcon");
    
    int tag = btn->getTag();
    
    if (setData[tag/100]["choiceType"].asInt() == 1)
    {
        std::vector<Button *> btnList = butMap[tag/100];
        for (int i = 0; i < btnList.size(); i++)
        {
            Button *btnTemp = btnList[i];
            ImageView *seleTemp = (ImageView*)btnTemp->getChildByName("selecIcon");
            seleTemp->setVisible(false);
            int selTag = btnTemp->getTag();
            
            setData[selTag/100]["gameCustomPlayInfoVOList"][selTag%100]["selected"] = false;
        }
        //单选
        selecIcon->setVisible(true);
        setData[tag/100]["gameCustomPlayInfoVOList"][tag%100]["selected"] = true;
        
    }else if (setData[tag/100]["choiceType"].asInt() == 2)
    {
        bool isSele = !(selecIcon->isVisible());
        //多选
        selecIcon->setVisible(isSele);
        setData[tag/100]["gameCustomPlayInfoVOList"][tag%100]["selected"] = isSele;
    }
    
    refreshListView();
}

Button *SetPlayLayer::selectBtnDraw(string titleStr, bool selected, int cellType)
{
    Button *btn = Button::create("Match/set_mes.png", "Match/set_mes_notcheck.png", "Match/set_mes_notcheck.png");
    btn->setScale9Enabled(true);
    btn->ignoreContentAdaptWithSize(false);
    btn->setScale9Enabled(true);
    btn->setContentSize(Size(250, 70));
    
    Text *title = Text::create(titleStr, ".SFUIDisplay-Semibold", 35);
    title->setContentSize(Size(145, 80));
    title->setTextColor(Color4B(0x99, 0x99, 0x99, 255));
    title->setAnchorPoint(Vec2(0, 0.5));
    title->setPosition(Vec2(50, btn->getContentSize().height/2));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    btn->addChild(title);
    
    ImageView *kongImg = ImageView::create();
    kongImg->setAnchorPoint(Vec2(0, 0.5));
    kongImg->setPosition(Vec2(10, btn->getContentSize().height/2));
    btn->addChild(kongImg);
    
    ImageView *selelctImg = ImageView::create();
    selelctImg->setName("selecIcon");
    selelctImg->setAnchorPoint(Vec2(0, 0.5));
    selelctImg->setPosition(kongImg->getPosition());
    btn->addChild(selelctImg, 2);
    selelctImg->setVisible(selected);
    
    if (cellType == 1)
    {
        kongImg->loadTexture("Match/point_hold_kong2.png");
        selelctImg->loadTexture("Match/play_point_check_sigle_can.png");
        
    }else if (cellType == 2)
    {
        kongImg->loadTexture("Match/point_hold_kong1.png");
        selelctImg->loadTexture("Match/play_point_check_can.png");
    }
    return  btn;
    
    
}
void SetPlayLayer::type3Down(Layout *cell, Json::Value json, int tag)
{
    //下拉
    Size sizeM = Size(330, 75);
    ImageView *downBg = ImageView::create("Match/set_text_Bg.png");
    downBg->setScale9Enabled(true);
    downBg->setTag(150);
    downBg->setContentSize(sizeM);
    downBg->setAnchorPoint(Vec2(0, 1));
    downBg->setPosition(Vec2(30, cell->getContentSize().height - 70));
    cell->addChild(downBg);
    
    Text *showText = Text::create("", ".SFUIDisplay-Semibold", 40);
    showText->setTextColor(Color4B(0x4c, 0x4c, 0x4c, 255));
    showText->setTag(250);
    showText->setPosition(Vec2((sizeM.width - 70)/2, sizeM.height/2));
    showText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    downBg->addChild(showText, 100);
    
    if (json["selected"].asBool())
    {
        showText->setString(json["playName"].asString());
    }

    isDownShow[tag] = false;
    CallFunc* func1 = CallFunc::create([=]
                                       {
                                           Size size = Size(330, 207);
                                           
                                           ImageView *listBg = ImageView::create("Match/set_down_listBg.png");
                                           listBg->setScale9Enabled(true);
                                           listBg->setTag(tag);
                                           listBg->setSwallowTouches(true);
                                           listBg->setContentSize(size);
                                           
                                           if (cell->getWorldPosition().y <= 200)
                                           {
                                               listBg->setAnchorPoint(Vec2(0, 0));
                                               listBg->setPosition(Vec2(cell->getWorldPosition().x-100, cell->getWorldPosition().y + 100));
                                           }else
                                           {
                                               listBg->setAnchorPoint(Vec2(0, 1));
                                               listBg->setPosition(Vec2(cell->getWorldPosition().x-100, cell->getWorldPosition().y + 25));
                                           }
                                           
                                           this->addChild(listBg, 20);
                                           listBg->setVisible(false);
                                           
                                           ListView *list_1 = ListView::create();
                                           list_1->setBounceEnabled(true);
                                           list_1->setScrollBarEnabled(false);
                                           list_1->setDirection(ScrollView::Direction::VERTICAL);
                                           list_1->setContentSize(Size(size.width - 10, size.height - 10));
                                           list_1->setPosition(Vec2(5, 5));
                                           list_1->setItemsMargin(0);
                                           list_1->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
                                           listBg->addChild(list_1);
                                           
                                           for (int i = 0; i < setData[tag/100]["gameCustomPlayInfoVOList"].size(); i++)
                                           {
                                               auto cellTemp = Layout::create();
                                               cellTemp->setTag(tag+i);
                                               cellTemp->setTouchEnabled(true);
                                               cellTemp->addClickEventListener(CC_CALLBACK_1(SetPlayLayer::onClickDownItem, this));
                                               cellTemp->setContentSize(Size(size.width, 70));
                                               list_1->addChild(cellTemp);
                                               
                                               auto line1 = Layout::create();
                                               line1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                                               line1->setBackGroundColor(Color3B(0xb3, 0xb3, 0xb3));
                                               line1->setPosition(Vec2(10, 0));
                                               line1->setContentSize(Size(size.width - 20, 2));
                                               cellTemp->addChild(line1);
                                               
                                               Text *showText = Text::create(setData[tag/100]["gameCustomPlayInfoVOList"][i]["playName"].asString(), ".SFUIDisplay-Semibold", 35);
                                               showText->setTextColor(Color4B(0x4c, 0x4c, 0x4c, 255));
                                               showText->setAnchorPoint(Vec2(0, 0));
                                               showText->setPosition(Vec2(15, 10));
                                               showText->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
                                               cellTemp->addChild(showText, 2);
                                           }

                                       });
    
    runAction(Sequence::create(DelayTime::create(0.3),func1, NULL));

    Button *downBtn = Button::create("Match/set_down_btn.png", "Match/set_down_btn.png", "Match/set_down_btn.png");
    downBtn->setAnchorPoint(Vec2(1, 0.5));
    downBtn->setPosition(Vec2(330, 75/2.0));
    downBtn->setTag(tag);
    downBtn->addClickEventListener(CC_CALLBACK_1(SetPlayLayer::showDownListView, this));
    downBg->addChild(downBtn);
    
    downCell[tag] = cell;
    
}
void SetPlayLayer::onClickDownItem(Ref*ref)
{
     Layout *item = (Layout *)ref;
     int tag = item->getTag();
     Layout *cellTemp = downCell[tag/100*100];
     ImageView *bg = (ImageView *)cellTemp->getChildByTag(150);
     Text *showText = (Text *)bg->getChildByTag(250);

    showText->setString(setData[tag/100]["gameCustomPlayInfoVOList"][tag%100]["playName"].asString());
    
    for (int j = 0; j < setData[tag/100]["gameCustomPlayInfoVOList"].size(); j++)
    {
        setData[tag/100]["gameCustomPlayInfoVOList"][j]["selected"] = false;
    }
    
    setData[tag/100]["gameCustomPlayInfoVOList"][tag%100]["selected"] = true;
    
    std::map<int, bool>::iterator it;
   
    for (it = isDownShow.begin(); it != isDownShow.end(); it++)
    {
        int key = it->first;
        ImageView *listBgT = (ImageView *)this->getChildByTag(key);
        listBgT->removeFromParent();
    }
    char buf[128];
    sprintf(buf, "%d", tag/100*100);
    Layout *lay = (Layout*)this->getChildByName(buf);
    lay->removeFromParent();
    
    refreshListView();
    
}

void SetPlayLayer::closeSelectView(Ref*ref)
{
    Layout *lay = (Layout*)ref;
    int downTag = atoi(lay->getName().c_str());
    ImageView *listBg = (ImageView *)this->getChildByTag(downTag);
    listBg->setVisible(false);
    isDownShow[downTag] = false;
    lay->removeFromParent();
    
}
void SetPlayLayer::showDownListView(Ref*ref)
{
    Button *downBtn = (Button *)ref;
    int downTag = downBtn->getTag();
    isDownShow[downTag] = !isDownShow[downTag];
    bool isShow = isDownShow[downTag];
    ImageView *listBg = (ImageView *)this->getChildByTag(downTag);
    if (isShow)
    {
        Layout *cell = downCell[downTag];
        
        if (cell->getWorldPosition().y <= 200)
        {
            listBg->setAnchorPoint(Vec2(0, 0));
            listBg->setPosition(Vec2(cell->getWorldPosition().x-100, cell->getWorldPosition().y + 100));
        }else
        {
            listBg->setAnchorPoint(Vec2(0, 1));
            listBg->setPosition(Vec2(cell->getWorldPosition().x-100, cell->getWorldPosition().y + 25));
        }

        
        listBg->setVisible(true);
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Layout *layout = Layout::create();
        char buf[128];
        sprintf(buf, "%d", downTag);
        layout->setName(buf);
        layout->setTouchEnabled(true);
        layout->addClickEventListener(CC_CALLBACK_1(SetPlayLayer::closeSelectView, this));
        layout->setContentSize(visibleSize);
        this->addChild(layout, 15);
    }else
    {
        listBg->setVisible(false);
    }
}
void SetPlayLayer::type4Edit(Layout *cell, Json::Value json, int tag)
{
    EditBox* edit = EditBox::create(Size(300, 70), "Match/set_text_Bg.png");
    edit->setPlaceholderFontSize(40);
    edit->setAnchorPoint(Vec2(0, 1));
    edit->setMaxLength(11);
    edit->setInputMode(EditBox::InputMode::NUMERIC);
    edit->setPosition(Vec2(35, cell->getContentSize().height - 70));
    edit->setFontColor(Color3B(0x99, 0x99, 0x99));
    edit->setPlaceholderFontColor(Color3B(0x4c, 0x4c, 0x4c));
    edit->setFontSize(40);
    edit->setDelegate(this);
    edit->setTag(tag);
    cell->addChild(edit, 2);
    
    editBoxList.push_back(edit);
    
    char buf[128];
    if (json["iptValue"].isNull())
    {
        json["iptValue"] = json["iptLower"];
    }
    
    sprintf(buf, "%d", json["iptValue"].asInt());
    
    edit->setText(buf);
    sprintf(buf, "输入范围：%d~%d数字", json["iptLower"].asInt(), json["iptUpper"].asInt());
    
    Text *fanWei = Text::create(buf, ".SFUIDisplay-Semibold", 35);
    fanWei->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
    fanWei->setAnchorPoint(Vec2(0, 0));
    fanWei->setPosition(Vec2(350, 30));
    fanWei->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    cell->addChild(fanWei, 2);
}

void SetPlayLayer::editBoxReturn(EditBox* editBox)
{
    
}

void SetPlayLayer::editBoxEditingDidBegin(EditBox* editBox)
{
    editBox->setText("");
}

void SetPlayLayer::editBoxEditingDidEnd(EditBox* editBox)
{
    int editTag = editBox->getTag();
    string valueStr = editBox->getText();
    int value = atoi(valueStr.c_str());
    Json::Value tempJson = setData[editTag/100]["gameCustomPlayInfoVOList"][editTag%100];
    
    if(value >= tempJson["iptLower"].asInt() && value <= tempJson["iptUpper"].asInt())
    {
        setData[editTag/100]["gameCustomPlayInfoVOList"][editTag%100]["iptValue"] = value;
    }else
    {
        char buf[128];
        sprintf(buf, "%d", tempJson["iptValue"].asInt());
        CallFunc* func1 = CallFunc::create([=]
                                           {
                                               editBox->setText(buf);
                                           });
        
        runAction(Sequence::create(DelayTime::create(1.0),func1, NULL));
        
        sprintf(buf, "请输入范围：%d~%d的数字", tempJson["iptLower"].asInt(), tempJson["iptUpper"].asInt());
        PlatformHelper::showToast(buf);
        
    }
    
}


void SetPlayLayer::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    
}