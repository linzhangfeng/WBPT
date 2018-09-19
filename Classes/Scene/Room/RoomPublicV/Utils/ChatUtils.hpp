//
//  ChatUtils.hpp
//  ZJH
//
//  Created by mac on 2016/12/9.
//
//

#ifndef ChatUtils_hpp
#define ChatUtils_hpp
#include "../../../../Model/MsgModel.h"
#include <stdio.h>
class ChatUtils
{
    public:
      static void componentsSeparatedByString_roomchat(std::string str,ui::RichText* _richText);
      static void componentsSeparatedByString_roomchat(std::string str,ui::RichText* _richText,int fontSize,Color3B color);
};

#endif /* ChatUtils_hpp */
