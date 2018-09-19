
var NetCMD =
{
    SERVER_LOGIN_SUCC_UC:4000,
    SERVER_LOGIN_SUCC_BC:4001,
    SERVER_LOGIN_ERR_UC         	 : 4002,
    SERVER_REBIND_UC				 : 4003,
    SERVER_LOGOUT_SUCC_BC			 : 4004,
    SERVER_LOGOUT_ERR_UC			 : 4005,
    SERVER_TABLE_INFO_UC			 : 4006,
    SERVER_READY_SUCC_BC			 : 4007,
    SERVER_READY_ERR_UC				 : 4008,
    SERVER_GAME_START_BC			 : 4009,
    SERVER_NEXT_BET_BC				 : 4010,
    SERVER_BET_SUCC_BC				 : 4011,
    SERVER_BET_SUCC_UC				 : 4012,
    SERVER_BET_ERR_UC			     : 4013,
    SERVER_GAME_END_BC				 : 4014,
    SERVER_GAME_PREREADY_BC			 : 4015,
    SERVER_CHAT_BC					 : 4016,
    SERVER_FACE_BC					 : 4017,
    SERVER_UPDATE_INFO_UC            : 4020,//更新房间金币
    SERVER_EMOTION_BC                : 4021,//大表情广播
    SERVER_PROP_SUCC_UC              : 4022,//道具使用成功返回
    SERVER_PROP_SUCC_BC              : 4023,//道具使用广播
    SERVER_PROP_ERR_UC               : 4024,//道具使用广播
    SERVER_BOX_SUCC_BC               : 4025,//vip开启宝箱的广播
    SERVER_ACTIVE_SUCC               : 4026,
    //SERVER_TWO_CARD_UC               : 4027,
    SERVER_BETTING_SUCC_UC           : 4027,
    SERVER_ACTIVE_UPDATE_BC          : 4028,
    SERVER_UPTABLE_INFO_UC           : 4029,
    SERVER_APPLY_UPTABLE_UC          : 4030,
    SERVER_UPTABLE_SUCCC_BC          : 4031,
    SERVER_AHEAD_DOWNTABLE_UC        : 4032,
    SERVER_DOWNTABLE_SUCC_BC         : 4033,
    SERVER_START_BET_BC              : 4034,
    SERVER_STOP_BET_BC               : 4035,
    SERVER_WIN_UC                    : 4036,
    SERVER_BETTING_ERR_UC            : 4037,
    SERVER_UPTABLE_ERR_UC            : 4038,
    SERVER_RETURN_UC                 : 4039,//收到打赏
    SERVER_REWARD_SUCC_UC            : 4040,//打赏成功

    
    SERVER_DRAW_CARD_BC              : 4600,

    SERVER_DAEMONIZE_SUCC_UC      : 4041,
    SERVER_BANKER_CANDIDATE_UC    : 4042,
    SERVER_COMPARE_INFO_BC        : 4043,
    SERVER_DOWN_BANKER_UC         : 4044,
    SERVER_GIVE_UP_BANKER_BC      : 4045,
    SERVER_GET_BANKER_BC          : 4046,
    SERVER_SHORT_UPTABLE_INFO_BC    :4049,
    SERVER_READY_TIME_BC            :4050,
    SERVER_SELECT_RATIO_UC				 : 4060,//选择倍率
    SERVER_SELECT_CARD_BC			 : 4065,//完成组牌
    SERVER_COMPARE_CARDS_BC			 : 4066,//结算
    // SERVER_COMBINATION_TIP_UC       : 4067,//组牌提示结果
    // SERVER_SECOND_CARD				 : 4068,//第二轮明牌
    // SERVER_THIRD_CARD				 : 4069,//第三轮明牌
    SERVER_START_COUNTDOWN			 : 4084,//普通场的
    SERVER_CHANGE_BANKER			 : 4085,//普通场的
    SERVER_BETS_BC					 : 4087,//玩家倍率选择广播
    SERVER_TREND_UC               : 4100,
    SERVER_ALL_PLAYER_INFO_UC     : 4110,
    SERVER_UP_GUEST_SEAT_ERROR_UC    : 4111,
    SERVER_UP_GUEST_SEAT_SUC_BC   : 4112,
    SERVER_DOWN_GUEST_SEAT_BC     : 4114,
    SERVER_POOL_INFO_UC           : 4115,
    SERVER_POOL_ALTER_BC          : 4116,
    SERVER_COMMUNITY_BC              : 4118,
    SERVER_TWO_COMMUNITY_UC          : 4119,
    SERVER_OTHER_SHOW_BC             : 4121,
    SERVER_GAME_RESULT_BC            : 4123,//十二生肖专属
    SERVER_UPBANKER_ERR_UC           : 4124,
    SERVER_DETAL_INFO_UC             : 4140,//获取外围玩家信息
    SERVER_LOTTERY_SHOW_CARDS        : 4170,

    SERVER_HISTORY_UC                : 4171,

    SERVER_MULTI_OF_LOSE_BC          : 4210,
    SERVER_COPARE_RESULT_FORROBOT    : 4212,

    SERVER_PREPLAY_ONE_BC            : 4214,        /* 开始叫地主 */
    SERVER_PREPLAY_ONE_SUCC_BC       : 4215,        /* 叫地主操作成功 */
    SERVER_PREPLAY_TWO_BC            : 4216,        /* 抢地主 */
    SERVER_PREPLAY_TWO_SUCC_BC       : 4217,        /* 抢地主成功 */
    SERVER_LANDLORD_BC               : 4218,        /* 地主广播 */
    SERVER_PLAY_CARD_BC              : 4219,        /* 玩牌 */
    SERVER_PLAY_CARD_SUCC_BC         : 4220,        /* 玩牌成功 */
    SERVER_PLAY_CARD_ERR_BC          : 4221,        /* 玩牌错误 */
    SERVER_ROBOT_BC                  : 4222,        /* 托管 */
    SERVER_COMFIMATION_TASK_BC       : 4223,        /* 确定底牌和手牌任务 */
    SERVER_TASK_SUCC_BC              : 4224,        /* 有手牌或底牌任务 */
    SEVER_BOOM_BC                    : 4225,        /* 炸弹广播 */
    SEVER_COUNT_CARD                 : 4226,        /* */
    SERVER_GAME_SINGLECARD           : 4227,        /* 最后手牌广播 */
    SERVER_PLAYER_WIN                : 4228,
    SERVER_ROUND_END                 : 4229,
    SEVER_PC_BC                      : 4230,
    SERVER_GAME_START_UC             : 4231,        /* 游戏开始单播，发牌 */
    SERVER_NO_DIZHU_BC               : 4232,


    //斗牛
    SERVER_DEAL_THREE_CARDS_BC       : 4240,        //明三张发牌
    SERVER_DEAL_FOUR_CARDS_UC        : 4241,        //看四张发牌
    SERVER_SET_DEALER_SECC_BC        : 4250,        //设定庄家
    SERVER_BET_TIMEOUT_BC            : 4500,        //下注超时
    SERVER_DEAL_REMAIN_CARDS_UC      : 4243,        //发剩余的牌
    SERVER_SHOW_BROADCASTCARDS_BC    : 4244,        //发剩下的牌广播
    SERVER_SHOW_CARDS_BC             : 4245,        //开牌
    SERVER_ROB_DEALER_BC             : 4247,        //抢庄开始
    SERVER_ROB_DEALER_SUCC_BC        : 4248,        //抢庄成功
    SERVER_PREREADY_BEGIN_BC         : 5001,        /* 弹出小结算 */




    

    CLIENT_ROB_DEALER_REQ            : 1100,        //发送抢

    /* 对战 */
    SERVER_DZ_LOGIN_SUCC_RES        : 4400, /* 对战模式登录成功返回 */
    SERVER_DZ_LOGIN_ERR_RES         : 4401, /* 对战模式登录失败返回 */
    SERVER_DZ_GAME_START_RES        : 7211, /* 对战模式游戏时间到 */
    SERVER_PRE_END_GAME        : 7215, /* 提前结束房间 */


    //麻将游戏协议码
    SERVER_USER_READY	         : 4998,//广播准备
    SERVER_GAME_SCENE            : 4999,//游戏场景
    SERVER_GAME_START            : 5000,
    SERVER_FIRST_HANDCARD		 : 5001,//第一手牌
    SERVER_CHANGE_CARD			 : 5009,//广播换牌
    SERVER_USER_LACK_TYPE 		 : 5002,//广播玩家的定缺色
    SERVER_USER_GETCARD			 : 5003,//摸牌
    SERVER_USER_OUTCARD			 : 5004,//出牌
    SERVER_OPERATE_NOTIFY    	 : 5005,//操作提示
    SERVER_OPERATE_RESULT        : 5006,//广播操作结果
    SERVER_USER_HU				 : 5007,//用户胡牌
    SERVER_GAME_END				 : 5008,//游戏结束
    SERVER_LISTEN_NOTIFY		 : 5010,//碰后有听牌提示
    SERVER_TRUSTEE				 : 5011, //托管
    SERVER_UPDATE_USERINFO	 	 : 5012,//更新玩家信息
    SERVER_MJ_POOL_MONEY		 : 5013,//麻将彩金池总金额
    SERVER_UPDATE_TIMER	 		 : 5013,//玩家超时，广播更新时钟
    SERVER_HEART_BEAT_RESP		 : 5014,//心跳包返回
    SERVER_BUY_HORSE             : 5015,
    SERVER_VIDEO_FIRST_HANDCARD  : 5016,
    SERVER_GET_JIPAI             : 5040,//展示鸡牌（贵州麻将）
    SERVER_BAO_TING_SUCC         : 5041,//报听成功（贵州麻将）
    SERVER_USER_OFFLINE_BC		 : 5020,//广播玩家掉线
    SERVER_OVERTIME_USER		 : 5021,//广播超时没出牌的玩家


    SERVER_START_PIAO_BC            : 5100,        //开始飘
    SERVER_PIAO_SUCC_BC       : 5101,        //飘成功通知
    SERVER_PIAO_ERR_UC       : 5102,        //飘失败

    // SERVER_START_TILA_BC            : 5110,        //开始踢拉
    // SERVER_TILA_SUCC_BC       : 5111,        //踢拉通知

    SERVER_START_MINGPAI_UC            : 5120,        //开始明牌
    // SERVER_MINGPAI_SUCC_BC       : 5121,        //明牌通知
    SERVER_MUST_BE_LANDLORD_BC       : 5129,        //必当地主通知
    SERVER_SERVER_PLAYER_OFFLINE_BC       : 5130,        //玩家掉线通知

    SERVER_APPLY_DISSOLVE_ROOM_ERR_UC : 15000,
    SERVER_APPLY_DISSOLVE_ROOM_SUCC_BC		 : 15001,//发送给其他用户，有玩家请求解散房间
    SERVER_DISSOLVE_ACTION_SUCC_BC		 : 15002,//玩家是否解散房间的选择
    SERVER_DISSOLVE_ROOM_RESULT_BC		 : 15003,//解散房间的结果

    CLIENT_LOGIN_REQ				: 1001,       /* join table */
    CLIENT_LOGOUT_REQ				: 1002,
    CLIENT_READY_REQ				: 1003,	      /* game ready */
    CLIENT_BET_REQ		 			: 1004,
    CLIENT_CHAT_REQ					: 1005,
    CLIENT_FACE_REQ					: 1006,
    CLIENT_SWITCH_ROOM              : 1007,
    CLIENT_ROBOT_REQUEST            : 1008,
    CLIENT_INFO_REQ                 : 1009,         /*update player info*/
    CLIENT_ROOM_INFO_REQ            : 1010,         /*update room info*/
    CLIENT_EMOTION_REQ              : 1011,         /* interaction emotion */
    CLIENT_PROP_REQ                 : 1012,         /* 使用道具 */
    CLIENT_BOX_REQ                  : 1013,         /* 使用道具 */

    CLIENT_ACTIVE_REQ            : 1014,
    CLIENT_BETTING_REQ           : 1015,
    CLIENT_UPTABLE_INFO_REQ      : 1016,
    CLIENT_UPTABLE_APPLY_REQ     : 1017,
    CLIENT_DOWNTABLE_AHEAD_REQ   : 1018,
    CLIENT_DOWNTABLE_REQ         : 1019,
    CLIENT_RETURN_REQ            : 1020,            /*打赏*/

    CLIENT_DAEMONIZE_REQ         : 1021,//清除zid信息

    CLINET_UPBANKER_REQ          : 1022,//申请上庄
    CLINET_UPBANKER_INFO_REQ     : 1023,//获取列表
    CLINET_DOWN_BANKER_REQ       : 1024,//申请下庄

    CLINET_HEART_BEAT_REQ        : 1025,
    CLIENT_FRUIT_BETTING_REQ    :1026,
    CLIENT_FRUIT_ACTIVE_REQ     :1020,
    CLIENT_RED_START_ROB        :1028,//抢红包
    //1027已被机器人占用
    CLIENT_OPEN_RED_REQ : 1024,//打开红包
    CLINET_TREND_REQ             : 1040,
    CLIENT_ALL_PLAYERS_REQ       : 1051,
    CLIENT_UP_GUEST_REQ          : 1052,
    CLIENT_DOWN_GUEST_REQ        : 1053,
    CLIENT_POOL_INFO_REQ         : 1054,
    CLINET_OTHER_SHOW_REQ        : 1056,
    CLIENT_CHOOSE_CARDS_REQ		 : 1057,
    CLIENT_TIP					 : 1059,
    CLIENT_CHOSE_BETS_NUM		 : 1060,
    CLIENT_DETAL_INFO_REQ        : 1070,

    CLIENT_RANK_REQ              : 1080,
    CLIENT_HISTORY_REQ           : 1090,

    CLIENT_PREPLAY_ONE_REQ       : 1091,    /*叫地主*/
    CLIENT_PREPLAY_TWO_REQ       : 1092,    /*抢地主*/
    CLIENT_PLAY_CARD_REQ         : 1093,    /*玩牌*/
    CLIENT_WIN                   : 1094,
    CLIENT_PC                    : 1095,
    CLIENT_ROBOT_NEED_REQ        : 1096,

    /* 对战 */
    CLIENT_DZ_LOGIN_REQ         : 1200,
    //飘
    CLIENT_USER_PIAO_REQ         : 1210,

    CLIENT_SPLIT_CARD_REQ        : 1211,

    CLIENT_GAME_END_REQ          : 1500,

    //踢拉
    CLIENT_TILA_REQ         : 1220,

    //明牌
    CLIENT_MINGPAI_REQ         : 1230,

    CLIENT_DISSOLVE_ROOM_REQ			 : 10000, //玩家请求解散房间
    CLIENT_DISSOLVE_ACTION_REQ		 : 10001, //玩家是否解散房间的选择

    CLIENT_SET_BOBO_REQ              : 1300,


//bobo
    SERVER_SET_BOBO_RES_UC             : 4521, 
    SERVER_OPTION_TIMEOUT_BC           : 4520,
    SERVER_SET_DEALER_BC               : 4522,
    SERVER_FIRST_CARD_UC               : 4067,
    SERVER_SECOND_CARD_BC              : 4068,
    SERVER_THIRD_CARD_BC               : 4069,
    SERVER_CHEPAI_BEGIN_BC             : 4610,
    SERVER_CHEPAI_RESULT_BC            : 4611,
    SERVER_ANALYSIS_RESULT_BC          : 4618,
    SERVER_FIRST_ROUND_UC              : 4525,

    SERVER_CHEPAI_SUCCESS_BC           : 4612,
    SERVER_CHEPAI_FINISH_BC            : 4613,
    SERVER_CHEPAI_TIMEOUT_BC            : 4614,
    SERVER_ALLIN_TCARDS_BC             : 4616,
    SERVER_TESHU_CARDS_BC              : 4619,

    SERVER_DZ_LOGIN_SUCC_BC            : 4800,
    

    CLIENT_CHEPAI_REQ                  : 1301,
    CLIENT_ANALYSIS_CARDTYPE_REQ       : 1308,

    SERVER_CARDS_NAME_UC               : 4070,
    SERVER_OFFLINE_BC                  : 4501,


}

var TCPSendCMD =
{
    CONNECT_OK_RES : 0,
    CONNECT_ERROR_RES:1,
    SEND_DATA_OK_RES:2,
    SEND_DATA_ERROR_RES:3,
    RECV_DATA_OK_RES:4,
    RECV_DATA_ERROR_RES:5,
    DISCONNECT_RES:6,
}

var PAYACTION = 
{
    PLAYER_CALL : 2001,  /* 跟（跟注）*/
    PLAYER_RAISE : 2002, /* 大（加注）*/
    PLAYER_ALLIN : 2003,  /* 敲（全押）*/
    PLAYER_XIU : 2004,  /* 休（过牌）*/
    PLAYER_FOLD : 2005,  /* 丢（弃牌）*/
}

/*
var CARDNAME = new Array
(
    "三花六",
    "三花十",
    "零点",
    "丁七皱",
    "斧九皱",
    "双花十",
    "四六挺",
    "一点",
    "丁八一",
    "五六一",
    "斧十一",
    "长三一",
    "四七一",
    "梅十一",
    "和十一",
    "人十一",
    "地九王",
    "二点",
    "五七二",
    "丁九二",
    "膏药二",
    "二六二",
    "四八二",
    "和十二",
    "人十二",
    "地十二",
    "天十二",
    "三点",
    "五八三",
    "六七三",
    "笤十三",
    "长十三",
    "四九三",
    "梅十三",
    "和十三",
    "人十三",
    "地斧三",
    "天斧三",
    "四点",
    "五九四",
    "双花七",
    "六八四",
    "丁斧四",
    "长十四",
    "梅十四",
    "和十四",
    "人十四",
    "天地公子",
    "五点",
    "七八五",
    "膏药五",
    "猫九五",
    "笤十五",
    "长三五",
    "板斧五",
    "梅十五",
    "和斧五",
    "人十五",
    "地丁五",
    "天丁五",
    "六点",
    "七九六",
    "膏药六",
    "笤十六",
    "斧五六",
    "长三六",
    "梅十六",
    "元宝六",
    "地官六",
    "天官六",
    "七点",
    "非洲七",
    "笤十七",
    "斧猫七",
    "斧长七",
    "丁板七",
    "梅十七",
    "丁和七",
    "苏州七",
    "地官七",
    "天官七",
    "八点",
    "丁五八",
    "笤十八",
    "斧七八",
    "梅十八",
    "和官八",
    "人十八",
    "地官八",
    "天官八",
    "九点",
    "丁猫九",
    "笤十九",
    "乌龙九",
    "丁长九",
    "板五九",
    "梅十九",
    "和官九",
    "人官九",
    "地官九",
    "天官九",
    "地黑杠",
    "地红杠",
    "天黑杠",
    "天红杠",
    "王",
    "四滥",
    "下腰对",
    "中三对",
    "和牌对",
    "人牌对",
    "地牌对",
    "天牌对",
    "丁二皇"
);
*/
