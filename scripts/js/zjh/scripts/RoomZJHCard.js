var RoomZJHCard = cc.Layer.extend({
    aId: 0,
    bId: 0,
    aSeatid: 0,
    bSeatid: 0,
    loseId: 0,
    position: 0,
    status: 0,
    flagArr: [[], [], [], [], [], [], []],
    isSee: 0,
    valueArr: [[], [], [], [], []],
    d: 0,

    beginCardX: 0,
    beginCardY: 0,
    endCardx: 0,
    endCardy: 0,
    cardNums: 0,
    curLoopSound: 0,
    p_nums: 0,
    car_nums: 0,

    kInAngleZ: 270, //里面卡牌的起始Z轴角度
    kInDeltaZ: 90,  //里面卡牌旋转的Z轴角度差
    kOutAngleZ: 0,   //封面卡牌的起始Z轴角度
    kOutDeltaZ: 90,  //封面卡牌旋转的Z轴角度差
    DZ_CARD_SCALE: 0.66,


    ctor: function () {
        this._super();

        this.p_nums = 5;
        // 偏移
        this.d = 50;
        this.car_nums = 3;
        var size = cc.director.getWinSize();
        this.ignoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.5, 0.5));
        this.setContentSize(cc.size(size.width, size.height));

        var c = RoomZJHConfig.getTablePos();
        this.beginCardX = c.x;
        this.beginCardY = size.height + 240;
        this.endCardx = c.x;
        this.endCardy = c.y + 200;

        var bg = new cc.LayerColor(cc.color(0, 0, 0, 200));
        bg.setContentSize(cc.size(380, 250));
        bg.setTag(10);
        bg.ignoreAnchorPointForPosition(false);
        bg.setAnchorPoint(cc.p(0.5, 0.5));
        bg.setPosition(cc.p(c.x, size.height / 2 + this.d));
        bg.setVisible(false);
        this.addChild(bg, 5);

        var card = new cc.Sprite(getResPath("Room/card_back.png"));
        card.setTag(100);
        card.setVisible(false);
        card.setAnchorPoint(cc.p(0.5, 0.5));
        card.setPosition(cc.p(this.beginCardX, this.beginCardY));
        card.setScale(this.DZ_CARD_SCALE);
        this.addChild(card, -100);

        this.createCard();  // -- 生成卡牌
        this.createFlag();  // -- 生成标志
        this.resetCard(); //初始化所有牌的位置
        this.hideAllCard();
    },

    resetCard: function () {
        for (var j = 0; j < this.p_nums; ++j) {
            for (var i = 0; i < this.car_nums; ++i) {
                this.getChildByTag(RoomZJHConfig.tag_card_begin + this.car_nums * j + i).setPosition(RoomZJHConfig.getCardPos(j, i, this.car_nums));
                this.getChildByTag(RoomZJHConfig.tag_card_begin + this.car_nums * j + i).stopAllActions();
            }
        }

    },

    beginFaPai: function () {
        var card = this.getChildByTag(100);
        card.setVisible(true);
        card.runAction(cc.moveTo(0.5, cc.p(this.endCardx, this.endCardy)));
        this.cardNums = 0;
    },

    endFaPai: function () {
        var card = this.getChildByTag(100);
        var moveTo = cc.moveTo(0.5, cc.p(this.beginCardX, this.beginCardY));
        var callback = cc.callFunc(function () {
            card.setVisible(false);
        });
        card.runAction(cc.sequence(moveTo, callback));
        this.cardNums = 0;
    },

    showCard: function (index, no, delay, value) {
        if (no == undefined && delay == undefined && value == undefined) {
            this.showCardByIndex(index);
            return;
        }
        this.cardNums++;
        if (index == 0) this.valueArr[no] = value;

        var size = cc.director.getWinSize();
        var begin = cc.p(this.endCardx, this.endCardy);
        var card = this.getChildByTag(RoomZJHConfig.tag_card_begin + index * this.car_nums + no);

        var texture = cc.textureCache.addImage(getResPath("Room/card_back.png"));
        card.setTexture(texture);

        var end = card.getPosition();

        card.setPosition(begin);
        card.setVisible(true);
        card.setOpacity(0);

        var wait = cc.delayTime(delay);
        var move = cc.moveTo(0.2, end);
        var moveAndScale = move;
        if (index == 0) {
            card.setScale(this.DZ_CARD_SCALE);
            moveAndScale = cc.spawn(move, cc.scaleTo(0.2, 1.0));
        }
        var callback = cc.callFunc(this.playDealCallback, this);
        var fadein = cc.fadeIn(0.1);
        var callback2 = cc.callFunc(this.playDealCallback2, this);

        if (value != 0) {
            var self = this;
            var callback3 = cc.callFunc(function () {
                self.playDealCallback3(card);
            });

            card.runAction(cc.sequence(wait, callback, fadein, moveAndScale, callback3, callback2));
        }
        else {
            card.runAction(cc.sequence(wait, callback, fadein, moveAndScale, callback2));
        }
    },
    sendFiveHandCards: function (delay, card_num) {
        var table_point = RoomZJHConfig.getTablePos();
        var dealPoint = cc.p(table_point.x, table_point.y + 220);
        var firstCardPoint = RoomZJHConfig.getCMCard();

        var backtexture = cc.textureCache.addImage(getResPath("Room/card_back.png"));
        for (var i = 0; i < card_num; ++i) {
            var bgcard = this.getChildByTag(RoomZJHConfig.tag_card_common_bg + i);
            if (null != bgcard) {
                bgcard.removeFromParentAndCleanup(true);
            }
            bgcard = new cc.Sprite(backtexture);
            bgcard.setTag(RoomZJHConfig.tag_card_common_bg + i);
            this.addChild(bgcard, -10);
            bgcard.setTexture(backtexture);
            bgcard.setPosition(dealPoint);
            bgcard.setVisible(true);

            var wait = cc.delayTime(delay + 0.2 * i);
            var sound_fapai = cc.callFunc(this.playDealCallback, this);
            var move = cc.moveTo(0.3, cc.p(firstCardPoint.x + 60 * i, firstCardPoint.y));
            bgcard.runAction(cc.sequence(wait, sound_fapai, move));
        }
    },

    openFiveHandCards: function (handCards, delay, start_card, end_card) {
        if (end_card > handCards.length) {
            return;
        }
        var firstCardPoint = RoomZJHConfig.getCMCard();
        for (var i = start_card; i < end_card; ++i) {
            var bgcard = this.getChildByTag(RoomZJHConfig.tag_card_common_bg + i);
            var wait = cc.delayTime(delay + 0.2 * i);
            var sound_fanpai = cc.callFunc(this.playDealCallback, this);
            var m_openAnimOut = cc.sequence(cc.orbitCamera(0.2, 1, 0, this.kOutAngleZ, this.kOutDeltaZ, 0, 0), cc.hide(), cc.delayTime(0.2));
            var wait_fanpai = cc.delayTime(0.6);
            var seq = cc.sequence(wait, wait_fanpai, sound_fanpai, m_openAnimOut);
            bgcard.runAction(seq);

            var cardStr = __String.createWithFormat("Room/card/card_%1.png", ((handCards.begin() + i)));
            var frontCard = this.getChildByTag(RoomZJHConfig.tag_card_common + i);
            if (null != frontCard) {
                frontCard.removeFromParentAndCleanup(true);
            }
            frontCard = new cc.Sprite(cardStr);
            frontCard.setTag(RoomZJHConfig.tag_card_common + i);
            this.addChild(frontCard, -10);
            frontCard.setTexture(cardStr);
            frontCard.setPosition(cc.p(firstCardPoint.x + 55 * i, firstCardPoint.y));
            frontCard.setVisible(false);

            var wait1 = cc.delayTime(delay + 0.2 * i);
            var m_openAnimIn = cc.sequence(cc.delayTime(0.2), cc.show(), cc.orbitCamera(0.2, 1, 0, this.kInAngleZ, this.kInDeltaZ, 0, 0));
            var wait_fanpai2 = cc.delayTime(0.6);
            var seq1 = cc.sequence(wait1, wait_fanpai2, m_openAnimIn);
            frontCard.runAction(seq1);
        }
    },

    firstGameShowFiveCards: function (handCards) {
        var firstCardPoint = RoomZJHConfig.getCMCard();

        for (var i = 0; i < handCards.length; ++i) {
            var cardStr = __String.createWithFormat("Room/card/card_%1.png", ((handCards.begin() + i)));

            var frontCard = this.getChildByTag(RoomZJHConfig.tag_card_common + i);
            if (null == frontCard) {
                frontCard = new cc.Sprite(cardStr);
                frontCard.setTag(RoomZJHConfig.tag_card_common + i);
                this.addChild(frontCard, -10);
            }
            frontCard.setTexture(cardStr);
            frontCard.setPosition(cc.p(firstCardPoint.x + 60 * i, firstCardPoint.y));
            frontCard.setVisible(true);
        }

        if (handCards.length <= 3) {
            for (var i = 3; i < 5; ++i) {
                var bgcard = this.getChildByTag(RoomZJHConfig.tag_card_common_bg + i);
                if (null != bgcard) {
                    bgcard.removeFromParentAndCleanup(true);
                }
                bgcard = new cc.Sprite(getResPath("Room/card_back.png"));
                bgcard.setTag(RoomZJHConfig.tag_card_common_bg + i);
                this.addChild(bgcard, -10);
                bgcard.setTexture(getResPath("Room/card_back.png"));
                bgcard.setPosition(cc.p(firstCardPoint.x + 60 * i, firstCardPoint.y));
                bgcard.setVisible(true);
            }
        }
    },

    playDealCallback: function () {
        Sound.getInstance().playEffect(getResPath("Sound/zjh/g_deal.mp3"));
    },

    playDealCallback2: function () {
        this.cardNums--;
        if (this.cardNums == 0) this.endFaPai();
    },
    playDealCallback3: function (card) {
        var no = (card.getTag() - RoomZJHConfig.tag_card_begin) % this.car_nums;
        this.showHoleCard2(no, this.valueArr[no]);
    },
    showCardByIndex: function (index) {
        var numcard = 3;
        for (var i = 0; i < this.car_nums; i++) {
            var card = this.getChildByTag(RoomZJHConfig.tag_card_begin + index * this.car_nums + i);
            if (index == 0) {
                card.setScale(1);
            }
            card.setTexture(getResPath("Room/card_back.png"));
            card.setPosition(RoomZJHConfig.getCardPos(index, i, numcard));
            card.setVisible(true);
        }
    },
    hideCard: function (index) {
        for (var i = 0; i < this.car_nums; i++) {
            var card = this.getChildByTag(RoomZJHConfig.tag_card_begin + index * this.car_nums + i);
            card.setVisible(false);
        }
    },
    hideAllCard: function () {
        for (var index = 0; index < this.p_nums; index++) {
            for (var i = 0; i < this.car_nums; i++) {
                var card0 = this.getChildByTag(RoomZJHConfig.tag_card_begin + index * this.car_nums + i);
                card0.setVisible(false);
            }
        }


        //隐藏5张底牌
        for (var i = 0; i < 5; ++i) {
            var bgcard = this.getChildByTag(RoomZJHConfig.tag_card_common_bg + i);
            if (bgcard != null) {
                bgcard.setColor(cc.color(255, 255, 255));
                bgcard.setVisible(false);
            }

            var frontCard = this.getChildByTag(RoomZJHConfig.tag_card_common + i);
            if (frontCard != null) {
                frontCard.setColor(cc.color(255, 255, 255));
                frontCard.setVisible(false);
            }
        }
    },
    showFlag: function (index, no) {
        this.hideFlag(index);
        if (index == 0) {
            if (no == 1) {
                this.flagArr[index] = no;
                return;
            }
        }
        if (no == 0) {
            return;
        }
        this.flagArr[index] = no;
        var temp = 0;
        if (no == 1) {
            temp = RoomZJHConfig.tag_card_flag_see_begin + index;
        }
        else if (no == 2) {
            temp = RoomZJHConfig.tag_card_flag_lose_begin + index;
        }
        else if (no == 3) {
            temp = RoomZJHConfig.tag_card_flag_fold_begin + index;
        }
        var flag = this.getChildByTag(temp);
        flag.setVisible(true);
    },

    hideFlagTmp: function (index) {
        var readcard = this.getChildByTag(RoomZJHConfig.tag_card_flag_see_begin + index);
        var lose = this.getChildByTag(RoomZJHConfig.tag_card_flag_lose_begin + index);
        var giveup = this.getChildByTag(RoomZJHConfig.tag_card_flag_fold_begin + index);

        readcard.setVisible(false);
        lose.setVisible(false);
        giveup.setVisible(false);

        this.flagArr[index] = 0;

    },

    hideFlag: function (index) {
        var readcard = this.getChildByTag(RoomZJHConfig.tag_card_flag_see_begin + index);
        var lose = this.getChildByTag(RoomZJHConfig.tag_card_flag_lose_begin + index);
        var giveup = this.getChildByTag(RoomZJHConfig.tag_card_flag_fold_begin + index);

        this.readcard.setVisible(false);
        this.lose.setVisible(false);
        this.giveup.setVisible(false);

        this.flagArr[index] = 0;
    },

    hideAllFlag: function () {
        for (var i = 0; i < this.p_nums; ++i) {
            var pflag = this.getChildByTag(RoomZJHConfig.tag_card_flag_see_begin + i);
            pflag.setVisible(false);
            pflag = this.getChildByTag(RoomZJHConfig.tag_card_flag_lose_begin + i);
            pflag.setVisible(false);
            pflag = this.getChildByTag(RoomZJHConfig.tag_card_flag_fold_begin + i);
            pflag.setVisible(false);

        }
        for (var i = 0; i < 7; i++) {
            this.flagArr[i] = 0;
        }
    },

    up: function (node, pSender) {
        this.reorderChild(pSender, 10);
    },
    down: function (node, pSender) {
        this.reorderChild(pSender, -1);
    },
    showBGPlayerInfo: function () {
        var size = cc.director.getWinSize();
        var c = RoomZJHConfig.getTablePos();

        var bg = this.getChildByTag(10);
        var vs = this.getChildByTag(11);
        bg.setVisible(true);
        vs.setVisible(true);

        var aPlayer = RoomZJHInstance.players[this.aSeatid];
        var bPlayer = RoomZJHInstance.players[this.bSeatid];

        var avatar1 = utils.createCircleAvatar(aPlayer.avatar, getResPath("Room/user4_unlogin.png"), getResPath("Room/StencilBg.png"), cc.size(80, 80));

        if (avatar1 != null) {
            avatar1.setTag(210);
            avatar1.setPosition(cc.p(c.x + 35 + 50, size.height / 2 + 90));
            this.addChild(avatar1, 1000);
        }
        var aName = new cc.LabelTTF(aPlayer.name, "Thonburi", 26);
        aName.setTag(211);
        aName.setColor(cc.color(255, 255, 50));
        aName.setPosition(cc.p(c.x + 35 + 50, size.height / 2 + 150));
        this.addChild(aName, 1000);

        var avatar2 = utils.createCircleAvatar(bPlayer.avatar, getResPath("Room/user4_unlogin.png"), getResPath("Avatars/StencilBg.png"), cc.size(80, 80));
        if (avatar2 != null) {
            avatar2.setTag(212);
            avatar2.setPosition(cc.p(c.x - 35 - 50, size.height / 2 + 90));
            this.addChild(avatar2, 1000);
        }

        var bName = new cc.LabelTTF(bPlayer.name, "Thonburi", 26);
        bName.setTag(213);
        bName.setColor(cc.color(255, 255, 50));
        bName.setPosition(cc.p(c.x - 35 - 50, size.height / 2 + 150));
        this.addChild(bName, 1000);
    },
    showLightning: function () {

        this.curLoopSound = Sound.getInstance().playEffect(getResPath("Sound/zjh/g_cmp_light.mp3"));

        var size = cc.director.getWinSize();
        var c = RoomZJHConfig.getTablePos();
        cc.spriteFrameCache.addSpriteFrames(getResPath("Room/game_lightning.plist"), getResPath("Room/game_lightning.png"));

        var sprite = new cc.Spirte();
        sprite.setAnchorPoint(cc.p(0.5, 0.5));
        sprite.setPosition(cc.p(c.x, c.y - 50 + d));
        var frames = [];
        for (var i = 1; i < 4; i++) {
            var name = __String.createWithFormat("game_lightning%1.png", i);
            frames.push(cc.spriteFrameCache.getSpriteFrame(name));
        }

        var animation = new cc.Animation(frames, 1 / 15);
        var animate = cc.animate(animation);
        sprite.runAction(cc.repeatForever(animate));
        var callback = cc.callFunc(this.ainimationEndCallBack, this);
        sprite.runAction(cc.sequence(cc.delayTime(3.0), callback));
        this.addChild(sprite, 1000);
        frames.clear();
    },

    showFire: function () {
        Sound.getInstance().playEffect(getResPath("Sound/zjh/g_cmp_fire.mp3"));

        var size = cc.director.getWinSize();
        var c = RoomZJHConfig.getTablePos();
        cc.spriteFrameCache.addSpriteFrames(getResPath("Room/game_fire.plist"), getResPath("Room/game_fire.png"));
        var sprite = new cc.Sprite();
        sprite.setAnchorPoint(cc.p(0.5, 0.5));
        if (position == 0) {
            sprite.setPosition(cc.p(c.x - 35 - 50, c.y - 50 + d));
        }
        else {
            sprite.setPosition(cc.p(c.x + 35 + 50, c.y - 50 + d));
        }

        var frames = [];
        for (var i = 1; i < 9; i++) {
            var name = __String.createWithFormat("game_fire%1.png", i);
            var frame = cc.spriteFrameCache.getSpriteFrame(name);
            frames.push(frame);
        }

        var animation = new cc.Animation(frames, 1 / 15);
        frames.clear();
        var animate = cc.animate(animation);
        sprite.runAction(animate);
        var callback = cc.callFunc(this.ainimationEndCallBack, this);
        sprite.runAction(cc.sequence(cc.delayTime(0.5), callback));
        this.addChild(sprite, 1000);
    },

    ainimationEndCallBack: function (pSender) {
        pSender.stopAllActions();
        pSender.removeAllChildrenWithCleanup(true);
        pSender.removeFromParentAndCleanup(true);
        //todo stop effect
        //SimpleAudioEngine::getInstance().stopEffect(curLoopSound);
    },

    onExit: function () {
        this.release();
        this._super();
        //todo stop effect
        //SimpleAudioEngine::getInstance().stopEffect(curLoopSound);
    },

    showFlagInfo: function () {
        this.setDouNiuCardTypeVisible(true);
        this.showFlag(this.aId, this.flagArr[aId]);
        this.showFlag(this.bId, this.flagArr[bId]);
        this.showFlag(this.loseId, 2);
        this.status = 0;

        if (this.aId == 0) {
            if (this.loseId == 0) {
                Sound.getInstance().playEffect(getResPath("Sound/zjh/g_cmp_fail.mp3"));
            }
            else {
                Sound.getInstance().playEffect(getResPath("Sound/zjh/g_cmp_suc.mp3"));
            }
        }

        if (this.bId == 0) {
            if (this.loseId == 0) {
                Sound.getInstance().playEffect(getResPath("Sound/zjh/g_cmp_fail.mp3"));
            }
            else {
                Sound.getInstance().playEffect(getResPath("Sound/zjh/g_cmp_suc.mp3"));
            }
        }
    },

    showHolesCard: function () {
        if ((this.aId == 0 || this.bId == 0) && this.isSee == 1) {
            this.showHolesCard1(0);
        }
    },

    showHolesCard1: function (isBack) {
        if (isBack == 1) {
            for (var i = 0; i < this.car_nums; i++) {
                this.showHoleCard(i);
            }
        }
        else {
            for (var i = 0; i < this.car_nums; i++) {
                this.showHoleCard2(i, this.valueArr[i]);
            }
        }

    },

    showHoleCard2: function (no, value) {
        this.isSee = 1;
        this.valueArr[no] = value;
        var card = this.getChildByTag(RoomZJHConfig.tag_card_begin + no);
        var scaleX1 = cc.scaleTo(0.17, 0.2, 1);
        var scaleX2 = cc.scaleTo(0.17, 1, 1);
        card.runAction(cc.sequence(scaleX1, scaleX2));
        var callback = cc.callFunc(this.showHoleCallBack1, this);
        card.runAction(cc.sequence(cc.delayTime(0.17), callback));
    },

    showHoleCallBack1: function (pSender) {
        var card = pSender;
        var index = card.getTag() - RoomZJHConfig.tag_card_begin;
        var name = __String.createWithFormat("Room/card/card_%1.png", this.valueArr[index]);
        var texture = cc.textureCache.addImage(name);
        card.setTexture(texture);
    },

    showHoleCard: function () {
        var card = this.getChildByTag(RoomZJHConfig.tag_card_begin + no);
        var scaleX1 = cc.scaleTo(0.3, 0.2, 1);
        var scaleX2 = cc.scaleTo(0.3, 1, 1);
        card.runAction(cc.sequence(scaleX1, scaleX2));
        var callback = cc.callFunc(this.showHoleCallBack2, this);
        card.runAction(cc.sequence(cc.delayTime(0.3), callback));
    },

    showHoleCallBack2: function (pSender) {
        var card = pSender;
        var texture = cc.textureCache.addImage(getResPath("Room/card_back.png"));
        card.setTexture(texture);
    },

    reset: function () {
        for (var i = 0; i < 7; i++) {
            this.flagArr[i] = 0;
        }

        this.isSee = 0;

        for (var i = 0; i < 5; i++) {
            this.valueArr[i] = 0;
        }

        this.removePlayerCard();
    },

    showWinCard: function (index, a, b, c, d, e, winCardType) {
        var size = cc.director.getWinSize();
        var card0 = null;
        var card1 = null;
        var card2 = null;
        var card3 = null;
        var card4 = null;
        var name = "";

        card0 = this.getChildByTag(RoomZJHConfig.tag_card_begin + index * this.car_nums);
        if (card0 != null) {
            name = __String.createWithFormat("Room/card/card_%1.png", a);
            var func = cc.callFunc(function () {
                var texture0 = cc.textureCache.addImage(name);
                card0.setTexture(texture0);
            });
            var caleX1 = cc.scaleTo(0.17, 0.2, 1);
            var caleX2 = cc.scaleTo(0.17, 1, 1);
            card0.runAction(cc.sequence(scaleX1, scaleX2));
            card0.runAction(cc.sequence(cc.delayTime(0.17), func));
        }
        name = __String.createWithFormat("Room/card/card_%1.png", b);
        card1 = this.getChildByTag(RoomZJHConfig.tag_card_begin + index * this.car_nums + 1);
        if (card1 != null) {
            name = __String.createWithFormat("Room/card/card_%1.png", b);
            var func = cc.callFunc(function () {
                var texture0 = cc.textureCache.addImage(name);
                card1.setTexture(texture0);
            });
            var scaleX1 = cc.scaleTo(0.17, 0.2, 1);
            var scaleX2 = cc.scaleTo(0.17, 1, 1);
            card1.runAction(cc.sequence(scaleX1, scaleX2));
            card1.runAction(cc.sequence(cc.delayTime(0.17), func));
        }
        if (c) {
            card2 = this.getChildByTag(RoomZJHConfig.tag_card_begin + index * this.car_nums + 2);
            if (card2) {
                name = __String.createWithFormat("Room/card/card_%1.png", c);
                var func = cc.callFunc(function () {
                    var texture0 = cc.textureCache.addImage(name);
                    card2.setTexture(texture0);
                });
                var scaleX1 = cc.scaleTo(0.17, 0.2, 1);
                var scaleX2 = cc.scaleTo(0.17, 1, 1);
                card2.runAction(cc.sequence(scaleX1, scaleX2));
                card2.runAction(cc.sequence(cc.delayTime(0.17), func));
            }
        }
        if (d) {
            card3 = this.getChildByTag(RoomZJHConfig.tag_card_begin + index * this.car_nums + 3);
            if (card3) {
                name = __String.createWithFormat("Room/card/card_%1.png", d);
                var func = cc.callFunc(function () {
                    var texture0 = cc.textureCache.addImage(name);
                    card3.setTexture(texture0);
                });
                var scaleX1 = cc.scaleTo(0.17, 0.2, 1);
                var scaleX2 = cc.scaleTo(0.17, 1, 1);
                card3.runAction(cc.sequence(scaleX1, scaleX2));
                card3.runAction(cc.sequence(cc.delayTime(0.17), func));
            }
        }
        if (e) {
            card4 = this.getChildByTag(RoomZJHConfig.tag_card_begin + index * this.car_nums + 4);
            if (card4) {
                name = __String.createWithFormat("Room/card/card_%1.png", e);
                var func = cc.callFunc(function () {
                    var texture0 = cc.textureCache.addImage(name);
                    card4.setTexture(texture0);
                });
                var scaleX1 = cc.scaleTo(0.17, 0.2, 1);
                var scaleX2 = cc.scaleTo(0.17, 1, 1);
                card4.runAction(cc.sequence(scaleX1, scaleX2));
                card4.runAction(cc.sequence(cc.delayTime(0.17), func));
            }
        }
    },

    wincardCallback: function (pSender) {
        pSender.removeAllChildrenWithCleanup(true);
        pSender.removeFromParentAndCleanup(true);
    },

    // -- 生成卡牌
    createCard: function () {
        var pCard = null;

        for (var j = 0; j < this.p_nums; ++j) {
            for (var i = 0; i < this.car_nums; ++i) {
                pCard = new cc.Sprite(getResPath("Room/card_back.png"));
                pCard.setTag(RoomZJHConfig.tag_card_begin + this.car_nums * j + i);
                pCard.setScale(this.DZ_CARD_SCALE);
                this.addChild(pCard, -1);
            }
        }
    },

    // -- 生成标志
    createFlag: function () {
        var pFlag = null;
        var text = null;
        for (var i = 0; i < this.p_nums; ++i) {
            var fileName;
            if (i == 1 || i == 2) {
                fileName = getResPath("Room/duizhan/room_tip_bg_right.png");
            } else {
                fileName = getResPath("Room/duizhan/room_tip_bg_left.png");
            }

            pFlag = new cc.Sprite(fileName);
            pFlag.setTag(RoomZJHConfig.tag_card_flag_see_begin + i);
            pFlag.setVisible(false);
            pFlag.setPosition(RoomZJHConfig.getFlagPos(i));
            this.addChild(pFlag);

            text = new cc.LabelTTF("已看牌", "Arial", 24);
            text.setColor(cc.color(95, 65, 56));
            text.setPosition(cc.p(43.5, 25));
            pFlag.addChild(text);

            if (i == 1 || i == 2) {
                fileName = getResPath("Room/duizhan/room_tip_bg_right_h.png");
            } else {
                fileName = getResPath("Room/duizhan/room_tip_bg_left_h.png");
            }

            pFlag = new cc.Sprite(fileName);
            pFlag.setTag(RoomZJHConfig.tag_card_flag_lose_begin + i);
            pFlag.setVisible(false);
            pFlag.setPosition(RoomZJHConfig.getFlagPos(i));
            this.addChild(pFlag);

            text = new cc.LabelTTF("已输", "Arial", 24);
            text.setPosition(cc.p(43.5, 25));
            pFlag.addChild(text);

            pFlag = new cc.Sprite(fileName);
            pFlag.setTag(RoomZJHConfig.tag_card_flag_fold_begin + i);
            pFlag.setVisible(false);
            pFlag.setPosition(RoomZJHConfig.getFlagPos(i));
            this.addChild(pFlag);

            text = new cc.LabelTTF("已弃牌", "Arial", 24);
            text.setPosition(cc.p(43.5, 25));
            pFlag.addChild(text);
        }
    },
    ///hyhy_mark douniu 为了方便显示和隐藏牌型，所以将这几个函数写在RoomZJHCard中，如果后续碰到其他问题，可以将这几个函数移到RoomTip中
    showDouNiuCardType: function (delay, pos, type) {
        //只会显示自己的牌型，所以暂时用不到pos参数
        var name =  __String.createWithFormat("Room/CardType/DN_Card_type%1.png", type);
        var node = new cc.Sprite(name);
        node.setTag(RoomZJHConfig.tag_card_douniu_cardtype);
        this.addChild(node, -1);
        node.setVisible(true);

        node.setPosition(cc.p(400, 180));
        var delayaction = cc.delayTime(delay);
        var scaleto1 = cc.scaleTo(0.17, 2.0);
        var delayaction2 = cc.delayTime(0.2);
        var scaleto2 = c.scaleTo(0.1, 0.85);
        node.setScale(0);
        node.runAction(cc.sequence(delayaction, scaleto1, delayaction2, scaleto2));
    },

    setDouNiuCardTypeVisible: function (visible) {
        var node = this.getChildByTag(RoomZJHConfig.tag_card_douniu_cardtype);
        if (node != null) {
            node.setVisible(visible);
        }
    },

    showPlayerCard: function (pos, values, cardType, delay) {
        var num = values.length;
        var begin = RoomZJHConfig.getPlayerCenterPos(pos);
        begin.y -= 27;
        var deltaX = 40;
        var buff = "";
        for (var i = 0; i < num; i++) {
            var cardSp = new cc.Sprite(getResPath("Room/card_back.png"));
            cardSp.setPosition(cc.p(begin.x + deltaX * (i - 1), begin.y));
            cardSp.setTag(555 + pos * 10 + i);
            this.addChild(cardSp);

            buff = __String.createWithFormat(getResPath("Room/card/card_%1.png"), values[i]);
            cardSp.runAction(cc.sequence(cc.scaleTo(0.17, 0.2, 1), cc.callFunc(function () {
                var texture = cc.textureCache.addImage(buff);
                cardSp.setTexture(texture);
            }), cc.scaleTo(0.17, 1, 1), cc.delayTime(2)));
        }

        var typeLabel = new cc.LabelTTF(RoomZJHInstance.getSetCardTypeString(cardType), "Arial", 30);

        if (pos == 0) {
            typeLabel.setPosition(cc.p(begin.x, begin.y - 37));
        } else {
            typeLabel.setPosition(cc.p(begin.x, begin.y - 80));
        }

        typeLabel.setVisible(false);
        typeLabel.setTag(555 + pos * 10);
        typeLabel.setColor(cc.color(252, 239, 166));
        this.addChild(typeLabel);
        typeLabel.runAction(cc.sequence(cc.delayTime(0.17), cc.show(), cc.delayTime(0.17 + 2)));
    },

    removePlayerCard: function () {
        for (var i = 0; i < 10; i++) {
            for (var j = 0; j < 5; j++) {
                var sp = this.getChildByTag(555 + i * 10 + j);
                if (sp != null) {
                    sp.removeFromParentAndCleanup(true);
                }
            }
            var lab = this.getChildByTag(555 + i * 10);
            if (lab != null) {
                lab.removeFromParentAndCleanup(true);
            }
        }
    }

});