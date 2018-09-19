var PackName = "hnmj";

require(PackName+"/scripts/MainScene.js");
require(PackName+"/scripts/MJTable.js");;

cc.game.onStart = function(){

    // Pass true to enable retina display, on Android disabled by default to improve performance
    cc.view.enableRetina(cc.sys.os === cc.sys.OS_IOS ? true : false);

    // Adjust viewport meta
    cc.view.adjustViewPort(true);

    ZJHModel.getInstance().hallToRoomV();
    //load resources
    cc.LoaderScene.preload({}, function () {
        cc.director.runScene(new MainScene());
    }, this);
};
cc.game.run();