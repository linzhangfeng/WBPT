//
//  FoundFriends.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef FoundFriends_hpp
#define FoundFriends_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"

USING_NS_CC;

class FoundFriends : public BaseLayer, public EditBoxDelegate
{
public:
    FoundFriends();
    
    CREATE_FUNC(FoundFriends);
    bool init();
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string &text);
    virtual void editBoxReturn(EditBox *editBox);
    void initList(bool init = false);

    void searchGameFriend();
    
    void addDataInList(Json::Value json);
private:
    Size winSize;
    int pageNo;
    Json::Value listData;
    ListView* listView;
    string friendId;
    
    bool searchflag;
    bool isconneting;
    EditBox* _editName;
    string searchID;
};

class FoundFriends_Item : public Layer
{
public:
    CREATE_FUNC(FoundFriends_Item);
    bool init();
    void initData(Json::Value &json);
private:
};

#endif /* FoundFriends_hpp */
