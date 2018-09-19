//
//  BaseLayer.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef BaseLayer_hpp
#define BaseLayer_hpp

#include "ZJH.h"
USING_NS_CC;

class BaseLayer : public Layer
{
public:
    
    ~BaseLayer();
    CREATE_FUNC(BaseLayer);
    bool init();
    void setHead(int h);
    int getHeadH();
    BaseLayer* setBackground(Color4B color);
    BaseLayer* setBackground(string strpath);
    BaseLayer* setReturn_txt(std::string return_str,const MyCallBack_Null &callback);
    BaseLayer* setReturn_img(std::string return_up,std::string return_down,const MyCallBack_Null &callback);
    BaseLayer* setPreView(BaseLayer* pre);
    BaseLayer* setTitle(std::string title);
    BaseLayer* setMore_txt(std::string more,const MyCallBack_Null &callback);
    BaseLayer* setMore_img(std::string more_up,std::string more_down,const MyCallBack_Null &callback);
    
    void appear();
    void disappear();
    virtual void onExit();

    BaseLayer* setPreByName(std::string name);
    int getSpriteHeight();
    void removeToPreByName(std::string name);
    void removePre(BaseLayer* pre, BaseLayer* toPreView);
    BaseLayer* pre_view;
    
    void onKeyReleased(EventKeyboard::KeyCode keycode,Event* event);
    
    void setCanReturn(bool v){this->canReturn = v;};
    
private:
    LayerColor* bg;
    int baseHeadH;
    int setHeadH;
    bool isRuning;
    
    bool canReturn;
    long long lastTouchBackKeyTime;
    MyCallBack_Null return_callback;
};

#endif /* BaseLayer_hpp */
