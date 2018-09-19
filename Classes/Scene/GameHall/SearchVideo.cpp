//
//  SearchVideo.cpp
//  ZJH
//
//  Created by mac on 2017/5/17.
//
//

#include "SearchVideo.hpp"
#include "RecordVideoBox.hpp"
bool SearchVideo::init()
{
    if (!Layout::init())
    {
        return false;
    }
    
    Size size = Director::getInstance()->getWinSize();
    setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    setBackGroundColorOpacity(150);
    setTouchEnabled(true);
    setSwallowTouches(true);
    setContentSize(size);
    addClickEventListener([=](Ref*ref){

    });
    
    ImageView * bg = ImageView::create("GameHall/search_video_bg.png");
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setPosition(Vec2(size.width/2, size.height/2));
    addChild(bg);
    
    float H = bg->getContentSize().height;
    float bgW = bg->getContentSize().width;
    
    Button *cancel = Button::create("GameHall/kefu/record_btn_003.png", "GameHall/kefu/record_btn_003c.png");
    cancel->setPosition(Vec2(bgW - 15, H - 15));
    cancel->addClickEventListener([=](Ref*ref){
        removeFromParent();
    });
    bg->addChild(cancel);
    
    ImageView *title = ImageView::create("GameHall/enter_video_code.png");
    title->setAnchorPoint(Vec2(0, 0.5));
    title->setPosition(Vec2(100, H - 110));
    bg->addChild(title);
    
    EditBox* edit = EditBox::create(Size(bg->getContentSize().width - 200, 100), "GameHall/video_input_bg.png");
    edit->setPlaceholderFontSize(42);
    edit->setAnchorPoint(Vec2(0.5, 0.5));
    edit->setPosition(Vec2(bg->getContentSize().width / 2, H - 210));
    edit->setFontColor(Color3B(0x99, 0x99, 0x99));
    edit->setFontSize(40);
    bg->addChild(edit);
    edit->setInputMode(EditBox::InputMode::NUMERIC);
    
    Button *but = Button::create("GameHall/look.png","GameHall/look_press.png");
    but->setAnchorPoint(Vec2(0.5, 0.5));
    but->setPosition(Vec2(bgW/2, 150));
    bg->addChild(but);
    but->addClickEventListener([=](Ref* sender)
                               {
                                   Json::Value json;
                                   json["code"] = edit->getText();
                                   CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
                                       CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
                                       if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
                                       
                                       if (loginPacket->status != 3)
                                       {
                                           PlatformHelper::showToast("网络链接失败，请稍后再试");
                                           return;
                                       }
                                       
                                       if (loginPacket->resultIsOK())
                                       {
                                           Json::Value data = loginPacket->recvVal["resultMap"];
                                           requestVideoInfo(data);
                                       }else
                                       {
                                           PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                                       }
                                   },"room/dz/video/get", json.toStyledString(), "videoget");
                                   
                               });
    return true;
}



void SearchVideo::requestVideoInfo(Json::Value value){
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"];
            data["roomID"] = value["roomId"];
            data["gameId"] = value["gameId"];
            data["pkgId"] = value["pkgId"];
            auto box =  RecordVideoBox::create();
            this->getParent()->addChild(box,1000000);
            box->showView(data);
            if (ZJHModel::getInstance()->curGameID == -1)
            {
                ZJHModel::getInstance()->setCurGameID(value["gameId"].asInt(), 1);
            }
            removeFromParent();
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    },"duiju/integral/detail", value.toStyledString(), "detail_hall");
}






