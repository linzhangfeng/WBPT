#ifndef _HTTP_AGENT_H_
#define _HTTP_AGENT_H_

#include <string>
#include <sstream>
#include <iostream>
#include <semaphore.h>
#include "cocos2d.h"
#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::network;

class CCHttpPacket
{
public:
    CCHttpPacket()
    {
        
    }
    
    virtual ~CCHttpPacket()
    {
        
    }
    
    std::string& tostring()
    {
        return sendData;
    }
    
    Json::Value& tojson()
    {
        return recvVal;
    }
    
    void end()
    {
        //sendData = sendVal.toStyledString().c_str();
        sendData = writer.write(sendVal);
    }
    
    int parse()
    {
        recvVal.clear();
        if (reader.parse(recvData, recvVal))
        {
            return -1;
        }
        
        return 0;
    }
    
    bool resultIsOK()
    {
        if (recvVal["status"].asInt() == 200)
        {
            return true;
        }
        return false;
    }
    
public:
    int                     cmd;
    int                     status;
    long                     httpCode;
    std::string             sendData;
    std::string             recvData;
    Json::Value             sendVal;
    Json::Value             recvVal;
    Json::Reader            reader;
    Json::FastWriter        writer;
    std::string             path;
};

class CCHttpAgent : public Ref
{    
public:
    /** Return the shared instance **/
    static CCHttpAgent *getInstance();
    
    /** Relase the shared instance **/
    static void destroyInstance();
	
    CCHttpAgent();
    virtual ~CCHttpAgent();
public:
    string              url;
    string              imageUrl;
    string              imageUploadUrl;
    string              imageDownloadUrl;
    string              uid;
    string              skey;
    std::map<string, CCHttpPacket*>        packets;
    std::map<string, MyCallBack_Str>       callbacks;
    std::map<string, long long>            httpRequestTime;

    bool init(void);
    void setUrl(string str);
    string getUrl(){return url;}

    void setImageUrl(string str);
    void setImageUploadUrl(string str);
    void setImageDownloadUrl(string str);
    void setUidAndSkey(int myUid, string mySkey);
    void deletHttpPacket(string tag);
//    void doLoadFile(const char* url,const MyCallBack_Str &callback);
//    void doHttpGet(const char* action,const char* param = "{}",const MyCallBack_Str &callback = NULL, const char* tag = "", int timeout = 15);
//    
//    void doHttpPost(const char* action,const char* param = "{}",const MyCallBack_Str &callback = NULL, const char* tag = "", int timeout = 15);
    
public:
//    void sendHttp(string action, string param,string tag, const MyCallBack_Str &callback = NULL, int timeout = 15, bool hasVersion = true);
//    void sendHttp2(string reqUrl, string tag, const MyCallBack_Str &callback = NULL, int timeout = 15);
    void sendHttpUrlPost(const MyCallBack_Str &callback, string action,  std::string param = "", string tag = "", int timeout = 15);
    
    
    void sendHttpPost(const MyCallBack_Str &callback, string action, std::string param = "", string tag = "", int timeout = 15);
    void sendLogPost(const MyCallBack_Str &callback, string action, std::string param = "", string tag = "", int timeout = 15);
    
    void sendHttpImageWithUrl(string reqUrl, string path,string tag, const MyCallBack_Str &callback = NULL, int timeout = 15);
    void sendHttpImage(string reqUrl, string path,string tag, const MyCallBack_Str &callback = NULL, int timeout = 15);
    int  sendHttpImagePost(string action, string path, string tag, const MyCallBack_Str &callback = NULL, int timeout = 15);
    
    void sendPayHttpPost(const MyCallBack_Str &callback, string action, std::string param = "", string tag = "", int timeout = 15);
    
    void sendAppStoreFinishHttpPost(const MyCallBack_Str &callback, string action, std::string param = "", string tag = "", int timeout = 15);
    
    void httpCallback(HttpClient *sender,HttpResponse *response);
    void httpImageCallback(HttpClient *sender,HttpResponse *response);
    void httpImagePostCallback(HttpClient *sender,HttpResponse *response);
    
    void _makeMultiDir(const char *mulDir);
    
    std::string  getPacketData(const std::string& k);
    void popPackets(const std::string& k);
    void checkChangeURL(std::string curURL);
    
private:
    std::stringstream stream;
    std::vector<std::string> url_list;
    
};

#endif //_HTTP_AGENT_H_
