//
//  FriendRemark.hpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/29.
//
//

#ifndef FriendRemark_hpp
#define FriendRemark_hpp

#include <stdio.h>
#include "ZJH.h"

class FriendRemark : public Layout
{
public:
    CREATE_FUNC(FriendRemark);
    bool init();
    void initView();
    void sendMesFixNick(string nick);
    void setuid(int uid){_cuid = uid;};
    void onExit();
private:
    Layout* laymain;
    int _cuid;  //选择的当前用户的id
};

#endif /* FriendRemark_hpp */
