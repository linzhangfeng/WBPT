var ZJHLogic = {

    msData:[],

    addCMD:function(cmd)
    {
        if(!this.msData.hasOwnProperty(cmd))
        {
            this.msData[cmd] = {"num":0,"time":0};
        }
        this.msData[cmd].num +=1;
        this.msData[cmd].time = utils.getTimeStamp();
    },
    checkCMD:function(cmd1)
    {
        var cmd = 0;
        switch (cmd1)
        {

        }
        if( this.msData.hasOwnProperty(cmd) )
        {
            this.msData[cmd].num -=1;
            var dis = utils.getTimeStamp() - this.msData[cmd].time;
            this.msData[cmd].time =utils.getTimeStamp();
            if(dis>1000)
            {
                var dat = {};
                dat["event"] ="102";
                var d = {};
                d["gameName"] ="拼三张";
                d["gameid"] =this.roomData["gameRoom"]["gameId"];
                d["roomid"] = this.curRoomID;
                d["roomCode"] = this.roomData["gameRoom"]["code"];
                d["ms"] = dis;
                d["cmd"] = cmd;
                dat["content"] =JSON.stringify(d);
                JsUtils.postNotifiStr("game_play_event",JSON.stringify(dat));
            }
        }
    },

    clearCMD:function()
    {
        this.msData =[];
    },

    //清楚联网请求列表
    resetResponse:function()
    {
        CCTcpClient.getInstance(RoomZJHInstance.curRoomID).reset_response_queue();
    },

    initNet:function(ip,host)
    {
        CCTcpClient.getInstance(RoomZJHInstance.curRoomID).setProtoType(0);
        CCTcpClient.getInstance(RoomZJHInstance.curRoomID).set_host_port(ip,host);
    },
    //连接tcp
    connectNet:function()
    {
        CCTcpClient.getInstance(RoomZJHInstance.curRoomID).connect();
    },

    isConnect:function()
    {
        CCTcpClient.getInstance(RoomZJHInstance.curRoomID).isConnect();
    },
    //获取联网数据
    getNetData:function()
    {
        return  CCTcpClient.getInstance(RoomZJHInstance.curRoomID).get();
    },
    //发送TCP联网数据
    sendData:function(d)
    {
        cc.log("___send:"+d);
        CCTcpClient.getInstance(RoomZJHInstance.curRoomID).sendCodeData(d);
    },

    //发送TCP联网数据
    sendData:function(d)
    {
        cc.log("___send:"+d);
        CCTcpClient.getInstance(RoomZJHInstance.curRoomID).sendCodeData(d);
    },

    //发送登录请求
    sendLogin:function()
    {
        var  d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_DZ_LOGIN_REQ;
        d.data["uid"]  =ZJHModel.getInstance().uid;
        d.data["skey"]  =ZJHModel.getInstance().UserKey;
        d.data["roomid"]  =""+RoomZJHInstance.curRoomID;
        this.sendData(d.toStr());
    },
    sendLogout:function () {
        var d = packet;
        d.data = {};
        d.data["cmd"] = NetCMD.CLIENT_LOGOUT_REQ;
        this.sendData(d.toStr());
    },

    sendReady:function () {
        var d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_READY_REQ;
        this.sendData(d.toStr());
    },
    sendDissolveRoom:function () {
        var d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_DISSOLVE_ROOM_REQ;
        d.data["uid"]  =ZJHModel.getInstance().uid;
        this.sendData(d.toStr());
    },
    sendDissolveAction:function (action) {
        var d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_DISSOLVE_ACTION_REQ;
        d.data["uid"]  =ZJHModel.getInstance().uid;
        jpacket.val["action"] = action;
        this.sendData(d.toStr());
    },

    sendRoomInfo:function () {
        var d = packet;
        d.data = {};
        d.data["cmd"]  =NetCMD.CLIENT_ROOM_INFO_REQ;
        d.data["uid"]  =ZJHModel.getInstance().uid;
        d.data["vid"]  =101;
        d.data["sky"]  = ZJHModel.getInstance().UserKey;
        this.sendData(d.toStr());
    },

    sendUpTable:function () {
        var d = packet;
        d.data = {};
        d.data["cmd"] = NetCMD.CLIENT_UPTABLE_APPLY_REQ;
        this.sendData(d.toStr());
    },

    sendDownTable:function () {
        var d = packet;
        d.data = {};
        d.data["cmd"] = NetCMD.CLIENT_DOWNTABLE_REQ;
        this.sendData(d.toStr());
    },

    sendBetAction:function (action, raiseBet, seatId, targetSeatId) {
        var d = packet;
        d.data = {};
        d.data["cmd"] = NetCMD.CLIENT_BET_REQ;
        d.data["action"] = action;
        if (raise_bet != undefined) {
            d.data["raise_bet"] = raiseBet;
        } else if (seatId != undefined && targetSeatId != null) {
            d.data["seatid"] = seatid;
            d.data["target_seatid"] = targetSeatid;
        }
        this.sendData(d.toStr());
    },

    sendOtherShow:function () {
        var d = packet;
        d.data = {};
        d.data["cmd"] = NetCMD.CLINET_OTHER_SHOW_REQ;
        d.data["action"] = action;
        this.sendData(d.toStr());
    },

    sendSwitchRoom:function () {
        var d = packet;
        d.data = {};
        d.data["cmd"] = NetCMD.CLIENT_SWITCH_ROOM;
        this.sendData(d.toStr());
    },

    sendUpGuest:function (seatNum) {
        var d = packet;
        d.data = {};
        d.data["cmd"] = NetCMD.CLIENT_UP_GUEST_REQ;
        d.data["seat_num"] = seatNum;
        this.sendData(d.toStr());
    },

    sendEmotion: function (faceId) {
        var d = packet;
        d.data = {};
        d.data["cmd"] = NetCMD.CLIENT_EMOTION_REQ;
        d.data["seatid"] = RoomZJHInstance.seatid;
        d.data["target_seatid"] = RoomZJHInstance.getSeatidByPos(pos);
        d.data["type"] = faceId;
        this.sendData(d.toStr());
    },

    sendHeartBeat:function () {
        var d = packet;
        d.data = {};
        d.data["cmd"] = NetCMD.CLINET_HEART_BEAT_REQ;
        d.data["active"] = 0;
        this.sendData(d.toStr());
        cc.log("sendHeartBeat");
    },
};