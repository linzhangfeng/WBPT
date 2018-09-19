var PlayerModel = cc.Class.extend({
    seatid: -1,
    sex: 0,
    uid: 0,
    name: "",
    avatar: "",
    money: 0,
    ready: 0,
    reset: function () {
        this.uid = -1;
        this.ready = 0;

        this.name = "";
        this.sex = 0;
        this.avatar = "";
        this.money = 0;
    },
});

var RoomZJHInstance = {

    roomData: null,
    curRoomID: 0,
    curGameType: 0,
    roomConnectFail: 0,
    log_time: 0,
    isExit: false,
    players: [],
    Perplayers: [],//围观玩家
    Upplayers: [],//围观玩家
    seatid: -1,
    tempSeatid: -1,
    state: 0,
    cur_round: 0,
    cur_seat: -1,
    cur_bet: 0,
    max_bet: 0,//决斗下注额
    IsMySee: false,//自己是否看牌
    roundEnd: false,//是否用尽了所有回合
    cur_match_bet: 0,
    per_time: 0,
    wait_time: 0,
    preready_time: 0,
    ready_time: 0,
    total_bet: 0,
    cmp_round: 0,
    max_round: 0,
    see_round: 0,
    compare_condition: 0,
    call_cnt: 0,
    raise: [],//第一个是初始押注，后四个是加注选项
    curRaise: [],
    dealer: 0,
    first_charge: 0, //锅底
    isPropRoom: 0,
    isDouNiuRoom: 0,
    textIndex: 0,
    lastRequstListTime: 0,
    lastSendTime: 0,
    callAll: false,
    isMy: false,
    isLogOut: false,
    reLogIn: false,
    gameReset: false,
    balanceOver: false,
    isEnterWait: false,
    allBetMoneyOnMe: 0,
    waitUpdate: false,
    isAllIn: false,
    mid: 0,
    base_money: 0,
    stand_money: 0,
    min_money: 0,
    max_money: 0,
    bestMoney: 0,//优先进入的钱 系统自动分配的时候的一个依据
    single_money: 0,//单注金额
    isFapai: false,
    canBuyMore: 0,//是否支持多买
    testCount: 0,
    pauseCount: 0,
    m_nBettingTime: 0,
    left_bet_time: 0,//剩余时间
    sendBeting: 0,
    beting: 0,
    Game_type: 0,//游戏类型 1.豹子王 2.全押王 3.斗牛
    IsWanRen: false,//是否是万人场
    PreNums: 0,//外围人数
    MyBetRadio: 0,//选中下标
    MyIsPer: false,//自己是否在外围桌上
    per_seatid: 0,//自己外围座位
    totalLife: 0,
    totalBet: [],
    total_count: 0,//投注数量
    MyWinMoney: 0,//我赢取金币
    MyBetMoney: 0,
    lastRequestPoolTime: 0,
    lastRequestAllPlayersTime: 0,
    pool_flag: 0,
    AllPerBetInfo: null,//所有玩家投注
    MyBetNums: [],//我四个投注按钮倍数
    Win_Interval: [],//赢取区间
    ratio: 0,//赢取倍数
    //保存5张底牌
    fiveHandCards: [],
    isOnTV: 0,
    lastHeartBeatTime: -1,
    baseBeishu: 0,
    cancompare: 0,

    initPlayers: function () {
        {
            for (var i = 0; i < 7; i++) {
                var player = new PlayerModel();
                player.seatid = i;
                this.players[i] = player;
            }
            for (var i = 0; i < 6; i++) {
                var player = new PlayerModel();
                player.seatid = i;
                this.Perplayers[i] = player;
            }
            for (var i = 0; i < 3; i++) {
                var player = new PlayerModel();
                player.seatid = i;
                this.Upplayers[i] = player;
            }
        }
    },

    init: function () {
        roomConnectFail = 0;
        isExit = false;
        isEnterWait = false;
        waitUpdate = false;
        seatid = -1;
        tempSeatid = -1;
        Game_type = 1;
        m_nBettingTime = 30;
        left_bet_time = 30;
        preready_time = 5;
        ready_time = 3;
        sendBeting = 0;
        sendBeting = 0;
        beting = 0;
        lastRequstListTime = 0;
        mid = 0;
        base_money = 0;
        min_money = 0;
        max_money = 0;
        bestMoney = 0;
        allBetMoneyOnMe = 0;
        IsMySee = false;
        PreNums = 6;
        MyBetRadio = 1;
        total_count = 0;
        totalLife = 0;
        per_seatid = 0;
        MyWinMoney = 0;
        MyBetMoney = 0;
        single_money = 0;
        lastSendTime = 0;
        textIndex = 0;
        callAll = false;
        isMy = false;
        isLogOut = false;
        reLogIn = false;
        gameReset = false;
        balanceOver = false;
        isAllIn = false;
        isFapai = false;
        canBuyMore = false;
        testCount = 0;
        pauseCount = 0;
        MyIsPer = false;
        per_seatid = -1;
        lastRequestPoolTime = 0;
        isOnTV = 0;
        IsWanRen = false;
        pool_flag = 1;
        lastRequestAllPlayersTime = 0;
        this.Win_Interval = [];
        ratio = 1;
        max_round = 20;
        baseBeishu = 1;
        this.totalBet = [0, 0, 0, 0, 0];
        this.MyBetNums = [];
        for (var i = 0; i < 4; i++) {
            this.MyBetNums.push(100 * 10 * i + 1000);
        }
        lastHeartBeatTime = -1;
        cancompare = 1;
        log_time = 0;
        this.initPlayers();
    },

    reset: function () {
        for (var i = 0; i < 7; i++) {
            var playerModel = this.players[i];
            playerModel.reset();
        }

    },

    isMyPlayer: function (seatid) {
        if (seatid == this.seatid) {
            return true;
        }
        else {
            return false;
        }
    },

    getPosBySeatid: function (seatid) {
        if (this.seatid == -1) {
            if (this.tempSeatid != -1) {
                var index = seatid - this.tempSeatid;
                if (index >= 0)
                    return index;
                return index + 5;
            }
            return seatid;
        }
        var index = seatid - this.seatid;
        if (index >= 0)
            return index;
        return index + 5;
    },

    getSeatidByPos: function (pos) {
        if (this.seatid == -1) {
            if (this.tempSeatid != -1) {
                var seatid = pos + this.tempSeatid;
                if (seatid >= 5) {
                    seatid = seatid - 5;
                }

                return seatid;
            }
            return pos;
        }

        var seatid = pos + this.seatid;
        if (seatid >= 5) {
            seatid = seatid - 5;
        }
        return seatid;
    },

    getSexBySeatid: function (seatid) {
        var player = this.players[seatid];
        return player.sex;
    },

    isState: function (status) {
        if (this.state == status) {
            return true;
        }
        return false;
    },


    getSetCardTypeString: function (cardType) {
        var typeString = "";
        switch (cardType) {
            case 0:
                break;
            case 1:
                typeString = "豹子";
                break;
            case 2:
                typeString = "天龙";
                break;
            case 3:
                typeString = "地龙";
                break;
            case 4:
                typeString = "顺金";
                break;
            case 5:
                typeString = "金花";
                break;
            case 6:
                typeString = "顺子";
                break;
            case 7:
                typeString = "对子";
                break;
            case 8:
                typeString = "单牌";
                break;
            case 9:
                typeString = "特殊";
                break;
            default:
                break;
        }

        return typeString;
    },


    getCardTypeString: function (cardType) {
        var typeString = "";
        switch (cardType) {
            case 0:
                break;
            case 1:
                typeString = "豹子";
                break;
            case 2:
                typeString = "顺金";
                break;
            case 3:
                typeString = "金花";
                break;
            case 4:
                typeString = "顺子";
                break;
            case 5:
                typeString = "对子";
                break;
            case 6:
                typeString = "单牌";
                break;
            case 7:
                typeString = "特殊";
                break;
            default:
                break;
        }

        return typeString;
    },

    getPlayerByUid: function (uid) {
        for (var i = 0; i < this.players.length; i++) {
            var player = this.players[i];
            if (player.uid == uid) {
                return player;
            }
        }
        return null;
    },

};