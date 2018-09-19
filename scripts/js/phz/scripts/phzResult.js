
var PHZResultItem1 = ccui.Layout.extend(
    {
        ctor:function ()
        {
            this._super();
            var json = ccs.load(PackName+"/res/resultItem1.json");
            this.uiNode =json.node;
            this.value =INVAIL_CARD;
            this.addChild(this.uiNode);
            this.setContentSize(this.uiNode.getContentSize());
            this.setAnchorPoint(cc.p(0.5,0.5));
            this.getUI("num").ignoreContentAdaptWithSize(true);
            for(var i =0;i<4;++i)
            {
                this.getUI("card"+i).setVisible(false);
            }
        },
        getUI:function(key)
        {
            return  utils.findNode(this.uiNode,key);
        },
        show:function(val)
        {
            this.getUI("num").setString(val["hu_xi"]);
            for(var i in val["card_list"])
            {
                this.getUI("card"+i).setVisible(true);
                if(val["card_list"][i]!=0)
                {
                    this.getUI("face"+i).loadTexture("phz/card_s_"+utils.toHex2(val["card_list"][i])+".png",1);
                }
            }
            var kind = val["weave_kind"];
            var file = "phz";
            switch (kind)
            {
                case PHZAck.ACK_CHI:
                    file += "/txt_chi.png";
                    break;
                case PHZAck.ACK_PAO:
                    file += "/txt_pao.png";
                    break;
                case PHZAck.ACK_PENG:
                    file += "/txt_peng.png";
                    break;
                case PHZAck.ACK_TI:
                    file += "/txt_ti.png";
                    break;
                case PHZAck.ACK_WEI:
                    file += "/txt_wei.png";
                    break;
                case PHZAck.ACK_KAN:
                    file += "/txt_kan.png";
                    break;
                case PHZAck.ACK_JIANG:
                    file += "/txt_jiang.png";
                    break;
            }
            this.getUI("type").loadTexture(file,1);
        }
    });


var PHZResultItem2 = ccui.Layout.extend(
    {
        ctor:function ()
        {
            this._super();
            var json = ccs.load(PackName+"/res/resultItem2.json");
            this.uiNode =json.node;
            this.value =INVAIL_CARD;
            this.addChild(this.uiNode);
            this.setContentSize(this.uiNode.getContentSize());
            this.setAnchorPoint(cc.p(0.5,0.5));
            this.getUI("win").setVisible(false);
            this.getUI("loss").setVisible(false);
            this.getUI("pao").setVisible(false);
            this.getUI("name").ignoreContentAdaptWithSize(true);
            this.getUI("coin").ignoreContentAdaptWithSize(true);
            this.getUI("niao").setVisible(false);
            this.getUI("piao").setVisible(false);
            utils.findNode( this.getUI("niao"),"num").ignoreContentAdaptWithSize(true);
            utils.findNode( this.getUI("piao"),"num").ignoreContentAdaptWithSize(true);
            utils.findNode( this.getUI("win"),"num").ignoreContentAdaptWithSize(true);
            utils.findNode( this.getUI("loss"),"num").ignoreContentAdaptWithSize(true);
        },
        getUI:function(key)
        {
            return  utils.findNode(this.uiNode,key);
        },
        show:function(val,pao)
        {
            utils.showAvatarIcon(this.getUI("icon"),val["avatar"],val["sex"]);
            this.getUI("name").setString(utils.parseName(6,utils.getValStr(val,"name") ));
            var score = utils.getValInt(val,"total_score");
            if (score<=0)
            {
                this.getUI("loss").setVisible(true);
                utils.findNode( this.getUI("loss"),"num").setString(":"+Math.abs(score));
            }
            else
            {
                this.getUI("win").setVisible(true);
                utils.findNode( this.getUI("win"),"num").setString(":"+score);
            }
            this.getUI("fz").setVisible(utils.getValInt(val,"uid")==PHZModel.fzUID);
            if(PHZModel.endScore <0)
            {
                this.getUI("coin").setString("总"+(utils.getValInt(val,"money")-PHZModel.roomGolden));
            }
            else{
                this.getUI("coin").setString("总胡"+(utils.getValInt(val,"money")-PHZModel.roomGolden));
            }

            if(PHZModel.endScore <0&&(PHZTips.hasNiao() ||PHZTips.hasPiao()))
            {
                if(PHZTips.hasNiao())
                {
                    this.getUI("niao").setVisible(true);
                    this.getUI("niao").getChildByName("num").setString(utils.getValInt(val,"niao"));
                }
                if(PHZTips.hasPiao())
                {
                    this.getUI("piao").setVisible(true);
                    this.getUI("piao").getChildByName("num").setString(utils.getValInt(val,"piao"));
                }
            }
            this.getUI("pao").setVisible(val["seatid"] == pao);
        }
    });
var PHZResultItem3 = ccui.Layout.extend(
    {
        ctor:function ()
        {
            this._super();
            var json = ccs.load(PackName+"/res/resultItem3.json");
            this.uiNode =json.node;
            this.value =INVAIL_CARD;
            this.addChild(this.uiNode);
            this.setContentSize(this.uiNode.getContentSize());
            this.setAnchorPoint(cc.p(0.5,0.5));
            this.getUI("txt").ignoreContentAdaptWithSize(true);
        },
        getUI:function(key)
        {
            return  utils.findNode(this.uiNode,key);
        },
        show:function(type,num,dat)
        {
            var mess = "";
            var d ="番数";
            if(dat ==1)
            {
                d ="囤数";
            }
            else if(dat ==2)
            {
                d ="胡息";
            }
            switch (type)
            {
                case PHZFan.MT_DIAN_PAO:
                    mess+="点炮+"+num+d;
                    break;
                case PHZFan.MT_ZI_MO:
                    mess+="自摸+"+num+d;
                    break;
                case PHZFan.MT_TIAN_HU:
                    mess+="天胡+"+num+d;
                    break;
                case PHZFan.MT_TIAN_HU_3TI5KAN_:
                    mess+="天胡三提五坎";
                    break;
                case PHZFan.MT_DI_HU:
                    mess+="地胡+"+num+d;
                    break;
                case PHZFan.MT_DI_HU_LIANG:
                    mess+="地胡胡亮张+"+num+d;
                    break;
                case PHZFan.MT_DIAN_HU:
                    mess+="一点红+"+num+d;
                    break;
                case  PHZFan.MT_SHI_HONG:
                    mess+="十红+"+num+d;
                    break;
                case  PHZFan.MT_HONG_HU:
                    mess+="红胡+"+num+d;
                    break;
                case  PHZFan.MT_HONG_WU:
                    mess+="十三红+"+num+d;
                    break;
                case  PHZFan.MT_WU_HU:
                    mess+="乌胡+"+num+d;
                    break;
                case PHZFan.MT_DUI_DUI_HU:
                    mess+="对对胡+"+num+d;
                    break;
                case PHZFan.MT_SHI_BA_DA:
                    mess+="十八大+"+num+d;
                    break;
                case PHZFan.MT_SHI_BA_XIAO:
                    mess+="十八小+"+num+d;
                    break;
                case PHZFan.MT_SHI_LIU_XIAO:
                    mess+="十六小+"+num+d;
                    break;
                case PHZFan.MT_HAI_DI_HU:
                    mess+="海底胡+"+num+d;
                    break;
                case PHZFan.MT_SHUA_HOU:
                    mess+="耍猴+"+num+d;
                    break;
                case PHZFan.MT_ER_BI:
                    mess+="二比+"+num+d;
                    break;
                case PHZFan.MT_SAN_BI:
                    mess+="三比+"+num+d;
                    break;
                case PHZFan.MT_SI_BI:
                    mess+="四比+"+num+d;
                    break;
                case PHZFan.MT_SHUANG_PIAO:
                    mess+="双飘+"+num+d;
                    break;
                case PHZFan.MT_HUANG_ZUANG:
                    mess+="黄番  "+d+"X"+num;
                    break;
                case PHZFan.MT_YI_KUAI_BIAN:
                    mess+="一块匾+"+num+d;
                    break;
                case PHZFan.MT_KA_HU:
                    mess+="三十卡胡+"+num+d;
                    break;
                case PHZFan.MT_ER_SHI_HU:
                    mess+="二十卡胡+"+num+d;
                    break;
                case PHZFan.MT_SAN_SHI_HU:
                    mess+="三十胡+"+num+d;
                    break;
                case PHZFan.MT_SAN_SHI_HU_SHI_HONG:
                    mess+="三十胡十红+"+num+d;
                    break;
            }
            this.getUI("txt").setString(mess);
        },

        show1:function(type,num)
        {
            var mess = "";
            switch (type)
            {
                case PHZFan.MT_ZI_MO:
                    mess+="自摸 胡息X"+num;
                    break;
                case PHZFan.MT_TIAN_HU:
                    mess+="天胡 胡息X"+num;
                    break;
                case PHZFan.MT_TIAN_HU_3TI5KAN_:
                    mess+="天胡三提五坎";
                    break;
                case PHZFan.MT_DI_HU:
                    mess+="地胡 胡息X"+num;
                    break;
                case PHZFan.MT_DI_HU_LIANG:
                    mess+="地胡胡亮张 胡息X"+num;
                    break;
                case PHZFan.MT_DIAN_HU:
                    mess+="一点红 胡息X"+num;
                    break;
                case  PHZFan.MT_SHI_HONG:
                    mess+="十红 胡息X"+num;
                    break;
                case  PHZFan.MT_HONG_HU:
                    mess+="红胡 胡息X"+num;
                    break;
                case  PHZFan.MT_HONG_WU:
                    mess+="十三红 胡息X"+num;
                    break;
                case  PHZFan.MT_WU_HU:
                    mess+="乌胡 胡息X"+num;
                    break;
                case PHZFan.MT_DUI_DUI_HU:
                    mess+="对对胡 胡息X"+num;
                    break;
                case PHZFan.MT_SHI_BA_DA:
                    mess+="十八大 胡息X"+num;
                    break;
                case PHZFan.MT_SHI_BA_XIAO:
                    mess+="十八小 胡息X"+num;
                    break;
                case PHZFan.MT_SHI_LIU_XIAO:
                    mess+="十六小 胡息X"+num;
                    break;
                case PHZFan.MT_HAI_DI_HU:
                    mess+="海底胡 胡息X"+num;
                    break;
                case PHZFan.MT_SHUA_HOU:
                    mess+="耍猴 胡息X"+num;
                    break;
                case PHZFan.MT_ER_BI:
                    mess+="二比 胡息X"+num;
                    break;
                case PHZFan.MT_SAN_BI:
                    mess+="三比 胡息X"+num;
                    break;
                case PHZFan.MT_SI_BI:
                    mess+="四比 胡息X"+num;
                    break;
                case PHZFan.MT_SHUANG_PIAO:
                    mess+="双飘 胡息X"+num;
                    break;
                case PHZFan.MT_YI_KUAI_BIAN:
                    mess+="一块匾 胡息X"+num;
                    break;
                case PHZFan.MT_KA_HU:
                    mess+="三十卡胡 胡息X"+num;
                    break;
                case PHZFan.MT_ER_SHI_HU:
                    mess+="二十卡胡  胡息X"+num;
                    break;
                case PHZFan.MT_SAN_SHI_HU:
                    mess+="三十胡  胡息X"+num;
                    break;
                case PHZFan.MT_SAN_SHI_HU_SHI_HONG:
                    mess+="三十胡十红  胡息X"+num;
                    break;
            }
            this.getUI("txt").setString(mess);
        },
        show2:function(type,num)
        {
            var mess = "";
            switch (type)
            {
                case PHZFan.MT_ZI_MO:
                    mess+="自摸 "+num+"胡息";
                    break;
                case PHZFan.MT_TIAN_HU:
                    mess+="天胡 "+num+"胡息";
                    break;
                case PHZFan.MT_TIAN_HU_3TI5KAN_:
                    mess+="天胡三提五坎";
                    break;
                case PHZFan.MT_DI_HU:
                    mess+="地胡 "+num+"胡息";
                    break;
                case PHZFan.MT_DI_HU_LIANG:
                    mess+="地胡胡亮张 胡息X"+num;
                    break;
                case PHZFan.MT_DIAN_HU:
                    mess+="一点红 "+num+"胡息";
                    break;
                case  PHZFan.MT_SHI_HONG:
                    mess+="十红 "+num+"胡息";
                    break;
                case  PHZFan.MT_HONG_HU:
                    mess+="红胡 "+num+"胡息";
                    break;
                case  PHZFan.MT_HONG_WU:
                    mess+="十三红 "+num+"胡息";
                    break;
                case  PHZFan.MT_WU_HU:
                    mess+="乌胡 "+num+"胡息";
                    break;
                case PHZFan.MT_DUI_DUI_HU:
                    mess+="对对胡 "+num+"胡息";
                    break;
                case PHZFan.MT_SHI_BA_DA:
                    mess+="十八大 "+num+"胡息";
                    break;
                case PHZFan.MT_SHI_BA_XIAO:
                    mess+="十八小 "+num+"胡息";
                    break;
                case PHZFan.MT_SHI_LIU_XIAO:
                    mess+="十六小 "+num+"胡息";
                    break;
                case PHZFan.MT_HAI_DI_HU:
                    mess+="海底胡 "+num+"胡息";
                    break;
                case PHZFan.MT_SHUA_HOU:
                    mess+="耍猴 "+num+"胡息";
                    break;
                case PHZFan.MT_ER_BI:
                    mess+="二比 "+num+"胡息";
                    break;
                case PHZFan.MT_SAN_BI:
                    mess+="三比 "+num+"胡息";
                    break;
                case PHZFan.MT_SI_BI:
                    mess+="四比 "+num+"胡息";
                    break;
                case PHZFan.MT_SHUANG_PIAO:
                    mess+="双飘 "+num+"胡息";
                    break;
                case PHZFan.MT_YI_KUAI_BIAN:
                    mess+="一块匾 "+num+"胡息";
                    break;
                case PHZFan.MT_KA_HU:
                    mess+="三十卡胡 "+num+"胡息";
                    break;
                case PHZFan.MT_ER_SHI_HU:
                    mess+="二十卡胡 "+num+"胡息";
                    break;
                case PHZFan.MT_SAN_SHI_HU:
                    mess+="三十胡 "+num+"胡息";
                    break;
                case PHZFan.MT_SAN_SHI_HU_SHI_HONG:
                    mess+="三十胡十红 "+num+"胡息";
                    break;
            }
            this.getUI("txt").setString(mess);
        }
    });


var PHZResult = cc.Layer.extend(
    {
        ctor: function () {
            this._super();

            this.dipai =[];


            this.uiJson = ccs.load(PackName+"/res/result.json");
            this.uiNode = this.uiJson.node;
            this.addChild(this.uiNode);

            this.getUI("room").ignoreContentAdaptWithSize(true);
            utils.scaleAll(this.getUI("bg"));
            this.shareBg = this.getUI("bg1");
            var self = this;
            this.getUI("logo").setVisible(false);

            this.getUI("shareBtn").addClickEventListener(function (sender, evt) {

                PHZPlayers.publicBox.setVisible(false);
                this.getUI("shareBtn").setVisible(false);
                this.getUI("returnBtn").setVisible(false);
                this.getUI("againBtn").setVisible(false);
                this.getUI("logo").setVisible(true);

                var size =this.shareBg.getContentSize();
                var origin = this.shareBg.getPosition();
                this.shareBg.setPosition(0,0);

                var renderTexture = new cc.RenderTexture(size.width, size.height, cc.Texture2D.PIXEL_FORMAT_RGBA8888);
                renderTexture.setPosition(this.shareBg.getPosition() );
                renderTexture.beginWithClear(0,0,0,0);
                this.shareBg.visit();
                renderTexture.end();
                renderTexture.saveToFile("record.jpg", cc.IMAGE_FORMAT_JPEG, true,function(text,path){
                    this.shareBg.setPosition(origin);
                    PlatformHelper.WeChatShareImage("",path);
                    PHZPlayers.publicBox.setVisible(true);
                    this.runAction(cc.sequence(cc.delayTime(0.5),cc.callFunc(function(){
                        this.getUI("shareBtn").setVisible(true);
                    }.bind(this))));
                    this.getUI("returnBtn").setVisible(true);
                    this.getUI("againBtn").setVisible(true);
                    this.getUI("logo").setVisible(false);
                }.bind(this));
            }.bind(this));

            this.getUI("returnBtn").addClickEventListener(function (sender, evt) {
                if(PHZModel.isMyInTable()&&PHZModel.getPlayerByPos(0)&&PHZModel.getPlayerByPos(0).ready !=1)
                {
                    PHZAction.showReadyBtn(true);
                }
                self.removeFromParent(true);

                if(PHZModel.isGameOver)
                {
                    PHZModel.sendOver();
                }
            });
            this.getUI("againBtn").addClickEventListener(function (sender, evt) {

                self.removeFromParent(true);
                if(PHZModel.isGameOver)
                {
                    PHZModel.sendOver();
                }
                else
                {
                    if(!PHZModel.isMyInTable())
                    {
                        PHZModel.sendUptable();
                    }
                    else if(PHZModel.getPlayerByPos(0)&&PHZModel.getPlayerByPos(0).ready !=1)
                    {
                        PHZModel.sendReady();
                    }
                }
            });

            this.time = 20;
            this.dipaiTxt = this.getUI("dipai");
            this.dipaiBg = this.getUI("bg1");
            this.dipai.push( this.dipaiTxt);
            this.getUI("huxi").ignoreContentAdaptWithSize(true);
            this.getUI("dengshu").ignoreContentAdaptWithSize(true);
            this.getUI("fanshu").ignoreContentAdaptWithSize(true);
            utils.findNode( this.getUI("huxi0"),"num").ignoreContentAdaptWithSize(true);
            utils.findNode( this.getUI("jifen"),"num").ignoreContentAdaptWithSize(true);
            this.getUI("weaves").setScrollBarEnabled(false);
            this.getUI("players").setScrollBarEnabled(false);

            this.showUI("jifen",false);
            this.showUI("huxi",false);
            this.showUI("dengshu",false);
            this.showUI("fanshu",false);
            this.showUI("huxi0",false);

            //this.getUI("players").setVisible(false);
           // this.getUI("fanList").setVisible(false);
           // this.getUI("weaves").setVisible(false);
            return true;
        },

        getUI:function(key)
        {
            return utils.findNode(this.uiNode,key);
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
        show:function(val)
        {
         //  val ={"base_money":1,"cmd":4014,"end_type":1,"fans":1,"hu_xis":23,"left_cards":[26,2,21,20,5,24,8,10,10],"players":[{"avatar":"http://188.188.1.111:20084/chess/chess_img/head/101875/1611011850511260.png","hand_cards":[[17,17,17],[25,25,25],[2,7,10],[3,4,5]],"money":1000,"name":"事件处理4278siug","niao":8,"niao_score":192,"piao":50,"piao_score":100,"score":12,"seatid":0,"sex":1,"total_score":304,"uid":101875},{"avatar":"http://188.188.1.111:20084/chess/chess_img/head/101877/1609281402569206.png","hand_cards":[[1,1,17],[2,2]],"money":1000,"name":"阿萨德","niao":8,"niao_score":-96,"piao":0,"piao_score":-50,"score":-6,"seatid":1,"sex":2,"total_score":-152,"uid":101877},{"avatar":"http://188.188.1.111:20084/chess/chess_img/head/default/00.jpg","hand_cards":[[3,3,3],[1,1],[4,5,6],[8,9,10]],"money":1000,"name":"test2","niao":8,"niao_score":-96,"piao":0,"piao_score":-50,"score":-6,"seatid":2,"sex":2,"total_score":-152,"uid":101878}],"total_huxi":23,"tuns":6,"weaves":[{"card_list":[9,9,9],"hu_xi":1,"weave_kind":32},{"card_list":[6,6,6],"hu_xi":1,"weave_kind":32},{"card_list":[22,22,22],"hu_xi":6,"weave_kind":4},{"card_list":[17,17,17],"hu_xi":6,"weave_kind":80},{"card_list":[25,25,25],"hu_xi":6,"weave_kind":80},{"card_list":[2,7,10],"hu_xi":3,"weave_kind":8},{"card_list":[3,4,5],"hu_xi":0,"weave_kind":8}],"win_seatid":0,"zimo_addtun":0};

            this.val = val;

            if(!PHZModel.isOnVideo)
            {
                this.getUI("room").setString("房间:"+this.getPlay()+"\n房号:"+PHZModel.roomData["gameRoom"]["code"]+"\n时间:"+utils.getTime(utils.getTimeStamp()));
            }
            else
            {
                this.getUI("room").setString("房间:"+this.getPlay()+"\n时间:"+utils.getTime(utils.getTimeStamp()));
            }
            var list = this.getUI("weaves");
            for(var i in val["weaves"])
            {
                var item = new PHZResultItem1();
                item.show(val["weaves"][i]);
                list.addChild(item);
            }
            list = this.getUI("players");
            var winID= utils.getValInt(val,"win_seatid",-1);
            var winNum = 0;
            for(var i in val["players"])
            {
                var item = new PHZResultItem2();

                if(val.hasOwnProperty("dian_pao_user"))
                {
                    item.show(val["players"][i],val["dian_pao_user"]);
                }
                else
                {
                    item.show(val["players"][i],-1);
                }
                list.addChild(item);
                if(winID==val["players"][i]["seatid"])
                {
                    winNum = val["players"][i]["total_score"];
                }
            }

            list = this.getUI("fanList");
            var fhu =function(key,type)
            {
                if(val.hasOwnProperty(key+"_addfan")&&val[key+"_addfan"]>0)
                {
                    var item = new PHZResultItem3();

                    if(PHZModel.endScore<0) {
                        item.show(type, val[key + "_addfan"], 0);
                    }
                    else
                    {
                        item.show1(type, val[key + "_addfan"]);
                    }
                    list.addChild(item);
                }
                if(val.hasOwnProperty(key+"_addxi")&&val[key+"_addxi"]>0)
                {
                    var item = new PHZResultItem3();
                    item.show(type,val[key+"_addxi"],2);
                    list.addChild(item);
                }
                if(val.hasOwnProperty(key+"_addtun")&&val[key+"_addtun"]>0)
                {
                    var item = new PHZResultItem3();
                    item.show(type,val[key+"_addtun"],1);
                    list.addChild(item);
                }
                if(val.hasOwnProperty(key+"_setxi")&&val[key+"_setxi"]>0)
                {
                    var item = new PHZResultItem3();
                    item.show2(type,val[key+"_setxi"],1);
                    list.addChild(item);
                }
            };

            if(PHZModel.endScore<0)
            {
                this.showUI("jifen",true);
                this.showUI("huxi",true);
                this.showUI("dengshu",true);
                this.showUI("fanshu",true);
                utils.findNode( this.getUI("jifen"),"num").setString(winNum);
                this.getUI("huxi").setString("胡息："+val["hu_xis"]);
                this.getUI("dengshu").setString("等数："+val["tuns"]);
                this.getUI("fanshu").setString("番数："+val["fans"]);
            }
            else
            {
                this.showUI("huxi0",true);
                utils.findNode( this.getUI("huxi0"),"num").setString(winNum);

                fhu("dihunochange",PHZFan.MT_DI_HU);
                fhu("dianpao",PHZFan.MT_DI_HU);
            }
            fhu("zimo",PHZFan.MT_ZI_MO);
            if(val.hasOwnProperty("huang_count")&&val["huang_count"]>0)
            {
                var item = new PHZResultItem3();
                item.show(PHZFan.MT_HUANG_ZUANG,val["huang_count"],1);
                list.addChild(item);
            }
            fhu("tianhushow",PHZFan.MT_TIAN_HU);
            if(val.hasOwnProperty("tianhu3ti5kan_addfan")||val.hasOwnProperty("tianhu3ti5kan_addxi")||val.hasOwnProperty("tianhu3ti5kan_addtun"))
            {
                var item = new PHZResultItem3();
                item.show(PHZFan.MT_TIAN_HU_3TI5KAN_,0,0);
                list.addChild(item);
            }
            fhu("dihushow",PHZFan.MT_DI_HU_LIANG);
            fhu("dihuout",PHZFan.MT_DI_HU);
            if(val.hasOwnProperty("fan_info"))
            {
                list.setScrollBarEnabled(val["fan_info"].length >6);
                for(var i in val["fan_info"])
                {
                    var d =val["fan_info"][i];
                    var item = new PHZResultItem3();

                    if(PHZModel.endScore<0)
                    {
                        item.show(d["fan_type"],d["fan_num"],0);
                    }
                    else
                    {
                        if ( d["set_xi"] >0  )
                        {
                            item.show2(d["fan_type"], d["set_xi"]);
                        }
                        if(d["fan_num"] >0)
                        {
                            item.show1(d["fan_type"], d["fan_num"]);
                        }
                    }
                    list.addChild(item);
                }
            }
            else
            {
                list.setScrollBarEnabled(false);
            }

            for(var i in val["left_cards"])
            {
                var card = new PHZCard1();
                this.dipaiBg.addChild(card);
                card.setState(0);
                card.setVisible(true);
                card.setValue(val["left_cards"][i]);
                card.setAnchorPoint(cc.p(0.5,0.5));
                this.dipai.push(card);
            }
            utils.sortNodes(this.dipai,588,this.dipaiTxt.getPositionY(),-2);
        },
        getPlay:function()
        {
            switch (PHZTips.type)
            {
                case 1000001:
                case 1000002:
                    return "常德跑胡子";
                case 1000003:
                    return "长沙跑胡子";
                case 1000004:
                    return "邵阳字牌";
                case 1000005:
                    return "怀化红拐弯";
                case 1000006:
                case 10000010:
                    return "自定义跑胡子";
                case 1000007:
                    return "娄底放炮罚";
                case 1000008:
                    return "湘乡告胡子";
                case 1000009:
                    return "邵阳剥皮";
            }
            return "跑胡子";
        }

    }
);


var PHZResultBox = cc.Layer.extend(
    {
        ctor: function () {
            this._super();

            this.uiJson = ccs.load(PackName+"/res/resultBox.json");
            this.uiNode = this.uiJson.node;
            this.addChild(this.uiNode);

            utils.scaleAll(this.getUI("bg"));
            this.dipai = [];
            this.dipaiTxt = this.getUI("dipai");
            this.dipai.push( this.dipaiTxt);

            var self = this;
            this.getUI("sureBtn").addClickEventListener(function (sender, evt) {
                if(utils.getValInt(self.val,"end_type") == 1)
                {
                    var box = new PHZResult();
                    self.getParent().addChild(box,2000,666);
                    box.show(self.val);
                }
                else
                {
                    if(!PHZModel.isMyInTable())
                    {
                        PHZModel.sendUptable();
                    }
                    else if(PHZModel.getPlayerByPos(0) && PHZModel.getPlayerByPos(0).ready !=1)
                    {
                        PHZModel.sendReady();
                    }
                    if(PHZModel.isGameOver)
                    {
                        PHZModel.sendOver();
                    }
                }
                self.removeFromParent(true);
            });

            return true;
        },
        ignoreSize:function(labels,is)
        {
            for(var i in labels)
            {
                labels[i].ignoreContentAdaptWithSize(is);
            }

        },

        getLastCardsPos: function (pos, xl, yl) {
            var ret = [];

            if(pos == 2)
            {
                var start = cc.p(280, 620);
                for (var i = 0; i < xl; ++i) {
                    ret[i] = [];
                    var x = start.x + 38 * i;
                    for (var j = 0; j < yl; ++j) {
                        ret[i][j] = cc.p(x, start.y - 38 * j);
                    }
                }
            }
            else if(pos == 1)
            {
                var start = cc.p(1000, 620);
                for (var i = 0; i < xl; ++i) {
                    ret[i] = [];
                    var x = start.x - 38 * i;
                    for (var j = 0; j < yl; ++j) {
                        ret[i][j] = cc.p(x, start.y - 38 * j);
                    }
                }
            }
            return ret;
        },
        showLastCards: function (pos,val)
        {
            var poses = this.getLastCardsPos(pos, val.length, 4);
            for (var i in val) {
                var cards = val[i];

                for (var j in cards) {
                    var card = new PHZCard1();
                    this.addChild(card,i * 10 + 4 - j);
                    card.setPosition(poses[i][j]);
                    card.setVisible(true);
                    card.setValue(cards[j]);
                    card.setState(0);
                }
            }
        },
        getUI:function(key)
        {
            return utils.findNode(this.uiNode,key);
        },
        show:function(val)
        {
            //val ={"base_money":1,"cmd":4014,"end_type":1,"fans":1,"hu_xis":23,"left_cards":[26,2,21,20,5,24,8,10,10],"players":[{"avatar":"http://188.188.1.111:20084/chess/chess_img/head/101875/1611011850511260.png","hand_cards":[[17,17,17],[25,25,25],[2,7,10],[3,4,5]],"money":1000,"name":"事件处理4278siug","niao":8,"niao_score":192,"piao":50,"piao_score":100,"score":12,"seatid":0,"sex":1,"total_score":304,"uid":101875},{"avatar":"http://188.188.1.111:20084/chess/chess_img/head/101877/1609281402569206.png","hand_cards":[[1,1,17],[2,2]],"money":1000,"name":"阿萨德","niao":8,"niao_score":-96,"piao":0,"piao_score":-50,"score":-6,"seatid":1,"sex":2,"total_score":-152,"uid":101877},{"avatar":"http://188.188.1.111:20084/chess/chess_img/head/default/00.jpg","hand_cards":[[3,3,3],[1,1],[4,5,6],[8,9,10]],"money":1000,"name":"test2","niao":8,"niao_score":-96,"piao":0,"piao_score":-50,"score":-6,"seatid":2,"sex":2,"total_score":-152,"uid":101878}],"total_huxi":23,"tuns":6,"weaves":[{"card_list":[9,9,9],"hu_xi":1,"weave_kind":32},{"card_list":[6,6,6],"hu_xi":1,"weave_kind":32},{"card_list":[22,22,22],"hu_xi":6,"weave_kind":4},{"card_list":[17,17,17],"hu_xi":6,"weave_kind":80},{"card_list":[25,25,25],"hu_xi":6,"weave_kind":80},{"card_list":[2,7,10],"hu_xi":3,"weave_kind":8},{"card_list":[3,4,5],"hu_xi":0,"weave_kind":8}],"win_seatid":0,"zimo_addtun":0};

            this.val =val;

            for(var i in val["players"])
            {
                var d = val["players"][i];
                var seatID = utils.getValInt(d,"seatid");
                var pos = PHZModel.getPosBySeatID(seatID);

                if(pos ==0)
                {
                    continue;
                }
                if(d.hasOwnProperty("hand_cards"))
                {
                    this.showLastCards(pos,d["hand_cards"]);
                }
            }

            for(var i in val["left_cards"])
            {
                var card = new PHZCard1();
                this.uiNode.addChild(card);
                card.setState(0);
                card.setVisible(true);
                card.setValue(val["left_cards"][i]);
                card.setAnchorPoint(cc.p(0.5,0.5));
                this.dipai.push(card);
            }
            this.dipai.push( this.getUI("haidipai"));
            utils.sortNodes(this.dipai,this.uiNode.getContentSize().width*0.5,this.dipaiTxt.getPosition().y,0);
        }

    }
);