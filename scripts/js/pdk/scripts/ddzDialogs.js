var DDZBigfaceBox = ccui.Layout.extend(
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
            this.uiNode[2] = ccs.load("public/res/bigfaceCL.json").node;
            this.addChild(this.uiNode[2] );
            this.uiNode[2].setPosition(40,520);

            this.uiNode[1] = ccs.load("public/res/bigfaceCR.json").node;
            this.addChild(this.uiNode[1] );
            this.uiNode[1].setPosition(1240,520);


            for(var i =1;i<3;++i)
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
                        DDZPlayers.publicBox.showPlayerList();
                    }
                    else
                    {
                        var id = Number(key.substr(3,1));
                        if( utils.getTimeStamp() - DDZTips.bigfaceTime >2000)
                        {
                            DDZModel.sendEmotion(self.pos,id);
                            DDZTips.bigfaceTime = utils.getTimeStamp();
                        }
                        var b = DDZPlayers.getPosition(0);
                        var t  =DDZPlayers.getPosition(self.pos);
                        DDZTips.showBigface(b,t,id,DDZPlayers.getSex(0));
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
            bg.setScale(0.1);
            bg.stopAllActions();
            bg.runAction(cc.scaleTo(0.3,1));
        }
    }
);