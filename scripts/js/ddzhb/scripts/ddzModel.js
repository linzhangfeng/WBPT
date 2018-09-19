
//游戏阶段
var DDZState ={
    INIT:-1,
    READY : 0,
    PREPLAY_ONE:1,
    PREPLAY_TWO:2,
    PLAYING:3,
    END_GAME:4,
    GAIQIANG:5,
    TI_LA:6,
    MING_PAI:7

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
    FOLLOW_CARD:1
};
var DDZModel =
{
    //***************通用数据*****************//

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
    msData:{},
    mustAll :false,
    fastShow:false,
    isOnVideo:false,
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
        this.mustAll = false;
    },
    resetPre :function()
{
    this.isTuoguan =-1;
    this.lastSeatID = -1;
    this.curSeatID = -1;
    this.dzSeatID = -1;
    this.mustAll = false;

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
    //添加玩家信息
    updatePlayer:function(val)
    {
        var seatid = utils.getValInt(val,"seatid");
        if(!this.players.hasOwnProperty(seatid))
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

        player.isOffline = 0;
        return player;
    },
    //重置玩家信息
    resetPlayer:function(seatid)
    {
        if(this.players.hasOwnProperty(seatid))
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
        if(this.players.hasOwnProperty(seatid))
        {
            return this.players[seatid];
        }
        return null;
    },
    getPlayerByPos:function(pos)
    {
        var seatid = this.getSeatIDByPos(pos);

        if(this.players.hasOwnProperty(seatid))
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
                return pos+3;
            }
            return seatID;
        }
        var pos = seatID - this.seatID;
        if(pos >=0)
        {
            return pos;
        }
        return pos+3;

    },
    //当前位置转换座位ID
    getSeatIDByPos:function(pos)
    {
        if(this.seatID == -1)
        {
            if(this.unDownSeatID != -1)
            {
                var seatID = pos + this.unDownSeatID;
                if(seatID >=3)
                {
                    seatID =seatID-3;
                }
                return seatID;
            }
            return pos;
        }
        var seatID = pos + this.seatID;
        if(seatID >=3)
        {
            seatID =seatID-3;
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
        if(val.hasOwnProperty("base_money"))
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
        if(val.hasOwnProperty("seat_ratio"))
        {
            for(var i=0;i<val["seat_ratio"].length;++i)
            {
                var d = val["seat_ratio"][i];
                this.ratios[utils.getValInt(d,"seatid")]=utils.getValInt(d,"ratio");
            }
        }
        else if(val.hasOwnProperty("ratio"))
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


    addCMD:function(cmd)
    {
        if(!this.msData.hasOwnProperty(cmd))
        {
            this.msData[cmd] = {"num":0,"time":0};
        }
        this.msData[cmd].num +=1;
        this.msData[cmd].time = utils.getTimeStamp();
    },
    checkCMD:function(cmd1)
    {
        var cmd = 0;
        switch (cmd1)
        {
            //心跳包
            case NetCMD.SERVER_HEART_BEAT_RESP:
                cmd = NetCMD.CLINET_HEART_BEAT_REQ;
                break;
            //牌桌信息
            case NetCMD.SERVER_TABLE_INFO_UC:
                cmd = NetCMD.CLIENT_ROOM_INFO_REQ;
                break;
            //上桌成功
            case NetCMD.SERVER_UPTABLE_SUCCC_BC:
                cmd = NetCMD.CLIENT_UPTABLE_APPLY_REQ;
                break;
            //上桌失败
            case NetCMD.SERVER_UPTABLE_ERR_UC:
                cmd = NetCMD.CLIENT_UPTABLE_APPLY_REQ;
                break;
            //准备成功
            case NetCMD.SERVER_READY_SUCC_BC:
                cmd = NetCMD.CLIENT_READY_REQ;
                break;
            //准备失败
            case NetCMD.SERVER_READY_ERR_UC:
                cmd = NetCMD.CLIENT_READY_REQ;
                break;
            //盖抢成功
            case NetCMD.SERVER_GAIQIANG_SUCC_UC:
                cmd = NetCMD.CLIENT_GAIQIANG_REQ;
                break;
            //出牌结果
            case NetCMD.SERVER_PLAY_CARD_SUCC_BC:
                cmd = NetCMD.CLIENT_PLAY_CARD_REQ;
                break;
            //出牌失败
            case NetCMD.SERVER_PLAY_CARD_ERR_BC:
                cmd = NetCMD.CLIENT_PLAY_CARD_REQ;
                break;
        }
        if( this.msData.hasOwnProperty(cmd) )
        {
            this.msData[cmd].num -=1;
            var dis = utils.getTimeStamp() - this.msData[cmd].time;
            this.msData[cmd].time =utils.getTimeStamp();
            if(dis>1000)
            {
                var dat = {};
                dat["event"] ="102";
                var d = {};
                d["gameid"] =this.roomData["gameRoom"]["gameId"];
                d["roomid"] = this.curRoomID;
                d["roomCode"] = this.roomData["gameRoom"]["code"];
                d["ms"] = dis;
                d["cmd"] = cmd;
                dat["content"] =JSON.stringify(d);
                JsUtils.postNotifiStr("game_play_event",JSON.stringify(dat));
            }
        }
    },
    clearCMD:function()
    {
        this.msData ={};
    },
    //清楚联网请求列表
    resetResponse:function()
    {
        CCTcpClient.getInstance(this.curRoomID).reset_response_queue();
    },

    initNet:function(ip,host)
    {
        CCTcpClient.getInstance(this.curRoomID).setProtoType(0);
        CCTcpClient.getInstance(this.curRoomID).set_host_port(ip,host);
    },
    //连接tcp
    connectNet:function()
    {
        CCTcpClient.getInstance(this.curRoomID).connect();
    },

    isConnect:function()
    {
        CCTcpClient.getInstance(this.curRoomID).isConnect();
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
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_DZ_LOGIN_REQ;
        d.data["uid"]  =ZJHModel.getInstance().uid;
        d.data["skey"]  =ZJHModel.getInstance().UserKey;
        d.data["roomid"]  =""+this.curRoomID;
       
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },

    //发送登出请求
    sendLogout:function()
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_LOGOUT_REQ;
       
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },
    //发送心跳包
    sendHearbeat:function()
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLINET_HEART_BEAT_REQ;
        d.data["active"]  =0;
       
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },
    //发送获取房间信息
    sendRoomInfo:function()
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_ROOM_INFO_REQ;
        d.data["uid"]  =ZJHModel.getInstance().uid;
        d.data["skey"]  =ZJHModel.getInstance().UserKey;
        d.data["vid"]  =101;

        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },

    //发送上桌请求
    sendUptable:function()
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_UPTABLE_APPLY_REQ;
       
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },

    //发送下桌请求
    sendDowntable:function()
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_DOWNTABLE_REQ;
       
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },
    //发送托管请求
    sendTuoguan:function()
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_ROBOT_REQUEST;
        d.data["robot"]  =this.isTuoguan == 1 ? 0:1;
       
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },

    //发送准备请求
    sendReady:function()
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_READY_REQ;
       
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },

    //发送飘
    sendPiao:function(action)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_USER_PIAO_REQ;
        d.data["piao"]  =action;
        d.data["uid"]  =ZJHModel.getInstance().uid;
       
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },

    //发送叫抢协议
    sendJiaoDZ:function(action)
    {
        var cmd = NetCMD.CLIENT_PREPLAY_ONE_REQ;
        if(this.state == DDZState.PREPLAY_TWO)
        {
            cmd = NetCMD.CLIENT_PREPLAY_TWO_REQ;
        }
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =cmd;
        d.data["action"]  =action;
       
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },
    //发送踢拉
    sendTiLa:function(action)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_TILA_REQ;
        d.data["tila"]  =action;
        d.data["uid"]  =ZJHModel.getInstance().uid;
       
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },
    //发送明牌
    sendMingPai:function(action)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_MINGPAI_REQ;
        d.data["mingpai"]  =action;
        d.data["uid"]  =ZJHModel.getInstance().uid;
       
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },
    //发送出牌
    sendOutCards:function(isOut)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_PLAY_CARD_REQ;
        d.data["action"]  =this.outCardMode;
        if(isOut)
        {
            d.data["card_type"]  =libddz.cardsParse.getCardType(DDZCards.createCards(DDZCards.upCardValues));
            d.data["cards"]  =DDZCards.upCardValues;
        }
        else
        {
            d.data["card_type"]  =0;
            d.data["cards"]  =[];
          //  d.data["cards"][0]  =0;
        }
       
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },
    //发送申请结束房间请求
    sendReqRoomEnd:function(action)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_DISSOLVE_ROOM_REQ;
        d.data["uid"]  =ZJHModel.getInstance().uid;
       
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },
    //发送结束房间投票请求
    sendRoomEnd:function(action)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_DISSOLVE_ACTION_REQ;
        d.data["action"]  =action;
        d.data["uid"]  =ZJHModel.getInstance().uid;
       
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },
    //发送表情互动
    sendEmotion:function(pos,faceID)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_EMOTION_REQ;
        d.data["seatid"]  =this.seatID;
        d.data["target_seatid"]  =this.getSeatIDByPos(pos);
        d.data["type"]  =faceID;
       
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },
    //发送盖抢请求
    sendGaiQiang:function(action)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_GAIQIANG_REQ;
        d.data["uid"]  =ZJHModel.getInstance().uid;
        d.data["gaiqiang"]  =action;
       
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
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
                            if( recv["resultMap"].hasOwnProperty("onlookersList")&&recv["resultMap"]["onlookersList"].hasOwnProperty("result"))
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
    }

};
