//
//  VersionMessageDialog.hpp
//  ZJH
//
//  Created by Jack on 16/9/26.
//
//

#ifndef VersionMessageDialog_hpp
#define VersionMessageDialog_hpp

#include "cocos2d.h"
#include "ZJh.h"

//我的里面显示版本信息对话框
class VersionMessageDialog : public Layer{
    
public:
    CREATE_FUNC(VersionMessageDialog);
    bool init();
    void show();
    void dismiss(Ref* ref);

};
#endif /* VersionMessageDialog_hpp */
