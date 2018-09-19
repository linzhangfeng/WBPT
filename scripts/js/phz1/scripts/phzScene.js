
var PHZScene = cc.Scene.extend({
    onEnter:function () {
        this._super();
        var layer = new PHZTable();
        this.addChild(layer);
    }
});

