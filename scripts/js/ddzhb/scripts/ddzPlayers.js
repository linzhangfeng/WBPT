
var DDZPlayer =
{
    seatID:-1,
    uiNode:null,
    player:null,
    init:function(seatID)
    {
        if(this.uiNode == null)
        {
            return;
        }
        this.seatID = seatID;
        this.showUI("tuoguan",false);
        this.showUI("winEf",false);
        this.showUI("ready",false);
        this.showUI("offline",false);
        this.uiNode.setVisible(false);

        this.getUI("name").ignoreContentAdaptWithSize(true);
                this.getUI("icon").ignoreContentAdaptWithSize(false);
        this.getUI("winEf").runAction(cc.repeatForever(utils.newAni("ddz/win/v_win_user_anim_",1,5,0.1)));
        this.getUI("offline").runAction(cc.repeatForever(cc.sequence(cc.fadeIn(0.8),cc.fadeOut(0.8))));
        var self = this;
        this.uiNode.addClickEventListener(function (sender, evt) {

            if(DDZModel.isMySeatID(self.seatID))
            {
                DDZPlayers.publicBox.showPlayerList();
            }
            else
            {
                if(DDZModel.isMyInTable())
                {
                    var box = new DDZBigfaceBox();
                    box.showPos(DDZModel.getPosBySeatID(self.seatID));
                    cc.Director.getInstance().getRunningScene().addChild(box,100);
                }
                else
                {
                    DDZPlayers.publicBox.showPlayerList();
                }
            }
        });
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

    updateSeatID:function(seatID)
    {
        this.seatID = seatID;
        this.player = DDZModel.getPlayerBySeatID(seatID);
        if(this.player == null)
        {
            cc.log("updateSeatID erro player:"+seatID);
            return;
        }
        this.getUI("name").setString(utils.parseName(6,this.player.name));
        this.getUI("coin").setString(this.player.money-DDZModel.roomGolden);

        utils.showAvatarIcon(this.getUI("icon"),this.player.avatar,this.player.sex);
        this.uiNode.setVisible(true);

    },
    reset:function()
    {
        this.showUI("tuoguan",false);
        this.showUI("winEf",false);
        this.showUI("ready",false);
        this.showUI("offline",false);
    }
};

var DDZPlayers =
{
    uiNode:null,
    players:{},
    init:function()
    {
        if(this.uiNode == null)
        {
            return;
        }

        for(var i =0 ; i < 3;++i)
        {
            this.players[i] = utils.clone(DDZPlayer);
            this.players[i].uiNode = this.getUI("player"+i);
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
    reset:function(pos)
    {
        if(pos == null)
        {
            for(var i =0 ; i < 3;++i) {
                this.players[i].reset();
            }
        }
        else
        {
            if(pos<0 || pos >2)
            {
                return;
            }
            this.players[pos].reset();
        }
    },
    //上桌
    uptable:function(player)
    {
        var pos = DDZModel.getPosBySeatID(player.seatid);
        if(pos<0 || pos >2)
        {
            cc.log("uptable erro:"+pos);
            return;
        }

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
        if(pos<0 || pos >2)
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

        if(pos<0 || pos >2)
        {
            cc.log("setReady erro:"+pos);
            return;
        }

        this.players[pos].showUI("ready",is);
    },
    //设置托管状态
    setTuoguan:function(pos,is)
    {
        if(pos<0 || pos >2)
        {
            cc.log("setTuoguan erro:"+pos);
            return;
        }

        this.players[pos].showUI("tuoguan",is);
    },
    setOffline:function(pos,is)
    {
        if(pos<0 || pos >2)
        {
            cc.log("setOffline erro:"+pos);
            return;
        }
        this.players[pos].showUI("offline",is);
    },
    showWinEf:function(pos,is)
    {
        if(pos<0 || pos >2)
        {
            return ;
        }
        this.players[pos].showUI("winEf",is);
        this.players[pos].getUI("winEf").runAction(cc.sequence(cc.delayTime(1.5)
            ,cc.hide()));
    },
    updateInfo:function(pos)
    {
        if(pos<0 || pos >2)
        {
            return ;
        }
        this.players[pos].getUI("name").setString(utils.parseName(6,this.players[pos].player.name));
        this.players[pos].getUI("coin").setString(this.players[pos].player.money -DDZModel.roomGolden);
    },
    getSex:function(pos)
    {
        if(pos<0 || pos >2)
        {
            return "f";
        }
        return  this.players[pos].player.sex ==1 ?"m":"f";
    },
    getSex1:function(pos)
    {
        if(pos<0 || pos >2)
        {
            return "w";
        }
        return  this.players[pos].player.sex ==1 ?"m":"w";
    },
    getPosition:function(pos)
    {
        if(pos<0 || pos >2)
        {
            return cc.p(0,0);
        }
        return  this.players[pos].uiNode.getPosition();
    }

};


