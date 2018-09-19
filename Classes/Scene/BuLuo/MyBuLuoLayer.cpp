//
//  FriendLayer.cpp
//  wanMain
//
//  Created by apple on 16/6/23.
//
//

#include "MyBuLuoLayer.h"
#include "../Mine/MaskSelectLayer.hpp"

MyBuLuoLayer::MyBuLuoLayer()
{
    
}

bool MyBuLuoLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){

        this->disappear();
    });
    setTitle("我的俱乐部");
    setName("MyBuLuoLayer");
    
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();
    
    BG = Layout::create();
    BG->setContentSize(Size(visibleSize.width, H));
    this->addChild(BG);
    
    
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"tribe/info", "" ,"Mybuluo");
    this->schedule(schedule_selector(MyBuLuoLayer::postData), 0.0f);
    
    return true;
}

void MyBuLuoLayer::postData(float dt)
{
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["Mybuluo"];
    if (packet && packet->status != 0)
    {
        if (packet->status != 3)
        {
            return;
        }
        packet->status = 0;
        
        if (packet->resultIsOK())
        {
            Json::Value data = packet->recvVal["resultMap"]["tribe"];
            
            infoJson = data;
            
            showInfoView(data);
            
            log("俱乐部：＝＝＝ [%s]",data.toStyledString().c_str());
            
        }else if (packet->recvVal["status"].asInt() == 11001)
        {
            noBuoLuoInfoView();
        }
        else
        {
            PlatformHelper::showToast(packet->recvVal["message"].asCString());
        }
        
        unschedule(schedule_selector(MyBuLuoLayer::postData));
        CCHttpAgent::getInstance()->packets["Mybuluo"] = NULL;
        delete packet;
    }
}

void MyBuLuoLayer::noBuoLuoInfoView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();

    ImageView*icon = ImageView::create("Mine/buluo_ICON.png");
    icon->setPosition(Vec2(visibleSize.width/2, H - 300));
    BG->addChild(icon);
    
    Text *title1 = Text::create("加入家乡俱乐部本地人一起对战", "", 44);
    title1->setPosition(Vec2(visibleSize.width/2, H - 490));
    title1->setTextColor(Color4B(0xdf, 0xce, 0xad, 255));
    BG->addChild(title1);
    
    Text *title2 = Text::create("使用收到的邀请码加入", "", 44);
    title2->setPosition(Vec2(visibleSize.width/2, H - 680));
    title2->setTextColor(Color4B(0x65, 0x65, 0x65, 255));
    BG->addChild(title2);
    
    textField2 = EditBox::create(Size(960, 120), "Mine/bar_sendtxt.png");
    textField2->setMaxLength(15);
    textField2->setPlaceHolder("请输入收到的邀请码");
    textField2->setFontSize(40);
    textField2->setPlaceholderFontSize(40);
    textField2->setInputMode(EditBox::InputMode::NUMERIC);
    textField2->setPosition(Vec2(visibleSize.width/2, H/2));
    textField2->setFontColor(Color3B(0x99, 0x99, 0x99));
    textField2->setFontSize(40);
    BG->addChild(textField2);
    
    Button *sureBtn = Button::create("Mine/btn_yellow_big.png","Mine/btn_yellow_big_check.png");
    sureBtn->setScale9Enabled(true);
    sureBtn->setContentSize(Size(960, 130));
    sureBtn->setPosition(Vec2(visibleSize.width/2, H/2 - 200));
    BG->addChild(sureBtn);
    sureBtn->addClickEventListener(CC_CALLBACK_1(MyBuLuoLayer::sureJoinOn, this));
    
    
    Text *sureTitle = Text::create("确认", "", 40);
    sureTitle->setPosition(Vec2(sureBtn->getContentSize().width/2, sureBtn->getContentSize().height/2));
    sureTitle->setTextColor(Color4B(0x65, 0x40, 0x02, 255));
    sureBtn->addChild(sureTitle);
    

    /*
    auto layoutBg = Layout::create();
    layoutBg->setTouchEnabled(true);
    layoutBg->addClickEventListener([=](Ref*ref){
    
        MaskSelectLayer *vipLayer = MaskSelectLayer::create(2);
        vipLayer->setPosition(Vec2(0, 220));
        
    });
    layoutBg->setContentSize(Size(visibleSize.width, 200));
    BG->addChild(layoutBg);
    

    Text *joinTitle = Text::create("我要创建俱乐部", "", 40);
    joinTitle->setPosition(Vec2(visibleSize.width/2 , 100));
    joinTitle->setTextColor(Color4B(0x65, 0x65, 0x65, 255));
    layoutBg->addChild(joinTitle);
    joinTitle->setTouchEnabled(true);
    joinTitle->addClickEventListener(CC_CALLBACK_1(MyBuLuoLayer::joinBuLuo, this));
    
    ImageView *arrow = ImageView::create("Public/detail_arrow.png");
    arrow->ignoreContentAdaptWithSize(false);
    arrow->setAnchorPoint(Vec2(1, 0.5));
    arrow->setPosition(Vec2(visibleSize.width/2 + 150, 100));
    layoutBg->addChild(arrow);
    
     */
}

void MyBuLuoLayer::joinBuLuo(Ref*ref)
{
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
            Json::Value data = loginPacket->recvVal["resultMap"]["vipStoreList"];
            showBuyZuZhang(data);
            
            log("VIP信息：＝＝＝ [%s]",data.toStyledString().c_str());
            
        }
    },"vip/getVipStoreList","","VipStoreList");
}

void MyBuLuoLayer::showBuyZuZhang(Json::Value json)
{
    MaskSelectLayer *buyLayer = MaskSelectLayer::create(2);
    buyLayer->setPosition(Vec2(0, 0));
    buyLayer->getVipJson(json[2]);
    this->addChild(buyLayer, 100);

}
void MyBuLuoLayer::sureJoinOn(Ref*ref)
{
    Json::Value json1;
    json1["code"] = textField2->getText();
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            BG->removeAllChildren();
            CCHttpAgent::getInstance()->sendHttpPost(NULL,"tribe/info", "" ,"Mybuluo");
            this->schedule(schedule_selector(MyBuLuoLayer::postData), 0.0f);
            ZJHModel::getInstance()->isTrait = true;
            
            //加入、退出部落账号时发送的主界面的消息，暂时主界面的部落信息已被屏蔽
            //__NotificationCenter::getInstance()->postNotification("buLuo_InfoChange");
            
            
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    },"tribe/join",json1.toStyledString(),"tribeJoin");

}

void MyBuLuoLayer::showInfoView(Json::Value json)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();
    
    ImageView*infoBg = ImageView::create("Mine/my_buluo_BG.png");
    infoBg->setContentSize(Size(visibleSize.width, 335));
    infoBg->ignoreContentAdaptWithSize(false);
    infoBg->setAnchorPoint(Vec2(0, 1));
    infoBg->setPosition(Vec2(0, H));
    BG->addChild(infoBg);
    infoBg->setTouchEnabled(true);
    infoBg->addClickEventListener([=](Ref*ref){
       
        ZJHModel::getInstance()->gotoView(VIEW_BULUO_INFO, infoJson);
        
    });

    float bgH = infoBg->getContentSize().height;
    ImageView *arrow = ImageView::create("Mine/me_comeback.png");
    arrow->ignoreContentAdaptWithSize(false);
    arrow->setAnchorPoint(Vec2(1, 0.5));
    arrow->setPosition(Vec2(visibleSize.width - 65 , bgH/2));
    infoBg->addChild(arrow);
    
    //头像
    Sprite *userPortrit = Utils::createCircleAvatar(json["logo"].asString() , "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(160,160));
    userPortrit->setPosition(Vec2(160, infoBg->getContentSize().height/2));
    infoBg->addChild(userPortrit, 20);

    //name
    Text *name = Text::create(json["name"].asString(), "", 44);
    name->setAnchorPoint(Vec2(0, 1));
    name->setPosition(Vec2(325, bgH - 95));
    name->setTextColor(Color4B(0xe1, 0xcd, 0xad, 255));
    infoBg->addChild(name);
    char buf[128];
    sprintf(buf, "ID:%d", json["id"].asInt());
    
    Text *ID = Text::create(buf, "", 44);
    ID->setAnchorPoint(Vec2(0, 1));
    ID->setPosition(Vec2(325, bgH - 160));
    ID->setTextColor(Color4B(0xb4, 0xb4, 0xb5, 255));
    infoBg->addChild(ID);
    
    sprintf(buf, "等级%d级", json["level"].asInt());
    
    Text *LV = Text::create(buf, "", 44);
    LV->setAnchorPoint(Vec2(0, 1));
    LV->setPosition(Vec2(325, bgH - 210));
    LV->setTextColor(Color4B(0xb4, 0xb4, 0xb5, 255));
    infoBg->addChild(LV);
    
    ListView *listView = ListView::create();
    listView->setInertiaScrollEnabled(false);
    listView->setDirection(ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(visibleSize.width, H - bgH));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 0));
    listView->setItemsMargin(0);
    listView->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    BG->addChild(listView);

    for (int i = 0; i < 3; i++)
    {
        Layout* cell;
        Text *leftText;
        if (i <  2)
        {
            cell = Layout::create();
            cell->setTouchEnabled(true);
            cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
            cell->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
            cell->setContentSize(Size(visibleSize.width, 180));
            listView->addChild(cell);
            
            leftText = Text::create("", "", 44);
            leftText->setAnchorPoint(Vec2(0, 0.5));
            leftText->setTextColor(Color4B(0xe6, 0xe6, 0xe6, 255));
            cell->addChild(leftText);
            
            auto line = Layout::create();
            line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
            line->setBackGroundColor(Color3B(0, 0, 0));
            line->setContentSize(Size(visibleSize.width, 1));
            cell->addChild(line);
        }
        
        
        switch (i)
        {
            case 0:
                sprintf(buf, "管理员(%d人)", json["managerCount"].asInt());
                leftText->setString(buf);
                leftText->setPosition(Vec2(40, cell->getContentSize().height/2));
                break;
            case 1:
                
                sprintf(buf, "俱乐部成员(%d人)", json["memberCount"].asInt());
                leftText->setString(buf);
                leftText->setPosition(Vec2(40, cell->getContentSize().height/2));

                break;
            case 2:
                showBuLuoGongNeng(listView);
                break;
        }
    }
    
    if(ZJHModel::getInstance()->vipLv == 3)
    {
        return;
    }
    Layout *exitBg = Layout::create();
    exitBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    exitBg->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    exitBg->setContentSize(Size(visibleSize.width, 210));
    BG->addChild(exitBg);
    
    auto line = Layout::create();
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->ignoreAnchorPointForPosition(false);
    line->setAnchorPoint(Vec2(0, 1));
    line->setBackGroundColor(Color3B(0, 0, 0));
    line->setPosition(Vec2(0, 210));
    line->setContentSize(Size(visibleSize.width, 5));
    exitBg->addChild(line);
    
    Button *exitBtn = Button::create("Public/btn_red.png","Public/btn_red.png", "Public/btn_red.png");
    exitBtn->setScale9Enabled(true);
    exitBtn->setContentSize(Size(960, 115));
    exitBtn->setPosition(Vec2(visibleSize.width/2, 105));
    exitBg->addChild(exitBtn);
    exitBtn->setTitleText("退出俱乐部");
    exitBtn->setTitleFontSize(40);
    exitBtn->setTitleColor(Color3B(255, 255, 255));
    exitBtn->setTitleFontName(".SFUIDisplay-Semibold");
    exitBtn->addClickEventListener(CC_CALLBACK_1(MyBuLuoLayer::exitTipView, this));

}

void MyBuLuoLayer::exitTipView(Ref*ref)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout *mask = Layout::create();
    mask->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    mask->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    mask->setBackGroundColorOpacity(200);
    mask->setContentSize(visibleSize);
    this->addChild(mask);
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(900, 400));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    mask->addChild(bg);
    
    RichText *title = RichText::create();
    title->ignoreContentAdaptWithSize(true);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2+50));
    bg->addChild(title);
    title->setContentSize(Size(visibleSize.width, 60));
    
    RichElementText* re1 = RichElementText::create(1, Color3B(0xff, 0xff, 0xff), 255, "确定退出当前", "", 40);
    title->pushBackElement(re1);
    
    RichElementText* re2 = RichElementText::create(1, Color3B(0xf0, 0xcb, 0x5c), 255, "[" + infoJson["name"].asString() + "]", "", 40);
    title->pushBackElement(re2);
    
    RichElementText* re3 = RichElementText::create(1, Color3B(0xff, 0xff, 0xff), 255, "俱乐部吗？", "", 40);
    title->pushBackElement(re3);
    
    Button *cacelBut = Button::create("Mine/btn_yellow_small.png", "Mine/btn_yellow_small.png");
    cacelBut->setScale9Enabled(true);
    cacelBut->setAnchorPoint(Vec2(0, 0));
    cacelBut->setContentSize(Size(300 , 85));
    cacelBut->setPosition(Vec2(50, 40));
    bg->addChild(cacelBut);
    cacelBut->addClickEventListener([=](Ref*ref){
        
        mask->removeFromParent();
    });
    
    Text *titleCancel = Text::create("取消", ".SFUIDisplay-Semibold", 40);
    titleCancel->setTextColor(Color4B(0x60,0x41,0x10, 255));
    titleCancel->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleCancel->setPosition(Vec2(cacelBut->getContentSize().width/2, cacelBut->getContentSize().height/2));
    cacelBut->addChild(titleCancel);
    
    Button *tongBut = Button::create("Mine/btn_yellow_small.png", "Mine/btn_yellow_small_check.png");
    tongBut->setScale9Enabled(true);
    tongBut->setAnchorPoint(Vec2(1, 0));
    tongBut->setContentSize(Size(300 , 85));
    tongBut->setPosition(Vec2(bg->getContentSize().width - 50, 40));
    bg->addChild(tongBut);
    tongBut->addClickEventListener([=](Ref*ref){
        exitBuLuoOn();
        mask->removeFromParent();
    });
    
    Text *titleTong = Text::create("确认退出", ".SFUIDisplay-Semibold", 40);
    titleTong->setTextColor(Color4B(0x60,0x41,0x10, 255));
    titleTong->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleTong->setPosition(Vec2(tongBut->getContentSize().width/2, tongBut->getContentSize().height/2));
    tongBut->addChild(titleTong);
}

void MyBuLuoLayer::exitBuLuoOn()
{
    Json::Value json1;
    json1["targetUserId"] = ZJHModel::getInstance()->uid;
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            BG->removeAllChildren();
            noBuoLuoInfoView();
            ZJHModel::getInstance()->isTrait = false;
            
            //加入、退出部落账号时发送的主界面的消息，暂时主界面的部落信息已被屏蔽
            //__NotificationCenter::getInstance()->postNotification("buLuo_InfoChange");
            
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    },"tribe/exit",json1.toStyledString(),"tribeExit");
    
}
void MyBuLuoLayer::showBuLuoGongNeng(ListView *listView)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto item1 = Layout::create();
    item1->setTouchEnabled(true);
    item1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    item1->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    item1->setContentSize(Size(visibleSize.width, 130));
    item1->setAnchorPoint(Vec2(0, 1));
    item1->ignoreAnchorPointForPosition(false);
    listView->addChild(item1);
    
    
    Text *leftText = Text::create("俱乐部功能", "", 44);
    leftText->setAnchorPoint(Vec2(0, 0.5));
    leftText->setPosition(Vec2(40, item1->getContentSize().height/2));
    leftText->setTextColor(Color4B(0xe6, 0xe6, 0xe6, 255));
    item1->addChild(leftText);
    
    auto line1 = Layout::create();
    line1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line1->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
    line1->setContentSize(Size(visibleSize.width, 1));
    item1->addChild(line1);
    
    auto item2 = Layout::create();
    item2->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    item2->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    item2->setContentSize(Size(visibleSize.width, 320));
    listView->addChild(item2);
    
    Vec2 btnPos[3] = {
        Vec2(130 + 80, item2->getContentSize().height/2),
        Vec2(visibleSize.width / 2, item2->getContentSize().height/2),
        Vec2(visibleSize.width - 130 - 80, item2->getContentSize().height/2),
    };
    
    if (ZJHModel::getInstance()->vipLv == 3)
    {
        for (int i = 0; i < 4; i++)
        {
            if (i == 3)
            {
                item2 = Layout::create();
                listView->addChild(item2);
                item2->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                item2->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
                item2->setContentSize(Size(visibleSize.width, 320));
            }
            
            Button *btn;
            switch (i)
            {
                case 0:
                    btn = Button::create("Mine/buluo_tuiguangyaoqingma.png", "Mine/buluo_tuiguangyaoqingma.png", "Mine/buluo_tuiguangyaoqingma.png");
                    btn->addClickEventListener([=](Ref*ref){
                        ZJHModel::getInstance()->gotoView(VIEW_BULUO_TUI_GUANG, infoJson);
                    });
                    break;
                case 1:
                    btn = Button::create("Mine/buluo_daka.png", "Mine/buluo_daka.png", "Mine/buluo_daka.png");
                    break;
                case 2:
                    btn = Button::create("Mine/buluo_youhui.png", "Mine/buluo_youhui.png", "Mine/buluo_youhui.png");
                    break;
                case 3:
                    btn = Button::create("Mine/buluo_linggongzi.png", "Mine/buluo_linggongzi.png", "Mine/buluo_linggongzi.png");
                    break;
                    
            }
            btn->setPosition(btnPos[i%3]);
            item2->addChild(btn);
            if (i != 0)
            {
                Text *title = Text::create("敬请期待", "", 34);
                title->setAnchorPoint(Vec2(0.5, 0));
                title->setPosition(Vec2(btn->getPositionX(), 10));
                title->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
                item2->addChild(title);
            }
            
        }

        
    }else
    {
        for (int i = 0; i < 3; i++)
        {
            Button *btn;
            switch (i)
            {
                case 0:
                    btn = Button::create("Mine/buluo_daka.png", "Mine/buluo_daka.png", "Mine/buluo_daka.png");
                    break;
                case 1:
                    btn = Button::create("Mine/buluo_youhui.png", "Mine/buluo_youhui.png", "Mine/buluo_youhui.png");
                    break;
                case 2:
                    btn = Button::create("Mine/buluo_linggongzi.png", "Mine/buluo_linggongzi.png", "Mine/buluo_linggongzi.png");
                    break;
            }
            btn->setPosition(btnPos[i%3]);
            item2->addChild(btn);
            
            Text *title = Text::create("敬请期待", "", 34);
            title->setAnchorPoint(Vec2(0.5, 0));
            title->setPosition(Vec2(btn->getPositionX(), 10));
            title->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
            item2->addChild(title);
            
        }
    }
    
    auto line2 = Layout::create();
    line2->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line2->setBackGroundColor(Color3B(0, 0, 0));
    line2->setContentSize(Size(visibleSize.width, 2));
    item2->addChild(line2);
}
