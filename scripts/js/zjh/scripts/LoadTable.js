var LoadTable = cc.Scene.extend({
    connectTime:0,
    onEnter: function () {
        this._super();
        RoomZJHInstance.init();
        // RoomZJHInstance.reset();
        RoomZJHInstance.roomData = JSON.parse(ZJHModel.getInstance().getRoomData());
        RoomZJHInstance.curRoomID = ZJHModel.getInstance().curRoomID;
        RoomZJHInstance.curGameType = ZJHModel.getInstance().curGameType;


        ZJHLogic.initNet(RoomZJHInstance.roomData["gameRoom"]["way"]["ip"], Number(RoomZJHInstance.roomData["gameRoom"]["way"]["port"]));
        ZJHLogic.connectNet();

        this.schedule(this.updateGame, 0.0);
        var winSize = cc.director.getWinSize();

        var loadText = new cc.Sprite("Loading/loading_txt2.png");
        loadText.setPosition(cc.p(winSize.width / 2, winSize.height / 2 + 40));
        this.addChild(loadText);

        var dots = new cc.Sprite("Loading/loading_1.png");
        dots.setPosition(cc.p(winSize.width / 2, winSize.height / 2 - 40));
        this.addChild(dots);

        var animation = new cc.Animation();
        for (var i = 1; i <= 8; i++) {
            animation.addSpriteFrameWithFile(__String.createWithFormat("Loading/loading_%1.png", i));
        }
        animation.setDelayPerUnit(0.2);
        var animate = cc.animate(animation);
        dots.runAction(animate.repeatForever());
        this.connectTime = utils.getTimeStamp();
    },
    updateGame: function (dt) {
        var curTime = utils.getTimeStamp();
        if (curTime - this.connectTime > 10 * 1000)
        {
            this.loginFial(-1);
            return;
        }
        var response = ZJHLogic.getNetData();

        if (response) {
            if (response.cmd == TCPCMD.DISCONNECT_RES || response.cmd == TCPCMD.CONNECT_ERROR_RES || response.cmd == TCPCMD.SEND_DATA_ERROR_RES || response.cmd == TCPCMD.RECV_DATA_ERROR_RES) //关闭
            {
                RoomZJHInstance.roomConnectFail++;
                if (RoomZJHInstance.roomConnectFail == 60 * 1) {
                    PlatformHelper.showToast("网络连接异常，正在重试，请稍候");
                }
                else if (RoomZJHInstance.roomConnectFail >= 60 * 10)
                {
                    this.loginFial(-2);
                }
                else
                {
                    ZJHLogic.connectNet();
                }
            }
            else if (response.cmd == TCPCMD.CONNECT_OK_RES) {
                ZJHLogic.sendLogin();
            }
            else if (response.cmd == TCPCMD.RECV_DATA_OK_RES) {
                RoomZJHInstance.roomConnectFail = 0;
                this.connectTime = utils.getTimeStamp();

                var data = response.parseDatta();
                var val;
                try{
                    val = JSON.parse(data);
                }
                catch(e)
                {
                    this.loginFial(data);
                    return;
                }
                var cmd = val.cmd;

                if (cmd == NetCMD.SERVER_LOGIN_SUCC_UC)
                {
                    this.loginSucc();
                }
                else if (cmd == NetCMD.SERVER_LOGIN_ERR_UC)
                {
                    var code = utils.getValInt(val, "code");
                    this.loginFial(code);
                }
            }
            response.release();
        } else {
        }
    },
    loginFial: function (code) {
        PlatformHelper.showToast("进入房间失败，请重试[" + code + "]");
        ZJHModel.getInstance().roomToHall(true);
    },

    loginSucc: function () {
        ZJHModel.getInstance().hallToRoomH();
        cc.director.replaceScene(new ZJHScene());
    }
});