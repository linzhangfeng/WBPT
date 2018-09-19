//
//  GamePlayingErrorDialog.hpp
//  ZJH
//
//  Created by mac on 2016/12/9.
//
//

#ifndef GamePlayingErrorDialog_hpp
#define GamePlayingErrorDialog_hpp

#include "cocos2d.h"
#include "ZJH.h"

class GamePlayingErrorDialog : public cocos2d::Layer
{
public:
    CREATE_FUNC(GamePlayingErrorDialog);
    virtual bool init();
    Label* code;
    Label* gameName;
    ImageView* gameLogo;
    void show(string code,string gameName,string gameLogo);
    void buttonClicked(Ref *ref);
    bool onTouchBegan(Touch* touch, Event* event);
    CC_SYNTHESIZE(MyCallBack_Str, m_pCallback, Callback);
};
#endif /* GamePlayingErrorDialog_hpp */
