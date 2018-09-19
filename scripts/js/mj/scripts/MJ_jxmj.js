var MJTable_jxmj = MJTable.extend({
    ctro: function()
    {
        this._super();
        cc.log("go into jiangxiMJ");
    },

    handler_out_card_sound: function (sex , card) {
        Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/jxmj/chupai/%1_%2.mp3"), sex == SEX_ID.SEX_FEMALE ? "female" : "male", Utils.toHex2(card)));
    },

    handler_op_sound: function (sex , OpType, action) {
        Sound.getInstance().playEffect(__String.createWithFormat(getResPath("RoomMJ/sound/jxmj/operator/%1_%2.mp3"), sex == SEX_ID.SEX_FEMALE ? "female" : "male", action));
    },

    showFinalMJResult: function (data) {
        var result = new MJResult_jxmj();
        result.setName("RoomMJResultDetail");
        this.addChild(result, 100000);
        result.updateContentView(data);
    },
});


var MJResult_jxmj = MJCommonResult.extend({
    // createGangHuLayer: function (seatid)
    // {
    //     var layer = new cc.Layer();
    //     var x = 0;
    //     var y = 0;
    //     var hnGameEndInfo = this.ackGameEnd.hnGameEndInfo;
    //
    //     this.createEndInfoTip(x , y , layer , seatid);
    //     return layer;
    // },
    createSpecialHuTip: function (startX, startY, seatid) {
        if (this.ackGameEnd.jxGameEndInfo.faninfo.length > seatid) {
            //胡牌类型
            var fanInfo = this.ackGameEnd.jxGameEndInfo.faninfo[seatid];
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

                if (fanInfo.FanType >= 0 && fanInfo.FanNum >= 0) {

                    var fanStrArr = ["天胡", "豪华七对天胡", "豪华七对", "单吊", "七对", "碰碰胡", "平胡", "抢杠胡"];

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
        var str = "";
        switch (type) {
            case HUGANG.HUGANG_ZHIGANG:
                str = "明杠";
                break;
            case HUGANG.HUGANG_WANGANG:
                str = "加杠";
                break;
            case HUGANG.HUGANG_ANGANG:
                str = "暗杠";
                break;
            case HUGANG.HUGANG_DIANGANG:
                str = "点杠";
                break;
            case HUGANG.HUGANG_DIANPAO:
                str = "点炮";
                break;
            case HUGANG.HUGANG_PAOHU:
                str = "炮胡";
                break;
            case HUGANG.HUGANG_BEIZIMO:
                str = "被自摸";
                break;
            case HUGANG.HUGANG_ZIMO:
                str = "自摸";
                break;
            default:
                break;
        }
        return str;
    },
});
