//
//  ChatFriendsMore_Jubao.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef ChatFriendsMore_Jubao_hpp
#define ChatFriendsMore_Jubao_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"
USING_NS_CC;

class ChatFriendsMore_Jubao : public BaseLayer
{
public:
    CREATE_FUNC(ChatFriendsMore_Jubao);
    bool init();
    void myInit(Json::Value &json);
    
    void sendMessCallback();
    void addListData();
    void cgettitleColor(int oindex,int nindex);
private:
    ListView* JBListView;
    Size size;
    int oldindex;
};

class JBListItem : public Layout
{
public:
    static JBListItem *createWithData(int index,int width,const MyCallBack_Int &callback);
    bool initWithData(int index,int width,const MyCallBack_Int &callback);
    Layout* getLineLayout(int width,int height);
};

#endif /* ChatFriendsMore_Jubao_hpp */
