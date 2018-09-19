//
//  DataBase.cpp
//  ZJH
//
//  Created by hyhy on 16/7/11.
//
//

#include "DataBase.h"
#include "../Utils/Utils.h"

std::map<int,DataBase*> allDatabase;

DataBase::DataBase()
    :m_pdb(nullptr)
{
    
}

DataBase::~DataBase()
{
    sqlite3_close(m_pdb);
}

DataBase *DataBase::getInstance(int id)
{
    if (allDatabase[id] == nullptr) {
        DataBase* db = new DataBase();
        if (!db || !db->init(id)) {
            delete db;
            db = nullptr;
            return NULL;
        }
        allDatabase[id] = db;
        return db;
    }
    return allDatabase[id];
}

bool DataBase::init(int id)
{
    std::string path = Utils::getSavePath() + "DataBase1.0/data" + __String::createWithFormat("%d",id)->getCString() + ".db";
    std::string sqlCMD;
    int ret;
    
    CCHttpAgent::getInstance()->_makeMultiDir(path.c_str());
    ret = sqlite3_open(path.c_str(), &m_pdb);
    if (ret != SQLITE_OK) {
        CCLOG("sqlite open error:%s", sqlite3_errmsg(m_pdb));
        sqlite3_close(m_pdb);
        return false;
    }
    
    /*
     struct Session
     {
     int type;//自定义的
     int msgType;
     int fromId;
     string content;
     long createTime;
     };
     */
    //创建Table
    sqlCMD = "create table if not exists Session(id integer primary key autoincrement, mid integer, type integer, msgType integer, fromId integer, content text, createTime integer64)";
    ret = sqlite3_exec(m_pdb, sqlCMD.c_str(), nullptr, nullptr, nullptr);
    if (ret != SQLITE_OK) {
        CCLOG("create table faild");
    }
    
    
    /*
     struct FriendInfo
     {
     int uid;
     int unReadNums;
     int lastReadTime;
     int friendType;
     string nickName;
     string photo;
     string noSendText;
     std::vector<Chat*> chatList;
     };
     */
    //创建Table
    sqlCMD = "create table if not exists FriendInfo (id integer primary key autoincrement, mid integer, unReadNums integer, friendType integer, nickName text, photo text)";
    ret = sqlite3_exec(m_pdb, sqlCMD.c_str(), nullptr, nullptr, nullptr);
    if (ret != SQLITE_OK) {
        CCLOG("create table faild");
    }
    
    /*
     struct Chat
     {
     int fromId;
     int toId;
     string content;
     int msgType;
     long createTime;
     int status;
     int roomID;
     
     long seqNo;
     string filePath;
     };
     */
    //创建Table
    sqlCMD = "create table if not exists Chat (id integer primary key autoincrement, mid integer, fromId integer, toId integer, content text, msgType integer, createTime integer64, status integer, roomID integer,contentType integer, seqNo integer64)";
    ret = sqlite3_exec(m_pdb, sqlCMD.c_str(), nullptr, nullptr, nullptr);
    if (ret != SQLITE_OK) {
        CCLOG("create table faild");
    }
    
//    sqlCMD = "create table if not exists test (id integer primary key autoincrement, uid integer)";
//    ret = sqlite3_exec(m_pdb, sqlCMD.c_str(), nullptr, nullptr, nullptr);
//    if (ret != SQLITE_OK) {
//        CCLOG("create table faild");
//    }
//    
//    sqlCMD = "insert into test('uid') values(50)";
//    ret = sqlite3_exec(m_pdb, sqlCMD.c_str(), nullptr, nullptr, nullptr);
//    if (ret != SQLITE_OK) {
//        CCLOG("create table faild");
//    }
    
    return true;
}

bool DataBase::addSession(MsgModel::Session* s)
{
    char buff[10240];
    snprintf(buff, sizeof(buff), "insert into Session('mid','type','msgType','fromId','content','createTime') values(%d, %d, %d, %d, '%s', %lld)", s->mid, s->type, s->msgType, s->fromId,s->content.c_str(),s->createTime);
    int ret = sqlite3_exec(m_pdb, buff, nullptr, nullptr, nullptr);
    if (ret != SQLITE_OK) {
        CCLOG("addSession data failed!");
        return false;
    }
    return true;
}

bool DataBase::updateSession(MsgModel::Session* s)
{
    char buff[10240];
    snprintf(buff, sizeof(buff), "update Session set type=%d,msgType=%d,fromId=%d,content='%s',createTime=%lld where mid=%d", s->type, s->msgType, s->fromId,s->content.c_str(),s->createTime,s->mid);
    int ret = sqlite3_exec(m_pdb, buff, nullptr, nullptr, nullptr);
    if (ret != SQLITE_OK) {
        CCLOG("updateSession data failed!");
        return false;
    }
    return true;
}

bool DataBase::deleteSession(MsgModel::Session* s)
{
    char buff[10240];
    snprintf(buff, sizeof(buff), "delete from Session where mid=%d", s->mid);
    int ret = sqlite3_exec(m_pdb, buff, nullptr, nullptr, nullptr);
    if (ret != SQLITE_OK) {
        CCLOG("deleteSession data failed!");
        return false;
    }
    return true;
}

std::map<int,MsgModel::Session*> DataBase::getAllSession()
{
    //"create table if not exists Session(id integer primary key autoincrement, mid integer, type integer, msgType integer, fromId integer, content text, createTime integer64)"
    char buff[10240];
    snprintf(buff, sizeof(buff), "%s", "SELECT * FROM Session");
    
    std::map<int,MsgModel::Session*> sessionList;
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(m_pdb, buff, -1, &stmt, nullptr) == SQLITE_OK) {
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            
            int mid = sqlite3_column_int(stmt, 1);
            
            int type = sqlite3_column_int(stmt, 2);
            
            int msgType = sqlite3_column_int(stmt, 3);
            
            int fromId = sqlite3_column_int(stmt, 4);
            
            char *content = (char *)sqlite3_column_text(stmt, 5);
            
            long long createTime = sqlite3_column_int64(stmt, 6);
            
            if (sessionList[mid] == NULL)sessionList[mid] = new MsgModel::Session;
            MsgModel::Session* s = sessionList[mid];
            s->mid = mid;
            s->type = type;
            s->fromId = fromId;
            s->msgType = msgType;
            s->content = content;
            s->createTime = createTime;
        }
        
        sqlite3_finalize(stmt);  
    }
    return sessionList;
}

bool DataBase::addFriend(MsgModel::FriendInfo* s)
{
    //"create table if not exists FriendInfo (id integer primary key autoincrement, uid integer primary key, unReadNums integer, friendType integer, nickName text, photo text)"
    char buff[10240];
    snprintf(buff, sizeof(buff), "insert into FriendInfo('mid','unReadNums','friendType','nickName','photo') values(%d, %d, %d, '%s', '%s')", s->mid, s->unReadNums, s->friendType,s->nickName.c_str(),s->photo.c_str());
    int ret = sqlite3_exec(m_pdb, buff, nullptr, nullptr, nullptr);
    if (ret != SQLITE_OK) {
        CCLOG("addFriend data failed!");
        return false;
    }
    return true;
}

bool DataBase::updateFriend(MsgModel::FriendInfo* s)
{
    char buff[10240];
    snprintf(buff, sizeof(buff), "update FriendInfo set unReadNums=%d,friendType=%d,nickName='%s',photo='%s' where mid=%d", s->unReadNums, s->friendType,s->nickName.c_str(),s->photo.c_str(),s->mid);
    int ret = sqlite3_exec(m_pdb, buff, nullptr, nullptr, nullptr);
    if (ret != SQLITE_OK) {
        CCLOG("updateFriend data failed!");
        return false;
    }
    return true;
}

std::map<long,MsgModel::FriendInfo*> DataBase::getAllFriendNormalInfo()
{
    char buff[10240];
    snprintf(buff, sizeof(buff), "%s", "SELECT * FROM FriendInfo where friendType != 4");
    return getAllFriendInfo(buff);
}

std::map<long,MsgModel::FriendInfo*> DataBase::getAllFriendGroupInfo()
{
    char buff[10240];
    snprintf(buff, sizeof(buff), "%s", "SELECT * FROM FriendInfo where friendType = 4");
    return getAllFriendInfo(buff);
}

std::map<long,MsgModel::FriendInfo*> DataBase::getAllFriendInfo(string sql)
{
    //"create table if not exists FriendInfo (id integer primary key autoincrement, uid integer, unReadNums integer, friendType integer, nickName text, photo text)"
    std::map<long,MsgModel::FriendInfo*> friendList;
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(m_pdb, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            
            int uid = sqlite3_column_int(stmt, 1);
            
            int unReadNums = sqlite3_column_int(stmt, 2);
            
            int friendType = sqlite3_column_int(stmt, 3);
            
            char *nickName = (char *)sqlite3_column_text(stmt, 4);
            
            char *photo = (char *)sqlite3_column_text(stmt, 5);
            
            if (friendList[uid] == NULL)friendList[uid] = new MsgModel::FriendInfo;
            MsgModel::FriendInfo* s = friendList[uid];
            s->mid = uid;
            s->unReadNums = unReadNums;
            s->friendType = friendType;
            s->nickName = nickName;
            s->photo = photo;
        }
        
        sqlite3_finalize(stmt);
    }
    return friendList;
}

bool DataBase::addChat(MsgModel::Chat* s)
{
    //"create table if not exists Chat (id integer primary key autoincrement, mid integer, fromId integer, toId integer, content text, msgType integer, createTime integer, status integer, roomID integer)"
    char buff[10240];
    snprintf(buff, sizeof(buff), "insert into Chat('mid','fromId','toId','content','msgType','createTime','status','roomID','contentType','seqNo') values(%d, %d, %d, '%s', '%d', '%lld', '%d', '%d','%d','%lld')", s->mid, s->fromId, s->toId,s->content.c_str(),s->msgType,s->createTime,s->status,s->roomID,s->contentType,s->seqNo);
    int ret = sqlite3_exec(m_pdb, buff, nullptr, nullptr, nullptr);
    if (ret != SQLITE_OK) {
        CCLOG("addChat data failed!");
        return false;
    }
    return true;
}

bool DataBase::updateChat(MsgModel::Chat* s)
{
    char buff[10240];
    if (s->seqNo != -1)
    {
        snprintf(buff, sizeof(buff), "update Chat set status=%d,createTime=%lld,content='%s' where seqNo=%lld", s->status, s->createTime,s->content.c_str(),s->seqNo);
    }
    else if(s->id != -1)
    {
        snprintf(buff, sizeof(buff), "update Chat set status=%d,createTime=%lld,content='%s' where id=%d", s->status, s->createTime,s->content.c_str(),s->id);
    }
    else
    {
        return false;
    }
    int ret = sqlite3_exec(m_pdb, buff, nullptr, nullptr, nullptr);
    if (ret != SQLITE_OK) {
        CCLOG("updateChat data failed!");
        return false;
    }
    return true;
}

bool DataBase::deleteChats(int uid)
{
    char buff[10240];
    snprintf(buff, sizeof(buff), "delete from Chat where mid=%d", uid);
    int ret = sqlite3_exec(m_pdb, buff, nullptr, nullptr, nullptr);
    if (ret != SQLITE_OK) {
        CCLOG("deleteChats data failed!");
        return false;
    }
    return true;
}

std::vector<MsgModel::Chat*> DataBase::getChat(int uid,int page, int pageSize, int beginid,int groupType)
{
    if(page == 0) page = 1;
    if(pageSize == 0) page = 20;
    
    //"create table if not exists Chat (id integer primary key autoincrement, mid integer, fromId integer, toId integer, content text, msgType integer, createTime integer64, status integer, roomID integer, seqNo integer64)"
    char buff[10240];
    if (page == 1)
    {
        snprintf(buff, sizeof(buff), "SELECT * FROM Chat where mid=%d and contentType=%d order by id desc limit %d offset %d", uid,groupType, pageSize,(page - 1) * pageSize);
    }
    else
    {
        snprintf(buff, sizeof(buff), "SELECT * FROM Chat where mid=%d and contentType=%d and id<%d order by id desc limit %d", uid,groupType, beginid, pageSize);
    }
    log("%s", buff);
    std::vector<MsgModel::Chat*> chatList;
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(m_pdb, buff, -1, &stmt, nullptr) == SQLITE_OK) {
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            
            int id = sqlite3_column_int(stmt, 0);
            int mid = sqlite3_column_int(stmt, 1);
            
            int fromId = sqlite3_column_int(stmt, 2);
            
            int toId = sqlite3_column_int(stmt, 3);
            
            char *content = (char *)sqlite3_column_text(stmt, 4);
            
            int msgType = sqlite3_column_int(stmt, 5);
            
            long long createTime = sqlite3_column_int64(stmt, 6);
            
            int status = sqlite3_column_int(stmt, 7);
            
            int roomID = sqlite3_column_int(stmt, 8);
            
            long long seqNo = sqlite3_column_int64(stmt, 9);
            
            MsgModel::Chat* s = new MsgModel::Chat;
            s->id = id;
            s->mid = mid;
            s->fromId = fromId;
            s->toId = toId;
            s->content = content;
            s->msgType = msgType;
            s->createTime = createTime;
            s->status = MSG_STATE::MSG_STATE_SUCC;
            s->roomID = roomID;
            s->seqNo = seqNo;
            chatList.push_back(s);
        }
        
        sqlite3_finalize(stmt);
    }
    return chatList;
}

bool DataBase::insert(std::string sql){
    int ret = sqlite3_exec(m_pdb, sql.c_str(), nullptr, nullptr, nullptr);
    if (ret != SQLITE_OK) {
        CCLOG("DataBase::insert failed!");
        return false;
    }
    return true;
}

bool DataBase::update(std::string sql){
    int ret = sqlite3_exec(m_pdb, sql.c_str(), nullptr, nullptr, nullptr);
    if (ret != SQLITE_OK) {
        log("%s update data failed!",sql.c_str());
        return false;
    }else {
        log("%s update data success!",sql.c_str());
        return true;
    }
}

std::string DataBase::select(std::string sql){
    Json::Value chatList;
    
    char** pResult;
    int nRow;
    int nCol;
    int nResult = sqlite3_get_table(m_pdb,sql.c_str(),&pResult,&nRow,&nCol,nullptr);
    if (nResult != SQLITE_OK)
    {
        return "";
    }
    
    string strOut;
    int nIndex = nCol;
    for(int i=0;i<nRow;i++)
    {
        Json::Value j_row;
        for(int j=0;j<nCol;j++)
        {
//            strOut+=pResult[j];
//            strOut+=":";
//            strOut+=pResult[nIndex];
//            strOut+="\n";
            j_row[j] = pResult[nIndex];
            ++nIndex;
        }
        chatList.append(j_row);
    }
    sqlite3_free_table(pResult);
    return chatList.toStyledString();
}
