
var CXScene = cc.Scene.extend({
    onEnter:function () {
        this._super();
        var layer = new CXTable();
        this.addChild(layer);
    }
});

