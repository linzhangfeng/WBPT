var HUGANG =
    {
        HUGANG_ZHIGANG: 2,				//直杠
        HUGANG_WANGANG: 4,				//弯杠
        HUGANG_ANGANG: 8,				//暗杠
        HUGANG_DIANPAO: 16,			//点炮
        HUGANG_PAOHU: 32,				//炮胡
        HUGANG_DIANGANG: 64,			//点杠
        HUGANG_BEIZIMO: 128,			//被自摸
        HUGANG_ZIMO: 256,				//自摸
        HUGANG_HUAZHU: 512,            //花猪
        HUGANG_DAJIAO: 1024,            //大叫
        HUGANG_BEIHUAZHU: 2048,            //被花猪赔付
        HUGANG_BEIDAJIAO: 4096,            //被大叫赔付

        //自定义部分
        HUGANG_HUAPAI: 250,                //广东麻将花牌
        HUGANG_PIAO: 260,                  //飘
        HUGANG_TINGPAI: 261,               //听牌
        HUGANG_TUIGANG: 270,               //退杠
        HUGANG_BEITUIGANG: 271,            //被退杠
        HUGANG_CHAOZHUANG: 280,            //抄庄
        HUGANG_BEICHAOZHUANG: 281,         //被抄庄

        HUGANG_BAOPAI: 500,            //包牌
        HUGANG_BEIBAOPAI: 501,            //被包牌
        HUGANG_SHABAO: 502,            //杀报
        HUGANG_BEISHABAO: 503,            //被杀报
    }

var MJResult = cc.Layer.extend({
        ackGameEnd: null,
        bgSpr: "",
        leftColor: null,
        rightColor: null,
        buff: "",
        ctor: function () {

            this._super();

            var size = cc.director.getWinSize();

            var layout = new ccui.Layout();
            layout.setContentSize(size);
            layout.setAnchorPoint(cc.p(0, 0));
            layout.setPosition(cc.p(0, 0));
            layout.setTouchEnabled(true);
            layout.setSwallowTouches(true);
            layout.addClickEventListener(function (Ref) {
            });
            this.addChild(layout, -1000);

            cc.log("----go into result----pp");
            if (MJModel.mj_roomType == 1) {
                this.bgSpr = new cc.Sprite(getResPath("RoomMJ/result-detail/bg_three.png"));
            } else {
                this.bgSpr = new cc.Sprite(getResPath("RoomMJ/result-detail/bg.png"));

            }
            this.bgSpr.setPosition(cc.p(size.width / 2, size.height / 2));
            this.addChild(this.bgSpr);

            //继续游戏
            var continueBtn = new ccui.Button(getResPath("RoomMJ/result-detail/result_detail_btn_continue_normal.png"), getResPath("RoomMJ/result-detail/result_detail_btn_continue_press.png"));
            continueBtn.setAnchorPoint(cc.p(0.5, 0.5));
            continueBtn.setPosition(cc.p(1090, 80));
            continueBtn.addClickEventListener(this.continueClick.bind(this));
            this.bgSpr.addChild(continueBtn);

            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
                //小鸡提示
                var xiaoji_flag = new cc.Sprite(getResPath("RoomMJ/GZMJ/gz_xiaoji_tip.png"));
                xiaoji_flag.setAnchorPoint(cc.p(0.0, 0.5));
                xiaoji_flag.setPosition(55, 80);
                this.bgSpr.addChild(xiaoji_flag);

            }
            this.leftColor = cc.color(77, 45, 30, 255);
            this.rightColor = cc.color(175, 90, 34, 255);

            return true;
        },

        updatePH: function (data) {

            for (var m = 0; m < 4; m++) {
                var pos = m - 1 < 0 ? 3 : m - 1;

                var startX = 100;
                var startY = 620 - m * 140;

                var seatid = MJModel.getSeatidByPos(pos);

                var headIcon = this.createAvatar(pos);
                headIcon.setPosition(cc.p(startX, startY));
                headIcon.setAnchorPoint(cc.p(0.5, 0.5));
                this.bgSpr.addChild(headIcon);

                startX = 180;

                //上排牌 杠胡、花牌
                var ganghuLayer = this.createGangHuLayer(seatid);
                this.bgSpr.addChild(ganghuLayer);
                ganghuLayer.setPosition(cc.p(startX, startY + 15));

                //下排手牌信息
                var layer = this.createHandCard(seatid);
                layer.setAnchorPoint(cc.p(0.0, 0.0));
                layer.setPosition(cc.p(startX, startY - 50));
                this.bgSpr.addChild(layer);

                startX = 920;
                //胡牌类型
                var hupaipath = "";
                var t = 0;
                if (this.ackGameEnd.EndInfo.length > seatid) t = this.ackGameEnd.EndInfo[seatid].EndType;
                switch (t) {
                    case 1:
                        hupaipath = getResPath("RoomMJ/result-detail/result_detail_zimo.png");
                        break;
                    case 3:
                        hupaipath = getResPath("RoomMJ/result-detail/result_detail_dianpao.png");
                        break;
                    case 2:
                        hupaipath = getResPath("RoomMJ/result-detail/result_detail_paohu.png");
                        break;
                    default:
                        break;
                }
                if (hupaipath != "") {
                    var hupaiTypePic = new cc.Sprite(hupaipath);
                    hupaiTypePic.setAnchorPoint(cc.p(0.5, 0.5));
                    hupaiTypePic.setPosition(cc.p(startX, startY));
                    this.bgSpr.addChild(hupaiTypePic);
                }

                startX = 980;
                //总成绩
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

        updateContentView: function (data) {

            this.ackGameEnd = parsePacket("proto.game.AckGameEnd", data);
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_MJ_PH_DZ_DZ) {
                this.updatePH(data);
            } else if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
                this.ShowGZMJData(data);
            } else if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_YNMJ_DZ) {
                this.updateSC(data);
            }
            else if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
                this.ShowGZMJData(data);
            }
            else if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GDMJ_DZ) {
                this.updateGD(data);
            }
            else {
                this.updatePH(data);
            }
        },

        updateSC: function (data) {
            this.ackGameEnd = parsePacket("proto.game.AckGameEnd", data);
            // var gameEnd = createPacket("proto.game.AckGameEnd");
            // var HandCards = createPacket("proto.game.HandCards");
            //
            // //测试数据
            // for (var i = 0; i < 4; i++) {
            //     gameEnd.Score.push(100);
            //     //四川麻将结算
            //     HandCards = gameEnd.UserCard;
            //     var temphandcards = HandCards;
            //
            //     temphandcards.ChangeableCardsLen = 7;
            //     for (var j = 0; j < temphandcards.ChangeableCardsLen; j++) {
            //         //            temphandcards->set_changeablecards(j, 16+random(1, 8));
            //         temphandcards.ChangeableCards = (16 + parseInt(Math.random(1, 8)*10));
            //         cc.log("temphandcards.ChangeableCards-------:"+temphandcards.ChangeableCards);
            //     }
            //     cc.log("ppppppp----:" + temphandcards.ChangeableCards);
            //     temphandcards.FixedCardsLen = 3;
            //     for (var j = 0; j < temphandcards.FixedCardsLen; j++) {
            //         var tempfixedcard = createPacket("proto.game.FixedCard");
            //         var map = {};
            //         if (map[0] == undefined) {
            //             map[0] = [];
            //             map[1] = [];
            //             map[2] = [];
            //         }
            //
            //         map[0].push(16);
            //         map[1].push(17);
            //         map[2].push(j);
            //         temphandcards.stFixedCards = map;
            //         cc.log("-------------oo-----"+temphandcards.stFixedCards.state);
            //         tempfixedcard = temphandcards.stFixedCards;
            //         tempfixedcard.CardData = (16 + parseInt(Math.random(1, 8)*10));
            //
            //         if (j == 0) {
            //             tempfixedcard.state = (ActionType.TYPE_PENG);
            //         } else {
            //             tempfixedcard.state = (ActionType.TYPE_ZHIGANG);
            //         }
            //         tempfixedcard.chairID = parseInt(Math.random(0, 3)*10);
            //     }
            // }
            // var scgameend = createPacket("proto.game.scmj.tagScGameEnd");
            // for (var i = 0; i < 4; i++) {
            //     //胡牌信息
            //     var temptaguserhuinfo = createPacket("proto.game.scmj.tagHu");
            //     temptaguserhuinfo = scgameend.stUserHu;
            //     temptaguserhuinfo.HuTotalscore = parseInt((Math.random(-1000, 1000)));
            //     for (var j = 0; j < 7; j++) {
            //         var temptaghu = createPacket("proto.game.scmj.tagUserHuInfo");
            //         temptaghu.Hucard = parseInt(Math.random(17, 22));
            //         temptaghu.pasHuchairid = parseInt(Math.random(0, 3));
            //     }
            //     //杠牌
            //     var tempendganghu = createPacket("proto.game.scmj.tagEndGangHu");
            //     scgameend.GangInfo = 10;
            //     tempendganghu = scgameend.GangInfo;
            //     for (var j = 0; j < 7; j++) {
            //         var tempganghu = createPacket("proto.game.scmj.tagGangHu");//tempendganghu.GangHu;
            //         var a = 0;
            //         var b = parseInt(Math.random(0, 6));
            //         if (b > 0) {
            //             a = 2;
            //         }
            //         for (var m = 0; m < b; m++) {
            //             a *= 2;
            //         }
            //         tempganghu.Type = (a);
            //         tempganghu.Chairid = parseInt(Math.random(0, 3));
            //         tempganghu.Card = parseInt(16 + Math.random(1, 8));
            //     }
            //     //退杠
            //     var tempreturnganginfo = createPacket("proto.game.scmj.ReturnGangInfo");
            //     scgameend.Returngangscore = 1;
            //     tempreturnganginfo = scgameend.Returngangscore;
            //     for (var j = 0; j < 3; j++) {
            //         tempreturnganginfo.State = parseInt(Math.random(0, 3));
            //         tempreturnganginfo.Card = parseInt(16 + Math.random(1, 8));
            //         tempreturnganginfo.Returngangscore = parseInt(Math.random(-1000, 1000));
            //     }
            //     //花猪大叫
            //     var temphuazhudajiao = createPacket("proto.game.scmj.tagHuazhudajiao");
            //     temphuazhudajiao = scgameend.Huazhudajiao;
            //     var a = parseInt(Math.random(0, 5));
            //     a = 1;
            //     temphuazhudajiao.ishuazhudajiao = (a);
            //     cc.log("ddddddddddd---p:" + a);
            //     temphuazhudajiao.Huazhuscore = (2);
            //     temphuazhudajiao.Dajiaoscore = (2);
            //     switch (a) {
            //         case 0:
            //             break;
            //         case 1:
            //             temphuazhudajiao.Huazhuscore = (-1000);
            //             break;
            //         case 2:
            //             temphuazhudajiao.Dajiaoscore = (-100);
            //             break;
            //         case 3:
            //             temphuazhudajiao.Huazhuscore = (1000);
            //             break;
            //         case 4:
            //             temphuazhudajiao.Dajiaoscore = (300);
            //             break;
            //         case 5:
            //             temphuazhudajiao.Huazhuscore = (1333);
            //             temphuazhudajiao.Dajiaoscore = (4646);
            //             break;
            //         default:
            //             break;
            //     }
            //     gameEnd.scEndInfo = scgameend;
            //     this.ackGameEnd = gameEnd;
            // }

            //箭头说明
            var arrowDetail;
            if (MJModel.mj_roomType == 1) {
                arrowDetail = new cc.Sprite(getResPath("RoomMJ/result-detail/result_detail_arrow_detail2.png"));
            } else {
                arrowDetail = new cc.Sprite(getResPath("RoomMJ/result-detail/result_detail_arrow_detail.png"));
            }
            if (arrowDetail) {
                arrowDetail.setAnchorPoint(cc.p(0.0, 0.5));
                arrowDetail.setPosition(cc.p(50, 80));
                this.bgSpr.addChild(arrowDetail);
            }

            if (MJModel.valueGuiPai) {
                var bg = new cc.Sprite();
                bg.setContentSize(cc.size(113, 64));

                var sp = new cc.Scale9Sprite(getResPath("RoomMJ/result-small/bg_frame_9.png"));
                sp.setAnchorPoint(cc.p(0.5, 0.5));
                sp.setPosition(cc.p(bg.getContentSize().width / 2, bg.getContentSize().height / 2));
                sp.setContentSize(cc.size(113, 64));
                bg.addChild(sp);

                //鬼牌
                var GuiPaibg = bg;
                GuiPaibg.setAnchorPoint(cc.p(0, 0.5));
                GuiPaibg.setPosition(cc.p(this.bgSpr.getContentSize().width / 2 + 100, 80));

                var guipaiTxt = new cc.Sprite(getResPath("RoomMJ2/guipai_txt.png"));
                guipaiTxt.setPosition(cc.p(30, GuiPaibg.getContentSize().height / 2));
                GuiPaibg.addChild(guipaiTxt);

                var mj_card = new cc.Sprite(getResPath("RoomMJ2/card_bg/hand_card_0.png"));
                mj_card.setPosition(cc.p(90, GuiPaibg.getContentSize().height / 2));
                mj_card.setTag(102);
                mj_card.setScale(0.45);

                var value = MJModel.valueGuiPai;
                this.buff = __String.createWithFormat(getResPath("RoomMJ2/card_value/card_0_%1.png"), Utils.toHex2(value));
                var num_texture = new cc.Sprite(this.buff);

                if (num_texture) {
                    num_texture.setPosition(cc.p(mj_card.getContentSize().width / 2, mj_card.getContentSize().height / 2 - 10));
                    mj_card.addChild(num_texture);
                }

                GuiPaibg.addChild(mj_card);
                this.bgSpr.addChild(GuiPaibg);
            }

            for (var m = 0; m < 4; m++) {
                var pos = m - 1 < 0 ? 3 : m - 1;
                if (pos == 2 && MJModel.mj_roomType == 1) {
                    continue;
                }

                var startY = 620 - m * 140;

                var seatid = MJModel.getSeatidByPos(pos);
                var scGameEnd = this.ackGameEnd.scEndInfo;

                //头像
                var startX = 100;
                var headIcon = this.createAvatar(pos);
                headIcon.setPosition(cc.p(startX, startY));
                headIcon.setAnchorPoint(cc.p(0.5, 0.5));
                this.bgSpr.addChild(headIcon);

                //上排杠胡牌
                startX = 180;

                var ganghuLayer = this.createGangHuLayer(seatid);
                this.bgSpr.addChild(ganghuLayer);
                ganghuLayer.setPosition(cc.p(startX, startY + 15));

                //下排所有牌
                var layer = this.createHandCard(seatid);
                layer.setPosition(cc.p(startX, startY - 50));
                this.bgSpr.addChild(layer);
                //花猪、大叫信息
                startX = 920;
                cc.log("----pppppppp----:" + scGameEnd.Huazhudajiao.length + "----ppppsetatid-----:" + seatid);
                if (scGameEnd.Huazhudajiao.length > seatid) {
                    var huazhudajiao = scGameEnd.Huazhudajiao[seatid];
                    cc.log("one--------huazhudajiao.ishuazhudajiao-------" + huazhudajiao.Huazhuscore + "-----huazhudajiao.Dajiaoscore-----" + huazhudajiao.Dajiaoscore);
                    if (huazhudajiao.ishuazhudajiao != 0) {
                        if (huazhudajiao.Huazhuscore != 0) {
                            cc.log("sssssssss----");
                            var huazhu = new cc.Sprite();
                            huazhu.setAnchorPoint(cc.p(0.5, 0.5));
                            this.bgSpr.addChild(huazhu);
                            var addH = 0;
                            if (huazhudajiao.Huazhuscore > 0) {
                                huazhu.setTexture(getResPath("RoomMJ/result-detail/beihuazhupeifu.png"));
                                //this.buff = ":" + Math.abs(huazhudajiao.Huazhuscore);
                                this.buff = "+" + Math.abs(huazhudajiao.Huazhuscore);
                                addH = 15;
                            } else {
                                huazhu.setTexture(getResPath("RoomMJ/result-detail/huazhu.png"));
                                //this.buff = ";" + Math.abs(huazhudajiao.Huazhuscore);
                                this.buff = "-" + Math.abs(huazhudajiao.Huazhuscore);
                            }
                            // var huazhuTex = new cc.LabelAtlas(this.buff, getResPath("RoomMJ/result-small/result_small_mutiple.png"), 20, 25, '0');
                            var huazhuTex = new cc.LabelTTF(this.buff, "Arial", 24);
                            huazhuTex.setAnchorPoint(cc.p(0.5, 0.5));
                            huazhuTex.setColor(this.rightColor);
                            this.bgSpr.addChild(huazhuTex);
                            if (huazhudajiao.Dajiaoscore == 0) {
                                huazhu.setPosition(cc.p(startX, startY + 20 + addH));
                            } else {
                                huazhu.setPosition(cc.p(startX, startY + 35 + addH));
                            }
                            huazhuTex.setPosition(cc.p(startX, huazhu.getPosition().y - huazhu.getContentSize().height / 2 - 10));
                        }
                        cc.log("two--------huazhudajiao.ishuazhudajiao-------" + huazhudajiao.Huazhuscore + "-----huazhudajiao.Dajiaoscore-----" + huazhudajiao.Dajiaoscore);
                        if (huazhudajiao.Dajiaoscore != 0) {
                            cc.log("go into Dajiaoscore-------");
                            var dajiao = new cc.Sprite();
                            dajiao.setAnchorPoint(cc.p(0.5, 0.5));
                            this.bgSpr.addChild(dajiao);

                            if (huazhudajiao.Dajiaoscore > 0) {
                                dajiao.setTexture(getResPath("RoomMJ/result-detail/beidajiaopeifu.png"));
                                //this.buff = ":" + Math.abs(huazhudajiao.Dajiaoscore);
                                this.buff = "+" + Math.abs(huazhudajiao.Dajiaoscore);
                            } else {
                                dajiao.setTexture(getResPath("RoomMJ/result-detail/dajiao.png"));
                                //this.buff = ";" + Math.abs(huazhudajiao.Dajiaoscore);
                                this.buff = "-" + Math.abs(huazhudajiao.Dajiaoscore);
                            }
                            //var dajiaoTex = new cc.LabelAtlas(this.buff, getResPath("RoomMJ/result-small/result_small_mutiple.png"), 20, 25, '0');
                            var dajiaoTex = new cc.LabelTTF(this.buff, "Arial", 24);
                            dajiaoTex.setAnchorPoint(cc.p(0.5, 0.5));
                            dajiaoTex.setColor(this.rightColor);
                            this.bgSpr.addChild(dajiaoTex);
                            if (huazhudajiao.Huazhuscore == 0) {
                                dajiao.setPosition(cc.p(startX, startY + 20));
                            } else {
                                dajiao.setPosition(cc.p(startX, startY - 25));
                            }
                            dajiaoTex.setPosition(cc.p(startX, dajiao.getPosition().y - dajiao.getContentSize().height / 2 - 10));
                        }
                    }
                }

                //总成绩、胡牌信息
                startX = 980;
                var scoreLayer = this.createScoreLayer(seatid);
                scoreLayer.setPosition(cc.p(startX, startY + 30));
                this.bgSpr.addChild(scoreLayer);

                //胡牌列表
                var huLayer = this.createHuLayer(seatid);
                if (huLayer) {
                    this.bgSpr.addChild(huLayer);
                    huLayer.setPosition(cc.p(startX, startY + 20));
                } else {
                    scoreLayer.setPosition(cc.p(startX, startY - 10));
                }
            }
        },

        updateGD: function (data) {
            this.ackGameEnd = parsePacket("proto.game.AckGameEnd", data);

            if (MJModel.gdmj_playid != 1) {
                this.buff = __String.createWithFormat(getResPath("RoomMJ/gdmj/zhuafeng/result-%1.png"), MJModel.gdmj_fengquan);
                var fengquanSpr = new cc.Sprite(this.buff);
                fengquanSpr.setAnchorPoint(cc.p(0, 0.5));
                this.bgSpr.addChild(fengquanSpr);
                fengquanSpr.setPosition(cc.p(50, 80));
            }

            if (MJModel.valueGuiPai) {
                var bg = new cc.Sprite();
                bg.setContentSize(cc.size(113, 64));

                var sp = new cc.Scale9Sprite(getResPath("RoomMJ/result-small/bg_frame_9.png"));
                sp.setAnchorPoint(cc.p(0.5, 0.5));
                sp.setPosition(cc.p(bg.getContentSize().width / 2, bg.getContentSize().height / 2));
                sp.setContentSize(cc.size(113, 64));
                bg.addChild(sp);

                //鬼牌
                var GuiPaibg = bg;
                GuiPaibg.setAnchorPoint(cc.p(0, 0.5));
                GuiPaibg.setPosition(cc.p(180, 80));

                var guipaiTxt = new cc.Sprite(getResPath("RoomMJ2/guipai_txt.png"));
                guipaiTxt.setPosition(cc.p(30, GuiPaibg.getContentSize().height / 2));
                GuiPaibg.addChild(guipaiTxt);

                var mj_card = new cc.Sprite(getResPath("RoomMJ2/card_bg/hand_card_0.png"));
                mj_card.setPosition(cc.p(90, GuiPaibg.getContentSize().height / 2));
                mj_card.setTag(102);
                mj_card.setScale(0.45);

                var value = MJModel.valueGuiPai;

                this.buff = __String.createWithFormat(getResPath("RoomMJ2/card_value/card_0_%1.png"), Utils.toHex2(value));
                var num_texture = new cc.Sprite(this.buff);

                if (num_texture) {
                    num_texture.setPosition(cc.p(mj_card.getContentSize().width / 2, mj_card.getContentSize().height / 2 - 10));
                    mj_card.addChild(num_texture);
                }

                GuiPaibg.addChild(mj_card);
                this.bgSpr.addChild(GuiPaibg);
            }

            var itemHeight = 140;

            for (var m = 0; m < 4; m++) {
                var pos = m - 1 < 0 ? 3 : m - 1;
                var seatid = MJModel.getSeatidByPos(pos);

                var startY = 620 - m * itemHeight;
                var startX = 100;

                //头像
                var headLayer = this.createAvatar(pos);
                headLayer.setPosition(cc.p(startX, startY));
                this.bgSpr.addChild(headLayer);

                startX = 180;
                //上排牌 杠胡、花牌
                var ganghuLayer = this.createGangHuLayer(seatid);
                this.bgSpr.addChild(ganghuLayer);
                ganghuLayer.setPosition(cc.p(startX, startY + 15));

                //下排牌
                var handCardLayer = this.createHandCard(seatid);
                handCardLayer.setPosition(cc.p(startX, startY - 50));
                this.bgSpr.addChild(handCardLayer);

                startX = 900;
                if (this.ackGameEnd.gdGameEndInfo && this.ackGameEnd.gdGameEndInfo.faninfo && this.ackGameEnd.gdGameEndInfo.faninfo.length > seatid) {
                    //胡牌类型
                    var fanInfo = this.ackGameEnd.gdGameEndInfo.faninfo[seatid];
                    var path = "";
                    switch (fanInfo.SpecialType) {
                        case 1:
                            path = "RoomMJ/result-detail/result_detail_zimo.png";
                            break;
                        case 3:
                            path = "RoomMJ/result-detail/result_detail_dianpao.png";
                            break;
                        case 2:
                            path = "RoomMJ/result-detail/result_detail_paohu.png";
                            break;
                        case 4:
                            path = "RoomMJ/result-detail/result_detail_beizimo.png";
                            break;
                        case 5:
                            path = "RoomMJ/result-detail/result_detail_baozimo.png";
                            break;
                        case 6:
                            path = "RoomMJ/result-detail/result_detail_zimo.png";
                            break;
                        case 7:
                            path = "RoomMJ/result-detail/result_detail_baopai.png";
                            break;
                        case 8:
                            path = "RoomMJ/result-detail/result_detail_tianhe.png";
                            break;
                        case 9:
                            path = "RoomMJ/result-detail/result_detail_dihe.png";
                            break;
                        case 10:
                            path = "RoomMJ/result-detail/result_detail_renhe.png";
                            break;
                        case 11:
                            path = "RoomMJ/result-detail/result_detail_qiangganghu.png";
                            break;
                        case 12:
                            path = "RoomMJ/result-detail/result_detail_haidi.png";
                            break;
                        case 13:
                            path = "RoomMJ/result-detail/result_detail_gangshang.png";
                            break;
                        default:
                            break;
                    }
                    if (path != "") {
                        var typeSprite = new cc.Sprite(getResPath(path));
                        typeSprite.setPosition(cc.p(startX + 20, startY + 20));
                        this.bgSpr.addChild(typeSprite);
                        if ((fanInfo.FanType >= 0 && fanInfo.FanNum >= 0 && MJModel.gdmj_playid == 0) ||
                            (fanInfo.FanType >= 0 && fanInfo.FanNum > 0 && MJModel.gdmj_playid == 1)) {

                            var fanStrArr = ["十三幺", "九莲宝灯", "大四喜", "大三元", "四暗刻", "十八罗汉", "清幺九", "字一色", "小四喜", "小三元", "混幺九", "七对子", "清碰", "混碰", "清一色", "混一色", "碰碰胡", "平胡", "鸡胡", "天胡", "地胡", "人胡", "抢杠胡", "海底捞月", "杠上开花", "四大金刚", "七花自摸"];
                            var huStrArr = ["小胡", "大胡"];

                            var fantype = "";
                            if (MJModel.gdmj_playid == 0) {
                                fantype = fanStrArr[fanInfo.FanType];
                                this.buff = fanInfo.FanNum + "番";
                            } else {
                                fantype = huStrArr[fanInfo.FanType];
                                this.buff = fanInfo.FanNum + "倍";
                            }

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


        ShowGZMJData: function (data) {
            this.ackGameEnd = parsePacket("proto.game.AckGameEnd", data);

            var size = cc.director.getWinSize();
            for (var m = 0; m < 4; m++) {
                var pos = m - 1 < 0 ? 3 : m - 1;

                var startX = 100;
                var startY = 620 - m * 140;

                var seatid = MJModel.getSeatidByPos(pos);

                var headIcon = this.createAvatar(pos);
                headIcon.setPosition(cc.p(startX, startY));
                headIcon.setAnchorPoint(cc.p(0.5, 0.5));
                this.bgSpr.addChild(headIcon);
                //是否报听
                if (this.ackGameEnd.GuiZhouEndInfo) {
                    if (this.ackGameEnd.GuiZhouEndInfo.IsBaoTing.length > seatid) {
                        var isbaoting = this.ackGameEnd.GuiZhouEndInfo.IsBaoTing[seatid];
                        if (isbaoting) {
                            var baotingPic = new cc.Sprite(getResPath("RoomMJ/tingpai_flag.png"));
                            baotingPic.setAnchorPoint(cc.p(0.0, 0.5));
                            baotingPic.setPosition(cc.p(startX + 20, startY - 18));
                            this.bgSpr.addChild(baotingPic);
                        }
                    }
                }
                //杠牌信息列表
                //上排牌 杠胡、花牌
                startX = 180;
                var ganghuLayer = this.createGangHuLayer(seatid);
                this.bgSpr.addChild(ganghuLayer);
                ganghuLayer.setPosition(cc.p(startX, startY + 15));
                //下排手牌信息
                var layer = this.createHandCard(seatid);
                layer.setAnchorPoint(cc.p(0.0, 0.0));
                layer.setPosition(cc.p(startX, startY - 50));
                this.bgSpr.addChild(layer);

                startX = 920;
                //胡牌类型
                var hupaipath = "";
                if (seatid < this.ackGameEnd.EndInfo.length) {
                    var t = this.ackGameEnd.EndInfo[seatid].EndType;
                    switch (t) {
                        case 1:
                            hupaipath = "RoomMJ/result-detail/result_detail_zimo.png";
                            break;
                        case 3:
                            hupaipath = "RoomMJ/result-detail/result_detail_dianpao.png";
                            break;
                        case 2:
                            hupaipath = "RoomMJ/result-detail/result_detail_paohu.png";
                            break;
                        case 5:
                            hupaipath = "RoomMJ/result-detail/result_detail_hangkaihua.png";
                            break;
                        case 6:
                            hupaipath = "RoomMJ/result-detail/result_detail_baopai.png";
                            break;
                        default:
                            break;
                    }
                    //是否杀报


                }
                if (seatid < this.ackGameEnd.EndInfo.length) {
                    //杀报
                    if (this.ackGameEnd.EndInfo[seatid].BaoTingType) {
                        var shabaotype = this.ackGameEnd.EndInfo[seatid].BaoTingType;
                        if (shabaotype == 1) {
                            hupaipath = "RoomMJ/result-detail/result_detail_shabao.png";
                        }
                    }
                }
                if (hupaipath != "") {
                    var hupaiTypePic = new cc.Sprite(getResPath(hupaipath));
                    hupaiTypePic.setAnchorPoint(cc.p(0.5, 0.5));
                    hupaiTypePic.setPosition(cc.p(startX - 5, startY + 5));
                    this.bgSpr.addChild(hupaiTypePic);
                }
                //具体胡牌类型
                if (seatid < this.ackGameEnd.GuiZhouEndInfo.HuFanType.length) {
                    hupaipath = "";
                    var type = this.ackGameEnd.GuiZhouEndInfo.HuFanType[seatid];
                    if (type > 0) {
                        var str = "";
                        switch (type) {
                            case 1:
                                str = "天胡";
                                break;
                            case 2:
                                str = "地胡";
                                break;
                            case 3:
                                str = "清龙背";
                                break;
                            case 4:
                                str = "清幺九";
                                break;
                            case 5:
                                str = "清七对";
                                break;
                            case 6:
                                str = "将对";
                                break;
                            case 7:
                                str = "龙七对";
                                break;
                            case 8:
                                str = "清大对";
                                break;
                            case 9:
                                str = "带幺九";
                                break;
                            case 10:
                                str = "七对";
                                break;
                            case 11:
                                str = "清一色";
                                break;
                            case 12:
                                str = "大对子";
                                break;
                            case 13:
                                str = "平胡";
                                break;
                            default:
                                str = "";
                                break;
                        }
                        //var hupaiTypePic = new cc.Sprite(__String.createWithFormat(getResPath("RoomMJ/GZMJ/result_detail_hupai_type_%1.png"), type));
                        if (str != "") {
                            var hupaiTypePic = new cc.LabelTTF(str, "Arial", 24);
                            hupaiTypePic.setColor(this.rightColor);
                            hupaiTypePic.setAnchorPoint(cc.p(0.5, 0.5));
                            this.bgSpr.addChild(hupaiTypePic);
                            var ratio = this.ackGameEnd.GuiZhouEndInfo.HuFanNums[seatid];
                            //var goldLabel = new cc.LabelAtlas("<" + ratio, getResPath("RoomMJ/GZMJ/result_small_score_nums.png"), 20, 25, '0');
                            var goldLabel = new cc.LabelTTF("X" + ratio, "Arial", 24);
                            goldLabel.setColor(this.rightColor);
                            goldLabel.setAnchorPoint(cc.p(0.0, 0.5));
                            this.bgSpr.addChild(goldLabel);
                            var ln = goldLabel.getBoundingBox().width;
                            hupaiTypePic.setPosition(startX - ln / 2 - 5, startY - 45);
                            goldLabel.setPosition(hupaiTypePic.getPositionX() + hupaiTypePic.getContentSize().width / 2 + 5, startY - 45);
                        }

                    }

                }

                startX = 980;
                //总成绩
                var scoreLayer = this.createScoreLayer(seatid);
                scoreLayer.setPosition(cc.p(startX, startY + 30));
                this.bgSpr.addChild(scoreLayer);
                //包鸡
                var isHasChongFengji = false;
                var isHasZeRenJi = false;
                var isBaoJi = false;
                if (this.ackGameEnd.GuiZhouEndInfo) {
                    if (this.ackGameEnd.GuiZhouEndInfo.ChongFengJiID) {
                        if (seatid == this.ackGameEnd.GuiZhouEndInfo.ChongFengJiID) {
                            isHasChongFengji = true;
                        }

                    }
                    if (this.ackGameEnd.GuiZhouEndInfo.ZeRenJiID) {
                        if (seatid == this.ackGameEnd.GuiZhouEndInfo.ZeRenJiID) {
                            isHasZeRenJi = true;
                        }
                    }
                    if (this.ackGameEnd.GuiZhouEndInfo.IsBaoJi) {
                        if (this.ackGameEnd.GuiZhouEndInfo.IsBaoJi) {
                            isBaoJi = true;
                        }
                    }
                    if (isBaoJi && isHasChongFengji) {
                        //是否包鸡
                        var zhongmaPic = new cc.Sprite(getResPath("RoomMJ/GZMJ/result_small_baoji_flag.png"));
                        zhongmaPic.setAnchorPoint(cc.p(0.0, 0.0));
                        zhongmaPic.setPosition(cc.p(startX, startY - 60));
                        this.bgSpr.addChild(zhongmaPic);
                    }
                }
                //鸡牌列表

                var allJipaiSize = this.ackGameEnd.GuiZhouEndInfo.AllJiPais.length;
                if (allJipaiSize > seatid) {
                    var jipai_nums = this.ackGameEnd.GuiZhouEndInfo.AllJiPais[seatid].JiPais.length;
                    if (jipai_nums > 0) {
                        var zhongmaPic = new cc.Sprite(getResPath("RoomMJ/GZMJ/result_small_jipai_flag.png"));
                        zhongmaPic.setAnchorPoint(cc.p(0.0, 0.0));
                        zhongmaPic.setPosition(cc.p(startX, startY - 10));
                        this.bgSpr.addChild(zhongmaPic);
                        var cardx = startX + zhongmaPic.getContentSize().width + 20;
                        var isHasJinJi = false;
                        var jiindex = 0;
                        var leftX = 0;
                        for (var i = 0; i < jipai_nums; i++) {
                            var maCard = new MJCard();
                            maCard.setAnchorPoint(cc.p(0.0, 0.0));
                            var value = this.ackGameEnd.GuiZhouEndInfo.AllJiPais[seatid].JiPais[i];
                            isHasJinJi = this.ackGameEnd.GuiZhouEndInfo.AllJiPais[seatid].IsJinJi[i];
                            maCard.setValue(value, 0, CardType.Card_End, 0);
                            maCard.setScale(0.53);
                            maCard.setPosition(cc.p(cardx + (i % 6) * maCard.getContentSize().width * 0.53, startY - parseInt((i / 6)) * maCard.getContentSize().height * 0.5));
                            this.bgSpr.addChild(maCard, i);
                            if (jiindex == 0) {
                                if ((isHasJinJi && isHasChongFengji) || (isHasJinJi && isHasZeRenJi)) {
                                    leftX = 10;
                                }
                            }

                            if (value == 0x11 && isHasJinJi) {
                                //金鸡
                                var sp = maCard.getChildByName("sprite");
                                if (sp) {
                                    var card_size = sp.getBoundingBox();
                                    var xiaoji_flag = new cc.Sprite(getResPath("RoomMJ/GZMJ/gz_xiaoji_type_3.png"));
                                    xiaoji_flag.setPosition(card_size.width / 2 - leftX, card_size.height);
                                    maCard.addChild(xiaoji_flag);
                                }

                            }
                            if (value == 17 && isHasChongFengji & (jiindex == 0)) {
                                //冲锋鸡
                                var sp = maCard.getChildByName("sprite");
                                if (sp) {
                                    var card_size = sp.getBoundingBox();
                                    var xiaoji_flag = new cc.Sprite(getResPath("RoomMJ/GZMJ/gz_xiaoji_type_1.png"));
                                    xiaoji_flag.setPosition(card_size.width / 2 + leftX, card_size.height);
                                    maCard.addChild(xiaoji_flag);
                                }
                                jiindex++;
                                leftX = 0;
                            }
                            if (value == 0x11 && isHasZeRenJi & (jiindex == 0)) {
                                //责任鸡
                                var sp = maCard.getChildByName("sprite");
                                if (sp) {
                                    var card_size = sp.getBoundingBox();
                                    var xiaoji_flag = new cc.Sprite(getResPath("RoomMJ/GZMJ/gz_xiaoji_type_2.png"));
                                    xiaoji_flag.setPosition(card_size.width / 2 + leftX, card_size.height);
                                    maCard.addChild(xiaoji_flag);
                                }
                                jiindex++;
                                leftX = 0;
                            }
                        }
                    }

                }

            }


        },

        ShowHuangZhuangData: function (data) {

            this.ackGameEnd = parsePacket("proto.game.AckGameEnd", data);

            var size = cc.director.getWinSize();
            for (var m = 0; m < 4; m++) {
                var pos = m - 1 < 0 ? 3 : m - 1;

                var startX = 100;
                var startY = 620 - m * 140;

                var seatid = MJModel.getSeatidByPos(pos);

                var headIcon = this.createAvatar(pos);
                headIcon.setPosition(cc.p(startX, startY));
                headIcon.setAnchorPoint(cc.p(0.5, 0.5));
                this.bgSpr.addChild(headIcon);
                startX = 180;
                //杠牌信息列表
                var ganghuLayer = this.createGangHuLayer(seatid);
                this.bgSpr.addChild(ganghuLayer);
                ganghuLayer.setPosition(cc.p(startX, startY + 30));
                //下排手牌信息
                var layer = this.createHandCard(seatid);
                layer.setAnchorPoint(cc.p(0.0, 0.0));
                layer.setPosition(cc.p(startX, startY - 50));
                this.bgSpr.addChild(layer);


                startX = 920;
                //是否听牌
                var hupaipath = "";
                if (this.ackGameEnd.isDajiao.length > seatid) {
                    var IsListen = this.ackGameEnd.isDajiao[seatid];
                    if (!IsListen) {
                        hupaipath = getResPath("RoomMJ/GZMJ/result_detail_tingpai.png");
                    }
                }

                if (hupaipath != "") {
                    var hupaiTypePic = new cc.Sprite(hupaipath);
                    hupaiTypePic.setAnchorPoint(cc.p(0.5, 0.5));
                    hupaiTypePic.setPosition(cc.p(startX, startY));
                    this.bgSpr.addChild(hupaiTypePic);

                    //具体胡牌类型
                    if (seatid < this.ackGameEnd.GuiZhouEndInfo.HuFanType.length) {
                        var type = this.ackGameEnd.GuiZhouEndInfo.HuFanType[seatid];
                        if (type > 0) {
                            var hupaiTypePic = new cc.Sprite(__String.createWithFormat(getResPath("RoomMJ/GZMJ/result_detail_hupai_type_%1.png"), type));
                            if (hupaiTypePic) {
                                hupaiTypePic.setAnchorPoint(cc.p(0.5, 0.5));
                                this.bgSpr.addChild(hupaiTypePic);
                                var ratio = this.ackGameEnd.GuiZhouEndInfo.HuFanNums[seatid];
                                var goldLabel = new cc.LabelAtlas("<" + ratio, getResPath("RoomMJ/GZMJ/result_small_score_nums.png"), 20, 25, '0');
                                goldLabel.setAnchorPoint(cc.p(0.0, 0.5));
                                this.bgSpr.addChild(goldLabel);
                                var ln = goldLabel.getBoundingBox().width;
                                hupaiTypePic.setPosition(startX + 10 - ln / 2, startY - 45);
                                goldLabel.setPosition(hupaiTypePic.getPositionX() + hupaiTypePic.getContentSize().width / 2 + 5, startY - 45);
                            }

                        }

                    }
                }
                startX += 80;

                var path1 = "";
                var path2 = "";
                var score = this.ackGameEnd.Score[seatid];
                if (score < 0) {
                    path1 = getResPath("RoomMJ/result-small/result_small_total_lose.png");
                    path2 = getResPath("RoomMJ/result-small/result_small_label_lose.png");
                    this.buff = ";" + Math.abs(score);
                } else if (score > 0) {
                    path1 = getResPath("RoomMJ/result-small/result_small_total_win.png");
                    path2 = getResPath("RoomMJ/result-small/result_small_label_win.png");
                    this.buff = ";" + Math.abs(score);
                } else {
                    path1 = getResPath("RoomMJ/result-small/result_small_total_lose.png");
                    path2 = getResPath("RoomMJ/result-small/result_small_label_lose.png");
                    this.buff = "0";
                }

                startX = 980;
                //总成绩

                var totallabelpath = getResPath("RoomMJ/result-small/result_total.png");
                var scorelabelpath = getResPath("RoomMJ/result-small/result_small_label_lose.png");
                if (score > 0) {
                    scorelabelpath = getResPath("RoomMJ/result-small/result_small_label_win.png");
                }
                var totalLabelSprite = new cc.Sprite(totallabelpath);
                totalLabelSprite.setAnchorPoint(cc.p(0, 0.5));
                totalLabelSprite.setPosition(cc.p(startX, startY));
                this.bgSpr.addChild(totalLabelSprite);

                var scoreLabel = new cc.LabelAtlas(":" + Math.abs(score), scorelabelpath, 20, 24, '0');
                scoreLabel.setAnchorPoint(cc.p(0.0, 0.5));
                scoreLabel.setPosition(cc.p(totalLabelSprite.getPositionX() + totalLabelSprite.getContentSize().width + 10, startY));
                this.bgSpr.addChild(scoreLabel);
            }
        },

        createHandCard: function (seatid) {

            var layer = new cc.Layer();
            if (this.ackGameEnd.UserCard.length > seatid) {
                var x = 0;
                var y = 0;
                var paiSpace = 10;
                var penggangSize = cc.size(120, 80);
                for (var i = 0; i < this.ackGameEnd.UserCard[seatid].FixedCardsLen; i++) {

                    var state = this.ackGameEnd.UserCard[seatid].stFixedCards[i].state;
                    var card = this.ackGameEnd.UserCard[seatid].stFixedCards[i].CardData;

                    if (card == undefined || card == null || card == 0) {
                        continue;
                    }
                    //下排杠牌
                    var gangCard = new MJPeng();
                    gangCard.setAnchorPoint(cc.p(0.0, 0.0));

                    if ((state & ActionType.TYPE_PENG) == ActionType.TYPE_PENG) {
                        gangCard.setValue(PengType.Peng_Peng, 0, card, 0);
                    }
                    else if ((state & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG) {
                        gangCard.setValue(PengType.Peng_AnKang, 0, card, 0);
                    }
                    else if ((state & ActionType.TYPE_LEFT_CHI) == ActionType.TYPE_LEFT_CHI) {
                        gangCard.setValue(PengType.Chi_L, 0, card, 0);
                    }
                    else if ((state & ActionType.TYPE_CENTER_CHI) == ActionType.TYPE_CENTER_CHI) {
                        gangCard.setValue(PengType.Chi_M, 0, card, 0);
                    }
                    else if ((state & ActionType.TYPE_RIGHT_CHI) == ActionType.TYPE_RIGHT_CHI) {
                        gangCard.setValue(PengType.Chi_R, 0, card, 0);
                    }
                    else {
                        gangCard.setValue(PengType.Peng_Kang, 0, card, 0);
                    }

                    gangCard.setPosition(cc.p(x - 10, y - 10));
                    gangCard.setScale(0.65);
                    x += paiSpace * 0.5 + penggangSize.width + 14;
                    layer.addChild(gangCard);
                }

                var is_remove = 0;
                //牌面信息
                for (var i = 0; i < this.ackGameEnd.UserCard[seatid].ChangeableCardsLen; i++) {

                    if (MJModel.curGameType != ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ && MJModel.curGameType != ROOM_TYPE.ROOM_TYPE_SCMJ_DZ && MJModel.curGameType != ROOM_TYPE.ROOM_TYPE_YNMJ_DZ && is_remove == 0 && this.ackGameEnd.EndInfo.length > seatid && this.ackGameEnd.EndInfo[seatid].EndType == 1) {
                        if (this.ackGameEnd.UserCard[seatid].ChangeableCards[i] == this.ackGameEnd.EndInfo[seatid].HuCard) {
                            is_remove = 1;
                            continue;
                        }
                    }
                    var handCard = new MJCard();
                    handCard.setValue(this.ackGameEnd.UserCard[seatid].ChangeableCards[i], 0, CardType.Card_End, 0);
                    //            handCard.setScale(0.67);
                    handCard.setScale(0.8);
                    handCard.setAnchorPoint(cc.p(0.0, 0.0));
                    handCard.setPosition(cc.p(x, y - 6));
                    x += handCard.getBoundingBox().width;
                    handCard.showGuiPai();
                    layer.addChild(handCard);
                }

                //胡的牌
                if (MJModel.curGameType != ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ && MJModel.curGameType != ROOM_TYPE.ROOM_TYPE_SCMJ_DZ && MJModel.curGameType != ROOM_TYPE.ROOM_TYPE_YNMJ_DZ) {
                    if (this.ackGameEnd.EndInfo.length > seatid && this.ackGameEnd.EndInfo[seatid].HuCard != 0) {
                        var huCard = new MJCard();
                        huCard.setValue(this.ackGameEnd.EndInfo[seatid].HuCard, 0, CardType.Card_End, 0);
                        huCard.setScale(0.8);
                        huCard.setAnchorPoint(cc.p(0.0, 0.0));
                        huCard.setPosition(cc.p(x + paiSpace, y - 6));
                        huCard.showGuiPai();
                        layer.addChild(huCard);
                    }
                }
            }

            return layer;
        }
        ,

        createSmallCard: function (val) {
            var paiBg = new cc.Sprite(getResPath("RoomMJ/result-detail/result_detail_frame_gang.png"));

            var num_str = MJConfig.getCardValueStr(val, 0, CardType.Card_Hand);

            if (num_str != "") {
                var num_sp = new cc.Sprite(num_str);
                num_sp.setScale(0.35);
                num_sp.setAnchorPoint(cc.p(0.5, 0.5));
                num_sp.setPosition(cc.p(paiBg.getContentSize().width / 2, paiBg.getContentSize().height / 2));
                paiBg.addChild(num_sp);
            }
            return paiBg;
        }
        ,

        createAvatar: function (pos) {
            var seatid = MJModel.getSeatidByPos(pos);
            var player = MJModel.players[seatid];
            var layer = new cc.Layer();

            var headPic = Utils.createCircleAvatar(player.avatar, "Avatars/user4_unlogin.png", getResPath("RoomMJ2/head_bg.png"), cc.size(74, 74));
            layer.addChild(headPic);
            //    headPic.setPosition(cc.p(startX,startY));

            var framepath;
            var scale = 1;
            if (pos == 0) {
                framepath = getResPath("RoomMJ/result-detail/result_detail_frame_wo.png");
            } else {
                framepath = getResPath("RoomMJ/result-detail/result_detail_frame_others.png");
                scale = 0.82;
            }
            var headFrame = new cc.Sprite(framepath);
            headFrame.setAnchorPoint(cc.p(0.5, 0.5));
            headFrame.setScale(scale);
            headFrame.setPosition(cc.p(0, 0));
            layer.addChild(headFrame, -1);

            var headSize = cc.size(88, 88);

            //是否是庄家
            if (MJModel.banker_seatid == seatid) {
                var bankerPic = new cc.Sprite(getResPath("RoomMJ/banker.png"));
                bankerPic.setAnchorPoint(cc.p(0.5, 0));
                bankerPic.setPosition(cc.p(-headSize.width / 2 + 5, -headSize.height / 2));
                layer.addChild(bankerPic);
            }

            //位置
            var posPic = new cc.Sprite(__String.createWithFormat(getResPath("RoomMJ/result-detail/result_detail_pos_%1.png"), pos));
            posPic.setAnchorPoint(cc.p(0.5, 0));
            posPic.setPosition(cc.p(0, headSize.height / 2 - 10));
            layer.addChild(posPic);

            //玩家名字
            var nameLabel = new cc.LabelTTF(Utils.parseName(4, player.name), "Arial", 24);
            nameLabel.setColor(this.leftColor);
            nameLabel.setAnchorPoint(cc.p(0.5, 0.5));
            nameLabel.setPosition(cc.p(0, -headSize.height / 2 - 5));
            layer.addChild(nameLabel);

            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GDMJ_DZ && MJModel.gdmj_playid != 1) {
                var quanSprite = new cc.Sprite(__String.createWithFormat(getResPath("RoomMJ/gdmj/zhuafeng/tip-%1.png"), seatid));
                quanSprite.setAnchorPoint(cc.p(0.5, 0));
                quanSprite.setPosition(cc.p(headSize.width / 2, -headSize.height / 2));
                layer.addChild(quanSprite);
            }

            return layer;
        }
        ,

//输赢分
        createScoreLayer: function (seatid) {
            var layer = null;
            layer = new cc.Layer();

            var score = 0;
            if (this.ackGameEnd.Score.length > seatid) score = this.ackGameEnd.Score[seatid];
            var width = 0;
            var totallabelpath = getResPath("RoomMJ/result-small/result_total.png");
            var scorelabelpath = getResPath("RoomMJ/result-small/result_small_label_lose.png");
            if (score > 0) {
                scorelabelpath = getResPath("RoomMJ/result-small/result_small_label_win.png");
            }
            var totalLabelSprite = new cc.Sprite(totallabelpath);
            totalLabelSprite.setAnchorPoint(cc.p(0, 0.5));
            totalLabelSprite.setPosition(cc.p(0, 15));
            layer.addChild(totalLabelSprite);
            width += totalLabelSprite.getContentSize().width + 10;

            var scoreLabel = new cc.LabelAtlas(":" + Math.abs(score), scorelabelpath, 20, 24, '0');
            scoreLabel.setAnchorPoint(cc.p(0.0, 0.5));
            scoreLabel.setPosition(cc.p(width, 15));
            layer.addChild(scoreLabel);
            width += scoreLabel.getContentSize().width;
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_YNMJ_DZ) {
                var scGameEnd = this.ackGameEnd.scEndInfo;
                if (scGameEnd.bIsWinScoreFengding.length > seatid) {
                    if (scGameEnd.bIsWinScoreFengding[seatid] != 0) {
                        //金额封顶
                        var str = getResPath("RoomMJ/result-small/fengding.png");
                        var fengdingSpr = new cc.Sprite(str);
                        fengdingSpr.setAnchorPoint(cc.p(0, 0.5));
                        fengdingSpr.setPosition(cc.p(width + 10, 15));
                        layer.addChild(fengdingSpr);
                    }
                }
                if (scGameEnd.bHuFanFengding.length > seatid) {
                    if (scGameEnd.bHuFanFengding[seatid] != 0) {
                        //最大番数封顶
                        var str = getResPath("RoomMJ/result-small/fengding2.png");
                        var fengdingSpr = new cc.Sprite(str);
                        fengdingSpr.setAnchorPoint(cc.p(0, 0.5));
                        fengdingSpr.setPosition(cc.p(width + 10, 15));
                        layer.addChild(fengdingSpr);
                    }
                }

            }
            return layer;
        },

//杠胡列表
        createGangHuLayer: function (seatid) {
            var layer = new cc.Layer();
            var x = -7;
            var y = 0;
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_YNMJ_DZ) {
                var scGameEnd = this.ackGameEnd.scEndInfo;
                if (scGameEnd.GangInfo.length > seatid) {

                    var endGangHu = scGameEnd.GangInfo[seatid];

                    //胡牌顺序
                    if (MJModel.scmjType == 1 && scGameEnd.UserHuRank.length > 0) {
                        if (scGameEnd.UserHuRank[seatid]) {
                            var str = "";
                            switch (scGameEnd.UserHuRank[seatid]) {
                                case 1:
                                    str = "胡牌顺序①";
                                    break;
                                case 2:
                                    str = "胡牌顺序②";
                                    break;
                                case 3:
                                    str = "胡牌顺序③";
                                    break;
                                default:
                                    break;
                            }
                            var hupaiSX = new cc.LabelTTF(str, "Arial", 22);
                            hupaiSX.setColor(this.rightColor);
                            hupaiSX.setAnchorPoint(cc.p(0, 0));
                            hupaiSX.setPosition(cc.p(x, y + 8));
                            layer.addChild(hupaiSX);
                            x += hupaiSX.getContentSize().width + 5;
                        }
                    }

                    var map = {};
                    var map2 = {};
                    for (var i = 0; i < endGangHu.GangHu.length; i++) {
                        var ganghu = endGangHu.GangHu[i];
                        if (map[ganghu.Type] == undefined) {
                            map[ganghu.Type] = [];
                        }
                        if (map2[ganghu.Type] == undefined) {
                            map2[ganghu.Type] = [];
                        }
                        map[ganghu.Type].push(ganghu.Card);
                        map2[ganghu.Type].push(ganghu.score);
                    }

                    for (var key in map) {
                        if (!map.hasOwnProperty(key))continue;
                        var typeLabel = new cc.LabelTTF(this.getStringByType(key), "Arial", 20);
                        typeLabel.setColor(this.leftColor);
                        typeLabel.setAnchorPoint(cc.p(0, 0));
                        typeLabel.setPosition(cc.p(x, y + 10));
                        layer.addChild(typeLabel);

                        x += typeLabel.getContentSize().width;
                        for (var j = 0; j < map[key].length; j++) {
                            var smallCard = this.createSmallCard(map[key][j]);
                            smallCard.setAnchorPoint(cc.p(0.0, 0));
                            smallCard.setPosition(cc.p(x, y));
                            layer.addChild(smallCard);
                            if (MJModel.curGameType != ROOM_TYPE.ROOM_TYPE_YNMJ_DZ) {
                                x += smallCard.getContentSize().width;
                                var scoreTxt = map2[key][j];
                                cc.log("--------socreTxt---" + scoreTxt);
                                var str = "";
                                if (scoreTxt > 0) {
                                    str = "+" + scoreTxt;
                                } else {
                                    str = "" + scoreTxt;
                                }
                                if (str != "") {
                                    var score = new cc.LabelTTF(str, "Arial", 20);
                                    score.setColor(this.leftColor);
                                    score.setAnchorPoint(cc.p(0, 0));
                                    score.setPosition(cc.p(x, y + 10));
                                    layer.addChild(score);
                                }
                            }

                            x += 28;
                            if (j == map[key].length - 1) {
                                x += 20;
                            }
                        }
                    }
                    var tempmap = {};
                    if (scGameEnd.Returngangscore.length > seatid) {
                        var returnGangInfo = scGameEnd.Returngangscore[seatid];
                        for (var i = 0; i < returnGangInfo.State.length; i++) {
                            if (returnGangInfo.State[i] != 0) {
                                if (tempmap[returnGangInfo.State[i]] == undefined) {
                                    tempmap[returnGangInfo.State[i]] = [];
                                }
                                tempmap[returnGangInfo.State[i]].push(returnGangInfo.Card[i]);
                            }
                        }
                        for (var key in tempmap) {
                            if (!tempmap.hasOwnProperty(key))continue;
                            var str = "";
                            var state = key;
                            if (state == 1) {
                                str = "退杠";
                            } else {
                                str = "被退杠";
                            }
                            var tuigangLabel = new cc.LabelTTF(str, "Arial", 20);
                            tuigangLabel.setColor(this.leftColor);
                            tuigangLabel.setAnchorPoint(cc.p(0, 0));
                            tuigangLabel.setPosition(cc.p(x, y + 10));
                            layer.addChild(tuigangLabel);

                            x += tuigangLabel.getContentSize().width;

                            var temp = tempmap[key];
                            for (var j = 0; j < temp.length; j++) {
                                var smallCard = this.createSmallCard(temp[j]);
                                smallCard.setAnchorPoint(cc.p(0.0, 0));
                                smallCard.setPosition(cc.p(x, y));
                                layer.addChild(smallCard);
                                x += 28;
                                if (j == temp.length - 1) {
                                    x += 20;
                                }
                            }
                        }
                    }
                    if (scGameEnd.piaoNum.length > 0) {
                        if (scGameEnd.piaoNum[seatid] > 0) {
                            var piaoLabel = new cc.LabelTTF(__String.createWithFormat("飘分数 %1", scGameEnd.piaoNum[seatid]), "Arial", 20);
                            piaoLabel.setColor(this.leftColor);
                            piaoLabel.setAnchorPoint(cc.p(0, 0));
                            piaoLabel.setPosition(cc.p(x, y + 10));
                            layer.addChild(piaoLabel);

                            x += piaoLabel.getContentSize().width + 20;
                        }
                    }
                    if (scGameEnd.ListenDouble.length > 0) {
                        if (scGameEnd.ListenDouble[seatid] > 0) {
                            var tingpaiLabel = new cc.LabelTTF(__String.createWithFormat("听牌倍数 x%1", scGameEnd.ListenDouble[seatid]), "Arial", 20);
                            tingpaiLabel.setColor(this.leftColor);
                            tingpaiLabel.setAnchorPoint(cc.p(0, 0));
                            tingpaiLabel.setPosition(cc.p(x, y + 10));
                            layer.addChild(tingpaiLabel);

                            x += tingpaiLabel.getContentSize().width + 20;
                        }
                    }
                }
            }
            else if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
                if (this.ackGameEnd.EndInfo.length > seatid) {
                    var gameEndInfo = this.ackGameEnd.EndInfo[seatid];
                    var map = {};
                    for (var i = 0; i < gameEndInfo.Gang.length; i++) {
                        if (gameEndInfo.Gang[i].GangType && gameEndInfo.Gang[i].GangCard) {
                            var type = gameEndInfo.Gang[i].GangType;
                            if (gameEndInfo.Gang[i].GZGang) {
                                if (gameEndInfo.Gang[i].GZGang.IsHanBaoDou) {
                                    type = 10;//憨包豆
                                }
                            }
                            var card = gameEndInfo.Gang[i].GangCard;
                            if (map[type] == undefined) {
                                map[type] = [];
                            }
                            map[type].push(card);
                        }

                    }
                    for (var key in map) {
                        if (!map.hasOwnProperty(key))continue;
                        var a = key;
                        var typeLabel = new cc.LabelTTF(this.getStringGuiZhouByType(a), "Arial", 20);
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

                    if (seatid < this.ackGameEnd.EndInfo.length) {

                        //责任鸡
                        if (this.ackGameEnd.GuiZhouEndInfo) {
                            if (this.ackGameEnd.GuiZhouEndInfo.ZeRenJiID && this.ackGameEnd.GuiZhouEndInfo.ZeRenJiID != ActionType.INVALID_CHAIR) {
                                if (this.ackGameEnd.GuiZhouEndInfo.ChongFengJiID && seatid == this.ackGameEnd.GuiZhouEndInfo.ChongFengJiID) {
                                    var dachu_flag = new cc.LabelTTF(this.getStringGuiZhouByType(11), "Arial", 20);
                                    dachu_flag.setColor(this.leftColor);
                                    dachu_flag.setAnchorPoint(cc.p(0, 0));
                                    dachu_flag.setPosition(cc.p(x, y + 10));
                                    layer.addChild(dachu_flag);
                                    x += dachu_flag.getContentSize().width + 5;
                                    var xiaoji_flag = new cc.Sprite(getResPath("RoomMJ/GZMJ/gz_xiaoji_type_2.png"));
                                    xiaoji_flag.setAnchorPoint(cc.p(0.0, 0.0));
                                    xiaoji_flag.setPosition(x, y);
                                    layer.addChild(xiaoji_flag);
                                }
                            }

                        }
                    }
                }
            }
            else if (this.ackGameEnd.EndInfo.length > seatid) {
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
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GDMJ_DZ) {
                var gdmjGameEndInfo = this.ackGameEnd.gdGameEndInfo;
                if (gdmjGameEndInfo.HuaPai.length > seatid) {
                    var huaPaiInfo = gdmjGameEndInfo.HuaPai[seatid];

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
            }
            return layer;
        }
        ,

        createMaLayer: function (seatid) {
            var layer = null;
            if (this.ackGameEnd.UserHorse.length > seatid) {
                var userHorseCard = this.ackGameEnd.UserHorse[seatid];
                var maNum = userHorseCard.Num;
                if (maNum > 0) {
                    layer = new cc.Layer();

                    var maPic = new cc.Sprite(getResPath("RoomMJ/result-small/result_small_zhongma.png"));
                    maPic.setName("zhongmaTip");
                    maPic.setAnchorPoint(cc.p(0.0, 1));
                    maPic.setPosition(cc.p(0, 0));
                    layer.addChild(maPic);

                    for (var i = 0; i < maNum; i++) {
                        var card = new MJCard();
                        card.setValue(userHorseCard.HorseCard[i], 0, CardType.Card_End, 0);
                        card.setAnchorPoint(cc.p(0.0, 1));
                        card.setScale(0.5);
                        card.setPosition(cc.p(maPic.getContentSize().width + parseInt((i % 6)) * card.getBoundingBox().width + 10, -parseInt((i / 6)) * card.getBoundingBox().height));
                        layer.addChild(card);

                    }
                }
            }

            return layer;
        }
        ,

        createHuLayer: function (seatid) {
            var layer = null;
            var scGameEnd = this.ackGameEnd.scEndInfo;
            if (scGameEnd.stUserHu.length > seatid) {
                var userHuInfo = scGameEnd.stUserHu[seatid];
                var huNum = userHuInfo.stHuinfo.length;
                if (huNum > 0) {
                    layer = new cc.Layer();
                    var hupaiPic = new cc.Sprite(getResPath("RoomMJ/result-detail/result_detail_pic_hupai.png"));
                    hupaiPic.setAnchorPoint(cc.p(0.0, 1));
                    hupaiPic.setPosition(cc.p(0, 0));
                    layer.addChild(hupaiPic);

                    for (var i = 0; i < huNum; i++) {
                        var tagHu = userHuInfo.stHuinfo[i];
                        var card = new MJCard();
                        card.setValue(tagHu.Hucard, 0, CardType.Card_End, 0);
                        card.setAnchorPoint(cc.p(0.0, 1));
                        card.setScale(0.5);
                        var height = 0;
                        if (parseInt(i / 6) == 0) {
                            height = -(parseInt(i / 6)) * card.getBoundingBox().height;
                        } else {
                            height = -(parseInt(i / 6)) * card.getBoundingBox().height + 10;
                        }
                        Log(this, "oooooooooooooooooooop-----height:", height);
                        card.setPosition(cc.p(hupaiPic.getContentSize().width + (i % 6) * card.getBoundingBox().width + 10, height));
                        layer.addChild(card, i);

                        var cardSize = cc.size(card.getBoundingBox().width * 2, card.getBoundingBox().height * 2);
                        //哪家
                        var pos = MJModel.getPosBySeatid(seatid);
                        var hupos = MJModel.getPosBySeatid(tagHu.pasHuchairid) - pos;
                        if (hupos < 0) {
                            hupos += 4;
                        }
                        var posSprite = new cc.Sprite(__String.createWithFormat(getResPath("RoomMJ/result-small/hu_pos_%1.png"), hupos));
                        posSprite.setAnchorPoint(cc.p(0.5, 0.0));
                        posSprite.setScale(1.5);
                        posSprite.setPosition(cc.p(cardSize.width / 2, cardSize.height / 2 + 15));
                        card.addChild(posSprite);
                    }
                }
            }
            return layer;
        }
        ,

        continueClick: function (ref) {
            if (MJModel.isOnVideo)return;
            sendReady();

            this.removeFromParent();
            MJModel.state = MJ_STATUS.MJ_GAME_FREE;
        }
        ,

        getStringByType: function (stype) {
            var type = Number(stype);
            var str = "";
            switch (type) {
                case HUGANG.HUGANG_ZHIGANG:
                    str = "直杠";
                    if ((MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ_DZ) && (MJModel.scmjType == 1 )) str = "弯杠";
                    ;
                    break;
                case HUGANG.HUGANG_WANGANG:
                    str = "弯杠";
                    if ((MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ_DZ) && (MJModel.scmjType == 1 )) str = "巴杠";
                    ;
                    break;
                case HUGANG.HUGANG_ANGANG:
                    str = "暗杠";
                    if ((MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ_DZ) && (MJModel.scmjType == 1)) str = "直杠";
                    ;
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
        }
        ,

        getStringGuiZhouByType: function (stype) {
            var type = Number(stype);
            var str = "";
            switch (type) {
                case 0:
                    str = "输豆";
                    break;
                case 1:
                    str = "爬坡豆";
                    break;
                case 2:
                    str = "点豆";
                    break;
                case 3:
                    str = "闷豆";
                    break;
                case 4:
                    str = "爬坡豆";
                    break;
                case 8:
                    str = "闷豆";
                    break;
                case 10:
                    str = "憨包豆";
                    break;
                case 11:
                    str = "打出";
                    break;
                case 64:
                    str = "输豆";
                    break;
                default:
                    break;
            }
            return str;
        }
    })
    ;

