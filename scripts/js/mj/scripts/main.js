/**
 * A brief explanation for "project.json":
 * Here is the content of project.json file, this is the global configuration for your game, you can modify it to customize some behavior.
 * The detail of each field is under it.
 {
    "project_type": "javascript",
    // "project_type" indicate the program language of your project, you can ignore this field

    "debugMode"     : 1,
    // "debugMode" possible values :
    //      0 - No message will be printed.
    //      1 - cc.error, cc.assert, cc.warn, cc.log will print in console.
    //      2 - cc.error, cc.assert, cc.warn will print in console.
    //      3 - cc.error, cc.assert will print in console.
    //      4 - cc.error, cc.assert, cc.warn, cc.log will print on canvas, available only on web.
    //      5 - cc.error, cc.assert, cc.warn will print on canvas, available only on web.
    //      6 - cc.error, cc.assert will print on canvas, available only on web.

    "showFPS"       : true,
    // Left bottom corner fps information will show when "showFPS" equals true, otherwise it will be hide.

    "frameRate"     : 60,
    // "frameRate" set the wanted frame rate for your game, but the real fps depends on your game implementation and the running environment.

    "id"            : "gameCanvas",
    // "gameCanvas" sets the id of your canvas element on the web page, it's useful only on web.

    "renderMode"    : 0,
    // "renderMode" sets the renderer type, only useful on web :
    //      0 - Automatically chosen by engine
    //      1 - Forced to use canvas renderer
    //      2 - Forced to use WebGL renderer, but this will be ignored on mobile browsers

    "engineDir"     : "frameworks/cocos2d-html5/",
    // In debug mode, if you use the whole engine to develop your game, you should specify its relative path with "engineDir",
    // but if you are using a single engine file, you can ignore it.

    "modules"       : ["cocos2d"],
    // "modules" defines which modules you will need in your game, it's useful only on web,
    // using this can greatly reduce your game's resource size, and the cocos console tool can package your game with only the modules you set.
    // For details about modules definitions, you can refer to "../../frameworks/cocos2d-html5/modulesConfig.json".

    "jsList"        : [
    ]
    // "jsList" sets the list of js files in your game.
 }
 *
 */
var PackName = ZJHModel.getInstance().packName;

if (PackName == "") {
    PackName = "mj";
}
var getSrcPath = function (v) {
    return PackName + "/scripts/" + v;
}

var getResPath = function (v) {
    cc.log("getResPath:" + v);
    return PackName + "/res/" + v;
}

require(getSrcPath("MJLogic.js"));
require(getSrcPath("MJScene.js"));
require(getSrcPath("MJTable.js"));
require(getSrcPath("MJAction.js"));
require(getSrcPath("MJCards.js"));
require(getSrcPath("MJConfig.js"));
require(getSrcPath("MJModel.js"));
require(getSrcPath("MJPlayer.js"));
require(getSrcPath("MJTips.js"));
require(getSrcPath("MJDialogs.js"));
require(getSrcPath("MJInfo.js"));
require(getSrcPath("MJ_scmj.js"));
require(getSrcPath("proto.js"));
require(getSrcPath("utils.js"));
require(getSrcPath("Log.js"));
require(getSrcPath("LoadTable.js"));
require(getSrcPath("MJVideo.js"));
require(getSrcPath("Base64.js"));
require(getSrcPath("MJResult.js"));
require(getSrcPath("MJCommonResult.js"));
require(getSrcPath("MJ_hnmj.js"));
require(getSrcPath("MJ_jxmj.js"));
require(getSrcPath("MJ_hunan.js"));


cc.game.onStart = function () {
    // Pass true to enable retina display, on Android disabled by default to improve performance
    cc.view.enableRetina(cc.sys.os === cc.sys.OS_IOS ? true : false);

    // Adjust viewport meta
    cc.view.adjustViewPort(true);

    //load resources
    cc.LoaderScene.preload({}, function () {
        if (ZJHModel.getInstance().isOnVideo) {
            ZJHModel.getInstance().hallToRoomV();
            cc.director.replaceScene(new MJScene());
        }
        else {
            cc.director.replaceScene(new LoadTable());
        }
    }, this);

    //-------------------------------------------------------------------
    /*
     var ProtoBuf = dcodeIO.ProtoBuf;
     var file = "mj/scripts/proto/test1.proto";
     var Build = ProtoBuf.loadProto(jsb.fileUtils.getStringFromFile(file),null,file);
     */
//    var Test2 = Build.build("proto.test1.Test2");
//    
//    var Test1 = Build.build("proto.test1.Test1");

    //-------------------------------------------------------------------//


    //-------------------------------------------------------------------
    /*
     var jsonStr = "mj/scripts/proto/majiang.json";
     var Build = ProtoBuf.loadJson(jsb.fileUtils.getStringFromFile(jsonStr),null,jsonStr);

     var Test2 = Build.build("nested.proto.nested.login.nested.ReqLogin");

     var Test1 = Build.build("nested.proto.nested.login.nested.ReqLogout");
     */
//    var test2 = new Test2;
//    test2.uid = 654321;
//    
//    var test1 = new Test1;
//    test1.uid = 123456;
//    cc.log("--------" + "test1 uid:" + test1);
//    cc.log("--------" + "test2 uid:" + test2.uid);
    /*
     var buffer = test1.encode64();

     cc.log("--------" + "buf:" + buffer);
     var msg = Test1.decode64(buffer);

     CCTcpClient.getInstance(10).send_data_pb2(buffer,2);
     cc.log("--------" + "uid:" + msg.uid);
     */
    //-------------------------------------------------------------------//
};
cc.game.run();