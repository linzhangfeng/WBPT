#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include "ZJH.h"

#include <fstream>
#include "HttpAgent.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

#include "extensions/cocos-ext.h"

USING_NS_CC_EXT;

static CCHttpAgent *self = NULL; // pointer to singleton

CCHttpAgent* CCHttpAgent::getInstance()
{
    if (self == NULL) {
        self = new CCHttpAgent();
        self->init();
    }
    
    return self;
}

void CCHttpAgent::destroyInstance()
{

}

CCHttpAgent::CCHttpAgent()
{

}

CCHttpAgent::~CCHttpAgent()
{
    
}

static std::string getJsonStr(string data)
{
    if (data == "" || data == "{}") return "";
    
    std::string result;
    Json::Reader reader;
    Json::Value value;
    reader.parse(data, value);
    Json::Value::Members member = value.getMemberNames();
    char buf[10240];
    for (Json::Value::Members::iterator iter = member.begin(); iter != member.end(); ++iter)
    {
        std::string name = *iter;
        
        if(value[name].isString())
        {
            snprintf(buf, sizeof(buf), "%s=%s&",name.c_str(),value[name].asCString());
            result += buf;
        }
        else if (value[name].isInt())
        {
            snprintf(buf, sizeof(buf), "%s=%d&",name.c_str(),value[name].asInt());
            result += buf;
        }
        else if (value[name].isDouble())
        {
            snprintf(buf, sizeof(buf), "%s=%f&",name.c_str(),value[name].asDouble());
            result += buf;
        }else if (value[name].isArray())
        {
            snprintf(buf, sizeof(buf), "%s=%s&",name.c_str(),value[name].toStyledString().c_str());
            result += buf;
        }
    }
    return result;
}

static std::vector<std::string> getHead(bool isFile = false)
{
    char buf[512];
    std::vector<std::string> headers;
    headers.push_back("accept-encoding:gzip");
    headers.push_back("charset:utf-8");
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    snprintf(buf, sizeof(buf), "User-Agent:iphone");
    headers.push_back(buf);
#else
    snprintf(buf, sizeof(buf), "User-Agent:android");
    headers.push_back(buf);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    snprintf(buf, sizeof(buf), "X-PkgName:pkgName=com.wbyl.wbdp");
    headers.push_back(buf);
#else
    snprintf(buf, sizeof(buf), "X-PkgName:pkgName=com.tencent.tmgp.wbdp");
    headers.push_back(buf);
#endif
    
    snprintf(buf, sizeof(buf), "UserKey:%s",ZJHModel::getInstance()->UserKey.c_str());
    headers.push_back(buf);
    
    snprintf(buf, sizeof(buf), "agentKey:%s",ZJHModel::getInstance()->agentKey.c_str());
    headers.push_back(buf);
    

    snprintf(buf, sizeof(buf), "X-App:vname=%s;vcode=%d",ZJHModel::getInstance()->vname.c_str(),ZJHModel::getInstance()->vcode);
    headers.push_back(buf);

    snprintf(buf, sizeof(buf), "X-Channel:cid=%s",ZJHModel::getInstance()->channel_id.c_str());
    headers.push_back(buf);

    snprintf(buf, sizeof(buf), "X-Device:imsi=%s;imei=%s;did=%s;dname=%s;os=%s;osver=%s",ZJHModel::getInstance()->phoneimei.c_str(),ZJHModel::getInstance()->phoneimei.c_str(),ZJHModel::getInstance()->phoneimei.c_str(),ZJHModel::getInstance()->phonename.c_str(), ZJHModel::getInstance()->phonePlatfm.c_str(), ZJHModel::getInstance()->phoneOs.c_str());
    
    headers.push_back(buf);

    snprintf(buf, sizeof(buf), "X-User:uid=%d",ZJHModel::getInstance()->uid);
    headers.push_back(buf);

    snprintf(buf, sizeof(buf), "fromType:%d",1);
    headers.push_back(buf);

    if (isFile)
    {
        snprintf(buf, sizeof(buf), "Content-Type:%s;boundary=%s","multipart/form-data","fO7mDiGg4J8kERYOGEp3v8lF2Ul64Z");
        headers.push_back(buf);
    }
    log("head[");
    for (int i = 0; i < headers.size(); i ++)
    {
        log("%s",headers[i].c_str());
    }
    log("]");
    return headers;
}

void CCHttpAgent::deletHttpPacket(string tag)
{
    CCHttpPacket* packet = packets[tag];
    if(packet != NULL)
    {
        packets[tag] = NULL;
        delete packet;
    }
}

bool CCHttpAgent::init()
{
    skey = "";
    uid = "";
    url_list.clear();
    return true;
}

void CCHttpAgent::setUrl(string str)
{
    url = str;
}

void CCHttpAgent::setImageUrl(string str)
{
    imageUrl = str;
}

void CCHttpAgent::setImageUploadUrl(string str)
{
    imageUploadUrl = str;
}

void CCHttpAgent::setImageDownloadUrl(string str)
{
    imageDownloadUrl = str;
}

void CCHttpAgent::setUidAndSkey(int myUid, string mySkey)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%d", myUid);
    uid = buf;
    skey = mySkey;
}

//void CCHttpAgent::sendHttp(string action, string param,string tag,const MyCallBack_Str &callback, int timeout, bool hasVersion)
//{
//    string urlStr;
//    urlStr += url;
//    
//    string reqStr;
//    reqStr += "action=" + action;
//    if(hasVersion)reqStr += "&ver=" + ZJHModel::getInstance()->ver;
//    reqStr += "&app_id=" + ZJHModel::getInstance()->appid;
//    reqStr += "&uid=" + uid;
//    reqStr += "&skey=" + skey;
//    
//    urlStr += reqStr;
//    if(strcmp(param.c_str(), "null") == 0)
//    {
//        param = "{}";
//    }
//    urlStr += "&param=" + param;
//    
//    sendHttp2(urlStr, tag, callback);
//}
//
//void CCHttpAgent::sendHttp2(string reqUrl, string tag,const MyCallBack_Str &callback, int timeout)
//{
//    if (ZJHModel::getInstance()->GameServer == ZSServer)
//    {
//        CCLOG("CCHttpAgent::sendHttp2 [%s][%s]", tag.c_str(), reqUrl.c_str());
//    }
//    else
//    {
//        log("CCHttpAgent::sendHttp2 [%s][%s]", tag.c_str(), reqUrl.c_str());
//    }
//    
//    CCHttpPacket* packet = packets[tag];
//    if(packet == NULL)
//    {
//        packet = new CCHttpPacket();
//        packets[tag] = packet;
//    }
//    
//    packet->status = 0;
//    
//    callbacks[tag] = callback;
//    
//    HttpRequest* request = new HttpRequest();
//    request->setTag(tag.c_str());
//    request->setUrl(reqUrl.c_str());
//    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
//    request->setResponseCallback(CC_CALLBACK_2(CCHttpAgent::httpCallback, this));
//    HttpClient::getInstance()->setTimeoutForConnect(timeout);
//    HttpClient::getInstance()->send(request);
//    request->release();
//}

//void CCHttpAgent::sendHttpGet(const MyCallBack_Str &callback, string action, std::string param, string tag, int timeout)
//{
//    if (tag == "") tag = action;
//    log("---------------------------begin--------------------------------");
//    if (ZJHModel::getInstance()->GameServer == ZSServer)
//    {
//        CCLOG("CCHttpAgent::sendHttpGet [%s][%s]", action.c_str(), param.c_str());
//    }
//    else
//    {
//        log("CCHttpAgent::sendHttpGet [%s][%s]", action.c_str(), param.c_str());
//    }
//    
//    string urlStr;
//    urlStr += url;
//    urlStr += action;
//    
//    string pkey = getJsonStr(param);
//    if(pkey != "")
//    {
//        urlStr += "?";
//        urlStr += pkey;
//    }
//    
//    CCHttpPacket* packet = packets[tag];
//    if(packet == NULL)
//    {
//        packet = new CCHttpPacket();
//        packets[tag] = packet;
//    }
//    
//    packet->status = 0;
//    
//    callbacks[tag] = callback;
//    
//    HttpRequest* request = new HttpRequest();
//    request->setTag(tag.c_str());
//    request->setUrl(urlStr.c_str());
//    
//    request->setHeaders(getHead());
//    
//    log("***********************************end********************************");
//    
//    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
//    request->setResponseCallback(CC_CALLBACK_2(CCHttpAgent::httpCallback, this));
//    HttpClient::getInstance()->setTimeoutForConnect(timeout);
//    HttpClient::getInstance()->send(request);
//    request->release();
//}

void CCHttpAgent::sendHttpUrlPost(const MyCallBack_Str &callback, string action, std::string param, string tag, int timeout)
{
    if (tag == "") tag = action;
    log("---------------------------begin--------------------------------");
    if (ZJHModel::getInstance()->GameServer == ZSServer)
    {
        CCLOG("CCHttpAgent::sendHttpPost [%s][%s]", action.c_str(), param.c_str());
    }
    else
    {
        log("CCHttpAgent::sendHttpPost [%s][%s]", action.c_str(), param.c_str());
    }

    
    string pkey = getJsonStr(param);
    //    if(pkey != "")
    //    {
    //        urlStr += "?";
    //        urlStr += pkey;
    //        log("CCHttpAgent::sendHttpPost test use get url[%s?%s]",urlStr.c_str(),pkey.c_str());
    //    }
    
    CCHttpPacket* packet = packets[tag];
    callbacks[tag] = callback;
    
    if(packet == NULL)
    {
        packet = new CCHttpPacket();
        packets[tag] = packet;
    }
    else if (packet->status == 0)
    {
        return;
    }
    packet->status = 0;
    
    HttpRequest* request = new HttpRequest();
    request->setTag(tag.c_str());
    request->setUrl(action.c_str());
    
    request->setHeaders(getHead());
    log("***********************************end********************************");
    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(CCHttpAgent::httpCallback, this));
    
    const char* postData = pkey.c_str();
    
    log("CCHttpAgent::sendHttpPost data[%s][%s]", action.c_str(), postData);
    
    request->setRequestData(postData, strlen(postData));
    
    HttpClient::getInstance()->setTimeoutForConnect(timeout);
    HttpClient::getInstance()->sendImmediate(request);
    request->release();
}

void CCHttpAgent::sendLogPost(const MyCallBack_Str &callback, string action, std::string param, string tag, int timeout){
    if (tag == "") tag = action;
    log("---------------------------begin--------------------------------");
    if (ZJHModel::getInstance()->GameServer == ZSServer)
    {
        CCLOG("CCHttpAgent::sendHttpPost [%s][%s]", action.c_str(), param.c_str());
    }
    else
    {
        log("CCHttpAgent::sendHttpPost [%s][%s]", action.c_str(), param.c_str());
    }
    
    string urlStr;
    if (action.find("http:") == -1)urlStr += url;
    urlStr += action;
    
    string pkey = getJsonStr(param);
    CCHttpPacket* packet = packets[tag];
    callbacks[tag] = callback;
    httpRequestTime[tag] = Utils::getCurrentSecend();
    if(packet == NULL)
    {
        packet = new CCHttpPacket();
        packets[tag] = packet;
    }
    packet->status = 0;
    
    HttpRequest* request = new HttpRequest();
    request->setTag(tag.c_str());
    request->setUrl(urlStr.c_str());
    
    request->setHeaders(getHead());
    log("***********************************end********************************");
    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(CCHttpAgent::httpCallback, this));
    
    const char* postData = pkey.c_str();
    
    log("CCHttpAgent::sendHttpPost data[%s][%s]", action.c_str(), postData);
    
    request->setRequestData(postData, strlen(postData));
    
    HttpClient::getInstance()->setTimeoutForConnect(timeout);
    HttpClient::getInstance()->send(request);
    request->release();
}

void CCHttpAgent::sendHttpPost(const MyCallBack_Str &callback, string action, std::string param, string tag, int timeout)
{
    if (tag == "") tag = action;
    log("---------------------------begin--------------------------------");
    if (ZJHModel::getInstance()->GameServer == ZSServer)
    {
        CCLOG("CCHttpAgent::sendHttpPost [%s][%s]", action.c_str(), param.c_str());
    }
    else
    {
        log("CCHttpAgent::sendHttpPost [%s][%s]", action.c_str(), param.c_str());
    }
    
    string urlStr;
    if (action.find("http:") == -1)urlStr += url;
    urlStr += action;
    
    string pkey = getJsonStr(param);
//    if(pkey != "")
//    {
//        urlStr += "?";
//        urlStr += pkey;
//        log("CCHttpAgent::sendHttpPost test use get url[%s?%s]",urlStr.c_str(),pkey.c_str());
//    }
    
    CCHttpPacket* packet = packets[tag];
    callbacks[tag] = callback;
    httpRequestTime[tag] = Utils::getCurrentSecend();
    if(packet == NULL)
    {
        packet = new CCHttpPacket();
        packets[tag] = packet;
    }
    else if (packet->status == 0)
    {
        return;
    }
    packet->status = 0;
    
    HttpRequest* request = new HttpRequest();
    request->setTag(tag.c_str());
    request->setUrl(urlStr.c_str());
    
    request->setHeaders(getHead());
    log("***********************************end********************************");
    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(CCHttpAgent::httpCallback, this));
    
    const char* postData = pkey.c_str();
    
    log("CCHttpAgent::sendHttpPost data[%s][%s]", action.c_str(), postData);
    
    request->setRequestData(postData, strlen(postData));
    
    HttpClient::getInstance()->setTimeoutForConnect(timeout);
    HttpClient::getInstance()->sendImmediate(request);
    request->release();
}

void CCHttpAgent::sendHttpImageWithUrl(string action, string path,string tag,const MyCallBack_Str &callback, int timeout)
{
    CCLOG("sendHttpImage[%s][%s]", tag.c_str(), action.c_str());
    
    CCHttpPacket* packet = packets[tag];
    callbacks[tag] = callback;
    
    if(packet == NULL)
    {
        packet = new CCHttpPacket();
        packets[tag] = packet;
    }
    else if (packet->status == 0)
    {
        return;
    }
    packet->status = 0;
    
    string urlStr = action;
    
    packet->path = path;
    
    HttpRequest* request = new HttpRequest();
    request->setTag(tag.c_str());
    request->setHeaders(getHead());
    request->setUrl(urlStr.c_str());
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(CCHttpAgent::httpImageCallback, this));
    HttpClient::getInstance()->setTimeoutForConnect(timeout);
    HttpClient::getInstance()->send(request);
    request->release();
}

void CCHttpAgent::sendHttpImage(string action, string path,string tag,const MyCallBack_Str &callback, int timeout)
{
    CCLOG("sendHttpImage[%s][%s]", tag.c_str(), action.c_str());
    
    CCHttpPacket* packet = packets[tag];
    callbacks[tag] = callback;
    if(packet == NULL)
    {
        packet = new CCHttpPacket();
        packets[tag] = packet;
    }
    else if (packet->status == 0)
    {
        return;
    }
    packet->status = 0;
    
    string urlStr;
    if (action.find("http:") == -1)urlStr += imageDownloadUrl;
    urlStr += action;
    
    packet->path = path;
    
    HttpRequest* request = new HttpRequest();
    request->setTag(tag.c_str());
    request->setHeaders(getHead());
    request->setUrl(urlStr.c_str());
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(CCHttpAgent::httpImageCallback, this));
    HttpClient::getInstance()->setTimeoutForConnect(timeout);
    HttpClient::getInstance()->send(request);
    request->release();
}

int CCHttpAgent::sendHttpImagePost(string action, string path,string tag,const MyCallBack_Str &callback, int timeout)
{
    CCHttpPacket* packet = packets[tag];
    if(packet == NULL)
    {
        packet = new CCHttpPacket();
        packets[tag] = packet;
    }
    else if (packet->status == 0)
    {
        return 0;
    }
    packet->status = 0;
    
    string urlStr;
    urlStr += imageUploadUrl;
    urlStr += action;
    
    callbacks[tag] = callback;
	FILE *  fp = fopen(FileUtils::getInstance()->fullPathForFilename(path).c_str(),"rb");
	if (fp)
	{
        std::string boundary_str = "fO7mDiGg4J8kERYOGEp3v8lF2Ul64Z";
        std::string p_name = "imageFile";
        std::string f_name = "filename";
        std::string cutImage = "0";
        std::string fileSuffix = "_240x240";
        std::string type = "gameroom";
        
        std::string sendBody_Begin;
        sendBody_Begin.clear();
        sendBody_Begin.append("--" + boundary_str + "\r\n");
        sendBody_Begin.append("Content-Disposition:form-data;name=\"cutImage\"\r\n\r\n" + cutImage + "\r\n");
        
        sendBody_Begin.append("--" + boundary_str + "\r\n");
        sendBody_Begin.append("Content-Disposition:form-data;name=\"type\"\r\n\r\n" + type + "\r\n");
        
        sendBody_Begin.append("--" + boundary_str + "\r\n");
        sendBody_Begin.append("Content-Disposition:form-data;name=\"fileSuffix\"\r\n\r\n" + fileSuffix + "\r\n");
        
        sendBody_Begin.append("--" + boundary_str + "\r\n");
        sendBody_Begin.append("Content-Disposition:form-data;name=\"imageFile\";filename=\"" + path + "\"\r\n");
        sendBody_Begin.append("Content-Type: application/octet-stream\r\n\r\n");
        
        const char* begin = sendBody_Begin.c_str();
        long beginL = sendBody_Begin.length();
        
        std::string sendBody_end;
        sendBody_end.clear();
        sendBody_end.append("\r\n--" + boundary_str + "--\r\n");
        const char* end = sendBody_end.c_str();
        long endL = sendBody_end.length();
		fseek(fp, 0, SEEK_END);
		long len = ftell(fp);
		fseek(fp, 0, SEEK_SET);
        
        char *buf = (char*)malloc(beginL + len + endL);

        memcpy(&buf[0], begin, beginL);
        
		fread(&buf[beginL], len, 1, fp);
        
		fclose(fp);

        memcpy(&buf[beginL + len], end, endL);
        
        HttpRequest* request = new HttpRequest();
        request->setUrl(urlStr.c_str());
        
        request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
        request->setResponseCallback(CC_CALLBACK_2(CCHttpAgent::httpCallback, this));

        request->setRequestData(buf, len + beginL + endL);
        
        request->setHeaders(getHead(true));
        request->setTag(tag.c_str());
        HttpClient::getInstance()->sendImmediate(request);
        request->release();
        if (buf)
        {
            free( buf);
        }
        return 0;
    }

    packet->status = 2;
    
    if (callback != NULL)
    {
        callback(tag);
        callbacks[tag] = NULL;
    }
    
    return -1;
}

void CCHttpAgent::checkChangeURL(std::string curURL)
{
    if (url_list.empty())
    {
        url_list.clear();
        Json::Value json = Utils::ReadFile("urls_tmp.data");
        if (!json["urls"].isNull() && json["urls"].isArray())
        {
            log("%s",json.toStyledString().c_str());
            for (int i = 0; i < json["urls"].size(); i ++)
            {
                if(json["urls"][i]["ip_status"].asString() == "1")url_list.push_back(json["urls"][i]["ip"].asString() + "/");
            }
        }
        else
        {
            url_list.push_back("http://wb.boxwan.cn/chess/");
            url_list.push_back("http://wb.ihuizhi.com/chess/");
            url_list.push_back("http://test1.boxwan.cn/chess/");
        }
    }
    
    if (curURL.find(url) != std::string::npos && url_list.size() > 1)
    {
        //use next url;
        int curIndex = -1;
        for (int i = 0; i < url_list.size(); i ++)
        {
            if (url_list[i] == url)
            {
                curIndex = i;
                break;
            }
        }
        
        curIndex++;
        if (curIndex >= url_list.size())curIndex -= url_list.size();
        
        url = url_list[curIndex];
        Manifest::_baseUrl = url;
        UserDefault::getInstance()->setStringForKey("default_use_url",url);
        UserDefault::getInstance()->flush();
    }
}

void CCHttpAgent::httpCallback(HttpClient *sender,HttpResponse *response)
{
    if (response == NULL)
        return;
    
    std::string tag = response->getHttpRequest()->getTag();
    long status = response->getResponseCode();
    CCHttpPacket* packet = packets[tag];
    
    if (!packet) return;
    
    if (ZJHModel::getInstance()->GameServer == ZSServer)
    {
        CCLOG("code[%ld] tag[%s]", status, tag.c_str());
    }
    else
    {
        CCLOG("code[%ld] tag[%s]", status, tag.c_str());
    }
    packet->httpCode = status;
    if (!response->isSucceed() || status != 200)
    {
        packet->status = 1;
        log("response failed");
        log("error buffer: %s", response->getErrorBuffer());
        checkChangeURL(response->getHttpRequest()->getUrl());
    }
    else
    {
        std::vector<char> *buffer = response->getResponseData();
        std::string returnStr = "";
        for (unsigned int i = 0; i < buffer->size(); i++)
        {
            returnStr.append(1, (*buffer)[i]);
        }
        
        if (returnStr == "")
        {
            Json::Value tempJson;
            tempJson["status"] = 404;
            tempJson["message"] = "网络异常，请重试";
            returnStr = tempJson.toStyledString();
        }
        packet->status = 3;
        packet->recvData = returnStr;
        if (packet->parse() < 0)
        {
//            packet->status = 2;
        }
        else
        {
//            packet->status = 3;
        }
        
        if (ZJHModel::getInstance()->GameServer == ZSServer)
        {
            CCLOG("status[%d][%s]", packet->status, returnStr.c_str());
        }else
        {
            CCLOG("status[%d][%s]", packet->status, returnStr.c_str());
        }
    }
    
    MyCallBack_Str callback = callbacks[tag];
    if (callback != NULL)
    {
        callback(tag);
        callbacks[tag] = NULL;
    }
    //计算响应时间差
    long long requestTime = httpRequestTime[tag];
    if(requestTime == 0){
        return;
    }
    long timeIntval = Utils::getCurrentSecend() - requestTime;
    if(response->getResponseCode() == 200 && timeIntval > 2500){
        //大于2秒则上报
         ostringstream os;
         os<<timeIntval;
         string result;
         istringstream is(os.str());
         is>>result;
         string requestUrl = response->getHttpRequest()->getUrl();
         Utils::gameCommandErrorRecord(requestUrl+"======响应时间:"+result);
    }
    httpRequestTime.erase(tag);
}

void CCHttpAgent::sendPayHttpPost(const MyCallBack_Str &callback, string action, std::string param, string tag, int timeout)
{
    if (tag == "") tag = action;
    log("---------------------------begin--------------------------------");
    if (ZJHModel::getInstance()->GameServer == ZSServer)
    {
        CCLOG("CCHttpAgent::sendHttpPost [%s][%s]", action.c_str(), param.c_str());
    }
    else
    {
        log("CCHttpAgent::sendHttpPost [%s][%s]", action.c_str(), param.c_str());
    }
    
    string urlStr;
    //urlStr += "http://pay.boxwan.cn/service/";
    urlStr += "http://test2.boxwan.cn/service/";
    //urlStr += "http://188.188.1.139:8081/service/";

    urlStr += action;
    
    if (action == "purchase/v2") {
        char buff[128];
        Json::Value json = Utils::ParseJsonStr(param);

        //json["notifyUrl"] = "http://wb.ihuizhi.com/chess/notify/callback/huizhi";
        //json["notifyUrl"] = "http://188.188.1.111:20006/chess/notify/callback/huizhi";
        
        json["spType"] = "1";
        json["qn"] = "86011019";
        
        json["line1Number"] = "";
        json["appId"] = "801";
        json["payMode"] = 2;
        json["v"] = "1.8.801.6";
        json["signType"] = "md5";
        
        //json["imei"] = PlatformHelper::sdkDoForStr_C("phoneimei");
        //json["imsi"] = PlatformHelper::sdkDoForStr_C("phoneimei");
        //支付服务器的数据库里imei和imsi的存储类型是long，所以此处只能用个虚假的值
        json["imei"] = "123456";
        json["imsi"] = "654321";
        
        snprintf(buff, sizeof(buff), "%lldd", Utils::getCurrentTime());
        json["tradeId"] = buff;
        json["phoneModel"] = "iPhone";
        snprintf(buff, sizeof(buff), "%d", ZJHModel::getInstance()->uid);
        json["payerId"] = buff;
        json["simSerialNumber"] = "89860063191507076262";
        json["partnerId"] = "1000100060000000";
        
        std::string encodeKeys[10] = {
            "appId",
            "extInfo",
            "notifyUrl",
            "partnerId",
            "payerId",
            "qn",
            "reqFee",
            "tradeDesc",
            "tradeId",
            "tradeName",
        };
        
        std::string codeStr = "";
        std::string sign;
        std::string sign2;
        
        for (int i = 0; i < 10; i++) {
            codeStr = codeStr + encodeKeys[i] + "=" + json[encodeKeys[i].c_str()].asString() + "&";
        }
        codeStr = codeStr + "8dbb166865b31d14d9c7bfa1a87f3abc";
        MD5::MD5Str(codeStr, sign);
        
        codeStr = "MHHIISDHI&" + json["imei"].asString() + json["imsi"].asString();
        MD5::MD5Str(codeStr, sign2);
        
        json["sign"] = sign;
        json["sign2"] = sign2;
        
        param = json.toStyledString();
    }
    
    string pkey = getJsonStr(param);
    
    CCHttpPacket* packet = packets[tag];
    if(packet == NULL)
    {
        packet = new CCHttpPacket();
        packets[tag] = packet;
    }
    
    packet->status = 0;
    
    callbacks[tag] = callback;
    
    HttpRequest* request = new HttpRequest();
    request->setTag(tag.c_str());
    request->setUrl(urlStr.c_str());
    
    request->setHeaders(getHead());
    log("***********************************end********************************");
    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(CCHttpAgent::httpCallback, this));
    
    const char* postData = pkey.c_str();
    
    log("CCHttpAgent::sendHttpPost data[%s][%s]", action.c_str(), postData);
    
    request->setRequestData(postData, strlen(postData));
    
    HttpClient::getInstance()->setTimeoutForConnect(timeout);
    HttpClient::getInstance()->sendImmediate(request);
    request->release();

}

void CCHttpAgent::sendAppStoreFinishHttpPost(const MyCallBack_Str &callback, string action, std::string param, string tag, int timeout)
{
    if (tag == "") tag = action;
    log("---------------------------begin--------------------------------");
    if (ZJHModel::getInstance()->GameServer == ZSServer)
    {
        CCLOG("CCHttpAgent::sendHttpPost [%s][%s]", action.c_str(), param.c_str());
    }
    else
    {
        log("CCHttpAgent::sendHttpPost [%s][%s]", action.c_str(), param.c_str());
    }
    
    string urlStr;
    //urlStr += "http://pay.boxwan.cn/service/";
    urlStr += "http://test2.boxwan.cn/service/";
     //urlStr += "http://188.188.1.139:8081/service/";
    
    urlStr += action;
    
    string pkey = getJsonStr(param);
    
    CCHttpPacket* packet = packets[tag];
    httpRequestTime[tag] = Utils::getCurrentSecend();
    if(packet == NULL)
    {
        packet = new CCHttpPacket();
        packets[tag] = packet;
    }
    
    packet->status = 0;
    
    callbacks[tag] = callback;
    
    HttpRequest* request = new HttpRequest();
    request->setTag(tag.c_str());
    request->setUrl(urlStr.c_str());
    
    request->setHeaders(getHead());
    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(CCHttpAgent::httpCallback, this));
    
    const char* postData = pkey.c_str();
    log("CCHttpAgent::sendHttpPost data[%s][%s]", action.c_str(), postData);
    
    request->setRequestData(postData, strlen(postData));
    HttpClient::getInstance()->setTimeoutForConnect(timeout);
    HttpClient::getInstance()->sendImmediate(request);
    request->release();
}

void CCHttpAgent::httpImageCallback(HttpClient *sender,HttpResponse *response)
{
    if (response == NULL)
        return;
    
    long status = response->getResponseCode();
    std::string tag = response->getHttpRequest()->getTag();
    CCHttpPacket* packet = packets[tag];
    
    if (!packet) return;
    CCLOG("httpImageCallback code[%ld] tag[%s]", status, tag.c_str());
    
    if (!response->isSucceed() || status != 200)
    {
        packet->status = 1;
    }
    else
    {
        _makeMultiDir(packet->path.c_str());
        
        std::vector<char> *buffer = response->getResponseData();
        std::ofstream outFile (packet->path.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
        if (outFile)
        {
            for (unsigned int i = 0; i < buffer->size(); i++)
            {
                outFile.write(&(*buffer)[i], 1);
            }
            outFile.close();
            packet->status = 3;
        }
        else
        {
            packet->status = 2;
        }
    }
    
    MyCallBack_Str callback = callbacks[tag];
    if (packet->status == 3 && callback != NULL)
    {
        callback(tag);
        callbacks[tag] = NULL;
    }
}

void CCHttpAgent::popPackets(const std::string& tag)
{    CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
    if (packet) {
        CCHttpAgent::getInstance()->packets.erase(tag);
        delete packet;
    }

}

void CCHttpAgent::_makeMultiDir(const char *mulDir)
{
    int i, len;
    char str[512];
    
    strncpy(str, mulDir, 512);
    len = strlen(str);
    for (i = 0; i < len; i++) {
        if (str[i] == '/') {
            str[i] = '\0';
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
            DIR *pDir = NULL;
            
            pDir = opendir (str);
            if (! pDir)
            {
                mkdir(str, S_IRWXU | S_IRWXG | S_IRWXO);
            }
#else
            if ((GetFileAttributesA(str)) == INVALID_FILE_ATTRIBUTES)
            {
                CreateDirectoryA(str, 0);
            }
#endif
            str[i] = '/';
        }
    }
}

std::string  CCHttpAgent::getPacketData(const std::string& k)
{
    auto p = packets[k];
    if(p)
    {
        if(p->status == 3)
        {
            return p->recvData;
        }
    }
    return "";
}
void CCHttpAgent::httpImagePostCallback(HttpClient *sender,HttpResponse *response)
{
//    if (!response)
//    {
//        return;
//    }
//    
//    long status = response->getResponseCode();
//    std::string tag = response->getHttpRequest()->getTag();
//    CCHttpPacket* packet = packets[tag];
//    
//    if (!packet) return;
//    CCLOG("httpImageCallback code[%ld] tag[%s]", status, tag.c_str());
//    
//    long statusCode = response->getResponseCode();
//    
//    if (!response->isSucceed() || status != 200)
//    {
//        packet->status = 1;
//    }
//    else
//    {
//        
//    }
//    
//    std::vector<char> *buffer = response->getResponseData();
//    std::string returnStr;
//    for (unsigned int i = 0; i < buffer->size(); i++)
//    {
//        returnStr.append(1, (*buffer)[i]);
//    }
//    
//    CCLOG("status[%ld][%s]", statusCode,returnStr.c_str());
//    
//    if (ZJHModel::getInstance()->GameServer == DebugServer)
//    {
//        
//    }
//    else
//    {
//        Utils::xorfunc(returnStr);
//    }
//    
//    CCLOG("status[%ld][%s]", statusCode,returnStr.c_str());
//    
//    CCHttpPacket packet;
//    packet.recvData = returnStr;
//    if (packet.parse() < 0)
//    {
//        
//    }
//    else
//    {
//        if (statusCode == 200 && tag.compare("photo") == 0)
//        {
//            if(packet.recvVal["ret"].asInt() == 0)
//            {
//                postImageStatus = 3;
//                log("upload photo is succ.");
//                MyCallBack_Str callback = callbacks[tag];
//                if (callback != NULL)
//                {
//                    callback(tag);
//                    callbacks[tag] = NULL;
//                }
//                return;
//            }
//        }
//    }
//    
//    log("upload photo is fail.");
//    postImageStatus = 2;
//    
//    MyCallBack_Str callback = callbacks[tag];
//    if (callback != NULL)
//    {
//        callback(tag);
//        callbacks[tag] = NULL;
//    }
}
