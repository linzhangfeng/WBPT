
var PHZPlayer =
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
        this.uiNode.setVisible(false);

       // this.showUI("ready",false);
        this.showUI("offline",false);
        this.showUI("think",false);
        this.getUI("coin").ignoreContentAdaptWithSize(true);
        this.getUI("icon").ignoreContentAdaptWithSize(false);
        this.getUI("name").ignoreContentAdaptWithSize(true);
        this.showUI("fz",false);
        var self = this;
        this.getUI("btn").addClickEventListener(function (sender, evt) {

            if(PHZModel.isMySeatID(self.seatID))
            {
                PHZPlayers.publicBox.showPlayerList();
            }
            else
            {
                if(PHZModel.isMyInTable())
                {
                    var box = new PHZBigfaceBox();
                    box.showPos(PHZModel.getPosBySeatID(self.seatID));
                    cc.Director.getInstance().getRunningScene().addChild(box,100);
                }
                else
                {
                    PHZPlayers.publicBox.showPlayerList();
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
        return  utils.findNode(this.uiNode,key);
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

    updateSeatID:function(seatID)
    {
        this.seatID = seatID;
        this.player = PHZModel.getPlayerBySeatID(seatID);
        if(this.player == null)
        {
            cc.log("updateSeatID erro player:"+seatID);
            return;
        }
        if(this.player.name == undefined)
        {
            this.player.name = "";
        }
        this.getUI("name").setString(utils.parseName(6,this.player.name));
        if(PHZModel.endScore>=0)
        {
            this.getUI("coin").setString("总胡"+(this.player.money-PHZModel.roomGolden));
        }
        else
        {
            this.getUI("coin").setString((this.player.money-PHZModel.roomGolden));
        }

        utils.showAvatarIcon(this.getUI("icon"),this.player.avatar,this.player.sex);
        this.uiNode.setVisible(true);

    },
    reset:function()
    {
      //  this.showUI("ready",false);
        this.showUI("offline",false);
        this.showUI("think",false);
    }
};

var PHZPlayers =
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
            this.players[i] = utils.clone(PHZPlayer);
            this.players[i].uiNode = this.getUI("player"+i);
            this.players[i].init();
        }

    },
    release:function()
    {
    },

    getUI:function(key)
    {
        return  utils.findNode(this.uiNode,key);
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
        var pos = PHZModel.getPosBySeatID(player.seatid);
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
        PHZTips.showHuxi(pos,-1);
        PHZTips.showPiao(pos,-1);
        PHZTips.showNiao(pos,-1);

        PHZTips.showReady(pos,false);
    },
    updateInfo:function(pos)
    {
        if(pos<0 || pos >2)
        {
            return ;
        }
        if(PHZModel.endScore>=0)
        {
            this.players[pos].getUI("coin").setString("总胡"+(this.players[pos].player.money-PHZModel.roomGolden));
        }
        else
        {

            this.players[pos].getUI("coin").setString((this.players[pos].player.money-PHZModel.roomGolden));
        }
    },
    getSex:function(pos)
    {
        if(pos<0 || pos >2)
        {
            return "f";
        }
        return  this.players[pos].player.sex ==1 ?"m":"f";
    },
    getPosition:function(pos)
    {
        if(pos<0 || pos >2)
        {
            return cc.p(0,0);
        }
        return  utils.addPP(this.players[pos].uiNode.getPosition(),cc.p(48,70)) ;
    },

    //设置准备状态
    setReady:function(pos,is)
    {

        if(pos<0 || pos >2)
        {
            return;
        }

        PHZTips.showReady(pos,is);
    },
    setOffline:function(pos,is)
    {
        if(pos<0 || pos >2)
        {
            return;
        }
        this.players[pos].showUI("offline",is);
    },
    setThink:function(pos,is)
    {
        if(pos<0 || pos >2)
        {
            return;
        }
        this.players[pos].showUI("think",is);
    },
    setFZ:function(pos,is)
    {
        if(pos<0 || pos >3)
        {
            return;
        }
        this.players[pos].showUI("fz",is);
    },
};


