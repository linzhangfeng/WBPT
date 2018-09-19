
var DDZOverItem = ccui.Layout.extend(
    {
        ctor: function () {
            this._super();
            var json = ccs.load(PackName + "/res/overItem.json");
            this.uiNode = json.node;
            this.addChild(this.uiNode);
            this.setContentSize(this.uiNode.getContentSize());
            this.setAnchorPoint(cc.p(0.5, 0.5));
            this.getUI("name").ignoreContentAdaptWithSize(true);
            this.getUI("id").ignoreContentAdaptWithSize(true);
            this.getUI("djNum").ignoreContentAdaptWithSize(true);
            this.getUI("zdNum").ignoreContentAdaptWithSize(true);
            this.getUI("score").ignoreContentAdaptWithSize(true);
            this.getUI("winNum").ignoreContentAdaptWithSize(true);
            this.getUI("lossNum").ignoreContentAdaptWithSize(true);
        },
        getUI:function(key)
        {
            return utils.findNode(this.uiNode,key);
        },
        show:function(val,fz)
        {
            utils.showAvatarIcon(utils.findNode(this.uiNode,"icon"),val["avatar"],val["sex"]);
            utils.findNode(this.uiNode,"name").setString(utils.parseName(8,val["name"]));
            this.getUI("id").setString("ID:"+val["uid"]);
            this.getUI("fz").setVisible(fz==val["uid"]);

            this.getUI("fail").setVisible(val["total_score"]<0);
            this.getUI("djNum").setString(utils.ssNum(val["max_score"]));
            this.getUI("zdNum").setString(utils.ssNum(val["bomb_cnt"]));
            this.getUI("winNum").setString(utils.ssNum(val["total_win"]));
            this.getUI("lossNum").setString(utils.ssNum(val["total_lose"]));
            var score = val["total_score"];
            this.getUI("score").setString(utils.sjNum(score));
            var win = score >=0;
            this.getUI("winBg").setVisible(win);
            this.getUI("fail").setVisible(!win);
            this.getUI("djTxt1").setVisible(win);
            this.getUI("zdTxt1").setVisible(win);
            this.getUI("jsTxt1").setVisible(win);
            this.getUI("djTxt0").setVisible(!win);
            this.getUI("zdTxt0").setVisible(!win);
            this.getUI("jsTxt0").setVisible(!win);
            this.getUI("score").setColor(win?cc.RED:cc.WHITE);

        }
    });
var DDZOver = cc.Layer.extend(
    {
        ctor: function () {
            this._super();

            this.uiJson = ccs.load(PackName+"/res/overBox.json");
            this.uiNode = this.uiJson.node;
            this.addChild(this.uiNode);

            this.getUI("room").ignoreContentAdaptWithSize(true);
            this.getUI("jushu").ignoreContentAdaptWithSize(true);
            this.getUI("startTime").ignoreContentAdaptWithSize(true);
            this.getUI("endTime").ignoreContentAdaptWithSize(true);
            this.getUI("players").setScrollBarEnabled(false);

            utils.scaleAll(this.getUI("bg"));

            this.shareBg = this.getUI("shareBg");
            this.getUI("logo").setVisible(false);

            var self = this;
            this.getUI("returnBtn").addClickEventListener(function (sender, evt) {
                ZJHModel.getInstance().mySeatid = -1;
                ZJHModel.getInstance().roomToShareView(DDZModel.curRoomID);
            });
            this.getUI("shareBtn").addClickEventListener(function (sender, evt) {

                DDZPlayers.publicBox.setVisible(false);
                this.getUI("shareBtn").setVisible(false);
                this.getUI("returnBtn").setVisible(false);
                this.getUI("logo").setVisible(true);

                var origin = this.shareBg.getPosition();
                var winSize = this.shareBg.getContentSize();
                this.shareBg.setPosition(0,0);

                var renderTexture = new cc.RenderTexture(winSize.width, winSize.height, cc.Texture2D.PIXEL_FORMAT_RGBA8888);
                renderTexture.beginWithClear(0,0,0,0);
                this.shareBg.visit();
                renderTexture.end();
                renderTexture.saveToFile("record.jpg", cc.IMAGE_FORMAT_JPEG, true,function(text,path){
                    this.runAction(cc.sequence(cc.delayTime(0),cc.callFunc(function(){
                        this.shareBg.setPosition(origin);
                        this.getUI("returnBtn").setVisible(true);
                        this.getUI("logo").setVisible(false);
                        DDZPlayers.publicBox.setVisible(true);
                        ZJHModel.getInstance().mySeatid = -1;
                        ZJHModel.getInstance().roomToShareView(DDZModel.curRoomID);
                        PlatformHelper.WeChatShareImage("",path);
                    }.bind(this))));
                }.bind(this));
            }.bind(this));

            return true;
        },
        getUI:function(key)
        {
            return utils.findNode(this.uiNode,key);
        },
        show:function(val)
        {
            this.val =val;
            this.getUI("room").setString("房间:"+this.getPlay()+"\n房号:"+DDZModel.roomData["gameRoom"]["code"]);
            this.getUI("startTime").setString("开始:"+utils.getTime(DDZModel.roomData["gameRoom"]["createTime"]));
            this.getUI("endTime").setString("结束:"+utils.getTime(utils.getValInt(val,"end_time") *1000));
            this.getUI("jushu").setString(utils.format("局数:%1/%2",DDZModel.curJuShu,DDZModel.maxJuShu));

            var list = this.getUI("players");

            var fz =val["owner_uid"];

            if(!val["players"])
            {
                return;
            }

            var win =  this.getUI("win");
            var light =  this.getUI("light");
            var fail =  this.getUI("fail");
            var myWin = false;
            for(var i in val["players"])
            {
                var d = val["players"][i];
                if(DDZModel.isMySeatID(d["seatid"]))
                {
                    myWin = d["total_score"]>=0;
                }
                var item  = new DDZOverItem();
                item.show(d,fz);
                list.addChild(item);

            }
            if(DDZModel.maxPlayer == 2)
            {
                list.setContentSize(cc.size(700,list.getContentSize().height));
                list.setItemsMargin(130);
            }

            light.runAction(cc.repeatForever(cc.rotateBy(12,360)));
            win.setVisible(myWin);
            light.setVisible(myWin);
            fail.setVisible(!myWin);
        },
        getPlay:function()
        {
            if(DDZModel.maxPlayer == 2)
            {
                return "二人跑得快("+ DDZModel.table_type+"张)";
            }

            return "跑得快("+ DDZModel.table_type+"张)";
        }

    }
);