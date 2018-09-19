
var yzdnScene = cc.Scene.extend({
    onEnter:function () {
        this._super();
        var layer = new YZDNTable();
        this.addChild(layer);
    }
});

