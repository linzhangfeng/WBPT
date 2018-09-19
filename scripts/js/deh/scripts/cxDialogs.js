var CXBigfaceBox = ccui.Layout.extend(
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
                        CXPlayers.publicBox.showPlayerList();
                    }
                    else
                    {
                       var id = Number(key.substr(3,1));
                        if( utils.getTimeStamp() - CXTips.bigfaceTime >2000)
                        {
                            CXModel.sendEmotion(self.pos,id);
                            CXTips.bigfaceTime = utils.getTimeStamp();
                        }
                        var b = CXPlayers.getPosition(0);
                        var t  =CXPlayers.getPosition(self.pos);
                        CXTips.showBigface(b,t,id,CXPlayers.getSex(0),self.pos);
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
            }else if(pos == 3 )
            {
                spos = 2;
            }else if(pos == 4 || pos == 5 )
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

var CXJiaZhuView = cc.Layer.extend(
    {
        uiJson: null,
        uiNode: null,

        slider:null,
        curSelMoney:null,

        mixBobo:0,
        maxBobo:0,
        minmaxdis:0,
        curMoney:0,

        callBackAction:null,

        ctor: function ()
        {
            this._super();
            this.uiJson = ccs.load(PackName+"/res/filling.json");
            this.uiNode = this.uiJson.node;
            this.addChild(this.uiNode);

            this.slider = this.getUI("slider");
            this.curSelMoney = this.getUI("num_bg").getChildByName("num");
            this.curSelMoney.ignoreContentAdaptWithSize(true);
            this.slider.addCCSEventListener(this.sliderStateChange.bind(this));

            var self = this;
            this.getUI("close").addClickEventListener(function (sender, evt) {
                if(self.callBackAction != null)
                {
                    self.callBackAction(0,self.curMoney);
                }
            });
            this.getUI("surebtns").addClickEventListener(function (sender, evt) {
                if(self.callBackAction != null)
                {
                    self.callBackAction(1,self.curMoney);
                }
            });


            this.getUI("reduce").addClickEventListener(function (sender, evt) {
                var curmoney = parseInt(self.curSelMoney.getString());
                if(curmoney > self.mixBobo)
                {
                    curmoney = curmoney - 1;
                }
                else
                {
                    curmoney = self.mixBobo;
                }
                self.curSelMoney.setString(curmoney);
                self.curMoney = curmoney;

                var per = Math.ceil((curmoney - self.mixBobo)*100/self.minmaxdis);
                self.slider.setPercent(per);
            });

            this.getUI("plus").addClickEventListener(function (sender, evt) {
               var curmoney = parseInt(self.curSelMoney.getString());
                if(curmoney < self.maxBobo)
                {
                    curmoney = curmoney + 1;
                }
                else
                {
                    curmoney = self.maxBobo;
                }
                self.curSelMoney.setString(curmoney);
                self.curMoney = curmoney;

                var per = Math.ceil((curmoney - self.mixBobo)*100/self.minmaxdis);
                self.slider.setPercent(per);
            });

            return true;
        },

        sliderStateChange:function(sender,type)
        {
            switch(type){
                case ccui.Slider.EVENT_PERCENT_CHANGED:
                {
                    cc.log("musicSlider percent : " + sender.getPercent());
                    var curPer = sender.getPercent();
                    this.curMoney = Math.ceil(this.mixBobo + (this.minmaxdis*curPer)/100);
                    this.curSelMoney.setString(this.curMoney);
                }
                break;
            }
        },

        refreshView:function()
        {
            var player = CXModel.players[CXModel.seatID];
            this.mixBobo = CXModel.cur_bet + CXModel.base_money;  //下注额 ＋ 皮
            this.maxBobo = player.bobo_money + player.bet;

            this.minmaxdis = this.maxBobo - this.mixBobo;

            var curPer = this.slider.getPercent();
            this.curMoney = Math.ceil(this.mixBobo + (this.minmaxdis*curPer)/100);
            this.curSelMoney.setString(this.curMoney);

            if(this.minmaxdis == 0)
            {
                this.slider.setPercent(100);
            }

        },

        getUI:function(key)
        {
            return this.uiNode.getChildByName(key);
        },
    }
);

var CXBoboView = cc.Layer.extend(
    {
        uiJson: null,
        uiNode: null,

        my_money:null,
        min_bo:null,
        max_bo:null,
        slider:null,

        curselMoney:null,

        max:0,
        mix:0,
        alllength:0,

        curMoney:0,  //当前选择的钱

        ctor: function ()
        {
            this._super();
            this.uiJson = ccs.load(PackName+"/res/bobo.json");
            this.uiNode = this.uiJson.node;
            this.addChild(this.uiNode);

            this.my_money = this.getUI("my_money");
            this.my_money .ignoreContentAdaptWithSize(true);
            this.min_bo = this.getUI("min_money");
            this.min_bo .ignoreContentAdaptWithSize(true);
            this.max_bo = this.getUI("max_money");
            this.max_bo .ignoreContentAdaptWithSize(true);
            this.curselMoney = this.getUI("Atlasmoney");
            this.curselMoney .ignoreContentAdaptWithSize(true);

            this.slider = this.getUI("slier");
            this.slider.addCCSEventListener(this.sliderStateChange.bind(this));

            var self = this;

            this.getUI("btn_sure").addClickEventListener(function (sender, evt) {
                CXModel.addBobo(self.curMoney);
            });

            return true;
        },

        sliderStateChange:function(sender,type)
        {
            switch(type){
                case ccui.Slider.EVENT_PERCENT_CHANGED:
                {
                    cc.log("musicSlider percent : " + sender.getPercent());
                    if(this.mix == this.max)
                    {
                        this.slider.setPercent(100);
                    }else{
                         var curPer = sender.getPercent();
                        this.curMoney = Math.ceil(this.mix + (this.alllength*curPer)/100);
                        this.setCurMoney(this.curMoney);
                    }
                   
                }
                break;
            }
        },

        setCurMoney:function(money)
        {
             this.curselMoney.setString(money);
        },

        getUI:function(key)
        {
            return this.uiNode.getChildByName(key);
        },

        refreshView:function()
        {
            if(CXModel.seatID == -1)
            {
                return;
            }
            var player = CXModel.players[CXModel.seatID];

            var myMoney = parseInt(player.money)  + parseInt(player.bobo_money);
            this.my_money.setString(myMoney);   //身上的钱加簸簸数

            // this.mix = CXModel.base_money*10;

            // if(this.mix > myMoney)
            // {
            //     this.mix = myMoney;
            //     this.max = myMoney;
            // }
            // else
            // {
            //     this.max = myMoney;
            // }

            this.mix = CXModel.min_bobo_money; 
            if(player.bobo_money >=  this.mix)
            {
                this.mix = player.bobo_money;
                this.max = myMoney;
            }
            else
            {
                this.max = myMoney;
            }
            
            this.min_bo.setString(""+this.mix);
            this.max_bo.setString(""+this.max);

            this.alllength = this.max - this.mix;

            if(this.max >  this.mix)
            {
                var curPer = this.slider.getPercent();
                this.curMoney = Math.ceil(this.mix + (this.alllength*curPer)/100);

            }else{
                this.curMoney = this.max;
                this.slider.setPercent(100);
            }
            

            this.curselMoney.setString(this.curMoney);


        }
    }
);


var CXCmbinationView = cc.Layer.extend(
    {
        uiJson: null,
        uiNode: null,

        cardSize:null,

        allCardPosition:[],  
        allCardRect:[],
        keepMyCard:[],

        recordOldPos:[],

        zhCards:[],

        mycardValue:null,

        type_one:null,
        type_two:null,

        type_one_bg:null,
        type_two_bg:null,

        flagFirst:true,

        isvisible:false,

        analyCardType:null,

        ctor: function ()
        {
            this._super();
            this.uiJson = ccs.load(PackName+"/res/zupai.json");
            this.uiNode = this.uiJson.node;
            this.addChild(this.uiNode);

            this.cardSize = cc.p(88,115)*1.2;

            this.keepMyCard.push(this.getUI("card0"));
            this.keepMyCard.push(this.getUI("card1"));
            this.keepMyCard.push(this.getUI("card2"));
            this.keepMyCard.push(this.getUI("card3"));

            this.type_one = this.getUI("type1");
            this.type_one.ignoreContentAdaptWithSize(true);
            this.type_two = this.getUI("type2")
            this.type_two.ignoreContentAdaptWithSize(true);

            this.type_one_bg = this.getUI("cardtype_bg1");
            this.type_two_bg = this.getUI("cardtype_bg2");

            this.analyCardType = cardAnalysis.HandCards.create();
            

            this.getUI("bg").setLocalZOrder(-1);

            for(var i = 0;i<6;i++)
            {
                this.zhCards[i] = null;
            }
            for(var i = 0;i<4;i++)
            {
                this.zhCards[i] =  this.keepMyCard[i];
            }

            var self = this;
            this.getUI("btn_sure").addClickEventListener(function (sender, evt) {
                var leng = self.zhCards.length;
                var temcardvalues = [];
                for(var i = 0; i<leng; i++)
                {
                    if(self.zhCards[i] != null)
                    {
                        var value = self.zhCards[i].getTag();
                        temcardvalues.push(value);
                    }
                }
                CXModel.myHandCard = [];
                CXModel.myHandCard = utils.clone(temcardvalues);
                log("===============come===========here===========");
                Sound.getInstance().playEffect(PackName+"/res/sound/com/button.mp3");
                CXModel.sendChePai(temcardvalues);
            });

            this.countPoint();
            this.countRect();
            this.init();
            return true;
        },

        countRect:function()
        {
            for(var i in this.allCardPosition)
            {
                var temppos = cc.p(this.allCardPosition[i].x,this.allCardPosition[i].y)
                cc.pAddIn(temppos,cc.p(-(88*1.2/2),-(115*1.2/2)));
                var Rect = cc.rect(temppos, cc.size(88*1.2 ,115*1.2));
                this.allCardRect.push(Rect);
            }
        },

        countPoint:function()
        {
            this.allCardPosition.push(cc.p(400+0,440+0));
            this.allCardPosition.push(cc.p(550+0,440+0));
            this.allCardPosition.push(cc.p(700+0,440+0));
            this.allCardPosition.push(cc.p(850+0,440+0));
            this.allCardPosition.push(cc.p(550+0,230+0));
            this.allCardPosition.push(cc.p(700+0,230+0));


            this.recordOldPos.push(cc.p(400,440));
            this.recordOldPos.push(cc.p(550,440));
            this.recordOldPos.push(cc.p(700,440));
            this.recordOldPos.push(cc.p(850,440));
            this.recordOldPos.push(cc.p(550,230));
            this.recordOldPos.push(cc.p(700,230));
        },

        init:function()
        {
            var self=this;

            self.getUI("btn_sure").setEnabled(false);

            var temcardvalues = [];
            var num = 0;
            var selectCard = -1;  //选择的牌

            if(!self.flagFirst)
            {
                return ;
            }

            var CardRects = this.allCardRect;
            var CardPos = this.allCardPosition;
            

            self.flagFirst = false;
            var listener = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches:false,
            onTouchBegan: function (touch, event) {

                if(!touch)return false;
                if(!touch.getLocation) return false;
                if(!self.isvisible)
                {
                    return;
                }
                var toucLoc = touch.getLocation();
                for(var i = 0;i < 6; i++)
                {
                    if(self.zhCards[i] != null)
                    {
                        if(cc.rectContainsPoint(CardRects[i],toucLoc))
                           {
                             selectCard = i;
                             num = 0;
                             temcardvalues = [];
                             break;
                           }
                    }
                   
                }
                return true;
            },
            onTouchMoved: function (touch, event) {

                if(!self.isvisible)
                {
                    return;
                }

                if(selectCard != -1)
                {
                    if(self.zhCards[selectCard] != null)
                    {
                        self.zhCards[selectCard].setLocalZOrder(1000);
                        var temLocal = cc.p(touch.getLocation().x,touch.getLocation().y);
                         cc.pAddIn(temLocal,cc.p(-0,-0));
                        self.zhCards[selectCard].setPosition(temLocal);
                    }
                    
                }
            },

            onTouchEnded: function (touch, event) {

                if(!self.isvisible)
                {
                    return;
                }


                if(selectCard != -1)
                {
                    var endlocal = touch.getLocation();

                    cc.pAddIn(endlocal,cc.p(-(88*1.2/2),-(115*1.2/2)));

                    var Rect = cc.rect(endlocal, cc.size(88*1.2 ,115*1.2));
                    if(cc.rectOverlapsRect(Rect,CardRects[4]))
                    {
                        log("============CardRects====4=================");
                        if(self.zhCards[4] == null)
                        {
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[4].x-0,CardPos[4].y-0));
                            self.zhCards[selectCard].setLocalZOrder(100);
                            self.zhCards[4] = self.zhCards[selectCard];
                            self.zhCards[selectCard] = null;
                        }
                        else
                        {
                            var temp = self.zhCards[4];
                            self.zhCards[4] = self.zhCards[selectCard];
                            self.zhCards[4].setPosition(cc.p(CardPos[4].x-0,CardPos[4].y-0));
                            self.zhCards[4].setLocalZOrder(100);
                            self.zhCards[selectCard] = temp;
                            self.zhCards[selectCard].setLocalZOrder(100);
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[selectCard].x-0,CardPos[selectCard].y-0));
                        }
                        
                    }
                    else if(cc.rectOverlapsRect(Rect,CardRects[5]))
                    {
                        log("============CardRects====5=================");
                        if(self.zhCards[5] == null)
                        {
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[5].x-0,CardPos[5].y-0));
                            self.zhCards[5] = self.zhCards[selectCard];
                            self.zhCards[5].setLocalZOrder(100);
                            self.zhCards[selectCard] = null;
                        }
                        else
                        {
                            var temp = self.zhCards[5];
                            self.zhCards[5] = self.zhCards[selectCard];
                            self.zhCards[5].setPosition(cc.p(CardPos[5].x-0,CardPos[5].y-0));
                            self.zhCards[5].setLocalZOrder(100);
                            self.zhCards[selectCard] = temp;
                            self.zhCards[selectCard].setLocalZOrder(100);
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[selectCard].x-0,CardPos[selectCard].y-0));
                        }
                        
                    }
                    else if(cc.rectOverlapsRect(Rect,CardRects[3]))
                    {
                        log("============CardRects====3=================");
                        if(self.zhCards[3] == null)
                        {
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[3].x-0,CardPos[3].y-0));
                            self.zhCards[3] = self.zhCards[selectCard];
                            self.zhCards[3].setLocalZOrder(100);
                            self.zhCards[selectCard] = null;
                        }
                        else
                        {
                            var temp = self.zhCards[3];
                            self.zhCards[3] = self.zhCards[selectCard];
                            self.zhCards[3].setPosition(cc.p(CardPos[3].x-0,CardPos[3].y-0));
                            self.zhCards[3].setLocalZOrder(100);
                            self.zhCards[selectCard] = temp;
                            self.zhCards[selectCard].setLocalZOrder(100);
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[selectCard].x-0,CardPos[selectCard].y-0));
                        }
                        
                    }
                    else if(cc.rectOverlapsRect(Rect,CardRects[2]))
                    {
                        log("============CardRects====2=================");
                        if(self.zhCards[2] == null)
                        {
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[2].x-0,CardPos[2].y-0));
                            self.zhCards[2] = self.zhCards[selectCard];
                            self.zhCards[2].setLocalZOrder(100);
                            self.zhCards[selectCard] = null;
                        }
                        else
                        {
                            var temp = self.zhCards[2];
                            self.zhCards[2] = self.zhCards[selectCard];
                            self.zhCards[2].setPosition(cc.p(CardPos[2].x-0,CardPos[2].y-0));
                            self.zhCards[2].setLocalZOrder(100);
                            self.zhCards[selectCard] = temp;
                            self.zhCards[selectCard].setLocalZOrder(100);
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[selectCard].x-0,CardPos[selectCard].y-0));
                        }
                        
                    }
                    else if(cc.rectOverlapsRect(Rect,CardRects[1]))
                    {
                        log("============CardRects====1=================");
                        if(self.zhCards[1] == null)
                        {
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[1].x-0,CardPos[1].y-0));
                            self.zhCards[1] = self.zhCards[selectCard];
                            self.zhCards[1].setLocalZOrder(100);
                            self.zhCards[selectCard] = null;
                        }
                        else
                        {
                            var temp = self.zhCards[1];
                            self.zhCards[1] = self.zhCards[selectCard];
                            self.zhCards[1].setPosition(cc.p(CardPos[1].x-0,CardPos[1].y-0));
                            self.zhCards[1].setLocalZOrder(100);
                            self.zhCards[selectCard] = temp;
                            self.zhCards[selectCard].setLocalZOrder(100);
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[selectCard].x-0,CardPos[selectCard].y-0));
                        }
                        
                    }
                    else if(cc.rectOverlapsRect(Rect,CardRects[0]))
                    {
                        if(self.zhCards[0] == null)
                        {
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[0].x-0,CardPos[0].y-0));
                            self.zhCards[0] = self.zhCards[selectCard];
                            self.zhCards[0].setLocalZOrder(100);
                            self.zhCards[selectCard] = null;
                        }
                        else
                        {
                            var temp = self.zhCards[0];
                            self.zhCards[0] = self.zhCards[selectCard];
                            self.zhCards[0].setPosition(cc.p(CardPos[0].x-0,CardPos[0].y-0));
                            self.zhCards[0].setLocalZOrder(100);
                            self.zhCards[selectCard] = temp;
                            self.zhCards[selectCard].setLocalZOrder(100);
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[selectCard].x-0,CardPos[selectCard].y-0));
                        }
                        
                    }
                    else{
                        self.zhCards[selectCard].setPosition(cc.p(CardPos[selectCard].x-0,CardPos[selectCard].y-0));
                        self.zhCards[selectCard].setLocalZOrder(100);
                    }


                    var leng = self.zhCards.length;
                    num = 0;
                    temcardvalues = [];
                    for(var i = 0; i< 4; i++)
                    {
                        if(self.zhCards[i] != null)
                        {
                            num++;
                            temcardvalues.push(self.zhCards[i].getTag());
                        }
                    }
                    if(num == 2 && self.zhCards[4] != null && self.zhCards[5] != null)
                    {

                        self.getUI("btn_sure").setEnabled(true);

                        temcardvalues.push(self.zhCards[4].getTag());
                        temcardvalues.push(self.zhCards[5].getTag());
                        CXModel.myHandCard = [];
                        log("===============come===========here========here===");
                        CXModel.myHandCard = utils.clone(temcardvalues);

                        // CXModel.CAnaisysCardType(temcardvalues);

                        self.analyCardType.createAnalysisCards(temcardvalues,0);
                        var tou = self.analyCardType.getTouCardName();
                        var wei = self.analyCardType.getWeiCardName();
                        self.showResultCardType(tou,wei);

                    }else{
                        self.type_one.setVisible(false);
                        self.type_two.setVisible(false);

                        self.type_one_bg.setVisible(false);
                        self.type_two_bg.setVisible(false);

                        self.getUI("btn_sure").setEnabled(false);


                    }
                    self.updataZhCards(self.zhCards);

                    
                }
                selectCard = -1;
                }
            });
            cc.eventManager.addListener(listener, this.uiNode);
        },

        updataZhCards:function(zcards)
        {
            this.zhCards = [];
            for(var i = 0; i< 6; i++)
            {

                this.zhCards[i] = zcards[i];
            }

        },

        showResultCardType:function(type1,type2)
        {

            this.type_one_bg.setVisible(true);
            this.type_two_bg.setVisible(true);

            this.type_one.loadTexture("CXCardType/"+type1+".png",1);
            this.type_one.setVisible(true);
            this.type_two.loadTexture("CXCardType/"+type2+".png",1);
            this.type_two.setVisible(true);
        },

        setTouchEnable:function()
        {
            this.isvisible = false;
        },

        refreshView:function(cardValues)
        {
            this.isvisible = true;

            this.keepMyCard = [];
            this.zhCards = [];
            this.mycardValue = [];

            this.type_one.setVisible(false);
            this.type_two.setVisible(false);

            this.type_one_bg.setVisible(false);
            this.type_two_bg.setVisible(false);

            var len = cardValues.length;
            log("=======cardValues---length===="+len);
            for(var i = 0;i < len; i++)
            {
                this.mycardValue.push(cardValues[i])
                this.getUI("card"+i).setTag(cardValues[i]);
                this.getUI("card"+i).loadTexture("CX/card_"+utils.toHex2(this.mycardValue[i])+".png",1);
                this.getUI("card"+i).setPosition(this.recordOldPos[i]);
                this.keepMyCard.push(this.getUI("card"+i));
            }
            
            for(var i = 0;i<6;i++)
            {
                this.zhCards[i] = null;
            }
            for(var i = 0;i<4;i++)
            {
                this.zhCards[i] =  this.keepMyCard[i];
            }

            this.init();
        },

        getUI:function(key)
        {
            return this.uiNode.getChildByName(key);
        },
    }
);


//--------添加小结算界面---------------



var CXBalanceItem = ccui.Layout.extend(
    {
        ctor: function () {
            this._super();
            var json = ccs.load(PackName + "/res/tmp.json");
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
                var pos = CXModel.getPosBySeatID(player.seatid);

                var Image = this.getUI("playImage");
                this.m_pAvatar = utils.createCircleAvatar(player.avatar, "deh/res/pic/p_bg.png", "deh/res/pic/p_bg.png", cc.size(92,92));
                this.m_pAvatar.setPosition(cc.p(55,58));
                Image.addChild(this.m_pAvatar,10);

                var name = this.getUI("playname");
                name.ignoreContentAdaptWithSize(true);
                name.setString(player.name);

                var cardtype1 = this.getUI("cardtype_bg1");
                cardtype1.setVisible(false);
                var cardtype2 = this.getUI("cardtype_bg2");
                cardtype2.setVisible(false);

                var cardTem = [];
                var card1 = this.getUI("card1");
                card1.loadTexture("CX/card_00.png",1);
                card1.setVisible(false);
                cardTem.push(card1);

                var card2 = this.getUI("card2");
                card2.loadTexture("CX/card_00.png",1);
                card2.setVisible(false);
                cardTem.push(card2);

                var card3 = this.getUI("card3");
                card3.loadTexture("CX/card_00.png",1);
                card3.setVisible(false);
                cardTem.push(card3);

                var card4 = this.getUI("card4");
                card4.loadTexture("CX/card_00.png",1);
                card4.setVisible(false);
                cardTem.push(card4);


                var zang = this.getUI("zang");
                if(player.seatid == CXModel.dealer)
                {
                    zang.setVisible(true);
                }
                else
                {
                    zang.setVisible(false);
                }

                if(player.betting == 1)
                {
                    //非特殊牌型
                    if(player.tou_cards_name != -1)
                    {
                        cardtype1.setVisible(true);
                        cardtype1.getChildByName("type").ignoreContentAdaptWithSize(true);
                        cardtype1.getChildByName("type").loadTexture("CXCardType/"+player.tou_cards_name+".png",1);
                    }
                    if(player.wei_cards_name != -1)
                    {
                        cardtype2.setVisible(true);
                        cardtype2.getChildByName("type").ignoreContentAdaptWithSize(true);
                        cardtype2.getChildByName("type").loadTexture("CXCardType/"+player.wei_cards_name+".png",1); 
                    }
                }
                


                if(CXModel.isMySeatID(player.seatid))
                {
                    var len = CXModel.myHandCard.length;
                    for(var k = 0; k<len; k++)
                    {
                        if(cardTem[k] != null)
                        {
                            cardTem[k].setVisible(true);
                            cardTem[k].loadTexture("CX/card_"+utils.toHex2(CXModel.myHandCard[k])+".png",1);
                        }
                    }
                }
                else
                {
                    var len = CXModel.otherPlayCards[pos].length;
                    var start = 0;
                    if(!CXModel.isOnVideo)
                    {
                        if(len <= 2)
                        {
                            start = 2;
                            len = len + 2;
                        }
                    }
                    

                    cardTem[0].setVisible(true);
                    cardTem[1].setVisible(true);
                    if(len > start)
                    {
                        for(var j = start; j<len; j++)
                        {
                            cardTem[j].setVisible(true);
                            if(start == 2)
                            {
                                cardTem[j].loadTexture("CX/card_"+utils.toHex2(CXModel.otherPlayCards[pos][j - 2])+".png",1);
                            }else{
                                cardTem[j].loadTexture("CX/card_"+utils.toHex2(CXModel.otherPlayCards[pos][j])+".png",1);
                            }
                            
                        }
                    }
                }
                

                var txt_r = this.getUI("txt_r");
                var txt_h = this.getUI("txt_h");
                txt_r.ignoreContentAdaptWithSize(true);
                txt_r.setVisible(false);
                txt_h.ignoreContentAdaptWithSize(true);
                txt_h.setVisible(false);

                var num = player.winmoney;
                if (num > 0)
                {
                    txt_r.setVisible(true);
                    txt_r.setString("+"+num)
                }
                else if(num == 0)
                {
                    txt_h.setVisible(true);
                    txt_h.setString("0");
                }
                else
                {
                    txt_h.setVisible(true);
                    txt_h.setString(""+num);
                }


                // var label=null;
                // var num = player.winmoney;
                // if (num > 0) {
                //     label = new  cc.LabelAtlas(":"+num,PackName+ "/res/font/s_number_a.png", 26, 33, '0');
                // }
                // else if(num == 0)
                // {
                //     label = new  cc.LabelAtlas((num),PackName+ "/res/font/s_number_f.png", 26, 33, '0');
                // }
                // else{
                //     label = new  cc.LabelAtlas(":"+(-num),PackName+ "/res/font/s_number_f.png", 26, 33, '0');
                // }
                // label.setAnchorPoint(cc.p(0.5, 0.5));

                // label.setTag(parseInt(8888));
                // label.setPosition(cc.p(92,30));
                // this.uiNode.addChild(label, 20000);

        }
    });




var CXBalanceView = cc.Layer.extend(
    {
        uiJson: null,
        uiNode: null,

        itemPlay0: null,
        itemPlay1: null,
        itemPlay2: null,
        itemPlay3: null,
        itemPlay4: null,

        callBackAction:null,

        allItem: [],

        m_pAvatar:null,

        list:null,

        shareBg:null,

        ctor: function ()
        {
            this._super();
            this.uiJson = ccs.load(PackName+"/res/balance.json");
            this.uiNode = this.uiJson.node;
            this.addChild(this.uiNode);

            this.shareBg = this.getUI("bg");

            this.getUI("bg").getChildByName("gnum").getChildByName("gamesnum").ignoreContentAdaptWithSize(true);
            this.getUI("bg").getChildByName("roomnum").ignoreContentAdaptWithSize(true);
            this.getUI("bg").getChildByName("gametime").ignoreContentAdaptWithSize(true);

            var self = this;
            this.getUI("bg").getChildByName("btn_sure").addClickEventListener(function (sender, evt) {
                if(self.callBackAction != null)
                {
                    self.callBackAction();
                }
            });

            this.getUI("bg").getChildByName("sharebtn").addClickEventListener(function (sender, evt) {

                this.getUI("bg").getChildByName("sharebtn").setVisible(false);
                CXPlayers.publicBox.setVisible(false);

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
                    CXPlayers.publicBox.setVisible(true);
                    this.runAction(cc.sequence(cc.delayTime(0.5),cc.callFunc(function(){
                        this.getUI("bg").getChildByName("sharebtn").setVisible(true);
                    }.bind(this))));
                    
                }.bind(this));
            }.bind(this));

            this.list = this.getUI("bg").getChildByName("players");
            this.list.setScrollBarEnabled(false);
            return true;
        },

        initdata:function()
        {

            this.getUI("bg").getChildByName("gnum").getChildByName("gamesnum").setString(CXModel.curGame+"");
            this.getUI("bg").getChildByName("roomnum").setString(CXModel.roomData["gameRoom"]["code"]);
            this.getUI("bg").getChildByName("gametime").setString("当前时间:"+utils.getTime(utils.getTimeStamp()));

            this.list.removeAllChildren();

             var tempPlayer = [];
            for(var i = 0;i < 6; i++)
            {
                var player = CXModel.players[i];
                if(player == null || player.ready == 0)
                {
                    continue;
                }

                if(i == CXModel.seatID)
                {
                    tempPlayer.push(player);
                }
            }

            for(var i = 0;i < 6; i++)
            {
                var player = CXModel.players[i];
                if(player == null || player.ready == 0)
                {
                    continue;
                }

                if(i != CXModel.seatID)
                {
                    tempPlayer.push(player);
                }
            }

            var len = tempPlayer.length;
            for(var i = 0;i < len; i++)
            {
                var player = tempPlayer[i];
                var temp = new CXBalanceItem();
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



var CXGameEndItem = ccui.Layout.extend(
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
            var Image = this.getUI("playImage");
            var avrt = utils.getValStr(data, "avatar");
            this.m_pAvatar = utils.createCircleAvatar(avrt, "deh/res/pic/p_bg.png", "deh/res/pic/p_bg.png", cc.size(92,92));
            this.m_pAvatar.setPosition(cc.p(55,58));
            Image.addChild(this.m_pAvatar,10);

            var name = this.getUI("playername");
            var playname = utils.getValStr(data, "name");
            name.setString(playname);


            var id = this.getUI("playid");
            var playid = utils.getValInt(data, "uid");
            id.setString("ID:"+playid);


            if(flag == true)
            {
                this.getUI("winbg").setVisible(true);
            }

            var txt_r = this.getUI("txt_r");
            var txt_h = this.getUI("txt_h");
            txt_r.ignoreContentAdaptWithSize(true);
            txt_r.setVisible(false);
            txt_h.ignoreContentAdaptWithSize(true);
            txt_h.setVisible(false);

            var num = utils.getValInt(data, "score");
            if (num > 0)
            {
                txt_r.setVisible(true);
                txt_r.setString("+"+num)
            }
            else if(num == 0)
            {
                txt_h.setVisible(true);
                txt_h.setString("0");
            }
            else
            {
                txt_h.setVisible(true);
                txt_h.setString(""+num);
            }


            // var label=null;
            // var num = utils.getValInt(data, "score");
            // if (num > 0) {
            //     label = new  cc.LabelAtlas(":"+num,PackName+ "/res/font/s_number_a.png", 26, 33, '0');
            //     if(flag == true)
            //     {
            //         this.getUI("winbg").setVisible(true);
            //     }
            // }
            // else if(num == 0)
            // {
            //     label = new  cc.LabelAtlas((num),PackName+ "/res/font/s_number_f.png", 26, 33, '0');
            // }
            // else{
            //     label = new  cc.LabelAtlas(":"+(-num),PackName+ "/res/font/s_number_f.png", 26, 33, '0');
            // }
            // label.setAnchorPoint(cc.p(0.5, 0.5));

            // label.setTag(parseInt(8888));
            // label.setPosition(cc.p(85,10));
            // this.uiNode.addChild(label, 20000);

        }
    });


//游戏结束 结算界面
var CXGameEndView = cc.Layer.extend(
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


            this.shareBg = this.getUI("shareBg");
            this.getUI("logo").setVisible(false);

            var self = this;
            this.getUI("btn_leave").addClickEventListener(function (sender, evt) {
                ZJHModel.getInstance().mySeatid = -1;
                CXModel.isGameOver = true;
                ZJHModel.getInstance().roomToShareView(CXModel.curRoomID);
                // self.removeFromParent(true);
            });

            this.getUI("btn_share").addClickEventListener(function (sender, evt) {

                CXPlayers.publicBox.setVisible(false);
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
                    CXPlayers.publicBox.setVisible(true);
                    this.runAction(cc.sequence(cc.delayTime(0.5),cc.callFunc(function(){
                        this.getUI("btn_share").setVisible(true);
                    }.bind(this))));
                    this.getUI("btn_leave").setVisible(true);
                    this.getUI("logo").setVisible(false);

                    this.runAction(cc.sequence(cc.delayTime(0.5),cc.callFunc(function(){
                        ZJHModel.getInstance().mySeatid = -1;
                        // ZJHModel.getInstance().roomToHall();
                        ZJHModel.getInstance().roomToShareView(CXModel.curRoomID);
                    }.bind(this))));
                    

                }.bind(this));
            }.bind(this));

            return true;
        },

        initdata:function(val)
        {
            
            this.getUI("roomnum").setString(CXModel.roomData["gameRoom"]["code"]);
            this.getUI("roomtime_s").setString("开始:"+utils.getTime(CXModel.roomData["gameRoom"]["createTime"]));
            this.getUI("roomtime_e").setString("结束:"+utils.getTime(utils.getValInt(val,"time") *1000));

            var list = this.getUI("players");
            // list.setScrollBarEnabled(false);

            var maxScoreIndex = 0;
            var maxScore = 0;

             var temData = [];

            if(val.hasOwnProperty("players"))
             {
                var len  = val["players"].length;
                for(var i = 0;i < len; i++)
                {

                    var uid = val["players"][i]["uid"];

                    if(CXModel.isMyUID(uid))
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

                    if(!CXModel.isMyUID(uid))
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
                    var item  = new CXGameEndItem();
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
