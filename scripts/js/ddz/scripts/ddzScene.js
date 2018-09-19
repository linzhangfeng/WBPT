
var DDZScene = cc.Scene.extend({
    onEnter:function () {
        this._super();
        var layer = new DDZTable();
        this.addChild(layer);
    }
});

