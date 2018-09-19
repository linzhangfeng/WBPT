var RoomZJHCompare = cc.Layer.extend({
    callback: null,

    ctor: function () {
        this.setKeypadEnabled(true);

        var base = new cc.LayerColor(cc.color(0, 0, 0, 150));
        this.addChild(base);

        var size = cc.director.getWinSize();

        var compare;
        var arrow;

        for (var i = 1; i < 5; i++) {
            compare = new ccui.Button();
            compare.setContentSize(cc.size(96, 100));
            compare.setCallback(this.btnCallback, this);
            compare.setTag(10 + i);
            compare.setPosition(RoomZJHConfig.getComparePos(i));
            this.addChild(compare);

            arrow = new cc.Sprite(getResPath("Room/room-vs-arrow.png"));
            arrow.setTag(20 + i);
            arrow.setPosition(RoomZJHConfig.getComparePos(i));
            this.addChild(arrow);
        }
    },

    removeSelf: function () {
        this.removeFromParentAndCleanup(true);
    },

    HideAll: function () {
        for (var i = 1; i < 5; i++) {
            var compare = this.getChildByTag(10 + i);
            compare.setVisible(false);

            var arrow = this.getChildByTag(20 + i);
            arrow.setVisible(false);
        }
    },

    showCompare: function () {
        var compare = this.getChildByTag(index + 10);
        compare.setVisible(true);

        var arrow = this.getChildByTag(index + 20);
        arrow.setVisible(true);

        var begin = arrow.getPosition();
        var end = arrow.getPosition();
        end.y += 20;

        var act1 = cc.scaleTo(0.5, 0.5);
        var act2 = cc.scaleTo(0.5, 1);
        var delay = cc.delayTime(0.1);
        var seq = cc.sequence(delay, act1, delay, act2);
        varrep = cc.repeatForever(seq);
        arrow.runAction(rep);
    },

    isTouchInside: function (touch) {
        var isTouched = false;
        var touchPoint = touch.getLocation();
        var localPos = this.getParent().convertToNodeSpace(touchPoint);
        var j = 0;
        for (var i = 11; i < 17; i++) {
            var p = this.getChildByTag(i);
            var rc = p.boundingBox();
            isTouched = rc.containsPoint(localPos);
            if (isTouched) {
                j++;
            }
        }
        if (j > 0) {
            return true;
        }
        else {
            this.removeSelf();
        }
        return false;
    },

    onTouchBegan: function (touch, event) {
        var touchPoint = touch.getLocation();

        if (!this.isTouchInside(touch)) {
            cc.log("ccTouchBegan a");
        }
        else {
            cc.log("ccTouchBegan b");
        }

        return true;
    },

    onTouchMoved: function (touch, event) {
        var touchPoint = touch.getLocation();
    },

    onTouchEnded: function (touch, event) {
        var touchPoint = touch.getLocation();
    },

    btnCallback: function (tag) {
        if (this.callback != null) {
            this.callback(tag - 10);
        }
        this.removeSelf();
    },

    onKeyReleased: function (keycode, event) {
        //todo KEY?
        if (keycode == cc.KEY.escape) {
            this.removeSelf();
        }
        event.stopPropagation();
    },

    setSelector: function (callback) {
        this.callback = callback;
    }

});