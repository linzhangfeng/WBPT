//
//  OpenMatchLayer.cpp
//  ZJH
//
//  Created by apple on 16/8/17.
//
//

#include "OpenMatchLayer.hpp"
#include "../Load/LoadTable.h"
#include "../Mine/TipShopLayer.hpp"
#include "../Mine/InviteLayer.hpp"
#include "MyScollView.hpp"
#include "GamePlayingErrorDialog.hpp"
#include "../Loading/Loading.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "../Load/ServerErrorTips.hpp"
#include "../../MyGUI/MatchEnterIconByUrl.h"

#include "GameListView.hpp"
#include "MatchBottomView.hpp"
#include "RoomWithoutListView.hpp"
bool OpenMatchLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }

    setName("OpenMatchLayer");
    addOpenObserver();
    
    setHead(0);
    
    showOpenMatch();
    
    initHttpParam();
    
    //启动查询是否邀请领奖
    CallFunc* func2 = CallFunc::create([=]
                                       {
                                           inviteLoadInto(NULL);
                                       });
    if (ZJHModel::getInstance()->firtEnterApp)runAction(Sequence::create(DelayTime::create(0.5),func2, NULL));
    addChild(GameListView::create());

    UserDefault::getInstance()->setIntegerForKey("lastRoomPlaying", 1);
    CallFunc* func = CallFunc::create([=]
                                      {
                                          ZJHModel::getInstance()->checkGamePlayingError([=](std::string str){
                                              if(str == "normal"){
                                                  gameReload(NULL);
                                                  gameReloadNew(NULL);
                                              }else{
                                                  enterRoomRequestWithCode(str);
                                              }
                                          });
                                      });
    
    if (ZJHModel::getInstance()->firtEnterApp){
        runAction(Sequence::create(DelayTime::create(0.5),func, NULL));
    }
     ZJHModel::getInstance()->firtEnterApp = false;
    
    return true;
}

void OpenMatchLayer::initHttpParam(){
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
            Json::Value data = loginPacket->recvVal["resultMap"]["myInfoVO"];
            
            char buf[128];
            Utils::parseName_1(6, buf, data ["nickName"].asString().c_str());
            nameUser->setString(buf);
            
            std::string userImage = data["thumbImage"].asString();
            ZJHModel::getInstance()->portrit = userImage;
            
            __String *bufStr = __String::createWithFormat("ID：%d", data["id"].asInt());
            userID->setString(bufStr->getCString());
            
            portritBg->removeAllChildren();
            Sprite* userPortrit = Utils::createCircleAvatar(userImage, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(140,140));
            userPortrit->setAnchorPoint(Vec2(0.5,0.5));
            userPortrit->setPosition(Vec2(portritBg->getContentSize().width/2, portritBg->getContentSize().height/2));
            portritBg->addChild(userPortrit);
        }
    },"account/myInfo","","myInfo");

   
    /* 部落小入口数据暂时不用
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
            
            if(data["tribeId"].asInt() != 0)
            {
                ZJHModel::getInstance()->isTrait = true;
            }else
            {
                ZJHModel::getInstance()->isTrait = false;
            }
            
            buLuoEnter();
            
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    },"tribe/isMember","","isBuLuo");
    
    */
    
    
    postMoney(NULL);
    //postFreeGift(NULL);
    //查询是邀请还是领房卡
    refreshCardsInvite(NULL);
}

void OpenMatchLayer::inviteLoadInto(Ref*ref)
{
    string inviteCode = PlatformHelper::loadInviteCode();
    if(inviteCode != "")
    {
        //准备进入推荐领奖界面
        CallFunc* func1 = CallFunc::create([=]
                                           {
                                               InviteLayer* invite = InviteLayer::create();
                                               Director::getInstance()->getRunningScene()->addChild(invite);
                                               invite->inviteCodeInto(inviteCode);
                                           });
        
        runAction(Sequence::create(DelayTime::create(1.0),func1, NULL));
    }
}
void OpenMatchLayer::gameReloadNew(Ref* r)
{
    if(ZJHModel::getInstance()->enterGame)
    {
        return;
    }
    
    string webCode = PlatformHelper::loadCodeNew();
    if(webCode != "")
    {
        PlatformHelper::showToast("正在进入房间");
        fromWebIntoGame(webCode);
    }
}

//
void OpenMatchLayer::gameReload(Ref* r)
{
    string webCode = PlatformHelper::loadCode();
    
    if (webCode.size() != 0)
    {
        const static std::string findFlag = "=";
        int cutPos = webCode.find(findFlag);
        if (cutPos == std::string::npos)
        {
            
        }
        else
        {
            string code = webCode.substr(cutPos + findFlag.length());
            if (webCode.find("webCode") != std::string::npos)
            {
                fromWebIntoGame(code);
            }
            else
            {
                PlatformHelper::sdkDoForVoid_C("copyToPasteboard",code);
                PlatformHelper::showToast("邀请码已复制到剪贴板");
            }
        }
    }
}

void OpenMatchLayer::fromWebIntoGame(std::string code)
{
    CallFunc* func1 = CallFunc::create([=]
                                       {
                                          ZJHModel::getInstance()->gotoRoomWithCode(code);
                                       });
    
    runAction(Sequence::create(DelayTime::create(1.0),func1, NULL));
}

void OpenMatchLayer::addOpenObserver()
{
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(OpenMatchLayer::updatePayAfter), "cz_succ_notify", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(OpenMatchLayer::moneyChange), "money_change", NULL);

     __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(OpenMatchLayer::updatePortrait), "userPotrait_change", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(OpenMatchLayer::updateNick), "nick_change", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(OpenMatchLayer::postMoney), "exit_Room_refere", NULL);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(OpenMatchLayer::gameReload), "game_reload",NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(OpenMatchLayer::gameReloadNew), "game_reload_new",NULL);
    
    //邀请进入客户端
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(OpenMatchLayer::inviteLoadInto), "invite_load_Inter",NULL);
    
    //邀请成功
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(OpenMatchLayer::refreshCardsInvite), "inviteSuccess",NULL);
    
}

void OpenMatchLayer::updatePayAfter(Ref*ref)
{
    CallFunc* func1 = CallFunc::create([=]
                                       {
                                           postMoney(NULL);
                                       });
    
    runAction(Sequence::create(DelayTime::create(1.0),func1, NULL));
    
}


void OpenMatchLayer::updateNick(Ref*ref)
{
    char buf[128];
    Utils::parseName_1(6, buf, ZJHModel::getInstance()->nick.c_str());
    
    nameUser->setString(buf);
}

void OpenMatchLayer::updatePortrait(Ref*ref)
{
    portritBg->removeAllChildren();
    Sprite* userPortrit = Utils::createCircleAvatar(ZJHModel::getInstance()->portrit, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(140,140));
    userPortrit->setAnchorPoint(Vec2(0.5,0.5));
    userPortrit->setPosition(Vec2(portritBg->getContentSize().width/2, portritBg->getContentSize().height/2));
    portritBg->addChild(userPortrit);
}

void OpenMatchLayer::postFreeGift(Ref* ref)
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
            Json::Value data = loginPacket->recvVal["resultMap"]["data"];
            giftJson = data;
            bool isShow = false;
            currentGiftIndex = 0;
            
            for (int i = 0; i < giftJson.size(); i++)
            {
                if (giftJson[i]["status"].asInt() == 0)
                {
                    continue;
                }
                isShow = true;
                currentGiftIndex = i;
                if (giftJson[i]["price"].asDouble() == 0)
                {
                    upGiftBtn->loadTextures("Match/freeRecive_n.png", "Match/freeRecive_s.png", "Match/freeRecive_s.png");
                    
                }else
                {
                    upGiftBtn->loadTextures("Match/first_rechange_n.png", "Match/first_rechange_s.png", "Match/first_rechange_s.png");
                }
                break;
            }
            
            if (isShow == false)
            {
                upGiftBtn->setVisible(false);
            }else{
                upGiftBtn->setVisible(true);
            }
            
            //首充礼包暂时不开
            upGiftBtn->setVisible(false);
            
            
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    },"store/lb/info","","gift");

}
void OpenMatchLayer::moneyChange(Ref* ref)
{
    postMoney(NULL);
    
    //postFreeGift(NULL);
    
}

void OpenMatchLayer::onExit()
{
    BaseLayer::onExit();
    CCHttpAgent::getInstance()->callbacks.erase("myInfo");
    CCHttpAgent::getInstance()->callbacks.erase("isBuLuo");
    CCHttpAgent::getInstance()->callbacks.erase("gift");
    CCHttpAgent::getInstance()->callbacks.erase("duijuCode");
    CCHttpAgent::getInstance()->callbacks.erase("wealth1");
    CCHttpAgent::getInstance()->callbacks.erase("isCardOrInvite");
    CCHttpAgent::getInstance()->callbacks.erase("buy");
}

//整个首页初始化
void OpenMatchLayer::showOpenMatch()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height;
    
    Bg = ImageView::create("Match/Open_BG.png");
    Bg->ignoreContentAdaptWithSize(false);
    Bg->setContentSize(Size(visibleSize.width, visibleSize.height));
    Bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(Bg);
    
    MatchBottomView *bottom = MatchBottomView::create();
    bottom->setPosition(Vec2(0, 175));
    bottom->setAnchorPoint(Vec2(0, 0));
    Bg->addChild(bottom);
    
    bottom->setCallback([=](std::string tag){
        if (tag == "openView") {
            joinInMatch();
        }else if(tag == "moreView"){
            moreRoomView();
        }
    });
    
//    MatchBottomView::quickAddRoom(Ref *pSender, Widget::TouchEventType type){
//    }

//    Text*banTitle = Text::create("京新广文［2016］686号", "", 28);
//    banTitle->setAnchorPoint(Vec2(1, 0));
//    banTitle->setTextHorizontalAlignment(TextHAlignment::RIGHT);
//    banTitle->setPosition(Vec2(visibleSize.width - 20, 175+40));
//    banTitle->setTextColor(Color4B(0x72, 0x72, 0x72, 255));
//    Bg->addChild(banTitle);
//    
//    Text*wangTitle = Text::create("粤网文［2014］0156-006号", "", 28);
//    wangTitle->setAnchorPoint(Vec2(1, 0));
//    wangTitle->setTextHorizontalAlignment(TextHAlignment::RIGHT);
//    wangTitle->setPosition(Vec2(visibleSize.width - 20, 175));
//    wangTitle->setTextColor(Color4B(0x72, 0x72, 0x72, 255));
//    Bg->addChild(wangTitle);
//    
//    //警示文字提示
//    Text*warningTips = Text::create("本游戏仅供休闲娱乐 禁止非法赌博", "", 28);
//    warningTips->setAnchorPoint(Vec2(0, 0));
//    warningTips->setPosition(Vec2(20, 175));
//    warningTips->setTextColor(Color4B(0x72, 0x72, 0x72, 255));
//    Bg->addChild(warningTips, 100);
//    
//    ImageView *spliteLine = ImageView::create("Match/splite_line.png");
//    spliteLine->setAnchorPoint(Vec2(0.5,0));
//    spliteLine->setPosition(Vec2(visibleSize.width/2, 520));
//    Bg->addChild(spliteLine,300);

    //加入对战btn
//    Button* join = Button::create("Match/btn_against_enter.png","Match/btn_against_enter_press.png", "Match/btn_against_enter.png");
//    join->addTouchEventListener(CC_CALLBACK_2(OpenMatchLayer::joinInMatch, this));
//    join->setAnchorPoint(Vec2(0.5, 0));
//    join->setPosition(Vec2(visibleSize.width/2, 360+30));
//    Bg->addChild(join, 100);
    
    //ImageView *guoJia = ImageView::create("Match/guojia_tiyuju_yellow.png");
    //guoJia->setAnchorPoint(Vec2(0.5,1));
    //guoJia->setPosition(Vec2(visibleSize.width/2, join->getPositionY() - 30));
    //Bg->addChild(guoJia);
    
//    std::string bottomImg = UserDefault::getInstance()->getStringForKey("bottomImg","");
//    if (bottomImg != ""){
//        MatchEnterIconByUrl *guoJia = MatchEnterIconByUrl::create();
//        guoJia->load(bottomImg, "");
//        guoJia->setAnchorPoint(Vec2(0.5,1));
//        guoJia->setPosition(Vec2(visibleSize.width/2, join->getPositionY() - 30));
//        Bg->addChild(guoJia);
//    }
    
    //新手提醒
//    int newGuidTag = UserDefault::getInstance()->getIntegerForKey("newGuidTag");
//    if(!newGuidTag){
//        ImageView *newGuid = ImageView::create("Match/new_guid.png");
//        newGuid->setTouchEnabled(true);
//        newGuid->setScale(Utils::getMaxScale());
//        newGuid->setAnchorPoint(Vec2(0.5,0.5));
//        newGuid->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
//        newGuid->addClickEventListener([=](Ref*ref){
//            newGuid->removeFromParent();
//            UserDefault::getInstance()->setIntegerForKey("newGuidTag", 1);
//        });
//        addChild(newGuid,500);
//    }
    //玩家信息
    showInfoView();
//    showKeyBordView();
    haveGiftView();
   
}

void OpenMatchLayer::buLuoEnter()
{
    IsShowBuLuo = true;
    
    buLuoBg = ImageView::create("Match/gift_bg.png");
    buLuoBg->setTouchEnabled(true);
    buLuoBg->addClickEventListener(CC_CALLBACK_1(OpenMatchLayer::buLuoEnterBtnOn, this));
    buLuoBg->setAnchorPoint(Vec2(1, 0));
    buLuoBg->setPosition(Vec2(Bg->getContentSize().width, 180*2));
    Bg->addChild(buLuoBg);
    
    ImageView*arrow = ImageView::create("Match/right_arrow.png");
    arrow->setName("arrow");
    arrow->setPosition(Vec2(30, buLuoBg->getContentSize().height/2));
    buLuoBg->addChild(arrow);
    

    if (ZJHModel::getInstance()->isTrait)
    {
        traitBtn = Button::create("Match/buluo_enter_have.png", "Match/buluo_enter_have.png", "Match/buluo_enter_have.png");
    }else
    {
        traitBtn = Button::create("Match/buluo_enter_no.png", "Match/buluo_enter_no.png", "Match/buluo_enter_no.png");
    }
    
    traitBtn->setAnchorPoint(Vec2(0.5, 0.5));
    traitBtn->setPosition(Vec2(buLuoBg->getContentSize().width/2+20, buLuoBg->getContentSize().height/2));
    traitBtn->addClickEventListener([=](Ref*ref){
        ZJHModel::getInstance()->gotoView(VIEW_MY_BULUO);
    });
    buLuoBg->addChild(traitBtn);
    buLuoBg->setVisible(false);
    
}
void OpenMatchLayer::haveGiftView()
{
    IsGiftShow = true;
    
    giftBg = ImageView::create("Match/gift_bg.png");
    giftBg->setTouchEnabled(true);
    giftBg->addClickEventListener(CC_CALLBACK_1(OpenMatchLayer::giftBtnOn, this));
    giftBg->setAnchorPoint(Vec2(1, 0));
    giftBg->setPosition(Vec2(Bg->getContentSize().width, 180));
    Bg->addChild(giftBg);
    giftBg->setVisible(false);
    
    //首充全部屏蔽
    giftBg->setVisible(false);
    giftBg->setTouchEnabled(false);
    
    
    ImageView*arrow = ImageView::create("Match/right_arrow.png");
    arrow->setName("arrow");
    arrow->setPosition(Vec2(30, giftBg->getContentSize().height/2));
    giftBg->addChild(arrow);
    
    Button*giftBtn = Button::create("Match/gift_Btn.png", "Match/gift_Btn.png", "Match/gift_Btn.png");
    giftBtn->setAnchorPoint(Vec2(0.5, 0.5));
    giftBtn->setPosition(Vec2(giftBg->getContentSize().width/2+20, giftBg->getContentSize().height/2));
    giftBtn->addClickEventListener([=](Ref*ref){
        ZJHModel::getInstance()->gotoView(VIEW_INVITE);
    });
    giftBg->addChild(giftBtn);
    
    
}

void OpenMatchLayer::buLuoEnterBtnOn(Ref*ref)
{
    IsShowBuLuo = !IsShowBuLuo;
    ImageView*arrow = (ImageView*)buLuoBg->getChildByName("arrow");
    if (IsShowBuLuo)
    {
        MoveTo *show = MoveTo::create(0.2, Vec2(Bg->getContentSize().width, 180*2));
        buLuoBg->runAction(show);
        arrow->loadTexture("Match/right_arrow.png");
    }else
    {
        MoveTo *hide = MoveTo::create(0.2, Vec2(Bg->getContentSize().width+110, 180*2));
        buLuoBg->runAction(hide);
        arrow->loadTexture("Match/left_arrow.png");
        
    }

}
void OpenMatchLayer::giftBtnOn(Ref*ref)
{
    IsGiftShow = !IsGiftShow;
    ImageView*arrow = (ImageView*)giftBg->getChildByName("arrow");
    if (IsGiftShow)
    {
        MoveTo *show = MoveTo::create(0.2, Vec2(Bg->getContentSize().width, 180));
        giftBg->runAction(show);
        arrow->loadTexture("Match/right_arrow.png");
    }else
    {
        MoveTo *hide = MoveTo::create(0.2, Vec2(Bg->getContentSize().width+110, 180));
        giftBg->runAction(hide);
        arrow->loadTexture("Match/left_arrow.png");

    }
    
}

//初始化键盘
void OpenMatchLayer::showKeyBordView()
{
    //蒙层
    maskBg = Layout::create();
    maskBg->setVisible(false);
    maskBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    maskBg->setBackGroundColor(Color3B(0, 0, 0));
    maskBg->setOpacity(0);
    maskBg->setContentSize(Bg->getContentSize());
    maskBg->setPosition(Vec2(0, 0));
    maskBg->setTouchEnabled(true);
    maskBg->addClickEventListener([=](Ref*ref){
//        codeStr = "";
//        codeText->setString("");
        ((Layout*)ref)->setVisible(false);
    });
    this->getParent()->addChild(maskBg, 300);

//    auto listenerkeyPad = EventListenerKeyboard::create();
//    listenerkeyPad->onKeyReleased = [=](EventKeyboard::KeyCode keycode,Event* event)
//    {
//        if (maskBg->isVisible() == false)
//        {
//            return;
//        }
//        if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
//        {
//            codeStr = "";
//            codeText->setString("");
//            maskBg->setVisible(false);
//        }
//        event->stopPropagation();//
//    };
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerkeyPad, maskBg);
//    Director::getInstance()->getEventDispatcher()->setPriority(listenerkeyPad, 1000);
    
    ImageView *editBg = ImageView::create("Match/key_input_bg.png");
    editBg->setTouchEnabled(true);
    editBg->setAnchorPoint(Vec2(0.5, 0));
    editBg->setPosition(Vec2(maskBg->getContentSize().width/2, 673));
    editBg->addTouchEventListener(CC_CALLBACK_2(OpenMatchLayer::showKeyBord, this));
    maskBg->addChild(editBg);
    
    float itemWidth = 137;
    
    //验证码输入框
    
    codeText6 = LabelAtlas::create("", "Match/key_nums.png", 40, 59, '0');
    codeText6->setAnchorPoint(Vec2(0.5, 0));
    codeText6->setPosition( Vec2(maskBg->getContentSize().width/2 + 6 + itemWidth / 2 + (12 + itemWidth) * 2 - 3, 725));
    codeText6->setString(":");
    maskBg->addChild(codeText6);
    
    codeText5 = LabelAtlas::create("", "Match/key_nums.png", 40, 59, '0');
    codeText5->setAnchorPoint(Vec2(0.5, 0));
    codeText5->setPosition( Vec2(maskBg->getContentSize().width/2 + 6 + itemWidth / 2 + 12 + itemWidth, 725));
    codeText5->setString(":");
    maskBg->addChild(codeText5);

    
    codeText4 = LabelAtlas::create("", "Match/key_nums.png", 40, 59, '0');
    codeText4->setAnchorPoint(Vec2(0.5, 0));
    codeText4->setPosition( Vec2(maskBg->getContentSize().width/2 + 6 + itemWidth / 2, 725));
    codeText4->setString(":");
    maskBg->addChild(codeText4);

    
    codeText3 = LabelAtlas::create("", "Match/key_nums.png", 40, 59, '0');
    codeText3->setAnchorPoint(Vec2(0.5, 0));
    codeText3->setPosition( Vec2(maskBg->getContentSize().width/2 - 6 - itemWidth / 2, 725));
    codeText3->setString(":");
    maskBg->addChild(codeText3);
    
    codeText2 = LabelAtlas::create("", "Match/key_nums.png", 40, 59, '0');
    codeText2->setAnchorPoint(Vec2(0.5, 0));
    codeText2->setPosition( Vec2(maskBg->getContentSize().width/2 - 6 - itemWidth - 12 - itemWidth / 2 , 725));
    codeText2->setString(":");
    maskBg->addChild(codeText2);

    codeText1 = LabelAtlas::create("", "Match/key_nums.png", 40, 59, '0');
    codeText1->setAnchorPoint(Vec2(0.5, 0));
    codeText1->setPosition( Vec2(maskBg->getContentSize().width/2 - 6 - (itemWidth + 12) * 2 - itemWidth / 2 + 3, 725));
    codeText1->setString(":");
    maskBg->addChild(codeText1);
    
//    ImageView *keyCodeTips = ImageView::create("Match/key_code_tips.png");
//    keyCodeTips->setAnchorPoint(Vec2(0.5,0));
//    keyCodeTips->setPosition(Vec2(maskBg->getContentSize().width/2,876));
//    maskBg->addChild(keyCodeTips,300);
    
    ImageView*keyBg = ImageView::create("Match/key_Bg.png");
    keyBg->setTouchEnabled(true);
    keyBg->setScale9Enabled(true);
    keyBg->setSwallowTouches(true);
    keyBg->setAnchorPoint(Vec2(0,0));
    keyBg->setPosition(Vec2(0, 0));
    maskBg->addChild(keyBg);
    
    char buf1[128];
    char buf2[128];
    
    //计算键盘数字的坐标
    float intervalHorizen = 0;
    float intervalVertical = 0;
    for (int i = 1; i <= 12; i++)
    {
        Button*btn;
        sprintf(buf1, "Match/key_%d_n.png", i);
        sprintf(buf2, "Match/key_%d_s.png", i);
        btn = Button::create(buf1, buf2);
        btn->setAnchorPoint(Vec2(0,1));
        if(intervalHorizen == 0 || intervalVertical == 0){
            intervalHorizen = (Bg->getContentSize().width - btn->getContentSize().width * 3) / 4;
            intervalVertical = (keyBg->getContentSize().height - btn->getContentSize().height * 4) / 5;
        }
        if(i == 11)
        {
            sprintf(buf1, "%d", 0);
        }else
        {
            sprintf(buf1, "%d", i);
        }
        btn->setName(buf1);
//        float xx = intervalHorizen * (i % 3 == 0 ? 3 : i % 3)+btnWidth * ((i-1) % 3);
        float xx = (intervalHorizen+btn->getContentSize().width)*((i-1)%3+1)-btn->getContentSize().width;
        float yy = keyBg->getContentSize().height - intervalVertical * (((i - 1) / 3)+1) - btn->getContentSize().height * ((i - 1) / 3);
        btn->setPosition(Vec2(xx,yy));
        btn->addClickEventListener(CC_CALLBACK_1(OpenMatchLayer::keyBtnOn, this));
        keyBg->addChild(btn);
        
    }
    
}

void OpenMatchLayer::setCodeText(string code){
    string c1 = ":";
    string c2 = ":";
    string c3 = ":";
    string c4 = ":";
    string c5 = ":";
    string c6 = ":";
    for(int i = 0; i < code.size() ; i++){
        switch (i) {
            case 0:
                c1 = code[i];
                break;
            case 1:
                c2 = code[i];
                break;
            case 2:
                c3 = code[i];
                break;
            case 3:
                c4 = code[i];
                break;
            case 4:
                c5 = code[i];
                break;
            case 5:
                c6 = code[i];
                break;
            default:
                break;
        }
    }
    codeText1->setString(c1);
    codeText2->setString(c2);
    codeText3->setString(c3);
    codeText4->setString(c4);
    codeText5->setString(c5);
    codeText6->setString(c6);
}

void OpenMatchLayer::keyBtnOn(Ref*ref)
{
    Sound::getInstance()->playEffect("Sound/click.mp3");
    Button *btn = (Button*)ref;
    string name = btn->getName();
    
    if (codeStr.size() >= 6)
    {
        if(name == "10")
        {
            codeStr = "";
            setCodeText("");
            maskBg->setVisible(false);
            return;
        }

        PlatformHelper::showToast("房间号为6位有效数字！");
        
        if (name == "12")
        {
            if(codeStr.size() <= 0)
            {
                return;
            }
            string tempStr;
            for (int i = 0; i < codeStr.size() - 1; i++)
            {
                tempStr += codeStr.at(i);
            }
            codeStr = tempStr;
            setCodeText(codeStr);
        }
        return;
    }
    
    if (name != "12" && name != "10")
    {
        codeStr += name;
    }else if(name == "12")
    {
        if(codeStr.size() <= 0)
        {
            return;
        }
        string tempStr;
        for (int i = 0; i < codeStr.size() - 1; i++)
        {
            tempStr += codeStr.at(i);
        }
        codeStr = tempStr;
    }else if(name == "10")
    {
        codeStr = "";
        setCodeText("");
        maskBg->setVisible(false);
    }
    setCodeText(codeStr);
    
//    if(codeStr.size() == 6)
//    {
//        //加入对局的请求
//        enterRoomRequest();
//    }
    
    if(codeStr.size() == 4)
    {
        //加入对局的请求
//        enterRoomRequest();
        ZJHModel::getInstance()->curGameID = atoi(codeStr.c_str());
        ZJHModel::getInstance()->toGameHall();
    }
}

void OpenMatchLayer::enterRoomRequestWithCode(string code){
    Json::Value json;
    json["code"] = code;
    json["userJoin"] = 0;
    
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
//            PlatformHelper::showToast("进入对战房间");
            log("对局邀请码后的：＝＝＝ [%s]",data.toStyledString().c_str());
            ZJHModel::getInstance()->gameHelpUrl = CCHttpAgent::getInstance()->url+data["gameRoom"]["gameHelpUrl"].asString();
            ZJHModel::getInstance()->roomData = data;
            Director::getInstance()->replaceScene(LoadTable::createScene());
            
            codeStr = "";
            setCodeText("");
            maskBg->setVisible(false);
        }else if(loginPacket->recvVal["status"].asInt() == 999)
        {
            //tomcat没有连上
            string content = loginPacket->recvVal["message"].asString();
            ServerErrorTips* tips = ServerErrorTips::create();
            tips->setContent(content);
            addChild(tips,1000);
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    },"duiju/view",json.toStyledString(),"duijuCode");
}

void OpenMatchLayer::enterRoomRequest(){
    enterRoomRequestWithCode(codeStr);
}


void OpenMatchLayer::postMoney(Ref* ref)
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
            Json::Value data = loginPacket->recvVal["resultMap"]["play"];
            int gold = data["goldenNum"].asInt();
            int wan = data["wanbei"].asInt();
            int card = loginPacket->recvVal["resultMap"]["roomCard"].asInt();
            
            printf("card == %d\n", card);
            
            __String *bufStr = NULL;
            
            if (cardText != NULL)
            {
                cardText->setString(ZJHModel::getInstance()->moneyShowDeal(card));
                goldText->setString(ZJHModel::getInstance()->moneyShowDeal(gold));
                bufStr = __String::createWithFormat("%d", wan);
                wanText->setString(bufStr->getCString());
            }
        }else
        {
            if (!loginPacket->recvVal["message"].isNull() && loginPacket->recvVal["message"].isString())
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
        }
        
    },"golden/userWealth","","wealth1");
    
}

//刷新当前显示邀请还是领房卡
void OpenMatchLayer::refreshCardsInvite(Ref*ref){
    
    if(ZJHModel::getInstance()->isOpenIPV6())
    {
        return;
    }
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        
        CCHttpPacket* packet = CCHttpAgent::getInstance()->packets[tag];
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (packet->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        if (packet->resultIsOK())
        {
            Json::Value data = packet->recvVal["resultMap"];
            infoBg->removeChildByName("topCardsTips");
            cocostudio::ArmatureDataManager::getInstance()->removeArmatureFileInfo("Match/card_anim/effects_ptyoujiang.ExportJson");
            // 领房卡
            auto tipsLayoutBg = Layout::create();
            tipsLayoutBg->setTouchEnabled(true);
            tipsLayoutBg->addClickEventListener([=](Ref*ref){
                InviteLayer* invite = InviteLayer::create();
                Director::getInstance()->getRunningScene()->addChild(invite);
                if (data["isDraw"].asInt() == 0)
                {
                    //没有被邀请
                    invite->tabClickFun(invite->getReceiveBtn());
                    
                }
                
            });
            tipsLayoutBg->setContentSize(Size(122,122));
            tipsLayoutBg->setPosition(Vec2(infoBg->getContentSize().width  - 40,infoBg->getContentSize().height  - 17));
            tipsLayoutBg->setAnchorPoint(Vec2(1,1));
            tipsLayoutBg->setName("topCardsTips");
            
            cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("Match/card_anim/effects_ptyoujiang.ExportJson");
            auto cardArmature = cocostudio::Armature::create("effects_ptyoujiang");
            cardArmature->setName("cardAnima");
            cardArmature->setPosition(Vec2(tipsLayoutBg->getContentSize().width,tipsLayoutBg->getContentSize().height));
            cardArmature->setAnchorPoint(Vec2(1,1));
            
            tipsLayoutBg->addChild(cardArmature);
            //            infoBg->addChild(cardArmature,5);
            infoBg->addChild(tipsLayoutBg,6);
            cardArmature->getAnimation()->playByIndex(0,-1,2);
        }
        CCHttpAgent::getInstance()->packets["isCardOrInvite"] = NULL;
        delete packet;
    },"invite/isDraw", "" ,"isCardOrInvite");
}

void OpenMatchLayer::showInfoView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height;
    
    infoBg = ImageView::create("Match/match_user_bg2.png");
    infoBg->setContentSize(Size(visibleSize.width, 194));
    infoBg->ignoreContentAdaptWithSize(false);
    infoBg->setAnchorPoint(Vec2(0, 1));
    infoBg->setPosition(Vec2(0, H));
    Bg->addChild(infoBg, 50);
    
    upGiftBtn = Button::create();
    upGiftBtn->setAnchorPoint(Vec2(1, 0.5));
    upGiftBtn->setPosition(Vec2(visibleSize.width - 30, 100));
    upGiftBtn->addClickEventListener([=](Ref*ref){
        
        showFreeReciveTip();
        
    });
    infoBg->addChild(upGiftBtn);
    upGiftBtn->setVisible(false);
    
    ImageView*redIcon = ImageView::create("Match/reddot.png");
    redIcon->setPosition(Vec2(177, 157));
    upGiftBtn->addChild(redIcon);
    
    portritBg = ImageView::create("Match/bottom_photo.png");
    portritBg->ignoreContentAdaptWithSize(false);
    portritBg->setContentSize(Size(153,153));
    portritBg->setAnchorPoint(Vec2(0, 1));
    portritBg->setPosition(Vec2(18, infoBg->getContentSize().height - 10));
    infoBg->addChild(portritBg,5);
    
    //头像
    Sprite*userPortrit = Sprite::create();
    userPortrit->setContentSize(Size(140, 140));
    userPortrit->setAnchorPoint(Vec2(0.5,0.5));
    userPortrit->setPosition(Vec2(portritBg->getContentSize().width/2, portritBg->getContentSize().height/2));
    portritBg->addChild(userPortrit);
    
    //名字背景
//    ImageView* nameBg = ImageView::create("Match/botton_name.png");
//    nameBg->setPosition(Vec2(118, infoBg->getContentSize().height - 35));
//    nameBg->setAnchorPoint(Vec2(0,1));
//    infoBg->addChild(nameBg,4);
    
    //name
    nameUser = Text::create("--", "", 44);
    nameUser->setTextHorizontalAlignment(TextHAlignment::LEFT);
    nameUser->setAnchorPoint(Vec2(0, 0));
    nameUser->setPosition(Vec2(200, 125));
    nameUser->setTextColor(Color4B(0xfd, 0xfd, 0xfd, 255));
    infoBg->addChild(nameUser,5);
    
    //ID
    userID = Text::create("--", "", 44);
    userID->setTextHorizontalAlignment(TextHAlignment::LEFT);
    userID->setAnchorPoint(Vec2(0, 0));
    userID->setPosition(Vec2(visibleSize.width/2 - 50, 125));
    userID->setTextColor(Color4B(0xfd, 0xfd, 0xfd, 255));
    infoBg->addChild(userID, 5);
    
    
    for (int i = 0; i < 3; i++)
    {
        ImageView *icon;
        Text *moneyText;
        switch (i)
        {
            case 0:
                icon = ImageView::create("Match/match_icon_money.png");
                moneyText = Text::create("--", "", 40);
                moneyText->setPosition(Vec2(283, 80));
                goldText = moneyText;
                icon->setVisible(false);
                moneyText->setVisible(false);
                break;
            case 1:
                icon = ImageView::create("Match/match_icon_wanbei.png");
                moneyText = Text::create("--", "", 40);
                moneyText->setTextHorizontalAlignment(TextHAlignment::LEFT);
                moneyText->setAnchorPoint(Vec2(0, 0));
                moneyText->setPosition(Vec2(273, 60));
                wanText = moneyText;
                break;
            case 2:
                icon = ImageView::create("Match/match_icon_roomcard.png");
                moneyText = Text::create("--", "", 40);
                moneyText->setTextHorizontalAlignment(TextHAlignment::LEFT);
                moneyText->setAnchorPoint(Vec2(0, 0));
                moneyText->setPosition(Vec2(visibleSize.width/2 + 45, 60));
                cardText = moneyText;
                break;
        }
        icon->setAnchorPoint(Vec2(0, 0));
        if (i == 1)
        {
            //玩呗
            icon->setPosition(Vec2(200, 46));
        }else if(i == 2)
        {
            //房卡
            icon->setPosition(Vec2(visibleSize.width/2 - 50, 46));
        }
        infoBg->addChild(icon);
        
        moneyText->setTextColor(Color4B(0xfe, 0xe7, 0x34, 255));
        infoBg->addChild(moneyText);
        
    }
}

void OpenMatchLayer::showFreeReciveTip()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    freeBg = Layout::create();
    freeBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    freeBg->setBackGroundColor(Color3B(0, 0, 0));
    freeBg->setOpacity(150);
    freeBg->setContentSize(Bg->getContentSize());
    freeBg->setPosition(Vec2(0, 0));
    freeBg->setTouchEnabled(true);
    freeBg->addClickEventListener([=](Ref*ref){
        
        freeBg->removeFromParent();
        
    });
    this->addChild(freeBg, 300);
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(880, 820));
    bg->setPosition(Vec2(maskBg->getContentSize().width/2, maskBg->getContentSize().height/2));
    freeBg->addChild(bg);
    
    ImageView * icon = ImageView::create();
    icon->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2));
    bg->addChild(icon);
    
    ZJHModel::getInstance()->loadingImageResource(icon, giftJson[currentGiftIndex]["imageUrl"].asString());
    
    Button *cacelBut = Button::create("Mine/cancel_icon_yellow.png", "Mine/cancel_icon_yellow.png");
    cacelBut->setPosition(Vec2(bg->getContentSize().width , bg->getContentSize().height));
    bg->addChild(cacelBut);
    cacelBut->addClickEventListener([=](Ref*ref){
        freeBg->removeFromParent();
    });
    
    Text *title = Text::create(giftJson[currentGiftIndex]["name"].asString(), "", 40);
    title->setTextColor(Color4B(0xf0, 0xcb, 0x5b, 255));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 60));
    bg->addChild(title);
    
    
    Button *sureBut = Button::create("Mine/btn_yellow_small.png", "Mine/btn_yellow_small_check.png");
    sureBut->setScale9Enabled(true);
    sureBut->setAnchorPoint(Vec2(0, 0));
    sureBut->setContentSize(Size(bg->getContentSize().width - 60, 120));
    sureBut->setPosition(Vec2(30, 20));
    bg->addChild(sureBut);
    sureBut->addClickEventListener(CC_CALLBACK_1(OpenMatchLayer::buyGiftBtnOn, this));
    
    
    Text *titleSure = Text::create("", "", 36);
    titleSure->setTextColor(Color4B(0x60,0x41,0x10, 255));
    titleSure->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleSure->setPosition(Vec2(sureBut->getContentSize().width/2, sureBut->getContentSize().height/2));
    sureBut->addChild(titleSure);
    
    if (giftJson[currentGiftIndex]["price"].asDouble() == 0)
    {
        titleSure->setString("免费领取");
    }else
    {
        titleSure->setString("立即购买");
    }
    
}

void OpenMatchLayer::buyGiftBtnOn(Ref*ref)
{
    if (giftJson[currentGiftIndex]["price"].asDouble() <= 0)
    {
        Json::Value jsonT;
        jsonT["id"] = giftJson[currentGiftIndex]["id"];
        jsonT["num"] = 1;
        jsonT["type"] = 0;     //0:支付类型公司 1:
        
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
                freeBg->removeFromParent();
                PlatformHelper::showToast("领取成功");
                __NotificationCenter::getInstance()->postNotification("money_change");
                
            }else
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
        },"store/buy/more", jsonT.toStyledString(),"buy");
        
    }else
    {   freeBg->removeFromParent();
        TipShopLayer *tip = TipShopLayer::create();
        tip->buyWanPayType(giftJson[currentGiftIndex], 0, 1);
        Director::getInstance()->getRunningScene()->addChild(tip, 100);
    }
}

void OpenMatchLayer::joinInMatch()
{
    //如果验证码是6位 则点击直接进入房间 如果验证码不足6位则显示键盘
    ZJHModel::getInstance()->checkGamePlayingError([=](std::string str){
        if(str == "normal"){
            maskBg->setVisible(true);
        }else{
            enterRoomRequestWithCode(str);
        }
    });
     //测试代码
//    ZJHModel::getInstance()->toMatchHall();
}

void OpenMatchLayer::moreRoomView(){
    Size winSize = Director::getInstance()->getWinSize();
    RoomWithoutListView *moreView = RoomWithoutListView::create();
    //moreView->setPosition(Vec2(0, 175));
    moreView->setPosition(Vec2(0, 160 - winSize.height));
    moreView->setAnchorPoint(Vec2(0, 0));
    moreView->runAction(MoveTo::create(0.1, Vec2(0, 175)));
    this->addChild(moreView);
}

void OpenMatchLayer::showKeyBord(Ref *pSender, Widget::TouchEventType type){
    if (type == Widget::TouchEventType::ENDED)
    {
        maskBg->setVisible(true);
    }
}



