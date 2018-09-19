
var PHZOverItem = ccui.Layout.extend(
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
            this.getUI("hpNum").ignoreContentAdaptWithSize(true);
            this.getUI("zzNum").ignoreContentAdaptWithSize(true);
            this.getUI("dpNum").ignoreContentAdaptWithSize(true);
            this.getUI("score").ignoreContentAdaptWithSize(true);
        },
        getUI:function(key)
        {
            return utils.findNode(this.uiNode,key);
        },
        show:function(val,fz,isWin,isLoss,isPao)
        {
            utils.showAvatarIcon(utils.findNode(this.uiNode,"icon"),val["avatar"],val["sex"]);
            utils.findNode(this.uiNode,"name").setString(utils.parseName(6,utils.getValStr( val,"name")));
            this.getUI("id").setString("ID:"+val["uid"]);
            this.getUI("fz").setVisible(fz==val["uid"]);
            this.getUI("win").setVisible(isWin);
            this.getUI("loss").setVisible(isLoss);
            this.getUI("pao").setVisible(isPao);

            this.getUI("fail").setVisible(val["total_score"]<0);
            this.getUI("hpNum").setString(utils.ssNum(val["hu_count"]));
            this.getUI("zzNum").setString(utils.ssNum(val["zhongzhuang_count"]));
            this.getUI("dpNum").setString(utils.ssNum(val["dianpao_count"]));
            var score = val["total_score"];
            this.getUI("score").setString(utils.sjNum(score));
            var win = score >=0;
            this.getUI("winBg").setVisible(win);
            this.getUI("fail").setVisible(!win);
            this.getUI("hpTxt1").setVisible(win);
            this.getUI("zzTxt1").setVisible(win);
            this.getUI("dpTxt1").setVisible(win);
            this.getUI("hpTxt0").setVisible(!win);
            this.getUI("zzTxt0").setVisible(!win);
            this.getUI("dpTxt0").setVisible(!win);
            this.getUI("score").setColor(win?cc.RED:cc.WHITE);
        }
    });

var PHZOver = cc.Layer.extend(
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
            this.sharePos =   this.shareBg.getPosition();
            this.getUI("logo").setVisible(false);

            var self = this;
            this.getUI("returnBtn").addClickEventListener(function (sender, evt) {
                ZJHModel.getInstance().mySeatid = -1;
                ZJHModel.getInstance().roomToShareView(PHZModel.curRoomID);
            });
            this.getUI("shareBtn").addClickEventListener(function (sender, evt) {

                var winSize = cc.director.getWinSize();
                var size =this.shareBg.getContentSize();
                var s = 1;
                PHZPlayers.publicBox.setVisible(false);
                this.getUI("shareBtn").setVisible(false);
                this.getUI("returnBtn").setVisible(false);
                this.getUI("logo").setVisible(true);
                this.shareBg.setPosition(0,0);

                this.shareBg.setScale(s);
                var renderTexture = new cc.RenderTexture(size.width*s, size.height*s, cc.Texture2D.PIXEL_FORMAT_RGBA8888);
                renderTexture.setPosition(this.sharePos );
                renderTexture.beginWithClear(0,0,0,0);
                this.shareBg.visit();
                renderTexture.end();
                renderTexture.saveToFile("record.jpg", cc.IMAGE_FORMAT_JPEG, true,function(text,path){

                    this.runAction(cc.sequence(cc.delayTime(0),cc.callFunc(function(){
                        this.shareBg.setPosition( this.sharePos );
                        this.shareBg.setScale(1);
                        PHZPlayers.publicBox.setVisible(true);
                        this.getUI("returnBtn").setVisible(true);
                        this.getUI("logo").setVisible(false);
                        ZJHModel.getInstance().mySeatid = -1;
                        ZJHModel.getInstance().roomToShareView(PHZModel.curRoomID);
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
            this.getUI("room").setString("房间:"+this.getPlay()+"  房号:"+PHZModel.roomData["gameRoom"]["code"]);
            this.getUI("startTime").setString("开始:"+utils.getTime(PHZModel.roomData["gameRoom"]["createTime"]));
            this.getUI("endTime").setString("结束:"+utils.getTime(val["ts"]*1000));
            this.getUI("jushu").setString(utils.format("局数:%1/%2",PHZModel.curJuShu,PHZModel.maxJuShu));

            var list = this.getUI("players");

            var fz =val["room_userid"];

            if(!val.hasOwnProperty("players"))
            {
                return;
            }
            var winMaxID =[];
            var lossMaxID =[];
            var paoMaxID =[];
            if(val["players"].length>1)
            {
                val["players"].sort(function(v1,v2){

                    if(v1["total_score"]> v2["total_score"])
                    {
                        return 1;
                    }
                    else if(v2["total_score"] <v1["total_score"])
                    {
                        return -1;
                    }
                    return 0;
                });
                var w = val["players"][val["players"].length-1]["total_score"];
                var l = val["players"][0]["total_score"];
                if(w !=l )
                {
                    for(var i in val["players"])
                    {
                        var d = val["players"][i];
                        if( d["total_score"]==w)
                        {
                            winMaxID.push(d["uid"]);
                        }
                        if( d["total_score"]==l)
                        {
                            lossMaxID.push(d["uid"]);
                        }
                    }
                }

                    val["players"].sort(function (v1, v2) {

                        if (v1["dianpao_count"] < v2["dianpao_count"]) {
                            return 1;
                        }
                        else if (v2["dianpao_count"] > v1["dianpao_count"]) {
                            return -1;
                        }
                        return 0;
                    });

                    var w = val["players"][val["players"].length-1]["dianpao_count"];
                    var l = val["players"][0]["dianpao_count"];
                    if(w !=l )
                    {
                        for(var i in val["players"])
                        {
                            var d = val["players"][i];
                            if( d["dianpao_count"]==l)
                            {
                                paoMaxID.push(d["uid"]);
                            }
                        }
                    }
                val["players"].sort(function(v1,v2){

                    if(v1["seatid"]> v2["seatid"])
                    {
                        return 1;
                    }
                    else if(v2["seatid"] <v1["seatid"])
                    {
                        return -1;
                    }
                    return 0;
                });
            }

            var win =  this.getUI("win");
            var light =  this.getUI("light");
            var fail =  this.getUI("fail");
            var myWin = false;
            for(var i in val["players"])
            {
                var d = val["players"][i];
                var  item  = new PHZOverItem();
                item.show(d,fz,winMaxID.indexOf(d["uid"])!=-1,lossMaxID.indexOf(d["uid"])!=-1,paoMaxID.indexOf(d["uid"])!=-1);
                list.addChild(item);

                if(PHZModel.isMySeatID(d["seatid"]) )
                {
                    myWin = d["total_score"]>=0;
                }
            }

            light.runAction(cc.repeatForever(cc.rotateBy(12,360)));
            win.setVisible(myWin);
            light.setVisible(myWin);
            fail.setVisible(!myWin);
        },
        getPlay:function()
        {
            return "碰胡";
        }

    }
);