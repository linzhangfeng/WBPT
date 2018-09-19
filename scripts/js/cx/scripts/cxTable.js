

var CXGameState ={
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


var CXTable = cc.Layer.extend({
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

        cc.spriteFrameCache.addSpriteFrames("cx/res/img/card.plist");
        cc.spriteFrameCache.addSpriteFrames("public/res/img/bigface.plist");
        cc.spriteFrameCache.addSpriteFrames("cx/res/img/cardtype.plist");
        cc.spriteFrameCache.addSpriteFrames("cx/res/img/common.plist");

        var b1 = (new Date).valueOf();
        this.uiJson = ccs.load(PackName+"/res/table.json");
        this.uiNode =this.uiJson.node;
        this.addChild(this.uiNode);

        // cc.log("tib1:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();

        // cc.log("tib2:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();
       
        CXModel.reset();

        //初始化数据模型
         this.initView();
        CXModel.roomData = JSON.parse( ZJHModel.getInstance().getRoomData());
        CXModel.curRoomID = ZJHModel.getInstance().curRoomID;

        CXModel.lastHearbeatTime = (new Date).valueOf()/1000;

        // cc.log("tib3:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();
        //监听联网状态
        this.schedule(this.updateGame,0.0);
        this.schedule(this.hearbeat,3.0);

        cc.log("roomID:"+ CXModel.curRoomID );

        CXPlayers.publicBox = RoomVerticalLayout.create();
        if (ZJHModel.getInstance().vcode > 20050){
            CXPlayers.publicBox.setRequestFlag(false);
        }
        CXPlayers.publicBox.setCallback(function(tag)
        {
            if(tag == "exit")
            {
                CXModel.sendDowntable();
                CXModel.sendLogout();
            }
            else if(tag == "end_room")
            {
                CXModel.sendReqRoomEnd();
            }
            else if(tag == "down_table")
            {
                CXModel.sendDowntable();
            }
            else if(tag == "seatBtn")
            {
                CXModel.sendUptable();
            }
        });

        this.addChild(CXPlayers.publicBox,10000);

        Sound.getInstance().preLoadEffect("js/"+PackName+"/res/sound");
        Sound.getInstance().PlayBgSound(PackName+"/res/sound/game_bgm.mp3");
        // cc.log("tib4:"+((new Date).valueOf()-b1));


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

                CXChips.reset();
                CXModel.release();
                CXPlayers.stopAllPlayTimer();
                CXTips.stopTipTimer();



                ccs.ArmatureDataManager.destroyInstance();
                cc.spriteFrameCache.removeSpriteFrames();
                cc.textureCache.removeAllTextures();
                jsb.fileUtils.purgeCachedEntries();

            }
        });
        cc.eventManager.addListener(this._listenerExit,this);

        CXModel.initNet( CXModel.roomData["gameRoom"]["way"]["ip"],Number(CXModel.roomData["gameRoom"]["way"]["port"]));
        CXModel.connectNet();

        // this.schedule(this.test,1.0);

        var _listenerHome2 = cc.EventListener.create({
                event: cc.EventListener.CUSTOM,
                eventName: "chat_sound_play_over_notify1",
                callback: function (evt) {
                    ZJHModel.getInstance().isPause = 0;
                }
            });
        cc.eventManager.addListener(_listenerHome2, this);

        return true;
    },


    test:function()
    {
        this.testCount++;
        CXModel.seatID = 0;
        

        if(this.testCount == 1)
        {
            CXAction.hideOutCardBtns();
            CXModel.jzView.setVisible(true);
            CXModel.boboView.setVisible(true);
            CXModel.CmbinationView.setVisible(true);

            CXPlayers.resetPlayerposition();
            for(var i =0;i<6;i++)
            {
                var player = CXPlayers.players[i];
                player.seatid = i;
                player.money = 1000;
                CXPlayers.uptable(player);
            }
        }
        if(this.testCount == 3)
        {

            // this.unschedule(this.test);
            // var nPlayer = CXPlayers.players[0];
            // nPlayer.startTimer(20,20);

            CXCards.fapaibegin();
            CXCards.resetCard();

        }else if(this.testCount == 5)
        {
            var delay = 0.2;
            for(var i =0;i<6;i++)
            {
                var pos = CXModel.getPosBySeatID(i);
                for(var j = 0;j<2;j++)
                {
                    delay += 0.05;
                    CXCards.showCards(pos,j,delay);
                }

            }
        }else if(this.testCount == 7)
        {
            var delay = 0.2;
            for(var i =0;i<6;i++)
            {
                delay += 0.15;
                var pos = CXModel.getPosBySeatID(i);
                
                CXCards.showCards(pos,2,delay);
            }
        }
        else if(this.testCount == 9)
        {
            var delay = 0.2;
            for(var i =0;i<6;i++)
            {
                delay += 0.15;
                var pos = CXModel.getPosBySeatID(i);
                CXCards.showCardValue(pos,2,delay,5);
            }
        }
        else if(this.testCount == 10)
        {
            var delay = 0.2;
            for(var i =0;i<6;i++)
            {
                delay += 0.15;
                var pos = CXModel.getPosBySeatID(i);
                CXCards.showCards(pos,3,delay);
            }
        }
        else if(this.testCount == 12)
        {
            var delay = 0.2;
            for(var i =0;i<6;i++)
            {
                delay += 0.25;
                var pos = CXModel.getPosBySeatID(i);
                CXCards.showCardValue(pos,3,delay,5);
            }
        }
        else if(this.testCount == 15)
        {
            CXCards.fapaiend();
        }
        else if(this.testCount == 17)
        {
            var delay = 0.2;
            for(var i =0;i<6;i++)
            {
                delay += 0.15;
                var pos = CXModel.getPosBySeatID(i);
                CXCards.reverseAllCards(pos,delay);
            }
        }else if(this.testCount == 20)
        {
            for(var i =0;i<6;i++)
            {
                var pos = CXModel.getPosBySeatID(i);
                CXCards.showCardValue(pos,0,0,5);
                CXCards.showCardValue(pos,1,0,5);
                CXCards.showCardType(pos,0,1,true);
            }
        }
        else if(this.testCount == 22)
        {
            for(var i =0;i<6;i++)
            {
                var pos = CXModel.getPosBySeatID(i);
                CXCards.showCardValue(pos,2,0,5);
                CXCards.showCardValue(pos,3,0,5);
                CXCards.showCardType(pos,1,2,true);
            }
        }
        else if(this.testCount == 25)
        {
            for(var i =0;i<6;i++)
            {
                var pos = CXModel.getPosBySeatID(i);
                CXChips.sendBetMoney(pos,50,6);
            }
        }

        else if(this.testCount == 27)
        {
            CXAction.showOutCardBtns();
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
        CXAction.uiNode =  this.getUI("btns");
        CXAction.uiNode.setLocalZOrder(99);
        CXAction.clickCB = this.onBtnClickEvt;
        CXAction.init();

        // cc.log("ti1:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();

        CXCards.uiNode =  this.getUI("cards");
        CXCards.uiNode.setLocalZOrder(97);
        CXCards.init();
        // cc.log("ti2:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();

        CXChips.uiNode =  this.getUI("cards");
        CXChips.uiNode.setLocalZOrder(98);
        CXChips.init();

        CXPlayers.uiNode =  this.getUI("players");
        CXPlayers.init();

        // cc.log("ti3:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();
        CXTips.uiNode =  this.getUI("tips");
        CXTips.uiNode.setLocalZOrder(100);
        CXTips.init();

        //加注的界面
        CXModel.jzView = new CXJiaZhuView();
        CXModel.jzView.setPosition(cc.p(260,0));
        CXModel.jzView.callBackAction = this.jzCallBack;
        this.addChild( CXModel.jzView,100);
        CXModel.jzView.setVisible(false);

        //设置簸簸界面
        CXModel.boboView  = new CXBoboView();
        CXModel.boboView.setPosition(cc.p(60,350));
        this.addChild( CXModel.boboView,100);
        CXModel.boboView.setVisible(false);

        //扯牌
        CXModel.CmbinationView  = new CXCmbinationView();
        CXModel.CmbinationView.setPosition(cc.p(110,340));
        this.addChild( CXModel.CmbinationView,100);
        CXModel.CmbinationView.setTouchEnable();
        CXModel.CmbinationView.setVisible(false);

        // cc.log("ti4:"+((new Date).valueOf()-b1));
        // b1 = (new Date).valueOf();
    },
    //心跳处理
    hearbeat:function( dt) {
        CXModel.sendHearbeat();
    },
    //联网监听
    updateGame:function( dt)
    {
        var curTime = (new Date).valueOf()/1000;
        //如果游戏退出到后台，返回前台时
        if(ZJHModel.getInstance().gamePuase)
        {
            ZJHModel.getInstance().gamePuase =false;
            if(curTime- ZJHModel.getInstance().pauseTime <=3)
            {
                return;
            }
            CXModel.waitUpdateInfo = true;

            if( CXModel.isConnect())
            {
                CXModel.resetResponse();
                CXModel.sendRoomInfo();

            }else{

                CXModel.connectNet();
            }

            return;
        }
        //心跳包超时
        if(CXModel.lastHearbeatTime != 0 && (curTime - CXModel.lastHearbeatTime ) > 10)
        {
            CXModel.connectNet();

            CXModel.lastHearbeatTime = curTime;
            if( this.retNetNum >0)
            {
                CXTips.showAllTip("网络连接异常，正在重试");
            }
            this.retNetNum ++;
            return;
        }
        var response = CXModel.getNetData();
        if(response != null)
        {
            if(response.cmd ==TCPSendCMD.DISCONNECT_RES || response.cmd ==TCPSendCMD.CONNECT_ERROR_RES
                || response.cmd ==TCPSendCMD.SEND_DATA_ERROR_RES|| response.cmd ==TCPSendCMD.RECV_DATA_ERROR_RES)
            {
                    cc.log("tcp error:"+response.cmd);
                    if(!CXModel.isLogout)
                    {
                        CXModel.connectNet();
                    }

                    if (CXModel.lastHearbeatTime == 0)
                    {
                        CXModel.lastHearbeatTime = utils.getTimeStamp();
                    }

            }
            else if( response.cmd ==TCPSendCMD.CONNECT_OK_RES)
            {
                cc.log("tcp connect succ!/n cmd:"+response.cmd +" data:"+response.data );
                if(!CXModel.isLogout)
                {
                    CXModel.sendLogin();
                }
                
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

        CXModel.resetData();//重置数据

        if(this.isResetGame)
        {
            return;
        }

        this.isResetGame = true;

        this.Advancefp = true;
        CXAction.reset();

        CXCards.reset();
        CXCards.resetCard();              //重置牌位置

        CXTips.reset();

        CXPlayers.reset(null);
        CXPlayers.resetPlayerposition();  //重置座位的
        CXPlayers.stopAllPlayTimer();

        CXModel.resetView();

        CXChips.v_recordChipNum();
        CXChips.reset();

        CXModel.resetPre();

        //我的id
        var player = CXModel.getPlayerBySeatID(CXModel.seatID);
       if(player)
       {
           ZJHModel.getInstance().rmb = player.rmb;
           ZJHModel.getInstance().money = player.money;
           ZJHModel.getInstance().coin = player.coin;
       }
    },

    onGameOver: function () {
        CXModel.isGameOver = true;
      if(CXModel.state == CXState.READY||CXModel.state == CXState.GAME_END)
      {
          var self  =this;
          // this.stopGameOver();
          var action = cc.sequence(cc.delayTime(7),cc.callFunc(function(){
              ZJHModel.getInstance().mySeatid = -1;
              CXModel.isGameOver = true;
              ZJHModel.getInstance().roomToShareView(CXModel.curRoomID);
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
        if(!CXModel.isMyInTable())
        {
            return;
        }
        var player = CXModel.getPlayerByUID(val["uid"]);
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
                var p = CXModel.getPlayerByUID(val["voters_uid"][i]);
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
        if(ZJHModel.getInstance().vcode > 20050){
            this.roomEndBox.setDefaultTips("若您超时未选择，则系统默认为\"反对解散\"");
        }
        this.roomEndBox.initWithJsonStr(JSON.stringify(json));
        this. addChild(this.roomEndBox,10000);

        this.roomEndBox.setCallback(function (str){
            if (str == "ok")
            {
                CXModel.sendRoomEnd(1);
            }
            else if (str == "no")
            {
                CXModel.sendRoomEnd(0);
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
            case "btn_ready":
                Sound.getInstance().playEffect(PackName+"/res/sound/com/button.mp3");
                CXModel.sendReady();
                break;
            case "btn_diu":
                Sound.getInstance().playEffect(PackName+"/res/sound/bet/bet_1.mp3");
                CXModel.sendBetRequest(PAYACTION.PLAYER_FOLD,CXModel.tobet);
                break;
            case "btn_xiu":
                Sound.getInstance().playEffect(PackName+"/res/sound/bet/bet_2.mp3");
                CXModel.sendBetRequest(PAYACTION.PLAYER_XIU,CXModel.tobet);
                break;
            case "btn_gen":
                {
                    var player = CXModel.players[CXModel.seatID];
                    if(player.bobo_money > CXModel.cur_gen_bet)
                    {
                        CXModel.sendBetRequest(PAYACTION.PLAYER_CALL,CXModel.tobet);
                    }else{
                        CXModel.sendBetRequest(PAYACTION.PLAYER_ALLIN,CXModel.tobet);
                    }
                    Sound.getInstance().playEffect(PackName+"/res/sound/bet/bet_3.mp3");
                }
                break;
            case "btn_da":
                {
                    if(!CXModel.myoptionfinish)
                    {
                        CXAction.hideOutCardBtnsSingle();
                        CXModel.jzView.refreshView();
                        CXModel.jzView.setVisible(true);
                    }
                }
                break;
            case "btn_qiao":
                Sound.getInstance().playEffect(PackName+"/res/sound/bet/bet_5.mp3");
                CXModel.sendBetRequest(PAYACTION.PLAYER_ALLIN,CXModel.tobet);
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
            CXModel.isGameOver = true;
            ZJHModel.getInstance().roomToShareView(CXModel.curRoomID);
            return;
        }

        if(CXModel.isGameover)
        {
            return;
        }

        if(CXModel.waitUpdateInfo && cmd !=NetCMD.SERVER_TABLE_INFO_UC)
        {
            return;
        }

        switch (cmd)
        {
            //心跳包
            case NetCMD.SERVER_HEART_BEAT_RESP:
                cc.log("SERVER_HEART_BEAT_RESP");
                CXModel.lastHearbeatTime = (new Date).valueOf()/1000;
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
                CXModel.isLogout = true;
                ZJHModel.getInstance().mySeatid = -1;
                CXModel.isGameOver = true;
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
                //设置bobo超时
            case NetCMD.SERVER_OPTION_TIMEOUT_BC:
                this.setBoboTimeOut(val);
                break;
            case NetCMD.SERVER_SET_DEALER_BC:
                this.setZang(val);
                break;
            case NetCMD.SERVER_FIRST_CARD_UC:  //一轮
                this.firstRound(val);
                break;
            case NetCMD.SERVER_SECOND_CARD_BC: //二轮
                this.secondRound(val);
                break;
            case NetCMD.SERVER_THIRD_CARD_BC:  //三轮
                this.thirdRound(val);
                break;
            case NetCMD.SERVER_NEXT_BET_BC:
                this.showNext_bet_bc(val);
                break;
            case NetCMD.SERVER_BET_SUCC_BC:
                this.sendBetSucc_bc(val);
                break;
            case NetCMD.SERVER_CHEPAI_BEGIN_BC:
                this.ChePaiBc(val);
                break;
            case NetCMD.SERVER_CHEPAI_RESULT_BC:
                this.ChePaiBc_SBC(val);
                break;
            case NetCMD.SERVER_ANALYSIS_RESULT_BC:
                this.AniSysCardType(val);
                break;
            case NetCMD.SERVER_CHEPAI_FINISH_BC:
                this.ChePaiFinish(val,true);
                break;
            case NetCMD.SERVER_BET_TIMEOUT_BC:  //下注超时
                this.BetTimeOut(val);
                break;
            case NetCMD.SERVER_CHEPAI_TIMEOUT_BC:
                this.setChePaiTimeOut(val);
                break;
            case NetCMD.SERVER_DZ_LOGIN_SUCC_BC:
                this.handlerLoginSuccBC(val);
                break;
            case NetCMD.SERVER_CHEPAI_SUCCESS_BC:
                this.handlerChePaiSuccBC(val);
                break;
            case NetCMD.SERVER_ALLIN_TCARDS_BC:
                this.handlerAllInCardsBC(val);
                break;
            case NetCMD.SERVER_CARDS_NAME_UC:
                this.handlerSpecialCardBC(val);
                break;
            case NetCMD.SERVER_OFFLINE_BC:
                this.handler_server_offline_bc(val);
                break;
            case NetCMD.SERVER_TESHU_CARDS_BC:
                this.ChePaiFinish(val,false);
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
        }
    },


    handler_server_offline_bc:function(val)
    {
        cc.log("SERVER_OFFLINE_BC");

        var seait = val["seatid"];
        var pos = CXModel.getPosBySeatID(seait); 
        var nplayer = CXPlayers.players[pos];
        if(nplayer != null)
        {
            nplayer.showUI("playlose",false);
            nplayer.showUI("plaything",false);

            nplayer.showUI("playlose",true);
        }
        
    },

    handlerAllInCardsBC:function(val)
    {
        cc.log("SERVER_ALLIN_TCARDS_BC");

        // log("=======myhandlecards====="+CXModel.myHandCard.length);
        // for(var m = 0; m <CXModel.myHandCard.length;m++)
        // {
        //     log("============CXModel.myHandCard======="+CXModel.myHandCard[m]);
        // }

        CXModel.allLastCard = val;
        if(val.hasOwnProperty("players"))
        {
            var playleng = val["players"].length;

            if(playleng > 0){
                if(val["players"][0].hasOwnProperty("cards"))
                {
                     CXCards.fapaibegin();
                }
            }
            this.Advancefp = false;
           
            var delay = 0.2;

            for(var i = 0;i<playleng;i++)
            {
                var d = val["players"][i];
                var seait = d["seatid"];
                var c_pos = CXModel.getPosBySeatID(seait);

                if(d.hasOwnProperty("cards"))
                {
                    var cardlong = 0;

                    if(CXModel.isMySeatID(seait))
                    {
                        cardlong = CXModel.myHandCard.length;
                    }
                    else
                    {
                        cardlong = CXModel.otherPlayCards[c_pos].length + 2;
                    }
                    var lencards = d["cards"].length;
                    

                    for(var j = 0;j<lencards;j++)
                    {
                        delay += 0.05;
                        var index = parseInt(cardlong+j);
                        CXCards.showCards(c_pos,index,delay);//发牌

                    }
                }
            }
            this.schedule(this.checkAllRoundFinish,1.0);
        }
    },

    checkAllRoundFinish:function(dt)
    {
        if(CXCards.cardNums <= 0)
        {
            this.unschedule(this.checkAllRoundFinish);
            var val = CXModel.allLastCard;
            if(val == null)
            {
                return;
            }
            var playleng = val["players"].length;

            for(var i = 0;i<playleng;i++)
            {
                var d = val["players"][i];
                var seait = d["seatid"];
                var c_pos = CXModel.getPosBySeatID(seait);

                if(d.hasOwnProperty("cards"))
                {
                    var lencards = d["cards"].length;
                    
                    for(var j = 0;j<lencards;j++)
                    {
                        if(CXModel.isMySeatID(seait))
                        {
                            var mycardleng = CXModel.myHandCard.length;
                            if(mycardleng <4)
                            {
                                 CXModel.myHandCard.push(d["cards"][j]);
                                CXModel.otherPlayCards[c_pos].push(d["cards"][j]);
                                mycardleng = CXModel.myHandCard.length;
                                log("======CXModel.myHandCard[mycardleng - 1]======="+CXModel.myHandCard[mycardleng - 1]);
                                CXCards.showCardValue(c_pos,mycardleng - 1,0.05,CXModel.myHandCard[mycardleng - 1]);
                            }
                           
                        }else{
                            var otcardleng = CXModel.otherPlayCards[c_pos].length;
                            if(otcardleng < 2)
                            {
                                CXModel.otherPlayCards[c_pos].push(d["cards"][j]);
                                otcardleng = CXModel.otherPlayCards[c_pos].length;
                                log("======CXModel.otherPlayCards[c_pos][otcardleng - 1]=======" + CXModel.otherPlayCards[c_pos][otcardleng - 1]+"pos"+c_pos);
                                CXCards.showCardValue(c_pos,otcardleng + 1,0.05,CXModel.otherPlayCards[c_pos][otcardleng - 1]);
                            }
                            
                        }
                    }
                }
            }
            this.Advancefp = true;
        }
    },


    handlerChePaiSuccBC:function(val)
    {
        cc.log("SERVER_CHEPAI_SUCCESS_BC");
        var seatid = val["seatid"];
        var pos = CXModel.getPosBySeatID(seatid);
        var nplayer = CXPlayers.players[pos];
        nplayer.showUI("playlose",false);
        nplayer.showUI("plaything",false);
        CXCards.addChePaiFinish(pos);  //完成扯牌

    },

    gameReadyStart:function(val)
    {
        cc.log("SERVER_READY_TIME_BC");
        CXTips.showLJXiu(false);
        CXTips.showTime(CXModel.ready_timer_stamp);
    },

    BetTimeOut:function(val)
    {
        cc.log("SERVER_BET_TIMEOUT_BC");
        var pos = CXModel.getPosBySeatID(CXModel.curSeatID); 
        var nplayer = CXPlayers.players[pos];

        nplayer.showUI("playlose",false);
        nplayer.showUI("plaything",false);

        nplayer.showUI("plaything",true);
    },


    ChePaiFinish:function(val,flag)
    {
        cc.log("SERVER_CHEPAI_FINISH_BC");
        if(flag)
        {
            CXModel.max_tou_pai_seatid = val["max_tou_seatid"];
            CXModel.max_wei_pai_seatid = val["max_wei_seatid"];
        }
        else
        {
            CXModel.max_tou_pai_seatid = -1;
            CXModel.max_wei_pai_seatid = -1;
        }
        

        var allseatid = [];
        if(val.hasOwnProperty("players"))
        {
            var length = val["players"].length;
            for(var i = 0;i<length;i++)
            {
                var vals = val["players"][i];
                if(!vals.hasOwnProperty("cards"))
                {
                    continue;
                }

                var seatid = vals["seatid"];
                var pos = CXModel.getPosBySeatID(seatid);
                allseatid.push(seatid);

                var vplayer = CXModel.players[seatid];
                var tou_card_name = vals["tou_cards_name"];
                var wei_card_name = vals["wei_cards_name"];

                vplayer.tou_cards_name = tou_card_name;
                vplayer.wei_cards_name = wei_card_name;

                var lens = vals["cards"].length;
                if(CXModel.isMySeatID(seatid))
                {
                    CXModel.myHandCard = [];
                    CXModel.otherPlayCards[pos] = [];

                    for(var k = 0; k < lens; k++)
                    {
                        CXModel.myHandCard.push(vals["cards"][k]);
                        CXModel.otherPlayCards[pos].push(vals["cards"][k]);
                    }
                }
                else
                {
                    CXModel.otherPlayCards[pos] = [];
                    for(var v = 0; v < lens; v++)
                    {
                        CXModel.otherPlayCards[pos].push(vals["cards"][v]);
                    }
                }
            }
        }
        this.ChepaiStart(allseatid,flag);
    },


    ChepaiStart:function(seatids,flag)
    {

        CXCards.removeFinishCPFlag();
        if(flag)
        {
            CXTips.showTipWei(true);   //尾  wei_cards_name
        }
        
        var show_tou = cc.callFunc(function(){
           
            var length = seatids.length;
            for(var i =0;i<length;i++)
            {

                var _player = CXModel.players[seatids[i]];
                var cpos = CXModel.getPosBySeatID(seatids[i]);
                var len = CXModel.otherPlayCards[cpos].length;
                if(len > 2)
                {

                    if(!CXModel.isMySeatID(seatids[i]))
                    {
                        if(_player.wei_cards_name == 0 || _player.wei_cards_name == 1)
                        {
                            CXCards.showCardValue(cpos,0,0,CXModel.otherPlayCards[cpos][2]);
                            if(CXModel.otherPlayCards[cpos].length > 3)
                            {
                                CXCards.showCardValue(cpos,1,0,CXModel.otherPlayCards[cpos][3]);
                            }
                        }
                        else
                        {
                            CXCards.showCardValueAndPos(cpos,0,0,CXModel.otherPlayCards[cpos][2]);
                            CXCards.showCardValueAndPos(cpos,1,0,CXModel.otherPlayCards[cpos][3]);
                        }
                        
                    }
                    else
                    {
                        if(_player.wei_cards_name != 0 && _player.wei_cards_name != 1)
                        {
                            CXCards.showCardValueAndPos(cpos,0,0,CXModel.otherPlayCards[cpos][2]);
                            CXCards.showCardValueAndPos(cpos,1,0,CXModel.otherPlayCards[cpos][3]);
                        }
                            
                    }
                    
                }

                
               if(_player.wei_cards_name != 0 && _player.wei_cards_name != 1)
               {    

                    if(_player.seatid == CXModel.max_wei_pai_seatid) //max_tou_pai_seatid
                    {
                        if(CXModel.isMySeatID(seatids[i]))
                        {
                            CXCards.changePlayerCardState(cpos,0,_player.wei_cards_name,true);
                        }else{
                            CXCards.showCardType(cpos,0,_player.wei_cards_name,true);
                        }
                        
                    }else{

                        if(CXModel.isMySeatID(seatids[i]))
                        {
                            CXCards.changePlayerCardState(cpos,0,_player.wei_cards_name,false);
                        }else{
                            CXCards.showCardType(cpos,0,_player.wei_cards_name,false);
                        } 
                    }
               }

                
            }
        });

        var delay = cc.delayTime(1);

        var show_wei = cc.callFunc(function(){  
        if(flag)
        {
            CXTips.showTipTou(true);   //头 tou_cards_name
        }
             
            
            var length = seatids.length;
            for(var i =0;i<length;i++)
            {

                var cpos = CXModel.getPosBySeatID(seatids[i]);
                var len = CXModel.otherPlayCards[cpos].length;
                 var _player = CXModel.players[seatids[i]];
                if(len > 0)
                {
                    if(!CXModel.isMySeatID(seatids[i]))
                    {

                        if(_player.tou_cards_name == 0 || _player.tou_cards_name == 1)
                        {
                           if(CXModel.otherPlayCards[cpos].length > 3)
                           {
                                CXCards.showCardValue(cpos,2,0,CXModel.otherPlayCards[cpos][0]);
                                CXCards.showCardValue(cpos,3,0,CXModel.otherPlayCards[cpos][1]);

                           }else{

                                CXCards.showCardValue(cpos,1,0,CXModel.otherPlayCards[cpos][0]);
                                CXCards.showCardValue(cpos,2,0,CXModel.otherPlayCards[cpos][1]);
                           }
                            
                        }
                        else{
                            
                            CXCards.showCardValueAndPos(cpos,2,0,CXModel.otherPlayCards[cpos][0]);
                            CXCards.showCardValueAndPos(cpos,3,0,CXModel.otherPlayCards[cpos][1]);
                        }
                        
                    }else
                    {
                        if(_player.tou_cards_name != 0 && _player.tou_cards_name != 1)
                        {
                             
                            CXCards.showCardValueAndPos(cpos,2,0,CXModel.otherPlayCards[cpos][0]);
                            CXCards.showCardValueAndPos(cpos,3,0,CXModel.otherPlayCards[cpos][1]);
                        }
                        
                    }
                    
                }

                
                if(_player.tou_cards_name != 0 && _player.tou_cards_name != 1)
                {
                    if(_player.seatid == CXModel.max_tou_pai_seatid)  //max_wei_pai_seatid
                    {
                        if(CXModel.isMySeatID(seatids[i]))
                        {
                            
                            CXCards.changePlayerCardState(cpos,1,_player.tou_cards_name,true);
                        }else{
                             
                             CXCards.showCardType(cpos,1,_player.tou_cards_name,true);
                        }
                       
                    }else{
                        if(CXModel.isMySeatID(seatids[i]))
                        {
                            
                            CXCards.changePlayerCardState(cpos,1,_player.tou_cards_name,false);
                        }else{
                            CXCards.showCardType(cpos,1,_player.tou_cards_name,false);
                        }
                        
                    }
                }else
                {
                    if(!CXModel.isMySeatID(seatids[i]))
                    {   
                     CXCards.showCardType(cpos,2,_player.tou_cards_name,true);
                    }
                }
                
            }
        });

        CXTips.showWaitTip("");
        this.runAction(cc.sequence(show_tou,delay,show_wei));
    },


    AniSysCardType:function(val)
    {
        cc.log("SERVER_ANALYSIS_RESULT_UC");
        var tou = val["tou_cards_name"];
        var wei = val["wei_cards_name"];

        if(val.hasOwnProperty("cards"))
        {
            var len = val["cards"].length;
            var temcard = [];
            for(var i =0;i<len;i++)
            {
                temcard.push(val["cards"][i]);
            }
            if((CXModel.myHandCard[0] == temcard[0] || CXModel.myHandCard[0] == temcard[1]) && (CXModel.myHandCard[1] == temcard[0] || CXModel.myHandCard[1] == temcard[1]))
            {
                CXModel.CmbinationView.showResultCardType(tou,wei);
            }else{
                CXModel.CmbinationView.showResultCardType(wei,tou);
            }
        }
        else{
            CXModel.CmbinationView.showResultCardType(tou,wei);
        }
       
        

    },

    //我的牌型
    ChePaiBc_SBC:function(val)
    {
        cc.log("SERVER_CHEPAI_RESULT_BC");

        var seatid = val["seatid"];
        var tou = val["tou_cards_name"];
        var wei = val["wei_cards_name"];
        var pos = CXModel.getPosBySeatID(seatid);

        var nplayer = CXPlayers.players[pos];

        nplayer.showUI("playlose",false);
        nplayer.showUI("plaything",false);

        if(CXModel.isMySeatID(seatid))
        {
            var leng  = val["cards"].length;
            CXModel.myHandCard = [];
            for(var k = 0;k<leng;k++)
            {
                CXModel.myHandCard.push(val["cards"][k]);
            }

            CXModel.CmbinationView.setVisible(false);
            CXModel.CmbinationView.setTouchEnable();
            for(var i =0; i<2;i++)
            {
                CXCards.showCardValue(pos,i,0,CXModel.myHandCard[i+2]);
            }

            // CXCards.showCardType(pos,0,tou,false);
            CXCards.showCardType(pos,0,wei,false);

            for(var i =2; i<4;i++)
            {
                CXCards.showCardValue(pos,i,0,CXModel.myHandCard[i-2]);
            }
            // CXCards.showCardType(pos,1,wei,false);
            CXCards.showCardType(pos,1,tou,false);
        }

        log("=========================finish hcepai==============");
        CXCards.addChePaiFinish(pos);  //完成扯牌
    },

    ChePaiBc:function(val)
    {
        cc.log("SERVER_CHEPAI_BEGIN_BC");

        CXModel.state == CXState.CHEPAI;
        this.cpGameData = val;
        this.schedule(this.FinishGameFp,1.0);

         
    },

    FinishGameFp:function(dt)
    {
        if(this.Advancefp == true)
        {
            this.unschedule(this.FinishGameFp);

            CXTips.showWaitTip("等待玩家扯牌");

            var val = this.cpGameData;
            if(val.hasOwnProperty("chepai_seatid"))
            {
                var lengths = val["chepai_seatid"].length;
                var delay = 0.2;
                for(var k = 0; k< lengths; k++)
                {
                   delay += 0.05;
                   var seat  =  val["chepai_seatid"][k];
                   var _pos = CXModel.getPosBySeatID(seat);
                   CXCards.reverseAllCards(_pos,delay);

                }
            }

            var flag = false;
            if(val.hasOwnProperty("chepai_seatid"))
            {
                var lengths = val["chepai_seatid"].length;
                for(var k = 0; k< lengths; k++)
                {
                   var seat  =  val["chepai_seatid"][k];
                   if(parseInt(CXModel.seatID) == parseInt(seat))
                   {
                        flag = true;
                        break;
                   }
                }
            }

            var player = CXModel.getPlayerBySeatID(CXModel.seatID);
            if(flag)
            {
                CXModel.CmbinationView.refreshView(CXModel.myHandCard);
                CXModel.CmbinationView.setVisible(true);
            }
        }
        
    },

//加注按钮的回调
    jzCallBack:function(type,betNum)
    {
        if(type == 1)
        {
             var player = CXModel.players[CXModel.seatID];
             var lastsendAllMon = betNum - player.bet;
             if(lastsendAllMon < player.bobo_money)
             {
                Sound.getInstance().playEffect(PackName+"/res/sound/bet/bet_4.mp3");
                CXModel.sendBetRequest(PAYACTION.PLAYER_RAISE,betNum);
             }
             else
             {
                Sound.getInstance().playEffect(PackName+"/res/sound/bet/bet_5.mp3");
                CXModel.sendBetRequest(PAYACTION.PLAYER_ALLIN,CXModel.tobet);
             }
        }
        else
        {
            CXModel.jzView.setVisible(false);
            CXAction.showOutCardBtnsSigle();
        }
    },

//当前的下注操作
    showNext_bet_bc:function(val)
    {
        cc.log("SERVER_NEXT_BET_BC");

        var seait = val["seatid"];
        CXModel.curSeatID = seait;
        CXModel.cur_bet = val["cur_bet"];
        CXModel.max_raidbet = val["cur_raise_bet"];
        CXModel.isxiu = utils.getValBool(val,"is_xiu");
        CXModel.max_raise_bet = utils.getValInt(val, "max_raise_bet");

        var option_bet = val["option_bet"];

        var pos = CXModel.getPosBySeatID(seait);
        var player = CXModel.players[seait];
        var nplayer = CXPlayers.players[pos];
        nplayer.startTimer(CXModel.bet_timer_stamp,CXModel.bet_timer_stamp);

        if(CXModel.isMySeatID(seait))
        {
            CXModel.myoptionfinish = false;

            Sound.getInstance().playEffect(PackName+"/res/sound/com/my_turn.mp3"); 

            CXAction.showBtnEnable("btn_xiu",true);
            CXAction.showBtnEnable("btn_gen",true);
            CXAction.showBtnEnable("btn_qiao",true);
            CXAction.showBtnEnable("btn_da",true);


            if(CXModel.isxiu)
            {
                CXAction.showBtnEnable("btn_xiu",true);
            }else{
                CXAction.showBtnEnable("btn_xiu",false);
            }

            // var NoutBet = CXModel.cur_bet + CXModel.base_money;   //最小的加注额
            // if(NoutBet > CXModel.max_raise_bet)
            // {
            //     CXAction.showBtnEnable("btn_da",false);
            // }

            var am = parseInt(player.bet) + parseInt(player.bobo_money);
            if(CXModel.cur_bet >= am)
            {
                CXAction.showBtnEnable("btn_da",false);
            }

            
            if(CXModel.cur_bet == 0)
            {
                CXAction.showBtnEnable("btn_gen",false);
                CXAction.setGenValue(0);
            }
            else
            {
                var canMon = parseInt(player.bobo_money) + parseInt(player.bet);
                if(canMon > CXModel.cur_bet)
                {

                    CXAction.showBtnEnable("btn_gen",true);
                    CXModel.cur_gen_bet = parseInt(CXModel.cur_bet) - parseInt(player.bet);
                    CXAction.setGenValue(CXModel.cur_gen_bet);

                }else{
                    CXAction.showBtnEnable("btn_gen",false);
                    CXAction.setGenValue(0);
                }
            }

            // var allmon = parseInt(player.bobo_money) + parseInt(player.bet);
            // if(allmon > CXModel.max_raise_bet)
            // {
            //     CXAction.showBtnEnable("btn_qiao",false);
            // }

            if(parseInt(player.bobo_money) <= 0)
            {
                CXAction.showBtnEnable("btn_da",false);
                CXAction.showBtnEnable("btn_gen",false);
                CXAction.setGenValue(0);
                CXAction.showBtnEnable("btn_qiao",false);
            }

           
            CXAction.showOutCardBtns();
        }
    },

 //下注结果的的返回
    sendBetSucc_bc:function(val)
    {
        cc.log("SERVER_BET_SUCC_BC");

        var seatid = val["seatid"];
        var action = val["action"];
        var bet = val["bet"];
        var option_bet = val["option_bet"];
        var total_bet = val["total_bet"];
        var bobomoney = val["bobo_money"];

        CXTips.showSpecialCard(false,"");

        if(CXModel.isMySeatID(seatid))
        {
            CXModel.myoptionfinish = true;
            CXAction.hideOutCardBtns();
            CXModel.jzView.setVisible(false);
        }

        var player = CXModel.players[seatid];
        if(player != null)
        {
            player.total_bet = total_bet;
            player.bet = bet;
            player.bobo_money = bobomoney;

            var pos = CXModel.getPosBySeatID(seatid);
            if(option_bet > 0)
            {
                var num = Math.ceil(option_bet/CXModel.stand_bobo);  
                CXChips.sendBetMoney(pos,CXModel.stand_bobo,num,player.total_bet);
            }
            CXPlayers.players[pos].updatePlayerMoney();
        }

        var pos = CXModel.getPosBySeatID(seatid);
        var nplayer = CXPlayers.players[pos];
        nplayer.stopTimer();

        nplayer.showUI("playlose",false);
        nplayer.showUI("plaything",false);

        if(action == 2002)   //大
        {
            nplayer.showDa();
        }
        else if(action == 2001)   //跟
        {
            nplayer.showGen();
        }
        else if(action == 2003)   //敲
        {
            nplayer.showQiao();
        }
        else if(action == 2004)
        {
             nplayer.showXiu();
        }
        else if(action == 2005)
        {
            //丢的结果返回
            nplayer.setTOpacity(100);
            CXCards.SetOpacityCardForPos(pos);
        }

    },

    //--------------------------------------------第一轮发牌-----------------------
    firstRound:function(val)
    {

        cc.log("SERVER_FIRST_CARD_UC");

        CXModel.state == CXState.FIRST_ROUND;

        CXTips.showMyAssets(true);

        if(val.hasOwnProperty("cards"))
        {
            var leng = val["cards"].length;
            for(var i = 0; i< leng;i++)
            {
                CXModel.myHandCard.push(val["cards"][i]);
            }
        }
        this.schedule(this.checkCardFinish,1.0);


    },

    checkCardFinish:function(dt) {
        if(CXCards.cardNums <= 0)
        {
            this.unschedule(this.checkCardFinish);

            var showCard = cc.callFunc(function(){
                    var length = CXModel.myHandCard.length;
                    var pos = CXModel.getPosBySeatID(CXModel.seatID);
                    var playerer = CXModel.players[CXModel.seatID];
                    if(playerer != null)
                    {
                        playerer.bet = 0;
                    }
                    var delay = 0.1;
                    for(var i =0; i< length; i++)
                    {
                        CXCards.showCardValue(pos,i,delay,CXModel.myHandCard[i]);
                    }
                });
            this.runAction(cc.sequence(cc.delayTime(0.5),showCard));
        }
    },


    //--------------------------------------------第二轮发牌-----------------------

    secondRound:function(val)
    {
        cc.log("SERVER_SECOND_CARD_BC");

        CXModel.state == CXState.SECOND_ROUND;

        if(val.hasOwnProperty("players"))
        {
            CXCards.fapaibegin();
            var delay = 0.2;
            var leng = val["players"].length;

            for(var  i = 0;i <leng; i++)
            {
                delay += 0.05;

                var seatid = val["players"][i]["seatid"];
                var c_pos = CXModel.getPosBySeatID(seatid);
                var cardValue = val["players"][i]["card"];
                
                var playerer = CXModel.players[seatid];
                if(playerer != null)
                {
                    playerer.bet = 0;
                }

                CXCards.showCards(c_pos,2,delay); //发牌


                if(CXModel.isMySeatID(seatid))
                {
                    CXModel.myHandCard.push(cardValue);
                    CXModel.otherPlayCards[c_pos].push(cardValue);

                }else{
                    CXModel.otherPlayCards[c_pos].push(cardValue);
                }
            }

            this.schedule(this.checkSecondRoundFinish,1.0);
        }
    },

    checkSecondRoundFinish:function(dt)
    {
        if(CXCards.cardNums <= 0)
        {
            this.unschedule(this.checkSecondRoundFinish);
            var delay = 0.2;
            for(var i =0;i<6;i++)
            {
                var num = CXModel.otherPlayCards[i].length;
                if(num > 0)
                {
                    CXCards.showCardValue(i,2,delay,CXModel.otherPlayCards[i][0]);
                }
            }
        }
    },



    handlerSpecialCardBC:function(val)
    {
        log("SERVER_CARDS_NAME_UC");
        var c_pos = CXModel.getPosBySeatID(CXModel.seatID);
        var sp_cards_name = val["cards_name"];
        if(sp_cards_name == 0)
        {
            var carname = "三花六";
            CXTips.showSpecialCard(true,carname);
        }else if(sp_cards_name == 1)
        {
             var carname = "三花十";
             CXTips.showSpecialCard(true,carname);
        }else{

            CXTips.showSpecialCard(false,"");
        }

        CXModel.speCardType[c_pos] = sp_cards_name;


        if(CXModel.speCardType[c_pos] != -1)
        {
            CXCards.showCardType(c_pos,2,CXModel.speCardType[c_pos],true);
        }else
        {
            CXCards.removeSpeCardType(c_pos,2);
        }

        
    },

   //--------------------------------------------第三轮发牌-----------------------

   thirdRound:function(val)
    {
        cc.log("SERVER_THIRD_CARD_BC");

        CXModel.state == CXState.THIRD_ROUND;

        if(val.hasOwnProperty("players"))
        {
            CXCards.fapaibegin();
            var delay = 0.2;
            var leng = val["players"].length;

            for(var  i = 0;i <leng; i++)
            {
                delay += 0.05;

                var seatid = val["players"][i]["seatid"];
                var c_pos = CXModel.getPosBySeatID(seatid);
                var cardValue = val["players"][i]["card"];

                var playerer = CXModel.players[seatid];
                if(playerer != null)
                {
                    playerer.bet = 0;
                }

                CXCards.showCards(c_pos,3,delay); //发牌

                if(CXModel.isMySeatID(seatid))
                {
                    CXModel.myHandCard.push(cardValue);
                    CXModel.otherPlayCards[c_pos].push(cardValue);

                }else{
                    CXModel.otherPlayCards[c_pos].push(cardValue);
                }

                
            }

            this.schedule(this.checkThirdRoundFinish,1.0);
        }
    },

    checkThirdRoundFinish:function(dt)
    {
        if(CXCards.cardNums <= 0)
        {
            this.unschedule(this.checkThirdRoundFinish);
            var delay = 0.2;
            for(var i =0;i<6;i++)
            {
                var num = CXModel.otherPlayCards[i].length;
                if(num > 1)
                {
                    CXCards.showCardValue(i,3,delay,CXModel.otherPlayCards[i][1]);
                }
            }
        }
    },


    //发牌确定庄家
    sureBanker:function(val)
    {
        if(val.hasOwnProperty("seats"))
        {
            CXCards.fapaibegin(true);
            CXModel.dealer = 0;
            var leng = val[seats].length;
            var delay = 0.2;
            for(var i = 0;i<leng;i++)
            {
                delay += 0.1;
                var seat = val[seats][i]["seatid"];
                var pos = CXModel.getPosBySeatID(seat);
                var cardValue = val[seats][i]["cards"];
                CXCards.showSureBankerCard(pos,1,delay,cardValue,cc.callFunc(function(){
                }));
            }
        }
    },

//设置簸簸超时
    setBoboTimeOut:function(val)
    {
        cc.log("=====SERVER_OPTION_TIMEOUT_BC=====");
        if(val.hasOwnProperty("players"))
        {
            var leng = val["players"].length;
            for(var i = 0;i<leng;i++)
            {
                var seat = val["players"][i]["seatid"];
                var offline = utils.getValBool(val["players"][i], "offline");

                var pos = CXModel.getPosBySeatID(seat);
                var nplayer = CXPlayers.players[pos];

                nplayer.showUI("playlose",false);
                nplayer.showUI("plaything",false);
                if(offline)
                {
                    nplayer.showUI("playlose",true);
                }else{
                    nplayer.showUI("plaything",true);
                }
            }
        }
    },

    //扯牌超时
    setChePaiTimeOut:function(val)
    {
        cc.log("=====SERVER_OPTION_TIMEOUT_BC=====");
        if(val.hasOwnProperty("players"))
        {
            var leng = val["players"].length;
            for(var i = 0;i<leng;i++)
            {
                var seat = val["players"][i]["seatid"];
                var offline = utils.getValBool(val["players"][i], "offline");

                var pos = CXModel.getPosBySeatID(seat);
                var nplayer = CXPlayers.players[pos];

                nplayer.showUI("playlose",false);
                nplayer.showUI("plaything",false);
                if(offline)
                {
                    nplayer.showUI("playlose",true);
                }else{
                    nplayer.showUI("plaything",true);
                }
            }
        }
    },

    First_Round_Bc:function(val)
    {
        cc.log("=====SERVER_FIRST_ROUND_UC====");

        JsUtils.postNotifi("notify_game_check_end"); //清除邀请

        this.resetGame();
        CXTips.hideTime();
        CXTips.showWaitTip("");

        CXCards.fapaibegin();
        var delay = 0.2;
        for(var i =0;i<6;i++)
        {
            var player = CXModel.getPlayerByPos(i)
            if(player != null && player.betting == 1 && player.seatid != -1)
            {
                for(var j = 0;j<2;j++)
                {
                    delay += 0.1;
                    CXCards.showCards(i,j,delay);
                } 
            }
        }

    },

//设置庄家
    setZang:function(val)
    {
        cc.log("=====setZang====");

        CXPlayers.resetZang();

        CXModel.dealer = val["dealer"];
        if(CXModel.dealer > -1)
        {
            var pos = CXModel.getPosBySeatID(CXModel.dealer);
            var player = CXPlayers.players[pos]; 
            player.showzang(pos);
        }
    },

//设置簸簸成功
    setBoboSuc:function(val)
    {
        cc.log("add bobo success");

        var mybobo = utils.getValInt(val,"bobo_money");
        var mymoney = utils.getValInt(val,"money");
        var seatid =  utils.getValInt(val,"seatid");   

        if(CXModel.isMySeatID(seatid))
        {
            CXModel.boboView.setVisible(false);
        }

        var player = CXModel.players[seatid];

        if(player == undefined || player == null)
        {
            return;
        }

        player.money = mymoney;
        player.bobo_money = mybobo;

        var pos = CXModel.getPosBySeatID(seatid);
        var nplayer = CXPlayers.players[pos];

        nplayer.stopTimer();

        nplayer.showUI("playlose",false);
        nplayer.showUI("plaything",false);

        nplayer.updatePlayerMoney();

        if(CXModel.isMySeatID(seatid))
        {
            CXTips.showMyAssets(true);
        }
    },
    
    //回调：房间信息
    handlerTableInfoUC:function(val) {
        cc.log("handlerTableInfoUC");

        CXModel.waitUpdateInfo = false;
        CXModel.reset();

        this.isResetGame = false;
        this.resetGame();

        CXPlayers.downtable();

        CXModel.roomGolden = utils.getValInt(val, "takein");
        CXModel.state = utils.getValInt(val, "state");
        CXModel.seatID = utils.getValInt(val, "seatid");


        
        CXModel.pauseTime = utils.getValInt(val, "game_pause_tstamp");

        CXModel.base_money = utils.getValInt(val, "base_money");
        CXModel.bet_timer_stamp = utils.getValInt(val, "bet_timer_stamp");
        CXModel.chepai_timer_stamp = utils.getValInt(val, "chepai_timer_stamp");
        CXModel.curSeatID = utils.getValInt(val, "cur_seat");
        CXModel.cur_bet =  utils.getValInt(val, "cur_bet");
        CXModel.is_bet_timeout =  utils.getValBool(val, "is_bet_timeout");
        CXModel.is_option_timeout = utils.getValBool(val, "is_option_timeout");
        CXModel.is_xiu = utils.getValBool(val, "is_xiu");
        CXModel.dealer = utils.getValInt(val, "dealer");
        CXModel.left_bet_timer = utils.getValInt(val, "left_bet_timer");
        CXModel.left_chepai_timer = utils.getValInt(val, "left_chepai_timer");
        CXModel.left_ready_timer = utils.getValInt(val, "left_ready_timer");
        CXModel.left_set_bobo_timer = utils.getValInt(val, "left_set_bobo_timer");
        CXModel.max_raise_bet = utils.getValInt(val, "max_raise_bet");

        CXModel.preready_timer_stamp = utils.getValInt(val, "preready_timer_stamp");
        CXModel.ready_timer_stamp = utils.getValInt(val, "ready_timer_stamp");
        CXModel.set_bobo_timer_stamp = utils.getValInt(val, "set_bobo_timer_stamp");
        CXModel.stand_money = utils.getValInt(val, "stand_money");
        CXModel.min_bobo_money = utils.getValInt(val, "min_bobo_money");

        CXModel.stand_bobo = 20;

        CXModel.cur_pi = utils.getValInt(val, "cur_pi");

        CXModel.isxiu = utils.getValBool(val,"is_xiu"); //这一轮是否休

        ZJHModel.getInstance().mySeatid = CXModel.seatID;

        if(val.hasOwnProperty("max_tou_seatid"))
        {
            CXModel.max_tou_pai_seatid = utils.getValBool(val,"max_tou_seatid");
        }
        if(val.hasOwnProperty("max_wei_seatid"))
        {
            CXModel.max_wei_pai_seatid = utils.getValBool(val,"max_wei_seatid");
        }


        //提示恢复
        CXPlayers.resetPlayerposition();  //重置座位的
        CXCards.resetCard();              //重置牌位置

        log("=============CXModel.state=================="+CXModel.state);
        //上桌玩家信息
        var len = 0;
        if (val.hasOwnProperty("players")) {
            len = val["players"].length;
            for (var i = 0; i < len; i++)
            {
                var d = val["players"][i];
                var player = CXModel.updatePlayer(d);
                CXPlayers.uptable(player);

                var pos = CXModel.getPosBySeatID(player.seatid);
                var nPlayers = CXPlayers.players[pos];


                if(CXModel.isMyUID(player.uid))
                {
                    CXTips.showMyAssets(true);
                }

                if(d.hasOwnProperty("cards"))
                {
                    var carleng = d["cards"].length;
                    for(var j = 0;j<carleng;j++)
                    {
                        if(CXModel.isMySeatID(player.seatid))
                        {
                            CXModel.myHandCard.push(d["cards"][j]);
                            if(j >1)
                            {
                                CXModel.otherPlayCards[pos].push(d["cards"][j]);
                            }
                        }
                        else
                        {
                            CXModel.otherPlayCards[pos].push(d["cards"][j]);
                        }
                    }
                }

                if(player.isOffline)
                {
                    nPlayers.showUI("playlose",true);
                }
                else
                {
                    if(CXModel.state == CXState.GAME_STATE)
                    {
                        if(player.betting == 1 && player.ready == 1 && CXModel.is_option_timeout && !player.issetbobo)
                        {
                            nPlayers.showUI("plaything",true);
                        }
                        
                    }
                    else if(CXModel.state == CXState.CHEPAI)
                    {
                        if(player.betting == 1 && player.ready == 1 && CXModel.is_option_timeout && !player.is_chepai_finish)
                        {
                            nPlayers.showUI("plaything",true);
                        }
                    }
                    else if(CXModel.state == CXState.FIRST_ROUND || CXModel.state == CXState.SECOND_ROUND || CXModel.state == CXState.THIRD_ROUND)
                    {
                        if(player.betting == 1 && player.ready == 1 && CXModel.is_option_timeout && CXModel.curSeatID == player.seatid)
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
                if (CXModel.state == CXState.READY) {
                    CXPlayers.setReady(pos, player.ready == 1);
                    if (CXModel.isMyUID(player.uid)) {
                        if(player.ready != 1)
                        {
                            nPlayers.showUI("playgray",true);
                            CXAction.showReadyBtn(true);
                        }
                        else
                        {
                            CXAction.showReadyBtn(false);
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

                if(CXModel.state == CXState.GAME_STATE)  //设置簸簸的界面
                {
                    //判断是否要弹出设置的bobo
                    if(CXModel.isMySeatID(player.seatid))
                    {
                        if(player.ready == 1 && !player.issetbobo)
                        {
                            CXModel.boboView.refreshView();
                            CXModel.boboView.setVisible(true);
                        }
                    }

                }

                if(player.ready == 1 && CXModel.state == CXState.FIRST_ROUND)  //第一轮
                {
                    //展示两张牌
                    if(CXModel.isMySeatID(player.seatid))
                    {
                        for(var k =0; k < 2; k++)
                        {
                           CXCards.showCardValue(pos,k,0,CXModel.myHandCard[k]);
                        }
                    }
                    else
                    {
                        for(var k =0; k < 2; k++)
                        {
                           CXCards.showCardValue(pos,k,0,-1);
                        }
                    }

                    if(player.total_bet > 0)
                    {
                        var num = Math.ceil(player.total_bet/CXModel.stand_bobo);  
                        CXChips.sendBetMoney(pos,CXModel.stand_bobo,num,player.total_bet);
                    }

                    if(player.ready == 1 && player.betting != 1)
                    {
                        nPlayers.setTOpacity(100);
                        CXCards.SetOpacityCardForPos(pos);
                    }

                }


                if(player.ready == 1 && CXModel.state == CXState.SECOND_ROUND)  //第二轮
                {
                    //展示两张牌
                    if(CXModel.isMySeatID(player.seatid))
                    {
                        var leng = CXModel.myHandCard.length;
                        for(var k =0; k < leng; k++)
                        {
                           CXCards.showCardValue(pos,k,0,CXModel.myHandCard[k]);
                        }

                        if(player.cards_name == 0 || player.cards_name == 1)
                        {
                            CXCards.showCardType(pos,2,player.cards_name,true);
                        }
                    }
                    else
                    {
                        for(var k = 0; k< 2; k++)
                        {
                           CXCards.showCardValue(pos,k,0,-1);
                        }
                        if(CXModel.otherPlayCards[pos].length > 0)
                        {
                            CXCards.showCardValue(pos,2,0,CXModel.otherPlayCards[pos][0]);
                        }
                        
                    }

                    var num = Math.ceil(player.total_bet/CXModel.stand_bobo);  
                    CXChips.sendBetMoney(pos,CXModel.stand_bobo,num,player.total_bet);


                    if(player.ready == 1 && player.betting != 1)
                    {
                        nPlayers.setTOpacity(100);
                        CXCards.SetOpacityCardForPos(pos);
                    }

                }

                if(player.ready == 1 && CXModel.state == CXState.THIRD_ROUND)  //第三轮
                {
                    //展示两张牌
                    if(CXModel.isMySeatID(player.seatid))
                    {
                        var leng = CXModel.myHandCard.length;
                        for(var k =0;k<leng;k++)
                        {
                           CXCards.showCardValue(pos,k,0,CXModel.myHandCard[k]);
                        }

                        if(player.cards_name == 0 || player.cards_name == 1)
                        {
                            CXCards.showCardType(pos,2,player.cards_name,true);
                        }
                    }
                    else
                    {
                        for(var k =0;k<2;k++)
                        {
                           CXCards.showCardValue(pos,k,0,-1);
                        }
                        if(CXModel.otherPlayCards[pos].length > 0)
                        {
                            CXCards.showCardValue(pos,2,0,CXModel.otherPlayCards[pos][0]);
                        }
                        if(CXModel.otherPlayCards[pos].length > 1)
                        {
                            CXCards.showCardValue(pos,3,0,CXModel.otherPlayCards[pos][1]);
                        }
                        
                    }

                    var num = Math.ceil(player.total_bet/CXModel.stand_bobo);  
                    CXChips.sendBetMoney(pos,CXModel.stand_bobo,num,player.total_bet);


                    if(player.ready == 1 && player.betting != 1)
                    {
                        nPlayers.setTOpacity(100);
                        CXCards.SetOpacityCardForPos(pos);
                    }


                }

                //扯牌的阶段

                if(CXModel.state == CXState.CHEPAI)  
                {


                    if(CXModel.isMySeatID(player.seatid))
                    {
                        if(player.ready == 1)
                        {
                            if(!player.is_chepai_finish)
                            {
                                if(player.betting == 1)
                                {
                                    CXCards.reverseAllCards(pos,0);
                                    CXModel.CmbinationView.refreshView(CXModel.myHandCard);
                                    CXModel.CmbinationView.setVisible(true);
                                }
                                else
                                {
                                    var leng = CXModel.myHandCard.length;
                                    for(var k =0;k<leng;k++)
                                    {
                                        CXCards.showCardValue(pos,k,0,CXModel.myHandCard[k]);
                                    }

                                    //  是特殊牌行要显示 特殊牌行
                                    if(player.cards_name == 0 || player.cards_name == 1)
                                    {
                                        CXCards.showCardType(pos,2,player.cards_name,true);
                                    }
                                }
                                
                            }
                            else
                            {
                                //扯牌完成 就必定有四张牌
                                if(player.cards_name == 0 || player.cards_name == 1)
                                {
                                    var leng = CXModel.myHandCard.length;
                                    for(var k =0;k<leng;k++)
                                    {
                                        CXCards.showCardValue(pos,k,0,CXModel.myHandCard[k]);
                                    }
                                    CXCards.showCardType(pos,2,player.cards_name,true);
                                   
                                }else{

                                    if(CXModel.myHandCard.length > 1)
                                    {
                                        CXCards.showCardValueAndPos(pos,0,0,CXModel.myHandCard[2]);
                                        CXCards.showCardValueAndPos(pos,1,0,CXModel.myHandCard[3]);
                                    }
                                    if(CXModel.myHandCard.length > 0)
                                    {
                                        CXCards.showCardValueAndPos(pos,2,0,CXModel.myHandCard[0]);
                                        CXCards.showCardValueAndPos(pos,3,0,CXModel.myHandCard[1]);
                                    }

                                    CXCards.showCardType(pos,0,player.wei_cards_name,false);
                                    CXCards.showCardType(pos,1,player.tou_cards_name,false);
                                }
                                
                                CXCards.addChePaiFinish(pos);
                            }
                        }
                    }
                    else
                    {
                        if(player.ready == 1)
                         {
                            if(player.cards_name != 0 && player.cards_name != 1)
                            {
                                if(player.betting == 1)
                                {
                                    CXCards.reverseAllCards(pos,0);
                                }
                                else
                                {
                                    for(var k =0;k<2;k++)
                                    {
                                        CXCards.showCardValue(pos,k,0,-1);
                                    }


                                    if(CXModel.otherPlayCards[pos].length > 0)
                                    {
                                        CXCards.showCardValue(pos,2,0,CXModel.otherPlayCards[pos][0]);
                                    }
                                    if(CXModel.otherPlayCards[pos].length > 1)
                                    {
                                        CXCards.showCardValue(pos,3,0,CXModel.otherPlayCards[pos][1]);
                                    }

                                }
                                
                            }
                            else
                            {
                                for(var k =0;k<2;k++)
                                {
                                    CXCards.showCardValue(pos,k,0,-1);
                                }

                                if(CXModel.otherPlayCards[pos].length > 0)
                                    {
                                        CXCards.showCardValue(pos,2,0,CXModel.otherPlayCards[pos][0]);
                                    }
                                    if(CXModel.otherPlayCards[pos].length > 1)
                                    {
                                        CXCards.showCardValue(pos,3,0,CXModel.otherPlayCards[pos][1]);
                                    }
                            }
                            
                            if(player.is_chepai_finish)
                            {
                                CXCards.addChePaiFinish(pos);
                            }
                         }
                    }

                    var num = Math.ceil(player.total_bet/CXModel.stand_bobo);  
                    CXChips.sendBetMoney(pos,CXModel.stand_bobo,num,player.total_bet);


                    if(player.ready == 1 && player.betting != 1)
                    {
                        nPlayers.setTOpacity(100);
                        CXCards.SetOpacityCardForPos(pos);
                    }
                    
                }


                // 拼牌阶段
                if(CXModel.state == CXState.SETTLEMENT)
                {
                    if(player.cards_name == 0 || player.cards_name == 1)
                    {
                        if(player.ready == 1)
                        {
                            if(CXModel.isMySeatID(player.seatid))
                            {
                                var leng = CXModel.myHandCard.length;
                                for(var k =0;k<leng;k++)
                                {
                                    CXCards.showCardValue(pos,k,0,CXModel.myHandCard[k]);
                                }
                                CXCards.showCardType(pos,2,player.cards_name,true);
                            }
                            else
                            {
                                var leng = CXModel.otherPlayCards[pos].length;
                                for(var k =0;k<leng;k++)
                                {
                                    CXCards.showCardValue(pos,k,0,CXModel.otherPlayCards[pos][k]);
                                }
                                CXCards.showCardType(pos,2,player.cards_name,true);
                            }
                        }  
                    }else{

                        if(player.ready == 1)
                        {
                            if(CXModel.isMySeatID(player.seatid))
                            {

                                if(player.is_chepai_finish)
                                {
                                    //扯牌完成
                                    if(CXModel.myHandCard.length > 1)
                                    {
                                        CXCards.showCardValueAndPos(pos,0,0,CXModel.myHandCard[2]);
                                        CXCards.showCardValueAndPos(pos,1,0,CXModel.myHandCard[3]);
                                    }
                                    if(CXModel.myHandCard.length > 0)
                                    {
                                        CXCards.showCardValueAndPos(pos,2,0,CXModel.myHandCard[0]);
                                        CXCards.showCardValueAndPos(pos,3,0,CXModel.myHandCard[1]);
                                    }
                                }
                                else
                                {

                                    var leng = CXModel.myHandCard.length;
                                    for(var k =0;k<leng;k++)
                                    {
                                        CXCards.showCardValue(pos,k,0,CXModel.myHandCard[k]);
                                    }
                                    
                                }

                                
                            }
                            else
                            {

                                if(player.is_chepai_finish)
                                {
                                    //扯牌完成
                                    if(CXModel.otherPlayCards[pos].length > 1)
                                    {
                                        CXCards.showCardValueAndPos(pos,0,0,CXModel.otherPlayCards[pos][2]);
                                        CXCards.showCardValueAndPos(pos,1,0,CXModel.otherPlayCards[pos][3]);
                                    }
                                    if(CXModel.otherPlayCards[pos].length > 0)
                                    {
                                        CXCards.showCardValueAndPos(pos,2,0,CXModel.otherPlayCards[pos][0]);
                                        CXCards.showCardValueAndPos(pos,3,0,CXModel.otherPlayCards[pos][1]);
                                    }
                                }
                                else
                                {
                                    var leng = CXModel.otherPlayCards[pos].length;
                                    for(var k =0;k<leng;k++)
                                    {
                                        CXCards.showCardValue(pos,k,0,CXModel.otherPlayCards[pos][k]);
                                    }
                                }
                            }

                            if(player.is_chepai_finish)
                            {
                                if(player.seatid == CXModel.max_tou_pai_seatid)
                                {
                                    CXCards.showCardType(pos,1,player.tou_cards_name,true);

                                }else{
                                    CXCards.showCardType(pos,1,player.tou_cards_name,false);
                                }

                                if(player.seatid == CXModel.max_tou_pai_seatid)
                                {
                                    CXCards.showCardType(pos,0,player.wei_cards_name,true);

                                }else{
                                    CXCards.showCardType(pos,0,player.wei_cards_name,false);
                                }
                            }
                        }
                    }
                }

                if(CXModel.state == CXState.SETTLEMENT )
                {
                    var num = Math.ceil(player.total_bet/CXModel.stand_bobo);  
                    CXChips.sendBetMoney(pos,CXModel.stand_bobo,num,player.total_bet);
                }

                if(player.ready == 1 && player.betting != 1)
                {
                    nPlayers.setTOpacity(100);
                    CXCards.SetOpacityCardForPos(pos);
                }

            }
        }

        // if(CXModel.state != CXState.READY)
        // {
            // JsUtils.postNotifi("notify_game_check_end");
        // }
        if(CXModel.state != CXState.READY && CXModel.state != CXState.GAME_STATE)   //设置庄家
        {
            if(CXModel.dealer != -1)
            {
                var pos = CXModel.getPosBySeatID(CXModel.dealer);
                var player = CXPlayers.players[pos]; 
                player.showzang(pos);    
             }
        }
        if(CXModel.state == CXState.GAME_STATE)
        {
            CXTips.showWaitTip("等待玩家设置颠簸");
        }

        if(CXModel.state == CXState.CHEPAI)
        {
            CXTips.showWaitTip("等待玩家扯牌");
        }

        if(CXPlayers.publicBox != null)
        {
            if(ZJHModel.getInstance().mySeatid == -1)
            {
                CXPlayers.publicBox.setGamePlayStatu(false);
            }else{
                CXPlayers.publicBox.setGamePlayStatu(true);
            }
            
        }

        if(CXModel.state == CXState.SETTLEMENT  || CXModel.state == CXState.GAME_END)
        {
            this.isResetGame = false;
        }

        if(len<6)
        {
            if(!CXModel.isMyInTable())
            {
                CXModel.sendUptable();
            }
        }

        if(CXModel.state != CXState.READY)
        {
            JsUtils.postNotifi("notify_game_check_end");
        }
        
//---------------------------------------------------------------------------------------------------------
        //轮到己方操作
        if(CXModel.state != CXState.READY && CXModel.state != CXState.GAME_STATE && CXModel.state != CXState.CHEPAI && CXModel.state != CXState.SETTLEMENT && CXModel.state != CXState.GAME_END)
        {
            if (CXModel.isMySeatID(CXModel.curSeatID)) {

                var pos = CXModel.getPosBySeatID(CXModel.curSeatID);
                var nplayer = CXPlayers.players[pos];
                if(!CXModel.is_option_timeout)
                {
                    nplayer.startTimer(CXModel.bet_timer_stamp,CXModel.left_bet_timer);
                }
                
            //出牌按钮的恢复

                CXModel.myoptionfinish = false;

                CXAction.showBtnEnable("btn_xiu",true);
                CXAction.showBtnEnable("btn_gen",true);
                CXAction.showBtnEnable("btn_qiao",true);
                CXAction.showBtnEnable("btn_da",true);

                var player = CXModel.players[CXModel.curSeatID];

                if(CXModel.isxiu)
                {
                    CXAction.showBtnEnable("btn_xiu",true);
                }else{
                    CXAction.showBtnEnable("btn_xiu",false);
                }

                // var NoutBet = CXModel.cur_bet + CXModel.base_money;   //最小的加注额
                // if(NoutBet > CXModel.max_raise_bet)
                // {
                //     CXAction.showBtnEnable("btn_da",false);
                // }

                var am = parseInt(player.bet) + parseInt(player.bobo_money);
                if(CXModel.cur_bet >= am)
                {
                    CXAction.showBtnEnable("btn_da",false);
                }

                if(CXModel.cur_bet == 0)
                {
                    CXAction.showBtnEnable("btn_gen",false);
                    CXAction.setGenValue(0);
                }
                else
                {
                    var canMon = parseInt(player.bobo_money) + parseInt(player.bet);
                    if(canMon > CXModel.cur_bet)
                    {

                        CXAction.showBtnEnable("btn_gen",true);
                        CXModel.cur_gen_bet = parseInt(CXModel.cur_bet) - parseInt(player.bet);
                        CXAction.setGenValue(CXModel.cur_gen_bet);

                    }else{
                        CXAction.showBtnEnable("btn_gen",false);
                        CXAction.setGenValue(0);
                    }
                }

                // var allmon = parseInt(player.bobo_money) + parseInt(player.bet);
                // if(allmon > CXModel.max_raise_bet)
                // {
                //     CXAction.showBtnEnable("btn_qiao",false);
                // }

                if(parseInt(player.bobo_money) <= 0)
                {
                    CXAction.showBtnEnable("btn_da",false);
                    CXAction.showBtnEnable("btn_gen",false);
                    CXAction.setGenValue(0);
                    CXAction.showBtnEnable("btn_qiao",false);
                }

                CXAction.showOutCardBtns();
            }
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

        // CXModel.updatePlayer(val);
        //更新掉线提示
        CXTips.showAllTip("");
        var seatID =utils.getValInt(val,"seatid");
        var player = CXModel.getPosBySeatID(seatID);
        player.isOffline = 0;
        if(seatID >=0 && seatID<=5)
        {
            var pos =CXModel.getPosBySeatID(seatID);
            CXPlayers.setOffline(pos,false);
        }
    },
    //广播：上桌成功
    handlerUptableSuccBC:function(val)
    {
        cc.log("handlerUptableSuccBC");
        var player = CXModel.updatePlayer(val);
        var Nplayer = CXPlayers.players[CXModel.getPosBySeatID(player.seatid)];

        Sound.getInstance().playEffect(PackName+"/res/sound/com/getin.mp3"); 
            
        if(CXModel.isMyUID(player.uid))
        {
            CXModel.waitUpdateInfo = true;
            CXModel.sendRoomInfo();
            CXTips.showMyTip("您已上桌");
        }
        else
        {
            CXPlayers.uptable(player);
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
            CXTips.showMyTip("您已在桌上");
        }else if(code == 104)
        {
            CXTips.showMyTip("金币不够，无法上桌");
        }else if(code == 105)
        {
            CXTips.showMyTip("桌上人已满");
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
            var pos = CXModel.getPosBySeatID(seatID);
            CXPlayers.downtable(pos);
            CXChips.resetPlayerchip(pos);
            CXCards.removeFinishCPForPos(pos);
            CXCards.HideCardsAndRemoveTip(pos);
        }

        if(CXModel.isMySeatID(seatID))
        {
            CXTips.showMyAssets(false);
            CXModel.unDownSeatID =CXModel.seatID;
            CXModel.seatID = -1;
            ZJHModel.getInstance().mySeatid = CXModel.seatID;
            CXAction.showReadyBtn(false);
            CXTips.showMyTip("您已下桌");

            if(ZJHModel.getInstance().mySeatid == -1)
            {
                CXPlayers.publicBox.setGamePlayStatu(false);
            }else{
                CXPlayers.publicBox.setGamePlayStatu(true);
            }
        }

        CXModel.resetPlayer(seatID);
    },

    //回调：准备成功
    handlerReadySuccBC:function(val)
    {
        cc.log("handlerReadySuccBC");


        var uid = utils.getValInt(val,"uid");
        var seatid = utils.getValInt(val,"seatid");
        var pos = CXModel.getPosBySeatID(seatid);

        if(CXModel.isMyUID(uid))
        {
            this.resetGame();
        }

        CXPlayers.setgray(pos,false);
        CXPlayers.setReady(pos,true);

        var player = CXModel.getPlayerBySeatID(seatid);
        if(player)
        {
            player.ready = 1;
            player.betting = 1;
        }

        if(CXModel.isMyUID(uid))
        {
            CXAction.showReadyBtn(false);

            for(var k = 0; k < 6;k++)
            {
                var player = CXModel.getPlayerByPos(k);
                if(player != null && player.seatid != -1 && player.ready == 1)
                {
                    CXPlayers.setgray(k,false);
                    CXPlayers.setReady(k,true);
                }
            }
            
        }

    },
    //回调：准备失败
    handlerReadyErroUC:function(val)
    {
        cc.log("handlerReadyErroUC");
        if(CXModel.isMyInTable())
        {
            CXTips.showMyTip("准备失败");
        }
        else
        {
            CXAction.showReadyBtn(false);
        }
    },

    //广播：准备开始新游戏
    handlerGamePrereadyBC:function(val)
    {
        cc.log("handlerGamePrereadyBC");

        if(CXModel.isMyInTable())
        {
            CXAction.showReadyBtn(true);
        }

        for(var i = 0;i<6;i++)
        {
            var player = CXModel.getPlayerByPos(i);
            if(player != null && player.seatid != -1)
            {
                player.ready = -1;
                player.betting = 0;
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
        if(face != -1 && target!=-1 && base != target && !CXModel.isMySeatID(base) )
        {
            var b = CXPlayers.getPosition(CXModel.getPosBySeatID(base));
            var t  =CXPlayers.getPosition(CXModel.getPosBySeatID(target));

            var tarpos = CXModel.getPosBySeatID(target);

            log("============kkkk============="+tarpos);

            CXTips.showBigface(b,t,face,CXPlayers.getSex(CXModel.getPosBySeatID(base)),tarpos);
        }
    },
    //广播：开始牌局，发牌
    handlerGameStartBC:function(val)
    {
        cc.log("handlerGameStartBC");

        CXTips.hideTime();

        for (var i = 0; i < 6; i++)
        {
            CXPlayers.setReady(i,false);
        }

        CXModel.state = CXState.GAME_STATE;

        if(val.hasOwnProperty("seats"))
        {

            CXTips.showWaitTip("等待玩家设置颠簸");

            var leng = val["seats"].length;
            for(var i = 0;i<leng;i++)
            {
                var seatid = val["seats"][i];
                var pos = CXModel.getPosBySeatID(seatid);
                var nPlayer = CXPlayers.players[pos];

                nPlayer.startTimer(CXModel.set_bobo_timer_stamp,CXModel.set_bobo_timer_stamp);

                if(CXModel.isMySeatID(seatid))
                {
                    nPlayer.setPlayerMoney(0);   //设置簸簸之前先显示

                    CXModel.boboView.refreshView();
                    CXModel.boboView.setVisible(true);

                }

            } 
        }

        this.stopGameOver();
        JsUtils.postNotifi("notify_game_check_end");
        CXModel.sendGetPlayerList();
    },


    //回调：看牌
    handlerGameStartUC:function(val)
    {
        cc.log("handlerGameStartUC");

        CXModel.state = CXState.QIANGGUAN_STATE; //翻牌之后就是抢关

        if(val.hasOwnProperty("cards"))
        {
            CXCards.handCardValues =[];
            CXCards.pushHandCard(val["cards"]);
        }

        if(CXModel.isFaPai)
        {
            var self = this;
            var ac = cc.repeatForever(cc.sequence(cc.delayTime(0.1),cc.callFunc(function(){

                if(!CXModel.isFaPai)
                {
                    self.runAction(cc.sequence(cc.delayTime(1),
                        cc.callFunc(function(){
                            if(CXModel.gamePlayType != 1)
                            {
                                var pos = CXModel.getPosBySeatID(CXModel.firstPlayerSeait);
                                CXTips.foPlayerOutCard(CXModel.table_type,pos);
                            }
                            
                        }),
                        cc.delayTime(0.8),
                        cc.callFunc(function(){
                        CXCards.showHandCards(0,CXModel.table_type);
                        CXCards.showHandCards(1,CXModel.table_type);
                        CXCards.showHandCards(2,CXModel.table_type);
                    }),cc.callFunc(function(){
                            CXTips.showMPAnimation(0,true);
                            CXTips.showMPAnimation(1,true);
                            CXTips.showMPAnimation(2,true);
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
                    if(CXModel.gamePlayType != 1)
                    {
                         var pos = CXModel.getPosBySeatID(CXModel.firstPlayerSeait);
                        CXTips.foPlayerOutCard(CXModel.table_type,pos);
                    }
                }),
                cc.delayTime(0.8),
                cc.callFunc(function(){
                CXCards.showHandCards(0,CXModel.table_type);
                CXCards.showHandCards(1,CXModel.table_type);
                CXCards.showHandCards(2,CXModel.table_type);
            }),cc.callFunc(function(){
                            CXTips.showMPAnimation(0,true);
                            CXTips.showMPAnimation(1,true);
                            CXTips.showMPAnimation(2,true);
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
        var pos = CXModel.getPosBySeatID(seatID);
        if(CXModel.isMySeatID(seatID))
        {
            CXAction.showMingPaiBtns(true);
        }
        CXTips.hideJiaoQiangTip();
        CXTips.hideThinkTips();

        CXTips.showTime(pos,CXModel.mingpaiTime);
        CXTips.startWaitTime(pos,CXModel.mingpaiTime);


        if(CXTips.needBuLa())
        {
            CXTips.showTiLa(CXModel.getPosBySeatID(CXModel.dzSeatID),2,1);
        }

    },


    //广播：进入出牌
    handlerStartOutCardBC:function(val)
    {
        cc.log("handlerStartOutCardBC");
        CXModel.noOutCardSeait = -1;  //重置不可以出牌的位置


        var seatID = utils.getValInt(val,"seatid");
        var pos = CXModel.getPosBySeatID(seatID);
        CXModel.curSeatID = seatID;
        CXModel.outCardMode =utils.getValInt(val,"action");

        CXModel.state = CXState.PLAYING;

        CXAction.showMingPaiBtns(false);
        CXTips.hideJiaoQiangTip();
        CXTips.hideThinkTips();


        CXTips.stopWaitTime(0);
        CXTips.stopWaitTime(1);
        CXTips.stopWaitTime(2);


        //重新初始出牌数组
        CXCards.showOutCards(pos,[]);
        CXTips.showOutCard(pos,0);


        CXTips.showTime(pos,CXModel.playTime);
        CXTips.startWaitTime(pos,CXModel.playTime);
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
        CXModel.state = CXState.GAME_END;

        this.isResetGame = false;

        CXTips.showAllTip("");
        CXTips.showMyTip("");
        CXTips.hideTouAndWei();

        CXChips.v_recordChipNum();

        var estate = utils.getValInt(val,"end_state");

        if(estate == 0)  //－－－－－－－－－－－－－－－－－－休－－－－－－－－－－－－－－
        {
            CXTips.showLJXiu(true);

            if(val.hasOwnProperty("players"))
            {
                var leng = val["players"].length;
                for(var i = 0; i< leng; i++)
                {
                    var seatid = val["players"][i]["seatid"];
                    var player = CXModel.players[seatid];
                    var last_bobo_money = val["players"][i]["bobo_money"];
                    player.bobo_money = last_bobo_money;
                }
            }
            for(var k = 0;k<6;k++)
            {
                var num = CXChips.playChip[k].length;
                if(num > 0)
                {
                    CXChips.chipBackToMySelf(k);
                }
            }
        }
        else if(estate == 1)   //－－－－－－－－－－－－－－－－－－弃－－－－－－－－－－－－－－
        {
            var winseatid = [];
            var lostseatid = [];
            var fraction = {};

            for(var k = 0;k < 6;k++)
            {
                fraction[k] = -1;
            }

            if(val.hasOwnProperty("players"))
            {
                var dealytime = 0.15;
                var leng = val["players"].length;
                for(var i = 0; i< leng; i++)
                {
                    var d = val["players"][i];
                    dealytime+=0.05;
                    var seatid = d["seatid"];
                    var pos =  CXModel.getPosBySeatID(seatid);
                    var player = CXModel.players[seatid];
                    var last_bobo_money = d["bobo_money"];
                    player.bobo_money = last_bobo_money;

                    if(d.hasOwnProperty("pi_money"))
                    {
                        var sendpi = d["pi_money"];
                        CXModel.cur_pi = CXModel.cur_pi + sendpi;
                        CXChips.sendPichipTotable(pos,sendpi,CXModel.cur_pi,dealytime);
                        fraction[pos] = -sendpi;
                    }
                }

                for(var i = 0; i< leng; i++)
                {
                    dealytime+=0.05;
                    var d = val["players"][i];
                    var seatid = d["seatid"];
                    var pos =  CXModel.getPosBySeatID(seatid);
                    var player = CXModel.players[seatid];
                    var last_bobo_money = d["bobo_money"];
                    player.bobo_money = last_bobo_money;

                    if(d.hasOwnProperty("lose_money"))
                    {
                        var lostmoney = d["lose_money"];
                        if(lostmoney == 0)
                        {
                            winseatid.push(seatid);
                            fraction[pos] = lostmoney;

                        }else{

                            CXModel.cur_pi = CXModel.cur_pi + lostmoney;
                            CXChips.sendMyBetToTable(pos,CXModel.cur_pi,dealytime);
                            lostseatid.push(seatid);
                            fraction[pos] = -lostmoney;
                        }
                        
                    }
                }

                for(var i = 0; i< leng; i++)
                {
                    dealytime+=0.05;
                    var d = val["players"][i];
                    var seatid = d["seatid"];
                    var pos =  CXModel.getPosBySeatID(seatid);
                    var player = CXModel.players[seatid];
                    var last_bobo_money = d["bobo_money"];
                    player.bobo_money = last_bobo_money;

                    if(d.hasOwnProperty("win_money"))
                    {
                        var winmoney = d["win_money"];
                        winseatid.push(seatid);
                        fraction[pos] = winmoney;
                    }
                }

                this.runAction(cc.sequence(cc.delayTime(2),cc.callFunc(function(){
                    for(var k =  0;k < winseatid.length;k++)
                    {
                         CXChips.winAllmoney(winseatid[k]);
                    }
                  
                }),cc.delayTime(1),cc.callFunc(function(){
                    for(var j = 0;j<winseatid.length;j++)
                    {
                        var vpos =  CXModel.getPosBySeatID(winseatid[j]);
                        CXChips.chipBackToMySelf(vpos);
                    }
                  
                }), cc.delayTime(2), cc.callFunc(function(){
                   for(var i = 0;i < 6;i++)
                   {
                        if(fraction[i] != -1)
                        {
                            CXTips.showEnunciationCoin(i,fraction[i]);
                        }
                   }
                }) ));
            }
        }
        else if(estate == 2)  //－－－－－－－－－－－－－－－－－－正常结算－－－－－－－－－－－－－－－－－－
        {
            var winplayer = [];
            var fraction = {};

            for(var k = 0;k < 6;k++)
            {
                fraction[k] = -1;
            }

            if(val.hasOwnProperty("players"))
            {
                var dealytime = 0.15;
                var leng = val["players"].length;
                for(var i = 0; i< leng; i++)
                {
                    var d = val["players"][i];
                    dealytime+=0.05;
                    var seatid = d["seatid"];
                    var pos =  CXModel.getPosBySeatID(seatid);
                    var player = CXModel.players[seatid];
                    var last_bobo_money = d["bobo_money"];
                    player.bobo_money = last_bobo_money;

                    var winlose = d["win_lose"];
                    var dp = utils.getValBool(d,"is_diupi");
                    if(dp)
                    {
                        CXModel.cur_pi = CXModel.cur_pi + Math.abs(winlose);
                        CXChips.sendPichipTotable(pos,Math.abs(winlose),CXModel.cur_pi,dealytime); 
                    }
                    else
                    {
                        if(winlose >= 0)
                        {
                            winplayer.push(pos);
                        }
                        else
                        {
                            if(player.total_bet > Math.abs(winlose))
                            {
                                CXModel.cur_pi = CXModel.cur_pi + Math.abs(winlose);
                                CXChips.chipBackForNum(pos,Math.abs(winlose),player.total_bet,CXModel.cur_pi);
                            }else{
                                CXModel.cur_pi = CXModel.cur_pi + Math.abs(winlose);
                                CXChips.sendMyBetToTable(pos,CXModel.cur_pi,dealytime);
                            }
                        }
                    }
                    fraction[pos] = winlose;
                }

                this.runAction(cc.sequence(cc.delayTime(2),cc.callFunc(function(){
                  for(var k = 0;k<6;k++)
                  {
                     CXChips.chipBackToMySelf(k);
                  }
                }),cc.delayTime(1),cc.callFunc(function(){
                    var allnum = CXChips.pichip.length;
                    var start = 0;
                    log("=========allnum=================="+allnum);
                    for(var k = 0;k<winplayer.length;k++)
                    {
                     var pos = winplayer[k];
                     var nmonwy = fraction[pos];
                     var nums = Math.ceil(nmonwy/CXModel.stand_bobo);

                     if(nums > allnum)
                     {
                        nums = allnum;
                        allnum = allnum - nums;
                     }else{
                        if(k == winplayer.length - 1)
                        {
                            nums = allnum;
                            allnum = allnum - nums;
                        }else{
                            allnum = allnum - nums;
                        }
                     }
                     log("=====pos===="+pos+"===start==="+start+"===nums==="+nums+"===allnum==="+allnum);
                     CXChips.winPiMoneyToPos(pos,start,nums);
                     start = nums;
                    }
                }), cc.delayTime(2), cc.callFunc(function(){
                   for(var i = 0;i < 6 ;i++)
                   {
                        if(fraction[i] != -1)
                        {
                            CXTips.showEnunciationCoin(i,fraction[i]);
                        }
                   }
                })));
               
            }
        }


        // this.runAction(cc.sequence(cc.delayTime(5),cc.callFunc(function(){
        //     if(CXModel.isMyInTable())
        //     {
        //         CXAction.showReadyBtn(true);
        //     }
        //     CXTips.showWaitTip("等待游戏开始");
        // })));

        if(CXModel.isGameOver)
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
        CXModel.isGameOver = true;
        ZJHModel.getInstance().roomToShareView(CXModel.curRoomID);
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
        var player = CXModel.getPlayerByUID(val["uid"]);
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
            PlatformHelper.showToast("房间解散成功，3秒后房间自动结束");
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
        var player = CXModel.getPlayerBySeatID(seatID);
        if(player)
        {
            player.isOffline = 1;
            if(seatID >-1)
            {
                var  pos = CXModel.getPosBySeatID(seatID);
                CXPlayers.setOffline(pos,true);
                CXTips.checkTimeOut(pos);
            }
        }
    }
});
