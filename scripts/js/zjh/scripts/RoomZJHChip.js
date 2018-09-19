var RoomZJHChip = cc.Layer.extend({
    cnt: 0,
    percnt: 0,
    otherBet: [[], [], [], [], []],
    totalBet: [[], [], [], [], []],
    guestBet: [[], [], [], [], [], []],
    maxdis: 0,
    deleteIndex: 0,
    TAG_CHIP_BEGIN: 1000,

    ctor: function () {
        this._super();

        this.cnt = 0;
        this.deleteIndex = 0;
        this.percnt = 0;

        var size = cc.director.getWinSize();
        this.ignoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.5, 0.5));
        this.setContentSize(cc.size(size.width, size.height));

        var begin = cc.p(142, 42);
        var end = RoomZJHConfig.getOtherPlayerBetPos(1);
        var x = Math.abs(begin.x - end.x);
        var y = Math.abs(begin.y - end.y);
        this.maxdis = Math.sqrt(x * x + y * y);
    },

    sendChip: function (pos, value, delay) {
        var chipIndex = 0;
        for (var i = 1; i < 5; i++) {
            if (RoomZJHInstance.raise[i] == value) {
                chipIndex = i;
            }
        }
        var buff = __String.createWithFormat("Room/duizhan/dz_bet_icon_%1.png", chipIndex);
        var chip = new cc.Sprite(buff);
        chip.setTag(this.TAG_CHIP_BEGIN + this.cnt++);
        this.addChild(chip);

        buff = __String.createWithFormat("%1", value);
        var numLabel = new cc.LabelAtlas(buff, getResPath("Room/duizhan/dz_bet_num.png"), 16, 25, '0');
        numLabel.setAnchorPoint(cc.p(0.5, 0.5));
        numLabel.setPosition(cc.p(chip.getContentSize().width/2, chip.getContentSize().height/2));
        chip.addChild(numLabel);

        var beginPos = RoomZJHConfig.getPlayerCenterPos(pos);
        var endPos = RoomZJHConfig.getTablePos();
        var x = Math.random(-150, 150);
        var y = Math.random(-150 + 95, 150 + 95);
        if (x < -75 || x > 75) {//防止筹码压到牌
            y = Math.random(-75 + 95, 75 + 95);
        }
        endPos = cc.pAdd(endPos, cc.p(x, y));

        if (pos == -1) {
            chip.setPosition(endPos);
        }
        else {
            chip.setPosition(beginPos);
            chip.runAction(cc.sequence(cc.delayTime(delay),
                cc.easeExponentialOut()(cc.moveTo(0.5, endPos))));
        }

        Sound.getInstance().playEffect(getResPath("Sound/zjh/g_addchip.mp3"));
    },

    sendWinChip: function (winSeats, delay) {
        var winNum = winSeats.length;
        if (winNum == 0) {
            return;
        }

        var num = this.cnt / winNum;
        if (num == 0) {
            return;
        }

        var perDelay = 0.5 / num;
        var tag = this.TAG_CHIP_BEGIN + cnt - 1;
        for (var i = 0; i < winNum; i++) {
            var begin = RoomZJHConfig.getPlayerCenterPos(winSeats[i]);

            for (var j = 0; j < num; j++) {
                var chip = this.getChildByTag(tag);
                if (chip != null) {
                    chip.runAction(cc.sequence(cc.delayTime(delay + perDelay * j),
                        cc.easeExponentialOut(cc.moveTo(0.5, begin)), cc.removeSelf));
                }
                tag--;
            }
        }
        Sound.getInstance().playEffect(getResPath("Sound/zjh/g_get_chip.mp3"));
    },

    sendBet: function (index, bet, delay) {
        this.sendChip(index, bet, delay);
        return;
    },

    sendMyBet: function (index, targetid, bet, type, delay) {
        //外围押注;type=1自己没有在外围桌上 2：外围在桌子上，3.不在外围桌子上
        if (bet <= 0) {
            return;
        }
        var size = cc.director.getWinSize();
        var name = getResPath("Room/room_bet_jinbi.png");
        var begin, end;
        if (type == 1) {
            begin = RoomZJHConfig.getMyAvatarPos();
        }
        else if (type == 2) {
            begin = RoomZJHConfig.getPerPlayerPos(index);
        }
        else if (type == 3) {
            begin = cc.p(75, 160);
        }
        end = RoomZJHConfig.getOtherPlayerBetPos(targetid);
        var curdis = Math.sqrt(Math.abs(begin.x - end.x) * Math.abs(begin.x - end.x) + Math.abs(begin.y - end.y) * Math.abs(begin.y - end.y));
        var time = curdis / this.maxdis * 0.5;
        if (this.otherBet[targetid] < bet) {
            var chipnums = (bet - this.otherBet[targetid]) / 20000 * 6;
            if (chipnums > 20) {
                chipnums = 20;
            }
            else if (chipnums <= 0) {

            }
            for (var i = 0; i < chipnums; i++) {
                var x = Math.random(0, 50);
                var moveend = cc.p(x + end.x, end.y);
                var chip = new cc.Sprite(name);
                chip.setPosition(begin);
                this.addChild(chip);
                var to = cc.moveTo(time, moveend);
                delay += 0.03;
                chip.runAction(cc.sequence(cc.delayTime(delay), cc.easeExponentialOut(to), cc.removeSelf()));
            }
            this.otherBet[targetid] = bet;
            cc.log(__String.createWithFormat("-------otherBet[%1] == %2", targetid, this.otherBet[targetid]));
        }
        Sound.getInstance().playEffect(getResPath("Sound/zjh/room_send_jinbi.mp3"));
    },

    sendPerBet: function (targetid, bet, json, value) {
        if (bet <= 0) {
            return;
        }
        if (this.otherBet[targetid] < bet) {
            var name = getResPath("Room/room_bet_jinbi.png");
            var begin, end;
            var per_total_money = 0;
            var guest_size = value.length;
            end = RoomZJHConfig.getOtherPlayerBetPos(targetid);
            var curdis = Math.sqrt(Math.abs(begin.x - end.x) * Math.abs(begin.x - end.x) + Math.abs(begin.y - end.y) * Math.abs(begin.y - end.y));
            var time = curdis / this.maxdis * 0.5;
            for (var i = 0; i < guest_size; i++) {
                for (var val in value[i]["guest_bets"]) {
                    var seat_id = utils.getValInt(val, "seat_id");
                    var pos = RoomZJHInstance.getInstance().getPosBySeatid(seat_id);
                    if (targetid == pos) {
                        var pre_seat = utils.getValInt(value[i], "seat_num") - 1;
                        var per_money = utils.getValInt(val, "bets_money");
                        if (this.guestBet[pre_seat] < per_money) {
                            per_total_money += per_money;
                            var chipnums = (bet - this.otherBet[targetid]) / 20000 * 6;
                            begin = RoomZJHConfig.getPerPlayerPos(pre_seat);

                            var delay = 0;
                            if (chipnums > 0) {
                                var perDelay = 0.500 / chipnums;
                                for (var i = 0; i < chipnums; i++) {

                                    var x = Math.random(0, 50);
                                    var moveend = cc.p(x + end.x, end.y);
                                    var chip = new cc.Sprite(name);
                                    chip.setPosition(begin);
                                    this.addChild(chip);
                                    var to = cc.moveTo(time, moveend);

                                    delay += perDelay;
                                    chip.runAction(cc.sequence(cc.delayTime(delay), cc.easeExponentialOut(to), cc.removeSelf()));
                                }
                                this.guestBet[pre_seat] = per_money;
                            }


                        }
                    }
                }
            }
            if (per_total_money < bet) {
                var chipnums = (bet - per_total_money) / 20000 * 8;
                if (chipnums > 0) {
                    var delay = 0;
                    begin = cc.p(75, 160);
                    curdis = Math.sqrt(Math.abs(begin.x - end.x) * Math.abs(begin.x - end.x) + Math.abs(begin.y - end.y) * Math.abs(begin.y - end.y));
                    time = curdis / this.maxdis * 0.5;
                    var perDelay = 0.500 / chipnums;
                    for (var i = 0; i < chipnums; i++) {
                        var x = random(0, 50);
                        var moveend = cc.p(x + end.x, end.y);
                        var chip = new cc.Sprite(name);
                        chip.setPosition(begin);
                        this.addChild(chip);
                        var to = cc.moveTo(time, moveend);
                        delay += perDelay;
                        chip.runAction(cc.sequence(cc.delayTime(delay), cc.easeExponentialOut(to), cc.removeSelf()));
                    }
                }

            }
            this.otherBet[targetid] = bet;
        }
        Sound.getInstance().playEffect(getResPath("Sound/zjh/room_send_jinbi.mp3"));
    },

    sendWinBet: function (index) {
        var size = cc.director.getWinSize();
        var begin = RoomZJHConfig.getPlayerCenterPos(index);

        var perDelay = 0.500 / this.cnt;
        var max = this.cnt + 10;
        max -= 1;
        var delay = 0;
        for (var i = max; i >= 10; i--) {
            var tmp;
            tmp.x = Math.random(begin.x - 0, begin.x + 0);
            tmp.y = Math.random(begin.y - 0, begin.y + 0);

            delay += perDelay;
            var chip = this.getChildByTag(i);
            if (chip == null) {
                return;
            }
            var callback = cc.callFunc(this.chipCallBack, this);
            chip.runAction(cc.sequence(cc.delayTime(delay), cc.moveTo(0.5, tmp), callback));
        }
        Sound.getInstance().playEffect(getResPath("Sound/zjh/sound_jinbi.mp3"));
    },

    CallbackPerBet: function (index, targetid) {
        var begin = RoomZJHConfig.getOtherPlayerBetPos(index);
        var total_money = 0;
        for (var i = 0; i < 5; i++) {
            total_money += this.otherBet[i];
        }
        var bet = total_money;
        var name = getResPaht("Room/room_bet_jinbi.png");
        var chipnums = bet / 20000 * 5;
        if (chipnums <= 0 || chipnums > 40) {
            chipnums = 40;
        }
        var delay = 0;

        for (var i = 0; i < chipnums; i++) {
            var end = RoomZJHConfig.getPlayerCenterPos(targetid);
            var x = Math.random(-25, 25);
            var y = Math.random(-25, 25);

            end = cc.pAdd(end + cc.p(x, Y));
            var chip = new cc.Sprite(name);
            chip.setPosition(begin);
            chip.setTag(10000 + percnt);
            this.addChild(chip);
            chip.setOpacity(0);
            var to = cc.moveTo(0.5, end);
            var sto = cc.scaleTo(0.8, 0.7);
            var fto = cc.fadeTo(0.2, 255);
            var sp = cc.spawn(to, sto, fto);
            delay += 0.02;
            chip.runAction(cc.sequence(cc.delayTime(delay), sp));
            this.percnt++;
        }
    },

    sendPerWinBet: function (PerIndex, PerwinBet, otherbet, mywinbet, _total_money) {
        var total_money = 0;
        for (var i = 0; i < 5; i++) {
            total_money += this.otherBet[i];
        }
        cc.log(__String.createWithFormat("---totalmoney:%1,cnt:%2,size:%3", total_money, this.percnt, PerIndex.length));
        var tag = 0;
        //外围桌上赢取金币
        for (var i = 0; i < PerIndex.length; i++) {
            var bet = PerwinBet[i];
            var nums = ( bet / _total_money * this.percnt);
            cc.log(__String.createWithFormat("---pre[%1] =bet[%2],nums:%3", PerIndex[i], bet, nums));
            var delay = 0;
            var perDelay = 0.500 / nums;
            var end = RoomZJHConfig.getPerPlayerPos(PerIndex[i]);
            for (var j = 0; j < nums; j++) {
                var chip = this.getChildByTag(tag + 10000);
                if (chip == null) {
                    return;
                }
                var callback = cc.callFunc(this.chipCallBack, this);
                chip.runAction(cc.sequence(cc.delayTime(delay), cc.moveTo(0.2, end), callback));
                tag++;

                if (delay > 0.5) {
                    delay += 0.001;
                }
                else {
                    delay += perDelay;
                }

            }
        }

        //自己赢取金币
        var end = cc.p(142, 42);
        var nums = (mywinbet / _total_money * percnt);

        if (!RoomZJHInstance.getInstance().MyIsPer) {
            var delay = 0;
            if (nums > 0) {
                var perDelay = 0.500 / nums;
                for (var j = 0; j < nums; j++) {
                    var chip = this.getChildByTag(tag + 10000);
                    if (chip == null) {
                        return;
                    }
                    var callback = cc.callFunc(this.chipCallBack, this);
                    chip.runAction(cc.sequence(cc.delayTime(delay), cc.moveTo(0.3, end), callback));
                    tag++;
                    if (delay > 0.5) {
                        delay += 0.001;
                    }
                    else {
                        delay += perDelay;
                    }

                }
            }

        }

        //外围玩家不在桌上
        end = cc.p(75, 160);
        nums = this.percnt - tag;
        if (nums > 0) {
            cc.log(__String.createWithFormat("---oher[%1] =nums:%2", nums, nums));
            var delay = 0;
            var perDelay = 0.500 / nums;
            for (var j = 0; j < nums; j++) {

                var chip = this.getChildByTag(tag + 10000);
                if (!chip) {
                    return;
                }
                var callback = cc.callFunc(this.chipCallBack, this);
                chip.runAction(cc.sequence(cc.delayTime(delay), cc.moveTo(0.3, end), callback));
                tag++;
                if (delay > 0.5) {
                    delay += 0.004;
                }
                else {
                    delay += perDelay;
                }

            }
        }
        Sound.getInstance().playEffect(getResPath("Sound/zjh/sound_jinbi.mp3"));
    },

    chipCallBack: function (pSender) {
        var chip = pSender;
        chip.stopAllActions();
        chip.removeFromParentAndCleanup(true);

    },

    removeAllChips: function () {
        this.stopAllActions();
        this.cnt = 0;
        this.percnt = 0;
        this.deleteIndex = 0;
        this.removeAllChildren();
    }

});