var createTable = function () {
    var curGameType = ZJHModel.getInstance().curGameType;
    if (curGameType == ROOM_TYPE.ROOM_TYPE_HNMJ_DZ) {
        layer = new MJTable_hnmj();
    }else if(curGameType == ROOM_TYPE.ROOM_TYPE_JXMJ_DZ){
        layer = new MJTable_jxmj();
    }else if (curGameType == ROOM_TYPE.ROOM_TYPE_HUNAN_DZ) {
        layer = new MJTable_hunan();
    }else {
        layer = new MJTable();
    }
    return layer;
}

var  MJScene = cc.Scene.extend({
    onEnter: function () {
        this._super();
        var stable = createTable();
        this.addChild(stable);

        if (ZJHModel.getInstance().isOnVideo) {
            var video = new MJVideo();
            video.runScript(stable);
            this.addChild(video,100000);
        }
    }
});