var MJTips = cc.Layer.extend(
    {
        tag: "MJTips",
        hu_tips: [],
        zhongma_tips: [],
        baoting_tips: [],
        zhongNiao_tips: [],
        count: 0,
        tip: null,
        tip_effect: null,
        tip_text: null,
        my_tip: null,
        my_tip_text: null,
        no_dizhu: null,
        callback: null,
        out_card_effect: null,
        ctor: function () {
            this._super();

            var size = cc.director.getWinSize();

            this.ignoreAnchorPointForPosition(false);
            this.setAnchorPoint(cc.p(0.5, 0.5));
            this.setContentSize(cc.size(size.width, size.height));
            var self = this;
            {
                this.tip = new cc.Sprite(getResPath("RoomMJ/text_bg.png"));
                this.tip.setPosition(cc.p(size.width / 2, 274));
                this.tip.setVisible(false);
                this.addChild(this.tip, 10000);

                this.tip_text = new ccui.Text("", "", 30);
                this.tip_text.setTextColor(cc.color(255, 249, 141));
                this.tip_text.setAnchorPoint(cc.p(0.5, 0.5));
                this.tip_text.setPosition(cc.p(this.tip.getContentSize().width / 2, this.tip.getContentSize().height / 2));
                this.tip.addChild(this.tip_text);

                this.tip_effect = new ccui.Text(".", "", 30);
                this.tip_effect.setTextColor(cc.color(255, 249, 141));
                this.tip_effect.setAnchorPoint(cc.p(0.0, 0.5));
                this.tip_effect.setPosition(cc.p(this.tip.getContentSize().width / 2, this.tip.getContentSize().height / 2));
                this.tip.addChild(this.tip_effect);


                this.count = 0;
                var cl = cc.callFunc(function () {

                    self.count++;
                    if (self.count > 2) {
                        self.count = 0;
                    }

                    if (self.count == 0) {
                        self.tip_effect.setString(".");
                    }
                    else if (self.count == 1) {
                        self.tip_effect.setString("..");
                    }
                    else if (self.count == 2) {
                        self.tip_effect.setString("...");
                    }
                });
                this.tip_effect.runAction(cc.sequence(cc.delayTime(0.2), cl).repeatForever());
            }

            {
                this.my_tip = new cc.Sprite(getResPath("RoomMJ/text_bg.png"));
                this.my_tip.setVisible(false);
                this.my_tip.setPosition(cc.p(size.width / 2, 274));
                this.addChild(this.my_tip, 10000);

                this.my_tip_text = new ccui.Text("", "AmericanTypewriter", 25);
                this.my_tip_text.setTextColor(cc.color(255, 249, 141));
                this.my_tip_text.setAnchorPoint(cc.p(0.5, 0.5));
                this.my_tip_text.setPosition(cc.p(this.my_tip.getContentSize().width / 2, this.my_tip.getContentSize().height / 2));
                this.my_tip.addChild(this.my_tip_text);
            }

            {
                for (var i = 0; i < 4; i++) {
                    var sp = new cc.Sprite(__String.createWithFormat(getResPath("RoomMJ2/card_bg/hu_tip_%1.png"), i));
                    sp.setPosition(MJConfig.getHuTipPos(i));
                    sp.setVisible(false);
                    this.hu_tips[i] = sp;
                    this.addChild(sp, 1000);
                }
            }

            {
                for (var i = 0; i < 4; i++) {
                    var sp = new cc.Sprite(__String.createWithFormat(getResPath("RoomMJ/maima/zhongma_%1.png"), i));
                    sp.setPosition(MJConfig.getZhongMaTipPos1(i));
                    sp.setVisible(false);
                    this.zhongma_tips[i] = sp;
                    this.addChild(sp, 1000);
                }
            }

            {
                var myTips = new cc.Sprite(getResPath("RoomMJ/hunan/zhongniao_border_h.png"));
                myTips.setPosition(MJConfig.getNiaoTipsPos(0));
                myTips.setVisible(false);

                var leftTips = new cc.Sprite(getResPath("RoomMJ/hunan/zhongniao_border_v.png"));
                leftTips.setPosition(MJConfig.getNiaoTipsPos(1));
                leftTips.setVisible(false);

                var acrossTips = new cc.Sprite(getResPath("RoomMJ/hunan/zhongniao_border_h.png"));
                acrossTips.setPosition(MJConfig.getNiaoTipsPos(2));
                acrossTips.setRotation(180);
                acrossTips.setVisible(false);

                var rightTips = new cc.Sprite(getResPath("RoomMJ/hunan/zhongniao_border_v.png"));
                rightTips.setPosition(MJConfig.getNiaoTipsPos(3));
                rightTips.setRotation(180);
                rightTips.setVisible(false);

                this.zhongNiao_tips[0] = myTips;
                this.zhongNiao_tips[1] = leftTips;
                this.zhongNiao_tips[2] = acrossTips;
                this.zhongNiao_tips[3] = rightTips;

                this.addChild(myTips, 1000);
                this.addChild(leftTips, 1000);
                this.addChild(acrossTips, 1000);
                this.addChild(rightTips, 1000);
            }

            //out card effect
            {

                this.out_card_effect = new cc.Layer();
                this.out_card_effect.ignoreAnchorPointForPosition(false);
                this.out_card_effect.setAnchorPoint(cc.p(0, 0));
                this.out_card_effect.setPosition(cp.vzero);
                this.out_card_effect.setVisible(false);
                this.addChild(this.out_card_effect, 10000);

                var text = new cc.Sprite(getResPath("RoomMJ/effect/outcard_tip/out_card_effect_text.png"));
                text.setPosition(cc.p(size.width / 2 - 50, 158));
                this.out_card_effect.addChild(text);

//        auto arrow = new cc.Sprite("RoomMJ/effect/outcard_tip/this.out_card_effect_point.png");
//        arrow.setPosition(cc.p(size.width - 370, 246));
//        this.out_card_effect.addChild(arrow);

                var sp = new cc.Sprite(getResPath("RoomMJ/effect/outcard_tip/out_card_effect_1.png"));
                sp.setPosition(cc.p(size.width / 2 + 28, 99));
                this.out_card_effect.addChild(sp);

                var disappearf = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
                var disappear = Utils.getCCAnimate(getResPath("RoomMJ/effect/outcard_tip/out_card_effect_%1.png"), 10, disappearf, 0.1, true, false);
                sp.runAction(disappear.repeatForever());
            }

            var _listenerChat = cc.EventListener.create({
                event: cc.EventListener.CUSTOM,
                eventName: "notify_room_chat1",
                callback: function (evt) {
                    self.hasChatNotify(JsUtils.decodeBase64(evt.getUserData()));
                    cc.log("notify_room_chat1");
                }
            });
            cc.eventManager.addListener(_listenerChat, this);

            ccs.armatureDataManager.addArmatureFileInfo(getResPath("RoomMJ/effect2/peng/effects_mjpen.ExportJson"));
            ccs.armatureDataManager.addArmatureFileInfo(getResPath("RoomMJ/effect2/hu/effects_mjhu.ExportJson"));
            ccs.armatureDataManager.addArmatureFileInfo(getResPath("RoomMJ/effect2/chi/effects_mjchi.ExportJson"));
            ccs.armatureDataManager.addArmatureFileInfo(getResPath("RoomMJ/effect2/gang/effects_mjgang.ExportJson"));
            ccs.armatureDataManager.addArmatureFileInfo(getResPath("RoomMJ/effect2/bg/effects_mjbj.ExportJson"));
            ccs.armatureDataManager.addArmatureFileInfo(getResPath("RoomMJ/scmj/effect/ankang/effects_xiayu.ExportJson"));
            ccs.armatureDataManager.addArmatureFileInfo(getResPath("RoomMJ/scmj/effect/kang/effects_guafen.ExportJson"));
        },
        showBigFace: function (bpos, epos, fid, sex) {
            if (bpos == epos) {
                return;
            }
            var begin;
            var end;

            var face = null;
            begin = MJConfig.getPlayerPos(bpos);
            end = MJConfig.getPlayerPos(epos);

            var appear = null;
            var move = null;
            var disappear = null;

            switch (fid) {
                case 1: {
                    face = new cc.Sprite("Room/bigface/flower/flower1.png");
                    if (face != null) {
                        appear = cc.fadeIn(0.5);
                        move = cc.moveTo(0.5, end);

                        var disappearf = [2, 3, 4, 5, 6, 7, 8, 9, 10];
                        disappear = Utils.getCCAnimate("Room/bigface/flower/flower%1.png", 9, disappearf, 0.2, false, false);
                    }
                    break;
                }
                case 2: {
                    face = new cc.Sprite("Room/bigface/cheers/cheers1.png");
                    if (face != null) {
                        appear = cc.fadeIn(0.5);
                        move = cc.moveTo(0.7, end);

                        var disappearf = [2, 3, 4, 5, 6, 7, 7, 8, 8, 9, 9, 10, 10];
                        disappear = Utils.getCCAnimate("Room/bigface/cheers/cheers%1.png", 13, disappearf, 0.10, false, false);
                    }
                    break;
                }
                case 3: {
                    face = new cc.Sprite("Room/bigface/kiss/kiss1.png");
                    if (face != null) {
                        appear = cc.fadeIn(0.5);
                        move = cc.moveTo(0.5, end);

                        var disappearf = [2, 3, 3, 4, 5, 6, 7, 8, 9, 10];
                        disappear = Utils.getCCAnimate("Room/bigface/kiss/kiss%1.png", 10, disappearf, 0.2, false, false);
                    }
                    break;
                }
                case 4: {
                    face = new cc.Sprite("Room/bigface/eggs/egg1.png");
                    if (face != null) {
                        {
                            end.y -= 30;
                        }
                        appear = cc.fadeIn(0.5);
                        move = cc.spawn(cc.rotateBy(0.5, 720), cc.moveTo(0.5, end));

                        var disappearf = [2, 2, 2, 3, 4, 5, 6];
                        disappear = Utils.getCCAnimate("Room/bigface/eggs/egg%1.png", 7, disappearf, 0.2, false, false);
                    }
                    break;
                }
                case 5: {
                    face = new cc.Sprite("Room/bigface/shoe/shoe1.png");
                    if (face != null) {
                        {
                            end.y -= 10;
                        }
                        appear = cc.fadeIn(0.5);
                        move = cc.spawn(cc.rotateBy(0.5, 360 + 360), cc.moveTo(0.5, end));
                        var disappearf = [2, 2, 2, 3, 4, 5, 6, 7, 8, 9];
                        disappear = Utils.getCCAnimate("Room/bigface/shoe/shoe%1.png", 10, disappearf, 0.2, false, false);
                    }

                    break;
                }
                case 6: {
                    face = new cc.Sprite("Room/bigface/bomb/bomb1.png");
                    if (face != null) {
                        appear = cc.fadeIn(0.5);
                        move = cc.spawn(cc.rotateBy(0.5, 720), cc.moveTo(0.5, end));

                        var disappearf = [2, 3, 4, 2, 3, 4, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 11, 12, 12, 12, 13, 14, 15];
                        disappear = Utils.getCCAnimate("Room/bigface/bomb/bomb%1.png", 24, disappearf, 0.1, false, false);
                    }
                    break;
                }
                default: {
                    break;
                }
            }

            if (fid > -1 && fid < 7) {
                if (face != null) {
                    face.setPosition(begin);
                    face.setOpacity(0);
                    face.setTag(1000 + fid);
                    this.bigFacePlayTalk(fid, sex);
                    var self = this;

                    var playsoundcallback = cc.callFunc(function () {
                        sex == SEX_ID.SEX_FEMALE ? self.bigFacePlaySoundf(face) : self.bigFacePlaySoundm(face);
                    });

                    face.runAction(cc.sequence(appear, move, playsoundcallback, disappear, cc.removeSelf()));
                    this.addChild(face);
                }

            }
        },
        bigFacePlayTalk: function (faceid, sex) {
            if (faceid == 1) {
                Sound.getInstance().playEffect(__String.createWithFormat("Sound/bigface/%1_xianhua.mp3", sex == SEX_ID.SEX_FEMALE ? "F" : "M"));
            }
            else if (faceid == 2) {
                Sound.getInstance().playEffect(__String.createWithFormat("Sound/bigface/%1_pijiu.mp3", sex == SEX_ID.SEX_FEMALE ? "F" : "M"));
            }
            else if (faceid == 3) {
                Sound.getInstance().playEffect(__String.createWithFormat("Sound/bigface/%1_kiss.mp3", sex == SEX_ID.SEX_FEMALE ? "F" : "M"));
            }
            else if (faceid == 4) {
                Sound.getInstance().playEffect(__String.createWithFormat("Sound/bigface/%1_jidan.mp3", sex == SEX_ID.SEX_FEMALE ? "F" : "M"));
            }
            else if (faceid == 5) {
                Sound.getInstance().playEffect(__String.createWithFormat("Sound/bigface/%1_tuoxie.mp3", sex == SEX_ID.SEX_FEMALE ? "F" : "M"));
            }
            else if (faceid == 6) {
                Sound.getInstance().playEffect(__String.createWithFormat("Sound/bigface/%1_zhadan.mp3", sex == SEX_ID.SEX_FEMALE ? "F" : "M"));
            }
        },
        bigFacePlaySoundf: function (pSender) {
            var faceid = pSender.getTag();
            faceid -= 1000;
            if (faceid == 1) {
                Sound.getInstance().playEffect("Sound/bigface/xianhua.mp3");
            }
            else if (faceid == 2) {
                Sound.getInstance().playEffect("Sound/bigface/pijiu.mp3");
            }
            else if (faceid == 3) {
                Sound.getInstance().playEffect("Sound/bigface/f_wen.mp3");
            }
            else if (faceid == 4) {
                Sound.getInstance().playEffect("Sound/bigface/jidan.mp3");
            }
            else if (faceid == 5) {
                Sound.getInstance().playEffect("Sound/bigface/tuoxie.mp3");
            }
            else if (faceid == 6) {
                Sound.getInstance().playEffect("Sound/bigface/zhadan.mp3");
            }
        },
        bigFacePlaySoundm: function (pSender) {
            var faceid = pSender.getTag();
            faceid -= 1000;

            if (faceid == 1) {
                Sound.getInstance().playEffect("Sound/bigface/xianhua.mp3");
            }
            else if (faceid == 2) {
                Sound.getInstance().playEffect("Sound/bigface/pijiu.mp3");
            }
            else if (faceid == 3) {
                Sound.getInstance().playEffect("Sound/bigface/m_wen.mp3");
            }
            else if (faceid == 4) {
                Sound.getInstance().playEffect("Sound/bigface/jidan.mp3");
            }
            else if (faceid == 5) {
                Sound.getInstance().playEffect("Sound/bigface/tuoxie.mp3");
            }
            else if (faceid == 6) {
                Sound.getInstance().playEffect("Sound/bigface/zhadan.mp3");
            }
        },

        reset: function () {
            this.showTextTip("");
        },
        showTextTip: function (str) {
            if (str == "") {
                this.tip.setVisible(false);
            }
            else {
                this.tip.setVisible(true);
                this.tip_text.setString(str);
                this.tip_effect.setPositionX(this.tip_text.getPositionX() + this.tip_text.getContentSize().width / 2);
            }
        },

        showMyTip: function (str) {
            if (str == "") {
                this.my_tip.setVisible(false);
            }
            else {
                this.my_tip.setVisible(true);
                this.my_tip_text.setString(str);
                this.my_tip.stopAllActions();
                this.my_tip.runAction(cc.sequence(cc.delayTime(3), cc.hide()));
            }
        },

        setCallback: function (callback) {
        },

        hasChatNotify: function (msg) {
            if (msg == null) {
                return;
            }

            var json = JSON.parse(msg);

            if (json == null)return;
            var uid = json["uid"];
            var name = json["name"];
            var content = json["content"];

            var seatid = -1;
            for (var i = 0; i < 4; i++) {
                var p = MJModel.players[i];
                if (p.uid == uid) {
                    seatid = i;
                    break;
                }
            }

//    if (seatid != -1)
            {
//        showChat(MJModel.getPosBySeatid(seatid),content);
                if (json.hasOwnProperty("filepath") && json["filepath"] != null) {
                    SoundModel.getInstance().addSound(json["filepath"]);
                }
            }

//    else
            {
                // this.ShowMoveChat(uid, name, content);
            }

            if (seatid != -1) {
                var p = MJModel.players[seatid];
                var chat_sound = Utils.getFastChatSound(content);
                if (chat_sound != "") {
                    Sound.getInstance().playEffect(__String.createWithFormat("Sound/fastchat/%1_%2.mp3", p.sex == SEX_ID.SEX_FEMALE ? "w" : "m", chat_sound));
                }
            }
        },
        showChat: function (pos, content) {
            var text = new ccui.Text(content, "AmericanTypewriter", 30);
            text.setColor(cc.BLACK);

            var chat_bg = new cc.Scale9Sprite("Chat/bar_txt.png");
            chat_bg.setPreferredSize(cc.size(50 + text.getContentSize().width, 20 + text.getContentSize().height));
            this.addChild(chat_bg, -100);

            text.setAnchorPoint(cc.p(0.5, 0.5));
            text.setPosition(cc.p(chat_bg.getContentSize().width / 2, chat_bg.getContentSize().height / 2));
            chat_bg.addChild(text);

            var p = MJConfig.getPlayerPos(pos);
            if (pos == 0) {
                chat_bg.setAnchorPoint(cc.p(0.0, 0.5));
                chat_bg.setPosition(cc.p(0 + p.x, 100 + p.y));
            }
            else if (pos == 1) {
                chat_bg.setAnchorPoint(cc.p(1.0, 0.5));
                chat_bg.setPosition(cc.p(-50 + p.x, 50 + p.y));
            }
            else if (pos == 2) {
                chat_bg.setAnchorPoint(cc.p(0.0, 0.5));
                chat_bg.setPosition(cc.p(50 + p.x, 50 + p.y));
            }
            else if (pos == 3) {
                chat_bg.setAnchorPoint(cc.p(0.0, 0.5));
                chat_bg.setPosition(cc.p(50 + p.x, 50 + p.y));
            }

            chat_bg.runAction(cc.sequence(cc.delayTime(3), cc.removeSelf()));
        },
        ShowMoveChat: function (uid, name, content) {
            var winSize = cc.director.getWinSize();
            var container = new cc.Sprite();
            this.addChild(container, 100);
            var totalWidth = 0;

            var contentLabel = new cc.LabelTTF(content, "Arial", 24);
            contentLabel.setColor(cc.WHITE);
            contentLabel.setAnchorPoint(cc.p(0, 0));
            container.addChild(contentLabel);
            totalWidth += contentLabel.getContentSize().width;

            var nameLabel = new cc.LabelTTF(name + ":", "Arial", 24);

            nameLabel.setAnchorPoint(cc.p(0, 0));
            nameLabel.setPosition(cc.p(0, 0));
            container.addChild(nameLabel);
            totalWidth += nameLabel.getContentSize().width;

            contentLabel.setPosition(cc.p(nameLabel.getContentSize().width, 0));

            if (ZJHModel.getInstance().uid == uid) {
                nameLabel.setColor(cc.GREEN);
            }
            else {
                nameLabel.setColor(cc.color(181, 243, 255));
            }

            var begin = cc.p(winSize.width, winSize.height - 120 - Utils.Random(0, 100));
            container.setPosition(begin);
            container.runAction(cc.sequence(cc.moveBy(6, cc.p(-winSize.width - totalWidth, 0)),
                cc.removeSelf()));
        },//弹幕

        hideAllTip: function () {
            for (var i = 0; i < 4; i++) {
                this.hu_tips[i].setVisible(false);
            }

            for (var i = 0; i < 4; i++) {
                this.zhongma_tips[i].setVisible(false);
            }

            for (var i = 0; i < 4; i++) {
                this.zhongNiao_tips[i].setVisible(false);
            }

//    if (MJModel.curGameType == ROOM_TYPE_GZMJ_DZ_DZ)
//    {
            this.HideAllBaoTing();
//    }

            while (this.getChildByName("hu_effect") != null) {
                this.removeChildByName("hu_effect");
            }
        },
        showHuaTip: function (pos) {
        },
        showHuTip: function (pos) {
            this.hu_tips[pos].setVisible(true);
        },
        showZhongMaTip: function (pos, isHaveHua) {
            if (isHaveHua) {
                this.zhongma_tips[pos].setPosition(MJConfig.getZhongMaTipPos2(pos))
            } else {
                this.zhongma_tips[pos].setPosition(MJConfig.getZhongMaTipPos1(pos))
            }
            this.zhongma_tips[pos].setVisible(true);
        },
        initZhongNiaoTips:function (isZhongNiao) {
            if (isZhongNiao) {
                this.zhongNiao_tips[0].setTexture(getResPath("RoomMJ/hunan/zhongniao_border_h.png"));
                this.zhongNiao_tips[1].setTexture(getResPath("RoomMJ/hunan/zhongniao_border_v.png"));
                this.zhongNiao_tips[2].setTexture(getResPath("RoomMJ/hunan/zhongniao_border_h.png"));
                this.zhongNiao_tips[3].setTexture(getResPath("RoomMJ/hunan/zhongniao_border_v.png"));
            } else {
                this.zhongNiao_tips[0].setTexture(getResPath("RoomMJ/hunan/zhongma_border_h.png"));
                this.zhongNiao_tips[1].setTexture(getResPath("RoomMJ/hunan/zhongma_border_v.png"));
                this.zhongNiao_tips[2].setTexture(getResPath("RoomMJ/hunan/zhongma_border_h.png"));
                this.zhongNiao_tips[3].setTexture(getResPath("RoomMJ/hunan/zhongma_border_v.png"));
            }
        },
        showZhongNiaoTip:function (pos) {
            this.zhongNiao_tips[pos].setVisible(true);
        },
        showEffect: function (pos, type) {
            try {
                var sp = null;
                var effect = null;
                var begin;
                var end;
                var size = cc.director.getWinSize();

                {
                    begin = MJConfig.getOperatorTipPos(pos);

                    if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ && type == ActionType.TYPE_BAOTING) {
                        begin = MJConfig.getOperatorTipPos(pos);
                        sp = new cc.Sprite(getResPath("RoomMJ/GZMJ/type_effect_bg.png"));
                        sp.setPosition(begin);
                        sp.setScale(2.0);
                        this.addChild(sp);
                        var text_sp = new cc.Sprite(getResPath("RoomMJ/action/baoting_text.png"));
                        text_sp.setPosition(cc.p(sp.getContentSize().width / 2, sp.getContentSize().height / 2));
                        sp.addChild(text_sp);
                        var sca = cc.scaleTo(0.3, 1.0);
                        sp.runAction(cc.sequence(sca, cc.delayTime(1), cc.removeSelf()));
                    } else if(type == ActionType.TYPE_BUZHANG || type == ActionType.TYPE_HAID_YAO){

                        if(type == ActionType.TYPE_BUZHANG){
                            sp = new cc.Sprite(getResPath("RoomMJ/hunan/buzhang_tip.png"));
                        }else{
                            sp = new cc.Sprite(getResPath("RoomMJ/hunan/yaopai_tip.png"));
                        }
                        if (sp != null) {
                            sp.setScale(1.5);
                            sp.setPosition(begin);
                            sp.runAction(cc.sequence(cc.scaleTo(0.17, 0.8), cc.scaleTo(0.17, 1.0), cc.delayTime(0.8), cc.removeSelf()));
                            this.addChild(sp, 10000);
                        }
                    }
                    else {
                        var armature_bg = null;
                        var armature = null;

                        if (type == ActionType.TYPE_PENG) {
                            armature_bg = new ccs.Armature("effects_mjbj");
                            var animation = armature_bg.getAnimation();
                            if (animation) {
                                if (typeof animation.playWithIndex === "function") {
                                    animation.playWithIndex(0);

                                    armature_bg.setPosition(begin);
                                    this.addChild(armature_bg, 10000);
                                }
                            }

                            armature = new ccs.Armature("effects_mjpen");
                            animation = armature.getAnimation();
                            if (animation) {
                                if (typeof animation.playWithIndex === "function") {
                                    animation.playWithIndex(0);

                                    armature.setPosition(begin);
                                    this.addChild(armature, 10000);
                                }
                            }
                        }
                        else if (type == ActionType.TYPE_HU) {
                            armature_bg = new ccs.Armature("effects_mjbj");
                            var animation = armature_bg.getAnimation();
                            if (animation) {
                                if (typeof animation.playWithIndex === "function") {
                                    animation.playWithIndex(0);

                                    armature_bg.setPosition(begin);
                                    this.addChild(armature_bg, 10000);
                                }
                            }

                            armature = new ccs.Armature("effects_mjhu");
                            animation = armature.getAnimation();
                            if (animation) {
                                if (typeof animation.playWithIndex === "function") {
                                    animation.playWithIndex(0);

                                    armature.setPosition(begin);
                                    this.addChild(armature, 10000);
                                }
                            }
                        }
                        else if (type == ActionType.TYPE_LEFT_CHI || type == ActionType.TYPE_CENTER_CHI || type == ActionType.TYPE_RIGHT_CHI) {
                            armature_bg = new ccs.Armature("effects_mjbj");
                            var animation = armature_bg.getAnimation();
                            if (animation) {
                                if (typeof animation.playWithIndex === "function") {
                                    animation.playWithIndex(0);

                                    armature_bg.setPosition(begin);
                                    this.addChild(armature_bg, 10000);
                                }
                            }

                            armature = new ccs.Armature("effects_mjchi");
                            animation = armature.getAnimation();
                            if (animation) {
                                if (typeof animation.playWithIndex === "function") {
                                    animation.playWithIndex(0);

                                    armature.setPosition(begin);
                                    this.addChild(armature, 10000);
                                }
                            }
                        }
                        else {
                            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_GZMJ_DZ_DZ) {
                                this.showDouEffect(pos, type);
                                return;
                            }

                            if (MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_SCMJ2_DZ || MJModel.curGameType == ROOM_TYPE.ROOM_TYPE_YNMJ_DZ) {
                                this.showSCMJ_KangEffect(pos, type);
                                return;
                            }

                            armature_bg = new ccs.Armature("effects_mjbj");
                            var animation = armature_bg.getAnimation();
                            if (animation) {
                                if (typeof animation.playWithIndex === "function") {
                                    animation.playWithIndex(0);

                                    armature_bg.setPosition(begin);
                                    this.addChild(armature_bg, 10000);
                                }
                            }

                            armature = new ccs.Armature("effects_mjgang");
                            animation = armature.getAnimation();
                            if (animation) {
                                if (typeof animation.playWithIndex === "function") {
                                    animation.playWithIndex(0);

                                    armature.setPosition(begin);
                                    this.addChild(armature, 10000);
                                }
                            }
                        }

                        if (armature_bg != null && armature != null) {
                            armature_bg.runAction(cc.sequence(cc.delayTime(2), cc.removeSelf()));
                            armature.runAction(cc.sequence(cc.delayTime(2), cc.removeSelf()));
                        }
                    }
                }
            } catch (e) {
                Log(this,e);
            }
        },
        showTingEffect: function (pos, type) {
            var sp = null;
            var begin;
            var size = cc.director.getWinSize();

            if (type == LISTEN_TYPE.LISTEN_CARD_BAIPAI) {
                sp = new cc.Sprite(getResPath("RoomMJ/effect2/ting/tangpai_tip.png"));
            }
            else if (type == LISTEN_TYPE.LISTEN_CARD_FIRST_BAOPAI) {
                sp = new cc.Sprite(getResPath("RoomMJ/effect2/ting/baopai_tip.png"));
            }
            else {
                sp = new cc.Sprite(getResPath("RoomMJ/effect2/ting/baoting_tip.png"));
            }

            if (pos == 0) {
                begin = cc.p(size.width / 2, 206);
            }
            else if (pos == 1) {
                begin = cc.p(size.width - 275, 383);
            }
            else if (pos == 2) {
                begin = cc.p(size.width / 2, 521);
            }
            else if (pos == 3) {
                begin = cc.p(275, 383);
            }
            if (sp != null) {
                sp.setScale(1.5);

                sp.setPosition(begin);

                sp.runAction(cc.sequence(cc.scaleTo(0.17, 0.8), cc.scaleTo(0.17, 1.0), cc.delayTime(0.8), cc.removeSelf()));

                this.addChild(sp, 10000);
            }
        },

        showOutCardEffect: function (v) {
            if (this.out_card_effect) this.out_card_effect.setVisible(v);
        },
        ShowGameState: function (type) {
            ///  type 1:游戏开始 2:玩牌结束 3。流局
            var size = cc.director.getWinSize();
            var jipai_bg = new cc.Sprite(getResPath("RoomMJ/GZMJ/show_jipai_bg.png"));
            jipai_bg.setAnchorPoint(cc.p(0.5, 0.5));
            jipai_bg.setPosition(cc.p(size.width / 2, size.height / 2));
            this.addChild(jipai_bg);
            var buf = __String.createWithFormat(getResPath("RoomMJ/effect/gamestate/effects_gamestate_%1.png"), type);
            var bg_size = jipai_bg.getContentSize();
            var effects_js = new cc.Sprite(buf);
            effects_js.setPosition(cc.p(bg_size.width / 2, bg_size.height / 2));
            jipai_bg.addChild(effects_js);
            effects_js.runAction(cc.sequence(cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 2), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 1.88), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 0.96), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 1.04), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 1.0)));
            var effects_js1 = new cc.Sprite(buf);
            effects_js1.setPosition(cc.p(bg_size.width / 2, bg_size.height / 2));
            jipai_bg.addChild(effects_js1);
            effects_js1.setOpacity(255 / 100.0 * 0.30);
            effects_js1.runAction(cc.sequence(cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 1.95), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 3.4, 1.95), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 1.88), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.removeSelf()));
            var effects_js2 = new cc.Sprite(buf);
            effects_js2.setPosition(cc.p(bg_size.width / 2, bg_size.height / 2));
            jipai_bg.addChild(effects_js2);
            var func = cc.callFunc(function () {
                effects_js2.runAction(cc.fadeTo(15.0 / 12 * 12.0 / 30, 255.0 / 100 * 0.46));
                effects_js2.runAction(cc.scaleTo(15.0 / 12 * 12.0 / 30, 1.2));
            });
            effects_js2.runAction(cc.sequence(cc.delayTime(13.0 / 12 * 12.0 / 30), func));
            var effects_jsguang = new cc.Sprite(__String.createWithFormat(getResPath("RoomMJ/effect/gamestate/effects_gamestatebg_%d.png"), type));
            effects_jsguang.setPosition(cc.p(bg_size.width / 2, bg_size.height / 2));
            jipai_bg.addChild(effects_jsguang);
            var func1 = cc.callFunc(function () {
                effects_jsguang.setOpacity(255.0 / 100 * 75);
                effects_jsguang.setScale(1.0);
            });
            effects_jsguang.runAction(cc.sequence(cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 1.70), cc.delayTime(6.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 1.46), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 1.23), cc.delayTime(1.0 / 12 * 12.0 / 30), func1, cc.delayTime(2.0 / 12 * 12.0 / 30), cc.fadeTo(0.02 * 12.0 / 30, 0)));
            var seq = cc.sequence(cc.delayTime(1.5), cc.removeSelf());
            jipai_bg.runAction(seq);
        },
        ////////////////////////贵州麻将新加//////////////////////////////////
        ShowJiPai: function (JiPaicardList) {
            var size = cc.director.getWinSize();
            var jipai_bg = new cc.Sprite(getResPath("RoomMJ/GZMJ/show_jipai_bg.png"));
            jipai_bg.setAnchorPoint(cc.p(0.5, 0.5));
            jipai_bg.setPosition(cc.p(size.width / 2, size.height / 2));
            this.addChild(jipai_bg);
            var bgsize = jipai_bg.getContentSize();
            var title_sp = new cc.Sprite(getResPath("RoomMJ/GZMJ/show_jipai_title.png"));
            title_sp.setPosition(cc.p(bgsize.width / 2, bgsize.height + 20));
            jipai_bg.addChild(title_sp);
            var card_bg = MJConfig.getCardBgStr(0, 0, CardType.Card_Outing, 0);
            var mj_card = new cc.Sprite(card_bg);

            var subx = 0;
            if (mj_card) {
                if (JiPaicardList.length > 1) {
                    subx = (JiPaicardList.length - 1) * mj_card.getContentSize().width / 2;
                }
            }
            var x = bgsize.width / 2 - subx;
            var dx = mj_card.getContentSize().width;
            var dy = mj_card.getContentSize().height / 2 + 20;
            for (var i = 0; i < JiPaicardList.length; i++) {
                if (mj_card) {
                    mj_card = new cc.Sprite(card_bg);
                    mj_card.setPosition(cc.p(x + dx * i, dy));
                    jipai_bg.addChild(mj_card);
                    var num_str = MJConfig.getCardValueStr(JiPaicardList[i], 0, CardType.Card_Outing);
                    if (num_str != "") {
                        var num_sp = new cc.Sprite(num_str);
                        num_sp.setPosition(cc.p(mj_card.getContentSize().width / 2, mj_card.getContentSize().height / 2 + 10));
                        mj_card.addChild(num_sp);
                    }
                }

            }
            jipai_bg.setScale(0);
            var seq = cc.sequence(cc.scaleTo(0.3, 1).easing(cc.easeBackOut()), cc.delayTime(1.5), cc.removeSelf());
            jipai_bg.runAction(seq);
        },//展示鸡牌（贵州麻将新加）
        ShowHuangZhuang: function (type) {
            //type  1: 黄庄  2 ：黄庄查叫
            var size = cc.director.getWinSize();
            var huangzhuang_bg = new cc.Sprite(getResPath("RoomMJ/GZMJ/show_jipai_bg.png"));
            huangzhuang_bg.setAnchorPoint(cc.p(0.5, 0.5));
            huangzhuang_bg.setPosition(cc.p(size.width / 2, size.height / 2));
            this.addChild(huangzhuang_bg);
            var bgsize = huangzhuang_bg.getContentSize();
            if (type == 1) {
                var huangzhuang_tip = new cc.Sprite(getResPath("RoomMJ/GZMJ/huangzhuang_tip.png"));
                huangzhuang_tip.setPosition(cc.p(bgsize.width / 2 - 70, bgsize.height / 2));
                huangzhuang_bg.addChild(huangzhuang_tip);

                var chajiao_tip = new cc.Sprite(getResPath("RoomMJ/GZMJ/chapai_tip.png"));
                chajiao_tip.setPosition(cc.p(bgsize.width / 2 + 70, bgsize.height / 2));
                huangzhuang_bg.addChild(chajiao_tip);

            }
            else {
                var huangzhuang_tip = new cc.Sprite(getResPath("RoomMJ/GZMJ/huangzhuang_tip.png"));
                huangzhuang_tip.setPosition(cc.p(bgsize.width / 2, bgsize.height / 2));
                huangzhuang_bg.addChild(huangzhuang_tip);
            }
            huangzhuang_bg.setScale(0);
            var seq = cc.sequence(cc.scaleTo(0.3, 1).easing(cc.easeBackOut()), cc.delayTime(1.5), cc.removeSelf());
            huangzhuang_bg.runAction(seq);
        },//展示黄庄
        ShowBaoTing: function (pos, v) {
            if (v == undefined) v = true;
            var baoting = this.baoting_tips[pos];
            if (!baoting && v) {
                baoting = new cc.Sprite(getResPath("RoomMJ2/ting_tip.png"));
                baoting.setAnchorPoint(cc.p(0.5, 0.5));
                baoting.setPosition(MJConfig.getBaoTingPos(pos));
                this.addChild(baoting);
                this.baoting_tips[pos] = baoting;
            }
            else {
                if (baoting) {
                    baoting.setVisible(v);
                }

            }
        },
        HideAllBaoTing: function () {
            for (var i = 0; i < 4; i++) {
                var baoting = this.baoting_tips[i];
                if (baoting) {
                    baoting.setVisible(false);
                }
            }
        },
        showDouEffect: function (pos, type1) {
            //type = 1点豆  2 闷豆 3；爬坡豆
            var type = 0;
            if (type1 == ActionType.TYPE_ANGANG) {
                type = 2;
            }
            else if (type1 == ActionType.TYPE_ZHIGANG) {
                type = 1;
            }
            else if (type1 == ActionType.TYPE_WANGANG) {
                type = 3;
            }
            if (MJModel.isHanBaoDou) {
                type = 4;
            }
            if (type == 0) {
                return;
            }
            var effect = null;
            var begin = MJConfig.getOperatorTipPos(pos);
            var end;
            var size = cc.director.getWinSize();
            var buf = __String.createWithFormat(getResPath("RoomMJ/effect/dou/effects_doutype_0%1.png"), type);
            var effect_dou_type1 = new cc.Sprite(buf);
            var sp = new cc.Sprite();
            sp.setContentSize(effect_dou_type1.getContentSize());
            sp.setPosition(begin);
            this.addChild(sp, 2);

            effect_dou_type1.setPosition(begin);
            this.addChild(effect_dou_type1, 2);

            var func = cc.callFunc(function () {
                effect_dou_type1.setScale(1.4);
                effect_dou_type1.setOpacity(0);
            });
            effect_dou_type1.runAction(cc.sequence(cc.delayTime(0.5 * 12.0 / 30), func));


            var effect_dou_type12 = new cc.Sprite(buf);
            effect_dou_type12.setPosition(cc.p(sp.getContentSize().width / 2, sp.getContentSize().height / 2));
            sp.addChild(effect_dou_type12, 2);
            effect_dou_type12.setScale(0.17);
            effect_dou_type12.runAction(cc.sequence(cc.delayTime(2.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 1), cc.delayTime(5.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 0.98), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 0.53), cc.delayTime(1.0 / 12), cc.scaleTo(0.02 * 12.0 / 30, 0.57), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 0.55), cc.fadeTo(0.0, 255.0 / 100 * 0.75), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.fadeTo(0.0, 255.0 / 100 * 0.55)));
            buf = __String.createWithFormat(getResPath("RoomMJ/effect/dou/effects_doutype_00%d.png"), type);
            var effect_dou_type2 = new cc.Sprite(buf);
            effect_dou_type2.setPosition(cc.p(sp.getContentSize().width / 2, sp.getContentSize().height / 2));
            effect_dou_type2.setVisible(false);
            sp.addChild(effect_dou_type2, 2);
            effect_dou_type2.runAction(cc.sequence(cc.delayTime(0.5 * 12.0 / 30), cc.show(), cc.delayTime(2.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 0.55)));
            {
                //
                //后面水墨画
                var x;
                var dx = 60;
                var y = sp.getContentSize().height / 2 - 10;
                if (type == 1) {
                    x = 300;
                }
                else if (type == 2) {
                    x = 215;
                }
                else if (type == 3) {
                    x = 425;
                    dx = 75;
                    y = sp.getContentSize().height / 2 - 15;
                }
                else {
                    x = 315;
                    dx = 70;
                    y = sp.getContentSize().height / 2 - 5;
                }
                var nums = type >= 3 ? 3 : 2;
                for (var i = 0; i < nums; i++) {
                    var effect_sp = new cc.Sprite(getResPath("RoomMJ/effect/dou/effects_hulizi_00000.png"));
                    var disappearf = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9];
                    effect = Utils.getCCAnimate(getResPath("RoomMJ/effect/dou/effects_hulizi_000%1.png"), 12, disappearf, 0, false, true);
                    if (effect_sp != null) {
                        effect_sp.setScale(type >= 3 ? 0.65 : 0.58);
                        effect_sp.setVisible(false);
                        effect_sp.setPosition(cc.p(x + dx * i, y));
                        effect_sp.runAction(cc.sequence(cc.delayTime(0), cc.show(), effect, cc.delayTime(0.5), cc.removeSelf()));
                        sp.addChild(effect_sp);
                    }
                    var effects_d = new cc.Sprite(getResPath("RoomMJ/effect/dou/effects_d.png"));
                    effects_d.setVisible(false);
                    effects_d.setPosition(cc.p(x + dx * i, y));
                    effects_d.setScale(type >= 3 ? 0.65 : 0.58);
                    sp.addChild(effects_d);
                    effects_d.runAction(cc.sequence(cc.delayTime(0), cc.show(), cc.delayTime(0.5), cc.removeSelf()));
                    Log(this, "1111111111");
                }

            }
            sp.runAction(cc.sequence(cc.delayTime(2), cc.removeSelf()));
        },
        showSCMJ_KangEffect: function (pos, type) {
            var size = cc.director.getWinSize();
            var begin;
            var begin2;
            if (pos == 0) {
                begin = cc.p(size.width / 2, 206);
                begin2 = cc.p(size.width / 2, 150);
            }
            else if (pos == 1) {
                begin = cc.p(size.width - 275, 383);
                begin2 = cc.p(size.width - 275, 383 - 50);
            }
            else if (pos == 2) {
                begin = cc.p(size.width / 2, 521);
                begin2 = cc.p(size.width / 2, 521 - 200);
            }
            else if (pos == 3) {
                begin = cc.p(275, 383);
                begin2 = cc.p(275, 383 - 50);
            }

            if (type == ActionType.TYPE_ANGANG) {
                var face = new cc.Sprite(getResPath("RoomMJ/scmj/effect/ankang1/effects_xyu_00.png"));
                if (face != null) {
                    var disappearf = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36];
                    var disappear = Utils.getCCAnimate(getResPath("RoomMJ/scmj/effect/ankang1/effects_xyu_%1.png"), 36, disappearf, 0.08, false, true);
                    face.setPosition(begin2);
                    face.setScale(2);
                    face.setAnchorPoint(cc.p(0.5, 0));
                    this.addChild(face, 10000);
                    face.runAction(cc.sequence(disappear, cc.removeSelf()));
                }

                var armature_bg = new ccs.Armature("effects_xiayu");
                var animation = armature_bg.getAnimation();
                if (animation) {
                    if (typeof animation.playWithIndex === "function") {
                        animation.playWithIndex(0);
                        armature_bg.setPosition(begin);
                        this.addChild(armature_bg, 10000);
                        armature_bg.runAction(cc.sequence(cc.delayTime(2), cc.removeSelf()));
                    }
                }
                Sound.getInstance().playEffect(getResPath("RoomMJ/sound/scmj/operator/audio_rain.mp3"));
            }
            else {
                var face = new cc.Sprite(getResPath("RoomMJ/scmj/effect/kang1/effects_feng_00.png"));
                if (face != null) {
                    var disappearf = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19];
                    var disappear = Utils.getCCAnimate(getResPath("RoomMJ/scmj/effect/kang1/effects_feng_%1.png"), 19, disappearf, 0.10, false, true);
                    face.setPosition(begin2);
                    face.setAnchorPoint(cc.p(0.5, 0));
                    this.addChild(face, 10000);
                    face.setScale(2);
                    face.runAction(cc.sequence(disappear, cc.removeSelf()));
                }

                var armature_bg = new ccs.Armature("effects_guafen");
                var animation = armature_bg.getAnimation();
                if (animation) {
                    if (typeof animation.playWithIndex === "function") {
                        animation.playWithIndex(0);
                        armature_bg.setPosition(begin);
                        this.addChild(armature_bg, 10000);
                        armature_bg.runAction(cc.sequence(cc.delayTime(2), cc.removeSelf()));
                    }
                }

                Sound.getInstance().playEffect(getResPath("RoomMJ/sound/scmj/operator/audio_guafeng.mp3"));
            }
        },
        showJiType: function (pos, type) {
            if (type == ActionType.JITYPE_CHONGFENGJI) {
                //冲锋鸡
            }
            var begin = MJConfig.getOperatorTipPos(pos);
            var size = cc.director.getWinSize();
            var effect_sp = new cc.Sprite(getResPath("RoomMJ/effect/jitype/effects_cfj_01.png"));
            var sp = new cc.Sprite();
            sp.setContentSize(effect_sp.getContentSize());
            sp.setPosition(begin);
            this.addChild(sp, 2);
            var disappearf = [1, 2, 3, 4, 5, 6, 7, 8, 9, 7, 9, 7, 9, 7, 9, 7, 9, 7];
            var effect = Utils.getCCAnimate(getResPath("RoomMJ/effect/jitype/effects_cfj_%1.png"), 18, disappearf, 0.1, false, true);
            var sp_size = sp.getContentSize();
            if (effect_sp != null) {
                effect_sp.setVisible(false);
                effect_sp.setPosition(cc.p(sp_size.width / 2, sp_size.height / 2));
                effect_sp.runAction(cc.sequence(cc.delayTime(2.0 / 12 * 12.0 / 30), cc.show(), effect));
                sp.addChild(effect_sp);
            }
            var pic_str = getResPath("RoomMJ/effect/jitype/effects_cfjz1.png");
            if (type == ActionType.JITYPE_ZERENJI) {
                pic_str = getResPath("RoomMJ/effect/jitype/effects_zrjz1.png");
            }
            var effects_zrjz1 = new cc.Sprite(pic_str);
            effects_zrjz1.setPosition(cc.p(sp_size.width / 2, sp_size.height / 2));
            sp.addChild(effects_zrjz1);
            effects_zrjz1.setScale(0);
            effects_zrjz1.runAction(cc.sequence(cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 0.42), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 1.05), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 0.95), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 1), cc.delayTime(3.0 / 12 * 12.0 / 30), cc.fadeTo(0.0, 0)));
            pic_str = getResPath("RoomMJ/effect/jitype/effects_cfjz2.png");
            if (type == ActionType.JITYPE_ZERENJI) {
                pic_str = getResPath("RoomMJ/effect/jitype/effects_zrjz2.png");
            }
            var effects_zrjz2 = new cc.Sprite(pic_str);
            effects_zrjz2.setPosition(cc.p(sp_size.width / 2, sp_size.height / 2));
            sp.addChild(effects_zrjz2);
            effects_zrjz2.setScale(0);
            effects_zrjz2.runAction(cc.sequence(cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 0.42), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 1.05), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 0.95), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 1)));
            pic_str = getResPath("RoomMJ/effect/jitype/effects_cfjz3.png");
            if (type == ActionType.JITYPE_ZERENJI) {
                pic_str = getResPath("RoomMJ/effect/jitype/effects_zrjz3.png");
            }
            var effects_zrjz3 = new cc.Sprite(pic_str);
            effects_zrjz3.setPosition(cc.p(sp_size.width / 2, sp_size.height / 2));
            sp.addChild(effects_zrjz3);
            effects_zrjz3.setScale(0);
            effects_zrjz3.runAction(cc.sequence(cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 0.42), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 1.05), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 0.95), cc.delayTime(1.0 / 12 * 12.0 / 30), cc.scaleTo(0.02 * 12.0 / 30, 1), cc.delayTime(6.0 / 12 * 12.0 / 30), cc.fadeTo(0.0, 0)));
            sp.runAction(cc.sequence(cc.delayTime(2), cc.removeSelf()));
        },

        showGuiPai: function (value) {
            var size = cc.director.getWinSize();

            //清除控件
            var guipaiBgTag = this.getChildByTag(98);
            var mj_cardTag = this.getChildByTag(99);
            if (guipaiBgTag) {
                this.removeChildByTag(98);
            }
            if (mj_cardTag) {
                this.removeChildByTag(99);
            }

            var guipai_bg = new cc.Sprite(getResPath("RoomMJ/GZMJ/show_jipai_bg.png"));
            guipai_bg.setAnchorPoint(cc.p(0.5, 0.5));
            guipai_bg.setPosition(cc.p(size.width / 2, size.height / 2));
            guipai_bg.setTag(98);
            this.addChild(guipai_bg);
            var bgsize = guipai_bg.getContentSize();
            var title_sp = new cc.Sprite(getResPath("RoomMJ2/show_guipai_title.png"));
            title_sp.setPosition(cc.p(bgsize.width / 2, bgsize.height + 20));
            guipai_bg.addChild(title_sp);
            var mj_card = new cc.Sprite(getResPath("RoomMJ2/card_bg/outing_card_v.png"));
            mj_card.setTag(99);
            var subx = 0;

            var x = bgsize.width / 2 - subx;
            var dx = mj_card.getContentSize().width;
            var dy = mj_card.getContentSize().height / 2 + 20;

            if (mj_card) {
                mj_card.setPosition(cc.p(guipai_bg.getPositionX() - bgsize.width / 2 + 200, guipai_bg.getPositionY() - bgsize.height / 2 + 70));
                this.addChild(mj_card);
                var buf = __String.createWithFormat(getResPath("RoomMJ2/card_value/card_0_%1.png"), Utils.toHex2(value));
                var num_texture = new cc.Sprite(buf);

                if (num_texture) {
                    num_texture.setPosition(cc.p(mj_card.getContentSize().width / 2, mj_card.getContentSize().height / 2 + 10));
                    mj_card.addChild(num_texture);
                }
            }

            var guipaiNumber = new cc.Sprite(getResPath("RoomMJ2/guipai_number.png"));
            guipaiNumber.setPosition(cc.p(mj_card.getContentSize().width + 45, mj_card.getContentSize().height / 2));
            mj_card.addChild(guipaiNumber);
            var self = this;
            var seq2 = cc.sequence(cc.scaleTo(0.3, 1).easing(cc.easeBackOut()), cc.delayTime(2), cc.removeSelf());
            var callback = cc.callFunc(function () {
                self.intoShowGuiPai();
            });
            var seq1 = cc.sequence(cc.delayTime(0.2), cc.moveTo(0.6, cc.p(100, size.height - 180)).easing(cc.easeIn(1.5)), cc.scaleTo(0.2, 0.0), callback, cc.removeSelf());

            mj_card.runAction(seq1);
            guipai_bg.runAction(seq2);
        },
        //显示鬼牌
        isShowGuiPai: function (flag) {
            var guipaiLeftBg = this.getChildByTag(101);
            cc.log(guipaiLeftBg);
            if (guipaiLeftBg) {
                var mj = guipaiLeftBg.getChildByTag(102);
                //每次更新清除控件
                if (mj) {
                    guipaiLeftBg.removeChildByTag(102);
                }
                if (flag) {
                    var mj_card = new cc.Sprite(getResPath("RoomMJ2/card_bg/hand_card_0.png"));
                    mj_card.setPosition(cc.p(90, guipaiLeftBg.getContentSize().height / 2));
                    mj_card.setTag(102);
                    mj_card.setScale(0.45);

                    var value = MJModel.valueGuiPai;
                    var buf = __String.createWithFormat(getResPath("RoomMJ2/card_value/card_0_%1.png"), Utils.toHex2(value));
                    var num_texture = new cc.Sprite(buf);

                    if (num_texture) {
                        num_texture.setPosition(cc.p(mj_card.getContentSize().width / 2, mj_card.getContentSize().height / 2 - 10));
                        mj_card.addChild(num_texture);
                    }

                    guipaiLeftBg.addChild(mj_card);
                }
            }
        },//是否显示左侧鬼牌
        showLeftGuiPaiBg: function () {
            var size = cc.director.getWinSize();
            var guipaiTag = this.getChildByTag(101);
            if (guipaiTag) {
                this.removeChildByTag(101);
            }
            var guipaiLeftBg = new cc.Sprite(getResPath("RoomMJ2/guipai_bg.png"));
            guipaiLeftBg.setTag(101);
            guipaiLeftBg.setPosition(cc.p(70, size.height - 180));

            var guipaiTxt = new cc.Sprite(getResPath("RoomMJ2/guipai_txt.png"));
            guipaiTxt.setPosition(cc.p(30, guipaiLeftBg.getContentSize().height / 2 + 1));
            guipaiLeftBg.addChild(guipaiTxt);
            this.addChild(guipaiLeftBg);
        },
        intoShowGuiPai: function () {
            this.isShowGuiPai(1);
        },
        resetRemove: function () {
            this.removeChildByTag(98);
            this.removeChildByTag(99);
            this.removeChildByTag(101);
        },//每局游戏结束，清空鬼牌控件
        showChaoZhuang: function () {
            var size = cc.director.getWinSize();
            this.removeChildByTag(103);
            var chaozhuang = new cc.Sprite(getResPath("RoomMJ2/chaozhuang.png"));
            chaozhuang.setTag(103);
            chaozhuang.setPosition(cc.p(size.width / 2, size.height / 2));
            var sq = cc.sequence(cc.scaleTo(1, 1.0), cc.removeSelf());
            chaozhuang.runAction(sq);
            this.addChild(chaozhuang);

        },//显示抄庄
    });
