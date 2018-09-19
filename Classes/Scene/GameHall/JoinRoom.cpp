//
//  JoinRoom.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "JoinRoom.hpp"
bool JoinRoom::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size size = Director::getInstance()->getWinSize();
    
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setAnchorPoint(Vec2(0,0));
    layout->setPosition(Vec2(0,0));
    layout->setTouchEnabled(true);
    layout->setSwallowTouches(true);
    layout->addClickEventListener([=](Ref*){
    });
    addChild(layout,-1000);
    
    auto bg = Sprite::create("GameHall/joinRoom/bg.png");
    bg->setPosition(size / 2);
    addChild(bg);
    
    Size csize = bg->getContentSize();
    
    auto closeBtn = Button::create("GameHall/joinRoom/c1.png", "GameHall/joinRoom/c2.png");
    closeBtn->setPosition(Vec2(csize.width - 21, csize.height - 10));
    closeBtn->addClickEventListener([=](Ref*){
        this->removeFromParent();
    });
    bg->addChild(closeBtn,10);
    
    auto t1 = Sprite::create("GameHall/joinRoom/t1.png");
    t1->setPosition(Vec2(csize.width / 2,csize.height - 67));
    bg->addChild(t1);
    
    auto t2 = Sprite::create("GameHall/joinRoom/t2.png");
    t2->setPosition(Vec2(csize.width / 2,csize.height - 176));
    bg->addChild(t2);
    
    vector<std::string> tags = {"1","2","3","4","5","6","7","8","9","clear","0","delete"};
    int bx = 294;
    int by = 515;
    int dx = 322;
    int dy = 136;
    for (int i = 0; i < tags.size(); i ++){
        int di = i % 3;
        int dj = i / 3;
        auto item_bg = Layout::create();
        item_bg->setAnchorPoint(Vec2(0.5,0.5));
        item_bg->setName(tags[i]);
        item_bg->setContentSize(Size(dx,dy));
        item_bg->setPosition(Vec2(bx + dx * di, by - dy * dj));
        item_bg->setTouchEnabled(true);
        item_bg->addClickEventListener(CC_CALLBACK_1(JoinRoom::numsOnClick, this));
        bg->addChild(item_bg);
        
        item_bg->addTouchEventListener([=](Ref* r,Widget::TouchEventType type){
            Layout* l = (Layout*)r;
            Sprite* sp = (Sprite*)l->getChildByName("sp");
            if (type == Widget::TouchEventType::BEGAN)
            {
                sp->setVisible(true);
            }
            else if (type == Widget::TouchEventType::ENDED || type == Widget::TouchEventType::CANCELED)
            {
                sp->setVisible(false);
            }
        });
        
        auto sp = Sprite::create("GameHall/joinRoom/btn1.png");
        sp->setName("sp");
        sp->setVisible(false);
        sp->setPosition(Vec2(dx / 2 + 5,dy / 2 + 5));
        item_bg->addChild(sp);
        
        auto num = Sprite::create(__String::createWithFormat("GameHall/joinRoom/%s.png",tags[i].c_str())->getCString());
        num->setPosition(item_bg->getContentSize() / 2);
        item_bg->addChild(num);
    }
    
    bx = 285;
    by = 618;
    dx = 137;
    for (int i = 0; i < 6; i ++){
        auto label_bg = Sprite::create("GameHall/joinRoom/num_bg.png");
        label_bg->setPosition(Vec2(bx + dx * i, by));
        bg->addChild(label_bg);
        
        auto label = LabelAtlas::create("", "GameHall/joinRoom/nums.png", 46, 62, '0');
//        auto label = Label::createWithSystemFont("", "Thonburi", 72);
//        label->setColor(Color3B(115,56,19));
        label->setAnchorPoint(Vec2(0.5,0));
        label->setPosition(Vec2(bx + dx * i, by + 5));
        bg->addChild(label);
        
        allNumsLabel.push_back(label);
    }
    return true;
}

void JoinRoom::setCodeText(string code){
    string c1 = "";
    string c2 = "";
    string c3 = "";
    string c4 = "";
    string c5 = "";
    string c6 = "";
    for(int i = 0; i < code.size() ; i++){
        switch (i) {
            case 0:
                c1 = code[i];
                break;
            case 1:
                c2 = code[i];
                break;
            case 2:
                c3 = code[i];
                break;
            case 3:
                c4 = code[i];
                break;
            case 4:
                c5 = code[i];
                break;
            case 5:
                c6 = code[i];
                break;
            default:
                break;
        }
    }
    allNumsLabel[0]->setString(c1);
    allNumsLabel[1]->setString(c2);
    allNumsLabel[2]->setString(c3);
    allNumsLabel[3]->setString(c4);
    allNumsLabel[4]->setString(c5);
    allNumsLabel[5]->setString(c6);
}

void JoinRoom::numsOnClick(Ref* r)
{
    Sound::getInstance()->playEffect("Sound/click.mp3");
    auto n = (Layout*)r;
    std::string name = n->getName();
    if (codeStr.size() >= 6)
    {
        if(name == "clear")
        {
            codeStr = "";
            setCodeText("");
            return;
        }
        
        PlatformHelper::showToast("房间号为6位有效数字！");
        
        if (name == "delete")
        {
            if(codeStr.size() <= 0)
            {
                return;
            }
            string tempStr;
            for (int i = 0; i < codeStr.size() - 1; i++)
            {
                tempStr += codeStr.at(i);
            }
            codeStr = tempStr;
            setCodeText(codeStr);
        }
        return;
    }
    
    if (name != "delete" && name != "clear")
    {
        codeStr += name;
    }
    else if(name == "delete")
    {
        if(codeStr.size() <= 0)
        {
            return;
        }
        string tempStr;
        for (int i = 0; i < codeStr.size() - 1; i++)
        {
            tempStr += codeStr.at(i);
        }
        codeStr = tempStr;
    }else if(name == "clear")
    {
        codeStr = "";
        setCodeText("");
    }
    setCodeText(codeStr);
    
    if(codeStr.size() == 6)
    {
        //加入对局的请求
        ZJHModel::getInstance()->gotoRoomWithCode(codeStr);
    }
}