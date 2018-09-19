//
//  TipChangeEditLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/23.
//
//

#include "TipChangeEditLayer.hpp"

TipChangeEditLayer* TipChangeEditLayer::create(int index, string text)
{
    TipChangeEditLayer *pRet = new(std::nothrow) TipChangeEditLayer();
    if (pRet && pRet->init(index, text))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool TipChangeEditLayer::init(int index, string text)
{
    if (!Layer::init())
    {
        return false;
    }
    nameStr = text;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    // 创建一个事件监听器类型为 OneByOne 的单点触摸
    auto listener1 = EventListenerTouchOneByOne::create();
    // 设置是否吞没事件，在 onTouchBegan 方法返回 true 时吞没
    listener1->setSwallowTouches(true);
    
    // 使用 lambda 实现 onTouchBegan 事件回调函数
    listener1->onTouchBegan = [](Touch* touch, Event* event) -> bool {
        // 获取事件所绑定的 target
        return true;
    };
    
    // 点击事件结束处理
    listener1->onTouchEnded = [=](Touch* touch, Event* event)
    {
        this->removeFromParent();
        
    };
    EventDispatcher * eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(720, 470));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(bg, 10);

    Button *cancel = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    cancel->setPosition(Vec2(bg->getContentSize().width, bg->getContentSize().height));
    cancel->addClickEventListener(CC_CALLBACK_1(TipChangeEditLayer::closeFun, this));
    bg->addChild(cancel);
    
    Text *title = Text::create("设置昵称", ".SFUIDisplay-Semibold", 40);
    title->setTextColor(Color4B(0xf1, 0xcb, 0x5b, 255));
    title->setPosition(Vec2(360, 410));
    bg->addChild(title);
    
    Button *sureBtn = Button::create("Public/btn_yellow.png","Public/btn_yellow.png");
    sureBtn->setScale9Enabled(true);
    sureBtn->setContentSize(Size(610, 90));
    sureBtn->setPosition(Vec2(360, 70));
    bg->addChild(sureBtn);
    sureBtn->setTitleText("确认");
    sureBtn->setTitleFontSize(40);
    sureBtn->setTitleColor(Color3B(0x68, 0x41, 0x03));
    sureBtn->setTitleFontName(".SFUIDisplay-Semibold");
    
    textField1 = EditBox::create(Size(610, 90), ui::Scale9Sprite::create("Public/window_txt_bar.png"));
    textField1->setPlaceholderFontName(".SFUIDisplay-Semibold");
    textField1->setPlaceholderFontSize(36);
    textField1->setInputMode(EditBox::InputMode::SINGLE_LINE);
    textField1->setPosition(Vec2(360, 235));
    textField1->setFontColor(Color3B(0x99, 0x99, 0x99));
    textField1->setFontName(".SFUIDisplay-Semibold");
    textField1->setPlaceholderFontColor(Color3B(0x4c, 0x4c, 0x4c));
    textField1->setFontSize(36);
    bg->addChild(textField1);
    
    //index == 1 修改昵称
    if (index == 1)
    {
        textField1->setMaxLength(12);
        if (text != "")
        {
            textField1->setPlaceHolder(text.c_str());
            
        }else
        {
            textField1->setPlaceHolder("好名字可以让你的朋友更容易记住你");
        }
        sureBtn->addClickEventListener(CC_CALLBACK_1(TipChangeEditLayer::sureFun, this));
        
    }else if(index == 2)
    {
        title->setString("设置个性签名");
        if (nameStr != "")
        {
            char buf[128];
            Utils::parseName(16, buf, nameStr.c_str());
            textField1->setPlaceHolder(buf);
            
        }else
        {
            textField1->setPlaceHolder("说出你的个性");
        }
        sureBtn->addClickEventListener(CC_CALLBACK_1(TipChangeEditLayer::qianMingFun, this));
    }else if (index == 4)
    {
        title->setString("设置俱乐部名称");
        textField1->setMaxLength(12);
        if (text != "")
        {
            textField1->setPlaceHolder(text.c_str());
            
        }else
        {
            textField1->setPlaceHolder("输出你的个性俱乐部名称");
        }
        sureBtn->addClickEventListener(CC_CALLBACK_1(TipChangeEditLayer::buluoNameFun, this));
    }
    
    
    return true;
}

void TipChangeEditLayer::qianMingFun(Ref*ref)
{
    //是否修改过
    string str = textField1->getText();
    if (nameStr == textField1->getText() || str == "")
    {
        this->removeFromParent();
        return;
    }else
    {
        if(str == "")
        {
            str = ZJHModel::getInstance()->signature;
        }
        Json::Value json;
        json["signature"] = str;
        
        CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
            
            CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
            
            if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)
            {
                return;
            }
            if (loginPacket->status != 3)
            {
                PlatformHelper::showToast("网络链接失败，请稍后再试");
                return;
            }
            
            if (loginPacket->resultIsOK())
            {
                Json::Value data = loginPacket->recvVal["resultMap"];
                //签名修改成功
                PlatformHelper::showToast("签名修改成功");
                ZJHModel::getInstance()->signature = textField1->getText();
                
                __NotificationCenter::getInstance()->postNotification("signature_change");
 
                this->removeFromParent();
            }else
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
        },"/account/updAccount",json.toStyledString(),"signature");
        
    }
}

void TipChangeEditLayer::buluoNameFun(Ref*ref)
{
    string str = textField1->getText();
    string strEmpty = replace(str);
    if(str != "" && strEmpty == ""){
        PlatformHelper::showToast("名字不能为空格");
        return;
    }
    //是否修改过
    if (nameStr == str || str == "")
    {
         this->removeFromParent();
        
    }else
    {
        if(str == "")
        {
            str = nameStr;
        }
        Json::Value json;
        json["name"] = str;
        
        CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
            
            CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
            
            if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)
            {
                return;
            }
            if (loginPacket->status != 3)
            {
                PlatformHelper::showToast("网络链接失败，请稍后再试");
                return;
            }
            
            if (loginPacket->resultIsOK())
            {
                Json::Value data = loginPacket->recvVal["resultMap"];
                //昵称修改成功
                PlatformHelper::showToast("名称修改成功");
                
                __NotificationCenter::getInstance()->postNotification("buluo_name_change", __String::create(str));
  
                this->removeFromParent();
            }else
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
        },"tribe/update/name",json.toStyledString(),"buluoname");
        
    }
}

void TipChangeEditLayer::sureFun(Ref*ref)
{
    string str = textField1->getText();
    string strEmpty = replace(str);
    if(str != "" && strEmpty == ""){
        PlatformHelper::showToast("昵称不能为空格");
        return;
    }
    //是否修改过
    if (nameStr == str || str == "")
    {
        this->removeFromParent();
        
    }else
    {
        if(str == "")
        {
            str = ZJHModel::getInstance()->nick;
        }
        Json::Value json;
        json["nickName"] = str;
        
        CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
            
            CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
            
            if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)
            {
                return;
            }
            if (loginPacket->status != 3)
            {
                PlatformHelper::showToast("网络链接失败，请稍后再试");
                return;
            }
            
            if (loginPacket->resultIsOK())
            {
                Json::Value data = loginPacket->recvVal["resultMap"];
                //昵称修改成功
                PlatformHelper::showToast("昵称修改成功");
                ZJHModel::getInstance()->nick = textField1->getText();
                __NotificationCenter::getInstance()->postNotification("nick_change");
                
                this->removeFromParent();
            }else
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
        },"/account/updAccount",json.toStyledString(),"nameChange");
        
    }
}

std::string TipChangeEditLayer::replace(std::string str){
    
    std::string::size_type startpos = 0;
    while (startpos!= std::string::npos)
    {
        startpos = str.find(' ');   //找到'.'的位置
        if( startpos != std::string::npos ) //std::string::npos表示没有找到该字符
        {
            str.replace(startpos,1,""); //实施替换，注意后面一定要用""引起来，表示字符串
        }
    }
    return str;
}

void TipChangeEditLayer::closeFun(Ref*ref)
{
    this->removeFromParent();
}
