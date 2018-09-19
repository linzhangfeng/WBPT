//
//  LoadLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/8.
//
//

#include "PhoneLoadLayer.hpp"
#include "../Loading/Loading.h"
#include "../Load/GameLoad.hpp"
#include "../Load/TipLoadLayer.hpp"

#include "../../Model/MsgModel.h"

PhoneLoadLayer::PhoneLoadLayer()
{
    nextBtn = NULL;
}

bool PhoneLoadLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    setTitle("手机号登录");
    setName("PhoneLoadLayer");
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();
    
    for (int i = 0; i < 2; i++)
    {
        auto line1 = Layout::create();
        line1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        line1->ignoreAnchorPointForPosition(false);
        line1->setBackGroundColor(Color3B(0xac, 0x88, 0x39));
        line1->setAnchorPoint(Vec2(0.5, 0));
        line1->setPosition(Vec2(visibleSize.width/2, H - 145 - i*135));
        line1->setContentSize(cocos2d::Size(885, 3));
        this->addChild(line1);
        
        ImageView *mark = ImageView::create();
        mark->ignoreContentAdaptWithSize(false);
        mark->setAnchorPoint(Vec2(0, 1));
        mark->setPosition(Vec2(110, H - 72 - 135*i));
        this->addChild(mark, 10);
        
        EditBox *Field1 = EditBox::create(cocos2d::Size(770, 95), Scale9Sprite::create());
        Field1->setAnchorPoint(Vec2(0, 0));
        Field1->setPlaceholderFontSize(40);
        Field1->setFontSize(45);
        Field1->setAnchorPoint(Vec2(0, 0));
        Field1->setPosition(Vec2(175, H - 145 - 135*i));
        Field1->setFontColor(Color3B(0x99, 0x99, 0x99));
        Field1->setPlaceholderFontColor(Color3B(0x71, 0x64, 0x4d));
        this->addChild(Field1);
        
        switch (i)
        {
            case 0:
                mark->loadTexture("Mine/load_phone.png");
                Field1->setPlaceHolder("输入手机号");
                textField1 = Field1;
                Field1->setMaxLength(11);
                Field1->setInputMode(EditBox::InputMode::PHONE_NUMBER);
                break;
            case 1:
                mark->loadTexture("Mine/load_pass.png");
                Field1->setPlaceHolder("输入密码");
                textField2 = Field1;
                Field1->setInputFlag(EditBox::InputFlag::PASSWORD);
                Field1->setInputMode(EditBox::InputMode::SINGLE_LINE);
                break;
            break;
        }
    }
    
    Text *text4 = Text::create("快速登录", "", 40);
    text4->setTouchEnabled(true);
    text4->addTouchEventListener(CC_CALLBACK_2(PhoneLoadLayer::speedLoadOn, this));
    text4->setAnchorPoint(Vec2(1, 0));
    text4->setTextHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
    text4->setPosition(Vec2(visibleSize.width - 100, H - 672));
    text4->setTextColor(Color4B(0x99, 0x99, 0x99, 255));
    this->addChild(text4);
    
    Text *text5 = Text::create("忘记密码", "", 40);
    text5->setTouchEnabled(true);
    text5->addTouchEventListener(CC_CALLBACK_2(PhoneLoadLayer::lostPassWordOn, this));
    text5->setAnchorPoint(Vec2(0, 0));
    text5->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    text5->setPosition(Vec2(100, text4->getPositionY()));
    text5->setTextColor(Color4B(0x99, 0x99, 0x99, 255));
    this->addChild(text5);
    
    nextBtn = Button::create("Mine/load_Btn1.png","Mine/load_Btn2.png", "Mine/load_Btn2.png");
    nextBtn->setAnchorPoint(Vec2(0.5, 0));
    nextBtn->setPosition(Vec2(visibleSize.width/2, H - 435));
    this->addChild(nextBtn);
    nextBtn->addClickEventListener(CC_CALLBACK_1(PhoneLoadLayer::loadOn, this));
    
    Button *registBtn = Button::create("Mine/zhuce_phone_btn1.png","Mine/zhuce_phone_btn1.png", "Mine/zhuce_phone_btn1.png");
    registBtn->setAnchorPoint(Vec2(0.5, 0));
    registBtn->setPosition(Vec2(visibleSize.width/2, H - 590));
    this->addChild(registBtn);
    registBtn->addClickEventListener([=](Ref*ref){
        
        ZJHModel::getInstance()->gotoView(VIEW_REGIST);
    });

    auto leftline = Layout::create();
    leftline->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    leftline->ignoreAnchorPointForPosition(false);
    leftline->setBackGroundColor(Color3B(0xac, 0x88, 0x39));
    leftline->setAnchorPoint(Vec2(0, 0));
    leftline->setPosition(Vec2(100, H - 690));
    leftline->setContentSize(cocos2d::Size(170, 3));
    this->addChild(leftline);
    
    auto rightline = Layout::create();
    rightline->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    rightline->ignoreAnchorPointForPosition(false);
    rightline->setBackGroundColor(Color3B(0xac, 0x88, 0x39));
    rightline->setAnchorPoint(Vec2(1, 0));
    rightline->setPosition(Vec2(visibleSize.width - 100, leftline->getPositionY()));
    rightline->setContentSize(cocos2d::Size(170, 3));
    this->addChild(rightline);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(PhoneLoadLayer::rePostLoad), "re_connect_LoadLayer", NULL);
    
    return true;
}

void PhoneLoadLayer::rePostLoad(Ref*ref)
{
    postLogin(loadJson);
}

//忘记密码
void PhoneLoadLayer::lostPassWordOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        //忘记密码
        ZJHModel::getInstance()->gotoView(VIEW_FORGET_PASSWORD1);
    }
    
}

//快速登录
void PhoneLoadLayer::speedLoadOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        //快速登录
        ZJHModel::getInstance()->gotoView(VIEW_FAST_LOAD);
    }
}

//登录响应
void PhoneLoadLayer::loadOn(Ref *pSender)
{
    nextBtn->setEnabled(false);
    
    std::string passString = textField2->getText();
    const char* pass = passString.c_str();
    pass = PlatformHelper::encryptWithString(pass);
    char passTemp[300];
    int k = 0;
    for (int i = 0; i < strlen(pass); i++)
    {
        if (pass[i] == '+')
        {
            passTemp[k++] = '%';
            passTemp[k++] = '2';
            passTemp[k++] = 'B';
        }else
        {
            passTemp[k++] = pass[i];
        }
    }
    passTemp[k] = '\0';
    
    log("DD = %s\n", passTemp);
    Json::Value json;
    json["phone"] = textField1->getText();
    json["loginType"] = 1;  //0:unknown 1:normal 2:fast 3:third
    json["password"] = passTemp;
    json["platFrom"] = 1; //1:app 2:wap 网页
    json["userFrom"] = 1; //1:app 2:wap 网页
    
    loadJson = json;
    //登录请求
    postLogin(json);
    
}

//登录请求
void PhoneLoadLayer::postLogin(Json::Value json)
{
    auto size = Director::getInstance()->getVisibleSize();
    Loading *loading = Loading::startLoading(Director::getInstance()->getRunningScene(), size / 2);
    loading->maskSwllowTouch();
    
    //登录请求
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
                                                 if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
                                                 
                                                 loading->removeFromParent();
                                                 
                                                 CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
                                                 
                                                 if (loginPacket->status != 3)
                                                 {
                                                     PlatformHelper::showToast("网络链接失败，请稍后再试");
                                                     
                                                     TipLoadLayer *loadTip = TipLoadLayer::create(2);
                                                     this->addChild(loadTip, 1000);
                                                     return;
                                                 }
                                                 if (loginPacket->resultIsOK())
                                                 {
                                                     
                                                     //注册完成记录本地存储账号的信息
                                                     Json::Value data = loginPacket->recvVal["resultMap"];
                                                     
                                                     //刷新设置界面
                                                     printf("登录成功%s\n", data.toStyledString().c_str() );
                                                     
                                                     ZJHModel::getInstance()->nick = data["nickName"].asString();
                                                     ZJHModel::getInstance()->vipLv = data["vipLv"].asInt();
                                                     ZJHModel::getInstance()->portrit = data["thumbImage"].asString();
                                                     ZJHModel::getInstance()->setUserType(data["userType"].asInt());
                                                     ZJHModel::getInstance()->pwdOK = data["pwdOK"].asBool();
                                                     
                                                     ZJHModel::getInstance()->setPhoneAndPwd(data["phone"].asString(), json["password"].asString());
                                                     ZJHModel::getInstance()->UserKey = data["userKey"].asString();
                                                     ZJHModel::getInstance()->uid = data["id"].asInt();
                                                     
                                                     MsgModel::getInstance()->ip = data["im"]["ipAddress"].asString();
                                                     MsgModel::getInstance()->port = data["im"]["port"].asInt();
                                                     
                                                     if (ZJHModel::getInstance()->isGameLoad)
                                                     {
                                                         Director::getInstance()->replaceScene(GameLoad::createScene());
                                                     }
                                                     else
                                                     {
                                                          __NotificationCenter::getInstance()->postNotification("account_change");
                                                         
                                                         this->removeToPreByName("LoadLayer");
                                                         this->disappear();
                                                     }
                                                 }else
                                                 {
                                                     
                                                     if (nextBtn != NULL)
                                                     {
                                                         nextBtn->setEnabled(true);
                                                     }
                                                     PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                                                 }
                                             },"user/login", json.toStyledString(),"login");

}
