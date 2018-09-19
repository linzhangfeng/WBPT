//
//  PaymentView.cpp
//  ZJH
//
//  Created by zhangzhenhua on 16/8/3.
//
//

#include "PaymentView.h"
bool PaymentView::init(){
    if(!Layout::init()){
        return false;
    }
    Size winSize = Director::getInstance()->getWinSize();
    this->setTouchEnabled(true);
    this->setContentSize(winSize);
    
    LayerColor* clo = LayerColor::create(Color4B(26,26,26,255));
    clo->setContentSize(Size(winSize.width,1200));
    
    this->addChild(clo);
    this->addClickEventListener([=](Ref *ref){
        this->removeFromParentAndCleanup(true);
    });
    
    float scalex = Utils::getScaleX();
    adimage_height = 300*scalex;
    selectTag = 1;
    _pay = ZJHModel::getInstance()->gameConfigJson["list"]["payTypeFilter"];
    return true;
}

ListView* PaymentView::getpayLayout(){
    Size winSize = Director::getInstance()->getWinSize();
    ListView* listView = ListView::create();
    listView->setDirection(ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(winSize.width, 700));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 215));
    listView->setItemsMargin(0);
    listView->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    for (int i = 0; i < 3; i++)
    {
        auto layout = Layout::create();
        layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        layout->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
        listView->addChild(layout);
        Layout *line;
        if (i == 0)
        {
            layout->setContentSize(Size(winSize.width, 85));
            layout->setTouchEnabled(false);
            
            line = Layout::create();
            line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
            line->setBackGroundColor(Color3B(0x30, 0x30, 0x31));
            line->setContentSize(Size(winSize.width, 3));
            layout->addChild(line);
            
            ImageView *icon2 = ImageView::create("Mine/shop_icon_3.png");
            icon2->setAnchorPoint(Vec2(0, 0.5));
            icon2->setPosition(Vec2(45, layout->getContentSize().height/2));
            layout->addChild(icon2);
            
            Text *text1 = Text::create("请选择支付方式", "", 36);
            text1->setAnchorPoint(Vec2(0, 0.5));
            text1->setPosition(Vec2(115, 40));
            text1->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
            text1->setTextColor(Color4B(0x99,0x99,0x99,255));
            layout->addChild(text1);
        }else
        {
            if(i == 1){
                if(!_pay["wechat"].asBool()){
                    continue;
                }
            }else if(i == 2){
                if(!_pay["alipay"].asBool()){
                    continue;
                }
            }else if(i == 3){
                if(!_pay["union"].asBool()){
                    continue;
                }
            }
            
            layout->setContentSize(Size(winSize.width, 180));
            layout->setTouchEnabled(true);
            layout->addTouchEventListener(CC_CALLBACK_2(PaymentView::selectPayFun, this));
            if(i != 3)
            {
                line = Layout::create();
                line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                line->setBackGroundColor(Color3B(0x30, 0x30, 0x31));
                line->setPosition(Vec2(40, 0));
                line->setContentSize(Size(winSize.width, 3));
                layout->addChild(line);
            }
            
            layout->setTag(i);
            
            ImageView *icon;
            Text *text = Text::create("", "", 40);
            text->setTextColor(Color4B(0x99, 0x99, 0x99, 255));
            
            ImageView *seleImage1 = ImageView::create("Mine/choice_icon.png");
            seleImage1->setPosition(Vec2(winSize.width - 50, layout->getContentSize().height/2));
            layout->addChild(seleImage1);
            seleImage1->setTag(100);
            
            ImageView *seleImage2 = ImageView::create("Mine/choice_icon_check.png");
            seleImage2->setPosition(Vec2(winSize.width - 50, layout->getContentSize().height/2));
            layout->addChild(seleImage2);
            seleImage2->setTag(200);
            seleImage2->setVisible(false);
            
            switch (i)
            {
                case 1:
                    selectPay = layout;
                    seleImage1->setVisible(false);
                    seleImage2->setVisible(true);
                    
                    icon = ImageView::create("Mine/pay_icon1.png");
                    text->setString("微信支付");
                    break;
                case 2:
                    icon = ImageView::create("Mine/pay_icon2.png");
                    text->setString("支付宝");
                    break;
                case 3:
                    icon = ImageView::create("Mine/pay_icon3.png");
                    text->setString("银联支付");
                    break;
                    
            }
            icon->setAnchorPoint(Vec2(0, 0.5));
            icon->setPosition(Vec2(40, layout->getContentSize().height/2));
            layout->addChild(icon);
            
            text->setAnchorPoint(Vec2(0, 0.5));
            text->setPosition(Vec2(170, icon->getPositionY()));
            layout->addChild(text);
        }
    }
    return listView;
}

void PaymentView::selectPayFun(Ref *pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED == type)
    {
        Layout *layout = (Layout *)pSender;
        selectTag = layout->getTag();
        if (layout != selectPay)
        {
            ImageView * image1 = (ImageView*)selectPay->getChildByTag(100);
            ImageView * image2 = (ImageView*)selectPay->getChildByTag(200);
            image1->setVisible(true);
            image2->setVisible(false);
            
            selectPay = layout;
            image1 = (ImageView*)selectPay->getChildByTag(100);
            image2 = (ImageView*)selectPay->getChildByTag(200);
            image1->setVisible(false);
            image2->setVisible(true);
        }
    }
}

void PaymentView::initDataMessage(Json::Value mValue){
    Size winSize = Director::getInstance()->getWinSize();
    
    ImageView *image = ImageView::create();
    image->loadTexture("Hall/Adtop.png");
    image->setAnchorPoint(Vec2(0,1));
    image->setContentSize(Size(winSize.width,adimage_height));
    image->setPosition(Vec2(0, winSize.height-(winSize.height - 1200)-50));
    image->ignoreContentAdaptWithSize(false);
    this->addChild(image);
    
    string imageUrl = mValue["imageUrl"].asString();
    if (Utils::hasImageFile(imageUrl)) {
        image->loadTexture(Utils::getImageSavePath(imageUrl));
    } else {
        image->loadTexture("Hall/pic_default2.png");
        CCHttpAgent::getInstance()->sendHttpImage(imageUrl, Utils::getImageSavePath(imageUrl), Utils::getImageSavePath(imageUrl), [=](std::string tag)
                                                  {
                                                      CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                      if (packet) {
                                                          CCHttpAgent::getInstance()->packets.erase(tag);
                                                          delete packet;
                                                      }
                                                      
                                                      if (image == NULL || image->getReferenceCount() <= 0 || image->getReferenceCount() > 10) return;
                                                      
                                                      if(Utils::hasImageFile(imageUrl))
                                                      {
                                                          image->loadTexture(Utils::getImageSavePath(imageUrl));
                                                          
                                                      }
                                                  });
    }
    
    Layout*  lays = getpayLayout();
    lays->setAnchorPoint(Vec2(0,1));
    lays->setPosition(Vec2(0, winSize.height-(winSize.height - 1200)-50-adimage_height));
    this->addChild(lays);
    
    
    Button* button = Button::create("Hall/button/btn_yellow_small.png","Hall/button/btn_yellow_big.png","Hall/button/btn_yellow_small_check.png");
    button->setContentSize(Size(winSize.width-200,120));
    button->setAnchorPoint(Vec2(0.5, 0.5));
    button->setScale9Enabled(true);
    button->setTitleText("立即支付");
    button->setTitleFontSize(50);
    button->setPosition(Vec2(winSize.width/2,120));
    button->addClickEventListener([=](Ref*){
        if(selectTag ==1){
            PlatformHelper::showToast("微信");
        }else if(selectTag == 2){
            PlatformHelper::showToast("支付宝");
        }else if(selectTag == 3){
            PlatformHelper::showToast("银联");
        }else{
            PlatformHelper::showToast("未知");
        }
        
    });
    this->addChild(button);
    
}
