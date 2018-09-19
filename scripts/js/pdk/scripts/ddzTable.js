

var DDZGameState ={
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

var DDZTable = cc.Layer.extend({
    uiNode:null,
    uiAction:null,
    retNetNum:0,
    showtimeFlag:[],
    outCardTip:false,
    isresetGame:false,

    isshowSpiltCards:false,

    getUI:function(key)
    {
        return this.uiNode.getChildByName(key);
    },
    ctor:function () {

        this._super();
        var self = this;

        cc.spriteFrameCache.addSpriteFrames("public/res/img/bigface.plist");
        cc.spriteFrameCache.addSpriteFrames("pdk/res/img/ef.plist");
        this.uiJson = ccs.load(PackName+"/res/table.json");
        this.uiNode =this.uiJson.node;
        this.addChild(this.uiNode);

        //初始化数据模型
        DDZModel.reset();
        DDZModel.roomData = JSON.parse( ZJHModel.getInstance().getRoomData());
        DDZModel.curRoomID = ZJHModel.getInstance().curRoomID;
        DDZModel.curGameType = ZJHModel.getInstance().curGameType;

        this.initView();
        Sound.getInstance().PlayBgSound(PackName+"/res/sound/game_bgm.mp3");

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
                cc.log("will_exit_room1");
            }
        });
        cc.eventManager.addListener(this._listenerExit,this);

        for(var i = 0; i<3;i++)
        {
            this.showtimeFlag[i] = false;
        }

        DDZModel.isOnVideo = ZJHModel.getInstance().isOnVideo;
        if( DDZModel.isOnVideo) {

            this.videoBox = new DDZVideo();
            this.addChild(this.videoBox,111111);
            this.videoBox.runScript(this);
        }
        else {
            //监听联网状态
            this.schedule(this.updateGame,0.0);
            this.schedule(this.hearbeat,3.0);


            DDZPlayers.publicBox = RoomPublic.create();
            DDZPlayers.publicBox.setCallback(function(tag)
            {
                if(tag == "exit")
                {
                    DDZModel.sendDowntable();
                    DDZModel.sendLogout();
                }
                else if(tag == "end_room")
                {
                    DDZModel.sendReqRoomEnd();
                }
                else if(tag == "down_table")
                {
                    DDZModel.sendDowntable();
                }
                else if(tag == "game_setting")
                {
                    if( ZJHModel.getInstance().gameUseDefaultSetting != undefined)
                    {
                    }
                }
            });

            this.addChild(DDZPlayers.publicBox,10000);

            //部分事件监听
            this._listenerGameOver = cc.EventListener.create({
                event:cc.EventListener.CUSTOM,
                eventName:"notify_against_room_end",
                callback:function(evt){
                    self.onGameOver();
                    cc.log("notify_against_room_end");
                }
            });
            cc.eventManager.addListener(this._listenerGameOver,this);

            this._listenerHide = cc.EventListener.create({
                event:cc.EventListener.CUSTOM,
                eventName:"game_on_hide",
                callback:function(evt){
                    DDZModel.resetResponse();
                    CCTcpClient.getInstance(DDZModel.curRoomID).disconnect(1);
                }
            });
            cc.eventManager.addListener(this._listenerHide,this);


            DDZModel.initNet( DDZModel.roomData["gameRoom"]["way"]["ip"],Number(DDZModel.roomData["gameRoom"]["way"]["port"]));
            DDZModel.connectNet();
        }



        return true;
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
            DDZModel.waitUpdateInfo = true;

            DDZModel.lastHearbeatTime =0;
            DDZModel.connectNet();

            return;
        }
        //心跳包超时
        if(DDZModel.lastHearbeatTime != 0 && (curTime - DDZModel.lastHearbeatTime ) > 10)
        {
            DDZModel.connectNet();

            DDZModel.lastHearbeatTime = curTime;
            if( this.retNetNum >0)
            {
                DDZTips.showAllTip("网络连接异常，正在重试");
            }
            this.retNetNum ++;
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

                if (!DDZModel.isLogout) {
                    DDZModel.sendLogin();
                }
            }
            else if( response.cmd ==TCPSendCMD.RECV_DATA_OK_RES)
            {
                cc.log("netTime______get:"+utils.getTimeStamp()+" _cmd:"+response.id);
                this.handlerCMD(response.id, response.data, true);
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

        for(var i = 0; i<3;i++)
        {
            this.showtimeFlag[i] = false;
        }
        this.outCardTip = false;
        this.isresetGame = true;

        this.isshowSpiltCards = false;

        DDZModel.nextPlayisSingle = -1;
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
                DDZModel.isGameOver = true;
                DDZModel.sendOver();
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
        if( val["voters_uid"])
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
        cc.log("onBtn:"+key);
        switch(key)
        {

            case "readyBtn":
                DDZModel.sendReady(1);
                break;
            case "noreadyBtn":
                DDZModel.sendReady(0);
                break;
            case "tuoguanBtn":


                if(DDZModel.state != DDZState.PLAYING)
                {
                    if( DDZModel.state == DDZState.MINGPAI_START_STATE)
                    {
                        DDZTips.showMyTip("选择明牌发牌阶段无法托管");
                    }

                    else if( DDZModel.state == DDZState.QIANGGUAN_STATE)
                    {
                        DDZTips.showMyTip("抢关阶段无法托管");
                    }
                    else if( DDZModel.state == DDZState.BASE_DOUBLE_STATE)
                    {
                        DDZTips.showMyTip("选择底分阶段无法托管");
                    }
                    else
                    {
                        DDZTips.showMyTip("游戏未开始无法托管");
                    }
                    return;
                }
                DDZModel.sendTuoguan();
                break;
            case "uptableBtn":
                DDZModel.sendUptable();
                break;
            case "bupiaoBtn":
                DDZModel.sendPiao(0);
                break;
            case "piaoBtn0":
                DDZModel.sendPiao(DDZModel.piaoCoins[0]);
                break;
            case "piaoBtn1":
                DDZModel.sendPiao(DDZModel.piaoCoins[1]);
                break;
            case "piaoBtn2":
                DDZModel.sendPiao(DDZModel.piaoCoins[2]);
                break;
            case "piaoBtn3":
                DDZModel.sendPiao(DDZModel.piaoCoins[3]);
                break;
            case "bujiaoBtn":
            case "buqiangBtn":
                DDZModel.sendJiaoDZ(5);
                break;
            case "jiaodzBtn":
                DDZModel.sendJiaoDZ(3);
                break;
            case "qiangdzBtn":
                DDZModel.sendJiaoDZ(4);
                break;
            case "jiaofenBtn0":
                DDZModel.sendJiaoDZ(1);
                break;
            case "jiaofenBtn1":
                DDZModel.sendJiaoDZ(2);
                break;
            case "jiaofenBtn2":
                DDZModel.sendJiaoDZ(3);
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
                DDZModel.chupaiNum++;
                if( DDZModel.chupaiNum>3)
                {
                    DDZTips.showMyTip("网络卡顿，请稍候。。。");
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
                DDZAction.checkOutCard();
                DDZAction.pdkcheckOutCard();
                break;


            //跑的快的一些按钮
            case "pdk_mingpaiBtn":
                DDZModel.sendPdkCommonBtns(2,1);
                break;
            case "pdk_bumingpaiBtn":
                DDZModel.sendPdkCommonBtns(1,1);
                break;
            case "qiangguanBtn":
                DDZModel.sendPdkCommonBtns(2,2);
                break;
            case "buqiangguanBtn":
                DDZModel.sendPdkCommonBtns(1,2);
                break;
            case "pdk_jiabeiBtn":
                DDZModel.sendPdkCommonBtns(2,3);
                break;
            case "pdk_bujiabeiBtn":
                DDZModel.sendPdkCommonBtns(1,3);
                break;
            case "pdk_omingpaiBtn":
                DDZModel.sendPdkCommonBtns(2,4);
                break;
            case "pdk_obujiabeiBtn":
                DDZModel.sendPdkCommonBtns(1,4);
                break;
            case "pdk_tishiBtn":
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
            case "pdk_chupaiBtn":
                if( DDZAction.getUI("pdk_chupaiBtn").getChildByName("enable").isVisible())
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
                DDZModel.chupaiNum++;
                if( DDZModel.chupaiNum>3)
                {
                    DDZTips.showMyTip("网络卡顿，请稍候。。。");
                }
                DDZModel.sendOutCards(true);
                break;
            case "pdk_splitcardBtn":
                DDZCards.showsplitcardAni();
                break;
            case "pdk_passsplitBtn":
                DDZCards.hideAllsplitCardTip();
                DDZModel.sendPdkSplitCard(1,1);
                break;
            case "pdk_sureBtn":
                DDZModel.sendPdkSplitCard(4,0);
                break;
        }

    },

    //*************************** 服务器返回信息处理 ************************************//
    //处理联网回调
    handlerCMD:function(cmd1,data,isDelay)
    {

        var val;
        var cmd =cmd1;
        if(cmd1 == 0)
        {
            try{
                val = JSON.parse(data);
            }
            catch(e)
            {
                cc.log("_error__data:"+data);
                return;
            }
            cmd = val.cmd;
        }
        cc.log("___cmd:"+cmd);
        DDZModel.fastShow = !isDelay;
        DDZModel.lastHearbeatTime = (new Date).valueOf()/1000;
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

                if(cmd1 != 0)
                {
                    val = parsePacket("PGame.TableInfo", data);
                    cc.log("___data:"+JSON.stringify(val) );
                }
                this.handlerTableInfoUC(val);
                break;
            //登录成功
            case NetCMD.SERVER_LOGIN_SUCC_UC:
                cc.log("登录成功");
                break;
            //登录成功
            case NetCMD.SERVER_LOGIN_SUCC_BC:
                if(cmd1 != 0) {
                    val = parsePacket("PGame.UptableSuccAck", data);
                }
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
                if(cmd1 != 0) {
                    val = parsePacket("PGame.UptableSuccAck", data);
                }
                this.handlerUptableSuccBC(val);
                break;
            //上桌失败
            case NetCMD.SERVER_UPTABLE_ERR_UC:
                this.handlerUptableErroUC(val);
                break;
            //下桌成功
            case NetCMD.SERVER_DOWNTABLE_SUCC_BC:
                if(cmd1 != 0) {
                    val = parsePacket("PGame.DowntableSuccAck", data);
                }
                this.handlerDowntableSuccBC(val);
                break;
            //准备成功
            case NetCMD.SERVER_READY_SUCC_BC:
                if(cmd1 != 0) {
                    val = parsePacket("PGame.ReadyAck", data);
                }
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
            //开始飘
            // case NetCMD.SERVER_START_PIAO_BC:
            //     this.handlerStartPiaoBC(val);
            //     break;
            //飘失败
            // case NetCMD.SERVER_PIAO_ERR_UC:
            //     this.handlerPiaoErroUC(val);
            //     break;
            //飘结果
            // case NetCMD.SERVER_PIAO_SUCC_BC:
            //     this.handlerPiaoSuccBC(val);
            //     break;
            //看牌
            case NetCMD.SERVER_GAME_START_UC:
                if(cmd1 != 0) {
                    val = parsePacket("PGame.GameStartUc", data);
                }
                this.handlerGameStartUC(val);
                break;
            //叫地主
            // case NetCMD.SERVER_PREPLAY_ONE_BC:
            //     this.handlerStartJiaoDiZhuBC(val);
            //     break;
            //叫地主结果
            // case NetCMD.SERVER_PREPLAY_ONE_SUCC_BC:
            //     this.handlerJiaoDiZhuSuccBC(val);
            //     break;
            //抢地主
            // case NetCMD.SERVER_PREPLAY_TWO_BC:
            //     this.handlerStartQiangDiZhuBC(val);
            //     break;
            //抢地主结果
            // case NetCMD.SERVER_PREPLAY_TWO_SUCC_BC:
            //     this.handlerQiangDiZhuSuccBC(val);
            //     break;
            //当地主广播
            // case NetCMD.SERVER_LANDLORD_BC:
            //     this.handlerDiZhuBC(val);
            //     break;
            //流局
            // case NetCMD.SERVER_NO_DIZHU_BC:
            //     this.handlerNoDiZhuBC(val);
            //     break;
            //必当地主
            // case NetCMD.SERVER_MUST_BE_LANDLORD_BC:
            //     break;
            //开始踢拉
            // case NetCMD.SERVER_START_TILA_BC:
            //     this.handlerStartTiLaBC(val);
            //     break;
            //踢拉结果
            // case NetCMD.SERVER_TILA_SUCC_BC:
            //     this.handlerTiLaSuccBC(val);
            //     break;

            //明牌结果
            // case NetCMD.SERVER_MINGPAI_SUCC_BC:
            //     this.handlerMingPaiSuccBC(val);
            //     break;
            //进入出牌
            case NetCMD.SERVER_PLAY_CARD_BC:

                if(cmd1 != 0) {
                    val = parsePacket("PGame.PlayCardAck", data);
                }
                this.handlerStartOutCardBC(val);
                break;
            //出牌结果
            case NetCMD.SERVER_PLAY_CARD_SUCC_BC:

                if(cmd1 != 0) {
                    val = parsePacket("PGame.PlayCardSuccAck", data);
                }
                this.handlerOutCardSuccBC(val);
                break;
            //出牌失败
            case NetCMD.SERVER_PLAY_CARD_ERR_BC:
                cc.log("SERVER_PLAY_CARD_ERR_BC");
                break;
            //对局结束，进行结算
            case NetCMD.SERVER_GAME_END_BC:

                if(cmd1 != 0) {
                    val = parsePacket("PGame.GameEndAck", data);
                    cc.log("___data:"+JSON.stringify(val) );
                }
                this.handlerGameEndBC(val);
                break;
            //托管结果
            case NetCMD.SERVER_ROBOT_BC:
                // this.handlerRobotBC(val);
                break;
            //玩家掉线状态
            case NetCMD.SERVER_SERVER_PLAYER_OFFLINE_BC:

                if(cmd1 != 0) {
                    val = parsePacket("PGame.OfflineAck", data);
                }
                this.handlerPlayerOfflineBC(val);
                break;
            //表情互动
            case NetCMD.SERVER_EMOTION_BC:

                if(cmd1 != 0) {
                    val = parsePacket("PGame.EmotionAck", data);
                }
                this.handlerEmotionBC(val);
                break;
            //房间对局结束
            case NetCMD.SERVER_DZ_GAME_START_RES:
            case NetCMD.SERVER_PRE_END_GAME:
                this.handlerGameOverBC(val);
                break;
            //申请结束房间失败
            case NetCMD.SERVER_APPLY_DISSOLVE_ROOM_ERR_UC:

                if(cmd1 != 0) {
                    val = parsePacket("PGame.ApplyDisRoomErrAck", data);
                }
                this.handlerReqEndRoomErroUC(val);
                break;
            //进行投票通知
            case NetCMD.SERVER_APPLY_DISSOLVE_ROOM_SUCC_BC:

                if(cmd1 != 0) {
                    val = parsePacket("PGame.ApplyDisRoomSuccAck", data);
                }
                this.handlerReqEndRoomSuccBC(val);
                break;
            //投票状态
            case NetCMD.SERVER_DISSOLVE_ACTION_SUCC_BC:
                if(cmd1 != 0)
                {
                    val = parsePacket("PGame.VoteDisRoomAck", data);
                }
                this.handlerEndRoomActionBC(val);
                break;
            //投票结果
            case NetCMD.SERVER_DISSOLVE_ROOM_RESULT_BC:
                if(cmd1 != 0)
                {
                    val = parsePacket("PGame.DisRoomResultAck", data);
                }
                this.handlerEndRoomResultBC(val);
                break;


            //跑的快的操作
            case NetCMD.SERVER_COMMON_OPT_START_BC:
                DDZCards.hideAllsplitCardTip();
                if(cmd1 != 0) {
                    val = parsePacket("PGame.UserComOptBC", data);
                }
                this.handlercommomUC(val);
                break;
            case NetCMD.SERVER_COMMON_OPT_SUCC_BC:
                if(cmd1 != 0)
                {
                    val = parsePacket("PGame.UserComOptAck", data);
                }
                this.handlecommonUC_succ(val);
                break;
            case NetCMD.SERVER_COMMON_OPT_ERR:
                if(cmd1 != 0)
                {
                    val = parsePacket("PGame.UserComOptErrAck", data);
                }
                this.handlecommonUC_err(val);
                break;
            case NetCMD.SERVER_FIRST_PLAY_BC:
                if(cmd1 != 0)
                {
                    val = parsePacket("PGame.FirstPlayAck", data);
                }
                this.handleFirstPlayer(val);
                break;
            case NetCMD.SERVER_MINGPAI_SUCC_BC:
                if(cmd1 != 0)
                {
                    val = parsePacket("PGame.MingPaiSuccAck", data);
                }
                this.handlerOtherMingpaiSuccessBc(val);
                break;
            case NetCMD.SERVER_MINGPAI_PLAY_SUCC_BC:
                if(cmd1 != 0)
                {
                    val = parsePacket("PGame.MingPaiSuccAck", data);
                }
                this.EndMingPaiSuccessBc(val);
                break;
            case NetCMD.SERVER_SINGLE_CARD_UC:
                if(cmd1 != 0)
                {
                    val = parsePacket("PGame.SingleCardAck", data);
                }
                this.handlerOutCardTis(val);
                break;
            case NetCMD.SERVER_PASS_CARD_BC:
                if(cmd1 != 0)
                {
                    val = parsePacket("PGame.PassCardAck", data);
                }
                this.handlerShowYaobuqiTip(val);
                break;
            case NetCMD.SERVER_BOMB_BC:
                if(cmd1 != 0)
                {
                    val = parsePacket("PGame.BombAck", data);
                }
                this.upbomPlayerMoney(val,cmd1 != 0);
                break;
            case NetCMD.SERVER_SPLIT_CARD_UC:
                if(cmd1 != 0)
                {
                    val = parsePacket("PGame.SplitInfo", data);
                }
                this.splitCard(val);
                break;
            case NetCMD.SERVER_SPLIT_CARD_BC:
                if(cmd1 != 0)
                {
                    val = parsePacket("PGame.SplitCardSuccAck", data);
                }
                this.chanSplitCardPos(val);
                break;
            //总结算
            case NetCMD.SERVER_PHZ_FINAL_END_RES:
                if(cmd1 != 0)
                {
                    val = parsePacket("PGame.SettleInfo", data);
                }
                this.handlerOverBC(val);
                break;

        }
    },




    chanSplitCardPos:function(val)
    {
        log("chanSplitCardPos");
        var seait = utils.getValInt(val,"seatid");
        var position = utils.getValInt(val,"position");

        this.showKaiSpliteCard(seait,position);
    },


    showKaiSpliteCard:function(seait,position)
    {
        if(!DDZModel.isMySeatID(seait))
        {


            if(this.isshowSpiltCards)
            {
                //改变切牌位置
                DDZCards.selectCard(position);
                //需要换的位置
                var all = DDZModel.table_type * 3;
                var per = 90*position/all;
                DDZTips.showSilder(true,per+2);  //需要桌转换
            }


            if(!this.isshowSpiltCards)
            {
                DDZTips.showSplitCardTip(1,true); // 提示  正在等待切牌

                var startPos = DDZModel.getPosBySeatID(0);
                var card = 0;
                var cardPos = DDZModel.table_type * 3;
                DDZCards.SplitFapai(startPos,card,cardPos);

                // DDZTips.showSplitCardTip(true,50);
                DDZTips.showSilderVis(true);
                DDZTips.showSilder(true,50 - 2);

                // DDZTips.hideTime();
                DDZTips.showSingleTime(DDZModel.split_card_after_tstamp);
                DDZTips.hideClockTO(false);


                this.isshowSpiltCards = true;
            }


        }

        // if(DDZModel.splitcardSeait != seait)
        // {
        var pos = DDZModel.getPosBySeatID(seait);
        DDZTips.showsplitReadyTip(pos,true);
        // }
    },

    //等待时间切牌
    splitCard:function(val)
    {
        log("splitCard");

        if(!this.isresetGame)
        {
            this.resetGame();
        }

        var seait = utils.getValInt(val,"seatid");

        this.startsplitCard(seait);
    },

    startsplitCard:function(seait)
    {
        DDZModel.spliceCardstate = true;

        JsUtils.postNotifi("notify_game_check_end");
        DDZModel.sendGetJuShu();

        DDZTips.showSplitCardTip(0,true); // 提示  正在等待切牌

        var pos = DDZModel.getPosBySeatID(seait);
        DDZTips.showTime(pos,DDZModel.split_card_tstamp);



        if(DDZModel.isMySeatID(seait))
        {
            DDZModel.splitcardSeait = seait;
            DDZAction.pdk_showsplitCardBtn(true);
        }
    },

    ///--------------------切牌的－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－

    upbomPlayerMoney:function(val,isProto)
    {
        log("upbomPlayerMoney");
        var winpos = 0;
        var players = isProto?"players":"player";
        if(val[players]&&val[players].length>0)
        {
            for(var i in val[players])
            {
                var d = val[players][i];
                var seatID = utils.getValInt(d,"seatid");
                var cur_money = utils.getValInt(d,"money");
                var win_money = utils.getValInt(d,"cur_money");
                if(win_money > 0 )
                {
                    winpos = DDZModel.getPosBySeatID(seatID);
                }

                var pos = DDZModel.getPosBySeatID(seatID);
                var player = DDZModel.getPlayerBySeatID(seatID);
                player.money = cur_money;
                DDZPlayers.updateInfo(pos);
                DDZTips.showWinCoin(pos,win_money);

                // var pos = DDZModel.getPosBySeatID(seatID);
                // var player = DDZModel.getPlayerBySeatID(seatID);
                // player.money = cur_money;
                // DDZPlayers.updateInfo(pos);
            }

            DDZTips.showbi(winpos);

            this.runAction(cc.sequence(cc.delayTime(0.5),
                cc.callFunc(function(){
                    for(var i in val[players])
                    {
                        var d = val[players][i];
                        var seatID = utils.getValInt(d,"seatid");
                        var cur_money = utils.getValInt(d,"money");
                        var win_money = utils.getValInt(d,"cur_money");
                        var pos = DDZModel.getPosBySeatID(seatID);
                        // var player = DDZModel.getPlayerBySeatID(seatID);
                        // player.money = cur_money;
                        // DDZPlayers.updateInfo(pos);
                        // DDZTips.showWinCoin(pos,win_money);
                    }
                })));
        }
    },

    commonStepHideTips:function()
    {
        for(var i = 0; i<3;i++)
        {
            this.showtimeFlag[i] = false;
        }
    },

    handlerShowYaobuqiTip:function(val)
    {
        log("handlerShowYaobuqiTip");
        var seatid = utils.getValInt(val, "seatid");
        DDZModel.noOutCardSeait = seatid;
        var po = DDZModel.getPosBySeatID(seatid);

        DDZTips.showPassTip("txt_yaobuqi",po,true);
        DDZCards.showOutCards(po,[]);
        DDZTips.showOutCard(po,0);
        if(po!=0)
        {
            DDZTips.showSZTip(po,false,0);
        }
        DDZModel.chupaiNum = 0;

        DDZTips.changeSignTime(po,DDZModel.playTime);
        DDZTips.showSingleClockTo(po);

        DDZTips.showAllTip("");
        DDZTips.hideThinkTips();
        DDZTips.showTimeOut(po,false);
        DDZTips.stopWaitTime(po);
        if(!DDZModel.fastShow) {
            var sf = utils.format(PackName + "/res/sound/pass/%1_buyao%2.mp3", DDZPlayers.getSex(po), 2);
            Sound.getInstance().playEffect(sf);
        }
        if(DDZModel.isMySeatID(seatid))
        {
            DDZAction.pdk_showoutCardBtns(false);

            DDZTips.hideMyOutCardTip();
            DDZCards.initAllDownCard();
            DDZCards.setAllMyCardState(false);
        }
    },

    handlerOutCardTis:function(val)
    {
        log("handlerOutCardTis");
        DDZModel.nextPlayisSingle = utils.getValInt(val, "type");

        DDZTips.hideMyOutCardTip();

        if(DDZModel.isMySeatID(DDZModel.curSeatID))
        {
            if(!(DDZModel.outCardMode ==DDZCardState.PASS_CARD||DDZModel.outCardMode ==DDZCardState.LAST_CARD))
            {
                DDZTips.showMyOutCardTip(DDZModel.nextPlayisSingle,true);
            }
            if(DDZModel.outCardMode == DDZCardState.FOLLOW_CARD)
            {
                // DDZCards.initAllDownCard();
                DDZCards.setAllMyCardState(true);
                DDZCards.autoTipCard();
            }
        }
    },

    handlerOtherMingpaiSuccessBc:function(val)
    {
        log("handlerOtherMingpaiSuccessBc");
        var seait = utils.getValInt(val, "seatid");
        var p = DDZModel.getPosBySeatID(seait);
        if(!DDZModel.isMySeatID(seait))
        {
            if(val["cards"]&&val["cards"].length > 0)
            {
                DDZCards.setMingPaiPos(p);
                DDZCards.showCardValues[p] =[];
                DDZCards.pushShowCard(p,val["cards"]);
            }
        }else{
            DDZCards.setMingPaiPos(p);
        }
    },

    EndMingPaiSuccessBc:function(val)
    {
        log("EndMingPaiSuccessBc");
        var seait = utils.getValInt(val, "seatid");
        var p = DDZModel.getPosBySeatID(seait);
        if(!DDZModel.isMySeatID(seait))
        {
            if(val["cards"]&&val["cards"].length > 0)
            {
                DDZCards.setMingPaiPos(p);
                DDZCards.showCardValues[p] =[];
                DDZCards.pushShowCard(p,val["cards"]);
            }
            DDZCards.showHandCards(p,DDZModel.table_type);
        }
        else
        {
            DDZCards.setMingPaiCardFlag(p,true);
        }
    },



    handleFirstPlayer:function(val){
        cc.log("handleFirstPlayer");

        var _seait = utils.getValInt(val, "seatid");
        var _gtype = utils.getValInt(val, "type");
        DDZModel.firstPlayerSeait =  _seait;
        DDZModel.gamePlayType = _gtype;
        DDZTips.showDiZhu(DDZModel.getPosBySeatID( DDZModel.firstPlayerSeait));
    },

    //通用操作请求
    handlercommomUC:function(val){
        cc.log("handlercommomUC");
        if(!this.isresetGame)
        {
            this.resetGame();
        }


        this.commonStepHideTips();
        var optype = utils.getValInt(val, "opt_type");
        if (val["seatid"] != null) {
            len = val["seatid"].length;
            for (var i = 0; i < len; ++i) {
                var seatID = val["seatid"][i];
                var p = DDZModel.getPosBySeatID(seatID);
                if(DDZModel.isMySeatID(seatID))
                {
                    if(optype == 1){
                        JsUtils.postNotifi("notify_game_check_end");
                        DDZModel.sendGetJuShu();
                        DDZAction.pdk_showmpBtns(true);
                        DDZModel.state = DDZState.MINGPAI_START_STATE;
                        DDZTips.showAllPlayReadytip();
                    }else if(optype == 2){
                        DDZAction.pdk_showqgBtns(true);
                        DDZModel.state = DDZState.QIANGGUAN_STATE;
                    }else if(optype == 3){
                        DDZAction.pdk_showjbBtns(true);
                        DDZModel.state = DDZState.BASE_DOUBLE_STATE;
                    }else if(optype == 4){
                        DDZAction.pdk_showompBtns(true);
                        DDZModel.state = DDZState.MINGPAI_PLAY_STATE;
                    }
                }

            }
        }
//-----------------------------------------------------
        if(optype == 1)
        {
            DDZTips.showSingleTime(DDZModel.comTime);

            if (val["seatid"] != null) {
                len = val["seatid"].length;
                var flag = false;
                for (var i = 0; i < len; ++i) {
                    var seatID = val["seatid"][i];
                    var p = DDZModel.getPosBySeatID(seatID);
                    if(DDZModel.isMySeatID(seatID))
                    {
                        flag = true;
                        DDZTips.showSingleClockTo(p);
                    }
                    DDZTips.startWaitTime(p,DDZModel.comTime);
                }

                if(!flag)
                {
                    var rseait= val["seatid"][0];
                    var p = DDZModel.getPosBySeatID(seatID);
                    DDZTips.showSingleClockTo(p);
                }
            }

        }else if(optype == 2){
            if (val["seatid"] != null) {
                len = val["seatid"].length;
                for (var i = 0; i < len; ++i) {
                    var seatID = val["seatid"][i];
                    var p = DDZModel.getPosBySeatID(seatID);
                    DDZTips.showTime(p,DDZModel.comTime);
                    DDZTips.startWaitTime(p,DDZModel.comTime);
                }
            }

        }else if(optype == 3){

            DDZTips.showSingleTime(DDZModel.comTime);
            if (val["seatid"] != null) {
                len = val["seatid"].length;
                var flag = false;
                for (var i = 0; i < len; ++i) {
                    var seatID = val["seatid"][i];
                    if(DDZModel.isMySeatID(seatID))
                    {
                        flag = true;
                        var p = DDZModel.getPosBySeatID(seatID);
                        DDZTips.showSingleClockTo(p);
                    }
                }

                if(!flag)
                {
                    var rseait= val["seatid"][0];
                    var p = DDZModel.getPosBySeatID(seatID);
                    DDZTips.showSingleClockTo(p);
                }
            }

        }else if(optype == 4){

            if (val["seatid"] != null) {
                len = val["seatid"].length;
                for (var i = 0; i < len; ++i) {
                    var seatID = val["seatid"][i];
                    var p = DDZModel.getPosBySeatID(seatID);
                    DDZTips.showTime(p,DDZModel.comTime);
                    DDZTips.startWaitTime(p,DDZModel.comTime);
                }
            }
        }

    },

    handlecommonUC_succ:function(val)
    {
        cc.log("handlecommonUC_succ");
        var _uid = utils.getValInt(val, "uid");
        var _seait = utils.getValInt(val, "seatid");
        var _ack_opt = utils.getValInt(val, "ack_opt");
        var _ack_type = utils.getValInt(val, "ack_type");

        DDZModel.curRadio = utils.getValInt(val, "ratio");
        if(DDZModel.isMySeatID(_seait) && DDZModel.isMyUID(_uid))
        {
            var player = DDZModel.getPlayerBySeatID(_seait);

            if(_ack_type == 1){
                player.mingpai_start = 2;
                DDZAction.pdk_showmpBtns(false);
            }else if(_ack_type == 2){
                player.qiangguan = 2;
                DDZAction.pdk_showqgBtns(false);
            }else if(_ack_type == 3){
                player.base_double = 2;
                DDZAction.pdk_showjbBtns(false);
            }else if(_ack_type == 4){
                player.mingpai_play = 2;
                DDZAction.pdk_showompBtns(false);
            }
        }
        // DDZTips.setRatio(DDZModel.curRadio);
        DDZTips.setRadioAndCoin();


//-----------------------------------------------------
        if(_ack_type == 1)
        {
            this.showtimeFlag[_seait] =  true;
            var needseait = -1;
            var cp = DDZModel.getPosBySeatID(_seait);
            DDZTips.stopWaitTime(cp);
            for(var i = 0; i<this.showtimeFlag.length;i++)
            {
                if(!this.showtimeFlag[i])
                {
                    needseait = i;
                    break;
                }
            }
            if(needseait != -1)
            {
                var p = DDZModel.getPosBySeatID(needseait);
                DDZTips.showSingleClockTo(p);
            }else{
                DDZTips.hideTime();
                log("need invisible robet");
            }
        }
        else if(_ack_type == 2)
        {
            DDZTips.hideTime();
            var p = DDZModel.getPosBySeatID(_seait);
            DDZTips.stopWaitTime(p);

        }
        else if(_ack_type == 3)
        {
            this.showtimeFlag[_seait] =  true;
            var needseait = -1;
            for(var i = 0 ;i<this.showtimeFlag.length;i++)
            {
                if(!this.showtimeFlag[i])
                {
                    needseait = i;
                    break;
                }
            }
            if(needseait != -1)
            {
                var p = DDZModel.getPosBySeatID(needseait);
                DDZTips.showSingleClockTo(p);
            }else{
                log("need invisible robet");
            }

        }else if(_ack_type == 4)
        {
            DDZTips.hideTime();
            var p = DDZModel.getPosBySeatID(_seait);
            DDZTips.stopWaitTime(p);
        }

    },

    handlecommonUC_err:function(val)
    {
        cc.log("handlecommonUC_err");
        var code = utils.getValInt(val, "error_code");
        if(code == 101){
            DDZTips.showMyTip("当前阶段错误");
        }else if(code == 102){
            DDZTips.showMyTip("请求类型错误");
        }else if(code == 103){
            DDZTips.showMyTip("已请求过此类型");
        }
    },

    //回调：房间信息
    handlerTableInfoUC:function(val) {
        cc.log("handlerTableInfoUC");

        DDZModel.waitUpdateInfo = false;
        DDZModel.reset();

        this.resetGame();
        // this.isresetGame = false;

        DDZPlayers.downtable();

        if(this.roomEndBox)
        {
            this.roomEndBox.removeFromParent(true);
            this.roomEndBox =null;
        }
        DDZModel.maxPlayer =  utils.getValInt(val, "max_players",3);
        DDZModel.roomGolden = utils.getValInt(val, "takein");
        DDZModel.state = utils.getValInt(val, "state");
        DDZModel.seatID = utils.getValInt(val, "seatid");
        DDZModel.curSeatID = utils.getValInt(val, "cur_seat");
        DDZModel.dzSeatID = utils.getValInt(val, "landlord");
        DDZModel.tableType = utils.getValInt(val, "table_type");
        DDZModel.outCardMode = utils.getValInt(val, "action");
        DDZModel.hasPiao = utils.getValInt(val, "has_piao");
        if(utils.isValue(val,"last_seatid"))
        {
            DDZModel.lastSeatID = utils.getValInt(val, "last_seatid");
        }
        else
        {
            DDZModel.lastSeatID =DDZModel.curSeatID;
        }
        DDZModel.spade3_owner =-1;
        if(val["spade3_owner"] != null)
        {
            DDZModel.spade3_owner = utils.getValInt(val, "spade3_owner");
        }

        if(DDZModel.isOnVideo)
        {
            DDZTips.firsttipFlag = true;
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
            DDZModel.piaoTime = 20;
            DDZModel.tilaTime = 20;
            DDZModel.mingpaiTime = 20;
            DDZModel.playTime = 20;
        }
        else{
            DDZModel.pauseTime = utils.getValInt(val, "game_pause_tstamp");
            DDZModel.piaoTime = utils.getValInt(val, "piao_timer_tstamp");
            DDZModel.tilaTime = utils.getValInt(val, "tila_timer_tstamp");
            DDZModel.mingpaiTime = utils.getValInt(val, "mingpai_timer_tstamp");
            DDZModel.playTime = utils.getValInt(val, "play_card_timer_stamp");
        }


        DDZModel.base_money = utils.getValInt(val, "base_money");

        //跑的快的一些参数
        DDZModel.pdk_mpCoints = utils.getValInt(val, "mingpai_start_ratio");
        DDZModel.pdk_ompCoints = utils.getValInt(val, "mingpai_play_ratio");
        DDZModel.pdk_jiabei = utils.getValInt(val, "base_money_double_ratio");
        DDZModel.pdk_qiangguan = utils.getValInt(val, "qiangguan_ratio");
        DDZModel.spring_ratio = utils.getValInt(val, "spring_ratio");
        DDZModel.bomb_ratio = utils.getValInt(val, "bomb_ratio");
        DDZModel.max_ratio = utils.getValInt(val, "max_ratio");
        DDZModel.bomb_can_split = utils.getValInt(val, "bomb_can_split");
        DDZModel.table_type = utils.getValInt(val, "table_type"); //牌形 有多少张牌
        DDZModel.comTime = utils.getValInt(val, "com_opt_tstamp");
        DDZModel.readyTime = utils.getValInt(val, "ready_timer_stamp");
        DDZModel.curRadio = utils.getValInt(val, "ratio");

        DDZModel.spadeoutcart = utils.getValInt(val, "spade3_first_out"); //黑桃三先出

        DDZModel.split_card_tstamp  = utils.getValInt(val, "split_card_tstamp");
        DDZModel.split_card_after_tstamp  = utils.getValInt(val, "split_card_after_tstamp");

        if (val["player_option"]!= null) {
            for (var i = 0; i < val["player_option"].length; ++i)
            {
                DDZModel.player_option.push(val["player_option"][i]);
            }
        }

        DDZModel.sign_rule = utils.getValInt(val, "single_rule");


        DDZModel.addRatios[DDZRatio.TI_LA] = utils.getValInt(val, "tila_ratio");
        DDZModel.addRatios[DDZRatio.MING_PAI] = utils.getValInt(val, "mingpai_ratio");
        DDZModel.addRatios[DDZRatio.BOMB] = utils.getValInt(val, "bomb_ratio");
        DDZModel.addRatios[DDZRatio.ROCKET] = utils.getValInt(val, "rocket_ratio");
        DDZModel.addRatios[DDZRatio.SPRING] = utils.getValInt(val, "spring_ratio");


        if (val["holes"]&&val["holes"].length>0) {
            DDZCards.pushHandCard(val["holes"]);
        }

        // DDZModel.updateRatio(val); //更新倍数
        // DDZModel.updateCoin(val);
        // DDZTips.setRatio(DDZModel.curRadio);

        ZJHModel.getInstance().mySeatid = DDZModel.seatID;

        //提示恢复
        // DDZTips.setCoin(DDZModel.base_money);
        // DDZTips.setRatio(DDZModel.curRadio);
        DDZTips.setRadioAndCoin();
        // DDZTips.setZha(utils.getValInt(val, "max_bomb"));

        // DDZTips.showMust();
        DDZTips.showMustTip();

        DDZModel.firstPlayerSeait =  utils.getValInt(val, "banker");
        //明牌
        if(val["mingpai_player"]&&val["mingpai_player"].length >0)
        {
            for (var i = 0; i < val["mingpai_player"].length; ++i)
            {
                var sID = utils.getValInt(val["mingpai_player"][i],"seatid");
                var pos  =  DDZModel.getPosBySeatID(sID);
                DDZCards.setMingPaiPos(pos);
                DDZCards.showCardValues[pos] =[];
                DDZCards.pushShowCard(pos,val["mingpai_player"][i]["cards"]);
            }
        }

        var spiltflag = true;
        if(val["split_info"]!= null)
        {
            spiltflag = false;
            var sta = utils.getValInt(val["split_info"],"state");
            var seait = utils.getValInt(val["split_info"],"seatid");
            var pos = utils.getValInt(val["split_info"],"positon");
            if(sta == 0)
            {
                DDZModel.spliceCardstate = true;

                if(DDZModel.isMySeatID(seait))
                {
                    DDZModel.splitcardSeait = seait;
                    this.startsplitCard(seait);
                }

            }
            else if(sta == 1)
            {
                DDZModel.spliceCardstate = true;
                if(DDZModel.isMySeatID(seait))
                {
                    DDZModel.splitcardSeait = seait;
                    DDZCards.showsplitcardAni();

                }else{
                    this.showKaiSpliteCard(seait,pos);
                }
            }

        }


        //恢复出牌
        if (val["last_cards"]&&val["last_cards"].length>0) {

            DDZCards.pushOutCard(val["last_cards"]);
            if (DDZCards.outCardValues.length > 0)
            {
                var pos = DDZModel.getPosBySeatID(DDZModel.lastSeatID);
                DDZCards.showOutCards(pos, DDZCards.outCardValues);


                if (pos != 0) {
                    var cards = libddz.cards.create(DDZCards.outCardValues);
                    var parse = libddz.cardsParse.create(cards);

                    var num = 0;
                    if (parse.type == libddz.CardsType.CARD_TYPE_ONELINE) {
                        num = cards.line1.length;
                    }
                    else if (parse.type == libddz.CardsType.CARD_TYPE_TWOLINE) {
                        num = cards.line2.length / 2;
                    }
                    else if (parse.type == libddz.CardsType.CARD_TYPE_THREELINE) {
                        num = cards.line3.length / 3;
                    }
                    if (num > 1) {
                        DDZTips.showSZTip(pos, true, num);
                    }
                }

            }
        }

        //上桌玩家信息
        var len = 0;
        // var tlType = utils.getValInt(val, "tila_type");
        if (val["players"]&&val["players"].length>0) {
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
                }
                //恢复准备状态
                if (DDZModel.state == DDZState.READY && !DDZModel.isOnVideo)
                {
                    DDZPlayers.setReady(pos, player.ready == 1);
                    if ( DDZModel.isMyUID(player.uid)) {
                        DDZAction.showReadyBtn(player.ready != 1);
                    }
                }
                if(DDZModel.isOnVideo)
                {
                    if (d["holes"]) {
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
                else if (DDZModel.state != DDZState.END_GAME && spiltflag) {
                    var num = utils.getValInt(d, "card_nums");
                    if(num > 0)
                    {
                        DDZTips.showAllPlayReadytip();
                        this.outCardTip = true;
                        DDZCards.showHandCards(pos, num);
                        DDZTips.showHandCardJB(pos,num==1);

                        var sid  = player.seatid-1;
                        if(sid <0)
                        {
                            sid =2;
                        }
                        if(len==3&&num==1&&DDZModel.isMySeatID(sid))
                        {
                            DDZModel.nextPlayisSingle =1;
                        }
                    }

                }
                else {
                    if(spiltflag)
                    {
                        DDZCards.showHandCards(pos, 0);
                    }
                }
                /*
                 if (DDZModel.state != DDZState.PIAO)                {
                 if(utils.getValInt(d, "is_piao") == 1)
                 {
                 DDZTips.showPiao(pos,utils.getValInt(d, "piao"));
                 }
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
                 */
                //飘恢复
                /*
                 if (DDZModel.state == DDZState.PIAO) {
                 var is = utils.getValInt(d, "is_piao") != 1;
                 if (DDZModel.isMySeatID(player.seatid)) {
                 DDZAction.showPiaoBtns(is);
                 }
                 if(is)
                 {
                 DDZTips.startWaitTime(pos,DDZModel.piaoTime);
                 }
                 }
                 */
                //踢拉恢复
                /*
                 else if (DDZModel.state == DDZState.TI_LA) {

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
                 */

                /*
                 else if (DDZModel.state == DDZState.MING_PAI) {

                 var type = utils.getValInt(d, "mingpai");
                 if (DDZModel.isMySeatID(player.seatid)&&DDZModel.isDZSeatID(player.seatid)) {

                 DDZAction.showMingPaiBtns(type == 0);
                 }
                 }
                 */
            }
        }



//------------------------------------------------------------------------------------
        var  pos = DDZModel.getPosBySeatID(DDZModel.curSeatID);
        /*
         var dzPos=DDZModel.getPosBySeatID(DDZModel.dzSeatID);
         //飘恢复
         if (DDZModel.state == DDZState.PIAO) {
         var startPos = DDZModel.getPosBySeatID(0);
         var card = utils.getValInt(val, "random_card");
         var cardPos = 53 - utils.getValInt(val, "position");
         DDZCards.showFaPaiCard(startPos, card, cardPos);
         DDZTips.showTime(DDZTips.getNoPiaoPos(),DDZModel.piaoTime);
         }
         else if (DDZModel.state == DDZState.TI_LA) {

         if(tlType == 1&&DDZTips.getNoTiPos()!=-1)
         {
         DDZTips.showTime(DDZTips.getNoTiPos(),DDZModel.tilaTime);
         }
         else
         {
         DDZTips.showTime(dzPos,DDZModel.tilaTime);
         }
         }
         */
        if (DDZModel.state == DDZState.MINGPAI_START_STATE ) {

            for (var i = 0; i < 3; ++i)
            {
                var player = DDZModel.getPlayerBySeatID(i);
                if(player.mingpai_start != 0)
                {
                    this.showtimeFlag[player.seatid] = true;
                }
            }
            var c_seait = -1;
            for(var k = 0; k<3; k++)
            {

                if(!this.showtimeFlag[k])
                {
                    var player = DDZModel.getPlayerBySeatID(k);
                    if(DDZModel.isMySeatID(player.seatid))
                    {

                        c_seait = player.seatid;
                    }

                }
            }
            if(c_seait == -1)
            {
                for(var j = 0; j<3; j++)
                {
                    if(!this.showtimeFlag[j])
                    {
                        c_seait = j;
                    }
                }
            }

            if(c_seait != -1)
            {
                var p = DDZModel.getPosBySeatID(c_seait);
                DDZTips.showTime(p,DDZModel.comTime);
                DDZTips.startWaitTime(p,DDZModel.comTime);

                if(DDZModel.isMySeatID(c_seait))
                {
                    DDZAction.pdk_showmpBtns(true);
                }

            }else{
                DDZTips.showSingleTime(DDZModel.comTime);
                DDZTips.hideClockTO();
            }

        }else if(DDZModel.state == DDZState.BASE_DOUBLE_STATE)
        {
            DDZTips.showSingleTime(DDZModel.comTime);
            for (var i = 0; i < 3; ++i)
            {
                var player = DDZModel.getPlayerBySeatID(i);
                if(player.base_double != 0)
                {
                    this.showtimeFlag[i] = true;
                }
            }
            var c_seait = -1;

            for(var k = 0; k<3; k++)
            {

                if(!this.showtimeFlag[k])
                {
                    var player = DDZModel.getPlayerBySeatID(k);
                    if(DDZModel.isMySeatID(player.seatid))
                    {

                        c_seait = player.seatid;
                    }

                }
            }
            if(c_seait  == -1)
            {
                for(var j = 0; j<3; j++)
                {
                    if(!this.showtimeFlag[j])
                    {
                        c_seait = j;
                    }
                }
            }

            if(c_seait != -1)
            {
                var p = DDZModel.getPosBySeatID(c_seait);
                DDZTips.showTime(p,DDZModel.comTime);
                DDZTips.startWaitTime(p,DDZModel.comTime);

                if(DDZModel.isMySeatID(c_seait))
                {
                    DDZAction.pdk_showjbBtns(true);
                }
            }else{

                DDZTips.showSingleTime(DDZModel.comTime);
                DDZTips.hideClockTO();
            }
        }
        else if(DDZModel.state == DDZState.QIANGGUAN_STATE || DDZModel.state == DDZState.MINGPAI_PLAY_STATE){
            DDZTips.showTime(pos,DDZModel.comTime);
            DDZTips.startWaitTime(pos,DDZModel.comTime);
        }
        else if (DDZModel.state == DDZState.PLAYING) {
            DDZTips.showDiZhu(DDZModel.getPosBySeatID( DDZModel.firstPlayerSeait));
            DDZTips.showTime(pos,DDZModel.playTime);
            DDZTips.startWaitTime(pos,DDZModel.playTime);
            DDZCards.showTuoGuan(DDZModel.isTuoguan == 1);
            this.outCardTip = true;


        }
        //我方是否已上桌   //上桌按钮的显示问题
        DDZAction.showUptableBtn(!DDZModel.isMyInTable());

        //轮到己方操作
        if (DDZModel.isMySeatID(DDZModel.curSeatID)) {
            //出牌按钮的恢复
            /*
             if (DDZModel.state == DDZState.PREPLAY_ONE || DDZModel.state == DDZState.PREPLAY_TWO) {
             if (DDZModel.tableType == 0) {
             var must = utils.getValInt(val, "must_call");

             if (must == 1) {
             DDZTips.showMyTip1("单王双2必当地主");
             }
             else if (must == 2) {
             DDZTips.showMyTip1("双王必当地主");
             }
             else if (must == 3) {
             DDZTips.showMyTip1("4个2必当地主");
             }
             DDZAction.showJiaoDZBtns(true);
             DDZAction.showJiaoFenAble(utils.getValInt(val,"action"), must != 0);
             }
             else if (DDZModel.tableType == 1) {
             if (DDZModel.state == DDZState.PREPLAY_ONE) {
             DDZAction.showJiaoDZBtns(true);
             }
             else {
             DDZAction.showQiangDZBtns(true);
             }
             }
             }
             */

        }


        if (DDZModel.state == DDZState.READY && !DDZModel.isOnVideo)
        {
            if(DDZModel.isMyInTable()&&DDZPlayers.playersNum()<DDZModel.maxPlayer)
            {
                DDZAction.showNoReadyBtn(!DDZAction.getUI("readyBtn").isVisible());
            }
        }
        if(DDZModel.state == DDZState.MINGPAI_START_STATE)
        {
            // DDZAction.pdk_showmpBtns(true);
        }
        else if (DDZModel.state == DDZState.QIANGGUAN_STATE)
        {
            if (DDZModel.isMySeatID(DDZModel.curSeatID)){
                DDZAction.pdk_showqgBtns(true);
            }
        }
        else if (DDZModel.state == DDZState.BASE_DOUBLE_STATE)
        {
            // DDZAction.pdk_showjbBtns(true);
        }
        else if (DDZModel.state == DDZState.MINGPAI_PLAY_STATE)
        {
            if (DDZModel.isMySeatID(DDZModel.curSeatID)){
                DDZAction.pdk_showompBtns(true);
            }
        }
        else if (DDZModel.state == DDZState.PLAYING)
        {

            if( DDZModel.spade3_owner!=-1 && DDZModel.isMySeatID( DDZModel.spade3_owner)
                && DDZModel.outCardMode == DDZCardState.NEW_CARD && DDZCards.handCardValues.length == DDZModel.table_type)
            {
                DDZTips.firsttipFlag = false;
                DDZTips.showfisrtCardThree(true);
            }
            else
            {
                DDZTips.firsttipFlag = true;
            }

            DDZCards.isMyFristAutoLine =  DDZModel.outCardMode == DDZCardState.NEW_CARD &&DDZModel.isMySeatID( DDZModel.curSeatID);
            DDZTips.showAllPlayReadytip();
            if (DDZModel.isMySeatID(DDZModel.curSeatID)){
                DDZCards.setAllMyCardState(true);
                DDZCards.autoTipCard();
                DDZAction.pdk_showoutCardBtns(true);
            }else{
                DDZCards.setAllMyCardState(false);
            }

            if(DDZModel.nextPlayisSingle >= 1){
                DDZTips.showMyOutCardTip(DDZModel.nextPlayisSingle,true);
            }else{
                DDZTips.showfisrtCardThree(true);
            }
            if((DDZModel.outCardMode ==DDZCardState.PASS_CARD||DDZModel.outCardMode ==DDZCardState.LAST_CARD) && DDZModel.isMySeatID( DDZModel.curSeatID))
            {
                DDZModel.sendOutCards(false);
            }
        }

        if(len<DDZModel.maxPlayer)
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
        DDZModel.sendGetJuShu();

        //恢复投票
        if (val["dissolve_room_info"]!= null && val["dissolve_room_info"]["state"] == 1)
        {
            this.showEndRoom(val["dissolve_room_info"]);
        }
    },
    //广播：登录成功,掉线重连
    handlerLoginSuccBC:function(val)
    {
        cc.log("handlerLoginSuccBC");

        DDZModel.updatePlayer(val.player);
        //更新掉线提示

        var seatID =utils.getValInt(val.player,"seatid");
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
        var player = DDZModel.updatePlayer(val.player);
        if(DDZModel.isMyUID(player.uid))
        {
            DDZModel.sendRoomInfo();
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
            DDZCards.removeMyhandAndourCard(pos);
            DDZTips.showwin(pos,false);

            DDZTips.showQunaguan(pos,false);
            DDZTips.showLastCardNum(pos,false,0);
        }

        if(DDZModel.isMySeatID(seatID))
        {

            DDZModel.unDownSeatID =DDZModel.seatID;
            DDZModel.seatID = -1;
            ZJHModel.getInstance().mySeatid = DDZModel.seatID;
            DDZAction.showUptableBtn(true);
            DDZAction.showReadyBtn(false);
            DDZAction.showNoReadyBtn(false);
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
        var ready = utils.getValInt(val,"ready");
        var pos = DDZModel.getPosBySeatID(seatid);
        var player = DDZModel.getPlayerBySeatID(seatid);
        if(player)
        {
            player.ready =ready;
            DDZPlayers.setReady(pos,ready==1);
        }

        if(DDZModel.isMyUID(uid))
        {
            DDZAction.showReadyBtn(ready!=1);
            DDZAction.showNoReadyBtn(false);
            if(DDZPlayers.playersNum()<DDZModel.maxPlayer)
            {
                DDZAction.showNoReadyBtn(ready==1);
            }
        }
        // DDZTips.mympLine[0].setVisible(true);
        // DDZTips.mympLine[0].getAnimation().playWithIndex(0,-1,0);

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
            DDZAction.showNoReadyBtn(false);
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
        DDZTips.hideTime();

        DDZTips.showSilder(false,0);
        DDZTips.hideAllspltReadyTip();

        DDZTips.showAllPlayReadytip();

        DDZAction.showReadyBtn(false);
        DDZAction.showNoReadyBtn(false);
        if(!this.isresetGame)
        {
            this.resetGame();
        }

        DDZCards.hideAllsplitCardTip(); //隐藏切牌的一些


        DDZAction.showUptableBtn(!DDZModel.isMyInTable());

        // DDZModel.state = DDZState.PREPLAY_ONE;
        DDZCards.showTuoGuan(false);
        DDZCards.fapai( DDZModel.table_type);
        // DDZModel.updateCoin(val);
        // DDZTips.setCoin(DDZModel.coin);
        // DDZTips.setRatio(DDZModel.getRatio());
        this.stopGameOver();
        JsUtils.postNotifi("notify_game_check_end");
        DDZModel.sendGetJuShu();

        DDZModel.sendGetPlayerList();

    },

    //广播：开始飘
    handlerStartPiaoBC:function(val)
    {
        cc.log("handlerStartPiaoBC");
        // DDZModel.state = DDZState.PIAO;

        if(DDZModel.isMyInTable())
        {
            DDZAction.showPiaoBtns(true);
        }
        DDZTips.showTime(0,DDZModel.piaoTime);
        DDZTips.startWaitTime(0,DDZModel.piaoTime);
        DDZTips.startWaitTime(1,DDZModel.piaoTime);
        DDZTips.startWaitTime(2,DDZModel.piaoTime);
    },

    //回调：飘失败
    handlerPiaoErroUC:function(val)
    {
        cc.log("handlerPiaoErroUC");

        var p = DDZTips.getNoPiaoPos();
        if(p!=-1)
        {
            DDZTips.showTime(p,DDZModel.piaoTime);
        }
        DDZTips.stopWaitTime(0);
        DDZAction.showPiaoBtns(false);
        DDZAction.showReadyBtn(false);
        var code = utils.getValInt(val,"code");
        if(code ==102)
        {
            DDZTips.showMyTip("剩余金币不足");
        }
        else
        {
            DDZTips.showMyTip("飘失败");
        }
    },

    //广播：飘结果
    handlerPiaoSuccBC:function(val)
    {
        cc.log("handlerPiaoSuccBC");

        var r = utils.getValInt(val,"piao_ratio");
        var seatID = utils.getValInt(val,"seatid");
        var pos = DDZModel.getPosBySeatID(seatID);

        if(DDZModel.isMySeatID(seatID))
        {
            DDZAction.showPiaoBtns(false);
        }
        DDZTips.showPiao(pos,r);

        DDZTips.showTimeOut(pos,false);
        DDZTips.stopWaitTime(pos);

        var p = DDZTips.getNoPiaoPos();
        if(p!=-1)
        {
            DDZTips.showTime(p,DDZModel.piaoTime);
        }
    },

    //回调：看牌
    handlerGameStartUC:function(val)
    {
        cc.log("handlerGameStartUC");

        DDZModel.state = DDZState.QIANGGUAN_STATE; //翻牌之后就是抢关
        DDZAction.showPiaoBtns(false);

        if(val["cards"]&&val["cards"].length>0)
        {
            DDZCards.handCardValues =[];
            DDZCards.pushHandCard(val["cards"]);
        }

        var self = this;
        if(DDZModel.isFaPai)
        {
            var ac = cc.repeatForever(cc.sequence(cc.delayTime(0.1),cc.callFunc(function(){

                if(!DDZModel.isFaPai)
                {
                    var dt = 0;
                    if(DDZModel.gamePlayType != 1&&DDZModel.maxPlayer ==3)
                    {
                        dt = 1.2;
                    }
                    self.runAction(cc.sequence(
                        cc.callFunc(function(){
                            if(DDZModel.gamePlayType != 1&&DDZModel.maxPlayer ==3)
                            {
                                var pos = DDZModel.getPosBySeatID(DDZModel.firstPlayerSeait);
                                DDZTips.foPlayerOutCard(DDZModel.table_type,pos);
                            }

                        }),
                        cc.delayTime(dt),
                        cc.callFunc(function(){
                            DDZCards.showHandCards(0,DDZModel.table_type);
                            DDZCards.showHandCards(1,DDZModel.table_type);
                            DDZCards.showHandCards(2,DDZModel.table_type);
                            for(var i = DDZCards.handCards[0].length-1;i>=0;--i)
                            {
                                var c  =DDZCards.handCards[0][i];
                                var v = c.value;
                                var p = c.getPosition();
                                c.setValue(0);
                                (function(v,c,i)
                                {
                                    c.runAction(cc.sequence(cc.delayTime(0.02*i),cc.callFunc(function(){
                                        c.setValue(v);
                                    })));
                                })(v,c,i);
                            }
                        }),cc.callFunc(function(){
                            DDZTips.showMPAnimation(0,true);
                            DDZTips.showMPAnimation(1,true);
                            DDZTips.showMPAnimation(2,true);
                        })
                        // ,cc.delayTime(2),
                        // cc.callFunc(function(){
                        //     DDZTips.showMPAnimation(0,false);
                        //     DDZTips.showMPAnimation(1,false);
                        //     DDZTips.showMPAnimation(2,false);
                        // })
                    ));
                    self.stopGameOver();
                    self.stopActionByTag(666666);
                }
            })));
            ac.setTag(666666);
            this.runAction(ac);
        }
        else
        {
            var dt = 0;
            if(DDZModel.gamePlayType != 1&&DDZModel.maxPlayer ==3)
            {
                dt = 1.2;
            }
            self.runAction(cc.sequence(
                cc.callFunc(function(){
                    if(DDZModel.gamePlayType != 1&&DDZModel.maxPlayer ==3)
                    {
                        var pos = DDZModel.getPosBySeatID(DDZModel.firstPlayerSeait);
                        DDZTips.foPlayerOutCard(DDZModel.table_type,pos);
                    }
                }),
                cc.delayTime(dt),
                cc.callFunc(function(){
                    DDZCards.showHandCards(0,DDZModel.table_type);
                    DDZCards.showHandCards(1,DDZModel.table_type);
                    DDZCards.showHandCards(2,DDZModel.table_type);

                    for(var i = DDZCards.handCards[0].length-1;i>=0;--i)
                    {
                        var c  =DDZCards.handCards[0][i];
                        var v = c.value;
                        var p = c.getPosition();
                        c.setValue(0);
                        (function(v,c,i)
                        {
                            c.runAction(cc.sequence(cc.delayTime(0.02*i),cc.callFunc(function(){
                                c.setValue(v);
                            })));
                        })(v,c,i);
                    }
                }),cc.callFunc(function(){
                    DDZTips.showMPAnimation(0,true);
                    DDZTips.showMPAnimation(1,true);
                    DDZTips.showMPAnimation(2,true);
                })
                // ,cc.delayTime(2),
                // cc.callFunc(function(){
                //     DDZTips.showMPAnimation(0,false);
                //     DDZTips.showMPAnimation(1,false);
                //     DDZTips.showMPAnimation(2,false);
                // })
            ));

            this.stopGameOver();
        }
    },

    //广播：叫地主
    handlerStartJiaoDiZhuBC:function(val)
    {
        cc.log("handlerStartJiaoDiZhuBC");
        // DDZModel.state = DDZState.PREPLAY_ONE;

        DDZAction.showPiaoBtns(false);
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
            DDZCards.showHandCards(0,DDZModel.table_type);
            DDZCards.showHandCards(1,DDZModel.table_type);
            DDZCards.showHandCards(2,DDZModel.table_type);
        }
        if(DDZModel.isMySeatID(seatID)) {

            DDZAction.showJiaoDZBtns(true);

            if(DDZModel.tableType ==0)
            {
                var must = utils.getValInt(val,"must_call");

                if (must == 1) {
                    DDZTips.showMyTip1("单王双2必当地主");
                }
                else if (must == 2) {
                    DDZTips.showMyTip1("双王必当地主");
                }
                else if (must == 3) {
                    DDZTips.showMyTip1("4个2必当地主");
                }
                DDZAction.showJiaoFenAble(action,must!=0);
            }
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
        if(DDZModel.tableType ==1)//叫抢
        {
            if(action>=1 && action<=3)
            {
                Sound.getInstance().playEffect(utils.format(PackName+"/res/sound/jiaofen/%1_jiaodizhu.mp3"
                    ,DDZPlayers.getSex(pos)));
            }
            else
            {
                Sound.getInstance().playEffect(utils.format(PackName+"/res/sound/jiaofen/%1_bujiao.mp3"
                    ,DDZPlayers.getSex(pos)));
            }
            DDZTips.showJiaoQiang(pos,action);
        }
        else
        {
            if(action>=1 && action<=3)
            {
                Sound.getInstance().playEffect(utils.format(PackName+"/res/sound/jiaofen/%1_%2_point.mp3"
                    ,DDZPlayers.getSex(pos),action));
            }
            else
            {
                Sound.getInstance().playEffect(utils.format(PackName+"/res/sound/jiaofen/%1_bujiao.mp3"
                    ,DDZPlayers.getSex(pos)));
            }
            DDZTips.showJiaoFen(pos,action);
        }

        DDZTips.showMyTip("");
    },
    //广播：抢地主
    handlerStartQiangDiZhuBC:function(val)
    {
        cc.log("handlerStartQiangDiZhuBC");
        // DDZModel.state = DDZState.PREPLAY_TWO;
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

        if(action>=1 && action<=4)
        {
            Sound.getInstance().playEffect(utils.format(PackName+"/res/sound/jiaofen/%1_qiangdizhu.mp3"
                ,DDZPlayers.getSex(pos)));
        }
        else
        {
            Sound.getInstance().playEffect(utils.format(PackName+"/res/sound/jiaofen/%1_buqiang.mp3"
                ,DDZPlayers.getSex(pos)));
        }

    },
    //广播：当地主
    handlerDiZhuBC:function(val)
    {
        cc.log("handlerDiZhuBC");
        // DDZModel.state = DDZState.PLAYING;
        DDZModel.dzSeatID = utils.getValInt(val,"landlord");
        DDZModel.updateCoin(val);
        DDZModel.updateRatio(val);
        DDZTips.setCoin(DDZModel.coin);
        DDZTips.setRatio(DDZModel.getRatio());

        var pos = DDZModel.getPosBySeatID( DDZModel.dzSeatID);
        DDZCards.setDiZhuPos(pos);
        if(val["comms"]&&val["comms"].length>0)
        {
            for(var i in val["comms"])
            {
                DDZCards.showPublicCard(i,val["comms"][i]);
            }
            if(DDZModel.isMyDZ())
            {
                DDZCards.pushHandCard(val["comms"]);
            }
        }
        for(var i=0;i<3;++i)
        {
            DDZCards.showHandCards(i,pos ==i ?20:17);
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

        // DDZModel.state = DDZState.TI_LA;
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
        // DDZModel.state = DDZState.MING_PAI;
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
        DDZCards.pushShowCard(val["holes"]);
        DDZCards.showHandCards(pos,DDZCards.showCardValues.length);
        DDZTips.showJiaBeiEf(DDZJiaBei.MING_PAI,DDZModel.addRatios[DDZRatio.MING_PAI]);

        Sound.getInstance().playEffect(utils.format(PackName+"/res/sound/jiaofen/%1_mingpai.mp3"
            ,DDZPlayers.getSex(pos)));
        DDZTips.showMp(pos);
    },
    //广播：进入出牌
    handlerStartOutCardBC:function(val)
    {
        cc.log("handlerStartOutCardBC");
        DDZModel.noOutCardSeait = -1;  //重置不可以出牌的位置


        DDZTips.hideFirstOutCardTip();  //首先出牌的提示

        if(!this.outCardTip)
        {
            this.outCardTip = true;

            var pos = DDZModel.getPosBySeatID(DDZModel.firstPlayerSeait);
            DDZTips.showFirstOutCardTip(DDZModel.gamePlayType,pos);
        }

        var seatID = utils.getValInt(val,"seatid");
        var pos = DDZModel.getPosBySeatID(seatID);
        DDZModel.curSeatID = seatID;
        DDZModel.outCardMode =utils.getValInt(val,"action");

        DDZModel.state = DDZState.PLAYING;
        DDZCards.isMyFristAutoLine =false;

        DDZAction.showMingPaiBtns(false);
        DDZTips.hideJiaoQiangTip();
        DDZTips.hideThinkTips();
        DDZTips.stopWaitTime(0);
        DDZTips.stopWaitTime(1);
        DDZTips.stopWaitTime(2);
        DDZTips.showTime(pos,DDZModel.playTime);
        DDZTips.startWaitTime(pos,DDZModel.playTime);


        if((DDZModel.outCardMode ==DDZCardState.PASS_CARD||DDZModel.outCardMode ==DDZCardState.LAST_CARD) && DDZModel.isMySeatID( seatID))
        {
            DDZModel.sendOutCards(false);
            return;
        }
        if(DDZModel.isTuoguan !=1 )
        {
            if(DDZModel.isMySeatID(seatID) && DDZModel.outCardMode == DDZCardState.FOLLOW_CARD)
            {
                // DDZCards.initAllDownCard();
                DDZCards.setAllMyCardState(true);
                DDZCards.autoTipCard()
            }
        }

        // DDZModel.state = DDZState.PLAYING;



        // DDZTips.hidePassTips();//要不起隐藏

        //DDZTips.hideTime();


        // if(DDZTips.needBuLa())
        // {
        //     DDZTips.showTiLa(DDZModel.getPosBySeatID(DDZModel.dzSeatID),2,1);
        // }




        //重新初始出牌数组
        DDZCards.showOutCards(pos,[]);
        DDZTips.showOutCard(pos,0);
        if(pos!=0)
        {
            DDZTips.showSZTip(pos,false,0);
        }

        if(DDZModel.isMySeatID(seatID))
        {
            //跟牌
            if(DDZModel.outCardMode == DDZCardState.FOLLOW_CARD)
            {
                DDZTips.showMyOutCardTip(DDZModel.nextPlayisSingle,true);

            }

            //自己出牌
            if(DDZModel.outCardMode == DDZCardState.NEW_CARD)
            {
                DDZCards.isMyFristAutoLine =true;
                if(DDZModel.nextPlayisSingle >= 1){
                    DDZTips.showMyOutCardTip(DDZModel.nextPlayisSingle,true);
                }else{
                    DDZTips.showfisrtCardThree(true);
                }


            }

            if(DDZModel.isTuoguan !=1 &&  DDZModel.noOutCardSeait != seatID)
            {
                DDZAction.pdk_showoutCardBtns(true);
            }
        }else{
            DDZTips.hideMyOutCardTip();
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
    //广播：出牌结果
    handlerOutCardSuccBC:function(val)
    {
        cc.log("handlerOutCardSuccBC");

        DDZTips.firsttipFlag = true;

        if(DDZModel.state != DDZState.PLAYING)
        {
            return;
        }

        DDZTips.hideMyOutCardTip();

        DDZAction.pdk_showoutCardBtns(false);

        var cardType = utils.getValInt(val,"card_type");
        var cardNum = utils.getValInt(val,"card_nums");
        var seatID = utils.getValInt(val,"seatid");
        DDZModel.curRadio = utils.getValInt(val, "ratio");



        DDZModel.chupaiNum = 0;

        if(DDZModel.isMySeatID(seatID))
        {
            DDZCards.initAllDownCard();
            DDZCards.setAllMyCardState(false);
        }

        var pos = DDZModel.getPosBySeatID(seatID);

        DDZTips.showSingleClockTo(pos);

        //设置倍数
        /*
         DDZModel.updateCoin(val);
         DDZModel.updateRatio(val);
         DDZTips.setCoin(DDZModel.coin);
         DDZTips.setRatio(DDZModel.getRatio());
         */
        DDZTips.setRatio(DDZModel.curRadio);

        //清除自身提示
        DDZTips.showAllTip("");
        DDZTips.hideThinkTips();
        DDZTips.showTimeOut(pos,false);

        //DDZTips.hideTime();
        DDZTips.stopWaitTime(pos);

        DDZModel.outCardType = cardType;//牌行
        DDZCards.resetTipCard();//重置出牌

        var isPass = true;

        if(val["cards"]&&val["cards"].length>0)
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
                    if(DDZModel.isOnVideo|| this.checkmpList(DDZCards.mpPos,pos)) {
                        var v = val["cards"][i];
                        DDZCards.deleteCard(DDZCards.showCardValues[pos],v);
                    }
                }
            }
            DDZCards.pushOutCard(val["cards"]);
            DDZCards.showOutCards(pos, DDZCards.outCardValues);
            if (pos != 0) {
                var cards = libddz.cards.create(DDZCards.outCardValues);
                var parse = libddz.cardsParse.create(cards);

                var num = 0;
                if (parse.type == libddz.CardsType.CARD_TYPE_ONELINE) {
                    num = cards.line1.length;
                }
                else if (parse.type == libddz.CardsType.CARD_TYPE_TWOLINE) {
                    num = cards.line2.length / 2;
                }
                else if (parse.type == libddz.CardsType.CARD_TYPE_THREELINE) {
                    num = cards.line3.length / 3;
                }
                if (num > 1) {
                    DDZTips.showSZTip(pos, true, num);
                }
            }
            DDZCards.showHandCards(pos,cardNum);
            DDZTips.showHandCardJB(pos,cardNum==1);

            if(!DDZModel.fastShow)
            {
                if (cardNum == 1)
                {
                    Sound.getInstance().playEffect(utils.format(PackName+"/res/sound/baojing/%1_baojing%2.mp3"
                        ,DDZPlayers.getSex(pos),cardNum));
                }

                if (cardType == libddz.CardsType.CARD_TYPE_ONE)
                {
                    var sf = utils.format(PackName+"/res/sound/danpai/%1_%2_card.mp3"
                        , (DDZCards.outCardValues[0].value % 16).toString(16),DDZPlayers.getSex(pos));
                    Sound.getInstance().playEffect(sf);
                }
                else if (cardType == libddz.CardsType.CARD_TYPE_TWO)
                {
                    var sf = utils.format(PackName+"/res/sound/duizi/%1_card_dui_0%2.mp3",DDZPlayers.getSex(pos), (DDZCards.outCardValues[0].value  % 16).toString(16));
                    Sound.getInstance().playEffect(sf);
                }
                else
                {
                    Sound.getInstance().playEffect(utils.format(PackName+"/res/sound/other/%1_type_%2.mp3"
                        ,DDZPlayers.getSex(pos), cardType));
                }
            }

        }
        else
        {
            DDZTips.showOutCard(pos,true);

            if(!DDZModel.fastShow)
            {
                var sf = utils.format(PackName+"/res/sound/pass/%1_buyao%2.mp3"
                    ,DDZPlayers.getSex(pos), utils.random(0,2));
                Sound.getInstance().playEffect(sf);
            }
        }


        if(DDZModel.isMySeatID(seatID))
        {
            DDZCards.initUpCard();
        }

        if(cardNum > 0)
        {
            DDZTips.showCardEf(pos,cardType);
        }
        if(cardType == libddz.CardsType.CARD_TYPE_BOMB)
        {
            DDZTips.showJiaBeiEf(DDZJiaBei.BOMB,DDZModel.bomb_ratio);
        }
        // else if(cardType == libddz.CardsType.CARD_TYPE_ROCKET)
        // {
        //     DDZTips.showJiaBeiEf(DDZJiaBei.ROCKET,DDZModel.addRatios[DDZRatio.ROCKET]);//火箭的倍数
        // }


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

        this.isresetGame = false;
        this.isshowSpiltCards = false;

        // DDZModel.updateRatio(val);
        // DDZTips.setRatio(DDZModel.getRatio());

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


        DDZModel.curRadio = utils.getValInt(val, "ratio");
        // DDZTips.setRatio(DDZModel.curRadio);
        DDZTips.setRadioAndCoin();

        DDZCards.handCardValues=[];
        //全关是否
        var fail = utils.getValInt(val,"fail");
        var isSpring = false;

        var winID = utils.getValInt(val,"seatid");

        for(var i in val["players"])
        {
            var d = val["players"][i];
            if( utils.getValInt(d,"cur_money") >0)
            {
                winID = utils.getValInt(d,"seatid");
                break;
            }
        }
        var winPos = DDZModel.getPosBySeatID(winID);
        for(var i in val["players"])
        {
            var d = val["players"][i];
            var seatID = utils.getValInt(d,"seatid");
            var pos = DDZModel.getPosBySeatID(seatID);

            if(d["holes"]&&d["holes"].length>0)
            {
                var cards =DDZCards.createCards(d["holes"]);
                if(DDZModel.isMySeatID(seatID))
                {
                    // DDZCards.showOutCards(pos,[]);
                    // DDZCards.pushOutCard(cards);

                    DDZCards.showOutCards(pos,cards);
                    DDZCards.showHandCards(pos,0);
                }
                else
                {
                    DDZCards.showHandCards1(pos,cards);
                }

                if(cards.length > 0)
                {
                    DDZTips.showLastCardNum(pos,true,cards.length);
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
            else
            {
                if(pos != winPos)
                {
                    DDZTips.showCoinTo(pos,winPos,-winCoin);
                }
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

            if(utils.getValInt(d,"spring") == 1)
            {
                isSpring = true;
                DDZTips.showQunaguan(pos,true);
            }
        }

        if(fail == 1)
        {
            isSpring = true;
            var qgid = utils.getValInt(val,"seatid");
            for(var i = 0;i<3;i++)
            {
                if(qgid == i)
                {
                    var pos = DDZModel.getPosBySeatID(i);
                    DDZTips.showQunaguan(pos,true);


                }else{
                    var pos = DDZModel.getPosBySeatID(i);
                    DDZTips.showwin(pos,true);
                    // DDZTips.showLastCardNum(pos,true,DDZCards.handCards[pos].length)
                }
            }
        }else{
            var sid = utils.getValInt(val,"seatid");
            var pos = DDZModel.getPosBySeatID(sid);
            DDZTips.showwin(pos,true);
            // for(var i = 0;i<3;i++)
            // {
            //    var cpos = DDZModel.getPosBySeatID(i);
            //    DDZTips.showLastCardNum(cpos,true,DDZCards.handCards[cpos].length)
            // }

        }

        if(isSpring)
        {
            Sound.getInstance().playEffect(PackName+"/res/other/f_type_sprite.mp3");
        }
        DDZCards.outCardValues =[];

        if(DDZModel.isOnVideo)
        {
            return;
        }
        this.runAction(cc.sequence(cc.delayTime(4),cc.callFunc(function(){
            if(DDZModel.isMyInTable()&&DDZModel.getPlayerByPos(0).ready !=1)
            {
                DDZAction.showReadyBtn(true);
                DDZModel.sendReady(1);
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
        DDZModel.sendOver();
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
    },
    //广播：总结算提示
    handlerOverBC:function(val) {
        cc.log("handlerOverBC");

        if( this.getChildByName("over"))
        {
            return;
        }
        var box = new DDZOver();
        box.show(val);
        this.addChild(box,100000);

    }
});
