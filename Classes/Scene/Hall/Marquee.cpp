//
//  Marquee.cpp
//  ZJH
//
//  Created by hyhy on 16/7/9.
//
//

#include "Marquee.h"
#include "../../Model/LabaModel.h"

USING_NS_CC;

Marquee *Marquee::createWithSize(cocos2d::Size size)
{
    Marquee *pRet = new Marquee();
    if (pRet && pRet->initWithSize(size)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

Marquee::~Marquee()
{
    
}

bool Marquee::initWithSize(cocos2d::Size size)
{
    if (Layer::init() == false) {
        return false;
    }
    
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2(0.5, 0.5));
    setContentSize(size);
    
    m_viewSize = size;
    m_pCurLabel = NULL;
    m_nCurCount = 0;
    m_nTotalCount = 0;
    
    m_pClip = ClippingNode::create();
    m_pClip->setContentSize(size);
    m_pClip->setAnchorPoint(Vec2(0, 0));
    m_pClip->setPosition(Vec2(0, 0));
    this->addChild(m_pClip);
    
    auto stencil = DrawNode::create();
    Vec2 rectangle[4] = {
        Vec2(0, 0),
        Vec2(m_pClip->getContentSize().width, 0),
        Vec2(m_pClip->getContentSize().width, m_pClip->getContentSize().height),
        Vec2(0, m_pClip->getContentSize().height),
    };
    
    Color4F white(1, 1, 1, 1);
    stencil->drawPolygon(rectangle, 4, white, 1, white);
    m_pClip->setStencil(stencil);
    
    this->schedule(schedule_selector(Marquee::start), 0.0);
    
    return true;
}

void Marquee::start(float dt)
{
    m_nCurCount = 0;
    m_nTotalCount = this->refreshData();
    if (m_nTotalCount > 0) {
        this->unschedule(schedule_selector(Marquee::start));
        this->schedule(schedule_selector(Marquee::checkNext), 0.0);
        this->setVisible(true);
    }
}

void Marquee::checkNext(float dt)
{
    LabaModel::getInstance()->timeCount += dt;
    if (LabaModel::getInstance()->timeCount < 4.0) {//每条消息显示4秒
        return;
    }
    
    LabaModel::getInstance()->timeCount = 0;
    
    if (m_nCurCount > m_nTotalCount)//消息播完了
    {
        this->unschedule(schedule_selector(Marquee::checkNext));
        this->schedule(schedule_selector(Marquee::start), 0.0);
        this->setVisible(false);
    }
    else
    {
        this->initCurOne(m_nCurCount);
        m_nCurCount++;
    }
}

void Marquee::initCurOne(int index)
{
    this->unschedule(schedule_selector(Marquee::updateMove));
    
    if (m_pCurLabel) {
        if (m_pCurLabel->getReferenceCount() > 0) {
            FiniteTimeAction *outActon = Sequence::create(Spawn::create(MoveBy::create(0.7, Vec2(0, m_viewSize.height)),
                                                                        FadeOut::create(1.0),
                                                                        NULL),
                                                          RemoveSelf::create(),
                                                          NULL);
            m_pCurLabel->stopAllActions();
            m_pCurLabel->runAction(outActon);
        }
    }
    
    m_pCurLabel = NULL;
    Json::Value &msgInfo = this->jsonData[index];
    std::string msgDesc = msgInfo["msgDesc"].asString();
    if (msgDesc.length()) {
        m_pCurLabel = Label::createWithSystemFont(msgDesc, "AmericanTypewriter", 40);
        m_fLabelWidth = m_pCurLabel->getContentSize().width;
        if (m_fLabelWidth > m_viewSize.width) {
            m_pCurLabel->setAnchorPoint(Vec2(0, 0.5));
            m_pCurLabel->setPosition(Vec2(0, -m_viewSize.height * 0.5));
            this->schedule(schedule_selector(Marquee::updateMove), 0.0);
        } else {
            m_pCurLabel->setAnchorPoint(Vec2(0.5, 0.5));
            m_pCurLabel->setPosition(Vec2(m_viewSize.width / 2, -m_viewSize.height * 0.5));
        }
        m_pCurLabel->runAction(MoveBy::create(0.3, Vec2(0, m_viewSize.height)));
        m_pClip->addChild(m_pCurLabel);
    }
}

void Marquee::updateMove(float dt)
{
    if (m_pCurLabel) {
        m_pCurLabel->setPositionX(m_pCurLabel->getPositionX() - 3);
        if (m_pCurLabel->getPositionX() <= -m_fLabelWidth) {
            m_pCurLabel->setPositionX(m_viewSize.width);
        }
    }
}


int Marquee::refreshData()
{
    this->jsonData = Json::Value::null;
    
    if (LabaModel::getInstance()->msgData.size() > 0) {
        this->jsonData = LabaModel::getInstance()->msgData;
        LabaModel::getInstance()->msgData = Json::Value::null;
    }
    
    return this->jsonData.size();
}