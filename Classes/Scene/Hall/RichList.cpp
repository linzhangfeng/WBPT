//
//  RichList.cpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/22.
//
//

#include "RichList.hpp"

bool RichList::init(){
    if(!BaseLayer::init()){
        return false;
    }
    Size size = Director::getInstance()->getWinSize();
    
    setHead(270);
    
    setBackground(Color4B(29, 29, 31, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    
    setName("RichList");
    
    setTitle("富豪榜");
    
    Layout* cenglay = Layout::create();
    cenglay->setTouchEnabled(true);
    cenglay->setContentSize(Size(size.width,120));
    cenglay->setPosition(Vec2(0,size.height-270));
    this->addChild(cenglay);
    
    int dx = 334;
    int bx = size.width / 2 - dx;
    int by = size.height - getHeadH() + 90;
    
    {
        Button* button = Button::create("Hall/btn_hold.png",
                                        "Hall/btn_hold.png",
                                        "Hall/btn_hold.png");
        button->setContentSize(Size(320,80));
        button->setScale9Enabled(true);
        button->setPosition(Vec2(bx,by));
        button->setEnabled(true);
        button->setTitleText("昨日充值");
        button->setTitleColor(Color3B::GRAY);
        button->setTitleFontSize(45);
        button->addClickEventListener(CC_CALLBACK_1(RichList::btnCallback, this));
        button->setName("recharge");
        button->setTag(1);
        addChild(button,1000);
        
        bx += dx;
    }
    
    {
        Button* button = Button::create("Hall/btn_hold.png",
                                        "Hall/btn_hold.png",
                                        "Hall/btn_hold.png");
        button->setContentSize(Size(320,80));
        button->setScale9Enabled(true);
        button->setPosition(Vec2(bx,by));
        button->setTitleText("金币榜");
        button->setTitleColor(Color3B::GRAY);
        button->setTitleFontSize(45);
        button->setEnabled(true);
        button->setName("gold");
        button->addClickEventListener(CC_CALLBACK_1(RichList::btnCallback, this));
        button->setTag(2);
        addChild(button,1000);
        
        bx += dx;
    }
    
    {
        Button* button = Button::create("Hall/btn_hold.png",
                                        "Hall/btn_hold.png",
                                        "Hall/btn_hold.png");
        button->setContentSize(Size(320,80));
        button->setScale9Enabled(true);
        button->setEnabled(true);
        button->setTitleText("玩币榜");
        button->setTitleFontSize(45);
        button->setTitleColor(Color3B::GRAY);
        button->setPosition(Vec2(bx,by));
        button->addClickEventListener(CC_CALLBACK_1(RichList::btnCallback, this));
        button->setName("Playmoney");
        button->setTag(3);
        addChild(button,1000);
        
        bx += dx;
    }
    
    m_pLoading = Loading::startLoading(this, size / 2);
    m_pLoading->setVisible(false);
    
    initview();
    Button* btn = (Button*)this->getChildByName("recharge");
    btnCallback(btn);
    initRequestType();
    return true;
}

Text* getTextView(string text){
    Text* _text = Text::create(text, "AmericanTypewriter", 45);
    _text->setTextColor(Color4B(254,243,210,255));
    _text->setAnchorPoint(Vec2(0.5,0.5));
    return _text;
}

void RichList::initview(){
    Size size = Director::getInstance()->getWinSize();
    layLineName = Layout::create();
    layLineName->setContentSize(Size(size.width,105));
    
    LayerColor* color  = LayerColor::create(Color4B(23,23,23,255));
    color->setContentSize(Size(size.width,105));
    layLineName->addChild(color);
    layLineName->setPosition(Vec2(0,size.height-375));
    this->addChild(layLineName);
    
    int leng = size.width/8;
    Text* m_text = getTextView("排名");
    layLineName->addChild(m_text);
    
    m_text->setPosition(Vec2(leng, 52));
    m_text = getTextView("玩家");
    m_text->setPosition(Vec2(leng*3, 52));
    layLineName->addChild(m_text);
    
    Text* mchange_text = getTextView("充值(元)");
    mchange_text->setName("money");
    mchange_text->setPosition(Vec2(leng*6, 52));
    layLineName->addChild(mchange_text);
    
    _mListView = ListView::create();
    _mListView->setDirection(ScrollView::Direction::VERTICAL);
    _mListView->setBounceEnabled(true);
    _mListView->setContentSize(Size(size.width, size.height-375));
    _mListView->setAnchorPoint(Vec2(0, 0));
    _mListView->setPosition(Vec2(0,0));
    _mListView->setScrollBarEnabled(false);
    this->addChild(_mListView);

}

void RichList::btnCallback(Ref* ref){
    Button* btn = (Button*)ref;
    if(_btn != NULL){
        _btn->setTitleColor(Color3B::GRAY);
    }
    btn->setTitleColor(Color3B(254,243,210));
    currentType = btn->getTag();
    Text* mchange_text = (Text*)layLineName->getChildByName("money");
    switch(currentType){
        case 1:
            mchange_text->setString("充值(元)");
            initData(currentType);
            break;
        case 2:
            mchange_text->setString("金币");
            initData(currentType);
            break;
        case 3:
            mchange_text->setString("玩币");
            initData(currentType);
            break;
    }
    _btn = btn;
}


void RichList::initRequestType(){
    Json::Value json;
    m_pLoading->setVisible(true);
    char buff[64];
    snprintf(buff, sizeof(buff), "ranking_%s", "ranking");
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
                                                 CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                 if (packet) {
                                                     if (getReferenceCount() == 0 || getReferenceCount() > 10)
                                                     {
                                                         //nothing
                                                     }
                                                     else
                                                     {
                                                         m_pLoading->setVisible(false);
                                                         if (packet->status == 3) {
                                                             packet->status = 0;
                                                             if (packet->resultIsOK())
                                                             {
                                                                 _value = packet->recvVal["resultMap"];
                                                                 initData(currentType);
                                                             }
                                                             else
                                                             {
                                                                 PlatformHelper::showToast(packet->recvVal["message"].asCString());
                                                             }
                                                         }
                                                         //
                                                     }
                                                     
                                                     //删除packet
                                                     CCHttpAgent::getInstance()->packets.erase(tag);
                                                     delete packet;
                                                 }
                                             },
                                             "ranking/index", json.toStyledString(), buff);
}


void RichList::initData(int type)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    if(_mListView->getChildrenCount() > 0){
        _mListView->removeAllChildren();
    }
    _mListView->jumpToTop();
    Json::Value listData = _value;
    if(type == 1){
        listData = _value["rechargeRanking"];
    }else if(type == 2){
        listData = _value["goldenRanking"];
    }else if(type == 3){
        listData = _value["wanBeiRanking"];
    }
    int nums = listData.size();
    m_pLoading->setVisible(false);
    if(nums > 0){
        for (int i = 0; i < nums; i ++) {
            Layout *item = RichListItem::getItemLayout(listData[i],i,type);
            _mListView->addChild(item);

        }
    }
    _mListView->refreshView();
}

Layout* RichListItem::getItemLayout(Json::Value _json,int index,int type){
     Size winSize = Director::getInstance()->getWinSize();
    Layout* lay = Layout::create();
    int lenfen = winSize.width/8;
    lay->setContentSize(Size(winSize.width,180));
    
    Text* _text = Text::create(__String::createWithFormat("%d",index+1)->getCString(), "AmericanTypewriter", 45);
    _text->setTextColor(Color4B(254,243,210,255));
    _text->setAnchorPoint(Vec2(0.5,0.5));
    _text->setPosition(Vec2(lenfen,90));
    lay->addChild(_text);
    
    string userImage = _json["image"].asString();
    if (!userImage.empty()) {
        Sprite *userAvatar = Utils::createCircleAvatar(userImage, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(144,144));
        userAvatar->setPosition(Vec2(lenfen*2, 90));
        lay->addChild(userAvatar);
 
    }
    
    int vipLevel = _json["vipLv"].asInt();
    if(vipLevel > 0){
        ImageView *vipimage = ImageView::create("Hall/user_vip_4.png");
        vipimage->setScale(0.8);
        vipimage->setAnchorPoint(Vec2(0.5, 0.5));
        vipimage->setPosition(Vec2(lenfen*2, 50));
        vipimage->setTouchEnabled(false);
        vipimage->ignoreContentAdaptWithSize(false);
        lay->addChild(vipimage);
    }
    
    Label* userName = Label::createWithSystemFont(_json["nickName"].asString(), "AmericanTypewriter", 45,Size(300,90));
    userName->setTextColor(Color4B::WHITE);
    userName->setAnchorPoint(Vec2(0,0.5));
    userName->setPosition(Vec2(lenfen*2 + 80,90));
    lay->addChild(userName);
    
    Text* userMoney;
    if(type == 1){
        long money = _json["num"].asInt();
        userMoney = Text::create(__String::createWithFormat("%s%ld","￥",money)->getCString(), "AmericanTypewriter", 45);
        userMoney->setTextColor(Color4B::WHITE);
        userMoney->setAnchorPoint(Vec2(0.5,0.5));
        userMoney->setPosition(Vec2(lenfen*6,90));
        lay->addChild(userMoney);
    }else if(type == 2){
        ImageView *moneyimage = ImageView::create("Hall/tab_icon2.png");
        moneyimage->setScale(1);
        moneyimage->setAnchorPoint(Vec2(0.5, 0.5));
        moneyimage->setPosition(Vec2(lenfen*6, 120));
        moneyimage->setTouchEnabled(false);
        moneyimage->ignoreContentAdaptWithSize(false);
        lay->addChild(moneyimage);
        
        long money = _json["num"].asInt();
        __String* numtext;
        if(money >100000000){
            double _money = money/100000000.00;
            numtext = __String::createWithFormat("%.2f %s",_money,"亿");
        }else if(money > 10000){
            double _money = money/10000.00;
            numtext = __String::createWithFormat("%.2f %s",_money,"万");
        }else{
            numtext = __String::createWithFormat("%d",(int)money);
        }
        userMoney = Text::create(numtext->getCString(), "AmericanTypewriter", 45);
        userMoney->setTextColor(Color4B::WHITE);
        userMoney->setAnchorPoint(Vec2(0.5,0.5));
        userMoney->setPosition(Vec2(lenfen*6,50));
        lay->addChild(userMoney);
    }else if(type == 3){
        ImageView *moneyimage = ImageView::create("Hall/tab_icon3.png");
        moneyimage->setScale(1);
        moneyimage->setAnchorPoint(Vec2(0.5, 0.5));
        moneyimage->setPosition(Vec2(lenfen*6, 120));
        moneyimage->setTouchEnabled(false);
        moneyimage->ignoreContentAdaptWithSize(false);
        lay->addChild(moneyimage);
        
        long money = _json["num"].asInt();
        __String* numtext;
        if(money >100000000){
            double _money = money/100000000.00;
            numtext = __String::createWithFormat("%.2f %s",_money,"亿");
        }else if(money > 10000){
            double _money = money/10000.00;
            numtext = __String::createWithFormat("%.2f %s",_money,"万");
        }else{
            numtext = __String::createWithFormat("%d",(int)money);
        }
        userMoney = Text::create(numtext->getCString(), "AmericanTypewriter", 45);
        userMoney->setTextColor(Color4B::WHITE);
        userMoney->setAnchorPoint(Vec2(0.5,0.5));
        userMoney->setPosition(Vec2(lenfen*6,50));
        lay->addChild(userMoney);
    }
    
    ImageView *lineLay = ImageView::create("Head/menu_up_line.png");
    lineLay->setContentSize(Size(winSize.width,2));
    lineLay->setAnchorPoint(Vec2(0.5, 0.5));
    lineLay->setPosition(Vec2(winSize.width/2, 2));
    lineLay->setTouchEnabled(false);
    lineLay->ignoreContentAdaptWithSize(false);
    lay->addChild(lineLay);
    
    return lay;
}
