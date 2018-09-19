//
//  FeedBacklayer.cpp
//  ZJH
//
//  Created by apple on 16/7/27.
//
//

#include "FeedBacklayer.hpp"
#include "MaskSelectLayer.hpp"

void FeedBacklayer::addImageFun(Ref*ref)
{
    __String* str = (__String*)ref;
    
    if (str != NULL)
    {
        std::string s = str->getCString();
        imagepath = s;
        
        
        if (tagIndex == 0)
        {
            image1->loadTexture(s);
            image1->setVisible(true);
        }else if (tagIndex == 1)
        {
            image2->loadTexture(s);
            image2->setVisible(true);
        }else if (tagIndex == 2)
        {
            image3->loadTexture(s);
            image3->setVisible(true);
        }
    }
}
bool FeedBacklayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(FeedBacklayer::addImageFun), "image_notify", NULL);
    
    setTitle("意见反馈");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();
    
    setBackground(Color4B(0x14, 0x14, 0x15, 255));
    
    Text *text1 = Text::create("编辑反馈意见", "", 40);
    text1->setAnchorPoint(Vec2(0, 1));
    text1->setTextColor(Color4B(0xb3, 0xb3, 0xb3, 255));
    text1->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    text1->setPosition(Vec2(30, H - 35));
    this->addChild(text1);
    
    auto cell1 = Layout::create();
    cell1->ignoreAnchorPointForPosition(false);
    cell1->setAnchorPoint(Vec2(0, 1));
    cell1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    cell1->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    cell1->setContentSize(Size(visibleSize.width, 460));
    cell1->setPosition(Vec2(30, H - 110));
    this->addChild(cell1);
    
    
    TextField* textField = TextField::create("","", 40);
    textField->ignoreContentAdaptWithSize(false);
    ((Label*)(textField->getVirtualRenderer()))->setLineBreakWithoutSpace(true);
    textField->setAnchorPoint(Vec2(0, 0.5));
    textField->setPosition(Vec2(35, cell1->getContentSize().height/2));
    textField->setContentSize(Size(cell1->getContentSize().width - 70, 400));
    textField->setTextHorizontalAlignment(TextHAlignment::LEFT);
    textField->setTextVerticalAlignment(TextVAlignment::TOP);
    cell1->addChild(textField);

    opption = textField;
    
    Text *text2 = Text::create("上传图片（若是软件使用问题，可以上传使用问题截图）", "", 40);
    text2->setAnchorPoint(Vec2(0, 1));
    text2->setTextColor(Color4B(0xb3, 0xb3, 0xb3, 255));
    text2->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    text2->setPosition(Vec2(30, H - 605));
    this->addChild(text2);
    
    auto cell2 = Layout::create();
    cell2->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    cell2->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    cell2->setContentSize(Size(visibleSize.width, 280));
    cell2->setPosition(Vec2(30, 770));
    this->addChild(cell2);
    
    float w = (visibleSize.width - 160)/3;
    
    for(int i = 0; i < 3; i++)
    {
        Button* btn1 = Button::create("Mine/more_add.png","Mine/more_add.png");
        btn1->setTag(100+i);
        btn1->setAnchorPoint(Vec2(0, 0.5));
        btn1->setPosition(Vec2(80 + w*i, cell2->getContentSize().height/2));
        btn1->addClickEventListener(CC_CALLBACK_1(FeedBacklayer::clickImageFun, this));
        cell2->addChild(btn1);
        
        ImageView *ima = ImageView::create();
        ima->cocos2d::ui::Widget::ignoreContentAdaptWithSize(false);
        ima->setContentSize(Size(200, 197));
        ima->setPosition(Vec2(btn1->getContentSize().width/2, btn1->getContentSize().height/2));
        btn1->addChild(ima);
        switch (i) {
            case 0:
                image1 = ima;
                image1->setVisible(false);
                break;
            case 1:
                image2 = ima;
                image2->setVisible(false);
                break;
            case 2:
                image3 = ima;
                image3->setVisible(false);
                break;
        }
        
    }
    
    Text *text3 = Text::create("您的联系方式（非必填）", "", 40);
    text3->setAnchorPoint(Vec2(0, 0));
    text3->setTextColor(Color4B(0xb3, 0xb3, 0xb3, 255));
    text3->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    text3->setPosition(Vec2(30, 715));
    this->addChild(text3);
    
    for (int i = 0; i < 2; i++)
    {
        auto cell = Layout::create();
        cell->ignoreAnchorPointForPosition(false);
        cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        cell->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
        cell->setContentSize(Size(visibleSize.width, 130));
        cell->setPosition(Vec2(0, 405 + (cell->getContentSize().height + 20)*i));
        this->addChild(cell);
        
        Text *title = Text::create("", "", 40);
        title->setPosition(Vec2(50, cell->getContentSize().height/2));
        title->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
        title->setAnchorPoint(Vec2(0, 0.5));
        title->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        cell->addChild(title);
        
        EditBox* edit = EditBox::create(Size(745, 120), "Mine/bar_sendtxt.png");
        edit->setPlaceholderFontSize(40);
        edit->setAnchorPoint(Vec2(0, 0.5));
        edit->setPosition(Vec2(220, cell->getContentSize().height/2));
        edit->setFontColor(Color3B(0x99, 0x99, 0x99));
        edit->setFontSize(40);
        cell->addChild(edit);
        switch (i)
        {
            case 0:
                qq = edit;
                edit->setInputMode(EditBox::InputMode::NUMERIC);
                title->setString("QQ号");
                break;
            case 1:
                email = edit;
                edit->setInputMode(EditBox::InputMode::EMAIL_ADDRESS);
                title->setString("邮箱");
                break;
        }

    }
    
    Button *but = Button::create("Mine/btn_yellow_big.png","Mine/btn_yellow_big_check.png");
    but->setScale9Enabled(true);
    but->setTitleText("提交反馈");
    but->setContentSize(Size(960, 130));
    but->setAnchorPoint(Vec2(0.5, 0));
    but->setPosition(Vec2(visibleSize.width/2, 40));
    this->addChild(but);
    but->setTitleFontSize(40);
    but->setTitleColor(Color3B(0x65, 0x40, 0x02));
    but->setTitleFontName("");
    but->addClickEventListener(CC_CALLBACK_1(FeedBacklayer::sureFun, this));
    
    return true;
    
}

void FeedBacklayer::sureFun(Ref *pSender)
{
    string opp = opption->getString();
    if(opp == "" && imagepath == "")
    {
        
        return;
    }
    Json::Value json;
    json["opinion"] = opption->getString();
    json["image"] = imagepath;
    json["qq"] = qq->getText();
    json["email"] = email->getText();
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
                                                 CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
                                                 if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
                                                 
                                                 if (loginPacket->status != 3)
                                                 {
                                                     PlatformHelper::showToast("网络链接失败，请稍后再试");
                                                     return;
                                                 }
                                                 if (loginPacket->resultIsOK())
                                                 {
                                                     PlatformHelper::showToast("提交成功！");
                                                     this->disappear();
                                                     
                                                 }else
                                                 {
                                                     PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                                                 }
                                             },"report/feedBack", json.toStyledString(),"feedBack");
    
}

void FeedBacklayer::clickImageFun(Ref *pSender)
{
    Button *btn = (Button *)pSender;
    tagIndex = btn->getTag() - 100;
    
    MaskSelectLayer *mask = MaskSelectLayer::create(1);
    mask->setPosition(Vec2::ZERO);
    this->addChild(mask);
}