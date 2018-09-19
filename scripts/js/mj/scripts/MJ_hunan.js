/**
 * Hu'nan Majhong.
 * Created by sherard on 2016/12/10.
 */

var HUNAN_MJ_TYPE = {
    1: "九莲宝灯",
    2: "大四喜",
    3: "小四喜",
    4: "大三元",
    5: "四暗刻",
    6: "十八罗汉",
    7: "清幺九",
    8: "字一色",
    9: "小三元",
    10: "混幺九",
    11: "清对(清一色+碰碰胡)",
    12: "清碰(清一色+碰碰胡)",
    13: "混碰(混一色+碰碰胡)",
    14: "清一色",
    15: "混一色",
    16: "碰碰胡",
    17: "平胡",
    18: "鸡胡",
    19: "天胡",
    20: "地胡",
    21: "人胡",
    22: "抢杠胡",
    23: "海底捞月",
    24: "杠上花",
    25: "清七对",
    26: "清龙七对",
    27: "龙七对",
    28: "全球人",
    29: "杠上炮",
    30: "门前清",
    31: "风一色",
    32: "门前清+清一色",
    33: "清一色+四暗刻",
    34: "海底炮",
    35: "十三幺",
    36: "七对",
    37: "带幺九",
    38: "将对",

    512: "四喜",
    513: "板板胡",
    514: "缺一门",
    515: "六六顺",
    516: "碰碰胡",
    517: "将将胡",
    518: "四个红中胡"
};

var PiaoType = {
    PIAO_2: "piao_2",
    PIAO_4: "piao_4",
    PIAO_6: "piao_6",
    PIAO_NO: "piao_no",
    getValue: function (type) {
        if (type == this.PIAO_2) {
            return 2;
        } else if (type == this.PIAO_4) {
            return 4;
        } else if (type == this.PIAO_6) {
            return 6;
        } else {
            return 0;
        }
    }
};
var MJPiao = cc.Layer.extend({
    tag: "MJPiao",
    oPiao2: null,
    oPiao4: null,
    oPiao6: null,
    oPiaoNo: null,
    ctor: function () {
        this._super();

        this.oPiao2 = new ccui.Button(getResPath("RoomMJ/hunan/piao2_normal.png"), getResPath("RoomMJ/hunan/piao2_pressed.png"));
        this.oPiao2.setPosition(cc.p(360 + this.oPiao2.getContentSize().width / 2, 180 + this.oPiao2.getContentSize().height / 2));
        this.oPiao2.setName(PiaoType.PIAO_2);
        this.addChild(this.oPiao2);

        this.oPiao4 = new ccui.Button(getResPath("RoomMJ/hunan/piao4_normal.png"), getResPath("RoomMJ/hunan/piao4_pressed.png"));
        this.oPiao4.setPosition(cc.p(this.oPiao2.getPositionX() + this.oPiao2.getContentSize().width + 60, 180 + this.oPiao2.getContentSize().height / 2));
        this.oPiao4.setName(PiaoType.PIAO_4);
        this.addChild(this.oPiao4);

        this.oPiao6 = new ccui.Button(getResPath("RoomMJ/hunan/piao6_normal.png"), getResPath("RoomMJ/hunan/piao6_pressed.png"));
        this.oPiao6.setPosition(cc.p(this.oPiao4.getPositionX() + this.oPiao4.getContentSize().width + 60, 180 + this.oPiao2.getContentSize().height / 2));
        this.oPiao6.setName(PiaoType.PIAO_6);
        this.addChild(this.oPiao6);

        this.oPiaoNo = new ccui.Button(getResPath("RoomMJ/hunan/piaoNo_normal.png"), getResPath("RoomMJ/hunan/piaoNo_pressed.png"));
        this.oPiaoNo.setPosition(cc.p(this.oPiao6.getPositionX() + this.oPiao6.getContentSize().width + 60, 180 + this.oPiao2.getContentSize().height / 2));
        this.oPiaoNo.setName(PiaoType.PIAO_NO);
        this.addChild(this.oPiaoNo);
    },
    addClickEventListener: function (callback) {
        this.oPiao2.addClickEventListener(callback);
        this.oPiao4.addClickEventListener(callback);
        this.oPiao6.addClickEventListener(callback);
        this.oPiaoNo.addClickEventListener(callback);
    }
});

var JiangmaTips = cc.Layer.extend({
    ctor:function () {
        this._super();
        var size = cc.director.getWinSize();

        var tipsBg = cc.Sprite(getResPath("RoomMJ/hunan/tips_gradient_bg.png"));
        tipsBg.setPosition(cc.p(size.width / 2, size.height / 2 + 20));
        this.addChild(tipsBg , 10000);

        var tipsTitle = cc.Sprite(getResPath("RoomMJ/hunan/zhongma_title.png"));
        if(MJModel.Jiangma == 1){
            tipsTitle.setTexture(getResPath("RoomMJ/hunan/zhongniao_title.png"));
        }
        tipsTitle.setPosition(cc.p(size.width / 2, tipsBg.getPosition().y + tipsTitle.getContentSize().height + 10));
        this.addChild(tipsTitle);
    },
    showZhongNiaoBigTips:function (cards, zhongFlag) {
        var length = cards.length;

        for (var i = 0; i < length; i++) {
            var cardValue = cards[i];
            var card = new MJCard();
            card.setValue(cardValue, 0, CardType.Card_Outing, i);
            card.setMyPosition(MJConfig.getNiaoBigCardPos(i, length));

            if (zhongFlag[i] == 1) {
                var cardBg = new cc.Sprite(getResPath("RoomMJ/hunan/card_light_bg.png"));
                cardBg.setPosition(card.getPosition());
                this.addChild(cardBg, 10000);
                this.addChild(card, 10000);
            } else {
                this.addChild(card, 9999);
            }
        }
    },
});

var MJTable_hunan = MJTable.extend({
    oMJPiao: null,
    ctor: function () {
        this._super();
    },
    initRoom: function () {
        this._super();

        var self = this;
        this.oMJPiao = new MJPiao();
        this.oMJPiao.setVisible(false);
        this.oMJPiao.addClickEventListener(function (node) {
            self.onPiaoClick(node);
        });
        this.addChild(this.oMJPiao);
    },

    onPiaoClick: function (ref) {
        var sPiaoName = ref.getName();
        cc.log("sPiaoName:" + sPiaoName);
        cc.log("iPiaoValue:" + PiaoType.getValue(sPiaoName));
        sendPiaoNums(PiaoType.getValue(sPiaoName));
    },

    handler_cmd: function (cmd, jpacket, canDelay) {
        this._super(cmd, jpacket, canDelay);
        if (cmd == CMD.SERVER_JIANG_MA) {
            this.handler_server_game_jiangma(jpacket, canDelay);
        }
    },

    handler_server_scene_info_uc: function (data, canDelay) {
        var ackGameFree = parsePacket("proto.game.AckGameScene", data);
        var sceneStatus = ackGameFree.SceneStatus;
        if(ackGameFree.hunanscene.JinZhiHuanPai.length > 0){
            for(var i = 0 ; i < ackGameFree.hunanscene.JinZhiHuanPai.length ; i++){
                if(MJModel.isMyPlayer(i) && ackGameFree.hunanscene.JinZhiHuanPai[i] == 1){
                    MJModel.isBaoting = true;
                }
            }
        }
        this._super(data, canDelay);
        MJModel.piaoTime = ackGameFree.hunanscene.piaoTime;
        MJModel.Jiangma = ackGameFree.hunanscene.Jiangma;
        this.room_Tip.initZhongNiaoTips(MJModel.Jiangma == 1);

        //可以补的牌
        var buZhang = ackGameFree.hunanscene.bu;
        if (buZhang && buZhang.bu.length > 0) {
            MJModel.bu_info = [];
            var bu_size = buZhang.bu.length;
            for (var i = 0; i < bu_size; i++) {
                var tgInfo = Utils.clone(tagGangInfo);
                tgInfo.curCard = buZhang.bu[i].Card;
                tgInfo.state = buZhang.bu[i].state;
                MJModel.bu_info.push(tgInfo);
            }
        }

        this.oMJPiao.setVisible(false);
        if (sceneStatus != 0 && sceneStatus != 4) {
            for (var i = 0; i < ackGameFree.hunanscene.UserPiao.length; i++) {
                var piao = ackGameFree.hunanscene.UserPiao[i];
                var pos = MJModel.getPosBySeatid(i);
                this.players[pos].setPiaoType(piao);
            }
        }

        if (MJModel.seatid != -1) {
            if (sceneStatus == 4) {
                if (!ackGameFree.hunanscene.IsPiaoyet) {
                    this.oMJPiao.setVisible(true);
                }
            }
        }

        if(ackGameFree.hunanscene.Laizi > 0){
            MJModel.valueGuiPai = ackGameFree.hunanscene.Laizi;
            if (MJModel.valueGuiPai) {
                this.room_Tip.showLeftGuiPaiBg();
                this.room_Tip.isShowGuiPai(1);
                this.room_Card.setGuipai();
            }
        }
    },

    handler_server_start_piao: function (data, canDelay) {
        this._super(data, canDelay);
        if (MJModel.seatid != -1 && canDelay) {
            this.oMJPiao.setVisible(true);
        }
    },

    handler_server_broad_piao: function (data, canDelay) {
        var broadPiao = parsePacket("proto.game.hunanmj.AckBroadPiao", data);
        var seatId = broadPiao.chariid;
        var pos = MJModel.getPosBySeatid(seatId);
        this.players[pos].setPiaoType(broadPiao.UserPiao);
    },

    handler_server_game_start_bc2: function (data, canDelay) {
        this._super(data, canDelay);
        if (MJModel.seatid != -1 && canDelay) {
            this.oMJPiao.setVisible(false);
        }
    },

    handler_server_table_info_uc: function (data, canDelay) {
        this._super(data, canDelay);
        this.removeChildByName("JiangmaTips");
    },

    handler_server_peng_kang_bc: function (data, canDelay) {
        this._super(data, canDelay);
        var ackBcOpResult = parsePacket("proto.game.AckBcOpResult", data);

        var seatid = ackBcOpResult.ChairID;
        var out_seatid = ackBcOpResult.OutCardChairID;
        var OpType = ackBcOpResult.OpType;
        var card = ackBcOpResult.Card;

        var pos = MJModel.getPosBySeatid(seatid);
        var out_pos = MJModel.getPosBySeatid(out_seatid);

        if((OpType & ActionType.TYPE_BUZHANG) == ActionType.TYPE_BUZHANG){
            if (canDelay) this.room_Tip.showEffect(pos, ActionType.TYPE_BUZHANG);
            var hnOpResult = ackBcOpResult.HNOPResult;
            if(hnOpResult.BuzhangOpType == ActionType.TYPE_ZHIGANG){
                this.room_Card.addPengCard(pos, PengType.Peng_Kang, card);
                this.room_Card.removeHandCard(pos, card, 3);
            }else if(hnOpResult.BuzhangOpType == ActionType.TYPE_WANGANG){
                this.room_Card.changPengCard(pos, PengType.Peng_Kang, card);
                this.room_Card.removeHandCard(pos, card, 1);
            }else if(hnOpResult.BuzhangOpType == ActionType.TYPE_ANGANG){
                this.room_Card.addPengCard(pos, PengType.Peng_Kang, card);
                this.room_Card.removeHandCard(pos, card, 4);
            }else {
                cc.log("buzhang type error! BuzhangOpType :" + hnOpResult.BuzhangOpType);
            }

        }else if((OpType & ActionType.TYPE_HAID_YAO) == ActionType.TYPE_HAID_YAO){

            if (canDelay) this.room_Tip.showEffect(pos, ActionType.TYPE_HAID_YAO);

        }

        if((OpType & ActionType.TYPE_JINHUAN_PAI) == ActionType.TYPE_JINHUAN_PAI){
            if (seatid == MJModel.seatid) {
                MJModel.canTouchCard = true;
                MJModel.isBaoting = true;
                MJModel.listen_info = [];
                this.room_Card.setBaoTingCardClickEnable(false, false);
            }
        }
    },

    handler_server_mo_card_bc: function (data, canDelay) {
        this._super(data, canDelay);
        var userGetCard = parsePacket("proto.game.AckUserGetCard", data);
        var seatId = userGetCard.ChairID;

        if (MJModel.isMyPlayer(seatId)) {
            MJModel.bu_info = [];
            var buZhang = userGetCard.bu;
            if (buZhang && buZhang.bu.length > 0) {
                var bu_size = buZhang.bu.length;
                for (var i = 0; i < bu_size; i++) {
                    var tgInfo = Utils.clone(tagGangInfo);
                    tgInfo.curCard = buZhang.bu[i].Card;
                    tgInfo.state = buZhang.bu[i].state;
                    MJModel.bu_info.push(tgInfo);
                }
            }
        }
    },

    handler_server_operate_notify_bc: function (data, canDelay) {
        this._super(data, canDelay);
        var ackOpNotify = parsePacket("proto.game.AckOpNotify", data);
        var seatid = ackOpNotify.ChairID;
        if (MJModel.isMyPlayer(seatid)) {

            if (ackOpNotify.Gang.length > 0) {
                MJModel.gang_info = [];
                var gang_size = ackGameFree.Gang.length;
                for (var i = 0; i < gang_size; i++) {
                    var tgInfo = Utils.clone(tagGangInfo);
                    tgInfo.curCard = ackGameFree.Gang[i].Card;
                    tgInfo.state = ackGameFree.Gang[i].state;
                    MJModel.gang_info.push(tgInfo);
                }
            }

            MJModel.bu_info = [];
            var buZhang = ackOpNotify.bu;
            if (buZhang && buZhang.bu.length > 0) {
                var bu_size = buZhang.bu.length;
                for (var i = 0; i < bu_size; i++) {
                    var tgInfo = Utils.clone(tagGangInfo);
                    tgInfo.curCard = buZhang.bu[i].Card;
                    tgInfo.state = buZhang.bu[i].state;
                    MJModel.bu_info.push(tgInfo);
                }
            }
        }
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
        MJModel.canTouchCard = true;
        if (MJModel.isBaoting) {
            this.room_Card.setBaoTingCardClickEnable(false, IsMoCard);
            this.room_Card.setTingPaiFlag(false);
        }
        else {
            this.room_Card.setBaoTingCardClickEnable(true);
            this.room_Card.setTingPaiFlag(true);
        }
        if (this.room_Action) this.room_Action.setTingPaiVis(false);
        if (MJModel.isOutCard == false) {
            this.room_Tip.showOutCardEffect(true);
        }
    },

    handler_server_game_jiangma: function (data, canDelay) {
        var jiangma = parsePacket("proto.game.hunanmj.tagJiangma", data);
        var seatId = jiangma.Chairid;
        var pos = MJModel.getPosBySeatid(seatId);
        var oJiangmaTips = new JiangmaTips();
        oJiangmaTips.setName("JiangmaTips");
        oJiangmaTips.showZhongNiaoBigTips(jiangma.Card, jiangma.ZhongCardFlag);
        this.addChild(oJiangmaTips , 10000);

        oJiangmaTips.runAction(cc.sequence(cc.delayTime(3) , cc.removeSelf()));

        this.room_Tip.showZhongNiaoTip(pos);
        for (var i = 0; i < jiangma.Card.length; i++) {
            this.room_Card.addZhongNiaoCard(pos, jiangma.Card[i]);
        }
    },

    handler_server_game_end_bc_maima: function (data, canDelay) {
        //买马已经单独的指令处理了，这里直接显示结算就可以了
        this.handler_server_game_end_bc_final(data);
    },

    showFinalMJResult: function (data) {
        var result = new MJResult_hunan();
        result.setName("RoomMJResultDetail");
        this.addChild(result, 10000);
        result.updateContentView(data);
    },

    test: function () {
        MJModel.seatid = 0;

        var outCards = [0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
            0x11, 0x12, 0x13, 0x14, 0x15, 0x26, 0x27, 0x28, 0x29, 0x21,
            0x22, 0x28, 0x29, 0x21,
            0x22];
        var cards = [0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x01, 0x23, 0x24, 0x12];

        for (var i = 0; i < 4; i++) {
            var player = this.players[i];
            player.setId(1);
            player.seatid = i;
            player.setPiaoType(6);

            this.room_Card.addPengCard(i, PengType.Peng_AnKang, 0x01);

            this.room_Tip.showZhongNiaoTip(i);
            this.room_Card.addZhongNiaoCard(i, 0x21);
            this.room_Card.addZhongNiaoCard(i, 0x22);
            this.room_Card.addZhongNiaoCard(i, 0x23);
            this.room_Card.addZhongNiaoCard(i, 0x24);
            this.room_Card.addZhongNiaoCard(i, 0x25);
            this.room_Card.addZhongNiaoCard(i, 0x26);

            if (i == 0) {
                this.room_Card.initHandCard(0, cards);
                this.room_Card.reflashHandCard(0, true);

                this.room_Card.initMoCard(i, 0x05, true);
            } else {
                var cardList = [];
                for (var m = 0; m < 10; m++) {
                    cardList.push(0);
                }
                this.room_Card.initHandCard(i, cardList);
                this.room_Card.reflashHandCard(i, true);

                this.room_Card.initMoCard(i, 0, true);
            }
            this.room_Card.initOutCard(i, outCards);
        }

        var cardList = [0x21, 0x22, 0x23, 0x24, 0x25, 0x26];
        var zhongList = [1,0,1,0,1,0];
        this.room_Tip.showZhongNiaoBigTips(cardList, zhongList);

        // this.roomPublic.setVisible(false);
        // this.room_Info.setVisible(true);
        // cc.log("test:" + HUNAN_MJ_TYPE[0]);
        // cc.log("test:" + HUNAN_MJ_TYPE[35]);
        // cc.log("test:" + HUNAN_MJ_TYPE[512]);
        // cc.log("test:" + HUNAN_MJ_TYPE[518]);

        cc.log("test:" + HUNAN_MJ_TYPE[0]);
        cc.log("test:" + HUNAN_MJ_TYPE[1]);
        cc.log("test:" + HUNAN_MJ_TYPE[2]);
        cc.log("test:" + HUNAN_MJ_TYPE[3]);
        cc.log("test:" + HUNAN_MJ_TYPE[4]);
        cc.log("test:" + HUNAN_MJ_TYPE[35]);
        cc.log("test:" + HUNAN_MJ_TYPE[36]);
        cc.log("test:" + HUNAN_MJ_TYPE[512]);
        cc.log("test:" + HUNAN_MJ_TYPE[517]);
        cc.log("test:" + HUNAN_MJ_TYPE[518]);
    }
});

var MJResult_hunan = MJCommonResult.extend({
    createGangHuLayer: function (seatid) {
        var layer = new cc.Layer();
        var x = 0;
        var y = 0;
        var hunanGameEndInfo = this.ackGameEnd.hunanGameEndInfo;
        var piaoNum = hunanGameEndInfo.piaoNum[seatid];
        if(piaoNum > 0){
            var piaoStr = "飘" + piaoNum;
            var piaoLabel = new cc.LabelTTF(piaoStr, "Arial", 20);
            piaoLabel.setColor(this.leftColor);
            piaoLabel.setAnchorPoint(cc.p(0, 0));
            piaoLabel.setPosition(cc.p(x, y + 10));
            layer.addChild(piaoLabel);
            x += piaoLabel.getContentSize().width;
        }

        if(hunanGameEndInfo.faninfo.length > seatid){
            var fanInfo = hunanGameEndInfo.faninfo[seatid];
            if(fanInfo.addfan.length > 0){
                var fanStrs = "【";
                for(var i = 0 ; i < fanInfo.addfan.length ; i++){
                    var addFan = fanInfo.addfan[i];
                    fanStrs += this.getAddFanStr(addFan.AddType , addFan.AddNum);
                    if(i < fanInfo.addfan.length - 1){
                        fanStrs += " ";
                    }
                }
                fanStrs += "】";
                var typeLabel = new cc.LabelTTF(fanStrs, "Arial", 20);
                typeLabel.setColor(this.leftColor);
                typeLabel.setAnchorPoint(cc.p(0, 0));
                typeLabel.setPosition(cc.p(x, y + 10));
                layer.addChild(typeLabel);
                x += typeLabel.getContentSize().width;
            }
        }

        this.createEndInfoTip(x , y , layer , seatid);
        return layer;
    },

    getStringByType: function (stype) {
        var type = Number(stype);
        if(type == ActionType.TYPE_BUZHANG){
            return "补张";
        }else{
            return this._super(stype);
        }
    },

    getAddFanStr: function (addType , addNum) {
        return HUNAN_MJ_TYPE[addType] + "+" + addNum;
    },

    createMaLayer: function (seatid) {
        var layer = this._super(seatid);
        if(layer){
            var maPic = layer.getChildByName("zhongmaTip");
            if(MJModel.Jiangma == 1){
                //改为中鸟图标
                maPic.setTexture(getResPath("RoomMJ/result-small/result_small_zhongniao.png"));
            }
        }
        return layer;
    },

    createSpecialHuTip: function (startX, startY, seatid) {

        if (this.ackGameEnd.hunanGameEndInfo.faninfo.length > seatid) {
            //胡牌类型
            var fanInfo = this.ackGameEnd.hunanGameEndInfo.faninfo[seatid];
            var path = "";
            //1表示自摸，2表示点炮胡，3表示放炮,4 被自摸
            switch (fanInfo.SpecialType) {
                case 1:
                    path = "RoomMJ/result-detail/result_detail_zimo.png";
                    break;
                case 2:
                    path = "RoomMJ/result-detail/result_detail_paohu.png";
                    break;
                case 3:
                    path = "RoomMJ/result-detail/result_detail_dianpao.png";
                    break;
                case 4:
                    path = "RoomMJ/result-detail/result_detail_beizimo.png";
                    break;
                default:
                    break;
            }
            if (path != "") {
                var typeSprite = new cc.Sprite(getResPath(path));
                typeSprite.setPosition(cc.p(startX + 20, startY + 20));
                this.bgSpr.addChild(typeSprite);

                if (fanInfo.FanNum > 0) {
                    var fantype = "总番:";
                    this.buff = fanInfo.FanNum + "番";
                    var fantypeLabel = new cc.LabelTTF(fantype, "Arial", 24);
                    fantypeLabel.setColor(this.rightColor);
                    fantypeLabel.setAnchorPoint(cc.p(1, 1));
                    typeSprite.addChild(fantypeLabel);
                    fantypeLabel.setPosition(cc.p(typeSprite.getContentSize().width / 2, -20));
                    var fanNumLabel = new cc.LabelTTF(this.buff, "Arial", 24);
                    fanNumLabel.setColor(this.rightColor);
                    fanNumLabel.setAnchorPoint(cc.p(0, 1));
                    fanNumLabel.setPosition(cc.p(fantypeLabel.getPosition().x + 10, fantypeLabel.getPosition().y));
                    typeSprite.addChild(fanNumLabel);

                }
            }
        }
    }
});