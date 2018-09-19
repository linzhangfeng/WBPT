

var YZDNGameState ={
    E_SHANG_ZHUO :0,
    E_ZHUN_BEI_START:1,
    E_ZHUN_BEI_WAIT:2,
    E_FA_PAI:3,
    E_PIAO_START:4,
    E_PIAO_BC:5,
    E_KAN_PAI:6,
    E_JIAO_DZ_START:7,
    E_JIAO_DZ_BC:8,
    E_QIANG_DZ_START:9,
    E_QIANG_DZ_BC:20,
    E_TI_LA_START:11,
    E_TI_LA_BC:12,
    E_MING_PAI_START:13,
    E_MING_PAI_BC:14,
    E_CHU_PAI_START:15,
    E_CHU_PAI_BC:16,
    E_GAME_OVER:17

    };


var YZDNTable = cc.Layer.extend({
    uiNode:null,
    uiAction:null,
    retNetNum:0,

    testCount:0,
    isresetGame:false,

    Advancefp:true,
    cpGameData:null,

    getUI:function(key)
    {
        return this.uiNode.getChildByName(key);
    },

    ctor:function () {

        this._super();

        cc.spriteFrameCache.addSpriteFrames("yznn/res/img/card.plist");
        cc.spriteFrameCache.addSpriteFrames("public/res/img/bigface.plist");
        cc.spriteFrameCache.addSpriteFrames("yznn/res/img/cardtype.plist");
        cc.spriteFrameCache.addSpriteFrames("yznn/res/img/common.plist");

        var b1 = (new Date).valueOf();
        this.uiJson = ccs.load(PackName+"/res/table.json");
        this.uiNode =this.uiJson.node;
        this.addChild(this.uiNode);

        // cc.log("tib1:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();

        // cc.log("tib2:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();
       
        YZDNModel.reset();

        //初始化数据模型
        this.initView();
        YZDNModel.roomData = JSON.parse( ZJHModel.getInstance().getRoomData());
        YZDNModel.curRoomID = ZJHModel.getInstance().curRoomID;

        // cc.log("tib3:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();
        //监听联网状态

        Sound.getInstance().preLoadEffect("js/"+PackName+"/res/sound");
        Sound.getInstance().PlayBgSound(PackName+"/res/sound/game_bgm.mp3");


        YZDNModel.isOnVideo = ZJHModel.getInstance().isOnVideo;
        if ( YZDNModel.isOnVideo ) {
            this.videoBox = new YZDNVideo();
            this.addChild(this.videoBox,111111);
            this.videoBox.runScript(this);
        }
        else {
            this.init();
        }

        return true;
    },

    init:function()
    {


         this.schedule(this.updateGame,0.0);
        this.schedule(this.hearbeat,3.0);

        cc.log("roomID:"+ YZDNModel.curRoomID );

        YZDNPlayers.publicBox = RoomPublic.create();
        if (ZJHModel.getInstance().vcode > 20050){
            YZDNPlayers.publicBox.setRequestFlag(false);
        }
        YZDNPlayers.publicBox.setCallback(function(tag)
        {
            if(tag == "exit")
            {
                YZDNModel.sendDowntable();
                YZDNModel.sendLogout();
            }
            else if(tag == "end_room")
            {
                YZDNModel.sendReqRoomEnd();
            }
            else if(tag == "down_table")
            {
                YZDNModel.sendDowntable();
            }
            else if(tag == "seatBtn")
            {
                YZDNModel.sendUptable();
            }
        });

        this.addChild(YZDNPlayers.publicBox,10000);

        //部分事件监听
        var self = this;
        this._listenerGameOver = cc.EventListener.create({
            event:cc.EventListener.CUSTOM,
            eventName:"notify_against_room_end",
            callback:function(evt){
                self.onGameOver();
                cc.log("notify_against_room_end");
            }
        });
        cc.eventManager.addListener(this._listenerGameOver,this);

        this._listenerExit = cc.EventListener.create({
            event:cc.EventListener.CUSTOM,
            eventName:"will_exit_room",
            callback:function(evt){
                self.unscheduleAllCallbacks();

                YZDNModel.release();
                YZDNPlayers.stopAllPlayTimer();
                YZDNTips.stopTipTimer();


                ccs.ArmatureDataManager.destroyInstance();
                cc.spriteFrameCache.removeSpriteFrames();
                cc.textureCache.removeAllTextures();
                jsb.fileUtils.purgeCachedEntries();
            }
        });
        cc.eventManager.addListener(this._listenerExit,this);

        YZDNModel.initNet( YZDNModel.roomData["gameRoom"]["way"]["ip"],Number(YZDNModel.roomData["gameRoom"]["way"]["port"]));
        YZDNModel.connectNet();


        //==notify_next_game_start
        this._listenerNext = cc.EventListener.create({
            event:cc.EventListener.CUSTOM,
            eventName:"notify_next_game_start",
            callback:function(evt){
                self.WaiteNextBureau();
                cc.log("notify_next_game_start");
            }
        });
        cc.eventManager.addListener(this._listenerNext,this);


        var _listenerHome2 = cc.EventListener.create({
                event: cc.EventListener.CUSTOM,
                eventName: "chat_sound_play_over_notify1",
                callback: function (evt) {
                    ZJHModel.getInstance().isPause = 0;
                }
            });
        cc.eventManager.addListener(_listenerHome2, this);

        // this.schedule(this.test,1.0);
    },


    test:function()
    {
        this.testCount++;
        YZDNModel.seatID = 0;
        

        if(this.testCount == 1)
        {
            JsUtils.postNotifi("notify_game_check_end");
            YZDNAction.hideOutCardBtns();


            YZDNPlayers.resetPlayerposition();
            for(var i =0;i<5;i++)
            {
                var player = YZDNPlayers.players[i];
                player.seatid = i;
                player.money = 1000;
                YZDNPlayers.uptable(player);
            }
        }
        if(this.testCount == 3)
        {

            for(var k = 0;k<5;k++)
            {
                for(var j = 0;j<3;j++)
                {
                    YZDNModel.otherPlayCards[k].push(5);
                }
                
            }
            
            YZDNCards.fapaibegin();
            YZDNCards.resetCard();

        }else if(this.testCount == 5)
        {
            var delay = 0.2;
            for(var i =0;i<5;i++)
            {
                var pos = YZDNModel.getPosBySeatID(i);
                for(var j = 0;j<3;j++)
                {
                    delay += 0.05;
                    YZDNCards.showCards(pos,j,delay);
                }

            }
        }else if(this.testCount == 7)
        {
            var delay = 0.2;
            for(var i =0;i<5;i++)
            {
                delay += 0.15;
                var pos = YZDNModel.getPosBySeatID(i);
                
                YZDNCards.showCards(pos,3,delay);
            }
        }
        else if(this.testCount == 9)
        {
            var delay = 0.2;
            for(var i =0;i<5;i++)
            {
                delay += 0.15;
                var pos = YZDNModel.getPosBySeatID(i);
                YZDNCards.showCardValue(pos,3,delay,5);
            }
        }
        else if(this.testCount == 10)
        {
            var delay = 0.2;
            for(var i =0;i<5;i++)
            {
                delay += 0.15;
                var pos = YZDNModel.getPosBySeatID(i);
                YZDNCards.showCards(pos,4,delay);
            }
        }
        else if(this.testCount == 12)
        {
            var delay = 0.2;
            for(var i =0;i<5;i++)
            {
                delay += 0.25;
                var pos = YZDNModel.getPosBySeatID(i);
                YZDNCards.showCardValue(pos,4,delay,5);
            }
        }
        else if(this.testCount == 15)
        {
            YZDNCards.fapaiend();
        }

        else if(this.testCount == 20)
        {
            for(var i =0;i<5;i++)
            {
                var pos = YZDNModel.getPosBySeatID(i);
                YZDNChips.sendBetMoney(pos,50,6);
            }
        }

        else if(this.testCount == 22)
        {
            YZDNAction.showOutCardBtns();
        }
       
    },



    onEnter:function()
    {
        this._super();
    },
    onExit:function()
    {
        this._super();
    },
    //初始化界面
    initView:function()
    {
        var b1 = (new Date).valueOf();
        YZDNAction.uiNode =  this.getUI("btns");
        YZDNAction.uiNode.setLocalZOrder(99);
        YZDNAction.clickCB = this.onBtnClickEvt;
        YZDNAction.init();

        // cc.log("ti1:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();

        YZDNCards.uiNode =  this.getUI("cards");
        YZDNCards.uiNode.setLocalZOrder(101);
        YZDNCards.init();
        // cc.log("ti2:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();

        YZDNChips.uiNode =  this.getUI("chip");
        YZDNChips.uiNode.setLocalZOrder(102);
        YZDNChips.init();

        YZDNPlayers.uiNode =  this.getUI("players");
        YZDNPlayers.init();

        // cc.log("ti3:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();
        YZDNTips.uiNode =  this.getUI("tips");
        YZDNTips.uiNode.setLocalZOrder(103);
        YZDNTips.init();

        // cc.log("ti4:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();
    },
    //心跳处理
    hearbeat:function( dt) {
        YZDNModel.sendHearbeat();
    },
    //联网监听
    updateGame:function( dt)
    {
        var curTime = (new Date).valueOf()/1000;


        if( YZDNModel.reconnectTime!=-1)
            {
                YZDNModel.reconnectTime +=dt;

                if(YZDNModel.reconnectTime >3)
                {
                    YZDNModel.changeIP();
                    YZDNModel.connectNet();
                    return;
                }
            }

        //如果游戏退出到后台，返回前台时
        if(ZJHModel.getInstance().gamePuase)
        {
            ZJHModel.getInstance().gamePuase =false;
            if(curTime- ZJHModel.getInstance().pauseTime <=3)
            {
                return;
            }
            YZDNModel.waitUpdateInfo = true;

            if( YZDNModel.isConnect())
            {
                YZDNModel.resetResponse();
                YZDNModel.sendRoomInfo();

            }else{

                YZDNModel.connectNet();
            }

            return;
        }
        //心跳包超时
        if(YZDNModel.lastHearbeatTime != 0 && (curTime - YZDNModel.lastHearbeatTime ) > 10)
        {
            YZDNModel.connectNet();

            YZDNModel.lastHearbeatTime = curTime;
            if( this.retNetNum >0)
            {
                YZDNTips.showAllTip("网络连接异常，正在重试");
            }
            this.retNetNum ++;
            return;
        }
        var response = YZDNModel.getNetData();
        if(response != null)
        {
            if(response.cmd ==TCPSendCMD.DISCONNECT_RES || response.cmd ==TCPSendCMD.CONNECT_ERROR_RES
                || response.cmd ==TCPSendCMD.SEND_DATA_ERROR_RES|| response.cmd ==TCPSendCMD.RECV_DATA_ERROR_RES)
            {
                    cc.log("tcp error:"+response.cmd);
                    if(!YZDNModel.isLogout)
                    {
                        YZDNModel.connectNet();
                    }

                    if (YZDNModel.lastHearbeatTime == 0)
                    {
                        YZDNModel.lastHearbeatTime = utils.getTimeStamp();
                    }


            }
            else if( response.cmd ==TCPSendCMD.CONNECT_OK_RES)
            {
                cc.log("tcp connect succ!/n cmd:"+response.cmd +" data:"+response.data );
                YZDNModel.reconnectTime = -1;
                YZDNModel.sendLogin();
            }
            else if( response.cmd ==TCPSendCMD.RECV_DATA_OK_RES)
            {
                 var  d = response.parseDatta();
                 this.handlerCMD(d,true);
            }
            response.release();
        }

    },

    //重置游戏信息
    resetGame:function()
    {

        YZDNModel.resetData();//重置数据

        if(this.isResetGame)
        {
            return;
        }

        this.isResetGame = true;

        this.Advancefp = true;
        YZDNAction.reset();

        YZDNCards.reset();
        YZDNCards.resetCard();              //重置牌位置

        YZDNTips.reset();

        YZDNPlayers.reset(null);
        YZDNPlayers.resetPlayerposition();  //重置座位的
        YZDNPlayers.stopAllPlayTimer();

        YZDNChips.reset();

        YZDNModel.resetPre();

        //我的id
        var player = YZDNModel.getPlayerBySeatID(YZDNModel.seatID);
       if(player)
       {
           ZJHModel.getInstance().rmb = player.rmb;
           ZJHModel.getInstance().money = player.money;
           ZJHModel.getInstance().coin = player.coin;
       }

    },

    onGameOver: function () {
        YZDNModel.isGameOver = true;
      if(YZDNModel.state == YZDNState.READY||YZDNModel.state == YZDNState.GAME_END)
      {
          var self  =this;
          // this.stopGameOver();
          var action = cc.sequence(cc.delayTime(7),cc.callFunc(function(){
              ZJHModel.getInstance().mySeatid = -1;
              YZDNModel.isGameOver = true;
              // ZJHModel.getInstance().roomToShareView(YZDNModel.curRoomID);
              YZDNModel.sendOver();
          }));
          action.setTag(10086);
          this.runAction(action);
      }
    },

    stopGameOver:function()
    {
        this.stopActionByTag(10086);
    },

    showEndRoom:function(val)
    {
        if(!YZDNModel.isMyInTable())
        {
            return;
        }
        var player = YZDNModel.getPlayerByUID(val["uid"]);
        if(!player)
        {
            return;
        }
        var json ={};
        json["ownerid"] = player.uid;
        json["ownername"] = player.name;
        json["time_left"] = val["remain_time"];

        var isCan=ZJHModel.getInstance().uid == player.uid;
        json["players"] = [];
        if(val.hasOwnProperty("voters_uid"))
        {
            for (var i = 0; i < val["voters_uid"].length; i ++)
            {
                var d = {};
                var p = YZDNModel.getPlayerByUID(val["voters_uid"][i]);
                d["uid"] = p.uid;
                d["name"] = p.name;
                d["avatar"] = p.avatar;
                d["state"] = 0;

                json["players"].push(d);
                if(ZJHModel.getInstance().uid == p.uid)
                {
                    isCan = true;
                }
            }
        }
        if(!isCan)
        {
            return;
        }
        if(this.roomEndBox)
        {
            this.roomEndBox.removeFromParent(true);
        }
        // this.roomEndBox = RoomEndTouPiao.create();
        this.roomEndBox = RoomHEndTouPiao.create();
        if (ZJHModel.getInstance().vcode > 20050){
            this.roomEndBox.setDefaultTips("若您超时未选择，则系统默认为\"反对解散\"");
        }
        this.roomEndBox.initWithJsonStr(JSON.stringify(json));
        this. addChild(this.roomEndBox,10000);

        this.roomEndBox.setCallback(function (str){
            if (str == "ok")
            {
                YZDNModel.sendRoomEnd(1);
            }
            else if (str == "no")
            {
                YZDNModel.sendRoomEnd(0);
            }
        });
    },
//*************************** 按钮点击发送服务器处理 ************************************//
    //点击事件处理
    onBtnClickEvt:function(key)
    {
        cc.log("onBtn:"+key);
        switch(key)
        {
            case "btn_sz":
                Sound.getInstance().playEffect(PackName+"/res/sound/com/button.mp3");
                YZDNModel.sendUptable();
                break;
            case "btn_ready":
                Sound.getInstance().playEffect(PackName+"/res/sound/com/button.mp3");
                YZDNModel.sendReady();
                break;
            case "btnq_bq":
                YZDNModel.sendQiangRequest(0);
                break;
            case "btnq_one":
                YZDNModel.sendQiangRequest(1);
                break;
            case "btnq_two":
                YZDNModel.sendQiangRequest(2);
                break;
            case "btnq_three":
                YZDNModel.sendQiangRequest(3);
                break;
            case "btnq_four":
                YZDNModel.sendQiangRequest(4);
                break;
            case "btn_one":
                {
                    YZDNModel.sendBetRequest(YZDNModel.betRadio[0]); 
                }
                break;
            case "btn_two":
                {
                    YZDNModel.sendBetRequest(YZDNModel.betRadio[1]);
                }
                break;
            case "btn_three":
                {
                    YZDNModel.sendBetRequest(YZDNModel.betRadio[2]);
                }
                break;
            case "btn_four":
                {
                    YZDNModel.sendBetRequest(YZDNModel.betRadio[3]);
                }
                break;
            case "btn_five":
                {
                    YZDNModel.sendBetRequest(YZDNModel.betRadio[4]);
                }
                break;
        }
    },

 //*************************** 服务器返回信息处理 ************************************//
    //处理联网回调
    handlerCMD:function(data,isDelay)
    {
        var val;
        try{
            val = JSON.parse(data);
        }
        catch(e)
        {
            cc.log("_error__data:"+data);
            return;
        }

        var cmd = val.cmd;
        cc.log("___cmd:"+cmd);
        cc.log("___data:"+data);

        YZDNModel.fastShow = !isDelay;

        if(NetCMD.SERVER_DZ_LOGIN_ERR_RES == cmd)
        {
            ZJHModel.getInstance().mySeatid = -1;
            YZDNModel.isGameOver = true;
            ZJHModel.getInstance().roomToShareView(YZDNModel.curRoomID);
            return;
        }

        if(YZDNModel.isGameover)
        {
            return;
        }

        if(YZDNModel.waitUpdateInfo && cmd !=NetCMD.SERVER_TABLE_INFO_UC)
        {
            return;
        }

        switch (cmd)
        {
            //心跳包
            case NetCMD.SERVER_HEART_BEAT_RESP:
                cc.log("SERVER_HEART_BEAT_RESP");
                YZDNModel.lastHearbeatTime = (new Date).valueOf()/1000;
                break;
            //牌桌信息
            case NetCMD.SERVER_TABLE_INFO_UC:
                this.handlerTableInfoUC(val)
                break;
            //登录成功
            case NetCMD.SERVER_LOGIN_SUCC_UC:
                cc.log("登录成功");
                break;
            //登录成功
            case NetCMD.SERVER_LOGIN_SUCC_BC:
                this.handlerLoginSuccBC(val)
                break;
            //登录失败
            case NetCMD.SERVER_LOGIN_ERR_UC:
                cc.log("SERVER_LOGIN_ERR_UC");
                break;
            //登出成功
            case NetCMD.SERVER_LOGOUT_SUCC_BC:
                cc.log("SERVER_LOGOUT_SUCC_BC");
                YZDNModel.isLogout = true;
                ZJHModel.getInstance().mySeatid = -1;
                YZDNModel.isGameOver = true;
                break;


                //明三张发牌
            case NetCMD.SERVER_DEAL_THREE_CARDS_BC:
                this.DisplayThreeCard(val);
                break;
                //暗四张大牌
            case NetCMD.SERVER_DEAL_FOUR_CARDS_UC:
                this.HideFourCard(val);
                break;
                //设定庄家
            case NetCMD.SERVER_SET_DEALER_SECC_BC:
                this.setZang(val);
                break;
            case NetCMD.SERVER_DEAL_REMAIN_CARDS_UC:
                this.ShowSurplusCards(val);
                break;
            case NetCMD.SERVER_SHOW_BROADCASTCARDS_BC:
                this.ShowSurplusCards_broadcast(val);
                break;
            case NetCMD.SERVER_NEXT_BET_BC:
                this.showNext_bet_bc(val);
                break;
            case NetCMD.SERVER_BET_SUCC_BC:
                this.sendBetSucc_bc(val);
                break;
            case NetCMD.SERVER_ROB_DEALER_BC:  //广播开始抢
                this.qiangBtnShow(val);
                break;
            case NetCMD.SERVER_SHOW_CARDS_BC:
                this.getSpellCards(val);
                break;
            case NetCMD.SERVER_ROB_DEALER_SUCC_BC:
                this.RobSuccess(val);
                break;
            case NetCMD.SERVER_PREREADY_BEGIN_BC:
                this.handlerPreready_Begin(val);
                break;







            //准备启动倒计时
            case NetCMD.SERVER_READY_TIME_BC:
                this.gameReadyStart(val);
                break;
            case NetCMD.SERVER_DRAW_CARD_BC:
                cc.log("SERVER_DRAW_CARD_BC");
                break;
            case NetCMD.SERVER_SET_BOBO_RES_UC:
                this.setBoboSuc(val);
                break;
            case NetCMD.SERVER_ANALYSIS_RESULT_BC:
                this.AniSysCardType(val);
                break;

            case NetCMD.SERVER_BET_TIMEOUT_BC:  //下注超时
                this.BetTimeOut(val);
                break;
   
            case NetCMD.SERVER_DZ_LOGIN_SUCC_BC:
                this.handlerLoginSuccBC(val);
                break;

            case NetCMD.SERVER_OFFLINE_BC:
                this.handler_server_offline_bc(val);
                break;
 
            case NetCMD.SERVER_FIRST_ROUND_UC:
                this.First_Round_Bc(val);
                break;
            //上桌成功
            case NetCMD.SERVER_UPTABLE_SUCCC_BC:
                this.handlerUptableSuccBC(val);
                break;
            //上桌失败
            case NetCMD.SERVER_UPTABLE_ERR_UC:
                this.handlerUptableErroUC(val);
                break;
            //下桌成功
            case NetCMD.SERVER_DOWNTABLE_SUCC_BC:
                this.handlerDowntableSuccBC(val);
                break;
            //准备成功
            case NetCMD.SERVER_READY_SUCC_BC:
                this.handlerReadySuccBC(val);
                break;
            //准备失败
            case NetCMD.SERVER_READY_ERR_UC:
                this.handlerReadyErroUC(val);
                break;
            //准备对局
            case NetCMD.SERVER_GAME_PREREADY_BC:
                this.handlerGamePrereadyBC(val);
                break;
            //开始发牌
            case NetCMD.SERVER_GAME_START_BC:
                this.handlerGameStartBC(val);
                break;

            //看牌
            case NetCMD.SERVER_GAME_START_UC:
                // this.handlerGameStartUC(val);
                break;
            //开始明牌
            case NetCMD.SERVER_START_MINGPAI_UC:
                this.handlerStartMingPaiUC(val);
                break;
            //进入出牌
            case NetCMD.SERVER_PLAY_CARD_BC:
                this.handlerStartOutCardBC(val);
                break;
            //出牌结果
            case NetCMD.SERVER_PLAY_CARD_SUCC_BC:
                this.handlerOutCardSuccBC(val);
                break;
            //出牌失败
            case NetCMD.SERVER_PLAY_CARD_ERR_BC:
                cc.log("SERVER_PLAY_CARD_ERR_BC");
                break;
            //对局结束，进行结算
            case NetCMD.SERVER_GAME_END_BC:
                this.handlerGameEndBC(val);
                break;
            //玩家掉线状态
            case NetCMD.SERVER_SERVER_PLAYER_OFFLINE_BC:
                this.handlerPlayerOfflineBC(val);
                break;
            //表情互动
            case NetCMD.SERVER_EMOTION_BC:
                this.handlerEmotionBC(val);
                break;
            //房间对局结束
            case NetCMD.SERVER_DZ_GAME_START_RES:
            case NetCMD.SERVER_PRE_END_GAME:
                this.handlerGameOverBC(val);
                break;

            //申请结束房间失败
            case NetCMD.SERVER_APPLY_DISSOLVE_ROOM_ERR_UC:
                this.handlerReqEndRoomErroUC(val);
                break;
            //进行投票通知
            case NetCMD.SERVER_APPLY_DISSOLVE_ROOM_SUCC_BC:
                this.handlerReqEndRoomSuccBC(val);
                break;
            //投票状态
            case NetCMD.SERVER_DISSOLVE_ACTION_SUCC_BC:
                this.handlerEndRoomActionBC(val);
                break;
            //投票结果
            case NetCMD.SERVER_DISSOLVE_ROOM_RESULT_BC:
                this.handlerEndRoomResultBC(val);
                break;

            case NetCMD.SERVER_SPLIT_CARD_BC:
                this.chanSplitCardPos(val)
                break;

            case NetCMD.SERVER_GAME_START:
                this.handlerShowGameEndView(val);
                break;
        }
    },


    //-------------------------------------------------------斗牛---------------------------



    handlerPreready_Begin:function(val)
    {
        cc.log("SERVER_PREREADY_BEGIN_BC");

        YZDNModel.state = YZDNState.START;

        if(val.hasOwnProperty("players"))
        {
            var playleng = val["players"].length;

            for(var i = 0;i<playleng;i++)
            {
                var d = val["players"][i];
                var seatid = d["seatid"];
                var player = YZDNModel.players[seatid];
                if(player != null)
                {
                    player.winmoney = d["win_lose"];
                }
            }
        }


        if(YZDNModel.gameEndView == null)
        {
            YZDNModel.gameEndView  = new YZDNBalanceView();
            YZDNModel.gameEndView.callBackAction = this.nextGameStart;
            YZDNModel.gameEndView.setPosition(cc.p(55,10));
            this.addChild(YZDNModel.gameEndView,100);
        }
        else
        {
            YZDNModel.gameEndView.setVisible(true);
        }

        YZDNModel.gameEndView.initdata();
    },


            //广播：开始牌局，发牌
    handlerGameStartBC:function(val)
    {
        cc.log("handlerGameStartBC");

        this.resetGame();

        YZDNModel.state = YZDNState.START;

        for (var i = 0; i < 5; i++)
        {
            YZDNPlayers.setReady(i,false);
        }

        YZDNTips.hideTime();

        this.stopGameOver();
        JsUtils.postNotifi("notify_game_check_end");
        YZDNModel.sendGetPlayerList();

        var cardlen = 0;
        if(YZDNModel.deal_cards_select == 1)
        {
            cardlen = 4;
        }
        else
        {
            cardlen = 3;
        }

        if(!YZDNModel.fastShow)
        {
            YZDNCards.fapaibegin(); //发牌
        }
        

        var delay = 0.2;
        for(var b = 0; b<5; b++)
        {
            var seaid = YZDNModel.getSeatIDByPos(b)
            var player = YZDNModel.players[seaid];
            if(player != null && player.betting == 1)
            {
                for(var c = 0;c<cardlen;c++)
                {
                    delay += 0.05;

                    if(YZDNModel.fastShow)
                    {
                        YZDNCards.showCardValue_video(b,c,0,-1);
                    }
                    else
                    {
                        YZDNCards.showCards(b,c,delay);
                    }
                    
                }
            }   
        }

    },



    DisplayThreeCard:function(val)
    {
        cc.log("SERVER_DEAL_THREE_CARDS_BC");

        if(val.hasOwnProperty("players"))
        {
            var leng = val["players"].length;
            for(var i = 0; i<leng; i++)
            {
                var d = val["players"][i];
                var seatid = d["seatid"];

                
                var player = YZDNModel.players[seatid];
                var pos =  YZDNModel.getPosBySeatID(player.seatid);
                var nplayer = YZDNPlayers.players[pos];
                if(player != null)
                {
                    var cardlen = d["cards"].length;
                    for(var k = 0; k<cardlen;k++)
                    {
                        player.cards.push(d["cards"][k]);

                        if(YZDNModel.fastShow)
                        {
                            YZDNCards.showCardValue(pos,k,0,d["cards"][k]);
                        }
                    }
                    player.is_niu = d["is_niu"];
                    player.max_card = d["max_card"];

                    if(YZDNModel.fastShow)
                    {
                        nplayer.showTipNiuShow(player.is_niu,player.max_card);
                    }

                }
            }

            // YZDNCards.fapaibegin(); //发牌

            // var delay = 0.2;
            // for(var b = 0; b<leng; b++)
            // {
            //     var seait = val["players"][b]["seatid"];
            //     var pos = YZDNModel.getPosBySeatID(seait);
            //     for(var c = 0;c<3;c++)
            //     {
            //         delay += 0.05;
            //         YZDNCards.showCards(pos,c,delay);
            //     }
            // }

            if(!YZDNModel.fastShow)
            {
                this.schedule(this.CheckThreeCardOver,1.0);
            }
            
        }
    },


    CheckThreeCardOver:function(dt)
    {
        if(YZDNCards.cardNums <= 0)
        {
            this.unschedule(this.CheckThreeCardOver);
            var delay = 0.2;
            for(var i=0; i<5; i++)
            {
                delay += 0.1;
                var seaid = YZDNModel.getSeatIDByPos(i)
                var player = YZDNModel.players[seaid];

                var pos = YZDNModel.getPosBySeatID(seaid);
                var nplayer = YZDNPlayers.players[pos];

                if(player != null)
                {
                    var len = player.cards.length;
                    if(len > 0)
                    {
                        for(var j = 0;j<len;j++)
                        {
                            delay += 0.05;
                            YZDNCards.showCardValue(i,j,delay,player.cards[j]);
                        }

                        // if(seaid == YZDNModel.seatID)
                        // {
                            nplayer.showTipNiuShow(player.is_niu,player.max_card);
                        // }
                        
                    }
                }
            }

        }
    },


    HideFourCard:function(val)
    {
        cc.log("SERVER_DEAL_FOUR_CARDS_UC");

        var seatid = val["seatid"];
        var player = YZDNModel.players[seatid];

        if(player != null)
        {
            var pos =  YZDNModel.getPosBySeatID(player.seatid);
            var nplayer = YZDNPlayers.players[pos];

            var cardlen = val["cards"].length;
            for(var k = 0; k<cardlen;k++)
            {
                player.cards.push(val["cards"][k]);

                // if(YZDNModel.fastShow && player.seatid == YZDNModel.seatID )   //  在发牌的时候就展示了所有人的牌
                if(YZDNModel.fastShow )   //  在发牌的时候就展示了所有人的牌
                {
                    YZDNCards.showCardValue(pos,k,0,val["cards"][k]);
                }
            }
            player.is_niu = val["is_niu"];
            player.max_card = val["max_card"];

            if(YZDNModel.fastShow)
            {
                nplayer.showTipNiuShow(player.is_niu,player.max_card);
            }


            // YZDNCards.fapaibegin(); //发牌

            // var delay = 0.2;
            // for(var b = 0; b<5; b++)
            // {
            //     var seaid = YZDNModel.getSeatIDByPos(b)
            //     var player = YZDNModel.players[seaid];
            //     if(player != null && player.betting == 1)
            //     {
            //         for(var c = 0;c<4;c++)
            //         {
            //             delay += 0.05;
            //             YZDNCards.showCards(b,c,delay);
            //         }
            //     }   
            // }

            if(!YZDNModel.fastShow)
            {
                this.schedule(this.CheckFourCardOver,1.0);
            }
            
        }
    },

    CheckFourCardOver:function(dt)
    {
        if(YZDNCards.cardNums <= 0)
        {
            this.unschedule(this.CheckFourCardOver);
            var delay = 0.2;
            var player = YZDNModel.players[YZDNModel.seatID];

            var pos = YZDNModel.getPosBySeatID(YZDNModel.seatID);
            var nplayer = YZDNPlayers.players[pos];

            if(player != null)
            {
                var len = player.cards.length;
                if(len > 0)
                {
                    for(var j = 0;j<len;j++)
                    {
                        delay += 0.05;
                        YZDNCards.showCardValue(0,j,delay,player.cards[j]);
                    }

                    nplayer.showTipNiuShow(player.is_niu,player.max_card);
                }
            }


        if (YZDNModel.isOnVideo)
        {
            for(var i=0; i<5; i++)
            {
                delay = 0.2;
                var seaid = YZDNModel.getSeatIDByPos(i)
                if(seaid != YZDNModel.seatID)
                {
                    var player = YZDNModel.players[seaid];

                    var pos = YZDNModel.getPosBySeatID(seaid);
                    var nplayer = YZDNPlayers.players[pos];

                    if(player != null)
                    {
                        var len = player.cards.length;
                        if(len > 0)
                        {
                            for(var j = 0;j<len;j++)
                            {
                                delay += 0.05;
                                YZDNCards.showCardValue(pos,j,delay,player.cards[j]);
                            }
                            nplayer.showTipNiuShow(player.is_niu,player.max_card);
                        }
                    }
                }
                
            }
        }

            
        }
    },



    //设置庄家
    setZang:function(val)
    {
        cc.log("SERVER_SET_DEALER_SECC_BC");

        YZDNModel.state == YZDNState.BETTING;

        YZDNPlayers.resetZang();
        YZDNModel.dealer = val["dealer"];
        YZDNModel.rob_ratio = val["rob_ratio"];


        var Ac = cc.callFunc(function(){
                    for(var i = 0;i<5;i++)
                    {
                        var player = YZDNModel.players[i];
                        if(player != null)
                        {
                            var pos = YZDNModel.getPosBySeatID(i);
                            var nplay =  YZDNPlayers.players[pos];
                            nplay.hidePnum();
                        }
                    }

                    if(YZDNModel.deal_cards_select == 1)
                    {
                        var pos = YZDNModel.getPosBySeatID(YZDNModel.dealer);
                        var nplay =  YZDNPlayers.players[pos];
                        nplay.showPnum(true,YZDNModel.rob_ratio);
                    }
                    

                    if(YZDNModel.dealer > -1)
                    {
                        var pos = YZDNModel.getPosBySeatID(YZDNModel.dealer);
                        var player = YZDNPlayers.players[pos]; 
                        player.showzang(pos);
                    }
                    if(YZDNModel.seatID > -1)
                    {
                        var player = YZDNModel.players[YZDNModel.seatID];
                        if(player != null && YZDNModel.dealer != YZDNModel.seatID && player.betting == 1)
                        {
                            if (!YZDNModel.isOnVideo)
                            {
                              YZDNAction.showOutCardBtns(false);  
                            }
                        }
                    }
                });

        
        var Tdelay = cc.delayTime(1.0);
        var runTip = cc.callFunc(function(){
                for(var i =0;i < 5;i++)
                {
                    if(YZDNModel.dealer != i)
                    {
                        var pos = YZDNModel.getPosBySeatID(i);
                        var player = YZDNModel.players[i];
                        if(player != null && player.betting == 1)
                        {
                            var nplayer = YZDNPlayers.players[pos];
                            nplayer.startTimer(YZDNModel.bet_timer_stamp,YZDNModel.bet_timer_stamp);
                            if(player.bet <= 0)
                            {
                                nplayer.setWaiteTip(true,"等待下注...")
                            }
                            
                        }
                    }
                }    
            });

        if(!YZDNModel.fastShow)
        {
           this.runAction(cc.sequence(Ac,Tdelay,runTip)); 
        }
        else
        {
            for(var i = 0;i<5;i++)
                    {
                        var player = YZDNModel.players[i];
                        if(player != null)
                        {
                            var pos = YZDNModel.getPosBySeatID(i);
                            var nplay =  YZDNPlayers.players[pos];
                            nplay.hidePnum();
                        }
                    }

                    if(YZDNModel.deal_cards_select == 1)
                    {
                        var pos = YZDNModel.getPosBySeatID(YZDNModel.dealer);
                        var nplay =  YZDNPlayers.players[pos];
                        nplay.showPnum(true,YZDNModel.rob_ratio);
                    }
                    

                    if(YZDNModel.dealer > -1)
                    {
                        var pos = YZDNModel.getPosBySeatID(YZDNModel.dealer);
                        var player = YZDNPlayers.players[pos]; 
                        player.showzang(pos);
                    }
                    if(YZDNModel.seatID > -1)
                    {
                        var player = YZDNModel.players[YZDNModel.seatID];
                        if(player != null && YZDNModel.dealer != YZDNModel.seatID && player.betting == 1)
                        {
                            if (!YZDNModel.isOnVideo)
                            {
                             YZDNAction.showOutCardBtnsVideo(false);  
                            }
                        }
                    }



            for(var i =0;i < 5;i++)
                {
                    if(YZDNModel.dealer != i)
                    {
                        var pos = YZDNModel.getPosBySeatID(i);
                        var player = YZDNModel.players[i];
                        if(player != null && player.betting == 1)
                        {
                            var nplayer = YZDNPlayers.players[pos];
                            nplayer.startTimer(YZDNModel.bet_timer_stamp,YZDNModel.bet_timer_stamp);
                            nplayer.setWaiteTip(true,"等待下注...")
                        }
                    }
                } 
        }
        

        
    },

    //发剩余的牌
    ShowSurplusCards:function(val)
    {
        cc.log("SERVER_DEAL_REMAIN_CARDS_UC");

        YZDNModel.state == YZDNState.REMAIN_CARDS;

        var seatid = 0;
        var player = null;

        if (YZDNModel.isOnVideo)
        {
             seatid = val["seatid"];
             player = YZDNModel.players[seatid];
        }
        else
        {
             seatid = YZDNModel.seatID;
             player = YZDNModel.players[seatid];
        }
        

        if(player != null)
        {

            var cardlen = val["cards"].length;
            YZDNModel.SurplusCards = cardlen;

            for(var k = 0; k<cardlen;k++)
            {
                player.cards.push(val["cards"][k]);
            }

        }
    },


     ShowSurplusCards_broadcast:function(val)
    {
        cc.log("SERVER_SHOW_BROADCASTCARDS_BC");
        var startindex = 0;
        if(YZDNModel.deal_cards_select == 1)
        {
            startindex = 4;
        }
        else
        {
            startindex = 3;
        }
        
        if(!YZDNModel.fastShow)
        {
            YZDNCards.fapaibegin(); //发牌
        }
        

        var delay = 0.2;
        for(var c = startindex; c<5; c++)
        {
            for(var b = 0; b<5; b++)
            {
                var player = YZDNModel.players[b];

                var pos = YZDNModel.getPosBySeatID(b);
                var nplayer = YZDNPlayers.players[pos];
                nplayer.hidetipNiu();

                if(player != null && player.ready == 1 && player.betting == 1)
                {
                    var pos = YZDNModel.getPosBySeatID(b);
                    delay += 0.05;

                    // if(YZDNModel.fastShow && player.seatid == YZDNModel.seatID)
                    if(YZDNModel.fastShow)
                    {
                        YZDNCards.showCardValue_video(pos,c,0,player.cards[c]);
                    }
                    else
                    {
                        YZDNCards.showCards(pos,c,delay);
                    }
                    
                }
            }
        }
        if(!YZDNModel.fastShow)
        {
            this.schedule(this.SurplusCardOver,1.0);
        }
    },


    //展示剩余的牌
    SurplusCardOver:function(dt)
    {
        if(YZDNCards.cardNums <= 0)
        {
            this.unschedule(this.SurplusCardOver);

            var player = YZDNModel.players[YZDNModel.seatID];
            var delay = 0.2;
            if(player != null)
            {
                var len = player.cards.length;
                if(len > 0)
                {
                    for(var j = YZDNModel.SurplusCards;j<len;j++)
                    {
                        delay += 0.05;
                        YZDNCards.showCardValue(0,j,delay,player.cards[j]);
                    }
                }
            }


            if (YZDNModel.isOnVideo)
            {
                for(var i=0; i<5; i++)
                {
                    delay = 0.2;
                    var seaid = YZDNModel.getSeatIDByPos(i)
                    if(seaid != YZDNModel.seatID)
                    {
                        var player = YZDNModel.players[seaid];
                        if(player != null)
                        {
                            var pos = YZDNModel.getPosBySeatID(seaid);
                            var len = player.cards.length;
                            if(len > 0)
                            {
                                for(var j = YZDNModel.SurplusCards;j<len;j++)
                                {
                                    delay += 0.05;
                                    YZDNCards.showCardValue(pos,j,delay,player.cards[j]);
                                }
                            }
                        }
                    }
                    
                }
            }
        }
    },


    qiangBtnShow:function(val)
    {
        cc.log("SERVER_ROB_DEALER_BC");

        this.resetGame();

        YZDNModel.state == YZDNState.SET_DEALER;

        if(YZDNModel.seatID != -1)
        {
            var player = YZDNModel.players[YZDNModel.seatID];
            if(player != null && player.betting == 1)
            {
                if (!YZDNModel.isOnVideo)
                {
                    if(YZDNModel.fastShow)
                    {
                        YZDNAction.showOutCardBtnsVideo(true);  //抢牌 
                    }
                    else
                    {
                        YZDNAction.showOutCardBtns(true);  //抢牌 
                    }
                }
                
            }
                
        }
        
        

        var Tdelay = cc.delayTime(1.0);
        var runTip = cc.callFunc(function(){
                for(var i =0;i < 5;i++)
                {
                    var pos = YZDNModel.getPosBySeatID(i);
                    var player = YZDNModel.players[i];
                    if(player != null && player.betting == 1)
                    {
                        var nplayer = YZDNPlayers.players[pos];
                        nplayer.startTimer(YZDNModel.bet_timer_stamp,YZDNModel.bet_timer_stamp);
                        nplayer.setWaiteTip(true,"等待抢庄...")
                    }
                }   
            });

        if(!YZDNModel.fastShow)
        {
            this.runAction(cc.sequence(Tdelay,runTip));
        }
        else
        {
            for(var i =0;i < 5;i++)
                {
                    var pos = YZDNModel.getPosBySeatID(i);
                    var player = YZDNModel.players[i];
                    if(player != null && player.betting == 1)
                    {
                        var nplayer = YZDNPlayers.players[pos];
                        nplayer.startTimer(YZDNModel.bet_timer_stamp,YZDNModel.bet_timer_stamp);
                        nplayer.setWaiteTip(true,"等待抢庄...")
                    }
                } 
        }
        

        
    },

    //展示所有人的牌
    getSpellCards:function(val)
    {
        cc.log("SERVER_SHOW_CARDS_BC");

        YZDNModel.state == YZDNState.SHOW_CARDS;

        if(val.hasOwnProperty("players"))
        {
            var leng = val["players"].length;
            for(var i = 0; i<leng; i++)
            {
                var d = val["players"][i];
                var seatid = d["seatid"];
                var player = YZDNModel.players[seatid];
                if(player != null)
                {
                    var cardlen = d["cards"].length;
                    player.spellcards = [];
                    for(var k = 0; k<cardlen;k++)
                    {
                        player.spellcards.push(d["cards"][k]);
                    }
                    player.card_type = d["card_type"];
                }
            }


            //展示所有玩家的牌

            var stime = 1.0;
            var jgtime = 0.5;

            if (YZDNModel.isOnVideo)
            {
                if(YZDNModel.fastShow)
                {
                    stime = 0;
                    jgtime = 0;
                }
                else
                {
                    stime = 0.5;
                    jgtime = 0.2;
                }
                 
            }

            var startSeatid = YZDNModel.dealer;  //开始位置
            var Tdelay = cc.delayTime(0.5);
            var delay = stime;
            var DTouplayers = cc.callFunc(function(){
                    for(var i = startSeatid + 1; i < 5; i++)
                    {
                        var player = YZDNModel.players[i];
                        if(player != null && player.betting == 1)
                        {
                            var pos = YZDNModel.getPosBySeatID(i);
                            delay += jgtime;
                            if(YZDNModel.isMySeatID(i))
                            {
                                YZDNCards.showfixMyCards(pos,player.cards,player.spellcards,player.card_type,delay);
                            }
                            else
                            {
                                YZDNCards.showfixOtherCards(pos,player.spellcards,player.card_type,delay);
                            } 
                        }
                    }
                });

            var DWeiplayers = cc.callFunc(function(){
                    for(var i = 0; i <= startSeatid; i++)
                    {
                        var player = YZDNModel.players[i];
                        if(player != null && player.betting == 1)
                        {
                            var pos = YZDNModel.getPosBySeatID(i);
                            delay += jgtime;
                            if(YZDNModel.isMySeatID(i))
                            {
                                YZDNCards.showfixMyCards(pos,player.cards,player.spellcards,player.card_type,delay);
                            }
                            else
                            {
                                YZDNCards.showfixOtherCards(pos,player.spellcards,player.card_type,delay);
                            } 
                        } 
                    }
                });

            this.uiNode.runAction(cc.sequence(Tdelay,DTouplayers,DWeiplayers));

        }

    },

    RobSuccess:function(val)
    {
        cc.log("SERVER_ROB_DEALER_SUCC_BC");
        var seatid = val["seatid"];
        var pos = YZDNModel.getPosBySeatID(seatid);
        var player = YZDNModel.players[seatid];
        var robe =  val["rob_ratio"];
        if(player != null)
        {
            player.rob_ratio = robe;

            var nplayer = YZDNPlayers.players[pos];
            nplayer.stopTimer();
            nplayer.setWaiteTip(false,"");

            nplayer.doActionPnum(robe);
        }

        if(YZDNModel.isMySeatID(seatid))
        {
            //抢成功了

            if (!YZDNModel.isOnVideo)
            {
                if(YZDNModel.fastShow)
                {
                    YZDNAction.hideOutCardBtns(true);
                }
                else
                {
                    YZDNAction.hideOutCardBtns(true);
                }
            }
            
        }

    },
    //-------------------------------------------------------斗牛---------------------------

    handlerShowGameEndView:function(val)
    {
        log("handlerShowGameEndView");

        var box = new YZDNGameEndView();
        box.setPosition(cc.p(55,10));
        box.initdata(val);
        box.setTag(19999);
        this.addChild(box,100000);
    },


    handler_server_offline_bc:function(val)
    {
        cc.log("SERVER_OFFLINE_BC");

        var seait = val["seatid"];
        var pos = YZDNModel.getPosBySeatID(seait); 
        var nplayer = YZDNPlayers.players[pos];
        if(nplayer != null)
        {
            nplayer.showUI("playlose",false);
            nplayer.showUI("plaything",false);

            nplayer.showUI("playlose",true);
        }
        
    },



    gameReadyStart:function(val)
    {
        cc.log("SERVER_READY_TIME_BC");
        YZDNTips.showTime(YZDNModel.ready_timer_stamp);
    },

    BetTimeOut:function(val)
    {
        cc.log("SERVER_BET_TIMEOUT_BC");
        var pos = YZDNModel.getPosBySeatID(YZDNModel.curSeatID); 
        var nplayer = YZDNPlayers.players[pos];

        nplayer.showUI("playlose",false);
        nplayer.showUI("plaything",false);

        nplayer.showUI("plaything",true);
    },



//当前的下注操作
    showNext_bet_bc:function(val)
    {
        cc.log("SERVER_NEXT_BET_BC");

        var seait = val["seatid"];
        YZDNModel.curSeatID = seait;

        var pos = YZDNModel.getPosBySeatID(seait);
        var player = YZDNModel.players[seait];
        var nplayer = YZDNPlayers.players[pos];

        nplayer.startTimer(YZDNModel.bet_timer_stamp,YZDNModel.bet_timer_stamp);

        if(YZDNModel.isMySeatID(seait))
        {
            YZDNModel.myoptionfinish = false;

            Sound.getInstance().playEffect(PackName+"/res/sound/com/my_turn.mp3"); 

            if(!YZDNModel.isOnVideo)
            {
                if(YZDNModel.fastShow)
                {
                    YZDNAction.showOutCardBtnsVideo(false);  //抢牌 
                }
                else
                {
                    YZDNAction.showOutCardBtns(false);  //抢牌 
                }
            }

            
        }
    },

 //下注结果的的返回
    sendBetSucc_bc:function(val)
    {
        cc.log("SERVER_BET_SUCC_BC");

        var seatid = val["seatid"];
        var bet = val["bet"];

        if(YZDNModel.isMySeatID(seatid))
        {
            YZDNModel.myoptionfinish = true;

           if(!YZDNModel.isOnVideo)
           {
                if(YZDNModel.fastShow)
                {
                    YZDNAction.hideOutCardBtnsVideo(false);
                }
                else
                {
                    YZDNAction.hideOutCardBtns(false);
                }
           } 
            
            
        }

        var player = YZDNModel.players[seatid];
        if(player != null)
        {
            player.bet = bet;
            var pos = YZDNModel.getPosBySeatID(seatid);
            if(player.bet > 0)
            {
                var num = Math.ceil((player.bet)/YZDNModel.stand_bobo);  
                YZDNChips.sendBetMoney(pos,num,player.bet);
            }
            YZDNPlayers.players[pos].updatePlayerMoney();
        }

        var pos = YZDNModel.getPosBySeatID(seatid);
        var nplayer = YZDNPlayers.players[pos];
        nplayer.stopTimer();
        nplayer.setWaiteTip(false,"")
        nplayer.hidetipNiu();
        nplayer.showUI("playlose",false);
        nplayer.showUI("plaything",false);


    },


    First_Round_Bc:function(val)
    {
        cc.log("=====SERVER_FIRST_ROUND_UC====");

        JsUtils.postNotifi("notify_game_check_end"); //清除邀请

        this.resetGame();
        YZDNTips.hideTime();
        YZDNTips.showWaitTip("");

        YZDNCards.fapaibegin();
        var delay = 0.2;
        for(var i =0;i<5;i++)
        {
            var player = YZDNModel.getPlayerByPos(i)
            if(player != null && player.betting == 1 && player.seatid != -1)
            {
                for(var j = 0;j<2;j++)
                {
                    delay += 0.1;
                    YZDNCards.showCards(i,j,delay);
                } 
            }
        }

    },


//设置簸簸成功
    setBoboSuc:function(val)
    {
        cc.log("add bobo success");

        var mybobo = utils.getValInt(val,"bobo_money");
        var mymoney = utils.getValInt(val,"money");
        var seatid =  utils.getValInt(val,"seatid");   

        var player = YZDNModel.players[seatid];

        if(player == undefined || player == null)
        {
            return;
        }

        player.money = mymoney;
        player.bobo_money = mybobo;

        var pos = YZDNModel.getPosBySeatID(seatid);
        var nplayer = YZDNPlayers.players[pos];

        nplayer.stopTimer();

        nplayer.showUI("playlose",false);
        nplayer.showUI("plaything",false);

        nplayer.updatePlayerMoney();
    },
    
    //回调：房间信息
    handlerTableInfoUC:function(val) {
        cc.log("handlerTableInfoUC");

        YZDNModel.waitUpdateInfo = false;
        YZDNModel.reset();

        if(YZDNModel.gameEndView != null)
        {
            YZDNModel.gameEndView.setVisible(false);
        }

        this.isResetGame = false;
        this.resetGame();

        YZDNPlayers.downtable();

        YZDNModel.roomGolden = utils.getValInt(val, "takein");
        YZDNModel.state = utils.getValInt(val, "state");
        YZDNModel.seatID = utils.getValInt(val, "seatid");


        
        YZDNModel.pauseTime = utils.getValInt(val, "game_pause_tstamp");

        YZDNModel.base_money = utils.getValInt(val, "base_money");
        YZDNModel.bet_timer_stamp = utils.getValInt(val, "bet_timer_stamp");
        YZDNModel.chepai_timer_stamp = utils.getValInt(val, "chepai_timer_stamp");
        YZDNModel.curSeatID = utils.getValInt(val, "cur_seat");
        YZDNModel.cur_bet =  utils.getValInt(val, "cur_bet");
        YZDNModel.is_bet_timeout =  utils.getValBool(val, "is_bet_timeout");
        YZDNModel.is_option_timeout = utils.getValBool(val, "is_option_timeout");
        YZDNModel.is_xiu = utils.getValBool(val, "is_xiu");
        YZDNModel.dealer = utils.getValInt(val, "dealer");
        YZDNModel.left_bet_timer = utils.getValInt(val, "left_bet_timer");
        YZDNModel.left_chepai_timer = utils.getValInt(val, "left_chepai_timer");
        YZDNModel.left_ready_timer = utils.getValInt(val, "left_ready_timer");
        YZDNModel.left_set_bobo_timer = utils.getValInt(val, "left_set_bobo_timer");
        YZDNModel.max_raise_bet = utils.getValInt(val, "max_raise_bet");

        YZDNModel.preready_timer_stamp = utils.getValInt(val, "preready_timer_stamp");
        YZDNModel.ready_timer_stamp = utils.getValInt(val, "ready_timer_stamp");
        YZDNModel.set_bobo_timer_stamp = utils.getValInt(val, "set_bobo_timer_stamp");
        YZDNModel.stand_money = utils.getValInt(val, "stand_money");

        YZDNModel.spell_card_timer_stamp = utils.getValInt(val, "spell_card_timer_stamp");

        YZDNModel.deal_cards_select = utils.getValInt(val, "deal_cards_select");
        YZDNModel.bet_ratio_select = utils.getValInt(val, "bet_ratio_select");
        YZDNModel.dealer_select = utils.getValInt(val, "dealer_select");
        YZDNModel.card_type_select = utils.getValInt(val, "card_type_select");
        YZDNModel.compare_select = utils.getValInt(val, "compare_select");


        YZDNModel.rob_ratio =  utils.getValInt(val, "rob_ratio");

        YZDNModel.stand_bobo = 1;
        if(val.hasOwnProperty("bet_ratio"))
        {
            var leng = val["bet_ratio"].length;
            for(var i = 0; i< leng;i++)
            {
                YZDNModel.betRadio[i] = val["bet_ratio"][i];
            }
            YZDNModel.stand_bobo = YZDNModel.betRadio[0];
        }



        ZJHModel.getInstance().mySeatid = YZDNModel.seatID;

        if(val.hasOwnProperty("max_tou_seatid"))
        {
            YZDNModel.max_tou_pai_seatid = utils.getValBool(val,"max_tou_seatid");
        }
        if(val.hasOwnProperty("max_wei_seatid"))
        {
            YZDNModel.max_wei_pai_seatid = utils.getValBool(val,"max_wei_seatid");
        }


        if (YZDNModel.isOnVideo){
            if (val.hasOwnProperty("players")) {
                len = val["players"].length;
                for (var i = 0; i < len; ++i){
                    var itemPlay = val["players"][i];
                    var seatId = utils.getValInt(itemPlay, "seatid");
                    var uid = utils.getValInt(itemPlay, "uid");
                    if (uid == ZJHModel.getInstance().uid){
                        YZDNModel.seatID = seatId;
                        ZJHModel.getInstance().mySeatid = YZDNModel.seatID;
                        break;
                    }
                }
            }
        }

        if(YZDNModel.seatID < 0 || YZDNModel.seatID > 4)
        {
             YZDNModel.seatID = 0;
        }


        YZDNAction.setBtnZhu(YZDNModel.betRadio);

        YZDNTips.setGameInfoMessage(YZDNModel.deal_cards_select,YZDNModel.bet_ratio_select,YZDNModel.dealer_select,YZDNModel.card_type_select,YZDNModel.compare_select);


        //提示恢复
        YZDNPlayers.resetPlayerposition();  //重置座位的
        YZDNCards.resetCard();              //重置牌位置

        //上桌玩家信息
        var len = 0;
        if (val.hasOwnProperty("players")) {
            len = val["players"].length;
            for (var i = 0; i < len; i++)
            {
                var d = val["players"][i];
                var player = YZDNModel.updatePlayer(d);
                YZDNPlayers.uptable(player);

                var pos = YZDNModel.getPosBySeatID(player.seatid);
                var nPlayers = YZDNPlayers.players[pos];


                if(player.isOffline)
                {
                    nPlayers.showUI("playlose",true);
                }
                else
                {
                    if(YZDNModel.state == YZDNState.SET_DEALER)
                    {
                        if(player.betting == 1 && player.ready == 1 && YZDNModel.is_option_timeout && player.rob_ratio <0)
                        {
                            nPlayers.showUI("plaything",true);
                        }
                        
                    }
                    else if(YZDNModel.state == YZDNState.BETTING)
                    {
                        if(player.betting == 1 && player.ready == 1 && YZDNModel.is_option_timeout && player.bet <= 0)
                        {
                            nPlayers.showUI("plaything",true);
                        }
                    }
                }

                if(player.ready == 1)
                {
                    nPlayers.showUI("playgray",false);
                }
                
                

                //恢复准备状态
                if (YZDNModel.state == YZDNState.READY) {
                    YZDNPlayers.setReady(pos, player.ready == 1);
                    if (YZDNModel.isMyUID(player.uid)) {
                        if(player.ready != 1)
                        {
                            nPlayers.showUI("playgray",true);
                            YZDNAction.showReadyBtn(true);
                        }
                        else
                        {
                            YZDNAction.showReadyBtn(false);
                            nPlayers.showUI("playgray",false);
                        }
                        
                    }else{
                        if(player.ready != 1)
                        {
                            nPlayers.showUI("playgray",true);
                            nPlayers.showUI("playready",false);
                        }
                        else
                        {
                            nPlayers.showUI("playready",true);
                            nPlayers.showUI("playgray",false);
                        }
                    }
                }


                //恢复玩家的状态－－－---------SET_DEALER-----------
                if (YZDNModel.state == YZDNState.SET_DEALER && player.betting == 1)
                {
                    if (YZDNModel.isMySeatID(player.seatid)) 
                    {
                        var leng= player.cards.length;
                        for(var k = 0; k<leng; k++)
                        {
                            YZDNCards.showCardValue(pos,k,0,player.cards[k]);
                        }

                        nPlayers.showTipNiuShow(player.is_niu,player.max_card); //是否有牛
                    }
                    else
                    {
                        for(var k = 0; k<4; k++)
                        {
                            YZDNCards.showCards(pos,k,0);
                        }
                        
                    }

                    if (YZDNModel.isMySeatID(player.seatid))
                    {
                        if(player.rob_ratio < 0)
                        {
                            nPlayers.hidePnum();
                            YZDNAction.showOutCardBtns(true);  //抢庄
                            nPlayers.setWaiteTip(true,"等待抢庄...");
                        }
                        else if(player.rob_ratio == 0)
                        {
                            nPlayers.showBQ(true);

                        }else
                        {
                            nPlayers.showPnum(true,player.rob_ratio);
                        }
                    }
                    else
                    {
                        if(player.rob_ratio < 0)
                        {
                            nPlayers.hidePnum();
                            nPlayers.setWaiteTip(true,"等待抢庄...");
                        }
                        else if(player.rob_ratio == 0)
                        {
                            nPlayers.showBQ(true);
                        }
                        else
                        {
                            nPlayers.showPnum(true,player.rob_ratio);
                        }
                    }

                    nPlayers.startTimer(YZDNModel.bet_timer_stamp,YZDNModel.left_bet_timer);

                }
                //恢复玩家的状态－－－---------BETTING-----------
                if(YZDNModel.state == YZDNState.BETTING && player.betting == 1)
                {
                    if(YZDNModel.deal_cards_select == 1)
                    {
                        if (YZDNModel.isMySeatID(player.seatid)) 
                        {
                            var leng= player.cards.length;
                            for(var k = 0; k<leng; k++)
                            {
                                YZDNCards.showCardValue(pos,k,0,player.cards[k]);
                            }

                            nPlayers.showTipNiuShow(player.is_niu,player.max_card); //是否有牛
                        }
                        else
                        {
                            for(var k = 0; k<4; k++)
                            {
                                YZDNCards.showCards(pos,k,0);
                            }
                            
                        }
                    }
                    else
                    {
                        var leng= player.cards.length;
                        for(var k = 0; k<leng; k++)
                        {
                            YZDNCards.showCardValue(pos,k,0,player.cards[k]);
                        }

                        nPlayers.showTipNiuShow(player.is_niu,player.max_card); //是否有牛
                    }


                    if (YZDNModel.isMySeatID(player.seatid))
                    {
                        if(player.seatid != YZDNModel.dealer)
                        { 
                            if(player.bet != 0)
                            {
                                var num = Math.ceil((player.bet)/YZDNModel.stand_bobo);  
                                YZDNChips.sendBetMoney(pos,num,player.bet);
                            }
                            else
                            {
                                YZDNAction.showOutCardBtns(false);  //下注
                                nPlayers.setWaiteTip(true,"等待下注...");
                            }
                            
                        }
                    }
                    else
                    {
                        if(player.seatid != YZDNModel.dealer)
                        {
                            if(player.bet != 0)
                            {
                                var num = Math.ceil((player.bet)/YZDNModel.stand_bobo);  
                                YZDNChips.sendBetMoney(pos,num,player.bet);
                            }
                            else
                            {
                                nPlayers.setWaiteTip(true,"等待下注...");
                            }
                            
                        }
                    }

                    if(player.seatid != YZDNModel.dealer)
                    { 
                        nPlayers.startTimer(YZDNModel.bet_timer_stamp,YZDNModel.left_bet_timer);
                    }

                }

                //恢复玩家的状态－－－---------REMAIN_CARDS-----------
                 if(YZDNModel.state == YZDNState.REMAIN_CARDS && player.betting == 1)
                 {
                    if(YZDNModel.deal_cards_select == 1)
                    {
                        if (YZDNModel.isMySeatID(player.seatid)) 
                        {
                            for(var k = 0; k<5; k++)
                            {
                                YZDNCards.showCardValue(pos,k,0,player.cards[k]);
                            }

                            nPlayers.showTipNiuShow(player.is_niu,player.max_card);   //是否有牛
                        }
                        else
                        {
                            for(var k = 0; k<5; k++)
                            {
                                YZDNCards.showCards(pos,k,0);
                            }
                        }
                    }
                    else
                    {
                        for(var k = 0; k<5; k++)
                        {
                            YZDNCards.showCardValue(pos,k,0,player.cards[k]);
                        }

                        // nPlayers.showTipNiuShow(player.is_niu,player.max_card);   //是否有牛
                    }

                    if(player.bet != 0)
                    {
                        var num = Math.ceil((player.bet)/YZDNModel.stand_bobo);  
                        YZDNChips.sendBetMoney(pos,num,player.bet);
                    }

                 }

                //恢复玩家的状态－－－---------SHOW_CARDS-----------  展示所有的牌
                if(YZDNModel.state == YZDNState.SHOW_CARDS && player.betting == 1)
                 {
                    var num = Math.ceil((player.bet)/YZDNModel.stand_bobo);  
                    YZDNChips.sendBetMoney(pos,num,player.bet);

                    if(YZDNModel.isMySeatID(player.seatid))
                    {
                        for(var k = 0; k<5; k++)
                        {
                            YZDNCards.showCardValue(pos,k,0,player.cards[k]);
                        }

                        YZDNCards.showfixMyCards(pos,player.cards,player.spellcards,player.card_type,0);
                    }
                    else
                    {
                        YZDNCards.showfixOtherCards(pos,player.cards,player.card_type,0);
                    }
                    
                 }

                 

                 if(YZDNModel.state == YZDNState.GAME_END && player.betting == 1)
                 {
                    if(YZDNModel.isMySeatID(player.seatid))
                    {
                        for(var k = 0; k<5; k++)
                        {
                            YZDNCards.showCardValue(pos,k,0,player.cards[k]);
                        }

                        YZDNCards.showfixMyCards(pos,player.cards,player.spellcards,player.card_type,0);
                    }
                    else
                    {
                        YZDNCards.showfixOtherCards(pos,player.cards,player.card_type,0);
                    }
                 }

            }
        }

        if(YZDNModel.state != YZDNState.READY && YZDNModel.state != YZDNState.START  && YZDNModel.state != YZDNState.SET_DEALER)   //设置庄家
        {
            if(YZDNModel.dealer != -1)
            {
                var pos = YZDNModel.getPosBySeatID(YZDNModel.dealer);
                var player = YZDNPlayers.players[pos]; 
                player.showzang(pos);

                if(YZDNModel.deal_cards_select == 1)
                {
                    var nPlayers = YZDNPlayers.players[pos];
                    nPlayers.showPnum(true,YZDNModel.rob_ratio); 
                }
                    
             }
        }

        if(ZJHModel.getInstance().mySeatid == -1)
        {
            YZDNAction.showSzBtn(true);
        }else{
            YZDNAction.showSzBtn(false);
        }


        if(YZDNModel.state == YZDNState.SHOW_CARDS  || YZDNModel.state == YZDNState.GAME_END)
        {
            this.isResetGame = false;
        }

        if(len<5)
        {
            if(!YZDNModel.isMyInTable())
            {
                YZDNModel.sendUptable();
            }
        }

        if(YZDNModel.state != YZDNState.READY)
        {
            JsUtils.postNotifi("notify_game_check_end");
        }

        //恢复投票
        if (val.hasOwnProperty("dissolve_room_info") && val["dissolve_room_info"]["state"] == 1)
        {
            this.showEndRoom(val["dissolve_room_info"]);
        }
    },




    //广播：登录成功,掉线重连
    handlerLoginSuccBC:function(val)
    {
        cc.log("handlerLoginSuccBC");

        // YZDNModel.updatePlayer(val);
        //更新掉线提示
        YZDNTips.showAllTip("");
        var seatID =utils.getValInt(val,"seatid");
        var player = YZDNModel.getPosBySeatID(seatID);
        player.isOffline = 0;
        if(seatID >=0 && seatID<=5)
        {
            var pos =YZDNModel.getPosBySeatID(seatID);
            YZDNPlayers.setOffline(pos,false);
        }
    },
    //广播：上桌成功
    handlerUptableSuccBC:function(val)
    {
        cc.log("handlerUptableSuccBC");
        var player = YZDNModel.updatePlayer(val);
        var Nplayer = YZDNPlayers.players[YZDNModel.getPosBySeatID(player.seatid)];

        Sound.getInstance().playEffect(PackName+"/res/sound/com/getin.mp3"); 
            
        if(YZDNModel.isMyUID(player.uid))
        {

            YZDNAction.showSzBtn(false);

            YZDNModel.waitUpdateInfo = true;
            YZDNModel.sendRoomInfo();
            YZDNTips.showMyTip("您已上桌");
        }
        else
        {
            YZDNPlayers.uptable(player);
            Nplayer.showUI("playgray",true);
        }
    },
    //回调：上桌失败
    handlerUptableErroUC:function(val)
    {
        cc.log("handlerUptableErroUC");
        var code = utils.getValInt(val, "code");
        if(code == 102)
        {
            YZDNTips.showMyTip("您已在桌上");
        }else if(code == 104)
        {
            YZDNTips.showMyTip("金币不够，无法上桌");
        }else if(code == 105)
        {
            YZDNTips.showMyTip("桌上人已满");
        }
    },
    //广播：下桌成功
    handlerDowntableSuccBC:function(val)
    {
        cc.log("handlerDowntableSuccBC");

        Sound.getInstance().playEffect(PackName+"/res/sound/com/getout.mp3"); 

        var seatID = utils.getValInt(val,"seatid");
        if(seatID>-1)
        {
            var pos = YZDNModel.getPosBySeatID(seatID);
            YZDNPlayers.downtable(pos);
            YZDNCards.removeFinishCPForPos(pos);
            YZDNCards.HideCardsAndRemoveTip(pos);
        }

        if(YZDNModel.isMySeatID(seatID))
        {
            YZDNModel.unDownSeatID =YZDNModel.seatID;
            YZDNModel.seatID = -1;
            ZJHModel.getInstance().mySeatid = YZDNModel.seatID;
            YZDNAction.showReadyBtn(false);
            YZDNTips.showMyTip("您已下桌");
            YZDNAction.showSzBtn(true);

            if(ZJHModel.getInstance().mySeatid == -1)
            {
                YZDNAction.showSzBtn(true);
            }else{
                YZDNAction.showSzBtn(false);
            }

        }

        YZDNModel.resetPlayer(seatID);
    },

    //回调：准备成功
    handlerReadySuccBC:function(val)
    {
        cc.log("handlerReadySuccBC");


        var uid = utils.getValInt(val,"uid");
        var seatid = utils.getValInt(val,"seatid");
        var pos = YZDNModel.getPosBySeatID(seatid);

        if(YZDNModel.isMyUID(uid))
        {
            this.resetGame();
        }

        YZDNPlayers.setgray(pos,false);
        YZDNPlayers.setReady(pos,true);

        var player = YZDNModel.getPlayerBySeatID(seatid);
        if(player)
        {
            player.ready = 1;
            player.betting = 1;
        }

        if(YZDNModel.isMyUID(uid))
        {
            YZDNAction.showReadyBtn(false);
            YZDNAction.showSzBtn(false);

            for(var k = 0; k < 5;k++)
            {
                var player = YZDNModel.getPlayerByPos(k);
                if(player != null && player.seatid != -1 && player.ready == 1)
                {
                    YZDNPlayers.setgray(k,false);
                    YZDNPlayers.setReady(k,true);
                }
            }
            
        }

    },
    //回调：准备失败
    handlerReadyErroUC:function(val)
    {
        cc.log("handlerReadyErroUC");
        if(YZDNModel.isMyInTable())
        {
            YZDNTips.showMyTip("准备失败");
        }
        else
        {
            YZDNAction.showReadyBtn(false);
            YZDNAction.showSzBtn(false);
        }
    },

    //广播：准备开始新游戏
    handlerGamePrereadyBC:function(val)
    {
        cc.log("handlerGamePrereadyBC");

        YZDNModel.state = YZDNState.READY;

        if(YZDNModel.isMyInTable())
        {
            // YZDNAction.showReadyBtn(true);
        }

        for(var i = 0;i<5;i++)
        {
            var player = YZDNModel.getPlayerByPos(i);
            if(player != null && player.seatid != -1)
            {
                player.ready = 0;
                player.betting = 0;
                player.winmoney = 0;
                player.cards = [];
                player.spellcards = [];
                player.card_type = -1;
            }
        }
        
        //提示准备工作
    },
    //广播：玩家表情互动
    handlerEmotionBC:function(val)
    {
        cc.log("handlerEmotionBC");
        var base = utils.getValInt(val,"seatid");
        var target = utils.getValInt(val,"target_seatid");
        var face = utils.getValInt(val,"type");
        if(face != -1 && target!=-1 && base != target && !YZDNModel.isMySeatID(base) )
        {
            var b = YZDNPlayers.getPosition(YZDNModel.getPosBySeatID(base));
            var t  =YZDNPlayers.getPosition(YZDNModel.getPosBySeatID(target));

            var tarpos = YZDNModel.getPosBySeatID(target);

            log("============kkkk============="+tarpos);

            YZDNTips.showBigface(b,t,face,YZDNPlayers.getSex(YZDNModel.getPosBySeatID(base)),tarpos);
        }
    },

    //回调：看牌
    handlerGameStartUC:function(val)
    {
        cc.log("handlerGameStartUC");

        YZDNModel.state = YZDNState.QIANGGUAN_STATE; //翻牌之后就是抢关

        if(val.hasOwnProperty("cards"))
        {
            YZDNCards.handCardValues =[];
            YZDNCards.pushHandCard(val["cards"]);
        }

        if(YZDNModel.isFaPai)
        {
            var self = this;
            var ac = cc.repeatForever(cc.sequence(cc.delayTime(0.1),cc.callFunc(function(){

                if(!YZDNModel.isFaPai)
                {
                    self.runAction(cc.sequence(cc.delayTime(1),
                        cc.callFunc(function(){
                            if(YZDNModel.gamePlayType != 1)
                            {
                                var pos = YZDNModel.getPosBySeatID(YZDNModel.firstPlayerSeait);
                                YZDNTips.foPlayerOutCard(YZDNModel.table_type,pos);
                            }
                            
                        }),
                        cc.delayTime(0.8),
                        cc.callFunc(function(){
                        YZDNCards.showHandCards(0,YZDNModel.table_type);
                        YZDNCards.showHandCards(1,YZDNModel.table_type);
                        YZDNCards.showHandCards(2,YZDNModel.table_type);
                    }),cc.callFunc(function(){
                            YZDNTips.showMPAnimation(0,true);
                            YZDNTips.showMPAnimation(1,true);
                            YZDNTips.showMPAnimation(2,true);
                        })));
                    self.stopGameOver();
                    self.stopActionByTag(666666);
                }
            })));
            ac.setTag(666666);
            this.runAction(ac);
        }
        else
        {

            self.runAction(cc.sequence(
                cc.callFunc(function(){
                    if(YZDNModel.gamePlayType != 1)
                    {
                         var pos = YZDNModel.getPosBySeatID(YZDNModel.firstPlayerSeait);
                        YZDNTips.foPlayerOutCard(YZDNModel.table_type,pos);
                    }
                }),
                cc.delayTime(0.8),
                cc.callFunc(function(){
                YZDNCards.showHandCards(0,YZDNModel.table_type);
                YZDNCards.showHandCards(1,YZDNModel.table_type);
                YZDNCards.showHandCards(2,YZDNModel.table_type);
            }),cc.callFunc(function(){
                            YZDNTips.showMPAnimation(0,true);
                            YZDNTips.showMPAnimation(1,true);
                            YZDNTips.showMPAnimation(2,true);
                        })
                ));

            this.stopGameOver();
        }
    },


    //回调：开始明牌
    handlerStartMingPaiUC:function(val)
    {
        cc.log("handlerStartMingPaiUC");
        var seatID = utils.getValInt(val,"seatid");
        var pos = YZDNModel.getPosBySeatID(seatID);
        if(YZDNModel.isMySeatID(seatID))
        {
            YZDNAction.showMingPaiBtns(true);
        }
        YZDNTips.hideJiaoQiangTip();
        YZDNTips.hideThinkTips();

        YZDNTips.showTime(pos,YZDNModel.mingpaiTime);
        YZDNTips.startWaitTime(pos,YZDNModel.mingpaiTime);


        if(YZDNTips.needBuLa())
        {
            YZDNTips.showTiLa(YZDNModel.getPosBySeatID(YZDNModel.dzSeatID),2,1);
        }

    },


    //广播：进入出牌
    handlerStartOutCardBC:function(val)
    {
        cc.log("handlerStartOutCardBC");
        YZDNModel.noOutCardSeait = -1;  //重置不可以出牌的位置


        var seatID = utils.getValInt(val,"seatid");
        var pos = YZDNModel.getPosBySeatID(seatID);
        YZDNModel.curSeatID = seatID;
        YZDNModel.outCardMode =utils.getValInt(val,"action");

        YZDNModel.state = YZDNState.PLAYING;

        YZDNAction.showMingPaiBtns(false);
        YZDNTips.hideJiaoQiangTip();
        YZDNTips.hideThinkTips();


        YZDNTips.stopWaitTime(0);
        YZDNTips.stopWaitTime(1);
        YZDNTips.stopWaitTime(2);


        //重新初始出牌数组
        YZDNCards.showOutCards(pos,[]);
        YZDNTips.showOutCard(pos,0);


        YZDNTips.showTime(pos,YZDNModel.playTime);
        YZDNTips.startWaitTime(pos,YZDNModel.playTime);
    },

    checkmpList:function(mplist,pos)
    {
        var flg = false;
        for(var i = 0 ;i<mplist.length;i++)
        {
            if(mplist[i]==pos)
            {
                flg = true;
                break;
            }
        }
        return flg;
    },
    //广播：出牌结果
    handlerOutCardSuccBC:function(val) 
    {
        cc.log("handlerOutCardSuccBC");
    },

    //广播：此局游戏结束，进入结算
    handlerGameEndBC:function(val)
    {
        cc.log("handlerGameEndBC");
        YZDNModel.state = YZDNState.GAME_END;
        this.isResetGame = false;
        YZDNTips.showAllTip("");
        YZDNTips.showMyTip("");

        //收集筹码
        YZDNChips.sendAllPlayerChipTpTable();

        var temposchip = {};
        var winLost = {};
        var sendChip = {};

        for(var i =0;i<5;i++)
        {
            temposchip[i] = null;
            winLost[i] = 0;
            sendChip[i] = 0;
        }

        if(val.hasOwnProperty("players"))
        {
            var playlen = val["players"].length;
            for(var k = 0;k<playlen;k++)
            {
                var d = val["players"][k];
                var seatid = d["seatid"];
                var pos = YZDNModel.getPosBySeatID(seatid);
                var player = YZDNModel.players[seatid];

                player.money = d["money"];    //玩家的钱
                var winlost = d["win_lose"];
                var  playbetFen = 0;
                if(YZDNModel.bet_ratio_select == 1)
                {
                    playbetFen = player.bet * YZDNModel.stand_bobo;
                }
                else 
                {
                     playbetFen = player.bet;
                }
                
                var lastFen = playbetFen + winlost;

                if(lastFen < 0)
                {
                    if(Math.abs(winlost) > playbetFen)
                    {
                        var Fen = Math.abs(winlost) - playbetFen;
                        //再丢筹码
                        var num = Math.ceil(Fen/YZDNModel.stand_bobo); 
                        sendChip[pos] = num;
                        
                    }
                }
                if(lastFen > 0)
                {
                    temposchip[pos] = lastFen;
                }
                
                winLost[pos] = winlost;
            }

        }
            
            var SChip = cc.callFunc(function(){
                    for(var i = 0; i < 5; i++)
                    {
                        var pos = YZDNModel.getPosBySeatID(i);
                        var player = YZDNModel.players[i];
                        if(player != null && sendChip[pos] != 0)
                        {
                            YZDNChips.sendBetMoneyToTable(pos,num);
                        }
                    }
                });

            var Tdelay = cc.delayTime(0.5);
            var delay = 0.2;
            var CAction = cc.callFunc(function(){
                    for(var i = 0; i < 5; i++)
                    {
                        var pos = YZDNModel.getPosBySeatID(i);
                        var player = YZDNModel.players[i];
                        if(player != null && temposchip[pos] != null)
                        {
                            YZDNChips.t_cun = YZDNChips.cun;
                            var numfen = temposchip[pos]/YZDNModel.stand_bobo;
                            YZDNChips.sendChipToPlayer(pos,numfen);
                        }
                    }
                });
            var wdelay = cc.delayTime(0.2);
            var FAction = cc.callFunc(function(){
                    for(var i = 0; i < 5; i++)
                    {
                        var pos = YZDNModel.getPosBySeatID(i);
                        var player = YZDNModel.players[i];
                        if(player != null && player.betting == 1 && winLost[pos] != null)
                        {
                            YZDNPlayers.updatePlayerMoney(pos);
                            YZDNTips.showEnunciationCoin(pos,winLost[pos]);
                        }
                    }
                });

            

            this.runAction(cc.sequence(Tdelay,SChip,Tdelay,CAction,wdelay,FAction));

    },

    nextGameStart:function()
    {
        if(YZDNModel.gameEndView != null)
        {
            YZDNModel.gameEndView.setVisible(false);

            Sound.getInstance().playEffect(PackName+"/res/sound/com/button.mp3");
            if(YZDNModel.state == YZDNState.READY)
            {
                YZDNModel.sendReady();
            }
        }

        JsUtils.postNotifi("notify_next_game_start");
    },

     WaiteNextBureau:function()
    {
        if(YZDNModel.isGameOver)
        {
            this.onGameOver();
        }else{

            this.stopGameOver();
            JsUtils.postNotifi("notify_game_check_end");
        }
    },

    //广播：房间对局结束
    handlerGameOverBC:function(val) {
        cc.log("handlerGameOverBC");
        if(this.getChildByName("RoomGameEndLayer"))
        {
            return;
        }
        ZJHModel.getInstance().mySeatid = -1;
        YZDNModel.isGameOver = true;
        // ZJHModel.getInstance().roomToShareView(YZDNModel.curRoomID);
        YZDNModel.sendOver();
    },
    //回调：申请结束房间失败
    handlerReqEndRoomErroUC:function(val)
    {
        cc.log("handlerReqEndRoomErroUC");
        var code = utils.getValInt(val,"code");
        if (code == 101)
        {
            PlatformHelper.showToast("每个玩家每局游戏只能申请两次解散房间！");
        }
        else if (code == 102)
        {
            PlatformHelper.showToast("只有桌子上的玩家可以申请解散房间！");
        }
        else if (code == 103)
        {
            PlatformHelper.showToast("当前有人正在申请解散房间！");
        }

    },
    //广播：申请结束房间成功
    handlerReqEndRoomSuccBC:function(val)
    {
        cc.log("handlerReqEndRoomSuccBC");
        this.showEndRoom(val);

    },
    //广播：投票状态
    handlerEndRoomActionBC:function(val)
    {
        cc.log("handlerEndRoomActionBC");
        var player = YZDNModel.getPlayerByUID(val["uid"]);
        if(!player)
        {
            return;
        }

        if(this.roomEndBox)
        {
            this.roomEndBox.updateState(player.uid,val["action"]);
        }
    },
    //广播：投票结果
    handlerEndRoomResultBC:function(val)
    {
        cc.log("handlerEndRoomResultBC");
        if(this.roomEndBox)
        {
            this.roomEndBox.removeFromParent(true);
        }
        this.roomEndBox = null;
        
        var result =val["result"];
        if (result == 1)
        {
            // PlatformHelper.showToast("房间解散成功，3秒后房间自动结束");
        }
        else
        {
            PlatformHelper.showToast("解散失败，稍后游戏继续！");
        }
    },

    //广播：玩家掉线情况
    handlerPlayerOfflineBC:function(val) {
        cc.log("handlerPlayerOfflineBC");
        var seatID = utils.getValInt(val,"seatid");
        var player = YZDNModel.getPlayerBySeatID(seatID);
        if(player)
        {
            player.isOffline = 1;
            if(seatID >-1)
            {
                var  pos = YZDNModel.getPosBySeatID(seatID);
                YZDNPlayers.setOffline(pos,true);
                YZDNTips.checkTimeOut(pos);
            }
        }
    }
});
