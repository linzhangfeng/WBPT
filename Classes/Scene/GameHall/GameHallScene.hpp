//
//  GameHallScene.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef GameHallScene_hpp
#define GameHallScene_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"

USING_NS_CC;

class GameHallScene : public Layer
{
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(GameHallScene);
    bool init();
    void initCsbBView();
    void initHttpParam();
    
    void onEnter();
    void onExit();
    
    void querySystemMsg();
private:
    Text* nameUser;
    Text* userID;
    Text* zhuanshiText;
    Text* roomCards;
    ImageView* topBg;
    ImageView* titleBg;
    ImageView* portritBg;
    LabelTTF* notice;
    string noticeText;
    ClippingNode* clip;
    Node* root;
    void menuCallback(cocos2d::Ref *sender);
    void touchCallback(Ref *pSender, Widget::TouchEventType type);
    void gameLoadWithWebCode(Ref* f);
    void gameReload(Ref* r);
    void onKeyReleased(EventKeyboard::KeyCode keycode,Event* event);
    long long lastTouchBackKeyTime;
};

#endif /* GameHallScene_hpp */
