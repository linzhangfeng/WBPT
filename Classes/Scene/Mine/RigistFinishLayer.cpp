//
//  SettingPassWordLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/8.
//
//

#include "RigistFinishLayer.hpp"
#include "../Load/GameLoad.hpp"
#include "../Loading/Loading.h"
#include "../../Model/MsgModel.h"

bool RigistFinishLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    if (ZJHModel::getInstance()->NOPwdSetPwd == "NOPwdSetPwd")
    {
        setTitle("设置密码");
        
    }else
    {
        setTitle("注册");
    }
    
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();
    
    auto cell = Layout::create();
    cell->ignoreAnchorPointForPosition(false);
    cell->setAnchorPoint(Vec2(0, 1));
    cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    cell->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    cell->setPosition(Vec2(0, H));
    cell->setContentSize(Size(visibleSize.width, 180));
    this->addChild(cell);
    
    Text *title = Text::create("设置密码", ".SFUIDisplay-Semibold", 40);
    title->setPosition(Vec2(210, 90));
    title->setTextColor(Color4B(0xde, 0xde, 0xdf, 255));
    title->setAnchorPoint(Vec2(1, 0.5));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    cell->addChild(title);
    
    textField1 = EditBox::create(Size(800, 100), "Mine/bar_sendtxt.png");
    textField1->setPlaceHolder("请输入密码");
    textField1->setAnchorPoint(Vec2(0, 0.5));
    textField1->setMaxLength(15);
    textField1->setInputMode(EditBox::InputMode::ANY);
    textField1->setInputFlag(EditBox::InputFlag::PASSWORD);
    textField1->setPosition(Vec2(235, 90));
    textField1->setFontColor(Color3B(0x66, 0x66, 0x66));
    textField1->setPlaceholderFontColor(Color3B(0xb2, 0xb2, 0xb2));
    textField1->setFontSize(45);
    textField1->setPlaceholderFontSize(40);
    cell->addChild(textField1);

    Button *nextBtn = Button::create("Mine/btn_yellow_big.png","Mine/btn_yellow_big_check.png","Mine/btn_yellow_big_check.png");
    nextBtn->setScale9Enabled(true);
    nextBtn->setName("registBtn");
    nextBtn->setContentSize(Size(960, 120));
    nextBtn->setPosition(Vec2(visibleSize.width/2, H - 280));
    this->addChild(nextBtn);
    if (ZJHModel::getInstance()->NOPwdSetPwd == "NOPwdSetPwd")
    {
        nextBtn->setTitleText("设置密码");
        nextBtn->addTouchEventListener(CC_CALLBACK_1(RigistFinishLayer::setPassWordFinish, this));
        
    }else
    {
        nextBtn->setTitleText("完成");
        nextBtn->addTouchEventListener(CC_CALLBACK_2(RigistFinishLayer::registFinish, this));
    }
    
    nextBtn->setTitleFontSize(40);
    nextBtn->setTitleColor(Color3B(0x65, 0x40, 0x02));
    nextBtn->setTitleFontName(".SFUIDisplay-Semibold");
    

    return true;
}

void RigistFinishLayer::setPassWordFinish(Ref *pSender)
{
    //设置密码提交注册
    std::string passString = textField1->getText();
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
    
    Json::Value json;
    json["password"] = passTemp;

    if (passString.size() == 0)
    {
        PlatformHelper::showToast("密码不能为空");
    }else
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
                //回到登录界面,存储用户
                PlatformHelper::showToast("设置密码成功");
                Json::Value data = loginPacket->recvVal["resultMap"]["user"];
    
                ZJHModel::getInstance()->pwdOK = data["pwdOK"].asBool();
                ZJHModel::getInstance()->setPhoneAndPwd(ZJHModel::getInstance()->phone, passTemp);
                
                ZJHModel::getInstance()->NOPwdSetPwd = "";
               __NotificationCenter::getInstance()->postNotification("setPwd_finish");
                
                this->setPreByName("SettingLayer");
                this->disappear();
                
            }else
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
        },"/account/setPwd",json.toStyledString(),"setPwd");
    }

}
//确认设置密码提交注册
void RigistFinishLayer::registFinish(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        //设置密码提交注册
        std::string passString = textField1->getText();
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
        
        Json::Value json;
        json["phone"] = acountJson["phone"];
        json["phoneCode"] = acountJson["phoneCode"];
        json["password"] = passTemp;
        json["platFrom"] = 1; //1:app 2:web 网页
        
        if (passString.size() == 0)
        {
            PlatformHelper::showToast("密码不能为空");
        }else
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
                    //回到登录界面,存储用户
                    Json::Value data = loginPacket->recvVal["resultMap"];
                    ZJHModel::getInstance()->setPhoneAndPwd(acountJson["phone"].asString(), passTemp);
                    postLogin();
                    Button *btn = (Button*)this->getChildByName("registBtn");
                    btn->setEnabled(false);
                    
                }else
                {
                    PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                }
            },"user/reg",json.toStyledString(),"regFinish");

        }

    }
}

//登录请求
void RigistFinishLayer::postLogin()
{
    Json::Value json;
    json["phone"] = ZJHModel::getInstance()->phone;
    json["loginType"] = 1;  //0:unknown 1:normal 2:fast 3:third
    json["password"] = ZJHModel::getInstance()->pwd;
    json["platFrom"] = 1; //1:app 2:wap 网页
    json["userFrom"] = 1; //1:app 2:wap 网页
    
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
                                                     this->removeToPreByName("LoadLayer");
                                                     this->disappear();
                                                     
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
                                                     
                                                     PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                                                 }
                                             },"user/login", json.toStyledString(),"login");
    
}


void RigistFinishLayer::getInfoJson(Json::Value json)
{
    acountJson = json;
}