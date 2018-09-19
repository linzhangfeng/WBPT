//
//  OpenMatchLayer.hpp
//  ZJH
//
//  Created by apple on 16/8/17.
//
//

#ifndef OpenMatchLayer_hpp
#define OpenMatchLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ZJh.h"


using namespace cocos2d::ui;
using namespace cocos2d;


class  OpenMatchLayer: public BaseLayer
{
    
public:
    virtual bool init();
    CREATE_FUNC(OpenMatchLayer);

    void haveGiftView();
    void buLuoEnter();
    
    void addOpenObserver();
    
    virtual void onExit();
    void updatePortrait(Ref*ref);
    void updateNick(Ref*ref);
    void updatePayAfter(Ref*ref);
    void fromWebIntoGame(std::string code);

    void gameReload(Ref* r);
    void gameReloadNew(Ref* r);
    
    void showKeyBordView();
    
    void moneyChange(Ref* ref);
    void postFreeGift(Ref* ref);
    void showGamePlayingDialog(string code,string gameName,string gameLogo);
    void refreshCardsInvite(Ref*ref);
    
    //推荐好友拉起的操作
    void inviteLoadInto(Ref*ref);
    void initHttpParam();
    
private:
    //------添加-------
    Size _mSize;
    void showInfoView();
    void postMoney(Ref* ref);
    void joinInMatch();
    //展开房间列表
    void moreRoomView();
    
    void keyBtnOn(Ref*ref);
    void giftBtnOn(Ref*ref);
    
    void buyGiftBtnOn(Ref*ref);
    
    void showFreeReciveTip();
    void showFirstRechangeTip();
    
    void showOpenMatch();
    void enterRoomRequest();
    void enterRoomRequestWithCode(string code);
    void showKeyBord(Ref *pSender, Widget::TouchEventType type);
    void setCodeText(string code);
    
    ImageView *Bg;
    ImageView *keyBg;
    Text *nameUser;
    ImageView *portritBg;
    Text *goldText;
    Text *wanText;
    Text *cardText;
    LabelAtlas * codeText1;
    LabelAtlas * codeText2;
    LabelAtlas * codeText3;
    LabelAtlas * codeText4;
    LabelAtlas * codeText5;
    LabelAtlas * codeText6;
    string codeStr;

    int count;
    Layout* maskBg;
    Button *upGiftBtn;
    
    Layout *freeBg;
    int currentGiftIndex;
    
    ImageView*giftBg;
    
    bool IsGiftShow;
    Json::Value giftJson;
    
    ImageView*infoBg;
    
    
    //俱乐部入口
    bool  IsShowBuLuo;
    ImageView*buLuoBg;
    Button*traitBtn;
    
    Text* userID;
    
    void buLuoEnterBtnOn(Ref*ref);
};

#endif /* OpenMatchLayer_hpp */
