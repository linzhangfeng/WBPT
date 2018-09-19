//
//  ChangePassWordLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/11.
//
//

#include "ChangePassWordLayer.hpp"

#define CELLH2  160
#define ITERH_MAX   30
#define ITERH_MIN   3

bool ChangePassWordLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("修改密码");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));

    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH() - 25;
    
    for (int i = 0; i < 3; i++)
    {
        auto cell = Layout::create();
        cell->ignoreAnchorPointForPosition(false);
        cell->setAnchorPoint(Vec2(0, 1));
        cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        cell->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
        cell->setContentSize(Size(visibleSize.width, 180));
        this->addChild(cell);
        
        Text *title = Text::create("", "", 40);
        title->setPosition(Vec2(210, 90));
        title->setTextColor(Color4B(0xde, 0xde, 0xdf, 255));
        title->setAnchorPoint(Vec2(1, 0.5));
        title->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        cell->addChild(title);
        
        EditBox* edit = EditBox::create(Size(800, 100), "Mine/bar_sendtxt.png");
        edit->setPlaceholderFontSize(40);
        edit->setAnchorPoint(Vec2(0, 0.5));
        edit->setMaxLength(15);
        edit->setInputMode(EditBox::InputMode::ANY);
        edit->setInputFlag(EditBox::InputFlag::PASSWORD);
        edit->setPosition(Vec2(235, cell->getContentSize().height/2));
        edit->setFontColor(Color3B(0x66, 0x66, 0x66));
        edit->setPlaceholderFontColor(Color3B(0xb2, 0xb2, 0xb2));
        edit->setFontSize(45);
        edit->setPlaceholderFontSize(40);
        cell->addChild(edit);
        
        Layout *line;
        
        switch (i)
        {
            case 0:
                edit1 = edit;
                cell->setPosition(Vec2(0, H));
                title->setString("原密码");
                edit->setPlaceHolder("请输入原密码");
                
                line = Layout::create();
                line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                line->setBackGroundColor(Color3B(0, 0, 0));
                line->setPosition(Vec2(0, 0));
                line->setContentSize(Size(visibleSize.width, 2));
                cell->addChild(line);
                
                break;
            case 1:
                edit2 = edit;
                cell->setPosition(Vec2(0, H - 30 - 180));
                title->setString("新密码");
                edit->setPlaceHolder("请输入新密码");
                
                line = Layout::create();
                line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                line->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
                line->setPosition(Vec2(30, 0));
                line->setContentSize(Size(visibleSize.width-30, 1));
                cell->addChild(line);

                
                break;
            case 2:
                edit3 = edit;
                cell->setPosition(Vec2(0, H - 30 - 180*2));
                title->setString("确认密码");
                edit->setPlaceHolder("请确认新密码");
                
                line = Layout::create();
                line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                line->setBackGroundColor(Color3B(0, 0, 0));
                line->setPosition(Vec2(0, 0));
                line->setContentSize(Size(visibleSize.width, 2));
                cell->addChild(line);

            break;
            }
    }

    Button *sureBtn = Button::create("Mine/btn_yellow_big.png","Mine/btn_yellow_big_check.png");
    sureBtn->setScale9Enabled(true);
    sureBtn->setContentSize(Size(960, 130));
    sureBtn->setPosition(Vec2(visibleSize.width/2, H - 700));
    this->addChild(sureBtn);
    sureBtn->setTitleText("确认修改");
    sureBtn->setTitleFontSize(40);
    sureBtn->setTitleColor(Color3B(0x65, 0x40, 0x02));
    sureBtn->setTitleFontName("");
    sureBtn->addTouchEventListener(CC_CALLBACK_2(ChangePassWordLayer::finishOn, this));
    
    return true;
}


//修改密码
void ChangePassWordLayer::finishOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        //确认修改密码
        std::string s1 = edit1->getText();
        std::string s2 = edit2->getText();
        std::string s3 = edit3->getText();
        
        //旧密码加密的
        const char* s1C = s1.c_str();
        s1C = PlatformHelper::encryptWithString(s1C);
        char s1Temp[300];
        int k = 0;
        for (int i = 0; i < strlen(s1C); i++)
        {
            if (s1C[i] == '+')
            {
                s1Temp[k++] = '%';
                s1Temp[k++] = '2';
                s1Temp[k++] = 'B';
            }else
            {
                s1Temp[k++] = s1C[i];
            }
        }
        s1Temp[k] = '\0';
        
        //新密码加密的
        const char* s2C = s2.c_str();
        s2C = PlatformHelper::encryptWithString(s2C);
        char s2Temp[300];
        k = 0;
        for (int i = 0; i < strlen(s2C); i++)
        {
            if (s2C[i] == '+')
            {
                s2Temp[k++] = '%';
                s2Temp[k++] = '2';
                s2Temp[k++] = 'B';
            }else
            {
                s2Temp[k++] = s2C[i];
            }
        }
        s2Temp[k] = '\0';
        
        if (s2 != s3)
        {
            PlatformHelper::showToast("请确认两次新密码一致");
        }else if(ZJHModel::getInstance()->pwd == s1)
        {
            PlatformHelper::showToast("密码错误");
        }else
        {
            Json::Value json;
            json["oldPassword"] =  s1Temp;
            json["newPassword"] =  s2Temp;
            
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
                    ZJHModel::getInstance()->setPhoneAndPwd(ZJHModel::getInstance()->phone, s2Temp);
                    
                    this->setPreByName("SettingLayer");
                    this->disappear();
                    
                    PlatformHelper::showToast("密码修改成功");
                    
                }else
                {
                    PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                }
            },"account/updatePwd",json.toStyledString(),"updatePwd");
        }
    }
    
}