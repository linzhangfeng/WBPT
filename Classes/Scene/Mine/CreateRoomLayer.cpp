//
//  CreateRoomLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/11.
//
//

#include "CreateRoomLayer.hpp"
#include "CreateRoomFinishMask.hpp"
#include "MaskTipBuyGold.hpp"

#define CELLH1  90
#define CELLH2  160
#define ITERH_MAX   30
#define ITERH_MIN   3

#define PUBIC_CELL_TAG  501
#define PRIVATE_CELL_TAG  500
#define GOLD_CELL_TAG  502

bool CreateRoomLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("开房间");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x17, 0x17, 0x17, 255));
    setHead(270);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();
    
    
    Button* btn1 = Button::create("Mine/createRoomSiMi_1.png","Mine/createRoomSiMi_2.png", "Mine/createRoomSiMi_2.png");
    btn1->setName("private");
    btn1->setPosition(Vec2(visibleSize.width/2 - btn1->getContentSize().width/2, H + 90));
    btn1->addClickEventListener(CC_CALLBACK_1(CreateRoomLayer::tabClickFun, this));
    this->addChild(btn1, 1000);
    btn1->setEnabled(false);
    
    Button* btn2 = Button::create("Mine/createRoomPublic_1.png","Mine/createRoomPublic_2.png","Mine/createRoomPublic_2.png");
    btn2->setName("pubilc");
    btn2->setPosition(Vec2(visibleSize.width/2 + btn1->getContentSize().width/2, btn1->getPositionY()));
    btn2->addClickEventListener(CC_CALLBACK_1(CreateRoomLayer::tabClickFun, this));
    this->addChild(btn2, 1000);
    
    
    Layout *createBg = Layout::create();
    createBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    createBg->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    createBg->setContentSize(Size(visibleSize.width, 210));
    this->addChild(createBg);
    
    auto line = Layout::create();
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->ignoreAnchorPointForPosition(false);
    line->setAnchorPoint(Vec2(0, 1));
    line->setBackGroundColor(Color3B(0, 0, 0));
    line->setPosition(Vec2(0, 210));
    line->setContentSize(Size(visibleSize.width, 3));
    createBg->addChild(line);
    
    createBtn = Button::create("Public/btn_yellow.png","Public/btn_yellow.png");
    createBtn->setScale9Enabled(true);
    createBtn->setContentSize(Size(960, 115));
    createBtn->setPosition(Vec2(visibleSize.width/2, 105));
    createBg->addChild(createBtn);
    createBtn->setTitleText("确认创建");
    createBtn->setTitleFontSize(40);
    createBtn->setTitleColor(Color3B(255, 255, 255));
    createBtn->setTitleFontName("");
    createBtn->addTouchEventListener(CC_CALLBACK_2(CreateRoomLayer::sureOn, this));
    
    showView();
    
    return true;
}

void CreateRoomLayer::tabClickFun(Ref* ref)
{
    Button *tab = (Button*)ref;
    Button * privateBtn = (Button*)this->getChildByName("private");
    Button * pubilcBtn = (Button*)this->getChildByName("pubilc");
    
    if (tab == privateBtn)
    {
        roomType = 0;
        pubilcBtn->setEnabled(true);
        privateView();
        
    }else if (tab == pubilcBtn)
    {
        roomType = 1;
        privateBtn->setEnabled(true);
        publicView();
    }
    
    tab->setEnabled(false);

}

void CreateRoomLayer::privateView()
{
    string nick = ZJHModel::getInstance()->nick + "的私密房";
    roomName->setPlaceHolder(nick.c_str());
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();

    peNumBg->setVisible(true);

    tip1->setString("提示：");
    tip1->setPosition(Vec2(55, H - 700));
  
    tip2->setString("私密房仅邀请的人可进入");
    tip2->setPosition(Vec2(55, tip1->getPositionY() - 40));
    
    tip3->setString("创建私密房需要VIP会员以上");
    tip3->setPosition(Vec2(55, tip2->getPositionY() - 40));
    
}

void CreateRoomLayer::publicView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();
    
    peNumBg->setVisible(false);
    
    string nick = ZJHModel::getInstance()->nick + "的公开房";
    roomName->setPlaceHolder(nick.c_str());

    tip1->setPosition(Vec2(55, H - 480));
    
    tip2->setString("公开房任何人可进入");
    tip2->setPosition(Vec2(55, tip1->getPositionY() - 40));
    
    tip3->setString("创建公开房需要开通族长会员");
    tip3->setPosition(Vec2(55, tip2->getPositionY() - 40));
}

void CreateRoomLayer::showView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float H = visibleSize.height - getHeadH();
    
    tip1 = Text::create("", "", 32);
    tip1->setAnchorPoint(Vec2(0, 1));
    tip1->setPosition(Vec2(55, H - 700));
    tip1->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
    this->addChild(tip1);
    
    tip2 = Text::create("", "", 32);
    tip2->setAnchorPoint(Vec2(0, 1));
    tip2->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
    this->addChild(tip2);
    
    tip3 = Text::create("", "", 32);
    tip3->setAnchorPoint(Vec2(0, 1));
    tip3->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
    this->addChild(tip3);

    
    peNumBg = Layout::create();
    peNumBg->ignoreAnchorPointForPosition(false);
    peNumBg->setAnchorPoint(Vec2(0, 1));
    peNumBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    peNumBg->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1d));
    peNumBg->setContentSize(Size(visibleSize.width, 180));
    peNumBg->setPosition(Vec2(0, H - 475));
    this->addChild(peNumBg);
    
    Text *title = Text::create("最大人数", "", 40);
    title->setPosition(Vec2(200, 90));
    title->setTextColor(Color4B(0xa6, 0xa6, 0xa6, 255));
    title->setAnchorPoint(Vec2(1, 0.5));
    peNumBg->addChild(title);
    
    Text *right = Text::create("100人", "", 40);
    right->setAnchorPoint(Vec2(1, 0.5));
    right->setPosition(Vec2(visibleSize.width - 45, 90));
    right->setTextColor(Color4B(0xa6, 0xa6, 0xa6, 255));
    peNumBg->addChild(right);
    
    //房间名称
    for (int i = 0; i < 2; i++)
    {
        auto cell = Layout::create();
        cell->ignoreAnchorPointForPosition(false);
        cell->setAnchorPoint(Vec2(0, 1));
        cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        cell->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1d));
        
        this->addChild(cell);
        
        Text *title = Text::create("", "", 40);
        title->setPosition(Vec2(200, 90));
        title->setTextColor(Color4B(0xe2, 0xcd, 0xad, 255));
        title->setAnchorPoint(Vec2(1, 0.5));
        title->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        cell->addChild(title);
        EditBox* edit;
        switch (i)
        {
            case 0:
                edit = EditBox::create(Size(760, 90), "Mine/bar_sendtxt.png");
                edit->setPlaceholderFontSize(40);
                edit->setAnchorPoint(Vec2(0, 0.5));
                edit->setMaxLength(20);
                edit->setInputMode(EditBox::InputMode::ANY);
                edit->setPosition(Vec2(240, 90));
                edit->setFontColor(Color3B(0x99, 0x99, 0x99));
                edit->setPlaceholderFontColor(Color3B(0x4c, 0x4c, 0x4c));
                edit->setFontSize(40);
                edit->setPlaceholderFontSize(36);
                cell->addChild(edit);
                roomName = edit;
                 edit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
                cell->setContentSize(Size(visibleSize.width, 180));
                cell->setPosition(Vec2(0, H));
                title->setString("房间名称");
                break;
            case 1:
            {
                cell->setContentSize(Size(visibleSize.width, 245));
                cell->setPosition(Vec2(0, H - 205));
                title->setString("娱乐类型");

                ListView *list = showGameTypeList();
                list->setPosition(Vec2(250, 0));
                cell->addChild(list);
            }
                break;
        }
    }
    //首先显示私密房
    privateView();
    
}

//小滑动条
ListView * CreateRoomLayer::showGameTypeList()
{
    ListView *listView = ListView::create();
    listView->setInertiaScrollEnabled(false);
    listView->setDirection(ScrollView::Direction::HORIZONTAL);
    listView->setBounceEnabled(true);
    listView->setContentSize(Size(800, 230));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 0));
    listView->setItemsMargin(20);
    listView->setGravity(ListView::Gravity::CENTER_VERTICAL);
    
    //小滑动条
    Json::Value jsonList = ZJHModel::getInstance()->gameConfigJson["list"]["gameTypeList"];
    
    //printf("BBB == %s\n", ZJHModel::getInstance()->gameConfigJson["list"].toStyledString().c_str());
    int num = jsonList.size();
    for (int i = 0; i < num; i++)
    {
        int gameId = jsonList[i]["id"].asInt();
        std::string logUrl = jsonList[i]["logUrl"].asString();
        
        auto Bg = Layout::create();
        Bg->setTag(gameId);
        Bg->setTouchEnabled(true);
        Bg->addTouchEventListener(CC_CALLBACK_2(CreateRoomLayer::selectImage, this));
        Bg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        Bg->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1d));
        Bg->ignoreAnchorPointForPosition(false);
        Bg->setContentSize(Size(160, 230));
        listView->addChild(Bg);
        
        ImageView *bgImage = ImageView::create("Mine/check_icon.png");
        bgImage->setTag(101);
        bgImage->setVisible(false);
        bgImage->setPosition(Vec2(80, 115));
        Bg->addChild(bgImage);
        
        ImageView *typeImage = ImageView::create();
        typeImage->setContentSize(Size(140, 140));
        typeImage->ignoreContentAdaptWithSize(false);
        typeImage->setPosition(Vec2(80, 140));
        Bg->addChild(typeImage);
        
        LabelTTF *text7 = LabelTTF::create(jsonList[i]["gameName"].asString(), "", 32,
                                      Size(bgImage->getContentSize().width, 40), TextHAlignment::CENTER);
        text7->setTag(100);
        text7->setAnchorPoint(Vec2(0.5, 0));
        text7->setPosition(Vec2(80, 20));
        text7->setColor(Color3B(0xa6, 0xa6, 0xa6));
        Bg->addChild(text7);
        
        if(i == 0)
        {
            lastSelect = Bg;
            bgImage->setVisible(true);
            text7->setColor(Color3B(0x65, 0x40, 0x05));
        }
        
        if (Utils::hasImageFile(logUrl))
        {
            typeImage->loadTexture(Utils::getImageSavePath(logUrl));
        }
        else
        {
            CCHttpAgent::getInstance()->sendHttpImage(logUrl, Utils::getImageSavePath(logUrl), Utils::getImageSavePath(logUrl), [typeImage, logUrl](std::string tag)
                                                      {
                                                          CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                          if (packet) {
                                                              CCHttpAgent::getInstance()->packets.erase(tag);
                                                              delete packet;
                                                          }
                                                          
                                                          if (typeImage == NULL || typeImage->getReferenceCount() <= 0 || typeImage->getReferenceCount() > 10)
                                                              return;
                                                          
                                                          if(Utils::hasImageFile(logUrl))
                                                          {
                                                              typeImage->loadTexture(Utils::getImageSavePath(logUrl));
                                                          }
                                                      });
        }

    }
    return listView;

}

void CreateRoomLayer::selectImage(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        //处理上一个
        LabelTTF *text1 = (LabelTTF *)lastSelect->getChildByTag(100);
        ImageView *mark1 = (ImageView *)lastSelect->getChildByTag(101);
        mark1->setVisible(false);
        text1->setColor(Color3B(0xa6, 0xa6, 0xa6));

        //处理下一个
        Layout *image = (Layout *) pSender;
        LabelTTF *text = (LabelTTF *)image->getChildByTag(100);
        ImageView *mark = (ImageView *) image->getChildByTag(101);
        mark->setVisible(true);
        text->setColor(Color3B(0x65, 0x40, 0x05));
        lastSelect = image;
        
        playIndex = image->getTag();
    }
}

void CreateRoomLayer::sureOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        int vipLv = ZJHModel::getInstance()->vipLv;
        if (vipLv < 1)
        {
            Json::Value tipJson;
            MaskTipBuyGold *tip;
            if (roomType == 0)
            {
                tipJson["roomType"] = 0;
                
            }else if (roomType == 1)
            {
                tipJson["roomType"] = 1;
                
            }
            tip = MaskTipBuyGold::create();
            tip->setPosition(Vec2::ZERO);
            tip->showCreateRoom(tipJson);
            this->addChild(tip);
            return;
        }
    
        Json::Value json;
        json["roomName"] =  roomName->getText(); //房间名称
        json["visible"] = roomType;    //0:私密 1:公开
        json["gameTypeId"] = playIndex;
        json["currencyType"] = 0; //筹码房或者金币房 (0:金币 1：筹码 )
        json["groupId"] = 0;    //群房间（暂时不用）
        
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
                //弹出进入房间小窗
                PlatformHelper::showToast("房间创建成功");
                Json::Value data = loginPacket->recvVal["resultMap"];
                
                CreateRoomFinishMask *mask = CreateRoomFinishMask::create();
                mask->setPosition(Vec2::ZERO);
                this->addChild(mask);
                
            }else
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
        },"game/save.action",json.toStyledString(),"room");
        
    }
}


