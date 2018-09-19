//
//  ZJHModel.h
//  ZJH
//
//  Created by luochuanting on 13-6-21.
//
//

#ifndef __ZJH__ZJHModel__
#define __ZJH__ZJHModel__

#include <iostream>
#include <string>
#include <map>

#include "ZJH.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "../Scene/BaseLayer.hpp"

using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;

class RoomModel;
class BaseLayer;

class ZJHModel : public Ref
{
public:
    ZJHModel();
    virtual ~ZJHModel();
    /** Return the shared instance **/
    static ZJHModel *getInstance();
    
    int count;
    int init();
    void initPhoneData();
    void GameResum();
    void listnerGameResum(float dt);
    
    const static char* getStr(const char* key);
    
    BaseLayer* gotoView(int id,Json::Value json = Json::Value::null);
    
    void checkUploadMoblieContacts();

    void requestPay(int goodsId, int num, int payType);
    void requestThirdPay(int goodsId, int num);
    void doPay(Json::Value data, int payType);
    
    std::string randomThirdID(bool isReset);
    
    bool isYoukeLogin();
    
    
    void gotoRoom(int id, int code = -1);
    void gotoRoomWithCode(std::string code,bool replaceScenc=true,std::string param="");//恢复场景时需要执行的参数
    void loadingImageResource(ImageView *image, string imageURL);
    
    std::string moneyShowDeal(int value);
    
    bool isCanPay(); //是否调起支付
    

    std::string converPointTwoBit(float value);
    
    string hidePhoneNum(string phone);
    
    void createTestMatchHall(float dt);
    //切换屏幕分辨率 自适应横竖屏切换
    void changeScreenSize(float screenWidth,float screenHeight);
    
    //处理游戏中进入俱乐部竖屏时卡崩的问题
    void delayToHall(float dt);
    
public:
    int GameServer;
    
    int isPause;
    
    __Dictionary* pLuaguaDict;
    
    int uid;
    
    //setting
    double bgMusic;
    double sound;
    int vibrate;
    int musicOn;
    int soundOn;
    
    std::string phoneOs;
    
    std::string thirdId;
    std::string UserKey;
    string vname;
    int vcode;
    std::string channel_id;
    
    Json::Value loginJson;
    Json::Value gameConfigJson;
    Json::Value accountJson;
    
    BaseLayer* curView;
    std::string agentKey;
    
    string phone;
    string pwd;//密码
    bool pwdOK;
    string nick;//昵称
    string signature;//签名
    vector<string> hobby; //爱好
    int vipLv; //用户等级
    string portrit; //用户头像
    int userType; //6:游客
    string userCity; //用户所在地
    string provice; //省
    string city;    //市
    string qu;  //区
    string weiXinBangPhone;
    string NOPwdSetPwd;
    
    string versionIOS;
    
    string phoneimei;
    string phonename;
    
    bool isTrait;
    
    ///begin=====
    ///@todo 房间相关头函数，完成后需要整理
    //my data
//    int uid;
    string gameHelpUrl;
    std::map<int, Json::Value> gameData;
    
    string source_id;
    string user;//用户名
    string name;//昵称
    int    sex;
    string avatar;
    long    money;
    int     rmb;
    int     coin;
    string skey;
    string dsc;//个性签名
    int exp;
    int total_win;
    int total_board;
    int curRoomID;
    int vlevel;
    //座位号
    int mySeatid;
    Vec2 myPoint;
    int myBet;
    
    int pauseTime;
    bool enterGame;
    bool gamePuase;
    bool gamehasinit;
    
    bool checkNoMoney(int value);
    bool checkNoRmb(int value);
    
    void setPhoneAndPwd(std::string phone, std::string pwd);
    void setUserType(int userType);
    bool isOpenIPV6();
    
    string matchEnterName();
    
    string goldCovertStr2(int value);
    
    ///end=======
    
    Json::Value _adPayData;
    
    Json::Value roomData;
    string getRoomData(){return roomData.toStyledString();}
    void setRoomData(std::string data);
    
    int roomUserID;
    
    bool isGameLoad;
    cocos2d::Size latestRunningSize = cocos2d::Size(0,0);//最后一次运行的场景尺寸
    bool isReplace;
    string recoveryHallParam = "";//大厅恢复时的参数
    int curGameType;
    bool isGamePhzFrist;
    
    void roomToHall(bool isFromRoom = true);
    void hallToRoom();
    void toGameHall(int state =0);
    void toMatchHall();
    void toMatchRoom(int gameid, int roomid);
    void toHall();
    void hallToRoomV();
    void hallToRoomH();
    void hotUpdateGame(int gameId);
    
    void gotoGameLoad(bool isinit);
    
    bool isOnVideo;
    std::string roomVideoData;
    
    void gotoVideo(int gid,int rid,std::string data, int pkgId);
    
    void uploadMyLocation();
    
    void roomToShareView(int roomid);

    void reLoginLoadGame(Ref *ref);
    
    void noticeGamePlayEvent(Ref *ref);

    std::string packName;

    void iosPurchaseResult(int type, const char *transactionId, const char *receipt);
    
    void listnerPaySucceed(float dt);
    
    //检查是否有异常退出的并且正在进行的游戏
    void checkGamePlayingError(const MyCallBack_Str &callback);
    
    string IosTransactionId;
    string IosReceipt;
    string appStoreInvoice;
    string iosTotalFee;
    string appleId;
    float iosPrice;

    void dealAppUpdate();
    
    //开启房间心跳  在房间里面则给服务器定时发送一次http请求
    void startRoomHeart();

    void roomHearHttpRequest(float dt);

    long long serverDiffrentTime;//服务器对时时差   每次请求congfig接口的时候获取
    
    int curGameID;
    int curGameHasHall;
    void setCurGameID(int id,int hall);
    bool firtEnterApp;
    bool isNoticeFirstShow;
    bool bootMusicDownload;
    Json::Value gameList;//缓存的游戏列表数据
    Json::Value diquList;//缓存的地区选择列表数据
    Json::Value diquSelfCach;//本地数据存储
    void setDiquCach(int cityId,std::string cityName);
    Json::Value diquGameList;//缓存的本地游戏列表数据
    
    bool gameUseDefaultSetting;
    bool gameUseDefaultChat;
    std::string gameDefaultChat;
    
    std::string phonePlatfm;
    
    Json::Value kefuDataCach;
    
    int iosAppBuySuceesePostTimes;
    
    Json::Value clubSelectGame;//俱乐部当前选择的游戏
    int loadingGameId;//当前正在下载的gameId
    std::map<int, std::string> gameDownloadMap;//已经下载好的游戏
    int club_id = 9905;
    
};
#endif /* defined(__ZJH__ZJHModel__) */
