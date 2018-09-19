
//
//  TaskLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/6.
//


#include "PlayWebLayer.hpp"
#include "../Loading/Loading.h"

bool PlayWebLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout *bgLayout = Layout::create();
    bgLayout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    bgLayout->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    bgLayout->setBackGroundColorOpacity(150);
    bgLayout->setTouchEnabled(true);
    bgLayout->setSwallowTouches(true);
    bgLayout->setContentSize(visibleSize);
    bgLayout->addClickEventListener([=](Ref*ref){
        this->removeFromParent();
        
    });
    
    this->addChild(bgLayout);

    bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(visibleSize.width - 100, visibleSize.height - 400));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    bgLayout->addChild(bg);
    
    H = bg->getContentSize().height;
    bgW = bg->getContentSize().width;
    
    Button *cancel = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    cancel->setPosition(Vec2(bgW, H));
    cancel->addClickEventListener([=](Ref*ref){
        this->removeFromParent();
    });
    bg->addChild(cancel);
    
    Text *title = Text::create("规则说明", "", 50);
    title->setAnchorPoint(Vec2(0.5, 1));
    title->setTextColor(Color4B(0xf1, 0xcb, 0x5b, 255));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(bgW/2, H - 40));
    bg->addChild(title);

    createView();
    
    return true;
}

void PlayWebLayer::createView()
{
    auto cellBao = Layout::create();
    cellBao->setName("cellBag");
    cellBao->setPosition(Vec2(0, H - 110));
    bg->addChild(cellBao);
    
    webView = cocos2d::experimental::ui::WebView::create();
    webView->setColor(Color3B::BLACK);
    webView->setAnchorPoint(Vec2(0, 0));
    webView->setPosition(Vec2(10, 10));
    webView->setContentSize(Size(bgW - 20, H - 110));
    webView->setScalesPageToFit(true);
    bg->addChild(webView);
}


void PlayWebLayer::createData(Json::Value Data,int index)
{
    setData = Data;
    baoIndex = index;
    
    Layout * cellBao = (Layout*)bg->getChildByName("cellBag");
    if(cellBao != NULL)
    {
        int nums = 0;
        int cheight = 0;
        if (setData.size() > 0 && setData.size() <= 8)
        {
            cheight = 20 +115*ceil(setData.size()/4.0);
            cellBao->setContentSize(Size(bgW, cheight));
            nums = setData.size();
            
        }else if (setData.size() > 8)
        {
            cheight = 20 +115*8/4.0;
            cellBao->setContentSize(Size(bgW, cheight));
            nums = 7;
            
            Button* moreBtn = Button::create("Match/set_more.png","Match/set_more_se－lect.png", "Match/set_more_select.png");
            moreBtn->addClickEventListener(CC_CALLBACK_1(PlayWebLayer::onMoreResponse, this));
            moreBtn->setPosition(Vec2(30 + 3*(220+15) + 120, cellBao->getContentSize().height - 80 - 100 - 15));
            cellBao->addChild(moreBtn);
        }
        
        cellBao->setPosition(Vec2(0, H - 110 - cheight));
        webView->setContentSize(Size(bgW - 20, H - 120 - cheight));
        
        if(nums < 7)
        {
            for (int i = 0; i < nums; i++)
            {
                Button* baoBtn = Button::create("Match/match_bao_n.png","Match/match_bao_s.png", "Match/match_bao_s.png");
                baoBtn->setScale(0.9);
                baoBtn->setTag(100+i);
                baoBtn->addClickEventListener(CC_CALLBACK_1(PlayWebLayer::onBaoBtnResponse, this));
                baoBtn->setAnchorPoint(Vec2(0, 0.5));
                baoBtn->setPosition(Vec2(30 + i%4*(220+15), cellBao->getContentSize().height - 80 - i/4*(100+15)));
                baoBtn->setTitleText(setData[i]["pkgName"].asString());
                baoBtn->setTitleFontName("");
                baoBtn->setTitleFontSize(40);
                cellBao->addChild(baoBtn);
                
                if(baoIndex == i)
                {
                    lastBaoBtn = baoBtn;
                    baoBtn->setEnabled(false);
                    baoBtn->setTitleColor(Color3B(0xf0, 0xca, 0x58));
                }
            }
        }else{
            onShouResponse(NULL);
        }
        string url = CCHttpAgent::getInstance()->url+setData[baoIndex]["helpUrl"].asString();
        webView->loadURL(url);
    }
}


void PlayWebLayer::onShouResponse(Ref*ref)
{
    
    Layout * cellBao = (Layout*)bg->getChildByName("cellBag");
    cellBao->removeAllChildren();
    cellBao->setContentSize(Size(bgW, 20 +115*2));
    
    cellBao->setPosition(Vec2(0, H - 110 - 250));
    webView->setContentSize(Size(bgW - 20, H - 120 - 250));
    
    Button* moreBtn = Button::create("Match/set_more.png","Match/set_more_select.png", "Match/set_more_select.png");
    moreBtn->addClickEventListener(CC_CALLBACK_1(PlayWebLayer::onMoreResponse, this));
    moreBtn->setPosition(Vec2(30 + 3*(220+15) + 120, cellBao->getContentSize().height - 80 - 100 - 15));
    cellBao->addChild(moreBtn);
    
    //包名的tag取值：100+
    
    int i = 0;
    if (baoIndex >= 7)
    {
        Button* firstBtn = Button::create("Match/match_bao_n.png","Match/match_bao_s.png", "Match/match_bao_s.png");
        firstBtn->setScale(0.9);
        firstBtn->setTag(baoIndex);
        firstBtn->addClickEventListener(CC_CALLBACK_1(PlayWebLayer::onBaoBtnResponse, this));
        firstBtn->setAnchorPoint(Vec2(0, 0.5));
        firstBtn->setPosition(Vec2(30, cellBao->getContentSize().height - 80));
        firstBtn->setTitleText(setData[baoIndex]["pkgName"].asString());
        firstBtn->setTitleFontName("");
        firstBtn->setTitleFontSize(40);
        cellBao->addChild(firstBtn);
        
        lastBaoBtn = firstBtn;
        firstBtn->setEnabled(false);
        firstBtn->setTitleColor(Color3B(0xf0, 0xca, 0x58));
        
        i = 1;
    }
    for (int count = 0; i < 7; i++, count++)
    {
        Button* baoBtn = Button::create("Match/match_bao_n.png","Match/match_bao_s.png", "Match/match_bao_s.png");
        baoBtn->setScale(0.9);
        baoBtn->setTag(100+count);
        baoBtn->addClickEventListener(CC_CALLBACK_1(PlayWebLayer::onBaoBtnResponse, this));
        baoBtn->setAnchorPoint(Vec2(0, 0.5));
        baoBtn->setPosition(Vec2(30 + i%4*(220+15), cellBao->getContentSize().height - 80 - i/4*(100+15)));
        baoBtn->setTitleText(setData[count]["pkgName"].asString());
        baoBtn->setTitleFontName("");
        baoBtn->setTitleFontSize(40);
        cellBao->addChild(baoBtn);
        
        if(baoIndex == count)
        {
            lastBaoBtn = baoBtn;
            baoBtn->setEnabled(false);
            baoBtn->setTitleColor(Color3B(0xf0, 0xca, 0x58));
        }
    }
    
    
}

//更多展开
void PlayWebLayer::onMoreResponse(Ref*ref)
{
    Layout * cellBao = (Layout*)bg->getChildByName("cellBag");
    cellBao->removeAllChildren();
    int hei = 20 +115*ceil((setData.size()+1)/4.0);
    cellBao->setContentSize(Size(bgW, hei));
    
    cellBao->setPosition(Vec2(0, H - 110 - hei));
    webView->setContentSize(Size(bgW - 20, H - 120 - hei));
    
    //包名的tag取值：100+
    for (int i = 0; i < setData.size(); i++)
    {
        Button* baoBtn = Button::create("Match/match_bao_n.png","Match/match_bao_s.png", "Match/match_bao_s.png");
        baoBtn->setScale(0.9);
        baoBtn->setTag(100+i);
        baoBtn->addClickEventListener(CC_CALLBACK_1(PlayWebLayer::onBaoBtnResponse, this));
        baoBtn->setAnchorPoint(Vec2(0, 0.5));
        baoBtn->setPosition(Vec2(30 + i%4*(220+15), cellBao->getContentSize().height - 80 - i/4*(100+15)));
        baoBtn->setTitleText(setData[i]["pkgName"].asString());
        baoBtn->setTitleFontName("");
        baoBtn->setTitleFontSize(40);
        cellBao->addChild(baoBtn);
        
        if(baoIndex == i)
        {
            lastBaoBtn = baoBtn;
            baoBtn->setEnabled(false);
            baoBtn->setTitleColor(Color3B(0xf0, 0xca, 0x58));
        }
    }
    
    Button* shouBtn = Button::create("Match/set_shou.png","Match/set_shou_select.png", "Match/set_shou_select.png");
    shouBtn->setName("shou");
    shouBtn->addClickEventListener(CC_CALLBACK_1(PlayWebLayer::onShouResponse, this));
    shouBtn->setPosition(Vec2(30 + setData.size()%4*(220+15) + 120, cellBao->getContentSize().height - 80 - setData.size()/4*(100+15)));
    cellBao->addChild(shouBtn);
    
}


void PlayWebLayer::onBaoBtnResponse(Ref*ref)
{
    Button *onBtn = (Button *)ref;
    int tagBao = onBtn->getTag()-100;
    if(onBtn == lastBaoBtn)
    {
        return;
    }

    onBtn->setEnabled(false);
    onBtn->setTitleColor(Color3B(0xf0, 0xca, 0x58));
    
    lastBaoBtn->setEnabled(true);
    lastBaoBtn->setTitleColor(Color3B(0xff, 0xff, 0xff));
    
    lastBaoBtn = onBtn;
    baoIndex = tagBao;
    
    
    
    string url = CCHttpAgent::getInstance()->url+setData[baoIndex]["helpUrl"].asString();
    webView->loadURL(url);
    
}
