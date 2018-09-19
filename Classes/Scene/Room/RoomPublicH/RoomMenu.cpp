//
//菜单Layer
//
//

#include "RoomMenu.h"
//#include "RoomCMConfig.h"
#include "ZJH.h"

using namespace cocos2d;

RoomMenu::RoomMenu()
    :m_menuCallback(NULL)
{
    
}

bool RoomMenu::init()
{
    if (!Layer::init())
    {
        return false;
    }
   
    auto touchListener =EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomMenu::onTouchBegan,this);
    touchListener->onTouchMoved =CC_CALLBACK_2(RoomMenu::onTouchMoved, this);
    touchListener->onTouchEnded =CC_CALLBACK_2(RoomMenu::onTouchEnded, this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(RoomMenu::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    return true;
}

void RoomMenu::MyInit(int type ,bool isCMRoom)
{
    //type 1：房主（有管理房间功能） 2：房客
    Size winSize = Director::getInstance()->getWinSize();
    int itemNums = 2;
    
    // 每个Item的高度76 其他边角高度为80
    Size dialogSize = Size(307, (76 * itemNums + 80));
    
    auto *dialog = ui::Scale9Sprite::create("RoomPublicH/menu_more_bg.png");
    dialog->setScale9Enabled(true);
    dialog->setPreferredSize(dialogSize);
    dialog->setAnchorPoint(Vec2(1, 1));
    dialog->setName("dialog");
    dialog->setPosition(Vec2(winSize.width, winSize.height - 67));
    this->addChild(dialog);
    
    char buff[128];
    float posX = dialogSize.width / 2;
    float deltaY = 76;
    float posY = 37 + deltaY* 0.5;
    std::string names[4] = {
//        "room_manage",
        "game_setting",
//        "game_help",
        "end_room",
        "exit_room",
    };
    
    std::string titles[4] = {
        "游戏设置",
        "解散房间",
        "退出房间",
    };
    
    
    for (int i = 3; i >= 1; i--)
    {
        if((!(type == 1 || !isCMRoom)) && (names[i - 1] == "end_room")){
            continue;
            
        }
        if((names[i - 1] == "exit_room") && !isCMRoom){
            continue;
        }
        if((names[i - 1] == "exit_room") && type == 1){
            continue;
        }
        
        Button *btn;
        if (i == 4){
            btn = Button::create("", "RoomPublicH/menu_item_tail_press.png");
        }else if (i == 1){
            btn = Button::create("", "RoomPublicH/menu_item_head_press.png");
        }else{
            btn = Button::create("", "RoomPublicH/menu_more_item_press.png");
        }

        btn->setContentSize(Size(246, 73));
        btn->setScale9Enabled(true);
        btn->setTitleText(titles[i-1].c_str());
        btn->setTitleColor(Color3B(0x4d, 0x2d, 0x1e));
        btn->setTitleFontSize(30);
        btn->setPosition(Vec2(posX, posY));
        btn->addClickEventListener(CC_CALLBACK_1(RoomMenu::buttonClicked, this));
        btn->setName(names[i - 1]);
        dialog->addChild(btn);
        
        if (i != 4)
        {
            Sprite *line = Sprite::create("RoomPublicH/menu_more_item_line.png");
            line->setPosition(Vec2(posX, posY - deltaY * 0.5));
            dialog->addChild(line);
        }
        
        posY += deltaY;
    }
}

void RoomMenu::buttonClicked(cocos2d::Ref *ref)
{
    Button *btn = dynamic_cast<Button *>(ref);
    if (btn) {
        std::string name = btn->getName();
        
        this->closeCallBack();
        
        if (m_menuCallback) {
            m_menuCallback(name);
        }
    }
}

void RoomMenu::closeCallBack()
{
    runAction(RemoveSelf::create());
}

bool RoomMenu::isTouchInside(Touch* touch)
{
    Point touchPoint = touch->getLocation();
    Node * dialog = dynamic_cast<Node *>(this->getChildByName("dialog"));
    
    return dialog->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation()));
}

bool RoomMenu::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void RoomMenu::onTouchMoved(Touch* touch, Event* event)
{
    
}

void RoomMenu::onTouchEnded(Touch* touch, Event* event)
{
    if (!isTouchInside(touch)) {
        this->closeCallBack();
    }
}

void RoomMenu::onKeyReleased(EventKeyboard::KeyCode keycode,Event* event)
{
    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
    {
        this->closeCallBack();
    }
    event->stopPropagation();
}
