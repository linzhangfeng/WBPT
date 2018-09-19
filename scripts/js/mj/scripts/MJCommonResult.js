var MJCommonResult = MJResult.extend({
    updateContentView: function (data) {
        this.ackGameEnd = parsePacket("proto.game.AckGameEnd", data);

        var itemHeight = 140;

        for (var m = 0; m < 4; m++) {
            var pos = m - 1 < 0 ? 3 : m - 1;

            if (pos == 2 && MJModel.mj_roomType == 1) {
                continue;
            }

            var seatid = MJModel.getSeatidByPos(pos);

            var startY = 620 - m * itemHeight;
            var startX = 100;

            //头像
            var headLayer = this.createAvatar(pos);
            headLayer.setPosition(cc.p(startX, startY));
            this.bgSpr.addChild(headLayer);

            startX = 180;
            //上排牌 杠
            var ganghuLayer = this.createGangHuLayer(seatid);
            this.bgSpr.addChild(ganghuLayer);
            ganghuLayer.setPosition(cc.p(startX, startY + 15));

            //下排牌
            var handCardLayer = this.createHandCard(seatid);
            handCardLayer.setPosition(cc.p(startX, startY - 50));
            this.bgSpr.addChild(handCardLayer);

            startX = 900;
            this.createSpecialHuTip(startX, startY, seatid);

            startX = 980;
            //输赢分
            var scoreLayer = this.createScoreLayer(seatid);
            scoreLayer.setPosition(cc.p(startX, startY + 30));
            this.bgSpr.addChild(scoreLayer);

            //中马列表
            var maLayer = this.createMaLayer(seatid);
            if (maLayer) {
                this.bgSpr.addChild(maLayer);
                maLayer.setPosition(cc.p(startX, startY + 20));
            } else {
                scoreLayer.setPosition(cc.p(startX, startY - 10));
            }

        }
    },

    createGangHuLayer: function (seatid) {
        var layer = new cc.Layer();
        var x = 0;
        var y = 0;
        this.createEndInfoTip(x, y, layer, seatid);
        return layer;
    },

    //游戏结束后通用的提示信息
    createEndInfoTip: function (x, y, layer, seatid) {
        if (this.ackGameEnd.EndInfo.length > seatid) {
            var gameEndInfo = this.ackGameEnd.EndInfo[seatid];
            var map = {};
            for (var i = 0; i < gameEndInfo.Gang.length; i++) {
                var gang = gameEndInfo.Gang[i];
                if (map[gang.GangType] == undefined) {
                    map[gang.GangType] = [];
                }
                map[gang.GangType].push(gang.GangCard);
            }
            for (var key in map) {
                if (!map.hasOwnProperty(key))continue;
                var a = key;
                var typeLabel = new cc.LabelTTF(this.getStringByType(a), "Arial", 20);
                typeLabel.setColor(this.leftColor);
                typeLabel.setAnchorPoint(cc.p(0, 0));
                typeLabel.setPosition(cc.p(x, y + 10));
                layer.addChild(typeLabel);

                x += typeLabel.getContentSize().width;

                var temp = map[key];
                for (var j = 0; j < temp.length; j++) {
                    var smallCard = this.createSmallCard(temp[j]);
                    smallCard.setAnchorPoint(cc.p(0.0, 0.0));
                    smallCard.setPosition(cc.p(x, y));
                    layer.addChild(smallCard);
                    x += 28;
                    if (j == temp.length - 1) {
                        x += 20;
                    }
                }
            }
        }

        return {x: x, y: y};
    },

    createSpecialHuTip: function (startX, startY, seatid) {
        //具体胡什么。。。
    },

});