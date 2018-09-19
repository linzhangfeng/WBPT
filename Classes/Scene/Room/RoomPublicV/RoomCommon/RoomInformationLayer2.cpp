//
//  RoomInformationLayer.cpp
//  ZJH
//
//  Created by hyhy on 16/7/26.
//
//

#include "RoomInformationLayer2.h"
#include "RoomShareLayer.h"

static const float scaleRate = 0.67;

void RoomInformationLayer2::show()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(Vec2(0, -winSize.height));
    this->runAction(MoveTo::create(0.17, Vec2(0, 0)));
    this->setVisible(true);
}

void RoomInformationLayer2::hide()
{
    this->setVisible(false);
    this->removeFromParentAndCleanup(true);
}

bool RoomInformationLayer2::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(RoomInformationLayer2::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void RoomInformationLayer2::myInit(Json::Value &data)
{
    Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = Size(winSize.width, 400);
    
    LayerColor *contentBg = LayerColor::create(Color4B(32, 32, 32, 255));
    contentBg->setContentSize(contentSize);
    contentBg->setPosition(Vec2(0, 0));
    contentBg->setName("content_bg");
    addChild(contentBg);
    
    LayerColor *titleBg = LayerColor::create(Color4B(32, 32, 32, 255));
    titleBg->setContentSize(Size(winSize.width, 111 * scaleRate));
    titleBg->setPosition(Vec2(0, contentBg->getPositionY() + contentBg->getContentSize().height));
    titleBg->setName("title_bg");
    this->addChild(titleBg);
    
    LayerColor *line = LayerColor::create(Color4B(0, 0, 0, 255));
    line->setContentSize(Size(winSize.width, 2));
    line->setPosition(titleBg->getPosition());
    this->addChild(line);
    
    Text* titlelabel = Text::create("房间信息", "AmericanTypewriter", 50 * scaleRate);
    titlelabel->setPosition(titleBg->getContentSize() / 2);
    titleBg->addChild(titlelabel);
    
    Button *closeBtn = Button::create("Public/icon_cancel.png");
    closeBtn->setZoomScale(0);
    closeBtn->setPosition(Vec2(winSize.width - 45, titleBg->getContentSize().height / 2));
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->hide();
                                    });
    titleBg->addChild(closeBtn);
    
    //内容
    std::string nickName = data["gameRoom"]["user"]["nickName"].asString();
    std::string roomName = data["gameRoom"]["roomName"].asString();
    std::string code = data["gameRoom"]["code"].asString();
    std::string roomType = data["way"]["name"].asString();
    char buff[258];

    snprintf(buff, sizeof(buff), "房间号：%s", code.c_str());
    Text *codeText = Text::create(buff, "AmericanTypewriter", 26);
    codeText->setAnchorPoint(Vec2(0, 1));
    codeText->setPosition(Vec2(40, contentSize.height - 32));
    codeText->setColor(Color3B(0xef, 0xcd, 0x5c));
    contentBg->addChild(codeText);
    
    //人数
    int onlineCount = data["gameRoom"]["onlineCount"].asInt();
    int againstNumber = data["gameRoom"]["againstNumber"].asInt();
    snprintf(buff, sizeof(buff), "人数:%d/%d人", onlineCount, againstNumber);
    Text *numText = Text::create(buff, "AmericanTypewriter", 26);
    numText->setColor(Color3B(0xef, 0xcd, 0x5c));
    numText->setAnchorPoint(Vec2(0, 1));
    numText->setPosition(Vec2(306, contentSize.height - 32));
    contentBg->addChild(numText);
    
    Text *gameTips = Text::create(data["gameTips"].asString(), "AmericanTypewriter", 26);
    gameTips->setColor(Color3B(0xef, 0xcd, 0x5c));
    gameTips->setAnchorPoint(Vec2(0, 1));
    gameTips->setPosition(Vec2(513, contentSize.height - 32));
    contentBg->addChild(gameTips);

    
    //房间名
    string contentStr = "房间名："+roomName;
    Utils::parseName(20, buff, contentStr.c_str());
    Text *roomNameTitle = Text::create(buff, "AmericanTypewriter", 26);
    roomNameTitle->setAnchorPoint(Vec2(0, 1));
    roomNameTitle->setPosition(Vec2(40, contentSize.height - 80));
    roomNameTitle->setColor(Color3B(0xb3, 0xb3, 0xb3));
    contentBg->addChild(roomNameTitle);
    
    //房主
    nickName = "房    主："+nickName;
    Utils::parseName(20, buff, nickName.c_str());
    Text *userName = Text::create(buff, "AmericanTypewriter", 26);
    userName->setAnchorPoint(Vec2(0, 1));
    userName->setPosition(Vec2(40, contentSize.height - 121));
    userName->setColor(Color3B(0xb3, 0xb3, 0xb3));
    contentBg->addChild(userName);
    
    //类型
    std::string gameName = data["gameRoom"]["gameName"].asString();
    int bottomNote = data["gameRoom"]["bottomNote"].asInt();
    int takeIn = data["gameRoom"]["takeIn"].asInt();
    snprintf(buff, sizeof(buff), "类    型：%s(底注%d初始带入%d)", gameName.c_str(), bottomNote, takeIn);
    Text *typeText = Text::create(buff, "AmericanTypewriter", 26);
    typeText->setAnchorPoint(Vec2(0, 1));
    typeText->setPosition(Vec2(40, contentSize.height - 157));
    typeText->setColor(Color3B(0xb3, 0xb3, 0xb3));
    contentBg->addChild(typeText);
    
    //规则
//    ImageView* ruleImage = ImageView::create("RoomPublicV/room_rule_bg.png");
//    ruleImage->setScale9Enabled(true);
//    ruleImage->ignoreContentAdaptWithSize(false);
//    ruleImage->setContentSize(Size(contentBg->getContentSize().width - 18 * 2,480));
//    ruleImage->setAnchorPoint(Vec2(0.5,1));
//    ruleImage->setPosition(Vec2(contentBg->getContentSize().width / 2,contentBg->getContentSize().height - 230));
//    contentBg->addChild(ruleImage);
//    
//    Text *ruleTips = Text::create("游戏规则预览", "AmericanTypewriter", 30);
//    ruleTips->setAnchorPoint(Vec2(0, 1));
//    ruleTips->setPosition(Vec2(21, ruleImage->getContentSize().height - 23));
//    ruleTips->setColor(Color3B(0xee, 0xcc, 0x5b));
//    ruleImage->addChild(ruleTips);
//
//    ListView *listV = ListView::create();
//    listV->setBounceEnabled(true);
//    listV->setScrollBarEnabled(false);
//    listV->setDirection(ScrollView::Direction::VERTICAL);
//    listV->setContentSize(Size(ruleImage->getContentSize().width - 21 * 2, ruleImage->getContentSize().height - 71 - 23));
//    listV->setPosition(Vec2(ruleImage->getContentSize().width / 2, ruleImage->getContentSize().height - 71));
//    listV->setItemsMargin(13);
//    listV->setAnchorPoint(Vec2(0.5,1));
//    listV->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
//    ruleImage->addChild(listV);
//    
//    int intK = 0;
//    Json::Value roomData = ZJHModel::getInstance()->roomData["gameRoom"]["innerWayRecommend"][intK];
//    bool isVisible;
//    for (int i = 0; i < roomData["gameCustomClassifyVOList"].size(); i++)
//    {
//        isVisible = false;
//        Json::Value pointJson = roomData["gameCustomClassifyVOList"][i]["gameCustomPlayValInfoList"];
//        
//        if (ZJHModel::getInstance()->isTrait)
//        {
//            if (roomData["gameCustomClassifyVOList"][i]["tribeVisible"].asBool())
//            {
//                isVisible = true;
//            }
//        }else
//        {
//            if (roomData["gameCustomClassifyVOList"][i]["platformVisible"].asBool())
//            {
//                isVisible = true;
//            }
//            
//        }
//        
//        if (isVisible == false)
//        {
//            continue;
//        }
//        
//        bool isHave = false;
//        for (int k = 0; k < pointJson.size(); k++)
//        {
//            if (pointJson[k]["iptValue"] != Json::Value::null)
//            {
//                isHave = true;
//                break;
//            }
//        }
//        
//        
//        if (isHave == false)
//        {
//            continue;
//        }
//        Layout* cell = Layout::create();
//        cell->setAnchorPoint(Vec2(0,1));
//        cell->setContentSize(Size(listV->getContentSize().width, 26));
//        cell->setPosition(Vec2(0,26));
//        listV->insertCustomItem(cell,0);
//        
//        Text* title = Text::create(roomData["gameCustomClassifyVOList"][i]["classifyName"].asString()+":", "AmericanTypewriter", 26);
//        title->setColor(Color3B(0xb3, 0xb3, 0xb3));
//        title->setAnchorPoint(Vec2(0, 0.5));
//        title->setPosition(Vec2(0,  cell->getContentSize().height / 2));
//        cell->addChild(title, 2);
//        
//        Text* value = Text::create("", "AmericanTypewriter", 26);
//        value->setColor(Color3B(0xb3, 0xb3, 0xb3));
//        value->setAnchorPoint(Vec2(0, 0.5));
//        value->setPosition(Vec2(title->getContentSize().width,  cell->getContentSize().height / 2));
//        for (int j = 0; j < pointJson.size(); j++)
//        {
//            bool isSelect = true;
//            if (pointJson[j]["iptValue"] == Json::Value::null)
//            {
//                isSelect = false;
//            }
//            
//            if (pointJson[j]["classifyId"].asInt() == 29)
//            {
//                char buff[128];
//                
//                int tempV = atoi(pointJson[j]["playName"].asString().c_str());
//                tempV *= ZJHModel::getInstance()->roomData["gameRoom"]["bottomNote"].asInt();
//                sprintf(buff, "%d", tempV);
//                if(isSelect){
//                   value->setString(buff);
//                }
//            }else
//            {
//                if (roomData["gameCustomClassifyVOList"][i]["choiceType"].asInt() == 4)
//                {
//                    char buff[128];
//                    sprintf(buff, "%d", pointJson[j]["iptValue"].asInt());
//                    if(isSelect){
//                        value->setString(buff);
//                    }
//                }else
//                {
//                    if(isSelect){
//                        value->setString(pointJson[j]["playName"].asString());
//                    }
//                }
//            }
//        }
//        std::cout<<"=============="<<value->getString()<<endl;
//        cell->addChild(value, 2);
//    }

}

bool RoomInformationLayer2::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    LayerColor *contentBg = dynamic_cast<LayerColor *>(this->getChildByName("content_bg"));
    LayerColor *titleBg = dynamic_cast<LayerColor *>(this->getChildByName("title_bg"));
    if (contentBg->boundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())) || titleBg->boundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())))
    {
        //
    }
    else
    {
        this->hide();
        event->stopPropagation();
    }
    return true;
}
