//
//  CreateRoomLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/11.
//
//

#ifndef CreateRoomLayer_hpp
#define CreateRoomLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  CreateRoomLayer: public BaseLayer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(CreateRoomLayer);
    
    void showView();
    
    void selectImage(Ref *pSender, Widget::TouchEventType type);
    void sureOn(Ref *pSender, Widget::TouchEventType type);
    void tabClickFun(Ref* ref);
    
    //公开还是私密界
    void privateView();
    void publicView();
    
private:
    ListView * showGameTypeList();
    Button *createBtn;
    EditBox *roomName;
    Layout *peNumBg;
    Text *tip1;
    Text *tip2;
    Text *tip3;
    int roomType; //0:私密 1:公开
    Layout *lastSelect;
    int playIndex;
    
    
};

#endif /* CreateRoomLayer_hpp */
