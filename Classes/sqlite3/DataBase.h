//
//  DataBase.h
//  ZJH
//
//  Created by hyhy on 16/7/11.
//
//

#ifndef __ZJH__DataBase__
#define __ZJH__DataBase__

#include "sqlite3.h"
#include "../Model/MsgModel.h"
#include "json.h"

class DataBase
{
public:
    static DataBase *getInstance(int id);
    
    bool addSession(MsgModel::Session* s);
    bool updateSession(MsgModel::Session* s);
    bool deleteSession(MsgModel::Session* s);
    std::map<int,MsgModel::Session*> getAllSession();
    
    bool addFriend(MsgModel::FriendInfo* f);
    bool updateFriend(MsgModel::FriendInfo* f);
    std::map<long,MsgModel::FriendInfo*> getAllFriendInfo(string sql);
    std::map<long,MsgModel::FriendInfo*> getAllFriendGroupInfo();//获取所有群好友信息
    std::map<long,MsgModel::FriendInfo*> getAllFriendNormalInfo();//获取所有普通好友信息
    
    bool addChat(MsgModel::Chat* f);
    bool deleteChats(int uid);
    bool updateChat(MsgModel::Chat* f);
    std::vector<MsgModel::Chat*> getChat(int uid,int page, int pageSize, int beginid,int groupType = CONTENT_POINT_TO_POINT);
    
    bool insert(std::string sql);
    bool update(std::string sql);
    std::string select(std::string sql);
protected:
    DataBase();
    ~DataBase();
    virtual bool init(int id);
    static DataBase* s_sharedDataBase;
private:
    sqlite3 *m_pdb;
};

#endif /* defined(__ZJH__DataBase__) */
