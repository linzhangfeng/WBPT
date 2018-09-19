//
//  AboutOurLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/27.
//
//

#include "AboutOurLayer.hpp"
#include "VersionMessageDialog.hpp"

bool AboutOurLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("关于我们");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();
    
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    auto cell1 = Layout::create();
    cell1->ignoreAnchorPointForPosition(false);
    cell1->setAnchorPoint(Vec2(0, 1));
    cell1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    cell1->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    cell1->setContentSize(Size(visibleSize.width, 400));
    cell1->setPosition(Vec2(0, H));
    this->addChild(cell1);
    
    cell1->setTouchEnabled(true);
    cell1->addClickEventListener(CC_CALLBACK_1(AboutOurLayer::showDialog, this));
    
    Layout *line1 = Layout::create();
    line1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line1->setBackGroundColor(Color3B(0, 0, 0));
    line1->setPosition(Vec2(0, 0));
    line1->setContentSize(Size(visibleSize.width, 2));
    cell1->addChild(line1);

    
    Text *text1 = Text::create("关于我们", "", 40);
    text1->setAnchorPoint(Vec2(0, 1));
    text1->setTextColor(Color4B(0xe6, 0xe6, 0xe6, 255));
    text1->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    text1->setPosition(Vec2(40, cell1->getContentSize().height - 30));
    cell1->addChild(text1);

    
    ImageView *textBg = ImageView::create("Mine/bar_aboutOur.png");
    textBg->setContentSize(Size(visibleSize.width - 80, 280));
    textBg->setScale9Enabled(true);
    textBg->ignoreContentAdaptWithSize(false);
    textBg->setAnchorPoint(Vec2(0, 0));
    textBg->setPosition(Vec2(40, 30));
    cell1->addChild(textBg);

    auto text3 = LabelTTF::create("    玩呗是一款基于游戏，语言等多种方式的娱乐社交平台，你可以开个房间和朋友进行私密的互动娱乐，并且房主还能获得收益，加入玩呗和数百万人一起体验前所未有的娱乐的方式。", "", 40,
                                 Size(955, 280), TextHAlignment::LEFT);
    text3->setAnchorPoint(Vec2(0, 1));
    text3->setColor(Color3B(0xcb, 0xcb, 0xcb));
    text3->setPosition(Vec2(30, textBg->getContentSize().height - 30));
    textBg->addChild(text3);
    
//    Label *text3 = Label::create("    玩呗是一款基于游戏，语言等多种方式的娱乐社交平台，你可以开个房间和朋友进行私密的互动娱乐，并且房主还能获得收益，加入玩呗和数百万人一起体验前所未有的娱乐的方式。", ".SFUIDisplay-Semibold", 40);
//    text3->setMaxLineWidth(955);
    
    auto cell2 = Layout::create();
    cell2->ignoreAnchorPointForPosition(false);
    cell2->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    cell2->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    cell2->setAnchorPoint(Vec2(0, 1));
    cell2->setContentSize(Size(visibleSize.width, 90));
    cell2->setPosition(Vec2(0, H - cell1->getContentSize().height - 30));
    this->addChild(cell2);
    
    Text *text2 = Text::create("联系我们", "", 36);
    text2->setAnchorPoint(Vec2(0, 1));
    text2->setTextColor(Color4B(0xb3, 0xb3, 0xb3, 255));
    text2->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    text2->setPosition(Vec2(40, cell2->getContentSize().height/2));
    cell2->addChild(text2);
    

    for (int i = 0; i < 4; i++)
    {
        auto cell = Layout::create();
        cell->ignoreAnchorPointForPosition(false);
        cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        cell->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
        cell->setAnchorPoint(Vec2(0, 1));
        cell->setContentSize(Size(visibleSize.width, 130));
        cell->setPosition(Vec2(0, H - cell1->getContentSize().height - 30 - i*130));
        this->addChild(cell);
        
        Text *title = Text::create("", "", 40);
        title->setPosition(Vec2(40, cell->getContentSize().height/2));
        title->setTextColor(Color4B(0xe6, 0xe6, 0xe6, 255));
        title->setAnchorPoint(Vec2(0, 0.5));
        title->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        cell->addChild(title);
        
        Text *right = Text::create("", "", 40);
        right->setPosition(Vec2(cell->getContentSize().width -  40, cell->getContentSize().height/2));
        right->setTextColor(Color4B(0x5a, 0x5a, 0x5a, 255));
        right->setAnchorPoint(Vec2(1, 0.5));
        right->setTextHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
        cell->addChild(right);
        
        Json:: Value json = ZJHModel::getInstance()->gameConfigJson["list"]["contactInfo"];
        Layout *line2;
        
        switch (i)
        {
            case 0:
                right->setString(json["email"].asString());
                title->setString("邮箱");
                
                line2 = Layout::create();
                line2->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                line2->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
                line2->setPosition(Vec2(30, 0));
                line2->setContentSize(Size(visibleSize.width-30, 1));
                cell->addChild(line2);
                
                break;
            case 1:
                right->setString(json["mobile"].asString());
                title->setString("客服电话");
                
                line2 = Layout::create();
                line2->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                line2->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
                line2->setPosition(Vec2(30, 0));
                line2->setContentSize(Size(visibleSize.width-30, 1));
                cell->addChild(line2);
                
                break;
            case 2:
                right->setString(json["qq"].asString());
                title->setString("客服QQ");
                
                line2 = Layout::create();
                line2->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                line2->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
                line2->setPosition(Vec2(30, 0));
                line2->setContentSize(Size(visibleSize.width-30, 1));
                cell->addChild(line2);
                break;
            case 3:
                right->setString(json["weixin"].asString());
                title->setString("微信");
                line2 = Layout::create();
                line2->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                line2->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
                line2->setPosition(Vec2(0, 0));
                line2->setContentSize(Size(visibleSize.width, 2));
                cell->addChild(line2);
                
                break;
            default:
                break;
        }
    }
    
    clickCount = 0;
    return true;
}

void AboutOurLayer::showDialog(Ref* ref)
{
    long long currentTime = Utils::getCurrentTime();
    if(currentTime - lastClickTime > 500)
    {
        clickCount = 1;
    }
    else
    {
        clickCount++;
    }
    lastClickTime = currentTime;
    if(clickCount == 5)
    {
        VersionMessageDialog* codeDialog = VersionMessageDialog::create();
        this->addChild(codeDialog,1000);
        clickCount = 0;
    }
}

