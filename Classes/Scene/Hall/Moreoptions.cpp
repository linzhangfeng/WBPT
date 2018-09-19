//
//  Moreoptions.cpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/13.
//
//

#include <stdio.h>
#include "Moreoptions.h"
#include "ui/CocosGUI.h"

bool Moreoptions::init(){
    if(!Layout::init()){
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    this->setTouchEnabled(true);
    this->setContentSize(winSize);

    
    listLayout =  Layout::create();
    listLayout->setTouchEnabled(true);
    listLayout->setContentSize(Size(400,500));
    
    ImageView* _sprite = ImageView::create("Hall/menu_up_bg.png");
    _sprite->setContentSize(Size(400,500));
    _sprite->setScale9Enabled(true);
    _sprite->ignoreAnchorPointForPosition(false);
    _sprite->setPosition(Vec2(200,240));
    listLayout->addChild(_sprite);
    
    listLayout->setPosition(Vec2(this->getContentSize().width - 420, this->getContentSize().height-150-500));
    this->addChild(listLayout);
    
    this->addClickEventListener([=](Ref *ref){
        if(_callfun != NULL){
            _callfun();
        }
    });
    initSelectoptions();
    return true;
}

void Moreoptions::initSelectoptions(){

    ListView *listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(400,500));
    listView->setAnchorPoint(Vec2(0, 1));
    listView->setPosition(Vec2(0, 470));
    listView->setScrollBarEnabled(false);
    listView->setItemsMargin(0);
    listLayout->addChild(listView);
    
    for(int i =0;i<4;i++){
        optionsListItem* item = optionsListItem::createWithData(i,[=](){
            if(_callfun != NULL){
                _callfun();
            }
        });
        listView->addChild(item);
    }
}


optionsListItem *optionsListItem::createWithData(int index,const MyCallBack_Null &callback)
{
    optionsListItem *pRet = new optionsListItem();
    if (pRet && pRet->initWithData(index,callback)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}


void Moreoptions::callFun(const MyCallBack_Null &callback){
    _callfun = callback;
}


bool optionsListItem::initWithData(int index,const MyCallBack_Null &callback)
{
    if (Widget::init() == false) {
        return false;
    }
    
    setTouchEnabled(true);
    
    Size itemSize = Size(400, 120);
    Size imageSize = Size(60, 60);
    
    this->setContentSize(itemSize);
    
    
    Layout* lay = Layout::create();
    lay->setContentSize(itemSize);
    lay->setTouchEnabled(true);
    LayerColor* clo = LayerColor::create(Color4B(0,0,0,0));
    clo->setContentSize(itemSize);
    lay->addChild(clo);
    this->addChild(lay);
  
    string tips = "";
    string tipsimg = "";
    switch(index){
        case 0:
            tips = "开房间";
            tipsimg = "Hall/menu_bar_icon_1.png";
            break;
        case 1:
             tips = "搜房间";
            tipsimg = "Hall/menu_bar_icon_3.png";
            break;
        case 2:
             tips = "找好友";
            tipsimg = "Hall/menu_bar_icon_4.png";
            break;
        case 3:
             tips = "扫一扫";
            tipsimg = "Hall/menu_bar_icon_2.png";
            break;
    }
    
    
    ImageView *image = ImageView::create(tipsimg);
    image->setContentSize(imageSize);
    image->setAnchorPoint(Vec2(0, 0));
    image->setPosition(Vec2(40, 30));
    image->setTouchEnabled(false);
    image->ignoreContentAdaptWithSize(false);
    this->addChild(image);

    //文字
    Text *name = Text::create(tips, "AmericanTypewriter", 40);
    name->setAnchorPoint(Vec2(0, 0));
    name->setPosition(Vec2(160,30));
    name->setColor(Color3B::WHITE);
    name->setTouchEnabled(false);
    this->addChild(name);
    
    lay->addClickEventListener([=](Ref *ref)
                                {
                                    if( callback!= NULL){
                                        callback();
                                    }
                                    if(index == 0){
                                        if(!ZJHModel::getInstance()->isYoukeLogin()){
                                            ZJHModel::getInstance()->gotoView(VIEW_MINE_CREAT_ROOM);
                                        }else{
                                            ZJHModel::getInstance()->gotoView(VIEW_LOAD);
                                        } 
                                    }else if(index == 1){
                                       ZJHModel::getInstance()->gotoView(VIEW_SEARCH_ROOM);
                                    }else if(index == 2){
                                        if(!ZJHModel::getInstance()->isYoukeLogin()){
                                           ZJHModel::getInstance()->gotoView(VIEW_ADD_FRIENDS);
                                        }else{
                                            ZJHModel::getInstance()->gotoView(VIEW_LOAD);
                                        }
                                        
                                    }else if(index == 3){
                                        PlatformHelper::showToast("扫一扫");
                                    }
                                    
                                });
    
    ImageView *lineLay = ImageView::create("Head/menu_up_line.png");
    lineLay->setContentSize(Size(itemSize.width,2));
    lineLay->setAnchorPoint(Vec2(0, 0));
    lineLay->setPosition(Vec2(0, 0));
    lineLay->setTouchEnabled(false);
    lineLay->ignoreContentAdaptWithSize(false);
    this->addChild(lineLay);
    
    return true;
}