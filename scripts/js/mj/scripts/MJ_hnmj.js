var MJGa = cc.Layer.extend(
    {
        tag: "MJGa",
        ga1: null,
        ga2: null,
        ga3:null,
        ga5: null,
        noGa: null,
        ctor: function () {
            this._super();
            this.ga1 = new ccui.Button(getResPath("RoomMJ/hnmj/ga1_1.png"),getResPath("RoomMJ/hnmj/ga1_2.png"));
            this.ga1.setPosition(cc.p(280 + this.ga1.getContentSize().width/2, 190 + this.ga1.getContentSize().height/2));
            this.ga1.setName("ga1");
            this.addChild(this.ga1);

            this.ga2 = new ccui.Button(getResPath("RoomMJ/hnmj/ga2_1.png"),getResPath("RoomMJ/hnmj/ga2_2.png"));
            this.ga2.setPosition(cc.p(this.ga1.getPositionX() + this.ga1.getContentSize().width + 45, this.ga1.getPositionY()));
            this.ga2.setName("ga2");
            this.addChild(this.ga2);

            this.ga3 = new ccui.Button(getResPath("RoomMJ/hnmj/ga3_1.png"),getResPath("RoomMJ/hnmj/ga3_2.png"));
            this.ga3.setPosition(cc.p(this.ga2.getPositionX() + this.ga1.getContentSize().width + 45, this.ga1.getPositionY()));
            this.ga3.setName("ga3");
            this.addChild(this.ga3);

            this.ga5 = new ccui.Button(getResPath("RoomMJ/hnmj/ga5_1.png"),getResPath("RoomMJ/hnmj/ga5_2.png"));
            this.ga5.setPosition(cc.p(this.ga3.getPositionX() + this.ga1.getContentSize().width+ 45, this.ga1.getPositionY()));
            this.ga5.setName("ga5");
            this.addChild(this.ga5);

            this.noGa = new ccui.Button(getResPath("RoomMJ/hnmj/ga0_1.png"),getResPath("RoomMJ/hnmj/ga0_2.png"));
            this.noGa.setPosition(cc.p(this.ga5.getPositionX() + this.ga1.getContentSize().width+ 45, this.ga1.getPositionY()));
            this.noGa.setName("noGa");
            this.addChild(this.noGa);
        },
        addClickEventListener: function (callback) {
            this.ga1.addClickEventListener(callback);
            this.ga2.addClickEventListener(callback);
            this.ga3.addClickEventListener(callback);
            this.ga5.addClickEventListener(callback);
            this.noGa.addClickEventListener(callback);
        },
    });

var MJTable_hnmj = MJTable.extend({

    mGaBar: null,

    ctor: function(){
        this._super();
    },

    initRoom: function(){
        this._super();

        var self = this;
        this.mGaBar = new MJGa();
        this.mGaBar.setVisible(false);
        this.mGaBar.addClickEventListener(function (node) {
            self.onPiaoClick(node);
        });
        this.addChild(this.mGaBar);

    },

    handler_out_card_sound: function (sex , card) {
        Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/hnmj/chupai/%1_%2.mp3"), sex == SEX_ID.SEX_FEMALE ? "female" : "male", Utils.toHex2(card)));
    },

    handler_op_sound: function (sex , OpType, action) {
        Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/hnmj/operator/%1_%2.mp3"), sex == SEX_ID.SEX_FEMALE ? "female" : "male", action));
    },

    handler_server_scene_info_uc: function (data, canDelay){
        var ackGameFree = parsePacket("proto.game.AckGameScene", data);
        var sceneStatus = ackGameFree.SceneStatus;
        MJModel.mj_roomType = ackGameFree.hnscene.roomType;
        this._super(data, canDelay);

        MJModel.piaoTime = ackGameFree.hnscene.piaoTime;


        if(sceneStatus != 0){
            //恢复各个玩家的花牌
            for (var i = 0; i < ackGameFree.hnscene.HuaPai.length; i++) {
                var seatid = i;
                var pos = MJModel.getPosBySeatid(seatid);
                for (var j = 0; j < ackGameFree.hnscene.HuaPai[i].HuaCount;j++){
                    this.room_Card.addHuaCard(pos, ackGameFree.hnscene.HuaPai[i].HuaCard[j]);
                }
            }
        }

        this.mGaBar.setVisible(false);
        if (sceneStatus != 0 && sceneStatus != 5) {
            for (var i = 0; i < ackGameFree.hnscene.UserPiao.length; i++) {
                var piao = ackGameFree.hnscene.UserPiao[i];
                var pos = MJModel.getPosBySeatid(i);
                this.players[pos].setGaType(piao);
            }
        }

        if (MJModel.seatid != -1) {
            if (sceneStatus == 5) {
                if (!ackGameFree.hnscene.IsPiaoyet) {
                    this.mGaBar.setVisible(true);
                }
            }
        }
    },

    handler_server_start_piao: function (data, canDelay) {
        cc.log("handler_server_start_piao.....");
        this._super(data, canDelay);
        if (MJModel.seatid != -1 && canDelay) {
            this.mGaBar.setVisible(true);
        }
    },

    handler_server_broad_piao: function (data, canDelay) {
        var broadPiao = parsePacket("proto.game.hnmahjonggame.AckBroadPiao", data);
        var seatId = broadPiao.chariid;
        var pos = MJModel.getPosBySeatid(seatId);
        this.players[pos].setGaType(broadPiao.UserPiao);
        var player = MJModel.players[seatId];
        Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/hnmj/operator/%1_shangga_%2.mp3"), player.sex == SEX_ID.SEX_FEMALE ? "female" : "male", broadPiao.UserPiao));
    },

    handler_server_game_start_bc2: function (data, canDelay) {
        this._super(data, canDelay);
        if (MJModel.seatid != -1 && canDelay) {
            this.mGaBar.setVisible(false);
        }
    },


    showFinalMJResult: function (data) {
        var result = new MJResult_hnmj();
        result.setName("RoomMJResultDetail");
        this.addChild(result, 100000);
        result.updateContentView(data);
    },

    onPiaoClick: function (ref) {
        var name = ref.getName();
        var nums = 0;
        this.mGaBar.setVisible(false);
        if (name == "ga1") {
            nums = 1;
        } else if (name == "ga2") {
            nums = 2;
        } else if (name == "ga3") {
            nums = 3;
        } else if (name == "ga5") {
            nums = 5;
        } else if (name == "noGa") {
            nums = 0;
        }
        sendGaNums(nums);
    },

});

var MJResult_hnmj = MJCommonResult.extend({

    createGangHuLayer: function (seatid) {
        var layer = new cc.Layer();
        var x = 0;
        var y = 0;
        var hnGameEndInfo = this.ackGameEnd.hnGameEndInfo;
        var piaoNum = hnGameEndInfo.piaoNum[seatid];
        if(piaoNum > 0){
            var gaTip = this.getGaTip(piaoNum);
            if(gaTip != ""){
                var typeLabel = new cc.LabelTTF(gaTip, "Arial", 20);
                typeLabel.setColor(this.leftColor);
                typeLabel.setAnchorPoint(cc.p(0, 0));
                typeLabel.setPosition(cc.p(x, y + 10));
                layer.addChild(typeLabel);
                x += typeLabel.getContentSize().width;
            }
        }

        if(hnGameEndInfo.faninfo.length > seatid){
            var fanInfo = hnGameEndInfo.faninfo[seatid];
            var remainBankerStr = this.getRemainBankerStr(fanInfo.remainBanker);
            if(remainBankerStr != ""){
                var typeLabel = new cc.LabelTTF(remainBankerStr, "Arial", 20);
                typeLabel.setColor(this.leftColor);
                typeLabel.setAnchorPoint(cc.p(0, 0));
                typeLabel.setPosition(cc.p(x, y + 10));
                layer.addChild(typeLabel);
                x += typeLabel.getContentSize().width;
            }
            if(fanInfo.addfan.length > 0){
                for(var i = 0 ; i < fanInfo.addfan.length ; i++){
                    var addFan = fanInfo.addfan[i];
                    var addFanStr = this.getAddFanStr(addFan.AddType , addFan.AddNum);
                    var typeLabel = new cc.LabelTTF(addFanStr, "Arial", 20);
                    typeLabel.setColor(this.leftColor);
                    typeLabel.setAnchorPoint(cc.p(0, 0));
                    typeLabel.setPosition(cc.p(x, y + 10));
                    layer.addChild(typeLabel);
                    x += typeLabel.getContentSize().width;
                }
            }
        }

        if (hnGameEndInfo.HuaPai.length > seatid) {
            var huaPaiInfo = hnGameEndInfo.HuaPai[seatid];
            if (huaPaiInfo.HuaCount > 0) {
                var huapaiLabel = new cc.LabelTTF("花牌", "Arial", 20);
                huapaiLabel.setColor(this.leftColor);
                huapaiLabel.setAnchorPoint(cc.p(0, 0));
                huapaiLabel.setPosition(cc.p(x, y + 10));
                layer.addChild(huapaiLabel);

                x += huapaiLabel.getContentSize().width;
                for (var i = 0; i < huaPaiInfo.HuaCount; i++) {
                    var card = this.createSmallCard(huaPaiInfo.HuaCard[i]);
                    card.setAnchorPoint(cc.p(0, 0));
                    card.setPosition(cc.p(x, y));
                    layer.addChild(card);
                    x += 28;
                    if (i == huaPaiInfo.HuaCount - 1) {
                        x += 20;
                    }
                }
            }
        }
        this.createEndInfoTip(x , y , layer , seatid);
        return layer;
    },

    createSpecialHuTip: function (startX, startY, seatid) {
        if (this.ackGameEnd.hnGameEndInfo.faninfo.length > seatid) {
            //胡牌类型
            var fanInfo = this.ackGameEnd.hnGameEndInfo.faninfo[seatid];
            var path = "";
            //1表示自摸，2表示点炮胡，3表示放炮,4 被自摸 5天和 6地和 7抢杠胡 8杠上开花 9花上添花
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
                case 5:
                    path = "RoomMJ/result-detail/result_detail_tianhe.png";
                    break;
                case 6:
                    path = "RoomMJ/result-detail/result_detail_dihe.png";
                    break;
                case 7:
                    path = "RoomMJ/result-detail/result_detail_qiangganghu.png";
                    break;
                case 8:
                    path = "RoomMJ/result-detail/result_detail_gangshang.png";
                    break;
                case 9:
                    path = "RoomMJ/result-detail/result_detail_huashanghua.png";
                    break;
                default:
                    break;
            }
            if (path != "") {
                var typeSprite = new cc.Sprite(getResPath(path));
                typeSprite.setPosition(cc.p(startX + 20, startY + 20));
                this.bgSpr.addChild(typeSprite);

                if (fanInfo.FanType >= 0 && fanInfo.FanNum >= 0) {

                    var fanStrArr = ["十三幺", "豪华七对", "七对", "清一色", "碰碰胡", "平胡", "花胡"];

                    var fantype = "";
                    fantype = fanStrArr[fanInfo.FanType];
                    this.buff = fanInfo.FanNum + "分";

                    if (fantype != "" && fanInfo.FanNum >= 0) {
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
    },

    getStringByType: function (stype) {
        var type = Number(stype);
        if(type == HUGANG.HUGANG_ZHIGANG){
            return "明杠";
        }else if(type == HUGANG.HUGANG_WANGANG){
            return "补杠";
        }else{
            return this._super(stype);
        }
    },

    getGaTip: function (piaoNum) {
        var gaTip = "";
        if(piaoNum == 1){
            gaTip = "【1噶】";
        }else if(piaoNum == 2){
            gaTip = "【2噶】";
        }else if(piaoNum == 3){
            gaTip = "【3噶】";
        }else if(piaoNum == 5){
            gaTip = "【5噶】";
        }
        return gaTip;
    },

    getAddFanStr: function (addType , addNum) {
        //另加分类型(1只吃不碰 2门清 3有眼 4箭牌刻 5 翻花对位 6风牌刻)
        var addTypeStr = "";
        if(addType == 1){
            addTypeStr = "只吃不碰"
        }else if(addType == 2){
            addTypeStr = "门清"
        }else if(addType == 3){
            addTypeStr = "有眼"
        }else if(addType == 4){
            addTypeStr = "箭牌刻"
        }else if(addType == 5){
            addTypeStr = "翻花对位"
        }else if(addType == 6){
            addTypeStr = "风牌刻"
        }

        var addNumStr = "";
        if(addNum > 1){
            addNumStr = "x" + addNum;
        }
        return "【" + addTypeStr + addNumStr + "】";
    },

    getRemainBankerStr: function (score) {
        if(score > 0){
            var str = "连庄";
            var scoreStr = score + "次";
            return "【" + str + scoreStr + "】";
        }else {
            return "";
        }
    },
});