var DDZVideo = cc.Layer.extend({
    roomTable: null,
    cmd_list: [],
    curIndex: -1,
    maxIndex: 0,
    isPlay: false,
    wantToIndex: -1,
    progress: null,
    minCount: 0,
    curTime: null,
    maxTime: null,
    isProto:true,
    isOK:false,
    getUI:function(key)
    {
        return  utils.findNode(this.uiNode,key);
    },
    addUIClickEvt:function(key)
    {
        var self = this;
        var ui= this.getUI(key);
        if(ui) {
            ui.addClickEventListener(function (sender, evt) {
                self.onClick(ui);
            });
        }
    },
    ctor: function () {
        this._super();
        this.uiJson = ccs.load("public/res/video.json");
        this.uiNode =this.uiJson.node;
        this.addChild(this.uiNode);

        this.addUIClickEvt("mask");
        this.addUIClickEvt("returnBtn");
        this.addUIClickEvt("playBtn");
        this.addUIClickEvt("pauseBtn");
        this.addUIClickEvt("preBtn");
        this.addUIClickEvt("nextBtn");
        this.getUI("playBtn").setVisible(false);
        this.getUI("mess").setVisible(false);
        this.getUI("bg").runAction(cc.sequence(cc.delayTime(3), cc.hide()));

        this.curTime =this.getUI("curTime");
        this.curTime.ignoreContentAdaptWithSize(true);
        this.maxTime =this.getUI("maxTime");
        this.maxTime.ignoreContentAdaptWithSize(true);

        var self = this;
        this.progress =this.getUI("progress");
        this.progress.setPercent(0);
        this.progress.addEventListener(function (slider, event) {
            var wantToIndex = parseInt(slider.getPercent() / 60);
            self.gotoIndex(wantToIndex);
        });

    },
    runScript: function (rtable) {
        this.roomTable = rtable;
        this.cmd_list = [];
        this.isPlay = true;



        if(JsUtils.decodeVideoData)
        {
            var data = JsUtils.decodeVideoData(ZJHModel.getInstance().roomVideoData);

            var cmdList = data.split(":E:");
            if( cmdList.length > 1)
            {
                this.isProto= false;
                this.isOK =true;
                for (var i = 0; i < cmdList.length; i++) {
                    var cmd_data = cmdList[i].split(":S:");
                    if (cmd_data.length < 2) continue;
                    var jp = {};
                    jp["cmd"] = 0;
                    jp["data"] =cmd_data[1];
                    this.cmd_list.push(jp);
                }
            }
        }
        if(this.isProto )
        {
            var data = base64decode(ZJHModel.getInstance().roomVideoData);
            var cmdList = data.split(":E:");

            if( cmdList.length > 1) {
                this.isOK =true;
                for (var i = 0; i < cmdList.length; i++) {
                    var cmd_data = cmdList[i].split(":S:");
                    if (cmd_data.length < 2) continue;
                    var jp = {};
                    jp["cmd"] = Number(cmd_data[0]);
                    jp["data"] = base64encode(cmd_data[1]);
                    this.cmd_list.push(jp);
                }
            }
        }
        this.getUI("mess").setVisible( !this.isOK );


        this.minCount = 60;
        this.maxIndex = this.cmd_list.length;
        this.progress.setMaxPercent(this.maxIndex * 60);
        this.maxTime.setString(utils.format("%1:%2", utils.pad(parseInt(this.maxIndex / 60),2), utils.pad(parseInt(this.maxIndex % 60),2)));

        if(this.isOK) {
            this.schedule(this.getData, 0.0);
        }
    },
    getData: function (dt) {
        if (this.curIndex >= this.maxIndex || this.isPlay == false || this.wantToIndex != -1)return;
        this.minCount+=2;
        this.progress.setPercent(parseInt(this.curIndex * 60 + this.minCount));
        if (this.minCount < 60) {
            return;
        }
        this.minCount = 0;
        this.curIndex++;
        if (this.curIndex >= this.maxIndex) {
            this.curIndex = this.maxIndex;
            return;
        }

        this.curTime.setString(utils.format("%1:%2",utils.pad(parseInt((this.curIndex + 1) / 60),2), utils.pad(parseInt((this.curIndex + 1) % 60),2)));

        var jp = this.cmd_list[this.curIndex];

        if(this.isOK) {
            this.handle_cmd(jp.cmd, jp.data, true);
        }
    },
    handle_cmd: function ( cmd,data, canDelay) {

        this.roomTable.handlerCMD( cmd,data, canDelay);
    },
    onClick: function (btn) {
        var name = btn.getName();

        var cur = this.curIndex;
        if (this.wantToIndex != -1)
        {
            cur = this.wantToIndex;
        }
        var wt = cur - 1;
        switch (name)
        {
            case "mask":
                var btn = this.getUI("bg");
                btn.setVisible(true);
                btn.stopAllActions();
                btn.runAction(cc.sequence(cc.delayTime(3), cc.hide()));
                break;
            case "preBtn":
                if (this.curIndex <= 0) {
                    return;
                }
                wt = cur - 1;
                this.gotoIndex(wt);
                break;
            case "nextBtn":
                if (this.curIndex >= this.maxIndex) {
                    return;
                }
                wt = cur + 1;
                this.gotoIndex(wt);
                break;
            case "pauseBtn":
                this.isPlay = false;
                this.getUI("playBtn").setVisible(true);
                btn.setVisible(false);
                break;
            case "playBtn":
                this.isPlay = true;
                this.getUI("pauseBtn").setVisible(true);
                btn.setVisible(false);
                break;
            case "returnBtn":
                ZJHModel.getInstance().roomToShareView(DDZModel.curRoomID);
                break;

        }
    },
    gotoIndex: function (index) {

        this.unschedule(this.gotoIndexNow);
        this.schedule(this.gotoIndexNow, 0.5);
        if (index < 0) index = 0;
        if (index > this.maxIndex) index = this.maxIndex;
        this.wantToIndex = index;
        this.progress.setPercent(index * 60);
        this.curTime.setString(utils.format("%1:%2", utils.pad(parseInt(this.wantToIndex / 60),2), utils.pad(parseInt(this.wantToIndex % 60),2)));
    },
    gotoIndexNow: function (dt) {
        this.unschedule(this.gotoIndexNow);

        if (this.wantToIndex < this.curIndex) {
            this.roomTable.removeChildByTag(666);
            cc.audioEngine.stopAllEffects();
            for (var i = 0; i < this.wantToIndex; i++) {
                var jp = this.cmd_list[i];
                if(this.isOK) {
                    this.handle_cmd(jp.cmd, jp.data, false);
                }
            }
            this.curIndex = this.wantToIndex;
            this.minCount = 60;
            if (this.wantToIndex < this.maxIndex && this.curIndex >= 0) {
                this.curIndex = this.wantToIndex - 1;
            }
        }
        else if (this.wantToIndex > this.curIndex) {
            if (this.curIndex < 0) this.curIndex = 0;
            for (var i = this.curIndex; i < this.wantToIndex; i++) {
                var jp = this.cmd_list[i];

                if(this.isOK) {
                    this.handle_cmd(jp.cmd, jp.data, false);
                }
            }
            if (this.wantToIndex < this.maxIndex && this.curIndex >= 0) {
                this.curIndex = this.wantToIndex - 1;
            }
            this.minCount = 60;
        }

        this.wantToIndex = -1;
    }
});



