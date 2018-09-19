
var LoadTable = cc.Scene.extend({
    connectTime:0,
    onEnter: function () {
        this._super();
        MJModel.init();
        MJModel.reset();
        MJModel.roomData = JSON.parse(ZJHModel.getInstance().getRoomData());
        MJModel.curRoomID = ZJHModel.getInstance().curRoomID;
        MJModel.curGameType = ZJHModel.getInstance().curGameType;
        MJModel.take_in = MJModel.roomData["gameRoom"]["takeIn"];

        CCTcpClient.getInstance(MJModel.curRoomID).setProtoType(MJModel.isProtobufCJJ3() ? ProtoType.PROTOBUF_CJJ3 : ProtoType.PROTOBUF_CJJ2);

        CCTcpClient.getInstance(MJModel.curRoomID).set_host_port(MJModel.roomData["gameRoom"]["way"]["ip"], Number(MJModel.roomData["gameRoom"]["way"]["port"]));

        CCTcpClient.getInstance(MJModel.curRoomID).connect();

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
        this.connectTime = time(null);
    },
    updateGame: function (dt) {
        var curTime = time(null);
        if (curTime - this.connectTime > 10 * 1000)
        {
            this.loginFial(-1);
            return;
        }
        var response = CCTcpClient.getInstance(MJModel.curRoomID).get();

        if (response) {
            if (response.cmd == TCPCMD.DISCONNECT_RES || response.cmd == TCPCMD.CONNECT_ERROR_RES || response.cmd == TCPCMD.SEND_DATA_ERROR_RES || response.cmd == TCPCMD.RECV_DATA_ERROR_RES) //关闭
            {
                MJModel.roomConnectFail++;
                if (MJModel.roomConnectFail == 60 * 1) {
                    PlatformHelper.showToast("网络连接异常，正在重试，请稍候");
                }
                else if (MJModel.roomConnectFail >= 60 * 10)
                {
                    this.loginFial(-2);
                }
                else
                {
                    CCTcpClient.getInstance(MJModel.curRoomID).connect();
                }
            }
            else if (response.cmd == TCPCMD.CONNECT_OK_RES) {
                sendLogin();
            }
            else if (response.cmd == TCPCMD.RECV_DATA_OK_RES) {
                MJModel.roomConnectFail = 0;
                this.connectTime = time(null);
                if (response.id == CMD.SERVER_LOGIN_SUCC_UC)
                {
                    this.loginSucc();
                }
                else if (response.id == CMD.SERVER_LOGIN_ERR_UC)
                {
                    var packet = parsePacket("proto.login.AckLoginError",response.data);
                    this.loginFial(packet.code);
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
        ZJHModel.getInstance().hallToRoomV();
        cc.director.replaceScene(new MJScene());
    }
});