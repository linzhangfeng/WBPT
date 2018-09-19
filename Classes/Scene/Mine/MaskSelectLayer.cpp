
//
//  MaskSelectLayer.cpp
//  wanMain
//
//  Created by apple on 16/6/29.
//
//

#include "MaskSelectLayer.hpp"
#include "VIPBuyTipLayer.hpp"

MaskSelectLayer* MaskSelectLayer::create(int index)
{
    MaskSelectLayer *pRet = new(std::nothrow) MaskSelectLayer();
    if (pRet && pRet->init(index))
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

void MaskSelectLayer::showVipBuyView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    //index == 2:买vip的页面
    
    //提交申请的按钮
    auto bg = Layout::create();
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    bg->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    if(vipJson["vipLvId"].asInt() == 2)
    {
        bg->setContentSize(Size(visibleSize.width, 1200));
    }else
    {
        bg->setContentSize(Size(visibleSize.width, 1150));
    }

    this->addChild(bg);
    
    Button *sureBtn = Button::create("Mine/btn_yellow_big.png","Mine/btn_yellow_big_check.png");
    sureBtn->setScale9Enabled(true);
    sureBtn->setContentSize(Size(960, 120));
    sureBtn->setAnchorPoint(Vec2(0.5, 0));
    sureBtn->setPosition(Vec2(visibleSize.width/2, 20));
    bg->addChild(sureBtn);
    sureBtn->setTitleText("确认");
    sureBtn->setTitleFontSize(40);
    sureBtn->setTitleColor(Color3B(0x65, 0x40, 0x02));
    sureBtn->setTitleFontName("");
    sureBtn->addClickEventListener([=](Ref*ref){
        if (PlatformHelper::sdkDoForInt_C("hasThirdChannel"))
        {
            ZJHModel::getInstance()->requestThirdPay(vipJson["sid"].asInt(), monthCount);
        }
        else
        {
            int tag = selectPay->getTag() - 300;
            //tag :0： 银行卡 1：微信支付 2:支付宝
            if (tag == 0)
            {
                return;
            }
            ZJHModel::getInstance()->requestPay(vipJson["sid"].asInt(), monthCount, tag);
        }
    });
    
    if (!PlatformHelper::sdkDoForInt_C("hasThirdChannel"))
    {
        Text *text4_1 = Text::create("支付方式", "", 36);
        text4_1->setAnchorPoint(Vec2(0, 0));
        text4_1->setPosition(Vec2(30, 450));
        text4_1->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        text4_1->setTextColor(Color4B(0xd9, 0xd9, 0xd9, 255));
        bg->addChild(text4_1);
        
        //3种支付方式
        for (int i = 0; i < 3; i++)
        {
            auto layout3 = Layout::create();
            layout3->ignoreAnchorPointForPosition(false);
            layout3->setAnchorPoint(Vec2(0.5, 0));
            layout3->setContentSize(Size(960, 90));
            layout3->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
            layout3->setBackGroundColor(Color3B(0x4b, 0x4b, 0x4b));
            layout3->setPosition(Vec2(visibleSize.width/2, 165 + 90*i));
            bg->addChild(layout3);
            
            layout3->setTag(300+i);
            layout3->setTouchEnabled(true);
            layout3->addTouchEventListener(CC_CALLBACK_2(MaskSelectLayer::selectPayFun, this));
            Text *text = Text::create("", "", 36);
            text->setTag(50);
            text->setTextColor(Color4B(0x23, 0x23, 0x23, 255));
            text->setPosition(Vec2(480, 45));
            layout3->addChild(text);
            if (i == 0)
            {
                //屏蔽银联支付
                layout3->setVisible(false);
                layout3->setEnabled(false);
            }else
            {
                //屏蔽银联支付
                layout3->setVisible(true);
                layout3->setEnabled(true);
            }
            
            switch (i)
            {
                case 0:
                    text->setString("银行卡");
                    
                    break;
                case 1:
                    text->setString("微信支付");
                    break;
                case 2:
                    text->setString("支付宝");
                    selectPay = layout3;
                    layout3->setBackGroundColor(Color3B(0x68, 0x68, 0x68));
                    text->setTextColor(Color4B(0xff, 0xff, 0xff, 255));
                    break;
            }
        }
    }

    for (int i = 0; i < 2; i++)
    {
        auto layout3 = Layout::create();
        layout3->setContentSize(Size(visibleSize.width, 135));
        layout3->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
        layout3->setBackGroundColor(Color3B(0x2a, 0x2a, 0x2a));
        layout3->setPosition(Vec2(0, 510 + 150*i));
        bg->addChild(layout3);
        
        Text *text = Text::create("", "", 36);
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setTextColor(Color4B(0x67, 0x67, 0x67, 255));
        text->setPosition(Vec2(65, 67));
        layout3->addChild(text);
        
        ImageView *editeBg = ImageView::create("Mine/bar_sendtxt.png");
        editeBg->setScale9Enabled(true);
        editeBg->setContentSize(Size(745, 80));
        editeBg->setAnchorPoint(Vec2(0, 0.5));
        editeBg->setPosition(Vec2(255, 67));
        layout3->addChild(editeBg);
        char s1[10];
        monthCount = 3;
        switch (i)
        {
            case 1:
            {
                reduceBut = Button::create("Mine/prop_remove.png", "Mine/prop_remove.png");
                reduceBut->setTag(150);
                reduceBut->setPosition(Vec2(256, 67));
                reduceBut->setAnchorPoint(Vec2(0, 0.5));
                layout3->addChild(reduceBut);
                reduceBut->addTouchEventListener(CC_CALLBACK_2(MaskSelectLayer::changeMonthTextFun, this));
                
                addBut = Button::create("Mine/prop_add.png", "Mine/prop_add.png");
                addBut->setTag(250);
                addBut->setPosition(Vec2(visibleSize.width - 80 , 67));
                addBut->setAnchorPoint(Vec2(1, 0.5));
                layout3->addChild(addBut);
                addBut->addTouchEventListener(CC_CALLBACK_2(MaskSelectLayer::changeMonthTextFun, this));
        
                sprintf(s1, "%d", monthCount);
                monthText = Text::create(s1, "", 36);
                monthText->setPosition(Vec2(372, 40));
                monthText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
                monthText->setTextColor(Color4B(0x33, 0x33, 0x33, 255));
                editeBg->addChild(monthText);

                text->setString("开通月份");
            }
                break;
            case 0:
                text->setString("支付金额");
                
                sprintf(s1, "%d元", monthCount*vipJson["price"].asInt());
                payMoney = Text::create(s1, "", 36);
                payMoney->setPosition(Vec2(372, 40));
                payMoney->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
                payMoney->setTextColor(Color4B(0x33, 0x33, 0x33, 255));
                editeBg->addChild(payMoney);
                
                break;
        }
    }
    
    Button *closeBtn = Button::create("Public/icon_cancel.png");
    closeBtn->setAnchorPoint(Vec2(1, 1));
    closeBtn->setZoomScale(1);
    closeBtn->setContentSize(Size(150, 150));
    closeBtn->ignoreContentAdaptWithSize(false);
    closeBtn->setFillContentSizeEnabled(false);
    closeBtn->setPosition(Vec2(visibleSize.width, bg->getContentSize().height));
    closeBtn->addClickEventListener([=](Ref *ref){
                                        this->removeFromParent();
    });
    bg->addChild(closeBtn);
    
    //购买的标题
    Text *payTitleTop = Text::create("购买" + vipJson["vipName"].asString(), "", 40);
    payTitleTop->setAnchorPoint(Vec2(0.5, 1));
    payTitleTop->setPosition(Vec2(visibleSize.width/2, bg->getContentSize().height - 25));
    payTitleTop->setTextColor(Color4B(0xe1, 0xcd, 0xad, 255));
    bg->addChild(payTitleTop);
    //
    Text *teTitle = Text::create(vipJson["vipName"].asString(), "", 36);
    teTitle->setPosition(Vec2(40, bg->getContentSize().height - 100));
    teTitle->setAnchorPoint(Vec2(0, 1));
    teTitle->setTextColor(Color4B(0xd9, 0xd9, 0xd9, 255));
    bg->addChild(teTitle);
    
    Text *desTitle = Text::create(vipJson["profile"].asString(), "", 32);
    desTitle->setPosition(Vec2(40, bg->getContentSize().height - 150));
    desTitle->setAnchorPoint(Vec2(0, 1));
    desTitle->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
    bg->addChild(desTitle);
    
}

bool MaskSelectLayer::init(int index)
{
    //index == 0 1 :性别 头像设置的选择
    
    //index == 3:邀请好友分享到的
    
    //index ＝＝ 2 VIP 购买
    
    auto visibleSize = Director::getInstance()->getVisibleSize();

    Layout *mask = Layout::create();
    mask->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    mask->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    mask->setBackGroundColorOpacity(150);
    mask->setContentSize(visibleSize);
    this->addChild(mask);
    
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
    
    if(index == 0 || index == 1)
    {
        auto layout = Layout::create();
        layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        layout->setBackGroundColor(Color3B(0x1c, 0x1c, 0x1c));
        layout->setContentSize(Size(visibleSize.width, 455));
        this->addChild(layout);
        
        for (int i = 0; i < 3; i++)
        {
            Button* btn;
            Text *title = Text::create("", "", 36);
            title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
            title->setTextColor(Color4B(255, 255, 255, 255));
            switch (i)
            {
                case 0:
                {
                    btn = Button::create("Public/btn_gray.png","Public/btn_gray.png");
                    btn->setContentSize(Size(960, 125));
                    btn->setPosition(Vec2(visibleSize.width/2, 76));
                    title->setString("取消");
                    btn->addTouchEventListener(CC_CALLBACK_2(MaskSelectLayer::closeFun, this));
                }
                    break;
                case 1:
                    btn = Button::create("Public/btn_yellow.png","Public/btn_yellow.png");
                    btn->setName("paiZhao");
                    btn->setContentSize(Size(960, 125));
                    btn->setPosition(Vec2(visibleSize.width/2, 225));
                    title->setString("拍照");
                    btn->addClickEventListener(CC_CALLBACK_1(MaskSelectLayer::portraitChange, this));
                    break;
                case 2:
                    btn = Button::create("Public/btn_yellow.png","Public/btn_yellow.png");
                    btn->setName("xiangCe");
                    btn->setContentSize(Size(960, 125));
                    btn->setPosition(Vec2(visibleSize.width/2, 360));
                    title->setString("从相册选取");
                    btn->addClickEventListener(CC_CALLBACK_1(MaskSelectLayer::portraitChange, this));
                    break;
                default:
                    break;
            }
            btn->setScale9Enabled(true);
            layout->addChild(btn);
            
            title->setPosition(Vec2(480, 125/2));
            btn->addChild(title);
        }
        
    }else if (index == 3)
    {
        //3:邀请分享的遮罩层
        auto layer1 = LayerColor::create(Color4B(0x10, 0x10, 0x10, 255), visibleSize.width, visibleSize.height);
        layer1->setPosition(Vec2(0, 0));
        
        _layer1 = layer1;
        this->addChild(layer1);
        showInviteShareMask(layer1);
    }else if (index == 2)
    {
        
    }
    
    return true;
}

void MaskSelectLayer::showInviteShareMask(LayerColor * layer)
{
    auto size = Director::getInstance()->getVisibleSize();
    
    auto layout1 = Layout::create();
    layout1->setTouchEnabled(true);
    layout1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    layout1->setBackGroundColor(Color3B(245, 245, 245));
    layout1->setPosition(Vec2(0, 0));
    layout1->setContentSize(Size(size.width, 90));
    layout1->addTouchEventListener(CC_CALLBACK_2(MaskSelectLayer::closeFun, this));
    layer->addChild(layout1);
    
    Text *title = Text::create("取消", "AmericanTypewriter", 22);
    title->setPosition(Vec2(size.width/2, 45));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setTextColor(Color4B(0, 0, 0, 255));
    layout1->addChild(title);
    
    auto layout2 = Layout::create();
    layout2->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    layout2->setBackGroundColor(Color3B(245, 245, 245));
    layout2->setPosition(Vec2(0, 3+90));
    layout2->setContentSize(Size(size.width, 300));
    layer->addChild(layout2);
    
    float w1 = (size.width - 30)/3;
    for (int i = 0; i < 6; i++)
    {
        char s[30];
        sprintf(s, "Mine/windows_share_icon%d.png", i+1);
        
        ImageView *icon = ImageView::create(s);
        icon->setScale(0.5);
        icon->setPosition(Vec2(15 + w1/2 + (i%3)*w1, 50 + (i/3)* 150));
        layout2->addChild(icon);
    }
    
}

void MaskSelectLayer::changeMonthTextFun(Ref *pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED == type)
    {
        Button* but = (Button *)pSender;
        if (but->getTag() == 150)
        {
            monthCount--;
            if (monthCount < 1)
            {
                monthCount = 1;
            }
            
        }else if(but->getTag() == 250)
        {
            monthCount++;
        }
        char s1[10];
        sprintf(s1, "%d", monthCount);
        monthText->setString(s1);
        sprintf(s1, "%d元", monthCount*vipJson["price"].asInt());
        payMoney->setString(s1);
    }
}

void MaskSelectLayer::selectPayFun(Ref *pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED == type)
    {
        Layout *layout = (Layout *)pSender;
        if (layout != selectPay)
        {
            layout->setBackGroundColor(Color3B(0x68, 0x68, 0x68));
            Text * text1 = (Text*)layout->getChildByTag(50);
            text1->setTextColor(Color4B(0xff, 0xff, 0xff, 255));
            
            selectPay->setBackGroundColor(Color3B(0x4b, 0x4b, 0x4b));
            Text * text2 = (Text*)selectPay->getChildByTag(50);
            text2->setTextColor(Color4B(0x23, 0x23, 0x23, 255));
            
            selectPay = layout;
        }
    }
}

void MaskSelectLayer::portraitChange(Ref *pSender)
{
    Button *btn = (Button *)pSender;
    
    if (btn->getName() == "paiZhao")
    {
        PlatformHelper::openCamera();
        
    }else if (btn->getName() == "xiangCe")
    {
        PlatformHelper::openSavedPhotosAlbum();
    }
    this->removeFromParent();
    
}

void MaskSelectLayer::closeFun(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        this->removeFromParent();
        
    }
}

void MaskSelectLayer::sexChangeFun(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        Layout *layout = (Layout *)pSender;
        int tag = layout->getTag() - 100;
        Json::Value json;
        json["sex"] = tag;
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
                //昵称修改成功
                printf("性别修改成功\n");
            }
        },"/account/updAccount",json.toStyledString(),"sexChange");
        
        this->removeFromParent();
        
    }else if (type == Widget::TouchEventType::BEGAN)
    {
        Layout * p = (Layout *)pSender;
        p->setBackGroundColor(Color3B(0xe9, 0xe7, 0xef));
    }
}

void MaskSelectLayer::getVipJson(Json::Value json)
{
    vipJson = json;
    showVipBuyView();
    
}
