//
//  AddFriend.hpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/15.
//
//

#ifndef AddFriend_hpp
#define AddFriend_hpp

#include <stdio.h>
#include "ZJH.h"
#include "../BaseLayer.hpp"

class AddFriend : public BaseLayer, public EditBoxDelegate
{
public:
    CREATE_FUNC(AddFriend);
    bool init();
    void initView();
    
    Layout* getSearchFriend();
    Layout* getAddImageFriend();
    Layout* getScanFriend();
    Layout* getNearFriend();
    Layout* getLineLayout(int height);
    void getShareMessage();
    
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string &text);
    virtual void editBoxReturn(EditBox *editBox);
    
private:
    Size size;
    Json::Value _shareJson;
};
#endif /* AddFriend_hpp */
