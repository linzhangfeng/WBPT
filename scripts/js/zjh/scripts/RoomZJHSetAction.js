var RoomZJHSetAction = cc.Layer({
    callback: null,
    raiseCallback: null,
    compareCallback: null,
    leftTime: 0,
    showTime: 0,
    m_layoutType: 0,
    _flag: false,
    m_pFold: null,
    m_pCompare: null,
    m_pRaise: null,
    m_pMax: null,
    m_pCall: null,
    m_pAuto: null,
    TAG_BUTTON_NUMBER: 5,
    TAG_SP_ANIMATE: 8,
    TAG_RAISE_BEGIN: 10,
    TAG_COMPARE_BEGIN: 20,
    TAG_READY_BTN: 30,

    setButtonLayoutType: function (m_layoutType) {
        this.m_layoutType = m_layoutType;
    },

    getButtonLayoutType: function () {
        return this.m_layoutType;
    },

    setButtonflag: function (_flag) {
        this._flag = _flag;
    },

    getButtonflag: function () {
        return this._flag;
    },

    ctor: function () {

        this._super();

        this.m_layoutType = -1;
        this.callback = null;
        this.raiseCallback = null;
        this.compareCallback = null;
        this.leftTime = 5;
        this.showTime = 20;

        var self = this;

        //弃牌
        this.m_pFold = new ccui.Button(getResPath("Room/duizhan/dz_fold_1.png"), getResPath("Room/duizhan/dz_fold_2.png"), getResPath("Room/duizhan/dz_disable.png"));
        this.m_pFold.setPosition(RoomZJHConfig.getPlayerPos(0));
        this.m_pFold.setVisible(false);
        this.m_pFold.setTag(RoomZJHConfig.tag_fold);
        this.m_pFold.addClickEventListener(function (sender) {
            self.buttonClickCallback(sender);
        });
        this.addChild(this.m_pFold);
        var foldTip = new cc.Sprite(getResPath("Room/duizhan/dz_fold_tip.png"));
        foldTip.setPosition(cc.p(66, -5));
        this.m_pFold.addChild(foldTip, 1);

        //比牌
        this.m_pCompare = new ccui.Button(getResPath("Room/duizhan/dz_compare_1.png"), getResPath("Room/duizhan/dz_compare_2.png"), getResPath("Room/duizhan/dz_disable.png"));
        this.m_pCompare.setPosition(RoomZJHConfig.getPlayerPos(0));
        this.m_pCompare.setVisible(false);
        this.m_pCompare.setTag(RoomZJHConfig.tag_compare);
        this.m_pCompare.addClickEventListener(function (sender) {
            self.buttonClickCallback(sender);
        });
        this.addChild(this.m_pCompare);
        var compareTip = new cc.Sprite(getResPath("Room/duizhan/dz_compare_tip.png"));
        compareTip.setPosition(cc.p(66, -5));
        this.m_pCompare.addChild(compareTip, 1);
        var numLabel = new cc.LabelAtlas("", getResPath("Room/duizhan/dz_bet_num.png"), 16, 25, '0');
        numLabel.setTag(this.TAG_BUTTON_NUMBER);
        numLabel.setAnchorPoint(cc.p(0.5, 0.5));
        numLabel.setPosition(cc.p(this.m_pCompare.getContentSize().width / 2, this.m_pCompare.getContentSize().height / 2));
        this.m_pCompare.addChild(numLabel, 2);

        //加注
        this.m_pRaise = new ccui.Button(getResPath("Room/duizhan/dz_raise_1.png"), getResPath("Room/duizhan/dz_raise_2.png"), getResPath("Room/duizhan/dz_raise_btn_enable.png"));
        this.m_pRaise.setPosition(RoomZJHConfig.getPlayerPos(0));
        this.m_pRaise.setVisible(false);
        this.m_pRaise.setTag(RoomZJHConfig.tag_raise);
        this.m_pRaise.addClickEventListener(function (sender) {
            self.buttonClickCallback(sender);
        });
        this.addChild(this.m_pRaise);
        var raiseTip = new cc.Sprite(getResPath("Room/duizhan/dz_raise_tip.png"));
        raiseTip.setPosition(cc.p(66, -5));
        this.m_pRaise.addChild(raiseTip, 1);


        //跟注
        this.m_pCall = new ccui.Button(getResPath("Room/duizhan/dz_call_1.png"), getResPath("Room/duizhan/dz_call_2.png"), getResPath("Room/duizhan/dz_disable.png"));
        this.m_pCall.setPosition(RoomZJHConfig.getPlayerPos(0));
        this.m_pCall.setVisible(false);
        this.m_pCall.setTag(RoomZJHConfig.tag_call);
        this.m_pCall.addClickEventListener(function (sender) {
            self.buttonClickCallback(sender);
        });
        this.addChild(this.m_pCall);
        var callTip = new cc.Sprite(getResPath("Room/duizhan/dz_call_tip.png"));
        callTip.setPosition(cc.p(66, -5));
        this.m_pCall.addChild(callTip, 1);
        numLabel = new cc.LabelAtlas("", getResPath("Room/duizhan/dz_bet_num.png"), 16, 25, '0');
        numLabel.setTag(this.TAG_BUTTON_NUMBER);
        numLabel.setAnchorPoint(cc.p(0.5, 0.5));
        numLabel.setPosition(cc.p(this.m_pCompare.getContentSize().width / 2, this.m_pCompare.getContentSize().height / 2));
        this.m_pCall.addChild(numLabel, 2);

        //自动跟注
        this.m_pAuto = new ccui.Button(getResPath("Room/duizhan/dz_call_1.png"), getResPath("Room/duizhan/dz_call_2.png"), getResPath("Room/duizhan/dz_disable.png"));
        this.m_pAuto.setPosition(RoomZJHConfig.getPlayerPos(0));
        this.m_pAuto.setVisible(false);
        this.m_pAuto.setTag(RoomZJHConfig.tag_callall);
        this.m_pAuto.addClickEventListener(function (sender) {
            self.buttonClickCallback(sender);
        });
        this.addChild(this.m_pAuto);
        var autoTip = new cc.Sprite(getResPath("Room/duizhan/dz_auto_tip.png"));
        autoTip.setPosition(cc.p(66, -5));
        this.m_pAuto.addChild(autoTip, 1);
        numLabel = new cc.LabelAtlas("", getResPath("Room/duizhan/dz_bet_num.png"), 16, 25, '0');
        numLabel.setTag(this.TAG_BUTTON_NUMBER);
        numLabel.setAnchorPoint(cc.p(0.5, 0.5));
        numLabel.setPosition(cc.p(this.m_pCompare.getContentSize().width / 2, this.m_pCompare.getContentSize().height / 2));
        this.m_pAuto.addChild(numLabel, 2);

        var anim = new cc.Sprite(getResPath("Room/duizhan/noseat_anim_1.png"));
        anim.setPosition(cc.p(this.m_pAuto.getContentSize().width / 2, this.m_pAuto.getContentSize().height / 2));
        anim.setTag(this.TAG_SP_ANIMATE);
        anim.setVisible(false);
        this.m_pAuto.addChild(anim, 3);

        var animation = new cc.Animation();
        for (var i = 1; i <= 12; i++) {
            animation.addSpriteFrameWithFile(__String.createWithFormat(getResPath("Room/duizhan/noseat_anim_%1.png"), i));
        }
        animation.setDelayPerUnit(0.1);
        anim.runAction(animate.repeatForever());

        //看牌
        var seePos = RoomZJHConfig.getPlayerPos(0);
        seePos = cc.p(seePos.x, seePos.y - 150);
        var seeBtn = new ccui.Button(getResPath("Room/duizhan/dz_see.png"));
        seeBtn.setTag(RoomZJHConfig.tag_see);
        seeBtn.setVisible(false);
        seeBtn.setZoomScale(0);
        seeBtn.setPosition(seePos);
        seeBtn.addClickEventListener(function (sender) {
            self.buttonClickCallback(sender);
        });
        this.addChild(seeBtn, 10);

        //亮牌
        var showCardBtn = new ccui.Button();
        showCardBtn.setAnchorPoint(cc.p(0.5, 0.5));
        showCardBtn.ignoreContentAdaptWithSize(false);
        showCardBtn.setTouchEnabled(true);
        showCardBtn.setContentSize(cc.size(189, 107));
        showCardBtn.setPosition(RoomZJHConfig.getCardPos(0, 1));
        showCardBtn.setZoomScale(0);
        showCardBtn.setTag(RoomZJHConfig.tag_show_card);
        showCardBtn.setVisible(false);
        showCardBtn.addClickEventListener(function (sender) {
            self.buttonClickCallback(sender);
        });
        this.addChild(showCardBtn);

        var deltaX = 60;
        var beginX = showCardBtn.getContentSize().width / 2 - deltaX;
        var beginY = showCardBtn.getContentSize().height / 2;
        for (var i = 0; i < 3; i++) {
            var cardSp = new cc.Sprite(getResPath("Room/card_back.png"));
            cardSp.setPosition(cc.p(beginX + i * deltaX, beginY));
            showCardBtn.addChild(cardSp);
        }

        var showSp = new cc.Sprite(getResPath("Room/duizhan/dz_show_cards_tip.png"));
        showSp.setTag(128);
        showSp.setPosition(cc.p(showCardBtn.getContentSize().width / 2, showCardBtn.getContentSize().height / 2));
        showCardBtn.addChild(showSp);

        var label = new cc.LabelAtlas("0", getResPath("Room/duizhan/dz_balance_num_win_2.png", 28, 40), '0');
        label.setAnchorPoint(cc.p(0.5, 0.5));
        label.setTag(256);
        label.setPosition(cc.p(showSp.getContentSize().width / 2 + 60, showSp.getContentSize().height / 2));
        showSp.addChild(label, 200);


        var lbTimeText = new cc.LabelTTF("等待游戏开始...0", "Arial", 30);
        lbTimeText.setName("lbTimeText");
        lbTimeText.setAnchorPoint(cc.p(0.5, 0.5));
        lbTimeText.setPosition(RoomZJHConfig.getTimePos());
        lbTimeText.setVisible(false);
        this.addChild(lbTimeText);


        //上桌
        var uptableBtn = new ccui.Button(getResPath("Room/duizhan/dz_sit.png"), getResPath("Room/duizhan/dz_sit_press.png"));
        uptableBtn.setTag(RoomZJHConfig.tag_up_table);
        uptableBtn.setPosition(RoomZJHConfig.getUpTablePos());
        uptableBtn.setVisible(false);
        uptableBtn.addClickEventListener(function (sender) {
            self.buttonClickCallback(sender);
        });
        this.addChild(uptableBtn);
        var text = new cc.Sprite(getResPath("RoomDN/dz_uptable_tip.png"));
        text.setName("text");
        text.setPosition(cc.p(uptableBtn.getContentSize().width / 2, -21));
        uptableBtn.addChild(text);

        //观众
        var outsideBtn = new ccui.Button(getResPath("Room/duizhan/dz_no_seat.png"), getResPath("Room/duizhan/dz_no_seat_press.png"));
        outsideBtn.setTag(RoomZJHConfig.tag_outside);
        outsideBtn.setPosition(RoomZJHConfig.getOutSidePos());
        outsideBtn.setVisible(false);
        outsideBtn.addClickEventListener(function (sender) {
            self.buttonClickCallback(sender);
        });
        this.addChild(outsideBtn);

        //加注界面
        var raiseLayer = new cc.LayerColor(cc.color(0, 0, 0, 100));
        raiseLayer.setName("raise_layer");
        raiseLayer.setPosition(cc.p(0, 0));
        raiseLayer.setVisible(false);
        this.addChild(raiseLayer, 20);

        var touchListener = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches: true,
            onTouchBegan: function (touch, event) {
                return raiseLayer.isVisible();
            },
        });
        if (touchListener) cc.eventManager.addListener(touchListener, raiseLayer);

        var cancelBtn = new ccui.Button(getResPath("Room/duizhan/dz_fold_1.png"), getResPath("Room/duizhan/dz_fold_2.png"), getResPath("Room/duizhan/dz_disable.png"));
        cancelBtn.setPosition(this.getButtonPosition(RoomZJHConfig.tag_raise));
        var _vec = this.getButtonPosition(RoomZJHConfig.tag_max);
        cancelBtn.addClickEventListener(function (sender) {
            self.hideRaiseLayer();
        });
        raiseLayer.addChild(cancelBtn);

        var raiseBg = new cc.Sprite(getResPath("Room/duizhan/dz_raiseset_bg.png"));
        raiseBg.setPosition(cc.p(_vec.x, _vec.y + 100));
        raiseLayer.addChild(raiseBg, -1);

        var addX = 135;
        var posX = raiseBg.getPositionX() - 1.5 * addX;
        var posY = raiseBg.getPositionY() + 2;

        var normalName = "";
        var selectedName = "";
        for (var i = 0; i < 4; i++) {
            normalName = __String.createWithFormat(getResPath("Room/duizhan/dz_raise%1.png"), i + 1)
            selectedName = __String.createWithFormat(getResPath("Room/duizhan/dz_raise%1_press.png"), i + 1)

            var raiseSp = new ccui.Button(normalName, selectedName, normalName);
            raiseSp.setPosition(cc.p(posX + addX * i, posY));
            raiseSp.setTag(this.TAG_RAISE_BEGIN + i);
            raiseSp.addClickEventListener(function (sender) {
                self.raiseSelectedCallback();
            });
            raiseLayer.addChild(raiseSp);

            var maskSp = new cc.Sprite(getResPath("Room/duizhan/dz_raise_enable.png"));
            maskSp.setPosition(cc.p(raiseSp.getContentSize().width / 2, raiseSp.getContentSize().height / 2));
            maskSp.setName("mask_sprite");
            raiseSp.addChild(maskSp);

            var raiseNum = new cc.LabelAtlas("", getResPath("Room/duizhan/dz_raise_num.png"), 20, 33, '0');
            raiseNum.setAnchorPoint(cc.p(0.5, 0.5));
            raiseNum.setPosition(cc.p(65, 70));
            raiseNum.setName("raise_num");
            raiseSp.addChild(raiseNum);
        }

        //比牌界面
        var compareLayer = new cc.LayerColor(cc.color(0, 0, 0, 100));
        compareLayer.setName("compare_layer");
        compareLayer.setPosition(cc.p(0, 0));
        compareLayer.setVisible(false);
        this.addChild(compareLayer, 30);

        var comTouchListener = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches: true,
            onTouchBegan: function (touch, event) {
                return compareLayer.isVisible();
            },
        });
        if (comTouchListener) cc.eventManager.addListener(comTouchListener, compareLayer);

        var cancelComBtn = new ccui.Button(getResPath("Room/duizhan/dz_fold_1.png"), getResPath("Room/duizhan/dz_fold_2.png"), getResPath("Room/duizhan/dz_disable.png"));
        cancelComBtn.setPosition(this.getButtonPosition(RoomZJHConfig.tag_compare));
        cancelComBtn.addClickEventListener(function (sender) {
            self.hideCompareLayer();
        });

        compareLayer.addChild(cancelComBtn);

        for (var i = 0; i < 5; i++) {
            var targetBtn = new ccui.Button(getResPath("Room/duizhan/compare_choice.png"), getResPath("Room/duizhan/compare_choice_press.png"));
            targetBtn.setPosition(RoomZJHConfig.getPlayerPos(i));
            targetBtn.setTag(this.TAG_COMPARE_BEGIN + i);
            targetBtn.addClickEventListener(function (sender) {
                self.compareSelectedCallback();
            });
            compareLayer.addChild(targetBtn);
        }
        //准备
        var size = cc.director.getWinSize();

        var btnReady = new ccui.Button(getResPath("Room/duizhan/btn_pure.png"), getResPath("Room/duizhan/btn_purep.png"));
        btnReady.setTag(this.TAG_READY_BTN);
        btnReady.setPosition(cc.p(size.width / 2, 45));
        btnReady.setCallback(function (sender) {
            self.ReadyCallback(sender.getTag());
        });
        btnReady.setVisible(false);
        this.addChild(btnReady);
    },

    buttonClickCallback: function (sender) {
        var tag = sender.getTag();
        if (tag == RoomZJHConfig.tag_callall) {
            var anim = this.m_pAuto.getChildByTag(this.TAG_SP_ANIMATE)
            if (anim) {
                anim.setVisible(!anim.sVisible());
            }
        }
        if (this.callback) {
            this.callback(tag);
        }
        Sound.getInstance().playEffect(getResPath("Sound/zjh/click.mp3"));
    },

    resetAllBetButton: function () {
        this.resetBetButton(RoomZJHConfig.tag_fold);
        this.resetBetButton(RoomZJHConfig.tag_compare);
        this.resetBetButton(RoomZJHConfig.tag_raise);
        this.resetBetButton(RoomZJHConfig.tag_call);
        this.resetBetButton(RoomZJHConfig.tag_callall);
    },

    resetBetButton: function (tag) {
        var btn = this.getChildByTag(tag)
        if (btn) {
            btn.setPosition(this.getButtonPosition(tag));
            btn.setVisible(false);
        }
    },

    stopAllBetButton: function () {
        this.stopBetButton(RoomZJHConfig.tag_fold);
        this.stopBetButton(RoomZJHConfig.tag_compare);
        this.stopBetButton(RoomZJHConfig.tag_raise);
        this.stopBetButton(RoomZJHConfig.tag_call);
        this.stopBetButton(RoomZJHConfig.tag_callall);
    },

    stopBetButton: function (tag) {
        var btn = this.getChildByTag(tag)
        if (btn) {
            btn.stopAllActions();
        }
    },

    retractAllButton: function (delay) {
        this.stopAllActions();
        this.stopAllBetButton();

        this.retractButton(RoomZJHConfig.tag_fold, delay);
        this.retractButton(RoomZJHConfig.tag_compare, delay);
        this.retractButton(RoomZJHConfig.tag_raise, delay);
        this.retractButton(RoomZJHConfig.tag_call, delay);
        this.retractButton(RoomZJHConfig.tag_callall, delay);

        this.m_layoutType = -1;
    },

    retractButton: function (tag, delay) {
        var btn = this.getChildByTag(tag)
        var pos1 = btn.getPosition();
        var pos2 = this.getButtonPosition(0);
        if (pos1.x == pos2.x && pos1.y == pos2.y) {
            return;
        }
        if (btn) {
            btn.runAction(cc.sequence(cc.delayTime(delay), cc.moveTo(0.08, this.getButtonPosition(0)), cc.hide()));
        }
    },

    showMyBetButtons: function () {
        cc.log("hyhy_test showOtherBetButtons, m_layoutType=" + this.m_layoutType);

        if (this.m_layoutType == 0 && this.getButtonflag()) {
            return;
        }
        this.setButtonflag(true);
        this.stopAllActions();
        this.stopAllBetButton();

        if (this.m_layoutType == -1) {
            this.resetAllBetButton();
            this.stretchButton(RoomZJHConfig.tag_fold);
            this.stretchButton(RoomZJHConfig.tag_compare);
            this.stretchButton(RoomZJHConfig.tag_raise);
            this.stretchButton(RoomZJHConfig.tag_call);
        } else {
            var self = this;
            this.runAction(cc.sequence(
                cc.callFunc(function () {
                    self.retractButton(RoomZJHConfig.tag_fold);
                    self.retractButton(RoomZJHConfig.tag_compare);
                    self.retractButton(RoomZJHConfig.tag_raise);
                    self.retractButton(RoomZJHConfig.tag_call);
                    self.retractButton(RoomZJHConfig.tag_callall);
                }),

                cc.delayTime(0.2),

                cc.callFunc(function () {
                    self.stretchButton(RoomZJHConfig.tag_fold);
                    self.stretchButton(RoomZJHConfig.tag_compare);
                    self.stretchButton(RoomZJHConfig.tag_raise);
                    self.stretchButton(RoomZJHConfig.tag_call);
                })
            ));
        }
        m_layoutType = 0;
    },

    showOtherBetButtons: function () {
        cc.log("hyhy_test showOtherBetButtons, m_layoutType=" + this.m_layoutType);
        if (this.m_layoutType == 1 && this.getButtonflag()) {
            return;
        }
        this.setButtonflag(true);
        this.stopAllActions();
        this.stopAllBetButton();

        if (this.m_layoutType == -1) {
            this.resetAllBetButton();
            this.stretchButton(RoomZJHConfig.tag_fold);
            this.stretchButton(RoomZJHConfig.tag_callall);
        } else {
            var self = this;
            this.runAction(cc.sequence(
                cc.callFunc(function () {
                    self.retractButton(RoomZJHConfig.tag_fold);
                    self.retractButton(RoomZJHConfig.tag_compare);
                    self.retractButton(RoomZJHConfig.tag_raise);
                    self.retractButton(RoomZJHConfig.tag_call);
                    self.retractButton(RoomZJHConfig.tag_callall);
                }),

                cc.delayTime(0.2),

                cc.callFunc(function () {
                    self.stretchButton(RoomZJHConfig.tag_fold);
                    self.stretchButton(RoomZJHConfig.tag_callall);
                })
            ))
            ;
        }
        this.m_layoutType = 1;
    },


    stretchButton: function (tag, delay) {
        var btn = this.getChildByTag(tag);
        if (btn) {
            var self = this;
            btn.runAction(cc.sequence(
                cc.delayTime(delay),
                cc.show(),
                cc.moveTo(0.15, this.getButtonPosition(tag)),
                cc.callFunc(function () {
                    btn.stopAllActions();
                    btn.setVisible(true);
                    btn.setPosition(self.getButtonPosition(tag));
                })
            ));
        }
    },

    getButtonPosition: function (index) {
        var begin = RoomZJHConfig.getPlayerPos(0);
        var offset = cc.p(0, 0);
        var radius = 200;
        if (index == RoomZJHConfig.tag_fold) {
            offset = cc.p(-radius, 0);
        }
        else if (index == RoomZJHConfig.tag_compare) {
            offset = cc.p(-radius * 0.6, radius * 0.75);
        }
        else if (index == RoomZJHConfig.tag_raise) {
            offset = cc.p(radius * 0.6, radius * 0.75);
        }
        else if (index == RoomZJHConfig.tag_max) {
            offset = cc.p(0, radius);
        }
        else if (index == RoomZJHConfig.tag_call) {
            offset = cc.p(radius, 0);
        }
        else if (index == RoomZJHConfig.tag_callall) {
            offset = cc.p(radius, 0);
        }
        return cc.p(begin.x + offset.x, begin.y + offset.y);
    },

    setButtonNumber: function (tag, number) {
        var btn = this.getChildByTag(tag);
        if (btn) {
            var numLabel = btn.getChildByTag(this.TAG_BUTTON_NUMBER)
            if (numLabel) {
                numLabel.setString(number + "");
            }
        }
    },

    setButtonVisible: function (tag, visible) {
        var btn = this.getChildByTag(tag);
        if (btn) {
            btn.setVisible(visible);
        }

        if (RoomZJHConfig.tag_show_card == tag) {
            if (visible == true) {
                var Label = btn.getChildByTag(128).getChildByTag(256)
                if (Label) {
                    Label.setString(this.leftTime + "");
                    this.schedule(this.timerleftdate, 1.0);
                }
            } else {
                this.leftTime = 5;
                this.unschedule(this.timerleftdate);
            }

        }
    },

    setButtonEnable: function (tag, enable) {
        var btn = this.getChildByTag(tag)
        if (btn) {
            btn.setEnabled(enable);
        }

        if (RoomZJHConfig.tag_show_card == tag) {
            if (enable == false) {
                this.leftTime = 5;
                this.unschedule(this.timerleftdate);
            }
        }

    },

    timerleftdate: function (dt) {
        this.leftTime--;
        var Label = this.getChildByTag(RoomZJHConfig.tag_show_card).getChildByTag(128).getChildByTag(256);
        if (Label) {
            if (this.leftTime >= 0) {
                Label.setString(this.leftTime + "");
            } else {
                this.leftTime = 5;
                this.unschedule(this.timerleftdate);
            }

        }
    },

    setAllBetButtonVisible: function (visible) {
        this.setButtonVisible(RoomZJHConfig.tag_fold, visible);
        this.setButtonVisible(RoomZJHConfig.tag_compare, visible);
        this.setButtonVisible(RoomZJHConfig.tag_raise, visible);
        this.setButtonVisible(RoomZJHConfig.tag_call, visible);
        this.setButtonVisible(RoomZJHConfig.tag_callall, visible);
    },

    setCallAll: function (v) {
        var anim = this.m_pAuto.getChildByTag(this.TAG_SP_ANIMATE);
        if (anim) {
            anim.setVisible(v);
        }
    },

    showRaiseLayer: function () {
        var raiseLayer = this.getChildByName("raise_layer");
        if (raiseLayer) {
            raiseLayer.setVisible(true);
            var buff = "";
            for (var i = 0; i < 4; i++) {
                var raiseSp = raiseLayer.getChildByTag(this.TAG_RAISE_BEGIN + i);
                var maskSp = raiseSp.getChildByName("mask_sprite");
                var raiseNum = raiseSp.getChildByName("raise_num");
                RoomZJHInstance.curRaise[i] = RoomZJHInstance.raise[i + 1] * RoomZJHInstance.base_money;
                if (RoomZJHInstance.IsMySee) {
                    buff = RoomZJHInstance.raise[i + 1] * 2 * RoomZJHInstance.base_money + "";
                } else {
                    buff = RoomZJHInstance.raise[i + 1] * RoomZJHInstance.base_money;
                }
                raiseNum.setString(buff);
                if (RoomZJHInstance.cur_bet >= RoomZJHInstance.raise[i + 1] * RoomZJHInstance.base_money) {
                    raiseSp.setEnabled(false);
                    maskSp.setVisible(true);
                } else {
                    raiseSp.setEnabled(true);
                    maskSp.setVisible(false);
                }
            }
        }
    },

    hideRaiseLayer: function () {
        var raiseLayer = this.getChildByName("raise_layer");
        if (raiseLayer) {
            raiseLayer.setVisible(false);
        }
    },

    setRaiseCallback: function (callback) {
        this.raiseCallback = callback;
    },

    raiseSelectedCallback: function (sender) {
        this.hideRaiseLayer();
        if (this.raiseCallback) {
            this.raiseCallback(sender.getTag() - this.TAG_RAISE_BEGIN);
        }
    },

    showCompareLayer: function (targets) {
        if (targets.length == 0) {
            return;
        }

        var compareLayer = this.getChildByName("compare_layer");
        if (compareLayer) {
            compareLayer.setVisible(true);

            for (var i = 0; i < 5; i++) {
                var btn = compareLayer.getChildByTag(this.TAG_COMPARE_BEGIN + i);
                btn.setVisible(false);
            }

            for (var i = 0; i < targets.length; i++) {
                var btn = compareLayer.getChildByTag(this.TAG_COMPARE_BEGIN + targets[i]);
                if (btn) {
                    btn.setVisible(true);
                }
            }
        }
    },

    hideCompareLayer: function () {
        var compareLayer = this.getChildByName("compare_layer");
        if (compareLayer) {
            compareLayer.setVisible(false);
        }
    },

    setCompareCallback: function (callback) {
        this.compareCallback = callback;
    },

    compareSelectedCallback: function (sender) {
        this.hideCompareLayer();
        if (this.compareCallback) {
            this.compareCallback(sender.getTag() - this.TAG_COMPARE_BEGIN);
        }
    },


    notificationCallbackBp: function (msg) {
        var bt = this.getChildByTag(777);

        if (bt != NULL) {
            bt.setVisible(false);
        }
    },

    setMyInfoBtnVisibel: function (v) {
        var my_info_btn = this.getChildByTag(RoomZJHConfig.tag_my_info_btn);

        if (my_info_btn) {
            my_info_btn.setVisible(v);
            my_info_btn.setEnabled(v);
        }

    },

    setCallValue: function (value) {
        var btn_bg = this.getChildByTag(RoomZJHConfig.tag_bet_bg);
        var pCall = btn_bg.getChildByTag(RoomZJHConfig.tag_call);
        if (pCall) {
            var call_money = pCall.getChildByTag(RoomZJHConfig.tag_call_money);
            if (call_money) {
                call_money.setString(value + "");
            }
        }
    },

    setPotVisisble: function (v) {
        var pot_btn = this.getChildByTag(RoomZJHConfig.tag_pot_btn);
        if (pot_btn) {
            pot_btn.setVisible(v);
            pot_btn.setEnabled(v);
        }
    },

    setTableApplyBtnType: function (type) {
        //type:1: 申请上桌  2：申请下桌
        if (type < 1 || type > 2) {
            type = 1;
        }
        var uplist = this.getChildByTag(RoomZJHConfig.tag_uplist);
        if (uplist) {
            var up_text = uplist.getChildByTag(1);
            var pic = type == 1 ? "list_up_text_1.png" : "list_down_text_1.png";
            up_text.setTexture(__String.createWithFormat("Room/RoomUp/%1", pic));
        }
    },

    btnCallback: function (tag) {
        if (RoomZJHConfig.tag_callall == tag) {
            var btn_bg = this.getChildByTag(RoomZJHConfig.tag_bet_bg);
            var n = btn_bg.getChildByTag(RoomZJHConfig.tag_callall);
            var callall_select_bg = n.getChildByTag(5);
            if (callall_select_bg) {
                callall_select_bg.setVisible(!callall_select_bg.isVisible());
            }
        }
        if (this.callback) {
            this.callback(tag);
        }
    },


    setselectBetVisible: function (v) {
        var Mybet_bg = this.getChildByTag(RoomZJHConfig.tag_My_Mul_select_bg);
        var mytab = Mybet_bg.getChildByTag(RoomZJHConfig.tag_My_bet_tab);
        if (Mybet_bg) {
            Mybet_bg.setVisible(v);
            if (v) {
                Mybet_bg.setPosition(cc.p(355, 5));
            }
            else {
                Mybet_bg.setPosition(cc.p(355, -125));
            }
        }
        if (mytab) {
            mytab.setVisible(v);
            mytab.setEnabled(v);
            mytab.setCur(RoomZJHConfig.tag_My_tab_bet_btn + RoomZJHInstance.MyBetRadio);
        }
    },

    selectBetNums: function (tag) {
        RoomZJHInstance.MyBetRadio = tag - RoomZJHConfig.tag_My_tab_bet_btn;
    },

    setselectBetNus: function (nums) {
        var Mybet_bg = this.getChildByTag(RoomZJHConfig.tag_My_Mul_select_bg);
        if (Mybet_bg) {

            for (var i = 0; i < 4; i++) {
                var bet_nums = Mybet_bg.getChildByTag(RoomZJHConfig.tag_My_tab_bet_nums + i);
                if (bet_nums) {
                    bet_nums.setString("X" + nums[i]);
                }
            }
        }
    },

    setPreSitDownVisible: function (index, v) {
        var btn = this.getChildByTag(RoomZJHConfig.tag_preplayers_sitdown_btn + index);
        if (btn) {
            btn.setVisible(v);
            btn.setEnabled(v);
        }
    },

    setAllPreSitDownVisible: function (v) {
        for (var i = 0; i < RoomZJHInstance.PreNums; i++) {
            var btn = this.getChildByTag(RoomZJHConfig.tag_preplayers_sitdown_btn + i);
            if (btn) {
                btn.setVisible(v);
                btn.setEnabled(v);
            }
        }
    },

    setReadVisible: function (v) {
        var pReady = this.getChildByTag(RoomZJHConfig.tag_ready);
        if (pReady) pReady.setVisible(false);
    },

    setAllUserInfoVisble: function (enabled) {
        var node;
        for (var i = 0; i < 5; i++) {
            node = this.getChildByTag(RoomZJHConfig.tag_player_info_btn + i);
            if (node) node.setVisible(enabled);
        }
    },

    setUserInfoVisble: function (index, enabled) {
        var node;
        node = this.getChildByTag(RoomZJHConfig.tag_player_info_btn + index);
        if (node) node.setVisible(enabled);
    },

    setCompareEnable: function (v) {
        this.setOneEnable(RoomZJHConfig.tag_compare, v);
    },

    setSeeEnable: function (v) {
        this.setOneEnable(RoomZJHConfig.tag_see, v);
    },

    setFoldEnable: function (v) {
        this.setOneEnable(RoomZJHConfig.tag_fold, v);
    },

    setRaiseEnable: function (v) {
        this.setOneEnable(RoomZJHConfig.tag_raise, v);
    },

    setCallEnable: function (v) {
        this.setOneEnable(RoomZJHConfig.tag_call, v);
    },

    setAllinEnable: function (v) {
        this.setOneEnable(RoomZJHConfig.tag_allin, v);
    },

    hideBetBtn: function (visible) {
        this.setAllBetButtonVisible(!visible);
    },

    setOneVisible: function (tag, v) {
        var btn_bg = this.getChildByTag(RoomZJHConfig.tag_bet_bg);
        if (btn_bg) {
            var n = btn_bg.getChildByTag(tag);
            if (n) n.setVisible(v);
        }
    },

    setOneEnable: function (tag, v) {
        var btn_bg = this.getChildByTag(RoomZJHConfig.tag_bet_bg);
        if (btn_bg) {
            var n = btn_bg.getChildByTag(tag);
            if (n) n.setEnabled(v);
        }
    },

    setECVisible: function (v) {
        var btn = this.getChildByTag(RoomZJHConfig.tag_Ec_btn);
        if (btn) {
            btn.setVisible(v);
            btn.setEnabled(v);
        }
    },

    setCallback: function (callback) {
        this.callback = callback;
    },

    setReadyVisible: function (v) {//准备按钮是否可见
        var btnReady = this.getChildByTag(this.TAG_READY_BTN);
        if (btnReady) {
            btnReady.setVisible(v);
        }
    },

    ReadyCallback: function (tag) {//准备按钮回调

        if (this.callback) {
            this.callback(tag);
        }
        ZJHLogic.sendReady();
    },


    showLaTime: function () {
        var _Label = this.getChildByName("lbTimeText");
        if (_Label) {
            _Label.setVisible(true);
            _Label.setString("等待游戏开始..." + this.showTime);
            this.schedule(this.timertipStart, 1.0);
        }
    },

    hideLaTime: function () {
        this.showTime = 20;
        this.unschedule(this.timertipStart);
        var _Label = this.getChildByName("lbTimeText");
        if (_Label) {
            _Label.setVisible(false);
        }
    },

    timertipStart: function (dt) {
        this.showTime--;
        var _Label = this.getChildByName("lbTimeText");
        if (_Label) {
            if (this.showTime >= 0) {
                if (this.showTime <= 5) {
                    if (this.showTime == 5) {
                        PlatformHelper.vibrate();
                    }
                    Sound.getInstance().playEffect(getResPath("Sound/zjh/overtime.mp3"));
                }
                _Label.setString("等待游戏开始..." + this.showTime);
            } else {
                this.hideLaTime();
            }

        }
    },

});