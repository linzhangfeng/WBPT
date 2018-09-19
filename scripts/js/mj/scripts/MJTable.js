var MJTable = cc.Layer.extend({
    tag: "RoomMJTable",
    players: [],
    room_Card: null,
    room_Tip: null,
    room_Info: null,
    room_Action: null,
    room_EndTouPiao: null,
    mChangeBar: null,
    mPaiSeBar: null,
    mPiaoBar: null,
    roomPublic: null,
    ctor: function () {
        this._super();
        if (ZJHModel.getInstance().isOnVideo) {
            this.initOnVideo();
        }
        else {
            this.init();
        }
    },
    initOnVideo: function () {

        MJModel.init();
        MJModel.reset();
        MJModel.curGameType = ZJHModel.getInstance().curGameType;
        MJModel.isOnVideo = true;

        this.initRoom();
    },
    init: function () {
        var self = this;
        //监听联网状态
        if (true) {
            this.schedule(this.updateGame, 0.0);
            this.schedule(this.gameHeartBeat, 3.0);
        }
        else {
            this.schedule(this.test, 1.0);
        }
        var size = cc.director.getWinSize();

        MJModel.init();
        MJModel.reset();
        MJModel.roomData = JSON.parse(ZJHModel.getInstance().getRoomData());
        MJModel.curRoomID = ZJHModel.getInstance().curRoomID;
        MJModel.curGameType = ZJHModel.getInstance().curGameType;
        MJModel.take_in = MJModel.roomData["gameRoom"]["takeIn"];

        var top = RoomPublic.create();
        top.setCallback(function (tag) {

            if (tag == "exit") {
                self.actionCallBack("downtable");
                sendLogout();
            }
            else if (tag == "end_room") {
                sendEndRoomREQ(1);
            }
            else if (tag == "down_table") {
                self.actionCallBack("downtable");
            }
        });

        this.roomPublic = top;

        this.addChild(top, 100000);

        this.initRoom();
    },

    onEnter: function () {
        this._super();
        //部分事件监听
        var self = this;
        this._listenerGameOver = cc.EventListener.create({
            event: cc.EventListener.CUSTOM,
            eventName: "notify_against_room_end",
            callback: function (evt) {
                Log(self, "notify_against_room_end");
                self.GameTimeEndNotify(null);
            }
        });
        cc.eventManager.addListener(this._listenerGameOver, this);

        this._listenerExit = cc.EventListener.create({
            event: cc.EventListener.CUSTOM,
            eventName: "will_exit_room",
            callback: function (evt) {
//                cc.spriteFrameCache.removeSpriteFrames();
//                cc.textureCache.removeAllTextures();
                Log(self, "will_exit_room");
            }
        });
        cc.eventManager.addListener(this._listenerExit, this);


    },
    onExit: function () {
        this._super();
    },
    //初始化界面
    initRoom: function () {
        var size = cc.director.getWinSize();

        var bg = new cc.Sprite(getResPath("RoomMJ2/bg.png"));
        bg.setAnchorPoint(cc.p(0.5, 0.5));
        bg.setPosition(size.width / 2, size.height / 2);
        bg.setScale(size.width / bg.getContentSize().width, size.height / bg.getContentSize().height);
        this.addChild(bg, -100);

        var mjType = new cc.Sprite(getResPath("RoomMJ/mj_type_") + MJModel.curGameType + ".png");
        if (mjType != null) {
            mjType.setPosition(cc.p(size.width / 2, 530));
            this.addChild(mjType, 0);
        }

        var self = this;
        this.room_Info = new MJInfo();
        this.room_Info.setVisible(false);
        this.addChild(this.room_Info, 0);

        //桌上玩家初始化
        for (var i = 0; i < 4; i++) {
            var player = new MJPlayer();
            player.setId(i);
            player.setPosition(MJConfig.getPlayerPos(i));
            this.addChild(player, 0);
            this.players[i] = player;
            player.setClickCallback(function (pos) {
                self.playerClickCallback(pos);
            });
        }

        var card = new cc.Layer();
        card.setTag(13);
        this.addChild(card, 20);

        this.room_Card = new MJCards();
        this.room_Card.setPosition(cc.p(size.width / 2, size.height / 2));
        card.addChild(this.room_Card);

        this.room_Tip = new MJTips();
        this.room_Tip.setPosition(cc.p(size.width / 2, size.height / 2));
        this.addChild(this.room_Tip, 100);

        this.room_Action = new MJAction();
        this.addChild(this.room_Action, 150);
        if (this.room_Action) this.room_Action.setCallback(function (v) {
            self.actionCallBack(v);
        });

        if (this.isScmjRoom()) {

            //如果该房间为四川麻将房则添加下面的组件

            this.mChangeBar = new MJChange();
            this.mChangeBar.setVisible(false);
            this.mChangeBar.addClickEventListener(function (node) {
                self.onChangeClick(node);
            });
            this.addChild(this.mChangeBar);

            this.mPaiSeBar = new MJPaiSe();
            this.mPaiSeBar.setVisible(false);
            this.mPaiSeBar.addClickEventListener(function (node) {
                self.onPaiSeClick(node);
            });
            this.addChild(this.mPaiSeBar);

            this.mPiaoBar = new MJPiao();
            this.mPiaoBar.setVisible(false);
            this.mPiaoBar.addClickEventListener(function (node) {
                self.onPiaoClick(node);
            });
            this.addChild(this.mPiaoBar);

        }

        Sound.getInstance().preLoadEffect(getResPath("RoomMJ/sound"));
        Sound.getInstance().PlayBgSound(getResPath("RoomMJ/sound/tablemusic.mp3"));
    },
    test: function (d) {
        MJModel.testCount++;
        var tempCount = MJModel.testCount;
        if (tempCount == 1) {
            MJModel.canTouchCard = true;
            MJModel.seatid = 0;
            var cardlist = [];
            for (var k = 1; k < 10; k++) {
                var v = Utils.Random(0, 2) * 16 + Utils.Random(1, 9);
                cardlist.push(v);
                Log(this, "----initCard" + v);
            }

            Log(this, "----111111---" + cardlist.length);
            this.room_Card.initHandCard(0, cardlist);
            this.room_Card.reflashHandCard(0, true);

            this.room_Card.initHandCard(1, cardlist);
            this.room_Card.reflashHandCard(1, true);

            this.room_Card.initHandCard(2, cardlist);
            this.room_Card.reflashHandCard(2, true);

            this.room_Card.initHandCard(3, cardlist);
            this.room_Card.reflashHandCard(3, true);

            this.room_Card.initMoCard(0, 1, false);
            this.room_Card.initMoCard(1, 0, false);
            this.room_Card.initMoCard(2, 0, false);
            this.room_Card.initMoCard(3, 0, false);

            for (var j = 0; j < 4; j++) {
                for (var i = 0; i < 27; i++) {
                    var v = Utils.Random(0, 2) * 16 + Utils.Random(1, 9);
                    Log(this, "----outCard" + v);
                    this.room_Card.addOutCard(j, v);
                }
            }
        }
        if (tempCount == 2) {
            // cc.log("test---------testtest");
            // MJModel.valueGuiPai = 1;
            // this.room_Tip.showGuiPai(1);
            // this.room_Tip.showLeftGuiPaiBg();
            // var result = new MJResult();
            // result.updateSC("sss");
            // this.addChild(result, 100000);
            // var buff = ";" + 32;
            // var huazhuTex = new cc.LabelAtlas(buff, getResPath("RoomMJ/result-small/result_small_mutiple.png"), 20, 25, '0');
            // huazhuTex.setAnchorPoint(cc.p(0.5, 0.5));
            // huazhuTex.setPosition(100,500);
            // this.addChild(huazhuTex);
        }
    },
    updateGame: function (dt) {
        var curTime = time(null);
        if (MJModel.lastLoginTime != -1 && curTime - MJModel.lastLoginTime > 3 * 1000) {
            sendLogin();
            MJModel.lastLoginTime = curTime;
        }
        if (MJModel.lastHeartBeatTime != -1 && curTime - MJModel.lastHeartBeatTime > 10 * 1000) {
            Log(this, "心跳异常，重连了");
            CCTcpClient.getInstance(MJModel.curRoomID).reset_response_queue();
            CCTcpClient.getInstance(MJModel.curRoomID).connect();

            MJModel.lastHeartBeatTime = curTime;

            if (MJModel.firstChongLian == false) this.room_Tip.showTextTip("网络连接异常，正在重试");

            MJModel.firstChongLian = false;
            MJModel.isEnterWait = false;
            return;
        }

        if (MJModel.isEnterWait) {
            Log(this, "MJModel.isEnterWait");
            return;
        }

        var response = CCTcpClient.getInstance(MJModel.curRoomID).get();

        if (response) {
            // Log(this, "cmd=" + response.cmd + " id=" + response.id + " data=" + response.data);
            if (response.cmd == TCPCMD.DISCONNECT_RES || response.cmd == TCPCMD.CONNECT_ERROR_RES || response.cmd == TCPCMD.SEND_DATA_ERROR_RES || response.cmd == TCPCMD.RECV_DATA_ERROR_RES) //关闭
            {
                if (!MJModel.isLogOut) {
                    MJModel.roomConnectFail++;
                    this.netError(1);
                    if (MJModel.roomConnectFail == 60 * 1) {
                        this.room_Tip.showTextTip("网络连接异常，正在重试");
                    }
                }
            }
            else if (response.cmd == TCPCMD.CONNECT_OK_RES) {
                sendLogin();
                MJModel.lastLoginTime = curTime;
            }
            else if (response.cmd == TCPCMD.RECV_DATA_OK_RES) {
                MJModel.roomConnectFail = 0;
                Log(this, response.id);
                this.handler_cmd(response.id, response.data, true);
            }
            response.release();
        } else {
        }
    },

    resetPlayer: function () {
        for (var i = 0; i < 4; i++) {
            var rp = this.players[i];
            var player = MJModel.players[i];
            if (player) {
                player.isTimeOut = 0;
                player.isTuoGuan = 0;
            }
            rp.reset();
        }
    },
    stopAllTimer: function () {
    },
    handler_cmd: function (cmd, jpacket, canDelay) {
        if (MJModel.waitUpdate && cmd != CMD.SERVER_GAME_SCENE
            && cmd != CMD.SERVER_TABLE_INFO_UC) {
            return 0;
        }

        switch (cmd) {
            case CMD.SERVER_LOGIN_SUCC_UC:
                this.handler_server_login_succ_uc(jpacket, canDelay);
                break;

            case CMD.SERVER_LOGIN_SUCC_BC:
                this.handler_server_login_succ_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_LOGIN_ERR_UC:
                this.handler_server_login_err_uc(jpacket, canDelay);
                break;

            case CMD.SERVER_REBIND_UC:
                this.handler_server_rebind_uc(jpacket, canDelay);
                break;

            case CMD.SERVER_LOGOUT_SUCC_BC:
                this.handler_server_logout_succ_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_LOGOUT_ERR_UC:
                this.handler_server_logout_err_uc(jpacket, canDelay);
                break;

            case CMD.SERVER_TABLE_INFO_UC:
                this.handler_server_table_info_uc(jpacket, canDelay);
                break;

            case CMD.SERVER_GAME_SCENE:
                this.handler_server_scene_info_uc(jpacket, canDelay);
                break;

            case CMD.SERVER_USER_READY:
                this.handler_server_ready_succ_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_READY_ERR_UC:
                this.handler_server_ready_err_uc(jpacket, canDelay);
                break;

            case CMD.SERVER_GAME_START:
                this.handler_server_game_start_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_START_PIAO:
                this.handler_server_start_piao(jpacket, canDelay);
                break;
            case CMD.SERVER_BROAD_PIAO:
                this.handler_server_broad_piao(jpacket, canDelay);
                break;
            case CMD.SERVER_BROAD_LISTENTYPE:
                this.handler_server_broad_listentype(jpacket, canDelay);
                break;

            case CMD.SERVER_FIRST_HANDCARD:
                this.handler_server_game_start_bc2(jpacket, canDelay);
                break;

            case CMD.SERVER_CHANGE_CARD:
                this.handler_server_change_card(jpacket, canDelay);
                break;

            case CMD.SERVER_USER_LACK_TYPE:
                this.handler_server_sure_lack_type(jpacket, canDelay);
                break;

            case CMD.SERVER_VIDEO_FIRST_HANDCARD:
                this.handler_server_game_start_bc_onvideo(jpacket, canDelay);
                break;

            case CMD.SERVER_USER_GETCARD:
                this.handler_server_mo_card_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_USER_OUTCARD:
                this.handler_server_out_card_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_OPERATE_NOTIFY:
                this.handler_server_operate_notify_bc(jpacket, canDelay);
                break;

            /* 广播玩家操作 */
            case CMD.SERVER_OPERATE_RESULT:
                this.handler_server_peng_kang_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_LISTEN_NOTIFY:
                this.handler_server_tingpai_notify(jpacket, canDelay);
                break;

            case CMD.SERVER_GAME_END:
                this.handler_server_game_end_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_GAME_PREREADY_BC:
                this.handler_server_game_preready_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_EMOTION_BC:
                this.handler_server_bigface_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_UPDATE_USERINFO:
                this.handler_server_update_info_uc(jpacket, canDelay);
                break;

            case CMD.SERVER_UPTABLE_SUCCC_BC:
                this.handler_server_up_table_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_DOWNTABLE_SUCC_BC:
                this.handler_server_down_table_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_DZ_GAME_START_RES:
                this.handler_game_time_over_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_UPDATE_TIMER:
                this.handler_game_time_out_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_HEART_BEAT_RESP:
                this.handler_game_heartbeat_uc(jpacket, canDelay);
                break;

            case CMD.SERVER_BUY_HORSE:
                this.handler_maima_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_APPLY_DISSOLVE_ROOM_ERR_UC:
                this.handler_server_end_room_req_fail_uc(jpacket, canDelay);
                break;

            case CMD.SERVER_APPLY_DISSOLVE_ROOM_SUCC_BC:
                this.handler_server_end_room_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_DISSOLVE_ACTION_SUCC_BC:
                this.handler_server_end_room_action_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_DISSOLVE_ROOM_RESULT_BC:
                this.handler_server_end_room_result_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_USER_OFFLINE_BC:
                this.handler_server_player_offline_bc(jpacket, canDelay);
                break;

            case CMD.SERVER_OVERTIME_USER:
                this.handler_server_player_timeout_bc(jpacket, canDelay);
                break;
            /* 鸡牌展示 */
            case CMD.SERVER_GET_JIPAI:
                this.handler_server_get_ji_pai(jpacket, canDelay);
                break;

            case CMD.SERVER_BAO_TING_SUCC:
                this.handler_server_baoting_bc(jpacket, canDelay);
                break;
            case CMD.SERVER_UPTABLE_INFO_UC:
                this.handler_server_uptable_info_uc(jpacket, canDelay);
                break;

            case CMD.SERVER_BEGIN_QUAN_FENG:
                this.handler_server_begin_zhuafeng(jpacket, canDelay);
                break;
            case CMD.SERVER_GET_GUIPAI:
                this.handler_server_get_guipai(jpacket, canDelay);
                break;

            case CMD.SERVER_GET_CHAOZHUANG:
                this.handler_server_get_chaozhuang(jpacket, canDelay);
                break;

            case CMD.SERVER_FLOWER_HAND_CARD:
                this.handler_server_gdmj_game_start3(jpacket, canDelay);
                break;

            case CMD.SERVER_GET_HUA_CARD:
                this.handler_server_gdmj_get_huapai(jpacket, canDelay);
                break;

            case CMD.SERVER_SEND_MESSAGE:
                this.handler_server_get_msg(jpacket, canDelay);
                break;

            case CMD.SERVER_HALL_LOGOUT:
                this.handler_server_die(jpacket, canDelay);
                break;

            case CMD.SERVER_LOGOUT_TO_HALL:
                this.handler_server_belogout_succ_bc(jpacket, canDelay);
                break;

            default:
                Log(this, "no handler of cmd %d", cmd);
        }

        return 0;
    },
    handler_server_die: function (jpacket, canDelay) {
        sendLogin();
        MJModel.lastLoginTime = time(null);
    },
    handler_server_login_succ_uc: function (data, canDelay) {
        Log(this, "handler_server_login_succ_uc");
        MJModel.lastLoginTime = -1;
    },
    handler_server_login_succ_bc: function (data, canDelay) {
        Log(this, "handler_server_login_succ_bc");
    },
    handler_server_login_err_uc: function (data, canDelay) {
    },
    handler_server_rebind_uc: function (data, canDelay) {
    },
    handler_server_logout_succ_bc: function (data, canDelay) {
        Log(this, "handler_server_logout_succ_bc");
        var jpacket = parsePacket("proto.login.AckLogoutBc", data);
        var uid = jpacket.uid;
        var type = jpacket.type;
        if (uid == ZJHModel.getInstance().uid) {
            ZJHModel.getInstance().roomToHall();
        }
    },
    handler_server_belogout_succ_bc: function (data, canDelay) {
        Log(this, "handler_server_belogout_succ_bc");
        ZJHModel.getInstance().roomToHall();
    },
    handler_server_logout_err_uc: function (data, canDelay) {
    },

    handler_server_table_info_uc: function (data, canDelay) {
        Log(this, "handler_server_table_info_uc");
        MJModel.reset();
        this.removeChildByName("RoomMJResultDetail");
        this.stopAllActions();
        this.room_Tip.showTextTip("");

        this.resetGame();

        for (var i = 0; i < 4; i++) {
            var mjPlayer = this.players[i];
            mjPlayer.logout();
        }

        var tableInfo = parsePacket("proto.login.AckTableInfo", data);
        Log(this, tableInfo);
        MJModel.seatid = -1;
        ZJHModel.getInstance().mySeatid = -1;

        MJModel.state = tableInfo.state;

        var len = tableInfo.players.length;


        for (var i = 0; i < len; i++) {
            var seatid = tableInfo.players[i].seatid;
            var uid = tableInfo.players[i].uid;
            if (uid == ZJHModel.getInstance().uid) {
                MJModel.seatid = seatid;
                ZJHModel.getInstance().mySeatid = seatid;
                break;
            }
        }

        for (var i = 0; i < len; i++) {
            var seatid = tableInfo.players[i].seatid;
            var player = MJModel.players[seatid];

            player.seatid = seatid;
            player.uid = tableInfo.players[i].uid;
            player.name = tableInfo.players[i].name;
            player.money = tableInfo.players[i].money;
            player.ready = tableInfo.players[i].ready;
            player.sex = tableInfo.players[i].sex;
            player.avatar = tableInfo.players[i].avatar;
        }

        this.room_Info.showDirectionTip(-1);
    },
    handler_server_scene_info_uc: function (data, canDelay) {
        Log(this, "handler_server_scene_info_uc");
        var ackGameFree = parsePacket("proto.game.AckGameScene", data);

        var sceneStatus = ackGameFree.SceneStatus;

        if(sceneStatus == 10){
            //房间已经结束
            MJModel.state = MJ_STATUS.MJ_GAME_FREE;
            this.startTimeEnd();
            return;
        }

        //出牌时间
        MJModel.outCardTime = ackGameFree.outcardtime;

        //底分
        MJModel.base_money = ackGameFree.basescore;

        //底分
        MJModel.stand_money = ackGameFree.stand_money;

        //操作时间
        MJModel.oprateTime = ackGameFree.operatetime;

        if (ackGameFree.scmjCustomied) {
            //是否需要可以换牌
            MJModel.isNeedChangeCard = ackGameFree.scmjCustomied.isChangeCard;
            cc.log("change:" + MJModel.isNeedChangeCard);
            //是否需要检测花猪大叫
            MJModel.isNeedSearchHuazhuDajiao = ackGameFree.scmjCustomied.isCheckhuazhudajiao;

            MJModel.scmjType = ackGameFree.scmjCustomied.gametype;

            MJModel.piaoTime = ackGameFree.scmjCustomied.piaoTime;

            MJModel.mj_roomType = ackGameFree.scmjCustomied.RoomType;

            MJModel.scmj_listenType = ackGameFree.scmjCustomied.ListenType;

            MJModel.lackTime = ackGameFree.lacktypetime;

            MJModel.changeTime = ackGameFree.changetime;
        }

        this.room_Info.setBaseMoney(MJModel.base_money);

        MJModel.waitUpdate = false;
        MJModel.isBaoting = false;
        this.initPlayer();

        if (this.room_Action) this.room_Action.setUpTableVisible(MJModel.seatid == -1);

        //转转麻将
        if (ackGameFree.IsZhuanZhuanRoom) {
            MJModel.isZhuanZhuan = ackGameFree.IsZhuanZhuanRoom;
            if (this.room_Action) {
                this.room_Action.setDuiLieVisible(true);
                this.room_Action.setUpTableVisible(false);
                if (MJModel.seatid != -1) {
                    this.room_Action.setDuiLieStr("游戏中");
                }
            }

            if (ackGameFree.UserReadyInfo) {
                MJModel.zhuanReadyTime = ackGameFree.UserReadyInfo.ReadyTime;
                for (var i = 0; i < ackGameFree.UserReadyInfo.Ready.length; i++) {
                    var player = MJModel.players[i];
                    player.ready = ackGameFree.UserReadyInfo.Ready[i];
                    player.zhuan_surplustime = ackGameFree.UserReadyInfo.RemainReadyTime[i];
                }
                if (MJModel.state != MJ_STATUS.MJ_GAME_PLAYING) {
                    this.startZhuanClock();
                }
            }
        }

        //is end room state
        if (ackGameFree.isDisbandroom == 1 && MJModel.seatid != -1) {
            var json = {};

            var owner_seatid = ackGameFree.Disbandinfo.applychairid;
            var time = ackGameFree.Disbandinfo.waittime;
            var player = MJModel.players[owner_seatid];
            if (player != undefined) {
                json["ownerid"] = player.uid;
                json["ownername"] = player.name;
                json["time_left"] = time;
                json["players"] = [];
                for (var i = 0; i < ackGameFree.Disbandinfo.onlinechairid.length; i++) {
                    var seatid = ackGameFree.Disbandinfo.onlinechairid[i];
                    var p = MJModel.players[seatid];
                    var d = {};
                    d["uid"] = p.uid;
                    d["name"] = p.name;
                    d["avatar"] = p.avatar;
                    d["state"] = 0;


                    for (var j = 0; j < ackGameFree.UserDisbandstate.length; j++) {
                        var sid = ackGameFree.UserDisbandstate[j].chairid;
                        var ac = ackGameFree.UserDisbandstate[j].choosestate;
                        if (sid == seatid) {
                            d["state"] = ac;
                            break;
                        }
                    }

                    json["players"].push(d);
                }

                this.room_EndTouPiao = RoomHEndTouPiao.create();
                this.room_EndTouPiao.initWithJsonStr(JSON.stringify(json));
                this.addChild(this.room_EndTouPiao, 10000000);

                MJModel.TouPiaoING = true;

                this.room_EndTouPiao.setCallback(function (str) {
                    if (str == "ok") {
                        sendEndRoomResult(1);
                    }
                    else if (str == "no") {
                        sendEndRoomResult(2);
                    }
                });
            }

        }

        if (ackGameFree.UserOnlinestate) {
            for (var i = 0; i < ackGameFree.UserOnlinestate.length; i++) {
                var seatid = ackGameFree.UserOnlinestate[i].chairid;
                var state = ackGameFree.UserOnlinestate[i].state;
                var pos = MJModel.getPosBySeatid(seatid);
                this.players[pos].setOffline(state == 1);
            }
        }

        //广东麻将玩法配置信息
        if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GDMJ_DZ && ackGameFree.gdscene) {
            MJModel.gdmj_playid = ackGameFree.gdscene.PlayID;
        }

        //ready
        if (this.room_Action) this.room_Action.setReadyVisible(false);
        Log(this, "curState:" + sceneStatus);
        if (sceneStatus == MJ_STATUS.MJ_GAME_FREE) {
            var nums = 0;
            var selfIsReady = true;
            //当前玩家是否准备
            for (var i = 0; i < 4; i++) {
                var pm = MJModel.players[i];
                if (pm.uid != -1) {
                    if (pm.ready) {
                        var pos = MJModel.getPosBySeatid(pm.seatid);
                        var mp = this.players[pos];
                        mp.setReadyFlag(true);
                    }
                    else {
                        if (pm.uid == ZJHModel.getInstance().uid) {
                            if (this.room_Action) this.room_Action.setReadyVisible(true);
                            selfIsReady = false;
                        }
                    }

                    nums++;
                }
            }

            if (MJModel.seatid == -1 && nums < 4) {
                this.actionCallBack("uptable");
                this.actionCallBack("ready");
            }
            else if (selfIsReady == false) {
                this.actionCallBack("ready");
            }
            return;
        }

        if (ackGameFree.UserOvertime) {
            for (var i = 0; i < ackGameFree.UserOvertime.length; i++) {
                var seatid = ackGameFree.UserOvertime[i].chairid;
                var state = ackGameFree.UserOvertime[i].state;
                if (state == 1) {
                    var pos = MJModel.getPosBySeatid(seatid);
                    this.players[pos].setTimeOut(true);
                }
                else if (state == 2) {
                    this.room_Tip.showTextTip("等待玩家操作");
                }
            }
        }

        MJModel.isFirstLogin = false;

        if (MJModel.isZhuanZhuan) {
            this.stopZhuanClock();
        }

        this.room_Info.setVisible(true);

        JsUtils.postNotifi("notify_game_check_end");//为了刷新局数 或者  开始倒计时

        var curMoPaiPos = -1;

        var cardList = [];
//当前玩家手牌
        if (ackGameFree.UserCard && ackGameFree.UserCard.ChangeableCards) {
            var changeable_Size = ackGameFree.UserCard.ChangeableCards.length;

            for (var i = 0; i < changeable_Size; i++) {
                cardList.push(ackGameFree.UserCard.ChangeableCards[i]);
            }
            this.room_Card.initHandCard(0, cardList);
            this.room_Card.reflashHandCard(0, true);
        }

//广东麻将 重新获取当前鬼牌,更新valueGuiPai
        if (ackGameFree.gdscene && ackGameFree.gdscene.CurJoker) {
            MJModel.valueGuiPai = ackGameFree.gdscene.CurJoker;
//断线重连，更新左侧鬼牌显示
            if (MJModel.valueGuiPai) {
                this.room_Tip.showLeftGuiPaiBg();
                this.room_Tip.isShowGuiPai(1);
                this.room_Card.setGuipai();
            }
        }
//四川麻将 重新获取当前鬼牌
        if (ackGameFree.scmjCustomied && ackGameFree.scmjCustomied.CurJoker) {
            MJModel.valueGuiPai = ackGameFree.scmjCustomied.CurJoker;
//断线重连，更新左侧鬼牌显示
            if (MJModel.valueGuiPai) {
                this.room_Tip.showLeftGuiPaiBg();
                this.room_Tip.isShowGuiPai(1);
                this.room_Card.setGuipai();
            }
        }

        var huCard = 0;

        if (this.isScmjRoom() && ackGameFree.HuInfo) {
//四川麻将还要初始化胡牌区
            for (var i = 0; i < ackGameFree.HuInfo.length; i++) {
                var pos = MJModel.getPosBySeatid(i);
                var huInfo = ackGameFree.HuInfo[i];
                if (huInfo.hu.length > 0) {
                    this.room_Tip.showHuTip(pos);
                    for (var j = 0; j < huInfo.hu.length; j++) {
                        var huTag = huInfo.hu[j];
                        this.room_Card.addHuCard(pos, huTag.HuCard);
                    }

                    if (i == MJModel.seatid) {

                        if (MJModel.scmjType == 1) {
                            huCard = huInfo.hu[0].HuCard;
                        }
                    }
                }
            }
        }

        if (cardList.length % 3 == 2) {
            if (MJModel.scmjType == 1) {
                if (!this.room_Card.isAlreadyHu(0)) {
                    curMoPaiPos = 0;
                } else {
//自摸胡牌
                    this.room_Card.removeHandCard(0, huCard);
                    this.room_Card.initMoCard(0, huCard, true);
                }
            } else {
                curMoPaiPos = 0;
            }
        }

        if (ackGameFree.UserCard && ackGameFree.UserCard.stFixedCards) {
            var fixedCardSize = ackGameFree.UserCard.stFixedCards.length;
//我碰杠胡的牌
            for (var i = 0; i < fixedCardSize; i++) {
                var cardData = ackGameFree.UserCard.stFixedCards[i].CardData;
                var opChairid = ackGameFree.UserCard.stFixedCards[i].chairID;
                var state = ackGameFree.UserCard.stFixedCards[i].state;
                if ((state & ActionType.TYPE_PENG) == ActionType.TYPE_PENG) {
                    this.room_Card.addPengCard(0, PengType.Peng_Peng, cardData);
                }
                else if ((state & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG) {
                    this.room_Card.addPengCard(0, PengType.Peng_AnKang, cardData);
                }
                else if ((state & ActionType.TYPE_LEFT_CHI) == ActionType.TYPE_LEFT_CHI) {
                    this.room_Card.addPengCard(0, PengType.Chi_L, cardData);
                }
                else if ((state & ActionType.TYPE_CENTER_CHI) == ActionType.TYPE_CENTER_CHI) {
                    this.room_Card.addPengCard(0, PengType.Chi_M, cardData);
                }
                else if ((state & ActionType.TYPE_RIGHT_CHI) == ActionType.TYPE_RIGHT_CHI) {
                    this.room_Card.addPengCard(0, PengType.Chi_R, cardData);
                }
                else {
                    this.room_Card.addPengCard(0, PengType.Peng_Kang, cardData);
                }
            }
        }

//其他玩家手牌
        var handcardnums_size = ackGameFree.HandCardNums.length;
        for (var i = 0; i < handcardnums_size; i++) {
            if (!MJModel.isMyPlayer(i)) {
                cardList = [];
                var otherCars = ackGameFree.HandCardNums[i];
                for (var j = 0; j < otherCars; j++) {
                    cardList.push(0);
                }
                var pos = MJModel.getPosBySeatid(i);
                this.room_Card.initHandCard(pos, cardList);
                this.room_Card.reflashHandCard(pos, true);

                if (cardList.length % 3 == 2) {
                    if (MJModel.scmjType == 1) {
                        if (!this.room_Card.isAlreadyHu(pos)) {
                            curMoPaiPos = pos;
                        }
                    } else {
                        curMoPaiPos = pos;
                    }

                }
            }
        }

//各个玩家碰杠牌
        var groupcard_size_id = ackGameFree.GroupCard.length;
        for (var i = 0; i < groupcard_size_id; i++) {

            if (!MJModel.isMyPlayer(i)) {
                var groupcard_size = ackGameFree.GroupCard[i].FixedCard.length;
                var pos = MJModel.getPosBySeatid(i);

                for (var j = 0; j < groupcard_size; j++) {
                    var cardData = ackGameFree.GroupCard[i].FixedCard[j].CardData;
                    var state = ackGameFree.GroupCard[i].FixedCard[j].state;
                    var chairId = ackGameFree.GroupCard[i].FixedCard[j].chairID;

                    if ((state & ActionType.TYPE_PENG) == ActionType.TYPE_PENG) {
                        this.room_Card.addPengCard(pos, PengType.Peng_Peng, cardData);
                    }
                    else if ((state & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG) {
                        this.room_Card.addPengCard(pos, PengType.Peng_AnKang, cardData);
                    }
                    else if ((state & ActionType.TYPE_LEFT_CHI) == ActionType.TYPE_LEFT_CHI) {
                        this.room_Card.addPengCard(pos, PengType.Chi_L, cardData);
                    }
                    else if ((state & ActionType.TYPE_CENTER_CHI) == ActionType.TYPE_CENTER_CHI) {
                        this.room_Card.addPengCard(pos, PengType.Chi_M, cardData);
                    }
                    else if ((state & ActionType.TYPE_RIGHT_CHI) == ActionType.TYPE_RIGHT_CHI) {
                        this.room_Card.addPengCard(pos, PengType.Chi_R, cardData);
                    }
                    else {
                        this.room_Card.addPengCard(pos, PengType.Peng_Kang, cardData);
                    }
                }
            }
        }

//各个方位的出牌
        {
            var showCardSize = ackGameFree.ShowCard.length;
            for (var i = 0; i < showCardSize; i++) {
                var outCardLen = ackGameFree.ShowCard[i].OutCard.length;
                cardList = [];
                var pos = MJModel.getPosBySeatid(i);
                for (var j = 0; j < outCardLen; j++) {
                    cardList.push(ackGameFree.ShowCard[i].OutCard[j]);
                }
                this.room_Card.initOutCard(pos, cardList);
            }
        }

//点击出来有听操作提示
        if (ackGameFree.ListenInfo.length > 0) {
            MJModel.listen_info = [];

            var listeninfo_size = ackGameFree.ListenInfo.length;

            for (var i = 0; i < listeninfo_size; i++) {
                var tli = Utils.clone(tagListenInfo);

                tli.OutCard = ackGameFree.ListenInfo[i].OutCard;
                var huinfoLen = ackGameFree.ListenInfo[i].HuInfo.length;
                tli.HuInfoLen = huinfoLen;
                for (var j = 0; j < huinfoLen; j++) {
                    var huInfo = Utils.clone(tagHuInfo);
                    huInfo.Card = ackGameFree.ListenInfo[i].HuInfo[j].Card;
                    huInfo.LeftNum = ackGameFree.ListenInfo[i].HuInfo[j].LeftNum;
                    huInfo.HuFanNum = ackGameFree.ListenInfo[i].HuInfo[j].HuFanNum;
                    tli.HuInfo.push(huInfo);
                }
                MJModel.listen_info.push(tli);
            }
        }

//可以扛的牌
        if (ackGameFree.GangInfo.length > 0) {
            MJModel.gang_info = [];
            var gang_size = ackGameFree.GangInfo.length;
            for (var i = 0; i < gang_size; i++) {
                var tgInfo = Utils.clone(tagGangInfo);
                tgInfo.curCard = ackGameFree.GangInfo[i].Card;
                tgInfo.state = ackGameFree.GangInfo[i].state;
                MJModel.gang_info.push(tgInfo);
            }
        }

//广东麻将恢复各个花牌 风位 当前风圈等信息
        if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GDMJ_DZ) {
//显示当前圈风
            MJModel.gdmj_fengquan = ackGameFree.gdscene.QuanFeng - 49;
            this.room_Info.gdmj_showFengQuan(MJModel.gdmj_fengquan);

//显示各个玩家的风位
            for (var i = 0; i < 4; i++) {
                this.players[i].setFengWei(MJModel.getSeatidByPos(i));
            }

//恢复各个玩家的花牌
            for (var i = 0; i < ackGameFree.gdscene.HuaPai.length; i++) {
                var seatid = i;
                var pos = MJModel.getPosBySeatid(seatid);
                for (var j = 0; j < ackGameFree.gdscene.HuaPai[i].HuaCount; j++) {
                    this.room_Card.addHuaCard(pos, ackGameFree.gdscene.HuaPai[i].HuaCard[j]);
                }
//            if (ackGameFree.gdscene.HuaPai[i].HuaCount > 0)
//            {
//                this.room_Tip.showHuaTip(pos);
//            }
//                 this.players[pos].setHuaPaiNum(ackGameFree.gdscene.HuaPai[i].HuaCount);
            }
        }

//恢复操作时间
        {
            var LeftTime = ackGameFree.LeftTime;
            this.room_Info.showTime(LeftTime);
            // if (LeftTime > MJModel.oprateTime && LeftTime > MJModel.outCardTime) {
            //     this.room_Tip.showTextTip("等待玩家重连");
            // }
        }

//恢复庄家标志
        {
            var banker_seatid = ackGameFree.BankerChairid;
            var banker_pos = MJModel.getPosBySeatid(banker_seatid);
            var player = this.players[banker_pos];
            MJModel.banker_seatid = banker_seatid;
//恢复买马标志
            var maima_nums = ackGameFree.HorseNum;

            if (banker_pos >= 0 && banker_pos <= 3) {
                if (player) {
                    player.setBankerFlag(true);
                    player.setMaiMa(maima_nums);
                }
            }
        }

//恢复剩余牌数
        {
            var leftCardNum = ackGameFree.PoolCardLeftNums;
            this.room_Info.showCardNums(leftCardNum);
        }

//恢复最后一张出的牌的标志
        {
            var curOutCardUser = ackGameFree.CurOutCardUser;
            var curOutCard = ackGameFree.curOutCard;
            var curOutCardPos = MJModel.getPosBySeatid(curOutCardUser);
            this.room_Card.initCurOutCardTip(curOutCardPos, curOutCard);

            var opType = ackGameFree.OpType;
            var curMoPaiCard = ackGameFree.Card;
            MJModel.curOperateCard = curMoPaiCard;
            MJModel.curOperate = opType;

            //服务器错误拦截 部分情况下服务器断线重连回来 有操作 但是没有Card数据
            if (MJModel.curOperate > 0 && MJModel.curOperateCard == 0){
                MJModel.curOperateCard = curOutCard;
            }
        }

//方向指示恢复 如果有人要出牌 则指向该人 否则 必然有人在选择 碰扛胡操作
        {
            if (curMoPaiPos != -1) {
                this.room_Info.showDirectionTip(curMoPaiPos);
            }
            else {
                this.room_Info.showDirectionTip(-1);
            }
        }

//是否处于自动胡牌
        var isAutoHu = ackGameFree.IsAutoHu;
        {
            MJModel.isAutoOutCard = isAutoHu == 1;
            if (this.room_Action) this.room_Action.setAutoOutCardVis(MJModel.isAutoOutCard);
            this.room_Card.setAutoOutCard(MJModel.isAutoOutCard);
        }

//自己状态恢复 如果有操作选项 就只显示操作选项  没有操作选项 看是不是自己要出牌 是的话 打开出牌功能 否则关闭
        {
            this.myselfCloseOutCard();


//四川麻将报听操作
            if (this.isScmjRoom() && MJModel.scmj_listenType != 0) {


// 处理报听躺牌
                for (var i = 0; i < ackGameFree.scmjCustomied.stOpListen.length; i++) {
                    var seatid = ackGameFree.scmjCustomied.stOpListen[i].ChairID;
                    var pos = MJModel.getPosBySeatid(seatid);
                    if (ackGameFree.scmjCustomied.stOpListen[i].isHaveOpListen) {
                        this.room_Tip.ShowBaoTing(pos);
                        if (MJModel.isMyPlayer(seatid)) {

                            MJModel.baoting_test = 1;
                            MJModel.isBaoting = true;
                        }
                    }

                    if (MJModel.scmj_listenType == LISTEN_TYPE.LISTEN_CARD_BAIPAI && ackGameFree.scmjCustomied.stOpListen[i].ChangeableCards.length > 0) {
// 处理躺牌
                        MJModel.setTangPaiFlag(pos, true);
                        var cards = [];
                        for (var j = 0; j < ackGameFree.scmjCustomied.stOpListen[i].ChangeableCards.length; j++) {
                            cards.push(ackGameFree.scmjCustomied.stOpListen[i].ChangeableCards[j]);
                        }
                        if (!MJModel.isMyPlayer(seatid)) {
                            this.room_Card.tangHandCard(pos, cards);
                        }
                    }
                }

            }

//报听操作
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ || this.isScmjRoom()) {
                if (( opType & ActionType.TYPE_BAOTING) == ActionType.TYPE_BAOTING) {
                    MJModel.IsMyHasBaoting = true;
                }
                else {
                    MJModel.IsMyHasBaoting = false;
                }
            }

            if ((opType & ActionType.TYPE_LISTEN) == ActionType.TYPE_LISTEN) {
                opType = opType - ActionType.TYPE_LISTEN;
            }

            if (opType > 0) {
                if (this.room_Action) {
                    this.room_Action.setOperator(opType);
                }
            }
            else {
                if (this.room_Action) this.room_Action.setOperator(-1);
            }
//贵州麻将
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
                if (ackGameFree.GZTableInfo) {

                    var jipai_size = ackGameFree.GZTableInfo.getJiPai.length;

                    var JiPaicardList = [];
                    MJModel.JiPaicardList = [];
                    for (var i = 0; i < jipai_size; i++) {
                        var value = ackGameFree.GZTableInfo.getJiPai[i].Jipai;
                        JiPaicardList.push(value);
                        MJModel.JiPaicardList.push(value);
                    }
                    if (ackGameFree.GZTableInfo.IsManTangJi) {
                        var IsManTangJi = ackGameFree.GZTableInfo.IsManTangJi;
                        for (var i = 0; i < 4; i++) {
                            this.room_Card.showAllJiPaiFlag(i, JiPaicardList, IsManTangJi);
                        }
                    }
                    var baoting_size = ackGameFree.GZTableInfo.IsBaoTing.length;
                    for (var i = 0; i < baoting_size; i++) {
                        var isbaoting = ackGameFree.GZTableInfo.IsBaoTing[i];
                        if (isbaoting) {
                            var pos = MJModel.getPosBySeatid(i);
                            this.room_Tip.ShowBaoTing(pos);
                            if (i == MJModel.seatid) {
                                MJModel.baoting_test = 2;
                                MJModel.isBaoting = true;
                            }
                        }
                    }
                }
            }

            if (curMoPaiPos != -1) {
                var mopai_seatid = MJModel.getSeatidByPos(curMoPaiPos);
                MJModel.cur_seat = mopai_seatid;

                if (MJModel.isMyPlayer(MJModel.cur_seat)) {
                    var is_ok = this.room_Card.removeHandCard(0, curMoPaiCard);
                    if (is_ok) this.room_Card.initMoCard(0, curMoPaiCard, true);
                    if (opType <= 0) this.myselfOpenOutCard(true);
                }
            }
        }

        if (this.isScmjRoom()) {

            if (this.room_Card.isAlreadyHu(0) && MJModel.seatid != -1) {
//开启自动胡牌
                this.scmjAutoPlay();
            }

//四川麻将初始化定缺状态
            this.mChangeBar.setVisible(false);
            this.mPaiSeBar.setVisible(false);
            this.mPiaoBar.setVisible(false);
            if (sceneStatus == 3) {
                for (var i = 0; i < ackGameFree.UserLackType.length; i++) {
                    var lack = ackGameFree.UserLackType[i];
                    var pos = MJModel.getPosBySeatid(i);
                    this.players[pos].setLackType(lack);

                    if (i == MJModel.seatid) {
                        MJModel.lackType = lack;
                        this.room_Card.sureLackType();
                    }
                }
            } else {
                if (this.room_Action) {
                    this.room_Action.setOperator(-1);
                }
            }

            if (sceneStatus != 0 && sceneStatus != 4) {
                for (var i = 0; i < ackGameFree.scmjCustomied.UserPiao.length; i++) {
                    var piao = ackGameFree.scmjCustomied.UserPiao[i];
                    var pos = MJModel.getPosBySeatid(i);
                    this.players[pos].setPiaoType(piao);
                }
            }

            if (MJModel.seatid != -1) {
                if (sceneStatus == 1) {
                    MJModel.canTouchCard = true;
                    MJModel.canChoiceMore = true;
                    this.mChangeBar.setVisible(true);
                } else if (sceneStatus == 2) {
                    this.mPaiSeBar.setGoodPaiSe(this.room_Card.getGoodPaiSe());
                    this.mPaiSeBar.setVisible(true);
                } else if (sceneStatus == 4) {
                    if (!ackGameFree.scmjCustomied.IsPiaoyet) {
                        this.mPiaoBar.setVisible(true);
                    }
                }

            }

        }

    },
    initPlayer: function () {
        var totalSeat = MJModel.mj_roomType == 1 ? 3 : 4;

        for (var i = 0; i < totalSeat; i++) {
            var player = MJModel.players[i];
            if (player.uid > 0) {
                var pos = MJModel.getPosBySeatid(i);
                var mjPlayer = this.players[pos];
                mjPlayer.login(i);
            }
        }
    },

    handler_server_ready_succ_bc: function (data, canDelay) {
        var ackUserReady = parsePacket("proto.game.AckUserReady", data);
        var ready_size = ackUserReady.ready.length;
        for (var i = 0; i < ready_size; i++) {
            var ready = ackUserReady.ready[i];

            var pos = MJModel.getPosBySeatid(i);
            var player = MJModel.players[i];
            player.ready = ready;
            if (player.uid != -1) {
                var mp = this.players[pos];
                if (ready != 0) {
                    mp.setReadyFlag(true);
                    if (MJModel.isZhuanZhuan) {
                        var player = this.players[pos];
                        player.removeZhuanClock();
                    }
                }
            }
            if (player.uid == ZJHModel.getInstance().uid && ready != 0) {
                if (this.room_Action) this.room_Action.setReadyVisible(false);
            }
        }
    },
    handler_server_ready_err_uc: function (data, canDelay) {
    },
    handler_server_game_start_bc: function (data, canDelay) {
        this.resetGame();
        this.room_Info.setVisible(true);
        if (canDelay) this.room_Tip.ShowGameState(1);
        MJModel.state = MJ_STATUS.MJ_GAME_PLAYING;
        if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GDMJ_DZ) {
            var gameStart = parsePacket("proto.game.gdmahjonggame.AckGameStart", data);
            MJModel.cur_seat = gameStart.bankerid;

            MJModel.banker_seatid = gameStart.bankerid;

            MJModel.gdmj_fengquan = gameStart.quanfeng - 49;
            this.room_Info.gdmj_showFengQuan(MJModel.gdmj_fengquan);

            //显示各个玩家的风位
            for (var i = 0; i < 4; i++) {
                this.players[i].setFengWei(MJModel.getSeatidByPos(i));
            }
        }
        else {
            var gameStart = parsePacket("proto.game.AckGameStart", data);
            MJModel.cur_seat = gameStart.ChairID;

            MJModel.banker_seatid = gameStart.ChairID;
        }

        var pos = MJModel.getPosBySeatid(MJModel.banker_seatid);
        var player = this.players[pos];
        if (player) {
            player.setBankerFlag(true);
        }

        if (MJModel.isZhuanZhuan) {
            this.stopZhuanClock();
        }

        this.room_Info.showDirectionTip(MJModel.getPosBySeatid(MJModel.cur_seat));

        JsUtils.postNotifi("notify_game_check_end");//为了刷新局数 或者  开始倒计时
    },

    handler_server_start_piao: function (data, canDelay) {
        if (MJModel.seatid != -1 && canDelay) {
            this.room_Info.showTime(MJModel.piaoTime);
            if (this.isScmjRoom()) {
                this.mPiaoBar.setVisible(true);
            }
        }
    },
    handler_server_broad_piao: function (data, canDelay) {
        var broadPiao = parsePacket("proto.game.scmj.AckBroadPiao", data);
        var seatId = broadPiao.chariid;
        var pos = MJModel.getPosBySeatid(seatId);
        this.players[pos].setPiaoType(broadPiao.UserPiao);
    },
    handler_server_broad_listentype: function (data, canDelay) {
        var ackbaoting = parsePacket("proto.game.scmj.AckBroadListentype", data);
        var seatid = ackbaoting.chariid;
        MJModel.scmj_listenType = ackbaoting.ListenType;
        var pos = MJModel.getPosBySeatid(seatid);
        this.room_Tip.ShowBaoTing(pos);
        this.room_Tip.showTingEffect(pos, MJModel.scmj_listenType);
        if (seatid == MJModel.seatid) {
            MJModel.canTouchCard = true;
            MJModel.isBaoting = true;
            this.room_Card.setBaoTingCardClickEnable(false, false);
            MJModel.baoting_test = 3;

        }
        var player = MJModel.players[seatid];
        if (player) {
            Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/gzsound/operator/gz_%1_jiaozui.mp3"), player.sex == SEX_ID.SEX_FEMALE ? "w" : "m"));
        }

        if (MJModel.scmj_listenType == LISTEN_TYPE.LISTEN_CARD_BAIPAI && ackbaoting.ChangeableCards.length > 0) {
            // 要处理一下躺牌的情况
            MJModel.setTangPaiFlag(pos, true);
            var cards = [];
            for (var i = 0; i < ackbaoting.ChangeableCards.length; i++) {
                cards.push(ackbaoting.ChangeableCards[i]);
            }

            if (!MJModel.isMyPlayer(seatid)) {
                this.room_Card.tangHandCard(pos, cards);
            }

        }
    },

    handler_server_game_start_bc2: function (data, canDelay) {
        if (data == "")return;
        var firstHandCard = parsePacket("proto.game.AckFirstHandCard", data);

        var cardData = [];
        if (firstHandCard.UserCard && firstHandCard.UserCard.ChangeableCards) {
            var changeable_Size = firstHandCard.UserCard.ChangeableCards.length;
            for (var i = 0; i < changeable_Size; i++) {
                var cardDataNum = firstHandCard.UserCard.ChangeableCards[i];
                cardData.push(cardDataNum);
            }
        }

        if (firstHandCard.HandCardNums) {
            var handcardnumsSize = firstHandCard.HandCardNums.length;
            //当前玩家手牌长度
            for (var i = 0; i < handcardnumsSize; i++) {
                var pos = MJModel.getPosBySeatid(i);
                var carNums = firstHandCard.HandCardNums[i];
                if (MJModel.isMyPlayer(i)) {
                    MJModel.isEnterWait = true;
                    this.room_Card.fapai(cardData);

                    //如果是四川麻将则需要换牌
                    if (this.isScmjRoom() && canDelay) {
                        this.mPiaoBar.setVisible(false);
                        if (MJModel.isNeedChangeCard) {
                            MJModel.canChoiceMore = true;
                            MJModel.canTouchCard = true;
                            this.room_Info.showTime(MJModel.changeTime);
                            this.mChangeBar.setVisible(true);
                        }
                    }

                    if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_HNMJ_DZ && canDelay) {
                        this.mGaBar.setVisible(false);
                    }
                } else {
                    var cardlist = [];
                    for (var j = 0; j < carNums; j++) {
                        cardlist.push(0);
                    }

                    this.room_Card.initHandCard(pos, cardlist);
                    this.room_Card.reflashHandCard(pos, true);
                }
            }
        }

        PlatformHelper.vibrate();
    },
    handler_server_game_start_bc_onvideo: function (data, canDelay) {
        var firstHandCard = parsePacket("proto.game.VideoFirstHandCard", data);
        for (var i = 0; i < firstHandCard.UserCard.length; i++) {
            var pos = MJModel.getPosBySeatid(i);
            var changeable_Size = firstHandCard.UserCard[i].ChangeableCards.length;
            var cardList = [];

            //手里的牌
            for (var j = 0; j < changeable_Size; j++) {
                cardList.push(firstHandCard.UserCard[i].ChangeableCards[j]);
            }
            this.room_Card.initHandCard(pos, cardList);
            this.room_Card.reflashHandCard(pos, true);
        }
    },
    handler_server_mo_card_bc: function (data, canDelay) {
        var userGetCard = parsePacket("proto.game.AckUserGetCard", data);
        var seatId = userGetCard.ChairID;
        var card = userGetCard.Card;
        var operate = userGetCard.OpType;
        var leftCardNum = userGetCard.LeftCardNum;

        MJModel.cur_seat = seatId;

        this.room_Info.showCardNums(leftCardNum);
        this.room_Info.showDirectionTip(MJModel.getPosBySeatid(seatId));
        this.room_Info.showTime(MJModel.outCardTime);

        var pos = MJModel.getPosBySeatid(seatId);

        if (canDelay == false) {
            this.room_Card.initMoCard(pos, card, true);
            return;
        }

        if (MJModel.isMyPlayer(seatId)) {
            Sound.getInstance().playEffect(getResPath("RoomMJ/sound/mo_card.mp3"));

            MJModel.curOperateCard = card;
            MJModel.curOperate = operate;
            this.room_Card.initMoCard(pos, card, false);

            //my operate action
            {
                MJModel.gang_info = [];
                MJModel.listen_info = [];

                //点击出来有听操作提示
                if (userGetCard.ListenInfo.length > 0) {
                    var listeninfo_size = userGetCard.ListenInfo.length;

                    for (var i = 0; i < listeninfo_size; i++) {
                        var tli = Utils.clone(tagListenInfo);
                        tli.HuInfo = [];

                        tli.OutCard = userGetCard.ListenInfo[i].OutCard;
                        var huinfoLen = userGetCard.ListenInfo[i].HuInfo.length;
                        tli.HuInfoLen = huinfoLen;
                        for (var j = 0; j < huinfoLen; j++) {
                            var huInfo = Utils.clone(tagHuInfo);
                            huInfo.Card = userGetCard.ListenInfo[i].HuInfo[j].Card;
                            huInfo.LeftNum = userGetCard.ListenInfo[i].HuInfo[j].LeftNum;
                            huInfo.HuFanNum = userGetCard.ListenInfo[i].HuInfo[j].HuFanNum;
                            tli.HuInfo.push(huInfo);
                        }
                        MJModel.listen_info.push(tli);
                    }
                }


                //报听操作
                if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ || this.isScmjRoom()) {
                    if (( operate & ActionType.TYPE_BAOTING) == ActionType.TYPE_BAOTING) {
                        MJModel.IsMyHasBaoting = true;

                    }
                    else {
                        MJModel.IsMyHasBaoting = false;
                    }
                }
                //贵州麻将新加 end
                if (userGetCard.GangInfo.length > 0) {
                    var gang_size = userGetCard.GangInfo.length;

                    for (var i = 0; i < gang_size; i++) {
                        var tgInfo = Utils.clone(tagGangInfo);
                        tgInfo.curCard = userGetCard.GangInfo[i].Card;
                        tgInfo.state = userGetCard.GangInfo[i].state;

                        MJModel.gang_info.push(tgInfo);
                    }
                }

                if (( operate & ActionType.TYPE_LISTEN) == ActionType.TYPE_LISTEN) {
                    operate = operate - ActionType.TYPE_LISTEN;
                }

                //是否有碰杠胡的操作
                if (operate > 0) {
                    if (this.room_Action) {
                        this.room_Action.setOperator(operate);
                        this.autoHuCard();
                    }
                    return;
                }
                else {
                    this.myselfOpenOutCard(true);
                }
            }
        }
        else {
            this.room_Card.initMoCard(pos, card, false);
        }
    },
    handler_server_out_card_bc: function (data, canDelay) {
        this.unschedule(this.autoOutCardTimeOut);
        this.unschedule(this.autoHuCardTimeOut);

        this.room_Info.stopTime();
        this.room_Tip.showTextTip("");
        var userOutCard = parsePacket("proto.game.AckUserOutCard", data);
        var seatId = userOutCard.ChairID;
        var card = userOutCard.Card;

        MJModel.cur_seat = -1;


        var pos = MJModel.getPosBySeatid(seatId);

        if (canDelay == false) {
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GDMJ_DZ && card / 16 == 5) {
                this.room_Card.addHuaCard(pos, card);
                this.room_Card.removeHandCard(pos, card);
                this.room_Tip.showHuaTip(pos);
            }
            else {
                this.room_Card.addOutCard(pos, card);
                this.room_Card.removeHandCard(pos, card);
            }
            return;
        }

        this.players[pos].setTimeOut(false);

        this.room_Card.outHandCard(pos, card);

        if (this.room_Action) this.room_Action.setOperator(-1);

        if (MJModel.isMyPlayer(seatId)) {
            if (this.isScmjRoom() && this.room_Card.isAlreadyHu(0) && MJModel.listen_info.length == 0) {
                //如果是四川麻将，并且我已经胡过了，但是没有听牌信息
                //说明刚刚自摸，此时服务器会直接将摸到的手牌打出去且不会下发听牌信息，所以要单独处理一下听牌信息
                if (this.room_Action) this.room_Action.checkAutoOutCardVis();
            } else {
                MJModel.final_listen_info = [];
                //检测出的牌是否在听牌里边 不在的话就清楚听牌信息
                for (var i = 0; i < MJModel.listen_info.length; i++) {
                    var lsi = MJModel.listen_info[i];
                    if (lsi.OutCard == card) {
                        MJModel.final_listen_info = lsi.HuInfo;
                        break;
                    }
                }

                MJModel.listen_info = [];

                //可以听牌  可以选择自动胡牌
                if (MJModel.final_listen_info.length > 0) {
                    if (this.room_Action) this.room_Action.checkAutoOutCardVis();
                    this.room_Card.setAutoOutCard(MJModel.isAutoOutCard);
                }
                else {
                    if (this.room_Action) this.room_Action.setAutoOutCardVis(false);
                    this.room_Card.setAutoOutCard(false);
                    MJModel.isAutoOutCard = false;
                }

            }

            this.myselfCloseOutCard();
        }
        if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
            //贵州麻将鸡牌冲锋鸡
            if (userOutCard.GZOutCard) {
                if (userOutCard.GZOutCard.IsChongFengJi) {
                    this.room_Tip.showJiType(pos, ActionType.JITYPE_CHONGFENGJI);
                }
            }
        }

        var player = MJModel.players[seatId];
        if (player) {
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
                Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/gzsound/chupai/gz_%1_%2.mp3"), player.sex == SEX_ID.SEX_FEMALE ? "w" : "m", Utils.toHex2(card)));
            }
            else if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GDMJ_DZ) {
                Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/gdmj/chupai/%1_%2.mp3"), player.sex == SEX_ID.SEX_FEMALE ? "female" : "male", Utils.toHex2(card)));
            } else if (this.isScmjRoom()) {
                Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/scmj/chupai/%1_%2.mp3"), player.sex == SEX_ID.SEX_FEMALE ? "sc_woman" : "sc_man", Utils.toHex2(card)));
            }
            else {
                this.handler_out_card_sound(player.sex, card);
            }

        }
    },

    handler_out_card_sound: function (sex, card) {
        Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/chupai/%1_%2.mp3"), sex == SEX_ID.SEX_FEMALE ? "w" : "m", Utils.toHex2(card)));
    },

    handler_server_operate_notify_bc: function (data, canDelay) {
        this.room_Info.showTime(MJModel.oprateTime);
        this.room_Info.showDirectionTip(-1);

        var ackOpNotify = parsePacket("proto.game.AckOpNotify", data);
        var seatid = ackOpNotify.ChairID;
        var optype = ackOpNotify.OpType;
        var card = ackOpNotify.Card;

        MJModel.cur_seat = -1;

        if (MJModel.isMyPlayer(seatid)) {

            MJModel.curOperateCard = card;
            MJModel.curOperate = optype;
            if (this.room_Action) this.room_Action.setOperator(optype);
            this.autoHuCard();

            MJModel.cur_seat = seatid;
        }
    },
    handler_server_peng_kang_bc: function (data, canDelay) {
        this.room_Tip.showTextTip("");
        var ackBcOpResult = parsePacket("proto.game.AckBcOpResult", data);
        this.unschedule(this.autoOutCardTimeOut);
        this.unschedule(this.autoHuCardTimeOut);

        var seatid = ackBcOpResult.ChairID;
        var out_seatid = ackBcOpResult.OutCardChairID;
        var OpType = ackBcOpResult.OpType;
        var card = ackBcOpResult.Card;

        if (MJModel.isMyPlayer(seatid)) {
            if (this.room_Action) this.room_Action.setOperator(-1);
        }

        var pos = MJModel.getPosBySeatid(seatid);
        var out_pos = MJModel.getPosBySeatid(out_seatid);

        this.players[pos].setTimeOut(false);

        if ((OpType & ActionType.TYPE_PASS) == ActionType.TYPE_PASS) {

            if (MJModel.isMyPlayer(seatid) && seatid == out_seatid)//如果是我自己摸的牌  自己过 则放开选牌功能
            {
                this.room_Info.showTime(MJModel.outCardTime);
                this.myselfOpenOutCard();
            }

            return;
        }
        //贵州麻将
        if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
            if (ackBcOpResult.GZGang) {
                MJModel.isHanBaoDou = ackBcOpResult.GZGang.IsHanBaoDou;
            }
            if (ackBcOpResult.GZOpResult) {
                if (ackBcOpResult.GZOpResult.IsZeRenJi) {
                    this.room_Tip.showJiType(pos, ActionType.JITYPE_ZERENJI);
                }
            }
        }

        if ((OpType & ActionType.TYPE_WANGANG) == ActionType.TYPE_WANGANG || (OpType & ActionType.TYPE_ZHIGANG) == ActionType.TYPE_ZHIGANG || (OpType & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG || (OpType & ActionType.TYPE_PENG) == ActionType.TYPE_PENG || (OpType & ActionType.TYPE_LEFT_CHI) == ActionType.TYPE_LEFT_CHI || (OpType & ActionType.TYPE_CENTER_CHI) == ActionType.TYPE_CENTER_CHI || (OpType & ActionType.TYPE_RIGHT_CHI) == ActionType.TYPE_RIGHT_CHI) {
            var needRemoveOutCard = false;
            if ((OpType & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG) {
                this.room_Card.addPengCard(pos, PengType.Peng_AnKang, card);
                this.room_Card.removeHandCard(pos, card, 4);
                if (canDelay) this.room_Tip.showEffect(pos, ActionType.TYPE_ANGANG);
            }
            else if ((OpType & ActionType.TYPE_PENG) == ActionType.TYPE_PENG) {
                this.room_Card.addPengCard(pos, PengType.Peng_Peng, card);
                this.room_Card.removeHandCard(pos, card, 2);
                if (canDelay) this.room_Tip.showEffect(pos, ActionType.TYPE_PENG);
                needRemoveOutCard = true;
            }
            else if ((OpType & ActionType.TYPE_ZHIGANG) == ActionType.TYPE_ZHIGANG) {
                this.room_Card.addPengCard(pos, PengType.Peng_Kang, card);
                this.room_Card.removeHandCard(pos, card, 3);
                if (canDelay) this.room_Tip.showEffect(pos, ActionType.TYPE_ZHIGANG);
                needRemoveOutCard = true;
            }
            else if ((OpType & ActionType.TYPE_WANGANG) == ActionType.TYPE_WANGANG) {
                this.room_Card.changPengCard(pos, PengType.Peng_Kang, card);
                this.room_Card.removeHandCard(pos, card, 1);
                if (canDelay) this.room_Tip.showEffect(pos, ActionType.TYPE_WANGANG);
            }
            else if ((OpType & ActionType.TYPE_LEFT_CHI) == ActionType.TYPE_LEFT_CHI) {
                this.room_Card.addPengCard(pos, PengType.Chi_L, card);
                this.room_Card.removeHandCard(pos, card + 1, 1);
                this.room_Card.removeHandCard(pos, card + 2, 1);
                if (canDelay) this.room_Tip.showEffect(pos, ActionType.TYPE_LEFT_CHI);
                needRemoveOutCard = true;
            }
            else if ((OpType & ActionType.TYPE_CENTER_CHI) == ActionType.TYPE_CENTER_CHI) {
                this.room_Card.addPengCard(pos, PengType.Chi_M, card);
                this.room_Card.removeHandCard(pos, card - 1, 1);
                this.room_Card.removeHandCard(pos, card + 1, 1);
                if (canDelay) this.room_Tip.showEffect(pos, ActionType.TYPE_CENTER_CHI);
                needRemoveOutCard = true;
            }
            else if ((OpType & ActionType.TYPE_RIGHT_CHI) == ActionType.TYPE_RIGHT_CHI) {
                this.room_Card.addPengCard(pos, PengType.Chi_R, card);
                this.room_Card.removeHandCard(pos, card - 1, 1);
                this.room_Card.removeHandCard(pos, card - 2, 1);
                if (canDelay) this.room_Tip.showEffect(pos, ActionType.TYPE_RIGHT_CHI);
                needRemoveOutCard = true;
            }

            if (needRemoveOutCard) {
                MJModel.cur_seat = seatid;

                this.room_Card.removeOutCard(out_pos, card);
                this.room_Card.setCurCardTip(false);
                this.room_Info.showDirectionTip(pos);
                if (MJModel.isMyPlayer(seatid)) {
                    this.myselfOpenOutCard();
                }
            }
        }
        else if ((OpType & ActionType.TYPE_HU) == ActionType.TYPE_HU) {
            if (!this.isScmjRoom()) {
                this.room_Info.showDirectionTip(-1);
                this.room_Info.stopTime();
                MJModel.canTouchCard = false;
            } else {
                this.room_Tip.showHuTip(pos);
                this.room_Card.addHuCard(pos, card);

                if (seatid == MJModel.seatid) {
                    //开启自动胡牌
                    this.scmjAutoPlay();
                } else {
                    if (this.room_Action.isHuOpVis()) {
                        this.room_Action.setOperator(ActionType.TYPE_HU);
                    } else {
                        this.room_Action.setOperator(-1);
                    }
                }
            }

            if (canDelay) this.room_Tip.showEffect(pos, ActionType.TYPE_HU);
        }

        if (MJModel.isMyPlayer(seatid) && !this.isScmjRoom()) {
            if (this.room_Action) this.room_Action.setAutoOutCardVis(false);
            MJModel.isAutoOutCard = false;
            this.room_Card.setAutoOutCard(false);
        }

        var player = MJModel.players[seatid];
        if (player && canDelay) {
            var action = "gang";
            if ((OpType & ActionType.TYPE_HU) == ActionType.TYPE_HU) {
                action = "hu";
            }
            else if ((OpType & ActionType.TYPE_PENG) == ActionType.TYPE_PENG) {
                action = "peng";
            }
            else if ((OpType & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG) {
                action = "angang";
            }
            else if ((OpType & ActionType.TYPE_LEFT_CHI) == ActionType.TYPE_LEFT_CHI || (OpType & ActionType.TYPE_CENTER_CHI) == ActionType.TYPE_CENTER_CHI || (OpType & ActionType.TYPE_RIGHT_CHI) == ActionType.TYPE_RIGHT_CHI) {
                action = "chi";
            }
            if ((OpType & ActionType.TYPE_WANGANG) == ActionType.TYPE_WANGANG || (OpType & ActionType.TYPE_ZHIGANG) == ActionType.TYPE_ZHIGANG || (OpType & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG || (OpType & ActionType.TYPE_PENG) == ActionType.TYPE_PENG || (OpType & ActionType.TYPE_HU) == ActionType.TYPE_HU) {
                if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
                    if ((OpType & ActionType.TYPE_WANGANG) == ActionType.TYPE_WANGANG) {
                        action = "paipodou";
                    }
                    else if ((OpType & ActionType.TYPE_ZHIGANG) == ActionType.TYPE_ZHIGANG) {
                        action = "diandou";
                    }
                    else if ((OpType & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG) {
                        action = "mendou";
                    }
                    else if ((OpType & ActionType.TYPE_ANGANG) == ActionType.TYPE_PENG) {
                        action = "peng";
                    }
                    else if ((OpType & ActionType.TYPE_ANGANG) == ActionType.TYPE_HU) {
                        action = "hu";
                    }
                    Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/gzsound/operator/gz_%1_%2.mp3"), player.sex == SEX_ID.SEX_FEMALE ? "w" : "m", action));
                    return;
                }
            }

            if ((OpType & ActionType.TYPE_WANGANG) == ActionType.TYPE_WANGANG || (OpType & ActionType.TYPE_ZHIGANG) == ActionType.TYPE_ZHIGANG || (OpType & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG) {
                if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_YNMJ_DZ) {
                    return;
                }
            }

            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GDMJ_DZ) {
                Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/gdmj/operator/%1_%2.mp3"), player.sex == SEX_ID.SEX_FEMALE ? "female" : "male", action));
            } else if (this.isScmjRoom()) {

                Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/scmj/operator/%1_%2.mp3"), player.sex == SEX_ID.SEX_FEMALE ? "sc_woman" : "sc_man", action));
            }
            else {
                this.handler_op_sound(player.sex, OpType, action);
            }
        }
    },

    handler_op_sound: function (sex, OpType, action) {
        Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/operator/%1_%2.mp3"), sex == SEX_ID.SEX_FEMALE ? "w" : "m", action));
    },

    handler_server_tingpai_notify: function (data, canDelay) {
        var ackListenNotify = parsePacket("proto.game.AckListenNotify", data);
        var OpType = ackListenNotify.OpType;

        //点击出来有听操作提示
        if (OpType == ActionType.TYPE_LISTEN && ackListenNotify.ListenInfo.length > 0) {
            var listeninfo_size = ackListenNotify.ListenInfo.length;
            this.room_Card.setTingPaiFlag(false);
            MJModel.listen_info = [];
            for (var i = 0; i < listeninfo_size; i++) {
                var tli = Utils.clone(tagListenInfo);
                tli.HuInfo = [];

                tli.OutCard = ackListenNotify.ListenInfo[i].OutCard;
                var huinfoLen = ackListenNotify.ListenInfo[i].HuInfo.length;
                tli.HuInfoLen = huinfoLen;
                for (var j = 0; j < huinfoLen; j++) {
                    var huInfo = Utils.clone(tagHuInfo);
                    huInfo.Card = ackListenNotify.ListenInfo[i].HuInfo[j].Card;
                    huInfo.LeftNum = ackListenNotify.ListenInfo[i].HuInfo[j].LeftNum;
                    huInfo.HuFanNum = ackListenNotify.ListenInfo[i].HuInfo[j].HuFanNum;
                    tli.HuInfo.push(huInfo);
                }
                MJModel.listen_info.push(tli);
            }
            if (MJModel.isMyPlayer(MJModel.cur_seat)) {
                this.room_Card.setTingPaiFlag(true);
                if (this.room_Action) this.room_Action.setTingPaiVis(false);
            }
        }
        else {
            MJModel.listen_info = [];
            this.room_Card.setTingPaiFlag(false);
            if (this.room_Action) this.room_Action.setTingPaiVis(false);
        }
    },
    handler_server_card_hu_bc: function (data, canDelay) {
    },

    handler_server_game_end_bc: function (data, canDelay) {
        Log(this, "handler_server_game_end_bc");

        if (true) {
            var self = this;
            this.room_Card.setAutoOutCard(false);
            if (this.room_Action) this.room_Action.setTingPaiVis(false);
            if (this.room_Action) this.room_Action.setAutoOutCardVis(false);
            if (this.room_Action) this.room_Action.setOperator(-1);
            this.room_Info.stopTime();
            var ackGameEnd = parsePacket("proto.game.AckGameEnd", data);
            if (ackGameEnd.HorseInfo && ackGameEnd.HorseInfo.HorseNum > 0 && canDelay) {
                this.handler_server_game_end_bc_maima(data, canDelay);
            }
            else {
                if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
                    if (canDelay) {
                        var state = ackGameEnd.EndState;
                        if (state == 1) {
                            //流局
                            if (ackGameEnd.GuiZhouEndInfo) {
                                var ischajiao = ackGameEnd.GuiZhouEndInfo.IsCheckDaJiao;//贵州麻将是否黄庄查叫
                                this.room_Tip.ShowHuangZhuang(ischajiao);
                            }

                        }
                        else {
                            this.room_Tip.ShowGameState(2);
                        }
                    }

                    var callback = cc.callFunc(function () {
                        self.handler_server_game_end_bc_final(data, canDelay);
                    });
                    this.runAction(cc.sequence(cc.delayTime(!canDelay ? 0 : 1.5), callback));
                }
                //如果是四川麻将，流局处理花猪和大叫
                else if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_YNMJ_DZ) {
                    if (canDelay) {
                        var state = ackGameEnd.EndState;
                        if (state == 1) {
                            var roomScEndCheck = new MJSCEndCheck();
                            roomScEndCheck.setName("RoomMJSCEndCheck");
                            roomScEndCheck.loadData(data);
                            this.addChild(roomScEndCheck, 100000);
                        }
                        else {
                            this.room_Tip.ShowGameState(2);
                        }
                    }
                    var callback = cc.callFunc(function () {

                        self.removeChildByName("RoomMJSCEndCheck");
                        self.handler_server_game_end_bc_final(data, canDelay);
                    });

                    this.runAction(cc.sequence(cc.delayTime(!canDelay ? 0 : ((MJModel.isNeedSearchHuazhuDajiao && ackGameEnd.EndState == 1) ? 6 : 2.5)), callback));
                }
                else {
                    if (canDelay) {
                        var state = ackGameEnd.EndState;
                        if (state == 1) {
                            //流局
                            this.room_Tip.ShowGameState(3);
                        }
                        else {
                            this.room_Tip.ShowGameState(2);
                        }
                    }
                    var callback = cc.callFunc(function () {
                        self.handler_server_game_end_bc_final(data, canDelay);
                    });
                    this.runAction(cc.sequence(cc.delayTime(!canDelay ? 0 : 1.5), callback));
                }

            }
        }
        else {
            this.room_Card.setAutoOutCard(false);
            if (this.room_Action) this.room_Action.setTingPaiVis(false);
            if (this.room_Action) this.room_Action.setAutoOutCardVis(false);
            if (this.room_Action) this.room_Action.setOperator(-1);
            this.room_Info.stopTime();

            MJModel.state = MJ_STATUS.MJ_GAME_FREE;

            var usercard_size = 4;

            var cardData = [];
            for (var i = 0; i < usercard_size; i++) {
                //结束玩家手牌数组
                cardData = [];

                for (var j = 0; j < 13; j++) {
                    var cardDataNum = 1;
                    cardData.push(cardDataNum);
                }

                var pos = MJModel.getPosBySeatid(i);
                this.room_Card.showEndCard(pos, cardData, 0, false);
            }

            var scoresLen = 4;
            for (var i = 0; i < scoresLen; i++) {
                var pos = MJModel.getPosBySeatid(i);
                var player = MJModel.players[i];
                if (player.uid > 0) {
                    var player = this.players[pos];
                    player.showBalanceNumber(1000);
                }
            }

            var callback = cc.callFunc(function () {
                if (MJModel.seatid != -1) {
                    if (self.room_Action) self.room_Action.setReadyVisible(true);
                }
            });
            this.runAction(cc.sequence(cc.delayTime(3), callback));
            if (MJModel.isTimeEnd) this.startTimeEnd();
        }
    },
    handler_server_game_preready_bc: function (data, canDelay) {
        Log(this, "handler_server_game_preready_bc");
        if (MJModel.isZhuanZhuan && !MJModel.isEnd) {
            for (var i = 0; i < 4; i++) {
                var player = MJModel.players[i];
                if (player) {
                    player.zhuan_surplustime = MJModel.zhuanReadyTime;
                }
            }
            this.startZhuanClock();

            if (MJModel.seatid != -1) {
                if (this.room_Action) this.room_Action.setReadyVisible(true);
            }
        }
    },
    handler_server_bigface_bc: function (data, canDelay) {
        var jpacket = parsePacket("proto.login.AckEmotionBC", data);
        var from_seat = jpacket.seatid;
        var to_seat = jpacket.target_seatid;
        var type = jpacket.type;

        var bpos = MJModel.getPosBySeatid(from_seat);
        var epos = MJModel.getPosBySeatid(to_seat);

        this.room_Tip.showBigFace(bpos, epos, type, MJModel.players[from_seat].sex);
    },
    handler_server_update_info_uc: function (data, canDelay) {
        var ackUpdateUserInfo = parsePacket("proto.game.AckUpdateUserInfo", data);
        var seadid = ackUpdateUserInfo.chairid;
        var myPlayer = MJModel.players[seadid];
        var uid = ackUpdateUserInfo.uid;
        var pos = MJModel.getPosBySeatid(seadid);
        if (myPlayer && uid == myPlayer.uid) {
            myPlayer.money = ackUpdateUserInfo.money;
            myPlayer.rmb = ackUpdateUserInfo.rmb;
            this.players[pos].updateInfo();
        }
    },

    handler_server_up_table_bc: function (data, canDelay) {
        var ackUpTableSuccess = parsePacket("proto.login.AckUpTableSuccess", data);
        var seatid = ackUpTableSuccess.seatid;

        var player = MJModel.players[seatid];
        player.reset();

        player.seatid = seatid;
        player.uid = ackUpTableSuccess.uid;
        player.name = ackUpTableSuccess.name;
        player.money = ackUpTableSuccess.money;
        player.ready = ackUpTableSuccess.ready;
        player.sex = ackUpTableSuccess.sex;
        player.avatar = ackUpTableSuccess.avatar;

        if (player.uid == ZJHModel.getInstance().uid) {
            MJModel.waitUpdate = true;
            CCTcpClient.getInstance(MJModel.curRoomID).reset_response_queue();

            sendTableInfoReq();

            // this.room_Tip.showMyTip("您已上桌");
            if (MJModel.isZhuanZhuan) {
                if (this.room_Action) {
                    this.room_Action.setDuiLieStr("游戏中");
                }
            }
        }
        else {
            var pos = MJModel.getPosBySeatid(seatid);
            var rPlayer = this.players[pos];
            rPlayer.login(player.seatid);

            JsUtils.postNotifi("notify_room_check_ip");
        }

        this.getPlayerList();

        if (MJModel.isZhuanZhuan) {
            var player = MJModel.players[seatid];
            player.zhuan_surplustime = MJModel.zhuanReadyTime;
            if (this.room_Action) {
                this.room_Action.setPlayerQueueVisible(false);
            }
        }
    },
    handler_server_down_table_bc: function (data, canDelay) {
        var ackDownTableSuccess = parsePacket("proto.login.AckDownTableSuccess", data);
        var uid = ackDownTableSuccess.uid;
        var seatid = ackDownTableSuccess.seatid;

        var player = MJModel.players[seatid];
        player.uid = 0;
        var pos = MJModel.getPosBySeatid(seatid);
        var roomPlayer = this.players[pos];
        roomPlayer.reset();
        roomPlayer.logout();
        this.room_Tip.ShowBaoTing(pos, false);
        if (MJModel.isMyPlayer(seatid)) {
            MJModel.tempSeatid = MJModel.seatid;
            MJModel.seatid = -1;
            ZJHModel.getInstance().mySeatid = MJModel.seatid;
            if (this.room_Action && !MJModel.isZhuanZhuan) {
                this.room_Action.setUpTableVisible(true);
            }

            // this.room_Tip.showMyTip("您已下桌");

            if (this.room_Action) {
                this.room_Action.setReadyVisible(false);
                this.room_Action.setDuiLieStr("不在队列中");
            }
        }
        if (MJModel.isZhuanZhuan) {
            if (this.room_Action) {
                this.room_Action.setPlayerQueueVisible(false);
            }
        }
    },

    handler_game_time_over_bc: function (data, canDelay) {
        MJModel.state = MJ_STATUS.MJ_GAME_FREE;
        this.startTimeEnd();
    },

    handler_game_time_out_bc: function (data, canDelay) {
        var ackUpdateUserInfo = parsePacket("proto.game.AckUpdateTimer", data);
        var seatid = ackUpdateUserInfo.ChairID;
        var time = ackUpdateUserInfo.lefttime;

        this.room_Info.showTime(time);
        if (seatid != -1) {
            this.room_Info.showDirectionTip(MJModel.getPosBySeatid(seatid));
        }

        if (!MJModel.isMyPlayer(seatid)) this.room_Tip.showTextTip("等待玩家重连");
    },

    handler_game_heartbeat_uc: function (data, canDelay) {
        MJModel.lastHeartBeatTime = time(null);
        Log(this, "心跳回来了");
    },

    handler_server_game_end_bc_final: function (data, canDelay) {
        Log(this, "handler_server_game_end_bc_final");

        var self = this;
        var ackGameEnd = parsePacket("proto.game.AckGameEnd", data);
        if (MJModel.isOnVideo) {
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
                var state = ackGameEnd.EndState;
                var ischajiao = ackGameEnd.GuiZhouEndInfo.IsCheckDaJiao;
                if (!ischajiao && state == 1) {
                    //流局并且黄庄不查叫
                    return;
                }
                else {
                    var result = new MJResult();
                    result.ShowGZMJData(data);
                    result.setName("RoomMJResultDetail");
                    this.addChild(result, 100000);
                    return;
                }
            } else {
                this.showFinalMJResult(data);
                return;
            }
        }


        var usercard_size = ackGameEnd.UserCard.length;
        var isGzInfo = ackGameEnd.GuiZhouEndInfo != null;
        var IsShowHandCard = true;
        if (isGzInfo) {
            //贵州麻将信息
            var state = ackGameEnd.EndState;
            if (state != 1) {
                IsShowHandCard = false;
            }
        }
        if (IsShowHandCard) {
            var cardData = [];
            for (var i = 0; i < usercard_size; i++) {
                //结束玩家手牌数组
                var changeable_Size = ackGameEnd.UserCard[i].ChangeableCards.length;
                cardData = [];

                for (var j = 0; j < changeable_Size; j++) {
                    var cardDataNum = ackGameEnd.UserCard[i].ChangeableCards[j];
                    cardData.push(cardDataNum);
                }

                var pos = MJModel.getPosBySeatid(i);
                if (ackGameEnd.EndInfo.length > i) {
                    this.room_Card.showEndCard(pos, cardData, ackGameEnd.EndInfo[i].HuCard, ackGameEnd.EndInfo[i].EndType == 1);
                }
                else {
                    this.room_Card.showEndCard(pos, cardData, 0, false);
                }
            }
        }

        var scoresLen = ackGameEnd.Score.length;
        var myselfwin = 0;

        for (var i = 0; i < scoresLen; i++) {
            var pos = MJModel.getPosBySeatid(i);
            var player = MJModel.players[i];
            if (player.uid > 0) {
                var player = this.players[pos];
                player.showBalanceNumber(ackGameEnd.Score[i]);

                if (pos == 0) {
                    myselfwin = ackGameEnd.Score[i];
                }
            }
        }

        if (MJModel.seatid != -1) {
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
                var state = ackGameEnd.EndState;
                var ischajiao = ackGameEnd.GuiZhouEndInfo.IsCheckDaJiao;
                if (state == 1 && !ischajiao) {
                    //黄庄未查叫
                    var callback = cc.callFunc(function () {
                        if (MJModel.seatid != -1) {
                            if (self.room_Action) self.room_Action.setReadyVisible(true);
                        }
                    });
                    this.runAction(cc.sequence(cc.delayTime(3), callback));
                    return;
                }
            }

            //展示小结算
            {
                if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
                    var state = ackGameEnd.EndState;
                    if (state == 1) {
                        //流局
                        var result = new MJResult();
                        result.ShowHuangZhuangData(data);
                        result.setName("RoomMJResultDetail");
                        this.addChild(result, 100000);
                    }
                    else {
                        var detailLayer = new MJResult();
                        detailLayer.setName("RoomMJResultDetail");
                        this.addChild(detailLayer, 100000);
                        detailLayer.updateContentView(data);
                    }
                }
                else {
                    this.showFinalMJResult(data);
                }

            }

            if (MJModel.isZhuanZhuan == false) {
                if (MJModel.seatid != -1) {
                    if (this.room_Action) this.room_Action.setReadyVisible(true);
                }
            }
        }

        if (MJModel.isTimeEnd) this.startTimeEnd();
    },

    showFinalMJResult: function (data) {
        var result = new MJResult();
        result.setName("RoomMJResultDetail");
        this.addChild(result, 100000);
        result.updateContentView(data);
    },

    handler_server_game_end_bc_maima: function (data, canDelay) {
        Log(this, "handler_server_game_end_bc_maima");
        var self = this;
        var ackGameEnd = parsePacket("proto.game.AckGameEnd", data);
        var card_map = [0, 0, 0, 0];
        var zhongma_pos_num = ackGameEnd.UserHorse.length;
        for (var i = 0; i < zhongma_pos_num; i++) {
            var seatid = ackGameEnd.UserHorse[i].chairID;
            var pos = MJModel.getPosBySeatid(seatid);
            var zhongma_nums = ackGameEnd.UserHorse[i].Num;
            for (var j = 0; j < zhongma_nums; j++) {
                var card = ackGameEnd.UserHorse[i].HorseCard[j];
                card_map[card] = pos;
            }
        }

        var nums = ackGameEnd.HorseInfo.HorseNum;
        var zhongmacardlist = [];
        var zhongmapos = [];
        for (var i = 0; i < nums; i++) {
            var card = ackGameEnd.HorseInfo.HorseCard[i];
            zhongmacardlist.push(card);
            zhongmapos.push(card_map[card]);
        }

        this.room_Card.showZhongMaEffect(zhongmacardlist, zhongmapos);

        var callback1 = cc.callFunc(function () {
            for (var i = 0; i < zhongmapos.length; i++) {
                var zm_pos = zhongmapos[i];
                self.room_Tip.showZhongMaTip(zm_pos, self.room_Card.isHaveHuaCard(zm_pos));
            }
        });

        var callback = cc.callFunc(function () {
            self.handler_server_game_end_bc_final(data);
        });
        this.runAction(cc.sequence(cc.delayTime(3.3), callback1, cc.delayTime(1), callback));
    },

    handler_maima_bc: function (data, canDelay) {
        MJModel.isEnterWait = true;
        var self = this;
        var ask_horse = parsePacket("proto.game.AckHorse", data);
        var seatid = ask_horse.chairID;
        var nums = ask_horse.HorseNum;
        var pos = MJModel.getPosBySeatid(seatid);
        this.room_Card.showMaiMaEffect(nums, pos);

        var callback = cc.callFunc.create(function () {
            var rp = self.players[pos];
            if (rp) rp.setMaiMa(nums);
            MJModel.isEnterWait = false;
        });
        this.runAction(cc.sequence(cc.delayTime(1.3), callback));
    },

    handler_server_end_room_req_fail_uc: function (data, canDelay) {
        var jpacket = parsePacket("proto.game.AckDisbandChooseFaild", data);
        var code = jpacket.code;
        if (code == 101) {
            PlatformHelper.showToast("每个玩家每局游戏只能申请两次解散房间！");
        }
        else if (code == 102) {
            PlatformHelper.showToast("只有桌子上的玩家可以申请解散房间！");
        }
        else if (code == 103) {
            PlatformHelper.showToast("当前有人正在申请解散房间！");
        }
    },
    handler_server_end_room_bc: function (data, canDelay) {
        var jpacket = parsePacket("proto.game.AckDisbandApply", data);
        if (MJModel.seatid == -1)return;

        var json = {};

        var owner_seatid = jpacket.ApplyChairid;
        var time = jpacket.Waittime;
        var player = MJModel.players[owner_seatid];

        json["ownerid"] = player.uid;
        json["ownername"] = player.name;
        json["time_left"] = time;
        json["players"] = [];
        for (var i = 0; i < jpacket.onlinechairid.length; i++) {
            var seatid = jpacket.onlinechairid[i];
            var p = MJModel.players[seatid];
            var d = {}
            d["uid"] = p.uid;
            d["name"] = p.name;
            d["avatar"] = p.avatar;
            d["state"] = 0;
            json["players"].push(d);
        }

        MJModel.TouPiaoING = true;

        this.room_EndTouPiao = RoomHEndTouPiao.create();
        this.room_EndTouPiao.initWithJsonStr(JSON.stringify(json));
        this.addChild(this.room_EndTouPiao, 10000000);

        this.room_EndTouPiao.setCallback(function (str) {
            if (str == "ok") {
                sendEndRoomResult(1);
            }
            else if (str == "no") {
                sendEndRoomResult(2);
            }
        });
    },
    handler_server_end_room_action_bc: function (data, canDelay) {
        var jpacket = parsePacket("proto.game.AckDisbandChooseBc", data);
        var seatid = jpacket.chairid;
        var action = jpacket.Choosestate;
        var p = MJModel.players[seatid];
        if (this.room_EndTouPiao) this.room_EndTouPiao.updateState(p.uid, action);
    },
    handler_server_end_room_result_bc: function (data, canDelay) {
        var jpacket = parsePacket("proto.game.AckDisbandResult", data);
        if (this.room_EndTouPiao) this.room_EndTouPiao.removeFromParent(true);
        this.room_EndTouPiao = null;
        MJModel.TouPiaoING = false;
        var result = jpacket.Result;
        if (result == 1) {
            PlatformHelper.showToast("房间解散成功，3秒后房间自动结束");
        }
        else {
            PlatformHelper.showToast("解散失败，稍后游戏继续！");
        }
    },

    handler_server_player_offline_bc: function (data, canDelay) {
        var jpacket = parsePacket("proto.game.AckUserOnlineState", data);
        var seatid = jpacket.chairid;
        var state = jpacket.state;
        var pos = MJModel.getPosBySeatid(seatid);
        this.players[pos].setOffline(state == 1);
    },
    handler_server_player_timeout_bc: function (data, canDelay) {
        var jpacket = parsePacket("proto.game.AckUserOvertimeState", data);
        var seatid = jpacket.chairid;
        var state = jpacket.state;

        if (state == 1) {
            var pos = MJModel.getPosBySeatid(seatid);
            if (this.players[pos])this.players[pos].setTimeOut(true);
        }
        else if (state == 2) {
            this.room_Tip.showTextTip("等待玩家操作");
        }
    },
    //贵州麻将 start
    handler_server_baoting_bc: function (data, canDelay) {
        var jpacket = parsePacket("proto.game.guizhou.AckBaoTingSucc", data);
        var seatid = jpacket.ChairID;
        var pos = MJModel.getPosBySeatid(seatid);
        this.room_Tip.ShowBaoTing(pos);

        if (seatid == MJModel.seatid) {
            this.room_Card.setBaoTingCardClickEnable(false, false);
            MJModel.isBaoting = true;
            MJModel.canTouchCard = true;
            MJModel.baoting_test = 4;
        }
        var player = MJModel.players[seatid];
        if (player) {
            Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/gzsound/operator/gz_%1_jiaozui.mp3"), player.sex == SEX_ID.SEX_FEMALE ? "w" : "m"));
        }
    },
    handler_server_get_ji_pai: function (data, canDelay) {
        var ackgetjipai = parsePacket("proto.game.guizhou.AckGetJiPai", data);

        //获取每个人手牌
        var usercard_size = ackgetjipai.UserCard.length;

        var cardData = [];
        for (var i = 0; i < usercard_size; i++) {
            //结束玩家手牌数组
            var changeable_Size = ackgetjipai.UserCard[i].ChangeableCards.length;
            cardData = [];

            for (var j = 0; j < changeable_Size; j++) {
                var cardDataNum = ackgetjipai.UserCard[i].ChangeableCards[j];
                cardData.push(cardDataNum);
            }

            var pos = MJModel.getPosBySeatid(i);

            this.room_Card.showEndCard(pos, cardData, 0, false);

        }
        var JiPaicardList = [];
        MJModel.JiPaicardList = [];
        var jipai_size = ackgetjipai.getJiPai.length;
        for (var i = 0; i < jipai_size; i++) {
            var value = ackgetjipai.getJiPai[i].Jipai;
            JiPaicardList.push(value);
            MJModel.JiPaicardList.push(value);
        }
        this.room_Tip.ShowJiPai(JiPaicardList);
        var IsManTangJi = ackgetjipai.IsManTangJi;
        for (var i = 0; i < 4; i++) {
            this.room_Card.showAllJiPaiFlag(i, JiPaicardList, IsManTangJi);
        }
    },
    //贵州麻将 end
    handler_server_uptable_info_uc: function (data, canDelay) {
        if (MJModel.isZhuanZhuan) {
            if (this.room_Action) {
                this.room_Action.setDuiLieData(data);
            }
        }
    },

    //四川麻将换牌
    handler_server_change_card: function (data, canDelay) {
        if (MJModel.seatid != -1) {
            //在座位上
            this.mChangeBar.setVisible(false);
            MJModel.canChoiceMore = false;
            MJModel.canTouchCard = false;

            var changeCard = parsePacket("proto.game.AckChangeCard", data);
            this.room_Card.setUp(false);

            if (changeCard.ischange) {

                for (var i = 0; i < changeCard.oldCard.length; i++) {
                    cc.log("oldCard:" + changeCard.oldCard[i]);
                    this.room_Card.removeHandCard(0, changeCard.oldCard[i]);
                }

                for (var i = 0; i < changeCard.Card.length; i++) {
                    this.room_Card.addHandCard(0, changeCard.Card[i]);
                }
                this.room_Card.reflashHandCard(0, true);
            }

            if (canDelay && MJModel.mj_roomType == 0) {
                this.room_Info.showTime(MJModel.lackTime);
                this.mPaiSeBar.setGoodPaiSe(this.room_Card.getGoodPaiSe());
                this.mPaiSeBar.setVisible(true);
            }
        }
    },

    //四川麻将定花色
    handler_server_sure_lack_type: function (data, canDelay) {
        this.mPaiSeBar.setVisible(false);
        var lackType = parsePacket("proto.game.AckUserLackType", data);
        for (var i = 0; i < lackType.LackType.length; i++) {
            var lack = lackType.LackType[i];
            var pos = MJModel.getPosBySeatid(i);
            this.players[pos].setLackType(lack);

            if (MJModel.seatid == i) {
                MJModel.lackType = lack;
                this.room_Card.sureLackType();
                this.room_Card.reflashHandCard(0);
            }
        }
    },

    handler_server_begin_zhuafeng: function (data, canDelay) {
        var self = this;
        this.resetGame();
        MJModel.state = MJ_STATUS.MJ_GAME_PLAYING;
        var jpacket = parsePacket("proto.game.gdmahjonggame.AckBeginQuanFeng", data);
        this.room_Info.setVisible(true);
        MJModel.isEnterWait = true;
        var new_seat_list = [];
        for (var i = 0; i < jpacket.QuanFengCard.length; i++) {
            new_seat_list.push(jpacket.QuanFengCard[i] - 49);
        }

        if (canDelay) {
            this.room_Card.showZhuaFengEffect(new_seat_list);

            var cb = cc.callFunc(function () {
                self.gdmj_exch_feng_wei_effect(new_seat_list, canDelay);
            });
            this.runAction(cc.sequence(cc.delayTime(4.3), cb));
        }
        else {
            this.gdmj_exch_feng_wei_effect(new_seat_list, canDelay);
        }

        JsUtils.postNotifi("notify_game_check_end");//为了刷新局数 或者  开始倒计时
    },

    //鬼牌
    handler_server_get_guipai: function (data, canDelay) {
        var jpacket = parsePacket("proto.game.AckEnSureJoker", data);
        var opencard = jpacket.OpenCard;
        MJModel.valueGuiPai = jpacket.CurJokerCard;
        //如果翻牌不等于255，则显示翻牌动作，否则仅显示左侧鬼牌显示。
        if (opencard != 255) {
            this.room_Tip.showGuiPai(opencard);
            this.room_Tip.showLeftGuiPaiBg();
            this.room_Card.reflashHandCard(0, true);
            this.room_Card.setGuipai();
        } else {
            this.room_Tip.showLeftGuiPaiBg();
            this.room_Tip.isShowGuiPai(1);
            this.room_Card.reflashHandCard(0, true);
            this.room_Card.setGuipai();
        }
    },

    handler_server_get_chaozhuang: function (data, canDelay) {
        var jpacket = parsePacket("proto.game.gdmahjonggame.AckCopyBankerReack", data);
        var isCopyBanker = jpacket.CopyBanker;
        if (isCopyBanker == 1) {
            //抄庄达成
            this.room_Tip.showChaoZhuang();
        }
    },


    handler_server_gdmj_game_start3: function (data, canDelay) {
        var firstHandCard = parsePacket("proto.game.FirstFlowerHandCard", data);
        if (firstHandCard.UserCard) {
            var allcards = [];
            for (var i = 0; i < firstHandCard.UserCard.ChangeableCards.length; i++) {
                allcards.push(firstHandCard.UserCard.ChangeableCards[i]);
            }

            this.room_Card.initHandCard(0, allcards);
            this.room_Card.reflashHandCard(0, true);
        }

        for (var i = 0; i < firstHandCard.HuaPai.length; i++) {
            var pos = MJModel.getPosBySeatid(i);
            // this.players[pos].setHuaPaiNum(firstHandCard.HuaPai[i].HuaCount);
            if (firstHandCard.HuaPai[i].HuaCount > 0) {
                var player = MJModel.players[i];
                Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/hnmj/operator/%1_buhua.mp3"), player.sex == SEX_ID.SEX_FEMALE ? "female" : "male"));
            }
            for (var j = 0; j < firstHandCard.HuaPai[i].HuaCount; j++) {
                this.room_Card.addHuaCard(pos, firstHandCard.HuaPai[i].HuaCard[j]);
            }


        }

    },

    handler_server_gdmj_get_huapai: function (data, canDelay) {
        var firstHandCard = parsePacket("proto.game.GetHuaPai", data);
        MJModel.isEnterWait = true;
        var seatid = firstHandCard.chairID;
        var pos = MJModel.getPosBySeatid(seatid);
        var card = firstHandCard.HuaCard;
        this.room_Card.getHuaCard(pos, card);

        var cardNums = firstHandCard.LeftCardNum;
        this.room_Info.showCardNums(cardNums);

        // this.players[pos].setHuaPaiNum(1);
        var player = MJModel.players[seatid];
        Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/hnmj/operator/%1_buhua.mp3"), player.sex == SEX_ID.SEX_FEMALE ? "female" : "male"));

    },


    handler_server_get_msg: function (data, canDelay) {
        var jpacket = parsePacket("proto.game.AckMessage", data);
        var msg = jpacket.message;
        this.room_Tip.showMyTip(msg);
    },

    actionCallBack: function (name) {
        Log(this, "actionCallBack[" + name + "]");
        if (name == "uptable") {
            var hasNULL = false;
            for (var i = 0; i < 4; i++) {
                if (MJModel.players[i].uid <= 0) {
                    hasNULL = true;
                    break;
                }
            }

            if (hasNULL) {
                sendUpTable();
            }
            else {
                this.room_Tip.showMyTip("桌上人已经满了");
            }
        }
        else if (name == "downtable") {
            sendDownTable();
        }
        else if (name == "weiguan") {
        }
        else if (name == "ready") {
            sendReady();
        }
        else if (name == "auto_out_card") {
            if (this.isScmjRoom()) {
                if (MJModel.isAutoOutCard && this.room_Card.isAlreadyHu(0)) {
                    return;
                }
            }

            MJModel.isAutoOutCard = !MJModel.isAutoOutCard;
            if (this.room_Action) this.room_Action.setAutoOutCardVis(true);
            this.room_Card.setAutoOutCard(MJModel.isAutoOutCard);
            if (MJModel.isMyPlayer(MJModel.cur_seat)) {
                if (MJModel.isAutoOutCard) {
                    this.autoOutCard();
                }
                else {
                    this.myselfOpenOutCard();
                }
            }
        }
    },

    showBigfaceMenu: function (pos) {
        var menu = new BigFaceMenu();
        this.addChild(menu, 100000);

        menu.setPosition(MJConfig.getPlayerPos(pos));
        var rotation = 0;
        if (pos == 1) rotation = 180;
        if (pos == 2) rotation = 270;
        menu.show(rotation);
        var self = this;
        menu.setCallback(function (tag) {
                var faceid = tag + 1;
                if (faceid == 7) {
                    self.roomPublic.showPlayerList();
                }
                else {
                    sendBigface(MJModel.seatid, MJModel.getSeatidByPos(pos), faceid);
                }
            }
        )
        ;
    },

    playerClickCallback: function (pos) {
        var seatid = MJModel.getSeatidByPos(pos);
        if (seatid == MJModel.seatid) {
            this.roomPublic.showPlayerList();

        }
        else {
            if (MJModel.seatid == -1) {
                var player = MJModel.players[MJModel.getSeatidByPos(pos)];
                var userInfo = new MJUserInfo();
                userInfo.init(player);
                this.addChild(userInfo, 1000000);
            }
            else {
                this.showBigfaceMenu(pos);
            }
        }
    },

    syncPlayerInfo: function () {
    },

    faPaiOver: function (dt) {
    },
    nextUserEnterBet: function () {
    },
    netError: function (index) {
        if (index == 1) {
            CCTcpClient.getInstance(MJModel.curRoomID).reset_response_queue();
            CCTcpClient.getInstance(MJModel.curRoomID).connect();
            MJModel.reLogIn = true;
        }
        else {
            //        Director.getInstance().replaceScene(GameHall.scene());
        }
    },
    resetGame: function () {
        MJModel.preGame();
        MJModel.hasCardOutIng = false;
        MJModel.valueGuiPai = 0;
        cc.log("guipai-test---------222value---:" + MJModel.valueGuiPai);
        this.room_Tip.hideAllTip();
        this.room_Tip.resetRemove();

        if (this.room_Action) this.room_Action.setTingPaiVis(false);
        if (this.room_Action) this.room_Action.setAutoOutCardVis(false);

        if (this.room_Action) this.room_Action.setReadyVisible(false);

        if (this.room_Action) this.room_Action.setOperator(-1);

        this.unschedule(this.autoOutCardTimeOut);
        this.unschedule(this.autoHuCardTimeOut);

        this.room_Card.reset();
        this.room_Info.gdmj_showFengQuan(-1);
        this.room_Info.showDirectionTip(-1);
        this.room_Info.showCardNums(0);
        this.room_Info.stopTime();
        for (var i = 0; i < 4; i++) {
            var player = this.players[i];
            player.reset();
            var model = MJModel.players[i];
            model.ready = 0;
        }

        if (this.room_EndTouPiao) {
            this.room_EndTouPiao.removeFromParent(true);
            this.room_EndTouPiao = null;
        }

        // ccs.armatureDataManager.clear();
        // cc.spriteFrameCache.removeUnusedSpriteFrames();
        // cc.textureCache.removeUnusedTextures();
    },

    gameHeartBeat: function (dt) {
        sendHeartbeat();
    },//心跳
    notificationCallback: function (msg) {
    },
    onExit: function () {
        this._super();
        cc.director.purgeCachedData();
    },

    GameTimeEndNotify: function (ref) {
        this.startTimeEnd();
    },
    startTimeEnd: function () {
        this.schedule(this.timeEnd, 1.0);
    },
    stopTimeEnd: function () {
        this.unschedule(this.timeEnd);
    },
    timeEnd: function (dt) {

        if (MJModel.isEnd)return;

        if (MJModel.isState(MJ_STATUS.MJ_GAME_FREE)) {
            ZJHModel.getInstance().roomToShareView(MJModel.curRoomID);
        }

    },

    getPlayerList: function () {
    },

    myselfOpenOutCard: function (IsMoCard) {
        if (IsMoCard == undefined) IsMoCard = false;
        if (MJModel.isOnVideo) {
            return;
        }

        if (MJModel.isAutoOutCard && IsMoCard) {
            this.autoOutCard();
            return;
        }
        this.room_Card.resetAllCardClickState();
        MJModel.canTouchCard = true;
        if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ || this.isScmjRoom()) {
            if (MJModel.isBaoting) {
                this.room_Card.setBaoTingCardClickEnable(false, IsMoCard);
                this.room_Card.setTingPaiFlag(false);
            }
            else {
                this.room_Card.setBaoTingCardClickEnable(true);
                this.room_Card.setTingPaiFlag(true);
                if (this.isScmjRoom())this.room_Card.openHuaZhuClickOnly();
            }
        }
        else {
            this.room_Card.setTingPaiFlag(true);
        }

        if (this.room_Action) this.room_Action.setTingPaiVis(false);

        if (MJModel.isOutCard == false) {
            this.room_Tip.showOutCardEffect(true);
        }
    },
    myselfCloseOutCard: function () {
        MJModel.canTouchCard = false;
        // this.room_Card.reflashHandCard(0, true);

        if (this.room_Action) this.room_Action.checkTingPaiVis();
        if (this.room_Action) this.room_Action.setOperator(-1);
        this.room_Card.setTingPaiFlag(false);
        this.room_Tip.showOutCardEffect(false);
        if (this.isScmjRoom())this.room_Card.closeHuaZhuClickOnly();
    },

    autoOutCard: function () {
        this.schedule(this.autoOutCardTimeOut, 3.0);
    },
    autoOutCardTimeOut: function (dt) {
        this.unschedule(this.autoOutCardTimeOut);
        if (MJModel.isAutoOutCard == false)return;
        if (MJModel.TouPiaoING == true) return;//投票期间 该功能不起作用
        sendOutCard(MJModel.curOperateCard);
    },
    autoHuCard: function () {
        this.schedule(this.autoHuCardTimeOut, 2.0);
    },
    autoHuCardTimeOut: function (dt) {
        this.unschedule(this.autoHuCardTimeOut);
        if (MJModel.isAutoOutCard == false)return;
        if (MJModel.TouPiaoING == true) return;//投票期间 该功能不起作用
        var operate = MJModel.curOperate;
        if (( operate & ActionType.TYPE_LISTEN) == ActionType.TYPE_LISTEN) {
            operate = operate - ActionType.TYPE_LISTEN;
        }

        if (operate > 0) {
            if ((operate & ActionType.TYPE_ZHIGANG) == ActionType.TYPE_ZHIGANG || (operate & ActionType.TYPE_WANGANG) == ActionType.TYPE_WANGANG || (operate & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG) {
                if (this.isScmjRoom() && this.room_Card.isAlreadyHu(0)) {
                    //在四川麻将中有杠，但用户已经胡过了则不能取消自动胡牌
                    return;
                }
                //有杠  取消自动胡牌功能
                MJModel.isAutoOutCard = false;
                if (this.room_Action) this.room_Action.setAutoOutCardVis(false);
                this.room_Card.setAutoOutCard(false);
            }
            else {
                var type = ActionType.TYPE_PASS;
                if ((operate & ActionType.TYPE_HU) == ActionType.TYPE_HU) {
                    type = ActionType.TYPE_HU;
                }

                sendOperateCard(type, 0);
                if (this.room_Action) this.room_Action.setOperator(-1);
            }
        }
    },

    //转转麻将倒计时
    startZhuanClock: function () {
        this.schedule(this.zhuanClock, 1.0);
    },
    stopZhuanClock: function () {
        this.unschedule(this.zhuanClock);
        for (var i = 0; i < 4; i++) {
            var player = this.players[i];
            player.removeZhuanClock();
        }
    },
    zhuanClock: function (dt) {
        if (MJModel.isState(MJ_STATUS.END_GAME)) {
            this.stopZhuanClock();
        }
        for (var i = 0; i < 4; i++) {
            var seatid = MJModel.getSeatidByPos(i);
            var player = MJModel.players[seatid];
            if (player) {
                player.zhuan_surplustime -= dt;
                var playerShow = this.players[i];
                if (playerShow) {
                    playerShow.setZhuanClock();
                }
            }
        }
    },

    //换牌点击事件回调
    onChangeClick: function (ref) {
        var name = ref.getName();

        if (name == "sureChoice") {
            var upCards = this.room_Card.getChoiceCards();
            sendChangeCard(1, upCards);
        }
        else if (name == "cancelChoice") {
            sendChangeCard(0, null);
        }

        this.mChangeBar.setVisible(false);
        MJModel.canChoiceMore = false;
        MJModel.canTouchCard = false;
    },

    //定花色点击事件回调
    onPaiSeClick: function (ref) {
        var name = ref.getName();

        var type;
        if (name == "tiaoChoice") {
            type = MJ_TYPE.MJ_TYPE_TIAO;
        } else if (name == "tongChoice") {
            type = MJ_TYPE.MJ_TYPE_TONG;
        } else if (name == "wanChoice") {
            type = MJ_TYPE.MJ_TYPE_WAN;
        }
        sendLackCard(type);
        this.mPaiSeBar.setVisible(false);

    },

    //选飘事件回调
    onPiaoClick: function (ref) {
        var name = ref.getName();
        var nums = 0;
        if (this.isScmjRoom()) {
            this.mPiaoBar.setVisible(false);
            if (name == "piao1") {
                nums = 1;
            } else if (name == "piao2") {
                nums = 2;
            } else if (name == "piao5") {
                nums = 5;
            } else if (name == "noPiao") {
                nums = 0;
            }
            sendPiaoNums(nums);
        }

    },

    //该房间是否为四川麻将房
    isScmjRoom: function () {
        return MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_YNMJ_DZ;
    },

    //四川麻将胡牌后自动出牌
    scmjAutoPlay: function () {
        MJModel.isAutoOutCard = true;
        if (this.room_Action) this.room_Action.setAutoOutCardVis(true);
        this.room_Card.setAutoOutCard(true);

        if (this.room_Card.isNeedOutCard(0)) {
            this.autoOutCard();
        }
    },

    //四川麻将相关测试代码
    testSC: function () {
        MJModel.seatid = 0;

        var outCards = [0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
            0x11, 0x12, 0x13, 0x14, 0x15, 0x26, 0x27, 0x28, 0x29, 0x21,
            0x22, 0x28, 0x29, 0x28, 0x29];

        var huCards = [0x21, 0x22, 0x23, 0x24, 0x12, 0x15, 0x01, 0x23, 0x24, 0x12, 0x15, 0x01];

        var cards = [0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x01, 0x23, 0x24, 0x12, 0x24, 0x12];

        var huaCards = [0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58];

        for (var i = 0; i < 4; i++) {
            this.players[i].seatid = i;
            this.players[i].setLackType(1);
            this.players[i].setPiaoType(5);

            // this.room_Tip.showHuTip(i);
            // this.room_Card.initHuCard(i , huCards);

            if (i == 0) {
                this.room_Card.initHandCard(0, cards);
                this.room_Card.reflashHandCard(0);
                this.room_Card.initMoCard(i, 0x05, true);
            } else {
                var cardList = [];
                for (var m = 0; m < 1; m++) {
                    cardList.push(0);
                }


                this.room_Card.initHandCard(i, cardList);
                this.room_Card.reflashHandCard(i);
                this.room_Card.initMoCard(i, 0, true);


            }
            // this.room_Card.initHuaCard(i , huaCards);
            this.room_Tip.showZhongMaTip(i, this.room_Card.isHaveHuaCard(i));
            this.room_Card.initZhongMaCard(i, huaCards);
            this.room_Card.addPengCard(i, PengType.Peng_Peng, 0x01);
            this.room_Card.addPengCard(i, PengType.Peng_Peng, 0x01);
            this.room_Card.addPengCard(i, PengType.Peng_Peng, 0x01);
            this.room_Card.addPengCard(i, PengType.Peng_Peng, 0x01);
            this.room_Card.initOutCard(i, outCards);


        }
    },

    gdmj_exch_feng_wei_effect: function (new_seat_list, canDelay) {
        var self = this;
        var temp_players = [];
        for (var i = 0; i < 4; i++) {
            temp_players[i] = MJModel.players[i];
        }

        var my_temp_seatid = -1;
        for (var i = 0; i < 4; i++) {
            var cur_seatid = i;
            var new_seatid = new_seat_list[i];
            MJModel.players[new_seatid] = temp_players[cur_seatid];

            if (my_temp_seatid == -1 && MJModel.seatid == cur_seatid) {
                my_temp_seatid = MJModel.seatid;
                MJModel.seatid = new_seatid;
            }
        }

        if (canDelay) {
            for (var i = 0; i < 4; i++) {
                var cur_seatid = i;
                var new_seatid = new_seat_list[i];
                if (new_seatid == MJModel.seatid)continue;

                var cur_pos = MJModel.getTempPosBySeatid(cur_seatid, my_temp_seatid);
                var new_pos = MJModel.getPosBySeatid(new_seatid);
                if (cur_pos == new_pos)continue;

                var p = this.players[cur_pos];
                p.runAction(cc.moveTo(0.5, MJConfig.getPlayerPos(new_pos)));
            }

            var cb = cc.callFunc(function () {

                self.room_Info.showDirectionTip(-1);
                for (var i = 0; i < 4; i++) {
                    var pos = MJModel.getPosBySeatid(i);
                    var p = self.players[pos];
                    p.login(i);
                    p.setFengWei(i);
                }
                MJModel.isEnterWait = false;
            });

            this.runAction(cc.sequence(cc.delayTime(1.3), cb));
        }
        else {
            this.room_Info.showDirectionTip(-1);
            for (var i = 0; i < 4; i++) {
                if (i == MJModel.seatid)continue;
                var pos = MJModel.getPosBySeatid(i);
                var p = this.players[pos];
                p.login(i);
                p.setFengWei(i);
            }
            MJModel.isEnterWait = false;
        }
    },
});
