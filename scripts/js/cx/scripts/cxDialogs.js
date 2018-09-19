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
            this.uiNode[3] = ccs.load("public/res/bigfaceCL.json").node;
            this.addChild(this.uiNode[3] );

            this.uiNode[1] = ccs.load("public/res/bigfaceCR.json").node;
            this.addChild(this.uiNode[1] );

            this.uiNode[2] = ccs.load("public/res/bigfaceCT.json").node;
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
            }else if(pos == 3)
            {
                spos = 2;
            }else if(pos == 4 || pos == 5)
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
            // if(player.bobo_money + player.bet > CXModel.max_raise_bet)
            // {
            //     this.maxBobo = CXModel.max_raise_bet;
            // }else{
                this.maxBobo = player.bobo_money + player.bet;
            // }

            this.minmaxdis = this.maxBobo - this.mixBobo;

            var curPer = this.slider.getPercent();
            this.curMoney = Math.ceil(this.mixBobo + (this.minmaxdis*curPer)/100);
            this.curSelMoney.setString(this.curMoney);

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
            
            this.min_bo.setString("$"+this.mix);
            this.max_bo.setString("$"+this.max);

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

        flagFirst:true,

        isvisible:false,

        analyCardType:null,

        ctor: function ()
        {
            this._super();
            this.uiJson = ccs.load(PackName+"/res/zupai.json");
            this.uiNode = this.uiJson.node;
            this.addChild(this.uiNode);

            this.cardSize = cc.p(88,115)*0.8;

            this.keepMyCard.push(this.getUI("card0"));
            this.keepMyCard.push(this.getUI("card1"));
            this.keepMyCard.push(this.getUI("card2"));
            this.keepMyCard.push(this.getUI("card3"));

            this.type_one = this.getUI("type1");
            this.type_one.ignoreContentAdaptWithSize(true);
            this.type_two = this.getUI("type2")
            this.type_two.ignoreContentAdaptWithSize(true);

            this.analyCardType = cardAnalysis.HandCards.create();
            

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
                cc.pAddIn(temppos,cc.p(-(88*0.8/2),-(115*0.8/2)));
                var Rect = cc.rect(temppos, cc.size(88*0.8 ,115*0.8));
                this.allCardRect.push(Rect);
            }
        },

        countPoint:function()
        {
            this.allCardPosition.push(cc.p(88+110,500+340));
            this.allCardPosition.push(cc.p(200+110,500+340));
            this.allCardPosition.push(cc.p(305+110,500+340));
            this.allCardPosition.push(cc.p(405+110,500+340));
            this.allCardPosition.push(cc.p(200+110,280+340));
            this.allCardPosition.push(cc.p(284+110,280+340));


            this.recordOldPos.push(cc.p(88,500));
            this.recordOldPos.push(cc.p(200,500));
            this.recordOldPos.push(cc.p(305,500));
            this.recordOldPos.push(cc.p(405,500));
            this.recordOldPos.push(cc.p(200,280));
            this.recordOldPos.push(cc.p(284,280));
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
                         cc.pAddIn(temLocal,cc.p(-110,-340));
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

                    cc.pAddIn(endlocal,cc.p(-(88*0.8/2),-(115*0.8/2)));

                    var Rect = cc.rect(endlocal, cc.size(88*0.8 ,115*0.8));
                    if(cc.rectOverlapsRect(Rect,CardRects[4]))
                    {
                        log("============CardRects====4=================");
                        if(self.zhCards[4] == null)
                        {
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[4].x-110,CardPos[4].y-340));
                            self.zhCards[selectCard].setLocalZOrder(100);
                            self.zhCards[4] = self.zhCards[selectCard];
                            self.zhCards[selectCard] = null;
                        }
                        else
                        {
                            var temp = self.zhCards[4];
                            self.zhCards[4] = self.zhCards[selectCard];
                            self.zhCards[4].setPosition(cc.p(CardPos[4].x-110,CardPos[4].y-340));
                            self.zhCards[4].setLocalZOrder(100);
                            self.zhCards[selectCard] = temp;
                            self.zhCards[selectCard].setLocalZOrder(100);
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[selectCard].x-110,CardPos[selectCard].y-340));
                        }
                        
                    }
                    else if(cc.rectOverlapsRect(Rect,CardRects[5]))
                    {
                        log("============CardRects====5=================");
                        if(self.zhCards[5] == null)
                        {
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[5].x-110,CardPos[5].y-340));
                            self.zhCards[5] = self.zhCards[selectCard];
                            self.zhCards[5].setLocalZOrder(100);
                            self.zhCards[selectCard] = null;
                        }
                        else
                        {
                            var temp = self.zhCards[5];
                            self.zhCards[5] = self.zhCards[selectCard];
                            self.zhCards[5].setPosition(cc.p(CardPos[5].x-110,CardPos[5].y-340));
                            self.zhCards[5].setLocalZOrder(100);
                            self.zhCards[selectCard] = temp;
                            self.zhCards[selectCard].setLocalZOrder(100);
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[selectCard].x-110,CardPos[selectCard].y-340));
                        }
                        
                    }
                    else if(cc.rectOverlapsRect(Rect,CardRects[3]))
                    {
                        log("============CardRects====3=================");
                        if(self.zhCards[3] == null)
                        {
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[3].x-110,CardPos[3].y-340));
                            self.zhCards[3] = self.zhCards[selectCard];
                            self.zhCards[3].setLocalZOrder(100);
                            self.zhCards[selectCard] = null;
                        }
                        else
                        {
                            var temp = self.zhCards[3];
                            self.zhCards[3] = self.zhCards[selectCard];
                            self.zhCards[3].setPosition(cc.p(CardPos[3].x-110,CardPos[3].y-340));
                            self.zhCards[3].setLocalZOrder(100);
                            self.zhCards[selectCard] = temp;
                            self.zhCards[selectCard].setLocalZOrder(100);
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[selectCard].x-110,CardPos[selectCard].y-340));
                        }
                        
                    }
                    else if(cc.rectOverlapsRect(Rect,CardRects[2]))
                    {
                        log("============CardRects====2=================");
                        if(self.zhCards[2] == null)
                        {
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[2].x-110,CardPos[2].y-340));
                            self.zhCards[2] = self.zhCards[selectCard];
                            self.zhCards[2].setLocalZOrder(100);
                            self.zhCards[selectCard] = null;
                        }
                        else
                        {
                            var temp = self.zhCards[2];
                            self.zhCards[2] = self.zhCards[selectCard];
                            self.zhCards[2].setPosition(cc.p(CardPos[2].x-110,CardPos[2].y-340));
                            self.zhCards[2].setLocalZOrder(100);
                            self.zhCards[selectCard] = temp;
                            self.zhCards[selectCard].setLocalZOrder(100);
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[selectCard].x-110,CardPos[selectCard].y-340));
                        }
                        
                    }
                    else if(cc.rectOverlapsRect(Rect,CardRects[1]))
                    {
                        log("============CardRects====1=================");
                        if(self.zhCards[1] == null)
                        {
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[1].x-110,CardPos[1].y-340));
                            self.zhCards[1] = self.zhCards[selectCard];
                            self.zhCards[1].setLocalZOrder(100);
                            self.zhCards[selectCard] = null;
                        }
                        else
                        {
                            var temp = self.zhCards[1];
                            self.zhCards[1] = self.zhCards[selectCard];
                            self.zhCards[1].setPosition(cc.p(CardPos[1].x-110,CardPos[1].y-340));
                            self.zhCards[1].setLocalZOrder(100);
                            self.zhCards[selectCard] = temp;
                            self.zhCards[selectCard].setLocalZOrder(100);
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[selectCard].x-110,CardPos[selectCard].y-340));
                        }
                        
                    }
                    else if(cc.rectOverlapsRect(Rect,CardRects[0]))
                    {
                        if(self.zhCards[0] == null)
                        {
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[0].x-110,CardPos[0].y-340));
                            self.zhCards[0] = self.zhCards[selectCard];
                            self.zhCards[0].setLocalZOrder(100);
                            self.zhCards[selectCard] = null;
                        }
                        else
                        {
                            var temp = self.zhCards[0];
                            self.zhCards[0] = self.zhCards[selectCard];
                            self.zhCards[0].setPosition(cc.p(CardPos[0].x-110,CardPos[0].y-340));
                            self.zhCards[0].setLocalZOrder(100);
                            self.zhCards[selectCard] = temp;
                            self.zhCards[selectCard].setLocalZOrder(100);
                            self.zhCards[selectCard].setPosition(cc.p(CardPos[selectCard].x-110,CardPos[selectCard].y-340));
                        }
                        
                    }
                    else{
                        self.zhCards[selectCard].setPosition(cc.p(CardPos[selectCard].x-110,CardPos[selectCard].y-340));
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

                        CXModel.CAnaisysCardType(temcardvalues);

                        // self.analyCardType.createAnalysisCards(temcardvalues,0);
                        // var tou = self.analyCardType.getTouCardName();
                        // var wei = self.analyCardType.getWeiCardName();
                        // self.showResultCardType(tou,wei);

                    }else{

                        self.getUI("btn_sure").setEnabled(false);

                        self.type_one.setVisible(false);
                        self.type_two.setVisible(false);
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
            this.type_one.loadTexture("CXCardType/jin/"+type1+".png",1);
            this.type_one.setVisible(true);
            this.type_two.loadTexture("CXCardType/jin/"+type2+".png",1);
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
