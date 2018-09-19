var MJVideo = cc.Layer.extend({
    tag: "MJVideo",
    room_Table: null,
    cmd_list: [],
    curIndex: -1,
    maxIndex: 0,
    isPlay: false,
    wantToIndex: -1,
    musicSlider: null,
    minCount: 0,
    curTime: null,
    maxTime: null,
    ctor: function () {
        this._super();
        var size = cc.director.getWinSize();
        this.ignoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.0, 0.0));
        this.setContentSize(size);

        var self = this;
        var layout = new ccui.Layout();
        layout.setContentSize(size);
        layout.setAnchorPoint(cc.p(0, 0));
        layout.setPosition(cc.p(0, 0));
        layout.setTouchEnabled(true);
        layout.setSwallowTouches(true);
        layout.addClickEventListener(function (btn) {
            var n = self.getChildByName("return");
            n.setVisible(true);
            n.stopAllActions();
            n.runAction(cc.sequence(cc.delayTime(3), cc.hide()));
        });
        this.addChild(layout);

        {
            var btn = new ccui.Button("RoomVideo/back01.png", "RoomVideo/back02.png");
            btn.setName("return");
            btn.addClickEventListener(function (btn) {
                self.btnCallback(btn);
            });
            btn.setPosition(cc.p(64, size.height - 51));
            btn.setVisible(false);
            this.addChild(btn);
        }

        {
            var tip = new cc.Sprite("RoomVideo/tip.png");
            tip.setPosition(cc.p(size.width / 2, size.height - 42));
            this.addChild(tip);
        }

        {
            var btn = new ccui.Button("RoomVideo/pre01.png", "RoomVideo/pre02.png");
            btn.setName("pre");
            btn.addClickEventListener(function (btn) {
                self.btnCallback(btn);
            });
            btn.setPosition(cc.p(size.width / 2 - 108, 114));
            this.addChild(btn);
        }

        {
            var btn = new ccui.Button("RoomVideo/pause01.png", "RoomVideo/pause02.png");
            btn.setName("pause");
            btn.addClickEventListener(function (btn) {
                self.btnCallback(btn);
            });
            btn.setPosition(cc.p(size.width / 2 - 0, 114));
            this.addChild(btn);
        }

        {
            var btn = new ccui.Button("RoomVideo/play01.png", "RoomVideo/play02.png");
            btn.setName("play");
            btn.setVisible(false);
            btn.addClickEventListener(function (btn) {
                self.btnCallback(btn);
            });
            btn.setPosition(cc.p(size.width / 2 - 0, 114));
            this.addChild(btn);
        }

        {
            var btn = new ccui.Button("RoomVideo/next01.png", "RoomVideo/next02.png");
            btn.setName("next");
            btn.addClickEventListener(function (btn) {
                self.btnCallback(btn);
            });
            btn.setPosition(cc.p(size.width / 2 + 108, 114));
            this.addChild(btn);
        }

        this.musicSlider = new ccui.Slider();
        this.musicSlider.loadProgressBarTexture("RoomVideo/process_fg.png");
        this.musicSlider.loadBarTexture("RoomVideo/process_bg.png");
        this.musicSlider.loadSlidBallTextures("RoomVideo/process_point.png", "RoomVideo/process_point.png", "");
        this.musicSlider.setPercent(0);
        this.musicSlider.setPosition(cc.p(size.width / 2, 21));
        this.musicSlider.addEventListener(function (slider, event) {
            var wantToIndex = parseInt(slider.getPercent() / 60);
            self.gotoIndex(wantToIndex);
        });

        this.addChild(this.musicSlider);

        {
            this.curTime = new cc.LabelTTF("00:00", "Thonburi", 20);
            this.curTime.setAnchorPoint(cc.p(1, 0.5));
            this.curTime.setPosition(cc.p(size.width / 2 - 488, 22));
            this.curTime.setColor(cc.color(119, 255, 0));
            this.addChild(this.curTime);

            this.maxTime = new cc.LabelTTF(__String.createWithFormat("%1:%2", Utils.to2d(parseInt(this.maxIndex / 60)), Utils.to2d(parseInt(this.maxIndex % 60))), "Thonburi", 20);
            this.maxTime.setAnchorPoint(cc.p(0, 0.5));
            this.maxTime.setPosition(cc.p(size.width / 2 + 488, 22));
            this.maxTime.setColor(cc.color(119, 255, 0));
            this.addChild(this.maxTime);
        }
    },
    runScript: function (rtable) {
        this.room_Table = rtable;
        Log(this, ZJHModel.getInstance().roomVideoData);
        var data = base64decode(ZJHModel.getInstance().roomVideoData);
        Log(this, data);
        this.cmd_list = [];
        var cmdList = data.split(":E:");
        Log(this, "cmd lens:" + cmdList.length);
        for (var i = 0; i < cmdList.length; i++) {
            var cmd_data = cmdList[i].split(":S:");
            if (cmd_data.length < 2) continue;
            var jp = {};
            jp["cmd"] = Number(cmd_data[0]);
            jp["data"] = base64encode(cmd_data[1]);
            Log(this, "cmd:" + jp["cmd"]);
            this.cmd_list.push(jp);
        }
        this.maxIndex = this.cmd_list.length;
        this.isPlay = true;
        this.musicSlider.setMaxPercent(this.maxIndex * 60);
        this.maxTime.setString(__String.createWithFormat("%1:%2", Utils.to2d(parseInt(this.maxIndex / 60)), Utils.to2d(parseInt(this.maxIndex % 60))));
        this.schedule(this.getData, 0.0);
    },
    getData: function (dt) {
        if (this.curIndex >= this.maxIndex || this.isPlay == false || this.wantToIndex != -1)return;
        this.minCount++;
        this.musicSlider.setPercent(parseInt(this.curIndex * 60 + this.minCount));
        if (this.minCount < 60) {
            return;
        }
        this.minCount = 0;
        this.curIndex++;
        if (this.curIndex >= this.maxIndex) {
            this.curIndex = this.maxIndex;
            return;
        }

        this.curTime.setString(__String.createWithFormat("%1:%2", Utils.to2d(parseInt((this.curIndex + 1) / 60)), Utils.to2d(parseInt((this.curIndex + 1) % 60))));

        var jp = this.cmd_list[this.curIndex];
        this.handle_cmd(jp.cmd, jp.data, true);
    },
    handle_cmd: function (cmd, data, canDelay) {
        Log(this,"handle_cmd:" + cmd);
        this.room_Table.handler_cmd(cmd, data, canDelay);
    },
    btnCallback: function (btn) {
        var name = btn.getName();
        if (name == "pre") {
            if (this.curIndex <= 0) {
                return;
            }
            var cur = this.curIndex;
            if (this.wantToIndex != -1) cur = this.wantToIndex;
            var wt = cur - 1;
            this.gotoIndex(wt);
        }

        if (name == "play") {
            this.isPlay = true;
            this.getChildByName("pause").setVisible(true);
            btn.setVisible(false);
        }

        if (name == "pause") {
            this.isPlay = false;
            this.getChildByName("play").setVisible(true);
            btn.setVisible(false);
        }

        if (name == "next") {
            if (this.curIndex >= this.maxIndex) {
                return;
            }
            var cur = this.curIndex;
            if (this.wantToIndex != -1) cur = this.wantToIndex;
            var wt = cur + 1;
            this.gotoIndex(wt);
        }

        if (name == "return") {
            ZJHModel.getInstance().roomToHall();
        }
    },
    gotoIndex: function (index) {
        this.unschedule(this.gotoIndexNow);
        this.schedule(this.gotoIndexNow, 1.0);
        if (index < 0) index = 0;
        if (index > this.maxIndex) index = this.maxIndex;
        this.wantToIndex = index;
        this.musicSlider.setPercent(index * 60);
        this.curTime.setString(__String.createWithFormat("%1:%2", Utils.to2d(parseInt(this.wantToIndex / 60)), Utils.to2d(parseInt(this.wantToIndex % 60))));
    },
    gotoIndexNow: function (dt) {
        this.unschedule(this.gotoIndexNow);
        if (this.wantToIndex < this.curIndex) {
            for (var i = 0; i < this.wantToIndex; i++) {
                var jp = this.cmd_list[i];
                this.handle_cmd(jp.cmd, jp.data, false);
            }

            this.curIndex = this.wantToIndex;
            if (this.wantToIndex < this.maxIndex && this.curIndex > 0) {
                this.curIndex = this.wantToIndex - 1;
            }

        }
        else if (this.wantToIndex > this.curIndex) {
            if (this.curIndex < 0) this.curIndex = 0;
            for (var i = this.curIndex; i < this.wantToIndex; i++) {
                var jp = this.cmd_list[i];
                this.handle_cmd(jp.cmd, jp.data, false);
            }
            this.curIndex = this.wantToIndex;
            if (this.wantToIndex < this.maxIndex && this.curIndex > 0) {
                this.curIndex = this.wantToIndex - 1;
            }
        }

        this.wantToIndex = -1;
    },
});



