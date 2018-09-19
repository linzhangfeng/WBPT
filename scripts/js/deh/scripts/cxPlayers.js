
var CXPlayer =
{
    seatID:-1,
    uiNode:null,
    player:null,

    protimer:null,
    sec:0,
    m_pAvatar:null,

    callback:null,

    init:function(seatID)
    {
        if(this.uiNode == null)
        {
            return;
        }
        this.seatID = seatID;
        // this.showUI("playimg",false);
        this.showUI("playlose",false);
        this.getUI("playlose").setLocalZOrder(30);
        this.showUI("playready",false);
        this.getUI("playready").setLocalZOrder(20);
        this.showUI("plaything",false);
        this.getUI("plaything").setLocalZOrder(30);
        this.showUI("playgray",false);
        this.getUI("playgray").setLocalZOrder(20);
        this.showUI("play_z_r",false);
        this.getUI("play_z_r").setLocalZOrder(20);
        this.showUI("play_z_l",false);
        this.getUI("play_z_l").setLocalZOrder(20);
        this.showUI("playwin",false);
        this.getUI("playwin").setLocalZOrder(20);
        this.showUI("protip",false);
        this.getUI("protip").setLocalZOrder(20);
        this.showUI("ovt_bg",false);
        this.getUI("ovt_bg").setLocalZOrder(20);

        this.showUI("da",false);
        this.getUI("da").setLocalZOrder(20);
        this.showUI("gen",false);
        this.getUI("gen").setLocalZOrder(20);
        this.showUI("qiao",false);
        this.getUI("qiao").setLocalZOrder(20);
        this.showUI("xiu",false);
        this.getUI("xiu").setLocalZOrder(20);
        this.showUI("diu",false);
        this.getUI("diu").setLocalZOrder(20);

        this.showUI("pname",false);
        this.getUI("pname").ignoreContentAdaptWithSize(true);

        this.protimer = cc.ProgressTimer(this.getUI("protip"));
        this.protimer.setType(cc.ProgressTimer.TYPE_RADIAL);
        this.protimer.setReverseDirection(true);
        this.protimer.setPercentage(0);
        this.protimer.setAnchorPoint(cc.p(0.5, 0.5));
        this.protimer.setPosition(cc.p(56,60));
        this.protimer.setTag(9);
        this.protimer.setVisible(false);
        this.uiNode.addChild(this.protimer, 1000);

        this.m_pAvatar = null;

        this.uiNode.setVisible(false);

        this.getUI("play_money_bg").getChildByName("num").ignoreContentAdaptWithSize(true);

        this.getUI("playlose").runAction(cc.repeatForever(cc.sequence(cc.fadeIn(0.8),cc.fadeOut(0.8))));
        this.getUI("plaything").runAction(cc.repeatForever(cc.sequence(cc.fadeIn(0.8),cc.fadeOut(0.8))));

        var self = this;

        this.uiNode.addClickEventListener(function (sender, evt) {

            if(CXModel.isMySeatID(self.seatID))
            {
                CXPlayers.publicBox.showPlayerList();
            }
            else
            {
                if(CXModel.isMyInTable())
                {
                    var box = new CXBigfaceBox();
                    box.showPos(CXModel.getPosBySeatID(self.seatID));
                    cc.Director.getInstance().getRunningScene().addChild(box,100);
                }
                else
                {
                    CXPlayers.publicBox.showPlayerList();
                }
            }
        });
    },

    showSuccessAction:function()
    {
        var chipAc = this.uiNode.getChildByTag(8888);
        if(chipAc != null)
        {
            this.uiNode.removeChild(chipAc);
            chipAc = null;
        }
    },

    showChipBgAnimation:function()
    {

        var chipAc = this.uiNode.getChildByTag(8888);
        if(chipAc != null)
        {
            this.uiNode.removeChild(chipAc);
            chipAc = null;
        }
        
        ccs.armatureDataManager.addArmatureFileInfo("deh/res/effect/effects_cxmjzmts.ExportJson");
        chipAc = new ccs.Armature("effects_cxmjzmts");

        try
        {
            if(chipAc != null)
            {
                chipAc.setTag(8888);
                chipAc.setLocalZOrder(-100);
                chipAc.setPosition(cc.p(55,58));
                this.uiNode.addChild(chipAc);
                chipAc.getAnimation().playWithIndex(0);       
            }
        }
        catch(e){

        }
        
    },




    setCallBack:function(cb)
    {
        this.callback = cb;
    },

    startTimer:function(alltime,second)
    {
        log("======alltime="+alltime+"===second=="+second);
        if(alltime <= 0)
        {
            return;
        }
        var self = this;
        self.sec = second - 1;
        var curper = second * 100/alltime;
        
        var to = cc.ProgressTo(self.sec, 0);
        var cf = cc.callFunc(function(){
                    self.stopTimer();
                    //播放声音
                });
        this.protimer.setVisible(true);
        self.protimer.setPercentage(curper);
        self.protimer.runAction(cc.Sequence(to, cf));

       self.uiNode.schedule(self.timerUpdate, 1.0);
    },

    showzang:function(pos)
    {
        if(CXModel.isMyInTable())
        {
            if(pos == 1||pos == 2)
            {
                this.getUI("play_z_r").setVisible(true);
            }else{
                this.getUI("play_z_l").setVisible(true);
            }
        }else{
            if(pos == 0 ||pos == 1||pos == 3)
            {
                this.getUI("play_z_r").setVisible(true);
            }else{
                this.getUI("play_z_l").setVisible(true);
            }
        }
    },

    stopTimer:function()
    {
        try{
            if(this.protimer)
            {
                this.protimer.setPercentage(0);
                this.protimer.stopAllActions();
                
            }
           this.uiNode.unschedule(this.timerUpdate);
        }catch(e)
        {
            log("==========");
        }
        
    },

    timerUpdate:function(dt)
    {
        if (CXModel.isMySeatID(this.seatID))
        {
            if(this.sec <= 5)
            {
                if(this.sec == 5)
                {
                    PlatformHelper.vibrate();
                }
                //播放声音
                Sound.getInstance().playEffect(PackName+"/res/sound/com/time_over.mp3");
            }
        }
        this.sec--;
    },

    showDa:function()
    {
        this.getUI("da").setVisible(true);
        this.getUI("da").runAction(cc.sequence(cc.delayTime(2),cc.hide()));
    },

    showGen:function()
    {
        this.getUI("gen").setVisible(true);
        this.getUI("gen").runAction(cc.sequence(cc.delayTime(2),cc.hide()));
    },

    showQiao:function()
    {
        this.getUI("qiao").setVisible(true);
        this.getUI("qiao").runAction(cc.sequence(cc.delayTime(2),cc.hide()));
    },

    showXiu:function()
    {
        this.getUI("xiu").setVisible(true);
        this.getUI("xiu").runAction(cc.sequence(cc.delayTime(2),cc.hide()));
    },

    showDiu:function()
    {
        this.getUI("diu").setVisible(true);
        this.getUI("diu").runAction(cc.sequence(cc.delayTime(2),cc.hide()));
    },

    setTOpacity:function(value)
    {
        this.uiNode.setOpacity(value);
    },

    show:function(is)
    {
        this.uiNode.setVisible(is);
    },
    getUI:function(key)
    {
        return this.uiNode.getChildByName(key);
    },
    showUI:function(key ,is)
    {
        var ui= this.getUI(key);
        if(ui)
        {
            ui.setVisible(is);
        }
        else{
            cc.log("ui erro :"+key);
        }
    },
    getSex:function()
    {
        return  this.player.sex ==1 ?"m":"w";
    },

    updatePlayerMoney:function()
    {
        this.getUI("play_money_bg").getChildByName("num").setString(this.player.bobo_money);
    },

    setPlayerMoney:function(value)
    {
        this.getUI("play_money_bg").getChildByName("num").setString(value);
    },

    updateSeatID:function(seatID)
    {
        this.seatID = seatID;
        this.player = CXModel.getPlayerBySeatID(seatID);
        if(this.player == null)
        {
            cc.log("updateSeatID erro player:"+seatID);
            return;
        }


        this.getUI("play_money_bg").getChildByName("num").setString(this.player.bobo_money);
        this.getUI("pname").setVisible(true);
        this.getUI("pname").setString(this.player.name);

        var tg = this.uiNode.getChildByTag(808);
        if(tg != null)
        {
           this.uiNode.removeChild(tg);
        }

        this.m_pAvatar = utils.createCircleAvatar(this.player.avatar, "deh/res/pic/p_bg.png", "deh/res/pic/p_bg.png", cc.size(92,92));
        this.m_pAvatar.setPosition(cc.p(55,58));
        this.m_pAvatar.setTag(808);
        this.uiNode.addChild(this.m_pAvatar,10);

        /*
        if(utils.hasImageFile(this.player.avatar))
        {
            this.getUI("icon").loadTexture(utils.getImageSavePath(this.player.avatar));
        }
        else
        {
            var self =this;
            self.getUI("icon").loadTexture("public/res/pic/avatars/"+(this.player.sex == 1 ? "gg":"mm")+".png");
            CCHttpAgent.getInstance().sendHttpImage(this.player.avatar,utils.getImageSavePath(this.player.avatar)
                ,utils.getImageSavePath(this.player.avatar),function(tag)
                {
                    CCHttpAgent.getInstance().popPackets(tag);
                    cc.log("down over");
                    cc.log("image change:"+utils.getImageSavePath(self.player.avatar));
                    if(utils.hasImageFile(self.player.avatar))
                    {
                        cc.log("image change succ");
                        self.getUI("icon").loadTexture(utils.getImageSavePath(self.player.avatar));
                    }
                },15);
        }
        */
        this.uiNode.setVisible(true);

    },

    reset:function()
    {
        this.showUI("playlose",false);
        this.showUI("playready",false);
        this.showUI("plaything",false);
        this.showUI("playgray",true);
        this.showUI("play_z_r",false);
        this.showUI("play_z_l",false);

        this.showUI("da",false);
        this.showUI("gen",false);
        this.showUI("qiao",false);
        this.showUI("xiu",false);
        this.showUI("diu",false);

        // this.showUI("pname",false);
        this.uiNode.setOpacity(255);

        this.showSuccessAction();
    },

    resetzang:function()
    {
        this.showUI("play_z_r",false);
        this.showUI("play_z_l",false);
    },
};

var CXPlayers =
{
    uiNode:null,
    players:{},
    init:function()
    {
        if(this.uiNode == null)
        {
            return;
        }

        for(var i =0 ; i < 6;++i)
        {
            this.players[i] = utils.clone(CXPlayer);
            this.players[i].uiNode = this.getUI("player"+i);
            this.players[i].uiNode.setPosition(utils.getPlayerPos(i));
            this.players[i].init();
        }

    },
    release:function()
    {
    },

    getUI:function(key)
    {
        return this.uiNode.getChildByName(key);
    },

    resetPlayerposition:function()
    {
        for(var i =0 ; i < 6;++i)
        {
            this.players[i].uiNode.setPosition(utils.getPlayerPos(i));
        }
    },

    updatePlayerMoney:function(pos)
    {
        this.players[pos].updatePlayerMoney();
    },


    stopAllPlayTimer:function()
    {
        for(var i =0 ; i < 6;++i)
        {
            this.players[i].stopTimer();
        }
    },

    resetZang:function()
    {
        for(var i =0 ; i < 6;++i)
        {
            this.players[i].resetzang();
        }
    },

    reset:function(pos)
    {
        if(pos == null)
        {
            for(var i =0 ; i < 6;++i) {
                this.players[i].reset();
            }
        }
        else
        {
            if(pos<0 || pos >5)
            {
                return;
            }
            this.players[pos].reset();
        }
    },
    //上桌
    uptable:function(player)
    {
        var pos = CXModel.getPosBySeatID(player.seatid);
        if(pos<0 || pos >5)
        {
            cc.log("uptable erro:"+pos);
            return;
        }
        log("=========uptable====pos==="+pos);
        this.players[pos].updateSeatID(player.seatid);
        this.players[pos].show(true);
    },
    //下桌
    downtable:function(pos)
    {
        if(pos == undefined)
        {
            for(var  i in this.players)
            {
                this.players[i].reset();
                this.players[i].show(false);
            }
            return;
        }
        if(pos<0 || pos >5)
        {
            cc.log("downtable erro:"+pos);
            return;
        }

        this.players[pos].reset();
        this.players[pos].show(false);
    },
    //设置准备状态
    setReady:function(pos,is)
    {

        if(pos<0 || pos >5)
        {
            cc.log("setReady erro:"+pos);
            return;
        }

        this.players[pos].showUI("playready",is);
    },

    setgray:function(pos,is)
    {

        if(pos<0 || pos >5)
        {
            cc.log("setgray erro:"+pos);
            return;
        }

        this.players[pos].showUI("playgray",is);
    },

    setOffline:function(pos,is)
    {
        if(pos<0 || pos >5)
        {
            cc.log("setOffline erro:"+pos);
            return;
        }
        this.players[pos].showUI("playlose",is);
    },
    showWinEf:function(pos,is)
    {
        if(pos<0 || pos >5)
        {
            return ;
        }
        this.players[pos].showUI("playwin",is);
    },

    updateInfo:function(pos)
    {
        if(pos<0 || pos >5)
        {
            return ;
        }
        this.players[pos].getUI("play_money_bg").getChildByName("num").setString(this.players[pos].player.money);
    },
    getSex:function(pos)
    {
        if(pos<0 || pos >5)
        {
            return "f";
        }
        return  this.players[pos].player.sex ==1 ?"m":"f";
    },

    getPosition:function(pos)
    {
        if(pos<0 || pos >5)
        {
            return cc.p(0,0);
        }
        return  this.players[pos].uiNode.getPosition();
    }

};


