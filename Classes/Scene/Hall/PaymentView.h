//
//  PaymentView.hpp
//  ZJH
//
//  Created by zhangzhenhua on 16/8/3.
//
//

#ifndef PaymentView_hpp
#define PaymentView_hpp

#include <stdio.h>
#include "ZJH.h"

class PaymentView : public Layout
{
public:
    CREATE_FUNC(PaymentView);
    bool init();
    
    void initDataMessage(Json::Value mValue);
    void selectPayFun(Ref *pSender, Widget::TouchEventType type);
    ListView* getpayLayout();
private:
    int adimage_height;
    Layout *selectPay;
    int selectTag;
    Json::Value _pay;
};

#endif /* PaymentView_hpp */
