//
//  ChatFriendsBottom.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef ChatFriendsBottom_hpp
#define ChatFriendsBottom_hpp

#include "ZJH.h"
USING_NS_CC;

class ChatFriendsBottom : public Layer, public cocos2d::ui::EditBoxDelegate
{
public:
    CREATE_FUNC(ChatFriendsBottom);
    bool init();
    void setBottomType(int type); //设置底部类型 0普通聊天类型  1俱乐部类型
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
    CC_SYNTHESIZE(MyCallBack_Str, m_pCallback, Callback);

private:
    Layer* allLayer;
    Layer* input_layer;
    Layer* face_layer;
    Layer* other_layer;
    cocos2d::ui::EditBox* _editText;
    
    cocos2d::ui::Button* btn_sound;
    cocos2d::ui::Button* btn_face;
    cocos2d::ui::Button* btn_add;
    cocos2d::ui::Button* btn_send;
    
    int type;
    float inputH;
    float faceH;
    float otherH;
};

#endif /* ChatFriendsBottom_hpp */
