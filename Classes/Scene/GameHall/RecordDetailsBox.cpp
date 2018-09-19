//
//  RecordDetailsBox.cpp
//  ZJH
//
//  Created by apple on 16/7/16.
//
//

#include "RecordDetailsBox.hpp"
#include "RecordVideoBox.hpp"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;


#include "../Loading/Loading.h"

#define XI_SHU 0.5

#pragma mark [ RecordDetailsItem ]
RecordDetailsItem *RecordDetailsItem ::createWithData(const Json::Value &info, int rate,int bigWinnerScore)
{
    RecordDetailsItem *pRet = new RecordDetailsItem();
    if (pRet && pRet->initWithData(info, rate ,bigWinnerScore)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool RecordDetailsItem::initWithData(const Json::Value &info, int rate,int bigWinnerScore)
{
    if (Layout::init() == false) {
        return false;
    }
    mRoot = CSLoader::createNode("GameHall/record/recordDetailsItem.csb");
    this->addChild(mRoot);
    this->setContentSize(mRoot->getContentSize());
    
    auto  txt = (Text*) Utils::findNode(mRoot,"no");
    txt->ignoreContentAdaptWithSize(true);
   txt->setString(StringUtils::format("%d",rate));
    
    auto bg = Utils::findNode(mRoot,"iconBg");
    //图片
    Sprite* portrait = Utils::createCircleAvatar(info["image"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(120, 120));
    portrait->setPosition(Vec2(bg->getContentSize().width*0.5, 95));
    bg->addChild(portrait);
    
        char buf[200];
    Utils::parseName(12, buf, info["userName"].asString().c_str());
    txt = (Text*) Utils::findNode(mRoot,"name");
    txt->ignoreContentAdaptWithSize(true);
    txt->setString(buf);
    
    txt = (Text*) Utils::findNode(mRoot,"id");
    txt->ignoreContentAdaptWithSize(true);
    txt->setString(StringUtils::format("ID:%d", info["userId"].asInt()));
    
    auto score =info["integral"].asInt();
    Utils::findNode(mRoot,"winner")->setVisible(score == bigWinnerScore && score > 0);
    Utils::findNode(mRoot,"winNum")->setVisible(score>0);
    Utils::findNode(mRoot,"lossNum")->setVisible(score<=0);
    if(score>0)
    {
        auto  txt =(TextAtlas *)Utils::findNode(mRoot,"winNum");
        txt->ignoreContentAdaptWithSize(true);

        txt->setString(StringUtils::format(":%d",score));
    }
    else if(score < 0)
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

    
    return true;
}




bool RecordDetailsBox::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    mRoot = CSLoader::createNode("GameHall/record/recordDetails.csb");
    this->addChild(mRoot);
    
    auto btn = (Button*)Utils::findNode(mRoot,"returnBtn");
    btn->addClickEventListener([=](Ref*){
        this->removeFromParent();
        return true;
    });
    Utils::scaleAll(Utils::findNode(mRoot,"mask"));
    
    btn = (Button*)Utils::findNode(mRoot,"wxBtn");
    btn->setVisible(false);
    btn->addClickEventListener([=](Ref*){
        this->shareClick("wx");
        return true;
    });
    btn = (Button*)Utils::findNode(mRoot,"pyqBtn");
    btn->setVisible(false);
    btn->addClickEventListener([=](Ref*){
        this->shareClick("pyq");
        return true;
    });
    btn = (Button*)Utils::findNode(mRoot,"videoBtn");
    btn->setVisible(false);
    btn->addClickEventListener([=](Ref*){
        this->gotoRecordVideo();
        return true;
    });
    
    btn = (Button*)Utils::findNode(mRoot,"updateBtn");
    btn->setVisible(false);
    btn->addClickEventListener([=](Ref*){
        Json::Value json;
        json["roomId"] = mRoomID;
        CCHttpAgent::getInstance()->sendHttpPost(NULL,"duiju/integral/detail", json.toStyledString(), "detail");
        auto winSize = Director::getInstance()->getVisibleSize();
        auto loading = Loading::startLoading(this, winSize / 2);
        loading->setName("loading");
        schedule(schedule_selector(RecordDetailsBox::getData), 1.0f);
    });

    mShowVideo = false;
    auto txt = (Text*)Utils::findNode(mRoot,"room");
    txt->ignoreContentAdaptWithSize(true);
    txt->setString("");
    
    txt = (Text*)Utils::findNode(mRoot,"time");
    txt->ignoreContentAdaptWithSize(true);
    txt->setString("");
    
    Utils::findNode(mRoot,"icon")->setVisible(false);
    Utils::findNode(mRoot,"novideo")->setVisible(false);
    
    return true;
}

void RecordDetailsBox::show(const Json::Value& jsonTemp)
{
    auto winSize = Director::getInstance()->getVisibleSize();
    mRoomID = jsonTemp["roomID"].asInt();
    mGameType = jsonTemp["gameId"].asInt();
    
    Json::Value json;
    json["roomId"] = mRoomID;
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"duiju/integral/detail", json.toStyledString(), "detail");
    auto loading = Loading::startLoading(this, winSize / 2);
    loading->setName("loading");
    schedule(schedule_selector(RecordDetailsBox::getData), 0.0f);
}

void RecordDetailsBox::getData(float dt)
{
    CCHttpPacket *packet = CCHttpAgent::getInstance()->packets["detail"];
    if (packet && packet->status != 0)
    {
        removeChildByName("loading");
        unschedule(schedule_selector(RecordDetailsBox::getData));
        if (packet->status != 3)
        {
           auto btn = (Button*)Utils::findNode(mRoot,"updateBtn");
        btn->setVisible(true);
        }
        else
        {
            packet->status = 0;
            if (packet->resultIsOK())
            {
                const Json::Value& data = packet->recvVal["resultMap"];
                showView(data);
                if(mShowVideo)
                {
                    gotoRecordVideo();
                    mShowVideo = false;
                }
            }
            else
            {
                PlatformHelper::showToast(packet->recvVal["message"].asCString());
            }
        }
        
        //删除packet
        CCHttpAgent::getInstance()->packets.erase("detail");
        delete packet;
    }
}

void RecordDetailsBox::showView(const Json::Value& json)
{
    mJsonData = json;
    
    mJsonData["roomID"] = mRoomID;
    mJsonData["gameId"] = mGameType;
    
    auto btn = (Button*)Utils::findNode(mRoot,"wxBtn");
    btn->setVisible(true);
    btn = (Button*)Utils::findNode(mRoot,"pyqBtn");
    btn->setVisible(true);
    
    btn = (Button*)Utils::findNode(mRoot,"updateBtn");
    btn->setVisible(false);

    char buf1[256];
    char buf2[256];
    Utils::parseName(12, buf1, json["gameTypeName"].asString().c_str());
    Utils::parseName(15, buf2, json["pkgName"].asString().c_str());
    
    Utils::findNode(mRoot,"icon")->setVisible(true);
    ZJHModel::getInstance()->loadingImageResource((ImageView *)Utils::findNode(mRoot,"icon"), json["gameUrl"].asString());
    
    char buf[256];
    Utils::parseName(8, buf, json["userName"].asString().c_str());

    string buff = StringUtils::format("房号:%s\n房主:%s\n局数:%d局\n游戏:%s(%s)",mJsonData["code"].asCString(),buf, json["count"].asInt(), buf1, buf2);
     auto txt = (Text*)Utils::findNode(mRoot,"room");
     txt->setString(buff);
                   
    //剩余时间的计算
     time_t starTime = json["startTime"].asDouble()/1000;
     time_t endTime = json["endTime"].asDouble()/1000;
                   
                   struct tm * starTm = localtime(&starTime);
                   int year = starTm->tm_year + 1900;
                   int mon = starTm->tm_mon+1;
                   int day = starTm->tm_mday;
                   int hour = starTm->tm_hour;
                   int min = starTm->tm_min;
                   
                   struct tm * endTm = localtime(&endTime);
                   int yearEnd = endTm->tm_year + 1900;
                   int monEnd = endTm->tm_mon+1;
                   int dayEnd = endTm->tm_mday;
    int hourEnd = endTm->tm_hour;
     int minEnd = endTm->tm_min;
                   
    txt = (Text*)Utils::findNode(mRoot,"time");
    txt->setString(StringUtils::format("%04d/%02d/%02d  %02d:%02d -- %04d/%02d/%02d  %02d:%02d", year, mon, day, hour, min, yearEnd, monEnd, dayEnd, hourEnd, minEnd));
          
                   
   btn = (Button*)Utils::findNode(mRoot,"videoBtn");
   btn->setVisible(json["videoNum"].asInt() > 0);
   Utils::findNode(mRoot,"novideo")->setVisible(json["videoNum"].asInt() <= 0);

    auto listNode =(ListView*)Utils::findNode(mRoot,"list");
    
    const Json::Value& list = json["playRank"];
    Layout* temp ;
    int bigWinnerScore = 0;
     for (int i = 0; i < list.size(); i++)
     {
         if(i == 0){
            bigWinnerScore = list[i]["integral"].asInt();
         }
         RecordDetailsItem *item = RecordDetailsItem::createWithData(list[i], i+1,bigWinnerScore);  item->mParent = this;


          if(i%2==0)
          {
              temp =Layout::create();
              temp->setContentSize(Size(1440,220));
              listNode->addChild(temp);
              temp->addChild(item);
          }
         else
         {
             item->setPositionX(720);
             temp->addChild(item);
         }
      }
    if(list.size() <=4)
    {
        listNode->setScrollBarEnabled(false);
        listNode->setBounceEnabled(false);
    }
    else
    {
        listNode->forceDoLayout();
    }
   
          
}
void RecordDetailsBox::shareClick(const string& key )
{
    
   auto btn = (Button*)Utils::findNode(mRoot,"wxBtn");
    btn->setVisible(false);
    btn = (Button*)Utils::findNode(mRoot,"pyqBtn");
    btn->setVisible(false);
    btn = (Button*)Utils::findNode(mRoot,"pyqBtn");
    btn->setVisible(false);

    
    auto listNode =(ListView*)Utils::findNode(mRoot,"list");
    listNode->setClippingEnabled(false);
    
    auto shareBg =Utils::findNode(mRoot,"shareBg");
    auto bg =Utils::findNode(mRoot,"bg");
    
    auto winSize = Director::getInstance()->getWinSize();
    
    auto h =ceil(mJsonData["playRank"].size() /2.0);
    float addY = -60;
    if(h >2)
    {
        addY +=(h-2)*220;
        bg->setContentSize(Size(bg->getContentSize().width,960+(h-2)*220));
    }
    h =  bg->getContentSize().height+60;
    float s  = 1;
    if(winSize.height <h)
    {
        s =winSize.height/h;
    }
    float preX =shareBg->getPositionX();
    shareBg->setPosition(0,addY*s);
    shareBg->setScale(s);
    auto listPos = listNode->getInnerContainerPosition();
    listNode->jumpToTop();
    RenderTexture* renderTexture = RenderTexture::create(shareBg->getContentSize().width*s, h*s, Texture2D::PixelFormat::RGBA8888);
    renderTexture->begin();
    shareBg->visit();
    renderTexture->end();
    
    renderTexture->saveToFile("record.jpg", Image::Format::JPG, false, [=](RenderTexture* o,std::string path){
        bg->setContentSize(Size(bg->getContentSize().width,960));
        shareBg->setPosition(preX,0);
        shareBg->setScale(1);
        listNode->setClippingEnabled(true);
        listNode->setInnerContainerPosition(listPos);

        auto btn = (Button*)Utils::findNode(mRoot,"wxBtn");
        btn->setVisible(true);
        btn = (Button*)Utils::findNode(mRoot,"pyqBtn");
        btn->setVisible(true);
        if(key == "wx")
       {
           PlatformHelper::WeChatShareImage("", path.c_str());
       }
        else if(key == "pyq")
        {
            PlatformHelper::WeChatFrendsCircleShareImage("", path.c_str());
        }
    });

}


void RecordDetailsBox::gotoRecordVideo()
{
    auto box =  RecordVideoBox::create();
    
    addChild(box,1000000);
    box->showView(mJsonData);

}
