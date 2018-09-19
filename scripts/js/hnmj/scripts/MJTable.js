var MJTable = cc.Layer.extend({
    uiNode:null,
    
    ctor:function () {
        this._super();
        this.uiJson = ccs.load(PackName+"/res/table.json");
        this.uiNode =this.uiJson.node;
        this.addChild(this.uiNode);
    }

});