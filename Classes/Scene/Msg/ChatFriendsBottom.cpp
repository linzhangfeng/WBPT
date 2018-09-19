//
//  ChatFriendsBottom.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "ChatFriendsBottom.hpp"
#include "../../Model/MsgModel.h"
#include "proto.h"
#include "ChatRecordEffect.hpp"

std::string subStr(std::string m_pInputText,int subNums)
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

void ChatFriendsBottom::setBottomType(int type)
{
    this->type = type;
    Size size = Director::getInstance()->getWinSize();
    
    ChatRecordEffect* effect = ChatRecordEffect::create();
    Content_Type contentType = CONTENT_CLUB;
    if(type == 0){
        contentType = CONTENT_POINT_TO_POINT;
    }
    effect->contentType = contentType;
    addChild(effect,100000);
    
    allLayer = Layer::create();
    allLayer->setName("allLayer");
    allLayer->ignoreAnchorPointForPosition(false);
    allLayer->setAnchorPoint(Vec2(0, 0));
    allLayer->setPosition(Vec2(0, 0));
    addChild(allLayer);
    
    //face area
    {
        int dx = size.width / 7;
        int dy = dx;
        int bx = dx / 2;
        int by = dy * 4 - dy / 2;
        
        faceH = dy * 4;
        
        face_layer = Layer::create();
        Size faceSize = Size(size.width,faceH);
        
        face_layer->setContentSize(faceSize);
        face_layer->ignoreAnchorPointForPosition(false);
        face_layer->setAnchorPoint(Vec2(0, 0));
        allLayer->addChild(face_layer);
        
        LayerColor* bg = LayerColor::create(Color4B(36,36,36,255));
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
                    
                    Button* image = Button::create();
                    
                    if (j != 20)
                    {
                        image->setName(nameArray[faceCount]);
                        image->loadTextures(__String::createWithFormat("Chat/face/tt_e%d.png",faceCount)->getCString(), __String::createWithFormat("Chat/face/tt_e%d.png",faceCount)->getCString());
                        faceCount++;
                    }
                    else
                    {
                        image->setName("del");
                        image->loadTextures("Chat/face/tt_default_emo_back_normal.png","Chat/face/tt_default_emo_back_normal.png");
                    }
                    
                    image->ignoreContentAdaptWithSize(false);
                    image->setFillContentSizeEnabled(false);
                    image->setSwallowTouches(false);
                    image->setContentSize(Size(dx, dy));
                    image->setAnchorPoint(Vec2(0.5, 0.5));
                    image->setPosition(Vec2(bx + di * dx,by - dj * dy));
                    layout->addChild(image);
                    image->addClickEventListener(CC_CALLBACK_1(ChatFriendsBottom::faceCallback, this));
                    
                }
            }
            else
            {
                for (int j = 0; j < 7; j ++)
                {
                    int di = j % 7;
                    int dj = j / 7;
                    Button* image = Button::create();
                    
                    if (j != 6)
                    {
                        image->setName(nameArray[faceCount]);
                        image->loadTextures(__String::createWithFormat("Chat/face/tt_e%d.png",faceCount)->getCString(),__String::createWithFormat("Chat/face/tt_e%d.png",faceCount)->getCString());
                        faceCount++;
                    }
                    else
                    {
                        image->setName("del");
                        image->loadTextures("Chat/face/tt_default_emo_back_normal.png","Chat/face/tt_default_emo_back_normal.png");
                    }
                    
                    image->ignoreContentAdaptWithSize(false);
                    image->setFillContentSizeEnabled(false);
                    image->setSwallowTouches(false);
                    image->setContentSize(Size(dx, dy));
                    image->setAnchorPoint(Vec2(0.5, 0.5));
                    image->setPosition(Vec2(bx + di * dx,by - dj * dy));
                    layout->addChild(image);
                    image->addClickEventListener(CC_CALLBACK_1(ChatFriendsBottom::faceCallback, this));
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
    
    //other area
    {
        int dx = size.width / 5;
        int dy = dx + 40;
        int bx = dx / 2;
        int by = dy * 1 - dy / 2;
        
        otherH = dy * 1;
        
        other_layer = Layer::create();
        Size otherSize = Size(size.width,otherH);
        
        other_layer->setContentSize(otherSize);
        other_layer->ignoreAnchorPointForPosition(false);
        other_layer->setAnchorPoint(Vec2(0, 0));
        other_layer->setPosition(Vec2(0, faceH - otherH));
        other_layer->setName("other_layer");
        allLayer->addChild(other_layer);
        
        LayerColor* bg = LayerColor::create(Color4B(36,36,36,255));
        bg->setContentSize(otherSize);
        bg->setAnchorPoint(Vec2(0.0f, 0.0f));
        bg->setPosition(0,0);
        other_layer->addChild(bg, -100);
        Button* btn = NULL;
        if(type == 1){
            //开房
            btn = Button::create("Chat/chat_create_room.png","Chat/chat_create_room_press.png");
            btn->setPosition(Vec2(bx,by));
            btn->addClickEventListener(CC_CALLBACK_1(ChatFriendsBottom::btnCallback, this));
            btn->setName("other_create_room");
            other_layer->addChild(btn);
            
            //战绩
            bx += dx;
            btn = Button::create("Chat/chat_zhanji.png","Chat/chat_zhanji_press.png");
            btn->setPosition(Vec2(bx,by));
            btn->addClickEventListener(CC_CALLBACK_1(ChatFriendsBottom::btnCallback, this));
            btn->setName("other_zhanji");
            other_layer->addChild(btn);
            
            //信用
            bx += dx;
            btn = Button::create("Chat/chat_xinyong.png","Chat/chat_xinyong.png");
            btn->setPosition(Vec2(bx,by));
            btn->addClickEventListener(CC_CALLBACK_1(ChatFriendsBottom::btnCallback, this));
            btn->setName("other_xinyong");
            other_layer->addChild(btn);
            bx += dx;
        }
        
        //相机
        btn = Button::create("Chat/chat_bottombar_photo.png","Chat/chat_bottombar_photo_press.png");
        btn->setPosition(Vec2(bx,by));
        btn->addClickEventListener(CC_CALLBACK_1(ChatFriendsBottom::btnCallback, this));
        btn->setName("other_photo");
        other_layer->addChild(btn);
        
        //照片
        bx += dx;
        btn = Button::create("Chat/chat_bottombar_pic.png","Chat/chat_bottombar_pic_press.png");
        btn->setPosition(Vec2(bx,by));
        btn->addClickEventListener(CC_CALLBACK_1(ChatFriendsBottom::btnCallback, this));
        btn->setName("other_img");
        other_layer->addChild(btn);
        
        //        bx += dx;
        //        btn = Button::create("Chat/chat_bottombar_coin.png",
        //                             "Chat/chat_bottombar_coin_press.png");
        //        btn->setPosition(Vec2(bx,by));
        //        btn->addClickEventListener(CC_CALLBACK_1(ChatFriendsBottom::btnCallback, this));
        //        btn->setName("other_jinbi");
        //        other_layer->addChild(btn);
        //
        //        bx += dx;
        //        btn = Button::create("Chat/chat_bottombar_shell.png",
        //                             "Chat/chat_bottombar_shell_press.png");
        //        btn->setPosition(Vec2(bx,by));
        //        btn->addClickEventListener(CC_CALLBACK_1(ChatFriendsBottom::btnCallback, this));
        //        btn->setName("other_wanbei");
        //        other_layer->addChild(btn);
    }
    
    //input area
    {
        input_layer = Layer::create();
        
        LayerColor* bg = LayerColor::create(Color4B(51,51,51,255));
        bg->setContentSize(Size(size.width,151));
        bg->setAnchorPoint(Vec2(0.0f, 0.0f));
        bg->setPosition(0,0);
        input_layer->addChild(bg, -100);
        
        //        Sprite* bg = Sprite::create("Chat/chat_bottombar_bg.png");
        //        bg->setScaleX(size.width / bg->getContentSize().width);
        //        bg->setAnchorPoint(Vec2(0.5, 0));
        //        bg->setPosition(size.width / 2, 0);
        //        input_layer->addChild(bg,-100);
        
        input_layer->setContentSize(bg->boundingBox().size);
        input_layer->ignoreAnchorPointForPosition(false);
        input_layer->setAnchorPoint(Vec2(0, 0));
        input_layer->setPosition(Vec2(0, faceH));
        allLayer->addChild(input_layer);
        
        int by = bg->getContentSize().height / 2;
        inputH = bg->getContentSize().height;
        
        btn_sound = Button::create("Chat/chat_bottombar_icon4.png",
                                   "Chat/chat_bottombar_icon4_press.png");
        btn_sound->ignoreContentAdaptWithSize(false);
        btn_sound->setFillContentSizeEnabled(false);
        btn_sound->setContentSize(Size(85 * 2, by * 2));
        btn_sound->setPosition(Vec2(85,by));
        //        btn_sound->addClickEventListener(CC_CALLBACK_1(ChatFriendsBottom::btnCallback, this));
        btn_sound->addTouchEventListener([=](Ref* r,Widget::TouchEventType type){
            if (type == Widget::TouchEventType::BEGAN)
            {
                PlatformHelper::startRecord();
            }
            else if (type == Widget::TouchEventType::ENDED || type == Widget::TouchEventType::CANCELED)
            {
                PlatformHelper::stopRecord();
                __NotificationCenter::getInstance()->postNotification("chat_sound_record_stop_notify");
            }
        });
        btn_sound->setName("chat_sound");
        input_layer->addChild(btn_sound);
        
        auto editBoxSize = Size(size.width - 155 - 285, 98);
        
        _editText = ui::EditBox::create(editBoxSize, ui::Scale9Sprite::create("Chat/bar_txt.png"));
        _editText->setAnchorPoint(Vec2(0, 0.5));
        _editText->setPosition(Vec2(155, by));
        _editText->setFontColor(Color3B::BLACK);
        //    _editText->setPlaceHolder("请输入文字");
        //    _editText->setPlaceholderFontSize(editBoxSize.height/2.5);
        //    _editText->setPlaceholderFontColor(Color3B(200,200,200));
        _editText->setMaxLength(100);
        _editText->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
        _editText->setFontSize(editBoxSize.height/2.5);
        _editText->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
        _editText->setDelegate(this);
        input_layer->addChild(_editText);
        
        btn_face = Button::create("Chat/chat_bottombar_icon3.png",
                                  "Chat/chat_bottombar_icon3_press.png");
        btn_face->setPosition(Vec2(size.width - 220,by));
        //        btn_face->ignoreContentAdaptWithSize(false);
        //        btn_face->setFillContentSizeEnabled(false);
        //        btn_face->setContentSize(Size(90 * 2, by * 2));
        btn_face->addClickEventListener(CC_CALLBACK_1(ChatFriendsBottom::btnCallback, this));
        btn_face->setName("chat_face");
        input_layer->addChild(btn_face);
        
        btn_add = Button::create("Chat/chat_bottombar_icon2.png",
                                 "Chat/chat_bottombar_icon2_press.png");
        //        btn_add->ignoreContentAdaptWithSize(false);
        //        btn_add->setFillContentSizeEnabled(false);
        //        btn_add->setContentSize(Size(85 * 2, by * 2));
        btn_add->setPosition(Vec2(size.width - 85,by));
        btn_add->addClickEventListener(CC_CALLBACK_1(ChatFriendsBottom::btnCallback, this));
        btn_add->setName("chat_add");
        input_layer->addChild(btn_add);
        
        
        btn_send = Button::create("Chat/bar_speak.png","Chat/bar_speak_press.png");
        //        btn_send->setTitleText("发送");
        btn_send->setTitleFontSize(45);
        btn_send->setScale(1.5);
        btn_send->setTitleColor(Color3B(100,100,100));
        btn_send->setVisible(false);
        btn_send->ignoreContentAdaptWithSize(false);
        btn_send->setFillContentSizeEnabled(false);
        btn_send->setPosition(Vec2(size.width - 85,by));
        btn_send->addClickEventListener(CC_CALLBACK_1(ChatFriendsBottom::btnCallback, this));
        btn_send->setName("chat_send");
        
        //        auto send_bg = ui::Scale9Sprite::create("Chat/bar_speak.png");
        //        send_bg->setPreferredSize(Size(Size(150, 80)));
        //        send_bg->setAnchorPoint(Vec2(0.5, 0.5));
        //        send_bg->setPosition(btn_send->getContentSize() / 2);
        //        btn_send->addChild(send_bg,-100);
        
        input_layer->addChild(btn_send);
    }
    
    allLayer->setContentSize(Size(size.width,inputH + faceH));
    allLayer->setPosition(Vec2(0, -faceH));
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch* touch, Event* event)
    {
        Point localP = touch->getLocation();
        
        if (this->allLayer->getBoundingBox().containsPoint(localP))
        {
            return true;
        }
        this->scollToView("null");
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatFriendsBottom::notificationReflash), "image_notify", NULL);

}

bool ChatFriendsBottom::init()
{
    if (!Layer::init())
    {
        return false;
    }
    return true;
}

void ChatFriendsBottom::faceCallback(Ref* ref)
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
            std::string send_text_result = subStr(send_text, 1);
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

void ChatFriendsBottom::btnCallback(Ref* ref)
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
        this->scollToView("face");
    }
    else if (name == "chat_send")
    {
        std::string send_text = _editText->getText();
        if(send_text == "")return;
        Content_Type contentType = CONTENT_CLUB;
        if(type == 0){
            contentType = CONTENT_POINT_TO_POINT;
        }
        MsgModel::getInstance()->sendText(send_text,contentType);
        _editText->setText("");
        checkContent();
        
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
    else if (name == "other_create_room"){
        if(m_pCallback)m_pCallback("create_room");
    }else{
        if(m_pCallback)m_pCallback(name);
    }
}

void ChatFriendsBottom::notificationReflash(Ref* msg)
{
    __String* str = (__String*)msg;
    if (str != NULL)
    {
        std::string s = str->getCString();
        Content_Type contentType = CONTENT_CLUB;
        if(type == 0){
            contentType = CONTENT_POINT_TO_POINT;
        }
        MsgModel::getInstance()->sendImg(s,contentType);
    }
}

void ChatFriendsBottom::checkContent()
{
    std::string text = _editText->getText();
    if (text == "")
    {
        btn_send->setVisible(false);
        btn_add->setVisible(true);
    }
    else
    {
        btn_send->setVisible(true);
        btn_add->setVisible(false);
    }
}

void ChatFriendsBottom::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
    log("editBox %p DidBegin !", editBox);
}

void ChatFriendsBottom::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    log("editBox %p DidEnd !", editBox);
}

void ChatFriendsBottom::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    log("editBox %p TextChanged, text: %s ", editBox, text.c_str());
    if (text == "")
    {
        btn_send->setVisible(false);
        btn_add->setVisible(true);
    }
    else
    {
        btn_send->setVisible(true);
        btn_add->setVisible(false);
    }
}

void ChatFriendsBottom::editBoxReturn(ui::EditBox* editBox)
{
    log("editBox %p was returned !",editBox);
    
    if (_editText == editBox)
    {
//        std::string send_text = _editText->getText();
//        if(send_text == "")return;
//        MsgModel::getInstance()->sendText(send_text);
//        _editText->setText("");
//        checkContent();
//        
//        this->scollToView("null");
    }
}

void ChatFriendsBottom::scollToView(std::string v)
{
    if (v == "null")
    {
        allLayer->setPosition(Vec2(0, -faceH));
    }
    else if (v == "face")
    {
        allLayer->setPosition(Vec2(0, 0));
        face_layer->setVisible(true);
        other_layer->setVisible(false);
    }
    else if (v == "other")
    {
        allLayer->setPosition(Vec2(0, -faceH + otherH));
        face_layer->setVisible(false);
        other_layer->setVisible(true);
    }
}

void ChatFriendsBottom::onExit()
{
    __NotificationCenter::getInstance()->removeAllObservers(this);
    Layer::onExit();
}
