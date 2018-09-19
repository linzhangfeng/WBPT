//
//  RealNameAuthLayer.cpp
//  ZJH
//
//  Created by mac on 2016/11/24.
//
//

#include "RealNameAuthLayer.hpp"
#include <regex>

bool RealNameAuthLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setTitle("实名认证");
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    
    ImageView* itemBg1 = ImageView::create("Mine/name_auth_item_bg.png");
    itemBg1->setAnchorPoint(Vec2(0,1));
    itemBg1->setPosition(Vec2(0,getContentSize().height-getHeadH()));
    addChild(itemBg1);
    
    Text* text = Text::create("身份证", "AmericanTypewriter", 40);
    text->setColor(Color3B(0xe6, 0xe6, 0xe6));
    text->setAnchorPoint(Vec2(0,0.5));
    text->setPosition(Vec2(42,itemBg1->getContentSize().height/2));
    itemBg1->addChild(text);
    
    auto editBoxSize = Size(763, 83);
    
    _editText = ui::EditBox::create(editBoxSize, ui::Scale9Sprite::create("Mine/auth_input_bg.png"));
    _editText->setAnchorPoint(Vec2(0, 0.5));
    _editText->setPosition(Vec2(247, itemBg1->getContentSize().height/2));
    _editText->setFontColor(Color3B(0x66, 0x66, 0x66));
    _editText->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    itemBg1->addChild(_editText);
    
    
    ImageView* itemBg2 = ImageView::create("Mine/name_auth_item_bg.png");
    itemBg2->setAnchorPoint(Vec2(0,1));
    itemBg2->setPosition(Vec2(0,getContentSize().height-getHeadH() - 200));
    addChild(itemBg2);
    
    Text* textName = Text::create("姓   名", "AmericanTypewriter", 40);
    textName->setColor(Color3B(0xe6, 0xe6, 0xe6));
    textName->setAnchorPoint(Vec2(0,0.5));
    textName->setPosition(Vec2(42,itemBg2->getContentSize().height/2));
    itemBg2->addChild(textName);
    
    
    _editTextName = ui::EditBox::create(editBoxSize, ui::Scale9Sprite::create("Mine/auth_input_bg.png"));
    _editTextName->setAnchorPoint(Vec2(0, 0.5));
    _editTextName->setPosition(Vec2(247, itemBg2->getContentSize().height/2));
    _editTextName->setFontColor(Color3B(0x66, 0x66, 0x66));
    _editTextName->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    itemBg2->addChild(_editTextName);
    
    
    Button* sumbmit = Button::create("RoomPublic/jiesan/btn_yellow.png");
    sumbmit->setScale9Enabled(true);
    sumbmit->setAnchorPoint(Vec2(0.5,1));
    sumbmit->setContentSize(Size(960, 118));
    sumbmit->setTitleText("提交申请");
    sumbmit->setTitleFontSize(48);
    sumbmit->setTitleColor(Color3B(0x35,0x26,0x02));
    sumbmit->setPosition(Vec2(getContentSize().width / 2,getContentSize().height - 430 - getHeadH()));
    sumbmit->setVisible(true);
    sumbmit->setName("ok");
    sumbmit->addClickEventListener([=](Ref*ref){
        string auth=_editText->getText();
        const regex pattern("^\\d{6}(1|2)\\d{3}(0|1)\\d[0-3]\\d\\d{3}(x|X|\\d)");
        std:: match_results<std::string::const_iterator> result;
        bool valid = std::regex_match(auth, result,pattern);
        if(!valid)
        {
          PlatformHelper::showToast("请输入正确的身份证号码");
        }
        
        int engnishSize = 0;
        string name = _editTextName->getText();
        for (int i =0; i != name.length(); i++) {
            // 判断是否为英文字母
            if (((name[i] >=48) && (name[i]<=57)) || ((name[i] >=65) && (name[i] <=90)) || ((name[i] >=97) && (name[i] <=122))) {
                engnishSize ++;
            }
        }
        if(engnishSize != 0){
            PlatformHelper::showToast("请输入中文名");
        }
        
    });
    addChild(sumbmit);
    
    return true;
}
