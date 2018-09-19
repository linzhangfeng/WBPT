var ZJHTable = cc.Layer.extend({
    tag: "ZJHTable",
    Z_ORDER_TOP: 3000000,//显示在最顶层的元素的ZOrder

    READY: 0,
    START_BET: 1,
    STOP_BET: 2,
    BETTING: 3,
    END_GAME: 4,


    Perplayers: [[], [], [], [], [], [], []],
    players: [[], [], [], [], [], [], []],
    Upplayers: [[], [], []],
    room_Chip: null,
    room_Info: null,
    room_Card: null,
    room_Tip: null,
    room_Action: null,
    up_table_Info_million: null,
    endJpacket: null,
    poolInfoJpacket: null,
    AllPlayersJpacket: null,
    m_roomJsonData: null,

    m_showGameEnd: false,
    hasShowCards: [[], [], [], [], []],
    roomVerticalLayout: null,
    room_EndTouPiao: null,
    bg: null,
    listenerGameOver: null,

    ctor: function () {
        this._super();

        if (ZJHModel.getInstance().musicOn == 1) {
            Sound.getInstance().PlayBgSound(getResPath("Sound/zjh/game_bgm.mp3"));
        }

        RoomZJHInstance.init();
        RoomZJHInstance.isOnTV = 0;

        var size = cc.director.getWinSize();

        this.bg = new cc.Sprite(getResPath("Room/room_bg.png"));
        this.bg.setAnchorPoint(cc.p(0.5, 0.5));
        this.bg.setPosition(size.width / 2, size.height / 2);
        this.bg.setScale(size.width / this.bg.getContentSize().width, size.height / this.bg.getContentSize().height);
        this.addChild(this.bg, -100);

        if (RoomZJHInstance.curGameType == 0) {

        } else {
            var table_bg = new cc.Sprite(getResPath("Room/table.png"));
            table_bg.setAnchorPoint(cc.p(0.5, 0.5));
            table_bg.setPosition(cc.p(RoomZJHConfig.getTablePos().x, 1280 + RoomZJHConfig.getTablePos().y));
            this.addChild(table_bg, -1);

            var move = cc.moveTo(0.3, cc.p(size.width/2, size.height/2 + 15));
            var self = this;
            table_bg.runAction(cc.sequence(move, cc.callFunc(function () {
                // self.settable = self;
                // self.addChild(this.settable, 4);
                self.init();
                // RoomZJHSetAction* action = RoomZJHSetAction::create();
                // this.settable.setAction(action);
            }) , cc.removeSelf()));

        }

    },

    init: function () {
        RoomZJHInstance.pauseCount = 0;
        this.m_roomJsonData = RoomZJHInstance.roomData;
        this.m_showGameEnd = false;
        this.room_EndTouPiao = null;

        RoomZJHInstance.reset();
        RoomZJHInstance.curRoomID = ZJHModel.getInstance().curRoomID;
        var RoomId = RoomZJHInstance.curRoomID;
        var size = cc.director.getWinSize();
        var table = new cc.Sprite(getResPath("Room/table.png"));
        table.setAnchorPoint(cc.p(0.5, 0.5));
        table.setPosition(RoomZJHConfig.getTablePos());
        this.addChild(table, -1);

        RoomZJHInstance.testCount = 0;
        RoomZJHInstance.callAll = false;
        RoomZJHInstance.isLogOut = false;
        RoomZJHInstance.reLogIn = false;
        RoomZJHInstance.isEnterWait = false;

        this.schedule(this.updateGame, 0.0);
        this.schedule(this.gameHeartBeat, 3.0);//心跳
        this.initRoom();
        this.initRaise();

        var self = this;
        this.listenerGameOver = cc.EventListener.create({
            event: cc.EventListener.CUSTOM,
            eventName: "notify_against_room_end",
            callback: function (evt) {
                cc.log("notify_against_room_end");
                self.notificationGameEnd(null);
            }
        });
        cc.eventManager.addListener(this.listenerGameOver, this);

        this.roomVerticalLayout = RoomVerticalLayout.create();
        this.roomVerticalLayout.setCallback(function (tag) {
            if (tag == "seatBtn") {
                ZJHLogic.sendUpTable();
            }
            else if (tag == "outSeatBtn") {
                if (RoomZJHInstance.seatid != -1) {
                    ZJHLogic.sendDownTable();
                } else {
                    PlatformHelper.showToast("您已经离桌");
                }

            }
            else if (tag == "exit") {
                RoomZJHInstance.isExit = true;
                ZJHLogic.sendDownTable();
                ZJHLogic.sendLogout();
            }
            else if (tag == "end_room") {
                ZJHLogic.sendDissolveRoom();
            }
        });
        this.addChild(this.roomVerticalLayout, 10001);
    },

    setAction: function (action) {
        this.room_Action = action;
        this.addChild(this.room_Action, 40);
        if (this.room_Action != null) {
            this.room_Action.setCallback(this.actionCallBack, this);
        }
    },


    setInfo: function (info) {
        this.room_Info = info;
        this.addChild(this.room_Info, 10);
    },

    initRoom: function () {
        var size = cc.director.getWinSize();

        ///目前各层级顺序 tip:50    action:40    chip:30    card:20    compare:120
        var chip = new cc.Layer();
        chip.setTag(12);
        this.addChild(chip, 30);

        this.room_Chip = new RoomZJHChip();
        this.room_Chip.setPosition(cc.p(size.width / 2, size.height / 2));
        chip.addChild(this.room_Chip);
        this.room_Info = null;

        var compareLayer = new cc.Layer();
        compareLayer.setName("compare_layer");
        this.addChild(compareLayer, 100);

        this.room_Action = null;
        //桌上玩家初始化
        for (var i = 0; i < 5; i++) {
            var player = new RoomZJHPlayer();
            player.setId(i);
            var player1 = RoomZJHInstance.players[i];
            player1.lifeNums = 5;
            player.reset();
            player.setPosition(RoomZJHConfig.getPlayerPos(i));
            player.setVisible(false);
            this.addChild(player, 0);
            this.players[i] = player;
            player.setClickCallback(this.playerClickCallback, this);
            if (i == 0) {
                player.setCallback(this.timeoutCallback, this);
            }
        }

        var card = new cc.Layer();
        card.setTag(13);
        this.addChild(card, 20);

        this.room_Card = new RoomZJHCard();
        this.room_Card.setPosition(cc.p(size.width / 2, size.height / 2));
        card.addChild(this.room_Card);

        this.room_Tip = new RoomZJHTip();
        this.room_Tip.setPosition(cc.p(size.width / 2, size.height / 2));
        this.addChild(this.room_Tip, 50);
    },

    updateGame: function (dt) {
        if (ZJHModel.getInstance().gamePuase) {
            if (this.room_Action != null)
                this.room_Action.setButtonflag(false);

            RoomZJHInstance.pauseCount++;
            if (RoomZJHInstance.pauseCount >= 60) {
                RoomZJHInstance.pauseCount = 0;
                ZJHModel.getInstance().gamePuase = false;

                var pausetime = ZJHModel.getInstance().pauseTime;
                var curTime = time();
                if ((curTime - pausetime) <= 15) {
                    return;
                }

                RoomZJHInstance.waitUpdate = true;

                ZJHLogic.resetResponse();
                ZJHLogic.sendRoomInfo();
            }
            cc.log("ZJHModel.getInstance().gamePuase= " + ZJHModel.getInstance().gamePuase);
            return;
        }

        var curTime = time(null);
        if (RoomZJHInstance.lastHeartBeatTime != -1 && curTime - RoomZJHInstance.lastHeartBeatTime > 10 * 1000) {
            if (this.room_Action)
                this.room_Action.setButtonflag(false);

            ZJHLogic.resetResponse();
            ZJHLogic.connectNet();

            if (this.room_Action != null)
                this.room_Action.setButtonflag(false);

            RoomZJHInstance.lastHeartBeatTime = time();

            PlatformHelper.showToast("网络连接异常，正在重试");
            cc.log("curTime: " + curTime + "; lastHeartBeatTime: " +RoomZJHInstance.lastHeartBeatTime );
            return;
        }

        if (RoomZJHInstance.isEnterWait) {
            cc.log("RoomZJHInstance.isEnterWait= " + RoomZJHInstance.isEnterWait);
            return;
        }

        var response = ZJHLogic.getNetData();
        if (response != null) {
            cc.log(__String.createWithFormat("RoomZJHInstance::getInstance()Room cmd %1 data %2", response.cmd, response.data));
            if (response.cmd == TCPCMD.DISCONNECT_RES || response.cmd == TCPCMD.CONNECT_ERROR_RES
                || response.cmd == TCPCMD.SEND_DATA_ERROR_RES || response.cmd == TCPCMD.RECV_DATA_ERROR_RES) //关闭
            {
                if (!RoomZJHInstance.isLogOut) {
                    RoomZJHInstance.roomConnectFail++;
                    this.netError(1);
                    if (RoomZJHInstance.roomConnectFail == 60) {
                        PlatformHelper.showToast("网络连接异常，正在重试");
                        cc.log("RoomZJHInstance.roomConnectFail=" + RoomZJHInstance.roomConnectFail);
                    }
                }
            }
            else if (response.cmd == TCPCMD.CONNECT_OK_RES) {
                ZJHLogic.sendLogin();
            }
            else if (response.cmd == TCPCMD.RECV_DATA_OK_RES) {
                RoomZJHInstance.roomConnectFail = 0;
                var d = response.parseDatta();
                this.handler_cmd(d);
            }
            response.release();
        } else {
            //cc.log("have not message!");
        }
    },


    netError: function (index) {
        if (index == 1) {
            ZJHLogic.resetResponse();
            ZJHLogic.connectNet();
            RoomZJHInstance.reLogIn = true;
        }
        else {
//        Director::getInstance()->replaceScene(GameHall::scene());
        }
    },

    resetPlayer: function () {
        var flag = false;
        for (var i = 0; i < 5; i++) {
            var RoomZJHPlayer = this.players[i];
            var player = RoomZJHInstance.players[RoomZJHInstance.getSeatidByPos(i)];
            if (player != null) {
                player.bet = 0;
                player.jinbiRound = 0;
                player.fanbeiRound = 0;
                player.fanbeiNums = 0;
                player.huanpaiTimes = 0;
                player.myBetNums = 0;
                player.allBetMoney = 0;
            }
            RoomZJHPlayer.stopTimer();
            RoomZJHPlayer.updateInfo();
            if (player.ready == 1) {
                RoomZJHPlayer.setMaskVisible(false);
                flag = true;
            } else {
                RoomZJHPlayer.setMaskVisible(true);
            }

        }

        if (RoomZJHInstance.state == this.READY && flag) {
            this.room_Action.hideLaTime();
        }
    },

    stopAllTimer: function () {
        for (var i = 0; i < 5; i++) {
            var player = this.players[i];
            player.stopTimer();
        }
    },

    handler_cmd: function (data) {
        var val;
        try {
            val = JSON.parse(data);
        }
        catch (e) {
            cc.log("_error__data:" + data);
            return;
        }
        var cmd = val.cmd;
        cc.log("_r__cmd:" + cmd);
        cc.log("_r__data:" + data);
        if (RoomZJHInstance.isExit) {
            return 0;
        }
        if (cmd == NetCMD.SERVER_DZ_LOGIN_ERR_RES) {
            ZJHModel.getInstance().mySeatid = -1;
            ZJHModel.getInstance().roomToShareView(RoomZJHInstance.curRoomID);
        }
        cc.log(__String.createWithFormat("=================cmd===%1=============", cmd));
        if (RoomZJHInstance.waitUpdate && cmd != NetCMD.SERVER_TABLE_INFO_UC) {
            return 0;
        }
        switch (cmd) {
            case NetCMD.SERVER_OFFLINE_BC:
                this.handler_server_offline_bc(val);
                break;
            case NetCMD.SERVER_DZ_LOGIN_SUCC_BC:
                this.handler_server_login_succ_bc(val);
                break;
            case NetCMD.SERVER_APPLY_DISSOLVE_ROOM_ERR_UC:
                this.handler_server_end_room_req_fail_uc(val);
                break;

            case NetCMD.SERVER_APPLY_DISSOLVE_ROOM_SUCC_BC:
                this.handler_server_end_room_bc(val);
                break;

            case NetCMD.SERVER_DISSOLVE_ACTION_SUCC_BC:
                this.handler_server_end_room_action_bc(val);
                break;

            case NetCMD.SERVER_DISSOLVE_ROOM_RESULT_BC:
                this.handler_server_end_room_result_bc(val);
                break;

            case NetCMD.SERVER_LOGIN_SUCC_UC:
                this.handler_server_login_succ_uc(val);
                break;

            case NetCMD.SERVER_LOGIN_SUCC_BC:
//            this.handler_server_login_succ_bc(val);
                break;

            case NetCMD.SERVER_LOGIN_ERR_UC:
                this.handler_server_login_err_uc(val);
                break;

            case NetCMD.SERVER_REBIND_UC:
                this.handler_server_rebind_uc(val);
                break;

            case NetCMD.SERVER_LOGOUT_SUCC_BC:
                this.handler_server_logout_succ_bc(val);
                break;

            case NetCMD.SERVER_LOGOUT_ERR_UC:
                this.handler_server_logout_err_uc(val);
                break;

            case NetCMD.SERVER_TABLE_INFO_UC:
                this.handler_server_table_info_uc(val);
                break;

            case NetCMD.SERVER_READY_SUCC_BC:
                this.handler_server_ready_succ_bc(val);
                break;

            case NetCMD.SERVER_READY_ERR_UC:
                this.handler_server_ready_err_uc(val);
                break;

            case NetCMD.SERVER_GAME_START_BC:
                this.handler_server_game_start_bc(val);
                break;
            case NetCMD.SERVER_START_BET_BC:
                this.handler_server_bet_start_bc(val);
                break;
            case NetCMD.SERVER_STOP_BET_BC:
                this.handler_server_bet_stop_bc(val);
                break;
            case NetCMD.SERVER_ACTIVE_UPDATE_BC:
                this.handler_server_all_bet_bc(val);
                break;
            case NetCMD.SERVER_NEXT_BET_BC:
                this.handler_server_next_bet_bc(val);
                break;

            case NetCMD.SERVER_BET_SUCC_BC:
                this.handler_server_bet_succ_bc(val);
                break;

            case NetCMD.SERVER_BET_SUCC_UC:
                this.handler_server_bet_succ_uc(val);
                break;

            case NetCMD.SERVER_BET_ERR_UC:
                this.handler_server_bet_err_uc(val);
                break;

            case NetCMD.SERVER_GAME_END_BC:
                this.handler_server_game_end_bc(val);
                break;

            case NetCMD.SERVER_GAME_PREREADY_BC:
                this.handler_server_game_preready_bc(val);
                break;

            case NetCMD.SERVER_CHAT_BC:
                this.handler_server_chat_bc(val);
                break;

            case NetCMD.SERVER_FACE_BC:
                this.handler_server_face_bc(val);
                break;

            case NetCMD.SERVER_EMOTION_BC:
                this.handler_server_bigface_bc(val);
                break;

            case NetCMD.SERVER_UPDATE_INFO_UC:
                this.handler_server_update_info_uc(val);
                break;

            case NetCMD.SERVER_PROP_SUCC_UC:
                this.handler_server_use_prop_uc(val);
                break;

            case NetCMD.SERVER_PROP_SUCC_BC:
                this.handler_server_use_prop_bc(val);
                break;

            case NetCMD.SERVER_BOX_SUCC_BC:
                this.handler_server_get_box_bc(val);
                break;

            case NetCMD.SERVER_BETTING_SUCC_UC:
                this.handler_server_my_bet_uc(val);
                break;

            case NetCMD.SERVER_WIN_UC:
                this.handler_server_win_uc(val);
                break;

            case NetCMD.SERVER_UPTABLE_SUCCC_BC:
                this.handler_server_up_table_bc(val);
                break;

            case NetCMD.SERVER_DOWNTABLE_SUCC_BC:
                this.handler_server_down_table_bc(val);
                break;
            case NetCMD.SERVER_UPTABLE_INFO_UC:
                this.handler_server_up_table_info_uc(val);
                break;

            case NetCMD.SERVER_DOWN_GUEST_SEAT_BC:
                this.handler_server_down_guest_seat_bc(val);
                break;

            case NetCMD.SERVER_UP_GUEST_SEAT_SUC_BC:
                this.handler_server_up_guest_seat_bc(val);
                break;

            case NetCMD.SERVER_UP_GUEST_SEAT_ERROR_UC:
                this.handler_server_up_guest_seat_error_uc(val);
                break;

            case NetCMD.SERVER_DETAL_INFO_UC:
                this.handler_server_up_get_PerPlayer_info(val);
                break;

            case NetCMD.SERVER_SHORT_UPTABLE_INFO_BC:
                this.handler_server_short_uptable_info(val);
                break;

            case NetCMD.SERVER_AHEAD_DOWNTABLE_UC:
                this.handler_server_request_down_table_uc(val);
                break;

            case NetCMD.SERVER_COMMUNITY_BC:
                this.handler_server_second_fapai_bc(val);
                break;

            case NetCMD.SERVER_TWO_COMMUNITY_UC:
                this.handler_server_second_fapai_uc(val);
                break;

            case NetCMD.SERVER_SECOND_CARD:
                this.handler_server_second_card_bc(val);
                break;

            case NetCMD.SERVER_THIRD_CARD:
                this.handler_server_third_card_bc(val);
                break;

            case NetCMD.SERVER_POOL_INFO_UC:
                this.handler_server_pool_info_uc(val);
                break;

            case NetCMD.SERVER_ALL_PLAYER_INFO_UC:
                this.handler_server_all_player_info(val);
                break;

            case NetCMD.SERVER_DZ_GAME_START_RES:
                this.handler_server_dz_game_start_res(val);
                break;

            case NetCMD.SERVER_ROUND_END:
                this.handler_server_round_end(val);
                break;

            case NetCMD.SERVER_OTHER_SHOW_BC:
                this.handler_server_other_show_bc(val);
                break;

            case NetCMD.SERVER_HEART_BEAT_RESP:
                this.handler_game_heartbeat_uc(val);
                break;
            case NetCMD.SERVER_READY_TIME_BC: //准备开始广播
                this.handler_server_ready_time_bc(val);
                break;

            case NetCMD.SERVER_BET_TIMEOUT_BC: //下注超时
                this.handler_server_bet_timeout_bc(val);
                break;

            case NetCMD.SERVER_UPTABLE_ERR_UC:
                this.handler_server_uptable_err_uc(val);
                break;
            default:
                cc.log(__String.createWithFormat("no handler of cmd %1", cmd));
        }

        return 0;
    },


    handler_server_uptable_err_uc: function (val) {
        cc.log("handler_server_uptable_err_uc");
        var code = utils.getValInt(val, "code");
        if (code == 102) {
            PlatformHelper.showToast("你已经在桌上");
        }
        else if (code == 104) {
            PlatformHelper.showToast("金币不够");
        }
        else if (code == 105) {
            PlatformHelper.showToast("桌上人已满");
        }
    },

    handler_server_ready_time_bc: function (val) {
        cc.log("handler_server_ready_time_bc");
        if (this.room_Action) {
            this.room_Action.showLaTime();
        }
    },


    handler_server_login_succ_uc: function (val) {
        cc.log("handler_server_login_succ_uc");
        RoomZJHInstance.isEnterWait = false;
    },

    handler_server_login_succ_bc: function (val) {
        cc.log("handler_server_login_succ_bc");

        var seatid = utils.getValInt(val, "seatid");
        if (seatid < 0) {
            return;
        }
        var player = RoomZJHInstance.players[seatid];

        player.uid = utils.getValInt(val, "uid");

        player.seatid = utils.getValInt(val, "seatid");
        player.ready = utils.getValInt(val, "ready");
        player.betting = utils.getValInt(val, "betting");
        player.see = utils.getValInt(val, "see");
        player.role = utils.getValInt(val, "role");
        player.status = utils.getValInt(val, "status");
        player.bet = utils.getValInt(val, "bet");
        player.dsc = utils.getValStr(val, "ps");

        player.name = utils.getValStr(val, "name");
        player.sex = utils.getValInt(val, "sex");
        player.avatar = utils.getValStr(val, "avatar");
        player.birthday = utils.getValStr(val, "birthday");
        player.zone = utils.getValStr(val, "zone");
        player.exp = utils.getValInt(val, "exp");
        player.rmb = utils.getValInt(val, "rmb");
        player.money = utils.getValInt(val, "money");
        player.coin = utils.getValInt(val, "coin");
        player.total_board = utils.getValInt(val, "total_board");
        player.total_win = utils.getValInt(val, "total_win");
        player.pcount = utils.getValInt(val, "pcount");
        player.vtime = utils.getValInt(val, "vtime");
        player.vlevel = utils.getValInt(val, "vlevel");
        player.matchMoney = utils.getValInt(val, "match_stake");

        var betTime = utils.getValInt(val, "is_bet_timeout");

        if (player.uid == ZJHModel.getInstance().uid) {
            RoomZJHInstance.seatid = seatid;
            ZJHModel.getInstance().mySeatid = RoomZJHInstance.seatid;
            //玩家自己上桌了
            for (var i = 0; i < 5; i++) {
                var pos = RoomZJHInstance.getPosBySeatid(i);
                var player = RoomZJHInstance.players[i];
                if (player.uid != -1) {
                    var rp = this.players[pos];
                    rp.login(player.seatid);

                    rp.setLostVisible(false);
                    rp.setThinkVisible(false);

                    if (betTime == 1 && player.seatid == RoomZJHInstance.cur_seat) {
                        rp.stopTimer();
                        rp.setThinkVisible(true);
                    }

                }
            }

        }
        else {
            var pos = RoomZJHInstance.getPosBySeatid(seatid);
            var RP = this.players[pos];
            RP.login(player.seatid);
            RP.setLostVisible(false);
            RP.setThinkVisible(false);
            if (betTime == 1 && player.seatid == RoomZJHInstance.cur_seat) {
                RP.stopTimer();
                RP.setThinkVisible(true);

            }
        }
    },

    handler_server_offline_bc: function (val) {
        cc.log("handler_server_offline_bc");
        var seatid = utils.getValInt(val, "seatid");

        if (seatid > -1) {
            var pos = RoomZJHInstance.getPosBySeatid(seatid);
            var RP = this.players[pos];
            if (RP != null) {
                RP.setLostVisible(true);
                RP.setThinkVisible(false);
            }
        }
    },

    handler_server_login_err_uc: function (val) {
        cc.log("handler_server_login_err_uc");
        if (utils.getValInt(val, "code") == 505)//skey 验证错误
        {
            this.unschedule(this.updateGame);
            ZJHLogic.sendLogout();
        }
    },

    handler_server_rebind_uc: function (val) {
        cc.log("handler_server_rebind_uc");
    },

    handler_server_logout_succ_bc: function (val) {
        cc.log("handler_server_logout_succ_bc");
        ZJHModel.getInstance().roomToHall();
    },


    handler_server_logout_err_uc: function (val) {
        cc.log("handler_server_logout_err_uc");
    },

    initRaise: function () {
        var a = [50000, 80000, 100000, 150000, 250000];
        for (var i = 0; i < 5; i++) {
            RoomZJHInstance.raise[i] = a[i];
        }
    },

    handler_server_table_info_uc: function (val) {
        cc.log("RoomZJHSetTablehandler_server_table_info_uc:");
        this.resetGame();
        this.room_Chip.removeAllChips();
        this.room_Card.reset();
        this.room_Tip.hideTime();

        for (var i = 0; i < 5; i++) {
            RoomZJHInstance.raise[i] = 50000;
            this.players[i].reset();
        }

        RoomZJHInstance.reset();
        RoomZJHInstance.cmp_round = utils.getValInt(val, "compare_round");
        RoomZJHInstance.per_time = 60;
        RoomZJHInstance.wait_time = 10;
        RoomZJHInstance.max_round = utils.getValInt(val, "max_round");
        RoomZJHInstance.see_round = utils.getValInt(val, "see_round");
        RoomZJHInstance.compare_condition = utils.getValInt(val, "compare_condition");

        RoomZJHInstance.stand_money = utils.getValInt(val, "stand_money");
        RoomZJHInstance.base_money = utils.getValInt(val, "base_money");
        RoomZJHInstance.m_nBettingTime = utils.getValInt(val, "bet_timer_stamp");
        RoomZJHInstance.left_bet_time = utils.getValInt(val, "left_bet_timer");
        RoomZJHInstance.tempSeatid = utils.getValInt(val, "seatid");
        RoomZJHInstance.seatid = utils.getValInt(val, "seatid");
        RoomZJHInstance.canBuyMore = utils.getValInt(val, "betting_type");
        RoomZJHInstance.state = utils.getValInt(val, "state");
        RoomZJHInstance.cur_round = utils.getValInt(val, "cur_round");
        RoomZJHInstance.cur_seat = utils.getValInt(val, "cur_seat");
        RoomZJHInstance.cur_bet = utils.getValInt(val, "cur_bet");
        RoomZJHInstance.total_bet = utils.getValInt(val, "total_bet");
        RoomZJHInstance.totalLife = utils.getValInt(val, "play_number");
        RoomZJHInstance.single_money = utils.getValInt(val, "single_money");
        RoomZJHInstance.max_bet = utils.getValInt(val, "max_allin");
        RoomZJHInstance.preready_time = utils.getValInt(val, "preready_timer_stamp");
        RoomZJHInstance.ready_time = utils.getValInt(val, "ready_timer_stamp");
        RoomZJHInstance.per_seatid = -1;
        ZJHModel.getInstance().mySeatid = RoomZJHInstance.seatid;
        RoomZJHInstance.cancompare = utils.getValInt(val, "can_compare");
        var left_bet_timer = utils.getValInt(val, "left_bet_timer");
        //raise相关
        {
            var mulsize = val["multi_bets"].length;
            if (mulsize > 4) {
                mulsize = 4;
            }
            RoomZJHInstance.raise[0] = RoomZJHInstance.base_money;
            for (var i = 0; i < mulsize; i++) {
                RoomZJHInstance.raise[i + 1] = val["multi_bets"][i];
            }

            var curIndex = 0;
            for (var i = 0; i < 5; i++) {
                if (RoomZJHInstance.cur_bet == RoomZJHInstance.raise[i] * RoomZJHInstance.base_money) {
                    curIndex = i;
                    break;
                }
            }
        }

        if (this.room_Tip != null) {
            if (RoomZJHInstance.isState(this.BETTING)) {
                this.room_Tip.setCurBet(RoomZJHInstance.cur_bet);
                this.room_Tip.setCurRound(RoomZJHInstance.cur_round);
                this.room_Tip.setTotalValue(RoomZJHInstance.total_bet);
            }
        }

        if (val["bet_timer_stamp"] != 0) {
            RoomZJHInstance.per_time = val["bet_timer_stamp"];
        }

        RoomZJHInstance.isMy = false;
        if (this.room_Action != null) {
            this.room_Action.setCallAll(false);
        }
        RoomZJHInstance.callAll = false;
        RoomZJHInstance.gameReset = true;

        var chipNum = Math.min(50, RoomZJHInstance.total_bet / RoomZJHInstance.base_money);
        for (var i = 0; i < chipNum; i++) {
            this.room_Chip.sendBet(-1, RoomZJHInstance.base_money, 0);
        }


        for (var i = 0; i < 5; i++) {
            this.players[i].setVisible(false);
        }

        var betTime = val["is_bet_timeout"];

        var len = val["players"] != null ? val["players"].length : 0;
        for (var i = 0; i < len; i++) {
            var seatid = val["players"][i]["seatid"];
            var player = RoomZJHInstance.players[seatid];
            player.uid = val["players"][i]["uid"];
            player.seatid = val["players"][i]["seatid"];
            player.ready = val["players"][i]["ready"];
            player.betting = val["players"][i]["betting"];
            player.see = val["players"][i]["see"];
            player.role = val["players"][i]["role"];
            player.status = val["players"][i]["status"];
            player.bet = val["players"][i]["bet"];

            player.name = val["players"][i]["name"];
            player.sex = val["players"][i]["sex"];
            player.avatar = val["players"][i]["avatar"];
            player.birthday = val["players"][i]["birthday"];
            player.zone = val["players"][i]["zone"];
            player.exp = val["players"][i]["exp"];
            player.rmb = val["players"][i]["rmb"];
            player.money = val["players"][i]["money"];
            player.coin = val["players"][i]["coin"];
            player.total_board = val["players"][i]["total_board"];
            player.total_win = val["players"][i]["total_win"];
            player.pcount = val["players"][i]["pcount"];
            player.vtime = val["players"][i]["vtime"];
            player.vlevel = val["players"][i]["vlevel"];
            player.dsc = val["players"][i]["ps"];
            player.matchMoney = val["players"][i]["match_stake"];

            player.jinbika = val["players"][i]["forbidden_card"];
            player.fanbeika = val["players"][i]["double_card"];
            player.huanpaika = val["players"][i]["change_card"];

            player.jinbiRound = val["players"][i]["is_forbidden"];//禁比最终轮
            player.fanbeiRound = val["players"][i]["is_double"];//比牌翻倍数
            player.fanbeiNums = val["players"][i]["is_double"];
            player.huanpaiTimes = val["players"][i]["is_change"];
            player.allBetMoney = val["players"][i]["bet_money"];//万人场 所有玩家在他身上投的钱
            player.myBetNums = val["players"][i]["my_count"];//万人场 我在他身上投的注数
            player.lifeNums = val["players"][i]["play_num"];

            player.offline = val["players"][i]["offline"];

            if (val["players"][i]["isAllIn"] == 1) {
                RoomZJHInstance.isAllIn = true;
            }

            var pos = RoomZJHInstance.getPosBySeatid(seatid);
            var mp = this.players[pos];
            mp.login(seatid);
            mp.stopTimer();

            if (player.offline) {
                mp.setLostVisible(true);
            }
            else {
                if (betTime == 1 && RoomZJHInstance.isState(this.BETTING) && RoomZJHInstance.cur_seat == seatid && player.bet == 0) {
                    mp.setThinkVisible(true);
                }
            }

            if (RoomZJHInstance.isState(this.START_BET) || RoomZJHInstance.isState(this.BETTING) || RoomZJHInstance.isState(this.STOP_BET)) {
                if (this.room_Info != null) {
                    this.room_Info.setMyBetValue(pos, player.myBetNums);
                    this.room_Info.setOtherBetValue(pos, player.allBetMoney);
                }
            }

            // this.room_Card.setJinBiRound(pos, player.jinbiRound);
            // this.room_Card.setFanBeiRound(pos, player.fanbeiRound, player.fanbeiNums);
            if (RoomZJHInstance.isMyPlayer(seatid)) {
                ZJHModel.getInstance().myBet = player.bet;
                RoomZJHInstance.MyIsPer = false;
                RoomZJHInstance.beting = player.betting;
                RoomZJHInstance.IsMySee = player.see;
                this.syncPlayerInfo();
            }

            if (RoomZJHInstance.isState(this.READY)) {

                cc.log("=======now======is=======ready");
                if (player.ready) {
                    mp.setReadyVisible(true);
                } else {
                    if (player.uid == ZJHModel.getInstance().uid) {
                        if (this.room_Action != null) {
                            this.room_Action.setReadyVisible(true);
                        }
                    }
                    mp.setMaskVisible(true);
                }
            }
            if (RoomZJHInstance.isState(this.BETTING)) {
                if (player.status == 2) {
                    this.room_Card.showFlag(pos, 2);
                }
                else if (player.status == 1) {
                    this.room_Card.showFlag(pos, 3);
                }
                else {
                    if (player.see == 1) {
                        this.room_Card.showFlag(pos, 1);
                    }
                    else {
                        this.room_Card.showFlag(pos, 0);
                    }
                }

                if (player.ready == 1) {
                    this.room_Card.showCard(pos);
                    if (RoomZJHInstance.isMyPlayer(seatid)) {
                        if (player.see == 1) {
                            this.room_Card.showFlag(pos, 1);
                            for (var j = 0; j < val["players"][i]["holes"].length; j++) {
                                var Cardvalue = val["players"][i]["holes"][j];
                                this.room_Card.showHoleCard2(j, Cardvalue);
                                player.myCard[j] = Cardvalue;
                                Sound.getInstance().playGameEffect(getResPath("Sound/zjh/g_flop.mp3"));
                            }
                            //图片加文字显示牌型
                            if (player.status != 1) {
                                var cardType = val["players"][i]["card_type"];
                                this.room_Tip.showCardTypeTips(pos, cardType);
                            }
                        }
                        else {
                            if (this.room_Action != null) {
                                this.room_Action.setButtonVisible(RoomZJHConfig.tag_see, true);
                            }
                        }
                    }
                }

                if (RoomZJHInstance.cur_seat == seatid) {
                    mp.startTimer(RoomZJHInstance.per_time, RoomZJHInstance.left_bet_time);
                }
            }
        }

        if (RoomZJHInstance.isState(this.BETTING) || RoomZJHInstance.isState(this.STOP_BET)) {
            if (RoomZJHInstance.seatid != -1) {
                if (this.room_Action != null) {
                    var ratio = RoomZJHInstance.IsMySee ? 2 : 1;

                    this.room_Action.setButtonNumber(RoomZJHConfig.tag_compare, RoomZJHInstance.cur_bet * ratio);
                    this.room_Action.setButtonNumber(RoomZJHConfig.tag_max, RoomZJHInstance.max_bet * ratio * RoomZJHInstance.base_money);
                    this.room_Action.setButtonNumber(RoomZJHConfig.tag_call, RoomZJHInstance.cur_bet * ratio);
                    this.room_Action.setButtonNumber(RoomZJHConfig.tag_callall, RoomZJHInstance.cur_bet * ratio);
                }

            }
        }
        else//空闲 啥事没干的
        {

        }

        //上桌
        if (this.room_Action != null) {
            if (RoomZJHInstance.seatid == -1) {
                if (len < 5) {
                    ZJHLogic.sendUpTable();
                    //RoomZJHInstance.send_logData(CLIENT_UPTABLE_APPLY_REQ);
                } else {
                    if (this.roomVerticalLayout != null) {
                        if (RoomZJHInstance.seatid == -1) {
                            this.roomVerticalLayout.setGamePlayStatu(false);
                        } else {
                            this.roomVerticalLayout.setGamePlayStatu(true);
                        }

                    }
//                room_Action->setButtonVisible(RoomZJHConfig::tag_up_table, RoomZJHInstance::getInstance()->seatid == -1);
                }
            }
        }

        if (this.roomVerticalLayout != null) {
            if (RoomZJHInstance.seatid == -1) {
                this.roomVerticalLayout.setGamePlayStatu(false);
            } else {
                this.roomVerticalLayout.setGamePlayStatu(true);
            }

        }

        if (RoomZJHInstance.isState(this.BETTING)) {
            this.nextUserEnterBet();
        }


        if (!RoomZJHInstance.isState(this.READY)) {
            JsUtils.postNotifi("notify_game_check_end");
        }


        if (this.room_EndTouPiao != null) {
            this.room_EndTouPiao.removeFromParent();
            this.room_EndTouPiao = null;
        }
        //is end room state
        if (val["dissolve_room_info"]["state"] == 1) {
            /*
             json
             {
             ownerid:220,
             ownername:"agag",
             time_left:20,
             players
             [
             {uid:111,name:"name",avatar:"avatar",state:1},//state: 0:等待 1：同意 2:反对
             ],
             }
             */

            var json;
            var player = RoomZJHInstance.getPlayerByUid(val["dissolve_room_info"]["uid"]);

            if (player && RoomZJHInstance.getPosBySeatid(player.seatid) >= 0) {
                var isCan = ZJHModel.getInstance().uid == player.uid;
                json["ownerid"] = player.uid;
                json["ownername"] = player.name;
                json["time_left"] = val["dissolve_room_info"]["remain_time"];
                for (var i = 0; i < val["dissolve_room_info"]["voters_uid"].length; i++) {
                    var p = RoomZJHInstance.getPlayerByUid(val["dissolve_room_info"]["voters_uid"][i]);
                    json["players"][i]["uid"] = p.uid;
                    json["players"][i]["name"] = p.name;
                    json["players"][i]["avatar"] = p.avatar;
                    json["players"][i]["state"] = 2;

                    for (var j = 0; j < val["dissolve_room_info"]["players"].length; j++) {
                        var sid = val["dissolve_room_info"]["players"][i]["uid"];
                        var ac = val["dissolve_room_info"]["players"][i]["action"];

                        if (ac == 0) {
                            ac = 2;
                        }
                        else if (ac == 2) {
                            ac = 0;
                        }

                        if (sid == p.uid) {
                            json["players"][i]["state"] = ac;
                            break;
                        }
                    }
                    if (ZJHModel.getInstance().uid == p.uid) {
                        isCan = true;
                    }
                }

                if (isCan) {
                    this.room_EndTouPiao = RoomHEndTouPiao.create();
                    this.room_EndTouPiao.initData(json);
                    this.addChild(this.room_EndTouPiao, 10000000);

                    this.room_EndTouPiao.setCallback(function (str) {
                        if (str == "ok") {
                            ZJHLogic.sendDissolveAction(1);
                        }
                        else if (str == "no") {
                            ZJHLogic.sendDissolveAction(0);
                        }
                    });
                }
            }

        }


        RoomZJHInstance.waitUpdate = false;

        Sound.getInstance().playEffect(getResPath("Sound/sound_gangjinru.mp3"));

        if (RoomZJHInstance.seatid != -1) {
            this.roomVerticalLayout.setFastSoundVis(true);
        }
        else {
            this.roomVerticalLayout.setFastSoundVis(false);
        }
    },

    handler_server_ready_succ_bc: function (val) {
        cc.log("handler_server_ready_succ_bc");

        var uid = val["uid"];
        var seatid = val["seatid"];
        var pos = RoomZJHInstance.getPosBySeatid(seatid);
        var rp = this.players[pos];
        if (rp != null) {
            cc.log("player ready===========");
            rp.setReadyVisible(true);
            rp.setMaskVisible(false);
        }
        if (uid == ZJHModel.getInstance().uid) {
            if (this.room_Action != null) {
                this.room_Action.setReadyVisible(false);
            }
        }
    },
    handler_server_ready_err_uc: function (val) {

    },

    handler_server_game_start_bc: function (val) {
        cc.log("handler_server_game_start_bc");

        RoomZJHInstance.balanceOver = false;
        RoomZJHInstance.gameReset = false;
        if (this.room_Action != null) {
            this.room_Action.hideLaTime();
        }


        this.room_Card.hideAllCard();
        this.room_Card.hideAllFlag();
        this.room_Card.reset();

        for (var i = 0; i < 5; i++) {
            this.players[i].setReadyVisible(false);
            this.players[i].setMaskVisible(false);
        }

        RoomZJHInstance.cur_round = val["cur_round"];
        RoomZJHInstance.cur_bet = val["cur_bet"];
        RoomZJHInstance.total_bet = val["total_bet"];
        RoomZJHInstance.dealer = val["dealer"];
        RoomZJHInstance.first_charge = val["first_charge"];
        RoomZJHInstance.state = this.BETTING;
        if (this.room_Tip != null) {
            this.room_Tip.setCurRound(RoomZJHInstance.cur_round);
            this.room_Tip.setCurBet(RoomZJHInstance.cur_bet);
            this.room_Tip.setTotalValue(RoomZJHInstance.total_bet);
        }

        for (var i = 0; i < 5; i++) {
            RoomZJHInstance.players[i].prepra();
        }

        var first_charge = val["first_charge"];
        var len = val["seatids"].length;
        for (var i = 0; i < len; i++) {
            var seatid = val["seatids"][i];
            var pos = RoomZJHInstance.getPosBySeatid(seatid);

            {
                this.room_Chip.sendBet(pos, RoomZJHInstance.cur_bet, 0);
            }

            Sound.getInstance().playGameEffect(getResPath("Sound/zjh/g_addchip.mp3"));
            var RoomZJHPlayer = this.players[pos];
            var player = RoomZJHInstance.players[seatid];

            {
                player.money -= RoomZJHInstance.cur_bet;
                player.bet = RoomZJHInstance.cur_bet + first_charge;
            }
            player.ready = 1;
            player.betting = 1;
            player.see = 0;
            player.status = 0;
            RoomZJHPlayer.updateInfo();
        }

        this.room_Card.beginFaPai();
        RoomZJHInstance.cur_seat = -1;
        RoomZJHInstance.isFapai = true;
        RoomZJHInstance.isEnterWait = true;

        var delay = 0.50 - 0.17;
        var cardNums = 3;
        for (var i = 0; i < cardNums; i++) {
            for (var j = 0; j < len; j++) {
                delay += 0.17;
                var seatid = val["seatids"][j];
                var pos = RoomZJHInstance.getInstance().getPosBySeatid(seatid);
                var cardValue = 0;
                if ((seatid == RoomZJHInstance.seatid) && RoomZJHInstance.isDouNiuRoom && val["cards"].length) {
                    cardValue = val["cards"][i];
                }
                this.room_Card.showCard(pos, i, delay, cardValue);
            }
        }
        this.changeCoinLayer(60);
        this.room_Tip.hideTime();
        this.schedule(this.faPaiOver);

        RoomZJHInstance.isAllIn = false;
        if (RoomZJHInstance.seatid == -1) {

        }
        else {
            if (this.room_Action != null) {
                this.room_Action.setButtonNumber(RoomZJHConfig.tag_compare, RoomZJHInstance.cur_bet);
                this.room_Action.setButtonNumber(RoomZJHConfig.tag_max, RoomZJHInstance.max_bet * RoomZJHInstance.base_money);
                this.room_Action.setButtonNumber(RoomZJHConfig.tag_call, RoomZJHInstance.cur_bet);
                this.room_Action.setButtonNumber(RoomZJHConfig.tag_callall, RoomZJHInstance.cur_bet);

                this.room_Action.setButtonEnable(RoomZJHConfig.tag_compare, false);
                this.room_Action.setButtonEnable(RoomZJHConfig.tag_max, false);

//            this.room_Action->stretchAllButton(1, 1.0);
                this.room_Action.showOtherBetButtons();
            }
        }
        JsUtils.postNotifi("notify_game_check_end");
    },


    handler_server_second_fapai_bc: function (val) {
        //第二轮发牌2张
        cc.log(__String.createWithFormat("handler_server_second_fapai_bc %1", val));
        RoomZJHInstance.isFapai = true;
        var communitys = val["communitys"];
        for (var i = 0; i < communitys.length; ++i) {
            //todo CardTypeChange ????
            var cardValue = Utils.CardTypeChange(communitys[i]);
            RoomZJHInstance.fiveHandCards.push(cardValue);
        }
        //翻开第4，5张底牌
        this.room_Card.openFiveHandCards(RoomZJHInstance.fiveHandCards, 0.2, 3, RoomZJHInstance.fiveHandCards.length);
        cc.log("handler_server_second_fapai_end");

        var callback = cc.callFunc(this.secondFaPaiOver, this);
        this.runAction(cc.sequence(cc.delayTime(2.0), callback));
    },

    handler_server_second_fapai_uc: function (val) {
        //第二轮发牌后检测玩家牌型
        cc.log("handler_server_second_fapai_uc");
        var player = RoomZJHInstance.players[RoomZJHInstance.seatid];
        if (player != null) {
            if (player.see == 1) {
                if (player.status != 1) {
                    var card_type = val["card_type"];
                    var pos = RoomZJHInstance.getPosBySeatid(RoomZJHInstance.seatid);
                    //__String.createWithFormat("room_DZcardType_%1",card_type);
                    this.room_Tip.showCardTypeTips(pos, card_type);
                }
            }
        }
    },


    handler_server_get_two_card: function (val) {
        cc.log("handler_server_get_two_card");

        this.room_Card.beginFaPai();
        RoomZJHInstance.isFapai = true;
        RoomZJHInstance.isEnterWait = true;
        var delay = 0.50 - 0.17;
        var cardNums = 5;
        for (var i = 3; i < cardNums; i++) {
            for (var j = 0; j < val["seatids"].length; j++) {
                delay += 0.17;
                var seatid = val["seatids"][j];
                var pos = RoomZJHInstance.getPosBySeatid(seatid);
                var cardValue = 0;
                if ((seatid == RoomZJHInstance.seatid) && RoomZJHInstance.isDouNiuRoom && val["cards"].length) {
                    cardValue = val["cards"][i - 3];
                }
                this.room_Card.showCard(pos, i, delay, cardValue);
            }
        }
        var cardType = val["card_type"];
        cc.log(__String.createWithFormat("Douniu Card Type %1", cardType));
        this.room_Card.showDouNiuCardType(delay + 0.4, 0, cardType);

        this.changeCoinLayer(30);

        this.schedule(this.faPaiOver);
    },

    removeSprite: function (sender) {
        sender.removeFromParentAndCleanup(true);
    },

    checkSendBet: function (pos, money) {
        while (money >= RoomZJHInstance.raise[0]) {
            while (money >= RoomZJHInstance.raise[1]) {
                while (money >= RoomZJHInstance.raise[2]) {
                    while (money >= RoomZJHInstance.raise[3]) {
                        while (money >= RoomZJHInstance.raise[4]) {
                            this.room_Chip.sendBet(pos, RoomZJHInstance.raise[4], 0);
                            money -= RoomZJHInstance.raise[4];
                        }
                        if (money < RoomZJHInstance.raise[3]) {
                            break;
                        }
                        this.room_Chip.sendBet(pos, RoomZJHInstance.raise[3], 0);
                        money -= RoomZJHInstance.raise[3];
                    }
                    if (money < RoomZJHInstance.raise[2]) {
                        break;
                    }
                    this.room_Chip.sendBet(pos, RoomZJHInstance.raise[2], 0);
                    money -= RoomZJHInstance.raise[2];
                }
                if (money < RoomZJHInstance.raise[1]) {
                    break;
                }
                this.room_Chip.sendBet(pos, RoomZJHInstance.raise[1], 0);
                money -= RoomZJHInstance.raise[1];
            }
            if (money < RoomZJHInstance.raise[0]) {
                break;
            }
            this.room_Chip.sendBet(pos, RoomZJHInstance.raise[0], 0);
            money -= RoomZJHInstance.raise[0];
        }
    },


    secondFaPaiOver: function () {
        RoomZJHInstance.isFapai = false;
        if (RoomZJHInstance.cur_seat != -1) {
            this.changeCoinLayer(30);
            this.nextUserEnterBet();
        }
    },


    faPaiOver: function (dt) {
        if (this.room_Card.cardNums <= 0) {
            this.unschedule(this.faPaiOver);
            this.changeCoinLayer(30);
            if (RoomZJHInstance.cur_seat != -1) {
                this.nextUserEnterBet();
            }
            if (RoomZJHInstance.seatid != -1) {
                room_Action.setButtonVisible(RoomZJHConfig.tag_see, true);
            }
            RoomZJHInstance.isFapai = false;
            RoomZJHInstance.isEnterWait = false;
        }
    },

    handler_server_next_bet_bc: function (val) {
        cc.log("handler_server_next_bet_bc");
        var seatid = val["seatid"];

        RoomZJHInstance.cur_round = val["cur_round"];
        RoomZJHInstance.cancompare = val["can_compare"];
        if (this.room_Tip != null) {
            this.room_Tip.setCurRound(RoomZJHInstance.cur_round);
        }

        for (var i = 0; i < 5; i++) {
            var RoomZJHPlayer = this.players[i];
            RoomZJHPlayer.stopTimer();
        }

        RoomZJHInstance.cur_seat = seatid;
        cc.log(__String.createWithFormat("======cur_seat==========%1", seatid));
        if (!RoomZJHInstance.isFapai) {
            this.changeCoinLayer(30);
            this.nextUserEnterBet();
        }
    },

    nextUserEnterBet: function () {
        var betnums = 0;
        for (var i = 0; i < 5; i++) {
            if (RoomZJHInstance.players[i].betting == 1) {
                betnums++;
            }
        }
        if (betnums <= 1) {
            return;
        }
        var seatid = RoomZJHInstance.cur_seat;
        var pos = RoomZJHInstance.getPosBySeatid(seatid);
        var room_Player = this.players[pos];
        var player = RoomZJHInstance.players[seatid];
        if (player.uid == 0) {
            return;
        }

        room_Player.startTimer(RoomZJHInstance.per_time, RoomZJHInstance.left_bet_time);
        RoomZJHInstance.left_bet_time = RoomZJHInstance.per_time;

        if (RoomZJHInstance.seatid == -1)return;


        if (RoomZJHInstance.isMyPlayer(seatid)) {
            Sound.getInstance().playEffect(getResPath("Sound/zjh/myself_order.mp3"));

            var player = RoomZJHInstance.players[RoomZJHInstance.seatid];
            if (player.betting == 1) {
                if (this.room_Action != null) {
                    RoomZJHInstance.isMy = true;
                    //自动跟注
                    if (RoomZJHInstance.callAll && RoomZJHInstance.cur_round <= RoomZJHInstance.max_round && !RoomZJHInstance.isAllIn) {
                        this.schedule(this.listerCallAll, 1.0);
                    }
                    else {
                        this.room_Action.showMyBetButtons();
                    }

                    if (RoomZJHInstance.isAllIn) {
                        this.room_Action.setButtonEnable(RoomZJHConfig.tag_compare, false);
                        this.room_Action.setButtonEnable(RoomZJHConfig.tag_raise, false);
                        this.room_Action.setButtonEnable(RoomZJHConfig.tag_call, false);
                        this.room_Action.setButtonEnable(RoomZJHConfig.tag_max, true);
                    }
                    else {
                        //call和raise
                        if (RoomZJHInstance.cur_round > RoomZJHInstance.max_round) {
                            this.room_Action.setButtonEnable(RoomZJHConfig.tag_call, false);
                            this.room_Action.setButtonEnable(RoomZJHConfig.tag_raise, false);
                        } else {
                            this.room_Action.setButtonEnable(RoomZJHConfig.tag_call, true);
                            if (RoomZJHInstance.cur_bet >= RoomZJHInstance.raise[4] * RoomZJHInstance.base_money) {
                                this.room_Action.setButtonEnable(RoomZJHConfig.tag_raise, false);
                            } else {
                                this.room_Action.setButtonEnable(RoomZJHConfig.tag_raise, true);
                            }
                        }

                        //compare和max

                        if (RoomZJHInstance.cancompare == 0) {
                            this.room_Action.setButtonEnable(RoomZJHConfig.tag_compare, false);
                        } else {
                            this.room_Action.setButtonEnable(RoomZJHConfig.tag_compare, true);
                        }

                    }
                }
            }
        }
        else {
            if (this.room_Action != null) {
                var player = RoomZJHInstance.players[RoomZJHInstance.seatid];
                if (player.betting == 1) {
                    this.room_Action.showOtherBetButtons();
                }
                else {
                    this.room_Action.retractAllButton();
                }
            }
        }
    },

    listerCallAll: function (dt) {
        if (RoomZJHInstance.isMy && RoomZJHInstance.callAll && RoomZJHInstance.cur_round <= RoomZJHInstance.max_round) {
            this.actionCallBack(RoomZJHConfig.tag_call);
        }
        this.overListerCallAll();
    },

    overListerCallAll: function () {
        this.unschedule(this.listerCallAll);
    },

    handler_server_bet_succ_bc: function (val) {
        cc.log("handler_server_bet_succ_bc");

        var seatid = val["seatid"];
        var action = val["action"];

        if (seatid == RoomZJHInstance.seatid) {
            //RoomZJHInstance.send_logData(CLIENT_BET_REQ);//下注
        }

        var pos = RoomZJHInstance.getPosBySeatid(seatid);
        var roomPlayer = this.players[pos];
        roomPlayer.stopTimer();
        this.room_Tip.hideTime();
        if (roomPlayer != null) {
            roomPlayer.setThinkVisible(false);
            roomPlayer.setLostVisible(false);
        }

        var player = RoomZJHInstance.players[seatid];
        if (action == CLIENT_ACTION.PLAYER_CALL) {
            RoomZJHInstance.cur_bet = val["cur_bet"];
            RoomZJHInstance.total_bet = val["total_bet"];
            RoomZJHInstance.cur_round = val["cur_round"];
            if (this.room_Tip != null) {
                this.room_Tip.setCurBet(RoomZJHInstance.cur_bet);
                this.room_Tip.setTotalValue(RoomZJHInstance.total_bet);
                this.room_Tip.setCurRound(RoomZJHInstance.cur_round);
            }

            player.money = val["money"];
            player.bet = val["bet"];
            player.matchMoney = val["match_stake"];
            roomPlayer.updateInfo();
            this.room_Tip.setPlayerBet(pos, player.bet);
            if (seatid != RoomZJHInstance.seatid) {
                var bet_ratio = val["bet_ratio"];
                for (var i = 0; i < bet_ratio; i++) {
                    this.room_Chip.sendBet(pos, RoomZJHInstance.cur_bet, 0);
                    Sound.getInstance().playGameEffect(getResPath("Sound/zjh/g_addchip.mp3"));
                }
            }
            else {
                if (RoomZJHInstance.callAll == false) {//手动跟注
                    this.room_Action.showOtherBetButtons();
                }
            }
            Sound.getInstance().playGameEffect(__String.createWithFormat(getResPath("Sound/zjh/%1_addchip.mp3"),
                RoomZJHInstance.getSexBySeatid(seatid) == 2 ? "m" : "f"));

            this.room_Tip.showHint(pos, this.HINT_CALL);
        }
        else if (action == CLIENT_ACTION.PLAYER_RAISE) {
            RoomZJHInstance.cur_bet = val["cur_bet"];
            RoomZJHInstance.total_bet = val["total_bet"];
            RoomZJHInstance.cur_round = val["cur_round"];
            if (this.room_Tip != null) {
                this.room_Tip.setCurBet(RoomZJHInstance.cur_bet);
                this.room_Tip.setTotalValue(RoomZJHInstance.total_bet);
                this.room_Tip.setCurRound(RoomZJHInstance.cur_round);
            }

            player.money = val["money"];
            player.bet = val["bet"];

            roomPlayer.updateInfo();
            this.room_Tip.setPlayerBet(pos, player.bet);
            if (seatid != RoomZJHInstance.seatid) {
                var bet_ratio = val["bet_ratio"];
                for (var i = 0; i < bet_ratio; i++) {
                    this.room_Chip.sendBet(pos, RoomZJHInstance.cur_bet, 0);
                }
            }
            else {
                this.room_Action.showOtherBetButtons();
            }
            this.room_Tip.showHint(pos, this.HINT_RAISE);
            var path = "";
            if (RoomZJHInstance.getSexBySeatid(seatid) == 2) {
                var randomNum = Math.random(0, 100) % 2;
                if (randomNum) {
                    path = getResPath("Sound/zjh/f_add1.mp3");
                } else {
                    path = getResPath("Sound/zjh/f_add2.mp3");
                }
            } else {
                path = getResPath("Sound/zjh/m_add.mp3");
            }
            Sound.getInstance().playGameEffect(path);

            RoomZJHInstance.call_cnt = 0;
        }
        else if (action == CLIENT_ACTION.PLAYER_COMPARE) {
            if (action == CLIENT_ACTION.PLAYER_COMPARE) {
                this.room_Tip.showHint(pos, this.HINT_CMP);
            }
            else {
                this.room_Tip.showHint(pos, this.HINT_MAX);
            }


            RoomZJHInstance.cur_bet = val["cur_bet"];
            RoomZJHInstance.total_bet = val["total_bet"];
            RoomZJHInstance.cur_round = val["cur_round"];
            if (this.room_Tip != null) {
                this.room_Tip.setCurBet(RoomZJHInstance.cur_bet);
                this.room_Tip.setTotalValue(RoomZJHInstance.total_bet);
                this.room_Tip.setCurRound(RoomZJHInstance.cur_round);
            }

            player.money = val["money"];
            player.bet = val["bet"];
            roomPlayer.updateInfo();
            this.room_Tip.setPlayerBet(pos, player.bet);
            var bet_ratio = val["bet_ratio"];
            var chipNum = Math.min(30, RoomZJHInstance.max_bet * bet_ratio);

            for (var i = 0; i < chipNum; i++) {
                this.room_Chip.sendChip(pos, RoomZJHInstance.base_money);
            }

            var targetSeatid = val["target_seatid"];
            var targetPos = RoomZJHInstance.getPosBySeatid(targetSeatid);
            var targetStatus = val["target_status"];

            var lose = 0;
            if (targetStatus == 2) {
                lose = targetPos;
                var targerPlayer = RoomZJHInstance.players[targetSeatid];
                targerPlayer.betting = 0;
                targerPlayer.status = 2;
            }
            else {
                lose = pos;
                player.betting = 0;
                player.status = 2;
            }

            this.showCompare(pos, targetPos, lose);

            if (RoomZJHInstance.isAllIn) {
                var path = "";
                var randomNum = Math.random(0, 100) % 2;
                if (RoomZJHInstance.getSexBySeatid(seatid) == 2) {
                    if (randomNum) {
                        path = getResPath("Sound/zjh/f_allin1.mp3");
                    } else {
                        path = getResPath("Sound/zjh/f_add2.mp3");
                    }
                } else {
                    if (randomNum) {
                        path = getResPath("Sound/zjh/m_allin1.mp3");
                    } else {
                        path = getResPath("Sound/zjh/m_allin2.mp3");
                    }
                }
                Sound.getInstance().playGameEffect(path);
            }
            else {
                var path = "";
                var randomNum = Math.random(0, 100) % 2;
                if (RoomZJHInstance.getSexBySeatid(seatid) == 2) {
                    if (randomNum) {
                        path = getResPath("Sound/zjh/f_cmp.mp3");
                    } else {
                        path = getResPath("Sound/zjh/f_cmp2.mp3");
                    }
                } else {
                    if (randomNum) {
                        path = getResPath("Sound/zjh/m_cmp.mp3");
                    } else {
                        path = getResPath("Sound/zjh/m_cmp2.mp3");
                    }
                }
                Sound.getInstance().playGameEffect(path);
            }

            RoomZJHInstance.call_cnt = 0;

            //处理比排过程中 放弃导致服务器死机的bug
            var betnums = 0;
            for (var i = 0; i < 5; i++) {
                if (RoomZJHInstance.players[i].betting == 1) {
                    betnums++;
                }
            }
            if (betnums <= 1) {
                if (this.room_Action != null) {
                    this.room_Action.setAllBetButtonVisible(false);
                }
            }

            if (lose == 0) {
                if (this.room_Action != null) {
                    this.room_Action.setAllBetButtonVisible(false);
                }
            }
            else if (targetPos == 0 || pos == 0) {
            }
        }
        else if (action == CLIENT_ACTION.PLAYER_SEE) {
            player.see = 1;
            this.room_Card.showFlag(pos, 1);

            this.room_Tip.showHint(pos, this.HINT_SEE);

            Sound.getInstance().playGameEffect(__String.createWithFormat(getResPath("Sound/zjh/%1_watch.mp3"),
                RoomZJHInstance.getSexBySeatid(seatid) == 2 ? "m" : "f"));

            RoomZJHInstance.call_cnt = 0;
            if (RoomZJHInstance.isMyPlayer(seatid)) {
                RoomZJHInstance.IsMySee = true;
                var cardType = val["card_type"];
                this.room_Tip.showCardTypeTips(pos, cardType, 0.5);
            }

        }
        else if (action == CLIENT_ACTION.PLAYER_FOLD) {
            this.room_Card.showFlag(pos, 3);
            if (RoomZJHInstance.isMyPlayer(seatid)) {
                //if(room_Action)room_Action->setAllBetBtnEnable(false);
                if (this.room_Action != null) {
                    this.room_Action.retractAllButton();
                    this.room_Action.setButtonVisible(RoomZJHConfig.tag_see, false);

                }
            }
            player.betting = 0;
            player.status = 1;

            this.room_Tip.showHint(pos, this.HINT_FOLD);
            Sound.getInstance().playGameEffect(__String.createWithFormat(getResPath("Sound/zjh/%1_giveup.mp3"),
                RoomZJHInstance.getSexBySeatid(seatid) == 2 ? "m" : "f"));
            RoomZJHInstance.call_cnt = 0;

            var bet_num = val["betting_num"];
            if (RoomZJHInstance.cur_seat != seatid) {
                if (bet_num == 2 && RoomZJHInstance.compare_condition == 0 && RoomZJHInstance.cur_round > RoomZJHInstance.cmp_round) {
                    if (this.room_Action != null) {
                        this.room_Action.setButtonEnable(RoomZJHConfig.tag_compare, true);
                    }

                }
            }

        }
        if (this.room_Action != null) {
            var ratio = RoomZJHInstance.IsMySee ? 2 : 1;

            this.room_Action.setButtonNumber(RoomZJHConfig.tag_compare, RoomZJHInstance.cur_bet * ratio);
            this.room_Action.setButtonNumber(RoomZJHConfig.tag_max, RoomZJHInstance.max_bet * ratio * RoomZJHInstance.base_money);
            this.room_Action.setButtonNumber(RoomZJHConfig.tag_call, RoomZJHInstance.cur_bet * ratio);
            this.room_Action.setButtonNumber(RoomZJHConfig.tag_callall, RoomZJHInstance.cur_bet * ratio);
        }
        this.syncPlayerInfo();
    },

    handler_server_bet_succ_uc: function (val) {
        cc.log("------RoomZJHSetTable::handler_server_bet_succ_uc:%1", val);
        var seatid = val["seatid"];
        var action = val["action"];

        var pos = RoomZJHInstance.getPosBySeatid(seatid);
        var RP = this.players[pos];
        RP.stopTimer();
//    RoomZJHPlayer *RoomZJHPlayer = players[pos];
        var player = RoomZJHInstance.players[seatid];
        player.see = 1;
        if (action == CLIENT_ACTION.PLAYER_CALL) {
            RoomZJHInstance.cur_bet = val["cur_bet"];
            RoomZJHInstance.total_bet = val["total_bet"];
            player.money = val["money"];
            player.bet = val["bet"];
            player.matchMoney = val["match_stake"];
            RP.updateInfo();
            this.room_Tip.setPlayerBet(pos, player.bet);
            if (!RoomZJHInstance.isMyPlayer(seatid) || (RoomZJHInstance.isMyPlayer(seatid) && val["auto_call"] == 1)) {
                var bet_ratio = val["bet_ratio"];
                for (var i = 0; i < bet_ratio; i++) {
                    this.room_Chip.sendBet(pos, RoomZJHInstance.cur_bet, 0);
                    Sound.getInstance().playGameEffect(getResPath("Sound/zjh/g_addchip.mp3"));
                }
                this.room_Tip.showHint(pos, this.HINT_CALL);
            }
            this.room_Tip.showHint(pos, this.HINT_CALL);

            RoomZJHInstance.call_cnt++;
            if (RoomZJHInstance.call_cnt > 2) {
                if (RoomZJHInstance.call_cnt % 2 == 0) {
                    Sound.getInstance().playGameEffect(__String.createWithFormat(getResPath("Sound/zjh/%1_follow.mp3"),
                        RoomZJHInstance.getSexBySeatid(seatid) == 2 ? "m" : "f"));
                }
                else {
                    Sound.getInstance().playGameEffect(__String.createWithFormat(getResPath("Sound/zjh/%1_follow.mp3"),
                        RoomZJHInstance.getSexBySeatid(seatid) == 2 ? "m" : "f"));
                }
            }
            else {
                Sound.getInstance().playGameEffect(__String.createWithFormat(getResPath("Sound/zjh/%1_follow.mp3"),
                    RoomZJHInstance.getSexBySeatid(seatid) == 2 ? "m" : "f"));
            }

            if (player.see == 1) {
                player.dangle += 1;
            }
        }
        else if (action == CLIENT_ACTION.PLAYER_RAISE) {
            RoomZJHInstance.cur_bet = val["cur_bet"];
            RoomZJHInstance.total_bet = val["total_bet"];

            player.money = val["money"];
            player.bet = val["bet"];
            player.matchMoney = val["match_stake"];
            RP.updateInfo();
            this.room_Tip.setPlayerBet(pos, player.bet);
            if (!RoomZJHInstance.isMyPlayer(seatid)) {
                var bet_ratio = val["bet_ratio"];
                for (var i = 0; i < bet_ratio; i++) {
                    this.room_Chip.sendBet(pos, RoomZJHInstance.cur_bet, 0);
                }
            }
            this.room_Tip.showHint(pos, this.HINT_RAISE);

            var path = "";
            if (RoomZJHInstance.getSexBySeatid(seatid) == 2) {
                var randomNum = Math.random(0, 100) % 2;
                if (randomNum) {
                    path = getResPath("Sound/zjh/f_add1.mp3");
                } else {
                    path = getResPath("Sound/zjh/f_add2.mp3");
                }
            } else {
                path = getResPath("Sound/zjh/m_add.mp3");
            }
            Sound.getInstance().playGameEffect(path);

            RoomZJHInstance.call_cnt = 0;
            if (player.see == 1) {
                player.dangle += 1;
            }

        }
        else if (action == CLIENT_ACTION.PLAYER_COMPARE) {
            if (action == CLIENT_ACTION.PLAYER_COMPARE) {
                this.room_Tip.showHint(pos, this.HINT_CMP);
            }
            else {
                this.room_Tip.showHint(pos, this.HINT_ALLIN);
            }

            //todo not found.
            Sound.getInstance().playEffect("Sound/sound_quanya.mp3");

            RoomZJHInstance.cur_bet = val["cur_bet"];
            RoomZJHInstance.total_bet = val["total_bet"];

            player.money = val["money"];
            player.bet = val["bet"];
            player.matchMoney = val["match_stake"];
            RP.updateInfo();
            this.room_Tip.setPlayerBet(pos, player.bet);
            var bet_ratio = val["bet_ratio"];

            if (RoomZJHInstance.cur_bet > RoomZJHInstance.raise[4] * RoomZJHInstance.base_money) {
                for (var i = 0; i < bet_ratio; i++) {
                    for (var j = 0; j < RoomZJHInstance.cur_bet / (RoomZJHInstance.raise[4] * RoomZJHInstance.base_money); j++) {
                        this.room_Chip.sendBet(pos, RoomZJHInstance.raise[4] * RoomZJHInstance.base_money, 0);
                    }
                }
            }
            else {
                for (var i = 0; i < bet_ratio; i++) {
                    this.room_Chip.sendBet(pos, RoomZJHInstance.cur_bet, 0);
                }
            }

            var targetSeatid = val["target_seatid"];
            var targetPos = RoomZJHInstance.getPosBySeatid(targetSeatid);
            var targetStatus = val["target_status"];

            var lose = 0;
            var targerPlayer = RoomZJHInstance.players[targetSeatid];

            if (targetStatus == 2) {
                lose = targetPos;
                targerPlayer.betting = 0;
                targerPlayer.status = 2;
                targerPlayer.isOut = 1;
                player.dangle += 1;
            }
            else {
                lose = pos;
                player.betting = 0;
                player.status = 2;
                player.isOut = 1;
                targerPlayer.dangle += 1;
            }

            this.showCompare(pos, targetPos, lose);

            if (RoomZJHInstance.isAllIn) {
                var path = "";
                var randomNum = Math.random(0, 100) % 2;
                if (RoomZJHInstance.getSexBySeatid(seatid) == 2) {
                    if (randomNum) {
                        path = getResPath("Sound/zjh/f_allin1.mp3");
                    } else {
                        path = getResPath("Sound/zjh/f_add2.mp3");
                    }
                } else {
                    if (randomNum) {
                        path = getResPath("Sound/zjh/m_allin1.mp3");
                    } else {
                        path = getResPath("Sound/zjh/m_allin2.mp3");
                    }
                }
                Sound.getInstance().playGameEffect(path);
            }
            else {
                var path = "";
                var randomNum = Math.random(0, 100) % 2;
                if (RoomZJHInstance.getSexBySeatid(seatid) == 2) {
                    if (randomNum) {
                        path = getResPath("Sound/zjh/f_cmp.mp3");
                    } else {
                        path = getResPath("Sound/zjh/f_cmp2.mp3");
                    }
                } else {
                    if (randomNum) {
                        path = getResPath("Sound/zjh/m_cmp.mp3");
                    } else {
                        path = getResPath("Sound/zjh/m_cmp2.mp3");
                    }
                }
                Sound.getInstance().playGameEffect(path);
            }

            RoomZJHInstance.call_cnt = 0;

            //处理比排过程中 放弃导致服务器死机的bug
            var betnums = 0;
            for (var i = 0; i < 5; i++) {
                if (RoomZJHInstance.players[i].betting == 1) {
                    betnums++;
                }
            }
            if (betnums <= 1) {
                if (this.room_Action != null) {

                }

            }
        }
        else if (action == CLIENT_ACTION.PLAYER_SEE) {
            this.room_Card.showFlag(pos, 1);
            var card_nums = 3;
            if (RoomZJHInstance.Game_type == GAME_TYPE_DZ) {
                card_nums = 2;
            }
            for (var i = 0; i < card_nums; i++) {
                var Cardvalue = val["holes"][i].asInt();
                this.room_Card.showHoleCard2(i, Cardvalue);
                player.myCard[i] = Cardvalue;
                Sound.getInstance().playGameEffect(getResPath("Sound/zjh/g_flop.mp3"));
            }
            if (RoomZJHInstance.isMyPlayer(seatid)) {
                var cardType = val["card_type"];
                RoomZJHInstance.IsMySee = true;
                this.room_Tip.showCardTypeTips(pos, cardType, 1);

                if (this.room_Action != null) {
                    this.room_Action.setButtonNumber(RoomZJHConfig.tag_compare, RoomZJHInstance.cur_bet * 2);
                    this.room_Action.setButtonNumber(RoomZJHConfig.tag_max, RoomZJHInstance.max_bet * 2 * RoomZJHInstance.base_money);
                    this.room_Action.setButtonNumber(RoomZJHConfig.tag_call, RoomZJHInstance.cur_bet * 2);
                    this.room_Action.setButtonNumber(RoomZJHConfig.tag_callall, RoomZJHInstance.cur_bet * 2);
                    this.room_Action.setButtonVisible(RoomZJHConfig.tag_see, false);
                }
            }

            this.room_Tip.showHint(pos, this.HINT_SEE);
            Sound.getInstance().playGameEffect(__String.createWithFormat(getResPath("Sound/zjh/%1_watch.mp3"),
                RoomZJHInstance.getSexBySeatid(seatid) == 2 ? "m" : "f"));

            Sound.getInstance().playEffect(getResPath("Sound/zjh/see.mp3"));
            RoomZJHInstance.call_cnt = 0;
        }
        else if (action == CLIENT_ACTION.PLAYER_FOLD) {
            this.room_Card.showFlag(pos, 3);
            player.isOut = 1;
            if (RoomZJHInstance.isMyPlayer(seatid)) {
                if (this.room_Action != null) {
                    this.room_Action.setAllBetButtonVisible(false);
                }

                Sound.getInstance().playGameEffect(getResPath("Sound/zjh/g_giveup.mp3"));
            }
            player.betting = 0;
            player.status = 1;

            this.room_Tip.showHint(pos, this.HINT_FOLD);
            Sound.getInstance().playGameEffect(__String.createWithFormat(getResPath("Sound/zjh/%1_giveup.mp3"),
                RoomZJHInstance.getSexBySeatid(seatid) == 2 ? "m" : "f"));
            RoomZJHInstance.call_cnt = 0;
        }
    },


    handler_server_bet_err_uc: function (val) {
        cc.log("handler_server_bet_err_uc");
    },

    handler_server_game_end_bc: function (val) {
        cc.log("RoomZJHSetTablehandler_server_game_end_bc");

        if (this.room_Action != null) {
            this.room_Action.setButtonEnable(RoomZJHConfig.tag_callall, true);
            this.room_Action.retractAllButton();
            this.room_Action.resetAllBetButton();
            this.room_Action.hideRaiseLayer();
            this.room_Action.hideCompareLayer();
            this.room_Action.setCallAll(false);
            this.room_Action.setButtonVisible(RoomZJHConfig.tag_see, false);
        }
        this.room_Card.hideAllCard();
        this.room_Card.hideAllFlag();
        this.room_TipHideAllCardTypeTips();
        this.stopAllTimer();
        RoomZJHInstance.callAll = false;
//    RoomZJHInstance.isEnterWait = true;
        RoomZJHInstance.isEnterWait = false;
        RoomZJHInstance.isMy = false;
        RoomZJHInstance.gameReset = false;
        RoomZJHInstance.state = this.END_GAME;

        var winSeats;

        for (var i = 0; i < val["players"].length; i++) {
            var seatid = val["players"][i]["seatid"];
            var win_money = val["players"][i]["win_money"];
            var money = val["players"][i]["money"];
            var total_win = val["players"][i]["total_win"];
            var total_board = val["players"][i]["total_board"];

            var extra_money = val["players"][i]["extra_money"]; //是否要出喜钱
            var pos = RoomZJHInstance.getPosBySeatid(seatid);

            var player = RoomZJHInstance.players[seatid];
            var roomPlayer = this.players[pos];

            player.money = money;
            player.total_board = total_board;
            player.total_win = total_win;

            if (win_money > 0) {
                roomPlayer.showWinEffect();
                winSeats.push(pos);
            }
            roomPlayer.showBalanceNumber(win_money);
            roomPlayer.updateInfo();

            //显示卡牌
            if (RoomZJHInstance.seatid != -1 && val["players"][i]["holes"].length > 0) {
                var showCard = false;
                if (RoomZJHInstance.isMyPlayer(seatid)) {
                    if (this.room_Action) {
                        this.room_Action.setButtonVisible(RoomZJHConfig.tag_show_card, true);
                    }

                    if (RoomZJHInstance.IsMySee || val["players"][i]["compare_seatids"].length > 0) {
                        showCard = true;
                    }
                }
                if (!showCard) {
                    for (var j = 0; j < val["players"][i]["compare_seatids"].length; j++) {
                        var compareId = val["players"][i]["compare_seatids"][j];
                        if (RoomZJHInstance.isMyPlayer(compareId)) {
                            showCard = true;
                            break;
                        }
                    }
                }


//          必须亮牌
                if (!showCard) {
                    if (extra_money > 0) {
                        showCard = true;
                    }
                }

                if (showCard && (RoomZJHInstance.roundEnd == false)) {
                    var values;
                    var cardType = val["players"][i]["card_type"];
                    for (var k = 0; k < val["players"][i]["holes"].length; k++) {
                        values.push(val["players"][i]["holes"][k]);
                    }
                    this.room_Card.showPlayerCard(pos, values, cardType);
                    this.hasShowCards[pos] = 1;
                }
            }
        }

//     room_Action->setButtonVisible(RoomZJHConfig::tag_show_card, true);

        //筹码
        var callfunc = cc.callFunc(new function () {
            if (RoomZJHInstance.seatid != -1) {
                for (var i = 0; i < val["players"].length; i++) {
                    var seatid = val["players"][i]["seatid"];
                    var extra_money = val["players"][i]["extra_money"]; //是否要出喜钱
                    var pos = RoomZJHInstance.getPosBySeatid(seatid);
                    if (extra_money < 0) {
                        if (this.room_Chip) {
                            this.room_Chip.sendBet(pos, Math.abs(extra_money), 0);
                            Sound.getInstance().playGameEffect(getResPath("Sound/zjh/g_addchip.mp3"));
                        }
                    }
                }
            }
        });


        this.runAction(cc.sequence(cc.delayTime(2), callfunc));


        var self = this;
        var callfunc2 = cc.callFunc(new function () {
            self.room_Chip.sendWinChip(winSeats, 0);
        });
        this.runAction(cc.sequence(cc.delayTime(3), callfunc2));


        if (this.m_showGameEnd) {
            this.runAction(cc.sequence(cc.delayTime(6),
                cc.callFunc(this.showGameEndLayer, this)));
        }
    },

    //万人场新加
    handler_server_all_bet_bc: function (val) { //所有玩家的投注 定时刷新
        cc.log(__String.createWithFormat("RoomZJHSetTablehandler_server_all_bet_bc:%1", val));
        var total_count = val["total_count"];
        if (total_count > RoomZJHInstance.total_count) {

            RoomZJHInstance.total_count = total_count;
            for (var i = 0; i < val["bets"].length; i++) {
                var seatid = val["bets"][i]["seatid"];
                var value = val["bets"][i]["bet_money"];
                var oldvalue = RoomZJHInstance.totalBet[seatid];
                if (oldvalue < value) {
                    //有更新
                    var pos = RoomZJHInstance.getPosBySeatid(seatid);
                    var RoomZJHPlayer = this.players[pos];
                    RoomZJHInstance.players[seatid].allBetMoney = value;
                    this.room_Chip.sendPerBet(pos, value, val["guests"]);
                    RoomZJHInstance.totalBet[seatid] = value;
                    RoomZJHInstance.AllPerBetInfo = val["guests"];
                }
            }
        }
    },

    handler_server_my_bet_uc: function (val) {
        cc.log("handler_server_my_bet_uc");
    },

    handler_server_win_uc: function (val) {
        cc.log("handler_server_win_uc");
        ZJHModel.getInstance().money = val["money"];
        var addMoney = val["prize"];
        RoomZJHInstance.MyWinMoney = addMoney;

        if (this.room_Info != null) {
            this.room_Info.setMyInfo(ZJHModel.getInstance().money, ZJHModel.getInstance().rmb);
        }
        PlatformHelper.showToast(__String.createWithFormat("恭喜你，万人场投注押中%1", addMoney));
    },

    handler_server_bet_start_bc: function (val) {
        cc.log("handler_server_bet_start_bc");
        RoomZJHInstance.canBuyMore = val["betting_type"];
        RoomZJHInstance.canBuyMore = 1;
        this.room_Tip.showTime(-1, RoomZJHInstance.m_nBettingTime);
        if (RoomZJHInstance.seatid != -1) {
            return;
        }

        RoomZJHInstance.sendBeting = 0;
        RoomZJHInstance.beting = 1;
        RoomZJHInstance.state = this.START_BET;

        if (this.room_Info != null) {
            this.room_Info.setMyBetVisible(false);
        }
        if (this.room_Action != null) {
            this.room_Action.hideBetBtn(true);
        }
        if (this.room_Info != null) {
            this.room_Info.setMyInfo(ZJHModel.getInstance().money, ZJHModel.getInstance().rmb);
        }

        for (var i = 0; i < val["seatids"].length; i++) {
            var seatid = val["seatids"][i];
            var p = RoomZJHInstance.players[seatid];
            p.ready = 1;
        }
    },

    handler_server_bet_stop_bc: function (val) {
        cc.log("handler_server_bet_stop_bc");
        RoomZJHInstance.state = this.STOP_BET;
        for (var i = 0; i < val["bets"].length; i++) {
            var seatid = val["bets"][i]["seatid"];
            var value = val["bets"][i]["bet_money"];
            var pos = RoomZJHInstance.getPosBySeatid(seatid);
            var RoomZJHPlayer = this.players[pos];
            RoomZJHInstance.players[seatid].allBetMoney = value;

        }
        if (this.room_Info != null) {
            this.room_Info.hideBetAction();
        }

        RoomZJHInstance.beting = 0;

        if (RoomZJHInstance.seatid != -1) {
            this.room_Tip.hideTime();
            if (this.room_Action != null) {
                this.room_Action.hideBetBtn(false);
            }
        }

        if (RoomZJHInstance.seatid == -1) {
            if (this.room_Info != null) {
                this.room_Info.setMyInfo(ZJHModel.getInstance().money, ZJHModel.getInstance().rmb);
            }
        }
    },

    resetGame: function () {
        RoomZJHInstance.isEnterWait = false;
        RoomZJHInstance.isAllIn = false;
        RoomZJHInstance.total_count = 0;
        RoomZJHInstance.MyWinMoney = 0;
        RoomZJHInstance.MyBetMoney = 0;
        this.room_Card.getParent().reorderChild(this.room_Card, 1);
        RoomZJHInstance.IsMySee = false;
        RoomZJHInstance.roundEnd = false;

        this.room_Card.resetCard();//重置所有牌的位置
        this.room_Card.hideAllCard();
        this.room_Card.hideAllFlag();
        this.room_Chip.removeAllChips();
        this.room_Card.reset();
        this.room_Tip.HideAllCardTypeTips();

        for (var i = 0; i < 5; i++) {
            this.hasShowCards[i] = 0;
        }

        if (this.room_Action != null) {
            this.room_Action.setAllBetButtonVisible(false);
            this.room_Action.setReadyVisible(false);
            this.room_Action.setButtonflag(false);
            this.room_Action.setButtonVisible(RoomZJHConfig.tag_see, false);
            this.room_Action.setButtonVisible(RoomZJHConfig.tag_show_card, false);
        }
        //
        if (RoomZJHInstance.seatid == -1) {
            if (this.room_Info != null) {
                this.room_Info.setMyInfo(ZJHModel.getInstance().money, ZJHModel.getInstance().rmb);
            }
        }

        RoomZJHInstance.cur_round = 0;
        RoomZJHInstance.cur_bet = 0;
        RoomZJHInstance.total_bet = 0;
        if (this.room_Tip != null) {
            this.room_Tip.resetTipValue();
        }

        this.resetPlayer();
        this.syncPlayerInfo();
        RoomZJHInstance.balanceOver = true;
    },

    handler_server_game_preready_bc: function (val) {
        cc.log("handler_server_game_preready_bc");

        //下一局准备开始 状态清除
        for (var i = 0; i < 5; i++) {
            var pos = RoomZJHInstance.getPosBySeatid(i);
            var player = RoomZJHInstance.players[i];
            if (player.uid != -1) {
                var rp = this.players[pos];
                rp.setLostVisible(false);
                rp.setThinkVisible(false);
            }
        }

        if (RoomZJHInstance.seatid != -1) {
            if (this.room_Action != null) {
                this.room_Action.setReadyVisible(true);
                this.room_Action.setButtonVisible(RoomZJHConfig.tag_show_card, false);
            }

        } else {
            if (this.room_Action != null) {
                this.room_Action.setReadyVisible(false);
            }
        }
    },


    handler_server_chat_bc: function (val) {

    },

    handler_server_face_bc: function (val) {
        cc.log("handler_server_face_bc");

        var seatid = val["seatid"];
        var faceid = val["faceid"];

        var pos = RoomZJHInstance.getPosBySeatid(seatid);
        this.room_Tip.showFace(pos, faceid);
    },

    handler_server_bigface_bc: function (val) {
        cc.log("handler_server_bigface_bc");

        var bseatid = val["seatid"];
        var eseatid = val["target_seatid"];
        var faceid = val["type"];
        var seat_num = val["seat_num"];
        var eseat_num = val["target_seat_num"];
        if (bseatid != -1 && eseatid != -1) {
            //玩牌玩家互动
            var bpos = RoomZJHInstance.getPosBySeatid(bseatid);
            var epos = RoomZJHInstance.getPosBySeatid(eseatid);

            var pm = RoomZJHInstance.players[bseatid];
            if (pm.uid == 0) {
                return;
            }

            this.room_Tip.showBigFace(bpos, epos, faceid, pm.sex);
        }
        else {
            //外围--玩家两者之间互动
            var type = 0;
            var bpos;
            var epos;
            var pm;
            if (seat_num > 0 && eseat_num > 0 && eseatid == -1) {
                //外围->外围
                type = 1;
                pm = RoomZJHInstance.Perplayers[seat_num - 1];
                bpos = seat_num - 1;
                epos = eseat_num - 1;
            }
            else if (seat_num > 0 && eseatid != -1 && eseat_num == 0) {
                //外围->玩家
                type = 2;
                bpos = seat_num - 1;
                epos = RoomZJHInstance.getPosBySeatid(eseatid);
                pm = RoomZJHInstance.Perplayers[seat_num - 1];
            }
            else if (bseatid != -1 && eseatid == -1 && eseat_num > 0) {
                //玩家->外围
                type = 3;
                bpos = RoomZJHInstance.getPosBySeatid(bseatid);
                epos = eseat_num - 1;
                pm = RoomZJHInstance.players[bseatid];
            }
            if (!pm) {
                return;
            }
            this.showPerBigFace(bpos, epos, faceid, pm.sex, type);
        }
    },

    handler_server_update_info_uc: function (val) {
        cc.log(__String.createWithFormat("RoomZJHSetTablehandler_server_update_info_uc:%1", val));

        var seadid = val["seatid"];
        var myPlayer = RoomZJHInstance.players[seadid];
        var uid = val["uid"];
        var pos = RoomZJHInstance.getPosBySeatid(seadid);
        if (myPlayer && uid == myPlayer.uid) {
            myPlayer.money = val["money"];
            myPlayer.rmb = val["rmb"];
            myPlayer.coin = val["coin"];
            this.players[pos].updateInfo();
        }
        if (pos == 0) {
            this.syncPlayerInfo();
        }
        //万人场

        if (uid == ZJHModel.getInstance().uid) {
            ZJHModel.getInstance().rmb = val["rmb"];
            ZJHModel.getInstance().money = val["money"];
            ZJHModel.getInstance().coin = val["coin"];
            if (this.room_Info != null) {
                this.room_Info.setMyInfo(ZJHModel.getInstance().money, ZJHModel.getInstance().rmb);
            }

            if (RoomZJHInstance.MyIsPer) {
                var seatid = RoomZJHInstance.per_seatid;
                var player = RoomZJHInstance.Perplayers[seatid];
                if (player != null) {
                    return;
                }
                player.money = ZJHModel.getInstance().money;
                player.rmb = ZJHModel.getInstance().rmb;
                player.coin = ZJHModel.getInstance().coin;
                var MP = this.Perplayers[seatid];
                if (MP != null) {
                    MP.updateInfo();
                }

            }

        }
    },

    handler_server_use_prop_uc: function (val) {
        cc.log("handler_server_use_prop_uc");
        var seatid = val["seatid"];
        var propid = val["prop_id"];
        var pos = RoomZJHInstance.getPosBySeatid(seatid);

        var myPlayer = RoomZJHInstance.players[seatid];
        if (myPlayer && propid == PROP_ITEM.CHANGE_CARD) {
            myPlayer.huanpaika = val["change_card"];
            myPlayer.huanpaiTimes = val["is_change"];
            for (var i = 0; i < 3; i++) {
                this.room_Card.showHoleCard2(i, val["holes"][i]);
                myPlayer.myCard[i] = val["holes"][i];
            }
        }

        if (pos == 0) {
            this.syncPlayerInfo();
        }
    },


    handler_server_use_prop_bc: function (val) {
        cc.log("handler_server_use_prop_bc");
        var seatid = val["seatid"];
        var propid = val["prop_id"];
        var pos = RoomZJHInstance.getPosBySeatid(seatid);

        var myPlayer = RoomZJHInstance.players[seatid];

        if (myPlayer && propid == PROP_ITEM.CHANGE_CARD) {
            myPlayer.huanpaika = val["change_card"];
        }
        else if (myPlayer && propid == PROP_ITEM.FORBIDDEN_CARD) {
            myPlayer.jinbika = val["forbidden_card"];
            myPlayer.jinbiRound = val["is_forbidden"];
            // this.room_Card.setJinBiRound(pos, myPlayer.jinbiRound);
        }
        else if (myPlayer && propid == PROP_ITEM.DOUBLE_CARD_FOUR) {
            myPlayer.fanbeika = val["double_card"];
            myPlayer.fanbeiRound = val["is_double"];

            myPlayer.fanbeiNums = val["is_double"];
            this.room_Card.setFanBeiRound(pos, myPlayer.fanbeiRound, myPlayer.fanbeiNums);
        }
        if (pos == 0) {
            this.syncPlayerInfo();
        }
    },


    handler_server_up_table_bc: function (val) {
        cc.log("handler_server_up_table_bc");
        var seatid = val["seatid"];
        var player = RoomZJHInstance.players[seatid];
        player.reset();

        player.uid = val["uid"];

        player.seatid = val["seatid"];
        player.ready = val["ready"];
        player.betting = val["betting"];
        player.see = val["see"];
        player.role = val["role"];
        player.status = val["status"];
        player.bet = val["bet"];
        player.dsc = val["ps"];

        player.name = val["name"];
        player.sex = val["sex"];
        player.avatar = val["avatar"];
        player.birthday = val["birthday"];
        player.zone = val["zone"];
        player.exp = val["exp"];
        player.rmb = val["rmb"];
        player.money = val["money"];
        player.coin = val["coin"];
        player.total_board = val["total_board"];
        player.total_win = val["total_win"];
        player.pcount = val["pcount"];
        player.vtime = val["vtime"];
        player.vlevel = val["vlevel"];
        player.matchMoney = val["match_stake"];
        player.lifeNums = val["play_num"];
        player.allBetMoney = 0;
        player.myBetNums = 0;

        if (player.uid == ZJHModel.getInstance().uid) {
            RoomZJHInstance.waitUpdate = true;
            ZJHModel.getInstance().myPoint = RoomZJHConfig.getBetPos(0);
            ZJHLogic.resetResponse();
            ZJHLogic.sendRoomInfo();
            this.removeChildByTag(250, true);
            //RoomZJHInstance::getInstance()->send_logData(CLIENT_UPTABLE_APPLY_REQ);
        }
        else {
            var pos = RoomZJHInstance.getPosBySeatid(seatid);
            var RoomZJHPlayer = this.players[pos];
            RoomZJHPlayer.login(player.seatid);
            RoomZJHPlayer.setLostVisible(false);
            RoomZJHPlayer.setThinkVisible(false);
            if (player.betting == 0) {
                RoomZJHPlayer.setMaskVisible(true);
            }
        }
        JsUtils.postNotifi("notify_room_check_ip");
        this.getPlayerList();
    },

    handler_server_down_table_bc: function (val) {
        cc.log("handler_server_down_table_bc");

        var seatid = val["seatid"];
        var player = RoomZJHInstance.players[seatid];
        player.reset();
        var pos = RoomZJHInstance.getPosBySeatid(seatid);
        var roomPlayer = this.players[pos];
        roomPlayer.reset();
        roomPlayer.stopTimer();
        roomPlayer.logout();

        roomPlayer.setLostVisible(false);
        roomPlayer.setThinkVisible(false);

        this.room_Card.hideCard(pos);
        this.room_Card.hideFlag(pos);
        if (RoomZJHInstance.isMyPlayer(seatid)) {
            RoomZJHInstance.seatid = -1;
            ZJHModel.getInstance().mySeatid = RoomZJHInstance.getInstance().seatid;
            if (this.room_Action != null) {
                this.room_Action.retractAllButton();
                this.room_Action.hideRaiseLayer();
                this.room_Action.hideCompareLayer();
//            room_Action.setButtonVisible(RoomZJHConfig::tag_up_table, true);
                if (this.roomVerticalLayout != null) {
                    this.roomVerticalLayout.setGamePlayStatu(false);
                }
                room_Action.setButtonVisible(RoomZJHConfig.tag_show_card, false);
                room_Action.setReadyVisible(false);
            }

            if (RoomZJHInstance.seatid != -1) {
                this.roomVerticalLayout.setFastSoundVis(true);
            }
            else {
                this.roomVerticalLayout.setFastSoundVis(false);
            }
        }

        this.getPlayerList();
    },

    handler_server_request_up_table_uc: function (val) {
        this.up_table_Info_million = val;
        RoomZJHInstance.lastRequstListTime = 0;
        var roomDNUpList = this.getChildByTag(250);
        if (roomDNUpList != null) {
            roomDNUpList.removeAllChildrenWithCleanup(true);
            roomDNUpList.myInit(val, 1);
        }
    },

    handler_server_request_down_table_uc: function (val) {
        var seatid = val["seatid"];
        if (RoomZJHInstance.isMyPlayer(seatid)) {
            PlatformHelper.showToast("本局结束后下桌");
        }
        return;
    },

    handler_server_up_table_info_uc: function (val) {
        this.up_table_Info_million = val;
    },

    handler_server_down_guest_seat_bc: function (val) {

    },

    handler_server_up_guest_seat_bc: function (val) {

    },

    handler_server_up_guest_seat_error_uc: function (val) {
        cc.log("handler_server_up_guest_seat_error_uc");
        var code = val["code"];
        if (code == 101) {
            PlatformHelper.showToast("此座位已被占用");
        }
        else if (code == 102) {
            PlatformHelper.showToast("您的金币不够上桌最低限制");
        }
        else if (code == 103) {
            PlatformHelper.showToast("您已经是庄家");
        }
        else if (code == 104) {
            PlatformHelper.showToast("您已在桌上");
        }
    },

    handler_server_up_get_PerPlayer_info: function (val) {
        cc.log("handler_server_up_get_PerPlayer_info");
    },

    handler_server_short_uptable_info: function (val) {
        cc.log("handler_server_short_uptable_info");
        var nums = val["playes"].length;
        if (nums > 3) {
            nums = 3;
        }
        for (var i = 0; i < nums; i++) {
            var uid = val["playes"][i]["uid"];
            var name = val["playes"][i]["name"];
            var money = val["playes"][i]["money"];
            var avatar = val["playes"][i]["avatar"];
            var vlevel = val["playes"][i]["vlevel"];
            var sex = val["playes"][i]["sex"];
            var player = RoomZJHInstance.Upplayers[i];
            player.uid = uid;
            player.seatid = i;
            player.vlevel = vlevel;
            player.avatar = avatar;
            player.money = money;
            player.name = name;
            player.sex = sex;
            var RoomZJHPlayer = this.Upplayers[i];
            RoomZJHPlayer.login(i);
        }
        if (nums < 3) {
            for (var i = nums; i < 3; i++) {
                var player = RoomZJHInstance.Upplayers[i];
                player.reset();
                var room = this.Upplayers[i];
                room.reset();
            }
        }
    },

    handler_server_second_card_bc: function (val) {
        cc.log("handler_server_second_card_bc");
        this.room_Card.beginFaPai();
        RoomZJHInstance.isFapai = true;
        RoomZJHInstance.isEnterWait = true;
        var delay = 0.50 - 0.17;
        var cardNums = 4;
        for (var i = 3; i < cardNums; i++) {
            for (var j = 0; j < val["players"].length; j++) {
                delay += 0.17;
                var seatid = val["players"][j]["seatid"];
                var pos = RoomZJHInstance.getPosBySeatid(seatid);
                var cardValue = val["players"][j]["cards"][0];
                this.room_Card.showCard(pos, i, delay, cardValue);
            }
        }
        this.schedule(this.faPaiOver);
        this.changeCoinLayer(100);
    },

    handler_server_third_card_bc: function (val) {
        cc.log("handler_server_third_card_bc");

        this.room_Card.beginFaPai();
        RoomZJHInstance.isFapai = true;
        RoomZJHInstance.isEnterWait = true;
        var delay = 0.50 - 0.17;
        var cardNums = 5;
        for (var i = 4; i < cardNums; i++) {
            for (var j = 0; j < val["players"].length; j++) {
                delay += 0.17;
                var seatid = val["players"][j]["seatid"];
                var pos = RoomZJHInstance.getPosBySeatid(seatid);
                var cardValue = val["players"][j]["cards"][0];
                this.room_Card.showCard(pos, i, delay, cardValue);
            }
        }

        this.changeCoinLayer(100);

        this.schedule(this.faPaiOver);
    },

    handler_server_pool_info_uc: function (val) {
        cc.log("handler_server_pool_info_uc");
    },

    handler_server_all_player_info: function (val) {

    },

    UpListCallBack: function (index) {

    },

    handler_server_get_box_bc: function (val) {
        cc.log("handler_server_get_box_bc");
        var seatid = val["seatid"];
        var type = val["type"];
        var value = val["value"];
        var pos = RoomZJHInstance.getPosBySeatid(seatid);
        if (pos != 0) {
            this.room_Tip.showBox(pos, type, value);
        }
    },

    handler_server_dz_game_start_res: function (val) {
        cc.log("handler_server_dz_game_start_res");

        if (this.getChildByName("game_end_layer")) {
            return;
        }
        var endTime = val["ts"];
        ZJHModel.getInstance().roomToShareView(RoomZJHInstance.curRoomID);
    },

    handler_server_round_end: function (val) {
        cc.log("handler_server_round_end");
        RoomZJHInstance.roundEnd = true;
        for (var i = 0; i < val["players"].length; i++) {
            var seatid = val["players"][i]["seatid"];
            var pos = RoomZJHInstance.getPosBySeatid(seatid);

            //显示卡牌
            {
                var values;
                var cardType = val["players"][i]["card_type"];
                for (var k = 0; k < val["players"][i]["holes"].length; k++) {
                    values.push(val["players"][i]["holes"][k]);
                }
                this.room_Card.showPlayerCard(pos, values, cardType);
            }
        }
    },

    handler_server_other_show_bc: function (val) {
        cc.log("handler_server_other_show_bc");

        var seatId = val["seatid"];
        var pos = RoomZJHInstance.getPosBySeatid(seatId);
        if (this.hasShowCards[pos]) {
            return;
        }

        var values;
        var cardType = val["card_type"];
        for (var k = 0; k < val["holes"].length; k++) {
            values.push_back(val["holes"][k]);
        }
        this.room_Card.showPlayerCard(pos, values, cardType);
    },

    handler_game_heartbeat_uc: function (val) {
        cc.log("handler_game_heartbeat_uc");
        RoomZJHInstance.lastHeartBeatTime = time(null);
    },

    actionCallBack: function (index) {
        cc.log("actionCallBack[%1]", index);
//    test(0);
//    return;

        if (index == RoomZJHConfig.tag_callall)//call aloway
        {
            RoomZJHInstance.callAll = !RoomZJHInstance.callAll;
            if (RoomZJHInstance.isMy && RoomZJHInstance.callAll && RoomZJHInstance.cur_round <= RoomZJHInstance.max_round) {
                cc.log("---------%1", RoomZJHInstance.cur_seat);
                this.actionCallBack(RoomZJHConfig.tag_call);
            }
            if (RoomZJHInstance.callAll == false && room_Action && RoomZJHInstance.isMy) {
//            room_Action->retractAllButton();
//            room_Action->stretchAllButton(0, 0.2);
                this.room_Action.showMyBetButtons();
            }
            this.overListerCallAll();
        }
        else if (index == RoomZJHConfig.tag_call) {
            this.overListerCallAll();

            var player = RoomZJHInstance.players[RoomZJHInstance.seatid];

            var total = RoomZJHInstance.cur_bet * (player.see == 1 ? 2 : 1);

            if (ZJHModel.checkNoMoney(total)) {
                return;
            }

            RoomZJHInstance.isMy = false;
            for (var i = 0; i < (player.see == 1 ? 2 : 1); i++) {
                this.room_Chip.sendBet(0, RoomZJHInstance.cur_bet, 0);
                Sound.getInstance().playGameEffect(getResPath("Sound/zjh/g_addchip.mp3"));
            }

            ZJHLogic.sendBetAction(CLIENT_ACTION.PLAYER_CALL);
            //RoomZJHInstance::getInstance()->send_logData(CLIENT_BET_REQ);//下注

            if (this.room_Action != null) {
                this.room_Action.setButtonEnable(RoomZJHConfig.tag_compare, false);
                this.room_Action.setButtonEnable(RoomZJHConfig.tag_raise, false);
                this.room_Action.setButtonEnable(RoomZJHConfig.tag_max, false);
                this.room_Action.setButtonEnable(RoomZJHConfig.tag_call, false);
            }

            this.stopAllTimer();
        }
        else if (index == RoomZJHConfig.tag_raise) {
            this.overListerCallAll();
            if (this.room_Action != null) {
                this.room_Action.showRaiseLayer();
                this.room_Action.setRaiseCallback(this.raiseCallBack, this);
            }
        }
        else if (index == RoomZJHConfig.tag_compare) {
            this.overListerCallAll();
            var targets;
            var conf = RoomZJHInstance.compare_condition;
            for (var i = 0; i < 5; i++) {
                var player = RoomZJHInstance.players[i];

                if (conf != 2) {
                    if (player.betting == 1 && player.jinbiRound <= RoomZJHInstance.cur_round) {
                        var pos = RoomZJHInstance.getPosBySeatid(player.seatid);
                        if (pos == 0) {
                            continue;
                        }
                        targets.push(pos);
                    }
                } else {
                    if (player.betting == 1 && player.see == 1 && player.jinbiRound <= RoomZJHInstance.cur_round) {
                        var pos = RoomZJHInstance.getPosBySeatid(player.seatid);
                        if (pos == 0) {
                            continue;
                        }
                        targets.push(pos);
                    }
                }

            }
            if (this.room_Action != null) {
                this.room_Action.setCompareCallback(this.compareCallBack, this);
                if (targets.length > 0) {
                    room_Action.showCompareLayer(targets);
                } else {
                    PlatformHelper.showToast("没有已看牌玩家不能比");
                }

            }
        }
        else if (index == RoomZJHConfig.tag_see) {
            if (RoomZJHInstance.cur_round > RoomZJHInstance.see_round) {
                this.overListerCallAll();
                ZJHLogic.sendBetAction(CLIENT_ACTION.PLAYER_SEE);

                if (this.room_Action != null) {
                    this.room_Action.setButtonEnable(RoomZJHConfig.tag_compare, false);
                    this.room_Action.setButtonEnable(RoomZJHConfig.tag_raise, false);
                    this.room_Action.setButtonEnable(RoomZJHConfig.tag_max, false);
                    this.room_Action.setButtonEnable(RoomZJHConfig.tag_call, false);
//                room_Action.setButtonVisible(RoomZJHConfig.tag_see, false);
                }

                if (RoomZJHInstance.cur_seat == RoomZJHInstance.seatid) {
                    this.stopAllTimer();
                }
            }
            else {
                var see_round = RoomZJHInstance.see_round;
                PlatformHelper.showToast(__String.createWithFormat("在第%1回合可以看牌", see_round + 1));
            }

        }
        else if (index == RoomZJHConfig.tag_fold) {
            var foldTip = sys.localStorage.getItem("fold_tip");
            if (foldTip === "true") {
                var alert = new AlertDialogFold();
                var self = this;
                alert.setCallBack(function (name) {
                    if (name == "fold_confirm") {
                        self.doFold();
                    }
                });
                this.addChild(alert, this.Z_ORDER_TOP);
            }
            else {
                this.doFold();
            }
        }
        else if (index == RoomZJHConfig.tag_ready) {

        }
        else if (index == RoomZJHConfig.tag_show_card) {
            ZJHLogic.sendOtherShow();
            this.room_Action.setButtonVisible(RoomZJHConfig.tag_show_card, false);
        }

        else if (index == RoomZJHConfig.tag_my_info_btn) {


        }
        else if (index == RoomZJHConfig.tag_help) {
            ZJHLogic.sendDownTable();
        }
        else if (index == RoomZJHConfig.tag_uplist) {
            //uplist
            if ((RoomZJHInstance.lastRequstListTime != 0 && time() - RoomZJHInstance.lastRequstListTime >= 5) || RoomZJHInstance.lastRequstListTime == 0 || this.up_table_Info_million.val["rank"] == 0) {
                ZJHLogic.sendUpTable();
                RoomZJHInstance.lastRequstListTime = time();
            }
            else {
                this.handler_server_up_table_info_uc(this.up_table_Info_million);
            }
        }
        else if (index == RoomZJHConfig.tag_shop) {
        }
        else if (index == RoomZJHConfig.tag_back_swtich) {
            ZJHLogic.sendSwitchRoom();
        }
        else if (index == RoomZJHConfig.tag_pot_btn) {
            //奖池
            if ((RoomZJHInstance.lastRequestPoolTime != 0 && time() - RoomZJHInstance.lastRequestPoolTime >= 5) || RoomZJHInstance.lastRequestPoolTime == 0) {

            }
            else {
                this.handler_server_pool_info_uc(this.poolInfoJpacket);
            }
        }
        else if (index == RoomZJHConfig.tag_back_return) {
            RoomZJHInstance.isEnterWait = true;
            RoomZJHInstance.isLogOut = true;

            ZJHLogic.sendLogout();
        }
        else if (index >= RoomZJHConfig.tag_player_info_btn && index < RoomZJHConfig.tag_player_info_btn + 10) {

        }
        else if (index >= RoomZJHConfig.tag_preplayers_sitdown_btn && index < RoomZJHConfig.tag_preplayers_sitdown_btn + RoomZJHInstance.PreNums) {
            //坐下
            if (RoomZJHInstance.seatid != -1) {
                PlatformHelper.showToast(ZJHModel.getStr("room_million_guest_up_tip"));
                return;
            }
            ZJHLogic.sendUpGuest(index + 1 - RoomZJHConfig.tag_preplayers_sitdown_btn);
            RoomZJHInstance.send_data(jpacket.tostring());
        }
        else if (index == 30) {
            this.resetGame();
        }
    },


    MenuCallBack: function (index) {

    },

    startRaiseDialog: function () {
        var RoomZJHRaise = new RoomZJHRaise();
        this.addChild(RoomZJHRaise, 1000);
        RoomZJHRaise.setSelector(this.raiseCallBack, this);
    },


    raiseCallBack: function (index) {
        cc.log(__String.createWithFormat("raiseCallBack[%1]", index));

        var player = RoomZJHInstance.players[RoomZJHInstance.seatid];
        if (player == null) {
            return;
        }
        var bet_nums = RoomZJHInstance.curRaise[index];

        if (ZJHModel.getInstance().checkNoMoney(bet_nums)) {
            return;
        }

        RoomZJHInstance.isMy = false;

        for (var i = 0; i < (player.see == 1 ? 2 : 1); i++) {
            this.room_Chip.sendBet(0, bet_nums, 0);
            Sound.getInstance().playGameEffect(getResPath("Sound/zjh/g_addchip.mp3"));
        }
        ZJHLogic.sendBetAction(CLIENT_ACTION.PLAYER_RAISE, RoomZJHInstance.raise[index + 1])
        //RoomZJHInstance::getInstance()->send_logData(CLIENT_BET_REQ);

        if (this.room_Action != null) {
            this.room_Action.setButtonEnable(RoomZJHConfig.tag_compare, false);
            this.room_Action.setButtonEnable(RoomZJHConfig.tag_max, false);
            this.room_Action.setButtonEnable(RoomZJHConfig.tag_raise, false);
            this.room_Action.setButtonEnable(RoomZJHConfig.tag_call, false);
        }

        this.stopAllTimer();
    },

    compareCallBack: function (index) {
        RoomZJHInstance.isMy = false;
        cc.log("compareCallBack[%1]", index);
        var seatid = RoomZJHInstance.getSeatidByPos(0);
        var targetSeatid = RoomZJHInstance.getSeatidByPos(index);

        ZJHLogic.sendBetAction(CLIENT_ACTION.PLAYER_COMPARE, undefined, seatid, targetSeatid);
        //RoomZJHInstance::getInstance()->send_logData(CLIENT_BET_REQ);
        if (this.room_Action != null) {
            this.room_Action.setButtonEnable(RoomZJHConfig.tag_compare, false);
            this.room_Action.setButtonEnable(RoomZJHConfig.tag_max, false);
            this.room_Action.setButtonEnable(RoomZJHConfig.tag_raise, false);
            this.room_Action.setButtonEnable(RoomZJHConfig.tag_call, false);
        }

        this.stopAllTimer();
    },

    showCompare: function (fPos, tPos, lose) {
        var winSize = cc.director.getWinSize();
        var tablePos = RoomZJHConfig.getTablePos();
        var perTime = 1.0 / 12.0;


        perTime *= 1;

        var fRoomPlayer = this.players[fPos];
        var tRoomPlayer = this.players[tPos];
        fRoomPlayer.setVisible(false);
        tRoomPlayer.setVisible(false);
        this.players[lose].setMaskVisible(true);
        var seatid = RoomZJHInstance.getSeatidByPos(lose);
        if (RoomZJHInstance.isMyPlayer(seatid)) {
            if (this.room_Action != null) {
                this.room_Action.setButtonVisible(RoomZJHConfig.tag_see, false);
            }
        }

        var leftPoint = cc.p(tablePos.x - 280, tablePos.y + 65);
        var rightPoint = cc.p(tablePos.x + 280, tablePos.y + 15);

        var compareAniLayer = this.getChildByName("compare_layer");
        if (compareAniLayer == null) {
            compareAniLayer = new ccui.Layout();
            compareAniLayer.setContentSize(winSize);
            compareAniLayer.setTouchEnabled(true);
            compareAniLayer.setName("compare_layer");
            this.addChild(compareAniLayer, 100);
        }
        compareAniLayer.removeAllChildrenWithCleanup(true);

        var mask = new ccui.LayerColor(cc.color(0, 0, 0, 100));
        compareAniLayer.addChild(mask, -1);

        var leftPlayer = new RoomZJHPlayer();
        leftPlayer.setId(fPos);
        leftPlayer.login(RoomZJHInstance.getSeatidByPos(fPos));
        leftPlayer.setPosition(RoomZJHConfig.getPlayerPos(fPos));
        compareAniLayer.addChild(leftPlayer, 10);

        var rightPlayer = new RoomZJHPlayer();
        rightPlayer.setId(tPos);
        rightPlayer.login(RoomZJHInstance.getSeatidByPos(tPos));
        rightPlayer.setPosition(RoomZJHConfiggetPlayerPos(tPos));
        compareAniLayer.addChild(rightPlayer, 10);

        leftPlayer.runAction(cc.sequence(cc.moveTo(perTime * 5, leftPoint),
            cc.delayTime(perTime * 34),
            cc.moveTo(perTime * 6, RoomZJHConfig.getPlayerPos(fPos)),
            cc.callFunc(function () {
                fRoomPlayer.setVisible(true);
            }),
            cc.removeSelf()));

        leftPlayer.runAction(cc.sequence(cc.delayTime(perTime * 20),
            cc.callFunc(function () {
                if (lose == fPos) {
                    leftPlayer.showCompareLose();
                } else {
                    leftPlayer.showCompareWin();
                }
                Sound.getInstance().playGameEffect(getResPath("Sound/zjh/g_cmp_fire.mp3"));
            })));

        rightPlayer.runAction(cc.sequence(cc.moveTo(perTime * 5, rightPoint),
            cc.delayTime(perTime * 34),
            cc.moveTo(perTime * 6, RoomZJHConfig.getPlayerPos(tPos)),
            cc.callFunc(function () {
                tRoomPlayer.setVisible(true);
            }),
            cc.removeSelf()));

        rightPlayer.runAction(cc.sequence(cc.delayTime(perTime * 20),
            cc.callFunc(function () {
                if (lose == tPos) {
                    rightPlayer.showCompareLose();
                } else {
                    rightPlayer.showCompareWin();
                }
                Sound.getInstance().playGameEffect(getResPath("Sound/zjh/g_cmp_fire.mp3"));
            })));


        var sx = winSize.width / this.bg.getContentSize().width;
        var leftSp = new cc.Sprite(getResPath("Room/duizhan/dz_pk_bg_left.png"));
        leftSp.setScaleX(sx);
        leftSp.setAnchorPoint(cc.p(0, 0.5));
        var beginPos = cc.p(-leftSp.getBoundingBox().size.width, leftPoint.y);
        var endPos = cc.p(0 - 10, beginPos.y);
        leftSp.setPosition(beginPos);
        leftSp.runAction(cc.sequence(cc.moveTo(perTime * 5, endPos),
            cc.delayTime(perTime * 36),
            cc.moveTo(perTime * 5, beginPos),
            cc.removeSelf()));
        compareAniLayer.addChild(leftSp);

        var rightSp = new cc.Sprite(getResPath("Room/duizhan/dz_pk_bg_right.png"));
        rightSp.setScaleX(sx);
        rightSp.setAnchorPoint(cc.p(1, 0.5));
        beginPos = cc.p(rightSp.getBoundingBox().size.width + winSize.width, rightPoint.y);
        endPos = cc.p(winSize.width + 10, beginPos.y);
        rightSp.setPosition(beginPos);
        rightSp.runAction(cc.sequence(cc.moveTo(perTime * 5, endPos),
            cc.delayTime(perTime * 36),
            cc.moveTo(perTime * 5, beginPos),
            cc.removeSelf()));
        compareAniLayer.addChild(rightSp);

        //闪电
        var showLightning = cc.callFunc(function () {
            var leftLight = new cc.Sprite(getResPath("Room/duizhan/dz_left_light_0.png"));
            leftLight.setPosition(leftPoint);
            var leftAnimation = new cc.Animation();
            for (var i = 0; i < 5; i++) {
                leftAnimation.addSpriteFrameWithFile(__String.createWithFormat("Room/duizhan/dz_left_light_%1.png", i));
            }
            leftAnimation.setDelayPerUnit(perTime);
            var leftAnimate = new cc.Animate(leftAnimation);
            leftLight.runAction(cc.sequence(cc.repeat(leftAnimate, 2),
                cc.removeSelf()));
            compareAniLayer.addChild(leftLight, 20);

            var rightLight = new cc.Sprite(getResPath("Room/duizhan/dz_right_light_0.png"));
            rightLight.setPosition(rightPoint);
            var rightAnimation = new cc.Animation();
            for (var i = 0; i < 5; i++) {
                rightAnimation.addSpriteFrameWithFile(__String.createWithFormat("Room/duizhan/dz_right_light_%1.png", i));
            }
            rightAnimation.setDelayPerUnit(perTime);
            var rightAnimate = new cc.Animate(rightAnimation);
            rightLight.runAction(cc.sequence(cc.repeat(rightAnimate, 2),
                cc.removeSelf()));
            compareAniLayer.addChild(rightLight, 20);

            Sound.getInstance().playGameEffect(getResPath("Sound/zjh/g_cmp_light.mp3"));
        });
        compareAniLayer.runAction(cc.sequence(cc.delayTime(perTime * 8), showLightning));

        //比、牌
        var showWord = cc.callFunc(function () {
            var biSp = new cc.Sprite(getResPath("Room/duizhan/pkzi1.png"));
            biSp.setScale(2.08);
            biSp.setOpacity(255 * 0.57);
            biSp.setPosition(cc.p(tablePos.x - 120, leftPoint.y));
            biSp.runAction(cc.sequence(cc.spawn(ccfadeTo(perTime * 5, 255),
                cc.easeElasticOut(cc.scaleTo(perTime * 5, 1))),
                cc.delayTime(perTime * 25),
                cc.fadeOut(perTime * 5),
                cc.removeSelf()));
            compareAniLayer.addChild(biSp, 15);

            var paiSp = new cc.Sprite(getResPath("Room/duizhan/pkzi.png"));
            paiSp.setScale(2.08);
            paiSp.setOpacity(255 * 0.57);
            paiSp.setPosition(cc.p(tablePos.x + 120, rightPoint.y));
            paiSp.runAction(cc.sequence(cc.spawn(cc.fadeTo(perTime * 5, 255),
                cc.easeElasticOut(cc.scaleTo(perTime * 5, 1))),
                cc.delayTime(perTime * 25),
                cc.fadeOut(perTime * 5),
                cc.removeSelf()));
            compareAniLayer.addChild(paiSp, 15);
        });

        compareAniLayer.runAction(cc.sequence(cc.delayTime(perTime * 4), showWord));
        compareAniLayer.runAction(cc.sequence(cc.delayTime(perTime * 47),
            cc.callFunc(function () {
                compareAniLayer.setTouchEnabled(false);
                compareAniLayer.removeAllChildrenWithCleanup(true);
            })));
    },

    showWinCard: function () {
        this.room_Card.hideAllFlag();
        this.room_Card.getParent().reorderChild(this.room_Card, -1);

        cc.log(__String.createWithFormat("------RoomZJHSetTable::showWinCard:%1", val));
        var winSeatid = val["win_seatid"];
        var pos = RoomZJHInstance.getPosBySeatid(winSeatid);

        var winCardType = -1;
        if (val["win_card_type"] != null) {
            winCardType = val["win_card_type"];
        }
        if (RoomZJHInstance.isMyPlayer(winSeatid)) {
            var player = RoomZJHInstance.players[winSeatid];
            if (player.see != 1) {
                this.room_Card.showWinCard(pos, this.room_Card.valueArr[0], this.room_Card.valueArr[1], this.room_Card.valueArr[2], this.room_Card.valueArr[3], this.room_Card.valueArr[4], winCardType);
            }

        }
        else {
            var cardVal;
            for (var i = 0; i < val["holes"].length; i++) {
                cardVal[i] = val["holes"][i];
            }
            if (val["holes"].length < 5) {
                for (var i = val["holes"].length; i < 5; i++) {
                    cardVal[i] = 0;
                }
            }

            this.room_Card.showWinCard(pos, cardVal[0], cardVal[1], cardVal[2], cardVal[3], cardVal[4], winCardType);
        }

        var len = val["players"].length;
        for (var i = 0; i < len; i++) {
            var seatid = val["players"][i]["seatid"];
            var player = RoomZJHInstance.players[seatid];
            if (player && player.uid == 0) {
                continue;
            }

            player.rmb = val["players"][i]["rmb"];
            player.money = val["players"][i]["money"];
            player.coin = val["players"][i]["coin"];
            player.matchMoney = val["players"][i]["match_stake"];
            player.lifeNums = val["players"][i]["play_num"];


            var pos = RoomZJHInstance.getPosBySeatid(seatid);
            var RoomZJHPlayer = this.players[pos];
            RoomZJHPlayer.updateInfo();
            this.room_Tip.setPlayerBet(pos, player.bet);
            if (winSeatid == seatid) {
                var cardType = val["players"][i]["card_type"];
                this.room_Tip.showCardTypeTips(pos, cardType);

            }


        }
        for (var i = 0; i < val["guests"].length; i++) {
            var seatid = val["guests"][i]["seat_num"] - 1;
            var player = RoomZJHInstance.Perplayers[seatid];
            player.money = val["guests"][i]["money"];
            var RoomZJHPlayer = this.Perplayers[seatid];
            RoomZJHPlayer.updateInfo();

        }
        this.syncPlayerInfo();
    },


    showBalance: function () {

    },

    changeCoinLayer: function (zorder) {
        var coin = this.getChildByTag(13);
        this.reorderChild(coin, zorder);
    },

    timeoutCallback: function (index) {
        cc.log(__String.createWithFormat("timeoutCallback[%1]", index));
        var rms = this.getChildByTag(223);
        if (rms != null) {
            rms.removeSelf();
        }
    },

    doFold: function () {
        this.overListerCallAll();
        RoomZJHInstance.isMy = false;
        ZJHLogic.sendBetAction(CLIENT_ACTION.PLAYER_FOLD);
        if (RoomZJHInstance.cur_seat == RoomZJHInstance.seatid) {
            this.stopAllTimer();
        }

        if (this.room_Action != null) {
            this.room_Action.retractAllButton();
        }
    },

    playerClickCallback: function (pos) {
        var seatid = RoomZJHInstance.getSeatidByPos(pos);
        if (seatid == RoomZJHInstance.seatid) {

            if (this.roomVerticalLayout != null) {
                this.roomVerticalLayout.showPlayerList();
            }
        }
        else {
            if (RoomZJHInstance.seatid == -1) {
                //var player = RoomZJHInstance.players[RoomZJHInstance.getSeatidByPos(pos)];
                // var userInfo = RoomZJHUserInfo(player);
                // this.addChild(userInfo, this.Z_ORDER_TOP);
            }
            else {
                this.showBigfaceMenu(pos);
            }
        }
    },

    showBigfaceMenu: function (pos) {
        var bigfaceLayer = new ccui.Layer();
        bigfaceLayer.setPosition(cc.p(0, 0));
        bigfaceLayer.setName("bigface_layer");
        bigfaceLayer.setTag(pos);
        this.addChild(bigfaceLayer, 200);

        var touchListener = new cc.EventListenerTouchOneByOne();
        touchListener.onTouchBegan = function (touch, event) {
            return true;
        };
        touchListener.onTouchEnded = function (touch, envet) {
            bigfaceLayer.removeFromParentAndCleanup(true);
        };

        touchListener.setSwallowTouches(true);
        cc.director.getEventDispatcher().addEventListenerWithSceneGraphPriority(touchListener, bigfaceLayer);

        var bgSp = new cc.Sprite(getResPath("Room/duizhan/other_menu_bottom.png"));
        bgSp.setAnchorPoint(cc.p(0.5, 0));
        bgSp.setVisible(false);
        bigfaceLayer.addChild(bgSp, -1);
        var flag = 1;
        var bgPos = RoomZJHConfig.getPlayerPos(pos);
        if (pos == 1 || pos == 2) {
            bgSp.setRotation(-90);
            bgPos.x += 35;
            flag = -1;
        }
        else if (pos == 3 || pos == 4) {
            bgSp.setRotation(90);
            bgPos.x -= 35;
            flag = 1;
        }
        bgSp.setPosition(bgPos);
        bgSp.runAction(cc.sequence(cc.delayTime(0.3), cc.show()));

        var begin = RoomZJHConfig.getPlayerPos(pos);
        var end = begin;
        var radius = 155;
        var offsets = [
            cc.p(radius * Math.cos(90.0 / 180.0 * Math.PI) * flag, radius * Math.sin(90.0 / 180.0 * Maht.PI)),
            cc.p(radius * Math.cos(60.0 / 180.0 * Math.PI) * flag, radius * Math.sin(60.0 / 180.0 * Math.PI)),
            cc.p(radius * Math.cos(30.0 / 180.0 * Math.PI) * flag, radius * Math.sin(30.0 / 180.0 * Math.PI)),
            cc.p(radius * Math.cos(0.0 / 180.0 * Math.PI) * flag, radius * Math.sin(0.0 / 180.0 * Math.PI)),
            cc.p(radius * Math.cos(30.0 / 180.0 * Math.PI) * flag, -radius * Math.sin(30.0 / 180.0 * Math.PI)),
            cc.p(radius * Math.cos(60.0 / 180.0 * Math.PI) * flag, -radius * Math.sin(60.0 / 180.0 * Math.PI)),
            cc.p(radius * Math.cos(90.0 / 180.0 * Math.PI) * flag, -radius * Math.sin(90.0 / 180.0 * Math.PI))
        ];
        var icons = [
            getResPath("Room/bigface/flower/flower1.png"),
            getResPath("Room/bigface/cheers/cheers1.png"),
            getResPath("Room/bigface/kiss/kiss1.png"),
            getResPath("Room/bigface/eggs/egg1.png"),
            getResPath("Room/bigface/shoe/shoe1.png"),
            getResPath("Room/bigface/bomb/bomb1.png")
        ];

        for (var i = 0; i < 7; i++) {
            var btn = new ccui.Button(getResPath("Room/duizhan/avatar_menu_item_bg_normal.png"), getResPath("Room/duizhan/avatar_menu_item_bg_normal.png"));
            btn.setPosition(begin);
            btn.setTag(i);
            btn.addClickEventListener(this.bigfaceMenuCallback);
            bigfaceLayer.addChild(btn);

            if (i != 6) {
                var img = new cc.Sprite(icons[i]);
                img.setPosition(btn.getContentSize() / 2);
                btn.addChild(img);
            } else {
                var word = new cc.LabelTTF("玩家\n列表", "Arial", 22);
                word.setPosition(cc.p(btn.getContentSize().width / 2 + 5, btn.getContentSize().height / 2));
                btn.addChild(word);
            }

            var myEnd = end;
            myEnd = cc.pAdd(myEnd, offsets[i]);

            btn.runAction(cc.moveTo(0.3, myEnd));
        }
    },

    bigfaceMenuCallback: function (ref) {
        cc.log("click btn");
        var bigfaceLayer = this.getChildByName("bigface_layer");
        if (bigfaceLayer == null) {
            return;
        }
        var pos = bigfaceLayer.getTag();
        bigfaceLayer.removeFromParentAndCleanup(true);

        var sender = ref;
        if (ref == null) {
            return;
        }
        var faceid = sender.getTag() + 1;
        if (faceid == 7) {
            if (this.roomVerticalLayout != null)
                this.roomVerticalLayout.showPlayerList();
        } else {
            ZJHLogic.sendEmotion(faceid);
        }
    },

    syncPlayerInfo: function () {
        var myPlayer = RoomZJHInstance.players[RoomZJHInstance.seatid];
        if (myPlayer && myPlayer.uid == ZJHModel.getInstance().uid) {
            cc.log("RoomZJHSetTable.syncPlayerInfo");
            ZJHModel.getInstance().rmb = myPlayer.rmb;
            ZJHModel.getInstance().money = myPlayer.money;
            ZJHModel.getInstance().coin = myPlayer.coin;
            ZJHModel.getInstance().myBet = myPlayer.bet + 10101010;
        }
    },


    quitCallBack: function (index) {
        if (index == 1) {
            this.actionCallBack(22);
        }
    },


    checkCanCompareCard: function () {
        if (RoomZJHInstance.cur_round < RoomZJHInstance.cmp_round || RoomZJHInstance.cur_seat != RoomZJHInstance.seatid) {
            return;
        }

        if (this.room_Action != null) {
            this.room_Action.setButtonEnable(RoomZJHConfig.tag_compare, false);

            var cnt = 0;
            for (var i = 0; i < 5; i++) {
                var player = RoomZJHInstance.players[i];
                if (player && player.betting == 1 && player.jinbiRound <= RoomZJHInstance.cur_round) {
                    if (player.seatid != RoomZJHInstance.seatid) {
                        cnt++;
                    }
                }
            }

            if (RoomZJHInstance.cur_round >= RoomZJHInstance.max_round || cnt > 0) {
                this.room_Action.setButtonEnable(RoomZJHConfigtag_compare, true);
            }
        }
    },

    gameHeartBeat: function (dt) {
        ZJHLogic.sendHeartBeat();
    },

    GetMyInfo: function () {

    },

    notificationCallback: function (msg) {
        if (msg == null) {
            return;
        }

        var json;

        try {
            json = JSON.parse(msg);
        }
        catch (e) {
            return;
        }

        var uid = json["uid"];
        var name = json["name"];
        var content = json["content"];

    },

    notificationGameEnd: function (msg) {
        this.scheduleOnce(this.GameEnd_Delay, 2.0);
    },

    GameEnd_Delay: function (dt) {
        if (RoomZJHInstance.isState(this.END_GAME) || RoomZJHInstance.isState(this.READY)) {
            this.showGameEndLayer();
        } else {
            this.m_showGameEnd = true;
        }
    },

    showGameEndLayer: function () {
        ZJHModel.getInstance().roomToShareView(RoomZJHInstance.curRoomID);
    },

    onExit: function () {
        this._super();
        cc.eventManager.removeListener(this.listenerGameOver)
    },

    getPlayerList: function () {

    },

    handler_server_bet_timeout_bc: function (val) {

        var offline = val["offline"];

        var pos = RoomZJHInstance.getPosBySeatid(RoomZJHInstance.cur_seat);
        var player = RoomZJHInstance.players[RoomZJHInstance.cur_seat];
        var RoomDNPlayer = this.players[pos];
        if (offline) {
            RoomDNPlayer.setLostVisible(true);
        }
        else {
            RoomDNPlayer.setThinkVisible(true);
        }
    },


    handler_server_end_room_req_fail_uc: function (val) {
        cc.log("handler_server_end_room_req_fail_uc");
        var code = val["code"];
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

    handler_server_end_room_bc: function (val) {
        cc.log("handler_server_end_room_bc");

        /*
         json
         {
         ownerid:220,
         ownername:"agag",
         time_left:20,
         players
         [
         {uid:111,name:"name",avatar:"avatar",state:1},//state: 0:等待 1：同意 2:反对
         ],
         }
         */


        if (RoomZJHInstance.seatid < 0) {
            return;
        }
        var player = RoomZJHInstance.getPlayerByUid(val["uid"]);


        if (player == null) {
            return;
        }


        var json;
        json["ownerid"] = player.uid;
        json["ownername"] = player.name;
        json["time_left"] = val["remain_time"];

        var isCan = ZJHModel.getInstance().uid == player.uid;
        for (var i = 0; i < val["voters_uid"].length; i++) {
            var p = RoomZJHInstance.getPlayerByUid(val["voters_uid"][i]);
            json["players"][i]["uid"] = p.uid;
            json["players"][i]["name"] = p.name;
            json["players"][i]["avatar"] = p.avatar;
            json["players"][i]["state"] = 0;

            if (ZJHModel.getInstance().uid == p.uid) {
                isCan = true;
            }
        }
        if (!isCan) {
            return;
        }

        this.room_EndTouPiao = RoomHEndTouPiao.create();
        this.room_EndTouPiao.initData(json);
        this.addChild(this.room_EndTouPiao, 10000000);

        this.room_EndTouPiao.setCallback(function (str) {
            if (str == "ok") {
                ZJHLogic.sendDissolveAction(1);
            }
            else if (str == "no") {
                ZJHLogic.sendDissolveAction(0);
            }
        });
    },

    handler_server_end_room_action_bc: function (val) {
        cc.log("handler_server_end_room_action_bc");

        var player = RoomZJHInstance.getPlayerByUid(val["uid"]);
        if (player == null) {
            return;
        }

        if (this.room_EndTouPiao != null) {
            this.room_EndTouPiao.updateState(player.uid, val["action"]);
        }
    },

    handler_server_end_room_result_bc: function (val) {
        cc.log("handler_server_end_room_result_bc");
        if (this.room_EndTouPiao != null) {
            this.room_EndTouPiao.removeFromParentAndCleanup(true);
        }
        this.room_EndTouPiao = null;

        var result = val["result"];
        if (result == 1) {
            JsUtils.postNotifi("notify_remove_share_btn");
            PlatformHelper.showToast("房间解散成功，3秒后房间自动结束");
        }
        else {
            PlatformHelper.showToast("解散失败，稍后游戏继续！");
        }
    }
});