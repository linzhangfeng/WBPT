//
//  ChatFriendsDT.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "ChatFriendsDT.hpp"
#include "../../Model/MsgModel.h"
#include "proto.h"

bool ChatFriendsDT::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setName("ChatFriendsDT");
    
    Size size = Director::getInstance()->getWinSize();
    
    return true;
}

void ChatFriendsDT::myInit(Json::Value &json)
{
    int uid = json["uid"].asInt();
    
    MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[uid];
    
    Size size = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(200, 200, 200, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", NULL);
    
    setTitle(fi->nickName);
    
    
    Json::Value json_s;
    json_s["friendId"] = uid;
    json_s["pageSize"] = 20;
    json_s["pageNo"] = 1;
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
        
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        loginPacket->status = 0;
        
        if (loginPacket->resultIsOK())
        {
            /*
             [{"errorType":"SUCCESS","resultMap":{"hasMore":false,"friendType":3,"count":0,"list":[{"commentCount":1,"pointCount":0,"createTime":1463988025000,"shareUrl":"http://188.188.1.111:20006/chess//share/dynamic/184.htm","isPoint":false,"isFav":false,"type":0,"dynamicId":184,"user":{"id":100014,"nickName":"IT黄贯中","image":"http://188.188.1.179:20084/chess/chess_img/head/100014/1510301726222933_120x120.jpg","phone":"15986759237","remarkName":"","signature":"2\nwv3x。","sex":0,"vipLv":3},"comment":{"content":[{"id":54,"content":"hege","createTime":1468394230000,"user":{"id":101374,"nickName":"玩呗101374","image":"http://188.188.0.111:20084/chess/chess_img/head/default/00.jpg","phone":"18503085536","remarkName":"","signature":null,"sex":0,"vipLv":2}}],"hasMore":false},"game":null,"normal":{"content":"博児不亖","image":[{"thumbUrl":"http://188.188.0.111:20084/chess/chess_img/dynamic/100014/1605231519464767_720x720.jpg","url":"http://188.188.0.111:20084/chess/chess_img/dynamic/100014/1605231519464767.jpg"},{"thumbUrl":"http://188.188.0.111:20084/chess/chess_img/dynamic/100014/1605231519571919_720x720.jpg","url":"http://188.188.0.111:20084/chess/chess_img/dynamic/100014/1605231519571919.jpg"},{"thumbUrl":"http://188.188.0.111:20084/chess/chess_img/dynamic/100014/1605231520033376_720x720.jpg","url":"http://188.188.0.111:20084/chess/chess_img/dynamic/100014/1605231520033376.jpg"}],"wap":null,"viewLimit":0},"status":false},{"commentCount":0,"pointCount":0,"createTime":1463987961000,"shareUrl":"http://188.188.1.111:20006/chess//share/dynamic/182.htm","isPoint":false,"isFav":false,"type":0,"dynamicId":182,"user":{"id":100014,"nickName":"IT黄贯中","image":"http://188.188.1.179:20084/chess/chess_img/head/100014/1510301726222933_120x120.jpg","phone":"15986759237","remarkName":"","signature":"2\nwv3x。","sex":0,"vipLv":3},"comment":{"content":[],"hasMore":false},"game":null,"normal":{"content":"2222","image":[{"thumbUrl":"http://188.188.0.111:20084/chess/chess_img/dynamic/100014/1605231519082525_720x720.jpg","url":"http://188.188.0.111:20084/chess/chess_img/dynamic/100014/1605231519082525.jpg"},{"thumbUrl":"http://188.188.0.111:20084/chess/chess_img/dynamic/100014/1605231519146604_720x720.jpg","url":"http://188.188.0.111:20084/chess/chess_img/dynamic/100014/1605231519146604.jpg"}],"wap":null,"viewLimit":0},"status":false}]},"success":true,"message":"操作成功","status":200}]
             */
            
            
            
        }
        else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
        
        CCHttpAgent::getInstance()->packets[tag] = NULL;
        delete loginPacket;
        
    },"dynamic/selectDynamicByUserId",json_s.toStyledString());
}