
//游戏阶段
var CXState ={
     INIT:-1,
     READY:0,       //准备
     GAME_STATE:1,    //游戏开始（设置簸簸，定庄）   
     FIRST_ROUND:2,   //第一轮（发2张牌）
     SECOND_ROUND:3,  //第二圈（发第3张牌）
     THIRD_ROUND:4,   //第三圈（发第四张牌）
     CHEPAI:5,            //扯牌
     SETTLEMENT:6,       // 结算
     GAME_END:7,      //游戏结束

};

var CardTagUM = {
    CHIPPISTART:4000,               // 丢皮

    ALLCARDBEGIN: 5000,             //卡牌开始
    ALLCARDTYPEBEGIN: 5100,         //卡牌牌型

    SENDBETMONEY_BG:6000,           //下注金额背景

    CARDGOLDKENG:5500,              //牌的金框

    CHEPAIFINISH:5700,              //扯牌完成标志

    CHIPSTART:5800,                 //下注额
}

var CXModel =
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

    //游戏阶段
    state:-1,

    //房间是否已结束
    isGameOver:false,

    //底注
    coin:0,

    //重连等待超时时间
    pauseTime:0,

    bet_timer_stamp:0,      //玩家下注时间
    chepai_timer_stamp:0,   //玩家扯牌时间
    cur_bet:0,              //当前的下注
    is_bet_timeout:0,        //玩家下注超时
    is_option_timeout:0,     //设置簸簸或扯牌超时
    is_xiu:0,               //是否开启休
    dealer:0,              //庄家
    left_bet_timer:0,       //剩余的下注时间
    left_chepai_timer:0,    //剩余扯牌时间
    left_ready_timer:0,     //剩余的准备时间
    left_set_bobo_timer:0,  //剩余设置簸簸时间
    max_raise_bet:0,        //最大加注额

    roomGolden:0,
    preready_timer_stamp:0,  //预准备时间
    ready_timer_stamp:0,        //准备时间
    set_bobo_timer_stamp:0,     //玩家设置簸簸时间
    stand_money:0,

    stand_bobo:20,

    isFaPai:false,

    base_money:1,

    myHandCard:[],
    otherPlayCards:{},
    speCardType:[],
    

    //临时
    cur_bet:0,
    max_raidbet:0,
    isxiu:false,

    tobet:0,

    jzView:null,
    boboView:null,
    CmbinationView:null,

    cur_pi:0,

    max_tou_pai_seatid:-1,
    max_wei_pai_seatid:-1,

    cur_gen_bet:0,

    isGameover:false,

    myoptionfinish:false,

    allLastCard:null,

    min_bobo_money:0,  //最小簸簸数

    sendDatatime:0,

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

        this.state =-1;

        this.isFaPai=false;

        this.bet_timer_stamp=0;
        this.chepai_timer_stamp=0; 
        this.cur_bet=0;              
        this.is_bet_timeout=0;        
        this.is_option_timeout=0;     
        this.is_xiu=0;              
        this.dealer=0;              
        this.left_bet_timer=0;
        this.left_chepai_timer=0;
        this.left_ready_timer=0;
        this.left_set_bobo_timer=0;
        this.max_raise_bet=0;

        this.roomGolden=0;
        this.preready_timer_stamp=0;
        this.ready_timer_stamp=0;
        this.set_bobo_timer_stamp=0;
        this.stand_money=0;

        this.stand_bobo=20;

        this.myHandCard = [];

        for(var k=0; k<6; k++)
        {
            this.otherPlayCards[k] = [];
            this.speCardType[k] = -1;
        }

        this.tobet = 0;
        this.cur_pi = 0;

        this.max_tou_pai_seatid = -1;
        this.max_wei_pai_seatid = -1;

        this.cur_gen_bet = 0;
        this.isGameover = false;

        this.myoptionfinish = false;

        this.allLastCard = null;

        this.min_bobo_money = 0;

        this.sendDatatime  = 0;

    },


    resetPre :function()
    {
        this.curSeatID = -1;
    },

    resetView:function()
    {
        this.jzView.setVisible(false);
        this.boboView.setVisible(false);

        CXModel.CmbinationView.setTouchEnable();
        this.CmbinationView.setVisible(false);
    },

    resetData:function()
    {
        this.myHandCard = [];
        this.cur_pi = 0;
        this.cur_gen_bet = 0;

        this.myoptionfinish = false;

        this.max_tou_pai_seatid = -1;
        this.max_wei_pai_seatid = -1;
        
        for(var k=0; k<6; k++)
        {
            this.otherPlayCards[k] = [];
            this.speCardType[k] = -1;
        }
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

    isPlayer_max:function()
    {
        var k = 0;
        var leng = this.players.length;
        for(var i = 0;i < leng;i++)
        {
            if(this.players[i]!= null && this.players[i].seatid != -1)
            {
                k++;
            }
        }
        if(k>6)
        {
            return true;
        }
        return false;
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
        if(!this.players.hasOwnProperty(seatid))
        {
            cc.log("player seatid:"+seatid);
            this.players[seatid] = new PlayerModel();
        }
        var player = this.players[seatid];
        player.reset();
        this.resetPlayMes(seatid);

        player.uid = utils.getValInt(val,"uid");
        player.seatid = utils.getValInt(val,"seatid");

        player.total_bet = utils.getValInt(val,"total_bet");
        player.is_chepai_finish = utils.getValBool(val,"is_chepai_finish");

        player.bet = utils.getValInt(val,"bet");
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
        player.pcount = utils.getValStr(val,"pcount");

        player.bobo_money = utils.getValInt(val,"bobo_money");
        player.issetbobo = utils.getValBool(val,"is_set_bobo_finish"); //是否设置了簸簸

        if(val.hasOwnProperty("cards_name"))
        {
            player.cards_name = utils.getValStr(val,"cards_name"); //特殊牌型
        }else
        {
            player.cards_name = -1;
        }

        if(val.hasOwnProperty("tou_cards_name"))
        {
            player.tou_cards_name = utils.getValStr(val,"tou_cards_name"); //头牌
        }else
        {
            player.tou_cards_name = -1;
        }
        
        if(val.hasOwnProperty("wei_cards_name"))
        {
            player.wei_cards_name = utils.getValStr(val,"wei_cards_name"); //尾牌
        }else
        {
            player.wei_cards_name = -1;
        }

        player.baseGold = utils.getValStr(val,"baseGold"); //底分

        player.isOffline = utils.getValBool(val,"offline");
        return player;
    },
    //重置玩家信息
    resetPlayer:function(seatid)
    {
        if(this.players.hasOwnProperty(seatid))
        {
            this.players[seatid].reset();
            this.resetPlayMes(seatid);
            return this.players[seatid];
        }
        return null;
    },

    resetPlayMes:function(seatid)
    {

        var player = this.players[seatid]
        player.bobo_money = 0;
        player.issetbobo = 0;
        player.baseGold = 0;
        player.total_bet = 0;
        player.is_chepai_finish = false;
        
        player.cards_name = 0;
        player.tou_cards_name = 0;
        player.wei_cards_name = 0;
        player.seatid = 0;
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
                return pos+6;
            }
            return seatID;
        }
        var pos = seatID - this.seatID;
        if(pos >=0)
        {
            return pos;
        }
        return pos+6;

    },
    //当前位置转换座位ID
    getSeatIDByPos:function(pos)
    {
        if(this.seatID == -1)
        {
            if(this.unDownSeatID != -1)
            {
                var seatID = pos + this.unDownSeatID;
                if(seatID >=6)
                {
                    seatID =seatID-6;
                }
                return seatID;
            }
            return pos;
        }
        var seatID = pos + this.seatID;
        if(seatID >=6)
        {
            seatID =seatID-6;
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


    //bobo 加注
    addBobo:function(money)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_SET_BOBO_REQ;
        d.data["bobo_money"] = money;
        this.sendData(d.toStr());
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
        this.sendData(d.toStr());
    },

    //发送登出请求
    sendLogout:function()
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_LOGOUT_REQ;
        this.sendData(d.toStr());
    },
    //发送心跳包
    sendHearbeat:function()
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLINET_HEART_BEAT_REQ;
        d.data["active"]  =0;
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
        this.sendData(d.toStr());
    },

    //发送上桌请求
    sendUptable:function()
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_UPTABLE_APPLY_REQ;
        this.sendData(d.toStr());
    },

    //发送下桌请求
    sendDowntable:function()
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_DOWNTABLE_REQ;
        this.sendData(d.toStr());
    },


    //发送准备请求
    sendReady:function()
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_READY_REQ;
        this.sendData(d.toStr());
    },

    sendBetRequest:function(ac,tobet)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  = NetCMD.CLIENT_BET_REQ;
        d.data["action"]  = ac;
        if(ac == PAYACTION.PLAYER_RAISE)
        {
            d.data["bet"]  = tobet;
        }

        var crutime = (new Date()).valueOf();
        if(crutime - this.sendDatatime > 1000 )
        {
            this.sendData(d.toStr());
            this.sendDatatime = (new Date()).valueOf();
        }
        
    },

    //发送申请结束房间请求
    sendReqRoomEnd:function(action)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_DISSOLVE_ROOM_REQ;
        d.data["uid"]  =ZJHModel.getInstance().uid;
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
        this.sendData(d.toStr());
    },

    sendChePai:function(cards)
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_CHEPAI_REQ;
        d.data["cards"]  =cards;
        this.sendData(d.toStr());
    },    

    CAnaisysCardType:function(cards)
    {
         var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_ANALYSIS_CARDTYPE_REQ;
        d.data["cards"]  =cards;
        this.sendData(d.toStr());
    } ,                                                                                                                                                                                                                                                                             


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
