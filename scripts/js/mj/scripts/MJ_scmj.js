var MJChange = cc.Layer.extend(
    {
        tag: "MJChange",
        cancelChoiceBtn: null,
        sureChoiceBtn: null,
        ctor: function () {
            this._super();
            var wsize = cc.director.getWinSize();

            var csize = cc.size(530 , 140);

            this.setIgnoreAnchorPointForPosition(false);
            this.setAnchorPoint(cc.p(0.5 , 0));
            this.setContentSize(csize);
            this.setPosition(cc.p(wsize.width/2 , 158));

            var tipIvSize = cc.size(450 , 45);
            var tipIV = new ccui.ImageView();
            tipIV.loadTexture(getResPath("RoomMJ/text_bg.png"));
            tipIV.setAnchorPoint(cc.p(0.5, 1));
            tipIV.setContentSize(tipIvSize);
            tipIV.ignoreContentAdaptWithSize(false);
            tipIV.setPosition(cc.p(csize.width/2, csize.height));
            this.addChild(tipIV);

            var tip = new ccui.Text("请选择任意要更换的牌(最多3张)", "", 25);
            tip.setTextColor(cc.color(0xff, 0xff, 0xff, 255));
            tip.setAnchorPoint(cc.p(0.5, 0.5));
            tip.setPosition(cc.p(tipIvSize.width/2, tipIvSize.height/2));
            tipIV.addChild(tip);


            this.cancelChoiceBtn = new ccui.Button(getResPath("RoomMJ/scmj/cancel_choice1.png"),getResPath("RoomMJ/scmj/cancel_choice2.png"));
            this.cancelChoiceBtn.setName("cancelChoice");
            this.cancelChoiceBtn.setAnchorPoint(cc.p(0, 0));
            this.cancelChoiceBtn.setPosition(cc.p(0, 0));
            this.addChild(this.cancelChoiceBtn);

            this.sureChoiceBtn = new ccui.Button(getResPath("RoomMJ/scmj/sure_choice1.png"),getResPath("RoomMJ/scmj/sure_choice2.png"));
            this.sureChoiceBtn.setName("sureChoice");
            this.sureChoiceBtn.setAnchorPoint(cc.p(1, 0));
            this.sureChoiceBtn.setPosition(cc.p(csize.width, 0));
            this.addChild(this.sureChoiceBtn);
        },
        addClickEventListener: function (callback) {
            this.cancelChoiceBtn.addClickEventListener(callback);
            this.sureChoiceBtn.addClickEventListener(callback);
        },
    });

var MJPaiSe = cc.Layer.extend(
    {
        tag: "MJPaiSe",
        tiaoBtn: null,
        tongBtn: null,
        wanBtn: null,
        goodPaiSe: -1,
        opAnim: null,
        ctor: function () {
            this._super();
            var wsize = cc.director.getWinSize();

            var csize = cc.size(430 , 110);

            this.setIgnoreAnchorPointForPosition(false);
            this.setAnchorPoint(cc.p(0.5 , 0));
            this.setContentSize(csize);
            this.setPosition(cc.p(wsize.width/2 , 158));

            this.opAnim = new cc.Sprite(getResPath("RoomMJ/scmj/op_anim_1.png"));
            this.opAnim.setAnchorPoint(cc.p(0.5, 0.5));
            var anim = new cc.Animation();
            for(var i = 1 ; i <= 7 ; i++){
                anim.addSpriteFrameWithFile(getResPath("RoomMJ/scmj/op_anim_") + i +".png");
            }
            anim.setDelayPerUnit(0.1);
            var action = new cc.Animate(anim);
            this.opAnim.runAction(action.repeatForever());
            this.addChild(this.opAnim);

            this.tiaoBtn = new ccui.Button(getResPath("RoomMJ/scmj/tiao1.png"),getResPath("RoomMJ/scmj/tiao2.png"));
            this.tiaoBtn.setName("tiaoChoice");
            this.tiaoBtn.setAnchorPoint(cc.p(0, 0));
            this.tiaoBtn.setPosition(cc.p(0, 0));
            this.addChild(this.tiaoBtn);

            this.tongBtn = new ccui.Button(getResPath("RoomMJ/scmj/tong1.png"),getResPath("RoomMJ/scmj/tong2.png"));
            this.tongBtn.setName("tongChoice");
            this.tongBtn.setAnchorPoint(cc.p(0, 0));
            this.tongBtn.setPosition(cc.p(csize.width/2 - (this.tongBtn.getContentSize()).width/2, 0));
            this.addChild(this.tongBtn);

            this.wanBtn = new ccui.Button(getResPath("RoomMJ/scmj/wan1.png"),getResPath("RoomMJ/scmj/wan2.png"));
            this.wanBtn.setName("wanChoice");
            this.wanBtn.setAnchorPoint(cc.p(0, 0));
            this.wanBtn.setPosition(cc.p(csize.width - (this.wanBtn.getContentSize()).width, 0));
            this.addChild(this.wanBtn);
        },
        setGoodPaiSe: function (goodPaiSe) {
            this.goodPaiSe = goodPaiSe;
            cc.log("goodPaiSe : " + goodPaiSe);

            var temp = null;

            if (goodPaiSe == MJ_TYPE.MJ_TYPE_TIAO) {
                temp = this.tiaoBtn;
            }else if(goodPaiSe == MJ_TYPE.MJ_TYPE_TONG){
                temp = this.tongBtn;
            }else if (goodPaiSe == MJ_TYPE.MJ_TYPE_WAN){
                temp = this.wanBtn;
            }

            if (temp) {
                var point = temp.getPosition();
                var size = temp.getContentSize();
                this.opAnim.setVisible(true);
                this.opAnim.setPosition(cc.p(point.x + size.width/2 , point.y + size.height/2));
            }else{
                this.opAnim.setVisible(false);
            }

        },
        addClickEventListener: function (callback) {
            this.tiaoBtn.addClickEventListener(callback);
            this.tongBtn.addClickEventListener(callback);
            this.wanBtn.addClickEventListener(callback);
        },
    });

var MJPiao = cc.Layer.extend(
    {
        tag: "MJPiao",
        piao1: null,
        piao2: null,
        piao5: null,
        noPiao: null,
        ctor: function () {
            this._super();
            this.piao1 = new ccui.Button(getResPath("RoomMJ/scmj/piao1_1.png"),getResPath("RoomMJ/scmj/piao1_2.png"));
            this.piao1.setPosition(cc.p(370 + this.piao1.getContentSize().width/2, 190 + this.piao1.getContentSize().height/2));
            this.piao1.setName("piao1");
            this.addChild(this.piao1);

            this.piao2 = new ccui.Button(getResPath("RoomMJ/scmj/piao2_1.png"),getResPath("RoomMJ/scmj/piao2_2.png"));
            this.piao2.setPosition(cc.p(this.piao1.getPositionX() + this.piao1.getContentSize().width + 45, this.piao1.getPositionY()));
            this.piao2.setName("piao2");
            this.addChild(this.piao2);

            this.piao5 = new ccui.Button(getResPath("RoomMJ/scmj/piao5_1.png"),getResPath("RoomMJ/scmj/piao5_2.png"));
            this.piao5.setPosition(cc.p(this.piao2.getPositionX() + this.piao1.getContentSize().width+ 45, this.piao1.getPositionY()));
            this.piao5.setName("piao5");
            this.addChild(this.piao5);

            this.noPiao = new ccui.Button(getResPath("RoomMJ/scmj/nopiao1.png"),getResPath("RoomMJ/scmj/nopiao2.png"));
            this.noPiao.setPosition(cc.p(this.piao5.getPositionX() + this.piao1.getContentSize().width+ 45, this.piao1.getPositionY()));
            this.noPiao.setName("noPiao");
            this.addChild(this.noPiao);
        },
        addClickEventListener: function (callback) {
            this.piao1.addClickEventListener(callback);
            this.piao2.addClickEventListener(callback);
            this.piao5.addClickEventListener(callback);
            this.noPiao.addClickEventListener(callback);
        },
    });

var MJSCEndCheck = cc.LayerColor.extend(
    {
        tag: "MJSCEndCheck",

        ctor: function () {
            this._super(cc.color(0, 0, 0, 100));

            //全屏蒙层
            var size = cc.director.getWinSize();
            this.setContentSize(size);

            //处理吞噬事件
            var ev = cc.EventListener.create({
                event: cc.EventListener.TOUCH_ONE_BY_ONE,
                swallowTouches:true,
                onTouchBegan:function(touch, event){
                    return true;
                },
            });
            if (ev)cc.eventManager.addListener(ev , this);
        },
        loadData: function (data) {
            var ackGameEnd = parsePacket("proto.game.AckGameEnd", data);
            //createPacket("proto.game.AckGameEnd");
            // ackGameEnd.isHuazhu.push(0);
            // ackGameEnd.isHuazhu.push(1);
            // ackGameEnd.isHuazhu.push(2);
            //
            // ackGameEnd.isDajiao.push(0);
            // ackGameEnd.isDajiao.push(1);
            // ackGameEnd.isDajiao.push(2);
            //
            // ackGameEnd.ReturnGangScore.push(-10);
            // ackGameEnd.ReturnGangScore.push(-1);
            // ackGameEnd.ReturnGangScore.push(2);

            var huazhuStr = "";
            var dajiaoStr = "";
            //服务器结束指令返回或者缓存在牌局信息
            //中间流局提示
            var size = cc.director.getWinSize();

            if (MJModel.isNeedSearchHuazhuDajiao){
                //流局花猪大叫的人名字
                for (var i = 0 ; i < MJModel.players.length ; i++){
                    var seat_id = i;
                    var pos = MJModel.getPosBySeatid(seat_id);
                    var player = MJModel.players[i];

                    var huaZhu_flag = 0;
                    var dajiao_flag = 0;
                    var returnGangScore = 0;

                    if (ackGameEnd.isHuazhu.length > seat_id){
                        huaZhu_flag = ackGameEnd.isHuazhu[seat_id];
                        if (huaZhu_flag == 1){
                            huazhuStr += player.name+" ";
                        }
                    }

                    if (ackGameEnd.isDajiao.length > seat_id){
                        dajiao_flag = ackGameEnd.isDajiao[seat_id];
                        if (dajiao_flag == 1){
                            dajiaoStr += player.name+" ";
                        }
                    }

                    if (ackGameEnd.ReturnGangScore.length > seat_id){
                        returnGangScore = ackGameEnd.ReturnGangScore[seat_id];
                    }


                    var roomMJUserTips = new RoomMJUserTips();
                    roomMJUserTips.setTips(huaZhu_flag,dajiao_flag,returnGangScore);
                    roomMJUserTips.setPosition(MJConfig.getPlayerPos(pos));
                    this.addChild(roomMJUserTips);
                }


                if (huazhuStr == ""){
                    huazhuStr = "本局没有花猪";
                }else{
                    huazhuStr = "本局花猪玩家 : "+huazhuStr;
                }
                if (dajiaoStr == ""){
                    dajiaoStr = "本局没有大叫";
                }else{
                    dajiaoStr = "本局大叫玩家 : "+dajiaoStr;
                }
            }

            var roomMJLJTips = new RoomMJLJTips();
            roomMJLJTips.setTips(MJModel.isNeedSearchHuazhuDajiao , huazhuStr,dajiaoStr)
            roomMJLJTips.setPosition(cc.p(size.width/2, 312));
            this.addChild(roomMJLJTips);

        },
    });

var RoomMJUserTips = cc.Layer.extend({
    tag: "RoomMJUserTips",
    ctor: function () {
        this._super();
        var size = cc.size(79,100);
        this.setIgnoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.5,0.5));
        this.setContentSize(size);
    },

    setTips: function(huaZhu_flag , dajiao_flag , returnGangScore){
        var size = this.getContentSize();
        //花猪
        if (huaZhu_flag == 1){
            var huazhuTips = new cc.Sprite(getResPath("RoomMJ/scmj/lj/huazhu.png"));
            huazhuTips.setAnchorPoint(cc.p(0.5, 0.5));
            huazhuTips.setPosition(cc.p(size.width/2, size.height/2));
            this.addChild(huazhuTips);
        }


        //大叫
        if (dajiao_flag == 1){
            var dajiaoTips = new cc.Sprite(getResPath("RoomMJ/scmj/lj/dajiao.png"));
            dajiaoTips.setAnchorPoint(cc.p(0.5, 0.5));
            dajiaoTips.setPosition(cc.p(size.width/2, size.height/2));
            this.addChild(dajiaoTips);
        }

        //退杠
        if (returnGangScore < 0){
            var tuigangTips = new cc.Sprite(getResPath("RoomMJ/scmj/lj/tuigang.png"));
            tuigangTips.setAnchorPoint(cc.p(0.5, 0.0));
            tuigangTips.setPosition(cc.p(size.width/2, size.height+6));
            this.addChild(tuigangTips);
        }

    }
});

var RoomMJLJTips = cc.Layer.extend({
    tag: "RoomMJLJTips",
    ctor: function () {
        this._super();
        var size = cc.size(480,228);
        this.setIgnoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.5,0));
        this.setContentSize(size);
    },

    setTips: function (isNeedSearch , huazhu , dajiao) {
        var size = this.getContentSize();
        var liujuTitle = new cc.Sprite(getResPath("RoomMJ/scmj/lj/liuju.png"));
        liujuTitle.setAnchorPoint(cc.p(0.5, 1.0));
        liujuTitle.setPosition(cc.p(size.width/2, size.height));
        this.addChild(liujuTitle);

        if (isNeedSearch){
            var huazhuSearchBg = new cc.Sprite(getResPath("RoomMJ/scmj/lj/search_bg.png"));
            huazhuSearchBg.setAnchorPoint(cc.p(0.5, 1.0));
            huazhuSearchBg.setContentSize(cc.size(size.width, 135));
            huazhuSearchBg.setPosition(cc.p(size.width/2, size.height-liujuTitle.getContentSize().height-8));
            this.addChild(huazhuSearchBg);


            var huazhuSearch = new cc.Sprite(getResPath("RoomMJ/scmj/lj/huazhu_search.png"));
            huazhuSearch.setAnchorPoint(cc.p(0.5, 1.0));
            huazhuSearch.setPosition(cc.p(huazhuSearchBg.getContentSize().width/2, huazhuSearchBg.getContentSize().height-12));
            huazhuSearchBg.addChild(huazhuSearch);

            //    auto dajiaoLabl = Label::createWithSystemFont("本局大叫玩家 : 阿呆", "Arial", 20);
            var dajiaoLabl = new cc.LabelTTF(dajiao, "Arial", 20);
            dajiaoLabl.setColor(cc.color(255, 177, 55, 255));
            dajiaoLabl.setAnchorPoint(cc.p(0.5, 1.0));
            dajiaoLabl.setPosition(cc.p(huazhuSearch.getContentSize().width/2, -12));
            huazhuSearch.addChild(dajiaoLabl);

            //    auto huazhuLabl = Label::createWithSystemFont("本局没有花猪", "Arial", 20);
            var huazhuLabl = new cc.LabelTTF(huazhu, "Arial", 20);
            huazhuLabl.setColor(cc.color(255, 238, 93, 255));
            huazhuLabl.setAnchorPoint(cc.p(0.5, 1.0));
            huazhuLabl.setPosition(cc.p(dajiaoLabl.getContentSize().width/2, -12));
            dajiaoLabl.addChild(huazhuLabl);
        }

    }
});

