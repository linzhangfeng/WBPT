//
//  RoomAllPlayerInfo.cpp
//  ZJH
//
//  Created by wuyu on 16/3/28.
//
//

#include "RoomAllPlayerInfo.h"
#include "ui/UIRichText.h"
using namespace cocos2d;
using namespace ui;
RoomAllPlayerInfo *RoomAllPlayerInfo::create(JPacket &jpacket)
{
    RoomAllPlayerInfo *pRet = new RoomAllPlayerInfo();
    if (pRet && pRet->init(jpacket)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool RoomAllPlayerInfo::init(JPacket &jpacket)
{
    if (!Layer::init()) {
        return false;
    }
//    Utils::talkingdata_click(TALLKINGDATA_CLICK_TYPE::PLAYER_LIST);
    Json::Value &val = jpacket.tojson();
    CCLOG("RoomDNTablehandler_server_table_info_uc:%s",val.toStyledString().c_str());
    Size winSize = Director::getInstance()->getWinSize();
    
    ///背景
//    Sprite *bg = Sprite::create("Room/million/PerPlayerinfo/room_PerPlayer_info_bg.png");
    
//    auto bg = Utils::createDialogBG(799,500);
    Sprite *bg = Sprite::create();
    bg->setContentSize(Size(799, 500));
    
    Size bgSize = bg->getContentSize();
    
    Sprite *dialog = Sprite::create();
    dialog->setContentSize(bgSize);
    dialog->setTag(10);
    bg->setPosition(bgSize / 2);
    dialog->setPosition(Vec2(winSize.width/2, winSize.height/2));
    dialog->addChild(bg);
    this->addChild(dialog);
    
    //标题
    Sprite *title = Sprite::create("Room/million/PerPlayerinfo/room_PerPlayer_info_title.png");
    title->setPosition(bgSize.width / 2, bgSize.height-30);
    dialog->addChild(title, 1);
    
    //外围标记
    Sprite * flagbg = Sprite::create("Room/million/PerPlayerinfo/room_PerPlayer_tipbg.png");
    flagbg->setPosition(Vec2(65, 425));
    dialog->addChild(flagbg);
    ui::RichText* _richText = ui::RichText::create();
    _richText->ignoreContentAdaptWithSize(false);
    _richText->setPosition(Vec2(70, 425));
    dialog->addChild(_richText);
    char buf[100];
    Label *player_nums_text = Label::createWithSystemFont(ZJHModel::getStr("room_perplayer_number_tip"), "Arial", 20);
    player_nums_text->setAnchorPoint(Vec2(0, 0.5));
    player_nums_text->setPosition(75, 425);
    dialog->addChild(player_nums_text);
    snprintf(buf, sizeof(buf), "%d",ZJHModel::getInstance()->curRoomID);
    if (!ZJHModel::getInstance()->gameConfigJson["venue_config"][buf].isNull()) {
        int random_nums = ZJHModel::getInstance()->gameConfigJson["venue_config"][buf]["online_number"].asInt() + Utils::Random(-1000, 1000);
        snprintf(buf, sizeof(buf),"%d",random_nums);
    }
    else{
        snprintf(buf, sizeof(buf), "%d",val["left_len"].asInt()+val["players"].size());
    }
   //
    Label * player_nums = Label::createWithSystemFont(buf, "Arial", 20);
    player_nums->setAnchorPoint(Vec2(0.5, 0.5));
    player_nums->setPosition(240, 425);
    player_nums->setColor(Color3B(111,194,58));
    dialog->addChild(player_nums);
    //外围玩家信息
    Sprite* tmp = Sprite::create("Common/panel_bg.9.png");
    Size size1 = tmp->getContentSize();
    Rect fullRect = Rect(0,0, size1.width, size1.height);
    Rect insetRect = Rect(size1.width/2, size1.height/2,2, 2);
    ui::Scale9Sprite* player_bg = ui::Scale9Sprite::create("Common/panel_bg.9.png",fullRect,insetRect);
    //;
    player_bg->setAnchorPoint(Vec2(0.5,0));
    player_bg->setPreferredSize(Size(710, 360));
    player_bg->setPosition(Vec2(bgSize.width/2, 40));
    dialog->addChild(player_bg);
    
    {
        MyListView* lst = MyListView::create();
        lst->initView(705, 348, 0);
        lst->setAnchorPoint(Vec2(0.5f, 1.0f));
        lst->setPosition(player_bg->getContentSize().width/2, player_bg->getContentSize().height-6);
        player_bg->addChild(lst);
        
        int curindex = 0;
        Sprite* item_tmp = Sprite::create("Room/balance/balance_9.png");
        Size item_size1 = item_tmp->getContentSize();
        Rect item_fullRect = Rect(0,0, item_size1.width, item_size1.height);
        Rect item_insetRect = Rect(item_size1.width/2, item_size1.height/2,2, 2);
        Size item_size;
        ui::Scale9Sprite * item_bg;
        int player_size = val["players"].size();
        for (int i= 0; i< player_size; i++)
        {
            
            int uid = val["players"][i]["uid"].asInt();
            string name = val["players"][i]["name"].asString();
            int vlevel = val["players"][i]["vlevel"].asInt();;
            string m_avatar = val["players"][i]["avatar"].asString();
            long money = val["players"][i]["money"].asDouble();
            int sex = Utils::safeSex(val["players"][i]["sex"].asInt());
            char buf[100];
            if (curindex == 0) {
                Layer* cell = lst->createItem();;
                lst->addItem(cell);
                cell->setContentSize(Size(705,110));
                item_bg = ui::Scale9Sprite::create("Room/balance/balance_9.png",item_fullRect,item_insetRect);
                //;
                item_bg->setAnchorPoint(Vec2(0.5,0.5));
                item_bg->setPreferredSize(Size(700, 110));
                item_bg->setPosition(Vec2(cell->getBoundingBox().size.width/2, cell->getBoundingBox().size.height/2));
                cell->addChild(item_bg);
                item_size = item_bg->getContentSize();
                Sprite * line = Sprite::create("Room/million/PerPlayerinfo/room_perplayers_line.png");
                line->setPosition(Vec2(item_size.width/2, item_size.height/2));
                item_bg->addChild(line);
            }
            
            if (item_bg)
            {
                curindex++;
                int x = 0;
                float dx = item_size.width/2;
                if (curindex >= 2)
                {
                    curindex =0;
                    x = x+dx;
                }
                Sprite * photo_bg =Sprite::create("Room/myinfo_photo_bg.png");
                photo_bg->setAnchorPoint(Vec2(0.5,0.5));
                photo_bg->setPosition(Vec2(x+55,item_size.height/2));
                item_bg->addChild(photo_bg,2);
//                Sprite *avatar = Utils::getAvatar(uid,m_avatar,sex);
                Sprite* avatar = Utils::createCircleImage(m_avatar, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(75,75));
                avatar->setPosition(Vec2(x+55, item_size.height/2));
                item_bg->addChild(avatar);
//                if(avatar != NULL)
//                {
//                    avatar->setAnchorPoint(Vec2(0.5, 0.5));
//                    
//                    item_bg->addChild(avatar);
//                    Rect rectAvatar = avatar->boundingBox();
//                    float scaleX = 75 / rectAvatar.size.width;
//                    float scaleY = 75 / rectAvatar.size.height;
//                    avatar->setScaleX(scaleX);
//                    avatar->setScaleY(scaleY);
//                }
                if(vlevel > 1)
                {
                    snprintf(buf, sizeof(buf), "Common/vip%d.png",vlevel);
                    Sprite * vlevel_bg = Sprite::create(buf);
                    vlevel_bg->setScale(0.8);
                    vlevel_bg->setAnchorPoint(Vec2(0.5, 1.0));
                    vlevel_bg->setPosition(Vec2(x+25, item_size.height-15));
                    item_bg->addChild(vlevel_bg);
                }
                snprintf(buf, sizeof(buf), "Common/common-sex%02d.png", sex);
                Sprite* sex_sp = Sprite::create(buf);
                if(sex_sp)
                {
                    sex_sp->setScale(0.5);
                    sex_sp->setAnchorPoint(Vec2(0.0, 0.5f));
                    sex_sp->setPosition(Vec2(x+105, item_size.height-25));
                    item_bg->addChild(sex_sp, 0);
                }
                Utils::parseName(6, buf, name.c_str());
                Label *labelname= Label::createWithSystemFont(buf, "Thonburi", 16);
                
                labelname->setAnchorPoint(Vec2(0.0f, 0.5f));
                labelname->setPosition(Vec2(x+125, item_size.height-25));
                item_bg->addChild(labelname);
                Sprite * money_bg =Sprite::create("Common/common-money.png");
                money_bg->setScale(0.8);
                money_bg->setAnchorPoint(Vec2(0.0, 0.5));
                money_bg->setPosition(Vec2(x+102, 35));
                item_bg->addChild(money_bg);
                Utils::parseNums1(buf, money);
                Label *labelMoney = Label::createWithSystemFont(buf, "Thonburi", 16);
                labelMoney->setColor(Color3B(37,204,220));
                labelMoney->setAnchorPoint(Vec2(0.0f, 0.5f));
                labelMoney->setPosition(Vec2(x+135, 35));
                item_bg->addChild(labelMoney);
            }
            
            
        }
        lst->reflash(false);
        lst->setScollPercent(0);
        
    }
    
    MyButton* close = MyButton::createWithFile("Common/common_close_btn.png", "Common/common_close_btn_h.png");
    close->setPosition(Vec2(bg->getContentSize().width - 10, bg->getContentSize().height - 10));
    bg->addChild(close);
    close->setCallback([=](int tag){this->removeFromParentAndCleanup(true);
    });
    
    auto touchListener =EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomAllPlayerInfo::onTouchBegan,this);
    touchListener->onTouchMoved =CC_CALLBACK_2(RoomAllPlayerInfo::onTouchMoved, this);
    touchListener->onTouchEnded =CC_CALLBACK_2(RoomAllPlayerInfo::onTouchEnded, this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(RoomAllPlayerInfo::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    return true;
}

bool RoomAllPlayerInfo::isTouchInside(Touch* touch)
{
    Vec2 touchPoint = touch->getLocation();
    Vec2 localPos = getParent()->convertToNodeSpace(touchPoint);
    //Rect rc = this->boundingBox();
    Sprite *p = (Sprite*)this->getChildByTag(10);
    Rect rc = p->boundingBox();
    bool isTouched = rc.containsPoint(localPos);
    
    return isTouched;
}

bool RoomAllPlayerInfo::onTouchBegan(Touch* touch, Event* event)
{
    
    
    return true;
}

void RoomAllPlayerInfo::onTouchMoved(Touch* touch, Event* event)
{
    Vec2 touchPoint = touch->getLocation();
    
}

void RoomAllPlayerInfo::onTouchEnded(Touch* touch, Event* event)
{
    Vec2 touchPoint = touch->getLocation();
    
    if (!isTouchInside(touch))
    {
        closeCallBack(this);
    }
    event->stopPropagation();
}


void RoomAllPlayerInfo::onKeyReleased(EventKeyboard::KeyCode keycode,Event* event)
{
    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
    {
        closeCallBack(this);
    }
    event->stopPropagation();
}

void RoomAllPlayerInfo::closeCallBack(cocos2d::Ref *psender)
{
    this->stopAllActions();
    this->removeFromParentAndCleanup(true);
}