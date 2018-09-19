var MatchHallScene = cc.Scene.extend({
    onEnter: function () {
        this._super();
        var stable = new MatchHall();
        this.addChild(stable);
    }
});

var MatchHall = cc.Layer.extend({
    tag: "MatchHall",
    ctor: function () {
        this._super();
    },
    release: function () {
        this.unscheduleAllCallbacks();
        ccs.ArmatureDataManager.destroyInstance();
        cc.spriteFrameCache.removeSpriteFrames();
        cc.textureCache.removeAllTextures();
        jsb.fileUtils.purgeCachedEntries();
    },
    onExit: function () {
        this.release();
        this._super();
    },
});