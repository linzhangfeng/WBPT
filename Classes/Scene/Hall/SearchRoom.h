//
//  SearchRoom.hpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/13.
//
//

#ifndef SearchRoom_hpp
#define SearchRoom_hpp

#include <stdio.h>
#include "ZJH.h"
#include "../BaseLayer.hpp"

class SearchRoom : public BaseLayer, public EditBoxDelegate
{
public:
    CREATE_FUNC(SearchRoom);
    bool init();
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string &text);
    virtual void editBoxReturn(EditBox *editBox);
    void searchGameRoom();
    void initListView(Json::Value mValue);
    
private:
    ListView *serListView;
    Json::Value serValue;
    int countpage;
    string roomId;
    
    Text* tipText;
};

#endif /* SearchRoom_hpp */
