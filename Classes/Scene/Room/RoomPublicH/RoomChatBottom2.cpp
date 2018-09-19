//
//  RoomChatBottom2.cpp
//  ZJH
//
//  Created by pg on 16/9/6.
//
//

#include "RoomChatBottom2.hpp"
#include "../../../Model/MsgModel.h"
#include "proto.h"
#include "ZJH.h"

RoomChatBottom2* RoomChatBottom2::create(Size nRoot)
{
    RoomChatBottom2* pRet = new RoomChatBottom2;
    
    if(pRet)
    {
        pRet->setRootSize(nRoot);
        if (pRet->init()) {
            pRet->autorelease();
            return pRet;
        }
        
        delete pRet;
        pRet = 0;
    }

    
    
    return 0;
}

std::string subStr_roomchat2(std::string m_pInputText,int subNums)
{
    int count = 0;
    const char* str_p = m_pInputText.c_str();
    long long len = strlen(str_p);
    std::deque<int> lens;
    
    for(int i = 0; i < len;i++)
    {
        if((str_p[i] >= '0' &&  str_p[i] <= '9') || (str_p[i] >= 'a' &&  str_p[i] <= 'z') || (str_p[i] >= 'A' &&  str_p[i] <= 'Z') || (str_p[i] >= 0 &&  str_p[i] <= 127))
        {
            count += 1;
            lens.push_back(1);
        }
        else
        {
            count += 3;
            i += 2;
            lens.push_back(3);
        }
    }
    
    int subL = 0;
    for (int i = 0; i < subNums; i ++)
    {
        subL += lens[lens.size() - i - 1];
    }
    
    char buf[count + 1 - subL];
    snprintf(buf, sizeof(buf), "%s",m_pInputText.c_str());
    return buf;
}

bool RoomChatBottom2::init()
{

    if (!Layer::init())
    {
        return false;
    }
    face_layer = NULL;
    faceLayout = NULL;
    fastLayout = NULL;
    
    //聊天发言背景
    Scale9Sprite* SendBG= Scale9Sprite::create();
    SendBG->setContentSize(Size(514, 84));
    SendBG->setAnchorPoint(Vec2(0, 0));
    SendBG->setPosition(Vec2(15,15));
    addChild(SendBG);
    
    
    

    
    //语音按钮
//    btn_sound = Button::create("RoomPublicH/chat/chat_voice.png");
//    btn_sound->ignoreContentAdaptWithSize(false);
//    btn_sound->setFillContentSizeEnabled(false);
//    btn_sound->setPosition(Vec2(btn_sound->getContentSize().width/2 + 5,SendBG->getContentSize().height / 2));
//    btn_sound->setName("chat_sound");
//    SendBG->addChild(btn_sound);
//    
//    btn_sound->addTouchEventListener([=](Ref* r,Widget::TouchEventType type){
//        if (type == Widget::TouchEventType::BEGAN)
//        {
//            PlatformHelper::startRecord();
//        }
//        else if (type == Widget::TouchEventType::ENDED || type == Widget::TouchEventType::CANCELED)
//        {
//            PlatformHelper::stopRecord();
//            __NotificationCenter::getInstance()->postNotification("chat_sound_record_stop_notify");
//        }
//    });
    
    //输入框
    auto editBoxSize = Size(267, 53);
    
    _editText = ui::EditBox::create(editBoxSize, ui::Scale9Sprite::create("RoomPublicH/chat/chat_txt.png"));
    _editText->setAnchorPoint(Vec2(0, 0.5));
    _editText->setPosition(Vec2(9, SendBG->getContentSize().height / 2));
    _editText->setFontColor(Color3B::BLACK);
    _editText->setMaxLength(100);
    _editText->setFontSize(editBoxSize.height/3);
    _editText->setInputMode(EditBox::InputMode::SINGLE_LINE);
    _editText->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editText->setDelegate(this);
    SendBG->addChild(_editText);
    
    btn_face = Button::create("RoomPublicH/chat/chat_face.png");
    btn_face->setAnchorPoint(Vec2(0, 0.5));
    btn_face->setPosition(Vec2(284, SendBG->getContentSize().height / 2));
    btn_face->addClickEventListener(CC_CALLBACK_1(RoomChatBottom2::btnCallback, this));
    btn_face->setName("chat_face");
    //    btn_face->setScale(ss);
    SendBG->addChild(btn_face);
    
    
    btn_send = Button::create("RoomPublicH/chat/chat_send.png", "RoomPublicH/chat/chat_send_press.png");
    btn_send->setVisible(true);
    btn_send->ignoreContentAdaptWithSize(false);
    btn_send->setFillContentSizeEnabled(false);
    btn_send->setAnchorPoint(Vec2(1, 0.5));
//    btn_send->setContentSize(Size(90, SendBG->getContentSize().height * 0.7));
    btn_send->setPosition(Vec2(SendBG->getContentSize().width - 5,SendBG->getContentSize().height / 2));
    btn_send->addClickEventListener(CC_CALLBACK_1(RoomChatBottom2::btnCallback, this));
    btn_send->setName("chat_send");
    SendBG->addChild(btn_send);
    btn_send->setVisible(false);
    
    //发送按钮或者快速发言按钮
    btn_fast = Button::create("RoomPublicH/chat/fast_send.png", "RoomPublicH/chat/fast_send_press.png");
    btn_fast->setVisible(true);
    btn_fast->ignoreContentAdaptWithSize(false);
    btn_fast->setFillContentSizeEnabled(false);
    btn_fast->setAnchorPoint(Vec2(1, 0.5));
//    btn_fast->setContentSize(Size(90, SendBG->getContentSize().height * 0.7));
    btn_fast->setPosition(Vec2(SendBG->getContentSize().width - 5,SendBG->getContentSize().height / 2));
    btn_fast->addClickEventListener(CC_CALLBACK_1(RoomChatBottom2::btnCallback, this));
    btn_fast->setName("fast_send");
    SendBG->addChild(btn_fast);
    
    
    //auto send_bg = ui::Scale9Sprite::create("Chat/bar_speak.png");
    //send_bg->setPreferredSize(Size(btn_send->getContentSize()));
    //send_bg->setAnchorPoint(Vec2(0.5, 0.5));
    //send_bg->setPosition(btn_send->getContentSize() / 2);
    //btn_send->addChild(send_bg,-100);
    
    
    
//    //
//    //other area
//    {
//        int dx = size.width / 4;
//        int dy = dx + 40;
//        int bx = dx / 2;
//        int by = dy * 1 - dy / 2;
//        
//        otherH = dy * 1;
//        
//        other_layer = Layer::create();
//        Size otherSize = Size(size.width,otherH);
//        
//        other_layer->setContentSize(otherSize);
//        other_layer->ignoreAnchorPointForPosition(false);
//        other_layer->setAnchorPoint(Vec2(0, 0));
//        other_layer->setPosition(Vec2(0, faceH - otherH));
//        allLayer->addChild(other_layer);
//        
//        LayerColor* bg = LayerColor::create(Color4B(36,36,36,255));
//        bg->setContentSize(otherSize);
//        bg->setAnchorPoint(Vec2(0.0f, 0.0f));
//        bg->setPosition(0,0);
//        other_layer->addChild(bg, -100);
//        
//        Button* btn = Button::create("Chat/chat_bottombar_photo.png",
//                                     "Chat/chat_bottombar_photo_press.png");
//        btn->setPosition(Vec2(bx,by));
//        btn->addClickEventListener(CC_CALLBACK_1(RoomChatBottom2::btnCallback, this));
//        btn->setName("other_photo");
//        btn->setScale(ss);
//        other_layer->addChild(btn);
//        
//        bx += dx;
//        btn = Button::create("Chat/chat_bottombar_pic.png",
//                             "Chat/chat_bottombar_pic_press.png");
//        btn->setPosition(Vec2(bx,by));
//        btn->addClickEventListener(CC_CALLBACK_1(RoomChatBottom2::btnCallback, this));
//        btn->setName("other_img");
//        btn->setScale(ss);
//        other_layer->addChild(btn);
//    }
    
//    //input area
//    {
//        input_layer = Layer::create();
//        
//        LayerColor* bg = LayerColor::create(Color4B(51,51,51,255));
//        bg->setContentSize(Size(size.width,151*ss));
//        bg->setAnchorPoint(Vec2(0.0f, 0.0f));
//        bg->setPosition(0,0);
//        input_layer->addChild(bg, -100);
//        
//        input_layer->setContentSize(bg->boundingBox().size);
//        input_layer->ignoreAnchorPointForPosition(false);
//        input_layer->setAnchorPoint(Vec2(0, 0));
//        input_layer->setPosition(Vec2(0, faceH));
//        allLayer->addChild(input_layer);
//        
//        int by = bg->boundingBox().size.height / 2;
//        inputH = bg->boundingBox().size.height;
//        
//        //语音按钮
//        btn_sound = Button::create("Chat/chat_bottombar_icon4.png",
//                                   "Chat/chat_bottombar_icon4_press.png");
//        btn_sound->ignoreContentAdaptWithSize(false);
//        btn_sound->setFillContentSizeEnabled(false);
//        btn_sound->setContentSize(Size(85 * 2, by * 2));
//        btn_sound->setPosition(Vec2(85 * ss,by));
//        btn_sound->addClickEventListener(CC_CALLBACK_1(RoomChatBottom2::btnCallback, this));
//        btn_sound->setName("chat_sound");
//        btn_sound->setScale(ss);
//        input_layer->addChild(btn_sound);
//        
//        //输入框
//        auto editBoxSize = Size(size.width - 155 * ss - 285 * ss, 98 * ss);
//        
//        _editText = ui::EditBox::create(editBoxSize, ui::Scale9Sprite::create("Chat/bar_txt.png"));
//        _editText->setAnchorPoint(Vec2(0, 0.5));
//        _editText->setPosition(Vec2(155 * ss, by));
//        _editText->setFontColor(Color3B::BLACK);
//        _editText->setMaxLength(100);
//        _editText->setFontSize(editBoxSize.height/2.5);
//        _editText->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
//        _editText->setDelegate(this);
//        input_layer->addChild(_editText);
    
//        //笑脸按钮
//        btn_face = Button::create("Chat/chat_bottombar_icon3.png",
//                                  "Chat/chat_bottombar_icon3_press.png");
//        btn_face->setPosition(Vec2(size.width - 220 * ss,by));
//        btn_face->addClickEventListener(CC_CALLBACK_1(RoomChatBottom2::btnCallback, this));
//        btn_face->setName("chat_face");
//        btn_face->setScale(ss);
//        input_layer->addChild(btn_face);
        
//        //加号按钮/发送切换
//        btn_add = Button::create("Chat/chat_bottombar_icon2.png",
//                                 "Chat/chat_bottombar_icon2_press.png");
//        btn_add->setPosition(Vec2(size.width - 85 * ss,by));
//        btn_add->addClickEventListener(CC_CALLBACK_1(RoomChatBottom2::btnCallback, this));
//        btn_add->setName("chat_add");
//        btn_add->setScale(ss);
//        input_layer->addChild(btn_add);
//        
//        btn_send = Button::create();
//        btn_send->setTitleText("发送");
//        btn_send->setTitleFontSize(45);
//        btn_send->setTitleColor(Color3B(100,100,100));
//        btn_send->setVisible(false);
//        btn_send->ignoreContentAdaptWithSize(false);
//        btn_send->setFillContentSizeEnabled(false);
//        btn_send->setContentSize(Size(150, by * 2));
//        btn_send->setPosition(Vec2(size.width - 85 * ss,by));
//        btn_send->setScale(ss);
//        btn_send->addClickEventListener(CC_CALLBACK_1(RoomChatBottom2::btnCallback, this));
//        btn_send->setName("chat_send");
//        
//        auto send_bg = ui::Scale9Sprite::create("Chat/bar_speak.png");
//        send_bg->setPreferredSize(Size(150 , 80));
//        send_bg->setAnchorPoint(Vec2(0.5, 0.5));
//        send_bg->setPosition(btn_send->getContentSize() / 2);
//        btn_send->addChild(send_bg,-100);
//        
//        input_layer->addChild(btn_send);
//    }
    
//    allLayer->setContentSize(Size(size.width,inputH + faceH));
//    allLayer->setPosition(Vec2(0, -faceH));
//    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch* touch, Event* event)
    {
        Point localP = touch->getLocation();
        if (faceLayout != NULL){
            if (!faceLayout->getBoundingBox().containsPoint(localP))
            {
                faceLayout->setVisible(false);
                return true;
            }
        }
        
        if (fastLayout != NULL){
            if (!fastLayout->getBoundingBox().containsPoint(localP))
            {
                fastLayout->setVisible(false);
                return true;
            }
        }
//        this->scollToView("null");
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomChatBottom2::notificationReflash), "image_notify", NULL);
    return true;
}

void RoomChatBottom2::faceCallback(Ref* ref)
{
    Node* n = (Node*)ref;
    string name = n->getName();
    if (name == "del")
    {
        std::string send_text = _editText->getText();
        if (send_text == "")
        {
            return;
        }
        else if (send_text.length() <= 1)
        {
            _editText->setText("");
        }
        else
        {
            std::string send_text_result = subStr_roomchat2(send_text, 1);
            _editText->setText(send_text_result.c_str());
        }
        checkContent();
    }
    else
    {
        std::string send_text = _editText->getText();
        _editText->setText(__String::createWithFormat("%s%s",send_text.c_str(),name.c_str())->getCString());
        checkContent();
    }
}

void RoomChatBottom2::btnCallback(Ref* ref)
{
    Node* n = (Node*)ref;
    string name = n->getName();
    if (name == "chat_sound")
    {
        PlatformHelper::showToast("暂未开放，敬请期待");
    }
    else if (name == "chat_add")
    {
        this->scollToView("other");
    }
    else if (name == "chat_face")
    {
//        this->scollToView("face");
        showFace();
    }
    else if (name == "fast_send")
    {
        //        this->scollToView("face");
        showFast();
    }
    else if (name == "chat_send")
    {
        std::string send_text = _editText->getText();
        if(send_text == "")return;
        MsgModel::getInstance()->sendText_group(send_text);
        _editText->setText("");
        checkContent();
        
        if (faceLayout != NULL){
            faceLayout->setVisible(false);
        }
        this->scollToView("null");
    }
    else if (name == "other_photo")
    {
        this->scollToView("null");
        PlatformHelper::openCamera();
    }
    else if (name == "other_img")
    {
        this->scollToView("null");
        PlatformHelper::openSavedPhotosAlbum();
    }
}

void RoomChatBottom2::notificationReflash(Ref* msg)
{
    __String* str = (__String*)msg;
    if (str != NULL)
    {
        std::string s = str->getCString();
        MsgModel::getInstance()->sendImg_group(s);
    }
}

void RoomChatBottom2::checkContent()
{
    std::string text = _editText->getText();
    if (text == "")
    {
        btn_send->setVisible(false);
        fastLayout->setVisible(true);
        btn_fast->setVisible(true);
    }
    else
    {
        btn_send->setVisible(true);
        fastLayout->setVisible(false);
        btn_fast->setVisible(false);
    }
}

void RoomChatBottom2::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
    log("editBox %p DidBegin !", editBox);
}

void RoomChatBottom2::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    log("editBox %p DidEnd !", editBox);
}

void RoomChatBottom2::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    log("editBox %p TextChanged, text: %s ", editBox, text.c_str());
    if (text == "")
    {
        if (btn_send != NULL){
            btn_send->setVisible(false);
        }
        if (fastLayout != NULL){
            fastLayout->setVisible(true);
        }
        if (btn_fast != NULL){
            btn_fast->setVisible(true);
        }
    }
    else
    {
        if (btn_send != NULL){
            btn_send->setVisible(true);
        }
        if (fastLayout != NULL){
            fastLayout->setVisible(false);
        }
        if (btn_fast != NULL){
            btn_fast->setVisible(false);
        }
    }
}

void RoomChatBottom2::editBoxReturn(ui::EditBox* editBox)
{
    log("editBox %p was returned !",editBox);
    
//    if (_editText->getText() != "" && _editText == editBox)
//    {
//                std::string send_text = _editText->getText();
//                MsgModel::getInstance()->sendText_group(send_text);
//                _editText->setText("");
//                checkContent();
//        
//                this->scollToView("null");
//    }
}

void RoomChatBottom2::scollToView(std::string v)
{
    if (v == "null")
    {
//        allLayer->setPosition(Vec2(0, -faceH));
    }
    else if (v == "face")
    {
//        allLayer->setPosition(Vec2(0, 0));
//        face_layer->setVisible(true);
//        other_layer->setVisible(false);
    }
    else if (v == "other")
    {
//        allLayer->setPosition(Vec2(0, -faceH + otherH));
//        face_layer->setVisible(false);
//        other_layer->setVisible(true);
    }
}

void RoomChatBottom2::onExit()
{
    __NotificationCenter::getInstance()->removeAllObservers(this);
    Layer::onExit();
}

void RoomChatBottom2::showFace(){
    Size size = Size(494, 84);
    if (faceLayout == NULL){
        faceLayout = Layout::create();
        faceLayout->setContentSize(Size(494, 300));
        faceLayout->setName("face_layout");
        faceLayout->setAnchorPoint(Vec2(0, 0));
        faceLayout->setPosition(Vec2(15+10,5+ size.height));
        faceLayout->setTouchEnabled(true);
        addChild(faceLayout);
        //face area
        {
            int dx = size.width / 7;
            int dy = dx;
            int bx = 0;
            int by = dy * 3;
            
            faceH = dy * 4;
            
            face_layer = Layer::create();
            Size faceSize = Size(size.width, 300);
            
            face_layer->setContentSize(faceSize);
            face_layer->ignoreAnchorPointForPosition(false);
            face_layer->setAnchorPoint(Vec2(0, 0));
            face_layer->setPosition(Vec2(0, 0));
            //        allLayer->addChild(face_layer);
            faceLayout->addChild(face_layer);
            
            Scale9Sprite* bg = Scale9Sprite::create("RoomPublicH/chat/face_bg.png");
            bg->setScale9Enabled(true);
            bg->setContentSize(faceSize);
            bg->setAnchorPoint(Vec2(0.0f, 0.0f));
            bg->setPosition(0,0);
            face_layer->addChild(bg, -100);
            
            PageView* pageView = PageView::create();
            pageView->setContentSize(faceSize);
            pageView->setAnchorPoint(Vec2(0, 0));
            pageView->setPosition(Vec2(0,0));
            pageView->setUsingCustomScrollThreshold(true);
            pageView->setCustomScrollThreshold(size.width / 5);
            pageView->removeAllPages();
            face_layer->addChild(pageView);
            
            int pageCount = 3;
            std::string nameArray[99] = {"[呵呵]","[嘻嘻]","[哈哈]","[晕]","[泪]","[馋嘴]","[抓狂]","[哼]","[可爱]","[怒]","[困]","[汗]","[睡觉]","[偷笑]","[吃惊]","[酷]","[闭嘴]","[花心]","[失望]","[生病]","[亲亲]","[右哼哼]","[嘘]","[挤眼]","[酷]","[感冒]","[做鬼脸]","[阴险]","[热吻]","[心]","[ok]","[不要]","[弱]","[good]","[拳头]","[耶]","[0]","[1]","[2]","[3]","[4]","[5]","[6]","[7]","[8]","[9]"};
            
            
            
            int faceCount = 0;
            for (int i = 0; i < pageCount; i++)
            {
                Layout* layout = Layout::create();
                layout->setContentSize(faceSize);
                if (i <= 1)
                {
                    for (int j = 0; j < 3 * 7; j ++)
                    {
                        int di = j % 7;
                        int dj = j / 7;
                        
                        auto* image = Layout::create();
                        image->setTouchEnabled(true);
                        image->setSwallowTouches(false);
                        image->setContentSize(Size(dx, dy));
                        image->setAnchorPoint(Vec2(0.0, 0.0));
                        image->setPosition(Vec2(bx + di * dx,by - dj * dy));
                        layout->addChild(image);
                        image->addClickEventListener(CC_CALLBACK_1(RoomChatBottom2::faceCallback, this));
                        
                        if (j != 20)
                        {
                            image->setName(nameArray[faceCount]);
                            Sprite* icon = Sprite::create(__String::createWithFormat("Chat/face/tt_e%d.png",faceCount)->getCString());
                            //                        icon->setScale(ss);
                            icon->setScale(0.5);
                            icon->setPosition(image->getContentSize() / 2);
                            image->addChild(icon);
                            faceCount++;
                        }
                        else
                        {
                            image->setName("del");
                            Sprite* icon = Sprite::create("Chat/face/tt_default_emo_back_normal.png");
                            //                        icon->setScale(ss);
                            icon->setScale(0.5);
                            icon->setPosition(image->getContentSize() / 2);
                            image->addChild(icon);
                        }
                    }
                }
                else
                {
                    for (int j = 0; j < 7; j ++)
                    {
                        int di = j % 7;
                        int dj = j / 7;
                        auto* image = Layout::create();
                        image->setTouchEnabled(true);
                        image->setSwallowTouches(false);
                        image->setContentSize(Size(dx, dy));
                        image->setAnchorPoint(Vec2(0.0, 0.0));
                        image->setPosition(Vec2(bx + di * dx + bx,by - dj * dy));
                        layout->addChild(image);
                        image->addClickEventListener(CC_CALLBACK_1(RoomChatBottom2::faceCallback, this));
                        
                        if (j != 6)
                        {
                            image->setName(nameArray[faceCount]);
                            Sprite* icon = Sprite::create(__String::createWithFormat("Chat/face/tt_e%d.png",faceCount)->getCString());
                            //                        icon->setScale(ss);
                            icon->setScale(0.5);
                            icon->setPosition(image->getContentSize() / 2);
                            image->addChild(icon);
                            faceCount++;
                        }
                        else
                        {
                            image->setName("del");
                            Sprite* icon = Sprite::create("Chat/face/tt_default_emo_back_normal.png");
                            //                        icon->setScale(ss);
                            icon->setScale(0.5);
                            icon->setPosition(image->getContentSize() / 2);
                            image->addChild(icon);
                        }
                    }
                }
                pageView->insertPage(layout,i);
            }
            
            pageView->scrollToPage(0);
            
            pageView->addEventListener([=](Ref *pSender, PageView::EventType type){
                switch (type)
                {
                    case PageView::EventType::TURNING:
                    {
                        PageView* pageView = dynamic_cast<PageView*>(pSender);
                        
                        //                    _displayValueLabel->setString(StringUtils::format("page = %ld", pageView->getCurPageIndex() + 1));
                    }
                        break;
                        
                    default:
                        break;
                }
            });
        }

    }else{
        if (faceLayout->isVisible()){
            faceLayout->setVisible(false);
        }else{
            faceLayout->setVisible(true);
        }
    }
    
}


//显示快速聊天
void RoomChatBottom2::showFast(){
    if(fastLayout == NULL){
        //快速发言内容
        std::vector<std::string> strs;
        strs.clear();
        bool useDefault = true;
        if (ZJHModel::getInstance()->gameUseDefaultChat == false)
        {
            Json::Value value_date;
            std::string data = ZJHModel::getInstance()->gameDefaultChat;
            if (data != "")
            {
                
                Json::Reader read;
                read.parse(data, value_date);
                if (value_date.size() > 0){
                    for (int i = 0; i < value_date.size(); i ++){
                        strs.push_back(value_date[i].asString());
                    }
                
                    useDefault = false;
                }
            }
        }
        if (useDefault)
        {
            strs.clear();
            strs.push_back("底裤都输光了");
            strs.push_back("风水轮流转");
            strs.push_back("别墨迹了");
            strs.push_back("今儿个真高兴");
            strs.push_back("你太牛了");
            strs.push_back("下手别这样重啊");
            strs.push_back("咋比乌龟还慢呢");
            strs.push_back("看我通杀全场");
            strs.push_back("快点下注吧");
            strs.push_back("这些钱都是我的");
        }
        
        //快速发言基础布局
        fastLayout = Layout::create();
        fastLayout->setContentSize(Size(321, 314));
        fastLayout->setName("fast_layout");
        fastLayout->setPosition(Vec2(199, 5+84));
        fastLayout->setTouchEnabled(true);
        addChild(fastLayout);
        
        
        Scale9Sprite* bg = Scale9Sprite::create("RoomPublicH/chat/fast_send_bg.png");
        bg->setScale9Enabled(true);
        bg->setContentSize(fastLayout->getContentSize());
        bg->setAnchorPoint(Vec2(0.0f, 0.0f));
        bg->setPosition(0,0);
        fastLayout->addChild(bg);
        
        
        ListView* listView = ListView::create();
        listView->setDirection(ui::ScrollView::Direction::VERTICAL);
        listView->setBounceEnabled(false);
        listView->setContentSize(Size(321, 275));
        listView->setAnchorPoint(Vec2(0, 1));
        listView->setPosition(Vec2(0,fastLayout->getContentSize().height-14));
        listView->setScrollBarEnabled(false);
        listView->setTouchEnabled(true);
        listView->setItemsMargin(0);
        listView->setName("fast_list");
        bg->addChild(listView);
        
        for (int i = 0; i < strs.size(); i++)
        {
            
            Layout* cell = Layout::create();
            cell->setContentSize(Size(321, 55));
            listView->pushBackCustomItem(cell);
            
            char buf[128];
            Utils::parseName(9, buf, strs[i].c_str());
            Button* item = Button::create();
            item->setTitleText(buf);
            item->setTitleFontSize(24);
            item->setScale9Enabled(true);
            item->loadTexturePressed("RoomPublicH/chat/fast_item_check.png");
            item->setAnchorPoint(Vec2(0, 0.5));
            item->setContentSize(Size(312, 55));
            item->setTitleColor(Color3B(0x4d,0x2d,0x1e));
            item->setPosition(Vec2(4, 55/2));
            item->setTouchEnabled(true);
            item->setTitleAlignment(TextHAlignment::LEFT);
            item->addClickEventListener([=](Ref* rf){
                MsgModel::getInstance()->sendText_group(strs[i]);
                fastLayout->setVisible(false);
            });
            
            auto line = Scale9Sprite::create("RoomPublicH/chat/fast_line.png");
            line->setAnchorPoint(Vec2(0, 0));
            line->setPosition(Vec2(2,0));
            item->addChild(line);
        
            cell->addChild(item);
      
        }
    }else{
        if(fastLayout->isVisible()){
            fastLayout->setVisible(false);
        }else{
            fastLayout->setVisible(true);
        }
        
    }
    
}

void RoomChatBottom2::initFast(){
    if (btn_fast && btn_fast->isVisible()){
        //默认真是快速发言
        if (fastLayout){
            fastLayout->setVisible(true);
        }else{
            showFast();
        }

    }
    
}
