var RoomZJHRaise = cc.Layer.extend({
    callback: null,
    m_pos1: null,
    m_pos2: null,
    isIng: false,


    ctor: function () {
        var size = cc.director.getWinSize();

        var dialog = new cc.Layer();
        dialog.setContentSize(cc.size(575, 105));
        dialog.setAnchorPoint(cc.p(0.0, 0.0));
        dialog.setPosition(cc.p(15, -170));
        dialog.setTag(10);
        this.addChild(dialog);

        var bg_size = dialog.getContentSize();
        var bx = 145;
        var by = bg_size.height - 70;
        var dx = 100;

        var chip;
        var buf;
        var buff;
        var temp;
        for (var i = 0; i < 5; i++) {

            //todo copy there image.
            buf = __String.createWithFormat("Room/M1/room_my_bet_%1.png", i + 1);
            buff = __String.createWithFormat("Room/M1/room_my_bet_%1_h.png", i + 1);
            temp = __String.createWithFormat("Room/M1/room_my_bet_%1_f.png", i + 1);
            if (i == 4) {
                buf = __String.createWithFormat("Room/M1/room_my_bet_%1.png", i - 1);
                buff = __String.createWithFormat("Room/M1/room_my_bet_%1_h.png", i - 1);
                temp = __String.createWithFormat("Room/M1/room_my_bet_%1_f.png", i - 1);
            }
            chip = new ccui.Button(buf, buff, temp);
            chip.setTag(10 + i);
            chip.setAnchorPoint(cc.p(0.5, 0.5));
            chip.setPosition(cc.p(bx + dx * i, by));
            chip.setCallback(this.btnCallback, this);
            dialog.addChild(chip);
            if (RoomZJHInstance.raise[i] >= 10000) {
                buf = __String.createWithFormat("%1:;", RoomZJHInstance.raise[i] / 10000);
            }
            else {
                buf = __String.createWithFormat("%1", RoomZJHInstance.raise[i]);
            }
            var bet_nums = new cc.LabelAtlas(buf, "Room/million/room_my_bet_nums.png", 17, 27, '0');
            bet_nums.setAnchorPoint(cc.p(0.5, 0.5));
            bet_nums.setPosition(cc.p(chip.getContentSize().width / 2, chip.getContentSize().height / 2));
            chip.addChild(bet_nums, 10);
            var cur = RoomZJHInstance.cur_bet;

            if (cur >= RoomZJHInstance.raise[i]) {
                chip.setEnabled(false);
            }
        }
        var touchListener = new cc.EventListenerTouchOneByOne();
        touchListener.onTouchBegan = cc.callFunc(this.onTouchBegan, this);
        touchListener.onTouchMoved = cc.callFunc(this.onTouchMoved, this);
        touchListener.onTouchEnded = cc.callFunc(this.onTouchEnded, this);
        touchListener.setSwallowTouches(true);
        cc.director.getEventDispatcher().addEventListenerWithSceneGraphPriority(touchListener, this);

        var listenerkeyPad = cc.EventListener.create();
        listenerkeyPad.onKeyReleased = cc.callFunc(this.onKeyReleased, this);
        cc.director.getEventDispatcher().addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

        var actionTo = cc.moveBy(0.17, cc.p(0, 290));
        dialog.runAction(cc.sequence(actionTo, cc.callFunc(this.downOver, this)));
        this.isIng = true;
    },

    downOver: function () {
        this.isIng = false;
    },

    closeCallBack: function (tag) {
        runAction(cc.sequence(cc.moveBy(0.17, cc.p(0, -640)), cc.callFunc(this.removeSelf, this)));
        this.isIng = true;
    },

    onKeyReleased: function (keycode, event) {
        if (keycode == cc.KEY.ESCAPE) {
            if (this.isIng) {
                return;
            }
            this.closeCallBack();
        }
        event.stopPropagation();
    },


    removeSelf: function () {
        this.removeFromParentAndCleanup(true);
    },

    btnCallback: function (tag) {
        if (this.callback) {
            this.callback(tag - 10);
        }
        this.closeCallBack();
    },


    isTouchInside: function (touch) {
        var touchPoint = touch.getLocation();
        var dialog = this.getChildByTag(10);
        var localPos = dialog.convertToNodeSpace(touchPoint);

        var rc = dialog.boundingBox();
        var isTouched = rc.containsPoint(localPos);

        return isTouched;
    },


    onTouchBegan: function (touch, event) {
        return true;
    },

    onTouchMoved: function (touch, event) {
    },

    onTouchEnded: function (touch, event) {
        this.closeCallBack();
    },

    setSelector: function (callback) {
        this.callback = callback;
    }

});