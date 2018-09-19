//
//  GoldShopLayer.cpp
//  ZJH
//
//  Created by apple on 16/8/29.
//
//

#include "GrowTaskLayer.hpp"
#include "ReciveEffectTipLayer.hpp"

bool GrowTaskLayer::init()
{
    if (!Widget::init())
    {
        return false;
    }
    this->setContentSize(Size(800, 960));
    
    Json::Value json2;
    json2["type"] = 2;
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"task/taskList",json2.toStyledString(),"taskList2");
    this->schedule(schedule_selector(GrowTaskLayer::postTask2), 0.0f);

    loading = Loading::startLoading(this, this->getContentSize()/2);
    
    return true;
}

void GrowTaskLayer::postTask2(float dt)
{
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["taskList2"];
    if (packet && packet->status != 0)
    {
        loading->removeFromParent();
        unschedule(schedule_selector(GrowTaskLayer::postTask2));
        if (packet->status != 3)
        {
            return;
        }
        packet->status = 0;
        
        if (packet->resultIsOK())
        {
            Json::Value data = packet->recvVal["resultMap"]["list"];
            showView1(data);
            log("任务列表2：＝＝＝ [%s]",data.toStyledString().c_str());
        }
        else
        {
            PlatformHelper::showToast(packet->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets["taskList2"] = NULL;
        delete packet;
    }
}

void GrowTaskLayer::showView1(Json::Value list)
{
    ListView *listView1 = ListView::create();
    listView1->setDirection(ScrollView::Direction::VERTICAL);
    listView1->setBounceEnabled(true);
    listView1->setContentSize(this->getContentSize());
    listView1->setAnchorPoint(Vec2(0, 0));
    listView1->setPosition(Vec2(0, 0));
    listView1->setItemsMargin(20);
    listView1->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(listView1);
    
    taskList = list;
    for (int i = 0; i < list.size(); i++)
    {
        Layout *item = createItem(list[i], i);
        listView1->pushBackCustomItem(item);
    }
    
}

Layout *GrowTaskLayer::createItem(Json::Value json, int index)
{
    Size itemSize = Size(810, 140);
    
    auto item = Layout::create();
    item->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    item->setBackGroundColor(Color3B(0x24, 0x24, 0x24));
    item->setContentSize(itemSize);
    
    Text *title = Text::create(json["taskName"].asString(), "", 36);
    title->setAnchorPoint(Vec2(0, 1));
    title->setTextColor(Color4B(0xa6, 0xa6, 0xa6, 255));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    title->setPosition(Vec2(35, itemSize.height - 25));
    item->addChild(title);
    
    //任务奖励描述
    char moneyS[100];
    if (json["moneyType"].asInt() == 1)
    {
        //1:金币奖励
        sprintf(moneyS, "金币 +%d",  json["recieveCount"].asInt());
    }else if (json["moneyType"].asInt() == 2)
    {
        //2:玩贝奖励
        sprintf(moneyS, "钻石 +%d",  json["recieveCount"].asInt());
    }else
    {
        //3:房卡
        sprintf(moneyS, "房卡 +%d",  json["recieveCount"].asInt());
    }
    
    RichText *record = RichText::create();
    record->cocos2d::ui::Widget::ignoreContentAdaptWithSize(true);
    record->setAnchorPoint(Vec2(0, 0));
    record->setPosition(Vec2(40, 20));
    item->addChild(record);
    
    RichElementText* re1 = RichElementText::create(1, Color3B(0x65, 0x65, 0x65), 255, "奖励：", "", 36);
    RichElementText *re2 = RichElementText::create(2, Color3B(0xf1, 0xcb, 0x5b), 255, moneyS, "", 36);
    
    record->pushBackElement(re1);
    record->pushBackElement(re2);
    
    Button * btn = Button::create("Mine/maskBut_n@2x.png","Mine/maskBut_s@2x.png");
    btn->setContentSize(Size(170, 105));
    btn->setAnchorPoint(Vec2(1, 0.5));
    btn->setScale9Enabled(true);
    btn->setPosition(Vec2(itemSize.width - 15, itemSize.height/2));
    item->addChild(btn);
    btn->setTitleFontSize(40);
    btn->setTitleFontName("");
    
    //领取状态
    switch (json["status"].asInt())
    {
        case 0:
            //未完成
            btn->setTitleText("未完成");
            btn->setTitleColor(Color3B(0xa6, 0xa6, 0xa6));
            btn->addClickEventListener([=](Ref*ref){
                if (ZJHModel::getInstance()->isYoukeLogin())
                {
                    ZJHModel::getInstance()->gotoView(VIEW_LOAD);
                }else
                {
                    
                }
            });
            break;
        case 1:
            //领取
            btn->setTitleText("领取");
            btn->setTag(100+index);
            btn->setTitleColor(Color3B(0x66, 0x41, 0x05));
            btn->addClickEventListener(CC_CALLBACK_1(GrowTaskLayer::receiveOnFun, this));
            break;
        case 2:
            //已领取
            btn->setTitleText("已领取");
            btn->setTitleColor(Color3B(0xa6, 0xa6, 0xa6));
            break;
        default:
            break;
    }
    
    RichText *timesText = RichText::create();
    timesText->cocos2d::ui::Widget::ignoreContentAdaptWithSize(true);
    timesText->setAnchorPoint(Vec2(1, 0.5));
    timesText->setPosition(Vec2(this->getContentSize().width - 225, itemSize.height/2));
    item->addChild(timesText);
    
    char buf[100];
    sprintf(buf, "%d", json["finishTimes"].asInt());
    
    RichElementText* re5 = RichElementText::create(1, Color3B(0xa6, 0xa6, 0xa6), 255, buf, "", 36);
    sprintf(buf, "%d", json["times"].asInt());
    RichElementText *re3 = RichElementText::create(2, Color3B(0xa6, 0xa6, 0xa6), 255, buf, "", 36);
    
    RichElementText* re4 = RichElementText::create(1, Color3B(0xa6, 0xa6, 0xa6), 255, "/", "", 36);
    
    timesText->pushBackElement(re5);
    timesText->pushBackElement(re4);
    timesText->pushBackElement(re3);
    
    return item;
}

void GrowTaskLayer::receiveOnFun(Ref *pSender)
{
    if (ZJHModel::getInstance()->isYoukeLogin())
    {
        ZJHModel::getInstance()->gotoView(VIEW_LOAD);
        return;
    }
    
    Button *but = (Button *)pSender;
    int index = but->getTag() - 100;
    
    Json::Value json;
    json["taskId"] = taskList[index]["taskDesc"];
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)
            return;
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            //领取状态改变
            but->setTitleText("已领取");
            but->setTitleColor(Color3B(0xa6, 0xa6, 0xa6));
            PlatformHelper::showToast("领取成功");
            __NotificationCenter::getInstance()->postNotification("money_change");
            
            ReciveEffectTipLayer* receive = ReciveEffectTipLayer::create();
            receive->showView(taskList[index]);
            Director::getInstance()->getRunningScene()->addChild(receive, 100);
            
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    },"task/doTask",json.toStyledString(),"doTask");
    
}

void GrowTaskLayer::onExit(){
    Widget::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}