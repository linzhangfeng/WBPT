//
//  RoomChatBottom.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "RoomChatBottom.hpp"
#include "../../../../Model/MsgModel.h"
#include "proto.h"

std::string subStr_roomchat(std::string m_pInputText,int subNums)
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

bool RoomChatBottom::init()
{
    if (!Layer::init())
    {
        return false;
    }
    fastChatPopWindow = NULL;
    emojiPopWindow = NULL;
    ss = 0.67;
    
    Size size = Director::getInstance()->getWinSize();
    
    allLayer = Layer::create();
    allLayer->ignoreAnchorPointForPosition(false);
    allLayer->setAnchorPoint(Vec2(0, 0));
    allLayer->setPosition(Vec2(0, 0));
    addChild(allLayer);
    
    
    
    //other area
    {
        int dx = size.width / 4;
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
        allLayer->addChild(other_layer);
        
        LayerColor* bg = LayerColor::create(Color4B(36,36,36,255));
        bg->setContentSize(otherSize);
        bg->setAnchorPoint(Vec2(0.0f, 0.0f));
        bg->setPosition(0,0);
        other_layer->addChild(bg, -100);
        
        Button* btn = Button::create("Chat/chat_bottombar_photo.png",
                                 "Chat/chat_bottombar_photo_press.png");
        btn->setPosition(Vec2(bx,by));
        btn->addClickEventListener(CC_CALLBACK_1(RoomChatBottom::btnCallback, this));
        btn->setName("other_photo");
        btn->setScale(ss);
        other_layer->addChild(btn);
        
        bx += dx;
        btn = Button::create("Chat/chat_bottombar_pic.png",
                             "Chat/chat_bottombar_pic_press.png");
        btn->setPosition(Vec2(bx,by));
        btn->addClickEventListener(CC_CALLBACK_1(RoomChatBottom::btnCallback, this));
        btn->setName("other_img");
        btn->setScale(ss);
        other_layer->addChild(btn);
        
//        bx += dx;
//        btn = Button::create("Chat/chat_bottombar_coin.png",
//                             "Chat/chat_bottombar_coin_press.png");
//        btn->setPosition(Vec2(bx,by));
//        btn->addClickEventListener(CC_CALLBACK_1(RoomChatBottom::btnCallback, this));
//        btn->setName("other_jinbi");
//        btn->setScale(ss);
//        other_layer->addChild(btn);
//        
//        bx += dx;
//        btn = Button::create("Chat/chat_bottombar_shell.png",
//                             "Chat/chat_bottombar_shell_press.png");
//        btn->setPosition(Vec2(bx,by));
//        btn->addClickEventListener(CC_CALLBACK_1(RoomChatBottom::btnCallback, this));
//        btn->setName("other_wanbei");
//        btn->setScale(ss);
//        other_layer->addChild(btn);
    }
    
    //input area
    {
        input_layer = Layout::create();
        
        LayerColor* bg = LayerColor::create(Color4B(51,51,51,255));
        bg->setContentSize(Size(size.width,151*ss));
        bg->setAnchorPoint(Vec2(0.0f, 0.0f));
        bg->setPosition(0,0);
        input_layer->addChild(bg, -100);
        
        input_layer->setContentSize(bg->boundingBox().size);
        input_layer->ignoreAnchorPointForPosition(false);
        input_layer->setAnchorPoint(Vec2(0, 0));
        input_layer->setPosition(Vec2(0, faceH));
        input_layer->setTouchEnabled(true);
        input_layer->addTouchEventListener([=](Ref* r,Widget::TouchEventType t){
        });
        allLayer->addChild(input_layer);
        
        int by = bg->boundingBox().size.height / 2;
        inputH = bg->boundingBox().size.height;
        
        //按住聊天按钮
//        btn_sound = Button::create("Chat/chat_bottombar_icon4.png", "Chat/chat_bottombar_icon4_press.png");
//        btn_sound->ignoreContentAdaptWithSize(false);
//        btn_sound->setFillContentSizeEnabled(false);
//        btn_sound->setContentSize(Size(85 * 2, by * 2));
//        btn_sound->setPosition(Vec2(85 * ss,by));
////        btn_sound->addClickEventListener(CC_CALLBACK_1(RoomChatBottom::btnCallback, this));
//        btn_sound->setName("chat_sound");
//        btn_sound->setScale(ss);
//        btn_sound->addTouchEventListener([=](Ref* r,Widget::TouchEventType type){
//            if (type == Widget::TouchEventType::BEGAN)
//            {
//                PlatformHelper::startRecord();
//            }
//            else if (type == Widget::TouchEventType::ENDED || type == Widget::TouchEventType::CANCELED)
//            {
//                PlatformHelper::stopRecord();
//                __NotificationCenter::getInstance()->postNotification("chat_sound_record_stop_notify");
//            }
//        });
//        input_layer->addChild(btn_sound);
        
        auto editBoxSize = Size(493, 67);
        
        _editText = ui::EditBox::create(editBoxSize, ui::Scale9Sprite::create("RoomPublicV/room_chat_input_bg.png"));
        _editText->setAnchorPoint(Vec2(0, 0.5));
        _editText->setPosition(Vec2(24, by));
        _editText->setFontColor(Color3B::WHITE);
        //    _editText->setPlaceHolder("请输入文字");
        //    _editText->setPlaceholderFontSize(editBoxSize.height/2.5);
        //    _editText->setPlaceholderFontColor(Color3B(200,200,200));
        _editText->setMaxLength(100);
        _editText->setFontSize(editBoxSize.height/2.5);
        _editText->setInputMode(EditBox::InputMode::SINGLE_LINE);
        _editText->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
        _editText->setDelegate(this);
        input_layer->addChild(_editText);
        
        btn_face = Button::create("Chat/chat_bottombar_icon3.png","Chat/chat_bottombar_icon3_press.png");
        btn_face->setPosition(Vec2(size.width - 220 * ss,by));
//        btn_face->ignoreContentAdaptWithSize(false);
//        btn_face->setFillContentSizeEnabled(false);
//        btn_face->setContentSize(Size(90 * 2, by * 2));
        btn_face->addClickEventListener(CC_CALLBACK_1(RoomChatBottom::btnCallback, this));
        btn_face->setName("chat_face");
        btn_face->setScale(ss);
        input_layer->addChild(btn_face);
        
        //工具栏更多 按钮
//        btn_add = Button::create("Chat/chat_bottombar_icon2.png",
//                                 "Chat/chat_bottombar_icon2_press.png");
//        btn_add->ignoreContentAdaptWithSize(false);
//        btn_add->setFillContentSizeEnabled(false);
//        btn_add->setContentSize(Size(85 * 2, by * 2));
//        btn_add->setPosition(Vec2(size.width - 85 * ss,by));
//        btn_add->addClickEventListener(CC_CALLBACK_1(RoomChatBottom::btnCallback, this));
//        btn_add->setName("chat_add");
//        btn_add->setScale(ss);
//        input_layer->addChild(btn_add);
        
        //快速聊天按钮
        btn_add = Button::create("RoomPublicV/fast_text_btn.png","RoomPublicV/fast_text_btn_press.png");
        btn_add->setAnchorPoint(Vec2(1,0.5));
        btn_add->setPosition(Vec2(size.width - 24,by));
        btn_add->setName("fast_chat_text");
        btn_add->addClickEventListener(CC_CALLBACK_1(RoomChatBottom::btnCallback, this));
        input_layer->addChild(btn_add);
        
        btn_send = Button::create("Chat/bar_speak.png","Chat/bar_speak_press.png");
        btn_send->setTitleFontSize(45);
        btn_send->setTitleColor(Color3B(100,100,100));
        btn_send->setVisible(false);
        btn_send->ignoreContentAdaptWithSize(false);
        btn_send->setFillContentSizeEnabled(false);
        btn_send->setContentSize(Size(150, by * 2));
        btn_send->setPosition(Vec2(size.width - 85 * ss,by));
        btn_send->addClickEventListener(CC_CALLBACK_1(RoomChatBottom::btnCallback, this));
        btn_send->setName("chat_send");
        
        _editText->setContentSize(Size(input_layer->getContentSize().width - btn_send->getContentSize().width - btn_face->getContentSize().width,editBoxSize.height));
        
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
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomChatBottom::notificationReflash), "image_notify", NULL);
    return true;
}

void RoomChatBottom::faceCallback(Ref* ref)
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
            std::string send_text_result = subStr_roomchat(send_text, 1);
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

void RoomChatBottom::btnCallback(Ref* ref)
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
        MsgModel::getInstance()->sendText_group(send_text);
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
    else if(name == "fast_chat_text"){
        //显示快速聊天文本弹窗
        showFastChatPopWindow();
    }
}

bool RoomChatBottom::isInputEmpty(){
    std::string text = _editText->getText();
    if (text == "")
    {
        return true;
    }
    return false;
}

//显示快速聊天文本弹出框
void RoomChatBottom::showFastChatPopWindow(){
    if(fastChatPopWindow == NULL){
        std::vector<std::string> strs;
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
        fastChatPopWindow = Layout::create();
        fastChatPopWindow->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        fastChatPopWindow->setBackGroundColor(Color3B(0, 0, 0));
        fastChatPopWindow->setOpacity(0);
        fastChatPopWindow->setContentSize(Director::getInstance()->getWinSize());
        fastChatPopWindow->setPosition(Vec2(0, 0));
        fastChatPopWindow->setTouchEnabled(true);
        fastChatPopWindow->addClickEventListener([=](Ref*ref){
            fastChatPopWindow->setVisible(false);
        });
        addChild(fastChatPopWindow,1001);
        ImageView* bgImage = ImageView::create("RoomPublicV/pop_window_bg.png");
        bgImage->setScale9Enabled(true);
        bgImage->setContentSize(Size(bgImage->getContentSize().width * fastChatPopWindow->getContentSize().width / 720,bgImage->getContentSize().height));
        bgImage->setAnchorPoint(Vec2(0.5,0));
        bgImage->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2,93));
        
        fastChatPopWindow->addChild(bgImage);
        ListView* listView = ListView::create();
        listView->setDirection(ui::ScrollView::Direction::VERTICAL);
        listView->setBounceEnabled(false);
        listView->setContentSize(Size(bgImage->getContentSize().width,373));
        listView->setItemsMargin(12);
        listView->setAnchorPoint(Vec2(0, 1));
        listView->setPosition(Vec2(0, bgImage->getContentSize().height - 17));
        listView->setScrollBarEnabled(false);
        listView->setName("fast_list");
        bgImage->addChild(listView);
        for (int i = 0; i < strs.size(); )
        {
            //整个一行item由2个小的item组合而成
            auto lineLayout = Layout::create();
            lineLayout->setContentSize(Size(listView->getContentSize().width,68));
            lineLayout->setAnchorPoint(Vec2(0,0));
            lineLayout->setPosition(Vec2(0,0));
            //单个item布局
            auto layout = Layout::create();
            layout->setContentSize(Size(lineLayout->getContentSize().width / 2,68));
            layout->setAnchorPoint(Vec2(0,0));
            layout->setPosition(Vec2(0,0));
            
            ImageView* bgImageLeft = ImageView::create("RoomPublicV/room_editor_bg.png");
            bgImageLeft->setContentSize(Size(lineLayout->getContentSize().width / 2 - 10,bgImageLeft->getContentSize().height));
            bgImageLeft->setScale9Enabled(true);
            bgImageLeft->setAnchorPoint(Vec2(0,0));
            bgImageLeft->setPosition(Vec2(10,0));
            
            char buf[128];
            Utils::parseName(9, buf, strs[i].c_str());
            Text* text = Text::create(buf, "AmericanTypewriter", 30);
            text->setAnchorPoint(Vec2(0, 0.5));
            text->setColor(Color3B(169,169,169));
            text->setPosition(Vec2(22, layout->getContentSize().height / 2));
            
            
            layout->addChild(bgImageLeft);
            layout->addChild(text);
            layout->setTouchEnabled(true);
            layout->addClickEventListener([=](Ref* rf){
                MsgModel::getInstance()->sendText_group(strs[i]);
                fastChatPopWindow->setVisible(false);
            });
            lineLayout->addChild(layout);
            
            //右边单个item
            i++;
            if(i < strs.size()){
                auto layoutRight = Layout::create();
                layoutRight->setContentSize(Size(lineLayout->getContentSize().width / 2,68));
                layoutRight->setAnchorPoint(Vec2(0,0));
                layoutRight->setPosition(Vec2(lineLayout->getContentSize().width / 2,0));
                
                ImageView* bgRightImage = ImageView::create("RoomPublicV/room_editor_bg.png");
                bgRightImage->setScale9Enabled(true);
                bgRightImage->setContentSize(Size(lineLayout->getContentSize().width / 2 - 20,bgRightImage->getContentSize().height));
                bgRightImage->setAnchorPoint(Vec2(0,0));
                bgRightImage->setPosition(Vec2(10,0));
                
                Utils::parseName(9, buf, strs[i].c_str());
                Text* textRight = Text::create(buf, "AmericanTypewriter", 30);
                textRight->setAnchorPoint(Vec2(0, 0.5));
                textRight->setColor(Color3B(169,169,169));
                textRight->setPosition(Vec2(22, layout->getContentSize().height / 2));
                
                
                layoutRight->addChild(bgRightImage);
                layoutRight->addChild(textRight);
                layoutRight->setTouchEnabled(true);
                layoutRight->addClickEventListener([=](Ref* rf){
                    MsgModel::getInstance()->sendText_group(strs[i]);
                    fastChatPopWindow->setVisible(false);
                });
                lineLayout->addChild(layoutRight);
            }

            listView->insertCustomItem(lineLayout, 0);
            i++;
        }
    }else{
        if(fastChatPopWindow->isVisible()){
            fastChatPopWindow->setVisible(false);
        }else{
            fastChatPopWindow->setVisible(true);
        }
        
    }
    
}

//显示表情弹窗
void RoomChatBottom::showEmojiPopWindow(){
    if(emojiPopWindow == NULL){
        emojiPopWindow = Layout::create();
        emojiPopWindow->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        emojiPopWindow->setBackGroundColor(Color3B(0, 0, 0));
        emojiPopWindow->setOpacity(0);
        emojiPopWindow->setContentSize(Director::getInstance()->getWinSize());
        emojiPopWindow->setPosition(Vec2(0, 0));
        emojiPopWindow->setTouchEnabled(true);
        emojiPopWindow->addClickEventListener([=](Ref*ref){
            emojiPopWindow->setVisible(false);
        });
        addChild(emojiPopWindow,1001);
        ImageView* bgImage = ImageView::create("RoomPublicV/emoji_bg.png");
        bgImage->setScale9Enabled(true);
        bgImage->setContentSize(Size(bgImage->getContentSize().width * fastChatPopWindow->getContentSize().width / 720,bgImage->getContentSize().height));
        bgImage->setAnchorPoint(Vec2(0.5,0));
        bgImage->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2,93));
        Size size = Director::getInstance()->getWinSize();

        //face area
        {
            int dx = bgImage->getContentSize().width / 7;
            int dy = 305 / 3;
            int bx = 0;
            int by = 305 - dy;
            
            faceH = dy * 3;
            
            face_layer = Layer::create();
            Size faceSize = Size(bgImage->getContentSize().width,305);
            
            face_layer->setContentSize(faceSize);
            face_layer->ignoreAnchorPointForPosition(false);
            face_layer->setAnchorPoint(Vec2(0, 1));
            face_layer->setPosition(Vec2(0,bgImage->getContentSize().height));
            
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
                        image->addClickEventListener(CC_CALLBACK_1(RoomChatBottom::faceCallback, this));
                        
                        if (j != 20)
                        {
                            image->setName(nameArray[faceCount]);
                            Sprite* icon = Sprite::create(__String::createWithFormat("Chat/face/tt_e%d.png",faceCount)->getCString());
                            icon->setScale(ss);
                            icon->setPosition(image->getContentSize() / 2);
                            image->addChild(icon);
                            faceCount++;
                        }
                        else
                        {
                            image->setName("del");
                            Sprite* icon = Sprite::create("Chat/face/tt_default_emo_back_normal.png");
                            icon->setScale(ss);
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
                        image->addClickEventListener(CC_CALLBACK_1(RoomChatBottom::faceCallback, this));
                        
                        if (j != 6)
                        {
                            image->setName(nameArray[faceCount]);
                            Sprite* icon = Sprite::create(__String::createWithFormat("Chat/face/tt_e%d.png",faceCount)->getCString());
                            icon->setScale(ss);
                            icon->setPosition(image->getContentSize() / 2);
                            image->addChild(icon);
                            faceCount++;
                        }
                        else
                        {
                            image->setName("del");
                            Sprite* icon = Sprite::create("Chat/face/tt_default_emo_back_normal.png");
                            icon->setScale(ss);
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
        
        bgImage->addChild(face_layer);
        emojiPopWindow->addChild(bgImage);
    }else{
        if(emojiPopWindow->isVisible()){
            emojiPopWindow->setVisible(false);
        }else{
            emojiPopWindow->setVisible(true);
        }
    }
}

void RoomChatBottom::notificationReflash(Ref* msg)
{
    __String* str = (__String*)msg;
    if (str != NULL)
    {
        std::string s = str->getCString();
        MsgModel::getInstance()->sendImg_group(s);
    }
}

void RoomChatBottom::checkContent()
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

void RoomChatBottom::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
    log("editBox %p DidBegin !", editBox);
}

void RoomChatBottom::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    log("editBox %p DidEnd !", editBox);
}

void RoomChatBottom::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
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

void RoomChatBottom::editBoxReturn(ui::EditBox* editBox)
{
    log("editBox %p was returned !",editBox);
    
    if (_editText == editBox)
    {
//        std::string send_text = _editText->getText();
//        if(send_text == "")return;
//        MsgModel::getInstance()->sendText_group(send_text);
//        _editText->setText("");
//        checkContent();
//        
//        this->scollToView("null");
    }
}

void RoomChatBottom::scollToView(std::string v)
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
        //弹出表情弹窗
        showEmojiPopWindow();
    }
    else if (v == "other")
    {
        allLayer->setPosition(Vec2(0, -faceH + otherH));
        face_layer->setVisible(false);
        other_layer->setVisible(true);
    }
}

void RoomChatBottom::onExit()
{
    __NotificationCenter::getInstance()->removeAllObservers(this);
    Layer::onExit();
}

void RoomChatBottom::onEnter(){
    Layer::onEnter();
//    showFastChatPopWindow();
}
