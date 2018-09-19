
var YZDNPlayer =
{
    seatID:-1,
    uiNode:null,
    player:null,

    protimer:null,
    sec:0,
    m_pAvatar:null,

    callback:null,
    CardNumImage:[],

    init:function(seatID)
    {
        if(this.uiNode == null)
        {
            return;
        }
        this.seatID = seatID;
        // this.showUI("playimg",false);
        this.showUI("playlose",false);
        this.getUI("playlose").setLocalZOrder(20);
        this.showUI("playready",false);
        this.getUI("playready").setLocalZOrder(20);
        this.showUI("plaything",false);
        this.getUI("plaything").setLocalZOrder(20);
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

        this.CardNumImage = ["1","2","3","4","5","6","7","8","9","10","J","Q","K"];
        

        this.showUI("pname",false);
        this.getUI("pname").ignoreContentAdaptWithSize(true);
        this.showUI("tipbg",false);
        this.showUI("sn_text",false);

        this.showUI("pnum",false);
        this.getUIs("pn").ignoreContentAdaptWithSize(true);

        this.protimer = cc.ProgressTimer(this.getUI("protip"));
        this.protimer.setType(cc.ProgressTimer.TYPE_RADIAL);
        this.protimer.setReverseDirection(true);
        this.protimer.setPercentage(0);
        this.protimer.setAnchorPoint(cc.p(0.5, 0.5));
        this.protimer.setPosition(cc.p(52,55));
        this.protimer.setTag(9);
        this.protimer.setVisible(false);
        this.uiNode.addChild(this.protimer, 1000);

        this.m_pAvatar = null;

        this.uiNode.setVisible(false);

        this.getUI("play_money_bg").getChildByName("num").ignoreContentAdaptWithSize(true);

        this.getUI("sn_text").getChildByName("t1").ignoreContentAdaptWithSize(true);
        this.getUI("sn_text").getChildByName("t2").ignoreContentAdaptWithSize(true);

        this.getUI("playlose").runAction(cc.repeatForever(cc.sequence(cc.fadeIn(0.8),cc.fadeOut(0.8))));
        this.getUI("plaything").runAction(cc.repeatForever(cc.sequence(cc.fadeIn(0.8),cc.fadeOut(0.8))));

        var self = this;

        this.uiNode.addClickEventListener(function (sender, evt) {

            if(YZDNModel.isMySeatID(self.seatID))
            {
                YZDNPlayers.publicBox.showPlayerList();
            }
            else
            {
                if(YZDNModel.isMyInTable())
                {
                    var box = new YZDNBigfaceBox();
                    box.showPos(YZDNModel.getPosBySeatID(self.seatID));
                    cc.Director.getInstance().getRunningScene().addChild(box,100);
                }
                else
                {
                    YZDNPlayers.publicBox.showPlayerList();
                }
            }
        });
    },

    setTipBet:function(is)
    {
        this.showUI("tipbg",is);
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
        if(pos == 0||pos == 1||pos == 2)
        {
            this.getUI("play_z_r").setVisible(true);
        }else{
            this.getUI("play_z_l").setVisible(true);
        }
    },

    showTipNiuShow:function(is,maxCard)
    {
        this.getUI("sn_text").setVisible(true);
        if(is == true)
        {
            this.getUI("sn_text").getChildByName("t1").setString("有牛");
            
        }else
        {
            this.getUI("sn_text").getChildByName("t1").setString("无牛");
        }


        this.getUI("sn_text").getChildByName("t2").setVisible(true);
            this.getUI("sn_text").getChildByName("mg").setVisible(true);

            if(maxCard == 0)
            {
                this.getUI("sn_text").getChildByName("t1").setVisible(false);
                this.getUI("sn_text").getChildByName("t2").setVisible(false);
                this.getUI("sn_text").getChildByName("mg").setVisible(false);

                return;
            }
            var type = parseInt(maxCard/16) + 1;
            log("===========type=====type====type="+type);
            if(type > 0  && type < 5)
            {
                log("===========rele=====type====type="+type);
                switch(type)
                {
                    case 1:
                        this.getUIs("mg").loadTexture("YZDN/icon/Color1.png",1);
                        break;
                    case 2:
                        this.getUIs("mg").loadTexture("YZDN/icon/Color2.png",1);
                        break;
                    case 3:
                        this.getUIs("mg").loadTexture("YZDN/icon/Color3.png",1);
                        break;
                    case 4:
                        this.getUIs("mg").loadTexture("YZDN/icon/Color4.png",1);
                        break;
                }
                
            }
            
            var lastNum = maxCard%16;
            this.getUI("sn_text").getChildByName("t2").setString(this.CardNumImage[lastNum - 1]);
    },

    showPnum:function(is,num)
    {

        this.getUIs("pn").setVisible(true);
        this.getUIs("text").setVisible(true);
        this.getUIs("bq").setVisible(false);

        var  img = this.getUIs("pn");
        switch(num)
        {
            case 1:
                img.loadTexture("YZDN/num/1.png",1);
                break;
            case 2:
                img.loadTexture("YZDN/num/2.png",1);
                break;
            case 3:
                img.loadTexture("YZDN/num/3.png",1);
                break;
            case 4:
                img.loadTexture("YZDN/num/4.png",1);
                break;
            case 5:
                img.loadTexture("YZDN/num/5.png",1);
                break;
        }
        this.showUI("pnum",is);
    },

    showBQ:function(is)
    {
        this.getUIs("pn").setVisible(false);
        this.getUIs("text").setVisible(false);
        this.getUIs("bq").setVisible(true);
        this.showUI("pnum",is);
    },

    hidePnum:function()
    {
        this.getUIs("pn").setVisible(false);
        this.getUIs("text").setVisible(false);
        this.getUIs("bq").setVisible(false);
    },

    doActionPnum:function(num)
    {
        var mypos = this.getUIs("playgray").getPosition();    //我的位置
        cc.pAddIn(mypos,cc.p(0,80)); 

        var pnumpos = this.getUIs("pnum").getPosition();
        var _sprite = null;
        if(num != 0)
        {
            cc.pAddIn(pnumpos,cc.p(27,26));
            _sprite = cc.Sprite(utils.spriteFrame("YZDN/num/" + num + ".png"));
        }
        else
        {
            cc.pAddIn(pnumpos,cc.p(50,20));
            _sprite = cc.Sprite(utils.spriteFrame("YZDN/num/bq.png"));
        }
         
        _sprite.setPosition(mypos);
        this.uiNode.addChild(_sprite,20);

        var sca_max = cc.ScaleTo(0.2,2.0);

        var sca_min = cc.ScaleTo(0.2,1.0);
        var sca_mov = cc.moveTo(0.2,pnumpos);
        var squa = cc.Spawn(sca_min,sca_mov);

        var self = this;
        var shpum = cc.callFunc(function(){
                    if(num == 0)
                    {
                        self.showBQ(true);
                    }else
                    {
                        self.showPnum(true,num);
                    }
                    
                });

        _sprite.runAction(cc.sequence(sca_max,squa,shpum,cc.RemoveSelf()));
        
    },

    getUIs:function(key)
        {
            return utils.findNode(this.uiNode,key);
        },

    hidetipNiu:function()
    {
        this.getUI("sn_text").setVisible(false);
    },

    setWaiteTip:function(is,value)
    {
        this.showUI("tipbg",is);
        this.getUI("tipbg").getChildByName("tex").ignoreContentAdaptWithSize(true);
        this.getUI("tipbg").getChildByName("tex").setString(value);
    },

    stopTimer:function()
    {
        if(this.protimer)
        {
            this.protimer.setPercentage(0);
            this.protimer.stopAllActions();
        }
       this.uiNode.unschedule(this.timerUpdate);
    },

    timerUpdate:function(dt)
    {
        if (YZDNModel.isMySeatID(this.seatID))
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
        this.getUI("play_money_bg").getChildByName("num").setString(this.player.money);
    },

    setPlayerMoney:function(value)
    {
        this.getUI("play_money_bg").getChildByName("num").setString(value);
    },

    updateSeatID:function(seatID)
    {
        this.seatID = seatID;
        this.player = YZDNModel.getPlayerBySeatID(seatID);
        if(this.player == null)
        {
            cc.log("updateSeatID erro player:"+seatID);
            return;
        }


        this.getUI("play_money_bg").getChildByName("num").setString(this.player.money);
        this.getUI("pname").setVisible(true);
        this.getUI("pname").setString(this.player.name);

        var tg = this.uiNode.getChildByTag(808);
        if(tg != null)
        {
           this.uiNode.removeChild(tg);
        }

        this.m_pAvatar = utils.createCircleAvatar(this.player.avatar, "yznn/res/pic/p_bg.png", "yznn/res/pic/p_bg.png", cc.size(92,92));
        this.m_pAvatar.setPosition(cc.p(51,54));
        this.m_pAvatar.setTag(808);
        this.uiNode.addChild(this.m_pAvatar,10);

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

        this.showUI("sn_text",false);
        this.showUI("tipbg",false);
        this.showUI("sn_text",false);
        this.getUIs("pnum").setVisible(false);

        this.showUI("pname",false);
        this.uiNode.setOpacity(255);
    },

    resetzang:function()
    {
        this.showUI("play_z_r",false);
        this.showUI("play_z_l",false);
    },
};

var YZDNPlayers =
{
    uiNode:null,
    players:{},
    init:function()
    {
        if(this.uiNode == null)
        {
            return;
        }

        for(var i =0 ; i < 5;++i)
        {
            this.players[i] = utils.clone(YZDNPlayer);
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
        for(var i =0 ; i < 5;++i)
        {
            this.players[i].uiNode.setPosition(utils.getPlayerPos(i));
        }
    },

    updatePlayerMoney:function(pos)
    {
        this.players[pos].updatePlayerMoney();
    },


    setWaiteTipVisible:function(pos,is,value)
    {
        this.players[pos].setWaiteTip(is,value);
    },

    stopAllPlayTimer:function()
    {
        for(var i =0 ; i < 5;++i)
        {
            this.players[i].stopTimer();
        }
    },

    resetZang:function()
    {
        for(var i =0 ; i < 5;++i)
        {
            this.players[i].resetzang();
        }
    },

    reset:function(pos)
    {
        if(pos == null)
        {
            for(var i =0 ; i < 5;++i) {
                this.players[i].reset();
            }
        }
        else
        {
            if(pos<0 || pos >4)
            {
                return;
            }
            this.players[pos].reset();
        }
    },
    //上桌
    uptable:function(player)
    {
        var pos = YZDNModel.getPosBySeatID(player.seatid);
        if(pos<0 || pos >4)
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
        if(pos<0 || pos >4)
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

        if(pos<0 || pos >4)
        {
            cc.log("setReady erro:"+pos);
            return;
        }

        this.players[pos].showUI("playready",is);
    },

    setgray:function(pos,is)
    {

        if(pos<0 || pos >4)
        {
            cc.log("setgray erro:"+pos);
            return;
        }

        this.players[pos].showUI("playgray",is);
    },

    setOffline:function(pos,is)
    {
        if(pos<0 || pos >4)
        {
            cc.log("setOffline erro:"+pos);
            return;
        }
        this.players[pos].showUI("playlose",is);
    },
    showWinEf:function(pos,is)
    {
        if(pos<0 || pos >4)
        {
            return ;
        }
        this.players[pos].showUI("playwin",is);
    },

    updateInfo:function(pos)
    {
        if(pos<0 || pos >4)
        {
            return ;
        }
        this.players[pos].getUI("play_money_bg").getChildByName("num").setString(this.players[pos].player.money);
    },
    getSex:function(pos)
    {
        if(pos<0 || pos >4)
        {
            return "f";
        }
        return  this.players[pos].player.sex ==1 ?"m":"f";
    },

    getPosition:function(pos)
    {
        if(pos<0 || pos >4)
        {
            return cc.p(0,0);
        }
        return  this.players[pos].uiNode.getPosition();
    }

};


