
var SettingBox = cc.Layer.extend(
    {
        ctor: function ()
        {
            this._super();
            this.uiJson = ccs.load(PackName+"/res/settingBox.json");
            this.uiNode = this.uiJson.node;
            this.addChild(this.uiNode);

            utils.scaleAll(this.getUI("bg"));
            this.addUIClickEvt("returnBtn");
            this.addUIClickEvt("soundCloseBtn");
            this.addUIClickEvt("soundOpenBtn");
            this.addUIClickEvt("musicCloseBtn");
            this.addUIClickEvt("musicOpenBtn");
            this.addUIClickEvt("vibrateCloseBtn");
            this.addUIClickEvt("vibrateOpenBtn");
            this.addUIClickEvt("soundBtn1");
            this.addUIClickEvt("soundBtn2");

            this.updateView();
            return true;
        },
        getUI:function(key)
        {
            return utils.findNode(this.uiNode,key);
        },
        addUIClickEvt:function(key)
        {
            var self = this;
            var ui= this.getUI(key);
            if(ui) {
                ui.addClickEventListener(function (sender, evt) {

                    switch (key)
                    {
                        case "soundCloseBtn":
                            ZJHModel.getInstance().sound =1;
                            cc.audioEngine.setEffectsVolume(1);
                            sys.localStorage.setItem("sound", 1);
                            break;
                        case "soundOpenBtn":
                            ZJHModel.getInstance().sound =0;
                            cc.audioEngine.setEffectsVolume(0);
                            sys.localStorage.setItem("sound", 0);
                            break;
                        case "musicCloseBtn":
                            ZJHModel.getInstance().bgMusic = 0.3;
                            cc.audioEngine.setMusicVolume(0.3);
                            sys.localStorage.setItem("bgMusic",0.3);
                            break;
                        case "musicOpenBtn":
                            ZJHModel.getInstance().bgMusic = 0;
                            cc.audioEngine.setMusicVolume(0);
                            sys.localStorage.setItem("bgMusic", 0);
                            break;
                        case "vibrateCloseBtn":
                            ZJHModel.getInstance().vibrate = 1;
                            PlatformHelper.sdkDoForVoid_C("setVibrate");
                            sys.localStorage.setItem("vibrate", 1);
                            break;
                        case "vibrateOpenBtn":
                            ZJHModel.getInstance().vibrate = 0;
                            sys.localStorage.setItem("vibrate", 0);
                            break;
                        case "soundBtn1":
                            PHZModel.soundIndex =1;
                            sys.localStorage.setItem("phz1_sound_index",1);
                            break;
                        case "soundBtn2":
                            sys.localStorage.setItem("phz1_sound_index",2);
                            PHZModel.soundIndex =2;
                            break;
                    }
                    self.updateView();
                    if(key =="returnBtn" )
                    {
                        self.removeFromParent(true);
                    }
                });
            }
        },
        updateView:function()
        {

            this.getUI("soundCloseBtn").setVisible(sys.localStorage.getItem("sound")<=0);
            this.getUI("soundOpenBtn").setVisible(sys.localStorage.getItem("sound")>0);
            this.getUI("musicCloseBtn").setVisible(sys.localStorage.getItem("bgMusic")<=0);
            this.getUI("musicOpenBtn").setVisible(sys.localStorage.getItem("bgMusic")>0);
            this.getUI("vibrateCloseBtn").setVisible(sys.localStorage.getItem("vibrate")==0);
            this.getUI("vibrateOpenBtn").setVisible(sys.localStorage.getItem("vibrate")==1);


            var index = sys.localStorage.getItem("phz1_sound_index");
            cc.log("sound index:"+index);
            utils.findNode(this.getUI("soundBtn1"),"click").setVisible(index==1);
            utils.findNode(this.getUI("soundBtn2"),"click").setVisible(index==2);
        }
    }
);