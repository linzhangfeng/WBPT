//
//  MaskTipBuyGold.cpp
//  ZJH
//
//  Created by apple on 16/7/19.
//
//

#include "MaskTipBuyGold.hpp"
#include "../Match/JoinTipLayer.hpp"


#define CELLH           700
#define RIGHT_W         40

bool MaskTipBuyGold::init()
{
    if (!Layer::init())
    {
        return false;
    }

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
    
    return true;
}

void MaskTipBuyGold::showCreateRoom(Json::Value json)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(720, 470));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(bg);
    
    Button *cancel = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    cancel->setPosition(Vec2(bg->getContentSize().width, bg->getContentSize().height));
    cancel->addTouchEventListener(CC_CALLBACK_2(MaskTipBuyGold::exitOn, this));
    bg->addChild(cancel);

    Button *sureBtn = Button::create("Public/btn_yellow.png","Public/btn_yellow.png");
    sureBtn->setScale9Enabled(true);
    sureBtn->setContentSize(Size(610, 90));
    sureBtn->setPosition(Vec2(360, 70));
    bg->addChild(sureBtn);
    sureBtn->setTitleText("开通");
    sureBtn->setTitleFontSize(40);
    sureBtn->setTitleColor(Color3B(0x68, 0x41, 0x03));
    sureBtn->setTitleFontName("");
    sureBtn->addClickEventListener(CC_CALLBACK_1(MaskTipBuyGold::gotoOpenOn, this));
    
    int  roomType = json["roomType"].asInt();
    string tipTitle;
    if (roomType == 0)
    {
        tipTitle = "需要VIP才能创建私密房";
        sureBtn->setName("VIP");
    }else
    {
        tipTitle = "需要族长才能创建公开房";
        sureBtn->setName("族长");
    }
    Text *title = Text::create(tipTitle, "", 40);
    title->setTextColor(Color4B(0xd9, 0xd9, 0xd9, 255));
    title->setPosition(Vec2(360,240));
    bg->addChild(title);
    
    

}

void  MaskTipBuyGold::showView(Json::Value json, bool is)
{
    jsonData = json;
    isShoping = is;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto layout = Layout::create();
    layout->setSwallowTouches(true);
    layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    layout->setBackGroundColor(Color3B(255, 255, 255));
    layout->setContentSize(Size(visibleSize.width, CELLH));
    this->addChild(layout);
    
    Text *title = Text::create("购买金币", "", 40);
    title->setPosition(Vec2(layout->getContentSize().width/2, CELLH - 90));
    title->setTextColor(Color4B::BLACK);
    layout->addChild(title);
    
    ImageView *cancel = ImageView::create("Public/icon_cancel.png");
    cancel->setTouchEnabled(true);
    cancel->setAnchorPoint(Vec2(1, 0.5));
    cancel->addTouchEventListener(CC_CALLBACK_2(MaskTipBuyGold::exitOn, this));
    cancel->setPosition(Vec2(layout->getContentSize().width - RIGHT_W, title->getPositionY()));
    layout->addChild(cancel);

    auto lay = Layout::create();
    lay->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    lay->setBackGroundColor(Color3B::BLACK);
    lay->setPosition(Vec2(RIGHT_W, CELLH - 90*2 - 60));
    lay->setContentSize(Size(visibleSize.width - RIGHT_W *2, 3));
    this->addChild(lay);
    
    //    Json::Value json;
    //    json["nameStr"]
    //    json["goldenNum"]
    //    json["wanbei"]
    //    json["buyGoldNum"]
    //    json["currentPrice"]

    for (int i = 0; i < 3; i++)
    {
        Text *text;
        char buf[20];
        
        switch (i)
        {
            case 0:
                sprintf(buf, "%d", json["wanbei"].asInt());
                text = Text::create("我的钻石：" + (string)buf, "", 40);
                text->setPosition(Vec2(RIGHT_W, CELLH - 90*2));
                break;
            case 1:
                sprintf(buf, "%d", json["buyGoldNum"].asInt());
                text = Text::create("金币数量：" + (string)buf, "", 40);
                text->setPosition(Vec2(RIGHT_W, CELLH - 90*3-40));
                break;
            case 2:
                sprintf(buf, "%d", json["currentPrice"].asInt());
                text = Text::create("支付钻石：" + (string)buf, "", 40);
                text->setPosition(Vec2(RIGHT_W, CELLH - 90*4 - 20));
                break;
            default:
                break;
        }
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setTextColor(Color4B::BLACK);
        layout->addChild(text);
    }
    
    auto sure = Layout::create();
    sure->setPosition(Vec2(RIGHT_W, RIGHT_W));
    sure->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    sure->setBackGroundColor(Color3B::GREEN);
    sure->setContentSize(Size(visibleSize.width - RIGHT_W*2, 100));
    layout->addChild(sure);
    sure->setTouchEnabled(true);
    sure->addTouchEventListener(CC_CALLBACK_2(MaskTipBuyGold::sureOn, this));
    
    
    Text *sureTitle = Text::create("确认购买", "", 40);
    sureTitle->setPosition(Vec2(sure->getContentSize().width/2, sure->getContentSize().height/2));
    sure->addChild(sureTitle);
}

void MaskTipBuyGold::sureOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        Json::Value json;
        json["id"] = jsonData["id"];
        json["num"] = jsonData["buyGoldNum"];
        json["type"] = 0;
        
        //玩呗不足
        if (jsonData["currentPrice"].asInt() >  jsonData["wanbei"].asInt() && !isShoping)
        {
            //商城中不提示去商城
            Node *parent = this->getParent();
            JoinTipLayer *tip = JoinTipLayer::create();
            tip->setPosition(Vec2::ZERO);
            //错误状态码10002：玩贝不足
            tip->showMoneyBuyTip(10002, "钻石不足!", "是否前往钻石商城购买？");
            parent->addChild(tip);
            this->removeFromParent();
            return;
            
        }else if(isShoping)
        {
            //商城中提示玩贝不足
            PlatformHelper::showToast("钻石不足");
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
                PlatformHelper::showToast("购买金币成功");
                __NotificationCenter::getInstance()->postNotification("money_change");
                this->removeFromParent();
            }else
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
        },"store/buy/more",json.toStyledString(),"buy");
    }
}
void MaskTipBuyGold::gotoOpenOn(Ref *pSender)
{
    Button *btn = (Button *) pSender;
    if (btn->getName() == "VIP")
    {
        
    }else if(btn->getName() == "族长")
    {
        
    }
}
void MaskTipBuyGold::exitOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        this->removeFromParent();
    }
}
