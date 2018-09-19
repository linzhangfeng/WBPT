
//
//  SettingLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/7.
//
//

#include "SettingLayer.hpp"
#include "../Loading/Loading.h"

#include "../../Model/MsgModel.h"

#define CELL_SWITCH_TAG_1   100 //开关背景－－通知开关tag
#define CELL_SWITCH_TAG_2   101 //开关背景－－通知栏通知tag
#define CELL_SWITCH_TAG_3   102 //开关背景－－锁屏tag

#define CELL_SWITCH_ON      103 //开关on 图片
#define CELL_SWITCH_OFF     104 //开关off 图片

bool SettingLayer::init()
{
    isWXBind = false;
    SettingLayer::checkBindState();
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("设置中心");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    setName("SettingLayer");
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(SettingLayer::updateStateView), "setPwd_finish", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(SettingLayer::updateStateView), "phone_change", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(SettingLayer::updateView), "account_change", NULL);
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SettingLayer::phoneWXBindPost), "wx_authorize", NULL);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float H = visibleSize.height - getHeadH() - 25 - 210;
    
    listView = ListView::create();
    listView->setInertiaScrollEnabled(false);
    listView->setDirection(ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(visibleSize.width, H));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 210));
    listView->setItemsMargin(0);
    listView->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(listView);
    
    Layout *exitBg = Layout::create();
    exitBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    exitBg->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    exitBg->setContentSize(Size(visibleSize.width, 210));
    this->addChild(exitBg);
    
    auto line = Layout::create();
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->ignoreAnchorPointForPosition(false);
    line->setAnchorPoint(Vec2(0, 1));
    line->setBackGroundColor(Color3B(0, 0, 0));
    line->setPosition(Vec2(0, 210));
    line->setContentSize(Size(visibleSize.width, 5));
    exitBg->addChild(line);
    
    exitBtn = Button::create("Public/btn_red.png","Public/btn_red.png", "Public/btn_red.png");
    exitBtn->setScale9Enabled(true);
    exitBtn->setContentSize(Size(960, 115));
    exitBtn->setPosition(Vec2(visibleSize.width/2, 105));
    exitBg->addChild(exitBtn);
    exitBtn->setTitleText("退出账号");
    exitBtn->setTitleFontSize(40);
    exitBtn->setTitleColor(Color3B(255, 255, 255));
    exitBtn->setTitleFontName(".SFUIDisplay-Semibold");
    exitBtn->addTouchEventListener(CC_CALLBACK_2(SettingLayer::exitLoadOn, this));
    
    //判断是否有账号存储登录
   
    if(ZJHModel::getInstance()->isYoukeLogin())
    {
        unLoadView();
        
    }else
    {
        loadView();
    }
    
    return true;
}

void SettingLayer::updateStateView(Ref* json)
{
    loadView();
}

void SettingLayer::updateView(Ref* ref)
{
    CallFunc* func1 = CallFunc::create([=]
                                       {
                                           isWXBind = false;
                                           SettingLayer::checkBindState();
                                           
                                           //判断是否有账号存储登录
                                           if(ZJHModel::getInstance()->isYoukeLogin())
                                           {
                                               unLoadView();
                                               
                                           }else
                                           {
                                               loadView();
                                           }
                                       });
    
    runAction(Sequence::create(DelayTime::create(0.0),func1, NULL));
    

}

//index 1:
Layout *SettingLayer::cellItem(int index, string leftStr, string leftIcon, string rightStr)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto cell = Layout::create();
    cell->ignoreAnchorPointForPosition(false);
    cell->setAnchorPoint(Vec2(0, 1));
    cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    cell->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    
    auto line = Layout::create();
    line->setTag(100);
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->setBackGroundColor(Color3B(0x30, 0x30, 0x31));
    line->setContentSize(Size(visibleSize.width - 40, 3));
    line->setPosition(Vec2(40, 0));
    cell->addChild(line);
    
    //cell标题
    Text *title = Text::create(leftStr, ".SFUIDisplay-Semibold", 36);
    title->setAnchorPoint(Vec2(0, 0.5));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    cell->addChild(title);
    
    //icon
    ImageView *icon = ImageView::create(leftIcon);
    icon->setAnchorPoint(Vec2(0, 0.5));
    cell->addChild(icon);
    
    ImageView *arrow = ImageView::create("Public/detail_arrow.png");
    arrow->setTag(500);
    arrow->setAnchorPoint(Vec2(1, 0.5));
    cell->addChild(arrow);
    
    if (index == 1)
    {
        //纯文字的
        cell->setTouchEnabled(false);
        cell->setContentSize(Size(visibleSize.width, 90));
        title->setTextColor(Color4B(0xb3, 0xb3, 0xb3, 255));
        title->setPosition(Vec2(40, cell->getContentSize().height/2));
        title->setFontSize(36);
        arrow->setVisible(false);
    }else if(index == 2)
    {
        arrow->setVisible(true);
        cell->setContentSize(Size(visibleSize.width, 130));
        title->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
        title->setPosition(Vec2(145, cell->getContentSize().height/2));
        title->setFontSize(36);
        //right
        Text *rightTitle = Text::create(rightStr, ".SFUIDisplay-Semibold", 36);
        rightTitle->setAnchorPoint(Vec2(1, 0.5));
        rightTitle->setTextHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
        cell->addChild(rightTitle);
        rightTitle->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
        rightTitle->setPosition(Vec2(visibleSize.width - 90, cell->getContentSize().height/2));
        
        cell->setTouchEnabled(true);
        
    }else if (index == 3)
    {
        cell->setContentSize(Size(visibleSize.width, 130));
        title->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
        title->setPosition(Vec2(145, cell->getContentSize().height/2));
        title->setFontSize(36);
        
        arrow->setVisible(false);
        
        ImageView*rightOn = ImageView::create("Mine/ON.png");
        ImageView*rightOff = ImageView::create("Mine/OFF.png");
        Layout *layoutSwitch = Layout::create();
        layoutSwitch->setAnchorPoint(Vec2(1, 0));
        layoutSwitch->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        layoutSwitch->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
        layoutSwitch->setContentSize(Size(160, 120));
        layoutSwitch->setTouchEnabled(true);
        layoutSwitch->setTag(105);
        layoutSwitch->addTouchEventListener(CC_CALLBACK_2(SettingLayer::swithClickFun, this));
        layoutSwitch->ignoreAnchorPointForPosition(false);
        layoutSwitch->setPosition(Vec2(visibleSize.width - 45, 5));
        cell->addChild(layoutSwitch);
        rightOn->setTag(CELL_SWITCH_ON);
        rightOn->setPosition(Vec2(rightOn->getContentSize().width/2, 60));
        layoutSwitch->addChild(rightOn);
    
        rightOff->setTag(CELL_SWITCH_OFF);
        rightOff->setPosition(Vec2(rightOn->getPosition()));
        layoutSwitch->addChild(rightOff);
        
    }
    
    icon->setPosition(Vec2(40, cell->getContentSize().height/2));
    arrow->setPosition(Vec2(visibleSize.width - 40, cell->getContentSize().height/2));
    return cell;
}
//登录的
void SettingLayer::loadView()
{
    exitBtn->setTitleText("退出账号");
    exitBtn->addTouchEventListener(CC_CALLBACK_2(SettingLayer::exitLoadOn, this));
    listView->removeAllChildren();
    
    string phone = ZJHModel::getInstance()->phone;
    string password =  ZJHModel::getInstance()->pwd;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();

    for (int i = 0; i < 4; i++)
    {
        //cell
        auto cell = Layout::create();
        cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        cell->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
        listView->addChild(cell);
        
        Layout *item1;
        Layout *item2;
        Layout *item3;
        Layout *item4;
        Layout *line;
        ImageView *arrow;
        
        switch (i)
        {
            case 0:
            {
                cell->setContentSize(Size(visibleSize.width, 220+20));
                item1 = cellItem(1,"手机号账号");
                item1->setPosition(Vec2(0, cell->getContentSize().height));
                
                if (UserDefault::getInstance()->getIntegerForKey("thirdtype") == 3 && phone == "")
                {
                    item2 = cellItem(2,"手机号", "Mine/view_icon3_setting.png", "未设置");
                    //绑定手机号
                    item2->addClickEventListener([=](Ref*ref){
                        ZJHModel::getInstance()->weiXinBangPhone = "weiXinBangPhone";
                        ZJHModel::getInstance()->gotoView(VIEW_REGIST);
                    });

                    //arrow = (ImageView *)item2->getChildByTag(500);
                    //arrow->setVisible(false);
                    
                }else
                {
                    item2 = cellItem(2,"手机号", "Mine/view_icon3_setting.png", ZJHModel::getInstance()->hidePhoneNum(phone));
                    item2->addTouchEventListener(CC_CALLBACK_2(SettingLayer::cellUpdatePhone, this));
                }
                
                item2->setPosition(Vec2(0, cell->getContentSize().height - item1->getContentSize().height));
                //line = (Layout*)item2->getChildByTag(100);
                //line->setVisible(false);
            }
                break;
            case 1:
            {
                cell->setContentSize(Size(visibleSize.width, 220+20));
                item1 = cellItem(1,"密码安全");
                item1->setPosition(Vec2(0, cell->getContentSize().height));
                string pwd;
                
                if (ZJHModel::getInstance()->pwdOK == false)
                {
                    item2 = cellItem(2,"修改密码", "Mine/view_icon14.png", "未设置");
                    if(UserDefault::getInstance()->getIntegerForKey("thirdtype") == 3 && phone != "")
                    {
                        //1:微信帮手机的设置密码
                        item2->addClickEventListener([=](Ref*ref){
                            ZJHModel::getInstance()->NOPwdSetPwd = "NOPwdSetPwd";
                            ZJHModel::getInstance()->gotoView(VIEW_REGIST_FINISH);
                        });
                        
                    }else if(UserDefault::getInstance()->getIntegerForKey("thirdtype") == 3 && phone == "")
                    {
                        //2:微信没有绑定手机，设置密码时
                        item2->addClickEventListener([=](Ref*ref){
                            PlatformHelper::showToast("请先绑定手机号！");
                        });
                    }else
                    {
                        //3:快速登录时没有密码
                        item2->addClickEventListener([=](Ref*ref){
                            ZJHModel::getInstance()->NOPwdSetPwd = "NOPwdSetPwd";
                            ZJHModel::getInstance()->gotoView(VIEW_REGIST_FINISH);
                        });
                        
                    }

                }else
                {
                    item2 = cellItem(2,"修改密码", "Mine/view_icon14.png", "已设置");
                    item2->addTouchEventListener(CC_CALLBACK_2(SettingLayer::cellUpdatePassWord, this));
                }
                
                item2->setPosition(Vec2(0, cell->getContentSize().height - item1->getContentSize().height));
                
                //line = (Layout*)item2->getChildByTag(100);
                //line->setVisible(false);
            }
                break;
            case 2:
                cell->setContentSize(Size(visibleSize.width, 220+20));
                item1 = cellItem(1,"第三方账号");
                item1->setPosition(Vec2(0, cell->getContentSize().height));
                if(UserDefault::getInstance()->getIntegerForKey("thirdtype") == 3)
                {
                    item2 = cellItem(2,"绑定微信账号", "Mine/bind_wechat.png", "已登录");
                    arrow = (ImageView *)item2->getChildByTag(500);
                    arrow->setVisible(false);
                    
                }else
                {
                    string str = UserDefault::getInstance()->getStringForKey("wx_login_openid");
                    if (str != "" || isWXBind)
                    {
                        item2 = cellItem(2,"绑定微信账号", "Mine/bind_wechat.png", "已绑定/更新");
                    }else
                    {
                        item2 = cellItem(2,"绑定微信账号", "Mine/bind_wechat.png", "未绑定");
                    }
                    item2->addClickEventListener(CC_CALLBACK_1(SettingLayer::bindWeiXinOn, this));
                    
                }
                
                item2->setPosition(Vec2(0, cell->getContentSize().height - item1->getContentSize().height));
                
                //line = (Layout*)item2->getChildByTag(100);
                //line->setVisible(false);
                
                break;
            case 3:
            {
                cell->setContentSize(Size(visibleSize.width, 485+20));
                item1 = cellItem(1,"开启通知及时知晓房间动态");
                item1->setPosition(Vec2(0, cell->getContentSize().height));
                
                item2 = cellItem(3,"动态通知", "Mine/mes.png", "");
                item2->setPosition(Vec2(0, cell->getContentSize().height - item1->getContentSize().height));
                
                Layout *swithBg2 = (Layout*)item2->getChildByTag(105);
                swithBg2->setTag(CELL_SWITCH_TAG_1);
                noticeBg = swithBg2;
                
                ImageView *on2 = (ImageView*)swithBg2->getChildByTag(CELL_SWITCH_ON);
                ImageView *off2 = (ImageView*)swithBg2->getChildByTag(CELL_SWITCH_OFF);
                
                on2->setVisible(!UserDefault::getInstance()->getBoolForKey(SETTINF_NOTICE_OFF));
                off2->setVisible(UserDefault::getInstance()->getBoolForKey(SETTINF_NOTICE_OFF));
            
                item3 = cellItem(3,"通知栏通知", "Mine/mes2.png", "");
                item3->setPosition(Vec2(0, item2->getPositionY() - item2->getContentSize().height));
                
                Layout *swithBg3 = (Layout*)item3->getChildByTag(105);
                barBg = swithBg3;
                
                ImageView *on3 = (ImageView*)swithBg3->getChildByTag(CELL_SWITCH_ON);
                ImageView *off3 = (ImageView*)swithBg3->getChildByTag(CELL_SWITCH_OFF);
                
                on3->setVisible(!UserDefault::getInstance()->getBoolForKey(SETTINF_NOTICE_BAR_OFF));
                off3->setVisible(UserDefault::getInstance()->getBoolForKey(SETTINF_NOTICE_BAR_OFF));
                
                item4 = cellItem(3,"锁屏通知", "Mine/view_icon3_setting.png", "");
                item4->setPosition(Vec2(0, item2->getPositionY() - item2->getContentSize().height*2));
                
                Layout *swithBg4 = (Layout*)item4->getChildByTag(105);
                lockBg = swithBg4;
                
                ImageView *on4 = (ImageView*)swithBg4->getChildByTag(CELL_SWITCH_ON);
                ImageView *off4 = (ImageView*)swithBg4->getChildByTag(CELL_SWITCH_OFF);
                
                on4->setVisible(!UserDefault::getInstance()->getBoolForKey(SETTINF_NOTICE_LOCK_SCREEN));
                off4->setVisible(UserDefault::getInstance()->getBoolForKey(SETTINF_NOTICE_LOCK_SCREEN));
                
                cell->addChild(item3);
                cell->addChild(item4);
                line = (Layout*)item4->getChildByTag(100);
                line->setVisible(false);
            }
                break;
                
        }
        cell->addChild(item1);
        cell->addChild(item2);
    }
    
    auto tipBg = Layout::create();
    tipBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    tipBg->setBackGroundColor(Color3B(0x14, 0x14, 0x15));
    tipBg->setContentSize(Size(visibleSize.width, 80));
    listView->addChild(tipBg);
    
    Text *tip = Text::create("(开启通知须至少选用一种通知形式)", ".SFUIDisplay-Semibold", 35);
    tip->ignoreAnchorPointForPosition(false);
    tip->setAnchorPoint(Vec2(0, 0.5));
    tip->setPosition(Vec2(30, tipBg->getContentSize().height/2));
    tip->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
    tipBg->addChild(tip);

    auto kongBg = Layout::create();
    kongBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    kongBg->setBackGroundColor(Color3B(0x14, 0x14, 0x15));
    kongBg->setContentSize(Size(visibleSize.width, 80));
    listView->addChild(kongBg);
    
    auto acountBg = Layout::create();
    acountBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    acountBg->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    acountBg->setContentSize(Size(visibleSize.width, 120));
    listView->addChild(acountBg);
    
    //登录账号
    std::string str;
    if (UserDefault::getInstance()->getIntegerForKey("thirdtype") == 3)
    {
        str = "登录微信账号:" + ZJHModel::getInstance()->nick;
    }else if(phone != "")
    {
         str = "登录账号:" + ZJHModel::getInstance()->hidePhoneNum(phone);
    }
    
    Text *acountText = Text::create(str, ".SFUIDisplay-Semibold", 50);
    acountText->setAnchorPoint(Vec2(0.5, 0.5));
    acountText->setPosition(Vec2(acountBg->getContentSize().width/2, acountBg->getContentSize().height/2));
    acountText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    acountText->setTextColor(Color4B(0xe1, 0xcc, 0xb0, 255));
    acountBg->addChild(acountText);

}

// 绑定微信
void SettingLayer::bindWeiXinOn(Ref *pSender)
{
    PlatformHelper::weiXinLoad(false);
}

//未登录的
void SettingLayer::unLoadView()
{
    listView->removeAllChildren();
    listView->setInertiaScrollEnabled(false);
    exitBtn->setEnabled(true);
    exitBtn->setTitleText("使用手机账号登录");
    exitBtn->addTouchEventListener(CC_CALLBACK_2(SettingLayer::cellPhoneGotoLoad, this));
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto cell = Layout::create();
    cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    cell->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    cell->setContentSize(Size(visibleSize.width, 220));
    listView->addChild(cell);
    
    Layout* item1 = cellItem(1,"手机号账号");
    item1->setPosition(Vec2(0, cell->getContentSize().height));
    cell->addChild(item1);
    
    Layout *item2 = cellItem(2,"手机号", "Mine/view_icon3_setting.png", "未登录");
    item2->setPosition(Vec2(0, cell->getContentSize().height - item1->getContentSize().height));
    item2->addTouchEventListener(CC_CALLBACK_2(SettingLayer::cellPhoneGotoLoad, this));
    cell->addChild(item2);
    
    Layout *line = (Layout*)item2->getChildByTag(100);
    line->setVisible(false);
    
    auto tipBg = Layout::create();
    tipBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    tipBg->setBackGroundColor(Color3B(0x14, 0x14, 0x15));
    tipBg->setContentSize(Size(visibleSize.width, 1160));
    listView->addChild(tipBg);
    
    //登录账号
    std::string str = "登录账号:" + ZJHModel::getInstance()->nick;
    Text *acountText = Text::create(str, ".SFUIDisplay-Semibold", 50);
    acountText->setTextColor(Color4B(0xe1, 0xcc, 0xb0, 255));
    listView->addChild(acountText);
    
}

//登录时，去修改手机号
void SettingLayer::cellUpdatePhone(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        //去修改手机号
        ZJHModel::getInstance()->gotoView(VIEW_UPDATE_PHONE2);
        
    }
}

//登录时，去修改密码
void SettingLayer::cellUpdatePassWord(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        //去修改密码
        ZJHModel::getInstance()->gotoView(VIEW_UPDATE_PASSWORD);
        
    }
}


//未登录的时候手机号cell去登录
void SettingLayer::cellPhoneGotoLoad(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        //登录界面
        ZJHModel::getInstance()->gotoView(VIEW_LOAD);
    }
    
}
void SettingLayer::exitLoadOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        exitBtn->setEnabled(false);
        //发送退出登录请求
        CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
            CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
            if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
            
            if (loginPacket->status != 3)
            {
                exitBtn->setEnabled(true);
                PlatformHelper::showToast("网络链接失败，请稍后再试");
                return;
            }
            
            if (loginPacket->resultIsOK())
            {
                //重新刷新界面为未登录状态

                unLoadView();
                
                Json::Value data = loginPacket->recvVal["resultMap"];
                
                //存储还原
                UserDefault::getInstance()->setIntegerForKey("thirdtype", -1);
                UserDefault::getInstance()->setStringForKey("wx_login_openid", data["thirdId"].asString());
                
                ZJHModel::getInstance()->setPhoneAndPwd("", "");
                
                ZJHModel::getInstance()->UserKey = data["userKey"].asString();
                ZJHModel::getInstance()->uid = data["id"].asInt();
                ZJHModel::getInstance()->nick = data["nickName"].asString();
                ZJHModel::getInstance()->portrit = data["thumbImage"].asString();
                ZJHModel::getInstance()->setUserType(data["userType"].asInt());
                ZJHModel::getInstance()->pwdOK = data["pwdOK"].asBool();
                
                MsgModel::getInstance()->ip = data["im"]["ipAddress"].asString();
                MsgModel::getInstance()->port = data["im"]["port"].asInt();
                
                ZJHModel::getInstance()->setUserType(6);
                
                __NotificationCenter::getInstance()->postNotification("account_logout");
                // 登录天数
                UserDefault::getInstance()->setIntegerForKey("days", 0);
                
                if (PlatformHelper::sdkDoForInt_C("hasThirdChannel"))
                {
                    ZJHModel::getInstance()->UserKey = "";
                    PlatformHelper::sdkDoForVoid_C("logout");
                } else {
                    ZJHModel::getInstance()->gotoView(VIEW_LOAD);
                }
            }else
            {
                exitBtn->setEnabled(true);
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
        }, "user/logout", "", "exit");
    }
    
}

void SettingLayer::checkBindState()
{
    Json::Value json2;
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
            Json::Value data1 = data["thirdBinded"];
            isWXBind = data1["wechat"].asBool();
            
            updateStateView(NULL);
        }
    },"account/getthirdbind", json2.toStyledString(), "isbindwx");
}

//手机绑定微信结果
void SettingLayer::phoneWXBindPost(Ref* pSender)
{
    __String *dic = (__String *)pSender;
    string code = dic->getCString();
    auto size = Director::getInstance()->getVisibleSize();
    Loading *loading = Loading::startLoading(Director::getInstance()->getRunningScene(), size / 2);
    loading->maskSwllowTouch();
    
    //手机绑定微信
    Json::Value json2;
    json2["platFrom"] = 1;
    json2["thirdType"] = 3;
    json2["loginType"] = 3;
    json2["userFrom"] = 1;
    json2["code"] = code;
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        loading->removeFromParent();
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"];
            UserDefault::getInstance()->setStringForKey("wx_login_openid", data["thirdId"].asString());
            
            postUserInfo();
            
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    }, "user/bindthird", json2.toStyledString(), "phoneWeiXinbing");
}

void SettingLayer::postUserInfo()
{
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            //跟新玩家的头像和昵称
            listView->removeAllChildren();
            updateView(NULL);
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"]["user"];
            
            ZJHModel::getInstance()->nick = data["nickName"].asString();
            ZJHModel::getInstance()->portrit = data["thumbImage"].asString();
            
            __NotificationCenter::getInstance()->postNotification("account_change");
            
        }else
        {
            //跟新玩家的头像和昵称
            listView->removeAllChildren();
            updateView(NULL);
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    }, "account/userInfo", "", "userInfoUpdate");

}
void SettingLayer::thirdLoginPost()
{
    auto size = Director::getInstance()->getVisibleSize();
    Loading *loading = Loading::startLoading(Director::getInstance()->getRunningScene(), size / 2);
    loading->maskSwllowTouch();
    
    //游客登录请求
    Json::Value json2;
    json2["platFrom"] = 1;
    json2["thirdType"] = 4;
    json2["loginType"] = 3;
    json2["userFrom"] = 1;
    json2["thirdId"] = ZJHModel::getInstance()->randomThirdID(true);
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        loading->removeFromParent();
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"];
            
            ZJHModel::getInstance()->setPhoneAndPwd("", "");
            
            ZJHModel::getInstance()->UserKey = data["userKey"].asString();
            ZJHModel::getInstance()->uid = data["id"].asInt();
            ZJHModel::getInstance()->nick = data["nickName"].asString();
            ZJHModel::getInstance()->portrit = data["thumbImage"].asString();
            ZJHModel::getInstance()->setUserType(data["userType"].asInt());
            ZJHModel::getInstance()->pwdOK = data["pwdOK"].asBool();
            MsgModel::getInstance()->ip = data["im"]["ipAddress"].asString();
            MsgModel::getInstance()->port = data["im"]["port"].asInt();
            
            __NotificationCenter::getInstance()->postNotification("account_change");
 //           MyNotification::getInstance()->postNotify("account_change");
            
        }else
        {
            ZJHModel::getInstance()->gotoView(VIEW_LOAD);
            this->disappear();
            
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    }, "user/thirdLogin", json2.toStyledString(), "Youlogin");
}

void SettingLayer::swithClickFun(Ref *pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::BEGAN == type)
    {
        //true ：关 false：开
        bool is1 =  UserDefault::getInstance()->getBoolForKey(SETTINF_NOTICE_OFF );
        bool is2 =  UserDefault::getInstance()->getBoolForKey(SETTINF_NOTICE_BAR_OFF);
        bool is3 =  UserDefault::getInstance()->getBoolForKey(SETTINF_NOTICE_LOCK_SCREEN);
        
        Layout *curBg = (Layout *)pSender;
        ImageView *on = (ImageView*)curBg->getChildByTag(CELL_SWITCH_ON);
        ImageView *off = (ImageView*)curBg->getChildByTag(CELL_SWITCH_OFF);
    
        if (curBg == noticeBg)
        {
            on->setVisible(!on->isVisible());
            off->setVisible(!off->isVisible());
            UserDefault::getInstance()->setBoolForKey(SETTINF_NOTICE_OFF, off->isVisible());
            
            ImageView *barOn = (ImageView*)barBg->getChildByTag(CELL_SWITCH_ON);
            ImageView *barOff = (ImageView*)barBg->getChildByTag(CELL_SWITCH_OFF);
            barOn->setVisible(on->isVisible());
            barOff->setVisible(off->isVisible());
            UserDefault::getInstance()->setBoolForKey(SETTINF_NOTICE_BAR_OFF, off->isVisible());
            
            ImageView *lockOn = (ImageView*)lockBg->getChildByTag(CELL_SWITCH_ON);
            ImageView *lockOff = (ImageView*)lockBg->getChildByTag(CELL_SWITCH_OFF);
            lockOn->setVisible(on->isVisible());
            lockOff->setVisible(off->isVisible());
            UserDefault::getInstance()->setBoolForKey(SETTINF_NOTICE_LOCK_SCREEN, off->isVisible());
            
        }else
        {
            if (is1)
            {
                return;
            }else
            {
                if (curBg == barBg)
                {
                    if (is3 == true && is2 == false)
                    {
                        PlatformHelper::showToast("至少开启一种通知形式");
                        return;
                    }else if (is3 == false)
                    {
                        on->setVisible(!on->isVisible());
                        off->setVisible(!off->isVisible());
                        UserDefault::getInstance()->setBoolForKey(SETTINF_NOTICE_BAR_OFF, off->isVisible());
                    }
                }else if (curBg == lockBg)
                {
                    if (is2 == true && is3 == false)
                    {
                        PlatformHelper::showToast("至少开启一种通知形式");
                        return;
                    }else if (is2 == false)
                    {
                        on->setVisible(!on->isVisible());
                        off->setVisible(!off->isVisible());
                        UserDefault::getInstance()->setBoolForKey(SETTINF_NOTICE_LOCK_SCREEN, off->isVisible());
                    }
                }
                
            }
        }
        
    }
    
}
