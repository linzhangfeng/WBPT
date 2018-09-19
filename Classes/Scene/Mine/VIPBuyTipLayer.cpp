//
//  VIPBuyTipLayer.cpp
//  ZJH
//
//  Created by apple on 16/8/4.
//
//

#include "VIPBuyTipLayer.hpp"
#include "MaskSelectLayer.hpp"

bool VIPBuyTipLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();

    Layout *mask = Layout::create();
    mask->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    mask->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    mask->setBackGroundColorOpacity(150);
    mask->setContentSize(winSize);
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
    
    
    Button *cancel = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    cancel->setPosition(Vec2(winSize.width - 150, winSize.height - 300));
    cancel->addClickEventListener([=](Ref*ref){
    
        this->removeFromParent();
    });
    mask->addChild(cancel, 50);
    
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
            Json::Value data = loginPacket->recvVal["resultMap"]["vipStoreList"];
            log("VIP信息：＝＝＝ [%s]",data.toStyledString().c_str());
            showView(data);
        }
    },"vip/getVipStoreList","","VipStoreList");
    
    return true;
}

ImageView* VIPBuyTipLayer::createItem(Json::Value json, int type)
{
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setScale9Enabled(true);
     bg->setContentSize(Size(380, 620));
//    if (type == 1)
//    {
//       
//    }else if(type == 2)
//    {
//        bg->setContentSize(Size(380+50, 620+50));
//    }
    
    
    float itemW = bg->getContentSize().width;
    float itemH = bg->getContentSize().height;
    
    Text *text4_1 = Text::create(json["vipName"].asString(), ".SFUIDisplay-Semibold", 40);
    text4_1->setAnchorPoint(Vec2(0.5, 1));
    text4_1->setPosition(Vec2(itemW/2, itemH - 25));
    text4_1->setTextColor(Color4B(0xef, 0xcb, 0x5a, 255));
    bg->addChild(text4_1);
    
    Text *vipDes = Text::create(json["profile"].asString(), ".SFUIDisplay-Semibold", 32);
    vipDes->ignoreContentAdaptWithSize(false);
    vipDes->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    vipDes->setTextColor(Color4B(0xb3, 0xb3, 0xb3, 255));
    vipDes->setContentSize(Size(itemW - 30, 220));
    vipDes->setAnchorPoint(Vec2(0, 1));
    vipDes->setPosition(Vec2(30, itemH - 300));
    bg->addChild(vipDes);
    
    char buf[128];
    sprintf(buf, "%d元/%d天", json["price"].asInt(), json["effectiveTime"].asInt());
    
    Text *price = Text::create(buf, ".SFUIDisplay-Semibold", 32);
    price->setAnchorPoint(Vec2(0.5, 1));
    price->setPosition(Vec2(itemW/2 , itemH - 90));
    price->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    price->setTextColor(Color4B(0xef, 0xcb, 0x5a, 255));
    bg->addChild(price);
    
    ImageView *icon;
    
    Button *buyBtn = Button::create("Public/btn_yellow.png","Public/btn_yellow.png", "Public/btn_yellow.png");
    buyBtn->setScale9Enabled(true);
    buyBtn->setAnchorPoint(Vec2(0.5, 0));
    buyBtn->setContentSize(Size(320, 80));
    buyBtn->setPosition(Vec2(itemW/2, 35));
    bg->addChild(buyBtn);
    buyBtn->setTitleFontSize(40);
    buyBtn->setTitleColor(Color3B(0x68, 0x41, 0x03));
    buyBtn->setTitleFontName(".SFUIDisplay-Semibold");
    buyBtn->addClickEventListener([=](Ref*ref){
       
        Layout *parent = (Layout *)this->getParent();
        this->removeFromParent();
        MaskSelectLayer *vipLayer = MaskSelectLayer::create(2);
        vipLayer->setPosition(Vec2(0, 220));
        vipLayer->getVipJson(json);
        parent->addChild(vipLayer, 100);
    });
    
    if (json["vipLvId"] == 1)
    {
        buyBtn->setTitleText("加购");
        
        icon = ImageView::create("Mine/vip_icon.png");
        
        
        buyBtn->setEnabled(true);
        
    }else if (json["vipLvId"] == 2)
    {
        if (ZJHModel::getInstance()->vipLv >= 2)
        {
            buyBtn->setTitleText("加购");
        }else
        {
            buyBtn->setTitleText("升级");
        }
        icon = ImageView::create("Mine/rich_icon.png");
        
    }else if (json["vipLvId"] == 3)
    {
        if (ZJHModel::getInstance()->vipLv >= 3)
        {
            buyBtn->setTitleText("加购");
        }else
        {
            buyBtn->setTitleText("升级");
        }
        icon = ImageView::create("Mine/leader_icon.png");
    }
    
    icon->setAnchorPoint(Vec2(0.5, 1));
    icon->setContentSize(Size(170, 180));
    icon->ignoreContentAdaptWithSize(false);
    icon->setPosition(Vec2(itemW/2, itemH - 130));
    bg->addChild(icon);

    return bg;
}

void VIPBuyTipLayer::showView(Json::Value json)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    ui::ScrollView* scrollView = ui::ScrollView::create();
    scrollView->setContentSize(Size(visibleSize.width, 715));
    scrollView->setPosition(Vec2());
    scrollView->setAnchorPoint(Vec2(0.5, 0.5));
    scrollView->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
    scrollView->setBounceEnabled(true);
    scrollView->setScrollBarEnabled(false);
    this->addChild(scrollView, 20);
    
    scrollView->setInnerContainerSize(Size(250+380*3 + 50*2, 0));
    
    for (int i = 0; i < json.size(); i++)
    {
        ImageView *bg = createItem(json[i], 1);
        bg->setTag(100 + json[i]["vipLvId"].asInt());
        bg->setPosition(Vec2(300+(bg->getContentSize().width + 50)*i, scrollView->getContentSize().height/2));
        scrollView->addChild(bg);
        
        ImageView *bg2 = createItem(json[i], 2);
        bg2->setTag(200 + json[i]["vipLvId"].asInt());
        bg2->setPosition(bg->getPosition());
        scrollView->addChild(bg2);
        bg2->setVisible(false);
        
    }
    
    if (ZJHModel::getInstance()->vipLv <= 1)
    {
        pageNum = 1;
        
    }else
    {
        pageNum = ZJHModel::getInstance()->vipLv;
    }
    
    lastNum = pageNum;
    
    ImageView *item1 = (ImageView *)scrollView->getChildByTag(100+pageNum);
    ImageView *item2 = (ImageView *)scrollView->getChildByTag(200+pageNum);
    item1->setVisible(false);
    item2->setVisible(true);
    
    m_scrollView = scrollView;
    
    CallFunc* func = CallFunc::create([=]
                                      {
                                          scrollView->jumpToBottom();
                                      });
    
    
    runAction(Sequence::create(DelayTime::create(0.0),func, NULL));
    
    countX = abs(m_scrollView->getInnerContainer()->getPositionX());
    m_scrollView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType){

        
        
    });
    
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(VIPBuyTipLayer::TouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(VIPBuyTipLayer::TouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(VIPBuyTipLayer::TouchEnded, this);
    touchListener->setSwallowTouches(false);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
}

bool VIPBuyTipLayer::TouchBegan(Touch * touch, Event * pEvent)
{
    //用开始的触摸点和scroll的偏移量初始化以下的成员变量
    this->m_startPoint  = touch->getLocation();
    
    
    return true;
}


/*以下代码的整体含义就是当手指移动的时候，让关卡跟随手指移动，当移动结束的时候，判断结束点和开始
 触摸点的位置，对关卡的位置做相应的处理*/

//设置关卡跟随手指的方向移动
void VIPBuyTipLayer::TouchMoved(Touch * touch, Event * pEvent)
{
    printf("22222222222\n");
}

//以下的代码是重点，当结束触摸的时候，为了使关卡显示在屏幕的中间，我们需要这么做
void VIPBuyTipLayer::TouchEnded(Touch * touch, Event * pEvent)
{
    Point endPoint = touch->getLocation();
    float distance = endPoint.x - m_startPoint.x;
    
    //scrollViewDidScroll(distance);
}

void VIPBuyTipLayer::scrollViewDidScroll(float offset)
{
    Size winSize = Director::getInstance()->getWinSize();
    // 我们根据 offset 的实际情况来判断移动效果
    //如果手指往左划，offset大于0，说明页面在减小，往右增大
    
    //将偏移量作为参数传进来
    if (offset > 0)
    {
        pageNum++;
        if (pageNum > 3)
        {
            pageNum = 3;
        }
        if(pageNum == lastNum)
        {
            return;
        }
        ImageView *item1 = (ImageView *)m_scrollView->getChildByTag(100+pageNum);
        ImageView *item2 = (ImageView *)m_scrollView->getChildByTag(200+pageNum);
        item1->setVisible(false);
        item2->setVisible(true);
        
        ImageView *item1Last = (ImageView *)m_scrollView->getChildByTag(100+lastNum);
        ImageView *item2Last = (ImageView *)m_scrollView->getChildByTag(200+lastNum);
        item1Last->setVisible(true);
        item2Last->setVisible(false);
        
    }else if (offset < 0)
    {
        pageNum--;
        if (pageNum < 1)
        {
            pageNum = 1;
        }
        if(pageNum == lastNum)
        {
            return;
        }
        ImageView *item1 = (ImageView *)m_scrollView->getChildByTag(100+pageNum);
        ImageView *item2 = (ImageView *)m_scrollView->getChildByTag(200+pageNum);
        item1->setVisible(false);
        item2->setVisible(true);
        
        ImageView *item1Last = (ImageView *)m_scrollView->getChildByTag(100+lastNum);
        ImageView *item2Last = (ImageView *)m_scrollView->getChildByTag(200+lastNum);
        item1Last->setVisible(true);
        item2Last->setVisible(false);
        
    }

    lastNum = pageNum;
    
    //Point adjustPoint = Point(-430 * (pageNum - 1) , 0);
    //m_scrollView->getInnerContainer()->setPositionX(adjustPoint.x);
}
