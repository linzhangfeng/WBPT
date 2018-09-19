
//游戏阶段
var PHZState ={
    GAME_FREE: 0,
    GAME_NIAO:1,
    GAME_PIAO:2,
    GAME_PLAYING:3,
    GAME_END:4
};

//游戏操作类型
var PHZAck ={
    ACK_NULL:0x00,
    ACK_TI:0x01,
    ACK_PAO:0x02,
    ACK_WEI:0x04,
    ACK_CHI:0x08,
    ACK_CHI_EX:0x10,
    ACK_PENG:0x20,
    ACK_HU:0x40,
    ACK_KAN:0x50,
    ACK_JIANG:0x60,
    ACK_CW:0x70,
    ACK_DIAN_PAO:0x71
};
//游戏吃牌类型
var PHZChi ={
    CK_NULL: 0x00,
    CK_XXD:0x01,
    CK_XDD:0x02,
    CK_EQS:0x04,
    CK_LEFT:0x10,
    CK_CENTER:0x20,
    CK_RIGHT:0x40
};
var PHZFan =
{
    MT_ZI_MO : 101,                            //自摸
    MT_HUANG_ZUANG : 102,                            //黄庄
    MT_DIAN_PAO : 103,                            //点炮
    MT_NULL : 0,                            //空
        MT_TIAN_HU: 1,                             //天胡
    MT_TIAN_HU_3TI5KAN_: 100,                             //天胡三提五坎
        MT_DI_HU: 2,                               //地胡
    MT_DI_HU_LIANG: 200,                               //地胡胡亮张
        MT_DIAN_HU: 3,                             //一点红
        MT_SHI_HONG: 4,                            //十红
        MT_HONG_WU: 5,                             //十三红
        MT_HONG_HU: 6,                             //红胡
        MT_WU_HU: 7,                               //乌胡
        MT_DUI_DUI_HU: 8,                          //对对胡
        MT_SHI_BA_DA: 9,                           //十八大（大胡）
        MT_SHI_BA_XIAO: 10,                         //十八小
        MT_SHI_LIU_XIAO: 11,                        //十六小（小胡）
        MT_HAI_DI_HU: 12,                           //海底胡
        MT_SHUA_HOU: 13,                            //耍猴
        MT_ER_BI: 14,                               //二比
        MT_SAN_BI: 15,                              //三比
        MT_SI_BI: 16,                               //四比
        MT_SHUANG_PIAO:17,                         //双飘
    MT_YI_KUAI_BIAN:18,                         //一块匾
    MT_KA_HU:19,                         //卡胡
    MT_ER_SHI_HU:20,                         //二十胡
    MT_SAN_SHI_HU:21,                         //三十胡
    MT_SAN_SHI_HU_SHI_HONG:22                         //三十胡十红
};


//碰偎类型
var PHZPwType ={
    TYPE_NULL: 0,
    TYPE_SAN_DA:1,
    TYPE_SI_QING:2,
    TYPE_WU_FU:3
};

var INVAIL =-1;

var PHZModel =
{

    //***************通用数据*****************//

    hearIndex:0,
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
    //我的座位ID
    seatID:INVAIL,
    //未下桌前的座位ID
    unDownSeatID:INVAIL,
    //地主座位ID
    dzSeatID:INVAIL,
    //游戏阶段
    state:INVAIL,
    //当前操作的座位ID
    curSeatID:INVAIL,
    //最后出牌的座位ID
    lastSeatID:INVAIL,

    //房间是否已结束
    isGameOver:false,
    needOutCard:false,
    neiBetNums:[],
    msData:{},
    soundIndex:2,

    isFaPai:false,
    //结束胡息
    endScore:-1,
    curJuShu:0,
    maxJuShu:0,
    ackAble:false,
    fzUID:-1,
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
        this.curSeatID = INVAIL;
        this.lastSeatID = INVAIL;
        this.seatID = INVAIL;
        this.dzSeatID = INVAIL;
        this.state =PHZState.GAME_FREE;
        this.isFaPai=false;
        this.endScore = -1;
        this.ackAble = false;
    },
    resetPre :function()
    {
        this.curSeatID = INVAIL;
        this.dzSeatID = INVAIL;
        this.state =PHZState.GAME_FREE;
        this.needOutCard=false;
        this.ackAble = false;
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
        return this.seatID!=INVAIL;
    },
    //该座位ID是否是地主
    isDZSeatID:function(seatID)
    {
        return seatID == this.dzSeatID;
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

        player.sex = utils.getValInt(val,"sex");
        player.exp = utils.getValInt(val,"exp");
        player.rmb = utils.getValInt(val,"rmb");
        player.money = utils.getValInt(val,"money");
        player.coin = utils.getValInt(val,"coin");
        player.total_board = utils.getValInt(val,"total_board");
        player.total_win = utils.getValInt(val,"total_win");
        player.pcount = utils.getValInt(val,"pcount");
        player.vlevel = utils.getValInt(val,"vlevel");
        player.name = utils.getValStr(val,"name");
        player.avatar = utils.getValStr(val,"avatar");
        player.ps = utils.getValStr(val,"ps");
        player.score = utils.getValInt(val,"all_score");

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
        if(this.seatID == INVAIL)
        {
            if(this.unDownSeatID != INVAIL)
            {
                var pos = seatID - this.unDownSeatID;
                if(pos >=0)
                {
                    return pos;
                }
                return pos+4;
            }
            return seatID;
        }
        var pos = seatID - this.seatID;
        if(pos >=0)
        {
            return pos;
        }
        return pos+4;

    },
    //当前位置转换座位ID
    getSeatIDByPos:function(pos)
    {
        if(this.seatID == INVAIL)
        {
            if(this.unDownSeatID != INVAIL)
            {
                var seatID = pos + this.unDownSeatID;
                if(seatID >=4)
                {
                    seatID =seatID-4;
                }
                return seatID;
            }
            return pos;
        }
        var seatID = pos + this.seatID;
        if(seatID >=4)
        {
            seatID =seatID-4;
        }
        return seatID;

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
            //牌桌信息
            case NetCMD.SERVER_TABLE_INFO_UC:
                cmd = NetCMD.CLIENT_ROOM_INFO_REQ;
                break;
            //操作
            case NetCMD.SERVER_PHZ_OPERATE_RES:
                cmd = NetCMD.CLIENT_PHZ_OPERATE_CARD_REQ;
                break;
            //出牌
            case NetCMD.SERVER_PHZ_OUT_CARD_RES:
                cmd = NetCMD.CLIENT_PHZ_OUT_CARD_REQ;
                break;
            case NetCMD.SERVER_PHZ_CHANGE_CARD_RES:
                cmd = NetCMD.CLIENT_PHZ_CHANGE_CARD_REQ;
                break;
            case NetCMD.SERVER_PHZ_UPDATE_CARD_RES:
                cmd = NetCMD.CLIENT_PHZ_UPDADE_CARD_REQ;
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
                cc.log("dis cmd:"+cmd+" ms:"+dis);
            }
        }
    },
    clearCMD:function()
    {
        this.msData ={};
    },
    //清除联网请求列表
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
        cc.log("_s_send:"+d);

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
        d.data["index"]  =this.hearIndex;
        d.data["time"]  =""+utils.getTimeStamp();

        this.hearIndex++;
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

    //发送准备请求
    sendReady:function()
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_READY_REQ;
     
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
    //获取玩家列表信息
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
    },
    //发送操作请求
    sendPreAck:function(ack,chi1,chi2,chi3)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_PHZ_PRE_OPERATE_REQ;
        d.data["operate_type"]  =ack;
        if(chi1)
        {
            d.data["chi_kind"]  =chi1;
        }
        if(chi2)
        {
            d.data["second_kind"]  =chi2;
        }
        if(chi3)
        {
            d.data["third_kind"]  =chi3;
        }

        this.sendData(d.toStr());
    },
    //发送操作请求
    sendAck:function(ack,chi1,chi2,chi3)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_PHZ_OPERATE_CARD_REQ;
        d.data["operate_type"]  =ack;
        if(chi1)
        {
            d.data["chi_kind"]  =chi1;
        }
        if(chi2)
        {
            d.data["second_kind"]  =chi2;
        }
        if(chi3)
        {
            d.data["third_kind"]  =chi3;
        }
     
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },
    //发送出牌请求
    sendOutCard:function(card,row,col,jb)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_PHZ_OUT_CARD_REQ;
        d.data["out_card"]  =card;
        d.data["row"]  =row;
        d.data["col"]  =col;
        if(jb)
        {
            d.data["warn"]  =jb;
        }
     
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },
    //发送换牌请求
    sendChangeCard:function(cards)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_PHZ_CHANGE_CARD_REQ;
        d.data["hand_cards"]  =cards;
     
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },
    //发送更新牌型请求
    sendUpdateCard:function()
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_PHZ_UPDADE_CARD_REQ;
     
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },
    //发送打鸟请求
    sendNiao:function(num)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_BETTING_REQ;
        d.data["cout"]  =num;
     
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },
    //发送飘请求
    sendPiao:function(num)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_PHZ_PIAO_REQ;
        d.data["cout"]  =num;
     
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
    },
    //发送结算请求
    sendOver:function()
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_PHZ_FINAL_END_REQ;
     
        this.addCMD( d.data["cmd"]);
        this.sendData(d.toStr());
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
                                if( recv["resultMap"].hasOwnProperty("way"))
                                {
                                    var d = recv["resultMap"];
                                    var type = d["way"];
                                    if(type == 1)
                                    {
                                        PHZModel.curJuShu = utils.getValInt(d,"currentCount");
                                        PHZModel.maxJuShu = utils.getValInt(d,"count");
                                    }
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
            "room/dz/status", d.toStr(), "room_status1");
    }
};