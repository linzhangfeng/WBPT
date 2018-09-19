var MJAction = cc.Layer.extend({
    tag: "MJAction",
    callback: null,
    uptable: null,
    ready: null,
    weiguan: null,
    sound: null,
    tingpai: null,
    varOutCard: null,
    paidui: null,
    actions: {},
    ctor: function () {
        this._super();

        var size = cc.director.getWinSize();
        var self = this;
        {
            var btn = new ccui.Button();
            btn.loadTextures(getResPath("RoomMJ/action/pass1.png"), getResPath("RoomMJ/action/pass2.png"), "");
            this.actions["pass"] = btn;
            btn.setName("pass");
            btn.addClickEventListener(function (sender, evt) {
                self.btnCallback(sender);
            });
            btn.setVisible(false);
            this.addChild(btn);
        }

        {
            var btn = new ccui.Button();
            btn.loadTextures(getResPath("RoomMJ/action/hu1.png"), getResPath("RoomMJ/action/hu2.png"), "");
            this.actions["hu"] = btn;
            btn.setName("hu");
            btn.addClickEventListener(function (sender, evt) {
                self.btnCallback(sender);
            });
            btn.setVisible(false);
            this.addChild(btn);
        }

        {
            var btn = new ccui.Button();
            btn.loadTextures(getResPath("RoomMJ/action/gang1.png"), getResPath("RoomMJ/action/gang2.png"), "");
            this.actions["gang"] = btn;
            btn.setName("gang");
            btn.addClickEventListener(function (sender, evt) {
                self.btnCallback(sender);
            });
            btn.setVisible(false);
            this.addChild(btn);
        }

        {
            var btn = new ccui.Button();
            btn.loadTextures(getResPath("RoomMJ/action/peng1.png"), getResPath("RoomMJ/action/peng2.png"), "");
            this.actions["peng"] = btn;
            btn.setName("peng");
            btn.addClickEventListener(function (sender, evt) {
                self.btnCallback(sender);
            });
            btn.setVisible(false);
            this.addChild(btn);
        }

        {
            var btn = new ccui.Button();
            btn.loadTextures(getResPath("RoomMJ/action/chi1.png"), getResPath("RoomMJ/action/chi2.png"), "");
            this.actions["chi"] = btn;
            btn.setName("chi");
            btn.addClickEventListener(function (sender, evt) {
                self.btnCallback(sender);
            });
            btn.setVisible(false);
            this.addChild(btn);
        }

        {
            var btn = new ccui.Button();
            btn.loadTextures(getResPath("RoomMJ/action/hua_hu1.png"), getResPath("RoomMJ/action/hua_hu2.png"), "");
            this.actions["huaHu"] = btn;
            btn.setName("huaHu");
            btn.addClickEventListener(function (sender, evt) {
                self.btnCallback(sender);
            });
            btn.setVisible(false);
            this.addChild(btn);

        }

        {
            this.uptable = new ccui.Button();
            this.uptable.loadTextures(getResPath("RoomMJ2/uptable1.png"), getResPath("RoomMJ2/uptable2.png"), "");
            this.uptable.setPosition(cc.p(size.width - 14 - this.uptable.getContentSize().width / 2, 10 + this.uptable.getContentSize().height / 2));
            this.uptable.setName("uptable");
            this.uptable.setVisible(false);
            this.uptable.addClickEventListener(function (sender, evt) {
                self.btnCallback(sender);
            });
            this.addChild(this.uptable);
        }

        {
            this.tingpai = new ccui.Button();
            this.tingpai.loadTextures(getResPath("RoomMJ2/tingpai1.png"), getResPath("RoomMJ2/tingpai2.png"), "");
            this.tingpai.setPosition(cc.p(this.uptable.getPosition().x + 30, this.uptable.getPosition().y));
            this.tingpai.setName("tingpai");
            this.tingpai.setVisible(false);
            this.tingpai.addClickEventListener(function (sender, evt) {
                self.btnCallback(sender);
            });
            this.addChild(this.tingpai);
        }

        {
            this.autoOutCard = new ccui.Button();
            this.autoOutCard.loadTextures(getResPath("RoomMJ2/auto_hu_sound_bg1.png"), getResPath("RoomMJ2/auto_hu_sound_bg2.png"), "");
            this.autoOutCard.setPosition(cc.p(size.width - 5 - this.autoOutCard.getContentSize().width / 2, this.uptable.getPosition().y + this.uptable.getContentSize().height / 2 + 10 + this.autoOutCard.getContentSize().height / 2));
            this.autoOutCard.setName("auto_out_card");
            this.autoOutCard.setVisible(false);
            this.autoOutCard.addClickEventListener(function (sender, evt) {
                self.btnCallback(sender);
            });
            this.addChild(this.autoOutCard);

            var huTip = new cc.Sprite(getResPath("RoomMJ2/auto_hu_tip.png"));
            huTip.setPosition(cc.p(this.autoOutCard.getContentSize().width - 15 - huTip.getContentSize().width / 2, this.autoOutCard.getContentSize().height / 2));
            this.autoOutCard.addChild(huTip);

            var huOkBg = new cc.Sprite(getResPath("RoomMJ2/auto_hu_ok_bg.png"));
            huOkBg.setPosition(cc.p(15 + huOkBg.getContentSize().width / 2, this.autoOutCard.getContentSize().height / 2));
            this.autoOutCard.addChild(huOkBg);

            var ok = new cc.Sprite(getResPath("RoomMJ2/auto_hu_ok.png"));
            ok.setPosition(cc.p(35, this.autoOutCard.getContentSize().height / 2 + 5));
            ok.setName("ok");
            ok.setVisible(false);
            this.autoOutCard.addChild(ok);
        }

        {
            this.sound = new ccui.Button();
            this.sound.loadTextures(getResPath("RoomMJ2/sound_bg1.png"), getResPath("RoomMJ2/sound_bg2.png"), "");
            this.sound.setPosition(cc.p(size.width - 5 - this.autoOutCard.getContentSize().width / 2, this.autoOutCard.getPosition().y + this.autoOutCard.getContentSize().height / 2 + 8 + this.sound.getContentSize().height / 2));
            this.sound.setName("sound");

            this.sound.addTouchEventListener(function (sender, evt) {

                if (evt == cc.EventTouch.EventCode.BEGAN) {
                    PlatformHelper.startRecord();
                }
                else if (evt == cc.EventTouch.EventCode.ENDED || evt == cc.EventTouch.EventCode.CANCELLED) {
                    PlatformHelper.stopRecord();
                }

            });
            this.addChild(this.sound);
        }

        {
            this.ready = new ccui.Button();
            this.ready.loadTextures(getResPath("RoomMJ/action/ready1.png"), getResPath("RoomMJ/action/ready2.png"), "");
            this.ready.setPosition(cc.p(size.width / 2, 182));
            this.ready.setName("ready");
            this.ready.addClickEventListener(function (sender, evt) {
                self.btnCallback(sender);
            });
            this.addChild(this.ready);
        }

        {
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_YNMJ_DZ) {

                var btn = new ccui.Button();
                btn.loadTextures(getResPath("RoomMJ/action/baoting1.png"), getResPath("RoomMJ/action/baoting2.png"), "");
                this.actions["baopai"] = btn;
                btn.setName("baopai");
                btn.addClickEventListener(function (sender, evt) {
                    self.btnCallback(sender);
                });
                btn.setVisible(false);
                this.addChild(btn);

            }
        }

        {
            var btn = new ccui.Button(getResPath("RoomMJ/hunan/buzhang1.png"), getResPath("RoomMJ/hunan/buzhang2.png"));
            this.actions["buzhang"] = btn;
            btn.setName("buzhang");
            btn.addClickEventListener(function (sender, evt) {
                self.btnCallback(sender);
            });
            btn.setVisible(false);
            this.addChild(btn);
        }

        {
            var btn = new ccui.Button(getResPath("RoomMJ/hunan/yaopai1.png"), getResPath("RoomMJ/hunan/yaopai2.png"));
            this.actions["yaopai"] = btn;
            btn.setName("yaopai");
            btn.addClickEventListener(function (sender, evt) {
                self.btnCallback(sender);
            });

            var haidiTip = new cc.Sprite(getResPath("RoomMJ/hunan/haidipai_tip.png"));
            haidiTip.setAnchorPoint(cc.p(0.5 , 0));
            haidiTip.setPosition(cc.p(btn.getContentSize().width/2 , btn.getContentSize().height - 20));
            btn.addChild(haidiTip);

            btn.setVisible(false);
            this.addChild(btn);
        }

        {
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
                var baoting = new ccui.Button();
                baoting.loadTextures(getResPath("RoomMJ/action/baoting_bg.png"), getResPath("RoomMJ/action/baoting_bg.png"), "");
                baoting.setPosition(cc.p(size.width / 2, 220));
                baoting.setName("baoting");
                baoting.setVisible(false);
                baoting.addClickEventListener(function (sender, evt) {
                    self.btnCallback(sender);
                });
                this.addChild(baoting);

                var text = new cc.Sprite(getResPath("RoomMJ/action/baoting_text.png"));
                text.setAnchorPoint(cc.p(0.5, 0.5));
                text.setPosition(cc.p(baoting.getContentSize().width / 2, baoting.getContentSize().height / 2));
                baoting.addChild(text);
                this.actions["baoting"] = baoting;
            }
        }

        {
            this.paidui = new ccui.Button();
            this.paidui.loadTextures("RoomPublic/public2/action/paidui1.png", "RoomPublic/public2/action/paidui2.png", "");
            this.paidui.setPosition(cc.p(size.width - 70, 570));
            this.paidui.setName("paidui");
            this.paidui.setVisible(false);
            this.paidui.addClickEventListener(function (sender, evt) {
                self.btnCallback(sender);
            });
            this.addChild(this.paidui);

            var duilieBg = new cc.Scale9Sprite("RoomPublic/public2/action/bg_duilie.png");
            duilieBg.setName("paiduibg");
            duilieBg.setAnchorPoint(cc.p(0.5, 0.5));
            duilieBg.setPosition(cc.p(this.paidui.getContentSize().width / 2, -10));
            this.paidui.addChild(duilieBg);

            var duilietext = new ccui.Text("不在队列中", "", 22);
            duilietext.setPosition(cc.p(this.paidui.getContentSize().width / 2, -10));
            duilietext.setTextColor(cc.color(240, 203, 91));
            duilietext.setName("paiduitext");
            duilietext.setAnchorPoint(cc.p(0.5, 0.5));
            this.paidui.addChild(duilietext);
            this.actions["paidui"] = this.paidui;
            duilieBg.width = duilietext.getContentSize().width + 20;
            duilieBg.height = duilietext.getContentSize().height + 10;
        }
    },
    setCallback: function (callback) {
        this.callback = callback;
    },
    btnCallback: function (ref) {

        var n = ref;
        var name = n.getName();
        Log(this, "btnCallback:" + name);
        var self = this;
        if (name == "pass" || name == "hu" || name == "gang" || name == "peng" || name == "chi" || name == "huaHu" || name == "buzhang" || name == "yaopai") {
            var type = 0;
            if (name == "pass") type = ActionType.TYPE_PASS;
            if (name == "hu") type = ActionType.TYPE_HU;
            if (name == "peng") type = ActionType.TYPE_PENG;
            if (name == "huaHu") type = ActionType.TYPE_HUA_HU;
            if (name == "yaopai") type = ActionType.TYPE_HAID_YAO;
            if (name == "gang") {
                var op = MJModel.curOperate;

                if ((op & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG || (op & ActionType.TYPE_WANGANG) == ActionType.TYPE_WANGANG) {
                    if (MJModel.gang_info.length > 1) {
                        var selectLayer = new ChooseGang();
                        selectLayer.setName("ChooseGang_selectLayer");
                        selectLayer.setPosition(cc.p(n.getPosition().x + 0, n.getPosition().y + 50));
                        selectLayer.setCallback(function (index) {
                            sendOperateCard(MJModel.gang_info[index].state, MJModel.gang_info[index].curCard);

                            self.setOperator(-1);
                            self.removeChildByName("ChooseGang_selectLayer");
                        });
                        this.addChild(selectLayer, 10000);
                        return;
                    }
                    else if (MJModel.gang_info.length > 0)
                    {
                        MJModel.curOperateCard = MJModel.gang_info[0].curCard;
                    }
                    else
                    {
                        return;
                    }
                }

                type = ActionType.TYPE_ZHIGANG;

                if ((op & ActionType.TYPE_WANGANG) == ActionType.TYPE_WANGANG) {
                    type = ActionType.TYPE_WANGANG;
                }
                else if ((op & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG) {
                    type = ActionType.TYPE_ANGANG;
                }
            }
            if(name == "buzhang"){
                if(MJModel.bu_info.length > 1){
                    var selectLayer = new ChooseGang();
                    selectLayer.setName("ChooseGang_selectLayer");
                    selectLayer.setPosition(cc.p(n.getPosition().x + 0, n.getPosition().y + 50));
                    selectLayer.setCallback(function (index) {
                        sendOperateCard(ActionType.TYPE_BUZHANG , MJModel.bu_info[index].curCard);
                        self.setOperator(-1);
                        self.removeChildByName("ChooseGang_selectLayer");
                    });
                    this.addChild(selectLayer, 10000);
                    return;
                }else if(MJModel.bu_info.length > 0){
                    MJModel.curOperateCard = MJModel.bu_info[0].curCard;
                }
                type = ActionType.TYPE_BUZHANG;
            }
            if (name == "chi") {
                var op = MJModel.curOperate;
                var op_nums = 0;
                var op_type = ActionType.TYPE_LEFT_CHI;
                if ((op & ActionType.TYPE_LEFT_CHI) == ActionType.TYPE_LEFT_CHI) {
                    op_nums++;
                    op_type = ActionType.TYPE_LEFT_CHI;
                }
                if ((op & ActionType.TYPE_CENTER_CHI) == ActionType.TYPE_CENTER_CHI) {
                    op_nums++;
                    op_type = ActionType.TYPE_CENTER_CHI;
                }
                if ((op & ActionType.TYPE_RIGHT_CHI) == ActionType.TYPE_RIGHT_CHI) {
                    op_nums++;
                    op_type = ActionType.TYPE_RIGHT_CHI;
                }

                if (op_nums > 1) {
                    var selectLayer = new ChooseChi();
                    selectLayer.setName("ChooseGang_selectLayer");
                    selectLayer.setPosition(cc.p(n.getPosition().x, n.getPosition().y + 50));
                    selectLayer.setCallback(function (type) {
                        sendOperateCard(type, MJModel.curOperateCard);
                        self.setOperator(-1);
                        self.removeChildByName("ChooseGang_selectLayer");
                    });
                    this.addChild(selectLayer, 10000);
                    return;
                }
                type = op_type;
            }
            var card = MJModel.curOperateCard;
            sendOperateCard(type, card);
            this.setOperator(-1);
            return;
        }
        else if (name == "tingpai") {
            var selectLayer = cc.director.getRunningScene().getChildByName("tingpai_layer");

            if (selectLayer) {
                selectLayer.removeFromParent(true);
                return;
            }
            selectLayer = new TingPaiLayer();
            selectLayer.show2(MJModel.final_listen_info);
            selectLayer.setPosition(cc.p(n.getPosition().x - selectLayer.getContentSize().width / 2 + 40, n.getPosition().y + 30 + 20));
            selectLayer.setName("tingpai_layer");
            cc.director.getRunningScene().addChild(selectLayer, 10000);
            return;
        }
        else if (name == "baopai") {
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_YNMJ_DZ) {
                sendListenType(MJModel.scmj_listenType);
                this.setOperator(-1);
            }
        }
        else if (name == "baoting") {
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
                //报听操作
                sendOperateCard(ActionType.TYPE_BAOTING, 0);
                this.setOperator(-1);
            }
        }
        else if (name == "paidui") {
            if (MJModel.isZhuanZhuan) {
                //获取排队列表
                sendUpTableInfoReq(0, 100);

                var roomPlayerQueue = this.getChildByName("playerqueue_layer");
                if (!roomPlayerQueue) {
                    roomPlayerQueue = new MJPlayerQueue();
                    roomPlayerQueue.setName("playerqueue_layer");
                    this.addChild(roomPlayerQueue, 1000000);
                    roomPlayerQueue.setRoomId(MJModel.curRoomID);
                    roomPlayerQueue.setSeatId(MJModel.seatid);
                }
                roomPlayerQueue.setVisible(true);
            }
        }

        if (this.callback) this.callback(name);
    },

    setUpTableVisible: function (v) {
        if (this.uptable) this.uptable.setVisible(v);
    },

    setReadyVisible: function (v) {
        if (this.ready) this.ready.setVisible(v);
    },

    checkAutoOutCardVis: function () {
        this.setAutoOutCardVis(MJModel.final_listen_info.length >= 1);
    },

    setAutoOutCardVis: function (v) {
        if (this.autoOutCard) {
            this.autoOutCard.setVisible(v);
            if (v == true) {
                this.autoOutCard.getChildByName("ok").setVisible(MJModel.isAutoOutCard);
            }
        }
    },

    isHuOpVis: function () {
        if (this.actions["hu"]) {
            return this.actions["hu"].isVisible();
        }
        return false;
    },

    setOperator: function (operat) {
        cc.log("operat" + operat);
        if (operat == -1 || operat == 0 || operat == null || operat == undefined) {
            if (this.actions["pass"]) this.actions["pass"].setVisible(false);
            if (this.actions["hu"]) this.actions["hu"].setVisible(false);
            if (this.actions["gang"]) this.actions["gang"].setVisible(false);
            if (this.actions["peng"]) this.actions["peng"].setVisible(false);
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
                if (this.actions["baoting"]) this.actions["baoting"].setVisible(false);
            }
            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_YNMJ_DZ) {
                if (this.actions["baopai"]) this.actions["baopai"].setVisible(false);
            }

            if (this.actions["chi"]) this.actions["chi"].setVisible(false);
            if (this.actions["huaHu"]) this.actions["huaHu"].setVisible(false);
            if (this.actions["buzhang"]) this.actions["buzhang"].setVisible(false);
            if (this.actions["yaopai"]) this.actions["yaopai"].setVisible(false);

            return;
        }

        MJModel.curOperate = operat;

        var size = cc.director.getWinSize();
        var bx = size.width - 184;
        var by = 174;
        var dx = 116;
        var index = 0;

        if (this.actions["pass"]) {
            this.actions["pass"].setVisible(true);
            this.actions["pass"].setPosition(cc.p(bx, by));
            index++;
        }

        if ((operat & ActionType.TYPE_HU) == ActionType.TYPE_HU) {
            if (this.actions["hu"]) {
                this.actions["hu"].setVisible(true);
                this.actions["hu"].setPosition(cc.p(bx - dx * index, by));
                index++;
            }
        }

        if ((operat & ActionType.TYPE_ZHIGANG) == ActionType.TYPE_ZHIGANG || (operat & ActionType.TYPE_WANGANG) == ActionType.TYPE_WANGANG || (operat & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG) {
            if (this.actions["gang"]) {
                this.actions["gang"].setVisible(true);
                this.actions["gang"].setPosition(cc.p(bx - dx * index, by));
                index++;
            }
        }

        if ((operat & ActionType.TYPE_PENG) == ActionType.TYPE_PENG) {
            if (this.actions["peng"]) {
                this.actions["peng"].setVisible(true);
                this.actions["peng"].setPosition(cc.p(bx - dx * index, by));
                index++;
            }
        }

        if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_YNMJ_DZ) {
            if ((operat & ActionType.TYPE_BAOTING) == ActionType.TYPE_BAOTING) {
                if (this.actions["baopai"]) {
                    switch (MJModel.scmj_listenType) {
                        case 1:
                            this.actions["baopai"].loadTextures(getResPath("RoomMJ/action/baoting1.png"), getResPath("RoomMJ/action/baoting2.png") , "");
                            break;

                        case 2:
                            this.actions["baopai"].loadTextures(getResPath("RoomMJ/action/tangpai1.png"), getResPath("RoomMJ/action/tangpai2.png") , "");
                            break;

                        case 3:
                            this.actions["baopai"].loadTextures(getResPath("RoomMJ/action/baopai1.png"), getResPath("RoomMJ/action/baopai2.png") , "");
                            break;
                    }

                    this.actions["baopai"].setVisible(true);
                    this.actions["baopai"].setPosition(cc.p(bx - dx * index, by));
                    index++;
                }
            }
        }

        if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
            if ((operat & ActionType.TYPE_BAOTING) == ActionType.TYPE_BAOTING) {
                if (this.actions["baoting"]) {
                    this.actions["baoting"].setVisible(true);
                    this.actions["baoting"].setPosition(cc.p(bx - dx * index, by));
                    index++;
                }
            }
        }

        if(MJModel.curGameType != ROOM_TYPE.ROOM_TYPE_SCMJ_DZ && MJModel.curGameType != ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ && MJModel.curGameType != ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ && MJModel.curGameType != ROOM_TYPE.ROOM_TYPE_YNMJ_DZ){
            if ((operat & ActionType.TYPE_LEFT_CHI) == ActionType.TYPE_LEFT_CHI || (operat & ActionType.TYPE_CENTER_CHI) == ActionType.TYPE_CENTER_CHI || (operat & ActionType.TYPE_RIGHT_CHI) == ActionType.TYPE_RIGHT_CHI) {
                if (this.actions["chi"]) {
                    this.actions["chi"].setVisible(true);
                    this.actions["chi"].setPosition(cc.p(bx - dx * index, by));
                    index++;
                }
            }
        }

        if((operat & ActionType.TYPE_HUA_HU) == ActionType.TYPE_HUA_HU){
            if (this.actions["huaHu"]) {
                this.actions["huaHu"].setVisible(true);
                this.actions["huaHu"].setPosition(cc.p(bx - dx * index, by));
                index++;
            }
        }

        if((operat & ActionType.TYPE_BUZHANG) == ActionType.TYPE_BUZHANG){
            if (this.actions["buzhang"]) {
                this.actions["buzhang"].setVisible(true);
                this.actions["buzhang"].setPosition(cc.p(bx - dx * index, by));
                index++;
            }
        }

        if((operat & ActionType.TYPE_HAID_YAO) == ActionType.TYPE_HAID_YAO){
            if (this.actions["yaopai"]) {
                this.actions["yaopai"].setVisible(true);
                this.actions["yaopai"].setPosition(cc.p(bx - dx * index, by));
                index++;
            }
        }

    },
    setBaoTingOperator: function (Gangoperat) {
        var size = cc.director.getWinSize();

        var bx = size.width - 184;
        var by = 174;
        var dx = 116;
        var index = 0;
        if (this.actions["pass"]) {
            this.actions["pass"].setVisible(true);
            this.actions["pass"].setPosition(cc.p(bx, by));
            index++;
        }


        if ((Gangoperat & ActionType.TYPE_ZHIGANG) == ActionType.TYPE_ZHIGANG || (Gangoperat & ActionType.TYPE_WANGANG) == ActionType.TYPE_WANGANG || (Gangoperat & ActionType.TYPE_ANGANG) == ActionType.TYPE_ANGANG) {
            if (this.actions["gang"]) {
                this.actions["gang"].setVisible(true);
                this.actions["gang"].setPosition(cc.p(bx - dx * index, by));
                index++;
            }
        }

        if (this.actions["baoting"]) {
            this.actions["baoting"].setVisible(true);
            this.actions["baoting"].setPosition(cc.p(bx - dx * index, by));
            index++;
        }
    },

    setDuiLieStr: function (str) {
        if (!this.paidui) {
            return;
        }
        var paiduitext = this.paidui.getChildByName("paiduitext");
        var paiduibg = this.paidui.getChildByName("paiduibg");
        paiduitext.setString(str);
        paiduibg.setContentSize(cc.size(paiduitext.getContentSize().width + 30, paiduitext.getContentSize().height));
    },

    setPlayerQueueVisible: function (v) {
        var roomPlayerQueue = this.getChildByName("playerqueue_layer");
        if (roomPlayerQueue) {
            roomPlayerQueue.setVisible(v);
        }
    },

    setDuiLieData: function (data) {
        if (!this.paidui) {
            return;
        }
        var str = "不在队列中";

        var status = 0;
        if (data != "") {
            var upTableInfo = parsePacket("proto.login.AckUpTableInfo_UC", data);
            if (upTableInfo.rank == 1) {
                status = 0;
                str = "等待上桌";
            }
            else if (upTableInfo.rank > 1) {
                status = 1;
                str = "队列中";
            }
            else if (MJModel.seatid != -1) {
                str = "游戏中";
                status = 2;
            }
            else {
                str = "不在队列中";
                status = 3;
            }
        }

        this.setDuiLieStr(str);

        var roomPlayerQueue = this.getChildByName("playerqueue_layer");
        if (roomPlayerQueue && roomPlayerQueue.isVisible()) {
            roomPlayerQueue.updateView(data);
            roomPlayerQueue.updateButton(status);
        }
    },

    setDuiLieVisible: function (v) {
        if (this.paidui) {
            this.paidui.setVisible(v);
        }
    },

    checkTingPaiVis: function () {
        this.setTingPaiVis(MJModel.final_listen_info.length >= 1);
    },

    setTingPaiVis: function (v) {
        if (this.tingpai) {
            this.tingpai.setVisible(v);
            if (v == false) cc.director.getRunningScene().removeChildByName("tingpai_layer");
        }
    }
});

var ChooseGang = cc.Layer.extend({
    _callback: null,
    ctor: function () {
        this._super();
        this.ignoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.5, 0.5));

        var self = this;

        var size = MJModel.gang_info.length;
        var card_size = cc.size(65, 96);
        var sp = new ccui.Scale9Sprite(getResPath("RoomMJ/bg.9.png"));
        sp.setAnchorPoint(cc.p(0.5, 0.0));
        sp.setContentSize(cc.size(card_size.width * size + 10 * (1 + size), card_size.height + 20));
        sp.setPosition(cc.p(sp.getContentSize().width / 2, sp.getContentSize().height / 2));
        this.addChild(sp);

        this.setContentSize(sp.getContentSize());

        for (var j = 0; j < size; j++) {
            var gif = MJModel.gang_info[j];
            var layout = new ccui.Layout();
            layout.setTag(j);
            layout.setContentSize(card_size);
            sp.addChild(layout);

            var tingCard = new MJCard();
            tingCard.setValue(gif.curCard, 0, CardType.Card_Hand, 0);
            tingCard.setPosition(cc.p(10 + card_size.width / 2 + (10 + card_size.width) * j, card_size.height / 2 + 10));
            tingCard.setAnchorPoint(cc.p(0.5, 0.5));
            tingCard.setScale(0.85);
            sp.addChild(tingCard);

            layout.setPosition(tingCard.getPosition());
            layout.setTouchEnabled(true);
            layout.addClickEventListener(function (sender, eve) {
                if (self._callback) self._callback(sender.getTag());
            });
        }
    },

    setCallback: function (callback) {
        this._callback = callback;
    },

    onEnter: function () {
        this._super();
        // 部分事件监听
        var self = this;
        var listener = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches: true,
            onTouchBegan: function (touch, event) {
                return self.onTouchBegan(touch, event);
            },
            onTouchEnded: function (touch, event) {
                self.onTouchEnded(touch, event);
            }
        });
        if (listener)cc.eventManager.addListener(listener, this);
    },
    _isTouchInside: function (touch) {
        var touchPoint = touch.getLocation();
        var localPos = this.convertToNodeSpace(touchPoint);

        var s = this.getContentSize();
        var rc = cc.rect(0, 0, s.width, s.height)
        var isTouched = cc.rectContainsPoint(rc, localPos);
        return isTouched;
    },

    onTouchBegan: function (touch, event) {
        return true;
    },

    onTouchEnded: function (touch, event) {
        if (this._isTouchInside(touch)) {
            return;
        }
        this.removeFromParent(true);
    }
});


var ChooseChi = cc.Layer.extend({
    _callback: null,
    ctor: function () {
        this._super();

        this.ignoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.5, 0.0));
        var self = this;
        var op_nums = 0;
        var op_type_v = [];
        var op = MJModel.curOperate;
        var cur_card = MJModel.curOperateCard;

        if ((op & ActionType.TYPE_LEFT_CHI) == ActionType.TYPE_LEFT_CHI) {
            op_nums++;
            op_type_v.push(ActionType.TYPE_LEFT_CHI);
        }
        if ((op & ActionType.TYPE_CENTER_CHI) == ActionType.TYPE_CENTER_CHI) {
            op_nums++;
            op_type_v.push(ActionType.TYPE_CENTER_CHI);
        }
        if ((op & ActionType.TYPE_RIGHT_CHI) == ActionType.TYPE_RIGHT_CHI) {
            op_nums++;
            op_type_v.push(ActionType.TYPE_RIGHT_CHI);
        }

        var card_size = cc.size(65, 96);
        var item_size = cc.size(65 * 2 + 30, 96 + 20);
        var size = cc.size((item_size.width + 20) * op_nums, item_size.height);
        this.setContentSize(size);

        var bx = size.width / 2 - (item_size.width + 20) / 2 * (op_nums - 1);
        for (var i = 0; i < op_nums; i++) {
            var opType = op_type_v[i];
            var layout = new ccui.Layout();
            layout.setAnchorPoint(cc.p(0.5, 0.5));
            layout.setContentSize(item_size);
            layout.setTag(opType);
            this.addChild(layout);
            layout.setPosition(cc.p(bx + (item_size.width + 20) * i, size.height / 2));
            layout.setTouchEnabled(true);
            layout.addClickEventListener(function (ref, evt) {
                var lay = ref;
                if (self._callback) self._callback(lay.getTag());
            });

            var sp = new cc.Scale9Sprite(getResPath("RoomMJ/bg.9.png"));
            sp.setAnchorPoint(cc.p(0.5, 0.5));
            sp.setContentSize(item_size);
            sp.setPosition(cc.p(layout.getContentSize().width / 2, layout.getContentSize().height / 2));
            layout.addChild(sp);

            for (var j = 0; j < 2; j++) {
                var card_value = 0;
                if (opType == ActionType.TYPE_LEFT_CHI) {
                    card_value = cur_card + j + 1;
                }
                if (opType == ActionType.TYPE_CENTER_CHI) {
                    card_value = cur_card + 2 * j - 1;
                }
                if (opType == ActionType.TYPE_RIGHT_CHI) {
                    card_value = cur_card + j - 2;
                }
                var tingCard = new MJCard();
                tingCard.setValue(card_value, 0, CardType.Card_Hand, 0);
                tingCard.setPosition(cc.p(item_size.width / 4 * (2 * j + 1), item_size.height / 2));
                tingCard.setAnchorPoint(cc.p(0.5, 0.5));
                tingCard.setScale(0.85);
                sp.addChild(tingCard);
            }
        }
    },

    setCallback: function (callback) {
        this._callback = callback;
    },

    onEnter: function () {
        this._super();
        // 部分事件监听
        var self = this;
        var listener = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches: true,
            onTouchBegan: function (touch, event) {
                return self.onTouchBegan(touch, event);
            },
            onTouchEnded: function (touch, event) {
                self.onTouchEnded(touch, event);
            }
        });
        if (listener)cc.eventManager.addListener(listener, this);
    },
    _isTouchInside: function (touch) {
        var touchPoint = touch.getLocation();
        var localPos = this.convertToNodeSpace(touchPoint);

        var s = this.getContentSize();
        var rc = cc.rect(0, 0, s.width, s.height)
        var isTouched = cc.rectContainsPoint(rc, localPos);
        return isTouched;
    },

    onTouchBegan: function (touch, event) {
        return true;
    },

    onTouchEnded: function (touch, event) {
        if (this._isTouchInside(touch)) {
            return;
        }
        this.removeFromParent(true);
    }
});

var TingPaiLayer = cc.Layer.extend({
    tag: "TingPaiLayer",
    ctor: function () {
        Log(this, "TingPaiLayer:init");
        this._super();
        this.ignoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.5, 0.0));
    },
    onEnter: function () {
        this._super();
        // 部分事件监听
        var self = this;
        var listener = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches: true,
            onTouchBegan: function (touch, event) {
                return self.onTouchBegan(touch, event);
            },
            onTouchEnded: function (touch, event) {
                self.onTouchEnded(touch, event);
            }
        });
        if (listener)cc.eventManager.addListener(listener, this);
    },
    show2: function (list) {
        var size = list.length;
        var card_size = cc.size(65, 96);
        var sp = new ccui.Scale9Sprite(getResPath("RoomMJ/bg.9.png"));
        sp.setAnchorPoint(cc.p(0.5, 0.5));
        sp.setContentSize(cc.size(card_size.width * size + 10 * (1 + size), card_size.height + 20 + 45));
        sp.setPosition(cc.p(sp.getContentSize().width / 2, sp.getContentSize().height / 2));
        this.addChild(sp);

        this.setContentSize(sp.getContentSize());

        for (var j = 0; j < size; j++) {
            var huinfo = list[j];

            // var huinfo = Utils.clone(tagHuInfo);
            // huinfo.Card = 1;
            // huinfo.HuFanNum = 1;
            // huinfo.LeftNum = 1;

            var tingCard = new MJCard();
            tingCard.setValue(huinfo.Card, 0, CardType.Card_Hand, 0);
            tingCard.setPosition(cc.p(10 + card_size.width / 2 + (10 + card_size.width) * j, 45 + card_size.height / 2 + 5));
            tingCard.setAnchorPoint(cc.p(0.5, 0.5));
            tingCard.setScale(0.85);
            sp.addChild(tingCard);
            var textlable = new cc.LabelTTF(huinfo.HuFanNum + "番", "Thonburi", 20);
            textlable.setColor(cc.WHITE);
            textlable.setAnchorPoint(cc.p(0.5, 0.5));
            textlable.setPosition(cc.p(tingCard.getPositionX(), 36));
            sp.addChild(textlable);

            textlable = new cc.LabelTTF(huinfo.LeftNum + "张", "Thonburi", 20);
            textlable.setPosition(cc.p(tingCard.getPositionX(), 13));
            textlable.setAnchorPoint(cc.p(0.5, 0.5));
            textlable.setColor(cc.WHITE);
            sp.addChild(textlable);
        }
    },

    _isTouchInside: function (touch) {
        var touchPoint = touch.getLocation();
        var localPos = this.convertToNodeSpace(touchPoint);

        var s = this.getContentSize();
        var rc = cc.rect(0, 0, s.width, s.height)
        var isTouched = cc.rectContainsPoint(rc, localPos);
        return isTouched;
    },

    onTouchBegan: function (touch, event) {
        return true;
    },

    onTouchEnded: function (touch, event) {
        if (this._isTouchInside(touch)) {
            return;
        }

        this.removeFromParent(true);
    }

});