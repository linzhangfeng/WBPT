//
//  ChatFriendsMore_Jubao.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "ChatFriendsMore_Jubao.hpp"
#include "../../Model/MsgModel.h"
#include "proto.h"

bool ChatFriendsMore_Jubao::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setName("ChatFriendsMore_Jubao");
    
    size = Director::getInstance()->getWinSize();
    
    return true;
}



void ChatFriendsMore_Jubao::myInit(Json::Value &json)
{
    int uid = json["uid"].asInt();
    
    setBackground(Color4B(200, 200, 200, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png",[=](){
        this->disappear();
    });
    
    setTitle("举报");
    
    JBListView = ListView::create();
    JBListView->setDirection(ui::ScrollView::Direction::VERTICAL);
    JBListView->setBounceEnabled(false);
    JBListView->setContentSize(Size(size.width,760));
    JBListView->setAnchorPoint(Vec2(0.5, 1));
    JBListView->setPosition(Vec2(size.width/2, size.height - this->getSpriteHeight() - 15));
    JBListView->setScrollBarEnabled(false);
    JBListView->setItemsMargin(0);
    JBListView->setColor(Color3B::WHITE);
    
    this->addChild(JBListView);
    
    addListData();
    oldindex = -1;
    
    Button* button = Button::create("Public/btn1.png","Public/btn2.png");
    button->setContentSize(Size(size.width-200,120));
    button->setAnchorPoint(Vec2(0.5, 0.5));
    button->setScale9Enabled(true);
    button->setTitleColor(Color3B::GREEN);
    button->setTitleText("提交");
    button->setTitleFontSize(50);
    button->setPosition(Vec2(size.width/2,size.height - this->getSpriteHeight()-860));
    button->addClickEventListener([=](Ref*){
        //举报的提交
        
    });
    this->addChild(button);
    
}
void ChatFriendsMore_Jubao::addListData()
{
    JBListItem* item = NULL;
    for(int index= 0;index<5;index++){

        item = JBListItem::createWithData(index,size.width,[=](int index){
            cgettitleColor(oldindex,index);
        });
        JBListView->addChild(item);
    }
    
}


void ChatFriendsMore_Jubao::cgettitleColor(int oindex,int nindex)
{
    if(oindex != -1){
        Layout* lay = (Layout*)JBListView->getItem(oindex);
        Text* te = (Text*)lay->getChildByName("jbname");
        if(te != NULL){
            te->setColor(Color3B::BLACK);
        }
        
        ImageView* view = (ImageView*)lay->getChildByName("jbsel");
        if(view != NULL){
            view->setVisible(false);
        }
    }
    Layout* lay = (Layout*)JBListView->getItem(nindex);
    Text* te = (Text*)lay->getChildByName("jbname");
    if(te != NULL){
        te->setColor(Color3B::GREEN);
    }
    
    ImageView* view = (ImageView*)lay->getChildByName("jbsel");
    if(view != NULL){
        view->setVisible(true);
    }
    oldindex = nindex;
}

JBListItem *JBListItem::createWithData(int index,int width,const MyCallBack_Int &callback)
{
    JBListItem *pRet = new JBListItem();
    if (pRet && pRet->initWithData(index,width,callback)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool JBListItem::initWithData(int index,int width,const MyCallBack_Int &callback)
{
    if (Layout::init() == false) {
        return false;
    }
    
    setTouchEnabled(true);
    
    Size itemSize = Size(width, 150);
    
    LayerColor* clo = LayerColor::create(Color4B(255,255,255,255));
    clo->setContentSize(itemSize);
    this->addChild(clo);


    this->setContentSize(itemSize);
    this->setAnchorPoint(Vec2(0,0));
    
    string stip = "";
    switch(index){
        case 0:
            stip="聚众赌博";
            break;
        case 1:
            stip="内容涉黄";
            break;
        case 2:
            stip="垃圾广告";
            break;
        case 3:
            stip="谣言欺骗";
            break;
        case 4:
            stip="不当言辞";
            break;
    }
    
    //文字
    Text *name = Text::create(stip, "AmericanTypewriter", 40);
    name->setAnchorPoint(Vec2(0, 0.5));
    name->setPosition(Vec2(50, 75));
    name->setName("jbname");
    name->setColor(Color3B::BLACK);
    this->addChild(name);
    
    //图片的
    ImageView *image_lv = ImageView::create("Mine/view_mark.png");
    image_lv->setContentSize(Size(60,60));
    image_lv->setAnchorPoint(Vec2(0.5, 0.5));
    image_lv->setName("jbsel");
    image_lv->setPosition(Vec2((width- 200), 75));
    image_lv->setTouchEnabled(false);
    image_lv->ignoreContentAdaptWithSize(false);
    image_lv->setVisible(false);
    this->addChild(image_lv);
    
    Layout* botLine = getLineLayout(width-100,2);
    botLine->setPosition(Vec2(50, 10));
    this->addChild(botLine);
    
    this->addClickEventListener([=](Ref* ref){
        if(callback != NULL){
            callback(index);
        }
    });
    
    return true;
}

Layout* JBListItem::getLineLayout(int width,int height)
{
    Layout* linelay = Layout::create();
    linelay->setContentSize(Size(width,height));
    linelay->setAnchorPoint(Vec2(0,0));
    LayerColor* clo = LayerColor::create(Color4B(122,122,122,122));
    clo->setContentSize(Size(width,height));
    linelay->addChild(clo);
    return linelay;
}


void ChatFriendsMore_Jubao::sendMessCallback(){
    Json::Value json_s;
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
        
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        loginPacket->status = 0;
        
        if (loginPacket->resultIsOK())
        {
            /*
             {"errorType":"SUCCESS","resultMap":{"hasMore":false,"friendType":3,"count":0,"list":[]},"success":true,"message":"操作成功","status":200}
             */
            
        }
        else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets[tag] = NULL;
        delete loginPacket;
        
    },"dynamic/selectDynamicByUserId",json_s.toStyledString());
}