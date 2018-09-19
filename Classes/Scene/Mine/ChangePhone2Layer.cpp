//
//  ChangePhone2Layer.cpp
//  ZJH
//
//  Created by apple on 16/7/11.
//
//

#include "ChangePhone2Layer.hpp"


bool ChangePhone2Layer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();
    
    setTitle("修改手机号");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    Button *sureBtn = Button::create("Mine/btn_yellow_big.png","Mine/btn_yellow_big_check.png", "Mine/btn_yellow_big_check.png");
    sureBtn->setScale9Enabled(true);
    sureBtn->setContentSize(Size(960, 120));
    sureBtn->setPosition(Vec2(visibleSize.width/2, H - 850));
    this->addChild(sureBtn);
    sureBtn->setTitleText("确认修改");
    sureBtn->setTitleFontSize(40);
    sureBtn->setTitleColor(Color3B(0x65, 0x40, 0x02));
    sureBtn->setTitleFontName("");
    sureBtn->addTouchEventListener(CC_CALLBACK_2(ChangePhone2Layer::finishOn, this));
    
    for (int i = 0; i < 2; i++)
    {
        Layout* cell = Layout::create();
        cell->ignoreAnchorPointForPosition(false);
        cell->setAnchorPoint(Vec2(0, 1));
        cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        cell->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
        cell->setContentSize(Size(visibleSize.width, 95));
        this->addChild(cell);
        
        Text *title = Text::create("", "", 36);
        title->setPosition(Vec2(210, cell->getContentSize().height/2));
        title->setTextColor(Color4B(0x99, 0x99, 0x99, 255));
        title->setAnchorPoint(Vec2(0, 0.5));
        title->setPosition(Vec2(40, cell->getContentSize().height/2));
        title->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        cell->addChild(title);
        
        Layout *line = Layout::create();
        line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        line->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
        line->setPosition(Vec2(40, 0));
        line->setContentSize(Size(visibleSize.width-40, 1));
        cell->addChild(line);

        switch (i)
        {
            case 0:
                cell->setPosition(Vec2(0, H - 30));
                title->setString("第一步验证原手机号");
                break;
                
            case 1:
                cell->setPosition(Vec2(0, H - 30*2 - 95 - 156));
                title->setString("第二步绑定新手机号");
                break;
        }
    }
    
    for (int i = 0; i < 3; i++)
    {
        auto cell = Layout::create();
        cell->ignoreAnchorPointForPosition(false);
        cell->setAnchorPoint(Vec2(0, 1));
        cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        cell->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
        cell->setContentSize(Size(visibleSize.width, 156));
        this->addChild(cell);
        
        Text *title = Text::create("", "", 40);
        title->setPosition(Vec2(210, cell->getContentSize().height/2));
        title->setTextColor(Color4B(0xde, 0xde, 0xdf, 255));
        title->setAnchorPoint(Vec2(1, 0.5));
        title->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        cell->addChild(title);
        
        EditBox* edit = EditBox::create(Size(800, 90), "Mine/bar_sendtxt.png");
        edit->setPlaceholderFontSize(40);
        edit->setAnchorPoint(Vec2(0, 0.5));
        edit->setMaxLength(11);
        edit->setInputMode(EditBox::InputMode::NUMERIC);
        edit->setPosition(Vec2(235, cell->getContentSize().height/2));
        edit->setFontColor(Color3B(0x99, 0x99, 0x99));
        edit->setPlaceholderFontColor(Color3B(0x4c, 0x4c, 0x4c));
        edit->setFontSize(40);
        edit->setPlaceholderFontSize(40);
        cell->addChild(edit);
        
        Layout *line;
        
        switch (i)
        {
            case 0:
                edit1 = edit;
                cell->setPosition(Vec2(0, H - 30 - 95));
                title->setString("原手机号");
                edit->setPlaceHolder("请输入原手机号");
                
                line = Layout::create();
                line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                line->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
                line->setPosition(Vec2(0, 0));
                line->setContentSize(Size(visibleSize.width, 2));
                cell->addChild(line);
                
                break;
            case 1:
                edit2 = edit;
                cell->setPosition(Vec2(0, H - 30*2 - 95*2 - 156));
                title->setString("新手机号");
                edit->setPlaceHolder("请输入新手机号");
                
                line = Layout::create();
                line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                line->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
                line->setPosition(Vec2(40, 0));
                line->setContentSize(Size(visibleSize.width - 40, 1));
                cell->addChild(line);
                
                break;
            case 2:
                edit3 = edit;
                cell->setPosition(Vec2(0, H - 30*2 - 95*2 - 156*2));
                title->setString("验证码");
                edit->setPlaceHolder("请输入短信验证码");
                edit->setContentSize(Size(490, 90));
                
                msgBtn = Button::create("Mine/btn_yellow_small.png","Mine/btn_yellow_small_check.png","Mine/btn_yellow_small_check.png");
                msgBtn->setScale9Enabled(true);
                msgBtn->setContentSize(Size(270, 90));
                msgBtn->setPosition(Vec2(visibleSize.width - 160, 90));
                cell->addChild(msgBtn);
                msgBtn->setTitleText("发送验证码");
                msgBtn->setTitleFontSize(36);
                msgBtn->setTitleColor(Color3B(0x65, 0x40, 0x02));
                msgBtn->setTitleFontName("");
                msgBtn->addTouchEventListener(CC_CALLBACK_2(ChangePhone2Layer::postCheckNumOn, this));
                
                line = Layout::create();
                line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                line->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
                line->setPosition(Vec2(0, 0));
                line->setContentSize(Size(visibleSize.width, 2));
                cell->addChild(line);
                
                break;
        }
    }
    return true;
}

//发送验证码
void ChangePhone2Layer::postCheckNumOn(Ref *pSender, Widget::TouchEventType type)
{
    Button *but = (Button *)pSender;
    but->setEnabled(false);
    if (type == Widget::TouchEventType::BEGAN)
    {
        /*
         sendType:
         (1,"注册类型"),	(2, "手机绑定"),	(3, "修改手机"),	(4, "重置密码"),
         (5, "实名认证"),	(6, "系统警报"),	(7, "忘记密码"),    (8,"登录类型") (9,"美女认证")
         */
        Json::Value json;
        json["phone"] =  edit2->getText();
        json["sendType"] = 3;
        
        CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
            CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
            if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
            
            if (loginPacket->status != 3)
            {
                but->setEnabled(true);
                PlatformHelper::showToast("网络链接失败，请稍后再试");
                return;
            }
            
            if (loginPacket->resultIsOK())
            {
                Json::Value data = loginPacket->recvVal["resultMap"];
                secend = 120;
                but->setTitleText("发送中...");
                this->unscheduleUpdate();
                this->schedule(schedule_selector(ChangePhone2Layer::timeDown), 1.0);
            }else
            {
                but->setEnabled(true);
                if(loginPacket->recvVal["message"].isString() && !loginPacket->recvVal["message"].isNull())
                {
                    PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                }
            }
        },"user/getPhoneCode",json.toStyledString(),"PhoneCode");

    }
}

void ChangePhone2Layer::timeDown(float dt)
{
    if (secend >= 0)
    {
        char s[128];
        sprintf(s, "%ds后重新发送", secend);
        msgBtn->setTitleText(s);
        msgBtn->setEnabled(false);
    }else
    {
        msgBtn->setTitleText("发送验证码");
        msgBtn->setEnabled(true);
        this->unscheduleUpdate();
    }
    secend--;
    
}

//确认修改新手机号
void ChangePhone2Layer::finishOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        if (edit1->getText() != ZJHModel::getInstance()->phone)
        {
            PlatformHelper::showToast("原手机号输入错误");
            return;
        }
    
        string edit3Str = edit3->getText();
        if (edit3Str == "")
        {
            PlatformHelper::showToast("验证码不能为空");
            return;
        }
        
        Button *finishBtn = (Button*)pSender;
        finishBtn->setEnabled(false);
        
        Json::Value json;
        json["oldPhone"] = ZJHModel::getInstance()->phone;
        json["newPhone"] = edit2->getText();
        json["phoneCode"] = edit3->getText();

        CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
            CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
            if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
            
            if (loginPacket->status != 3)
            {
                finishBtn->setEnabled(true);
                PlatformHelper::showToast("网络链接失败，请稍后再试");
                return;
            }
            
            if (loginPacket->resultIsOK())
            {
                ZJHModel::getInstance()->setPhoneAndPwd(edit2->getText(), ZJHModel::getInstance()->pwd);
                __NotificationCenter::getInstance()->postNotification("phone_change");
                this->unscheduleUpdate();
                this->disappear();
                
            }else
            {
                finishBtn->setEnabled(true);
                if(loginPacket->recvVal["message"].isString() && !loginPacket->recvVal["message"].isNull())
                {
                    PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                }
            }
        },"account/updatePhone",json.toStyledString(),"updatePhone");
    }
}


