//
//  ChatFriendsMore_Nick.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef ChatFriendsMore_Nick_hpp
#define ChatFriendsMore_Nick_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"
USING_NS_CC;

class ChatFriendsMore_Nick : public BaseLayer, public EditBoxDelegate
{
public:
    CREATE_FUNC(ChatFriendsMore_Nick);
    bool init();
    void myInit(Json::Value &json);
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string &text);
    virtual void editBoxReturn(EditBox *editBox);
    void sendMesFixNick(string nick);
private:
    Size size;
    EditBox* _editName;
    int _cuid;
};

#endif /* ChatFriendsMore_Nick_hpp */
