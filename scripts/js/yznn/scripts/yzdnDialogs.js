var YZDNBigfaceBox = ccui.Layout.extend(
    {
        pos:0,
        ctor: function ()
        {
            var self =this;
            this._super();
            this.setContentSize(cc.director.getWinSize());
            this.setTouchEnabled(true);
            this.addClickEventListener(function (sender, evt) {
                self.removeFromParent(true);
            });

            this.uiNode = [];
            this.uiNode[3] = ccs.load("public/res/bigfaceCL1.json").node;
            this.addChild(this.uiNode[3] );

            this.uiNode[1] = ccs.load("public/res/bigfaceCR1.json").node;
            this.addChild(this.uiNode[1] );

            this.uiNode[2] = ccs.load("public/res/bigfaceCT1.json").node;
            this.addChild(this.uiNode[2] );


            for(var i =1;i<4;++i)
            {
                var bg = utils.findNode(this.uiNode[i],"bg");
                for(var j =1;j<8;++j)
                {
                    this.addUIClickEvt(bg, "btn" + j);
                }
                this.addUIClickEvt(bg, "btn8");
                bg.setVisible(false);
            }


            return true;
        },
        addUIClickEvt:function(node,key)
        {
            var self = this;
            var ui= node.getChildByName(key);
            if(ui) {
                ui.addClickEventListener(function (sender, evt) {

                    if(key == "btn7")
                    {
                        YZDNPlayers.publicBox.showPlayerList();
                    }
                    else
                    {
                       var id = Number(key.substr(3,1));
                        if( utils.getTimeStamp() - YZDNTips.bigfaceTime >2000)
                        {
                            YZDNModel.sendEmotion(self.pos,id);
                            YZDNTips.bigfaceTime = utils.getTimeStamp();
                        }
                        var b = YZDNPlayers.getPosition(0);
                        var t  =YZDNPlayers.getPosition(self.pos);
                        YZDNTips.showBigface(b,t,id,YZDNPlayers.getSex(0),self.pos);
                    }
                    self.removeFromParent(true);
                });
            }
        },

        showPos:function(pos)
        {

            

            var spos = 0;
            if(pos == 1 || pos == 2)
            {
                spos = 1;
            }else if(pos == 3 || pos == 4 )
            {
                spos = 3;
            }

            if(!this.uiNode[spos])
            {
                return;
            }

            var bg = utils.findNode(this.uiNode[spos],"bg");
            if(!bg)
            {
                return;
            }

            bg.setPosition(utils.getPlayerPos(pos))
            this.pos = pos;
            bg.setVisible(true);
            bg.setScale(0.1);
            bg.stopAllActions();
            bg.runAction(cc.scaleTo(0.3,1));
        }
    }
);

//--------添加小结算界面---------------

var YZDNBalanceItem = ccui.Layout.extend(
    {
        ctor: function () {
            this._super();
            var json = ccs.load(PackName + "/res/btemp.json");
            this.uiNode = json.node;
            this.addChild(this.uiNode);
            this.setContentSize(this.uiNode.getContentSize());
            this.setAnchorPoint(cc.p(0.5, 0.5));
        },
        getUI:function(key)
        {
            return utils.findNode(this.uiNode,key);
        },

        show:function(player)
        {
                var pos = YZDNModel.getPosBySeatID(player.seatid);

                var Image = this.getUI("playImage");
                var m_pAvatar = utils.createCircleAvatar(player.avatar, "yznn/res/pic/p_bg.png", "yznn/res/pic/p_bg.png", cc.size(92,92));
                m_pAvatar.setPosition(cc.p(50,53));
                Image.addChild(m_pAvatar,10);

                var name = this.getUI("playname");
                name.ignoreContentAdaptWithSize(true);
                name.setString(player.name);

                var cardtype = this.getUI("cardtype_bg");

                var card1 = this.getUI("card1");
                card1.loadTexture("YZDN/card_00.png",1);
                var card2 = this.getUI("card2");
                card2.loadTexture("YZDN/card_00.png",1);
                var card3 = this.getUI("card3");
                card3.loadTexture("YZDN/card_00.png",1);
                var card4 = this.getUI("card4");
                card4.loadTexture("YZDN/card_00.png",1);
                var card5 = this.getUI("card5");
                card5.loadTexture("YZDN/card_00.png",1);

                if(player.betting == 1)
                {

                    if(player.card_type == 0)
                    {
                        cardtype.loadTexture("YZDN/no_niu_bg.png",1);
                    }
                    else
                    {
                        cardtype.loadTexture("YZDN/niuniu_bg.png",1);
                    }
                    cardtype.setVisible(true);

                    var pos = YZDNModel.getPosBySeatID(i);

                    cardtype.getChildByName("type").loadTexture("YZDN/cardtype/"+player.card_type+".png",1);

                    card1.loadTexture("YZDN/card_"+utils.toHex2(player.spellcards[0])+".png",1);
                    card2.loadTexture("YZDN/card_"+utils.toHex2(player.spellcards[1])+".png",1);
                    card3.loadTexture("YZDN/card_"+utils.toHex2(player.spellcards[2])+".png",1);
                    card4.loadTexture("YZDN/card_"+utils.toHex2(player.spellcards[3])+".png",1);
                    card5.loadTexture("YZDN/card_"+utils.toHex2(player.spellcards[4])+".png",1);
                }
                


                var label=null;
                var num = player.winmoney;
                if (num > 0) {
                    label = new  cc.LabelAtlas(":"+num,PackName+ "/res/font/s_number_a.png", 26, 33, '0');
                }
                else if(num == 0)
                {
                    label = new  cc.LabelAtlas((num),PackName+ "/res/font/s_number_f.png", 26, 33, '0');
                }
                else{
                    label = new  cc.LabelAtlas(":"+(-num),PackName+ "/res/font/s_number_f.png", 26, 33, '0');
                }
                label.setAnchorPoint(cc.p(0.5, 0.5));

                var pos = YZDNModel.getPosBySeatID(player.seatid);
                label.setTag(parseInt(8888));
                label.setPosition(cc.p(92,36));
                this.uiNode.addChild(label, 20000);

        }
    });



var YZDNBalanceView = cc.Layer.extend(
    {
        uiJson: null,
        uiNode: null,

        itemPlay0: null,
        itemPlay1: null,
        itemPlay2: null,
        itemPlay3: null,
        itemPlay4: null,

        callBackAction:null,

        list:null,

        shareBg:null,

        ctor: function ()
        {
            this._super();
            this.uiJson = ccs.load(PackName+"/res/balance.json");
            this.uiNode = this.uiJson.node;
            this.addChild(this.uiNode);

            this.shareBg = this.getUI("bg");

            var self = this;
            this.getUI("btn_sure").addClickEventListener(function (sender, evt) {
                if(self.callBackAction != null)
                {
                    self.callBackAction();
                }
            });

            this.list = this.getUI("players");

            return true;
        },

        initdata:function()
        {

            this.list.removeAllChildren();

            var tempPlayer = [];
            for(var i = 0;i < 5; i++)
            {
                var player = YZDNModel.players[i];
                if(player == null || player.ready == 0)
                {
                    continue;
                }

                if(i == YZDNModel.seatID)
                {
                    tempPlayer.push(player);
                }
            }

            for(var i = 0;i < 5; i++)
            {
                var player = YZDNModel.players[i];
                if(player == null || player.ready == 0)
                {
                    continue;
                }

                if(i != YZDNModel.seatID)
                {
                    tempPlayer.push(player);
                }
            }

            var len = tempPlayer.length;
            for(var i = 0;i < len; i++)
            {
                var player = tempPlayer[i];
                var temp = new YZDNBalanceItem();
                temp.show(player);
                this.list.addChild(temp);
            }

        },

        getUI:function(key)
        {
            return this.uiNode.getChildByName(key);
        },
    }
);



var YZDNGameEndItem = ccui.Layout.extend(
    {
        ctor: function () {
            this._super();
            var json = ccs.load(PackName + "/res/temp.json");
            this.uiNode = json.node;
            this.addChild(this.uiNode);
            this.setContentSize(this.uiNode.getContentSize());
            this.setAnchorPoint(cc.p(0.5, 0.5));

            this.getUI("playername").ignoreContentAdaptWithSize(true);
            this.getUI("playid").ignoreContentAdaptWithSize(true);
            this.getUI("winbg").setVisible(false);
        },
        getUI:function(key)
        {
            return utils.findNode(this.uiNode,key);
        },
        show:function(data,flag)
        {
            var Image = this.getUI("item").getChildByName("playImage");
            var avrt = utils.getValStr(data, "avatar");
            this.m_pAvatar = utils.createCircleAvatar(avrt, "yznn/res/pic/p_bg.png", "yznn/res/pic/p_bg.png", cc.size(92,92));
            this.m_pAvatar.setPosition(cc.p(50,53));
            Image.addChild(this.m_pAvatar,10);

            var name = this.getUI("item").getChildByName("playername");
            var playname = utils.getValStr(data, "name");
            name.setString(playname);


            var id = this.getUI("item").getChildByName("playid");
            var playid = utils.getValInt(data, "uid");
            id.setString("ID:"+playid);


            var label=null;
            var num = utils.getValInt(data, "score");
            if (num > 0) {
                label = new  cc.LabelAtlas(":"+num,PackName+ "/res/font/s_number_a.png", 26, 33, '0');
                if(flag == true)
                {
                    this.getUI("winbg").setVisible(true);
                }
            }
            else if(num == 0)
            {
                label = new  cc.LabelAtlas((num),PackName+ "/res/font/s_number_f.png", 26, 33, '0');
            }
            else{
                label = new  cc.LabelAtlas(":"+(-num),PackName+ "/res/font/s_number_f.png", 26, 33, '0');
            }
            label.setAnchorPoint(cc.p(0.5, 0.5));

            label.setTag(parseInt(8888));
            label.setPosition(cc.p(88,35));
            this.uiNode.addChild(label, 20000);

        }
    });


//游戏结束 结算界面
var YZDNGameEndView = cc.Layer.extend(
    {
        uiJson: null,
        uiNode: null,

        m_pAvatar:null,
        shareBg:null,

        ctor: function ()
        {
            this._super();
            this.uiJson = ccs.load(PackName+"/res/geview.json");
            this.uiNode = this.uiJson.node;
            this.addChild(this.uiNode);

            this.getUI("roomnum").ignoreContentAdaptWithSize(true);
            this.getUI("roomtime_s").ignoreContentAdaptWithSize(true);
            this.getUI("roomtime_e").ignoreContentAdaptWithSize(true);
            this.getUI("players").setScrollBarEnabled(false);

            var self = this;

            this.shareBg = this.getUI("shareBg");
            this.getUI("logo").setVisible(false);

            this.getUI("btn_leave").addClickEventListener(function (sender, evt) {
                ZJHModel.getInstance().mySeatid = -1;
                YZDNModel.isGameOver = true;
                ZJHModel.getInstance().roomToShareView(YZDNModel.curRoomID);
                // self.removeFromParent(true);
            });

            this.getUI("btn_share").addClickEventListener(function (sender, evt) {

                YZDNPlayers.publicBox.setVisible(false);
                this.getUI("btn_share").setVisible(false);
                this.getUI("btn_leave").setVisible(false);
                this.getUI("logo").setVisible(true);

                var origin = this.shareBg.getPosition();
                var winSize = this.shareBg.getContentSize();
                this.shareBg.setPosition(0,0);

                var renderTexture = new cc.RenderTexture(winSize.width, winSize.height, cc.Texture2D.PIXEL_FORMAT_RGBA8888);
                renderTexture.beginWithClear(0,0,0,0);
                this.shareBg.visit();
                renderTexture.end();
                renderTexture.saveToFile("record.jpg", cc.IMAGE_FORMAT_JPEG, true,function(text,path){
                    this.shareBg.setPosition(origin);
                    PlatformHelper.WeChatShareImage("",path);
                    YZDNPlayers.publicBox.setVisible(true);
                    this.runAction(cc.sequence(cc.delayTime(0.5),cc.callFunc(function(){
                        this.getUI("btn_share").setVisible(true);
                    }.bind(this))));
                    this.getUI("btn_leave").setVisible(true);
                    this.getUI("logo").setVisible(false);


                    this.runAction(cc.sequence(cc.delayTime(0.5),cc.callFunc(function(){
                        ZJHModel.getInstance().mySeatid = -1;
                        // ZJHModel.getInstance().roomToHall();
                        ZJHModel.getInstance().roomToShareView(YZDNModel.curRoomID);
                    }.bind(this))));
                    
                }.bind(this));
            }.bind(this));

            return true;
        },

        initdata:function(val)
        {

            this.getUI("roomnum").setString(YZDNModel.roomData["gameRoom"]["code"]);
            this.getUI("roomtime_s").setString("开始:"+utils.getTime(YZDNModel.roomData["gameRoom"]["createTime"]));
            this.getUI("roomtime_e").setString("结束:"+utils.getTime(utils.getValInt(val,"time") *1000));

            var list = this.getUI("players");

            var maxScoreIndex = 0;
            var maxScore = 0;

            var temData = [];

            if(val.hasOwnProperty("players"))
             {
                var len  = val["players"].length;
                for(var i = 0;i < len; i++)
                {

                    var uid = val["players"][i]["uid"];

                    if(YZDNModel.isMyUID(uid))
                    {
                        temData.push(val["players"][i]);
                    }
                }
             }

             if(val.hasOwnProperty("players"))
             {
                var len  = val["players"].length;
                for(var i = 0;i < len; i++)
                {

                    var uid = val["players"][i]["uid"];

                    if(!YZDNModel.isMyUID(uid))
                    {
                        temData.push(val["players"][i]);
                    }
                }
             }

                var len  = temData.length;
                for(var i = 0;i < len; i++)
                {

                    var scor = temData[i]["score"];
                   if(scor > maxScore)
                   {
                      maxScore = scor;
                      maxScoreIndex = i;
                   }
                }

                var len  = temData.length;
                for(var i = 0;i < len; i++)
                {

                    var data = temData[i];
                    var item  = new YZDNGameEndItem();
                    if(maxScoreIndex == i)
                    {
                        item.show(data,true);
                    }
                    else
                    {
                        item.show(data,false);
                    }
                    
                    list.addChild(item);
                }
            
        },

        getUI:function(key)
        {
            // return this.uiNode.getChildByName(key);
            return utils.findNode(this.uiNode,key);
        },
    }
);
