var  ZJHScene = cc.Scene.extend({
    onEnter: function () {
        this._super();
        var stable = new ZJHTable();
        this.addChild(stable);
    }
});