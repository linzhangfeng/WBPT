//
//  OtherGameTypeView.cpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/21.
//
//

#include "OtherGameTypeView.h"

bool OtherGameTypeView::init(){
    if(!Layout::init()){
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    Size nSize = Size(winSize.width,winSize.height-310);
    LayerColor* color = LayerColor::create(Color4B(20,20,20,255));
    color->setContentSize(nSize);
    this->setContentSize(nSize);
    this->addChild(color);
    
    Button* button = Button::create("Hall/button/btn_upmenu_cancel_check.png",
                                    "Hall/button/btn_upmenu_cancel.png");
    button->setAnchorPoint(Vec2(0.5,0.5));
    button->setPosition(Vec2(winSize.width - button->getContentSize().width/2 - 10,winSize.height - button->getContentSize().height/2 - 320));
    button->addClickEventListener([=](Ref* ref){
        this->removeFromParentAndCleanup(true);
    });
    this->addChild(button);
    initView();
    return true;
}

void OtherGameTypeView::initView(){
    
     Size winSize = Director::getInstance()->getWinSize();
    _mListView = ListView::create();
    _mListView->setDirection(ScrollView::Direction::VERTICAL);
    _mListView->setBounceEnabled(false);
    _mListView->setContentSize(Size(winSize.width - 100, winSize.height - 310 - 400));
    _mListView->setAnchorPoint(Vec2(0, 1));
    _mListView->setPosition(Vec2(50, winSize.height - 510));
    _mListView->setItemsMargin(0);
    _mListView->setScrollBarEnabled(false);
    _mListView->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(_mListView);
}

void OtherGameTypeView::initData(Json::Value _value){
    
    Size winSize = Director::getInstance()->getWinSize();
    
    Json::Value &listData = _value;
    int nums = listData.size();
    if(nums > 0){
        int mary = (winSize.width - 1000)/4;
        Size rowSize = Size(winSize.width - 100, 300);
        for (int i = 0; i < nums; i += 3) {//每排
            Layout *layout = Layout::create();
            layout->setContentSize(rowSize);
            
            int gameId = listData[i]["id"].asInt();
            Layout *item = TypeItem::createWithData(listData[i],[=](int gameId){
                if(_callback){
                    this->removeFromParentAndCleanup(true);
                    _callback(gameId);
                }
            });
            item->setPosition(Vec2(mary, 0));
            layout->addChild(item);
            
            if (i + 1 < nums) {
                gameId = listData[i + 1]["id"].asInt();
                item = TypeItem::createWithData(listData[i + 1],[=](int gameId){
                    if(_callback){
                        this->removeFromParentAndCleanup(true);
                        _callback(gameId);
                    }
                });
                item->setPosition(Vec2(mary*2 + 300, 0));
                layout->addChild(item);
            }
            
            if (i + 2 < nums) {
                gameId = listData[i + 2]["id"].asInt();
                item = TypeItem::createWithData(listData[i + 2],[=](int gameId){
                    if(_callback){
                        this->removeFromParentAndCleanup(true);
                        _callback(gameId);
                    }
                });
                item->setPosition(Vec2(mary*3 + 300*2, 0));
                layout->addChild(item);
            }
            
            _mListView->addChild(layout);
        }
    }
}

Layout* TypeItem::createWithData(Json::Value info,const MyCallBack_Int &callback){
    
    int gameId = info["id"].asInt();
    string logUrl = info["logo"].asString();
    string gamename = info["classifyName"].asString();
    
    Layout* lays = Layout::create();
    lays->setAnchorPoint(Vec2(0,0));
    lays->setContentSize(Size(300,300));
    lays->setTouchEnabled(true);
    
    ImageView *typeImage = ImageView::create();
    typeImage->setContentSize(Size(150,150));
    typeImage->setTouchEnabled(false);
    typeImage->ignoreContentAdaptWithSize(false);
    typeImage->setTag(gameId);
    typeImage->setAnchorPoint(Vec2(0,0));
    typeImage->setPosition(Vec2(75,75));
    lays->addChild(typeImage);

    
    if (Utils::hasImageFile(logUrl)) {
        typeImage->loadTexture(Utils::getImageSavePath(logUrl));
    } else {
        typeImage->loadTexture("Hall/notification.png");
        CCHttpAgent::getInstance()->sendHttpImage(logUrl, Utils::getImageSavePath(logUrl), Utils::getImageSavePath(logUrl), [typeImage, logUrl](std::string tag)
                                                  {
                                                      CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                      if (packet) {
                                                          CCHttpAgent::getInstance()->packets.erase(tag);
                                                          delete packet;
                                                      }
                                                      
                                                      if (typeImage == NULL || typeImage->getReferenceCount() <= 0 || typeImage->getReferenceCount() > 10) return;
                                                      
                                                      if(Utils::hasImageFile(logUrl))
                                                      {
                                                          typeImage->loadTexture(Utils::getImageSavePath(logUrl));
                                                      }
                                                  });
    }
    
    Text *name = Text::create(gamename, "AmericanTypewriter", 40);
    name->setContentSize(Size(250,200));
    name->setTextAreaSize(Size(250,200));
    name->setAnchorPoint(Vec2(0.5, 0.5));
    name->ignoreContentAdaptWithSize(false);
    name->setTextColor(Color4B::WHITE);
    name->setPosition(Vec2(150,0));
    name->setTextHorizontalAlignment(TextHAlignment::CENTER);
    name->setTextVerticalAlignment(TextVAlignment::CENTER);
    lays->addChild(name);
    
    lays->addClickEventListener([=](Ref *ref){
        if(callback!= NULL){
            callback(gameId);
        }
    });
    
    return lays;
}

void OtherGameTypeView::setCallBack(const MyCallBack_Int &callback){
    _callback = callback;
}