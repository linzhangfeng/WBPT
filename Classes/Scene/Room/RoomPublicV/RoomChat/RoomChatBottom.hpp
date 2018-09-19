//
//  RoomChatBottom.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef RoomChatBottom_hpp
#define RoomChatBottom_hpp

#include "ZJH.h"
USING_NS_CC;

class RoomChatBottom : public Layer, public cocos2d::ui::EditBoxDelegate
{
public:
    CREATE_FUNC(RoomChatBottom);
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
    virtual void onEnter();
    
    void showFastChatPopWindow();
    
    void showEmojiPopWindow();
    
    bool isInputEmpty();
    
private:
    Layer* allLayer;
    Layout* input_layer;
    Layer* face_layer;
    Layer* other_layer;
    Layout* fastChatPopWindow;
    Layout* emojiPopWindow;
    cocos2d::ui::EditBox* _editText;
    
    cocos2d::ui::Button* btn_sound;
    cocos2d::ui::Button* btn_face;
    cocos2d::ui::Button* btn_add;
    cocos2d::ui::Button* btn_send;
    
    float inputH;
    float faceH;
    float otherH;
    
    float ss;
};

#endif /* RoomChatBottom_hpp */
