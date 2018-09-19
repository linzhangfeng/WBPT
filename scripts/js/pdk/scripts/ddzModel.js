
//游戏阶段
var DDZState ={
    INIT:-1,
    // READY : 0,
    // PREPLAY_ONE:1,
    // PREPLAY_TWO:2,
    // PLAYING:3,
    // END_GAME:4,
    // PIAO:5,
    // TI_LA:6,
    // MING_PAI:7
    READY:0,
    MINGPAI_START_STATE:1,//明牌发牌
    QIANGGUAN_STATE:2,//抢关
    BASE_DOUBLE_STATE:3,//底分加倍
    MINGPAI_PLAY_STATE:4,//明牌打牌
    PLAYING:5,//出牌
    END_GAME:6,//游戏结束


};

var CARDGAMETYPE ={
    QIANGGUAN:1,
    MINGPAIFAPAI:2,
    MINGPAIDAPAI:3,
    FOURWITHTWO:4,
    FOURWITHTHREE:5,
    THREEWITHONE:6,
    PLANEWITHLESS:7,
    BASEDOUBLE:8,
    BOMENOSPILT:9,
};

//游戏操作
var DDZGameType ={
    READY : 0,
    FA_PAI:1,
    PIAO:2,
    KAN_PAI:3,
    JIAO_DZ:4,
    TI_LA:5,
    MING_PAI:6,
    CHU_PAI:7,
    WAIT:8
};

//加倍类型
var DDZRatio ={
    TI_LA : 0,
    MING_PAI:1,
    BOMB:2,
    ROCKET:3,
    SPRING:4,
    TOTAL:5
};
//出牌状态
var  DDZCardState =
{
    NEW_CARD:0,
    FOLLOW_CARD:1,
    PASS_CARD:2,
   LAST_CARD:3
};
var createPacket = function (obj) {
    cc.log(obj)
    var Object = Build.build(obj);
    var packet = new Object;
    return packet;
};

var parsePacket = function (obj, data) {
    var Object = Build.build(obj);
    var packet = Object.decode64(data);
    return packet;
};

var sendPacket = function (packet, cmd) {
    cc.log("netTime______send:"+utils.getTimeStamp()+" _cmd:"+cmd);
    if(packet == null)
    {
        CCTcpClient.getInstance(DDZModel.curRoomID).send_data_pb3("", cmd , DDZModel.curGameType);
    }
    else
    {
        cc.log("___send:"+JSON.stringify(packet) );
        var sendData = packet.encode64();
        CCTcpClient.getInstance(DDZModel.curRoomID).send_data_pb3(sendData, cmd , DDZModel.curGameType);
    }
};
var DDZModel =
{
    //***************通用数据*****************//
    maxPlayer:3,
    chupaiNum:0,
    //是否退出登录
    isLogout:false,
    //联网错误次数
    netErroNum:0,
    //是否等待重连
    waitUpdateInfo:false,
    //心跳时间戳
    lastHearbeatTime:0,

    //当前房间号
    curRoomID:0,
    curGameType:0,

    //玩家信息列表
    players:{},
    //当前操作的座位ID
    curSeatID:-1,
    //我的座位ID
    seatID:-1,
    //未下桌前的座位ID
    unDownSeatID:-1,
    //地主座位ID
    dzSeatID:-1,
    //最后出牌的座位ID
    lastSeatID:-1,
//游戏阶段
    state:-1,
    //是否处于托管状态 （0：否 1：是）
    isTuoguan:-1,
    //房间是否已结束
    isGameOver:false,

    //游戏类型（0:叫分 1:叫抢）
    tableType:0,
    //底注
    coin:0,
    //倍率
    ratios:{},
    //最高倍率
    maxRatios:999999,
    //加倍的倍率
    addRatios:{},

    //叫抢、出牌显示操作时间
    playTime:0,
    //重连等待超时时间
    pauseTime:0,
    //飘时间
    piaoTime:0,
    //踢拉时间
    tilaTime:0,
    //明牌时间
    mingpaiTime:0,
    //可飘底注
    piaoCoins:[],
    mustDz:[],
    hasPiao:0,

    //出牌模式（0：新牌 1：压牌）
    outCardMode:0,
    outCardType:0,

    roomGolden:0,
    isFaPai:false,

    base_money:1,


    pdk_mpCoints:1,
    pdk_ompCoints:1,
    pdk_jiabei:1,
    pdk_qiangguan:1,
    spring_ratio:1,
    bomb_ratio:1,      //爆炸倍率
    max_ratio:1,       //封顶倍率
    bomb_can_split:1, // 0--不可拆 1--可拆

    firstPlayerSeait:1,
    gamePlayType:1,
    table_type:-1,

    comTime:0,
    readyTime:0,

    curRadio:1,
    noOutCardSeait:-1,

    player_option:[],
    nextPlayisSingle:-1,

    sign_rule:-1, //1报单顶大  0放走包赔

    splitcardSeait:-1,  //切牌的位置

    spliceCardstate:false,

    sendDatatime:0,

    split_card_tstamp:0,

    split_card_after_tstamp:0,

    spadeoutcart:0,

    fastShow:false,
    isOnVideo:false,
    curJuShu:0,
    maxJuShu:0,

//*********************动态数据处理************************///
    //重置属性
    reset :function()
    {
        //玩家信息初始化
        for(var p in this.players)
        {
            this.players[p].reset();
        }
        this.curSeatID = -1;
        this.seatID = -1;
        this.dzSeatID = -1;
        this.state =-1;
        this.isTuoguan =-1;
        this.lastSeatID = -1;
        this.isFaPai=false;

        for(var i = 0;i<3;++i)
        {
            this.ratios[i] =0;
        }

        for(var i = 0;i<DDZRatio.TOTAL;++i)
        {
            this.addRatios[i] =0;
        }
        this.playTime = 0;
        this.pauseTime = 0;
        this.piaoTime = 0;
        this.tilaTime = 0;
        this.mingpaiTime = 0;
        this.piaoCoins = [];
        this.maxRatios=999999;


        this.pdk_mpCoints = 1;
        this.pdk_ompCoints = 1;
        this.pdk_jiabei = 1;
        this.pdk_qiangguan = 1;
        this.spring_ratio = 1;
        this.bomb_ratio = 1;
        this.max_ratio = 1;
        this.bomb_can_split = 1;

        this.firstPlayerSeait = -1;
        this.gamePlayType = -1;

        this.comTime = 0;
        this.readyTime = 0;
        this.curRadio = 1;
        this.noOutCardSeait = -1;
        this.player_option = [];
        this.nextPlayisSingle = -1;
        this.sign_rule = -1;
        this.splitcardSeait = -1;
        this.spliceCardstate = false;
        this.sendDatatime = 0;

        this.split_card_tstamp = 0;
        this.split_card_after_tstamp = 0;
        this.chupaiNum =0;

    },
    resetPre :function()
{
    this.isTuoguan =-1;
    this.lastSeatID = -1;
    this.curSeatID = -1;
    this.dzSeatID = -1;
    this.chupaiNum =0;
},

    release:function()
    {
        for(var i in this.players)
        {
            this.players[i].release();
        }
        this.players ={};
    },

    //该uID是否是我
    isMyUID:function(uID)
    {
        return uID == ZJHModel.getInstance().uid;
    },
    //该座位ID是否是我
    isMySeatID:function(seatID)
    {
      return seatID == this.seatID;
    },
    //我是否是地主
    isMyDZ:function()
    {
        return this.dzSeatID == this.seatID;
    },
    //我是否在桌上
    isMyInTable:function()
    {
        return this.seatID!=-1;
    },
    //该座位ID是否是地主
    isDZSeatID:function(seatID)
    {
        return seatID == this.dzSeatID;
    },
    getDzPos:function()
    {
        return this.getPosBySeatID(this.dzSeatID);
    },

    isConnect:function()
    {
        CCTcpClient.getInstance(this.curRoomID).isConnect();
    },
    
    //添加玩家信息
    updatePlayer:function(val)
    {
        var seatid = utils.getValInt(val,"seatid");
        if(!this.players[seatid])
        {
            cc.log("player seatid:"+seatid);
            this.players[seatid] = new PlayerModel();
        }
        var player = this.players[seatid];
        player.reset();
        player.uid = utils.getValInt(val,"uid");
        player.seatid = utils.getValInt(val,"seatid");

        player.ready = utils.getValInt(val,"ready");
        player.betting = utils.getValInt(val,"betting");
        player.see = utils.getValInt(val,"see");
        player.role = utils.getValInt(val,"role");
        player.status = utils.getValInt(val,"status");
        player.sex = utils.getValInt(val,"sex");
        player.exp = utils.getValInt(val,"exp");
        player.rmb = utils.getValInt(val,"rmb");
        player.money = utils.getValInt(val,"money");
        player.coin = utils.getValInt(val,"coin");
        player.total_board = utils.getValInt(val,"total_board");
        player.total_win = utils.getValInt(val,"total_win");
        player.pcount = utils.getValInt(val,"pcount");
        player.vtime = utils.getValInt(val,"vtime");
        player.vlevel = utils.getValInt(val,"vlevel");
        player.matchMoney = utils.getValInt(val,"matchMoney");
        player.dsc = utils.getValStr(val,"dsc");
        player.name = utils.getValStr(val,"name");
        player.avatar = utils.getValStr(val,"avatar");
        player.birthday = utils.getValStr(val,"birthday");
        player.zone = utils.getValStr(val,"zone");

        player.mingpai_start = utils.getValStr(val,"mingpai_start");
        player.base_double = utils.getValStr(val,"base_double");
        player.mingpai_play = utils.getValStr(val,"mingpai_play");
        player.qiangguan = utils.getValStr(val,"qiangguan");

        player.isOffline = 0;
        return player;
    },
    //重置玩家信息
    resetPlayer:function(seatid)
    {
        if(this.players[seatid])
        {
            this.players[seatid].reset();
            return this.players[seatid];
        }
        return null;
    },
    //获取玩家
    getPlayerByUID:function(uid)
    {
        for(var p in this.players)
        {
            if(this.players[p].uid == uid)
            {
                return this.players[p];
            }
        }
        return null;
    },
    getPlayerBySeatID:function(seatid)
    {
        if(this.players[seatid])
        {
            return this.players[seatid];
        }
        return null;
    },
    getPlayerByPos:function(pos)
    {
        var seatid = this.getSeatIDByPos(pos);

        if(this.players[seatid])
        {
            return this.players[seatid];
        }
        cc.log("getPlayerByPos erro:"+seatid);
        return null;
    },
    //座位ID转换当前位置
    getPosBySeatID:function(seatID)
    {
        if(this.seatID == -1)
        {
            if(this.unDownSeatID != -1)
            {
                var pos = seatID - this.unDownSeatID;
                if(pos >=0)
                {
                    return pos;
                }
                return pos+this.maxPlayer;
            }
            return seatID;
        }
        var pos = seatID - this.seatID;
        if(pos >=0)
        {
            return pos;
        }
        return pos+this.maxPlayer;

    },
    //当前位置转换座位ID
    getSeatIDByPos:function(pos)
    {
        if(this.seatID == -1)
        {
            if(this.unDownSeatID != -1)
            {
                var seatID = pos + this.unDownSeatID;
                if(seatID >=this.maxPlayer)
                {
                    seatID =seatID-this.maxPlayer;
                }
                return seatID;
            }
            return pos;
        }
        var seatID = pos + this.seatID;
        if(seatID >=this.maxPlayer)
        {
            seatID =seatID-this.maxPlayer;
        }
        return seatID;

    },

    getRatio:function()
    {
      if(this.seatID > 0)
      {
          return this.ratios[this.seatID];
      }
        return this.ratios[0];
    },
    //更新底注
    updateCoin:function(val)
    {
        if(val["base_money"]!= null)
        {
            this.coin = utils.getValInt(val,"base_money");
            if(this.tableType == 1)
            {
                this.coin = utils.getValInt(val,"base_score");
            }
        }
        else
        {
            this.coin =1;
        }

    },

    //更新倍率
    updateRatio:function(val)
    {
       if(val["ratio"]!=0)
        {
            for(var i=0;i<3;++i)
            {
                this.ratios[i]=utils.getValInt(val,"ratio");
                if(this.isDZSeatID(i))
                {
                    this.ratios[i]*=2;
                }
            }
        }
    },
 //*********************联网协议处理************************///

    //清楚联网请求列表
    resetResponse:function()
    {
        CCTcpClient.getInstance(this.curRoomID).reset_response_queue();
    },

    initNet:function(ip,host)
    {
        CCTcpClient.getInstance(this.curRoomID).setProtoType(4);
        CCTcpClient.getInstance(this.curRoomID).set_host_port(ip,host);
    },
    //连接tcp
    connectNet:function()
    {
        CCTcpClient.getInstance(this.curRoomID).connect();
    },

    //获取联网数据
    getNetData:function()
    {
        return  CCTcpClient.getInstance(this.curRoomID).get();
    },
    //发送TCP联网数据
    sendData:function(d)
    {
        cc.log("___send:"+d);
        CCTcpClient.getInstance(this.curRoomID).sendCodeData(d);
    },


    //发送登录请求
    sendLogin:function()
    {
        var d = createPacket("PGame.LoginReq");
        d["uid"]  =ZJHModel.getInstance().uid;
        d["skey"]  =ZJHModel.getInstance().UserKey;
        d["roomid"]  =""+this.curRoomID;
        sendPacket(d, NetCMD.CLIENT_DZ_LOGIN_REQ);
    },

    //发送登出请求
    sendLogout:function()
    {
        sendPacket(null, NetCMD.CLIENT_LOGOUT_REQ);
    },
    //发送心跳包
    sendHearbeat:function()
    {
        sendPacket(null, NetCMD.CLINET_HEART_BEAT_REQ);
    },
    //发送获取房间信息
    sendRoomInfo:function()
    {
        sendPacket(null, NetCMD.CLIENT_ROOM_INFO_REQ);
    },

    //发送上桌请求
    sendUptable:function()
    {
        sendPacket(null, NetCMD.CLIENT_UPTABLE_APPLY_REQ);
    },

    //发送下桌请求
    sendDowntable:function()
    {
        sendPacket(null, NetCMD.CLIENT_DOWNTABLE_REQ);
    },
    //发送托管请求
    sendTuoguan:function()
    {
    },

    //发送准备请求
    sendReady:function(is)
    {
        var d = createPacket("PGame.ReadyReq");
        d["ready"]  =is;
        sendPacket(d, NetCMD.CLIENT_READY_REQ);
    },

    //发送飘
    sendPiao:function(action)
    {
    },

    //发送叫抢协议
    sendJiaoDZ:function(action)
    {
    },
    //发送明牌
    sendMingPai:function(action)
    {
    },
    //发送出牌
    sendOutCards:function(isOut)
    {
        var d = createPacket("PGame.PlayCardReq");
        d["action"]  =this.outCardMode;
        if(isOut)
        {
            d["card_type"]  =libddz.cardsParse.getCardType(DDZCards.createCards(DDZCards.upCardValues));
            d["cards"]  =DDZCards.upCardValues;
        }
        else
        {
            d["card_type"]  =0;
            d["cards"]  =[];
        }
        sendPacket(d, NetCMD.CLIENT_PLAY_CARD_REQ);
    },
    //发送申请结束房间请求
    sendReqRoomEnd:function(action)
    {
        sendPacket(null, NetCMD.CLIENT_DISSOLVE_ROOM_REQ);
    },
    //发送结束房间投票请求
    sendRoomEnd:function(action)
    {
        var d = createPacket("PGame.VoteDisRoomReq");
        d["action"]  =action;
        sendPacket(d, NetCMD.CLIENT_DISSOLVE_ACTION_REQ);
    },
    //发送表情互动
    sendEmotion:function(pos,faceID)
    {
        var d = createPacket("PGame.EmotionReq");
        d["target_seatid"]  =this.getSeatIDByPos(pos);
        d["type"]  =faceID;
        sendPacket(d, NetCMD.CLIENT_EMOTION_REQ);
    },


     //跑的快
    sendPdkCommonBtns:function(action,atype)
    {
        var d = createPacket("PGame.UserComOptReq");
        d["req_opt"]  =action;
        d["req_type"]  =atype;
        sendPacket(d, NetCMD.CLIENT_COMMON_OPT_REQ);
    },

    sendPdkSplitCard:function(ensure,position)
    {
        // log("=====sendPdkSplitCard======");
        var d = createPacket("PGame.SplitCardReq");
        d["ensure"]  =ensure;
        if(ensure > 1)
        {
            d["position"]  = position;
        }

        var crutime = (new Date()).valueOf();
        if(crutime - this.sendDatatime > 0 )
        {
            sendPacket(d, NetCMD.CLIENT_SPLIT_CARD_REQ);
            this.sendDatatime = (new Date()).valueOf();
        }
    },

    //发送结算请求
    sendOver:function()
    {
        sendPacket(null, NetCMD.CLIENT_PHZ_FINAL_END_REQ);
    },
    sendGetPlayerList:function()
    {
        var  d = packet;
        d.data = {};
        d.data["roomId"]  =this.curRoomID;
        d.data["pageIndex"]  =1;
        d.data["pageSize"]  =20;
        d.data["UserKey"]  =ZJHModel.getInstance().UserKey;


        CCHttpAgent.getInstance().sendHttpPost(function(tag)
            {
                var data = CCHttpAgent.getInstance().getPacketData(tag);
                if (data!="")
                {
                    var recv = null;
                    try{
                        recv = JSON.parse(data);
                        if(recv)
                        {
                            if(recv.status == 200)
                            {
                                if( recv["resultMap"]["onlookersList"]&&recv["resultMap"]["onlookersList"]["result"])
                                {
                                    var num = recv["resultMap"]["onlookersList"]["result"].length;
                                    JsUtils.postNotifiInt("notify_room_player_number", num);
                                }
                            }
                            else
                            {
                                PlatformHelper.showToast(recv["message"]);
                            }
                        }
                    }
                    catch(e)
                    {
                    }
                }
                CCHttpAgent.getInstance().popPackets(tag);
            },
            "duiju/invite/onlookersList", d.toStr(), "room_all_player");
    },
    sendGetJuShu:function()
    {
        var  d = packet;
        d.data = {};
        d.data["roomId"]  =this.curRoomID;

        CCHttpAgent.getInstance().sendHttpPost(function(tag)
            {
                var data = CCHttpAgent.getInstance().getPacketData(tag);
                if (data!="")
                {
                    var recv = null;
                    try{
                        recv = JSON.parse(data);
                        if(recv)
                        {
                            if(recv.status == 200)
                            {
                                if( recv["resultMap"]["way"]!=null)
                                {
                                    var d = recv["resultMap"];
                                    var type = d["way"];
                                    if(type == 1)
                                    {
                                        DDZModel.curJuShu = utils.getValInt(d,"currentCount");
                                        DDZModel.maxJuShu = utils.getValInt(d,"count");
                                    }
                                }
                            }
                        }
                    }
                    catch(e)
                    {
                    }
                }
                CCHttpAgent.getInstance().popPackets(tag);
            },
            "room/dz/status", d.toStr(), "room_status1");
    }

};
