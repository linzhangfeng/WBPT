#include "LabaModel.h"
#include "ZJH.h"

LabaModel *labaModel;
using namespace::cocos2d;

LabaModel* LabaModel::getInstance()
{
    if (labaModel == NULL) {
        labaModel = new LabaModel();
        labaModel->init();
        labaModel->retain();
    }
    
    return labaModel;
}

void LabaModel::init()
{
    timeCount = 0;
}

void LabaModel::start()
{
    cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(LabaModel::reflashData),this,120,false);
    cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(LabaModel::getData),this,0,false);
}

void LabaModel::reflashData(float dt)
{
    Json::Value json;
    
    json["createTime"] = (int)Utils::getCurrentTime();
    
    //pre load msg data
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"msgTask/msgData",json.toStyledString(),"msgdata");
}

void LabaModel::getData(float dt)
{
    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets["msgdata"];
    if (loginPacket && loginPacket->status != 0)
    {
        if (loginPacket->status != 3)
        {
            return;
        }
        
        loginPacket->status = 0;
        
        if (loginPacket->resultIsOK())
        {
//            Json::Value data = loginPacket->recvVal["resultMap"]["taskMsg"];
//            log("has new msg [%s]",data.toStyledString().c_str());
            this->msgData = loginPacket->recvVal["resultMap"]["taskMsg"];
        }
        else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets["msgdata"] = NULL;
        delete loginPacket;
    }
}