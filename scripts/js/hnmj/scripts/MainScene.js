var MainScene = cc.Scene.extend({
    onEnter:function () {
        this._super();
        var layer = new MJTable();
        this.addChild(layer);
    }
});