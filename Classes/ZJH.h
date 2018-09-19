//
//  ZJH.h
//  ZJH
//
//  Created by luochuanting on 13-4-14.
//
//

#ifndef ZJH_ZJH_h
#define ZJH_ZJH_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "json.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

//std::bind(&Class::fun,this);
//[](){}
//[=](){}
//[this](int v){this->fun()};
//[other obj](Node* node){};
//CC_CALLBACK_0(Class:fun,this);
#define CC_CREATE_FUNC(fun)             CallFunc::create([=](){fun;})

typedef std::function<void()> MyCallBack_Null;
typedef std::function<void(std::string v)> MyCallBack_Str;
typedef std::function<void(int v)> MyCallBack_Int;
typedef std::function<void(Node* node)> MyCallBack_Node;
typedef std::function<void(Json::Value _json)> MyCallBack_Json;

enum ROOM_TYPE
{
    ROOM_TYPE_ZJH_DZ = 1001,
    ROOM_TYPE_ZJH_DZ_SET = 2001,
    ROOM_TYPE_DDZ_DZ = 1003,
    ROOM_TYPE_DZ_DZ = 1011,
    ROOM_TYPE_MJ_DZ = 1009,
    ROOM_TYPE_MJ_PH_DZ = 1010,
    
    ROOM_TYPE_MJ_PH_DZ_DZ = 2201,//麻将定制
    ROOM_TYPE_GZMJ_DZ_DZ = 2202,//贵州麻将定制
    ROOM_TYPE_DDZ_DZ_DZ = 2101,//斗地主定制
    ROOM_TYPE_DDZ_DN_DZ = 2301, //斗牛定制
    ROOM_TYPE_DDZ_ZDN_DZ = 2302, //斗牛定制 --有庄
    
    ROOM_TYPE_SCMJ_DZ = 2203,//四川麻将定制
    ROOM_TYPE_GDMJ_DZ = 2204,//广东麻将定制
    ROOM_TYPE_SCMJ2_DZ = 2205,//四川麻将新
    ROOM_TYPE_YNMJ_DZ = 2206,//四川麻将新
};

enum SEX_ID
{
    SEX_MAN = 1,
    SEX_FEMALE = 2,
};

enum TCP_ID
{
    TCP_MSG = 9999,
    TCP_ROOM = 99999,
};

enum MESSAGE_TYPE
{
    MSG_TYPE_TEXT = 1,
    MSG_TYPE_AUDIO = 2,
    MSG_TYPE_IMAGE = 3,
    
    MSG_TYPE_SYSTEM_GAME_IVALITE = 4,//牌局邀请消息
    MSG_TYPE_SYSTEM_GAME_CHANGE = 5,//系统消息
    MSG_TYPE_SYSTEM_ONLINE_INVALITE = 6,//线上房邀请
    MSG_TYPE_SYSTEM_RED_PAPER_RERECEIVE = 7,//收到红包
    MSG_TYPE_SYSTEM_RED_PAPER_OPEND = 8, //红包被打开通知
    MSG_TYPE_SYSTEM_RED_PAPER_SEND = 9, //发送红包
    MSG_TYPE_GIFT = 10,//消息
    MSG_TYPE_SYSTEM_AGAINST_INVITE = 11,//对局房间邀请
    MSG_TYPE_SYSTEM_MEG = -10,
    MSG_TYPE_CLUB_ROOM_CREATE = 12,
    
    SESSION_POINT_MESSAGE = 1,//点对点私人消息会话
    SESSION_TRIBLE_GROUP_MESSAGE = 4,//俱乐部群消息会话
    SESSION_FIND_NEW_FRIENDS = -1, //发现牌友
    SESSION_PAIJU_DONGTAI = 10001, //牌局动态
    SESSION_AGAINST_NOTIFY = 10002, //对局通知
    SESSION_JOIN_TRIBLE_REQUEST = 10003, //部落加入申请
    SESSION_SYSTEM_WEB_MESSAGE = 10004, //网页系统消息
    SESSION_ROOM_CHARG_LOG = 10005,//房卡充值记录
};

/*
 消息分组 点对点 或者 群聊
 */
enum Content_Type
{
    CONTENT_POINT_TO_POINT = 1,
    CONTENT_GROUP = 2,
    CONTENT_CLUB = 4,
    CONTENT_CLUB_SPECAL = 5,//特殊部落消息 主要是服务器的特殊处理  客户端处理同4类型

};

enum FRIEND_TYPE
{
    FRIEND_TYPE_NULL_TO_NULL = 0,
    FRIEND_TYPE_NULL_TO_FRIEND = 1,
    FRIEND_TYPE_FRIEND_TO_NULL = 2,
    FRIEND_TYPE_FRIEND_TO_FRIEND = 3,
    FRIEND_TYPE_GROUP = 4,//俱乐部好友类型
};

enum NOTIFY_TYPE
{
    GAME_CHANGE = 1, // 牌局动态 0
    GAME_INVITED = 2, // 牌局邀请  0
    ADD_FRIEND = 3, // 添加好友  1
    ONLION_INVITE = 5,  //线上房邀请 1
    RED_PAPER_RECEIVE = 6, //收到红包  1
    ROOM_CHANGE_GAME_TYPE = 11, //房间切换玩法
    GOLD_REQUEST = 8,//筹码申请 1
    RED_PAPER_OPEND = 9,  //红包被打开通知 1
    RECHARGE_SUCCESS = 10,//充值成功
    DUOBAO_SUCCESS = 12,//夺宝中奖了
    GROUP_INVITE = 13,//邀请进入群组聊天 2
    ZHIBO_EVENT = 14,//直播事件
    AGAINST_ROOM_INVITE = 15,//对局房间邀请 1
    AGAINST_ROOM_START = 16,//对局房间开始
    AGAINST_NOTIFY = 17,//对局人数满的通知
    JOIN_TRIBLE_REQUEST = 27,//请求加入俱乐部通知
    AGAINST_ROOM_DISS = 22,//解散房间的通知
    ROOM_CARD_CHARG_SUCCESS = 23,//房卡充值成功
    CLUB_TICHU = 24,//被踢出俱乐部
    CLUB_REJECT_JOIN = 25,//被拒绝加入俱乐部
    CLUB_AGREE_JOIN = 26,//被批准加入俱乐部
    MATCH_NOTIFY = 100,//比赛通知
};

enum MSG_CMD
{
    MSG_SEND_REQ = 101,
    MSG_SEND_RES = 102,
    MSG_RECIEVE_MSG = 101,
    MSG_RECIEVE_MSG_RES_REQ = 102,
    MSG_UNREAD_NUM_REQ = 103,
    MSG_UNREAD_NUM_RES = 104,
    MSG_UNREAD_LIST_REQ = 105,
    MSG_UNREAD_LIST_RES = 106,
    MSG_DELETE_UNREAD_REQ = 107,
    MSG_DELETE_UNREAD_RES = 108,
    MSG_JOIN_GROUP_REQ = 111,
    MSG_JOIN_GROUP_RES = 112,
    MSG_RECIEVE_NOTIFY = 113,
    MSG_RECIEVE_NOTIFY_SUCC_REQ = 114,
    MSG_BE_OUT_RES = 115,
    MSG_HEARTBEAT_REQ = 1,
    MSG_HEARTBEAT_RES = 2,
};

enum MSG_STATE
{
    MSG_STATE_SENDING = 0,
    MSG_STATE_FAIL,
    MSG_STATE_SUCC,
};

enum VIEW_ID
{
    VIEW_SHOP = 1,
    VIEW_VIP,
    VIEW_TASK,
    VIEW_SETTING,
    VIEW_MY_FRIENDS,//我的好友列表
    VIEW_ADD_FRIENDS,//添加好友
    VIEW_FOUND_FRIENDS,//发现好友
    VIEW_NEAR_FRIENDS,//附近好友
    VIEW_FRIENDS_INFO,//好友资料
    VIEW_FRIENDS_DT,//好友动态
    VIEW_FRIENDS_ROOMS,//好友房间
    VIEW_FRIENDS_MORE,//好友更多设置
    VIEW_FRIENDS_MORE_NICK,//好友更多设置-修改备注
    VIEW_FRIENDS_MORE_JUBAO,//好友更多设置－举报
    VIEW_MINE_USER_INFO,
    VIEW_HOBBY,
    VIEW_WAN_SHOP,

    VIEW_CHAT_FRIENDS,//附近好友
    VIEW_CHAT_CLUB,//俱乐部聊天
    VIEW_AGAINST_NOTIFY,//对局通知
    VIEW_TRIBLE_JOIN_REQUEST,//部落加入通知
    VIEW_PAIJU_DONGTAI,//牌局动态
    VIEW_ROOM_CHARGE,//房卡充值

    VIEW_VIP_CENTER,// vip中心
    VIEW_TE_QUAN, //特权的web界面
    VIEW_INVITE, //邀请界面
    VIEW_MINE_SETTING,//设置中心界面
    VIEW_LOAD, //登录界面
    VIEW_REGIST, //注册界面
    VIEW_SET_PASSWORD, //设置密码界面
    VIEW_REGIST_FINISH, //注册完成界面
    VIEW_UPDATE_PHONE2, //修改手机号2界面
    VIEW_UPDATE_PASSWORD, //修改密码
    VIEW_FAST_LOAD, //快速登录
    VIEW_FORGET_PASSWORD1, //忘记密码1
    VIEW_FORGET_PASSWORD2, //忘记密码2
    VIEW_MINE_ROOM, //我的房间
    VIEW_MINE_CREAT_ROOM, //创建房间

    VIEW_HOT_LIST,//热门房间
    VIEW_SEARCH_ROOM,//搜索房间

    VIEW_SHOW_IMAGE,

    VIEW_INVITE_FRIEND, //邀请好友的界面
    VIEW_CREATE_MATCH, //创建对局界面
    VIEW_WINRATE, //游戏胜率界面
    VIEW_MATCH_PLAYER, //对局玩家

    VIEW_RECORD_DETIL, //战绩详情

    VIEW_FRIEND_BLACK, //黑名单好友

    VIEW_ER_WEI_MA, //二维码界面
    
    VIEW_MY_PROP, //我的道具界面
    
    VIEW_RICH_LIST, //富豪榜
    
    VIEW_START_FAST, //快速开始
    
    VIEW_MINE_MORE, //我的更多
    
    VIEW_FEEDBACK, //意见反馈
    
    VIEW_ABOUT_OUR, //关于我们
    
    VIEW_SHARE_WANBEI_FRIEND, //分享玩呗好友的界面
    
    VIEW_MY_BULUO, //我的部落界面
    
    VIEW_BULUO_INFO, //部落信息界面
    
    VIEW_BULUO_MANAGER, //部落管理界面
    
    VIEW_BULUO_TUI_GUANG, //部落推广
    
    VIEW_ZUZHANG_BULUO_INFO, //族长部落信息
    
    VIEW_LUXIANG_REVIEW, //回顾录像
    
    VIEW_PHONE_LOAD, //手机登录
    
    VIEW_REAL_NAME_AUTH,//实名认证
    
    VIEW_MATCH_RESULT, //战绩界面
    
    VIEW_MINE_MY_DAILI, //我的代理界面
    
    VIEW_KEFU_CENTER,
    
    VIEW_GAME_PLAY_SET, //俱乐部游戏玩法设置
    
    VIEW_CRITE_MANAGE, //信用管理
    
};

const float DZ_CARD_SCALE = 0.66;
const int Z_ORDER_IN_GAME_LAYER = 100;
const int Z_ORDER_NEW_VIEW = 100;

#define SETTINF_NOTICE_OFF   "noticeOff"
#define SETTINF_NOTICE_BAR_OFF   "noticeBarOff"
#define SETTINF_NOTICE_LOCK_SCREEN   "noticeLockScreenOff"

#define USER_PASSWORD        "userPassWord"
#define USER_PHONE           "userPhone"
#define USER_TYPE           "userType"


#include "PlatformHelper.h"

#include "Utils/Utils.h"
#include "Utils/md5.h"

#include "Net/Queue.h"
#include "Net/Network.h"
#include "Net/JPacket.h"
#include "Net/TcpClient.h"
#include "Net/HttpAgent.h"
#include "Model/ZJHModel.h"
#include "Model/MyNotification.h"
#include "Sound/Sound.h"




///begin=====
///@todo 房间相关头文件，完成后需要把头文件在对应用到的类里面引用，而不是放在ZJH.h这里
#include "Model/PlayerModel.h"
#include "Model/RoomModel.h"

#include "MyGUI/MyButton.h"
#include "MyGUI/MyListView.h"
#include "MyGUI/MySwitch.h"
#include "MyGUI/MyTab.h"
#include "MyGUI/MyPushRod.hpp"
#include "MyGUI/MyPushRod2.hpp"
#include "MyGUI/MyControlSwitch.hpp"
#include "MyGUI/MyTab2.h"
#include "MyGUI/MyDynamicLabel.hpp"
#include "MyGUI/MyDynamicLabelAtlas.hpp"

#include "proto.h"

/* game prop */
enum prop_item
{
    CHANGE_CARD = 3001,  /* change card */
    FORBIDDEN_CARD = 3002, /* forbidden compare card */
    DOUBLE_CARD_FOUR = 3003,    /* compare four multiple bet card */
    DOUBLE_CARD_SIX = 3004,    /* compare six multiple bet card */
    DOUBLE_CARD_EIGHT = 3005,    /* compare eight multiple bet card */
};

//道具类型定义
enum propType
{
    prop_type_rmb = 1,
    prop_type_money = 2,
    prop_type_coin = 3,
    prop_type_huafeika = 4,
    prop_type_sibeika = 7,
    prop_type_jinbika = 5,
    prop_type_huanpaika = 6,
};

struct MsgData
{
    string name;
    int sex;
    int vlevel;
    string content;
    int uid;
    string avatar;
};

enum{
    HINT_RAISE = 0,
    HINT_CALL = 1,
    HINT_FOLD = 2,
    HINT_SEE = 3,
    HINT_CMP = 4,
    HINT_ALLIN = 5,
    HINT_MAX = 6,
};

enum GAME_TYPE
{
    GAME_TYPE_ZJH = 1,
    GAME_TYPE_DZ = 2,
    GAME_TYPE_DN = 3,
};

enum PLAYER_TYPE
{
    //玩家类型
    PLAYER_TYPE_PLAYERCARD = 1,//桌上玩牌玩家
    PLAYER_TYPE_PERTABLE = 2,//外围桌上玩家
    PLAYER_TYPE_BANKER = 3,//庄家
    PLAYER_TYPE_OTHER = 4,//其他玩家（外围不在桌上和不在玩牌）
};

enum WIN_TYPE
{
    //玩家类型
    WIN_TYPE_BIG = 1,//大赢
    WIN_TYPE_SMALL = 2,//小赢
};

enum ALERT_DIALOG_BUTTON
{
    ALERT_DIALOG_BUTTON_CONFIRM = 20,
    ALERT_DIALOG_BUTTON_CANCEL = 21,
    ALERT_DIALOG_BUTTON_CLOSE = 22,
};

enum roomID
{
    ZJH_MaJiang = 1500,
    MJ_Room1,//经典血战场
    MJ_Room2,//快胡场
    MJ_Room3,//
    MJ_TongSha,//麻将通杀场
};


enum RECHARGE_VIEW_TYPE
{
    RECHARGE_VIEW_FORGETPASS = 1,
    RECHARGE_VIEW_REGISTER = 2,
    RECHARGE_VIEW_SETPASSWORD = 3,
    RECHARGE_VIEW_MYFRIEND = 4,
    RECHARGE_VIEW_HISTORY = 5,
    RECHARGE_VIEW_MINE = 6,
    RECHARGE_VIEW_ACTIVITYCENTER = 7,
    RECHARGE_VIEW_SETCENTER = 8,
    RECHARGE_VIEW_MODIFYPASSWORD = 9,
    RECHARGE_VIEW_NOTICEVIEW = 10,
    RECHARGE_VIEW_AGENTVIEW = 11,
    RECHARGE_VIEW_EXTENDVIEW = 12,
    RECHARGE_VIEW_SIGNSET = 13,
    RECHARGE_VIEW_COLLECTION = 14,
};

///end====






#define HALL_BG_MUSIC       "Sound/lob_bg.mp3"
#define ROOM_BG_MUSIC       "Sound/game_bgm.mp3"

//特权的web地址
#if 0

#define TE_QUAN_WEB "http://188.188.1.179:20007/chess-admin/huizhi/vip/info"

#else

#define TE_QUAN_WEB "http://wb.ihuizhi.com/chess-admin/huizhi/vip/info"

#endif


#define BZW_KEY "313f46c8559a33de553f70e3e551fa63"

#define DebugServer 0
#define ZSServer 1



#endif
