var RoomZJHPlayer = cc.Layer.extend({
    LIFE_BG: 100,

    m_pName: null,
    m_pMoney: null,
    m_size: null,
    m_pAvatar: null,
    spReady: null,

    pid: 0,
    uid: 0,
    seatid: 0,
    sec: 0,
    callback: null,
    clickCallback: null,
    curSound: 0,
    isLogout: 0,
    total_time: 0,

    TAG_MASK: 19,
    TAG_WIN_FRAME: 20,
    TAG_WIN_ANIMATE: 21,
    TAG_COMPARE_WIN_FRAME: 30,
    TAG_COMPARE_LOSE_FRAME: 31,

    TAG_THING_MASK: 41,
    TAG_THING2_MASK: 42,
    TAG_LOST_MASK: 43,
    TAG_LOST2_MASK: 44,

    ctor: function () {
        this._super();

        this.ignoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.5, 0.5));
    },

    login: function (seatid) {
        this.seatid = seatid;
        this.isLogout = false;
        var player = RoomZJHInstance.players[seatid];
        if (player == null) {
            return;
        }
        this.uid = player.uid;
        var buf = "";

        var bgBtn = this.getChildByTag(5);
        if (this.uid == ZJHModel.getInstance().uid) {
            bgBtn.loadTextureNormal(getResPath("Room/player_bg_me.png"));
        } else {
            bgBtn.loadTextureNormal(getResPath("Room/player_bg_normal.png"));
        }

        buf = player.name;
        this.m_pName.setString(buf);

        buf = player.money;
        this.m_pMoney.setString(buf);

        //头像
        var imageUrl = player.avatar;
        this.m_pAvatar = utils.createCircleAvatar(imageUrl, "Avatars/gg.png", getResPath("Room/duizhan/dz_player_avatar_bg.png"), cc.size(90, 90));
        this.m_pAvatar.setPosition(cc.p(this.getContentSize().width / 2, this.getContentSize().height / 2));
        this.addChild(this.m_pAvatar);

        this.setVisible(true);
    },

    logout: function () {
        this.isLogout = true;
        this.uid = 0;
        this.setVisible(false);
    },
    reset: function () {
        this.uid = 0;
        this.stopTimer();
        this.setMaskVisible(false);
        this.stopWinEffect();
        this.setReadyVisible(false);
    },

    setId: function (id) {
        this.pid = id;
        var btn = new ccui.Button(getResPath("Room/duizhan/dz_player_bg.png"));
        btn.setZoomScale(0);
        btn.setPosition(cc.p(btn.getContentSize().width/2, btn.getContentSize().height/2));
        btn.setTag(5);
        btn.addClickEventListener(this.playerClickCallback);
        this.addChild(btn, -1);

        var grayMask = new cc.Sprite(getResPath("Room/duizhan/dz_player_gray.png"));
        grayMask.setPosition(btn.getPosition());
        grayMask.setTag(this.TAG_MASK);
        grayMask.setVisible(false);
        this.addChild(grayMask, 10);

        var THINKMask = new cc.Sprite(getResPath("RoomDN/thing1.png"));
        THINKMask.setPosition(cc.p(btn.getPositionX(), btn.getPositionY() - 16));
        THINKMask.setTag(this.TAG_THING_MASK);
        THINKMask.setVisible(false);
        this.addChild(THINKMask, 1000);


        var LOSTMask = new cc.Sprite(getResPath("RoomDN/lost1.png"));
        LOSTMask.setPosition(cc.p(btn.getPositionX(), btn.getPositionY() - 16));
        LOSTMask.setTag(this.TAG_LOST_MASK);
        LOSTMask.setVisible(false);
        this.addChild(LOSTMask, 1000);

        this.m_size = btn.getContentSize();
        this.setContentSize(this.m_size);

        this.m_pName = new cc.LabelTTF("name", "Thonburi", 20);
        this.m_pName.setPosition(cc.p(this.m_size.width / 2, this.m_size.height - 18));
        this.addChild(this.m_pName);

        this.m_pMoney = new cc.LabelTTF("0", "Thonburi", 20);
        this.m_pMoney.setPosition(cc.p(this.m_size.width / 2, 20));
        this.m_pMoney.setColor(cc.color(255, 255, 0));
        this.addChild(this.m_pMoney);

        // var avatarBg = new cc.Sprite(getResPath("Room/duizhan/dz_player_avatar_bg.png"));
        // avatarBg.setPosition(this.m_size / 2);
        // this.addChild(avatarBg);

        //倒计时
        var pt = new cc.ProgressTimer(new cc.Sprite(getResPath("Room/duizhan/dz_order_tip.png")));
        pt.setType(cc.ProgressTimer.TYPE_RADIAL);
        pt.setReverseDirection(true);
        pt.setPercentage(0);
        pt.setAnchorPoint(cc.p(0.5, 0.5));
        pt.setPosition(cc.p(this.m_size.width/2, this.m_size.height/2));
        pt.setTag(9);
        this.addChild(pt, 100);

        //准备
        this.spReady = new cc.Sprite(getResPath("Room/duizhan/v_ready.png"));
        this.spReady.setPosition(cc.p(this.getContentSize().width/2, this.getContentSize().height/2));
        this.spReady.setVisible(false);
        this.addChild(this.spReady, 1000);
    },

    startTimer: function (alltime, second) {
        this.sec = second - 1;
        this.total_time = alltime;
        var curper = second * 100 / alltime;

        var to = cc.progressTo(sec, 0);
        var cf = cc.callFun(this.timeoutCallback, this);

        var pt = this.getChildByTag(9);
        pt.setPercentage(curper);
        pt.runAction(cc.sequence(to, cf));

        this.schedule(this.timerUpdate, 1.0);
    },

    stopTimer: function () {
        var pt = this.getChildByTag(9);
        pt.setPercentage(0);
        pt.stopAllActions();
        this.unschedule(this.timerUpdate);
    },

    timerUpdate: function (dt) {
        if (this.uid == ZJHModel.getInstance().uid) {
            if (this.sec <= 5) {
                if (this.sec == 5) {
                    PlatformHelper.vibrate();
                }
                Sound.getInstance().playEffect(getResPath("Sound/zjh/overtime.mp3"));
            }
        }
        this.sec--;
    },


    timeoutCallback: function () {
        if (this.callback != null) {
            this.callback(this.pid);
        }
        this.stopTimer();
        Sound.getInstance().playEffect(getResPath("Sound/zjh/time_over.mp3"));
    },

    playerClickCallback: function (ref) {
        cc.log(__String.createWithFormat("playerClickCallback pid[%1]uid[%2]seatid[%3]", this.pid, this.uid, this.seatid));
        if (this.uid > 0) {
            if (this.clickCallback) {
                this.clickCallback(this.pid);
            }
        }
    },

    setCallback: function (callback) {
        this.callback = callback;
    },

    setClickCallback: function (callback) {
        this.clickCallback = callback;
    },

    showWinEffect: function () {
        var winFrame = this.getChildByTag(this.TAG_WIN_FRAME);
        if (winFrame == null) {
            winFrame = new cc.Sprite(getResPath("Room/duizhan/dz_win_player_bg.png"));
            winFrame.setTag(this.TAG_WIN_FRAME);
            winFrame.setPosition(cc.p(this.getContentSize().width / 2, this.getContentSize().height / 2 + 10));
            this.addChild(winFrame, 10);
        }

        var winAnimate = this.getChildByTag(this.TAG_WIN_ANIMATE)
        if (winAnimate == null) {
            winAnimate = new cc.Sprite(getResPath("Room/duizhan/v_win_user_anim_1.png"));
            winAnimate.setPosition(this.getContentSize() / 2);
            winAnimate.setTag(this.TAG_WIN_ANIMATE);
            this.addChild(winAnimate, 11);

            var animation = new cc.Animation();
            for (var i = 1; i <= 5; i++) {
                animation.addSpriteFrameWithFile(__String.createWithFormat("Room/duizhan/v_win_user_anim_%1.png", i));
            }
            animation.setDelayPerUnit(0.1);
            winAnimate.runAction(cc.repeatForever(cc.animate(animation)));
        }

        winFrame.setVisible(true);
        winAnimate.setVisible(true);
        this.runAction(cc.sequence(cc.delayTime(1.5), cc.callFunc(this.stopWinEffect, this)));
    },

    stopWinEffect: function () {
        var winFrame = this.getChildByTag(this.TAG_WIN_FRAME);
        if (winFrame != null) {
            winFrame.setVisible(false);
        }
        var winAnimate = this.getChildByTag(this.TAG_WIN_ANIMATE);
        if (winAnimate != null) {
            winAnimate.setVisible(false);
        }
    },
    showBalanceNumber: function (value) {
        if (value == 0) {
            return;
        }

        var buff = "";
        var label = null;
        if (value > 0) {
            buff = __String.createWithFormat(":%1", value);
            label = new cc.LabelAtlas(buff, getResPath("Room/duizhan/dz_balance_num_win.png"), 28, 40, '0');
        } else {
            buff = __String.createWithFormat(":%1", -value);
            label = new cc.LabelAtlas(buff, getResPath("Room/duizhan/dz_balance_num_lose.png"), 28, 40, '0');
        }
        label.setAnchorPoint(cc.p(0.5, 0.5));
        label.setPosition(cc.p(this.getContentSize().width / 2, this.getContentSize().height - 10));
        this.addChild(label, 200);

        label.runAction(cc.sequence(cc.moveBy(0.5, cc.p(0, 30)), cc.removeSelf()));
    },

    showCompareWin: function () {
        var compareWinFrame = this.getChildByTag(this.TAG_COMPARE_WIN_FRAME);
        if (compareWinFrame == null) {
            compareWinFrame = new cc.Sprite(getResPath("Room/duizhan/dz_pk_win_tip.png"));
            compareWinFrame.setPosition(this.getContentSize() / 2);
            compareWinFrame.setTag(this.TAG_COMPARE_WIN_FRAME);
            this.addChild(compareWinFrame, 100);
        }
        compareWinFrame.setVisible(true);

        var perTime = 1.0 / 12.0;
        this.setScale(1.08);

        var scaleAction = cc.sequence(cc.scaleTo(perTime * 3, 1.18),
            cc.scaleTo(perTime * 3, 1.08));
        var self = this;
        this.runAction(cc.sequence(cc.repeat(scaleAction, 3), cc.callFunc(function () {
            self.setScale(1.0);
            compareWinFrame.setVisible(false);
        })));
    },

    showCompareLose: function () {
        var compareLoseFrame = this.getChildByTag(this.TAG_COMPARE_LOSE_FRAME);
        if (compareLoseFrame == null) {
            compareLoseFrame = new cc.Sprite(getResPath("Room/duizhan/dz_pk_lose_tip.png"));
            compareLoseFrame.setPosition(this.getContentSize() / 2);
            compareLoseFrame.setTag(this.TAG_COMPARE_LOSE_FRAME);
            this.addChild(compareLoseFrame, 100);
        }
        compareLoseFrame.setVisible(true);

        var perTime = 1.0 / 12.0;
        this.setScale(0.93);

        var self = this;
        this.runAction(cc.sequence(cc.scaleTo(perTime * 3, 0.93), cc.callFunc(function () {
            self.setScale(1.0);
            compareLoseFrame.setVisible(false);
            self.setMaskVisible(true);
        })));
    },

    updateInfo: function () {
        var player = RoomZJHInstance.players[seatid];
        if (player == null) {
            return;
        }

        // char buf[128];
        // {
        //     Utils::parseNums1(buf,player->money);
        //     m_pMoney->setString(buf);
        // }
        //todo format money ?????
        cc.log("updateInfo: player=" + player);
        this.m_pMoney.setString(player.money);
    },

    setMaskVisible: function (visible) {
        var mask = this.getChildByTag(this.TAG_MASK);
        if (mask != null) {
            mask.setVisible(visible);
        }
    },


    setReadyVisible: function (visible) {
        //设置准备状态是否可见
        if (this.spReady != null) {
            this.spReady.setVisible(visible);
        }
    },

    setThinkVisible: function (visible) {
        var mask1 = this.getChildByTag(this.TAG_THING_MASK);
        if (mask1 != null) {
            mask1.setVisible(visible);
        }
    },

    setLostVisible: function (visible) {
        var mask1 = this.getChildByTag(this.TAG_LOST_MASK);
        if (mask1 != null) {
            mask1.setVisible(visible);
        }
    }
});