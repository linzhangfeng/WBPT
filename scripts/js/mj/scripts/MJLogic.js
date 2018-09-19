var createPacket = function (obj) {
    cc.log("createPacket:" + obj);
    var Object = Build.build(obj);
    var packet = new Object;
    return packet;
}

var parsePacket = function (obj, data) {
    cc.log("parsePacket:" + obj);
    var Object = Build.build(obj);
    var packet = Object.decode64(data);
    return packet;
}

var sendPacket = function (packet, cmd) {
    var sendData = packet.encode64();
    if(MJModel.isProtobufCJJ3()){
        // CCTcpClient.getInstance(MJModel.curRoomID).send_data_pb3(sendData, cmd , MJModel.roomData["gameRoom"]["way"]["playId"]);
        CCTcpClient.getInstance(MJModel.curRoomID).send_data_pb3(sendData, cmd , MJModel.curGameType);
    }else{
        CCTcpClient.getInstance(MJModel.curRoomID).send_data_pb2(sendData, cmd);
    }
}

var sendLogin = function () {
    CCTcpClient.getInstance(MJModel.curRoomID).reset_response_queue();

    var packet = createPacket("proto.login.ReqLogin");
    packet.uid = ZJHModel.getInstance().uid;
    packet.skey = ZJHModel.getInstance().UserKey;
    packet.roomid = MJModel.curRoomID;
    packet.base_money = MJModel.base_money;
    packet.stand_money = MJModel.stand_money;
    sendPacket(packet, CMD.CLIENT_DZ_LOGIN_REQ);
}

var sendLogout = function () {
    var packet = createPacket("proto.login.ReqLogout");
    packet.uid = ZJHModel.getInstance().uid;
    sendPacket(packet, CMD.CLIENT_LOGOUT_REQ);
}

var sendTableInfoReq = function () {
    var packet = createPacket("proto.game.ReqRoomInfo");
    packet.uid = ZJHModel.getInstance().uid;
    packet.skey = ZJHModel.getInstance().UserKey;
    packet.vid = 101;
    sendPacket(packet, CMD.CLIENT_MJ_HOME_RETURN);
}

var sendUpTable = function () {
    var packet = createPacket("proto.login.ReqTableAction");
    packet.uid = ZJHModel.getInstance().uid;
    packet.roomid = MJModel.curRoomID;
    sendPacket(packet, CMD.CLIENT_UPTABLE_APPLY_REQ);
}

var sendDownTable = function () {
    var packet = createPacket("proto.login.ReqTableAction");
    packet.uid = ZJHModel.getInstance().uid;
    packet.roomid = MJModel.curRoomID;
    sendPacket(packet, CMD.CLIENT_DOWNTABLE_REQ);
}

var sendReady = function () {
    var packet = createPacket("proto.game.ReqAction");
    packet.action = 2;
    sendPacket(packet, CMD.CLIENT_MJ_USER_ACTION);
}

var sendHeartbeat = function () {
    var packet = createPacket("proto.login.ReqHeatBeat");
    packet.state = 1;
    sendPacket(packet, CMD.CLINET_HEART_BEAT_REQ);
}

var sendEndRoomREQ = function (v) {
    var packet = createPacket("proto.game.ReqChooseDisband");
    packet.Choosestate = v;
    sendPacket(packet, CMD.CLIENT_DISSOLVE_ROOM_REQ);
}

var sendEndRoomResult = function (v) {
    var packet = createPacket("proto.game.ReqChooseDisband");
    packet.Choosestate = v;
    sendPacket(packet, CMD.CLIENT_DISSOLVE_ACTION_REQ);
}

var sendOperateCard = function (type, card) {
    var packet = createPacket("proto.game.ReqOperateResult");
    packet.Type = type;
    packet.Card = card;
    sendPacket(packet, CMD.CLIENT_MJ_OPERATE_CARD);
}

var sendChangeCard = function (v, cardlist) {
    var packet = createPacket("proto.game.ReqChangeCard");
    packet.ischange = v;
    packet.Card = [];
    if (v == 1) {
        for (var i in cardlist) {
            packet.Card.push(cardlist[i]);
        }
    }
    sendPacket(packet, CMD.CLIENT_MJ_CHANGE_CARD);
}

var sendLackCard = function (v) {
    var packet = createPacket("proto.game.ReqChooseLackType");
    packet.Type = v;
    sendPacket(packet, CMD.CLIENT_MJ_CHOOSE_LACKTYPE);
}

var sendPiaoNums = function (v) {
    var packet = createPacket("proto.game.scmj.ReqChoosePiao");
    packet.PiaoNum = v;
    sendPacket(packet, CMD.CLIENT_MJ_CHOOSE_PIAO);
}

var sendGaNums = function (v) {
    var packet = createPacket("proto.game.hnmahjonggame.ReqChoosePiao");
    packet.PiaoNum = v;
    sendPacket(packet , CMD.CLIENT_MJ_CHOOSE_PIAO);
}

var sendListenType = function (type) {
    var packet = createPacket("proto.game.scmj.ReqChooseListen");
    packet.ListenType = type;
    sendPacket(packet, CMD.CLIENT_MJ_LISTEN_TYPE);
}

var sendUpTableInfoReq = function (start_index, end_index) {
    var packet = createPacket("proto.login.AckUpTableInfoReq");
    packet.start_index = start_index;
    packet.end_index = end_index;
    sendPacket(packet, CMD.CLIENT_UPTABLE_INFO_REQ);
}

var sendOutCard = function (card) {
    var packet = createPacket("proto.game.ReqOutCard");
    packet.Card = card;
    sendPacket(packet, CMD.CLIENT_MJ_OUT_CARD);
}

var sendBigface = function (seatid,target_seatid,type) {
    var packet = createPacket("proto.login.ReqEmotion");
    packet.seatid = seatid;
    packet.target_seatid = target_seatid;
    packet.type = type;
    sendPacket(packet, CMD.CLIENT_EMOTION_REQ);
}