

#include "RecordVideoBox.hpp"
#include "cocostudio/CocoStudio.h"
#include "../../JsUtils/JsUtils.h"
using namespace cocostudio;

#pragma mark [ RecordVideoItem ]
RecordVideoItem *RecordVideoItem ::createWithData(const Json::Value &info)
{
    RecordVideoItem *pRet = new RecordVideoItem();
    if (pRet && pRet->initWithData(info)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool RecordVideoItem::initWithData(const Json::Value &info)
{
    if (Layout::init() == false) {
        return false;
    }
    
    mRoot = CSLoader::createNode("GameHall/record/recordVideoItem.csb");
    this->addChild(mRoot);
    this->setContentSize(mRoot->getContentSize());
    
    jsonData = info;
    mGameID =info["gameId"].asInt();
    mRoomID =info["roomID"].asInt();
    mIndex =info["index"].asInt();
    mPkgId = info["pkgId"].asInt();


    auto btn = (Button*)Utils::findNode(mRoot,"btn");
    btn->addClickEventListener([=](Ref*){
        this->postReviewData();
        return true;
    });
    auto txt =(Text *)Utils::findNode(mRoot,"no");
    txt->ignoreContentAdaptWithSize(true);
    txt->setString(StringUtils::format("第%d局", info["index"].asInt()));

    return true;
}

#include <zlib.h>
void RecordVideoItem::postReviewData()
{
    Json::Value json2;
    json2["roomId"] =mRoomID ;
    json2["num"] = mIndex;
    
    auto btn = (Button*)Utils::findNode(mRoot,"btn");
    btn->setEnabled(false);
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
          btn->setEnabled(true);
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            int index = 0;
            Json::Value data = loginPacket->recvVal["resultMap"]["data"][index];
            
            ZJHModel::getInstance()->roomData["videoData"]["code"] = jsonData["code"];
            ZJHModel::getInstance()->roomData["videoData"]["videoNumList"] = jsonData["videoNumList"];
            ZJHModel::getInstance()->roomData["videoData"]["curCount"] = jsonData["index"];
            ZJHModel::getInstance()->roomData["videoData"]["count"] = jsonData["count"];
            ZJHModel::getInstance()->roomData["videoData"]["roomID"] = jsonData["roomID"];
            ZJHModel::getInstance()->roomData["videoData"]["pkgId"] = jsonData["pkgId"];
            ZJHModel::getInstance()->roomData["videoData"]["gameId"] = jsonData["gameId"];
            
            ZJHModel::getInstance()->gotoVideo(mGameID, mRoomID, data["content"].asString(), mPkgId);
            
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            
        }
    }, "room/dz/video", json2.toStyledString(), "videoPost");
}

void RecordVideoItem::onExit()
{
    Layout::onExit();
    CCHttpAgent::getInstance()->callbacks.erase("videoPost");
}

bool RecordVideoBox::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    mRoot = CSLoader::createNode("GameHall/record/recordVideo.csb");
    this->addChild(mRoot);
    
    auto btn = (Button*)Utils::findNode(mRoot,"returnBtn");
    btn->addClickEventListener([=](Ref*){
        this->removeFromParent();
        return true;
    });
    return true;
}


void RecordVideoBox::showView( Json::Value&  json)
{
    auto listNode =(ListView*)Utils::findNode(mRoot,"list");

        
    for (int i = 0; i < json["videoNumList"].size(); i++)
    {
          json["index"] = json["videoNumList"][i];
        RecordVideoItem *item = RecordVideoItem::createWithData(json);
        listNode->addChild(item);
    }
    
    if(json["videoNumList"].size() <4)
    {
        listNode->setScrollBarEnabled(false);
        listNode->setBounceEnabled(false);
    }
    else
    {
        listNode->forceDoLayout();
    }

}
