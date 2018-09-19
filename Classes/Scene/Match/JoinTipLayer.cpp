//
//  JoinTipLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/13.
//
//

#include "JoinTipLayer.hpp"
#define CELLH      400
#define INTER_MAX        30
#define CELLH_PROP      500

bool JoinTipLayer::init()
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

//买道具的提示
void JoinTipLayer::buyPropView(Json::Value json)
{
    propJson = json;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(720, 475));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(bg);
    
    Button *cancel = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    cancel->setPosition(Vec2(bg->getContentSize().width, bg->getContentSize().height));
    cancel->addTouchEventListener(CC_CALLBACK_2(JoinTipLayer::exitOn, this));
    bg->addChild(cancel);
    
    Text *title = Text::create(propJson["name"].asString(), "", 40);
    title->setTextColor(Color4B(0xf1, 0xcb, 0x5b, 255));
    title->setPosition(Vec2(360, 410));
    bg->addChild(title);
    
    Button *sureBtn = Button::create("Public/btn_yellow.png","Public/btn_yellow.png");
    sureBtn->setScale9Enabled(true);
    sureBtn->setContentSize(Size(610, 90));
    sureBtn->setPosition(Vec2(360, 80));
    bg->addChild(sureBtn);
    sureBtn->setTitleText("购买");
    sureBtn->setTitleFontSize(40);
    sureBtn->setTitleColor(Color3B(0x68, 0x41, 0x03));
    sureBtn->setTitleFontName("");
    sureBtn->addTouchEventListener(CC_CALLBACK_2(JoinTipLayer::buyPropOn, this));
    
    ImageView *editeBg = ImageView::create("Mine/bar_sendtxt.png");
    editeBg->setScale9Enabled(true);
    editeBg->setContentSize(Size(610, 80));
    editeBg->setPosition(Vec2(bg->getContentSize().width/2, 290));
    bg->addChild(editeBg);
    
    Button *reduceBut = Button::create("Mine/prop_remove.png", "Mine/prop_remove.png");
    reduceBut->setTag(100);
    reduceBut->setPosition(Vec2(0, 40));
    reduceBut->setAnchorPoint(Vec2(0, 0.5));
    editeBg->addChild(reduceBut);
    reduceBut->addTouchEventListener(CC_CALLBACK_2(JoinTipLayer::numChange, this));
            
    Button *addBut = Button::create("Mine/prop_add.png", "Mine/prop_add.png");
    addBut->setTag(200);
    addBut->setPosition(Vec2(editeBg->getContentSize().width, 40));
    addBut->setAnchorPoint(Vec2(1, 0.5));
    editeBg->addChild(addBut);
    addBut->addTouchEventListener(CC_CALLBACK_2(JoinTipLayer::numChange, this));
    
    nums = 1;
    numsText = Text::create("", "", 36);
    numsText->setPosition(Vec2(editeBg->getContentSize().width/2, editeBg->getContentSize().height/2));
    numsText->setTextColor(Color4B(0xe5, 0x5d, 0x0e, 255));
    editeBg->addChild(numsText);
    
    priceText = Text::create("", "", 36);
    priceText->setTextColor(Color4B(0xe5, 0x5d, 0x0e, 255));
    priceText->setPosition(Vec2(bg->getContentSize().width/2, 175));
    bg->addChild(priceText);
    
    //初始化钱数的显示
    priceByTypeString();
}

void JoinTipLayer::showMoneyBuyTip(int states, string titleStr, string textStr)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    ImageView * layout = ImageView::create("Public/bar_window_yellow.png");
    layout->setTouchEnabled(true);
    layout->setSwallowTouches(true);
    layout->setScale9Enabled(true);
    layout->setContentSize(Size(visibleSize.width - 200, CELLH));
    layout->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(layout, 10);
    
    Button *cancel = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    cancel->setPosition(Vec2(layout->getContentSize().width, layout->getContentSize().height));
    cancel->addTouchEventListener(CC_CALLBACK_2(JoinTipLayer::exitOn, this));
    layout->addChild(cancel);
    
    Text *title;
    Text *text;
    switch (states)
    {
        case 10002:
            //玩贝不足，购买玩贝
        {
            title = Text::create(titleStr, "", 36);
            text = Text::create(textStr, "", 36);
            
            auto sure = Layout::create();
            sure->setTag(10002);
            sure->setTouchEnabled(true);
            sure->addTouchEventListener(CC_CALLBACK_2(JoinTipLayer::sureOn, this));
            sure->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
            sure->setBackGroundColor(Color3B(0, 255, 0));
            sure->ignoreAnchorPointForPosition(false);
            sure->setAnchorPoint(Vec2(0.5, 0));
            sure->setContentSize(Size(layout->getContentSize().width - INTER_MAX*2, 100));
            sure->setPosition(Vec2(layout->getContentSize().width/2, INTER_MAX));
            layout->addChild(sure);
            
            Text *text1 = Text::create("前往商城", "", 36);
            text1->setPosition(Vec2(sure->getContentSize().width/2, sure->getContentSize().height/2));
            text1->setTextColor(Color4B::WHITE);
            sure->addChild(text1);
        }
            break;
            
        default:
            break;
    }
    title->setTextColor(Color4B::BLACK);
    title->setPosition(Vec2(layout->getContentSize().width/2, CELLH - 90));
    layout->addChild(title);
    
    text->setTextColor(Color4B::BLACK);
    text->setPosition(Vec2(layout->getContentSize().width/2, layout->getContentSize().height/2));
    layout->addChild(text);
}

void JoinTipLayer::numChange(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        ImageView *image = (ImageView*)pSender ;
        int tag = image->getTag();
        if (tag == 100)
        {
            //-
            nums--;
            if (nums <= 0)
            {
                nums = 1;
            }
            
        }else if(tag == 200)
        {
            //+
            nums++;
            
        }
        
        //钱数显示的改变
        priceByTypeString();
    }
}

void JoinTipLayer::priceByTypeString()
{
    char buf[10];
    sprintf(buf, "%d", nums);
    numsText->setString(buf);
    
    if(propJson["payType"].asInt() == 1)
    {
        //金币
        sprintf(buf, "%.f", propJson["price"].asDouble()*nums);
        priceText->setString("需要"+ (string)buf + "金币");
        
    }else if(propJson["payType"].asInt() == 2)
    {
        //玩贝
        sprintf(buf, "%.f", propJson["price"].asDouble()*nums);
        priceText->setString("需要"+ (string)buf + "钻石");
    }
}

void JoinTipLayer::buyPropOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        if(propJson["payType"].asInt() == 1 && propJson["goldenNum"].asInt() < propJson["price"].asDouble()*nums)
        {
             PlatformHelper::showToast("金币数不足，请充值");
             this->removeFromParent();
            return;
        }else if(propJson["payType"].asInt() == 2 && propJson["wanbei"].asInt() < propJson["price"].asDouble()*nums)
        {
             PlatformHelper::showToast("钻石数不足，请充值");
             this->removeFromParent();
            return;
        }
        
        Json::Value json;
        json["id"] = propJson["id"];
        json["num"] = nums;
        json["type"] = 0;

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
                
                PlatformHelper::showToast("购买成功!");
                __NotificationCenter::getInstance()->postNotification("money_change");
                 this->removeFromParent();
                
            }else
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
        },"store/buy/more",json.toStyledString(),"buy");

    }
}
void JoinTipLayer::sureOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        Layout *layout = (Layout *)pSender;
        int tag = layout->getTag();
        
        switch (tag)
        {
            case 10002:
                //购买玩贝
               
                ZJHModel::getInstance()->gotoView(VIEW_WAN_SHOP);
                
                break;
        }
        
        this->removeFromParent();
    }
    
}
void JoinTipLayer::exitOn(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        this->removeFromParent();
    }
}
