var PlayerModel = cc.Class.extend({
    seatid: -1,
    sex: 0,
    uid: 0,
    name: "",
    avatar: "",
    money: 0,
    ready: 0,
    zhuan_surplustime: 0,
    reset: function () {
        this.uid = -1;
        this.ready = 0;

        this.name = "";
        this.sex = 0;
        this.avatar = "";
        this.money = 0;
        this.zhuan_surplustime = 0;
    },
});


var MJModel =
    {
        //***************通用数据*****************//
        curRoomID: 0,
        players: [],
        seatid: -1,
        tempSeatid: -1,
        lackType: -1,
        state: 0,
        cur_seat: -1,
        isEnterWait: false,
        waitUpdate: false,
        base_money: 0,
        stand_money: 0,
        take_in: 0,
        isTimeEnd: false,
        isEnd: false,
        testCount: 0,
        outCardTime: 0,
        oprateTime: 0,
        lackTime: 0,
        changeTime: 0,
        piaoTime: 0,
        pauseCount: 0,
        isFapai: false,
        canTouchCard: false,
        canChoiceMore: false,
        gang_info: [],
        bu_info:[], //湖南麻将特有的补张操作
        listen_info: [],
        final_listen_info: [],
        curOperate: 0,
        curOperateCard: 0,
        isAutoOutCard: 0,
        isOutCard: false,
        hasCardOutIng: false,
        lastHeartBeatTime: -1,
        lastLoginTime:-1,
        zhuanReadyTime: 0,
        isZhuanZhuan: false,
        banker_seatid: -1,
        curGameType: 0,
        isBaoting: false,
        JiPaicardList: [],
        isHanBaoDou: false,
        IsMyHasBaoting: false,
        isOnVideo: false,
        TouPiaoING: false,
        isNeedChangeCard: false,
        isNeedSearchHuazhuDajiao: false,
        scmjType: 0,
        gdmj_fengquan: -1,
        gdmj_playid: 0,
        valueGuiPai: 0,
        mj_roomType: 0, //0:普通房间 ； 1:三人两房
        scmj_listenType: 0,
        tangPai_flags: {},

        Play_num: 4,

        roomData: null,
        jiangma:0,
        init: function () {
            for (var i = 0; i < this.Play_num; i++) {
                var player = new PlayerModel();
                player.seatid = i;
                this.players[i] = player;
            }
        },
        //重置属性
        reset: function () {
            //玩家信息初始化
            for (var p in this.players) {
                this.players[p].reset();
            }
            this.cur_seat = -1;
            this.state = -1;
            this.isEnterWait = false;
            this.isZhuanZhuan = false;
            this.zhuanReadyTime = 0;
            this.preGame();
        },
        preGame: function () {
            this.listen_info = [];
            this.gang_info = [];
            this.bu_info = [];
            this.final_listen_info = [];
            this.isAutoOutCard = false;
            this.canTouchCard = false;
            this.lackType = -1;

            this.isEnterWait = false;
            this.isBaoting = false;
            this.curOperateCard = 0;
            this.curOperate = -1;
            this.isHanBaoDou = false;
            this.IsMyHasBaoting = false;

            this.TouPiaoING = false;

            this.tangPai_flags = {};

            this.baoting_test = -1;
        },
        release: function () {
            for (var i in this.players) {
                this.players[i].release();
            }
            this.players = {};
        },

        //该座位ID是否是我
        isMyPlayer: function (seatID) {
            return seatID == this.seatid;
        },

        //获取玩家
        getPlayerByUID: function (uid) {
            for (var p in this.players) {
                if (this.players[p].uid == uid) {
                    return this.players[p];
                }
            }
            return null;
        },
        getPlayerBySeatID: function (seatid) {
            if (this.players.hasOwnProperty(seatid)) {
                return this.players[seatid];
            }
            return null;
        },
        getPlayerByPos: function (pos) {
            var seatid = this.getSeatIDByPos(pos);

            if (this.players.hasOwnProperty(seatid)) {
                return this.players[seatid];
            }
            cc.log("getPlayerByPos erro:" + seatid);
            return null;
        },
        //座位ID转换当前位置
        getPosBySeatid: function (seatid) {
            var rePos = 0;

            if (this.seatid == -1) {
                if (this.tempSeatid != -1) {
                    var index = seatid - this.tempSeatid;
                    if (index >= 0) {
                        rePos = index;
                    } else {
                        rePos = index + this.Play_num;
                    }

                } else {
                    rePos = seatid;
                }
            } else {
                var index = seatid - this.seatid;
                if (index >= 0) {
                    rePos = index;
                } else {
                    rePos = index + this.Play_num;
                }
            }

            if (this.mj_roomType == 1) {
                if (rePos == 2) {
                    if (this.seatid == -1) {
                        rePos = 3;
                    } else {
                        if (this.seatid == 0) {
                            rePos = 3;
                        } else if (this.seatid == 2) {
                            rePos = 1;
                        }
                    }
                }
            }

            //防错措施 避免闪退
            if (rePos < 0 || rePos > 3){
                rePos = 0;
            }
            return rePos;

        },
        isState: function (status) {
            if (this.state == status) {
                return true;
            }
            return false;
        },
        getTempPosBySeatid: function (seatid, temp) {
            if (temp == -1) {
                return seatid;
            }
            var index = seatid - temp;
            if (index >= 0)
                return index;
            return index + this.Play_num;
        },
        //当前位置转换座位ID
        getSeatidByPos: function (pos) {
            var reSeatId = 0;

            if (this.seatid == -1) {
                if (this.tempSeatid != -1) {
                    var seatid = pos + this.tempSeatid;
                    if (seatid >= this.Play_num) {
                        reSeatId = seatid - this.Play_num;
                    } else {
                        reSeatId = seatid;
                    }

                } else {
                    reSeatId = pos;
                }
            } else {
                var seatid = pos + this.seatid;
                if (seatid >= this.Play_num) {
                    reSeatId = seatid - this.Play_num;
                } else {
                    reSeatId = seatid;
                }

            }

            if (this.mj_roomType == 1) {
                if (reSeatId == 3) {
                    if (this.seatid == -1) {
                        reSeatId = 2;
                    } else {
                        if (this.seatid == 0) {
                            reSeatId = 2;
                        } else if (this.seatid == 2) {
                            reSeatId = 0;
                        }
                    }
                }
            }
            //防错措施 避免闪退
            if (reSeatId < 0 || reSeatId > 3){
                reSeatId = 0;
            }
            return reSeatId;
        },

        setTangPaiFlag: function (pos, tangPai_flag) {
            this.tangPai_flags[pos] = tangPai_flag;
        },

        getTangPaiFlag: function (pos) {
            return this.tangPai_flags[pos];
        },

        isProtobufCJJ3: function () {
            if (this.curGameType == ROOM_TYPE.ROOM_TYPE_HNMJ_DZ
                || this.curGameType == ROOM_TYPE.ROOM_TYPE_HUNAN_DZ || this.curGameType == ROOM_TYPE.ROOM_TYPE_JXMJ_DZ) {
                return true;
            } else {
                return false;
            }
        },
    };



