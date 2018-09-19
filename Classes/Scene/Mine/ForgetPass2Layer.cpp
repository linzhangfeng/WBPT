
//
//  ForgetPass2Layer.cpp
//  ZJH
//
//  Created by apple on 16/7/11.
//
//

#include "ForgetPass2Layer.hpp"

#define CELLH2  160
#define ITERH_MAX   30
#define ITERH_MIN   3


bool ForgetPass2Layer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("忘记密码");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    return true;
}

//确认设置密码提交注册
void ForgetPass2Layer::finish(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        std::string passString1 = textPass1->getText();
        std::string passString2 = textPass2->getText();
        
        if(passString1 != passString2)
        {
            PlatformHelper::showToast("两次密码输入不一致");
            return;
        }else if(passString1 == "")
        {
            PlatformHelper::showToast("密码不能为空");
            return;
        }else if(passString1 != "" && passString2 == "")
        {
            PlatformHelper::showToast("请确认密码");
            return;
        }
        
        //设置密码提交注册
        const char* pass = passString2.c_str();
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
                    //设置密码成功
                    //当前层移除
                    PlatformHelper::showToast("找回密码成功");
                    Json::Value data = loginPacket->recvVal["resultMap"];
                    
                    ZJHModel::getInstance()->setPhoneAndPwd(acountJson["phone"].asString(), passTemp);
                    
                    //跳转到登录界面
                    this->setPreByName("PhoneLoadLayer");
                    this->disappear();
                }else
                {
                    PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                }
                
            },"account/forgetPwd",json.toStyledString(),"forgetPwd");
        }
}

void ForgetPass2Layer::getInfoJson(Json::Value json)
{
    acountJson = json;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH() - 25;
    
    for (int i = 0; i < 3; i++)
    {
        EditBox *textField;
        Text *title;
        Layout *cell;
        Text *phoneText;
        string phone;
        Layout* line;
        
        switch (i)
        {
            case 0:
                cell = Layout::create();
                cell->ignoreAnchorPointForPosition(false);
                cell->setAnchorPoint(Vec2(0, 1));
                cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                cell->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
                cell->setContentSize(Size(visibleSize.width, 180));
                this->addChild(cell);
                
                title = Text::create("手机号", "", 40);
                cell->setPosition(Vec2(0, H));
                
                phone = ZJHModel::getInstance()->hidePhoneNum(acountJson["phone"].asString());
                
                phoneText = Text::create(phone.c_str(), "", 40);
                phoneText->setPosition(Vec2(235, 90));
                phoneText->setTextColor(Color4B(0xe6, 0xe6, 0xe6, 255));
                phoneText->setAnchorPoint(Vec2(0, 0.5));
                phoneText->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
                cell->addChild(phoneText);
                
                line = Layout::create();
                line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                line->setBackGroundColor(Color3B(0, 0, 0));
                line->setPosition(Vec2(0, 0));
                line->setContentSize(Size(visibleSize.width, 2));
                cell->addChild(line);
                
                break;
                
            case 1:
                cell = Layout::create();
                cell->ignoreAnchorPointForPosition(false);
                cell->setAnchorPoint(Vec2(0, 1));
                cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                cell->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
                cell->setContentSize(Size(visibleSize.width, 180));
                cell->setPosition(Vec2(0, H - 30 - 180));
                this->addChild(cell);
                
                title = Text::create("新密码", "", 40);
                textField = EditBox::create(Size(800, 100), "Mine/bar_sendtxt.png");
                textField->setPlaceholderFontSize(40);
                textField->setAnchorPoint(Vec2(0, 0.5));
                textField->setMaxLength(15);
                textField->setInputMode(EditBox::InputMode::SINGLE_LINE);
                textField->setInputFlag(EditBox::InputFlag::PASSWORD);
                textField->setPosition(Vec2(235, 90));
                textField->setFontColor(Color3B(0x66, 0x66, 0x66));
                textField->setPlaceholderFontColor(Color3B(0xb2, 0xb2, 0xb2));
                textField->setFontSize(45);
                textField->setPlaceholderFontSize(40);
                cell->addChild(textField);
                
                line = Layout::create();
                line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                line->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
                line->setPosition(Vec2(30, 0));
                line->setContentSize(Size(visibleSize.width-30, 1));
                cell->addChild(line);
                
                textPass1 =textField;
                break;
                
            case 2:
                cell = Layout::create();
                cell->ignoreAnchorPointForPosition(false);
                cell->setAnchorPoint(Vec2(0, 1));
                cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                cell->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
                cell->setContentSize(Size(visibleSize.width, 180));
                cell->setPosition(Vec2(0, H - 30 - 180*2));
                this->addChild(cell);
                
                title = Text::create("确认密码", "", 40);
                textField = EditBox::create(Size(800, 100), "Mine/bar_sendtxt.png");
                textField->setPlaceholderFontSize(40);
                textField->setAnchorPoint(Vec2(0, 0.5));
                textField->setMaxLength(15);
                textField->setInputMode(EditBox::InputMode::SINGLE_LINE);
                textField->setInputFlag(EditBox::InputFlag::PASSWORD);
                textField->setPosition(Vec2(235, 90));
                textField->setFontColor(Color3B(0x66, 0x66, 0x66));
                textField->setPlaceholderFontColor(Color3B(0xb2, 0xb2, 0xb2));
                textField->setFontSize(45);
                textField->setPlaceholderFontSize(40);
                cell->addChild(textField);
                
                line = Layout::create();
                line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                line->setBackGroundColor(Color3B(0, 0, 0));
                line->setPosition(Vec2(0, 0));
                line->setContentSize(Size(visibleSize.width, 2));
                cell->addChild(line);
                
                textPass2 =textField;
                break;
        }
        title->setPosition(Vec2(210, 90));
        title->setTextColor(Color4B(0xde, 0xde, 0xdf, 255));
        title->setAnchorPoint(Vec2(1, 0.5));
        title->setTextHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
        cell->addChild(title);
    }
    
    Button *finishBtn = Button::create("Mine/btn_yellow_big.png","Mine/btn_yellow_big_check.png");
    finishBtn->setScale9Enabled(true);
    finishBtn->setContentSize(Size(960, 120));
    finishBtn->setPosition(Vec2(visibleSize.width/2, H - 480-180));
    this->addChild(finishBtn);
    finishBtn->setTitleText("完成");
    finishBtn->setTitleFontSize(40);
    finishBtn->setTitleColor(Color3B(0x65, 0x40, 0x02));
    finishBtn->setTitleFontName("");
    finishBtn->addTouchEventListener(CC_CALLBACK_2(ForgetPass2Layer::finish, this));

}
