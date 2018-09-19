//
//  AddFriend.cpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/15.
//
//

#include "AddFriends.hpp"

bool AddFriend::init(){
    if (!BaseLayer::init()) {
        return false;
    }
    
    size = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(20, 20, 22, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    
    setName("AddFriend");
    
    setTitle("添加好友");
    
    setMore_txt("黑名单", [=](){
        ZJHModel::getInstance()->gotoView(VIEW_FRIEND_BLACK);
    });
    
    _shareJson = ZJHModel::getInstance()->gameConfigJson["list"]["recommend"];
    
    initView();
    
    return true;
}

//微信分享
void AddFriend::getShareMessage(){
    string title = _shareJson["title"].asString();
    string imgurl = _shareJson["img"].asString();
    string url = _shareJson["url"].asString();
    
    PlatformHelper::WeChatShare(title.c_str(),"",url.c_str(),"");
}


void AddFriend::initView(){
    float by = size.height - getHeadH();
    float dy = 25;
    Layout* topserLay = AddFriend::getSearchFriend();
    by = by - dy - topserLay->getContentSize().height;
    topserLay->setPosition(Vec2(0,by));
    this->addChild(topserLay);
    
    Layout* lay1 = AddFriend::getAddImageFriend();
    by = by - dy - lay1->getContentSize().height;
    lay1->setPosition(Vec2(0,by));
    this->addChild(lay1);
    
    Layout* lay2 = AddFriend::getScanFriend();
    by = by - dy - lay2->getContentSize().height;
    lay2->setPosition(Vec2(0,by));
    this->addChild(lay2);
    
    Layout* lay3 = AddFriend::getNearFriend();
    by = by - dy - lay3->getContentSize().height;
    lay3->setPosition(Vec2(0,by));
    this->addChild(lay3);
    
}

Layout* AddFriend::getSearchFriend(){
    Layout* serLay = Layout::create();
    serLay->setContentSize(Size(size.width,180));
    serLay->setAnchorPoint(Vec2(0,0));
    
    LayerColor* click_lay = LayerColor::create(Color4B(29,29,31, 255));
    click_lay->setContentSize(serLay->getContentSize());
    click_lay->setAnchorPoint(Vec2(0.0f, 0.0f));
    click_lay->setPosition(Vec2(0, 0));
    serLay->addChild(click_lay, -100);
    
    click_lay = LayerColor::create(Color4B(195,195,195, 255));
    click_lay->setContentSize(Size(size.width - 41 * 2,90));
    click_lay->setAnchorPoint(Vec2(0.0f, 0.0f));
    click_lay->setPosition(Vec2(41, (180 - 90) / 2));
    serLay->addChild(click_lay, -100);
    
    serLay->setTouchEnabled(true);
    serLay->addClickEventListener([=](Ref*){
        ZJHModel::getInstance()->gotoView(VIEW_FOUND_FRIENDS);
    });
    
    ImageView* button = ImageView::create("Head/title-icon4.png");
    button->setAnchorPoint(Vec2(0.5,0.5));
    button->setPosition(Vec2(40,45));
    click_lay->addChild(button);
    
    Text *tex1 = Text::create("用户ID／用户昵称", "AmericanTypewriter", 40);
    tex1->setAnchorPoint(Vec2(0.0, 0.5));
    tex1->setColor(Color3B(139,139,139));
    tex1->setPosition(button->getPosition() + Vec2(40, 0));
    click_lay->addChild(tex1);
    return serLay;
}


Layout* AddFriend::getAddImageFriend(){
    
    Size itemSize = Size(size.width,276);
    Layout* serLay = Layout::create();
    serLay->setContentSize(itemSize);
    serLay->setAnchorPoint(Vec2(0,0));
    
    LayerColor* click_lay = LayerColor::create(Color4B(29,29,31, 255));
    click_lay->setContentSize(serLay->getContentSize());
    click_lay->setAnchorPoint(Vec2(0.0f, 0.0f));
    click_lay->setPosition(Vec2(0, 0));
    serLay->addChild(click_lay, -100);
    
    int singleWidth = size.width/3;
    int bx = singleWidth / 2;
    int by = itemSize.height / 2;
    
    Layout* lay1 = Layout::create();
    lay1->setContentSize(Size(singleWidth,itemSize.height));
    lay1->setAnchorPoint(Vec2(0,0));
    lay1->setTouchEnabled(true);
    lay1->addClickEventListener([=](Ref*){
        PlatformHelper::showToast("暂未开放，敬请期待");
    });
    
    Button *image1 = Button::create("Msg/icon_big_01.png","Msg/icon_big_01_press.png","Msg/icon_big_01.png");
    image1->setAnchorPoint(Vec2(0.5,0.5));
    image1->setPosition(Vec2(bx,by + 37));
    lay1->addChild(image1);
    
    Text *tex1 = Text::create("添加手机联系人", "AmericanTypewriter", 40);
    tex1->setAnchorPoint(Vec2(0.5, 0.5));
    tex1->setPosition(Vec2(bx, 60));
    tex1->setColor(Color3B(224,203,174));
    lay1->addChild(tex1);
    lay1->setPosition(Vec2(0,0));
    serLay->addChild(lay1);
    
    Layout* lay2 = Layout::create();
    lay2->setContentSize(Size(singleWidth,itemSize.height));
    lay2->setAnchorPoint(Vec2(0,0));
    lay2->setTouchEnabled(true);
    lay2->addClickEventListener([=](Ref*){
//      PlatformHelper::showToast("暂未开放，敬请期待");
        getShareMessage();
    });
    
    Button *image2 = Button::create("Msg/icon_big_02.png","Msg/icon_big_02_press.png","Msg/icon_big_02.png");
    image2->setAnchorPoint(Vec2(0.5,0.5));
    image2->setPosition(Vec2(bx,by + 37));
    lay2->addChild(image2);
    
    Text *tex2 = Text::create("添加微信好友", "AmericanTypewriter", 40);
    tex2->setAnchorPoint(Vec2(0.5, 0.5));
    tex2->setPosition(Vec2(bx, 60));
    tex2->setColor(Color3B(224,203,174));
    lay2->addChild(tex2);
    lay2->setPosition(Vec2(singleWidth,0));
    serLay->addChild(lay2);
    
    
    Layout* lay3 = Layout::create();
    lay3->setContentSize(Size(singleWidth,itemSize.height));
    lay3->setAnchorPoint(Vec2(0,0));
    lay3->setTouchEnabled(true);
    lay3->addClickEventListener([=](Ref*){
        PlatformHelper::showToast("暂未开放，敬请期待");
    });
    
    Button *image3 = Button::create("Msg/icon_big_03.png","Msg/icon_big_03_press.png","Msg/icon_big_03.png");
    image3->setAnchorPoint(Vec2(0.5,0.5));
    image3->setPosition(Vec2(bx,by + 37));
    lay3->addChild(image3);
    
    Text *tex3 = Text::create("添加QQ好友", "AmericanTypewriter", 40);
    tex3->setAnchorPoint(Vec2(0.5, 0.5));
    tex3->setPosition(Vec2(bx, 60));
    tex3->setColor(Color3B(224,203,174));
    lay3->addChild(tex3);
    lay3->setPosition(Vec2(singleWidth*2,0));
    serLay->addChild(lay3);
    
    LayerColor* line = LayerColor::create(Color4B(50,50,50, 255));
    line->setContentSize(Size(5,80));
    line->setAnchorPoint(Vec2(0.5f, 0.0f));
    line->setPosition(Vec2(singleWidth, 99));
    serLay->addChild(line, 100);
    
    line = LayerColor::create(Color4B(50,50,50, 255));
    line->setContentSize(Size(5,80));
    line->setAnchorPoint(Vec2(0.5f, 0.0f));
    line->setPosition(Vec2(singleWidth * 2, 99));
    serLay->addChild(line, 100);
    return serLay;
}

Layout* AddFriend::getScanFriend(){
    
    Size itemSize = Size(size.width,133);
    
    Layout* serLay = Layout::create();
    serLay->setContentSize(itemSize);
    serLay->setAnchorPoint(Vec2(0,0));
    serLay->setTouchEnabled(true);
    serLay->addClickEventListener([=](Ref*){
        PlatformHelper::showToast("暂未开放，敬请期待");
    });
    
    LayerColor* click_lay = LayerColor::create(Color4B(29,29,31, 255));
    click_lay->setContentSize(itemSize);
    click_lay->setAnchorPoint(Vec2(0.0f, 0.0f));
    click_lay->setPosition(Vec2(0, 0));
    serLay->addChild(click_lay, -100);
    
    ImageView *imageleft = ImageView::create("Msg/scan_icon.png");
    imageleft->setPosition(Vec2(78,itemSize.height/2));
    serLay->addChild(imageleft);
    
    Text *tex = Text::create("扫一扫加好友", "AmericanTypewriter", 40);
    tex->setAnchorPoint(Vec2(0, 0.5));
    tex->setPosition(Vec2(125,itemSize.height/2));
    tex->setColor(Color3B(105,105,105));
    serLay->addChild(tex);
    
    ImageView *imageright = ImageView::create("Msg/icon_more.png");
    imageright->setAnchorPoint(Vec2(0.5,0.5));
    imageright->setPosition(Vec2(size.width - 60 ,itemSize.height / 2));
    serLay->addChild(imageright);
    
    serLay->addClickEventListener([=](Ref *ref){
        PlatformHelper::showToast("暂未开放");
    });
    
    return serLay;
}

Layout* AddFriend::getNearFriend(){
    Size itemSize = Size(size.width,133);
    
    Layout* serLay = Layout::create();
    serLay->setContentSize(itemSize);
    serLay->setAnchorPoint(Vec2(0,0));
    serLay->setTouchEnabled(true);
    serLay->addClickEventListener([=](Ref*){
        PlatformHelper::showToast("暂未开放，敬请期待");
    });
    
    LayerColor* click_lay = LayerColor::create(Color4B(29,29,31, 255));
    click_lay->setContentSize(itemSize);
    click_lay->setAnchorPoint(Vec2(0.0f, 0.0f));
    click_lay->setPosition(Vec2(0, 0));
    serLay->addChild(click_lay, -100);
    
    ImageView *imageleft = ImageView::create("Msg/found_icon.png");
    imageleft->setPosition(Vec2(78,itemSize.height / 2));
    serLay->addChild(imageleft);
    
    Text *tex = Text::create("发现附近的好友", "AmericanTypewriter", 40);
    tex->setAnchorPoint(Vec2(0, 0.5));
    tex->setPosition(Vec2(125,itemSize.height / 2));
    tex->setColor(Color3B(105,105,105));
    serLay->addChild(tex);
    
    ImageView *imageright = ImageView::create("Msg/icon_more.png");
    imageright->setAnchorPoint(Vec2(0.5,0.5));
    imageright->setPosition(Vec2(size.width - 60 ,itemSize.height / 2));
    serLay->addChild(imageright);
    
    serLay->addClickEventListener([=](Ref *ref){
        ZJHModel::getInstance()->gotoView(VIEW_NEAR_FRIENDS);
    });

    return serLay;
}


Layout* AddFriend::getLineLayout(int height)
{
    Layout* linelay = Layout::create();
    linelay->setContentSize(Size(size.width,height));
    linelay->setAnchorPoint(Vec2(0,0));
    LayerColor* clo = LayerColor::create(Color4B(122,122,122,122));
    clo->setContentSize(Size(size.width,height));
    linelay->addChild(clo);
    return linelay;
}

void AddFriend::editBoxEditingDidBegin(EditBox* editBox)
{
    ZJHModel::getInstance()->gotoView(VIEW_FOUND_FRIENDS);
    log("editBox %p DidBegin.", editBox);
    
}

void AddFriend::editBoxEditingDidEnd(EditBox* editBox)
{
    log("editBox %p DidEnd.", editBox);
}

void AddFriend::editBoxTextChanged(EditBox* editBox, const std::string &text)
{
    
}

void AddFriend::editBoxReturn(EditBox *editBox)
{
    log("editBox %p was returned.", editBox);
}
