
#include "RecordBox.hpp"
#include "RecordDetailsBox.hpp"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;


#pragma mark [ RecordItem ]

RecordItem *RecordItem::createWithData(const Json::Value &info)
{
    RecordItem *pRet = new RecordItem();
    if (pRet && pRet->initWithData(info))
    {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool RecordItem::initWithData(const Json::Value &info)
{
    if (Layout::init() == false) {
        return false;
    }
    
    mRoot = CSLoader::createNode("GameHall/record/recordItem.csb");
    this->addChild(mRoot);
    this->setContentSize(mRoot->getContentSize());
    
    auto txt =(Text *)Utils::findNode(mRoot,"time");
    txt->ignoreContentAdaptWithSize(true);
    dealTimeFormate(info["endTime"].asDouble()/1000, txt);
    
    string imageName = info["gameLogo"].asString();
    ZJHModel::getInstance()->loadingImageResource((ImageView *)Utils::findNode(mRoot,"icon"), imageName);
    
    string buff = StringUtils::format("%s\n房号:%s\n局数:%d局,%d人参与",info["gameName"].asCString(),info["code"].asCString(),info["count"].asInt(),info["number"].asInt());
    txt =(Text *)Utils::findNode(mRoot,"room");
    txt->ignoreContentAdaptWithSize(true);
    txt->setString(buff);
    
    auto score =info["integral"].asInt();
    Utils::findNode(mRoot,"winNum")->setVisible(score>0);
    Utils::findNode(mRoot,"lossNum")->setVisible(score<=0);
    if(score>0)
    {
        auto  txt =(TextAtlas *)Utils::findNode(mRoot,"winNum");
        txt->ignoreContentAdaptWithSize(true);

        txt->setString(StringUtils::format(":%d",score));
    }
    else if(score<0)
    {
        auto txt =(TextAtlas *)Utils::findNode(mRoot,"lossNum");
        txt->ignoreContentAdaptWithSize(true);

        txt->setString(StringUtils::format(":%d",-score));

    }
    else
    {
        auto txt =(TextAtlas *)Utils::findNode(mRoot,"lossNum");
        txt->ignoreContentAdaptWithSize(true);
        
        txt->setString("0");
        
    }
    
    
    this->setTouchEnabled(true);
    this->addClickEventListener([=](Ref*){
        mParent->gotoRecordDetail(this->getTag());
    });

    return true;
}

void RecordItem::dealTimeFormate(long long sec, Text *create1)
{
    //房间结束时间计算
    struct timeval nowTimeval;
    gettimeofday(&nowTimeval, NULL);
    time_t time_sec ;
    time_sec = nowTimeval.tv_sec;
    time_t time_sec1 = sec;
    
    struct tm * test = localtime(&time_sec1);
    int year = test->tm_year + 1900;
    int mon = test->tm_mon+1;
    int day = test->tm_mday;
    int hour = test->tm_hour;
    int min = test->tm_min;
    
    struct tm * curTest = localtime(&time_sec);
    int curDay = curTest->tm_mday;
    
    //秒数转分钟
    long int intervalDay = (time_sec - time_sec1)*(1.0/60.0);
    
    string bufStr = "";
    if (intervalDay >= 0 && intervalDay < 60*24)
    {
        //24小时之内
        if (day == curDay)
        {
            bufStr = "今天\n";
        }else
        {
            bufStr = "昨天\n";
        }
    }else if (intervalDay >= 60*24 && intervalDay < 48*60)
    {
        //24小时－－48小时
        if (day == curDay-1)
        {
            bufStr = "昨天\n";
        }else
        {
            bufStr = "前天\n";
        }
    }else if (intervalDay >= 48*60)
    {
        bufStr = StringUtils::format("%04d年%d月%d日\n", year, mon, day);
    }
    
    bufStr +=StringUtils::format("%02d:%02d", hour, min);
    create1->setString(bufStr);
}




bool RecordBox::init()
{
    if (!Layer::init())
    {
        return false;
    }
    mRoot = CSLoader::createNode("GameHall/record/recordBox.csb");
    this->addChild(mRoot);
    
    Button* btn = (Button*)Utils::findNode(mRoot,"returnBtn");
    
    btn->addClickEventListener([=](Ref*){
        this->removeFromParent();
        return true;
    });
    
    mHaveMore = true;
    mRecordIndex = 0;
    mPageIndex =1;
    ListView* list = (ListView*)Utils::findNode(mRoot,"list");
    ((ScrollView*)list)->addEventListener([=](Ref *sendre, ScrollView::EventType eventType){
        if (eventType == ScrollView::EventType::SCROLL_TO_BOTTOM && mHaveMore)
        {
            mPageIndex++;
            Json::Value json5;
            json5["pageIndex"] = mPageIndex;
            json5["pageSize"] = 10;
            
            if(mLoading == nullptr)
            {
                Size winSize = Director::getInstance()->getWinSize();
                mLoading = Loading::startLoading(this, winSize / 2);
                mLoading->maskSwllowTouch();

            }
            list->stopAutoScroll();
            auto pos = list->getInnerContainerPosition();
            if(pos.y>220)
            {
                pos.y = 220;
                list->setBounceEnabled(false);
                list->setInnerContainerPosition(pos);
                list->setBounceEnabled(true);

            }
            CCHttpAgent::getInstance()->sendHttpPost(NULL,"duiju/historyList", json5.toStyledString(), "historyListMore");
            schedule(schedule_selector(RecordBox::getDataMore),0);
        }
        
    });
    Utils::scaleAll(Utils::findNode(mRoot,"mask"));
    
    Utils::findNode(mRoot,"mess")->setVisible(false);
    sendRecord();
    
    return true;
}
void RecordBox::getDataMore(float)
{
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["historyListMore"];
    if (packet && packet->status != 0)
    {
        mLoading->removeFromParent();
        mLoading = nullptr;
        unschedule(schedule_selector(RecordBox::getDataMore));
        if (packet->status != 3)
        {
            return;
        }
        packet->status = 0;
        
        if (packet->resultIsOK())
        {
            const Json::Value& data = packet->recvVal["resultMap"]["historyList"];
            mHaveMore = data["hasMore"].asBool();
            addDataInList(data);
        }
        else
        {
            PlatformHelper::showToast(packet->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets["historyListMore"] = NULL;
        delete packet;
    }
    
}

void RecordBox::getData(float)
{
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["historyList"];
    if (packet && packet->status != 0)
    {
        mLoading->removeFromParent();
        mLoading = nullptr;
        unschedule(schedule_selector(RecordBox::getData));
        if (packet->status != 3)
        {
            return;
        }
        packet->status = 0;
        
        if (packet->resultIsOK())
        {
            const Json::Value& data = packet->recvVal["resultMap"]["historyList"];
            mHaveMore = data["hasMore"].asBool();
            mRecordJson.clear();
            showRecordList(data);
        }
        else
        {
            PlatformHelper::showToast(packet->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets["historyList"] = NULL;
        delete packet;
    }

}

void RecordBox::sendRecord()
{
    Size winSize = Director::getInstance()->getWinSize();
    mLoading = Loading::startLoading(this, winSize / 2);
    mLoading->maskSwllowTouch();

    Json::Value json3;
    json3["pageIndex"] = 1;
    json3["pageSize"] = 10;
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"duiju/historyList", json3.toStyledString(), "historyList");
    schedule(schedule_selector(RecordBox::getData), 0.0f);
}

void RecordBox::showRecordList(const Json::Value& json)
{
    const Json::Value& listData = json["result"];
    
    int nums = listData.size();
    if (nums == 0 )
    {
        Utils::findNode(mRoot,"mess")->setVisible(true);
        return;
    }
    
    addDataInList(json);
}


void RecordBox::addDataInList(const Json::Value& json)
{
    const Json::Value& listData = json["result"];
    int nums = listData.size();
    
    ListView* list = (ListView*)Utils::findNode(mRoot,"list");
    float h =0;
    for (int i = 0; i < nums; i++)
    {
        RecordItem *item = RecordItem::createWithData(listData[i]);
        item->mParent = this;
        item->setTag(mRecordIndex++);
        list->addChild(item);
        
        mRecordJson.push_back(listData[i]);
        
        h+=item->getContentSize().height;
    }
    if(nums > 0)
    {
        auto pos = list->getInnerContainerPosition();
        pos.y-=h;
        list->forceDoLayout();
        
        auto items = list->getItems();
        auto len =items.size();
        
        if(len !=nums )
        {
            list->setInnerContainerPosition(pos);
        }
        list->stopAutoScroll();
        
        auto is = mHaveMore;
        mHaveMore = false;
        this->runAction(Sequence::create(DelayTime::create(0.1*nums+0.5),CallFunc::create([=](){
            mHaveMore = is;
        }), NULL));

    }
}

void RecordBox::gotoRecordDetail(int index)
{
    Json::Value data;
    data["roomID"] = mRecordJson[index]["id"];
    data["gameId"] = mRecordJson[index]["gameId"];
    data["code"] = mRecordJson[index]["code"];
    
    //战绩详情
    auto box =  RecordDetailsBox::create();
    addChild(box,1000000);
    box->show(data);
}
