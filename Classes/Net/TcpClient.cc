#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include "ZJH.h"
#include "TcpClient.h"
#include "proto.h"
#include <arpa/inet.h>
#include "../JsUtils/JsUtils.h"

using namespace cocos2d;

static void xorfunc(std::string &nString)
{
	const int KEY = 13;
	int strLen = (nString.length());
	char *cString = (char*)(nString.c_str());
	
	for (int i = 0; i < strLen; i++)
	{
		*(cString + i) = (*(cString + i) ^ KEY);
	}
}

static void *requestThread(void *data)
{
    CCTcpRequest *request = NULL;
	CCTcpClient* self = (CCTcpClient*)data;
	
	while (1)
	{
		self->s_requestQueue->get_msg(request);
//		printf("requestThread cmd %d data %s\n", request->cmd, request->data.c_str());
		if (CONNECT_REQ == request->cmd)
		{
            self->reset_response_queue();
			self->connect_req(request);
		}
		else if (self->isConnected && DISCONNECT_REQ == request->cmd)
		{
            self->disconnect_req(request);
		}
		else if (self->isConnected && DATA_REQ == request->cmd)
		{
            if (self->protoType == ProtoType::JSON)
            {
                self->data_req(request);
            }
            else if (self->protoType == ProtoType::PROTOBUF_WANBEI)
            {
                self->data_req_pb(request);
            }
            else if (self->protoType == ProtoType::PROTOBUF_CJJ || self->protoType == ProtoType::PROTOBUF_CJJ2)
            {
                self->data_req_pb2(request);
            }
            else if (self->protoType == ProtoType::PROTOBUF_CJJ3)
            {
                self->data_req_pb3(request);
            }
		}
		delete request;
	}
	pthread_exit(NULL);
}

static void *responseThread(void *data)
{
    CCTcpClient* self = (CCTcpClient*)data;
	while (1)
	{        
        if (self->isConnected == false)
        {
            sleep(1);
            continue;
        }
        CCLOG("lct:begin to read phase in responseThread");
                
        if (self->isConnected)
        {
            if (self->protoType == ProtoType::JSON)
            {
                self->read_res();
            }
            else if (self->protoType == ProtoType::PROTOBUF_WANBEI)
            {
                self->read_res_pb();
            }
            else if (self->protoType == ProtoType::PROTOBUF_CJJ || self->protoType == ProtoType::PROTOBUF_CJJ2)
            {
                self->read_res_pb2();
            }
            else if (self->protoType == ProtoType::PROTOBUF_CJJ3)
            {
                self->read_res_pb3();
            }
        }
        else
        {
            CCLOG("lct:no connect");
            continue;
        }
	}
	
	pthread_exit(NULL);
}

std::map<int, CCTcpClient *> tcpClients;

CCTcpClient* CCTcpClient::getInstance(int id)
{
    CCTcpClient* tcpClient = tcpClients[id];
    if (tcpClient == NULL)
    {
        tcpClient = new CCTcpClient();
        tcpClient->init();
        tcpClient->mid = id;
        tcpClients[id] = tcpClient;
    }
    
    return tcpClient;
}

void CCTcpClient::destroyInstance()
{
    
}

CCTcpClient::CCTcpClient()
: fd(-1)
,mid(-1)
,s_requestQueue(NULL)
,s_responseQueue(NULL)
{

}

CCTcpClient::~CCTcpClient()
{
}

bool CCTcpClient::init()
{
    protoType = ProtoType::JSON;
    
	header_p = (struct Header*)header;
    header_p_pb = (struct MsgHeader*)header_pb;
    header_p_pb2 = (struct MsgHeader2*)header_pb2;
    header_p_pb3 = (struct MsgHeader3*)header_pb3;
    
	isConnected = false;
	s_requestQueue = new Queue<CCTcpRequest*>();
	s_responseQueue = new Queue<CCTcpResponse*>();
	
	pthread_create(&s_requestThread, NULL, requestThread, this);
	pthread_detach(s_requestThread);
	
	pthread_create(&s_responseThread, NULL, responseThread, this);
	pthread_detach(s_responseThread);
    
    return true;
}

void CCTcpClient::put(CCTcpRequest* request)
{
	s_requestQueue->put_msg(request);
}

CCTcpResponse *CCTcpClient::get()
{
	CCTcpResponse *response = NULL;
	int ret = s_responseQueue->pop_msg(response);
	if (ret == 0)
	{
		return response;
	}
	else
	{
		return NULL;
	}	
}

long long CCTcpClient::create_request(int cmd, string data, int id, long long seqNo)
{
    CCTcpRequest* request = new CCTcpRequest();
    request->cmd = cmd;
    request->data = data;
    request->id = id;
    request->seqNo = seqNo == 0 ? Utils::getCurrentTime() : seqNo;
	s_requestQueue->put_msg(request);
    return request->seqNo;
}

void CCTcpClient::create_response(int cmd, string data, int id, long long seqNo, int svrid)
{
    CCTcpResponse* response = new CCTcpResponse();
    response->cmd = cmd;
    response->data = data;
    response->id = id;
    response->seqNo = seqNo;
    response->svrid = svrid;
    response->time = Utils::getCurrentTime();
	s_responseQueue->put_msg(response);
}

void CCTcpClient::reset_connect()
{
	if (isConnected)
	{
        close(fd);
		fd = -1;
		isConnected = false;
        isClose = 0;
	}
    else
    {
		fd = -1;
		isConnected = false;
        isClose = 0;
    }
}

void CCTcpClient::connect_req(CCTcpRequest *request)
{
    if (host.empty()) {
        CCLOG("[TcpClient] ip string is empty!!!");
        return;
    }
    fd = Network::tcp_connect(host.c_str(), port, 3, 3);
	if (fd < 0)
	{
        create_response(CONNECT_ERROR_RES, "CONNECT_ERROR_RES");
	}
	else
	{
        create_response(CONNECT_OK_RES, "CONNECT_OK_RES");
		isConnected = true;
        CCLOG("connect is ok fd is [%d].", fd);
	}
}

void CCTcpClient::disconnect_req(CCTcpRequest *request)
{    
    reset_connect();
}

void CCTcpClient::data_req(CCTcpRequest *request)
{
	int ret = write_from_string(request->data);
	if (ret < 0)
	{
        reset_connect();
        create_response(DISCONNECT_RES, "DISCONNECT_RES");
	}
	else
	{
        create_response(SEND_DATA_OK_RES, "SEND_DATA_OK_RES");
	}

}

bool CCTcpClient::isConnect()
{
    return isConnected;
}

ssize_t CCTcpClient::write_from_string(string &buffer)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;
    string result;
//	struct Header my_header;
	
    ptr = buffer.c_str();
    nleft = buffer.length();
    
    while (nleft > 0) {
        nwritten = write(fd, ptr, nleft);
        if (nwritten <= 0) {
            if (errno == EINTR)
                nwritten = 0;
            else
            {
                CCLOG("write_from_string fd[%d] errno[%d]", fd, errno);
                return -1;
            }
        }
        nleft -= nwritten;
        ptr += nwritten;
    }

    return result.length();
}

void CCTcpClient::set_host_port(std::string h, int p)
{
	host = h;
	port = p;
}

void CCTcpClient::connect()
{
    reset_response_queue();
    reset_connect();
    create_request(CONNECT_REQ, "CONNECT_REQ");
}


void CCTcpClient::reset_response_queue()
{
    CCTcpResponse *response = NULL;
    while (true) {
        response = get();
        if (!response)
            break;
        delete response;
    }
}

void CCTcpClient::disconnect(int type)
{
    if (type == 0)
    {
        create_request(DISCONNECT_REQ, "DISCONNECT_REQ");
    }
    else
    {
        reset_connect();
    }
}

void CCTcpClient::send_data(std::string &data)
{
    create_request(DATA_REQ, data);
}

void CCTcpClient::sendCodeData(std::string &data)
{
    create_request(DATA_REQ, JPacket::codeStr(data));
}
void CCTcpClient::read_res()
{
    int ret;
    int res = 0;
    fd_set fds;
    struct timeval tv;
    char recv_buf[65536];
    
    cur_head_len = 0;
    state = PARSE_HEADER;
    isClose = 1;
   
    while (1)
    {
       if (isClose == 0)
	    {
	    	break;
	    }
        
		FD_ZERO(&fds);
    	if(fd != -1)FD_SET(fd, &fds);
    	tv.tv_sec = 1;
    	tv.tv_usec = 0;
        if(fd != -1)res = select(fd + 1, &fds, NULL, NULL, &tv);
    	if (isClose == 1 && res > 0)
    	{
	        if (state == PARSE_HEADER)
	        {
               
                ret = read(fd, &header[cur_head_len],
                               sizeof(struct Header) - cur_head_len);
	            if (ret <= 0)
	            {
	                if (errno == EINTR)
	                {
	                    continue;
	                }
                    
                    CCLOG("DISCONNECT_RES  lct:parse header ret[%d] fd[%d] errno[%d]", ret, fd, errno);
                    reset_connect();
                    create_response(DISCONNECT_RES, "DISCONNECT_RES");
	                break;
	            }
	            
	            cur_head_len += ret;
	            if (cur_head_len == sizeof(header))
	            {
	                if (header_p->length > 65536 || header_p->length == 0)
	                {
                        reset_connect();
                        create_response(DISCONNECT_RES, "DISCONNECT_RES");
	                    break;
	                }
	                
	                state = PARSE_BODY;
	                cur_head_len = 0;
                    cur_body_len = 0;
	                body.clear();
	            }
	        }
	        else if (state == PARSE_BODY)
	        {
	            ret = read(fd, recv_buf, header_p->length - cur_body_len);
	            if (ret <= 0)
	            {
	                if (errno == EINTR)
	                {
	                    continue;
	                }
                    reset_connect();
                    create_response(DISCONNECT_RES, "DISCONNECT_RES");
	                CCLOG("lct:parse body[%d]", errno);
	                break;
	            }
	            
                cur_body_len += ret;
	            recv_buf[ret] = '\0';
	            body.append(recv_buf,ret);
	            
	            if (cur_body_len == header_p->length)
	            {
	                state = PARSE_HEADER;
	                cur_head_len = 0;
                    cur_body_len = 0;
                    // xorfunc(body);
                    create_response(RECV_DATA_OK_RES, body);
	            }
	        }
	    }
	    else
	    {

	    }
        
	    if (isClose == 0)
	    {
	    	CCLOG("lct:close\n");
	    	break;
	    }
    }
    CCLOG("lct:read end\n");
}

void CCTcpClient::sendToAll(std::string &data)
{
    for(int i = 0; i < tcpClients.size(); i ++)
    {
        tcpClients[i]->send_data(data);
    }
}

void CCTcpClient::data_req_pb(CCTcpRequest *request)
{
    int ret = write_from_string_pb(request);
    if (ret < 0)
    {
        reset_connect();
        create_response(SEND_DATA_ERROR_RES, "SEND_DATA_ERROR_RES");
    }
    else
    {
        create_response(SEND_DATA_OK_RES, "SEND_DATA_OK_RES");
    }
    
}

ssize_t CCTcpClient::write_from_string_pb(CCTcpRequest *request)
{
    int bodyL = request->data.length();
    
    MsgHeader tmHeader;
    tmHeader.commandId    = request->id;
    tmHeader.serviceId = 1000;
    tmHeader.version = 1;
    tmHeader.userId = ZJHModel::getInstance()->uid;
    tmHeader.seqNo = request->seqNo;
    
    std::string macs = ZJHModel::getInstance()->phoneimei;
    std::string keys = ZJHModel::getInstance()->UserKey;

    snprintf(tmHeader.mac, sizeof(tmHeader.mac), "%s",macs.c_str());
    snprintf(tmHeader.userKey, sizeof(tmHeader.userKey), "%s",keys.c_str());
    
    tmHeader.macL = strlen(macs.c_str());
    tmHeader.keyL = strlen(keys.c_str());
    
    tmHeader.length = sizeof(tmHeader.length) + sizeof(tmHeader.serviceId) + sizeof(tmHeader.commandId) + sizeof(tmHeader.version) + sizeof(tmHeader.userId) + sizeof(tmHeader.seqNo) + sizeof(tmHeader.macL) + tmHeader.macL + sizeof(tmHeader.keyL) + tmHeader.keyL + bodyL;
    
    char allbuf[65536] = {0};
    char* pall = allbuf;
    int headL = PackHeader(&tmHeader, pall);
    
    int allLen = headL + bodyL;
    
    pall = pall + headL;
    
    memcpy(pall, request->data.c_str(), bodyL);
    
//    int ret = send(fd, allbuf, allLen, 0);
    
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;
    
    ptr = allbuf;
    nleft = allLen;
    
    //    ptr = buffer.c_str();
    //    nleft = buffer.length();
    
    while (nleft > 0) {
        nwritten = write(fd, ptr, nleft);
        
        if (nwritten <= 0) {
            if (errno == EINTR)
                nwritten = 0;
            else
            {
                CCLOG("write_from_string fd[%d] errno[%d]", fd, errno);
                return -1;
            }
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return 0;
}

long long CCTcpClient::send_data_pb(const std::string &data,int cmd,long long seqNo)
{
    if (protoType != ProtoType::PROTOBUF_WANBEI)
    {
        log("warning!!!!! ProtobufTcpClient !isProtoBuf must use send_data");
        return -1;
    }
    
    if (cmd <= 0)
    {
        log("warning!!!!! ProtobufTcpClient send_data cmd[%d] must > 0",cmd);
        return -1;
    }
    return create_request(DATA_REQ, data,cmd,seqNo);
}

void CCTcpClient::read_res_pb()
{
    int ret;
    int res;
    fd_set fds;
    struct timeval tv;
    char recv_buf[65536];
    
    cur_head_len = 0;
    cur_body_len = 0;
    state = PARSE_HEADER;
    isClose = 1;
    
    while (1)
    {
        if (isClose == 0)
        {
            break;
        }
        
        FD_ZERO(&fds);
        if(fd != -1)FD_SET(fd, &fds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        if(fd != -1)res = select(fd + 1, &fds, NULL, NULL, &tv);
        if (isClose == 1 && res > 0)
        {
            if (state == PARSE_HEADER)
            {
                ret = read(fd, recv_buf + cur_body_len, sizeof(header_p_pb->length) - cur_body_len);
                if (ret <= 0)
                {
                    if (errno == EINTR)
                    {
                        continue;
                    }
                    reset_connect();
                    create_response(DISCONNECT_RES, "DISCONNECT_RES");
                    CCLOG("lct:parse head[%d]", errno);
                    break;
                }
                
                cur_body_len += ret;
                if (cur_body_len == sizeof(header_p_pb->length))
                {
                    memcpy(&(header_p_pb->length), recv_buf, sizeof(header_p_pb->length));
                    header_p_pb->length      = ntohl(header_p_pb->length);
                    
                    if (header_p_pb->length > 65536)
                    {
                        reset_connect();
                        create_response(DISCONNECT_RES, "DISCONNECT_RES");
                        break;
                    }
                    
                    state = PARSE_BODY;
                }
            }
            else if (state == PARSE_BODY)
            {
                ret = read(fd, recv_buf + cur_body_len, header_p_pb->length - cur_body_len);
                if (ret <= 0)
                {
                    if (errno == EINTR)
                    {
                        continue;
                    }
                    reset_connect();
                    create_response(DISCONNECT_RES, "DISCONNECT_RES");
                    CCLOG("lct:parse body[%d]", errno);
                    break;
                }
                
                cur_body_len += ret;
                if (cur_body_len == header_p_pb->length)
                {
                    cur_head_len = UnPackHeader(header_p_pb,recv_buf);
                    
                    int dl = 4;
                    if (header_p_pb->commandId == 2) dl = 0;
                    body.clear();
                    body.append(recv_buf + cur_head_len + dl, cur_body_len - cur_head_len - dl);
                    
//                    if (header_p_pb->commandId == 101)
//                    {
//                        com::huizhi::im::protobuf::MessageInfo msg;
//                        
////                        char buf[cur_body_len - cur_head_len + 1 - dl];
////                        memcpy(buf, recv_buf + cur_head_len + dl,cur_body_len - cur_head_len - dl);
////                        buf[cur_body_len - cur_head_len + 1 - dl] = '\0';
//                        body.clear();
////                        body.append(buf);
//                        body.append(recv_buf + cur_head_len + dl, cur_body_len - cur_head_len - dl);
////                        memcpy(&body, recv_buf + cur_head_len + dl,cur_body_len - cur_head_len - dl);
////                        body.append('\0');
//                        msg.ParseFromString(body);
//                        
//                        log("MSG_RECIEVE_MSG receiv data from uid[%d] msgType[%d] contentType[%d] content[%s]",msg.fromuserid(),msg.messagetype(),msg.contenttype(),msg.content().c_str());
//                    }
                    
                    state = PARSE_HEADER;
                    cur_head_len = 0;
                    cur_body_len = 0;
//                    // xorfunc(body);
                    create_response(RECV_DATA_OK_RES, body, header_p_pb->commandId,header_p_pb->seqNo);
                }
            }
        }
        else
        {
            
        }
        
        if (isClose == 0)
        {
            CCLOG("lct:close\n");
            break;
        }
    }
    CCLOG("lct:read end\n");
}

int CCTcpClient::PackHeader(MsgHeader* pHeader, char* pBuf)
{
    pHeader->length      = htonl(pHeader->length);
    pHeader->serviceId   = htonl(pHeader->serviceId);
    pHeader->commandId   = htonl(pHeader->commandId);
    pHeader->version   = htonl(pHeader->version);
    pHeader->userId   = htonl(pHeader->userId);
    pHeader->seqNo   = htonl(pHeader->seqNo);
    pHeader->macL   = htonl(pHeader->macL);
    pHeader->keyL   = htonl(pHeader->keyL);
    
    char * buf = pBuf;
    int l = 0;
    
    memcpy(buf, &(pHeader->length), sizeof(pHeader->length));
    buf = buf + sizeof(pHeader->length);
    l += sizeof(pHeader->length);
			 
    memcpy(buf, &(pHeader->serviceId), sizeof(pHeader->serviceId));
    buf = buf + sizeof(pHeader->serviceId);
    l += sizeof(pHeader->serviceId);
    
    memcpy(buf, &(pHeader->commandId), sizeof(pHeader->commandId));
    buf = buf + sizeof(pHeader->commandId);
    l += sizeof(pHeader->commandId);

    memcpy(buf, &(pHeader->version), sizeof(pHeader->version));
    buf = buf + sizeof(pHeader->version);
    l += sizeof(pHeader->version);

    memcpy(buf, &(pHeader->userId), sizeof(pHeader->userId));
    buf = buf + sizeof(pHeader->userId);
    l += sizeof(pHeader->userId);
    
    memcpy(buf, &(pHeader->seqNo), sizeof(pHeader->seqNo));
    buf = buf + sizeof(pHeader->seqNo);
    l += sizeof(pHeader->seqNo);
    
    memcpy(buf, &(pHeader->macL), sizeof(pHeader->macL));
    buf = buf + sizeof(pHeader->macL);
    l += sizeof(pHeader->macL);
    
    memcpy(buf, &(pHeader->mac), ntohl(pHeader->macL));
    buf = buf + ntohl(pHeader->macL);
    l += ntohl(pHeader->macL);
    
    memcpy(buf, &(pHeader->keyL), sizeof(pHeader->keyL));
    buf = buf + sizeof(pHeader->keyL);
    l += sizeof(pHeader->keyL);
    
    memcpy(buf, &(pHeader->userKey), ntohl(pHeader->keyL));
    buf = buf + ntohl(pHeader->keyL);
    l += ntohl(pHeader->keyL);
    return l;
}

int CCTcpClient::UnPackHeader(MsgHeader* pHeader, char* pBuf)
{
    char * recv_buf = pBuf;
    int l = 0;

    memcpy(&(pHeader->length), &recv_buf[l], sizeof(pHeader->length));
    pHeader->length      = ntohl(pHeader->length);
    l += sizeof(header_p_pb->length);

    memcpy(&(pHeader->serviceId), &recv_buf[l], sizeof(pHeader->serviceId));
    pHeader->serviceId      = ntohl(pHeader->serviceId);
    l += sizeof(pHeader->serviceId);

    memcpy(&(pHeader->commandId), &recv_buf[l], sizeof(pHeader->commandId));
    pHeader->commandId      = ntohl(pHeader->commandId);
    l += sizeof(pHeader->commandId);

    memcpy(&(pHeader->version), &recv_buf[l], sizeof(pHeader->version));
    pHeader->version      = ntohl(pHeader->version);
    l += sizeof(pHeader->version);

    memcpy(&(pHeader->userId), &recv_buf[l], sizeof(pHeader->userId));
    pHeader->userId      = ntohl(pHeader->userId);
    l += sizeof(pHeader->userId);

    memcpy(&(pHeader->seqNo), &recv_buf[l], sizeof(pHeader->seqNo));
    pHeader->seqNo      = ntohl(pHeader->seqNo);
    l += sizeof(pHeader->seqNo);

    memcpy(&(pHeader->macL), &recv_buf[l], sizeof(pHeader->macL));
    pHeader->macL      = ntohl(pHeader->macL);
    l += sizeof(pHeader->macL);

    memcpy(&(pHeader->mac), &recv_buf[l], pHeader->macL);
    l += pHeader->macL;

    memcpy(&(pHeader->keyL), &recv_buf[l], sizeof(pHeader->keyL));
    pHeader->keyL      = ntohl(pHeader->keyL);
    l += sizeof(header_p_pb->keyL);

    memcpy(&(pHeader->userKey), &recv_buf[l], pHeader->keyL);
    l += pHeader->keyL;
    
    return l;
}


//mj

void CCTcpClient::data_req_pb2(CCTcpRequest *request)
{
    int ret = write_from_string_pb2(request);
    if (ret < 0)
    {
        reset_connect();
        create_response(SEND_DATA_ERROR_RES, "SEND_DATA_ERROR_RES");
    }
    else
    {
        create_response(SEND_DATA_OK_RES, "SEND_DATA_OK_RES");
    }
}

ssize_t CCTcpClient::write_from_string_pb2(CCTcpRequest *request)
{
    MsgHeader2 tmHeader;
    tmHeader.cmd          = request->id;
    tmHeader.length       = request->data.length();
    
    int allLen = sizeof(tmHeader) + tmHeader.length;
    
    char allbuf[65536] = {0};
    char* pall = allbuf;
    PackHeader2(&tmHeader, pall);
    
    pall = pall + sizeof(tmHeader);
    
    memcpy(pall, request->data.c_str(), tmHeader.length);
    
    //    int ret = send(fd, allbuf, allLen, 0);
    
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;
    
    ptr = allbuf;
    nleft = allLen;
    
    //    ptr = buffer.c_str();
    //    nleft = buffer.length();
    
    while (nleft > 0) {
        nwritten = write(fd, ptr, nleft);
        
        if (nwritten <= 0) {
            if (errno == EINTR)
                nwritten = 0;
            else
            {
                CCLOG("write_from_string pb id[%d] fd[%d] errno[%d]", mid, fd, errno);
                return -1;
            }
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return 0;
}

long long CCTcpClient::send_data_pb2(const std::string &data,int cmd,long long seqNo)
{
    if (protoType != ProtoType::PROTOBUF_CJJ && protoType != ProtoType::PROTOBUF_CJJ2)
    {
        log("warning!!!!! ProtobufTcpClient !isProtoBuf must use send_data");
        return -1;
    }
    
    if (cmd <= 0)
    {
        log("warning!!!!! ProtobufTcpClient send_data cmd[%d] must > 0",cmd);
        return -1;
    }
    return create_request(DATA_REQ, protoType == ProtoType::PROTOBUF_CJJ2 ? JsUtils::decodeBase64(data) : data,cmd);
}

void CCTcpClient::read_res_pb2()
{
    int ret;
    int res;
    fd_set fds;
    struct timeval tv;
    char recv_buf[65536];
    
    cur_head_len = 0;
    state = PARSE_HEADER;
    isClose = 1;
    while (1)
    {
        if (isClose == 0)
        {
            break;
        }
        
        FD_ZERO(&fds);
        if(fd != -1)FD_SET(fd, &fds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        if(fd != -1)res = select(fd + 1, &fds, NULL, NULL, &tv);
        if (isClose == 1 && res > 0)
        {
            if (state == PARSE_HEADER)
            {
                
                ret = read(fd, &header_pb2[cur_head_len],
                           sizeof(struct MsgHeader2) - cur_head_len);
                if (ret <= 0)
                {
                    if (errno == EINTR)
                    {
                        continue;
                    }
                    
                    reset_connect();
                    create_response(DISCONNECT_RES, "DISCONNECT_RES");
                    CCLOG("lct:parse header ret[%d] fd[%d] errno[%d]", ret, fd, errno);
                    break;
                }
                
                cur_head_len += ret;
                if (cur_head_len == sizeof(header_pb2))
                {
                    UnPackHeader2(header_p_pb2,header_pb2);
//                    CCLOG("head cmd[%d] len[%d]",header_p_pb2->cmd,header_p_pb2->length);
                    if (header_p_pb2->length > 65536)
                    {
                        reset_connect();
                        create_response(DISCONNECT_RES, "DISCONNECT_RES");
                        break;
                    }
                    
                    state = PARSE_BODY;
                    cur_head_len = 0;
                    cur_body_len = 0;
                    body.clear();
                }
            }
            else if (state == PARSE_BODY)
            {
                if (header_p_pb2->length == 0)
                {
                    state = PARSE_HEADER;
                    cur_head_len = 0;
                    cur_body_len = 0;
                    // xorfunc(body);
                    create_response(RECV_DATA_OK_RES, "", header_p_pb2->cmd);
                    continue;
                }
                ret = read(fd, recv_buf, header_p_pb2->length - cur_body_len);
                if (ret <= 0)
                {
                    if (errno == EINTR)
                    {
                        continue;
                    }
                    reset_connect();
                    create_response(DISCONNECT_RES, "DISCONNECT_RES");
                    CCLOG("lct:parse body[%d]", errno);
                    break;
                }
                
                cur_body_len += ret;
                
                recv_buf[ret] = '\0';
                body.append(recv_buf,ret);
                
                if (cur_body_len == header_p_pb2->length)
                {
                    state = PARSE_HEADER;
                    cur_head_len = 0;
                    cur_body_len = 0;
                    // xorfunc(body);
                    
                    if (protoType == ProtoType::PROTOBUF_CJJ2)
                    {
                        char *b;
                        int l = cocos2d::base64Encode((unsigned char*)body.c_str(), (unsigned int)body.length(), &b);
                        //                    log("%s",b);
                        std::string data_str = std::string(( char *)b,l);
                        create_response(RECV_DATA_OK_RES, data_str, header_p_pb2->cmd);
                    }
                    else
                    {
                        create_response(RECV_DATA_OK_RES, body, header_p_pb2->cmd);
                    }
                }
            }
        }
        else
        {
            
        }
        
        if (isClose == 0)
        {
            CCLOG("lct:close\n");
            break;
        }
    }
    CCLOG("lct:read end\n");
}

int CCTcpClient::PackHeader2(MsgHeader2* pHeader, char* pBuf)
{
    //    memcpy(pBuf, pHeader, sizeof(MsgHeader2));
    
    char *buf = pBuf;
//    pHeader->cmd   = htonl(pHeader->cmd);
//    pHeader->length   = htonl(pHeader->length);

    memcpy(buf, &(pHeader->cmd), sizeof(pHeader->cmd));
    buf = buf + sizeof(pHeader->cmd);
			 
    memcpy(buf, &(pHeader->length), sizeof(pHeader->length));
    return 0;
}

int CCTcpClient::UnPackHeader2(MsgHeader2* pHeader, char* pBuf)
{
    char * buf = pBuf;
    memcpy(&(pHeader->cmd), buf, sizeof(pHeader->cmd));
    buf = buf + sizeof(pHeader->cmd);
			 
    memcpy(&(pHeader->length), buf, sizeof(pHeader->length));
    
//    pHeader->cmd = ntohl(pHeader->cmd);
//    pHeader->length = ntohl(pHeader->length);
    return 0;
}

//new server framework

void CCTcpClient::data_req_pb3(CCTcpRequest *request)
{
    int ret = write_from_string_pb3(request);
    if (ret < 0)
    {
        reset_connect();
        create_response(SEND_DATA_ERROR_RES, "SEND_DATA_ERROR_RES");
    }
    else
    {
        create_response(SEND_DATA_OK_RES, "SEND_DATA_OK_RES");
    }
}

ssize_t CCTcpClient::write_from_string_pb3(CCTcpRequest *request)
{
    int svrid = (int)request->seqNo;
    MsgHeader3 tmHeader;
    tmHeader.cmd          = request->id;
    tmHeader.length       = request->data.length();
    tmHeader.uid = ZJHModel::getInstance()->uid;
    tmHeader.svrid = svrid;
    
    int allLen = sizeof(tmHeader) + tmHeader.length;
    
    char allbuf[65536] = {0};
    char* pall = allbuf;
    PackHeader3(&tmHeader, pall);
    
    pall = pall + sizeof(tmHeader);
    
    memcpy(pall, request->data.c_str(), tmHeader.length);
    
    //    int ret = send(fd, allbuf, allLen, 0);
    
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;
    
    ptr = allbuf;
    nleft = allLen;
    
    //    ptr = buffer.c_str();
    //    nleft = buffer.length();
    
    while (nleft > 0) {
        nwritten = write(fd, ptr, nleft);
        
        if (nwritten <= 0) {
            if (errno == EINTR)
                nwritten = 0;
            else
            {
                CCLOG("write_from_string pb id[%d] fd[%d] errno[%d]", mid, fd, errno);
                return -1;
            }
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return 0;
}

long long CCTcpClient::send_data_pb3(const std::string &data,int cmd,long long seqNo)
{
    if (protoType != ProtoType::PROTOBUF_CJJ3)
    {
        log("warning!!!!! ProtobufTcpClient !isProtoBuf must use send_data");
        return -1;
    }
    
    if (cmd <= 0)
    {
        log("warning!!!!! ProtobufTcpClient send_data cmd[%d] must > 0",cmd);
        return -1;
    }
    return create_request(DATA_REQ, JsUtils::decodeBase64(data),cmd, seqNo);
}

void CCTcpClient::read_res_pb3()
{
    int ret;
    int res;
    fd_set fds;
    struct timeval tv;
    char recv_buf[65536];
    
    cur_head_len = 0;
    state = PARSE_HEADER;
    isClose = 1;
    while (1)
    {
        if (isClose == 0)
        {
            break;
        }
        
        FD_ZERO(&fds);
        if(fd != -1)FD_SET(fd, &fds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        if(fd != -1)res = select(fd + 1, &fds, NULL, NULL, &tv);
        if (isClose == 1 && res > 0)
        {
            if (state == PARSE_HEADER)
            {
                
                ret = read(fd, &header_pb3[cur_head_len],
                           sizeof(struct MsgHeader3) - cur_head_len);
                if (ret <= 0)
                {
                    if (errno == EINTR)
                    {
                        continue;
                    }
                    
                    reset_connect();
                    create_response(DISCONNECT_RES, "DISCONNECT_RES");
                    CCLOG("lct:parse header ret[%d] fd[%d] errno[%d]", ret, fd, errno);
                    break;
                }
                
                cur_head_len += ret;
                if (cur_head_len == sizeof(header_pb3))
                {
                    UnPackHeader3(header_p_pb3,header_pb3);
                    //                    CCLOG("head cmd[%d] len[%d]",header_p_pb2->cmd,header_p_pb2->length);
                    if (header_p_pb3->length > 65536
                        || header_p_pb3->length < 0)
                    {
                        reset_connect();
                        create_response(DISCONNECT_RES, "DISCONNECT_RES");
                        break;
                    }
                    
                    state = PARSE_BODY;
                    cur_head_len = 0;
                    cur_body_len = 0;
                    body.clear();
                }
            }
            else if (state == PARSE_BODY)
            {
                if (header_p_pb3->length == 0)
                {
                    state = PARSE_HEADER;
                    cur_head_len = 0;
                    cur_body_len = 0;
                    // xorfunc(body);
                    create_response(RECV_DATA_OK_RES, "", header_p_pb3->cmd, header_p_pb3->svrid, header_p_pb3->svrid);
                    continue;
                }
                
                ret = read(fd, recv_buf, header_p_pb3->length - cur_body_len);
                if (ret <= 0)
                {
                    if (errno == EINTR)
                    {
                        continue;
                    }
                    reset_connect();
                    create_response(DISCONNECT_RES, "DISCONNECT_RES");
                    CCLOG("lct:parse body[%d]", errno);
                    break;
                }
                
                cur_body_len += ret;
                
                recv_buf[ret] = '\0';
                body.append(recv_buf,ret);
                
                if (cur_body_len == header_p_pb3->length)
                {
                    state = PARSE_HEADER;
                    cur_head_len = 0;
                    cur_body_len = 0;
                    // xorfunc(body);
//                    create_response(RECV_DATA_OK_RES, body, header_p_pb3->cmd);
                    char *b;
                    int l = cocos2d::base64Encode((unsigned char*)body.c_str(), (unsigned int)body.length(), &b);
                    //                    log("%s",b);
                    std::string data_str = std::string(( char *)b,l);
                    create_response(RECV_DATA_OK_RES, data_str, header_p_pb3->cmd, header_p_pb3->svrid, header_p_pb3->svrid);
                }
            }
        }
        else
        {
            
        }
        
        if (isClose == 0)
        {
            CCLOG("lct:close\n");
            break;
        }
    }
    CCLOG("lct:read end\n");
}

int CCTcpClient::PackHeader3(MsgHeader3* pHeader, char* pBuf)
{
    //    memcpy(pBuf, pHeader, sizeof(MsgHeader2));
    
    char *buf = pBuf;
//    pHeader->cmd   = htonl(pHeader->cmd);
//    pHeader->length   = htons(pHeader->length);
//    pHeader->uid   = htonl(pHeader->uid);
//    pHeader->svrid   = htons(pHeader->svrid);
    
    memcpy(buf, &(pHeader->cmd), sizeof(pHeader->cmd));
    buf = buf + sizeof(pHeader->cmd);
			 
    memcpy(buf, &(pHeader->length), sizeof(pHeader->length));
    buf = buf + sizeof(pHeader->length);
    
    memcpy(buf, &(pHeader->uid), sizeof(pHeader->uid));
    buf = buf + sizeof(pHeader->uid);
    
    memcpy(buf, &(pHeader->svrid), sizeof(pHeader->svrid));
    buf = buf + sizeof(pHeader->svrid);
    return 0;
}

int CCTcpClient::UnPackHeader3(MsgHeader3* pHeader, char* pBuf)
{
    char * buf = pBuf;
    memcpy(&(pHeader->cmd), buf, sizeof(pHeader->cmd));
    buf = buf + sizeof(pHeader->cmd);
			 
    memcpy(&(pHeader->length), buf, sizeof(pHeader->length));
    buf = buf + sizeof(pHeader->length);
    
    memcpy(&(pHeader->uid), buf, sizeof(pHeader->uid));
    buf = buf + sizeof(pHeader->uid);
    
    memcpy(&(pHeader->svrid), buf, sizeof(pHeader->svrid));
    buf = buf + sizeof(pHeader->svrid);
    
//    pHeader->cmd = ntohl(pHeader->cmd);
//    pHeader->length = ntohs(pHeader->length);
//    pHeader->uid = ntohl(pHeader->uid);
//    pHeader->svrid = ntohs(pHeader->svrid);
    return 0;
}
