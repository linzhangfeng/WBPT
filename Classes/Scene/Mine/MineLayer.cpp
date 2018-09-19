//
//  FriendLayer.cpp
//  wanMain
//
//  Created by apple on 16/6/23.
//
//

#include "MineLayer.h"
#include "MaskSelectLayer.hpp"
#include "../Hall/DailyReward.h"
#include "TaskLayer.hpp"
#include "ShareAppInviteLayer.hpp"
#include "VIPBuyTipLayer.hpp"

MineLayer::MineLayer()
{
    lastBtn = NULL;
    listView_s = NULL;
    lastClick = NULL;
    listView_do = NULL;
    userPortrit = NULL;
    portritBg = NULL;
    infoBg = NULL;
    name = NULL;
    listLayer1 = NULL;
}

bool MineLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("我的");
    setHead(130);
    setMore_txt("更多", [=](){
        
        ZJHModel::getInstance()->gotoView(VIEW_MINE_MORE);
    });
    
    //创建按钮map
    createButMap();
    showInfoView();
    updateView(NULL);
    
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
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
            infoList = data;
            
            char buf[256];
            Utils::parseName(12, buf, infoList ["nickName"].asString().c_str());
            name->setString(buf);
            __String *tempStr = __String::createWithFormat("ID:%d", infoList["id"].asInt());
            IDText->setString(tempStr->getCString());
            
            std::string userImage = infoList["thumbImage"].asString();
            ZJHModel::getInstance()->portrit = userImage;
            ZJHModel::getInstance()->name = infoList ["nickName"].asString();
            
            Vec2 position =  userPortrit->getPosition();
            userPortrit->removeFromParent();
            userPortrit = Utils::createCircleAvatar(userImage, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(200, 200));
            userPortrit->setPosition(position);
            infoBg->addChild(userPortrit);
            
            updateView(NULL);
            
        }
    },"account/myInfo","","myInfo");
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MineLayer::updateView), "account_change", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MineLayer::updatePortrait), "userPotrait_change", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MineLayer::updateDailyReceive), "daily_receive", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MineLayer::updateNick), "nick_change", NULL);
    
    return true;
}

void MineLayer::showInfoView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();
    
    
    infoBg = ImageView::create("Mine/mine_user_bg.png");
    infoBg->setContentSize(Size(visibleSize.width, 450));
    infoBg->ignoreContentAdaptWithSize(false);
    infoBg->setAnchorPoint(Vec2(0, 1));
    infoBg->setPosition(Vec2(0, H));
    this->addChild(infoBg);
    
    float bgH = infoBg->getContentSize().height;
    
    ImageView *arrow = ImageView::create("Mine/me_comeback.png");
    arrow->setPosition(Vec2(visibleSize.width - 70, bgH/2));
    infoBg->addChild(arrow);
    
    portritBg = Layout::create();
    portritBg->setTouchEnabled(true);
    portritBg->ignoreAnchorPointForPosition(false);
    portritBg->setAnchorPoint(Vec2(0, 1));
    portritBg->setPosition(Vec2(0, bgH));
    portritBg->setContentSize(Size(visibleSize.width, 300));
    infoBg->addChild(portritBg);
    portritBg->addClickEventListener([=](Ref*ref){
        
        ZJHModel::getInstance()->gotoView(VIEW_MINE_USER_INFO);
        
    });
    
    //头像
    userPortrit = Sprite::create("Mine/user4_unlogin.png");
    userPortrit->setContentSize(Size(200, 200));
    userPortrit->setPosition(Vec2(visibleSize.width/2, bgH - 160));
    infoBg->addChild(userPortrit);
    
    //name
    name = Text::create("--", "", 50);
    name->setAnchorPoint(Vec2(0.5, 0));
    name->setPosition(Vec2(visibleSize.width/2, 100));
    name->setTextColor(Color4B(0xe6, 0xe6, 0xe6, 255));
    infoBg->addChild(name);
    
    
    IDText = Text::create("ID:--", "", 50);
    IDText->setAnchorPoint(Vec2(0.5, 0));
    IDText->setPosition(Vec2(visibleSize.width/2, 30));
    IDText->setTextColor(Color4B(0xe6, 0xe6, 0xe6, 255));
    infoBg->addChild(IDText);
    
    //显示全部按钮视图
    registView();

}

void MineLayer::createButMap()
{
    Sprite *redTip;
    for (int i = 0; i < 12; i++)
    {
        Button *btn = NULL;
        switch (i) {
            case 0:
                btn = Button::create("Mine/me_free_reveive_n@2x.png","Mine/me_free_reveive_n@2x.png", "Mine/me_free_reveive_n@2x.png");
                btn->setName("freeRecive");
                butMap["freeRecive"] = btn;
                
                redTip = Sprite::create("Match/reddot.png");
                redTip->setVisible(false);
                redTip->setPosition(Vec2(btn->getContentSize().width, btn->getContentSize().height));
                redTip->setName("red");
                btn->addChild(redTip);
                break;
                
            case 1:
                btn = Button::create("Mine/me_vip_center_n@2x.png","Mine/me_vip_center_n@2x.png", "Mine/me_vip_center_n@2x.png");
                btn->setName("vipCenter");
                butMap["vipCenter"] = btn;
                break;
            case 2:
                btn = Button::create("Mine/me_invite_n@2x.png","Mine/me_invite_n@2x.png", "Mine/me_invite_n@2x.png");
                btn->setName("invite");
                butMap["invite"] = btn;
                break;
            case 3:
                btn = Button::create("Mine/me_dai_buy_n@2x.png","Mine/me_dai_buy_n@2x.png", "Mine/me_dai_buy_n@2x.png");
                btn->setName("delegateBuy");
                butMap["delegateBuy"] = btn;
                break;
            case 4:
                btn = Button::create("Mine/me_wan_task_n@2x.png","Mine/me_wan_task_n@2x.png", "Mine/me_wan_task_n@2x.png");
                btn->setName("task");
                butMap["task"] = btn;
                break;
            case 5:
                btn = Button::create("Mine/me_shar_app_n@2x.png","Mine/me_shar_app_n@2x.png", "Mine/me_shar_app_n@2x.png");
                btn->setName("shareLoad");
                butMap["shareLoad"] = btn;
                break;
                
            case 6:
                btn = Button::create("Mine/buLuoGuanLi_icon.png","Mine/buLuoGuanLi_icon.png", "Mine/buLuoGuanLi_icon.png");
                btn->setName("buLuoGuanLi");
                butMap["buLuoGuanLi"] = btn;
                break;
            case 7:
                btn = Button::create("Mine/real_name_auth.png","Mine/real_name_auth.png", "Mine/real_name_auth.png");
                btn->setName("realNameAuth");
                butMap["realNameAuth"] = btn;
                break;
            case 8:
                btn = Button::create("Mine/me_daili.png","Mine/me_daili.png", "Mine/me_daili.png");
                btn->setName("myDaiLi");
                butMap["myDaiLi"] = btn;
                break;
                
            case 9:
                btn = Button::create("Mine/kefu_center01.png","Mine/kefu_center02.png", "Mine/kefu_center02.png");
                btn->setName("kefucenter");
                butMap["kefucenter"] = btn;
                break;
                
            case 10:
                btn = Button::create("Mine/set_Btn_1.png","Mine/set_Btn_2.png", "Mine/set_Btn_1.png");
                btn->setName("set");
                butMap["set"] = btn;
                break;
                
            case 11:
                btn = Button::create("Mine/zhanji_Btn_1.png","Mine/zhanji_Btn_2.png", "Mine/zhanji_Btn_1.png");
                btn->setName("zhanJi");
                butMap["zhanJi"] = btn;
                break;
                
        }
        if(btn)btn->addTouchEventListener(CC_CALLBACK_2(MineLayer::butClickOn, this));
    }
}

//游客
void MineLayer::youKeView()
{
    createButMap();
    if(listView_do != NULL)
    {
        listView_do->removeFromParent();
    }
    
    std::vector<std::string> btnNames;
    btnNames.push_back("task");
    btnNames.push_back("shareLoad");
    btnNames.push_back("myDaiLi");
    btnNames.push_back("kefucenter");
    listView_do = this->generateListView(btnNames);
    this->addChild(listView_do);
}


//注册账户普通用户
void MineLayer::registView()
{
    createButMap();
    if(listView_do != NULL)
    {
        listView_do->removeFromParent();
    }
    
    std::vector<std::string> btnNames;
    
    //1、战绩
    btnNames.push_back("zhanJi");
    
    //2、推荐有奖
    btnNames.push_back("invite");
    
    //暂时不用
    //btnNames.push_back("myDaiLi");
    
    //3、平台任务
    btnNames.push_back("task");
    
    //4、实名认证
    btnNames.push_back("realNameAuth");
    
    //5、客服中心
    btnNames.push_back("kefucenter");
    
    //6、设置
    btnNames.push_back("set");

    
    Button *btn = butMap["freeRecive"];
    if (ZJHModel::getInstance()->loginJson["signToday"].asBool())
    {
        btn->getChildByName("red")->setVisible(false);
    }else
    {
        btn->getChildByName("red")->setVisible(true);
    }
    
    listView_do = this->generateListView(btnNames);
    this->addChild(listView_do);
}

ListView *MineLayer::generateListView(std::vector<std::string> btnNames)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float conH = visibleSize.height/2 + 220;
    
    ListView * listView = ListView::create();
    listView->setScrollBarEnabled(true);
    listView->setDirection(ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(true);
    listView->setContentSize(Size(visibleSize.width, conH));
    listView->setPosition(Vec2(0, 160));
    listView->setItemsMargin(0);
    
    Vec2 btnPos[3] = {
        Vec2(130 + 80, 174),
        Vec2(visibleSize.width / 2, 174),
        Vec2(visibleSize.width - 130 - 80, 174),
    };
    
    Layout *topItem = Layout::create();
    topItem->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    topItem->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    topItem->setContentSize(Size(visibleSize.width, 120));
    listView->addChild(topItem);
    
    Layout *item = Layout::create();
    item->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    item->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    item->setContentSize(Size(visibleSize.width, (conH-120)/4));
    listView->addChild(item);
    
    int btnNum = 0;
    int btnIndex = 0;
    Button *btn = NULL;
    for (int i = 0; i < btnNames.size(); i++) {
        std::string name = btnNames[i];
        if (name == "invite")
        {
            if (ZJHModel::getInstance()->isOpenIPV6() == true)
            {
                continue;
            }
        }else if (name == "realNameAuth")
        {
            if (ZJHModel::getInstance()->isOpenIPV6() == true)
            {
                continue;
            }
        }else if (name == "myDaiLi")
        {
            if (ZJHModel::getInstance()->isOpenIPV6() == true)
            {
                continue;
            }
        }
        btn = butMap[name];
        btn->setPosition(btnPos[btnNum % 3]);
        btnIndex++;
        
        if (btnNum == 3) {
            item = Layout::create();
            listView->addChild(item);
            item->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
            item->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
            item->setContentSize(Size(visibleSize.width, (conH-120)/4));
            
            btnNum = 0;
        }
        
        item->addChild(btn);
        btnNum++;
    }
    
    return listView;
}

//VIP
void MineLayer::VIPView()
{
    createButMap();
    if(listView_do != NULL)
    {
        listView_do->removeFromParent();
    }
    
    std::vector<std::string> btnNames;
    btnNames.push_back("invite");
    btnNames.push_back("myDaiLi");
    btnNames.push_back("task");
    btnNames.push_back("realNameAuth");
    btnNames.push_back("kefucenter");
    
    listView_do = this->generateListView(btnNames);
    this->addChild(listView_do);
    
    Button *btn1 = butMap["freeRecive"];
    if (ZJHModel::getInstance()->loginJson["signToday"].asBool())
    {
        btn1->getChildByName("red")->setVisible(false);
    }else
    {
        btn1->getChildByName("red")->setVisible(true);
    }
}

//贵族
void MineLayer::richView()
{
    createButMap();
    if(listView_do != NULL)
    {
        listView_do->removeFromParent();
    }
    
    
    std::vector<std::string> btnNames;
    btnNames.push_back("invite");
    btnNames.push_back("myDaiLi");
    btnNames.push_back("task");
    btnNames.push_back("realNameAuth");
    btnNames.push_back("kefucenter");
    
    
    listView_do = this->generateListView(btnNames);
    this->addChild(listView_do);
    
    Button *btn1 = butMap["freeRecive"];
    if (ZJHModel::getInstance()->loginJson["signToday"].asBool())
    {
        btn1->getChildByName("red")->setVisible(false);
    }else
    {
        btn1->getChildByName("red")->setVisible(true);
    }
    
}

//族长
void MineLayer::leaderView()
{
    createButMap();
    if(listView_do != NULL)
    {
        listView_do->removeFromParent();
    }
    
    std::vector<std::string> btnNames;
    btnNames.push_back("invite");
    btnNames.push_back("myDaiLi");
    btnNames.push_back("task");
    btnNames.push_back("realNameAuth");
    btnNames.push_back("kefucenter");
    
    listView_do = this->generateListView(btnNames);
    this->addChild(listView_do);
    
    
    Button *btn1 = butMap["freeRecive"];
    if (ZJHModel::getInstance()->loginJson["signToday"].asBool())
    {
        btn1->getChildByName("red")->setVisible(false);
    }else
    {
        btn1->getChildByName("red")->setVisible(true);
    }
}

void MineLayer::updateDailyReceive(Ref*ref)
{
    if (lastClick->getName() == "regist")
    {
        registView();
    }else if(lastClick->getName() == "youKe")
    {
        youKeView();
        
    }else if (lastClick->getName() == "VIP")
    {
        VIPView();
        
    }else if (lastClick->getName() == "rich")
    {
        richView();
        
    }else if (lastClick->getName() == "leader")
    {
        leaderView();
    }
    
}

void MineLayer::updatePortrait(Ref*ref)
{
    Vec2 position = userPortrit->getPosition();
    userPortrit->removeFromParent();
    userPortrit = Utils::createCircleAvatar(ZJHModel::getInstance()->portrit, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(200, 200));
    userPortrit->setPosition(position);
    infoBg->addChild(userPortrit);
    
}

void MineLayer::updateNick(Ref*ref)
{
    char buf[128];
    Utils::parseName(12, buf, ZJHModel::getInstance()->nick.c_str());
    name->setString(buf);
}

void MineLayer::updateView(Ref* ref)
{
    CallFunc* func1 = CallFunc::create([=]
                                       {
                                           //游客的更新处理
//                                           if(ZJHModel::getInstance()->isYoukeLogin())
//                                           {
//                                               name->setString("点击登录");
//                                               // 使用 lambda 实现 onTouchBegan 事件回调函数
//                                               portritBg->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
//                                                                                {
//                                                                                    if (type == Widget::TouchEventType::ENDED)
//                                                                                    {
//                                                                                        // 获取事件所绑定的 target
//                                                                                        ZJHModel::getInstance()->gotoView(VIEW_LOAD);
//                                                                                    }
//                                                                                });
//                                           }else
//                                           {
//                                               char buf[128];
//                                               Utils::parseName(12, buf, ZJHModel::getInstance()->nick.c_str());
//                                               name->setString(buf);
//                                           }
                                           
                                           char buf[128];
                                           Utils::parseName(12, buf, ZJHModel::getInstance()->nick.c_str());
                                           name->setString(buf);
                                           
                                           __String *tempStr = __String::createWithFormat("ID:%d", ZJHModel::getInstance()->uid);
                                           IDText->setString(tempStr->getCString());
                                           
                                           Vec2 position =  userPortrit ->getPosition();
                                           userPortrit->removeFromParent();
                                           userPortrit = Utils::createCircleAvatar(ZJHModel::getInstance()->portrit, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(200, 200));
                                           userPortrit->setPosition(position);
                                           infoBg->addChild(userPortrit);
                                           
                                       });
    
    runAction(Sequence::create(DelayTime::create(1.0),func1, NULL));
}

void MineLayer::butClickOn(Ref* ref, Widget::TouchEventType type)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Button *but = (Button *)ref;
    string butName = but->getName();
    
    if (type == Widget::TouchEventType::ENDED)
    {
        if (butName == "zhanJi")
        {
            ZJHModel::getInstance()->gotoView(VIEW_MATCH_RESULT);
            
        }else if (butName == "set")
        {
            ZJHModel::getInstance()->gotoView(VIEW_MINE_SETTING);
            
        }else if (butName == "freeRecive")
        {
            DailyReward* rew = DailyReward::create();
            rew->addJsonData(ZJHModel::getInstance()->loginJson);
            rew->setPosition(Vec2(0,-visibleSize.height));
            this->addChild(rew, 100);
            rew->runAction(MoveTo::create(0.5, Vec2(0,0)));
            
        }else if (butName == "vipCenter")
        {
            Json::Value json;
            json["id"] = infoList["id"];
            json["nickName"] = infoList["nickName"];
            
            ZJHModel::getInstance()->gotoView(VIEW_VIP_CENTER, json);
        }
        else if (butName == "kefucenter")
        {
            ZJHModel::getInstance()->gotoView(VIEW_KEFU_CENTER);
        }
        else if (butName == "invite")
        {
            ZJHModel::getInstance()->gotoView(VIEW_INVITE);
        }
        else if (butName == "delegateBuy")
        {
            showDaiLiWeiXinView();
        }
        else if (butName == "task")
        {
            TaskLayer* task = TaskLayer::create();
            task->setPosition(Vec2(0,-visibleSize.height));
            this->addChild(task, 100);
            task->runAction(MoveTo::create(0.5, Vec2(0,0)));
        }
        else if (butName == "shareLoad")
        {
            Scene *currentScent =  Director::getInstance()->getRunningScene();
            
            ShareAppInviteLayer *share = ShareAppInviteLayer::create();
            share->setPosition(Vec2::ZERO);
            share->setShareTypeView(2, ZJHModel::getInstance()->gameConfigJson["list"]["recommend"]);
            currentScent->addChild(share, 200);
        }else if(butName == "buLuoGuanLi")
        {
            ZJHModel::getInstance()->gotoView(VIEW_BULUO_MANAGER);
        }else if(butName == "realNameAuth"){
            ZJHModel::getInstance()->gotoView(VIEW_REAL_NAME_AUTH);
        }else if(butName == "myDaiLi")
        {
            ZJHModel::getInstance()->gotoView(VIEW_MINE_MY_DAILI);
        }
        
    }
    
    lastBtn = but;
    
}

void MineLayer::showDaiLiWeiXinView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout *daiLi = Layout::create();
    daiLi->setTouchEnabled(true);
    daiLi->setSwallowTouches(true);
    daiLi->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    daiLi->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    daiLi->setBackGroundColorOpacity(150);
    daiLi->setContentSize(visibleSize);
    daiLi->addClickEventListener([=](Ref*ref){
        
        daiLi->removeFromParentAndCleanup(true);
        
    });
    this->addChild(daiLi, 100);
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(visibleSize.width - 250, 400));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    daiLi->addChild(bg);
    
    Text *title = Text::create("提示", "", 50);
    title->setTextColor(Color4B(0xd9, 0xd9, 0xd9, 255));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 65));
    bg->addChild(title);
    
//    Text *title2 = Text::create("代理招募：w14300846【微信】", "", 45);
    Text *title2 = Text::create("请联系当地代理商", "", 45);
    title2->setTextColor(Color4B(0xd9, 0xd9, 0xd9, 255));
    title2->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title2->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2));
    bg->addChild(title2);
    
    
    //    Button *cancel = Button::create("Public/btn_yellow.png","Public/btn_yellow.png");
    //    cancel->setPosition(Vec2(bg->getContentSize().width/2 - 50, 15));
    //    cancel->setScale9Enabled(true);
    //    cancel->setAnchorPoint(Vec2(0, 0));
    //    cancel->setContentSize(Size(200, 90));
    //    cancel->setTitleText("取消");
    //    cancel->setTitleFontSize(40);
    //    cancel->setTitleColor(Color3B(0x68, 0x41, 0x03));
    //    cancel->setTitleFontName(".SFUIDisplay-Semibold");
    //    cancel->addClickEventListener([=](Ref*ref){
    //
    //        daiLi->removeFromParentAndCleanup(true);
    //
    //    });
    //    bg->addChild(cancel);
    
//    Button *fuzhi = Button::create("Public/btn_yellow.png", "Public/btn_yellow.png");
//    fuzhi->setPosition(Vec2(bg->getContentSize().width/2, 30));
//    fuzhi->setScale9Enabled(true);
//    fuzhi->setAnchorPoint(Vec2(0.5, 0));
//    fuzhi->setContentSize(Size(350, 90));
//    fuzhi->setTitleText("复制到粘贴板");
//    fuzhi->setTitleFontSize(40);
//    fuzhi->setTitleColor(Color3B(0x68, 0x41, 0x03));
//    fuzhi->setTitleFontName("");
//    fuzhi->addClickEventListener([=](Ref*ref){
//        PlatformHelper::sdkDoForVoid_C("copyToPasteboard", "w14300846");
//        PlatformHelper::showToast("复制成功");
//        daiLi->removeFromParentAndCleanup(true);
//    });
//    bg->addChild(fuzhi);
}


void MineLayer::fastBuyVip(Ref* ref)
{
    Size winSize = Director::getInstance()->getWinSize();
    VIPBuyTipLayer *tip = VIPBuyTipLayer::create();
    tip->setPosition(Vec2(winSize.width, 0));
    this->addChild(tip, 100);
    tip->runAction(MoveTo::create(0.3, Vec2(0,0)));
}

void MineLayer::fastLoad(Ref*ref)
{
    ZJHModel::getInstance()->gotoView(VIEW_LOAD);
}
