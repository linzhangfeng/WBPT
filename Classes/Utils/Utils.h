//
//  Utils.h
//  ZJH
//
//  Created by Apple on 13-11-27.
//
//

#ifndef ZJH_Utils_h
#define ZJH_Utils_h

#include "cocos2d.h"
#include "json.h"
#include <string>
#include "md5.h"
#include "ZJH.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

class Utils
{
public:
static void parseNums(char *mbuf,long nums)
{
    if(nums < 0)
    {
        nums = 0;
    }
    char buf[32];
    std::string s = "";
    long d = nums;
    while (d / 1000 != 0) {
        int n = d % 1000;
        
        d = d / 1000;
        snprintf(buf, sizeof(buf), "%03d", n);
        s = buf + s;
    }
    if(d % 1000 != 0)
    {
        snprintf(buf, sizeof(buf), "%ld", d % 1000);
        s = buf + s;
    }
    
    if (nums == 0)
    {
        snprintf(buf, sizeof(buf), "%d", 0);
        s = buf + s;
    }
    strcpy(mbuf, s.c_str());
}
    
    
static void parseNumsFix(char *mbuf,long nums)
{
    if(nums < 0)
    {
        nums = 0;
    }
    int n = nums;
    char buf[32];
    
    memset(mbuf, 0, strlen(mbuf));
     sprintf(buf, "%d", n);
    sprintf(buf, "%s",buf);
    strcpy(mbuf, buf);
}


static void parseNums1(char *mbuf,long nums)
{
    if(nums < 0)
    {
        nums = 0;
    }
    float n = nums;
    char buf[32];
    
    memset(mbuf, 0, strlen(mbuf));
    if(nums >= 10000  &&nums < 99999999)
    {
        //snprintf(buf, sizeof(buf), "%.2f万", n/10000);
        sprintf(buf, "%.0f", n/10000);
        char temp[10]="万";
        sprintf(buf, "%s%s",buf,temp);
        
    }
    else if(nums <10000){
        sprintf(buf, "%ld", nums);
    }
    else{
        sprintf(buf, "%.2f", n/100000000);
        char temp[10]="亿";
        sprintf(buf, "%s%s",buf,temp);
    }
    strcpy(mbuf, buf);
}

static void parseNums2(char *mbuf,long nums)
{
    if(nums < 0)
    {
        nums = 0;
    }
    float n = nums;
    char buf[32];
    
    memset(mbuf, 0, strlen(mbuf));
    if(nums <10000)
    {
        sprintf(buf, "%ld", nums);
    }
    else if(nums >= 10000  &&nums < 99999999)
    {
        //snprintf(buf, sizeof(buf), "%.2f万", n/10000);
        if(nums /10000<10)
        {
            sprintf(buf, "%.3f", n/10000);
        }
        else if(nums /10000<100)
        {
            sprintf(buf, "%.2f", n/10000);
        }
        else if (nums /10000<1000)
        {
            sprintf(buf, "%.1f", n/10000);
        }
        else{
            sprintf(buf, "%.0f", n/10000);
        }
        
        char temp[10]="万";
        sprintf(buf, "%s%s",buf,temp);
        
    }
    else{
        if(nums /100000000<10)
        {
            sprintf(buf, "%.3f", n/100000000);
        }
        else if(nums /100000000<100)
        {
            sprintf(buf, "%.2f", n/100000000);
        }
        else if (nums /100000000<1000)
        {
            sprintf(buf, "%.1f", n/100000000);
        }
        else{
            sprintf(buf, "%.0f", n/100000000);
        }
        char temp[10]="亿";
        sprintf(buf, "%s%s",buf,temp);
    }
    strcpy(mbuf, buf);
}
   
static void parseNums3(char *mbuf,long nums);
    
static void parseCutNums(int length,char *mbuf,long nums,string replaceStr = ",")
{
    if(nums < 0)
    {
        nums = 0;
    }
    char buf[100];
    
    memset(mbuf, 0, strlen(mbuf));
    sprintf(buf, "%ld",nums);
    string m_pInputNums;
    m_pInputNums.append(buf);
    int len = (int)strlen(m_pInputNums.c_str());
    if(length < len)
    {
        for(int index =(int) len-length; index > 0; index -= length)
        {
            m_pInputNums.insert(index,replaceStr);
        }
    }
    snprintf(buf, sizeof(buf), "%s",m_pInputNums.c_str());
    strcpy(mbuf, buf);
    
}
 
static void parseCutNums1(int length,char *mbuf,float nums,string replaceStr = ",");
    
static void parseName(int maxLength,char *mbuf,const char *text)
{
    string m_pInputText;
    m_pInputText.append(text);
    int count = 0;
    float curL = 0.0;
    
    for(int i = 0; i < strlen(m_pInputText.c_str());i++)
    {
        if((m_pInputText.c_str()[i] >= '0' &&  m_pInputText.c_str()[i] <= '9') || (m_pInputText.c_str()[i] >= 'a' &&  m_pInputText.c_str()[i] <= 'z'))
        {
            count += 1;
            curL += 1;
            if(curL <= maxLength * 2)
            {
                continue;
            }
            else
            {
                if(curL > maxLength * 2)
                {
                    count -= 1;
                }
                break;
            }
        }
        else if((m_pInputText.c_str()[i] >= 'A' &&  m_pInputText.c_str()[i] <= 'Z'))
        {
            count += 1;
            curL += 1.5;
            if(curL <= maxLength * 2)
            {
                continue;
            }
            else
            {
                if(curL > maxLength * 2)
                {
                    count -= 1;
                }
                break;
            }
        }
        else if((m_pInputText.c_str()[i] >= 0 &&  m_pInputText.c_str()[i] <= 127))
        {
            count += 1;
            curL += 1;
            if(curL < maxLength * 2)
            {
                continue;
            }
            else
            {
                if(curL > maxLength * 2)
                {
                    count -= 1;
                }
                break;
            }
        }
        else
        {
            count += 3;
            curL += 2;
            if(curL <= maxLength * 2)
            {
                i+=2;
                continue;
            }
            else
            {
                if(curL > maxLength * 2)
                {
                    count -= 3;
                }
                break;
            }
        }
    }
    
    char buf[count + 1];
    snprintf(buf, sizeof(buf), "%s",m_pInputText.c_str());
    
    if(curL > maxLength * 2)
    {
        char tbuf[count + 1 + 10];
        snprintf(tbuf, sizeof(tbuf), "%s%s",buf,"..");
        strcpy(mbuf, tbuf);
    }
    else
    {
        strcpy(mbuf, buf);
    }
}
    
static void parseName_1(int maxLength,char *mbuf,const char *text)
{
    string m_pInputText;
    m_pInputText.append(text);
    int count = 0;
    float curL = 0.0;
        
    for(int i = 0; i < strlen(m_pInputText.c_str());i++)
    {
        if((m_pInputText.c_str()[i] >= '0' &&  m_pInputText.c_str()[i] <= '9') || (m_pInputText.c_str()[i] >= 'a' &&  m_pInputText.c_str()[i] <= 'z'))
        {
            count += 1;
            curL += 1;
            if(curL <= maxLength * 1.9)
            {
                continue;
            }
            else
            {
                if(curL > maxLength * 1.9)
                {
                    count -= 1;
                }
                break;
            }
        }
        else if((m_pInputText.c_str()[i] >= 'A' &&  m_pInputText.c_str()[i] <= 'Z'))
        {
            count += 1;
            curL += 1.5;
            if(curL <= maxLength * 1.9)
                {
                    continue;
                }
                else
                {
                if(curL > maxLength * 1.9)
                {
                    count -= 1;
                }
                break;
            }
        }
        else if((m_pInputText.c_str()[i] >= 0 &&  m_pInputText.c_str()[i] <= 127))
        {
            count += 1;
            curL += 1;
            if(curL < maxLength * 1.9)
            {
                continue;
            }
            else
            {
                if(curL > maxLength * 1.9)
                {
                    count -= 1;
                }
                break;
            }
        }
        else
        {
            count += 3;
            curL += 2;
            if(curL <= maxLength * 2)
            {
                i+=2;
                continue;
            }
            else
            {
                if(curL > maxLength * 2)
                {
                    count -= 3;
                }
                break;
            }
        }
    }
        
    char buf[count + 1];
    snprintf(buf, sizeof(buf), "%s",m_pInputText.c_str());
        
    if(curL > maxLength * 1.9)
    {
        char tbuf[count + 1 + 10];
        snprintf(tbuf, sizeof(tbuf), "%s%s",buf,"..");
        strcpy(mbuf, tbuf);
    }
    else
    {
        strcpy(mbuf, buf);
    }
}

    
static void WriteFile(string filename,string filedate)
{
    string filepath = getSavePath() + filename;
    char *str1=(char*)filedate.data();
    FILE *fp=fopen(filepath.c_str(), "wb");
    if (fp)
    {
        fwrite(str1, 1, strlen(str1), fp);
        fclose(fp);
    }
}
 
static Json::Value ReadFile(string filename)
{
    Json::Value value_date;
    if(filename.length() > 0)
    {
        std::string data = FileUtils::getInstance()->getStringFromFile(filename.c_str());
        if (data != "")
        {
            Json::Reader read;
            read.parse(data, value_date);
        }
    }
    return value_date;
}
    
static Json::Value ParseJsonStr(string str)
{
    Json::Value value_date;
    if(str.length() > 0)
    {
        if (str != "")
        {
            Json::Reader read;
            read.parse(str, value_date);
        }
    }
    return value_date;
}
    
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
    
static int Random (int a, int b)
{
    return a + rand() % (b - a + 1);
}
    
static Json::Value getJsonValueFromFile(const char *filePath)
{
    Json::Value val;
    Json::Reader reader;
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filePath);
    FILE *fp = fopen(fullPath.c_str(), "rb");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        int len = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        char *buff = (char *)malloc(len);
        fread(buff, len, 1, fp);
        fclose(fp);
        reader.parse(buff, val);
        if (buff) {
            free(buff);
        }
    }
    
    return val;
}

static Sprite* getAvatar(int uid, string avatar,int sex)
{
    if(uid == 0)
    {
        return Sprite::create("Avatars/system.png");
    }
    
    Sprite *pSprite = Sprite::create(getAvatarLocalPath(uid,avatar,sex).c_str());
    if(pSprite == NULL)
    {
        pSprite = Sprite::create(getAvatarLocalPath(uid,"gg.png",sex).c_str());
    }
    return pSprite;
}

static std::string getAvatarUri(int uid)
{
    char buf[128];
    snprintf(buf, sizeof(buf),"image_%d.png",uid);
    string md5str = buf;
    string result;
    MD5::MD5Str(md5str,result);
    snprintf(buf, sizeof(buf),"%c%c/%c%c/image_%d.png",result.c_str()[0],result.c_str()[1],result.c_str()[2],result.c_str()[3],uid);
    result = buf;
    return result;
}

static std::string getAvatarSavePath(int uid)
{
    return getSavePath() + "Avatars/" + getAvatarUri(uid);
}
    
static std::string getSoundSavePath(std::string ab_path)
{
    return getSavePath() + "Sound/" + ab_path;
}
    
static std::string getSoundPathUri(int accountid,int fromUserId,int toid)
{
    return __String::createWithFormat("%d/%d/%d/%lldd_%d.spx",accountid,fromUserId,toid,Utils::getCurrentTime(),Utils::Random(1000, 9999))->getCString();
}
    
static std::string getSavePath()
{
    std::string result = FileUtils::getInstance()->getWritablePath() + "Assets/";
    log("game save path[%s]",result.c_str());
    return result;
}

static  Sprite * CirCleSprite(Sprite *avatar,std::string StencilBg);

static int safeSex(int sex)
{
    if (sex == SEX_MAN)
    {
        return SEX_MAN;
    }
    else
    {
        return SEX_FEMALE;
    }
}
    
static std::string getAvatarLocalPath(int uid, string avatar,int sex)
{
    string path;
    
    if(uid == 0 || uid == -1)
    {
        path = "Avatars/system.png";
        return path;
    }
    
    if(avatar == "gg.png" || avatar == "mm.png")
    {
        if(sex == SEX_MAN)
        {
            path = "Avatars/gg.png";
        }
        else
        {
            path = "Avatars/mm.png";
        }
        return path;
    }
    
    std::string result = "Avatars/" + getAvatarUri(uid);
    return result;
}

static bool hasDownloadAvatar(int uid, string avatar,int sex)
{
    if(avatar == "gg.png" || avatar == "mm.png")
    {
        return true;
    }
    
    if(avatar == "gg.jpg" || avatar == "mm.jpg")
    {
        return true;
    }
    
    Sprite *pSprite = Sprite::create(getAvatarLocalPath(uid,avatar,sex).c_str());
    if(pSprite == NULL)
    {
        return false;
    }
    return true;
}
    
static int CardTypeChange(const char * value)
{
    //全押王牌型转换
    char buf[100];
    string m_pInputText;
    m_pInputText.append(value);
    int ZjhStart[4] =   {0,1,2,3};
    string Dzend[4] = {"d","c","h","s"};
    string ZjhEnd[13] =  {"1","2","3","4","5","6","7","8","9","a","b","c","d"};
    string Dzstart[13] = {"A","2","3","4","5","6","7","8","9","T","J","Q","K"};
    string endstr;
    int startstr;
    for(int i = 0; i < strlen(m_pInputText.c_str());i++)
    {
        
        if ((m_pInputText.c_str()[i] >= 'A' &&  m_pInputText.c_str()[i] <= 'Z') || (m_pInputText.c_str()[i] >= '0' &&  m_pInputText.c_str()[i] <= '9')) {
            for (int j = 0; j<14;j++) {
                if (strcmp(Dzstart[j].c_str(), m_pInputText.substr(0,1).c_str()) ==0)
                {
                    endstr = ZjhEnd[j];
                    break;
                }
            }
        }
        else if(m_pInputText.c_str()[i] >= 'a' &&  m_pInputText.c_str()[i] <= 'z'){
            for (int j = 0; j<4;j++) {
                if (strcmp(Dzend[j].c_str(), m_pInputText.substr(1,2).c_str()) ==0)
                {
                    startstr = ZjhStart[j];
                    break;
                }
            }
        }
        else
        {
            //错误牌型
            endstr ="0";
            startstr =0;
        }
    }
    snprintf(buf, sizeof(buf), "%d%s",startstr,endstr.c_str());
    return HexToDec(buf);
}

static int HexToDec(const char*s)
{
    //16进制转换10进制
    int i,t;
    int sum=0;
    for(i=0;s[i];i++)
    {
        if(s[i]<='9')t=s[i]-'0';
        else  t=s[i]-'a'+10;
        sum=sum*16+t;
    }
    return sum;
}

static Animate* getCCAnimate(std::string buf,int frameNums, int* frame, float perunit, int loop = 0)
{
    Animation *SpriteAni = Animation::create();
    if(loop != 0)SpriteAni->setLoops(loop);
    
    char tempbuf[100];
    for(int i = 0; i < frameNums; i++)
    {
        snprintf(tempbuf, sizeof(tempbuf), buf.c_str(),frame[i]);
        SpriteAni->addSpriteFrameWithFile(tempbuf);
    }
    SpriteAni->setDelayPerUnit(perunit);
    
    return Animate::create(SpriteAni);
}
    
static bool hasImageFile(std::string fileUrl);
static std::string getImageSavePath(std::string fileUrl);
    
static std::string getTmpFoldPath();
    
static std::string getFileName(string path);
    
static Sprite* createDialogBG(int width, int height);

static void sortJson(Json::Value &json)
{
    if (json.size() <= 1)
    {
        return;
    }
    
    for (int i = 0; i < json.size() - 1; i ++)
    {
        for (int j = i + 1; j < json.size(); j ++)
        {
            if (json[i]["sort"].asInt() > json[j]["sort"].asInt())
            {
                Json::Value ijson = json[i];
                json[i] = json[j];
                json[j] = ijson;
            }
        }
    }
}
    
static void sortJsonByKey(Json::Value &json, const char *key)
{
    if (json.size() <= 1) {
        return;
    }
    
    for (int i = 0; i < json.size() - 1; i ++)
    {
        for (int j = i + 1; j < json.size(); j ++)
        {
            if (json[i][key].asInt() > json[j][key].asInt())
            {
                Json::Value ijson = json[i];
                json[i] = json[j];
                json[j] = ijson;
            }
        }
    }
}
    
static float getScaleX()
{
    cocos2d::Size size = Director::getInstance()->getWinSize();
    float sx = size.width / 1080;
    return sx;
}

static float getScaleY()
{
    cocos2d::Size size = Director::getInstance()->getWinSize();
    float sy = size.height / 1920;
    return sy;
}

static float getMaxScale()
{
    cocos2d::Size size = Director::getInstance()->getWinSize();
    float sx = size.width / 1080;
    float sy = size.height / 1920;
    float ss = MAX(sx, sy);
    return ss;
}
    
static float getScaleX_Room()
{
    cocos2d::Size size = Director::getInstance()->getWinSize();
    float sx = size.width / 720;
    return sx;
}

static float getScaleY_Room()
{
    cocos2d::Size size = Director::getInstance()->getWinSize();
    float sy = size.height / 1280;
    return sy;
}

static float getMaxScale_Room()
{
    cocos2d::Size size = Director::getInstance()->getWinSize();
    float sx = size.width / 720;
    float sy = size.height / 1280;
    float ss = MAX(sx, sy);
    return ss;
}
    
    
static float getMaxPHScale_Room()
{
//    cocos2d::Size size = Director::getInstance()->getWinSize();
//    float sx = size.width / 1280;
//    float sy = size.height / 720;
//    float ss = MAX(sx, sy);
    return 1;
}
    
static float getScalePHX_Room()
{
//    cocos2d::Size size = Director::getInstance()->getWinSize();
//    float sx = size.width / 1280;
    return 1;
}
    
static float getScalePHY_Room()
{
//    cocos2d::Size size = Director::getInstance()->getWinSize();
//    float sy = size.height / 720;
    return 1;
}
    
static long long getCurrentTime();
    
static long long getCurrentSecend();
    
static string getMessageTimeFormat(long long time);
    
static std::string getFormatedTime(long long time);

static void makeCircleSprite(Sprite* sp,std::string avatafile,std::string clipfile,cocos2d::Size size);
static Sprite* createCircleAvatar(std::string url,std::string defualtFile,std::string clipfile,cocos2d::Size size);
    
static void getTimeOver(long long time,char *buf);
    
static void saveUseAndPwd(std::string userStr,std::string passwordStr);
    
static Sprite* createMaskedSprite(Sprite* src, const char* maskFile);
    
static Sprite* createCircleImage(std::string url,std::string defualtFile,std::string clipfile, cocos2d::Size size);
    
static ImageView* createNormalImage(std::string url,std::string defualtFile, cocos2d::Size size);
    
static map<string,vector<string>> searchIpSameName(Json::Value list);
static bool isExitGather(vector<int> curlist, vector<vector<int>> &retlist);
static void searchInGather(int index,Json::Value data,vector<int> curlist,vector<vector<int>> &retlist);
static void searchGather(vector<vector<int>> &retlist,Json::Value list);
static void searchGatherWithPath(vector<map<vector<int>,int>> &retlist,Json::Value list,int tipLength);
    
static void gameCommandErrorRecord(const string str);

static void uploadGameCommandRecord();
static void uploadOnce(Json::Value value);
    
static bool createDirectory(string path);
static std::vector<std::string> splitString(std::string str,std::string pattern);
    
static string getFastChatSound(std::string chat);
    
    static Node* findNode(Node*,const string& key);
    static void scaleAll(Node*);
};

#endif
