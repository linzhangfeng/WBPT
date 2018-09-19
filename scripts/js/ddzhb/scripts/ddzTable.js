

var DDZGameState ={
    E_SHANG_ZHUO :0,
    E_ZHUN_BEI_START:1,
    E_ZHUN_BEI_WAIT:2,
    E_FA_PAI:3,
    E_GAIQIANG_START:4,
    E_GAIQIANG_BC:5,
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

var DDZTable = cc.Layer.extend({
    uiNode:null,
    uiAction:null,
    retNetNum:0,
getUI:function(key)
    {
        return this.uiNode.getChildByName(key);
    },

    ctor:function () {

        this._super();

        var self = this;
        cc.spriteFrameCache.addSpriteFrames("public/res/img/bigface.plist");
        this.uiJson = ccs.load(PackName+"/res/table.json");
        this.uiNode =this.uiJson.node;
        this.addChild(this.uiNode);

        //初始化数据模型
        DDZModel.reset();
        DDZModel.roomData = JSON.parse( ZJHModel.getInstance().getRoomData());
        DDZModel.curRoomID = ZJHModel.getInstance().curRoomID;

        this.initView();

        this._listenerExit = cc.EventListener.create({
            event:cc.EventListener.CUSTOM,
            eventName:"will_exit_room",
            callback:function(evt){
                cc.log("will_exit_room");
                DDZModel.release();
                ccs.ArmatureDataManager.destroyInstance();
                cc.spriteFrameCache.removeSpriteFrames();
                cc.textureCache.removeAllTextures();
                jsb.fileUtils.purgeCachedEntries();

                self.unscheduleAllCallbacks();
                if(self.videoBox)
                {
                    self.videoBox.unscheduleAllCallbacks();
                }
                self.removeFromParent();
            }
        });
        cc.eventManager.addListener(this._listenerExit,this);
        //  Sound.getInstance().preLoadEffect("js/"+PackName+"/res/sound");
        Sound.getInstance().PlayBgSound(PackName+"/res/sound/game_bgm.mp3");

        DDZModel.isOnVideo = ZJHModel.getInstance().isOnVideo;
        if( DDZModel.isOnVideo) {

            this.videoBox = new DDZVideo();
            this.addChild(this.videoBox,111111);
            this.videoBox.runScript(this);
        }
        else {

            //监听联网状态
            this.schedule(this.updateGame, 0.0);
            this.schedule(this.hearbeat, 3.0);

            DDZPlayers.publicBox = RoomPublic.create();
            DDZPlayers.publicBox.setCallback(function (tag) {
                if (tag == "exit") {
                    DDZModel.sendDowntable();
                    DDZModel.sendLogout();
                }
                else if (tag == "end_room") {
                    DDZModel.sendReqRoomEnd();
                }
                else if (tag == "down_table") {
                    DDZModel.sendDowntable();
                }
            });

            this.addChild(DDZPlayers.publicBox, 10000);


            //部分事件监听
            var self = this;
            this._listenerGameOver = cc.EventListener.create({
                event: cc.EventListener.CUSTOM,
                eventName: "notify_against_room_end",
                callback: function (evt) {
                    self.onGameOver();
                    cc.log("notify_against_room_end");
                }
            });
            cc.eventManager.addListener(this._listenerGameOver, this);


            DDZModel.initNet(DDZModel.roomData["gameRoom"]["way"]["ip"], Number(DDZModel.roomData["gameRoom"]["way"]["port"]));
            DDZModel.connectNet();
        }

        return true;
    },
    //初始化界面
    initView:function()
    {
        utils.scaleAll(this.getUI("bg"));

        DDZAction.uiNode =  this.getUI("btns");
        DDZAction.clickCB = this.onBtnClickEvt;
        DDZAction.init();

        DDZCards.uiNode =  this.getUI("cards");
        DDZCards.init();

        DDZPlayers.uiNode =  this.getUI("players");
        DDZPlayers.init();

        DDZTips.uiNode =  this.getUI("tips");
        DDZTips.init();

    },
    //心跳处理
    hearbeat:function( dt) {
        DDZModel.sendHearbeat();
    },
    //联网监听
    updateGame:function( dt)
    {
        var curTime = (new Date).valueOf()/1000;
        //如果游戏退出到后台，返回前台时
        if(ZJHModel.getInstance().gamePuase)
        {
            ZJHModel.getInstance().gamePuase =false;
            DDZModel.lastHearbeatTime =0;
                DDZModel.waitUpdateInfo = true;
            DDZModel.clearCMD();
                DDZModel.connectNet();

            return;
        }
        //心跳包超时
        if(DDZModel.lastHearbeatTime != 0 && (curTime - DDZModel.lastHearbeatTime ) > 10)
        {
            DDZModel.clearCMD();
            DDZModel.connectNet();

            DDZModel.lastHearbeatTime = curTime;
            if( this.retNetNum >0)
            {
                DDZTips.showAllTip("网络连接异常，正在重试");
            }
            this.retNetNum ++;
            return;
        }
        if(DDZModel.isFaPai)
        {
            DDZModel.clearCMD();
            return;
        }
        var response = DDZModel.getNetData();
        if(response != null)
        {
            if(response.cmd ==TCPSendCMD.DISCONNECT_RES || response.cmd ==TCPSendCMD.CONNECT_ERROR_RES
                || response.cmd ==TCPSendCMD.SEND_DATA_ERROR_RES|| response.cmd ==TCPSendCMD.RECV_DATA_ERROR_RES)
            {
                    cc.log("tcp error:"+response.cmd);
                    if(!DDZModel.isLogout)
                    {
                        DDZModel.connectNet();
                    }
            }
            else if( response.cmd ==TCPSendCMD.CONNECT_OK_RES)
            {
                cc.log("tcp connect succ!/n cmd:"+response.cmd +" data:"+response.data );
                DDZModel.clearCMD();

                if (!DDZModel.isLogout) {
                    DDZModel.sendLogin();
                }
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
        DDZAction.reset();
        DDZCards.reset();
        DDZTips.reset();
        DDZPlayers.reset();
        DDZModel.resetPre();

        var player = DDZModel.getPlayerBySeatID(DDZModel.seatID);
       if(player)
       {
           ZJHModel.getInstance().rmb = player.rmb;
           ZJHModel.getInstance().money = player.money;
           ZJHModel.getInstance().coin = player.coin;
       }
    },

    onGameOver: function () {
        DDZModel.isGameOver = true;
      if(DDZModel.state == DDZState.READY||DDZModel.state == DDZState.END_GAME)
      {
          var self  =this;
          this.stopGameOver();
          var action = cc.sequence(cc.delayTime(5),cc.callFunc(function(){
              ZJHModel.getInstance().mySeatid = -1;
              DDZModel.isGameOver = true;
              ZJHModel.getInstance().roomToShareView(DDZModel.curRoomID);
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
        if(!DDZModel.isMyInTable())
        {
            return;
        }
        var player = DDZModel.getPlayerByUID(val["uid"]);
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
                var p = DDZModel.getPlayerByUID(val["voters_uid"][i]);
                if(p) {
                    d["uid"] = p.uid;
                    d["name"] = p.name;
                    d["avatar"] = p.avatar;
                    d["state"] = 0;

                    json["players"].push(d);
                    if (ZJHModel.getInstance().uid == p.uid) {
                        isCan = true;
                    }
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
        this.roomEndBox = RoomHEndTouPiao.create();
        this.roomEndBox.initWithJsonStr(JSON.stringify(json));
        this. addChild(this.roomEndBox,10000);

        this.roomEndBox.setCallback(function (str){
            if (str == "ok")
            {
                DDZModel.sendRoomEnd(1);
            }
            else if (str == "no")
            {
                DDZModel.sendRoomEnd(0);
            }
        });
    },
//*************************** 按钮点击发送服务器处理 ************************************//
    //点击事件处理
    onBtnClickEvt:function(key)
    {
        Sound.getInstance().playEffect(utils.format("%1/res/sound/btn_click.mp3",PackName));
        cc.log("onBtn:"+key);
        switch(key)
        {

            case "readyBtn":
                DDZModel.sendReady();
                break;
            case "tuoguanBtn":
                if(DDZModel.state != DDZState.PLAYING)
                {
                    if( DDZModel.state == DDZState.TI_LA)
                    {
                        DDZTips.showMyTip("踢拉选择阶段无法托管");
                    }

                   else if( DDZModel.state == DDZState.MING_PAI)
                    {
                        DDZTips.showMyTip("明牌选择阶段无法托管");
                    }
                    else
                    {
                        DDZTips.showMyTip("确定地主后才能使用托管");
                    }
                    return;
                }
                DDZModel.sendTuoguan();
                break;
            case "uptableBtn":
                DDZModel.sendUptable();
                break;
            case "bugaiqiangBtn":
                DDZModel.sendGaiQiang(1);
                break;
            case "gaiqiangBtn":
                DDZModel.sendGaiQiang(2);
                break;
            case "budangBtn":
            case "bujiaoBtn":
            case "buqiangBtn":
                DDZModel.sendJiaoDZ(5);
                break;
            case "dangBtn":
            case "jiaodzBtn":
                DDZModel.sendJiaoDZ(3);
                break;
            case "qiangdzBtn":
                DDZModel.sendJiaoDZ(4);
                break;
            case "tiBtn":
            case "laBtn":
                DDZModel.sendTiLa(2);
                break;
            case "butiBtn":
            case "bulaBtn":
                DDZModel.sendTiLa(1);
                break;
            case "mingpaiBtn":
                DDZModel.sendMingPai(2);
                break;
            case "bumingpaiBtn":
                DDZModel.sendMingPai(1);
                break;
            case "buchuBtn":
            case "passBtn":
                DDZModel.sendOutCards(false);
                break;
            case "chupaiBtn":
                if( DDZAction.getUI("chupaiBtn").getChildByName("enable").isVisible())
                {
                    if(DDZCards.upCardValues.length >0)
                    {
                        DDZTips.showMyTip("牌型不符");
                    }
                    else
                    {
                        DDZTips.showMyTip("请选择要出的牌");
                    }
                    break;
                }
                DDZModel.sendOutCards(true);
                break;
            case "tishiBtn":
                DDZCards.tipCard();
                if(DDZCards.tipCardValues.length == 0)
                {
                    DDZModel.sendOutCards(false);
                }
                else
                {
                    DDZCards.showUpCard(DDZCards.tipCardValues);
                }
                break;
            case "chongxuanBtn":
                DDZCards.initUpCard();
                DDZCards.initLaiZi();
                DDZAction.checkOutCard();
                break;
        }

    },

 //*************************** 服务器返回信息处理 ************************************//
    //处理联网回调
    handlerCMD:function(data,isDelay)
    {
        var val;// =   JSON.parse(data);//data.toJson();
        try{
            val = JSON.parse(data);
        }
        catch(e)
        {
            cc.log("_error__data:"+data);
            return;
        }
        DDZModel.fastShow = !isDelay;
        var cmd = val.cmd;
        cc.log("_r__cmd:"+cmd);
        cc.log("_r__data:"+data);

        DDZModel.lastHearbeatTime = (new Date).valueOf()/1000;
        DDZModel.checkCMD(cmd);
        //登录失败
        if(NetCMD.SERVER_DZ_LOGIN_ERR_RES == cmd)
        {
            ZJHModel.getInstance().mySeatid = -1;
            DDZModel.isGameOver = true;
            ZJHModel.getInstance().roomToShareView(DDZModel.curRoomID);
            return;
        }
        if(DDZModel.waitUpdateInfo && cmd !=NetCMD.SERVER_TABLE_INFO_UC)
        {
            return;
        }

        switch (cmd)
        {
            //心跳包
            case NetCMD.SERVER_HEART_BEAT_RESP:
                cc.log("SERVER_HEART_BEAT_RESP");
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
                this.handlerLoginSuccBC(val);
                break;
            //登录失败
            case NetCMD.SERVER_LOGIN_ERR_UC:
                cc.log("SERVER_LOGIN_ERR_UC");
                break;
            //登出成功
            case NetCMD.SERVER_LOGOUT_SUCC_BC:
                cc.log("SERVER_LOGOUT_SUCC_BC");
                DDZModel.isLogout = true;
                break;
            //
            case NetCMD.SERVER_UPTABLE_INFO_UC:
                cc.log("SERVER_UPTABLE_INFO_UC");
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
            //开始盖抢
            case NetCMD.SERVER_START_GAIQIANG_BC:
                this.handlerStartGaiQiangBC(val);
                break;
            //盖抢成功
            case NetCMD.SERVER_GAIQIANG_SUCC_UC:
                this.handlerGaiQiangSuccUC(val);
                break;
            //盖抢结果
            case NetCMD.SERVER_GAIQIANG_RESULT_BC:
                this.handlerGaiQiangResultBC(val);
                break;
            //看牌
            case NetCMD.SERVER_GAME_START_UC:
                this.handlerGameStartUC(val);
                break;
            //叫地主
            case NetCMD.SERVER_PREPLAY_ONE_BC:
                this.handlerStartJiaoDiZhuBC(val);
                break;
            //叫地主结果
            case NetCMD.SERVER_PREPLAY_ONE_SUCC_BC:
                this.handlerJiaoDiZhuSuccBC(val);
                break;
            //抢地主
            case NetCMD.SERVER_PREPLAY_TWO_BC:
                this.handlerStartQiangDiZhuBC(val);
                break;
            //抢地主结果
            case NetCMD.SERVER_PREPLAY_TWO_SUCC_BC:
                this.handlerQiangDiZhuSuccBC(val);
                break;
            //当地主广播
            case NetCMD.SERVER_LANDLORD_BC:
                this.handlerDiZhuBC(val);
                break;
            //流局
            case NetCMD.SERVER_NO_DIZHU_BC:
                this.handlerNoDiZhuBC(val);
                break;
            //必当地主
            case NetCMD.SERVER_MUST_BE_LANDLORD_BC:
                break;
            //开始踢拉
            case NetCMD.SERVER_START_TILA_BC:
                this.handlerStartTiLaBC(val);
                break;
            //踢拉结果
            case NetCMD.SERVER_TILA_SUCC_BC:
                this.handlerTiLaSuccBC(val);
                break;
            //开始明牌
            case NetCMD.SERVER_START_MINGPAI_UC:
                this.handlerStartMingPaiUC(val);
                break;
            //明牌结果
            case NetCMD.SERVER_MINGPAI_SUCC_BC:
                this.handlerMingPaiSuccBC(val);
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
            case NetCMD.SERVER_PLAY_CARD_ERR_UC:
                this.handlerOutCardErrorUC(val);
                break;
            //对局结束，进行结算
            case NetCMD.SERVER_GAME_END_BC:
                this.handlerGameEndBC(val);
                break;
            //托管结果
            case NetCMD.SERVER_ROBOT_BC:
                this.handlerRobotBC(val);
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
        }
    },
    //回调：房间信息
    handlerTableInfoUC:function(val) {
        cc.log("handlerTableInfoUC");

        DDZModel.waitUpdateInfo = false;
        DDZModel.reset();
        this.resetGame();
        DDZPlayers.downtable();
        if(this.roomEndBox)
        {
            this.roomEndBox.removeFromParent(true);
            this.roomEndBox =null;
        }


        DDZModel.roomGolden = utils.getValInt(val, "takein");
        DDZModel.state = utils.getValInt(val, "state");
        DDZModel.seatID = utils.getValInt(val, "seatid");
        DDZModel.curSeatID = utils.getValInt(val, "cur_seat");
        DDZModel.dzSeatID = utils.getValInt(val, "landlord");
        DDZModel.tableType = utils.getValInt(val, "table_type");
        DDZModel.lastSeatID = utils.getValInt(val, "last_seatid");
        DDZModel.outCardMode = utils.getValInt(val, "action");

        if(DDZModel.isOnVideo)
        {
            var len  = val["players"].length;
            for (var i = 0; i < len; ++i) {
                var d = val["players"][i];
                if(DDZModel.isMyUID(d["uid"]))
                {
                    DDZModel.seatID = d["seatid"];
                    break;
                }
            }
            DDZModel.pauseTime = 20;
            DDZModel.tilaTime = 20;
            DDZModel.mingpaiTime = 20;
            DDZModel.playTime = 20;
        }
        else{
            DDZModel.playTime = utils.getValInt(val, "play_timer_stamp");
            DDZModel.pauseTime = utils.getValInt(val, "game_pause_tstamp");
            DDZModel.tilaTime = utils.getValInt(val, "tila_timer_tstamp");
            DDZModel.mingpaiTime = utils.getValInt(val, "mingpai_timer_tstamp");
        }


        DDZModel.addRatios[DDZRatio.TI_LA] = utils.getValInt(val, "tila_ratio");
        DDZModel.addRatios[DDZRatio.MING_PAI] = utils.getValInt(val, "mingpai_ratio");
        DDZModel.addRatios[DDZRatio.BOMB] = utils.getValInt(val, "bomb_ratio");
        DDZModel.addRatios[DDZRatio.ROCKET] = utils.getValInt(val, "rocket_ratio");
        DDZModel.addRatios[DDZRatio.SPRING] = utils.getValInt(val, "spring_ratio");

        if(val.hasOwnProperty( "player_option"))
        {
            for(var i in val[ "player_option"])
            {
                if(val[ "player_option"][i] ==1)
                {
                    libddz.cardsFind.splitRocket =false;
                }
                else if(val[ "player_option"][i] ==2)
                {
                    DDZModel.zh = true;
                }

            }
        }
        DDZTips.showPlayType();
        if (val.hasOwnProperty("max_ratio")) {
            DDZModel.maxRatios = utils.getValInt(val, "max_ratio");
            DDZTips.setZha(DDZModel.maxRatios);
        }


        if (val.hasOwnProperty("holes")) {
            DDZCards.pushHandCard(val["holes"]);
        }
        DDZModel.updateRatio(val);
        DDZModel.updateCoin(val);

        ZJHModel.getInstance().mySeatid = DDZModel.seatID;

        //提示恢复
        DDZTips.setCoin(DDZModel.coin);
        DDZTips.setRatio(DDZModel.getRatio());


        //明牌
        if(val.hasOwnProperty("mingpai_player"))
        {
            var sID =utils.getValInt(val["mingpai_player"][0],"seatid");
            var mpPos=DDZModel.getPosBySeatID(sID);
            DDZCards.setMingPaiPos(mpPos);
            DDZCards.pushShowCard(mpPos,val["mingpai_player"][0]["holes"]);
        }
        //恢复公共牌
        if (DDZModel.state == DDZState.PREPLAY_ONE || DDZModel.state == DDZState.PREPLAY_TWO || DDZModel.state == DDZState.GAIQIANG) {
            for (var i = 0; i < 4; ++i) {
                DDZCards.showPublicCard(i, 0);
            }
        }
        else if (DDZModel.state == DDZState.PLAYING || DDZModel.state == DDZState.MING_PAI|| DDZModel.state == DDZState.TI_LA) {
            if (val.hasOwnProperty("comms")) {
                for (var i = 0; i < val["comms"].length; ++i) {
                    DDZCards.showPublicCard(i, val["comms"][i]);
                }
            }
            var p = DDZModel.getPosBySeatID(DDZModel.dzSeatID);
            DDZTips.showDiZhu(p);
            DDZCards.setDiZhuPos(p);
        }

        //恢复出牌
        if (val.hasOwnProperty("last_cards")) {

            DDZCards.pushOutCard(val["last_cards"]);
            if (DDZCards.outCardValues.length > 0)
            {
                var pos = DDZModel.getPosBySeatID(DDZModel.lastSeatID);
                DDZCards.showOutCards(pos, DDZCards.outCardValues);

            }
        }

        //上桌玩家信息
        var len = 0;
        var tlType = utils.getValInt(val, "tila_type");
        if (val.hasOwnProperty("players")) {
            len = val["players"].length;
            for (var i = 0; i < len; ++i) {
                var d = val["players"][i];
                var player = DDZModel.updatePlayer(d);
                DDZPlayers.uptable(player);

                var pos = DDZModel.getPosBySeatID(player.seatid);
                //恢复托管
                player.isTuoGuan = utils.getValInt(d, "robot");
                player.isOffline = utils.getValInt(d, "offline");
                DDZPlayers.setTuoguan(pos, player.isTuoGuan == 1);
                DDZPlayers.setOffline(pos, player.isOffline == 1);
                if (DDZModel.isMySeatID(player.seatid)) {
                    DDZModel.isTuoguan = player.isTuoGuan;
                    if (DDZModel.state == DDZState.GAIQIANG) {
                        var type = utils.getValInt(d, "gaiqiang");
                        DDZAction.showJiaoDZBtns(type==0);
                        if(type==2)
                        {
                            DDZTips.showJiaoQiang(pos,3);
                        }
                        else  if(type==1)
                        {
                            DDZTips.showJiaoQiang(pos,5);
                        }
                    }
                }
                var gqtype = utils.getValInt(d, "gaiqiang");
                DDZTips.showGaiQiang(pos,gqtype);
                //恢复准备状态
                if (DDZModel.state == DDZState.READY) {
                    DDZPlayers.setReady(pos, player.ready == 1);
                    if (player.ready != 1 && DDZModel.isMyUID(player.uid)) {
                        DDZAction.showReadyBtn(true);
                    }
                }

                if(DDZModel.isOnVideo)
                {
                    if (d.hasOwnProperty("holes")) {
                        if (DDZModel.isMySeatID(player.seatid)) {
                            DDZCards.pushHandCard(d["holes"]);
                        }
                        else
                        {
                            DDZCards.showCardValues[pos] = [];
                            DDZCards.pushShowCard(pos, d["holes"]);
                        }
                        DDZCards.showHandCards(pos, 0);
                    }
                }
                else  if (DDZModel.state != DDZState.END_GAME && DDZModel.state != DDZState.INIT
                    && DDZModel.state != DDZState.READY && DDZModel.state != DDZState.GAIQIANG) {
                    var num = utils.getValInt(d, "card_nums");
                    DDZCards.showHandCards(pos, num);
                    DDZTips.showHandCardJB(pos,num>=1&&num<=2);
                }
                else {
                    DDZCards.showHandCards(pos, 0);
                }
                var tlAction = utils.getValInt(d, "tila");
                //ti
                if (!DDZModel.isDZSeatID(player.seatid)) {
                    DDZTips.showTiLa(pos,1,tlAction);
                }
                //la
                else
                {
                    DDZTips.showTiLa(pos,2,tlAction);
                }
//踢拉恢复
                 if (DDZModel.state == DDZState.TI_LA) {

                    if (DDZModel.isMySeatID(player.seatid)) {

                        if (DDZModel.isDZSeatID(player.seatid)) {
                            if (tlType == 2) {
                                DDZAction.showTiLaBtns(tlType, tlAction == 0);
                            }
                        }
                        else {
                            if (tlType == 1) {
                                DDZAction.showTiLaBtns(tlType, tlAction == 0);
                            }
                        }
                    }
                    //ti
                    if (tlType == 1) {
                        if (!DDZModel.isDZSeatID(player.seatid)) {
                            DDZTips.startWaitTime(pos,DDZModel.tilaTime);
                        }
                    }
                    //la
                    else (tlType == 2)
                    {
                        if (DDZModel.isDZSeatID(player.seatid)) {
                            DDZTips.startWaitTime(pos,DDZModel.tilaTime);
                        }
                    }
                }//明牌恢复
                else if (DDZModel.state == DDZState.MING_PAI) {

                    var type = utils.getValInt(d, "mingpai");
                    if (DDZModel.isMySeatID(player.seatid)&&DDZModel.isDZSeatID(player.seatid)) {

                        DDZAction.showMingPaiBtns(type == 0);
                    }
                }
            }
        }

        if(len<3)
        {
            if(!DDZModel.isMyInTable())
            {
                DDZModel.sendUptable();
            }
            //DDZTips.showWaitTip("等待玩家上桌");
        }
        else if(!DDZModel.isOnVideo)
        {
            JsUtils.postNotifi("notify_game_check_end");
        }
        var  pos = DDZModel.getPosBySeatID(DDZModel.curSeatID);
        var dzPos=DDZModel.getPosBySeatID(DDZModel.dzSeatID);
        //盖抢恢复
        if (DDZModel.state == DDZState.GAIQIANG) {
            var startPos = DDZModel.getPosBySeatID(0);
            var card = utils.getValInt(val, "random_card");
            var cardPos = 53 - utils.getValInt(val, "position");
            DDZCards.showFaPaiCard(startPos, card, cardPos);
        }
        if (DDZModel.state == DDZState.TI_LA) {

            if(tlType == 1&&DDZTips.getNoTiPos()!=-1)
            {
                DDZTips.showTime(DDZTips.getNoTiPos(),DDZModel.tilaTime);
            }
            else
            {
                DDZTips.showTime(dzPos,DDZModel.tilaTime);
            }
        }
        else if (DDZModel.state == DDZState.MING_PAI) {
            DDZTips.showTime(dzPos,DDZModel.mingpaiTime);
            DDZTips.startWaitTime(dzPos,DDZModel.mingpaiTime);
        }
        else if (DDZModel.state == DDZState.PREPLAY_ONE||DDZModel.state == DDZState.PREPLAY_TWO ){
            DDZTips.showTime(pos,DDZModel.playTime);
            DDZTips.startWaitTime(pos,DDZModel.playTime);
        }
        else if (DDZModel.state == DDZState.PLAYING) {
            DDZTips.showTime(pos,DDZModel.playTime);
            DDZTips.startWaitTime(pos,DDZModel.playTime);
            DDZCards.showTuoGuan(DDZModel.isTuoguan == 1);
        }
        //我方是否已上桌
        DDZAction.showUptableBtn(!DDZModel.isMyInTable());

        //轮到己方操作
        if (DDZModel.isMySeatID(DDZModel.curSeatID)) {
            //叫抢恢复
            if (DDZModel.state == DDZState.PREPLAY_ONE || DDZModel.state == DDZState.PREPLAY_TWO) {

                 if (DDZModel.tableType == 3) {
                    if (DDZModel.state == DDZState.PREPLAY_ONE) {
                        DDZAction.showJiaoDZBtns(true);
                    }
                    else {
                        DDZAction.showQiangDZBtns(true);
                    }
                }
                 else
                 {
                     DDZAction.showJiaoDZBtns(true);
                 }
            }//出牌恢复
            else if (DDZModel.state == DDZState.PLAYING) {

                DDZModel.mustAll =  utils.getValInt(val,"must_all_out")==1;

                if( DDZModel.mustAll)
                {
                    DDZTips.showMyTip("最后一手必接");
                    DDZCards.showUpCard(DDZCards.handCardValues);
                }
                DDZAction.showOutCardBtns(true);
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

        DDZModel.updatePlayer(val);
        //更新掉线提示

        var seatID =utils.getValInt(val,"seatid");
        var player = DDZModel.getPosBySeatID(seatID);
        player.isOffline = 0;
        if(seatID >=0 && seatID<=2)
        {
            var pos =DDZModel.getPosBySeatID(seatID);
            DDZPlayers.setOffline(pos,false);
            DDZTips.updateTimeOut(pos);
        }
    },
    //广播：上桌成功
    handlerUptableSuccBC:function(val)
    {
        cc.log("handlerUptableSuccBC");
        var player = DDZModel.updatePlayer(val);
        if(DDZModel.isMyUID(player.uid))
        {
            DDZModel.sendRoomInfo();
            DDZModel.sendReady();
            DDZTips.showMyTip("您已上桌");
        }
        else
        {
            DDZPlayers.uptable(player);
        }
        JsUtils.postNotifi("notify_room_check_ip");
    },
    //回调：上桌失败
    handlerUptableErroUC:function(val)
    {
        cc.log("handlerUptableErroUC");
        if(!DDZModel.isMyInTable())
        {
            DDZTips.showMyTip("上桌失败");
        }
    },
    //广播：下桌成功
    handlerDowntableSuccBC:function(val)
    {
        cc.log("handlerDowntableSuccBC");
        var seatID = utils.getValInt(val,"seatid");
        if(seatID>-1)
        {
            var pos = DDZModel.getPosBySeatID(seatID);
            DDZPlayers.downtable(pos);
        }

        if(DDZModel.isMySeatID(seatID))
        {
            DDZModel.unDownSeatID =DDZModel.seatID;
            DDZModel.seatID = -1;
            ZJHModel.getInstance().mySeatid = DDZModel.seatID;
            DDZAction.showUptableBtn(true);
            DDZAction.showReadyBtn(false);
            DDZCards.showTuoGuan(false);
            DDZTips.showMyTip("您已下桌");
        }
    },

    //回调：准备成功
    handlerReadySuccBC:function(val)
    {
        cc.log("handlerReadySuccBC");
        var uid = utils.getValInt(val,"uid");
        var seatid = utils.getValInt(val,"seatid");
        var pos = DDZModel.getPosBySeatID(seatid);
        DDZPlayers.setReady(pos,true);
        if(DDZModel.isMyUID(uid))
        {
            DDZAction.showReadyBtn(false);
        }
       var player = DDZModel.getPlayerBySeatID(seatid);
        if(player)
        {
            player.ready =1;
        }
    },
    //回调：准备失败
    handlerReadyErroUC:function(val)
    {
        cc.log("handlerReadyErroUC");
        if(DDZModel.isMyInTable())
        {
            DDZTips.showMyTip("准备失败");
        }
        else
        {
            DDZAction.showReadyBtn(false);
        }
    },
    //广播：准备开始新游戏
    handlerGamePrereadyBC:function(val)
    {
        cc.log("handlerGamePrereadyBC");
    },

    //广播：玩家表情互动
    handlerEmotionBC:function(val)
    {
        cc.log("handlerEmotionBC");
        var base = utils.getValInt(val,"seatid");
        var target = utils.getValInt(val,"target_seatid");
        var face = utils.getValInt(val,"type");
        if(face != -1 && target!=-1 && base != target &&  !DDZModel.isMySeatID(base) )
        {
            var b = DDZPlayers.getPosition(DDZModel.getPosBySeatID(base));
            var t  =DDZPlayers.getPosition(DDZModel.getPosBySeatID(target));

            DDZTips.showBigface(b,t,face,DDZPlayers.getSex(DDZModel.getPosBySeatID(base)));
        }
    },
    //广播：开始牌局，发牌
    handlerGameStartBC:function(val)
    {
        cc.log("handlerGameStartBC");
        this.resetGame();
        DDZAction.showUptableBtn(!DDZModel.isMyInTable());

        DDZModel.state = DDZState.PREPLAY_ONE;
        DDZCards.showTuoGuan(false);
        var startPos = DDZModel.getPosBySeatID(0);
        var card = utils.getValInt(val,"random_card");
        var cardPos = 53 - utils.getValInt(val,"position");
        DDZCards.fapai(startPos,card,cardPos);
        DDZModel.updateCoin(val);
        DDZTips.setCoin(DDZModel.coin);
        DDZTips.setRatio(DDZModel.getRatio());
        this.stopGameOver();
        JsUtils.postNotifi("notify_game_check_end");

        DDZModel.sendGetPlayerList();

    },

    //广播：开始盖抢
    handlerStartGaiQiangBC:function(val)
    {
        cc.log("handlerStartGaiQiangBC");
        DDZModel.state = DDZState.GAIQIANG;
        DDZModel.tableType=2;
        DDZAction.showJiaoDZBtns(true);
    },
    //回调：盖抢成功
    handlerGaiQiangSuccUC:function(val)
    {
        cc.log("handlerGaiQiangSuccUC");
        DDZModel.state = DDZState.GAIQIANG;
        var seatID = utils.getValInt(val,"seatid");
        var pos = DDZModel.getPosBySeatID(seatID);
        var type = val["gaiqiang"];

        if(type == 2)
        {
            Sound.getInstance().playEffect(utils.format(PackName+"/res/sound/jiaofen/%1_jiaodizhu.mp3"
                ,DDZPlayers.getSex(pos)));
            DDZTips.showJiaoQiang(pos,3);
        }
        else
        {
            Sound.getInstance().playEffect(utils.format(PackName+"/res/sound/jiaofen/%1_bujiao.mp3"
                ,DDZPlayers.getSex(pos)));
            DDZTips.showJiaoQiang(pos,5);
        }

        DDZTips.showGaiQiang(pos,type);
        DDZAction.showJiaoDZBtns(false);
    },
    //广播：盖抢结果
    handlerGaiQiangResultBC:function(val)
    {
        cc.log("handlerGaiQiangResultBC");
        DDZModel.state = DDZState.GAIQIANG;

        DDZAction.showJiaoDZBtns(false);
        for(var i in val["result"] )
        {

            var pos = DDZModel.getPosBySeatID(val["result"][i]["seatid"]);
            var type = val["result"][i]["gaiqiang"];
            if(type == 2)
            {
                if(!DDZModel.fastShow) {
                    Sound.getInstance().playEffect(utils.format(PackName + "/res/sound/jiaofen/%1_jiaodizhu.mp3"
                        , DDZPlayers.getSex(pos)));
                }
                DDZTips.showJiaoQiang(pos,3);
            }
            else
            {
                if(!DDZModel.fastShow) {
                    Sound.getInstance().playEffect(utils.format(PackName + "/res/sound/jiaofen/%1_bujiao.mp3"
                        , DDZPlayers.getSex(pos)));
                }
                DDZTips.showJiaoQiang(pos,5);
            }
            DDZTips.showGaiQiang(pos,type);
        }
    },

    //回调：看牌
    handlerGameStartUC:function(val)
    {
        cc.log("handlerGameStartUC");

        DDZModel.state = DDZState.PREPLAY_ONE;
        if(val.hasOwnProperty("holes"))
        {
            DDZCards.handCardValues =[];
            DDZCards.pushHandCard(val["holes"]);
        }
        var self = this;
        var ac = cc.repeatForever(cc.sequence(cc.delayTime(1),cc.callFunc(function(){

                    DDZCards.showHandCards(0,17);
                    DDZCards.showHandCards(1,17);
                    DDZCards.showHandCards(2,17);
                self.stopGameOver();
                self.stopActionByTag(666666);

        })));
        ac.setTag(666666);
        this.runAction(ac);
    },

    //广播：叫地主
    handlerStartJiaoDiZhuBC:function(val)
    {
        cc.log("handlerStartJiaoDiZhuBC");
        DDZModel.state = DDZState.PREPLAY_ONE;

        DDZTips.hideThinkTips();
       // DDZTips.hideTime();

        DDZModel.tableType = utils.getValInt(val,"table_type");
        var seatID = utils.getValInt(val,"seatid");
        var action = utils.getValInt(val,"action");
        var pos = DDZModel.getPosBySeatID(seatID);
        DDZTips.showTime(pos,DDZModel.playTime);
        DDZTips.startWaitTime(pos,DDZModel.playTime);


        if(!DDZModel.isMyInTable())
        {
            DDZCards.showHandCards(0,17);
            DDZCards.showHandCards(1,17);
            DDZCards.showHandCards(2,17);
        }
        if(DDZModel.isMySeatID(seatID)) {

            DDZAction.showJiaoDZBtns(true);

        }
    },
    //广播：叫地主结果
    handlerJiaoDiZhuSuccBC:function(val)
    {
        cc.log("handlerJiaoDiZhuSuccBC");

        var seatID = utils.getValInt(val,"seatid");
        var action = utils.getValInt(val,"action");
        var pos = DDZModel.getPosBySeatID(seatID);
        DDZModel.tableType = utils.getValInt(val,"table_type");

        DDZModel.updateCoin(val);
        DDZModel.updateRatio(val);
        DDZTips.setCoin(DDZModel.coin);
        DDZTips.setRatio(DDZModel.getRatio());

        DDZTips.showTimeOut(pos,false);
       // DDZTips.hideTime();
        DDZTips.stopWaitTime(pos);

        if(DDZModel.isMySeatID(seatID))
        {
            DDZAction.showJiaoDZBtns(false);
        }
        if(DDZModel.tableType ==3)//叫抢
        {

            if(!DDZModel.fastShow) {
                if (action >= 1 && action <= 3) {
                    Sound.getInstance().playEffect(utils.format(PackName + "/res/sound/jiaofen/%1_jiaodizhu.mp3"
                        , DDZPlayers.getSex(pos)));
                }
                else {
                    Sound.getInstance().playEffect(utils.format(PackName + "/res/sound/jiaofen/%1_bujiao.mp3"
                        , DDZPlayers.getSex(pos)));
                }
            }
            DDZTips.showJiaoQiang(pos,action);
        }
        else
        {
        }

        DDZTips.showMyTip("");
    },
    //广播：抢地主
    handlerStartQiangDiZhuBC:function(val)
    {
        cc.log("handlerStartQiangDiZhuBC");
        DDZModel.state = DDZState.PREPLAY_TWO;
        var seatID = utils.getValInt(val,"seatid");
        var pos = DDZModel.getPosBySeatID(seatID);
        if(DDZModel.isMySeatID(seatID))
        {
            DDZAction.showQiangDZBtns(true);
        }
       // DDZTips.hideTime();
        DDZTips.showJiaoQiang(pos,0);

        DDZTips.showTime(pos,DDZModel.playTime);
        DDZTips.startWaitTime(pos,DDZModel.playTime);


    },
    //广播：抢地主结果
    handlerQiangDiZhuSuccBC:function(val)
    {
        cc.log("handlerQiangDiZhuSuccBC");
        DDZModel.updateCoin(val);
        DDZModel.updateRatio(val);
        DDZTips.setCoin(DDZModel.coin);
        DDZTips.setRatio(DDZModel.getRatio());


        var action = utils.getValInt(val,"action");
        var seatID = utils.getValInt(val,"seatid");
        var pos = DDZModel.getPosBySeatID(seatID);
        if(DDZModel.isMySeatID(seatID))
        {
            DDZAction.showQiangDZBtns(false);
        }
        DDZTips.showJiaoQiang(DDZModel.getPosBySeatID( seatID),action);
        DDZTips.showTimeOut(pos,false);
       // DDZTips.hideTime();
        DDZTips.stopWaitTime(pos);

        if(!DDZModel.fastShow) {
            if (action >= 1 && action <= 4) {
                Sound.getInstance().playEffect(utils.format(PackName + "/res/sound/jiaofen/%1_qiangdizhu.mp3"
                    , DDZPlayers.getSex(pos)));
            }
            else {
                Sound.getInstance().playEffect(utils.format(PackName + "/res/sound/jiaofen/%1_buqiang.mp3"
                    , DDZPlayers.getSex(pos)));
            }
        }

    },
    //广播：当地主
    handlerDiZhuBC:function(val)
    {
        cc.log("handlerDiZhuBC");
        DDZModel.state = DDZState.PLAYING;
        DDZModel.dzSeatID = utils.getValInt(val,"landlord");
        DDZModel.updateCoin(val);
        DDZModel.updateRatio(val);
        DDZTips.setCoin(DDZModel.coin);
        DDZTips.setRatio(DDZModel.getRatio());

        var pos = DDZModel.getPosBySeatID( DDZModel.dzSeatID);
        DDZCards.setDiZhuPos(pos);
        if(val.hasOwnProperty("comms"))
        {
            for(var i in val["comms"])
            {
                DDZCards.showPublicCard(i,val["comms"][i]);
            }

            if(DDZModel.isMyDZ())
            {
                DDZCards.pushHandCard(val["comms"]);
            }
            else if(DDZModel.isOnVideo)
            {
                DDZCards.pushShowCard(pos,val["comms"]);
            }
        }
        for(var i=0;i<3;++i)
        {
            DDZCards.showHandCards(i,pos ==i ?21:17);
        }
        DDZTips.hidePassTips();
        DDZTips.hideThinkTips();
       // DDZTips.hideTime();
        DDZTips.showDiZhu(pos);
        DDZTips.stopWaitTime(pos);
    },
    //广播：无地主
    handlerNoDiZhuBC:function(val)
    {
        cc.log("handlerNoDiZhuBC");
        DDZTips.showNoDiZhu(true);
    },
    //广播：踢拉
    handlerStartTiLaBC:function(val)
    {
        cc.log("handlerStartTiLaBC");
        var tlType =  utils.getValInt(val,"tila_type");

        DDZModel.state = DDZState.TI_LA;
        DDZTips.hideJiaoQiangTip();
        DDZTips.hideThinkTips();
       // DDZTips.hideTime();
        for(var i in val["seatid"])
        {
            var seatID = val["seatid"][i];

            if(DDZModel.isMySeatID(seatID))
            {
                DDZAction.showTiLaBtns(tlType,true);
            }
            DDZTips.startWaitTime(DDZModel.getPosBySeatID( seatID),DDZModel.tilaTime);
            DDZTips.showTime(DDZModel.getPosBySeatID(seatID),DDZModel.tilaTime);
        }
    },
    //广播：踢拉结果
    handlerTiLaSuccBC:function(val)
    {
        cc.log("handlerTiLaSuccBC");
        var tlType =  utils.getValInt(val,"tila_type");
        var tlAction =  utils.getValInt(val,"tila");
        var seatID = utils.getValInt(val,"seatid");
        var pos = DDZModel.getPosBySeatID(seatID);

        DDZModel.updateRatio(val);
        DDZTips.setRatio(DDZModel.getRatio());

        DDZTips.showTimeOut(pos,false);
        DDZTips.stopWaitTime(pos);

        if(DDZModel.isMySeatID(seatID))
        {
            DDZAction.showTiLaBtns(tlType,false);
            if(tlAction == 2) {
                DDZTips.showJiaBeiEf(tlType == 1 ? DDZJiaBei.TI:DDZJiaBei.LA,DDZModel.addRatios[DDZRatio.TI_LA]);
            }

        }
        DDZTips.showTiLa(pos,tlType,tlAction);

        if(tlType == 1&&DDZTips.getNoTiPos()!=-1)
        {
            DDZTips.showTime(DDZTips.getNoTiPos(),DDZModel.tilaTime);
        }
        else
        {
            DDZTips.showTime(DDZModel.getDzPos(),DDZModel.tilaTime);
        }

    },
    //回调：开始明牌
    handlerStartMingPaiUC:function(val)
    {
        cc.log("handlerStartMingPaiUC");
        DDZModel.state = DDZState.MING_PAI;
        var seatID = utils.getValInt(val,"seatid");
        var pos = DDZModel.getPosBySeatID(seatID);
        if(DDZModel.isMySeatID(seatID))
        {
            DDZAction.showMingPaiBtns(true);
        }
        DDZTips.hideJiaoQiangTip();
        DDZTips.hideThinkTips();
       // DDZTips.hideTime();
        DDZTips.showTime(pos,DDZModel.mingpaiTime);
        DDZTips.startWaitTime(pos,DDZModel.mingpaiTime);


        if(DDZTips.needBuLa())
        {
            DDZTips.showTiLa(DDZModel.getPosBySeatID(DDZModel.dzSeatID),2,1);
        }

    },
    //广播：明牌结果
    handlerMingPaiSuccBC:function(val)
    {
        cc.log("handlerMingPaiSuccBC");
        var seatID = utils.getValInt(val,"seatid");
        var pos = DDZModel.getPosBySeatID(seatID);
        DDZAction.showMingPaiBtns(false);
        DDZTips.hideJiaoQiangTip();
        DDZTips.hideThinkTips();
       // DDZTips.hideTime();
        DDZTips.stopWaitTime(pos);

        DDZModel.updateRatio(val);
        DDZTips.setRatio(DDZModel.getRatio());
        DDZCards.setMingPaiPos(pos);
        DDZCards.pushShowCard(pos,val["holes"]);
        DDZCards.showHandCards(pos,DDZCards.showCardValues[pos].length);
        DDZTips.showJiaBeiEf(DDZJiaBei.MING_PAI,DDZModel.addRatios[DDZRatio.MING_PAI]);

        Sound.getInstance().playEffect(utils.format(PackName+"/res/sound/jiaofen/%1_mingpai.mp3"
            ,DDZPlayers.getSex(pos)));
        DDZTips.showMp(pos);
    },
    //广播：进入出牌
    handlerStartOutCardBC:function(val)
    {
        cc.log("handlerStartOutCardBC");
        DDZModel.state = DDZState.PLAYING;

        DDZAction.showMingPaiBtns(false);
        DDZTips.hideJiaoQiangTip();
        DDZTips.hideThinkTips();
        //DDZTips.hideTime();
        DDZTips.stopWaitTime(0);
        DDZTips.stopWaitTime(1);
        DDZTips.stopWaitTime(2);
        if(DDZTips.needBuLa())
        {
            DDZTips.showTiLa(DDZModel.getPosBySeatID(DDZModel.dzSeatID),2,1);
        }
        var seatID = utils.getValInt(val,"seatid");
        var pos = DDZModel.getPosBySeatID(seatID);
        DDZModel.curSeatID = seatID;
        DDZModel.outCardMode =utils.getValInt(val,"action");

        DDZCards.showOutCards(pos,[]);
        DDZTips.showOutCard(pos,0);
        if(DDZModel.isMySeatID(seatID))
        {
            DDZModel.mustAll =  utils.getValInt(val,"must_all_out")==1;

            if( DDZModel.mustAll)
            {
                DDZTips.showMyTip("最后一手必接");
                DDZCards.showUpCard(DDZCards.handCardValues);
            }
            if(DDZModel.isTuoguan !=1)
            {
                DDZAction.showOutCardBtns(true);
            }
        }
        DDZTips.showTime(pos,DDZModel.playTime);
        DDZTips.startWaitTime(pos,DDZModel.playTime);
    },
    //广播：出牌结果
    handlerOutCardSuccBC:function(val)
    {
        cc.log("handlerOutCardSuccBC");
        var cardType = utils.getValInt(val,"card_type");
        var cardNum = utils.getValInt(val,"card_nums");
        var seatID = utils.getValInt(val,"seatid");
        var pos = DDZModel.getPosBySeatID(seatID);
        DDZModel.updateCoin(val);
        DDZModel.updateRatio(val);
        DDZTips.setCoin(DDZModel.coin);
        DDZTips.setRatio(DDZModel.getRatio());
        DDZTips.showAllTip("");
        DDZTips.hideThinkTips();
        DDZTips.showTimeOut(pos,false);
        //DDZTips.hideTime();
        DDZTips.stopWaitTime(pos);
        DDZModel.outCardType = cardType;
        DDZCards.resetTipCard();
        var isPass = true;
        if(val.hasOwnProperty("cards"))
        {
            for(var i in val["cards"])
            {
                if(val["cards"][i] > 0)
                {
                    isPass = false;
                    break;
                }
            }
        }
        if(!isPass)
        {
            DDZModel.lastSeatID = seatID;
            DDZCards.outCardValues = [];
            for(var i in val["cards"])
            {
                if(val["cards"][i] > 0)
                {
                    if(DDZModel.isOnVideo ||  DDZCards.mpPos == pos) {
                        var v = val["cards"][i];
                        DDZCards.deleteCard(DDZCards.showCardValues[pos],v);
                    }
                }
            }
            DDZCards.pushOutCard(val["cards"]);
            DDZCards.showOutCards(pos, DDZCards.outCardValues);
            DDZCards.showHandCards(pos,cardNum);
            DDZTips.showHandCardJB(pos,cardNum>=1&&cardNum<=2);

            if(!DDZModel.fastShow) {
                if (cardNum > 0 && cardNum <= 2) {
                    Sound.getInstance().playEffect(utils.format(PackName + "/res/sound/baojing/%1_baojing%2.mp3"
                        , DDZPlayers.getSex(pos), cardNum));
                }
                if (cardType == libddz.CardsType.CARD_TYPE_ONE) {
                    var sf = utils.format(PackName + "/res/sound/danpai/%1_%2_card.mp3"
                        , (DDZCards.outCardValues[0].value % 16).toString(16), DDZPlayers.getSex(pos));
                    Sound.getInstance().playEffect(sf);
                }
                else if (cardType == libddz.CardsType.CARD_TYPE_TWO) {
                    var sf = utils.format(PackName + "/res/sound/duizi/%1_card_dui_0%2.mp3"
                        , DDZPlayers.getSex(pos), (DDZCards.outCardValues[0].value % 16).toString(16));
                    Sound.getInstance().playEffect(sf);
                }
                else {
                    Sound.getInstance().playEffect(utils.format(PackName + "/res/sound/other/%1_type_%2.mp3"
                        , DDZPlayers.getSex(pos), cardType));
                }
            }
        }
        else
        {
            DDZTips.showOutCard(pos,true);
            if(!DDZModel.fastShow) {
                var sf = utils.format(PackName + "/res/sound/pass/%1_buyao%2.mp3"
                    , DDZPlayers.getSex(pos), utils.random(0, 2));
                Sound.getInstance().playEffect(sf);
            }
        }
        if(DDZModel.isMySeatID(seatID))
        {
            DDZCards.initUpCard();
        }
        DDZAction.showOutCardBtns(false);
        DDZTips.showCardEf(cardType);
        if(cardType == libddz.CardsType.CARD_TYPE_BOMB)
        {
            DDZTips.showJiaBeiEf(DDZJiaBei.BOMB,DDZModel.addRatios[DDZRatio.BOMB]);
        }
        else if(cardType == libddz.CardsType.CARD_TYPE_ROCKET)
        {
            DDZTips.showJiaBeiEf(DDZJiaBei.ROCKET,DDZModel.addRatios[DDZRatio.ROCKET]);
        }
    },
    handlerOutCardErrorUC:function(val)
    {
        cc.log("handlerOutCardErrorUC");
        DDZTips.showMyTip("出牌有误，请重新出牌");
        if (val.hasOwnProperty("holes")) {
            DDZCards.handCardValues = [];
            DDZCards.pushHandCard(val["holes"]);
            DDZCards.showHandCards(0,0);
        }

    },
    //广播：托管结果
    handlerRobotBC:function(val)
    {
        cc.log("handlerRobotBC");
        var robot = utils.getValInt(val,"robot");
        var seatID = utils.getValInt(val,"seatid");
        var pos = DDZModel.getPosBySeatID(seatID);
        if(DDZModel.isMySeatID(seatID))
        {
            DDZModel.isTuoguan = robot;
            DDZCards.showTuoGuan(robot==1);
            DDZCards.initUpCard();
        }
        DDZPlayers.setTuoguan(pos,robot ==1);
    },
    //广播：此局游戏结束，进入结算
    handlerGameEndBC:function(val)
    {
        cc.log("handlerGameEndBC");
        DDZModel.state = DDZState.END_GAME;

        DDZModel.updateRatio(val);
        DDZTips.setRatio(DDZModel.getRatio());

        DDZTips.hideTime();
        DDZTips.stopWaitTime(0);
        DDZTips.stopWaitTime(1);
        DDZTips.stopWaitTime(2);


        DDZTips.showHandCardJB(0,false);
        DDZTips.showHandCardJB(1,false);
        DDZTips.showHandCardJB(2,false);
        DDZTips.showAllTip("");
        DDZTips.showMyTip("");
        DDZTips.hideOutCardTip();
        DDZCards.showTuoGuan(false);

        var winType = utils.getValInt(val,"win_type");

        if(utils.getValInt(val,"spring") == 1)
        {
            DDZTips.showJiaBeiEf(winType,DDZModel.addRatios[DDZRatio.SPRING]);
            Sound.getInstance().playEffect(PackName+"/res/other/f_type_sprite.mp3");
        }
        DDZCards.handCardValues=[];
        for(var i in val["players"])
        {
            var d = val["players"][i];
            var seatID = utils.getValInt(d,"seatid");
            var pos = DDZModel.getPosBySeatID(seatID);
            if(d.hasOwnProperty("holes"))
            {
                var cards =DDZCards.createCards(d["holes"]);
                if(DDZModel.isMySeatID(seatID))
                {
                    DDZCards.showOutCards(pos,cards);
                    DDZCards.showHandCards(pos,0);
                }
                else
                {
                    DDZCards.showHandCards1(pos,cards);
                }
            }

            var winCoin = utils.getValInt(d,"cur_money");
            var curCoin = utils.getValInt(d,"money");
            var player = DDZModel.getPlayerBySeatID(seatID);
            player.total_board = utils.getValInt(d,"total_board");
            player.total_win = utils.getValInt(d,"total_win");
            player.money = curCoin;

            if(winCoin>0)
            {
                DDZPlayers.showWinEf(pos,true);
            }

            if (DDZModel.isMySeatID(seatID))
            {
                player.ready =0;
                Sound.getInstance().playEffect(utils.format(PackName+"/res/sound/effect/%1_%2_sound.mp3"
                        ,DDZModel.isDZSeatID(seatID) ? "dz" : "nm", winCoin>0 ? "win" : "lose"));
            }

            DDZTips.showWinCoin(pos,winCoin);
            DDZPlayers.updateInfo(pos);
            DDZPlayers.setTuoguan(pos,false);
        }

        if(DDZModel.isOnVideo)
        {
            return;
        }
        this.runAction(cc.sequence(cc.delayTime(5),cc.callFunc(function(){
            if(DDZModel.isMyInTable()&&DDZModel.getPlayerByPos(0).ready !=1)
            {
                DDZAction.showReadyBtn(true);
            }
            DDZTips.showWaitTip("等待游戏开始");
        })));
        if(DDZModel.isGameOver)
        {
            this.onGameOver();
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
        DDZModel.isGameOver = true;
        ZJHModel.getInstance().roomToShareView(DDZModel.curRoomID);
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
        var player = DDZModel.getPlayerByUID(val["uid"]);
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
            //PlatformHelper.showToast("房间解散成功，3秒后房间自动结束");
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
        var player = DDZModel.getPlayerBySeatID(seatID);
        if(player)
        {
            player.isOffline = 1;
            if(seatID >-1)
            {
                var  pos = DDZModel.getPosBySeatID(seatID);
                DDZPlayers.setOffline(pos,true);
                DDZTips.checkTimeOut(pos);
            }
        }
    }
});
