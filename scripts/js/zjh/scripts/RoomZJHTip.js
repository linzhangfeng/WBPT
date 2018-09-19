var RoomZJHTip = cc.Layer.extend({

    HintDirection_Left: 50,
    HintDirection_Right: 51,

    TIME_BG: 20,
    TIME_ProgressTimer: 21,
    TIME_LABLE: 22,
    TIP_BG: 30,
    BET_TIP: 31,
    ROUND_TIP: 32,
    SIGLE_BET_TIP: 33,
    TOTAL_BET_NUMS: 34,
    CALL_MONEY_BG: 35,
    CALL_MONEY: 36,
    POT_NUMS: 37,
    TABLE_PLAYER_TOTAL: 60,


    chatnums: 0,
    chatItem: [[], [], []],
    chatTime: [[], [], []],
    m_readyTime: 0,

    TAG_BET_NODE: 10,
    TAG_TOTAL_BET: 21,
    TAG_CUR_BET: 22,
    TAG_ROUND: 23,
    TAG_READY_TIME: 30,

    HINT_RAISE: 0,
    HINT_CALL: 1,
    HINT_FOLD: 2,
    HINT_SEE: 3,
    HINT_CMP: 4,
    HINT_ALLIN: 5,
    HINT_MAX: 6,

    SEX_MAN: 1,
    SEX_FEMALE: 2,

    _listenerChat: null,

    ctor: function () {
        this._super();
        var size = cc.director.getWinSize();

        this.ignoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.5, 0.5));
        this.setContentSize(cc.size(size.width, size.height));

        //押注信息加在一个node下，方便调整位置
        var labelColor = cc.color(255, 247, 191);
        var labelFontSize = 22;
        var point = RoomZJHConfig.getTablePos();
        point.y += 400;
        var betNode = new cc.Node();
        betNode.setTag(this.TAG_BET_NODE);
        betNode.setPosition(point);
        this.addChild(betNode);

        var totalBetBg = new cc.Sprite(getResPath("Room/duizhan/total_bet_bg.png"));
        totalBetBg.setPosition(cc.p(0, 0));
        betNode.addChild(totalBetBg, -1);

        var totalBetIcon = new cc.Sprite(getResPath("Room/duizhan/total_bet_icon.png"));
        totalBetIcon.setPosition(cc.p(-70, 0));
        betNode.addChild(totalBetIcon);

        var totalBetLabel = new cc.LabelTTF("-", "Arial", labelFontSize);
        totalBetLabel.setAnchorPoint(cc.p(0.5, 0.5));
        totalBetLabel.setPosition(cc.p(20, 0));
        totalBetLabel.setColor(labelColor);
        totalBetLabel.setTag(this.TAG_TOTAL_BET);
        betNode.addChild(totalBetLabel);

        var curBetHead = new cc.LabelTTF("单注：", "Arial", labelFontSize);
        curBetHead.setAnchorPoint(cc.p(1, 0.5));
        curBetHead.setPosition(cc.p(-50, -40));
        curBetHead.setColor(labelColor);
        betNode.addChild(curBetHead);

        var curBetLabel = new cc.LabelTTF("-", "Arial", labelFontSize);
        curBetLabel.setAnchorPoint(cc.p(0, 0.5));
        curBetLabel.setPosition(curBetHead.getPosition());
        curBetLabel.setColor(labelColor);
        curBetLabel.setTag(this.TAG_CUR_BET);
        betNode.addChild(curBetLabel);

        var roundHead = new cc.LabelTTF("回合：", "Arial", labelFontSize);
        roundHead.setAnchorPoint(cc.p(1, 0.5));
        roundHead.setPosition(cc.p(70, curBetHead.getPositionY()));
        roundHead.setColor(labelColor);
        betNode.addChild(roundHead);

        var roundLabel = new cc.LabelTTF("-/-", "Arial", labelFontSize);
        roundLabel.setAnchorPoint(cc.p(0, 0.5));
        roundLabel.setPosition(roundHead.getPosition());
        roundLabel.setColor(labelColor);
        roundLabel.setTag(this.TAG_ROUND);
        betNode.addChild(roundLabel);

        var tablePos = RoomZJHConfig.getTablePos();
        var readyTime = new cc.LabelTTF("", "Arial", 32);
        readyTime.setPosition(cc.p(tablePos.x, tablePos.y - 160));
        readyTime.setTag(this.TAG_READY_TIME);
        this.addChild(readyTime);

        var self = this;
        this._listenerChat = cc.EventListener.create({
            event: cc.EventListener.CUSTOM,
            eventName: "notify_room_chat",
            callback: function (evt) {
                self.hasChatNotify(JsUtils.decodeBase64(evt.getUserData()));
                cc.log("notify_room_chat");
            }
        });
        cc.eventManager.addListener(this._listenerChat, this);
    },

    setPlayerBet: function (index, bet) {

    },

    setPotValue: function (nums) {
        var pot_money = this.getChildByTag(this.POT_NUMS);
        if (RoomZJHInstance.pool_flag == 0) {
            pot_money.setVisible(false);
            return;
        }

        if (pot_money != null) {
            var buf = __String.createWithFormat("%1", nums);
            pot_money.setString(buf);
            pot_money.setVisible(true);
            // if (nums >= 100000000) {
            //     char buff[100];
            //     Utils::parseCutNums(2, buf, nums/1000000,":");
            //     snprintf(buff, sizeof(buff), "%s<=",buf);
            //     pot_money->setString(buff);
            // }
            // else{
            //     Utils::parseCutNums(3, buf, nums,";");
            //     pot_money.setString(buf);
            // }
        }
    },

    setPotValueVisible: function (visible) {
        var pot_money = this.getChildByTag(this.POT_NUMS);
        if (pot_money != null) {
            pot_money.setVisible(visible);
        }
    },

    setTotalValue: function (value) {
        var betNode = this.getChildByTag(this.TAG_BET_NODE);
        if (betNode != null) {
            var totalBetLabel = betNode.getChildByTag(this.TAG_TOTAL_BET);
            totalBetLabel.setString(__String.createWithFormat("%1", value));
        }
    },

    setCurRound: function (round) {
        var betNode = this.getChildByTag(this.TAG_BET_NODE);
        if (betNode != null) {
            var curRoundLabel = betNode.getChildByTag(this.TAG_ROUND);
            curRoundLabel.setString(__String.createWithFormat("%1/%1", round, RoomZJHInstance.max_round));
        }
    },

    setCurBet: function (bet) {
        var betNode = this.getChildByTag(this.TAG_BET_NODE)
        if (betNode != null) {
            var curBetLabel = betNode.getChildByTag(this.TAG_CUR_BET);
            curBetLabel.setString(__String.createWithFormat("%1", bet));
        }
    },

    resetTipValue: function () {
        var betNode = this.getChildByTag(this.TAG_BET_NODE);
        if (betNode != null) {
            var totalBetLabel = betNode.getChildByTag(this.TAG_TOTAL_BET);
            totalBetLabel.setString("-");
        }
        betNode = this.getChildByTag(this.TAG_BET_NODE);
        if (betNode != null) {
            var curRoundLabel = betNode.getChildByTag(this.TAG_ROUND);
            curRoundLabel.setString("-/-");
        }

        betNode = this.getChildByTag(this.TAG_BET_NODE);
        if (betNode != null) {
            var curBetLabel = betNode.getChildByTag(this.TAG_CUR_BET);
            curBetLabel.setString("-");
        }
    },

    showTime: function (index, t) {
        this.curTime = t - 1;
        var size = cc.director.getWinSize();
        var cx = size.width / 2;
        var cy = size.height / 2 + 100;
        cx = RoomZJHConfig.getTablePos().x;
        cy = RoomZJHConfig.getTablePos().y + 20;
        if (index == -1) {
            var timeBg = this.getChildByTag(this.TIME_BG);
            if (timeBg != null) {
                timeBg.setVisible(true);
                var pt = timeBg.getChildByTag(this.TIME_ProgressTimer);
                var time_nums = timeBg.getChildByTag(this.TIME_LABLE);
                var to = cc.progressTo(t, 0);
                pt.setPercentage(100);
                pt.runAction(to);
                time_nums.setString(__String.createWithFormat("%1", this.curTime));
                this.schedule(this.updateTime, 1.0);
            }
        }
    },

    hideTime: function () {
        var timeBg = this.getChildByTag(this.TIME_BG);
        if (timeBg != null) {
            timeBg.setVisible(false);
        }
        this.unschedule(this.updateTime);
    },

    updateTime: function (dt) {
        var timeBg = this.getChildByTag(this.TIME_BG);
        var time_nums = this.timeBg.getChildByTag(this.TIME_LABLE);
        this.curTime--;
        if (this.curTime < 0) {
            timeBg.setVisible(false);
            return;
        }
        Sound.getInstance().playEffect(getResPath("Sound/zjh/clock_sec.mp3"));
        time_nums.setString(__String.createWithFormat("%1", curTime));
    },

    showReadyTime: function (time) {
        var readyTime = this.getChildByTag(this.TAG_READY_TIME);
        if (readyTime != null) {
            this.m_readyTime = time;
            readyTime.setString(__String.createWithFormat("距离下局...%1", this.m_readyTime));
            readyTime.setVisible(true);
            this.unschedule(this.updateReadyTime);
            this.schedule(this.updateReadyTime, 1.0);
        }
    },

    hideReadyTime: function () {
        var readyTime = this.getChildByTag(this.TAG_READY_TIME);
        if (readyTime != null) {
            readyTime.setVisible(false);
        }
        this.unschedule(this.updateReadyTime);
    },

    updateReadyTime: function (dt) {
        var readyTime = this.getChildByTag(this.TAG_READY_TIME);
        if (readyTime != null) {
            this.m_readyTime--;
            if (this.m_readyTime <= 0) {
                readyTime.setVisible(false);
                return;
            }
            readyTime.setString(__String.createWithFormat("距离下局...%1", this.m_readyTime));
        }
    },

    showDownAndUpAction: function (pos, type, delay) {
        //type:1离桌  2：上桌
        var pic = type == 1 ? getResPath("Room/effect/effectslizhuo.png") : getResPath("Room/effect/effectsshangzhuo.png");
        var effect_sp = new cc.Sprite(pic);
        var begin = RoomZJHConfig.getPerPlayerPos(pos);
        if (effect_sp != null) {
            effect_sp.setPosition(begin);
            this.addChild(effect_sp);
            effect_sp.setScale(0.7);
            var dele = 3;
            if (type == 1) {

                var sc = cc.scaleTo(dele, 0.4);
                var fa = cc.fadeTo(dele, 0);
                var ro = cc.rotateBy(dele, dele * 2 * 360);
                var sp = cc.spawn(sc, fa, ro);
                var seq = cc.sequence(cc.delayTime(delay), sp);
                effect_sp.runAction(seq);
            }
            else {
                var func = callFunc(function () {
                    var to = cc.scaleBy(0.5, 0.8);
                    var to1 = cc.scaleBy(0.5, 1.2);
                    var forev = cc.repeatForever(cc.sequence(to, to1));
                    effect_sp.runAction(forev);
                });

                var seq = cc.sequence(cc.delayTime(delay), func, cc.delayTime(dele), cc.removeSelf);
                effect_sp.runAction(seq);

            }
        }
    },

    showHint: function (index, flag) {
        var opStr = "";
        if (flag == this.HINT_SEE) {
            opStr = "看牌";
        }
        else if (flag == this.HINT_FOLD) {
            opStr = "弃牌";
        }
        else if (flag == this.HINT_ALLIN) {
            opStr = "全押";
        }
        else if (flag == this.HINT_CALL) {
            opStr = "跟注";
        }
        else if (flag == this.HINT_CMP) {
            opStr = "比牌";
        }
        else if (flag == this.HINT_RAISE) {
            opStr = "加注";
        }
        else if (flag == this.HINT_MAX) {
            opStr = "决斗";
        }

        var label = new cc.LabelTTF(opStr, "Arial", 26);
        var hint = null;
        var begin = RoomZJHConfig.getPlayerCenterPos(index);
        begin.y += 45;
        if (index == 0 || index == 3 || index == 4) {
            hint = new cc.Sprite(getResPath("Room/duizhan/op_tip_left.png"));
            hint.setAnchorPoint(cc.p(0, 0.5));
            label.setPosition(cc.p(63, 31));
            hint.addChild(label);

            begin.x += 42;
        } else {
            hint = new cc.Sprite(getResPath("Room/duizhan/op_tip_right.png"));
            hint.setAnchorPoint(cc.p(1, 0.5));
            label.setPosition(cc.p(50, 31));
            hint.addChild(label);

            begin.x -= 42;
        }

        hint.setPosition(begin);
        this.addChild(hint);
        hint.runAction(cc.sequence(cc.fadeIn(0.2),
            cc.delayTime(2.0),
            cc.removeSelf()));

    },


// -- showHint中hint显示的图片
    hintName: function (flag, name, hintDirection) {
        if (hintDirection == this.HintDirection_Left) {
            switch (flag) {
                case 0: {
                    name = getResPath("Room/room-tip-raise-left.png");
                    break;
                }
                case 1: {
                    name = getResPath("Room/room-tip-call-left.png");
                    break;
                }
                case 2: {
                    name = getResPath("Room/room-tip-fold-left.png");
                    break;
                }
                case 3: {
                    name = getResPath("Room/room-tip-see-left.png");
                    break;
                }
                case 4: {
                    name = getResPath("Room/room-tip-cmp-left.png");
                    break;
                }
                case 5: {
                    name = getResPath("Room/room-tip-allin-left.png");
                    break;
                }
                default: {
                    cc.log(__String.createWithFormat("RoomZJHTip hintName flag %1 is fail ", flag));
                    name = getResPath("Room/room-tip-raise-left.png");
                    break;
                }
            }
        } else if (hintDirection == this.HintDirection_Right) {
            switch (flag) {
                case 0: {
                    name = getResPath("Room/room-tip-raise-right.png");
                    break;
                }
                case 1: {
                    name = getResPath("Room/room-tip-call-right.png");
                    break;
                }
                case 2: {
                    name = getResPath("Room/room-tip-fold-right.png");
                    break;
                }
                case 3: {
                    name = getResPath("Room/room-tip-see-right.png");
                    break;
                }
                case 4: {
                    name = getResPath("Room/room-tip-cmp-right.png");
                    break;
                }
                case 5: {
                    name = getResPath("Room/room-tip-allin-right.png");
                    break;
                }
                default: {
                    cc.log(__String.createWithFormat("RoomZJHTip hintName flag %1 is fail ", flag));
                    name = getResPath("Room/room-tip-raise-right.png");
                    break;
                }
            }
        }
    },


    hintCallback: function (pSender) {
        pSender.removeAllChildrenWithCleanup(true);
        pSender.removeFromParentAndCleanup(true);
    },

    showFace: function (index, flag) {
        var begin = RoomZJHConfig.getPlayerCenterPos(index);
        var end;
        var buf;
        buf = __String.createWithFormat(getResPath("Room/face/game_face%1.png"), flag);
        cc.log(String("[%1][%2]", index, buf));
        var name = buf;
        var size = cc.director.getWinSize();
        var i = 115;
        var j = 30;

        var face = null;

        face = new cc.Sprite(name);
        end.setPoint(begin.x, begin.y + j);
        // -- modification wujunru 2014.04.14

        if (face == null) {
            return;
        }

        face.setPosition(begin);

        var move0 = cc.moveTo(0.6, end);
        var move1 = cc.moveTo(0.6, begin);
        var move2 = cc.moveTo(0.6, end);
        var move3 = cc.moveTo(0.6, begin);
        var fadeto = cc.fadeTo(0.3, 100);
        var callback = cc.callFunc(this.faceCallback, this);
        face.runAction(cc.sequence(move0, move1, move2, move3, fadeto, callback));
        this.addChild(face, 1000);
    },

    showBigFace: function (bpos, epos, fid, sex) {
        if (bpos == epos) {
            return;
        }

        var begin;
        var end;

        var face = null;
        begin = RoomZJHConfig.getPlayerCenterPos(bpos);
        end = RoomZJHConfig.getPlayerCenterPos(epos);

        var appear = null;
        var move = null;
        var disappear = null;

        switch (fid) {
            case 1: {
                face = new cc.Sprite(getResPath("Room/bigface/flower/flower1.png"));
                if (face != null) {
                    appear = cc.fadeIn(0.5);
                    move = cc.moveTo(0.5, end);

                    var disappearf = [2, 3, 4, 5, 6, 7, 8, 9, 10];
                    disappear = utils.getCCAnimate(getResPath("Room/bigface/flower/flower%1.png"), 9, disappearf, 0.2, false, false);
                }
                break;
            }
            case 2: {
                face = new cc.Sprite(getResPath("Room/bigface/cheers/cheers1.png"));
                if (face != null) {
                    appear = cc.fadeIn(0.5);
                    move = cc.moveTo(0.7, end);

                    var disappearf = [2, 3, 4, 5, 6, 7, 7, 8, 8, 9, 9, 10, 10];
                    disappear = utils.getCCAnimate(getResPath("Room/bigface/cheers/cheers%1.png"), 13, disappearf, 0.10, false, false);
                }
                break;
            }
            case 3: {
                face = new cc.Sprite(getResPath("Room/bigface/kiss/kiss1.png"));
                if (face != null) {
                    appear = cc.fadeIn(0.5);
                    move = cc.moveTo(0.5, end);

                    var disappearf = [2, 3, 3, 4, 5, 6, 7, 8, 9, 10];
                    disappear = utils.getCCAnimate(getResPath("Room/bigface/kiss/kiss%1.png"), 10, disappearf, 0.2, false, false);
                }
                break;
            }
            case 4: {
                face = new cc.Sprite(getResPath("Room/bigface/eggs/egg1.png"));
                if (face != null) {
                    {
                        end.y -= 30;
                    }
                    appear = cc.fadeIn(0.5);
                    move = cc.spawn(cc.rotateBy(0.5, 720), cc.moveTo(0.5, end));

                    var disappearf = [2, 2, 2, 3, 4, 5, 6];
                    disappear = utils.getCCAnimate(getResPath("Room/bigface/eggs/egg%1.png"), 7, disappearf, 0.2, false, false);
                }
                break;
            }
            case 5: {
                face = new cc.Sprite(getResPath("Room/bigface/shoe/shoe1.png"));
                if (face != null) {
                    {
                        end.y -= 10;
                    }
                    appear = cc.fadeIn(0.5);
                    move = cc.spawn(cc.rotateB(0.5, 360 + 360), cc.moveTo(0.5, end));
                    var disappearf = [2, 2, 2, 3, 4, 5, 6, 7, 8, 9];
                    disappear = utils.getCCAnimate(getResPath("Room/bigface/shoe/shoe%1.png"), 10, disappearf, 0.2, false, false);
                }

                break;
            }
            case 6: {
                face = new cc.Sprite(getResPath("Room/bigface/bomb/bomb1.png"));
                if (face != null) {
                    appear = cc.fadeIn(0.5);
                    move = cc.spawn(cc.rotateBy(0.5, 720), cc.moveTo(0.5, end));

                    var disappearf = [2, 3, 4, 2, 3, 4, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 11, 12, 12, 12, 13, 14, 15];
                    disappear = utils.getCCAnimate(getResPath("Room/bigface/bomb/bomb%1.png"), 24, disappearf, 0.1, false, false);
                }
                break;
            }
            default: {
                cc.log(__String.createWithFormat("RoomZJHTip showBigFace fid is %1 ", fid));
                break;
            }
        }

        if (fid > -1 && fid < 7) {
            if (face != null) {
                face.setPosition(begin);
                face.setOpacity(0);
                face.setTag(1000 + fid);
                this.bigFacePlayTalk(fid, sex);

                var callback = cc.callFunc(this.actionEnd, this);

                var playsoundcallback = sex == this.SEX_FEMALE ?
                    cc.callFunc(this.bigFacePlaySoundf, this) :
                    cc.callFunc(this.bigFacePlaySoundm, this);

                face.runAction(cc.sequence(appear, move, playsoundcallback, disappear, callback));
                this.addChild(face);
            }

        }
    },

    showPerBigFace: function (bpos, epos, fid, sex, type) {
        // 1://外围->外围  2://外围->玩家 3://玩家->外围
        if (type <= 0) {
            return;
        }

        var begin;
        var end;

        var face = null;
        if (type == 1) {
            begin = RoomZJHConfig.getPerPlayerPos(bpos);
            end = RoomZJHConfig.getPerPlayerPos(epos);
        }
        else if (type == 2) {
            begin = RoomZJHConfig.getPerPlayerPos(bpos);
            end = RoomZJHConfig.getPlayerCenterPos(epos);
        }
        else if (type == 3) {
            begin = RoomZJHConfig.getPlayerCenterPos(bpos);
            end = RoomZJHConfig.getPerPlayerPos(epos);
        }

        var appear = null;
        var move = null;
        var disappear = null;

        switch (fid) {
            case 1: {
                face = new cc.Sprite(getResPath("Room/bigface/flower/flower1.png"));
                if (face != null) {
                    appear = cc.fadeIn(0.5);
                    move = cc.moveTo(0.5, end);

                    var disappearf = [2, 3, 4, 5, 6, 7, 8, 9, 10];
                    disappear = utils.getCCAnimate(getResPath("Room/bigface/flower/flower%1.png"), 9, disappearf, 0.2, false, false);
                }
                break;
            }
            case 2: {
                face = new cc.Sprite(getResPath("Room/bigface/cheers/cheers1.png"));
                if (face != null) {
                    appear = cc.fadeIn(0.5);
                    move = cc.moveTo(0.7, end);

                    var disappearf = [2, 3, 4, 5, 6, 7, 7, 8, 8, 9, 9, 10, 10];
                    disappear = utils.getCCAnimate(getResPath("Room/bigface/cheers/cheers%1.png"), 13, disappearf, 0.10, false, false);
                }
                break;
            }
            case 3: {
                face = new cc.Sprite(getResPath("Room/bigface/kiss/kiss1.png"));
                if (face != null) {
                    appear = cc.fadeIn(0.5);
                    move = cc.moveTo(0.5, end);

                    var disappearf = [2, 3, 3, 4, 5, 6, 7, 8, 9, 10];
                    disappear = utils.getCCAnimate(getResPath("Room/bigface/kiss/kiss%1.png"), 10, disappearf, 0.2, false, false);
                }
                break;
            }
            case 4: {
                face = new cc.Sprite(getResPath("Room/bigface/eggs/egg1.png"));
                if (face != null) {
                    {
                        end.y -= 30;
                    }
                    appear = cc.fadeIn(0.5);
                    move = cc.spawn(cc.rotateBy(0.5, 720), cc.moveTo(0.5, end));

                    var disappearf = [2, 2, 2, 3, 4, 5, 6];
                    disappear = utils.getCCAnimate(getResPath("Room/bigface/eggs/egg%1.png"), 7, disappearf, 0.2, false, false);
                }
                break;
            }
            case 5: {
                face = new cc.Sprite(getResPath("Room/bigface/shoe/shoe1.png"));
                if (face != null) {
                    {
                        end.y -= 10;
                    }
                    appear = cc.fadeIn(0.5);
                    move = cc.spawn(cc.rotateBy(0.5, 360 + 360), cc.moveTo(0.5, end));
                    var disappearf = [2, 2, 2, 3, 4, 5, 6, 7, 8, 9];
                    disappear = utils.getCCAnimate(getResPath("Room/bigface/shoe/shoe%1.png"), 10, disappearf, 0.2, false, false);
                }

                break;
            }
            case 6: {
                face = new cc.Sprite(getResPath("Room/bigface/bomb/bomb1.png"));
                if (face != null) {
                    appear = cc.fadeIn(0.5);
                    move = cc.spawn(cc.rotateBy(0.5, 720), cc.moveTo(0.5, end));

                    var disappearf = [2, 3, 4, 2, 3, 4, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 11, 12, 12, 12, 13, 14, 15];
                    disappear = utils.getCCAnimate(getResPath("Room/bigface/bomb/bomb%1.png"), 24, disappearf, 0.1, false, false);
                }
                break;
            }
            default: {
                cc.log(__String.createWithFormat("RoomZJHTip showBigFace fid is %1 ", fid));
                break;
            }
        }

        if (fid > -1 && fid < 7) {
            if (face != null) {
                face.setPosition(begin);
                face.setOpacity(0);
                face.setTag(1000 + fid);
                this.bigFacePlayTalk(fid, sex);

                var callback = cc.callFunc(this.actionEnd, this);

                var playsoundcallback = sex == this.SEX_FEMALE ?
                    cc.callFunc(this.bigFacePlaySoundf, this) :
                    cc.callFunc(this.bigFacePlaySoundm, this);

                face.runAction(cc.sequence(appear, move, playsoundcallback, disappear, callback));
                this.addChild(face);
            }

        }
    },

    bigFacePlayTalk: function (faceid, sex) {
        var buf;
        if (faceid == 1) {
            buf = getResPath(__String.createWithFormat("Sound/bigface/%1_xianhua.mp3", sex == SEX_FEMALE ? "F" : "M"));
            Sound.getInstance().playEffect(buf);
        }
        else if (faceid == 2) {
            buf = getResPath(__String.createWithFormat("Sound/bigface/%1_pijiu.mp3", sex == SEX_FEMALE ? "F" : "M"));
            Sound.getInstance().playEffect(buf);
        }
        else if (faceid == 3) {
            buf = getResPath(__String.createWithFormat("Sound/bigface/%1_kiss.mp3", sex == SEX_FEMALE ? "F" : "M"));
            Sound.getInstance().playEffect(buf);
        }
        else if (faceid == 4) {
            buf = getResPath(__String.createWithFormat("Sound/bigface/%1_jidan.mp3", sex == SEX_FEMALE ? "F" : "M"));
            Sound.getInstance().playEffect(buf);
        }
        else if (faceid == 5) {
            buf = getResPath(__String.createWithFormat("Sound/bigface/%1_tuoxie.mp3", sex == SEX_FEMALE ? "F" : "M"));
            Sound.getInstance().playEffect(buf);
        }
        else if (faceid == 6) {
            buf = getResPath(__String.createWithFormat("Sound/bigface/%1_zhadan.mp3", sex == SEX_FEMALE ? "F" : "M"));
            Sound.getInstance().playEffect(buf);
        }
    },

    bigFacePlaySoundf: function (pSender) {
        var faceid = pSender.getTag();
        faceid -= 1000;
        if (faceid == 1) {
            Sound.getInstance().playEffect(getResPath("Sound/bigface/xianhua.mp3"));
        }
        else if (faceid == 2) {
            Sound.getInstance().playEffect(getResPath("Sound/bigface/pijiu.mp3"));
        }
        else if (faceid == 3) {
            Sound.getInstance().playEffect(getResPath("Sound/bigface/f_wen.mp3"));
        }
        else if (faceid == 4) {
            Sound.getInstance().playEffect(getResPath("Sound/bigface/jidan.mp3"));
        }
        else if (faceid == 5) {
            Sound.getInstance().playEffect(getResPath("Sound/bigface/tuoxie.mp3"));
        }
        else if (faceid == 6) {
            Sound.getInstance().playEffect(getResPath("Sound/bigface/zhadan.mp3"));
        }
    },

    bigFacePlaySoundm: function (pSender) {
        var faceid = pSender.getTag();
        faceid -= 1000;

        if (faceid == 1) {
            Sound.getInstance().playEffect(getResPath("Sound/bigface/xianhua.mp3"));
        }
        else if (faceid == 2) {
            Sound.getInstance().playEffect(getResPath("Sound/bigface/pijiu.mp3"));
        }
        else if (faceid == 3) {
            Sound.getInstance().playEffect(getResPath("Sound/bigface/m_wen.mp3"));
        }
        else if (faceid == 4) {
            Sound.getInstance().playEffect(getResPath("Sound/bigface/jidan.mp3"));
        }
        else if (faceid == 5) {
            Sound.getInstance().playEffect(getResPath("Sound/bigface/tuoxie.mp3"));
        }
        else if (faceid == 6) {
            Sound.getInstance().playEffect(getResPath("Sound/bigface/zhadan.mp3"));
        }
    },

    actionEnd: function (pSender) {
        pSender.stopAllActions();
        pSender.removeAllChildrenWithCleanup(true);
        pSender.removeFromParentAndCleanup(true);
    },

    faceCallback: function (pSender) {
        pSender.removeAllChildrenWithCleanup(true);
        pSender.removeFromParentAndCleanup(true);
    },


    showPlayerWinType: function (pos, type) {
        var begin;
        if (pos == -1) {
            begin = RoomZJHConfig.getOutSidePos();
        }
        else {
            begin = RoomZJHConfig.getPlayerPos(pos);
        }
    },

    showPerPlayerWinType: function (pos, type) {

    },

    GetChatTime: function (node, index) {
        var size = cc.director.getWinSize();
        var deley = 0;
        var time = 0;
        var curx = node.getPositionX();
        var beginx = this.GetChatPos(chatnums).x + node.getContentSize().width;
        var dx = beginx - curx;
        if (dx <= 0) {
            deley = 3;
        }
        else if (dx >= size.width / 2) {
            deley = 0;
        }
        else {
            deley = 2;
        }
        time = this.chatTime[index - 1] + deley;
        return time;
    },

    showCardTypeTips: function (pos, card_type, delay) {
        if (pos != 0) {//现在只会看自己的牌
            return;
        }
        if (card_type < 0 || card_type > 9) {
            return;
        }
        var begin = RoomZJHConfig.getCardPos(0, 1);
        begin.y -= 72;
        var typeLabel = this.getChildByTag(RoomZJHConfig.tag_cardType_Tips);
        if (typeLabel) {
            typeLabel.removeFromParentAndCleanup(true);
            typeLabel = null;
        }
        if (RoomZJHInstance.curGameType == 0) {
            typeLabel = new cc.LabelTTF(RoomZJHInstance.getCardTypeString(card_type), "Arial", 32);
        } else {
            typeLabel = new cc.LabelTTF(RoomZJHInstance.getSetCardTypeString(card_type), "Arial", 32);
        }

        typeLabel.setPosition(begin);
        typeLabel.setColor(cc.color(255, 253, 152));
        typeLabel.setTag(RoomZJHConfig.tag_cardType_Tips);
        this.addChild(typeLabel);
    },

    HideCardTypeTips: function (pos) {
        this.removeChildByTag(RoomZJHConfig.tag_cardType_Tips + pos);
    },

    HideAllCardTypeTips: function () {
        for (var i = 0; i < 5; i++) {
            var cardTypeBG = this.getChildByTag(RoomZJHConfig.tag_cardType_Tips + i);
            if (cardTypeBG != null) {
                cardTypeBG.setVisible(false);
            }
        }
    },


    chatCallback: function (pSender) {
        pSender.removeAllChildrenWithCleanup(true);
        pSender.removeFromParentAndCleanup(true);
    },

    showBox: function (pos, type, value) {

        var mpos = RoomZJHConfig.getPlayerCenterPos(pos);
        var size = cc.director.getWinSize();
        varsp1 = new cc.Sprite(getResPath("Room/box/box.png"));
        sp1.setScale(1.7);
        sp1.setPosition(cc.p(mpos.x, mpos.y - 20));
        this.addChild(sp1);

        var sp2 = new cc.Sprite(getResPath("Room/box/boxopen.png"));
        sp2.setAnchorPoint(cc.p(0.5, 0));
        sp2.setVisible(false);
        sp2.setPosition(cc.p(mpos.x, mpos.y - 60));
        sp2.setScaleY(0);
        this.addChild(sp2);

        var buf;
        var buf2;

        buf = __String.createWithFormat(":%1", value);
        if (type == prop_type_rmb) {
            buf2 = __String.createWithFormat("%1", ("Common/zuanshi.png"));
        }
        else if (type == prop_type_money) {
            buf2 = __String.createWithFormat("%1", ("Common/jinbi.png"));
        }
        else if (type == prop_type_coin) {
            buf2 = __String.createWithFormat("%1", ("Common/duijiangquan.png"));
        }
        else if (type == prop_type_sibeika) {
            buf2 = __String.createWithFormat("%1", ("Common/sibeika.png"));
        }
        else if (type == prop_type_jinbika) {
            buf2 = __String.createWithFormat("%1", ("Common/jinbika.png"));
        }
        else if (type == prop_type_huanpaika) {
            buf2 = __String.createWithFormat("%1", ("Common/huanpaika.png"));
        }
        else {
            return;
        }

        var l = new cc.LabelAtlas(buf, "Common/nums.png", 38, 46, '0');
        l.setAnchorPoint(cc.p(0.0, 0.5));
        this.addChild(l);
        l.setVisible(false);
        l.setScale(0.5);


        varsp = new cc.Sprite(buf2);
        sp.setAnchorPoint(cc.p(0.0, 0.5));
        this.addChild(sp);
        sp.setScale(0.5);
        sp.setVisible(false);


        var ln = l.boundingBox().size.width + sp.boundingBox().size.width;

        sp.setPosition(cc.p(mpos.x - ln / 2, mpos.y + 20));
        l.setPosition(cc.p(sp.getPositionX() + sp.boundingBox().size.width, mpos.y + 20));

        sp1.runAction(cc.sequence(cc.repeat(cc.sequence(cc.rotateT(0.05, 5.0), cc.rotateTo(0.05, 0.0), cc.rotateTo(0.05, -5.0), cc.rotateTo(0.05, 0.0)), 5), cc.hide(), cc.callFunc(this.actionEnd, this)));
        sp2.runAction(cc.sequence(cc.delayTime(1), cc.show(), cc.scaleTo(0.05, 1.5, 1.5), cc.delayTime(1.5), cc.callFunc(this.actionEnd, this)));
        sp.runAction(cc.sequence(cc.delayTime(1.2), cc.show(), cc.sequence(cc.moveBy(1.0, cc.p(0, 100)), cc.spawn(cc.moveBy(0.5, cc.p(0, 50)), cc.fadeOut(0.5))), cc.callFunc(this.actionEnd, this)));
        l.runAction(cc.sequence(cc.delayTime(1.2), cc.show(), cc.sequence(cc.moveBy(1.0, cc.p(0, 100)), cc.spawn(cc.moveBy(0.5, cc.p(0, 50)), cc.fadeOut(0.5))), cc.callFunc(this.actionEnd, this)));
    },

    hasChatNotify: function (msg) {
        if (msg == null) {
            return;
        }

        var str = msg;

        var json;
        try {
            json = JSON.parse(str);
        }
        catch (e) {
            return;
        }

        var uid = utils.getValInt(json, "uid");
        var name = utils.getValStr(json, "name");
        var content = utils.getValStr(json, "content");

        var seatid = -1;
        for (var i = 0; i < 5; i++) {
            var p = RoomZJHInstance.players[i];
            if (p.uid == uid) {
                seatid = i;
                break;
            }
        }

        if (seatid != -1) {
            this.showChat(RoomZJHInstance.getPosBySeatid(seatid), content);
            var filePath = utils.getValStr(json, "filepath");
            if (filePath != null && filePath != "") {
                SoundModel.getInstance().addSound(filePath);
            }
        }

        if (seatid != -1) {
            var p = RoomZJHInstance.players[seatid];
            var chat_sound = Utils.getFastChatSound(content);
            if (chat_sound != "") {
                Sound.getInstance().playEffect(__String.createWithFormat("Sound/fastchat/%s_%s.mp3", p.sex == this.SEX_FEMALE ? "w" : "m", chat_sound));
            }
        }
    },

    showChat: function (pos, content) {
        var text = new cc.LabelTTF(content, "AmericanTypewriter", 30);
        text.setColor(cc.color.BLACK);

        var chat_bg = new cc.Scale9Sprite("Chat/bar_txt.png");
        chat_bg.setPreferredSize(cc.size(50 + text.getContentSize().x, 20 + text.getContentSize().y));
        this.addChild(chat_bg, -100);

        text.setAnchorPoint(cc.p(0.5, 0.5));
        text.setPosition(cc.p(chat_bg.getContentSize().width/2, chat_bg.getContentSize().height/2));
        chat_bg.addChild(text);

        var p = RoomZJHConfig.getPlayerPos(pos);
        if (pos == 0) {
            chat_bg.setAnchorPoint(cc.p(0.5, 0.5));
            chat_bg.setPosition(p + cc.p(0, 100));
        }
        else if (pos == 1 || pos == 2) {
            chat_bg.setAnchorPoint(cc.p(1.0, 0.5));
            chat_bg.setPosition(p + cc.p(-50, 50));
        }
        else {
            chat_bg.setAnchorPoint(cc.p(0.0, 0.5));
            chat_bg.setPosition(p + cc.p(50, 50));
        }

        chat_bg.runAction(cc.sequence(cc.delayTime(3), cc.removeSelf()));
    },

    ShowMoveChat: function (uid, name, content) {
        var size = cc.director.getWinSize();
        var container = new cc.Sprite();
        this.addChild(container, 100);
        var totalWidth = 0;

        var contentLabel = new cc.LabelTTF(content, "Arial", 24);
        contentLabel.setColor(cc.color.WHITE);
        contentLabel.setAnchorPoint(cc.p(0, 0));
        container.addChild(contentLabel);
        totalWidth += contentLabel.getContentSize().width;

        var nameLabel = new cc.LabelTTF(name + ":", "Arial", 24);
        nameLabel.setColor(cc.color(181, 243, 255));
        nameLabel.setAnchorPoint(cc.p(0, 0));
        nameLabel.setPosition(cc.p(0, 0));
        container.addChild(nameLabel);
        totalWidth += nameLabel.getContentSize().width;

        contentLabel.setPosition(cc.p(nameLabel.getContentSize().width, 0));

        if (ZJHModel.getInstance().uid == uid) {
            nameLabel.setColor(cc.color.GREEN);
        }
        else {
            nameLabel.setColor(cc.color(181, 243, 255));
        }

        var begin = cc.p(winSize.width, Math.random(winSize.height * 0.7, winSize.height * 0.9));
        container.setPosition(begin);
        container.runAction(cc.sequence(cc.moveBy(6, cc.p(-winSize.width - totalWidth, 0)), cc.removeSelf()));
    },

    GetChatPos: function (nums) {
        var size = cc.director.getWinSize();
        var p = cc.p(size.width, size.height - 50);
        if (nums == 1) {
            p = cc.p(size.width, size.height - 130);
        }
        else if (nums == 2) {
            p = cc.p(size.width, size.height / 2);
        }
        else if (nums == 3) {
            p = cc.p(size.width, 200);
        }
        return p;
    },

    onExit: function () {
        this._super();
        if (this._listenerChat != null) {
            cc.eventManager.removeListener(this._listenerChat);
        }
    }
});