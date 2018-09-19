

var WZDNGameState ={
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


var WZDNTable = cc.Layer.extend({
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

        cc.spriteFrameCache.addSpriteFrames("wznn/res/img/card.plist");
        cc.spriteFrameCache.addSpriteFrames("public/res/img/bigface.plist");
        cc.spriteFrameCache.addSpriteFrames("wznn/res/img/cardtype.plist");
        cc.spriteFrameCache.addSpriteFrames("wznn/res/img/common.plist");

        var b1 = (new Date).valueOf();
        this.uiJson = ccs.load(PackName+"/res/table.json");
        this.uiNode =this.uiJson.node;
        this.addChild(this.uiNode);

        // cc.log("tib1:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();

        // cc.log("tib2:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();
       
        WZDNModel.reset();

        //初始化数据模型
         this.initView();
        WZDNModel.roomData = JSON.parse( ZJHModel.getInstance().getRoomData());
        WZDNModel.curRoomID = ZJHModel.getInstance().curRoomID;

        // cc.log("tib3:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();
        //监听联网状态

        this.schedule(this.updateGame,0.0);
        this.schedule(this.hearbeat,3.0);

        cc.log("roomID:"+ WZDNModel.curRoomID );

        WZDNPlayers.publicBox = RoomPublic.create();
        if (ZJHModel.getInstance().vcode > 20050){
            WZDNPlayers.publicBox.setRequestFlag(false);
        }
        WZDNPlayers.publicBox.setCallback(function(tag)
        {
            if(tag == "exit")
            {
                WZDNModel.sendDowntable();
                WZDNModel.sendLogout();
            }
            else if(tag == "end_room")
            {
                WZDNModel.sendReqRoomEnd();
            }
            else if(tag == "down_table")
            {
                WZDNModel.sendDowntable();
            }
            else if(tag == "seatBtn")
            {
                WZDNModel.sendUptable();
            }
        });

        this.addChild(WZDNPlayers.publicBox,10000);

        Sound.getInstance().preLoadEffect("js/"+PackName+"/res/sound");
        Sound.getInstance().PlayBgSound(PackName+"/res/sound/game_bgm.mp3");


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

                WZDNModel.release();
                WZDNPlayers.stopAllPlayTimer();
                WZDNTips.stopTipTimer();


                ccs.ArmatureDataManager.destroyInstance();
                cc.spriteFrameCache.removeSpriteFrames();
                cc.textureCache.removeAllTextures();
                jsb.fileUtils.purgeCachedEntries();

            }
        });
        cc.eventManager.addListener(this._listenerExit,this);

        WZDNModel.initNet( WZDNModel.roomData["gameRoom"]["way"]["ip"],Number(WZDNModel.roomData["gameRoom"]["way"]["port"]));
        WZDNModel.connectNet();


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

        return true;
    },


    test:function()
    {
        this.testCount++;
        WZDNModel.seatID = 0;
        

        if(this.testCount == 1)
        {
            JsUtils.postNotifi("notify_game_check_end");
            WZDNAction.hideOutCardBtns();


            WZDNPlayers.resetPlayerposition();
            for(var i =0;i<5;i++)
            {
                var player = WZDNPlayers.players[i];
                player.seatid = i;
                player.money = 1000;
                WZDNPlayers.uptable(player);
            }
        }
        if(this.testCount == 3)
        {

            for(var k = 0;k<5;k++)
            {
                for(var j = 0;j<3;j++)
                {
                    WZDNModel.otherPlayCards[k].push(5);
                }
                
            }
            
            WZDNCards.fapaibegin();
            WZDNCards.resetCard();

        }else if(this.testCount == 5)
        {
            var delay = 0.2;
            for(var i =0;i<5;i++)
            {
                var pos = WZDNModel.getPosBySeatID(i);
                for(var j = 0;j<3;j++)
                {
                    delay += 0.05;
                    WZDNCards.showCards(pos,j,delay);
                }

            }
        }else if(this.testCount == 7)
        {
            var delay = 0.2;
            for(var i =0;i<5;i++)
            {
                delay += 0.15;
                var pos = WZDNModel.getPosBySeatID(i);
                
                WZDNCards.showCards(pos,3,delay);
            }
        }
        else if(this.testCount == 9)
        {
            var delay = 0.2;
            for(var i =0;i<5;i++)
            {
                delay += 0.15;
                var pos = WZDNModel.getPosBySeatID(i);
                WZDNCards.showCardValue(pos,3,delay,5);
            }
        }
        else if(this.testCount == 10)
        {
            var delay = 0.2;
            for(var i =0;i<5;i++)
            {
                delay += 0.15;
                var pos = WZDNModel.getPosBySeatID(i);
                WZDNCards.showCards(pos,4,delay);
            }
        }
        else if(this.testCount == 12)
        {
            var delay = 0.2;
            for(var i =0;i<5;i++)
            {
                delay += 0.25;
                var pos = WZDNModel.getPosBySeatID(i);
                WZDNCards.showCardValue(pos,4,delay,5);
            }
        }
        else if(this.testCount == 15)
        {
            WZDNCards.fapaiend();
        }

        else if(this.testCount == 20)
        {
            for(var i =0;i<5;i++)
            {
                var pos = WZDNModel.getPosBySeatID(i);
                WZDNChips.sendBetMoney(pos,50,6);
            }
        }

        else if(this.testCount == 22)
        {
            WZDNAction.showOutCardBtns();
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
        WZDNAction.uiNode =  this.getUI("btns");
        WZDNAction.uiNode.setLocalZOrder(99);
        WZDNAction.clickCB = this.onBtnClickEvt;
        WZDNAction.init();

        // cc.log("ti1:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();

        WZDNCards.uiNode =  this.getUI("cards");
        WZDNCards.uiNode.setLocalZOrder(101);
        WZDNCards.init();
        // cc.log("ti2:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();

        WZDNChips.uiNode =  this.getUI("chip");
        WZDNChips.uiNode.setLocalZOrder(102);
        WZDNChips.init();

        WZDNPlayers.uiNode =  this.getUI("players");
        WZDNPlayers.init();

        // cc.log("ti3:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();
        WZDNTips.uiNode =  this.getUI("tips");
        WZDNTips.uiNode.setLocalZOrder(100);
        WZDNTips.init();

        // cc.log("ti4:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();
    },
    //心跳处理
    hearbeat:function( dt) {
        WZDNModel.sendHearbeat();
    },
    //联网监听
    updateGame:function( dt)
    {
        var curTime = (new Date).valueOf()/1000;


        if( WZDNModel.reconnectTime!=-1)
            {
                WZDNModel.reconnectTime +=dt;

                if(WZDNModel.reconnectTime >3)
                {
                    WZDNModel.changeIP();
                    WZDNModel.connectNet();
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
            WZDNModel.waitUpdateInfo = true;

            if( WZDNModel.isConnect())
            {
                WZDNModel.resetResponse();
                WZDNModel.sendRoomInfo();

            }else{

                WZDNModel.connectNet();
            }

            return;
        }
        //心跳包超时
        if(WZDNModel.lastHearbeatTime != 0 && (curTime - WZDNModel.lastHearbeatTime ) > 10)
        {
            WZDNModel.connectNet();

            WZDNModel.lastHearbeatTime = curTime;
            if( this.retNetNum >0)
            {
                WZDNTips.showAllTip("网络连接异常，正在重试");
            }
            this.retNetNum ++;
            return;
        }
        var response = WZDNModel.getNetData();
        if(response != null)
        {
            if(response.cmd ==TCPSendCMD.DISCONNECT_RES || response.cmd ==TCPSendCMD.CONNECT_ERROR_RES
                || response.cmd ==TCPSendCMD.SEND_DATA_ERROR_RES|| response.cmd ==TCPSendCMD.RECV_DATA_ERROR_RES)
            {
                    cc.log("tcp error:"+response.cmd);
                    if(!WZDNModel.isLogout)
                    {
                        WZDNModel.connectNet();
                    }

                    if (WZDNModel.lastHearbeatTime == 0)
                    {
                        WZDNModel.lastHearbeatTime = utils.getTimeStamp();
                    }


            }
            else if( response.cmd ==TCPSendCMD.CONNECT_OK_RES)
            {
                cc.log("tcp connect succ!/n cmd:"+response.cmd +" data:"+response.data );
                WZDNModel.reconnectTime = -1;
                WZDNModel.sendLogin();
            }
            else if( response.cmd ==TCPSendCMD.RECV_DATA_OK_RES)
            {
                 var  d = response.parseDatta();
                 this.handlerCMD(d);
            }
            response.release();
        }

    },

    //重置游戏信息
    resetGame:function()
    {

        WZDNModel.resetData();//重置数据

        if(this.isResetGame)
        {
            return;
        }

        this.isResetGame = true;

        this.Advancefp = true;
        WZDNAction.reset();

        WZDNCards.reset();
        WZDNCards.resetCard();              //重置牌位置

        WZDNTips.reset();

        WZDNPlayers.reset(null);
        WZDNPlayers.resetPlayerposition();  //重置座位的
        WZDNPlayers.stopAllPlayTimer();

        WZDNChips.reset();

        WZDNModel.resetPre();

        //我的id
        var player = WZDNModel.getPlayerBySeatID(WZDNModel.seatID);
       if(player)
       {
           ZJHModel.getInstance().rmb = player.rmb;
           ZJHModel.getInstance().money = player.money;
           ZJHModel.getInstance().coin = player.coin;
       }

    },

    onGameOver: function () {
        WZDNModel.isGameOver = true;
      if(WZDNModel.state == WZDNState.READY||WZDNModel.state == WZDNState.GAME_END)
      {
          var self  =this;
          // this.stopGameOver();
          var action = cc.sequence(cc.delayTime(7),cc.callFunc(function(){
              ZJHModel.getInstance().mySeatid = -1;
              WZDNModel.isGameOver = true;
              ZJHModel.getInstance().roomToShareView(WZDNModel.curRoomID);
              // WZDNModel.sendOver();
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
        if(!WZDNModel.isMyInTable())
        {
            return;
        }
        var player = WZDNModel.getPlayerByUID(val["uid"]);
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
                var p = WZDNModel.getPlayerByUID(val["voters_uid"][i]);
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
                WZDNModel.sendRoomEnd(1);
            }
            else if (str == "no")
            {
                WZDNModel.sendRoomEnd(0);
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
                WZDNModel.sendUptable();
                break;
            case "btn_ready":
                Sound.getInstance().playEffect(PackName+"/res/sound/com/button.mp3");
                WZDNModel.sendReady();
                break;
            case "btn_one":
                {
                    WZDNModel.sendBetRequest(WZDNModel.betRadio[0]); 
                }
                break;
            case "btn_two":
                {
                    WZDNModel.sendBetRequest(WZDNModel.betRadio[1]);
                }
                break;
            case "btn_three":
                {
                    WZDNModel.sendBetRequest(WZDNModel.betRadio[2]);
                }
                break;
            case "btn_four":
                {
                    WZDNModel.sendBetRequest(WZDNModel.betRadio[3]);
                }
                break;
            case "btn_five":
                {
                    WZDNModel.sendBetRequest(WZDNModel.betRadio[4]);
                }
                break;
        }
    },

 //*************************** 服务器返回信息处理 ************************************//
    //处理联网回调
    handlerCMD:function(data)
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


        if(NetCMD.SERVER_DZ_LOGIN_ERR_RES == cmd)
        {
            ZJHModel.getInstance().mySeatid = -1;
            WZDNModel.isGameOver = true;
            ZJHModel.getInstance().roomToShareView(WZDNModel.curRoomID);
            return;
        }

        if(WZDNModel.isGameover)
        {
            return;
        }

        if(WZDNModel.waitUpdateInfo && cmd !=NetCMD.SERVER_TABLE_INFO_UC)
        {
            return;
        }

        switch (cmd)
        {
            //心跳包
            case NetCMD.SERVER_HEART_BEAT_RESP:
                cc.log("SERVER_HEART_BEAT_RESP");
                WZDNModel.lastHearbeatTime = (new Date).valueOf()/1000;
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
                WZDNModel.isLogout = true;
                ZJHModel.getInstance().mySeatid = -1;
                WZDNModel.isGameOver = true;
                break;



            case NetCMD.SERVER_SHOW_CARDS_BC:      //展示所有的牌
                this.getSpellCards(val);
                break;

            case NetCMD.SERVER_PREREADY_BEGIN_BC:   //小结提示界面
                this.handlerPreready_Begin(val);
                break;



            //-－无庄斗牛    发牌
             case NetCMD.SERVER_GAME_START_UC:     //游戏开始发牌
                this.handlerGameStartUC(val);
                break;
            case NetCMD.SERVER_BET_SUCC_BC:
                this.sendBetSucc_bc(val);
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




    //-------------------------------------------------------无庄斗牛---------------------------



    handlerGameStartUC:function(val)
    {
        cc.log("handlerGameStartUC");

        WZDNModel.state = WZDNState.SPELLING; //

        if(val.hasOwnProperty("player"))
        {
            var playleng = val["player"].length;

            for(var i = 0;i<playleng;i++)
            {
                var d = val["player"][i];
                var seatid = d["seatid"];
                var cardType = d["card_type"];
                var player = WZDNModel.players[seatid];
                if(player != null)
                {
                    player.card_type = cardType;

                    if(d.hasOwnProperty("cards"))
                    {
                        var le = d["cards"].length;
                        for(var k = 0 ;k <le;k++)
                        {
                            player.cards.push(d["cards"][k]);
                        }
                    }

                    if(d.hasOwnProperty("spell_cards"))
                    {
                        var le = d["spell_cards"].length;
                        for(var k = 0 ;k <le;k++)
                        {
                            player.spellcards.push(d["spell_cards"][k]);
                        }
                    }

                }
            }


            WZDNCards.fapaibegin(); //发牌

            var delay = 0.2;
            for(var b = 0; b<5; b++)
            {
                var seaid = WZDNModel.getSeatIDByPos(b)
                var player = WZDNModel.players[seaid];
                if(player != null && player.betting == 1)
                {
                    for(var c = 0;c<5;c++)
                    {
                        delay += 0.05;
                        WZDNCards.showCards(b,c,delay);
                    }
                }   
            }
            
            this.schedule(this.CheckAllCardOver,1.0);
        }
    },


    CheckAllCardOver:function(dt)
    {
        if(WZDNCards.cardNums <= 0)
        {
            this.unschedule(this.CheckAllCardOver);
            var delay = 0.2;
            var player = WZDNModel.players[WZDNModel.seatID];

            var pos = WZDNModel.getPosBySeatID(WZDNModel.seatID);
            var nplayer = WZDNPlayers.players[pos];

            if(player != null)
            {
                var len = player.cards.length;
                if(len > 0)
                {
                    for(var j = 0;j<len;j++)
                    {
                        delay += 0.05;
                        WZDNCards.showCardValue(0,j,delay,player.cards[j]);
                    }
                }
            }

            this.showAllPlayersCards();

        }
    },


    //展示所有人的牌
    showAllPlayersCards:function()
    {

        WZDNModel.state == WZDNState.SHOW_CARDS;

            //展示所有玩家的牌
            var startSeatid = WZDNModel.dealer;  //开始位置
            var Tdelay = cc.delayTime(2);
            var delay = 1.0;
            var DTouplayers = cc.callFunc(function(){
                    for(var i = 0; i < 5; i++)
                    {
                        var player = WZDNModel.players[i];
                        if(player != null && player.betting == 1)
                        {
                            var pos = WZDNModel.getPosBySeatID(i);
                            delay += 0.2;
                            if(WZDNModel.isMySeatID(i))
                            {
                                WZDNCards.showfixMyCards(pos,player.cards,player.spellcards,player.card_type,delay);
                            }
                            else
                            {
                                WZDNCards.showfixOtherCards(pos,player.spellcards,player.card_type,delay);
                            } 
                        }
                    }
                });


            this.uiNode.runAction(cc.sequence(Tdelay,DTouplayers));
    },



            //广播：开始牌局，发牌
    handlerGameStartBC:function(val)
    {
        cc.log("handlerGameStartBC");

        this.resetGame();

        WZDNModel.state = WZDNState.START;

        for (var i = 0; i < 5; i++)
        {
            WZDNPlayers.setReady(i,false);
        }

        WZDNTips.hideTime();

        this.stopGameOver();
        JsUtils.postNotifi("notify_game_check_end");
        WZDNModel.sendGetPlayerList();

        if(WZDNModel.playtype == 1){
            WZDNTips.showTime(WZDNModel.per_time);
            WZDNModel.per_time = WZDNModel.left_bet_timer;
        }

        if (WZDNModel.seatID != -1) {
        if(WZDNModel.playtype == 1) //是下注的话
        {
            WZDNAction.showOutCardBtns(false);
            //转圈
            var pos = WZDNModel.getPosBySeatID(WZDNModel.seatID);
            var nplayer = WZDNPlayers.players[pos];
            nplayer.startTimer(WZDNModel.per_time,WZDNModel.left_bet_timer);
        }
    }

    },



    //-------------------------------------------------------斗牛---------------------------



    handlerPreready_Begin:function(val)
    {
        cc.log("SERVER_PREREADY_BEGIN_BC");

        WZDNModel.state = WZDNState.START;

        if(val.hasOwnProperty("players"))
        {
            var playleng = val["players"].length;

            for(var i = 0;i<playleng;i++)
            {
                var d = val["players"][i];
                var seatid = d["seatid"];
                var player = WZDNModel.players[seatid];
                if(player != null)
                {
                    player.winmoney = d["win_lose"];
                }
            }
        }


        if(WZDNModel.gameEndView == null)
        {
            WZDNModel.gameEndView  = new WZDNBalanceView();
            WZDNModel.gameEndView.callBackAction = this.nextGameStart;
            WZDNModel.gameEndView.setPosition(cc.p(55,10));
            this.addChild(WZDNModel.gameEndView,100);
        }
        else
        {
            WZDNModel.gameEndView.setVisible(true);
        }

        WZDNModel.gameEndView.initdata();
    },


    //展示所有人的牌
    getSpellCards:function(val)
    {
        cc.log("SERVER_SHOW_CARDS_BC");

        WZDNModel.state == WZDNState.SHOW_CARDS;

        if(val.hasOwnProperty("players"))
        {
            var leng = val["players"].length;
            for(var i = 0; i<leng; i++)
            {
                var d = val["players"][i];
                var seatid = d["seatid"];
                var player = WZDNModel.players[seatid];
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
            var startSeatid = WZDNModel.dealer;  //开始位置
            var Tdelay = cc.delayTime(0.5);
            var delay = 1.0;
            var DTouplayers = cc.callFunc(function(){
                    for(var i = startSeatid + 1; i < 5; i++)
                    {
                        var player = WZDNModel.players[i];
                        if(player != null && player.betting == 1)
                        {
                            var pos = WZDNModel.getPosBySeatID(i);
                            delay += 1.0;
                            if(WZDNModel.isMySeatID(i))
                            {
                                WZDNCards.showfixMyCards(pos,player.cards,player.spellcards,player.card_type,delay);
                            }
                            else
                            {
                                WZDNCards.showfixOtherCards(pos,player.spellcards,player.card_type,delay);
                            } 
                        }
                    }
                });

            var DWeiplayers = cc.callFunc(function(){
                    for(var i = 0; i <= startSeatid; i++)
                    {
                        var player = WZDNModel.players[i];
                        if(player != null && player.betting == 1)
                        {
                            var pos = WZDNModel.getPosBySeatID(i);
                            delay += 1.0;
                            if(WZDNModel.isMySeatID(i))
                            {
                                WZDNCards.showfixMyCards(pos,player.cards,player.spellcards,player.card_type,delay);
                            }
                            else
                            {
                                WZDNCards.showfixOtherCards(pos,player.spellcards,player.card_type,delay);
                            } 
                        } 
                    }
                });

            this.uiNode.runAction(cc.sequence(Tdelay,DTouplayers,DWeiplayers));

        }

    },


    //-------------------------------------------------------斗牛---------------------------

    handlerShowGameEndView:function(val)
    {
        log("handlerShowGameEndView");

        var box = new WZDNGameEndView();
        box.setPosition(cc.p(55,10));
        box.initdata(val);
        box.setTag(19999);
        this.addChild(box,100000);
    },


    handler_server_offline_bc:function(val)
    {
        cc.log("SERVER_OFFLINE_BC");

        var seait = val["seatid"];
        var pos = WZDNModel.getPosBySeatID(seait); 
        var nplayer = WZDNPlayers.players[pos];
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
        WZDNTips.showTime(WZDNModel.ready_timer_stamp);
    },

    BetTimeOut:function(val)
    {
        cc.log("SERVER_BET_TIMEOUT_BC");

        if(val.hasOwnProperty("player"))
        {
            var playleng = val["player"].length;
            for(var i = 0;i < playleng;i++)
            {
                var d = val["player"][i];
                var seid = d["seatid"];

                var pos = WZDNModel.getPosBySeatID(seid); 
                var nplayer = WZDNPlayers.players[pos];

                nplayer.showUI("playlose",false);
                nplayer.showUI("plaything",false);

                nplayer.showUI("plaything",true);
            }

        }
        
    },




    First_Round_Bc:function(val)
    {
        cc.log("=====SERVER_FIRST_ROUND_UC====");

        JsUtils.postNotifi("notify_game_check_end"); //清除邀请

        this.resetGame();
        WZDNTips.hideTime();
        WZDNTips.showWaitTip("");

        WZDNCards.fapaibegin();
        var delay = 0.2;
        for(var i =0;i<5;i++)
        {
            var player = WZDNModel.getPlayerByPos(i)
            if(player != null && player.betting == 1 && player.seatid != -1)
            {
                for(var j = 0;j<2;j++)
                {
                    delay += 0.1;
                    WZDNCards.showCards(i,j,delay);
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

        var player = WZDNModel.players[seatid];

        if(player == undefined || player == null)
        {
            return;
        }

        player.money = mymoney;
        player.bobo_money = mybobo;

        var pos = WZDNModel.getPosBySeatID(seatid);
        var nplayer = WZDNPlayers.players[pos];

        nplayer.stopTimer();

        nplayer.showUI("playlose",false);
        nplayer.showUI("plaything",false);

        nplayer.updatePlayerMoney();
    },
    
    //回调：房间信息
    handlerTableInfoUC:function(val) {
        cc.log("handlerTableInfoUC");

        WZDNModel.waitUpdateInfo = false;
        WZDNModel.reset();

        this.isResetGame = false;
        this.resetGame();

        WZDNPlayers.downtable();

        WZDNModel.roomGolden = utils.getValInt(val, "takein");
        WZDNModel.state = utils.getValInt(val, "state");
        WZDNModel.seatID = utils.getValInt(val, "seatid");


        
        WZDNModel.pauseTime = utils.getValInt(val, "game_pause_tstamp");

        WZDNModel.base_money = utils.getValInt(val, "base_money");
        WZDNModel.bet_timer_stamp = utils.getValInt(val, "bet_timer_stamp");
        WZDNModel.chepai_timer_stamp = utils.getValInt(val, "chepai_timer_stamp");
        WZDNModel.curSeatID = utils.getValInt(val, "cur_seat");
        WZDNModel.cur_bet =  utils.getValInt(val, "cur_bet");
        WZDNModel.is_bet_timeout =  utils.getValBool(val, "is_bet_timeout");
        WZDNModel.is_option_timeout = utils.getValBool(val, "is_option_timeout");
        WZDNModel.is_xiu = utils.getValBool(val, "is_xiu");
        WZDNModel.dealer = utils.getValInt(val, "dealer");
        WZDNModel.left_bet_timer = utils.getValInt(val, "left_bet_timer");
        WZDNModel.left_chepai_timer = utils.getValInt(val, "left_chepai_timer");
        WZDNModel.left_ready_timer = utils.getValInt(val, "left_ready_timer");
        WZDNModel.left_set_bobo_timer = utils.getValInt(val, "left_set_bobo_timer");
        WZDNModel.max_raise_bet = utils.getValInt(val, "max_raise_bet");

        WZDNModel.preready_timer_stamp = utils.getValInt(val, "preready_timer_stamp");
        WZDNModel.ready_timer_stamp = utils.getValInt(val, "ready_timer_stamp");
        WZDNModel.set_bobo_timer_stamp = utils.getValInt(val, "set_bobo_timer_stamp");
        WZDNModel.stand_money = utils.getValInt(val, "stand_money");

        WZDNModel.spell_card_timer_stamp = utils.getValInt(val, "spell_card_timer_stamp");

        WZDNModel.stand_bobo = 1;

        WZDNModel.per_time  = utils.getValInt(val, "bet_timer_stamp");

        ZJHModel.getInstance().mySeatid = WZDNModel.seatID;

        WZDNModel.playtype = utils.getValInt(val, "play_select");

        var mulsize = val["multi_bets"].length;
        for (var i = 0; i <= mulsize; i++) {
            var redio = val["multi_bets"][i];
            WZDNModel.betRadio[i] = redio * WZDNModel.base_money;
        }

        WZDNAction.setBtnZhu(WZDNModel.betRadio);

        //提示恢复
        WZDNPlayers.resetPlayerposition();  //重置座位的
        WZDNCards.resetCard();              //重置牌位置

        log("=============WZDNModel.state=================="+WZDNModel.state);
        //上桌玩家信息
        var len = 0;
        if (val.hasOwnProperty("players")) {
            len = val["players"].length;
            for (var i = 0; i < len; i++)
            {
                var d = val["players"][i];
                var player = WZDNModel.updatePlayer(d);
                WZDNPlayers.uptable(player);

                var pos = WZDNModel.getPosBySeatID(player.seatid);
                var nPlayers = WZDNPlayers.players[pos];


                if(player.isOffline)
                {
                    nPlayers.showUI("playlose",true);
                }
                else
                {
                    if(WZDNModel.state == WZDNState.BETTING)
                    {
                        if(player.betting == 1 && player.ready == 1 && WZDNModel.is_option_timeout && player.bet < 0)
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
                if (WZDNModel.state == WZDNState.READY) {
                    WZDNPlayers.setReady(pos, player.ready == 1);
                    if (WZDNModel.isMyUID(player.uid)) {
                        if(player.ready != 1)
                        {
                            nPlayers.showUI("playgray",true);
                            WZDNAction.showReadyBtn(true);
                        }
                        else
                        {
                            WZDNAction.showReadyBtn(false);
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


                //恢复玩家的状态
                if(WZDNModel.state == WZDNState.BETTING  && player.betting == 1)
                {
                    nPlayers.showUI("playlose",false);
                    nPlayers.showUI("plaything",false);

                    if(player.bet > 0)
                    {
                        var num = Math.ceil((player.bet)/WZDNModel.stand_bobo);  
                        WZDNChips.sendBetMoney(pos,num,player.bet);
                    }

                }

                if(WZDNModel.state == WZDNState.SPELLING  && player.betting == 1)
                {
                    nPlayers.showUI("playlose",false);
                    nPlayers.showUI("plaything",false);

                    if(player.bet > 0)
                    {
                        var num = Math.ceil((player.bet)/WZDNModel.stand_bobo);  
                        WZDNChips.sendBetMoney(pos,num,player.bet);
                    }

                    if(WZDNModel.isMySeatID(player.seatid))
                    {
                        for(var k = 0; k<5; k++)
                        {
                            WZDNCards.showCardValue(pos,k,0,player.cards[k]);
                        }

                        WZDNCards.showfixMyCards(pos,player.cards,player.spellcards,player.card_type,0);
                    }
                    else
                    {
                        WZDNCards.showfixOtherCards(pos,player.cards,player.card_type,0);
                    }

                }


                if(WZDNModel.state == WZDNState.GAME_END  && player.betting == 1)
                {
                    nPlayers.showUI("playlose",false);
                    nPlayers.showUI("plaything",false);

                    if(WZDNModel.isMySeatID(player.seatid))
                    {
                        for(var k = 0; k<5; k++)
                        {
                            WZDNCards.showCardValue(pos,k,0,player.cards[k]);
                        }

                        WZDNCards.showfixMyCards(pos,player.cards,player.spellcards,player.card_type,0);
                    }
                    else
                    {
                        WZDNCards.showfixOtherCards(pos,player.cards,player.card_type,0);
                    }

                }

            }
        }


        if(WZDNModel.state == WZDNState.BETTING)
        {
            if(WZDNModel.seatID != -1)
            {
                var player = WZDNModel.players[WZDNModel.seatID];
                if(WZDNModel.playtype == 1 && player.bet <= 0 && player.betting == 1)
                {
                    WZDNAction.showOutCardBtns(false);
                }
            }

            if(WZDNModel.is_bet_timeout == 1)
            {
                WZDNTips.showTime(0);
            }
            else
            {
                WZDNTips.showTime(WZDNModel.left_bet_timer);
            }
        }


        if(ZJHModel.getInstance().mySeatid == -1)
        {
            WZDNAction.showSzBtn(true);
        }else{
            WZDNAction.showSzBtn(false);
        }


        if(WZDNModel.state == WZDNState.SHOW_CARDS  || WZDNModel.state == WZDNState.GAME_END)
        {
            this.isResetGame = false;
        }

        if(len<5)
        {
            if(!WZDNModel.isMyInTable())
            {
                WZDNModel.sendUptable();
            }
        }

        if(WZDNModel.state != WZDNState.READY)
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

        // WZDNModel.updatePlayer(val);
        //更新掉线提示
        WZDNTips.showAllTip("");
        var seatID =utils.getValInt(val,"seatid");
        var player = WZDNModel.getPosBySeatID(seatID);
        player.isOffline = 0;
        if(seatID >=0 && seatID<=5)
        {
            var pos =WZDNModel.getPosBySeatID(seatID);
            WZDNPlayers.setOffline(pos,false);
        }
    },
    //广播：上桌成功
    handlerUptableSuccBC:function(val)
    {
        cc.log("handlerUptableSuccBC");
        var player = WZDNModel.updatePlayer(val);
        var Nplayer = WZDNPlayers.players[WZDNModel.getPosBySeatID(player.seatid)];

        Sound.getInstance().playEffect(PackName+"/res/sound/com/getin.mp3"); 
            
        if(WZDNModel.isMyUID(player.uid))
        {

            WZDNAction.showSzBtn(false);

            WZDNModel.waitUpdateInfo = true;
            WZDNModel.sendRoomInfo();
            WZDNTips.showMyTip("您已上桌");
        }
        else
        {
            WZDNPlayers.uptable(player);
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
            WZDNTips.showMyTip("您已在桌上");
        }else if(code == 104)
        {
            WZDNTips.showMyTip("金币不够，无法上桌");
        }else if(code == 105)
        {
            WZDNTips.showMyTip("桌上人已满");
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
            var pos = WZDNModel.getPosBySeatID(seatID);
            WZDNPlayers.downtable(pos);
            WZDNCards.removeSpeCardT(pos);

            WZDNCards.HideCardsAndRemoveTip(pos);
        }

        if(WZDNModel.isMySeatID(seatID))
        {
            WZDNModel.unDownSeatID =WZDNModel.seatID;
            WZDNModel.seatID = -1;
            ZJHModel.getInstance().mySeatid = WZDNModel.seatID;
            WZDNAction.showReadyBtn(false);
            WZDNTips.showMyTip("您已下桌");
            WZDNAction.showSzBtn(true);

            if(ZJHModel.getInstance().mySeatid == -1)
            {
                WZDNAction.showSzBtn(true);
            }else{
                WZDNAction.showSzBtn(false);
            }

        }

        WZDNModel.resetPlayer(seatID);
    },

    //回调：准备成功
    handlerReadySuccBC:function(val)
    {
        cc.log("handlerReadySuccBC");


        var uid = utils.getValInt(val,"uid");
        var seatid = utils.getValInt(val,"seatid");
        var pos = WZDNModel.getPosBySeatID(seatid);

        if(WZDNModel.isMyUID(uid))
        {
            this.resetGame();
        }

        WZDNPlayers.setgray(pos,false);
        WZDNPlayers.setReady(pos,true);

        var player = WZDNModel.getPlayerBySeatID(seatid);
        if(player)
        {
            player.ready = 1;
            player.betting = 1;
        }

        if(WZDNModel.isMyUID(uid))
        {
            WZDNAction.showReadyBtn(false);
            WZDNAction.showSzBtn(false);

            for(var k = 0; k < 5;k++)
            {
                var player = WZDNModel.getPlayerByPos(k);
                if(player != null && player.seatid != -1 && player.ready == 1)
                {
                    WZDNPlayers.setgray(k,false);
                    WZDNPlayers.setReady(k,true);
                }
            }
            
        }

    },
    //回调：准备失败
    handlerReadyErroUC:function(val)
    {
        cc.log("handlerReadyErroUC");
        if(WZDNModel.isMyInTable())
        {
            WZDNTips.showMyTip("准备失败");
        }
        else
        {
            WZDNAction.showReadyBtn(false);
            WZDNAction.showSzBtn(false);
        }
    },

    //广播：准备开始新游戏
    handlerGamePrereadyBC:function(val)
    {
        cc.log("handlerGamePrereadyBC");

        WZDNModel.state = WZDNState.READY;

        if(WZDNModel.isMyInTable())
        {
            WZDNAction.showReadyBtn(true);
        }

        for(var i = 0;i<5;i++)
        {
            var player = WZDNModel.getPlayerByPos(i);
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
        if(face != -1 && target!=-1 && base != target && !WZDNModel.isMySeatID(base) )
        {
            var b = WZDNPlayers.getPosition(WZDNModel.getPosBySeatID(base));
            var t  =WZDNPlayers.getPosition(WZDNModel.getPosBySeatID(target));

            var tarpos = WZDNModel.getPosBySeatID(target);

            log("============kkkk============="+tarpos);

            WZDNTips.showBigface(b,t,face,WZDNPlayers.getSex(WZDNModel.getPosBySeatID(base)),tarpos);
        }
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



    //下注结果的的返回
    sendBetSucc_bc:function(val)
    {
        cc.log("SERVER_BET_SUCC_BC");

        var seatid = val["seatid"];
        var bet = val["bet"];

        if(WZDNModel.isMySeatID(seatid))
        {
            WZDNModel.myoptionfinish = true;
            WZDNAction.hideOutCardBtns(false);
        }

        var player = WZDNModel.players[seatid];
        if(player != null)
        {
            player.bet = bet;
            var pos = WZDNModel.getPosBySeatID(seatid);
            if(player.bet > 0)
            {
                var num = Math.ceil((player.bet)/WZDNModel.stand_bobo);  
                WZDNChips.sendBetMoney(pos,num,player.bet);
            }
            WZDNPlayers.players[pos].updatePlayerMoney();
        }

        WZDNTips.hideTime();
        var pos = WZDNModel.getPosBySeatID(seatid);
        var nplayer = WZDNPlayers.players[pos];
        nplayer.stopTimer();
        nplayer.setWaiteTip(false,"")
        nplayer.hidetipNiu();
        nplayer.showUI("playlose",false);
        nplayer.showUI("plaything",false);

    },


    //广播：此局游戏结束，进入结算
    handlerGameEndBC:function(val)
    {
        cc.log("handlerGameEndBC");
        WZDNModel.state = WZDNState.GAME_END;
        this.isResetGame = false;
        WZDNTips.showAllTip("");
        WZDNTips.showMyTip("");

        //收集筹码
        WZDNChips.sendAllPlayerChipTpTable();

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
                var pos = WZDNModel.getPosBySeatID(seatid);
                var player = WZDNModel.players[seatid];

                player.money = d["money"];    //玩家的钱
                var winlost = d["win_lose"];

                var  playbetFen = player.bet * WZDNModel.stand_bobo;
                var lastFen = playbetFen + winlost;

                if(lastFen < 0)
                {
                    if(Math.abs(winlost) > playbetFen)
                    {
                        var Fen = Math.abs(winlost) - playbetFen;
                        //再丢筹码
                        var num = Math.ceil(Fen/WZDNModel.stand_bobo); 
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
                        var pos = WZDNModel.getPosBySeatID(i);
                        var player = WZDNModel.players[i];
                        if(player != null && sendChip[pos] != 0)
                        {
                            WZDNChips.sendBetMoneyToTable(pos,num);
                        }
                    }
                });

            var Tdelay = cc.delayTime(0.5);
            var delay = 0.2;
            var CAction = cc.callFunc(function(){
                    for(var i = 0; i < 5; i++)
                    {
                        var pos = WZDNModel.getPosBySeatID(i);
                        var player = WZDNModel.players[i];
                        if(player != null && temposchip[pos] != null)
                        {
                            WZDNChips.t_cun = WZDNChips.cun;
                            var numfen = temposchip[pos]/WZDNModel.stand_bobo;
                            WZDNChips.sendChipToPlayer(pos,numfen);
                        }
                    }
                });
            var wdelay = cc.delayTime(0.2);
            var FAction = cc.callFunc(function(){
                    for(var i = 0; i < 5; i++)
                    {
                        var pos = WZDNModel.getPosBySeatID(i);
                        var player = WZDNModel.players[i];
                        if(player != null && player.betting == 1 && winLost[pos] != null)
                        {
                            WZDNPlayers.updatePlayerMoney(pos);
                            WZDNTips.showEnunciationCoin(pos,winLost[pos]);
                        }
                    }
                });

            

            this.runAction(cc.sequence(Tdelay,SChip,Tdelay,CAction,wdelay,FAction));

    },

    nextGameStart:function()
    {
        if(WZDNModel.gameEndView != null)
        {
            WZDNModel.gameEndView.setVisible(false);

            Sound.getInstance().playEffect(PackName+"/res/sound/com/button.mp3");
            if(WZDNModel.state == WZDNState.READY)
            {
                WZDNModel.sendReady();
            }
        }

        JsUtils.postNotifi("notify_next_game_start");
    },

     WaiteNextBureau:function()
    {
        if(WZDNModel.isGameOver)
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
        WZDNModel.isGameOver = true;
        ZJHModel.getInstance().roomToShareView(WZDNModel.curRoomID);
        
        // WZDNModel.sendOver();
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
        var player = WZDNModel.getPlayerByUID(val["uid"]);
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
        var player = WZDNModel.getPlayerBySeatID(seatID);
        if(player)
        {
            player.isOffline = 1;
            if(seatID >-1)
            {
                var  pos = WZDNModel.getPosBySeatID(seatID);
                WZDNPlayers.setOffline(pos,true);
                WZDNTips.checkTimeOut(pos);
            }
        }
    }
});
