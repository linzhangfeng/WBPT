var findCardByValue = function (value, cardList) {
    if (cardList == null) return null;
    for (var i = cardList.length - 1; i >= 0; i--) {
        var _card = cardList[i];
        if (_card.value == value) {
            return _card;
        }
    }
    return null;
};

var removeCardByValue = function (value, cardList) {
    if (cardList == null) return null;
    var card = null;
    var index = -1;

    for (var i = cardList.length - 1; i >= 0; i--) {
        var _card = cardList[i];
        cc.log("removeCardByValue :" + "_card:" + _card.value + ";value:" + value);
        if (_card.value == value) {
            card = _card;
            cardList.splice(i, 1);
            break;
        }
    }

    return card;
};

var removeCardByCard = function (card, cardList) {
    if (cardList == null) return null;
    for (var i in cardList) {
        var _card = cardList[i];
        if (_card == card) {
            cardList.splice(i, 1);
            return _card;
        }
    }
    return null;
};

var MJCards = cc.Layer.extend({
    tag: "MJCards",
    hand_card: [[], [], [], []],
    out_card: [[], [], [], []],
    peng_card: [[], [], [], []],
    hu_card: [[], [], [], []],
    niao_card: [[], [], [], []],
    hua_card: [[], [], [], []],
    curCardTip: null,
    outCardBtn: null,
    curTouchCard: null,
    ctor: function () {
        this._super();
        var size = cc.director.getWinSize();
        this.ignoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.5, 0.5));
        this.setContentSize(size);
    },
    onEnter: function () {
        this._super();
        // 部分事件监听
        var self = this;
        var listener = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches: true,
            onTouchBegan: function (touch, event) {
                return self.onTouchBegan(touch, event);
            },
            onTouchMoved: function (touch, event) {
                return self.onTouchMoved(touch, event);
            },
            onTouchEnded: function (touch, event) {
                self.onTouchEnded(touch, event);
            }
        });
        if (listener) cc.eventManager.addListener(listener, this);
    },

    onTouchBegan: function (touch, event) {
        if (MJModel.seatid == -1) return false;
        if (!touch.getLocation) {
            return false;
        }

        if (MJModel.canTouchCard == false) {
            return false;
        }

        try {
            for (var i = 0; i < this.hand_card[0].length; i++) {
                var card = this.hand_card[0][i];
                if (card.onTouchBegan(touch, event)) {
                    this.curTouchCard = card;
                    return true;
                }
            }
        } catch (e) {

        }
        return false;
    },

    onTouchMoved: function (touch, event) {
        try {
            if (this.curTouchCard) this.curTouchCard.onTouchMoved(touch, event);
        } catch (e) {

        }
    },

    onTouchEnded: function (touch, event) {
        try {
            if (this.curTouchCard) this.curTouchCard.onTouchEnded(touch, event);
            this.curTouchCard = null;
        } catch (e) {

        }
    },
    fapai: function (cardList) {
        this.initHandCard(0, cardList);
        this.reflashHandCard(0, false);
        var dl = 0;
        for (var i = 0; i < this.hand_card[0].length; i++) {
            var card = this.hand_card[0][i];
            card.setVisible(false);
            card.runAction(cc.sequence(cc.delayTime(dl), cc.show()));
            dl += 0.1;
        }
        dl += 0.1;

        var self = this;
        var callback = cc.callFunc(function () {
            for (var i = 0; i < self.hand_card[0].length; i++) {
                var card = self.hand_card[0][i];
                card.setVisible(false);
                card.runAction(cc.sequence(cc.delayTime(1), cc.show()));

                var temp_card = new MJCard();
                temp_card.setValue(0, 0, CardType.Card_Mo, 0);
                temp_card.setPosition(card.getPosition());
                self.addChild(temp_card, MJConfig.getHandCardOrder(0, i));
                temp_card.runAction(cc.sequence(cc.delayTime(1), cc.removeSelf()));
            }

        });

        var callback2 = cc.callFunc(function () {
            MJModel.isEnterWait = false;
            for (var i = 0; i < self.hand_card[0].length; i++) {
                var card = self.hand_card[0][i];
                card.setVisible(true);
            }

            self.reflashHandCard(0, true);
        });
        this.runAction(cc.sequence(cc.delayTime(dl), callback, cc.delayTime(1), callback2));
    },

    initHandCard: function (pos, cardList) {

        for (var i = 0; i < this.hand_card[pos].length; i++) {
            this.hand_card[pos][i].removeFromParent(true);
        }
        var self = this;
        this.hand_card[pos] = [];
        for (var i in cardList) {
            var card = new MJCard();
            card.setValue(cardList[i], pos, MJModel.isOnVideo ? CardType.Card_End : CardType.Card_Hand, i);
            this.hand_card[pos].push(card);
            this.addChild(card, MJConfig.getHandCardOrder(pos, i));

            if (pos == 0 && MJModel.seatid != -1) {
                card.setTouchEnable(true);
                card.setUpCallback(function (card) {
                    self.clickHandCardCallback(card);
                });
                card.setOutCallback(function (card) {
                    self.outHandCardCallback(card);
                });
            }
        }
    },

    addHandCard: function (pos, value) {
        var card = new MJCard();

        this.hand_card[pos].push(card);
        card.setValue(value, pos, MJModel.isOnVideo ? CardType.Card_End : CardType.Card_Hand, this.hand_card[pos].length - 1);
        this.addChild(card, MJConfig.getHandCardOrder(pos, this.hand_card[pos].length - 1));
        if (pos == 0 && MJModel.seatid != -1) {
            card.setTouchEnable(true);
            var self = this;
            card.setUpCallback(function (card) {
                self.clickHandCardCallback(card);
            });
            card.setOutCallback(function (card) {
                self.outHandCardCallback(card);
            });

            if (MJModel.isAutoOutCard) card.setBlackVis(true);
        }
    },

    sortCard: function (a, b) {
        if (MJModel.valueGuiPai) {
            //鬼牌最前
            var value = MJModel.valueGuiPai;
            if (a.value == value && b.value != value)return -1;//鬼牌放最前边
            if (a.value != value && b.value == value)return 1;
        }

        //花猪放在最右边
        var lackType = MJModel.lackType;
        if (lackType != -1) {
            var paiSea = parseInt(a.value / 16);
            var paiSeb = parseInt(b.value / 16);
            if (paiSea == lackType && paiSeb != lackType)return 1;
            if (paiSea != lackType && paiSeb == lackType)return -1;
        }

        if (parseInt(a.value / 16) == 5 && parseInt(b.value / 16) != 5)return -1;//花牌放最前边
        if (parseInt(a.value / 16) != 5 && parseInt(b.value / 16) == 5)return 1;
        return a.value - b.value;
    },
    setCurCardTip: function (v) {
        if (this.curCardTip) this.curCardTip.setVisible(v);
    },
    reflashHandCard: function (pos, isSort) {
        if (isSort == undefined) isSort = true;
        if (isSort) this.hand_card[pos].sort(this.sortCard);

        for (var i = 0; i < this.hand_card[pos].length; i++) {
            var card = this.hand_card[pos][i];
            card.reset();
            if (MJModel.getTangPaiFlag(pos) && !MJModel.isMyPlayer(MJModel.getSeatidByPos(pos))) {
                card.setMyPosition(MJConfig.getHandCardShowPos(pos, i, this.hand_card[pos].length - 1));
            } else {
                card.setMyPosition(MJConfig.getHandCardPos(pos, i, this.hand_card[pos].length - 1));
            }
            this.reorderChild(card, MJConfig.getHandCardOrder(pos, i));
        }
    },

    getGoodPaiSe: function () {
        var wanCount = 0;
        var tiaoCount = 0;
        var tongCount = 0;
        for (var i = 0; i < this.hand_card[0].length; i++) {
            var card = this.hand_card[0][i];
            var paiSe = parseInt(card.value / 16);
            switch (paiSe) {
                case MJ_TYPE.MJ_TYPE_WAN:
                    wanCount++;
                    break;
                case MJ_TYPE.MJ_TYPE_TIAO:
                    tiaoCount++;
                    break;
                case MJ_TYPE.MJ_TYPE_TONG:
                    tongCount++;
                    break;
                default:
                    break;
            }

        }
        cc.log("wanCount:" + wanCount + "tiaoCount:" + tiaoCount + "tongCount:" + tongCount);
        if (wanCount <= tiaoCount && wanCount <= tongCount) {
            return MJ_TYPE.MJ_TYPE_WAN;
        }
        else if (tiaoCount <= wanCount && tiaoCount <= tongCount) {
            return MJ_TYPE.MJ_TYPE_TIAO;
        }
        else {
            return MJ_TYPE.MJ_TYPE_TONG;
        }
    },

    showEndCard: function (pos, cardList, hucard, isZiMo) {

        for (var i = 0; i < this.hand_card[pos].length; i++) {
            this.hand_card[pos][i].removeFromParent(true);
        }
        this.hand_card[pos] = [];
        var is_remove = 0;
        var real_index = 0;
        var _vec;
        for (var i = 0; i < cardList.length; i++) {
            if (is_remove == 0 && isZiMo) {
                if (cardList[i] == hucard) {
                    is_remove = 1;
                    continue;
                }
            }
            var card = new MJCard();
            card.setValue(cardList[i], pos, CardType.Card_End, real_index);
            _vec = MJConfig.getHandCardShowPos(pos, real_index, cardList.length - 1);
            card.setPosition(_vec);
            this.addChild(card, MJConfig.getHandCardOrder(pos, real_index));
            real_index++;
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
                this.hand_card[pos].push(card);
            }

        }

        if (hucard != 0 && hucard != null && hucard != undefined) {
            var addv = cc.p(0, 0);
            if (pos == 0) addv = cc.p(74, 0);
            if (pos == 1) addv = cc.p(0, 50);
            if (pos == 2) addv = cc.p(-46, 0);
            if (pos == 3) addv = cc.p(0, -50);
            var card = new MJCard();
            card.setValue(hucard, pos, CardType.Card_End, 0);
            card.setPosition(cc.p(_vec.x + addv.x, _vec.y + addv.y));
            this.addChild(card, 0);
        }
    },

    tangHandCard: function (pos, cardList) {
        for (var i = 0; i < this.hand_card[pos].length; i++) {
            this.hand_card[pos][i].removeFromParent(true);
        }
        this.hand_card[pos] = [];
        for (var i = 0; i < cardList.length; i++) {
            var card = new MJCard();

            card.setValue(cardList[i], pos, CardType.Card_End, i);
            card.setPosition(MJConfig.getHandCardShowPos(pos, i, cardList.length - 1));
            this.hand_card[pos].push(card);
            this.addChild(card, MJConfig.getHandCardOrder(pos, i));
        }
    },

    initMoCard: function (pos, value, isDiretor) {
        var card = new MJCard();
        this.hand_card[pos].push(card);
        card.setValue(value, pos, MJModel.isOnVideo ? CardType.Card_End : CardType.Card_Hand, this.hand_card[pos].length - 1);

        card.setMyPosition(MJConfig.getMoCardPos(pos));
        if (pos == 0 && MJModel.seatid != -1) {
            card.setTouchEnable(true);
            var self = this;
            card.setUpCallback(function (card) {
                self.clickHandCardCallback(card);
            });
            card.setOutCallback(function (card) {
                self.outHandCardCallback(card);
            });
            if (MJModel.isAutoOutCard) card.setBlackVis(true);
            this.setGuipai();
        }

        this.addChild(card, MJConfig.getHandCardOrder(pos, this.hand_card[pos].length - 1));
    },

    outHandCard: function (pos, value) {
        var size = cc.director.getWinSize();
        if (pos == 0 && MJModel.seatid != -1) {
            MJModel.hasCardOutIng = false;
            cc.director.getRunningScene().removeChildByName("tingpai_layer");
            if (this.outCardBtn) this.outCardBtn.setVisible(false);

            var card = null;
            card = findCardByValue(value, this.hand_card[pos]);

            if (card) {
                card = removeCardByCard(card, this.hand_card[pos]);

                if (card) {
                    card.reset();

                    var self = this;
                    var move_callbck = cc.callFunc(function () {
                        card.removeFromParent(true);
                        self.reflashHandCard(pos, true);
                        self.addOutCard(pos, value);
                        MJModel.isEnterWait = false;
                    });
                    card.runAction(cc.sequence(cc.moveTo(0.2, MJConfig.getOutCardPos(pos, this.out_card[pos].length)), move_callbck));
                }
                else {
                    self.addOutCard(pos, value);
                    MJModel.isEnterWait = false;
                }
            }
            else {
                MJModel.isEnterWait = false;
            }
        }
        else {
            var card = null;
            var isHaveZiero = true;
            if (MJModel.getTangPaiFlag(pos)) {
                Log(this, "error getTangPaiFlag");
                isHaveZiero = false;
                for (var i = 0; i < this.hand_card[pos].length; i++) {
                    if (this.hand_card[pos][i].value == 0) {
                        isHaveZiero = true;
                    }
                }
                cc.log("getTangPaiFlag:" + isHaveZiero);
            }

            card = removeCardByValue(MJModel.isOnVideo || !isHaveZiero ? value : 0, this.hand_card[pos]);
            if (card) {
                card.removeFromParent(true);
                this.reflashHandCard(pos, true);
                this.outHandCardEffct(pos, value);
            }
            else {
                Log(this, "error no find card");
                this.outHandCardEffct(pos, value);
                MJModel.isEnterWait = false;
            }
        }
    },

    outHandCardEffct: function (pos, value) {
        var size = cc.director.getWinSize();
        var card = new MJCard();
        card.setValue(value, pos, CardType.Card_Outing, 0);
        card.setMyPosition(MJConfig.getOutingCardPos(pos, 0));
        this.addChild(card, MJConfig.getOutingCardOrder(pos, 0));

        var self = this;
        var move_callbck = cc.callFunc(function () {
            card.removeFromParent(true);
            self.addOutCard(pos, value);
            MJModel.isEnterWait = false;
        });

        if (pos == 2) {
            card.runAction(cc.sequence(cc.delayTime(0.3), cc.moveTo(0.0, MJConfig.getOutCardPos(pos, this.out_card[pos].length)), move_callbck));
        }
        else {
            card.runAction(cc.sequence(cc.moveTo(0.3, MJConfig.getOutCardPos(pos, this.out_card[pos].length)), move_callbck));
        }
    },

    getHuaCard: function (pos, value) {
        var size = cc.director.getWinSize();
        var card = new MJCard();
        if (MJModel.isOnVideo) {
            card.setValue(value, pos, MJModel.isOnVideo ? CardType.Card_End : CardType.Card_Hand, this.hand_card[pos].length - 1);
        }
        else {
            card.setValue(MJModel.seatid != -1 && pos == 0 ? value : 0, pos, MJModel.isOnVideo ? CardType.Card_End : CardType.Card_Hand, this.hand_card[pos].length - 1);
        }
        card.setMyPosition(MJConfig.getMoCardPos(pos));
        this.addChild(card, MJConfig.getHandCardOrder(pos, this.hand_card[pos].length - 1));

        var self = this;
        var move_callbck = cc.callFunc(function () {
            card.removeFromParent(true);
            self.outHuaCardEffct(pos, value);
        });

        if (MJModel.seatid != -1 && pos == 0) {
            card.runAction(cc.sequence(cc.delayTime(0.5), cc.moveTo(0.1, MJConfig.getOutingCardPos(pos, 0)), move_callbck));
        }
        else {
            card.runAction(cc.sequence(cc.delayTime(0.5), move_callbck));
        }
    },

    outHuaCardEffct: function (pos, value) {
        var card1 = new MJCard();
        card1.setValue(value, pos, CardType.Card_Outing, 0);
        card1.setMyPosition(MJConfig.getOutingCardPos(pos, 0));
        this.addChild(card1, MJConfig.getOutingCardOrder(pos, 0));

        //outing effect
        var self = this;
        var move_callbck = cc.callFunc(function () {
            card1.removeFromParent(true);
            self.addHuaCard(pos, value);
            MJModel.isEnterWait = false;
        });
        card1.runAction(cc.sequence(cc.delayTime(1), cc.moveTo(0.3, MJConfig.getHuaCardPos(pos, this.hua_card[pos].length)), move_callbck));
    },

    removeHandCard: function (pos, value, nums) {
        if (nums == undefined) nums = 1;
        if (!(MJModel.isOnVideo) && !(MJModel.getTangPaiFlag(pos))) {
            if (MJModel.seatid == -1) {
                value = 0;
            }
            else if (pos != 0) {
                value = 0;
            }
        }
        Log(this, "removeHandCardBegin:pos=" + pos + "nums=" + nums + "curLen=" + this.hand_card[pos].length);
        var card = null;
        var isOk = false;
        for (var i = 0; i < nums; i++) {
            if (MJModel.getTangPaiFlag(pos)) {
                var zcard = removeCardByValue(0, this.hand_card[pos]);
                if (zcard) {
                    zcard.removeFromParent(true);
                    isOk = true;
                    continue;
                }
                Log(this, "removeHandCard error");
            }
            card = removeCardByValue(value, this.hand_card[pos]);
            if (card) {
                card.removeFromParent(true);
                isOk = true;
            }
        }
        Log(this, "removeHandCardEnd:pos=" + pos + "nums=" + nums + "curLen=" + this.hand_card[pos].length);
        this.reflashHandCard(pos, true);
        return isOk;
    },

    getChoiceCards: function () {
        var cards = [];

        for (var i = 0; i < this.hand_card[0].length; i++) {
            var fcard = this.hand_card[0][i];
            if (fcard) {
                if (fcard.isUp) {
                    cards.push(fcard.value);
                }
            }
        }
        return cards;
    },

    isNeedOutCard: function (pos) {
        return this.hand_card[pos].length % 3 == 2;
    },

    sureLackType: function () {
        for (var i = 0; i < this.hand_card[0].length; i++) {
            var fcard = this.hand_card[0][i];
            fcard.showHuaZhu();
        }
    },

    setGuipai: function () {
        for (var i = 0; i < this.hand_card[0].length; i++) {
            var fcard = this.hand_card[0][i];
            fcard.showGuiPai();
        }
    },

    setUp: function (isUp) {
        for (var i = 0; i < this.hand_card[0].length; i++) {
            var fcard = this.hand_card[0][i];
            if (fcard.isUp != isUp) {
                fcard.setUp(isUp);
            }
        }
    },

    clickHandCardCallback: function (n) {
        if (MJModel.canTouchCard == false) {
            return;
        }
        var card = n;

        if (MJModel.canChoiceMore) {
            if (card.isUp) {
                card.setUp(false);
            }
            else {
                var upCards = this.getChoiceCards();
                if (upCards.length < 3) {
                    card.setUp(true);
                }
            }
            return;
        }

        var preCard = null;

        for (var i = 0; i < this.hand_card[0].length; i++) {
            var fcard = this.hand_card[0][i];
            if (fcard) {
                if (fcard.isUp) preCard = fcard;
                fcard.setUp(false);
            }
        }

        if (this.outCardBtn) this.outCardBtn.setVisible(false);

        if (preCard && preCard == card) {
            return;
        }

        card.setUp(true);

        cc.director.getRunningScene().removeChildByName("tingpai_layer");
        var hasTingPaiInfo = false;

        if (MJModel.listen_info.length > 0) {
            for (var i = 0; i < MJModel.listen_info.length; i++) {
                var tli = MJModel.listen_info[i];
                if (card.value == tli.OutCard) {
                    var curTingPaiLayer = new TingPaiLayer();
                    curTingPaiLayer.show2(tli.HuInfo);

                    curTingPaiLayer.setName("tingpai_layer");
                    cc.director.getRunningScene().addChild(curTingPaiLayer, 10000);
                    if (card.getPosition().x < curTingPaiLayer.getContentSize().width / 2){
                        curTingPaiLayer.setPosition(cc.p(0 + curTingPaiLayer.getContentSize().width / 2, card.getPosition().y + 70));
                    }else{
                        curTingPaiLayer.setPosition(cc.p(0 + card.getPosition().x, card.getPosition().y + 70));
                    }
                    hasTingPaiInfo = true;
                    break;
                }
            }
        }

        if (this.outCardBtn == null) {
            var self = this;
            this.outCardBtn = new ccui.Button(getResPath("RoomMJ/outcard01.png"), getResPath("RoomMJ/outcard02.png"));

            this.outCardBtn.setName("outCardBtn");
            this.outCardBtn.addClickEventListener(function (sender, evt) {
                for (var i = 0; i < self.hand_card[0].length; i++) {
                    var fcard = self.hand_card[0][i];
                    if (fcard) {
                        if (fcard.isUp) {
                            self.outHandCardCallback(fcard);
                            self.outCardBtn.setVisible(false);
                            break;
                        }
                    }
                }
            });
            this.addChild(this.outCardBtn, 100000);
        }
        else {
            this.outCardBtn.setVisible(true);
        }

        if (this.outCardBtn) {
            var size = cc.director.getWinSize();

            if (hasTingPaiInfo && card.getPositionX() > size.width - 318) {
                this.outCardBtn.setPosition(cc.p(size.width - 168, 340 + 50));
            }
            else {
                this.outCardBtn.setPosition(cc.p(size.width - 168, 189));
            }
        }
    },

    outHandCardCallback: function (n) {
        if (this.outCardBtn) this.outCardBtn.setVisible(false);

        MJModel.isOutCard = true;

        var card = n;

        if (MJModel.canTouchCard == true && MJModel.hasCardOutIng == false && MJModel.isMyPlayer(MJModel.cur_seat)) {
            sendOutCard(card.value);
            card.runAction(cc.sequence(cc.delayTime(0.3), cc.callFunc(function () {
                card.goBack();
            })));
            MJModel.hasCardOutIng = true;
        }
        else {
            card.goBack();
        }
    },

    ///////////////////////////出牌///////////////////////////
    initOutCard: function (pos, cardList) {
        for (var i = 0; i < cardList.length; i++) {
            var card = new MJCard();
            card.setValue(cardList[i], pos, CardType.Card_Out, i);
            this.out_card[pos].push(card);
            card.setMyPosition(MJConfig.getOutCardPos(pos, i));
            this.addChild(card, MJConfig.getOutCardOrder(pos, i));
        }
    },

    addOutCard: function (pos, value) {
        var card = new MJCard();

        this.out_card[pos].push(card);
        card.setValue(value, pos, CardType.Card_Out, this.out_card[pos].length - 1);
        card.setMyPosition(MJConfig.getOutCardPos(pos, this.out_card[pos].length - 1));
        this.addChild(card, MJConfig.getOutCardOrder(pos, this.out_card[pos].length - 1));
        Log(this, this.curCardTip);
        if (this.curCardTip == null) {

            this.curCardTip = new cc.Sprite(getResPath("RoomMJ/cur_card.png"));
            this.curCardTip.runAction(cc.sequence(cc.moveBy(0.5, cc.p(0, 10)), cc.moveBy(0.5, cc.p(0, -10))).repeatForever());
            this.curCardTip.setVisible(false);
            this.addChild(this.curCardTip, 10000);
            Log(this, this.curCardTip);
        }

        if (this.curCardTip != null) {
            Log(this, this.curCardTip);
            this.curCardTip.setPosition(cc.p(card.getPosition().x, card.getPosition().y + 30));
            this.curCardTip.setVisible(true);
        }
    },

    initCurOutCardTip: function (pos, value) {
        var card = findCardByValue(value, this.out_card[pos]);
        if (card) {
            if (this.curCardTip == null) {
                this.curCardTip = new cc.Sprite(getResPath("RoomMJ/cur_card.png"));
                this.curCardTip.runAction(cc.sequence(cc.moveBy(0.5, cc.p(0, 10)), cc.moveBy(0.5, cc.p(0, -10))).repeatForever());
                this.curCardTip.setVisible(false);
                this.addChild(this.curCardTip, 10000);
            }

            if (this.curCardTip) {
                this.curCardTip.setPosition(cc.p(card.getPosition().x, card.getPosition().y + 30));
                this.curCardTip.setVisible(true);
            }
        }
    },

    removeOutCard: function (pos, value) {
        var card = removeCardByValue(value, this.out_card[pos]);
        if (card) card.removeFromParent(true);
    },

    ///////////////////////////胡牌///////////////////////////
    initHuCard: function (pos, cardList) {
        this.hu_card[pos] = [];
        if (cardList.length > 0) {
            for (var i = 0; i < cardList.length; i++) {
                this.addHuCard(pos, cardList[i]);
            }
        }
    },


    addHuCard: function (pos, value) {
        var card = new MJCard();

        this.hu_card[pos].push(card);
        card.setValue(value, pos, CardType.Card_Hu, this.hu_card[pos].length - 1);
        card.setMyPosition(MJConfig.getHuCardPos(pos, this.hu_card[pos].length - 1));
        this.addChild(card, MJConfig.getHuCardOrder(pos, this.hu_card[pos].length - 1));
    },

    initHuaCard: function (pos, cardList) {
        this.hua_card[pos] = [];
        if (cardList.length > 0) {
            for (var i = 0; i < cardList.length; i++) {
                cc.log("1pos:" + pos);
                this.addHuaCard(pos, cardList[i]);
            }
        }
    },

    isHaveHuaCard: function (pos) {
        return this.hua_card[pos].length > 0;
    },

    addHuaCard: function (pos, value) {
        var card = new MJCard();
        cc.log("2pos:" + pos);
        this.hua_card[pos].push(card);
        card.setValue(value, pos, CardType.Card_Hu, this.hu_card[pos].length - 1);
        card.setMyPosition(MJConfig.getHuaCardPos(pos, this.hua_card[pos].length - 1));
        this.addChild(card);
    },

    isAlreadyHu: function (pos) {
        return this.hu_card[pos].length > 0;
    },

    setAutoOutCard: function (v) {
        if (MJModel.seatid == -1)return;
        for (var i = 0; i < this.hand_card[0].length; i++) {
            this.hand_card[0][i].setBlackVis(v);
        }
    },

    ///////////////////////////碰扛牌///////////////////////////
    addPengCard: function (pos, type, value) {
        var card = new MJPeng();

        this.peng_card[pos].push(card);
        card.setValue(type, pos, value, this.peng_card[pos].length - 1);
        this.addChild(card, MJConfig.getPengKangCardOrder(pos, this.peng_card[pos].length - 1));
    },

    changPengCard: function (pos, type, value) {
        for (var i = 0; i < this.peng_card[pos].length; i++) {
            if (this.peng_card[pos][i].value == value) {
                this.peng_card[pos][i].setValue(type, pos, value, i);
                break;
            }
        }
    },

    setTingPaiFlag: function (v) {
        if (v) {
            for (var i = 0; i < MJModel.listen_info.length; i++) {
                var tli = MJModel.listen_info[i];
                for (var j = 0; j < this.hand_card[0].length; j++) {
                    var card = this.hand_card[0][j];
                    if (card.value == tli.OutCard) {
                        card.setTingPai(true);
                    }
                }
            }
        }
        else {
            for (var j = 0; j < this.hand_card[0].length; j++) {
                var card = this.hand_card[0][j];
                card.setTingPai(false);
            }
            cc.director.getRunningScene().removeChildByName("tingpai_layer");
        }
    },

    reset: function () {
        this.removeAllChildren();
        for (var i = 0; i < 4; i++) {
            this.hand_card[i] = [];
            this.out_card[i] = [];
            this.peng_card[i] = [];
            this.hu_card[i] = [];
            this.niao_card[i] = [];
            this.hua_card[i] = [];
        }
        this.curCardTip = null;
        this.outCardBtn = null;
    },

    showMaiMaEffect: function (nums, pos) {
        var size = cc.director.getWinSize();

        for (var i = 0; i < nums; i++) {
            var ma = new cc.Sprite(getResPath("RoomMJ/maima/maima_bg.png"));
            ma.setPosition(cc.p(size.width / 2 - 63 * (nums - 1) / 2 + i * 63, 254));
            this.addChild(ma, 100000);

            ma.runAction(cc.sequence(cc.delayTime(1), cc.moveTo(0.3, MJConfig.getPlayerPos(pos)), cc.removeSelf()));
        }
    },

    showZhongMaEffect: function (allcardlist, cardlist_pos) {
        var size = cc.director.getWinSize();

        var nums = allcardlist.length;
        var self = this;
        for (var i = 0; i < nums; i++) {

            (function (i) {
                var ma_black = new cc.Sprite(getResPath("RoomMJ/maima/maima_bg.png"));
                ma_black.setPosition(cc.p(size.width / 2 - 63 * (nums - 1) / 2 + i * 63, 254));
                self.addChild(ma_black, 10000);
                ma_black.runAction(cc.sequence(cc.delayTime(1), cc.removeSelf()));

                var to_pos = cardlist_pos[i];
                var ma = new MJCard();
                ma.setVisible(false);
                ma.setValue(allcardlist[i], 0, CardType.Card_Outing, 0);
                ma.setPosition(cc.p(size.width / 2 - 63 * (nums - 1) / 2 + i * 63, 254));
                self.addChild(ma, 100000);

                var callback = cc.callFunc(function () {
                    self.addZhongMaCard(to_pos, allcardlist[i]);
                });
                ma.runAction(cc.sequence(cc.delayTime(1), cc.show(), cc.delayTime(2), cc.moveTo(0.3, MJConfig.getPlayerPos(to_pos)), callback, cc.removeSelf()));
            })(i);
        }
    },

    initZhongMaCard: function (pos, cardList) {
        this.hu_card[pos] = [];
        if (cardList.length > 0) {
            for (var i = 0; i < cardList.length; i++) {
                this.addZhongMaCard(pos, cardList[i]);
            }
        }
    },

    addZhongMaCard: function (pos, value) {
        var card = new MJCard();

        this.hu_card[pos].push(card);
        card.setValue(value, pos, CardType.Card_Hu, this.hu_card[pos].length - 1);
        card.setMyPosition(this.isHaveHuaCard(pos) ? MJConfig.getZhongMaCardPos2(pos, this.hu_card[pos].length - 1) : MJConfig.getZhongMaCardPos1(pos, this.hu_card[pos].length - 1));
        this.addChild(card, MJConfig.getHuCardOrder(pos, this.hu_card[pos].length - 1));
    },

    addZhongNiaoCard: function (pos, value) {
        var card = new MJCard();
        card.setScale(0.87);
        this.niao_card[pos].push(card);
        card.setValue(value, pos, CardType.Card_Hu, this.niao_card[pos].length - 1);
        card.setMyPosition(MJConfig.getNiaoCardPos(pos, this.niao_card[pos].length - 1));
        this.addChild(card, MJConfig.getHuCardOrder(pos, this.niao_card[pos].length - 1));
    },

    showZhuaFengEffect: function (new_seat_list) {
        var size = cc.director.getWinSize();

        var nums = new_seat_list.length;

        for (var i = 0; i < nums; i++) {
            var cur_seat = i;
            var cur_pos = MJModel.getPosBySeatid(cur_seat);

            var new_seat = new_seat_list[i];

            var ma_black = new cc.Sprite(getResPath("RoomMJ/gdmj/zhuafeng/feng_bg.png"));
            ma_black.setPosition(cc.p(size.width / 2 - 63 * (nums - 1) / 2 + i * 63, 254));
            this.addChild(ma_black, 10000);
            ma_black.runAction(cc.sequence(cc.delayTime(1), cc.moveTo(0.3, MJConfig.getFengWeiPos(cur_pos)), cc.removeSelf()));

            var ma = new cc.Sprite(MJConfig.getCardBgStr(0, 0, CardType.Card_Outing, 0));
            ma.setVisible(false);
            ma.setPosition(MJConfig.getFengWeiPos(cur_pos));
            this.addChild(ma, 100000);

            var sp = new cc.Sprite(getResPath("RoomMJ/gdmj/zhuafeng/effect-") + new_seat + ".png");
            sp.setPosition(MJConfig.getCardValuePos(0, CardType.Card_Outing, ma.getContentSize()));
            ma.addChild(sp);

            ma.runAction(cc.sequence(cc.delayTime(1.3), cc.show(), cc.delayTime(3.0), cc.removeSelf()));
        }
    },

    showAllJiPaiFlag: function (pos, JiPaicardList, isManTangJi) {
        if (isManTangJi == undefined) isManTangJi = false;
        //每个人出的牌手牌是否有鸡牌
        for (var i = 0; i < this.hand_card[pos].length; i++) {
            var card = this.hand_card[pos][i];
            if (card) {
                for (var j = 0; j < JiPaicardList.length; j++) {
                    if (card.value == JiPaicardList[j]) {
                        var bg_sp = card.getChildByName("sprite");
                        if (bg_sp) {
                            var nums_sp = bg_sp.getChildByName("num_sp");
                            if (nums_sp) {
                                this.addJiPaiFlag(pos, i, CardType.Card_Hand, bg_sp, nums_sp);
                            }
                        }
                        break;
                    }
                }
            }
        }
        if (isManTangJi) {
            //每个人出的牌是否有鸡牌
            for (var i = 0; i < this.out_card[pos].length; i++) {
                var card = this.out_card[pos][i];
                if (card) {

                    for (var j = 0; j < JiPaicardList.length; j++) {
                        if (card.value == JiPaicardList[j]) {
                            var bg_sp = card.getChildByName("sprite");
                            if (bg_sp) {
                                var nums_sp = bg_sp.getChildByName("num_sp");
                                if (nums_sp) {
                                    this.addJiPaiFlag(pos, i, CardType.Card_Out, bg_sp, nums_sp);
                                }
                            }
                            break;
                        }
                    }
                }
            }
        }

        //碰杠是否有鸡牌
        for (var i = 0; i < this.peng_card[pos].length; i++) {
            var pengcard = this.peng_card[pos][i];
            for (var j = 0; j < pengcard.cardList.length; j++) {
                var card = pengcard.cardList[j];
                if (card) {
                    for (var k = 0; k < JiPaicardList.length; k++) {
                        if (card.value == JiPaicardList[k]) {
                            var bg_sp = card.getChildByName("sprite");
                            if (bg_sp) {
                                var nums_sp = bg_sp.getChildByName("num_sp");
                                if (nums_sp) {
                                    this.addJiPaiFlag(pos, i, CardType.Card_Peng, bg_sp, nums_sp);
                                }
                            }
                            break;
                        }
                    }
                }
            }
        }
    },

    addJiPaiFlag: function (pos, index, type, bgcard, num_sp) {
        var bg_size = bgcard.getBoundingBox().size;
        var nums_size = num_sp.getBoundingBox().size;
        var nums_pos = num_sp.getPosition();
        var flag_str = "";
        if (type == CardType.Card_Hand || type == CardType.Card_Peng) {
            if (pos == 0) {
                flag_str = getResPath("RoomMJ/GZMJ/jipai_flag_2.png");
            }
            else if (pos == 2) {
                flag_str = getResPath("RoomMJ/GZMJ/jipai_flag_3.png");
            }
            else {
                flag_str = getResPath("RoomMJ/GZMJ/jipai_flag_4.png");
            }
        }
        else if (type == CardType.Card_Out) {
            if (pos == 0 || pos == 2) {
                flag_str = getResPath("RoomMJ/GZMJ/jipai_flag_3.png");
            }
            else {
                flag_str = getResPath("RoomMJ/GZMJ/jipai_flag_4.png");
            }
        }
        var flag_sp = new cc.Sprite(flag_str);
        if (flag_sp) {
            var flag_size = flag_sp.getContentSize();
            flag_sp.setAnchorPoint(cc.p(0.5, 0.5));
            flag_sp.setPosition(nums_pos);
            bgcard.addChild(flag_sp);
        }
    },

    setBaoTingCardClickEnable: function (v, IsMoCard) {
        if (IsMoCard == undefined) IsMoCard = false;
        if (IsMoCard) {
            //是自己摸牌的话 除摸牌的情况下禁用
            for (var i = 0; i < this.hand_card[0].length - 1; i++) {
                var card = this.hand_card[0][i];
                if (card) {
                    card.setCardTouchEnable(v);
                    card.setBlackVis(!v);
                }
            }
        }
        else {
            if (v) {
                for (var i = 0; i < this.hand_card[0].length; i++) {
                    var card = this.hand_card[0][i];
                    if (card) {
                        card.setCardTouchEnable(v);
                    }
                }
            }
            else {
                for (var i = 0; i < this.hand_card[0].length; i++) {
                    var card = this.hand_card[0][i];
                    card.setCardTouchEnable(false);
                    card.setBlackVis(true);
                    for (var j = 0; j < MJModel.listen_info.length; j++) {
                        var tli = MJModel.listen_info[j];
                        if (card.value == tli.OutCard) {
                            card.setTingPai(true);
                            card.setCardTouchEnable(true);
                            card.setBlackVis(false);
                            break;
                        }
                    }
                }
            }
        }
    },
    resetAllCardClickState:function(){
        for (var i = 0; i < this.hand_card[0].length; i++) {
            var card = this.hand_card[0][i];
            card.setCardTouchEnable(true);
            card.setBlackVis(false);
        }
    },
    openHuaZhuClickOnly: function () {
        var lackType = MJModel.lackType;
        if (lackType == -1) {
            return;
        }
        var hasHuaZhu = false;
        for (var i = 0; i < this.hand_card[0].length; i++) {
            var card = this.hand_card[0][i];
            var paiSe = parseInt(card.value / 16);
            if (lackType != -1 && paiSe == lackType){
                hasHuaZhu = true;
                break;
            }
        }

        if (hasHuaZhu){
            for (var i = 0; i < this.hand_card[0].length; i++) {
                var card = this.hand_card[0][i];
                var paiSe = parseInt(card.value / 16);
                if (lackType != -1 && paiSe == lackType) {
                    card.setCardTouchEnable(true);
                    card.setBlackVis(false);
                }else{
                    card.setCardTouchEnable(false);
                    card.setBlackVis(true);
                }

            }
        }else{
            for (var i = 0; i < this.hand_card[0].length; i++) {
                var card = this.hand_card[0][i];
                card.setCardTouchEnable(true);
                card.setBlackVis(false);
            }
        }
    },
    closeHuaZhuClickOnly:function () {
        if (MJModel.isAutoOutCard || MJModel.isBaoting || MJModel.lackType == -1){
            return;
        }
        for (var i = 0; i < this.hand_card[0].length; i++) {
            var card = this.hand_card[0][i];
            card.setBlackVis(false);
        }
    }
});

var MJCard = cc.Layer.extend({
    tag: "MJCard",
    value: 0,
    pos: -1,
    type: 0,
    touchEnable: false,
    isUp: false,
    isMove: false,
    touchEnd: false,
    isCheckUp: false,
    isCardTouch: true,
    touchBegin: false,
    touchBeginY: 0,

    light_bg: null,
    tingpai_flag: null,
    black_bg: null,

    huazhu_flag: null,
    GuiPaiIdentify: null,

    initPos: null,
    _upcallback: null,
    _outcallback: null,
    listener: null,
    ctor: function () {
        this._super();
    },
    setUpCallback: function (callback) {
        this._upcallback = callback;
    },
    setOutCallback: function (callback) {
        this._outcallback = callback;
    },
    setValue: function (value, pos, type, index) {
        this.pos = pos;
        this.value = (value == null || value == undefined) ? 0 : value;
        this.type = type;

        this.removeChildByName("sprite");

        var bg_str = MJConfig.getCardBgStr(value, pos, type, index);
        if (bg_str != "") {
            var bg_sp = new cc.Sprite(bg_str);
            bg_sp.setName("sprite");
            bg_sp.setPosition(cc.p(bg_sp.getContentSize().width / 2, bg_sp.getContentSize().height / 2));
            this.addChild(bg_sp);

            if (pos == 0 && MJModel.seatid != -1 && type == CardType.Card_Hand) {
                this.light_bg = new cc.Sprite(getResPath("RoomMJ/choose.png"));
                this.light_bg.setPosition(cc.p(bg_sp.getContentSize().width / 2, bg_sp.getContentSize().height / 2));
                this.light_bg.setVisible(false);
                this.light_bg.setScale(1.17);
                this.addChild(this.light_bg);

                this.tingpai_flag = new cc.Sprite(getResPath("RoomMJ/tingpai_flag.png"));
                this.tingpai_flag.setPosition(cc.p(bg_sp.getContentSize().width / 2, bg_sp.getContentSize().height / 2 + 50));
                this.tingpai_flag.setVisible(false);
                this.addChild(this.tingpai_flag);

                this.huazhu_flag = new cc.Sprite(getResPath("RoomMJ/scmj/huazhu_icon.png"));
                this.huazhu_flag.setPosition(cc.p(bg_sp.getContentSize().width / 2, bg_sp.getContentSize().height / 2 + 50));
                this.showHuaZhu();
                this.addChild(this.huazhu_flag);

                this.black_bg = new cc.Sprite(getResPath("RoomMJ/card_black.png"));
                this.black_bg.setAnchorPoint(cc.p(0, 0));
                this.black_bg.setPosition(cc.p(0, 0));
                this.black_bg.setVisible(false);
                this.black_bg.setScale(bg_sp.getContentSize().width / this.black_bg.getContentSize().width, bg_sp.getContentSize().height / this.black_bg.getContentSize().height);
                this.addChild(this.black_bg, 10);

                this.GuiPaiIdentify = new cc.Sprite(getResPath("RoomMJ2/guipai_corner.png"));
                this.GuiPaiIdentify.setAnchorPoint(cc.p(0, 0));
                this.GuiPaiIdentify.setPosition(cc.p(0, 0));
                this.GuiPaiIdentify.setVisible(false);
                bg_sp.addChild(this.GuiPaiIdentify);
            }
            if (type == CardType.Card_End) {
                this.GuiPaiIdentify = new cc.Sprite(getResPath("RoomMJ2/guipai_corner.png"));
                this.GuiPaiIdentify.setAnchorPoint(cc.p(0, 0));
                this.GuiPaiIdentify.setPosition(cc.p(0, 22));
                this.GuiPaiIdentify.setVisible(false);
                bg_sp.addChild(this.GuiPaiIdentify);
            }
            if (value > 0 && type != CardType.Card_Mo) {
                var num_str = MJConfig.getCardValueStr(value, pos, type);
                var num_sp = new cc.Sprite(num_str);
                if (num_sp) {
                    num_sp.setPosition(MJConfig.getCardValuePos(pos, type, bg_sp.getContentSize()));
                    num_sp.setName("num_sp");
                    num_sp.setRotation(MJConfig.getRotation(pos, type));
                    num_sp.setScale(MJConfig.getScale(pos, type));
                    bg_sp.addChild(num_sp);
                }
            }

            this.ignoreAnchorPointForPosition(false);
            this.setContentSize(bg_sp.getContentSize());
            this.setAnchorPoint(cc.p(0.5, 0.5));

        }
    },

    showHuaZhu: function () {
        var paiSe = parseInt(this.value / 16);
        var lackType = MJModel.lackType;
        if (lackType != -1 && paiSe == lackType) {
            this.huazhu_flag.setVisible(true);
        } else {
            this.huazhu_flag.setVisible(false);
        }
    },

    showGuiPai: function () {
        if (this.value == MJModel.valueGuiPai) {
            if (this.GuiPaiIdentify) {
                this.GuiPaiIdentify.setVisible(true);
            }
        }
    },

    reset: function () {
        this.setTingPai(false);
        this.setLight(false);
        this.stopAllActions();
    },

    setMyPosition: function (pos) {
        this.initPos = pos;
        this.setPosition(this.initPos);
    },
    onExit: function () {
        try {
            if (this.listener) cc.eventManager.removeListener(this.listener);
        } catch (e) {
        }

        this._super();
    },
    setTouchEnable: function (b) {
        // this.touchEnable = b;
        // var self = this;
        // try {
        //     if (this.touchEnable) {
        //         this.listener = cc.EventListener.create({
        //             event: cc.EventListener.TOUCH_ONE_BY_ONE,
        //             swallowTouches: true,
        //             onTouchBegan: function (touch, event) {
        //                 return self.onTouchBegan(touch, event);
        //             },
        //             onTouchMoved: function (touch, event) {
        //                 self.onTouchMoved(touch, event);
        //             },
        //             onTouchEnded: function (touch, event) {
        //                 self.onTouchEnded(touch, event);
        //             }
        //         });
        //         if (this.listener) cc.eventManager.addListener(this.listener, this);
        //     }
        // } catch (e) {
        // }
    },

    setTingPai: function (v) {
        if (this.tingpai_flag) this.tingpai_flag.setVisible(v);
    },

    setUp: function (v) {
        this.isUp = v;
        this.setPosition(cc.p(this.initPos.x, this.initPos.y + (v ? 30 : 0)));
        this.setLight(v);
    },

    setBlackVis: function (v) {
        if (this.black_bg) this.black_bg.setVisible(v);
    },

    setLight: function (v) {
        if (this.light_bg) this.light_bg.setVisible(v);
    },

    goBack: function () {
        this.setPosition(this.initPos);
        this.isCheckUp = false;
        this.touchEnd = true;
        this.setLight(false);
        this.setLocalZOrder(0);
    },

    _isTouchInside: function (touch) {
        var touchPoint = touch.getLocation();
        var localPos = this.convertToNodeSpace(touchPoint);

        var s = this.getContentSize();
        var rc = cc.rect(0, 0, s.width, s.height)
        var isTouched = cc.rectContainsPoint(rc, localPos);
        return isTouched;
    },

    onTouchBegan: function (touch, event) {
        if (!touch.getLocation) {
            return false;
        }

        if (!this.isCardTouch) {
            return false;
        }
        if (MJModel.canTouchCard == false || this.isVisible() == false) {
            return false;
        }

        if ((MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_YNMJ_DZ) && this.value == MJModel.valueGuiPai) {
            return false;
        }

        if (this._isTouchInside(touch)) {
            this.touchBegin = touch.getLocation();
            this.touchBeginY = this.getPositionY();
            this.isMove = false;
            this.touchEnd = false;

            if (this.isCheckUp && !MJModel.canChoiceMore) {
                if (this._outcallback) this._outcallback(this);
                this.touchEnd = true;
            }
            this.unschedule(this.checkUp);
            return true;
        }
        return false;
    },

    onTouchMoved: function (touch, event) {
        if (MJModel.canTouchCard == false || MJModel.canChoiceMore || this.touchEnd == true)return;
        var touchPoint = touch.getLocation();
        if (Math.abs(touchPoint.x - this.touchBegin.x) > 5 || Math.abs(touchPoint.y - this.touchBegin.y) > 5) {
            this.isMove = true;
        }

        {
            if (touchPoint.y < this.initPos.y) {
                touchPoint.y = this.initPos.y;
            }
            this.setPosition(touchPoint);
            this.setLocalZOrder(10000);
        }
    },

    onTouchEnded: function (touch, event) {
        if (MJModel.canTouchCard == false || this.touchEnd == true)return;
        if (this.isMove) {
            var dy = this.getPositionY() - this.initPos.y;
            if (dy >= 100) {
                if (this._outcallback) this._outcallback(this);
            }
            else {
                this.goBack();
            }

            return;
        }

        if (!this._isTouchInside(touch)) {
            this.goBack();
            return;
        }

        if (this._upcallback) this._upcallback(this);
        this.schedule(this.checkUp, 0.2);
        this.isCheckUp = true;
    },

    checkUp: function (dt) {
        this.unschedule(this.checkUp);
        this.isCheckUp = false;
    },

    setCardTouchEnable: function (v) {
        this.isCardTouch = v;
    }
});

var MJPeng = cc.Layer.extend({
    pengType: 0,
    value: 0,
    cardList: [],

    ctor: function () {
        this._super();
    },
    setValue: function (type, pos, value, index) {
        this.pengType = type;
        this.value = value;
        this.removeAllChildren();
        this.cardList = [];

        for (var i = 0; i < 3; i++) {
            var card = new MJCard();
            if (this.pengType == PengType.Chi_L) {
                card.setValue(value + i, pos, CardType.Card_Peng, i);
            }
            else if (this.pengType == PengType.Chi_M) {
                card.setValue(value + (i - 1), pos, CardType.Card_Peng, i);
            }
            else if (this.pengType == PengType.Chi_R) {
                card.setValue(value + (i - 2), pos, CardType.Card_Peng, i);
            }
            else {
                card.setValue(this.pengType == PengType.Peng_AnKang ? 0 : value, pos, CardType.Card_Peng, i);
            }

            card.setMyPosition(MJConfig.getPengKangCardPos2(pos, index, i));
            this.addChild(card, MJConfig.getPengKangCardOrder2(pos, index, i));

            this.cardList[i] = card;
        }

        if (type == PengType.Peng_Kang || type == PengType.Peng_AnKang) {
            var card = new MJCard();
            card.setValue(value, pos, CardType.Card_Peng, 3);
            card.setMyPosition(MJConfig.getPengKangCardPos2(pos, index, 3));
            this.addChild(card, MJConfig.getPengKangCardOrder2(pos, index, 3));
            this.cardList[3] = card;
        }
    },
});