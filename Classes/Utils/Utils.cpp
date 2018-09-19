//
//  Utils.cpp
//  ZJH
//
//  Created by wujunru on 14-4-16.
//
//

#include "Utils.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <fstream>
#include <istream>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include "PingCommand.hpp"
#endif


void Utils::saveUseAndPwd(std::string userStr,std::string passwordStr)
{
    if (ZJHModel::getInstance()->GameServer == DebugServer)
    {
        UserDefault::getInstance()->setStringForKey("userDebug", userStr);
        UserDefault::getInstance()->setStringForKey("passwordDebug", passwordStr);
        UserDefault::getInstance()->flush();
    }
    else
    {
        UserDefault::getInstance()->setStringForKey("user", userStr);
        UserDefault::getInstance()->setStringForKey("password", passwordStr);
        UserDefault::getInstance()->flush();
    }
}

bool Utils::hasImageFile(std::string fileUrl)
{
    return FileUtils::getInstance()->isFileExist(getImageSavePath(fileUrl));
}

std::string Utils::getImageSavePath(string fileUrl)
{
    const static std::string findFlag = "/chess_img/";
    int cutPos = fileUrl.find(findFlag);
    if (cutPos == std::string::npos) {
        return fileUrl;
    } else {
        std::string localPath = fileUrl.substr(cutPos + findFlag.length());
        return getSavePath() + "images/" + localPath;
    }
}

std::string Utils::getFileName(string path){
     int pos = path.find_last_of('/');
     return path.substr(pos + 1);
}

std::string Utils::getTmpFoldPath()
{
    return getSavePath() + "tmp/";
}

Sprite * Utils::CirCleSprite(Sprite *avatar,std::string StencilBg)
{
    //参数解释 avatar 要显示头像 StencilBg 底板 必须透明
    //得到的头像必须使用  Sprite->getTexture()->setAntiAliasTexParameters();来抗锯齿，
    /*
     例子 Sprite * photo = CirCleSprite("","");
     photo->getTexture()->setAntiAliasTexParameters();
     */
    if (!avatar) {
        return NULL;
    }
    if(StencilBg == "")
    {
        return NULL;
    }
    Sprite * Stencil =Sprite::create(StencilBg);
    if (!Stencil) {
        return NULL;
    }
    cocos2d::Rect rect = avatar->boundingBox();
    if (Stencil->getContentSize().width < rect.size.width)
    {
        float scaleX = (Stencil->getContentSize().width-2) / rect.size.width;
        float scaleY = (Stencil->getContentSize().height-2) / rect.size.height;
        avatar->setScaleX(scaleX);
        avatar->setScaleY(scaleY);
    }
    
    int x = Stencil->getContentSize().width - avatar->getContentSize().width;
    int y = Stencil->getContentSize().height - avatar->getContentSize().height;
    RenderTexture * renderTexture = RenderTexture::create(Stencil->getContentSize().width, Stencil->getContentSize().height);
    
    Stencil->setPosition(Vec2(Stencil->getContentSize().width / 2, Stencil->getContentSize().height / 2));
    avatar->setPosition(Vec2(avatar->getContentSize().width / 2+ x / 2, avatar->getContentSize().height / 2 + y / 2));
    
    Stencil->setBlendFunc((BlendFunc){GL_ONE, GL_ZERO});
    avatar->setBlendFunc((BlendFunc){GL_DST_ALPHA, GL_ZERO});
    
    renderTexture->begin();
    Stencil->visit();
    avatar->visit();
    renderTexture->end();
    
    Sprite * retval = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
    retval->setFlippedY(true);
    return retval;
}

long long Utils::getCurrentTime()
{
    return Utils::getCurrentSecend();
}

long long Utils::getCurrentSecend()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return ms;
}

string Utils::getMessageTimeFormat( long long time){
    time_t t;
    struct tm *p;
    char bufTime[128];
    
    t = Utils::getCurrentTime() * 0.001;
    p = localtime(&t);
    strftime(bufTime, sizeof(bufTime), "%Y-%m-%d", p);
    std::string now_Day = bufTime;
    
    t = time * 0.001;
    p = localtime(&t);
    strftime(bufTime, sizeof(bufTime), "%Y-%m-%d", p);
    std::string to_Day = bufTime;
    
    if (to_Day == now_Day)
    {
        strftime(bufTime, sizeof(bufTime), "%H:%M", p);
    }
    else
    {
        strftime(bufTime, sizeof(bufTime), "%Y-%m-%d %H:%M", p);
    }
    return bufTime;
}

void Utils::getTimeOver(long long totime,char *buf)
{
    long long time = (Utils::getCurrentTime() - totime);
    time = time * 0.001;
    if (time <= 60)
    {
        snprintf(buf, 128, "%s","刚刚");
    }
    else if (time <= 60 * 60)
    {
        snprintf(buf, 128, "%lld分钟前",time / 60);
    }
    else if (time <= 60 * 60 * 24)
    {
        snprintf(buf, 128, "%lld小时前",time / 60 / 60);
    }
    else
    {
        time_t t;
        struct tm *p;
        t = totime * 0.001 + 8 * 60 * 60;
        p = localtime(&t);
        strftime(buf, 128, "%Y-%m-%d", p);
//        strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", p);
    }
}

std::string Utils::getFormatedTime(long long time)
{
    int hour = time / 3600;
    int minus = (time % 3600) / 60;
    int second = time % 60;
    char buff[64];
    
    snprintf(buff, sizeof(buff), "%02d:%02d:%02d", hour, minus,second);
    return buff;
}

Sprite *Utils::createDialogBG(int width, int height)
{
    Sprite *bg = Sprite::create();
    bg->setContentSize(Size(width, height));
    
    Scale9Sprite *sp = Scale9Sprite::create("Room/dialog/v_dialog_info_bg.9.png");
    sp->setAnchorPoint(Vec2(0.5, 0.5));
    sp->setPosition(bg->getContentSize() / 2);
    sp->setContentSize(Size(width, height));
    bg->addChild(sp);
    
    return bg;
}


Sprite*  Utils::createMaskedSprite(Sprite* src, const char* maskFile)
{
    Sprite * mask = Sprite::create(maskFile);
    
    assert(src);
    assert(mask);
    
    Size srcContent = src->getContentSize();
    Size maskContent = mask->getContentSize();
    
    RenderTexture * rt = RenderTexture::create(srcContent.width, srcContent.height);
    
    float ratiow = srcContent.width / maskContent.width;
    float ratioh = srcContent.height / maskContent.height;
    mask->setScaleX(ratiow);
    mask->setScaleY(ratioh);
    
    mask->setPosition(Vec2(srcContent.width / 2, srcContent.height / 2));
    src->setPosition(Vec2(srcContent.width / 2, srcContent.height / 2));
    
    BlendFunc blendFunc2 = { GL_ONE, GL_ZERO };
    mask->setBlendFunc(blendFunc2);
    BlendFunc blendFunc3 = { GL_DST_ALPHA, GL_ZERO };
    src->setBlendFunc(blendFunc3);
    
    rt->begin();
    mask->visit();
    src->visit();
    rt->end();
    
    Sprite * retval = Sprite::createWithTexture(rt->getSprite()->getTexture());
    retval->setFlipY(true);
    return retval;
}

ImageView* Utils::createNormalImage(std::string url, std::string defualtFile, cocos2d::Size size){
    std::string savePath = Utils::getImageSavePath(url);
    Sprite* tp = Sprite::create(savePath);

    if (hasImageFile(url) && tp != NULL)
    {
        ImageView* avatar = ImageView::create(savePath);
        if (avatar == NULL)
        {
            avatar = ImageView::create(defualtFile);
        }
        return avatar;
    }
    else
    {
        ImageView* avatar = ImageView::create(defualtFile);
        CCHttpAgent::getInstance()->sendHttpImage(url, savePath, __String::createWithFormat("%s_%lld_%d",url.c_str(),getCurrentTime(),Random(0, 9999))->getCString(), [=](std::string tag)
                                                  {
                                                      CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                      if (packet) {
                                                          CCHttpAgent::getInstance()->packets.erase(tag);
                                                      }
                                                      
                                                      if (avatar->getReferenceCount() <= 0 || avatar->getReferenceCount() > 10)return;
                                                      
                                                      if (packet->status == 3)
                                                      {
                                                          Sprite* temp = Sprite::create(savePath);
                                                          if(Utils::hasImageFile(url) && temp != NULL)
                                                          {
                                                              avatar->loadTexture(Utils::getImageSavePath(url));
                                                          }
                                                      }
                                                      delete packet;
                                                  });
        
        return avatar;
    }
}

Sprite* Utils::createCircleImage(std::string url,std::string defualtFile,std::string clipfile, cocos2d::Size size)
{
//    Sprite* sp = Sprite::create();
//    sp->setContentSize(size);
//    
//    ClippingNode* clippingNode = ClippingNode::create();
////    clippingNode->ignoreAnchorPointForPosition(false);
//    clippingNode->setContentSize(size);
//    Sprite* Stencil = Sprite::create(clipfile);
//    Stencil->setScale(size.width / Stencil->getContentSize().width, size.height / Stencil->getContentSize().height);
//    clippingNode->setStencil(Stencil);
//    clippingNode->setAlphaThreshold(0.05f);
//    clippingNode->setInverted(false);
//    
//    Sprite* avatar = ImageByUrl::create(url, defualtFile, size);
//    clippingNode->addChild(avatar);
//    sp->addChild(clippingNode);
//    
//    return sp;
    
    return createCircleAvatar(url,defualtFile,clipfile,size);
}

Sprite* Utils::createCircleAvatar(std::string url,std::string defualtFile,std::string clipfile, cocos2d::Size size)
{
    std::string savePath = Utils::getImageSavePath(url);
    
    if (hasImageFile(url))
    {
        Sprite* avatar = Sprite::create(savePath);
        if (avatar == NULL)
        {
            avatar = Sprite::create(defualtFile);
        }
        makeCircleSprite(avatar,savePath,clipfile,size);
        return avatar;
    }
    else
    {
        Sprite* avatar = Sprite::create(defualtFile);
        makeCircleSprite(avatar,defualtFile,clipfile,size);
        CCHttpAgent::getInstance()->sendHttpImage(url, savePath, __String::createWithFormat("%s_%ld_%d",url.c_str(),getCurrentTime(),Random(0, 9999))->getCString(), [=](std::string tag)
                                                  {
                                                      CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                      if (packet) {
                                                          CCHttpAgent::getInstance()->packets.erase(tag);
                                                      }
                                                      
                                                      if (avatar->getReferenceCount() <= 0 || avatar->getReferenceCount() > 10)return;
                                                      
                                                      if (packet->status == 3)
                                                      {
                                                          makeCircleSprite(avatar,savePath,clipfile,size);
                                                      }
                                                      delete packet;
                                                  });
        
        return avatar;
    }
}

void Utils::makeCircleSprite(Sprite* avatar_s,std::string avatafile,std::string clipfile, cocos2d::Size size)
{
    Sprite * Stencil = Sprite::create(clipfile);
    Sprite * avatar = Sprite::create(avatafile);
    
    if (avatar == NULL)
    {
        float s_izex = size.width / avatar_s->getContentSize().width;
        float s_izey = size.height / avatar_s->getContentSize().height;
        avatar_s->setScale(s_izex,s_izey);
        return;
    }
    cocos2d::Size a_rect = avatar->getContentSize();
    cocos2d::Size cl_rect = Stencil->getContentSize();
    
    float scaleX = (a_rect.width) / cl_rect.width;
    float scaleY = (a_rect.height) / cl_rect.height;
    
    float minscale = MIN(scaleX,scaleY);
    Stencil->setScale(minscale);
    
    RenderTexture * renderTexture = RenderTexture::create(a_rect.width, a_rect.height);
    
    Stencil->setPosition(Vec2(a_rect.width / 2, a_rect.height / 2));
    avatar->setPosition(Vec2(a_rect.width / 2, a_rect.height / 2));
    
    Stencil->setBlendFunc((BlendFunc){GL_ONE, GL_ZERO});
    avatar->setBlendFunc((BlendFunc){GL_DST_ALPHA, GL_ZERO});
    
    renderTexture->begin();
    Stencil->visit();
    avatar->visit();
    renderTexture->end();
    
    avatar_s->setTexture(renderTexture->getSprite()->getTexture());
    avatar_s->setFlippedY(true);
    
    avatar_s->getTexture()->setAntiAliasTexParameters();
    
    float s_izex = size.width / avatar_s->getContentSize().width;
    float s_izey = size.height / avatar_s->getContentSize().height;
    avatar_s->setScale(s_izex,s_izey);
}

map<string,vector<string>> Utils::searchIpSameName(Json::Value list){
    map<string,vector<string>> map;
    for (int i = 0; i < list.size(); i++) {
        Json::Value temp = list[i];
        string key = temp["ip"].asString();
        if (key != "") {
            char buff[128];
            memset(buff, 0, sizeof(buff));
            string nickName = temp["nickName"].asString();
            Utils::parseName(6,buff, nickName.c_str());
            string str = string(buff);
            map[key].push_back(str);
            //测试代码
//            map[key].push_back(str+"1");
//            map[key].push_back(str+"2");
        }
    }
    return map;
}

bool Utils::isExitGather(vector<int> curlist, vector<vector<int>> &retlist) {
    if (retlist.size() == 0) {
        return false;
    }
    bool isNew = false;
    for (int i = 0; i < retlist.size(); i++) {
        vector<int> temp = retlist[i];
        int a = 0;
        for (int j = 0; j < curlist.size(); j++) {
            for (int k = 0; k < temp.size(); k++) {
                if (curlist[j] == temp[k]) {
                    a++;
                }
            }
        }
        if (a == curlist.size()) {
            return true;
        }
    }
    return false;
}

void Utils::searchInGather(int index,Json::Value data,vector<int> curlist,vector<vector<int>> &retlist) {
    if (index == data.size() - 1) return;
    for (int i = index; i < data.size(); i++) {
        bool isClose = true;
        Json::Value data1 = data[i];
        cocos2d::Point point1 = cocos2d::Point(data1["latitude"].asDouble(),data1["longitude"].asDouble());
        for (int j = 0; j < curlist.size(); j++) {
            int tempIndex =  curlist[j];
            cocos2d::Point point2 = cocos2d::Point(data[tempIndex]["latitude"].asDouble(),data[tempIndex]["longitude"].asDouble());
            if (abs(PlatformHelper::getDistanceByCoordinate(point1,point2)) > 500) {
                isClose = false;
            }
        }
        if (isClose) {
            curlist.push_back(i);
        } else {
            vector<int> cc(curlist);
            searchInGather(i+1, data, cc,retlist);
        }
        if (i == data.size() - 1) {
            if (curlist.size() > 1 && !isExitGather(curlist, retlist)) {
                retlist.push_back(curlist);
            }
        }
    }
}

void Utils::searchGatherWithPath(vector<map<vector<int>,int>> &retlist,Json::Value list,int tipLength){
    for(int i = 0; i < list.size(); i++){
        map<vector<int>,int> map;
        Json::Value dataFirst = list[i];
        cocos2d::Point point1 = cocos2d::Point(dataFirst["latitude"].asDouble(),dataFirst["longitude"].asDouble());
        for(int j = i+1;j < list.size();j++){
            Json::Value dataSecond = list[j];
            vector<int> temp;
            cocos2d::Point point2 = cocos2d::Point(dataSecond["latitude"].asDouble(),dataSecond["longitude"].asDouble());
            int distance = abs(PlatformHelper::getDistanceByCoordinate(point1,point2));
            if (distance <= tipLength && distance > 0) {
                temp.push_back(i);
                temp.push_back(j);
                map.insert(pair<vector<int>,int>(temp,distance));
            }
        }
        if(map.size() > 0){
            retlist.push_back(map);
        }
    }
}

void Utils::searchGather(vector<vector<int>> &retlist,Json::Value list) {
    //循环有多少个人
    for (int i = 0; i < list.size(); i++) {
        vector<int> temp;
        temp.push_back(i);
        searchInGather(i+1, list, temp, retlist);
    }
}

string Utils::getFastChatSound(std::string chat)
{
    std::map<std::string,std::string> fastChat_map;
    fastChat_map["别墨迹了"] = "moji";
    fastChat_map["底裤都输光了"] = "shuguang";
    fastChat_map["风水轮流转"] = "fengshui";
    fastChat_map["今儿个真高兴"] = "gaoxing";
    fastChat_map["看我通杀全场"] = "tongsha";
    fastChat_map["快点下注吧"] = "xiazhu";
    fastChat_map["你太牛了"] = "tainiu";
    fastChat_map["下手别这样重啊"] = "xiashou";
    fastChat_map["咋比乌龟还慢呢"] = "wugui";
    fastChat_map["这些钱都是我的"] = "wode";
    if (fastChat_map.find(chat) != fastChat_map.end())
    {
        return fastChat_map[chat];
    }
    return "";
}

//游戏指令错误记录 网络类型，运营商信息，gameId,发生时间
void Utils::gameCommandErrorRecord(const string str){
    Json::Value config = ZJHModel::getInstance()->gameConfigJson;
    int isOpen = atoi(config["list"]["openLog"]["clientLogOpen"].asString().c_str()); // 1开启  -1关闭
//    int isOpen = config["list"]["clientLogOpen"].asInt(); // 1开启  -1关闭
    if(isOpen != 1){
        printf("=======日志上报没有开启=====");
        return;
    }
    if (str.find("www.baidu.com") < str.length())
    {
        return;
    }
    string netInfo = PlatformHelper::getNetWorkType();
    
    Json::Value json = ParseJsonStr(netInfo);
    //{userKey,networkType,operatorType，gameId，tcpCode，produceTime}
    ostringstream os;
    os<<Utils::getCurrentTime();
    string result;
    istringstream is(os.str());
    is>>result;

    ostringstream osBai;
    osBai<<PingCommand::getInstance()->baiduRespTime;
    string bai;
    istringstream isBai(osBai.str());
    isBai>>bai;
    
    string cts = "";
    cts += json["networkType"].asString() + ";@" + json["operatorType"].asString() + ";@" + str + ";@" + result + ";@" + "百度最后一次响应时间:" + bai + "\n";
    
    
    FileUtils* fileUtils = FileUtils::getInstance();
    string path = fileUtils->getWritablePath();
    bool isExist = fileUtils->isFileExist(path);
    if(!isExist){
        bool isSuccess = createDirectory(path);
        printf("=======isSuccess=====创建文件=%d",isSuccess);
    }else{
        printf("=======isSuccess======文件已存在");
    }
    path += "error_record.txt";
    FILE* file = fopen(path.c_str(), "ab");
    string content = json.toStyledString();
    //向本地写入文件
    if (file)
    {
        fseek(file, 0, SEEK_END);
        fputs(cts.c_str(), file);
        fclose(file);
        log(" isSuccess Write succeed");
    }
    else
        log(" isSuccess Write failed %s",path.c_str());
    
}

//客户端启动的时候调用一次
void Utils::uploadGameCommandRecord(){
    //读取文件
    string path = FileUtils::getInstance()->getWritablePath() + "error_record.txt";
    ifstream inFile;
    inFile.open(path);//打开文件
    if (inFile) {
        Json::Value outJson;
        int count = 0;
        for(string str;getline(inFile,str);)//一行行的读取
        {
            std::vector<std::string> result = splitString(str,";@");
            Json::Value json;
            if(result.size() == 5){
                json["networkType"] = result[0];
                json["operatorType"] = result[1];
                json["content"] = result[2];
                json["produceTimeStr"] = result[3];
                json["extend"] = result[4];
                outJson.append(json);
                count++;
                if(count == 20){
                    count = 0;
                    uploadOnce(outJson);
                    outJson.clear();
                }
            }
        }
        if(outJson.size() > 0){
            uploadOnce(outJson);
        }
        inFile.close();
        FileUtils::getInstance()->removeFile(path);
    }
}

std::vector<std::string> Utils::splitString(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();
    
    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

void Utils::uploadOnce(Json::Value outJson){
    if(outJson.size() > 0){
        Json::Value param;
        param["data"] = outJson;
        //上报游戏指令异常
        CCHttpAgent::getInstance()->sendLogPost(NULL,"reporting/networklog",param.toStyledString(),"gameCommandErrorRecord");
    }
}

bool Utils::createDirectory(string path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    int ret = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    return true;
#else
    BOOL ret = CreateDirectoryA(path, NULL);
    if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
    {
        return false;
    }
    return true;
#endif
    
}

Node* Utils::findNode(Node* node,const string& name)
{
    if(!node)
    {
        return NULL;
    }
    if(node->getName() ==name)
    {
        return node;
    }
    auto& cs = node->getChildren();
    for(auto i : cs)
    {
        auto ret = Utils::findNode(i,name);
        if(ret)
        {
            return ret;
        }
    }
    return NULL;
}

void Utils::scaleAll(Node* node)
{
    auto size = Director::getInstance()->getWinSize();
    auto bgSize = node->getContentSize();
    node->setScaleX(size.width/bgSize.width);
    node->setScaleY(size.height/bgSize.height);
    node->setAnchorPoint(Vec2(0,0));
    node->setPosition(0,0);
}


