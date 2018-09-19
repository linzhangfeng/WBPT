//
//  RoomChatBottom2.hpp
//  ZJH
//
//  Created by pg on 16/9/6.
//
//

#ifndef RoomChatBottom2_hpp
#define RoomChatBottom2_hpp

#include "ZJH.h"
USING_NS_CC;


class RoomChatBottom2 : public Layer, public cocos2d::ui::EditBoxDelegate
{
public:
    
    //设置bottom做到
//    CREATE_FUNC(RoomChatBottom2);
    static RoomChatBottom2* create(Size nRoot);
    bool init();
    void btnCallback(Ref* ref);
    
    void faceCallback(Ref* ref);
    void checkContent();
    void scollToView(std::string v);
    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)override;
    virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)override;
    virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)override;
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox)override;
    
    void notificationReflash(Ref* msg);
    
    virtual void onExit();
    void showFace();
    void showFast();
    
    void initFast();
    
private:
    //聊天框背景
    CC_SYNTHESIZE(Size, m_nRootSize, RootSize);
    
    Layer* allLayer;
    Layer* input_layer;
    Layer  *face_layer;
    Layout *faceLayout;
    Layout *fastLayout;
//    Layer* other_layer;
    cocos2d::ui::EditBox* _editText;
    
    cocos2d::ui::Button* btn_sound;
    cocos2d::ui::Button* btn_face; //表情
//    cocos2d::ui::Button* btn_add;
    cocos2d::ui::Button* btn_send;
    cocos2d::ui::Button* btn_fast;
    
    
//    float inputH;
    float faceH;
//    float otherH;
    
//    float ss;
};



#endif /* RoomChatBottom2_hpp */
