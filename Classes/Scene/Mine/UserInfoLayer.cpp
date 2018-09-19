//
//  UserInfoLayer.cpp
//  wanMain
//
//  Created by apple on 16/6/29.
//
//

#include "UserInfoLayer.h"
#include "TipChangeEditLayer.hpp"
#include "MaskSelectLayer.hpp"
#include "HobbyLayer.hpp"
#include "SetCityLayer.hpp"

UserInfoLayer::UserInfoLayer()
{
    isBangWX = false;
}

bool UserInfoLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    
    setTitle("我的资料");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(UserInfoLayer::updateHobby), "hobby_change", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(UserInfoLayer::updatePortrait), "image_notify", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(UserInfoLayer::updateAdress), "adress_notify", NULL);

    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(UserInfoLayer::updateNick), "nick_change", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(UserInfoLayer::updateSigna), "signature_change", NULL);
    
    //1、获取玩家昵称和头像
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"account/userInfo","","userInfo");
    this->schedule(schedule_selector(UserInfoLayer::postData), 0.0f);
    
    loading2 = Loading::startLoading(this, Director::getInstance()->getWinSize() / 2);
    loading2->maskSwllowTouch();
    
        __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(UserInfoLayer::thirdWXPost), "wx_sysInfo", NULL);
    return true;
}

void UserInfoLayer::postData(float dt)
{
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["userInfo"];
    if (packet && packet->status != 0)
    {
        loading2->removeFromParent();
        unschedule(schedule_selector(UserInfoLayer::postData));
        
        if (packet->status != 3)
        {
            return;
        }
        packet->status = 0;
        
        if (packet->resultIsOK())
        {
            Json::Value data = packet->recvVal["resultMap"]["user"];
            userData = data;
            ZJHModel::getInstance()->signature =  data["signature"].asString();
            log("个人资料：＝＝＝ [%d]\n",data["sex"].asInt());
            showView(data);
            
            //获取微信的绑定状态
            checkBangWeiXinState();
        }
        else
        {
            PlatformHelper::showToast(packet->recvVal["message"].asCString());
        }
        
        unschedule(schedule_selector(UserInfoLayer::postData));
        CCHttpAgent::getInstance()->packets["userInfo"] = NULL;
        
        delete packet;
    }

}

void UserInfoLayer::updateAdress(Ref*ref)
{
    adressText->setString(ZJHModel::getInstance()->userCity);
}

void UserInfoLayer::updatePortrait(Ref*ref)
{
    auto size = Director::getInstance()->getVisibleSize();
    Loading *loading = Loading::startLoading(Director::getInstance()->getRunningScene(), size / 2);
    loading->maskSwllowTouch();
    //头像
    __String* str = (__String*)ref;
    if (str != NULL)
    {
        std::string s = str->getCString();
        
        //post image to server
        CCHttpAgent::getInstance()->sendHttpImagePost("image/uploadHeadImage", s, "portrait",[=](std::string tag){
            
            loading->removeFromParent();
            
            CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
            
            CCHttpAgent::getInstance()->packets.erase(tag);
            CCHttpAgent::getInstance()->callbacks.erase(tag);
            
            if (loginPacket->status != 3)
            {
                PlatformHelper::showToast("网络链接失败，请稍后再试");
                return;
            }
            
            loginPacket->status = 0;
            
            if (loginPacket->resultIsOK())
            {
                Json::Value data = loginPacket->recvVal["resultMap"];
                //imageUrl thumbUrl
                
                std::string userImage = data["thumbUrl"].asString();
                
                Data d = FileUtils::getInstance()->getDataFromFile(s);
                
                std::string savePath = Utils::getImageSavePath(userImage);
                CCHttpAgent::getInstance()->_makeMultiDir(savePath.c_str());
                FILE *fp = fopen(savePath.c_str(), "wb+");
                
                const unsigned char *buffer = d.getBytes();
                if (fp)
                {
                    fwrite(buffer, d.getSize(), 1, fp);
                    fclose(fp);
                }
                
                Sprite *userPortrit = (Sprite *)portraiCell->getChildByTag(100);
                Point point = userPortrit->getPosition();
                userPortrit->removeFromParent();
                
                Sprite *sp = Utils::createCircleAvatar(userImage, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(160, 160));
                sp->setTag(100);
                sp->setPosition(point);
                portraiCell->addChild(sp);
                
                PlatformHelper::showToast("头像上传成功");
                ZJHModel::getInstance()->portrit = userImage;
                __NotificationCenter::getInstance()->postNotification("userPotrait_change");
                
            }
            else
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
            
            delete loginPacket;
        });
        
    }

}

void UserInfoLayer::updateNick(Ref*ref)
{
    //nick
    char buf[128];
    
    Utils::parseName(20, buf, ZJHModel::getInstance()->nick.c_str());
    nickText->setString(buf);
    
}

void UserInfoLayer::updateSigna(Ref*ref)
{
    char buf[128];
    
    //signature
    Utils::parseName(20, buf, ZJHModel::getInstance()->signature.c_str());
    signatureText->setString(buf);

}
void UserInfoLayer::updateHobby(Ref*ref)
{
    char buf[128];
    //hobby
    string hobbyStr;
    for (int i = 0; i < ZJHModel::getInstance()->hobby.size(); i++)
    {
        hobbyStr += ZJHModel::getInstance()->hobby[i] + " ";
    }
    Utils::parseName(20, buf, hobbyStr.c_str());
    hobbyText->setString(buf);
}

void UserInfoLayer::sysWeiXinInfo(Ref*ref)
{
    wx_tongbu->setVisible(false);
    wx_tongbuing->setVisible(true);
    PlatformHelper::weiXin_sysInfo();
}

void UserInfoLayer::thirdWXPost(Ref* pSender)
{
        __String *dic = (__String*)pSender;
        string code = dic->getCString();
        //微信登录请求
        Json::Value json2;
        json2["code"] = code;
    
        CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
    
            CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
            if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
            wx_tongbuing->setVisible(false);
            wx_tongbuok->setVisible(true);
            
            if (loginPacket->status != 3)
            {
                PlatformHelper::showToast("网络链接失败，请稍后再试");
                return;
            }
    
            if (loginPacket->resultIsOK())
            {
                Json::Value data = loginPacket->recvVal["resultMap"];
    
                ZJHModel::getInstance()->nick = data["nickName"].asString();
                ZJHModel::getInstance()->name = data["nickName"].asString();
                ZJHModel::getInstance()->portrit = data["thumbImage"].asString();
                
                __NotificationCenter::getInstance()->postNotification("userPotrait_change");
                __NotificationCenter::getInstance()->postNotification("nick_change");
                
                Sprite *userPortrit = (Sprite *)portraiCell->getChildByTag(100);
                Point point = userPortrit->getPosition();
                userPortrit->removeFromParent();
                
                Sprite *sp = Utils::createCircleAvatar(ZJHModel::getInstance()->portrit, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(160, 160));
                sp->setTag(100);
                sp->setPosition(point);
                portraiCell->addChild(sp);
                
                char buf[128];
                Utils::parseName(20, buf, ZJHModel::getInstance()->nick.c_str());
                nickText->setString(buf);
                
            }else
            {
                
            }
        }, "weixin/authorizeIF", json2.toStyledString(), "sysWeiXinInfo");
}

void UserInfoLayer::onExit()
{
    BaseLayer::onExit();
    CCHttpAgent::getInstance()->callbacks.erase("sysWeiXinInfo");
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

//检查绑定微信的状态
void UserInfoLayer::checkBangWeiXinState()
{
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            isBangWX = loginPacket->recvVal["resultMap"]["thirdBinded"]["wechat"].asBool();
            if (isBangWX)
            {
                portraiCell->setTouchEnabled(false);
                portraiArrow->setVisible(false);
                nickCell->setTouchEnabled(false);
                nickArrow->setVisible(false);
                portraiText->setString("");
                
//                wx_tongbu->setVisible(true);
            }
        }
    },"account/getthirdbind", "", "isbindwx");
}

void UserInfoLayer::showView(Json::Value data)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();
    
    ListView* listView1 = ListView::create();
    listView1->setDirection(ScrollView::Direction::VERTICAL);
    listView1->setBounceEnabled(false);
    listView1->setContentSize(Size(visibleSize.width, H - 213));
    listView1->setAnchorPoint(Vec2(0, 0));
    listView1->setPosition(Vec2(0, 213));
    listView1->setItemsMargin(25);
    listView1->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(listView1);

    Size cellSize = Size(visibleSize.width, 130);
    for (int i = 0; i < 8; i++)
    {
        auto cell = Layout::create();
        cell->setTouchEnabled(true);
        cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        cell->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
        cell->setContentSize(cellSize);
        listView1->addChild(cell);
        
        Text *left = Text::create("", ".SFUIDisplay-Semibold", 36);
        left->setAnchorPoint(Vec2(0, 0.5));
        left->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        left->setTextColor(Color4B(0xa6, 0xa6, 0xa6, 255));
        
        Text *rightT2 = Text::create("", ".SFUIDisplay-Semibold", 36);
        rightT2->setAnchorPoint(Vec2(1.0, 0.5));
        rightT2->setTextHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
        rightT2->setTextColor(Color4B(0x61, 0x61, 0x61, 255));
        
        char buf[200];
        
        switch (i)
        {
            case 0:
            {
                portraiCell = cell;
                portraiText = rightT2;
                rightT2->setString("设置");
                cell->setContentSize(Size(visibleSize.width, 200));
                std::string userImage = data["thumbImage"].asString();
                
                Sprite* portrit = Utils::createCircleAvatar(userImage, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(160,160));
                portrit->setTag(100);
                portrit->setPosition(140, cell->getContentSize().height/2);
                cell->addChild(portrit);
                cell->addTouchEventListener(CC_CALLBACK_2(UserInfoLayer::portraitFun, this));
                
                //微信同步
                {
                    wx_tongbu = Button::create("Mine/ziliao/btn.png","Mine/ziliao/btn.png");
                    wx_tongbu->setContentSize(Size(300,100));
                    wx_tongbu->setScale9Enabled(true);
                    wx_tongbu->setPosition(Vec2(cellSize.width - 180, cell->getContentSize().height/2));
                    wx_tongbu->setZoomScale(0.1);
                    wx_tongbu->setPressedActionEnabled(true);
                    wx_tongbu->setVisible(false);
                    wx_tongbu->addClickEventListener(CC_CALLBACK_1(UserInfoLayer::sysWeiXinInfo, this));
                    cell->addChild(wx_tongbu);
                    
                    auto title = Text::create("同步微信资料", "Thonburi", 40);
                    title->setColor(Color3B(120,85, 34));
                    title->setAnchorPoint(Vec2(0.5, 0.5));
                    title->setPosition(Vec2(wx_tongbu->getContentSize().width / 2, wx_tongbu->getContentSize().height / 2));
                    wx_tongbu->addChild(title);
                }
                
                //微信同步中
                {
                    wx_tongbuing = Button::create("Mine/ziliao/btn2.png","Mine/ziliao/btn2.png");
                    wx_tongbuing->setPosition(Vec2(cellSize.width - 180, cell->getContentSize().height/2));
                    wx_tongbuing->setScale9Enabled(true);
                    wx_tongbuing->setContentSize(Size(300,100));
                    wx_tongbuing->setEnabled(false);
                    wx_tongbuing->setVisible(false);
                    cell->addChild(wx_tongbuing);
                    
                    auto updateing = Sprite::create("Mine/ziliao/updateing.png");
                    updateing->setPosition(Vec2(wx_tongbuing->getContentSize().width / 2 - 50, wx_tongbuing->getContentSize().height / 2));
                    ActionInterval* actionBy = RotateBy::create(3, 360);
                    updateing->runAction(RepeatForever::create(actionBy));
                    wx_tongbuing->addChild(updateing);

                    auto title = Text::create("同步中", "Thonburi", 40);
                    title->setColor(Color3B(104,104, 104));
                    title->setAnchorPoint(Vec2(0.5, 0.5));
                    title->setPosition(Vec2(wx_tongbuing->getContentSize().width / 2 + 30, wx_tongbuing->getContentSize().height / 2));
                    wx_tongbuing->addChild(title);
                }
                
                //微信同步成功
                {
                    wx_tongbuok = Button::create("Mine/ziliao/btn2.png","Mine/ziliao/btn2.png");
                    wx_tongbuok->setPosition(Vec2(cellSize.width - 161, cell->getContentSize().height/2));
                    wx_tongbuok->setContentSize(Size(300,100));
                    wx_tongbuok->setScale9Enabled(true);
                    wx_tongbuok->setEnabled(false);
                    wx_tongbuok->setVisible(false);
                    cell->addChild(wx_tongbuok);
                    
                    auto title = Text::create("同步成功", "Thonburi", 45);
                    title->setColor(Color3B(104,104, 104));
                    title->setAnchorPoint(Vec2(0.5, 0.5));
                    title->setPosition(Vec2(wx_tongbuok->getContentSize().width / 2, wx_tongbuok->getContentSize().height / 2));
                    wx_tongbuok->addChild(title);
                }
            }
                
                break;
            case 1:
            {
                ImageView * erWeiIcon = ImageView::create("Mine/view_qrcode.png");
                erWeiIcon->setAnchorPoint(Vec2(1, 0.5));
                erWeiIcon->setPosition(Vec2(visibleSize.width - 100, cell->getContentSize().height/2));
                cell->addChild(erWeiIcon);
                
                left->setString("我的二维码");
                cell->addTouchEventListener(CC_CALLBACK_2(UserInfoLayer::qrcodeFun, this));
                break;
            }
            case 2:
            {
                left->setString("昵称");
                Utils::parseName(20, buf, data["nickName"].asString().c_str());
                rightT2->setString(buf);
                nickText = rightT2;
                //thirdtype ＝ 3:微信用户
                
                cell->addTouchEventListener(CC_CALLBACK_2(UserInfoLayer::nameFun, this));
                
                nickCell = cell;
                
            }
                break;
            case 3:
                left->setString("ID");
                char idC[10];
                sprintf(idC, "%d", data["id"].asInt());
                rightT2->setString(idC);
                break;
            case 4:
            {
                left->setString("性别");
                auto Bg = Layout::create();
                Bg->setTouchEnabled(true);
                Bg->addTouchEventListener((CC_CALLBACK_2(UserInfoLayer::sexFun, this)));
                Bg->setContentSize(Size(150, 74));
                Bg->setPosition(Vec2(visibleSize.width - 210, 25));
                cell->addChild(Bg);
                
                ImageView *manImage = ImageView::create("Mine/OFF.png");
                manImage->setPosition(Vec2(75, 37));
                Bg->addChild(manImage);
                manImage->setTag(101);
                
                ImageView *womanImage = ImageView::create("Mine/ON.png");
                womanImage->setPosition(Vec2(75, 37));
                Bg->addChild(womanImage);
                womanImage->setTag(102);
                
                Text *sexT = Text::create("", ".SFUIDisplay-Semibold", 36);
                sexT->setTag(103);
                sexT->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
                Bg->addChild(sexT, 10);
                
                int sex = data["sex"].asInt();
                
                if (sex == 1)
                {
                    //男
                    manImage->setVisible(true);
                    womanImage->setVisible(false);
                    sexT->setString("男");
                    sexT->setTextColor(Color4B(0xb3, 0xb3, 0xb3, 255));
                    sexT->cocos2d::Node::setPosition(90, 37);
                    
                }else if (sex == 2)
                {
                    //女
                    manImage->setVisible(false);
                    womanImage->setVisible(true);
                    sexT->setString("女");
                    sexT->setTextColor(Color4B(0x65, 0x41, 0x00, 255));
                    sexT->cocos2d::Node::setPosition(50, 37);
                }
                break;
            }
                
            case 5:
                signatureText = rightT2;
                left->setString("个性签名");
                if (data["signature"].asString() == "")
                {
                    rightT2->setString("未设置");
                }else
                {
                    Utils::parseName(20, buf, data["signature"].asString().c_str());
                    rightT2->setString(buf);
                }
                
                cell->addTouchEventListener(CC_CALLBACK_2(UserInfoLayer::selfTextFun, this));
                break;
            case 6:
            {
                ZJHModel::getInstance()->userCity = data["province"].asString() + " "+ data["city"].asString() + " "+ data["area"].asString();
                ZJHModel::getInstance()->provice = data["province"].asString();
                ZJHModel::getInstance()->city = data["city"].asString();
                ZJHModel::getInstance()->qu = data["area"].asString();
                
                left->setString("地区");
                if (data["city"].isNull())
                {
                    rightT2->setString("");
                }else
                {
                    if (data["province"].asString() == data["city"].asString())
                    {
                        rightT2->setString(data["city"].asString() + " " + data["area"].asString());
                    }else
                    {
                        rightT2->setString(data["province"].asString() + " "+ data["city"].asString() + " "+ data["area"].asString());
                    }
                    
                }
                adressText = rightT2;
                cell->addTouchEventListener(CC_CALLBACK_2(UserInfoLayer::homeFun, this));
            }
                break;
            case 7:
            {
                left->setString("喜欢玩什么");
                cell->addTouchEventListener(CC_CALLBACK_2(UserInfoLayer::likeFun, this));
                
                std::string hobbyData = data["hobby"].asString();
                const char *hobbyStr = hobbyData.c_str();
                const char * split = ",";
                char *p;
                std::string hobby;
                p = strtok((char *)hobbyStr, split);
                while(p!=NULL) {
                    hobby = hobby + p + "  ";
                    printf ("%s\n",p);
                    ZJHModel::getInstance()->hobby.push_back(p);
                    p = strtok(NULL,split);
                }
                Utils::parseName(20, buf, hobby.c_str());
                rightT2->setString(buf);
                hobbyText = rightT2;
                break;
            }
            default:
                break;
        }
        ImageView *rightArrow;
        if (i != 4 && i!=3)
        {
            rightArrow = ImageView::create("Public/detail_arrow.png");
            rightArrow->setPosition(Vec2(cellSize.width - 60 - rightArrow->getContentSize().width/2, cell->getContentSize().height/2));
            cell->addChild(rightArrow);
            
            if(i == 0)
            {
                portraiArrow = rightArrow;
            }else if(i == 2)
            {
                nickArrow = rightArrow;
            }
            
        }
        left->setPosition(Vec2(85, cell->getContentSize().height/2));
        cell->addChild(left);
        
        rightT2->setPosition(Vec2(visibleSize.width - 100, cell->getContentSize().height/2));
        cell->addChild(rightT2);
        
    }
   
}

void UserInfoLayer::portraitFun(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        MaskSelectLayer *mask = MaskSelectLayer::create(1);
        mask->setPosition(Vec2::ZERO);
        this->addChild(mask);
    }

}
void UserInfoLayer::qrcodeFun(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        
        ZJHModel::getInstance()->gotoView(VIEW_ER_WEI_MA, userData);
    }

}
void UserInfoLayer::nameFun(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        //昵称修改弹窗
        TipChangeEditLayer *tip = TipChangeEditLayer::create(1, ZJHModel::getInstance()->nick);
        tip->setPosition(Vec2::ZERO);
        this->addChild(tip);
    }

}
void UserInfoLayer::sexFun(Ref *pSender, Widget::TouchEventType type)
{
    Json::Value json;
    if (type == Widget::TouchEventType::ENDED)
    {
        Layout * Bg = (Layout*)pSender;
        ImageView *manImag = (ImageView *)Bg->getChildByTag(101);
        ImageView *womanImag = (ImageView *)Bg->getChildByTag(102);
        Text *sexT = (Text *)Bg->getChildByTag(103);
        manImag->setVisible(!manImag->isVisible());
        womanImag->setVisible(!womanImag->isVisible());

        if (manImag->isVisible())
        {
            sexT->setString("男");
            sexT->cocos2d::Node::setPosition(90, 37);
            sexT->setTextColor(Color4B(0xb3, 0xb3, 0xb3, 255));
            json["sex"] = 1;
            
        }else
        {
            sexT->setString("女");
            sexT->cocos2d::Node::setPosition(50, 37);
            sexT->setTextColor(Color4B(0x65, 0x41, 0x00, 255));
            json["sex"] = 2;
        }
        
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
                //性别修改成功
                PlatformHelper::showToast("性别修改成功");
                
            }
        },"/account/updAccount",json.toStyledString(),"sexChange");
        
    }
    
}
void UserInfoLayer::likeFun(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        ZJHModel::getInstance()->gotoView(VIEW_HOBBY);
    }

}
void UserInfoLayer::homeFun(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        SetCityLayer *city = SetCityLayer::create();
        city->setPosition(Vec2::ZERO);
        this->addChild(city);
    }

}
void UserInfoLayer::selfTextFun(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        //个性签名修改弹窗
        TipChangeEditLayer *tip = TipChangeEditLayer::create(2, ZJHModel::getInstance()->signature);
        tip->setPosition(Vec2::ZERO);
        this->addChild(tip);
    }

}


