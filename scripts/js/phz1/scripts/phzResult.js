
var PHZResultItemCard = ccui.Layout.extend(
    {
        ctor:function ()
        {
            this._super();
            var json = ccs.load(PackName+"/res/resultItem2.json");
            this.uiNode =json.node;
            this.addChild(this.uiNode);
            this.setContentSize(this.uiNode.getContentSize());
            this.setAnchorPoint(cc.p(0.5,0));
            return true;
        },
        getUI:function(key)
        {
            return  utils.findNode(this.uiNode,key);
        },
        show:function(card)
        {
            this.getUI("face0").loadTexture("phz/card_s_"+utils.toHex2(card)+".png",1);
            this.getUI("face1").loadTexture("phz/card_s_"+utils.toHex2(card)+".png",1);
        }
    });

var PHZResultItem1 = ccui.Layout.extend(
    {
        ctor:function ()
        {
            this._super();
            var json = ccs.load(PackName+"/res/resultItem1.json");
            this.uiNode =json.node;
            this.addChild(this.uiNode);
            this.setContentSize(this.uiNode.getContentSize());
            this.setAnchorPoint(cc.p(0.5,0.5));
            for(var i =0;i<3;++i)
            {
                this.getUI("card"+i).setVisible(false);
            }
            return true;
        },
        getUI:function(key)
        {
            return  utils.findNode(this.uiNode,key);
        },
        show:function(cards)
        {
            for(var i in cards)
            {
                this.getUI("card"+i).setVisible(true);
                if(cards[i]!=0)
                {
                    this.getUI("face"+i).loadTexture("phz/card_s_"+utils.toHex2(cards[i])+".png",1);
                }
            }
        }
    });


var PHZResultItem2 = ccui.Layout.extend(
    {
        ctor:function ()
        {
            this._super();
            this.kind = new cc.Sprite(utils.spriteFrame("phz/txt_chi.png"));
            this.addChild(this.kind,2);
            return true;
        },
        getUI:function(key)
        {
            return  utils.findNode(this.uiNode,key);
        },
        show:function(val)
        {
           var cards = [];
            for(var i in val["card_list"])
            {
                if(val["card_list"][i]!=0)
                {
                    var card = new PHZResultItemCard();
                    card.show(val["card_list"][i]);
                    card.setAnchorPoint(cc.p(0.5,0));
                    this.addChild(card);
                    cards.push(card);
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
            }
            this.kind.setSpriteFrame(utils.spriteFrame(file));

            var size = cards[0].getContentSize();
            size.width*=cards[0].getScaleX();
            size.height*=cards[0].getScaleY();
            this.kind.setPosition(cards.length*size.width*0.5+10,100);
            utils.sortNodes(cards,cards.length*size.width*0.5+10,0,0);

            this.setContentSize(cc.size(cards.length*size.width+10,110));
        }
    });

var PHZResultItem3 = ccui.Layout.extend(
    {
        ctor:function ()
        {
            this._super();
            var json = ccs.load(PackName+"/res/resultItem3.json");
            this.uiNode =json.node;
            this.addChild(this.uiNode);
            this.setContentSize(this.uiNode.getContentSize());
            this.setAnchorPoint(cc.p(0.5,0));
            return true;
        },
        getUI:function(key)
        {
            return  utils.findNode(this.uiNode,key);
        },
        show:function(card)
        {
            this.getUI("face0").loadTexture("phz/card_s_"+utils.toHex2(card)+".png",1);
            this.getUI("face1").loadTexture("phz/card_s_"+utils.toHex2(card)+".png",1);
        }
    });


var PHZResultItem = ccui.Layout.extend(
 {
        ctor:function ()
        {
            this._super();
            var json = ccs.load(PackName+"/res/resultItem.json");
            this.uiNode =json.node;
            this.value =INVAIL_CARD;
            this.addChild(this.uiNode);
            this.setContentSize(this.uiNode.getContentSize());
            this.setAnchorPoint(cc.p(0.5,0.5));
            this.getUI("name").ignoreContentAdaptWithSize(true);
            this.getUI("coin").ignoreContentAdaptWithSize(true);
            this.getUI("hu").ignoreContentAdaptWithSize(true);
            this.getUI("lian").getChildByName("num").ignoreContentAdaptWithSize(true);

            this.getUI("cards").setScrollBarEnabled(false);
            this.getUI("win").setVisible(false);
            this.getUI("fail").setVisible(false);
            this.getUI("zhuan").setVisible(false);
            this.getUI("niao").setVisible(false);
            this.getUI("piao").setVisible(false);
            utils.findNode( this.getUI("niao"),"num").ignoreContentAdaptWithSize(true);
            utils.findNode( this.getUI("piao"),"num").ignoreContentAdaptWithSize(true);
            return true;
        },
        getUI:function(key)
        {
            return  utils.findNode(this.uiNode,key);
        },
     show:function(val,winCard,huType)
    {

        this.getUI("fz").setVisible(utils.getValInt(val,"uid")==PHZModel.fzUID);
        utils.showAvatarIcon(this.getUI("icon"),utils.getValStr(val,"avatar"),utils.getValStr(val,"sex"));
        this.getUI("name").setString(utils.parseName(6,utils.getValStr(val,"name")));
        var score = utils.getValInt(val,"total_score");
        if (score<0)
        {
            this.getUI("hu").setString(":"+Math.abs(score));
        }
        else
        {
            this.getUI("hu").setString(score);
        }
        this.getUI("coin").setString("总"+(utils.getValInt(val,"money")-PHZModel.roomGolden));

        var list = this.getUI("cards");
        if(val.hasOwnProperty("hand_cards")) {
            for (var i in val["hand_cards"]) {
                var ds = [];
                var d = val["hand_cards"][i];
                var len = d.length;
                if (len >= 4) {
                    for (var j = 0; j < len; j += 2) {
                        ds.push(d.slice(j, j + 2));
                    }
                }
                else {
                    ds.push(d);
                }
                for (var j in ds) {
                    var item = new PHZResultItem1();
                    item.show(ds[j]);
                    list.addChild(item);
                }
            }
        }
        if(val.hasOwnProperty("weaves"))
        {
            for(var i in val["weaves"])
            {
                var item = new PHZResultItem2();
                item.show(val["weaves"][i]);
                list.addChild(item);
            }

        }
        if(winCard != INVAIL_CARD)
        {
            var item = new PHZResultItem3();
            item.show(winCard);
            list.addChild(item);
        }
        if(PHZModel.isDZSeatID(utils.getValInt(val,"seatid"))&&PHZTips.zhuangNum>0)
        {
            this.getUI("zhuan").setVisible(true);
            this.getUI("lian1").setVisible(PHZTips.zhongZhuang);
            this.getUI("lian").setVisible(PHZTips.lianZhuang);
            utils.findNode(this.getUI("lian"),"num").setString(PHZTips.zhuangNum+1);

        }
        if(huType<=0)
        {
        }
        else  if(huType<=12)
        {
            this.getUI("win").setVisible(true);

            utils.findNode(this.getUI("win"),"type").setSpriteFrame(utils.spriteFrame("phz/hu_type_"+huType+".png"));
        }
        else  if(huType>=20)
        {
            this.getUI("fail").setVisible(true);

            this.getUI("fail").setSpriteFrame(utils.spriteFrame("phz/hu_type_"+huType+".png"));

        }

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
            this.dipai = this.getUI("dipai");
            this.dipai.setVisible(false);
            var self = this;
            this.getUI("lookBtn").addClickEventListener(function (sender, evt) {

                self.dipai.setVisible(true);
                self.getUI("lookBtn").setVisible(false);
                self.showDiPai();
            });
            this.getUI("againBtn").addClickEventListener(function (sender, evt) {

                self.removeFromParent(true);
                if(PHZModel.isGameOver)
                {
                    PHZModel.sendOver();
                }
                else{
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

            this.getUI("players").setScrollBarEnabled(false);
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
            this.val = val;

            if(!PHZModel.isOnVideo) {
                this.getUI("room").setString("房间:碰胡" + "\n房号:" + PHZModel.roomData["gameRoom"]["code"] + "\n时间:" + utils.getTime(utils.getTimeStamp()));
            }
            else
            {
                this.getUI("room").setString("房间:碰胡" + "\n时间:" + utils.getTime(utils.getTimeStamp()));
            }
            var winID= -1;
            if(val.hasOwnProperty("win_seatid"))
            {
                winID = utils.getValInt(val,"win_seatid");
            }
            var huType = utils.getValInt(val,"hu_type");
            var huCard = utils.getValInt(val,"hu_card");
            if(val["end_type"] ==0)
            {
                huCard =INVAIL_CARD;
                huType = 20;
                winID = PHZModel.dzSeatID;
            }
            var paoID = -1;
            if(val.hasOwnProperty("dianpao_user"))
            {
                paoID = utils.getValInt(val,"dianpao_user");
            }
            var list = this.getUI("players");
            for(var i in val["players"])
            {
                var d = val["players"][i];
                var item = new PHZResultItem();

                if(d["seatid"] == winID)
                {
                    item.show(d,huCard,huType);
                }
                else  if(d["seatid"] == paoID)
                {
                    item.show(d,INVAIL_CARD,21);
                }
                else
                {
                    item.show(d,INVAIL_CARD,0);
                }
                list.addChild(item);
            }
        },
        getLastCardsPos: function ( xl) {
            var ret = [];

            var start = cc.p(20, 90);
            for (var i = 0; i < xl; ++i) {

                var di = i%9;
                var dj = Math.floor(i/9);
                var x = start.x + 40 * di;
                ret[i] = cc.p(x, start.y - 38 * dj);
            }
            return ret;
        },
        showDiPai:function()
        {
            if(!this.val.hasOwnProperty("left_cards"))
            {
                return;
            }
            var poses = this.getLastCardsPos(this.val["left_cards"].length+1);
            for(var i in  this.val["left_cards"])
            {
                var card = new PHZCard1();
                this.dipai.addChild(card);
                card.setState(0);
                card.setVisible(true);
                card.setValue( this.val["left_cards"][i]);
                card.setPosition(poses[Number(i)+1]);
            }
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

                var box = new PHZResult();
                self.getParent().addChild(box,2000,666);
                box.show(self.val);
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

            if(pos == 3)
            {
                var start = cc.p(180, 500);
                for (var i = 0; i < xl; ++i) {
                    ret[i] = [];
                    var x = start.x + 38 * i;
                    for (var j = 0; j < yl; ++j) {
                        ret[i][j] = cc.p(x, start.y - 38 * j);
                    }
                }
            }
            if(pos == 2)
            {
                var start = cc.p(460, 620);
                for (var i = 0; i < xl; ++i) {
                    ret[i] = [];
                    var x = start.x - 38 * i;
                    for (var j = 0; j < yl; ++j) {
                        ret[i][j] = cc.p(x, start.y - 38 * j);
                    }
                }
            }
            else if(pos == 1)
            {
                var start = cc.p(1060, 500);
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

           /* val ={
             "banker" : 3,
             "base_money" : 1,
             "cmd" : 4014,
             "end_type" : 1,
             "hu_card" : 7,
             "hu_type" : 1,
             "left_cards" : [ 21, 25, 24, 1, 26, 22, 23, 9, 9, 10, 17, 7, 20, 5, 23, 19, 5, 24 ],
             "lian_zhuang_count" : 0,
             "players" : [
             {
             "avatar" : "http://188.188.1.202:20084/chess/chess_img/head/default/00.jpg",
             "hand_cards" : [
             [ 2, 7, 10 ],
             [ 3, 4 ]
             ],
             "money" : 99980.0,
             "name" : "wb17000000015",
             "niao" : -1,
             "niao_score" : 0,
             "piao" : -1,
             "piao_score" : 0,
             "score" : -5,
             "seatid" : 0,
             "sex" : 2,
             "total_score" : -5,
             "uid" : 18357,
             "weaves" : [
             {
             "card_count" : 3,
             "card_list" : [ 17, 18, 19 ],
             "center_card" : 18,
             "weave_kind" : 8
             },
             {
             "card_count" : 3,
             "card_list" : [ 18, 23, 26 ],
             "center_card" : 18,
             "weave_kind" : 8
             },
             {
             "card_count" : 3,
             "card_list" : [ 22, 22, 22 ],
             "center_card" : 22,
             "weave_kind" : 4
             }
             ]
             },
             {
             "avatar" : "http://188.188.1.202:20084/chess/chess_img/head/default/00.jpg",
             "hand_cards" : [
             [ 2, 7, 10 ],
             [ 17, 18, 19 ],
             [ 4, 5 ],
             [ 8, 9 ],
             [ 2 ]
             ],
             "money" : 99920.0,
             "name" : "wb17000000012",
             "niao" : -1,
             "niao_score" : 0,
             "piao" : -1,
             "piao_score" : 0,
             "score" : -20,
             "seatid" : 1,
             "sex" : 2,
             "total_score" : -20,
             "uid" : 18354,
             "weaves" : [
             {
             "card_count" : 3,
             "card_list" : [ 20, 20, 20 ],
             "center_card" : 20,
             "weave_kind" : 4
             }
             ]
             },
             {
             "avatar" : "http://188.188.1.202:20084/chess/chess_img/head/18353/1612041402586896.png",
             "hand_cards" : [
             [ 26, 26, 10 ],
             [ 17, 18, 19 ],
             [ 5, 6 ]
             ],
             "money" : 100044.0,
             "name" : "v1▲",
             "niao" : -1,
             "niao_score" : 0,
             "piao" : -1,
             "piao_score" : 0,
             "score" : 14,
             "seatid" : 2,
             "sex" : 1,
             "total_score" : 14,
             "uid" : 18353,
             "weaves" : [
             {
             "card_count" : 3,
             "card_list" : [ 1, 1, 1 ],
             "center_card" : 1,
             "weave_kind" : 4
             },
             {
             "card_count" : 3,
             "card_list" : [ 21, 21, 21 ],
             "center_card" : 21,
             "weave_kind" : 32
             }
             ]
             },
             {
             "avatar" : "http://188.188.1.202:20084/chess/chess_img/head/default/00.jpg",
             "hand_cards" : [
             [ 4, 4 ],
             [ 24, 24, 8 ],
             [ 25, 25, 9 ]
             ],
             "money" : 100060.0,
             "name" : "战5渣(ง •̀_•́)ง",
             "niao" : -1,
             "niao_score" : 0,
             "piao" : -1,
             "piao_score" : 0,
             "score" : 15,
             "seatid" : 3,
             "sex" : 2,
             "total_score" : 15,
             "uid" : 18352,
             "weaves" : [
             {
             "card_count" : 3,
             "card_list" : [ 6, 6, 6 ],
             "center_card" : 6,
             "weave_kind" : 4
             },
             {
             "card_count" : 3,
             "card_list" : [ 3, 3, 3 ],
             "center_card" : 3,
             "weave_kind" : 32
             }
             ]
             }
             ],
             "weaves" : [
             {
             "card_list" : [ 5, 6, 7 ],
             "hu_xi" : 0,
             "weave_kind" : 8
             },
             {
             "card_list" : [ 17, 18, 19 ],
             "hu_xi" : 6,
             "weave_kind" : 8
             },
             {
             "card_list" : [ 26, 26, 10 ],
             "hu_xi" : 0,
             "weave_kind" : 8
             }
             ],
             "win_seatid" : 2
             };*/
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