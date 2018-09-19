
var wzdnScene = cc.Scene.extend({
    onEnter:function () {
        this._super();
        var layer = new WZDNTable();
        this.addChild(layer);
    }
});

