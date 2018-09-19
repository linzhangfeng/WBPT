//
//  RoomGameEndLayer.cpp
//  ZJH
//
//  Created by hyhy on 16/7/28.
//
//

#include "RoomGameEndLayer.h"
#include "../../../Match/RecordDegitalLayer.hpp"


void RoomGameEndLayer::show()
{
//    Size winSize = Director::getInstance()->getWinSize();
//    this->setPosition(Vec2(0, -winSize.height));
//    this->runAction(MoveTo::create(0.17, Vec2(0, 0)));
    this->setVisible(true);
}

void RoomGameEndLayer::hide()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->runAction(Sequence::create(MoveTo::create(0.17, Vec2(0, -winSize.height)),
                                     RemoveSelf::create(),
                                     NULL));
}

bool RoomGameEndLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomGameEndLayer::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void RoomGameEndLayer::myInit(int endTime, int roomId)
{
    m_roomId = roomId;
    
    Size winSize = Director::getInstance()->getWinSize();
    
    time_t tick = endTime;
    struct tm tm;
    char timeStr[128];
    
    tick = time(NULL);
    tm = *localtime(&tick);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tm);
    
    LayerColor *mask = LayerColor::create(Color4B(0, 0, 0, 150));
    this->addChild(mask);
    
    Sprite *circleSp = Sprite::create("Public/icon_timeout.png");
    circleSp->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 100));
    circleSp->setScale(0.75);
    this->addChild(circleSp);
    
    Label *timeOutLabel = Label::createWithSystemFont("已结束", "Arial", 32);
    timeOutLabel->setPosition(circleSp->getPosition());
    this->addChild(timeOutLabel);
    
    char buff[256];
    snprintf(buff, sizeof(buff), "结束时间：%s", timeStr);
    Label *endLabel = Label::createWithSystemFont(buff, "Arial", 30);
    endLabel->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    this->addChild(endLabel);
    
    Button *shareBtn = Button::create("Public/btn_share_green.png");
    shareBtn->setZoomScale(0);
    shareBtn->setScale(0.75);
    shareBtn->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 200));
    shareBtn->setName("share_button");
    shareBtn->addClickEventListener(CC_CALLBACK_1(RoomGameEndLayer::buttonClicked, this));
    this->addChild(shareBtn);
}

void RoomGameEndLayer::buttonClicked(cocos2d::Ref *sender)
{
    Button *btn = dynamic_cast<Button *>(sender);
    if (btn) {
        std::string name = btn->getName();
        
        if (name == "share_button") {
            btn->setEnabled(false);
            Json::Value json;
            json["roomId"] = m_roomId;
            CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                                     {
                                                         if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
                                                         btn->setEnabled(true);
                                                         CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                         if (packet) {
                                                             if (getReferenceCount() == 0 || getReferenceCount() > 10)
                                                             {
                                                                 //nothing
                                                             }
                                                             else
                                                             {
                                                                 if (packet->status == 3)
                                                                 {
                                                                     packet->status = 0;
                                                                     if (packet->resultIsOK())
                                                                     {
                                                                         Json::Value data = packet->recvVal["resultMap"];
                                                                         data["roomID"] = m_roomId;
                                                                         data["gameId"] = ZJHModel::getInstance()->curGameType;
                                                                         
                                                                         printf("VVVV == %s\n", data.toStyledString().c_str());

                                                                         ZJHModel::getInstance()->roomToHall();
                                                                         
                                                                         RecordDegitalLayer * recordDetail = RecordDegitalLayer::create();
                                                                         recordDetail->showView(data);
                                                                         recordDetail->setCanReturn(false);
                                                                         Scene *scene = Scene::create();
                                                                         scene->addChild(recordDetail);
                                                                         
                                                                         auto listenerkeyPad = EventListenerKeyboard::create();
                                                                         listenerkeyPad->onKeyReleased = [=](EventKeyboard::KeyCode keycode,Event* event)
                                                                         {
                                                                             if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
                                                                             {
                                                                                 Director::getInstance()->getEventDispatcher()->removeEventListener(listenerkeyPad);
                                                                                 Director::getInstance()->popScene();
                                                                             }
                                                                             event->stopPropagation();//
                                                                         };
                                                                         Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerkeyPad, recordDetail);
                                                                         Director::getInstance()->getEventDispatcher()->setPriority(listenerkeyPad, 1000);
                                                                         Director::getInstance()->pushScene(scene);
                                                                         
                                                                         recordDetail->setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
                                                                             Director::getInstance()->popScene();
                                                                             
                                                                         });
                                                                         recordDetail->setPosition(Vec2(0, 0));
                                                                     }
                                                                     else
                                                                     {
                                                                         PlatformHelper::showToast(packet->recvVal["message"].asCString());
                                                                     }
                                                                     
                                                                 }
                                                             }
                                                             
                                                             //删除packet
                                                             CCHttpAgent::getInstance()->packets.erase(tag);
                                                             delete packet;
                                                         }
                                                     },
                                                     "duiju/integral/detail", json.toStyledString(), "detail");
        }
    }
}

bool RoomGameEndLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}
