#include "MsgModel.h"
#include "ZJH.h"
#include <arpa/inet.h>
#include "proto.h"
#include "../sqlite3/DataBase.h"
#include "../Scene/Msg/BeOfflineDialog.h"

MsgModel *model;
using namespace::cocos2d;

MsgModel* MsgModel::getInstance()
{
    if (model == NULL) {
        model = new MsgModel();
        model->init();
        model->retain();
    }
    
    return model;
}

void MsgModel::init()
{
    initAccountData();
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MsgModel::notificationReflash), "account_change", NULL);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MsgModel::notifyLogout), "account_logout", NULL);
    lastHeartBeatTime = -1;
}

void MsgModel::notifyLogout(Ref* msg)
{
    stop();
}

void MsgModel::notificationReflash(Ref* msg)
{
    curChatUID = -1;
    
    curSendList_tmp.clear();
    
    std::map<int,MsgModel::Session*>::iterator it;
    for(it = sessionList.begin();it != sessionList.end(); it ++)
    {
        MsgModel::Session* session = it->second;
        if (session)delete session;
    }
    sessionList.clear();
    
    std::map<long,FriendInfo*>::iterator itf;
    for(itf = friendList.begin();itf != friendList.end(); itf ++)
    {
        MsgModel::FriendInfo* friendInfo = itf->second;
        if (!friendInfo)continue;
        
        for (long i = friendInfo->chatList.size() - 1; i >= 0 ; i --)
        {
            MsgModel::Chat* chat = friendInfo->chatList[i];
            if (!chat)continue;
            delete chat;
        }
        friendInfo->chatList.clear();
        delete friendInfo;
    }
    sessionList.clear();
    
    initAccountData();
    
    __NotificationCenter::getInstance()->postNotification("Msg_reflash");//notify msg has new msg
    __NotificationCenter::getInstance()->postNotification("MyFriend_reflash");//notify msg has new msg
    
//    com::huizhi::im::protobuf::HeartRequest heartReq;
//    heartReq.set_type(1);
//    
//    std::string str = heartReq.SerializeAsString();
//    
//    CCTcpClient::getInstance(TCP_MSG)->send_data_pb(str,MSG_HEARTBEAT_REQ);
//    
//    com::huizhi::im::protobuf::MessageInfo sendMsg;
//    sendMsg.set_fromuserid(ZJHModel::getInstance()->uid);
//    sendMsg.set_touserid(ZJHModel::getInstance()->uid);
//    sendMsg.set_messagetype(MSG_TYPE_TEXT);
//    sendMsg.set_contenttype(SESSION_POINT_MESSAGE);
//    sendMsg.set_content("");
//    
//    std::string sendData = sendMsg.SerializeAsString();
//    
//    CCTcpClient::getInstance(TCP_MSG)->send_data_pb(sendData,MSG_UNREAD_NUM_REQ);
    
    start();
}

void MsgModel::initAccountData()
{
    DataBase::getInstance(ZJHModel::getInstance()->uid);
    sessionList = DataBase::getInstance(ZJHModel::getInstance()->uid)->getAllSession();
    friendList = DataBase::getInstance(ZJHModel::getInstance()->uid)->getAllFriendNormalInfo();
    friendGroupList = DataBase::getInstance(ZJHModel::getInstance()->uid)->getAllFriendGroupInfo();
    
    MsgModel::FriendInfo* fi = new MsgModel::FriendInfo;
    fi->mid = ZJHModel::getInstance()->uid;
    fi->nickName = ZJHModel::getInstance()->nick;
    fi->photo = ZJHModel::getInstance()->portrit;
    MsgModel::getInstance()->addFriend(fi);
    
    isShowBeOfflineDialog = false;
    
    connect_count = 0;
    
    lastHeartBeatTime = -1;
    
    curUID = ZJHModel::getInstance()->uid;
}

void MsgModel::stop()
{
    cocos2d::Director::getInstance()->getScheduler()->unschedule(schedule_selector(MsgModel::getData),this);
    cocos2d::Director::getInstance()->getScheduler()->unschedule(schedule_selector(MsgModel::doHeartbeat),this);
}

void MsgModel::start()
{
    if (curUID != ZJHModel::getInstance()->uid) initAccountData();
    
//    log("MsgModel::start ip[%s] port[%d]",ip.c_str(),port);
    CCTcpClient::getInstance(TCP_MSG)->setProtoType(ProtoType::PROTOBUF_WANBEI);
    CCTcpClient::getInstance(TCP_MSG)->reset_response_queue();
    CCTcpClient::getInstance(TCP_MSG)->set_host_port(ip,port);
//    CCTcpClient::getInstance(TCP_MSG)->set_host_port("188.188.1.147",8888);
    CCTcpClient::getInstance(TCP_MSG)->connect();
    
    cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(MsgModel::getData),this,0,false);
    
    cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(MsgModel::doHeartbeat),this,30,false);
    
    log("MsgModel::start");
    
    isShowBeOfflineDialog = false;
}

void MsgModel::doHeartbeat(float dt)
{
    connect_count = 0;
    
    com::huizhi::im::protobuf::HeartRequest heartReq;
    heartReq.set_type(0);
    
    std::string str = heartReq.SerializeAsString();
    
    CCTcpClient::getInstance(TCP_MSG)->send_data_pb(str,MSG_HEARTBEAT_REQ);
}

void MsgModel::getData(float dt)
{
    if (!isShowBeOfflineDialog && lastHeartBeatTime != -1 && time(NULL) - lastHeartBeatTime > 120)
    {
        CCTcpClient::getInstance(TCP_MSG)->reset_response_queue();
        CCTcpClient::getInstance(TCP_MSG)->connect();
        lastHeartBeatTime = time(NULL);
        return;
    }
    
    CCTcpResponse *response = CCTcpClient::getInstance(TCP_MSG)->get();
    if (response)
    {
//        log("MsgModel::cmd[%d]",response->cmd);
        if (response->cmd == DISCONNECT_RES || response->cmd == CONNECT_ERROR_RES) //关闭
        {
            log("MsgModel::response->cmd == DISCONNECT_RES");
            if(!isShowBeOfflineDialog)
            {
//                connect_count ++;
//                if (connect_count < 20)
//                {
                    CCTcpClient::getInstance(TCP_MSG)->connect();
//                }
            }
        }
        else if (response->cmd == CONNECT_OK_RES)
        {
            log("MsgModel::response->cmd == CONNECT_OK_RES");
            com::huizhi::im::protobuf::HeartRequest heartReq;
            heartReq.set_type(1);
            
            std::string str = heartReq.SerializeAsString();
            
            CCTcpClient::getInstance(TCP_MSG)->send_data_pb(str,MSG_HEARTBEAT_REQ);
            
            com::huizhi::im::protobuf::MessageInfo sendMsg;
            sendMsg.set_fromuserid(ZJHModel::getInstance()->uid);
            sendMsg.set_touserid(ZJHModel::getInstance()->uid);
            sendMsg.set_messagetype(MSG_TYPE_TEXT);
            sendMsg.set_contenttype(SESSION_POINT_MESSAGE);
            sendMsg.set_content("");
            
            std::string sendData = sendMsg.SerializeAsString();
            
            CCTcpClient::getInstance(TCP_MSG)->send_data_pb(sendData,MSG_UNREAD_NUM_REQ);
            
//            join_group(3212);
        }
        else if (response->cmd == RECV_DATA_OK_RES)
        {
            connect_count = 0;
            handler_cmd(response->id,response->data,response->seqNo);
        }
        else if (response->cmd == SEND_DATA_ERROR_RES)
        {
//            CCTcpClient::getInstance(TCP_MSG)->connect();
            
            std::map<long,MsgModel::Chat*>::iterator it;
            for(it = curSendList_tmp.begin();it != curSendList_tmp.end(); it ++)
            {
                MsgModel::Chat *chat = it->second;
                if(chat)
                {
                    chat->status = MSG_STATE::MSG_STATE_FAIL;
                    this->updateChat(chat);
                }
            }
            
            curSendList_tmp.clear();
        }
        delete response;
    }
    else
    {
    }
}

void MsgModel::handler_cmd(int cmd,std::string &data,long long seqNo)
{
    switch (cmd)
    {
        case MSG_HEARTBEAT_RES:
        {
            com::huizhi::im::protobuf::HeartResponse heartRsp;
            heartRsp.ParseFromString(data);
//            log("receiv data %d",heartRsp.result());
            lastHeartBeatTime = time(NULL);
            break;
        }
            
        case MSG_BE_OUT_RES:
        {
            log("receiv data be out");
            Director::getInstance()->getRunningScene()->addChild(BeOfflineDialog::create(),100000);
            isShowBeOfflineDialog = true;
            break;
        }
            
        case MSG_RECIEVE_MSG:
        {
            com::huizhi::im::protobuf::MessageInfo msg;
            msg.ParseFromString(data);

            log("MSG_RECIEVE_MSG receiv data from uid[%d] msgType[%d] contentType[%d] content[%s]",msg.fromuserid(),msg.messagetype(),msg.contenttype(),msg.content().c_str());
            
            int fromUserId = msg.fromuserid();
            std::string fromNickName = msg.fromnickname();
            std::string fromUserPhoto = msg.fromuserphoto();
            
            int toUserId = msg.touserid();
            int messageType = msg.messagetype();
            std::string content = msg.content();
            long long time = msg.time();
            long long messageId = msg.messageid();
            
            com::huizhi::im::protobuf::MessageResponse res;
            res.set_result(0);
            res.set_messageid(messageId);
            res.set_fromuserid(fromUserId);
            res.set_touserid(toUserId);
            res.set_created(time);
            std::string sendData = res.SerializeAsString();
            CCTcpClient::getInstance(TCP_MSG)->send_data_pb(sendData,MSG_RECIEVE_MSG_RES_REQ);
            if(msg.contenttype() == CONTENT_CLUB_SPECAL){
                msg.set_contenttype(CONTENT_CLUB);
            }
            int contentType = msg.contenttype();//point to point or other
            if(msg.contenttype() == CONTENT_CLUB){
                //加人
                FriendInfo* fi = new FriendInfo;
                fi->mid = toUserId;
                fi->nickName = fromNickName;
                fi->photo = fromUserPhoto;
                if (fromUserId != curChatUID)
                {
                    if (fi->unReadNums == -1)
                    {
                        fi->unReadNums = 1;
                    }
                    else
                    {
                        fi->unReadNums ++;
                    }
                }
                addFriend(fi);

                FriendInfo* fiClub = new FriendInfo;
                fiClub->mid = fromUserId;
                fiClub->nickName = "新俱乐部消息";
                fiClub->photo = "";
                fiClub->friendType = FRIEND_TYPE_GROUP;
                if (fromUserId != curChatUID)
                {
                    if (fiClub->unReadNums == -1)
                    {
                        fiClub->unReadNums = 1;
                    }
                    else
                    {
                        fiClub->unReadNums ++;
                    }
                }
                addFriend(fiClub,false);
            }else{
                //加人
                FriendInfo* fi = new FriendInfo;
                fi->mid = fromUserId;
                fi->nickName = fromNickName;
                fi->photo = fromUserPhoto;
                if (fromUserId != curChatUID)
                {
                    if (fi->unReadNums == -1)
                    {
                        fi->unReadNums = 1;
                    }
                    else
                    {
                        fi->unReadNums ++;
                    }
                }
                addFriend(fi);
            }
            
            
            Chat* chat = new Chat();
            chat->mid = fromUserId;
            chat->fromId = fromUserId;
            chat->toId = toUserId;
            chat->msgType = messageType;
            chat->content = content;
            chat->createTime = time;
            chat->contentType = contentType;//消息所属类型 私聊消息 普通群 或者俱乐部群
            chat->status = MSG_STATE::MSG_STATE_SUCC;
            
            if (chat->msgType == MSG_TYPE_AUDIO)
            {
                std::string abaspath = Utils::getSoundPathUri(ZJHModel::getInstance()->uid,fromUserId,ZJHModel::getInstance()->uid);
                
                std::string filepath = Utils::getSoundSavePath(abaspath);
                
                CCHttpAgent::getInstance()->_makeMultiDir(filepath.c_str());
                
                log("sound file path[%s]",filepath.c_str());
                
                {
                    const char* content_buf = content.c_str();
    
                    int content_len = content.length() - 4;
                    char sound_lenbuf[4];
                    char sound_content[content_len];
                    
                    memcpy(sound_lenbuf, content_buf, 4);
                    memcpy(sound_content, content_buf + 4, content_len);
                    
                    int sound_len;
                    memcpy(&sound_len, sound_lenbuf, 4);
                    
                    chat->roomID = ntohl(sound_len);
                    chat->content = abaspath;
                    
                    FILE *fp = fopen(filepath.c_str(), "wb+");

                    if (fp)
                    {
                        fwrite(sound_content, content_len, 1, fp);
                        fclose(fp);
                    }

//                    PlatformHelper::playRecord(filepath);
                }
            }
            
            if (msg.contenttype() == CONTENT_POINT_TO_POINT)
            {
                addChat(chat);
            
                Session* s = new Session;
                s->mid = fromUserId;
                s->fromId = fromUserId;
                s->type = contentType;
                s->msgType = messageType;
                s->content = content;
                s->createTime = time;
                addSession(s);
                
                __NotificationCenter::getInstance()->postNotification("Msg_reflash");//notify msg has new msg
                __NotificationCenter::getInstance()->postNotification("hasNewMsg_notify");//notify msg has new msg
                if (fromUserId == curChatUID)
                {
                    __NotificationCenter::getInstance()->postNotification("ChatFriends_reflash");//notify chat view
                }
            }
            else if (msg.contenttype() == CONTENT_GROUP)
            {
                if (curGroupID == fromUserId) //fromUserId  群id
                {
                    roomChatList.push_back(chat);
                    __NotificationCenter::getInstance()->postNotification("RoomChat_reflash");
                    
                    Json::Value json;
                    json["uid"] = toUserId;      //真正发送人id
                    json["name"] = fromNickName; //真正发送人昵称
                    json["content"] = content;
                    if (messageType == MESSAGE_TYPE::MSG_TYPE_IMAGE)
                    {
                        json["content"] = "[图片]";
                    }
                    else if (messageType == MESSAGE_TYPE::MSG_TYPE_AUDIO)
                    {
                        json["type"] = messageType;
                        json["filepath"] = chat->content;
                        json["time"] = chat->roomID;
                        json["content"] = "[语音]";
                    }
                    __NotificationCenter::getInstance()->postNotification("notify_room_chat",__String::create(json.toStyledString()));
                }
            }else if(msg.contenttype() == CONTENT_CLUB){
                //加群
                chat->fromId = toUserId;
                addChat(chat);
                
                Session* s = new Session;
                s->mid = fromUserId;
                s->fromId = toUserId;
                s->type = contentType;
                s->msgType = messageType;
                s->content = content;
                s->createTime = time;
                addSession(s);
                
                __NotificationCenter::getInstance()->postNotification("Msg_reflash");//notify msg has new msg
                __NotificationCenter::getInstance()->postNotification("hasNewMsg_notify");//notify msg has new msg
                if (fromUserId == curChatUID)
                {
                    __NotificationCenter::getInstance()->postNotification("ChatFriends_reflash");//notify chat view
                }
            }
            break;
        }
        
        case MSG_SEND_RES:
        {
            com::huizhi::im::protobuf::MessageResponse msg;
            msg.ParseFromString(data);
            
            log("MSG_SEND_RES receiv data from result[%d]",msg.result());
            
            Chat* chat = curSendList_tmp[seqNo];
            if (chat)
            {
                chat->status = MSG_STATE::MSG_STATE_SUCC;
                chat->createTime = msg.created();
                curSendList_tmp[seqNo] = NULL;
                this->updateChat(chat);
            }
            
            break;
        }
            
        case MSG_UNREAD_NUM_RES:
        {
            com::huizhi::im::protobuf::UnReadInfo msg;
            msg.ParseFromString(data);
            
            log("MSG_RECIEVE_MSG receiv data from MSG_UNREAD_NUM_RES unreadsize[%d]",msg.unreadsize());
            
            int size = msg.uids_size();
            for (int i = 0; i < size; i ++)
            {
                com::huizhi::im::protobuf::Counter counter = msg.uids(i);
                int uid = counter.uid();
                int count = counter.count();
                
                log("you have unread msg from uid[%d] count[%d]",uid,count);
                
                com::huizhi::im::protobuf::UnReadMessageInfo msg;
                msg.set_fromuserid(uid);
                msg.set_touserid(ZJHModel::getInstance()->uid);
                
                std::string sendData = msg.SerializeAsString();
                
                CCTcpClient::getInstance(TCP_MSG)->send_data_pb(sendData,MSG_UNREAD_LIST_REQ);
            }
            
            break;
        }
            
        case MSG_UNREAD_LIST_RES:
        {
            com::huizhi::im::protobuf::UnReadMessageInfo msg;
            msg.ParseFromString(data);
            
            log("MSG_RECIEVE_MSG receiv data from MSG_UNREAD_NUM_RES unreadsize[%d]",msg.fromuserid());
            int fromUserId = msg.fromuserid();
            
            std::string fromUserPhoto = msg.fromuserphoto();
            std::string fromNickName = msg.fromnickname();
            int toUserId = msg.touserid();
            int msg_size = msg.messages_size();
            
            FriendInfo* fi = new FriendInfo;
            fi->mid = fromUserId;
            fi->nickName = fromNickName;
            fi->photo = fromUserPhoto;
            fi->unReadNums = msg_size;
            addFriend(fi);
            
            for (int i = 0; i < msg_size; i ++)
            {
                com::huizhi::im::protobuf::UnReadMessage msgContent = msg.messages(i);
                int type = msgContent.type();
                std::string content = msgContent.content();
                long long created = msgContent.created();
                
                Chat* chat = new Chat;
                chat->mid = fromUserId;
                chat->fromId = fromUserId;
                chat->toId = toUserId;
                chat->msgType = type;
                chat->content = content;
                chat->createTime = created;
                chat->status = MSG_STATE::MSG_STATE_SUCC;
                
                if (chat->msgType == MSG_TYPE_AUDIO)
                {
                    std::string abaspath = Utils::getSoundPathUri(ZJHModel::getInstance()->uid,fromUserId,ZJHModel::getInstance()->uid);
                    
                    std::string filepath = Utils::getSoundSavePath(abaspath);
                    
                    CCHttpAgent::getInstance()->_makeMultiDir(filepath.c_str());
                    
                    log("sound file path[%s]",filepath.c_str());
                    
                    {
                        const char* content_buf = content.c_str();
                        
                        int content_len = content.length() - 4;
                        char sound_lenbuf[4];
                        char sound_content[content_len];
                        
                        memcpy(sound_lenbuf, content_buf, 4);
                        memcpy(sound_content, content_buf + 4, content_len);
                        
                        int sound_len;
                        memcpy(&sound_len, sound_lenbuf, 4);
                        
                        chat->roomID = ntohl(sound_len);
                        chat->content = abaspath;
                        
                        FILE *fp = fopen(filepath.c_str(), "wb+");
                        
                        if (fp)
                        {
                            fwrite(sound_content, content_len, 1, fp);
                            fclose(fp);
                        }
                        
                        //                    PlatformHelper::playRecord(filepath);
                    }
                }
                
                if (i == msg_size - 1)
                {
                    Session* s = new Session;
                    s->mid = fromUserId;
                    s->fromId = fromUserId;
                    s->type = MESSAGE_TYPE::SESSION_POINT_MESSAGE;
                    s->msgType = type;
                    s->content = content;
                    s->createTime = created;
                    addSession(s);
                }
                
                addChat(chat);
            }
            
            __NotificationCenter::getInstance()->postNotification("Msg_reflash");//notify msg has new msg
            __NotificationCenter::getInstance()->postNotification("hasNewMsg_notify");//notify msg has new msg
            break;
        }
            
        case MSG_RECIEVE_NOTIFY:
        {
            com::huizhi::im::protobuf::Notice msg;
            msg.ParseFromString(data);
            
            int type = msg.type();
            int fromUserId = msg.userid();
            long long noticeId = msg.noticeid();
            long long time = Utils::getCurrentTime();
            
            std::string fromNickName = msg.nickname();
            std::string fromUserPhoto = msg.photo();
            std::string content = msg.content();
            
            com::huizhi::im::protobuf::NoticeResponse res;
            res.set_result(0);
            res.set_noticeid(noticeId);
            std::string sendData = res.SerializeAsString();
            CCTcpClient::getInstance(TCP_MSG)->send_data_pb(sendData,MSG_RECIEVE_NOTIFY_SUCC_REQ);
            
            log("you have receive notify from type[%d] uid[%d] msg[%s]",type,fromUserId,content.c_str());
            
            int sessionType = 0;
            Session* s = NULL;
            if(type == NOTIFY_TYPE::MATCH_NOTIFY){
                __NotificationCenter::getInstance()->postNotification("im_notify",__String::create(content));
            }else if(type == NOTIFY_TYPE::CLUB_TICHU || type == NOTIFY_TYPE::CLUB_REJECT_JOIN ||type == NOTIFY_TYPE::CLUB_AGREE_JOIN ){
                sessionType = MESSAGE_TYPE::SESSION_PAIJU_DONGTAI;
                //保存消息
                Json::Value json = Utils::ParseJsonStr(content);
                json["type"] = type;
                fromUserId = SESSION_PAIJU_DONGTAI;
                type = MESSAGE_TYPE::MSG_TYPE_TEXT;
                
                FriendInfo* fi = new FriendInfo;
                fi->mid = fromUserId;
                fi->unReadNums = 1;
                addFriend(fi);
                
                Chat* chat = new Chat();
                chat->mid = fromUserId;
                chat->fromId = fromUserId;
                chat->toId = ZJHModel::getInstance()->uid;
                chat->msgType = type;
                chat->content = json.toStyledString();
                chat->createTime = time;
                chat->status = MSG_STATE::MSG_STATE_SUCC;
                addChat(chat);
                
                content = json.toStyledString();
                s = new Session;
                if(msg.type() == NOTIFY_TYPE::CLUB_TICHU){
                    Session* tempSession = new Session();
                    tempSession->mid = json["groupId"].asInt();
                    DataBase::getInstance(ZJHModel::getInstance()->uid)->deleteSession(tempSession);
                    sessionList.erase(tempSession->mid);
                    delete tempSession;
                }
                //发送事件给js
                Json::Value eventjson;
                eventjson["event"] = 102;
                eventjson["content"] = json.toStyledString();
                __NotificationCenter::getInstance()->postNotification("js_module_notify",__String::create(eventjson.toStyledString()));
            }else if(type == NOTIFY_TYPE::ROOM_CARD_CHARG_SUCCESS){
                log("===========ROOM_CARD_CHARG_SUCCESS===========％s",content.c_str());
                /*{"content":"杜海(134204)给您充值10张房卡已到账!","number":10,"sendId":134204,"sendName":"杜海","sendPhoto":"/chess_img/head/134204/16121410482957800_120x120.jpg"}*/
                //房卡充值成功
                sessionType = MESSAGE_TYPE::SESSION_ROOM_CHARG_LOG;
                //保存消息
                Json::Value json = Utils::ParseJsonStr(content);
                fromUserId = SESSION_ROOM_CHARG_LOG;
                type = MESSAGE_TYPE::MSG_TYPE_TEXT;
                
                FriendInfo* fi = new FriendInfo;
                fi->mid = fromUserId;
                fi->unReadNums = 1;
                addFriend(fi);
                
                
                Chat* chat = new Chat();
                chat->mid = fromUserId;
                chat->fromId = fromUserId;
                chat->toId = ZJHModel::getInstance()->uid;
                chat->msgType = type;
                chat->content = content;
                chat->createTime = time;
                chat->status = MSG_STATE::MSG_STATE_SUCC;
                addChat(chat);
                
                s = new Session;
                __NotificationCenter::getInstance()->postNotification("money_change");
            }else if (type == NOTIFY_TYPE::GAME_CHANGE || type == NOTIFY_TYPE::GAME_INVITED )
            {
                sessionType = MESSAGE_TYPE::SESSION_PAIJU_DONGTAI;
                //动态通知流程  收到通知消息后  再去从新请求一次服务器的最新参数
                log("===========动态===========％s",content.c_str());
                Json::Value json = Utils::ParseJsonStr(content);
                requestDynamicMessage(json);
            }
            else if (type == NOTIFY_TYPE::ADD_FRIEND)
            {
                sessionType = MESSAGE_TYPE::SESSION_POINT_MESSAGE;
                
                Json::Value json = Utils::ParseJsonStr(content);
                fromUserId = json["sendId"].asInt();
                content = "我已经把您加为好友了。";//json["content"].asString();
                fromNickName = json["sendName"].asString();
                fromUserPhoto = json["sendPhoto"].asString();
//                time = time;
                type = MESSAGE_TYPE::MSG_TYPE_SYSTEM_MEG;
                
                FriendInfo* fi = new FriendInfo;
                fi->mid = fromUserId;
                fi->nickName = fromNickName;
                fi->photo = fromUserPhoto;
                if (fromUserId != curChatUID)
                {
                    if (fi->unReadNums == -1)
                    {
                        fi->unReadNums = 1;
                    }
                    else
                    {
                        fi->unReadNums ++;
                    }
                }
                addFriend(fi);
                
                Chat* chat = new Chat();
                chat->mid = fromUserId;
                chat->fromId = fromUserId;
                chat->toId = ZJHModel::getInstance()->uid;
                chat->msgType = type;
                chat->content = content;
                chat->createTime = time;
                chat->status = MSG_STATE::MSG_STATE_SUCC;
                addChat(chat);
                
                s = new Session;
            }
            else if (type == AGAINST_ROOM_INVITE)
            {
                /*
                 {"bottom":1,"disPlayMode":1,"duration":120,"invite":"邀请您加入炸金花对局房间：玩呗101379的对局房间","number":5,"roomId":1196,"sendId":101379,"sendName":"玩呗101379","sendPhoto":"/chess_img/head/default/00.jpg","takeIn":500,"timestamp":1469446056933}
                 */
                
                sessionType = MESSAGE_TYPE::SESSION_POINT_MESSAGE;
                Json::Value json = Utils::ParseJsonStr(content);
                fromUserId = json["sendId"].asInt();
                fromNickName = json["sendName"].asString();
                fromUserPhoto = json["sendPhoto"].asString();
//                time = json["timestamp"].asDouble();
                int roomId = json["roomId"].asInt();
                
                type = MESSAGE_TYPE::MSG_TYPE_SYSTEM_AGAINST_INVITE;
                
                if (fromUserId == ZJHModel::getInstance()->uid)return;
                
                FriendInfo* fi = new FriendInfo;
                fi->mid = fromUserId;
                fi->nickName = fromNickName;
                fi->photo = fromUserPhoto;
                fi->unReadNums = 1;
                addFriend(fi);
                
                Chat* chat = new Chat();
                chat->mid = fromUserId;
                chat->fromId = fromUserId;
                chat->toId = ZJHModel::getInstance()->uid;
                chat->msgType = type;
                chat->content = content;
                chat->createTime = time;
                chat->roomID = roomId;
                chat->status = MSG_STATE::MSG_STATE_SUCC;
                addChat(chat);

                content = json["invite"].asString();
                s = new Session;
            }
            else if (type == GROUP_INVITE)
            {
                sessionType = MESSAGE_TYPE::SESSION_TRIBLE_GROUP_MESSAGE;
            }else if(type == JOIN_TRIBLE_REQUEST){
                /*{"content":"test","sendId":10003,"sendName":"俱乐部通知","targetId":1000317,"timestamp":1474964312504}*/
                //部落加入申请
                sessionType = MESSAGE_TYPE::SESSION_JOIN_TRIBLE_REQUEST;
                //保存消息
                Json::Value json = Utils::ParseJsonStr(content);
                fromUserId = 10003;
                std::cout<<"======收到俱乐部加入通知======"<<content<<endl;
                type = MESSAGE_TYPE::MSG_TYPE_TEXT;
                
                FriendInfo* fi = new FriendInfo;
                fi->mid = fromUserId;
                fi->unReadNums = 1;
                addFriend(fi);
                
                
                Chat* chat = new Chat();
                chat->mid = fromUserId;
                chat->fromId = fromUserId;
                chat->toId = ZJHModel::getInstance()->uid;
                chat->msgType = type;
                chat->content = content;
                chat->createTime = time;
                chat->status = MSG_STATE::MSG_STATE_SUCC;
                addChat(chat);
                
                s = new Session;
                
            }else if(type == AGAINST_ROOM_DISS){
                /*{"content":"您的对局“小江的对局房间” 房间,由于长时间没有玩，现在已被系统自动解散！系统退回房卡数目1张","gameLogo":"/chess_img/game/logo/1610251801193206.png","gameName":"四川麻将定制版本","roomId":6627,"roomName":"小江的对局房间","sendId":10002,"sendName":"用户通知","timestamp":1479534825960}*/
                sessionType = MESSAGE_TYPE::SESSION_AGAINST_NOTIFY;
                //保存消息
                Json::Value json = Utils::ParseJsonStr(content);
                fromUserId = 10002;
                fromNickName = json["sendName"].asString();
                fromUserPhoto = json["sendPhoto"].asString();
                
                json["type"] = 1;//声明一下是解散房间的对局通知消息
                std::cout<<"===========AGAINST_ROOM_DISS==========="<<json.toStyledString()<<endl;
                
                type = MESSAGE_TYPE::MSG_TYPE_TEXT;
                
                FriendInfo* fi = new FriendInfo;
                fi->mid = fromUserId;
                fi->unReadNums = 1;
                addFriend(fi);
                
                
                Chat* chat = new Chat();
                chat->mid = fromUserId;
                chat->fromId = fromUserId;
                chat->toId = ZJHModel::getInstance()->uid;
                chat->msgType = type;
                chat->content = json.toStyledString();
                content = json.toStyledString();
                chat->createTime = time;
                chat->status = MSG_STATE::MSG_STATE_SUCC;
                addChat(chat);
                
                s = new Session;
            }
            else if(type == AGAINST_NOTIFY){
                //人满的消息通知
                /*{"code":"940895","gameLogo":"ess_img/game/logo/1603281513352371.png","gameName":"经典拼三张","number":2,"roomId":3024,"roomName":"玩呗100054的对局房间","sendId":10002,"sendName":"用户通知","timestamp":1474614661333}*/
                //如果正在房间  则直接丢弃消息
//                if(ZJHModel::getInstance()->enterGame){
//                    log("正在房间中，不发送通知消息");
//                    return;
//                }
                
                sessionType = MESSAGE_TYPE::SESSION_AGAINST_NOTIFY;
                //保存消息
                Json::Value json = Utils::ParseJsonStr(content);
                fromUserId = json["sendId"].asInt();
                fromNickName = json["sendName"].asString();
                fromUserPhoto = json["sendPhoto"].asString();
                
                type = MESSAGE_TYPE::MSG_TYPE_TEXT;
                
                FriendInfo* fi = new FriendInfo;
                fi->mid = fromUserId;
                fi->unReadNums = 1;
                addFriend(fi);
                
                
                Chat* chat = new Chat();
                chat->mid = fromUserId;
                chat->fromId = fromUserId;
                chat->toId = ZJHModel::getInstance()->uid;
                chat->msgType = type;
                chat->content = content;
                chat->createTime = time;
                chat->status = MSG_STATE::MSG_STATE_SUCC;
                addChat(chat);

                s = new Session;
            }
            else
            {
                if (type == RECHARGE_SUCCESS)
                {
                    
                }
                else if (type == DUOBAO_SUCCESS)
                {
                    
                }
                else if (type == ZHIBO_EVENT)
                {
                    
                }
                else if (type == AGAINST_ROOM_START)
                {
                    Json::Value json = Utils::ParseJsonStr(content);
                    int roomId = json["roomId"].asInt();
                    __NotificationCenter::getInstance()->postNotification("notify_against_room_start", __Integer::create(roomId));
                }
            }
            
            if (s)
            {
                s->mid = fromUserId;
                s->fromId = fromUserId;
                s->type = sessionType;
                s->msgType = type;
                s->content = content;
                s->createTime = time;
                addSession(s);
                
                __NotificationCenter::getInstance()->postNotification("Msg_reflash");//notify msg has new msg
                __NotificationCenter::getInstance()->postNotification("hasNewMsg_notify");//notify msg has new msg
                if (fromUserId == curChatUID)
                {
                    __NotificationCenter::getInstance()->postNotification("ChatFriends_reflash");//notify chat view
                }            }

            break;
        }
        
        case MSG_JOIN_GROUP_RES:
        {
            com::huizhi::im::protobuf::GroupJoinResponse msg;
            msg.ParseFromString(data);
            log("join in groudid result[%d]",msg.result());
            break;
        }
            
        default:
            break;
    }
}

//请求服务器动态真实数据
void MsgModel::requestDynamicMessage(Json::Value json){
    json["ids"] = json["id"];
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* dynamicPacket = CCHttpAgent::getInstance()->packets[tag];
        if (dynamicPacket && dynamicPacket->status != 0)
        {
            if (dynamicPacket->status != 3)
            {
                return;
            }
            
            dynamicPacket->status = 0;
            if (dynamicPacket->resultIsOK())
            {
                //真实数据返回  添加到数据库中
                int fromUserId = json["system_id"].asInt();
                FriendInfo* fi = new FriendInfo;
                fi->mid = fromUserId;
                fi->unReadNums = 1;
                addFriend(fi);
                
                
                Chat* chat = new Chat();
                chat->mid = fromUserId;
                chat->fromId = fromUserId;
                chat->toId = ZJHModel::getInstance()->uid;
                chat->msgType = MESSAGE_TYPE::MSG_TYPE_SYSTEM_GAME_CHANGE;
                chat->content = dynamicPacket->recvVal["resultMap"]["dynamicList"]["0"].asString();
                chat->createTime = Utils::getCurrentTime();
                chat->status = MSG_STATE::MSG_STATE_SUCC;
                addChat(chat);
                
                Session *s = new Session;
                if (s)
                {
                    s->mid = fromUserId;
                    s->fromId = fromUserId;
                    s->type = MESSAGE_TYPE::SESSION_PAIJU_DONGTAI;
                    s->msgType = chat->msgType;
                    s->content = chat->content;
                    s->createTime = chat->createTime;
                    addSession(s);
                    
                    __NotificationCenter::getInstance()->postNotification("Msg_reflash");//notify msg has new msg
                    __NotificationCenter::getInstance()->postNotification("hasNewMsg_notify");//notify msg has new msg
                    if (fromUserId == curChatUID)
                    {
                        __NotificationCenter::getInstance()->postNotification("ChatFriends_reflash");//notify chat view
                    }
                }
            }
            
            CCHttpAgent::getInstance()->packets[tag] = NULL;
            delete dynamicPacket;
        }
    },"game/dynamic",json.toStyledString());
}

void MsgModel::sendText(std::string text,int groupType)
{
    com::huizhi::im::protobuf::MessageInfo sendMsg;
    sendMsg.set_fromuserid(ZJHModel::getInstance()->uid);
    sendMsg.set_touserid(MsgModel::getInstance()->curChatUID);
    sendMsg.set_messagetype(MSG_TYPE_TEXT);
    sendMsg.set_contenttype(groupType);
    sendMsg.set_content(text);
    log("======发送消息类型==%d==",groupType);
    std::string sendData = sendMsg.SerializeAsString();
    
    long long seqNo = CCTcpClient::getInstance(TCP_MSG)->send_data_pb(sendData,MSG_SEND_REQ);
    
    MsgModel::Chat* chat = new MsgModel::Chat();
    chat->mid = sendMsg.touserid();
    chat->seqNo = seqNo;
    chat->fromId = sendMsg.fromuserid();
    chat->toId = sendMsg.touserid();
    chat->msgType = sendMsg.messagetype();
    chat->content = sendMsg.content();
    chat->createTime = Utils::getCurrentTime();
    chat->status = MSG_STATE::MSG_STATE_SENDING;
    chat->contentType = groupType;
    chat = addChat(chat);
    
    MsgModel::getInstance()->curSendList_tmp[seqNo] = chat;
    
    __NotificationCenter::getInstance()->postNotification("ChatFriends_reflash");
    
    Session* s = new Session;
    s->mid = chat->mid;
    s->fromId = chat->fromId;
    if(groupType == CONTENT_CLUB){
        s->type = MESSAGE_TYPE::SESSION_TRIBLE_GROUP_MESSAGE;
    }else{
        s->type = MESSAGE_TYPE::SESSION_POINT_MESSAGE;
    }
    
    s->msgType = chat->msgType;
    s->content = chat->content;
    s->createTime = chat->createTime;
    addSession(s);
}

void MsgModel::sendImg(std::string filePath,int groupType)
{
    com::huizhi::im::protobuf::MessageInfo* sendMsg = new com::huizhi::im::protobuf::MessageInfo;
    sendMsg->set_fromuserid(ZJHModel::getInstance()->uid);
    sendMsg->set_touserid(MsgModel::getInstance()->curChatUID);
    sendMsg->set_messagetype(MSG_TYPE_IMAGE);
    sendMsg->set_contenttype(groupType);
    sendMsg->set_content(filePath);
    log("======发送消息类型==%d==",groupType);
    MsgModel::Chat* chat = new MsgModel::Chat();
    chat->seqNo = Utils::getCurrentTime();
    chat->mid = sendMsg->touserid();
    chat->fromId = sendMsg->fromuserid();
    chat->toId = sendMsg->touserid();
    chat->msgType = sendMsg->messagetype();
    chat->content = sendMsg->content();
    chat->createTime = Utils::getCurrentTime();
    chat->status = MSG_STATE::MSG_STATE_SENDING;
    chat->contentType = groupType;
    Chat* schat = addChat(chat);
    
    Session* s = new Session;
    s->mid = schat->mid;
    s->fromId = schat->fromId;
    if(groupType == CONTENT_CLUB){
        s->type = MESSAGE_TYPE::SESSION_TRIBLE_GROUP_MESSAGE;
    }else{
        s->type = MESSAGE_TYPE::SESSION_POINT_MESSAGE;
    }
    s->msgType = schat->msgType;
    s->content = "[图片]";
    s->createTime = schat->createTime;
    addSession(s);
    
    __NotificationCenter::getInstance()->postNotification("ChatFriends_reflash");
    
    char buf[128];
    snprintf(buf, sizeof(buf), "%ld",schat->createTime);
    
    //post image to server
    CCHttpAgent::getInstance()->sendHttpImagePost("image/uploadImImage", filePath, buf,[=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        CCHttpAgent::getInstance()->packets.erase(tag);
        CCHttpAgent::getInstance()->callbacks.erase(tag);
        if (loginPacket->status != 3)
        {
            schat->status = MSG_STATE::MSG_STATE_FAIL;
            this->updateChat(schat);
            delete loginPacket;
            return;
        }
        
        loginPacket->status = 0;
        
        if (loginPacket->resultIsOK())
        {
            schat->status = MSG_STATE::MSG_STATE_SUCC;
            this->updateChat(schat);
            
            sendMsg->set_content(loginPacket->recvVal["resultMap"]["imageUrl"].asString());
            schat->content = loginPacket->recvVal["resultMap"]["imageUrl"].asString();
            std::string sendData = sendMsg->SerializeAsString();
            long long seqNo = CCTcpClient::getInstance(TCP_MSG)->send_data_pb(sendData,MSG_SEND_REQ,schat->seqNo);
//            MsgModel::getInstance()->curSendList_tmp[seqNo] = schat;
//            schat->seqNo = seqNo;
//            this->updateChat(schat);
            
            Data d = FileUtils::getInstance()->getDataFromFile(filePath);
            
            std::string savePath = Utils::getImageSavePath(schat->content);
            CCHttpAgent::getInstance()->_makeMultiDir(savePath.c_str());
            FILE *fp = fopen(savePath.c_str(), "wb+");
            
            const unsigned char *buffer = d.getBytes();
            if (fp)
            {
                fwrite(buffer, d.getSize(), 1, fp);
                fclose(fp);
            }
        }
        else
        {
            schat->status = MSG_STATE::MSG_STATE_FAIL;
            this->updateChat(schat);
        }
        
        delete loginPacket;
    });
}

void MsgModel::sendSound(std::string filePath,int time,int groupType)
{
    FILE *  fp = fopen(FileUtils::getInstance()->fullPathForFilename(filePath).c_str(),"rb");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        long long len = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        char *buf = (char*)malloc(4 + len);
        
        int l = time;
        l = htonl(l);
        
        memcpy(&buf[0], &l, 4);
        fread(&buf[4], len, 1, fp);
        fclose(fp);
        
        string sos;
        sos.clear();
        sos.append(buf,4 + len);
        
        log("======发送消息类型==%d==",groupType);
        com::huizhi::im::protobuf::MessageInfo sendMsg;
        sendMsg.set_fromuserid(ZJHModel::getInstance()->uid);
        sendMsg.set_touserid(curChatUID);
        sendMsg.set_messagetype(MSG_TYPE_AUDIO);
        sendMsg.set_contenttype(groupType);
        
        sendMsg.set_content(sos);
        
        std::string sendData = sendMsg.SerializeAsString();
        
        long long seqNo = CCTcpClient::getInstance(TCP_MSG)->send_data_pb(sendData,MSG_SEND_REQ);
        
        std::string abs_path = Utils::getSoundPathUri(ZJHModel::getInstance()->uid, ZJHModel::getInstance()->uid, curChatUID);
        std::string real_path = Utils::getSoundSavePath(abs_path);
        CCHttpAgent::getInstance()->_makeMultiDir(real_path.c_str());
        
        FILE *fp = fopen(real_path.c_str(), "wb+");
        if (fp)
        {
            fwrite(&buf[4], len, 1, fp);
            fclose(fp);
        }
        
        if (buf)
        {
            free( buf);
        }
        
        remove(FileUtils::getInstance()->fullPathForFilename(filePath).c_str());
        
        MsgModel::Chat* chat = new MsgModel::Chat();
        chat->mid = sendMsg.touserid();
        chat->seqNo = seqNo;
        chat->fromId = sendMsg.fromuserid();
        chat->toId = sendMsg.touserid();
        chat->msgType = sendMsg.messagetype();
        chat->content = abs_path;
        chat->roomID = time;
        chat->createTime = Utils::getCurrentTime();
        chat->status = MSG_STATE::MSG_STATE_SENDING;
        chat->contentType = groupType;
        addChat(chat);
        
        MsgModel::getInstance()->curSendList_tmp[seqNo] = chat;
        
        __NotificationCenter::getInstance()->postNotification("ChatFriends_reflash");
        
        Session* s = new Session;
        s->mid = chat->mid;
        s->fromId = chat->fromId;
        if(groupType == CONTENT_CLUB){
            s->type = MESSAGE_TYPE::SESSION_TRIBLE_GROUP_MESSAGE;
        }else{
            s->type = MESSAGE_TYPE::SESSION_POINT_MESSAGE;
        }
        s->msgType = chat->msgType;
        s->content = "[语音]";
        s->createTime = chat->createTime;
        addSession(s);
    }
}

MsgModel::Session* MsgModel::addSession(Session* s)
{
    if(!s)return NULL;
    if (sessionList.find(s->mid) != sessionList.end())
    {
        if (sessionList[s->mid] != s)
        {
            if(s->type != -1)sessionList[s->mid]->type = s->type;
            if(s->msgType != -1)sessionList[s->mid]->msgType = s->msgType;
            if(s->fromId != -1)sessionList[s->mid]->fromId = s->fromId;
            if(s->content != "")sessionList[s->mid]->content = s->content;
            if(s->createTime != -1)sessionList[s->mid]->createTime = s->createTime;
        }
        
        DataBase::getInstance(ZJHModel::getInstance()->uid)->updateSession(sessionList[s->mid]);
        
        if (sessionList[s->mid] != s)delete s;
    }
    else
    {
        sessionList[s->mid] = s;
        DataBase::getInstance(ZJHModel::getInstance()->uid)->addSession(s);
    }
    
    return sessionList[s->mid];
}

MsgModel::FriendInfo* MsgModel::addFriend(FriendInfo* s,bool needAutoUpdate)
{
    if(!s)return NULL;
    if(s->friendType == FRIEND_TYPE_GROUP){
        if(friendGroupList.find(s->mid) == friendGroupList.end())
        {
            friendGroupList = DataBase::getInstance(ZJHModel::getInstance()->uid)->getAllFriendGroupInfo();
        }
        
        if (friendGroupList.find(s->mid) != friendGroupList.end() && friendGroupList.find(s->mid)->second != NULL)
        {
            if(needAutoUpdate){
            if (friendGroupList[s->mid] != s)
            {
                if(s->unReadNums != -1)friendGroupList[s->mid]->unReadNums += s->unReadNums;
                if(s->friendType != -1)friendGroupList[s->mid]->friendType = s->friendType;
                if(s->nickName != "")friendGroupList[s->mid]->nickName = s->nickName;
                if(s->photo != "")friendGroupList[s->mid]->photo = s->photo;
                if(s->rmkName != "")friendGroupList[s->mid]->rmkName = s->rmkName;
            }
            DataBase::getInstance(ZJHModel::getInstance()->uid)->updateFriend(friendGroupList[s->mid]);
            }
            if (friendGroupList[s->mid] != s)delete s;
        }
        else
        {
            friendGroupList[s->mid] = s;
            DataBase::getInstance(ZJHModel::getInstance()->uid)->addFriend(s);
        }
        return friendGroupList[s->mid];
       
    }else{
        if (friendList.find(s->mid) != friendList.end() && friendList.find(s->mid)->second != NULL)
        {
            if (friendList[s->mid] != s)
            {
                if(s->unReadNums != -1)friendList[s->mid]->unReadNums += s->unReadNums;
                if(s->friendType != -1)friendList[s->mid]->friendType = s->friendType;
                if(s->nickName != "")friendList[s->mid]->nickName = s->nickName;
                if(s->photo != "")friendList[s->mid]->photo = s->photo;
                if(s->rmkName != "")friendList[s->mid]->rmkName = s->rmkName;
            }
            
            DataBase::getInstance(ZJHModel::getInstance()->uid)->updateFriend(friendList[s->mid]);
            
            if (friendList[s->mid] != s)delete s;
        }
        else
        {
            s->friendType = FRIEND_TYPE::FRIEND_TYPE_FRIEND_TO_NULL;
            friendList[s->mid] = s;
            
            DataBase::getInstance(ZJHModel::getInstance()->uid)->addFriend(s);
        }
        return friendList[s->mid];
    }
}

MsgModel::FriendInfo* MsgModel::updateFriendInfo(FriendInfo* fi)
{
    if(fi)DataBase::getInstance(ZJHModel::getInstance()->uid)->updateFriend(fi);
    return fi;
}

MsgModel::Chat* MsgModel::addChat(Chat* s)
{
    if(!s)return NULL;
    if(s->contentType == CONTENT_CLUB){
        if(friendGroupList.find(s->mid) == friendGroupList.end())
        {
            friendGroupList = DataBase::getInstance(ZJHModel::getInstance()->uid)->getAllFriendGroupInfo();
        }
        if (friendGroupList.find(s->mid) != friendGroupList.end())
        {
            if(curChatUID == s->mid)friendGroupList[s->mid]->chatList.push_back(s);
            DataBase::getInstance(ZJHModel::getInstance()->uid)->addChat(s);
            return s;
        }
        else
        {
            delete s;
            return NULL;
        }
    }else{
        if (friendList.find(s->mid) != friendList.end())
        {
            if(curChatUID == s->mid)friendList[s->mid]->chatList.push_back(s);
            DataBase::getInstance(ZJHModel::getInstance()->uid)->addChat(s);
            return s;
        }
        else
        {
            delete s;
            return NULL;
        }
    }
}

MsgModel::Chat* MsgModel::updateChat(Chat* s)
{
    if(!s)return NULL;
    if (friendList.find(s->mid) != friendList.end())
    {
        DataBase::getInstance(ZJHModel::getInstance()->uid)->updateChat(s);
        return s;
    }
    else
    {
        return NULL;
    }
}

void MsgModel::sendText_group(std::string text)
{
    com::huizhi::im::protobuf::MessageInfo sendMsg;
    sendMsg.set_fromuserid(ZJHModel::getInstance()->uid);
    sendMsg.set_touserid(curGroupID);
    sendMsg.set_messagetype(MSG_TYPE_TEXT);
    sendMsg.set_contenttype(CONTENT_GROUP);
    sendMsg.set_content(text);
    
    std::string sendData = sendMsg.SerializeAsString();
    
    long long seqNo = CCTcpClient::getInstance(TCP_MSG)->send_data_pb(sendData,MSG_SEND_REQ);
    
    MsgModel::Chat* chat = new MsgModel::Chat();
    chat->mid = sendMsg.touserid();
    chat->seqNo = seqNo;
    chat->fromId = sendMsg.fromuserid();
    chat->toId = sendMsg.touserid();
    chat->msgType = sendMsg.messagetype();
    chat->content = sendMsg.content();
    chat->createTime = Utils::getCurrentTime();
    chat->status = MSG_STATE::MSG_STATE_SENDING;
    
    roomChatList.push_back(chat);
    
    MsgModel::getInstance()->curSendList_tmp[seqNo] = chat;
    
    __NotificationCenter::getInstance()->postNotification("RoomChat_reflash");
    
    Json::Value json;
    json["uid"] = ZJHModel::getInstance()->uid;
    json["name"] = ZJHModel::getInstance()->nick;
    json["content"] = text;
    __NotificationCenter::getInstance()->postNotification("notify_room_chat",__String::create(json.toStyledString()));
}

void MsgModel::sendImg_group(std::string filePath)
{
    com::huizhi::im::protobuf::MessageInfo* sendMsg = new com::huizhi::im::protobuf::MessageInfo;
    sendMsg->set_fromuserid(ZJHModel::getInstance()->uid);
    sendMsg->set_touserid(curGroupID);
    sendMsg->set_messagetype(MSG_TYPE_IMAGE);
    sendMsg->set_contenttype(CONTENT_GROUP);
    sendMsg->set_content(filePath);
    
    MsgModel::Chat* chat = new MsgModel::Chat();
    chat->seqNo = Utils::getCurrentTime();
    chat->mid = sendMsg->touserid();
    chat->fromId = sendMsg->fromuserid();
    chat->toId = sendMsg->touserid();
    chat->msgType = sendMsg->messagetype();
    chat->content = sendMsg->content();
    chat->createTime = Utils::getCurrentTime();
    chat->status = MSG_STATE::MSG_STATE_SENDING;
    roomChatList.push_back(chat);
    
    __NotificationCenter::getInstance()->postNotification("RoomChat_reflash");
    
    char buf[128];
    snprintf(buf, sizeof(buf), "%ld",chat->createTime);
    
    //post image to server
    CCHttpAgent::getInstance()->sendHttpImagePost("image/uploadImImage", filePath, buf,[=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        CCHttpAgent::getInstance()->packets.erase(tag);
        CCHttpAgent::getInstance()->callbacks.erase(tag);
        if (loginPacket->status != 3)
        {
            chat->status = MSG_STATE::MSG_STATE_FAIL;
            delete loginPacket;
            return;
        }
        
        loginPacket->status = 0;
        
        if (loginPacket->resultIsOK())
        {
            sendMsg->set_content(loginPacket->recvVal["resultMap"]["imageUrl"].asString());
            chat->content = loginPacket->recvVal["resultMap"]["imageUrl"].asString();
            std::string sendData = sendMsg->SerializeAsString();
            
            long long seqNo = CCTcpClient::getInstance(TCP_MSG)->send_data_pb(sendData,MSG_SEND_REQ,chat->seqNo);
            MsgModel::getInstance()->curSendList_tmp[seqNo] = chat;
            chat->seqNo = seqNo;
            
            Data d = FileUtils::getInstance()->getDataFromFile(filePath);
            
            std::string savePath = Utils::getImageSavePath(chat->content);
            CCHttpAgent::getInstance()->_makeMultiDir(savePath.c_str());
            FILE *fp = fopen(savePath.c_str(), "wb+");
            
            const unsigned char *buffer = d.getBytes();
            if (fp)
            {
                fwrite(buffer, d.getSize(), 1, fp);
                fclose(fp);
            }
            
            Json::Value json;
            json["uid"] = ZJHModel::getInstance()->uid;
            json["name"] = ZJHModel::getInstance()->nick;
            json["content"] = "[图片]";
            json["imagepath"] = chat->content;
            __NotificationCenter::getInstance()->postNotification("notify_room_chat",__String::create(json.toStyledString()));
        }
        else
        {
            chat->status = MSG_STATE::MSG_STATE_FAIL;
        }
        
        delete loginPacket;
    });
}

void MsgModel::sendSound_group(std::string filePath,int times)
{
    FILE *  fp = fopen(FileUtils::getInstance()->fullPathForFilename(filePath).c_str(),"rb");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        long long len = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        char *buf = (char*)malloc(4 + len);
        
        int l = times;
        l = htonl(l);
        
        memcpy(&buf[0], &l, 4);
        fread(&buf[4], len, 1, fp);
        fclose(fp);
        
        string sos;
        sos.clear();
        sos.append(buf,4 + len);
        
        com::huizhi::im::protobuf::MessageInfo sendMsg;
        sendMsg.set_fromuserid(ZJHModel::getInstance()->uid);
        sendMsg.set_touserid(curGroupID);
        sendMsg.set_messagetype(MSG_TYPE_AUDIO);
        sendMsg.set_contenttype(CONTENT_GROUP);
        
        sendMsg.set_content(sos);
        
        std::string sendData = sendMsg.SerializeAsString();
        
        long long seqNo = CCTcpClient::getInstance(TCP_MSG)->send_data_pb(sendData,MSG_SEND_REQ);
        
        std::string abs_path = Utils::getSoundPathUri(ZJHModel::getInstance()->uid, ZJHModel::getInstance()->uid, curGroupID);
        std::string real_path = Utils::getSoundSavePath(abs_path);
        CCHttpAgent::getInstance()->_makeMultiDir(real_path.c_str());
        
        FILE *fp = fopen(real_path.c_str(), "wb+");
        if (fp)
        {
            fwrite(&buf[4], len, 1, fp);
            fclose(fp);
        }
        
        if (buf)
        {
            free( buf);
        }
        
        remove(FileUtils::getInstance()->fullPathForFilename(filePath).c_str());
        
        MsgModel::Chat* chat = new MsgModel::Chat();
        chat->mid = ZJHModel::getInstance()->uid;
        chat->seqNo = Utils::getCurrentTime();
        chat->fromId = ZJHModel::getInstance()->uid;
        chat->toId = curGroupID;
        chat->msgType = MSG_TYPE_AUDIO;
        chat->content = abs_path;
        chat->roomID = times;
        chat->createTime = Utils::getCurrentTime();
        chat->status = MSG_STATE::MSG_STATE_SENDING;
        
        chat->seqNo = seqNo;
        MsgModel::getInstance()->curSendList_tmp[seqNo] = chat;
        
        roomChatList.push_back(chat);
        
        log("___testtime chat[%ld]",time(NULL));
        
        __NotificationCenter::getInstance()->postNotification("RoomChat_reflash");
        
        Json::Value json;
        json["uid"] = ZJHModel::getInstance()->uid;
        json["name"] = ZJHModel::getInstance()->nick;
        json["content"] = "[语音]";
        json["filepath"] = chat->content;
        json["time"] = times;
        __NotificationCenter::getInstance()->postNotification("notify_room_chat",__String::create(json.toStyledString()));
    }
}

void MsgModel::join_group(int id)
{
    com::huizhi::im::protobuf::GroupJoinRequest sendMsg;
    sendMsg.set_groupid(id);
    sendMsg.set_userid(ZJHModel::getInstance()->uid);
    sendMsg.set_type(1);
    
    std::string sendData = sendMsg.SerializeAsString();
    
    long long seqNo = CCTcpClient::getInstance(TCP_MSG)->send_data_pb(sendData,MSG_JOIN_GROUP_REQ);
    
    curGroupID = id;
}

void MsgModel::goout_group(int id)
{
    com::huizhi::im::protobuf::GroupJoinRequest sendMsg;
    sendMsg.set_groupid(id);
    sendMsg.set_userid(ZJHModel::getInstance()->uid);
    sendMsg.set_type(2);
    
    std::string sendData = sendMsg.SerializeAsString();
    
    long long seqNo = CCTcpClient::getInstance(TCP_MSG)->send_data_pb(sendData,MSG_JOIN_GROUP_REQ);
    
    curGroupID = -1;
}
