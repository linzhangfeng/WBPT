#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include <string>
#include <semaphore.h>
#include "cocos2d.h"

#include "ZJH.h"

enum request_cmd
{
    CONNECT_REQ = 0,
    DISCONNECT_REQ,
    DATA_REQ,
};

enum response_cmd
{
    CONNECT_OK_RES = 0,
    CONNECT_ERROR_RES,
    SEND_DATA_OK_RES,
    SEND_DATA_ERROR_RES,
    RECV_DATA_OK_RES,
    RECV_DATA_ERROR_RES,
    DISCONNECT_RES,
};

enum parse_state
{
    PARSE_HEADER    	= 0,
    PARSE_BODY      	= 1,
    PARSE_ERR       	= 2,
    PARSE_CROSS_DOMAIN	= 3,
};

struct Header {
    //unsigned char   sig[2];
    unsigned int    length;
    //unsigned int    info;
};

struct MsgHeader
{
    int length;
    int serviceId;
    int commandId;
    int version;
    int userId;
    long long seqNo;
    int macL;
    char mac[128];
    int keyL;
    char userKey[128];
};

struct MsgHeader2
{
    int cmd;
    int length;
};

#pragma pack(push)
#pragma pack(1)
struct MsgHeader3
{
    unsigned int cmd;
    short length;
    unsigned int uid;
    short svrid;
};
#pragma pack(pop)

enum class ProtoType
{
    JSON,
    PROTOBUF_WANBEI,
    PROTOBUF_CJJ,
    PROTOBUF_CJJ2,//same of PROTOBUF_CJJ, use for js
    PROTOBUF_CJJ3,
};

class CCTcpRequest
{
public:
    int cmd;
    std::string data;
    int id;
    long long seqNo;
};

class CCTcpResponse
{
public:
    int cmd;
    std::string data;
    int id;
    long long seqNo;
    int svrid;
    long long time;
    void release(){delete this;}
    std::string parseDatta(){return JPacket::decodeStr(data);}
};

class CCTcpClient
{
public:
    /** Return the shared instance **/
    static CCTcpClient *getInstance(int vid = -1);
    
    /** Relase the shared instance **/
    static void destroyInstance();
	
    CCTcpClient();
    virtual ~CCTcpClient();
    
	void put(CCTcpRequest* request);
	CCTcpResponse *get();
    long long create_request(int cmd, std::string data, int id = -1,long long seqNo = 0);
    void create_response(int cmd, std::string data, int id = -1,long long seqNo = 0, int svrid = 0);
    void reset_connect();
	void connect_req(CCTcpRequest *request);
    void disconnect_req(CCTcpRequest *request);
	void data_req(CCTcpRequest *request);
    
	ssize_t write_from_string(std::string &buffer);
	void set_host_port(std::string h, int p);
	void connect();
    void reset_response_queue();
    void disconnect(int type = 0);
    void send_data(std::string &data);
    void sendCodeData(std::string &data);
    void read_res();
    bool isConnect();
    
    static void sendToAll(std::string &data);
    
    void data_req_pb(CCTcpRequest *request);
    ssize_t write_from_string_pb(CCTcpRequest *request);
    long long send_data_pb(const std::string &data,int id = -1,long long seqNo = 0);
    void read_res_pb();
    
    int PackHeader(MsgHeader* pHeader, char* pBuf);
    int UnPackHeader(MsgHeader* pHeader, char* pBuf);
    
    
    void data_req_pb2(CCTcpRequest *request);
    ssize_t write_from_string_pb2(CCTcpRequest *request);
    long long send_data_pb2(const std::string &data,int id = -1,long long seqNo = 0);
    void read_res_pb2();
    
    int PackHeader2(MsgHeader2* pHeader, char* pBuf);
    int UnPackHeader2(MsgHeader2* pHeader, char* pBuf);
    
    void data_req_pb3(CCTcpRequest *request);
    ssize_t write_from_string_pb3(CCTcpRequest *request);
    long long send_data_pb3(const std::string &data,int id = -1,long long seqNo = 0);
    void read_res_pb3();
    
    int PackHeader3(MsgHeader3* pHeader, char* pBuf);
    int UnPackHeader3(MsgHeader3* pHeader, char* pBuf);
    
private:
    bool init(void);
    
public:
    int 				fd;
	std::string 		host;
	int 				port;
	int 				state;
	int					cur_head_len;
    
    int					cur_body_len;
    
    char                header[sizeof(struct Header)];
    struct Header       *header_p;
    
    char                header_pb[sizeof(struct MsgHeader)];
    struct MsgHeader    *header_p_pb;
    
    char                header_pb2[sizeof(struct MsgHeader2)];
    struct MsgHeader2    *header_p_pb2;
    
    char                header_pb3[sizeof(struct MsgHeader3)];
    struct MsgHeader3    *header_p_pb3;
    
	std::string         body;
    
    pthread_t        s_requestThread;
    pthread_t        s_responseThread;
    
    bool				isConnected;
    sem_t *          s_pSem;
    int 				isClose;
    
    Queue<CCTcpRequest*> *s_requestQueue;
    Queue<CCTcpResponse*> *s_responseQueue;
    
    sem_t s_sem;
    
    int mid;
    
public:
    ProtoType protoType;
    
    void setProtoType(ProtoType v){protoType = v;};
};

#endif //_TCP_CLIENT_H_
