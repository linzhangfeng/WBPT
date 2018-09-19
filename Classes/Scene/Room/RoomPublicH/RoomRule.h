//
//

#ifndef RoomRule_h
#define RoomRule_h

#include "cocos2d.h"
#include "ZJH.h"

class RoomRule : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomRule);
    virtual bool init();
    void initHelp();
    
    void initHelpWeb();
    void buttonCallback(Ref *sender);
    bool onTouchBegan(Touch* touch, Event* event);
    RichText * createRichText(const std::string &label, const std::string &content);
    Button * selectBtnDraw(const std::string &titleStr, bool selected, int cellType);
    
private:
    cocos2d::experimental::ui::WebView* webView;
    Button *helpBtn;
    Button *ruleBtn;
    ListView *ruleListView;
    
    
};

#endif /* defined(RoomRule_h) */
