var PHZBigfaceBox = ccui.Layout.extend(
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
            this.uiNode[2] = ccs.load("public/res/bigfaceLL.json").node;
            this.addChild(this.uiNode[2] );
            this.uiNode[2].setPosition(385,560);

            this.uiNode[1] = ccs.load("public/res/bigfaceLR.json").node;
            this.addChild(this.uiNode[1] );
            this.uiNode[1].setPosition(1220,460);

            this.uiNode[3] = ccs.load("public/res/bigfaceLL.json").node;
            this.addChild(this.uiNode[3] );
            this.uiNode[3].setPosition(60,460);


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
                        PHZPlayers.publicBox.showPlayerList();
                    }
                    else
                    {
                        var id = Number(key.substr(3,1));
                        if( utils.getTimeStamp() - PHZTips.bigfaceTime >2000)
                        {
                            PHZModel.sendEmotion(self.pos,id);
                            PHZTips.bigfaceTime = utils.getTimeStamp();
                        }
                        var b = PHZPlayers.getPosition(0);
                        var t  =PHZPlayers.getPosition(self.pos);
                        PHZTips.showBigface(b,t,id,PHZPlayers.getSex(0));
                    }
                    self.removeFromParent(true);
                });
            }
        },
        showPos:function(pos)
        {
            if(!this.uiNode[pos])
            {
                return;
            }
            var bg = utils.findNode(this.uiNode[pos],"bg");

            this.pos = pos;
            bg.setVisible(true);

            var dt =0;
            for(var i=0;i<7;++i)
            {
                var btn ;
                if(i ==0)
                {
                    btn =utils.findNode(bg,"btn7");
                }
                else
                {
                    btn =utils.findNode(bg,"btn"+i);
                }
                btn.stopAllActions();
                btn.setScale(0);
                btn.runAction(cc.sequence(cc.delayTime(dt),cc.scaleTo(0.05,1.)));
                dt+=0.03;
            }
        }
    }
);
var PHZChiBox = cc.Layer.extend(
    {
        uiJson: null,
        uiNode: null,
        ctor: function () {
            this._super();
            this.uiJson = ccs.load(PackName+"/res/boxChi.json");
            this.uiNode = this.uiJson.node;
            this.addChild(this.uiNode);


            this.kind0 = null;
            this.kind1 = null;
            this.kind2 = null;
            this.bg0 = this.getUI("bg0");
            this.bg1 = this.getUI("bg1");
            this.bg2 = this.getUI("bg2");
            var self = this;
            this.getUI("returnBtn").addClickEventListener(function (sender, evt) {
                self.setVisible(false);
            });
            return true;
        },
        getUI:function(key)
        {
            return utils.findNode(this.uiNode,key);
        },

        showCards:function(val)
        {
        /*  val =  [
         {
         "cards" : [ 18, 18, 2 ],
         "center_card" : 18,
         "chi_kind" : 2,
         "second_info" : [
         {
         "cards" : [ 18, 23, 26 ],
         "center_card" : 18,
         "chi_kind" : 4
         },
         {
         "cards" : [ 18, 19, 20 ],
         "center_card" : 18,
         "chi_kind" : 16
         },
         {
         "cards" : [ 17, 18, 19 ],
         "center_card" : 18,
         "chi_kind" : 32
         }
         ]
         },
         {
         "cards" : [ 18, 2, 2 ],
         "center_card" : 18,
         "chi_kind" : 1,
         "second_info" : [
         {
         "cards" : [ 18, 23, 26 ],
         "center_card" : 18,
         "chi_kind" : 4,
         "third_info" : [
         {
         "cards" : [ 18, 19, 20 ],
         "center_card" : 18,
         "chi_kind" : 16
         },
         {
         "cards" : [ 17, 18, 19 ],
         "center_card" : 18,
         "chi_kind" : 32
         }
         ]
         },
         {
         "cards" : [ 18, 19, 20 ],
         "center_card" : 18,
         "chi_kind" : 16,
         "third_info" : [
         {
         "cards" : [ 18, 23, 26 ],
         "center_card" : 18,
         "chi_kind" : 4
         }
         ]
         },
         {
         "cards" : [ 17, 18, 19 ],
         "center_card" : 18,
         "chi_kind" : 32,
         "third_info" : [
         {
         "cards" : [ 18, 23, 26 ],
         "center_card" : 18,
         "chi_kind" : 4
         }
         ]
         }
         ]
         },
         {
         "cards" : [ 18, 23, 26 ],
         "center_card" : 18,
         "chi_kind" : 4,
         "second_info" : [
         {
         "cards" : [ 18, 18, 2 ],
         "center_card" : 18,
         "chi_kind" : 2
         },
         {
         "cards" : [ 18, 2, 2 ],
         "center_card" : 18,
         "chi_kind" : 1,
         "third_info" : [
         {
         "cards" : [ 18, 19, 20 ],
         "center_card" : 18,
         "chi_kind" : 16
         },
         {
         "cards" : [ 17, 18, 19 ],
         "center_card" : 18,
         "chi_kind" : 32
         }
         ]
         },
         {
         "cards" : [ 18, 19, 20 ],
         "center_card" : 18,
         "chi_kind" : 16,
         "third_info" : [
         {
         "cards" : [ 18, 2, 2 ],
         "center_card" : 18,
         "chi_kind" : 1
         }
         ]
         },
         {
         "cards" : [ 17, 18, 19 ],
         "center_card" : 18,
         "chi_kind" : 32,
         "third_info" : [
         {
         "cards" : [ 18, 2, 2 ],
         "center_card" : 18,
         "chi_kind" : 1
         }
         ]
         }
         ]
         },
         {
         "cards" : [ 18, 19, 20 ],
         "center_card" : 18,
         "chi_kind" : 16,
         "second_info" : [
         {
         "cards" : [ 18, 18, 2 ],
         "center_card" : 18,
         "chi_kind" : 2
         },
         {
         "cards" : [ 18, 2, 2 ],
         "center_card" : 18,
         "chi_kind" : 1,
         "third_info" : [
         {
         "cards" : [ 18, 23, 26 ],
         "center_card" : 18,
         "chi_kind" : 4
         }
         ]
         },
         {
         "cards" : [ 18, 23, 26 ],
         "center_card" : 18,
         "chi_kind" : 4,
         "third_info" : [
         {
         "cards" : [ 18, 2, 2 ],
         "center_card" : 18,
         "chi_kind" : 1
         }
         ]
         }
         ]
         },
         {
         "cards" : [ 17, 18, 19 ],
         "center_card" : 18,
         "chi_kind" : 32,
         "second_info" : [
         {
         "cards" : [ 18, 18, 2 ],
         "center_card" : 18,
         "chi_kind" : 2
         },
         {
         "cards" : [ 18, 2, 2 ],
         "center_card" : 18,
         "chi_kind" : 1,
         "third_info" : [
         {
         "cards" : [ 18, 23, 26 ],
         "center_card" : 18,
         "chi_kind" : 4
         }
         ]
         },
         {
         "cards" : [ 18, 23, 26 ],
         "center_card" : 18,
         "chi_kind" : 4,
         "third_info" : [
         {
         "cards" : [ 18, 2, 2 ],
         "center_card" : 18,
         "chi_kind" : 1
         }
         ]
         }
         ]
         }
         ];
*/
            // val =[{"cards":[3,3,19],"center_card":3,"chi_kind":1},{"cards":[3,19,19],"center_card":3,"chi_kind":2,"second_info":[{"cards":[1,2,3],"center_card":3,"chi_kind":64}]},{"cards":[1,2,3],"center_card":3,"chi_kind":64,"second_info":[{"cards":[3,19,19],"center_card":3,"chi_kind":2,"third_info":[{"cards":[3,6,19],"center_card":3,"chi_kind":2},{"cards":[3,5,19],"center_card":3,"chi_kind":2},{"cards":[3,8,19],"center_card":3,"chi_kind":2}]},{"cards":[3,7,2],"center_card":3,"chi_kind":2},{"cards":[3,8,19],"center_card":3,"chi_kind":2,"third_info":[{"cards":[3,6,19],"center_card":3,"chi_kind":2},{"cards":[3,5,19],"center_card":3,"chi_kind":2},{"cards":[3,8,9],"center_card":3,"chi_kind":2}]}]}]
            this.bg1.setVisible(false);
            this.bg2.setVisible(false);
            this.bg0.removeAllChildren();
            this.cards0 =[];
            this.val0 = val;
            for(var i in val)
            {
                var d = PHZCards.newChiCards(val[i]["chi_kind"],val[i]["cards"]);
                d.setVisible(true);
                d.val = val[i];
                d.setScale(0.6);
                this.bg0.addChild(d);
                d.clickCBTarget=this;
                d.clickCB = this.onClick0;
                this.cards0.push(d);
            }
            utils.sortNodes(this.cards0,this.bg0.getContentSize().width*0.5,this.bg0.getContentSize().height*0.5,100);
        },
        showBi1:function(val)
        {
            this.bg1.removeAllChildren();
            this.bg1.setVisible(true);
            this.cards1 =[];
            this.val1 = val;
            for(var i in val)
            {
                var d = PHZCards.newChiCards(val[i]["chi_kind"],val[i]["cards"]);
                d.setVisible(true);
                d.setScale(0.5);
                d.val = val[i];
                this.bg1.addChild(d);
                d.clickCBTarget=this;
                d.clickCB = this.onClick1;
                this.cards1.push(d);
            }
            utils.sortNodes(this.cards1,this.bg1.getContentSize().width*0.5,this.bg1.getContentSize().height*0.5,100);
        },
        showBi2:function(val)
        {
            this.bg2.removeAllChildren();
            this.bg2.setVisible(true);
            this.cards2 =[];
            this.val2 = val;
            for(var i in val)
            {
                var d = PHZCards.newChiCards(val[i]["chi_kind"],val[i]["cards"]);
                d.setVisible(true);
                d.setScale(0.5);
                d.val = val[i];
                this.bg2.addChild(d);
                d.clickCBTarget=this;
                d.clickCB = this.onClick2;
                this.cards2.push(d);
            }
            utils.sortNodes(this.cards2,this.bg2.getContentSize().width*0.5,this.bg2.getContentSize().height*0.5,100);
        },
        onClick0:function(self,kind)
        {
            for(var i in self.cards0) {
                self.cards0[i].setClick(false);
            }
            this.setClick(true);
            self.kind0 =  kind;
            if(this.val.hasOwnProperty("second_info"))
            {
                self.bg2.setVisible(false);
                self.showBi1(this.val["second_info"]);
            }
            else
            {

                if(PHZModel.ackAble)
                {
                    PHZModel.sendAck(PHZAck.ACK_CHI, kind);
                }
                else
                {
                    PHZModel.sendPreAck(PHZAck.ACK_CHI, kind);
                }
            }
        },
        onClick1:function(self,kind)
        {
            for(var i in self.cards1) {
                self.cards1[i].setClick(false);
            }
            this.setClick(true);
            self.kind1 =  kind;
            if(this.val.hasOwnProperty("third_info"))
            {
                self.showBi2(this.val["third_info"]);
            }
            else
            {
                if(PHZModel.ackAble)
                {
                    PHZModel.sendAck(PHZAck.ACK_CHI, self.kind0,kind);
                }
                else
                {
                    PHZModel.sendPreAck(PHZAck.ACK_CHI, self.kind0,kind);
                }
            }
        },
        onClick2:function(self,kind)
        {
            for(var i in self.cards2) {
                self.cards2[i].setClick(false);
            }
            this.setClick(true);
            self.kind2 = kind;
            if(PHZModel.ackAble)
            {
                PHZModel.sendAck(PHZAck.ACK_CHI, self.kind0,self.kind1,kind);
            }
            else
            {
                PHZModel.sendPreAck(PHZAck.ACK_CHI, self.kind0,self.kind1,kind);
            }
        },
        reSend:function()
        {
            PHZModel.sendAck(PHZAck.ACK_CHI, this.kind0,this.kind1,this.kind2);
        },
        show:function(is)
        {
            this.setVisible(is);
            if(!is)
            {
                this.bg1.setVisible(false);
                this.bg2.setVisible(false);
                this.bg0.removeAllChildren();
                this.cards0 =[];
                this.cards1 =[];
                this.cards2 =[];
                this.kind0 = null;
                this.kind1 = null;
                this.kind2 = null;
            }
        }
    }
);


var PHZTipBox = cc.Layer.extend(
    {
        uiJson: null,
        uiNode: null,
        ctor: function () {
            this._super();
            this.uiJson = ccs.load(PackName+"/res/jbBox.json");
            this.uiNode = this.uiJson.node;
            this.addChild(this.uiNode);

            utils.scaleAll(this.getUI("bg"));
            this.card=null;
            var self = this;
            this.getUI("returnBtn").addClickEventListener(function (sender, evt) {
                PHZModel.sendOutCard(self.card.value, self.card.row, self.card.col);
                PHZCards.preOutMyCard(self.card);
                self.removeFromParent();
            });
            this.getUI("cancelBtn").addClickEventListener(function (sender, evt) {
                PHZModel.sendOutCard(self.card.value, self.card.row, self.card.col);
                PHZCards.preOutMyCard(self.card);
                self.removeFromParent();
            });
            this.getUI("sureBtn").addClickEventListener(function (sender, evt) {
                PHZModel.sendOutCard(self.card.value, self.card.row, self.card.col,true);
                PHZCards.preOutMyCard(self.card);
                self.removeFromParent();
            }.bind(this));
            return true;
        },
        getUI:function(key)
        {
            return utils.findNode(this.uiNode,key);
        }

    }
);
