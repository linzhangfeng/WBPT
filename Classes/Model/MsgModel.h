#ifndef __MSG_MODEL_H__
#define __MSG_MODEL_H__

#include <string>
#include <stdint.h>
#include "ZJH.h"

class MsgModel : public Ref
{
public:
    struct Session
    {
        int id;//database id
        int mid;//映射到数据库中的一个集合
        int type;//自定义的
        int msgType;
        int fromId;
        string content;
        long long createTime;
        
        Session()
        {
            mid = -1;
            type = -1;
            msgType = -1;
            fromId = -1;
            content = "";
            createTime = -1;
        }
    };

    struct Chat
    {
        int id;//database id
        long long seqNo;
        
        int mid;//映射到数据库中的一个集合
        int fromId;
        int toId;
        string content;
        int msgType;
        long long createTime;
        int status;
        int roomID;
        int contentType;  //消息所属类型  点对点   普通群   俱乐部
        string filePath;
        
        Chat()
        {
            mid = -1;
            fromId = -1;
            toId = -1;
            msgType = -1;
            content = "";
            createTime = -1;
            status = -1;
            roomID = -1;
            contentType = CONTENT_POINT_TO_POINT; //默认为点对点消息
            seqNo = -1;
        }
        
        Json::Value toJson()
        {
            Json::Value json;
            json["id"] = id;
            json["seqNo"] = StringUtils::format("%lldd",seqNo);
            json["mid"] = mid;
            json["fromId"] = fromId;
            json["toId"] = toId;
            json["content"] = content;
            json["msgType"] = msgType;
            json["createTime"] = StringUtils::format("%lldd",createTime);
            json["status"] = status;
            json["roomID"] = roomID;
            json["contentType"] = contentType;
            json["filePath"] = filePath;
            return json;
        }
    };
    
    struct FriendInfo
    {
        int id;//database id
        int mid;//映射到数据库中的一个集合
        int unReadNums;
        
        string nickName;
        string photo;
        
        string noSendText;
        std::vector<Chat*> chatList;
        
        
        //temp
        int blackType;
        int friendType;
        string rmkName;
        
        FriendInfo()
        {
            mid = -1;
            unReadNums = -1;
            nickName = "";
            photo = "";
            
            
            //temp
            blackType = -1;
            friendType = -1;
            rmkName = "";
        }
    };

public:
    static MsgModel *getInstance();
    
    void init();
    void start();
    void stop();
    
    void doHeartbeat(float dt);
    
    void getData(float dt);
    
    void handler_cmd(int cmd,std::string &data,long long seqNo);
    
    void sendText(std::string text,int groupType = CONTENT_POINT_TO_POINT);//groupType 所属群组类型 点对点消息 普通群消息  俱乐部消息
    void sendImg(std::string filePath,int groupType = CONTENT_POINT_TO_POINT);
    void sendSound(std::string filePath,int time,int groupType = CONTENT_POINT_TO_POINT);
    
    void notificationReflash(Ref* msg);
    void notifyLogout(Ref* msg);
    void initAccountData();
    
    void sendText_group(std::string text);
    void sendImg_group(std::string filePath);
    void sendSound_group(std::string filePath,int time);
    void join_group(int id);
    void goout_group(int id);
    void requestDynamicMessage(Json::Value json);
    
public:
    MsgModel::Session* addSession(Session* s);
    MsgModel::FriendInfo* addFriend(FriendInfo* fi,bool needAutoUpdate = true);
    MsgModel::FriendInfo* updateFriendInfo(FriendInfo* fi);
    MsgModel::Chat* addChat(Chat* fi);
    MsgModel::Chat* updateChat(Chat* fi);
    
public:
    std::map<int,Session*> sessionList;
    std::map<long,FriendInfo*> friendList;      //普通好友信息
    std::map<long,FriendInfo*> friendGroupList; //俱乐部好友信息
    std::map<long,Chat*> curSendList_tmp;
    int curChatUID;
    
    std::string ip;
    int port;
    
    int curGroupID;
    std::vector<Chat*> roomChatList;
    
    bool isShowBeOfflineDialog;
    
    int connect_count;
    
    int lastHeartBeatTime;
    
    int curUID;
};

#endif
