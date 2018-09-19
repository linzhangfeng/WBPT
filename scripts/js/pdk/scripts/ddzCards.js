
var DDZCard = cc.Node.extend(
    {
        
        ischeckcard:true,
        ctor:function ()
        {
            this._super();
            var json = ccs.load(PackName+"/res/card.json");
            this.uiNode =json.node;
            this.addChild(this.uiNode);
            this.setContentSize(this.uiNode.getContentSize());
            this.setAnchorPoint(cc.p(0.5,0.5));
            this.getUI("num").ignoreContentAdaptWithSize(true);
            this.isUp=false;
            this.isClick=false;
            this.isChoose=false;
            this.istouch=true;
            this.value=-1;
            this.num=0;
            this.reset();
            this.curY = -1111;
            this.setCascadeOpacityEnabled(true);
            this.setOpacity(255);
        },

        getUI:function(key)
        {
            return this.uiNode.getChildByName(key);
        },
        showUI:function(key ,is)
        {
            this.getUI(key).setVisible(is);
        },
        //设置牌点数
        setValue:function(value)
        {
            this.value = value;
            if(value == -1)
            {
                this.getUI("card").loadTexture("ddz/card_000.png",1);
            }
            else
            {
                this.getUI("card").loadTexture("ddz/card_"+utils.toHex2(value)+".png",1);
            }
        },
        //设置数量
        setNum:function(num)
        {
            this.num = num;

            var label = this.getUI("num");
            if(num>0)
            {
                label.setString(""+num);
                label.setVisible(true);
            }
            else
            {
                label.setVisible(false);
            }
        },

        //设置是否点选
        setClick:function(is)
        {
            this.isClick= is;
            this.showUI("click",is);
        },

        setTouch:function(is)
        {
            this.istouch = is;
        },

        //设置是否选中
        setChoose:function(is)
        {
            this.isChoose= is;
            this.showUI("choose",is);
        },
        //设置是否为地主牌
        setDizhu:function(is)
        {
            this.showUI("dizhu",is);
        },

        //设置是否为明牌
        setMingpai:function(is)
        {
            this.showUI("mingpai",is);
        },
        //设置是否上移
        setUp:function(is)
        {
            if(this.isUp == is)
            {
                return;
            }
            this.isUp= is;

            if(this.curY == -1111)
            {
                this.curY = this.getPositionY();
            }
           // this.stopAllActions();
            if(is)
            {
                this.setPositionY(this.curY+15);
               // this.runAction(cc.moveTo(0.01,this.getPositionX(),this.curY+15));
            }
            else{
                this.setPositionY(this.curY);
              //  this.runAction(cc.moveTo(0.01,this.getPositionX(),this.curY));
            }
        },
        getisUp:function()
        {
            return this.isUp;
        },
        reset:function()
        {
            this.stopAllActions();
            this.setNum(0);
            this.setUp(false);
            this.setClick(false);
            this.setChoose(false);
            this.setDizhu(false);
            this.setMingpai(false);
            this.setScale(1);
            this.setVisible(false);
            this.setTouch(true);
        }

    });

var DDZCards =
{
    uiNode:null,
    tuoguanBg:null,
    qxtuoguanBtn:null,
    cardPool:[],
    handCards:[],
    outCards:[],
    publicCards:{},

    upCardValues:[],

    handCardValues:[],
    showCardValues:[],
    outCardValues:[],


    dzPos:-1,
    mpPos:[],

    cardFind:null,
    tipCardValues:[],
    tipNum:0,
    tipTimes:-1,

    cardW:155,
    cardH:216,
    dx:73,
    dy:216,
    my:108,
    ocardW:70,
    ocardH:92,
    odx:30,
    ody:45,
    touchBegin:{},

    tempCards:[],
    sendDatatime:0,
    isMyFristAutoLine:false,
    init:function()
    {
        if(this.uiNode == null)
        {
            return;
        }
        this.cardFind =libddz.cardsFind.create();
        this.tuoguanBg = this.getUI("tuoguanBg");
        this.tuoguanBg.setLocalZOrder(9999);
        this.tuoguanBg.setVisible(false);
        this.qxtuoguanBtn =  this.tuoguanBg.getChildByName("quxiaotuoguanBtn");
        this.uiNode.setContentSize(cc.director.getWinSize());
        
        for(var i =0;i <1;++i)
        {
            var card = new DDZCard();
            this.uiNode.addChild(card);
            this.cardPool.push(card);
        }
        var size =cc.director.getWinSize();
        for(var i =0; i < 3;++i)
        {
            this.handCards[i]=[];

            // this.publicCards[i] =  this.getUI("card"+i);
        }
        for(var i =0; i < 3;++i)
        {
            this.showCardValues[i]=[];
        }
        var self=this;
        self.touchBegin = cc.p(0,0);
        var listener = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches:false,
            onTouchBegan: function (touch, event) {

                if(!touch)return false;
                if(!touch.getLocation) return false;

                self.isCheckTempTouch = false;
                self.touchBegin = touch.getLocation();
                if(DDZModel.spliceCardstate)
                {

                    if(DDZModel.splitcardSeait != -1)
                    {

                        var rect = cc.rect(120,320,1100,200);
                        if(cc.rectContainsPoint(rect,self.touchBegin))
                        {
                            // this.ischeckcard = true;
                            self.isCheckTempTouch = true;
                            self.checkTempTouch(touch);
                        }
                    }
                }
                

                if(!DDZModel.spliceCardstate)
                {
                    if(DDZModel.isTuoguan == 1 || DDZModel.seatID == -1 || DDZModel.state != DDZState.PLAYING)
                    {
                        return false;
                    }
                    self.checkTouch(touch);
                }
                
                return true;
            },
            onTouchMoved: function (touch, event) {
                if(DDZModel.splitcardSeait != -1 && self.isCheckTempTouch)
                {
                    // var crutime = (new Date()).valueOf();
                    // if(crutime - this.sendDatatime > 50 )
                    // {
                        self.checkTempTouch(touch);        
                        // this.sendDatatime = (new Date()).valueOf();
                    // }
                    
                }

                if(DDZModel.isTuoguan == 1|| DDZModel.seatID == -1|| DDZModel.state != DDZState.PLAYING)
                {
                    return;
                }
                self.checkTouch(touch);
            },
            onTouchEnded: function (touch, event) {

                if(DDZModel.splitcardSeait != -1 && self.isCheckTempTouch)
                {
                    self.checkTempTouch(touch);        
                }

                if(DDZModel.isTuoguan == 1|| DDZModel.seatID == -1|| DDZModel.state != DDZState.PLAYING)
                {
                    return;
                }
                self.checkTouch(touch);
                self.touchBegin = cc.p(0,0);
                self.checkCardUp();
            }
        });

        this.qxtuoguanBtn.addClickEventListener(function (sender, evt) {

            DDZModel.sendTuoguan();
        });
        cc.eventManager.addListener(listener, this.uiNode);

        this.reset();
    },
    reset:function()
    {
        for(var i in this.cardPool)
        {
            this.cardPool[i].reset();
        }
        for(var i =0; i < 3;++i)
        {
            this.handCards[i]=[];
            this.outCards[i]=[];
            // this.publicCards[i].setVisible(false);
        }
        this.handCardValues=[];
        this.outCardValues=[];
        this.upCardValues=[];
        for(var i =0; i < 3;++i)
        {
            this.showCardValues[i]=[];
        }
        this.touchBegin = cc.p(0,0);
        this.setDiZhuPos(-1);
        this.setMingPaiPos(-1);

        this.cardFind.clear();
        this.tipCardValues =[];
        this.tipNum=0;
        this.tipTimes=-1;
        DDZCards.showTuoGuan(false);
        this.mpPos = [];
        this.sendDatatime = 0;
        this.isMyFristAutoLine = false;
    },

    findCard:function(cards,val)
    {
        for(var i in cards)
        {
            if(cards[i].value == val)
            {
                return i;
            }
        }
        return -1;
    },
    deleteCard:function(cards,val)
    {
        for(var i in cards)
        {
            if(cards[i].value == val)
            {
                cards.splice(i,1);
                break;
            }
        }
    },
    pushHandCard:function(list)
    {
        for(var i in list)
        {
            if(list[i] <=0)
            {
                continue;
            }
            this.handCardValues.push(libddz.card.create(list[i]));
        }
        libddz.cards.sortDown( this.handCardValues);
    },

    pushShowCard:function(pos,list)
    {
        for(var i in list)
        {
            if(list[i] <=0)
            {
                continue;
            }
            this.showCardValues[pos].push(libddz.card.create(list[i]));
        }
        libddz.cards.sortDown(this.showCardValues[pos]);
    },
    pushOutCard:function(list)
    {
        for(var i in list)
        {
            if(list[i] <=0)
            {
                continue;
            }
            this.outCardValues.push(libddz.card.create(list[i]));
        }
        // libddz.cards.sortDown( this.outCardValues);
    },
    createCards:function(list)
    {
        var cards=[];
        for(var i in list)
        {
            if(list[i] <=0)
            {
                continue;
            }
            cards.push(libddz.card.create(list[i]));
        }
        libddz.cards.sortDown( cards);
        return cards;
    },

    createOldCard:function(list)
    {
         var cards=[];
        for(var i in list)
        {
            if(list[i] <=0)
            {
                continue;
            }
            cards.push(libddz.card.create(list[i]));
        }
        return cards;
    },

    getRect: function (v1,v2)
    {
        if (v1 == v2)
        {
            return cc.rect(v1, cc.size(1, 1));
        }
        else
        {
            var x = Math.min(v1.x,v2.x);
            var y =  Math.min(v1.y,v2.y);

            return cc.rect(x,y, Math.max(Math.abs(v1.x - v2.x),1), Math.max(Math.abs(v1.y - v2.y),1));
        }
    },


    checkTouch:function(touch)
    {
        var rect = this.getRect(this.touchBegin, touch.getLocation());
        var vsize = this.handCards[0].length;
        for (var i = 0 ; i < vsize; i ++)
        {
            var mycard = this.handCards[0][i];
            if(mycard.istouch)
            {
                mycard.setChoose(false);
                var pos = mycard.getPosition();
                pos.x -= this.cardW*0.5;
                pos.y += this.cardH*0.5- this.dy;
                var myRect = cc.rect(pos, cc.size(this.dx ,this.dy ));

                if(vsize-1 == i)
                {
                    myRect.width =this.cardW;
                }

                if ( cc.rectIntersectsRect(rect,myRect))
                {
                    mycard.setChoose(true);
                }
            }
        }
    },

    checkTempTouch:function(touch)
    {

        // if(!this.ischeckcard)
        // {
        //     return ;
        // }


        this.ischeckcard = false;

        var Trect = this.getRect(cc.p(120 ,320), touch.getLocation());
        var vsize = this.tempCards.length;
        var max = 0;
        for (var i = 0 ; i < vsize; i ++)
        {
            var mycard = this.tempCards[i];

                var pos = mycard.getPosition();
                pos.x -= this.cardW*0.5;
                pos.y += this.cardH*0.5- this.dy;
                var myRect = cc.rect(pos, cc.size(this.dx ,this.dy ));

                if(vsize-1 == i)
                {
                    myRect.width =this.cardW;
                }
                if ( cc.rectIntersectsRect(Trect,myRect))
                {
                    if(i > max)
                    {
                        max = i;
                    }    
                }
        }
        this.selectCard(max);
        DDZModel.sendPdkSplitCard(3,max);
        this.ischeckcard = true;
    },

    selectCard:function(num)
    {
        log("send message-================"+num);
        

        var vsize = this.tempCards.length;
        for (var i = 0 ; i < vsize; i ++)
        {
            var mycard = this.tempCards[i];
            if(i<num)
            {
                mycard.setChoose(true);
            }else{
                mycard.setChoose(false);
            }
        }


        var all = DDZModel.table_type * 3;
        var per = 90*num/all;
        DDZTips.showSilder(true,per+2);
    },

    checkCardUp:function()
    {
        var needAuto = false;
        if(!DDZModel.isMySeatID(DDZModel.lastSeatID))
        {
            var type = libddz.cardsParse.getCardType(this.outCardValues);
            if(type == libddz.CardsType.CARD_TYPE_ONE || type == libddz.CardsType.CARD_TYPE_TWO
                ||type == libddz.CardsType.CARD_TYPE_THREE )//单张、对子、三张自动处理
            {
                var choose =[];
                var chooseCard =[];
                for(var i  in this.handCards[0])
                {
                    var card  = this.handCards[0][i];
                    if(card.isChoose&&!card.isUp&&card.istouch)
                    {
                        chooseCard.push(card);
                        choose.push(card.value);
                    }
                }

                var cards1 = this.createCards(choose);
                var ct1 = libddz.cardsParse.getCardType(cards1);
                this.upCardValues=this.upCardValues.concat(choose);
                var cards = this.createCards(this.upCardValues);
                var ct = libddz.cardsParse.getCardType(cards);
                var is =  ct== libddz.CardsType.CARD_TYPE_BOMB;
                if(!is)
                {
                    if(ct1 == type)
                    {
                        needAuto = true;
                        this.initUpCard();
                        for(var i  in chooseCard)
                        {
                            var card  = chooseCard[i];
                            card.setUp(true);
                        }
                        this.upCardValues = choose;
                    }

                }
            }
        }

        if(!needAuto&& this.upCardValues.length ==0)
        {
            var choose =[];
            for(var i  in this.handCards[0])
            {
                var card  = this.handCards[0][i];
                if(card.isChoose&&!card.isUp&&card.istouch)
                {
                    choose.push(card.value);
                }
            }
            this.upCardValues=this.upCardValues.concat(choose);
            var cards = this.createCards(this.upCardValues);
            var targetCards = libddz.cards.create(cards);
            var lines =[];
            if(!DDZModel.isMySeatID(DDZModel.lastSeatID)&&this.outCardValues.length>0)
            {
                cc.log("sssssssssssss0");
                var d = libddz.cardsFind.tip(this.outCardValues,cards);
                if(d>0)
                {
                    lines =libddz.cardsFind.results[0];
                }
            }
            else
            {
                cc.log("sssssssssssss1");
                lines = libddz.cardsFind.getRobotLine(targetCards);
            }
            if(lines.length >0)
            {
                this.isMyFristAutoLine = false;
                needAuto = true;
                this.initUpCard();

                this.upCardValues =[];
                this.showUpCard(lines);
            }
        }

        if(!needAuto)
        {
            this.upCardValues =[];
            for(var i  in this.handCards[0])
            {
                var card  = this.handCards[0][i];
                if(card.isChoose&&card.istouch)
                {
                    card.setUp(!card.isUp);
                }
                if(card.istouch)
                {
                    card.setChoose(false);
                }
                
                if(card.isUp&&card.istouch)
                {
                    this.upCardValues.push(card.value);
                }
            }
        }
        DDZAction.pdkcheckOutCard();
    },

    showUpCard:function(cs)
    {
        this.initUpCard();
        for(var i  in cs) {
            if(cs[i].value>0)
            {
                this.upCardValues.push(cs[i].value) ;
            }
        }
        for(var i  in this.handCards[0])
        {
            var card  = this.handCards[0][i];
            for(var j  in this.upCardValues)
            {
                if(this.upCardValues[j] == card.value)
                {
                    card.setUp(true);
                    break;
                }
            }
        }
        DDZAction.pdkcheckOutCard();
    },

    getUI:function(key)
    {
        return  utils.findNode(this.uiNode,key);
    },
    showUI:function(key ,is)
    {
        this.getUI(key).setVisible(is);
    },
    //手牌坐标
    handCardPos:function(pos,num,cardw,dx,dy)
    {
        var size =cc.director.getWinSize();
        var ret = [];
        if(pos ==0)
        {
            var line_nums = (num - 1);
            var bx = size.width / 2 - (dx * line_nums + cardw) / 2 + cardw / 2;
            for (var i = 0 ; i < 20; i ++)
            {
                ret.push(cc.p(bx + dx * i, this.my ));
            }
        }
        else if(pos == 1)
        {

            var line_nums = num >10? 9:(num - 1);
            var bx = size.width - 90 - (dx * line_nums + cardw);

            for (var i = 0 ; i < 20; i ++)
            {
               var di = i % 10;
                var dj = Math.floor(i/ 10);
                ret.push(cc.p(bx + dx * di, 530 - dy*dj ));
            }
        }
        else if(pos == 2)
        {
            var bx = 160;
            for (var i = 0 ; i < 20; i ++)
            {
                var di = i % 10;
                var dj = Math.floor(i/ 10);
                ret.push(cc.p(bx + dx * di, 530 - dy*dj ));
            }
        }
        return ret;
    },
    //取出牌
    newCard:function()
    {
        for(var i in this.cardPool)
        {
            if(!this.cardPool[i].isVisible())
            {
                return this.cardPool[i];
            }
        }

        var card = new DDZCard();
        this.uiNode.addChild(card);
        this.cardPool.push(card);
        return card;
    },
    resetCards:function()
    {
        for(var i in this.cardPool)
        {
            this.cardPool[i].reset();
        }
        for(var i =0; i < 3;++i)
        {
            this.handCards[i]=[];
        }
    },

    SplitFapai:function(startPos,cardValue,cardPos)
    {
        this.resetCards();
        var allcards = cardPos;
        if (DDZModel.isMyInTable())
        {
            cardPos =allcards/3;
            mcardPos = allcards%3;
            DDZModel.isFaPai = true;
            var dt = 0;
            this.tempCards =[];
            for (var i = 0 ; i < allcards; i ++)//牌组叠起
            {
                var card =this.newCard();
                if(i == mcardPos+3)
                {
                    card.setValue(cardValue);
                }
                else
                {
                    card.setValue(0);
                }
                card.setScale(0.8);

                card.setPosition(180+0.1*i,400+0.1*i);
                card.setVisible(true);
                card.setLocalZOrder(i);
                card.runAction(cc.sequence(cc.delayTime(dt),cc.show()));
                dt+=0.007;
                this.tempCards.push(card);
            }

            dt+=0.1;
            for (var i = allcards-1 ; i >= 0; --i)//牌组平移
            {
                var w = 19.5*i-0.1*i;
                var card  = this.tempCards[i];
                card.setVisible(false);
                if(i<allcards/2)
                {
                    card.setChoose(true);
                }
                
                card.runAction(cc.sequence(cc.delayTime(dt),cc.moveBy(w/10000, cc.p(w,-0.1*i))));
            }
        }
    },

    qiecard:function(startpos,endpos)
    {
        if(this.tempCards.length > 0)
        {
            for(var i=0;i<this.tempCards.length;i++)
            {
            
            }
        }
    },

    fapai:function(num)
    {
        this.resetCards();

       var  poses = this.handCardPos(0,num,this.cardW,this.dx,this.dy);

        DDZModel.isFaPai = true;
        for(var i = 0; i < num;++i)
        {
            var card =this.newCard();

            card.setVisible(true);
            card.setLocalZOrder(i);
            card.setValue(0);

            card.setPosition(poses[i].x+2000,poses[i].y);
            utils.scaleToSize(card,this.cardW,this.cardH);
            this.handCards[0].push(card);

            card.runAction(cc.sequence(cc.delayTime(0.01*i),cc.moveTo(0.1,poses[i])));
        }


        this.uiNode.runAction(cc.sequence(cc.delayTime(0.01*num),cc.callFunc(function(){
            DDZModel.isFaPai = false;
        })));
    },
    //发牌
    showFaPaiCard:function(num)
    {
        this.resetCards();

    },

    //展示手牌
    showHandCards:function(pos,num)
    {

        for(var i  in this.handCards[pos])
        {
            this.handCards[pos][i].reset();
        }
        this.handCards[pos] =[];


        if(pos != 0 &&!(DDZModel.isOnVideo || this.checkmpList(this.mpPos,pos)))
        {
            return;
        }

        var cw =this.ocardW;
        var ch =this.ocardH;
        var cx =this.odx;
        var cy =this.ody;
        var l = num;
        var values =[];

        if(DDZModel.isMyInTable() && pos == 0)
        {
            cw =this.cardW;
            ch =this.cardH;
            cx =this.dx;
            cy =this.dy;
            values = this.handCardValues;
            l =values.length;

            if(l ==0)
            {
                l =  num;
            }
        }
        else  if(DDZModel.isOnVideo || this.checkmpList(this.mpPos,pos))
        {
            values = this.showCardValues[pos];
            l =values.length;

            if(l ==0)
            {
                l =  num;
            }
            log("=====mmmmmmmmmmmmmmmmmmm=====");
        }
        var poses =this.handCardPos(pos,l,cw,cx,cy);
        if(values.length == 0&&pos != 0)
        {
            poses =this.handCardPos(pos,1,cw,cx,cy);
        }
        for(var i =0;i<l;++i)
        {

            var card =this.newCard();
            card.setVisible(true);
            card.setLocalZOrder(i);
            utils.scaleToSize(card,cw,ch);


            if(values.length == 0)
            {
                if(pos == 0)
                {
                    card.setPosition(poses[i]);
                }
                else
                {
                    cc.pAddIn(poses[0],cc.p(-0.3,0.3));
                    card.setPosition(poses[0]);
                    // card.setVisible(false);
                }

                if(num<=2)
                {
                    card.setValue(-1);
                    // card.setNum(num);
                }
                else
                {
                    card.setValue(0);
                }
            }
            else
            {
                log("========show=========poses[i].x="+poses[i].x+"===y=="+poses[i].y);
                card.setPosition(poses[i]);
                card.setValue(values[i].value);
            }

           

            this.handCards[pos].push(card);


            if(this.checkmpList(this.mpPos,pos))
            {
                card.setMingpai(i==l-1);
            }

        }
        log("-------pos----"+pos);
        log("========length======"+this.handCards[pos].length);

        if(DDZModel.isMyInTable() && pos == 0) {
            var w = this.handCards[0].length * this.dx + this.cardW - this.dx;
            if (w < 200) {
                w = 200;
            }
            this.tuoguanBg.setContentSize(cc.size(w, this.cardH));
            this.qxtuoguanBtn.setPositionX(w * 0.5);
            this.tuoguanBg.setPosition(cc.director.getWinSize().width * 0.5,this.cardH*0.5);
        }
    },

    checkmpList:function(mplist,pos)
    {

        var flg = false;
        for(var i = 0 ;i<mplist.length;i++)
        {
            if(mplist[i]==pos)
            {
                flg = true;
                break;
            }
        }
        return flg;
    },

    setMingPaiCardFlag:function(pos,is)
    {
        var leng = this.handCards[pos].length;
        for(var i = 0;i < leng; i++)
        {
            var card = this.handCards[pos][i];
            card.setMingpai(is);
        }
    },

    //牌局结束时展示非己方手牌
    showHandCards1:function(pos,values)
    {
        for(var i  in this.handCards[pos])
        {
            this.handCards[pos][i].reset();
        }
        this.handCards[pos] =[];
        var cw =this.ocardW;
        var ch =this.ocardH;
        var cx =this.odx;
        var cy =this.ody;
        var l = values.length;
        var poses =this.handCardPos(pos,l,cw,cx,cy);
        for(var i =0;i<l;++i)
        {
            var card =this.newCard();
            card.setVisible(true);
            card.setLocalZOrder(i);
            card.setPosition(poses[i]);
            utils.scaleToSize(card,cw,ch);
            card.setValue(values[i].value);
            this.handCards[pos].push(card);
            if(this.dzPos == pos)
            {
                card.setDizhu(i==l-1);
            }
            card.setOpacity(0);
            card.stopAllActions();
            card.runAction(cc.sequence(cc.delayTime(0.05*i),cc.fadeIn(0)));
        }

    },

    removeMyhandAndourCard:function(pos)
    {
        for(var i  in this.outCards[pos])
        {
            this.outCards[pos][i].reset();
        }
        this.outCards[pos] =[];

        for(var i  in this.handCards[pos])
        {
            this.handCards[pos][i].reset();
        }
        this.handCards[pos] =[];
    },

    //展示出牌
    showOutCards:function(pos,values)
    {
        var l = values.length;
        var size = cc.director.getWinSize();
        var obx = 0;
        var oby = 0;
        if (pos == 2)
        {
            obx = 200;
            oby = 380;
        }
        else if (pos == 1)
        {
            var line_nums = (l - 1);
            obx = size.width - 130 - (this.odx * line_nums + this.ocardW);
            oby = 380;
        }
        else if (pos == 0)
        {
            log("laile -------here-----"+pos+"==obx=="+obx);
            var line_nums = (l - 1);
            obx = size.width / 2 - this.odx * line_nums / 2;
            oby = 280;

        }

        for(var i  in this.outCards[pos])
        {
            this.outCards[pos][i].reset();
        }
        this.outCards[pos] =[];

        if(DDZModel.isMyInTable()&&pos == 0)//我方主动出牌时
        {
            oby = 300;
            for(var i= 0,len= this.handCards[0].length;i<len;++i)
            {
                var card = this.handCards[0][i];
                if(this.findCard(this.outCardValues,card.value) > -1)
                {
                    this.outCards[pos].push(card);
                    this.deleteCard(this.handCardValues,card.value);
                    this.handCards[0].splice(i,1);
                    --i;
                    len =  this.handCards[0].length;
                }
            }
        }

        if( this.outCards[pos].length > 0)//我方主动出牌时
        {

            l =this.outCards[pos].length;
            var self = this;
            for(var i in this.outCards[pos]) {
                var card = this.outCards[0][i];
                card.setVisible(true);
                card.setChoose(false);
                card.setMingpai(false);
                card.setUp(false);
                card.stopAllActions();
                (function(z,c)
                {
                    var  func = cc.callFunc(function () {
                        c.setLocalZOrder(z);
                    });
                    c.runAction(cc.spawn(cc.moveTo(0.17, cc.p(obx + z * self.odx, oby))
                        , cc.scaleTo(0.17, self.ocardW / c.getContentSize().width, self.ocardH / c.getContentSize().height)
                        , cc.sequence(cc.delayTime(0.1), func)));
                })(i,card);
            }
        }
        else
        {

            for(var i =0;i<l;++i)
            {
                var card =this.newCard();
                card.setVisible(true);
                card.setChoose(false);
                // card.setMingpai(true);
                card.setLocalZOrder(i);
                var xp = obx + i * this.odx;
                card.setPosition(obx + i * this.odx , oby );
                utils.scaleToSize(card,this.ocardW,this.ocardH);

                card.setValue(values[i].value);
                this.outCards[pos].push(card);
                // if(this.dzPos == pos)
                // {
                //     card.setDizhu(i==l-1);
                // }
            }
        }
    },
    //展示公共牌
    showPublicCard:function(pos,value)
    {
        /*
        if(this.publicCards[pos])
        {
            if(value == -1)
            {
                this.publicCards[pos].setVisible(false);
            }
            else
            {
                this.publicCards[pos].loadTexture("ddz/card_s_"+utils.toHex2(value)+".png",1);
                this.publicCards[pos].setVisible(true);
            }
        }
        */

    },
    //初始化选中的牌
    initUpCard:function()
    {
        this.upCardValues = [];
        for(var i  in this.handCards[0])
        {
            var card  = this.handCards[0][i];
            if(card.istouch)
            {
                card.setUp(false);
                card.setChoose(false);
            }
        }
    },

    initAllDownCard:function()
    {
        this.upCardValues = [];
        for(var i  in this.handCards[0])
        {
            var card  = this.handCards[0][i];
            card.setUp(false);
            card.setChoose(false);
        }
    },

    showTuoGuan:function(is)
    {
        this.tuoguanBg.setVisible(is);
    },
    setDiZhuPos:function(pos)
    {
        this.dzPos = pos;
        if(pos == -1)
        {
            for(var i in this.cardPool)
            {
                this.cardPool[i].setDizhu(false);
            }
        }
    },
    setMingPaiPos:function(pos)
    {
        this.mpPos.push(pos);
        
        if(pos == -1)
        {
            for(var i in this.cardPool)
            {
                this.cardPool[i].setMingpai(false);
            }
        }
        
    },
    resetTipCard:function()
    {
        this.tipCardValues=[];
        this.tipNum = 0;
        this.tipTimes = -1;
    },
    //提示数量
    tipCardNum:function()
    {
        if(this.tipNum == 0)
        {
            this.tipCard();
            var ret = this.tipNum;
            this.resetTipCard();
            return ret;
        }

        return this.tipNum;
    },

    setAllMyCardState:function(is)
    {
        var vsize = this.handCards[0].length;
        for (var i = 0 ; i < vsize; i ++)
        {
            var mycard = this.handCards[0][i];
            mycard.setTouch(!is);
            mycard.setChoose(is);
        }
    },

    checkIsSingleCards:function(allResult)
    {
        var flag = true;
        for(var i =0;i<allResult.length;i++)
        {
            var tempCards = libddz.cards.create(allResult[i]);
            var tempCardsParse = libddz.cardsParse.create(tempCards);
            var type = tempCardsParse.type;

            if(type != libddz.CardsType.CARD_TYPE_ONE)
            {
                flag = false;
                break;
            }
        }
        return flag;
    
    },

    autoTipCard:function()
    {
        this.cardFind.bomb_split = DDZModel.bomb_can_split;  //炸弹可拆不可拆tou

        if(DDZModel.outCardMode == DDZCardState.NEW_CARD)
        {

           this.setAllMyCardState(false);
        }
        else  if(DDZModel.outCardMode == DDZCardState.FOLLOW_CARD)
        {
            if(this.tipTimes == -1)
            {
                this.tipNum = this.cardFind.tip(this.outCardValues,this.handCardValues);
                libddz.cards.sortDown(this.handCardValues);
            }

            if(this.tipNum > 0)
            {
                this.tipTimes=-1;
                this.tipCardValues = this.cardFind.results[0];

                //有牌可以出的时候 需要判断哪些牌要亮起来的

                var outtype = libddz.cardsParse.getCardType(this.outCardValues);
                // var tempCards = libddz.cards.create(this.outCardValues);
                // var tempCardsParse = libddz.cardsParse.create(tempCards);
                 // log("=======typetypetypetypetypetyddddddddddddpe===="+tempCardsParse.type);

                var resultLength = this.cardFind.results.length;
                var allcardValues = [];

                var flag = false;
                for(var i =0;i<resultLength;i++)
                {
                    var tempCards = libddz.cards.create(this.cardFind.results[i]);
                    var tempCardsParse = libddz.cardsParse.create(tempCards);
                    var type = tempCardsParse.type;

                    if(type == libddz.CardsType.CARD_TYPE_THREELINE || type == libddz.CardsType.CARD_TYPE_THREEWITHONE || type == libddz.CardsType.CARD_TYPE_THREEWITHTWO
                        || type == libddz.CardsType.CARD_TYPE_PLANEWITHONE ||type == libddz.CardsType.CARD_TYPE_PLANEWITHWING ||type == libddz.CardsType.CARD_TYPE_PLANEWHITHLACK)
                    {
                        flag = true;
                        break;
                    }

                }

                if(!flag)
                {
                    if(DDZModel.nextPlayisSingle == 1 && this.outCardValues.length == 1)
                    {
                        //只能出最大的牌
                        allcardValues.push(this.handCardValues[0]);

                    }
                    else
                    {
                        for(var i = 0; i < resultLength; i++)
                        {
                            for(var j = 0; j < this.cardFind.results[i].length; j++)
                            {
                                if(!this.checkContentValues(allcardValues,this.cardFind.results[i][j]))
                                {
                                    allcardValues.push(this.cardFind.results[i][j]);
                                }
                            }
                        }
                    }
                    //遮牌
                    this.autoHideCards(allcardValues);
                }else{
                    this.setAllMyCardState(false);
                }

                if(resultLength >  0)
                {
                    if(resultLength == 1 && outtype != libddz.CardsType.CARD_TYPE_PLANEWITHWING)
                    {
                        if(outtype == libddz.CardsType.CARD_TYPE_THREEWITHTWO)
                        {
                            var cs = libddz.cards.create(DDZCards.tipCardValues);
                            this.showUpCard(cs.card3);
                        }
                        else
                        {
                            this.showUpCard(DDZCards.tipCardValues);
                        }

                    }else{
                        if(DDZModel.nextPlayisSingle == 1 && this.outCardValues.length == 1)
                        {
                            this.showUpCard(allcardValues);
                        }
                    }
                    
                }

            }
        }
        return true;
    },

    autoHideCards:function(callcards)
    {
        var hvalues = this.handCardValues;
        if(hvalues.length > 0)
        {
            for(var i = 0;i<hvalues.length;i++)
            {
                // var card = cards[i];
                if(this.checkContentValues(callcards,hvalues[i]))
                {
                    this.handCards[0][i].setChoose(false);
                    this.handCards[0][i].setTouch(true);
                }else{
                    if(this.handCards[0][i].getisUp())
                    {
                        this.handCards[0][i].setUp(false);
                        this.deleteCard(this.upCardValues,hvalues[i].value);
                    }
                    this.handCards[0][i].setChoose(true);
                    this.handCards[0][i].setTouch(false);
                }
            }
        }
    },

    checkContentValues:function(list,va)
    {
        var flag = false;
        for(var i = 0;i < list.length;i++)
        {
            if(list[i] == va || list[i].face == va.face)
            {
                flag = true;
                break;
            }
        }
        return flag;
    },

    checkAllCardis:function(list,va)
    {
        var flag = false;
        for(var i = 0;i < list.length;i++)
        {
            if(libddz.card.compare(list[i],va) == 0)
            {
                flag = true;
                break;
            }
        }
        return flag;
    },

    //提示出牌
    tipCard:function()
    {
        this.cardFind.bomb_split = DDZModel.bomb_can_split;  //炸弹可拆不可拆

        if(DDZModel.outCardMode == DDZCardState.NEW_CARD)
        {
            var isRobot =true;
            if(this.tipCardValues.length !=0)
            {
                this.tipNum = this.cardFind.tip(this.tipCardValues,this.handCardValues);
                libddz.cards.sortDown(this.handCardValues);
                if(this.tipNum > 0)
                {
                    this.tipCardValues = this.cardFind.results[0];
                    isRobot =false;
                }
            }
            if(isRobot)
            {
                this.tipNum = this.cardFind.robot(this.handCardValues);
                libddz.cards.sortDown(this.handCardValues);
                if(this.tipNum > 0)
                {

                    this.tipCardValues = this.cardFind.results[0];
                }
            }
        }
        else
        {
            if(this.tipTimes == -1)
            {
                this.tipNum = this.cardFind.tip(this.outCardValues,this.handCardValues);
                libddz.cards.sortDown(this.handCardValues);
            }
            if(this.tipNum > 0)
            {
                if(this.tipTimes < this.tipNum -1)
                {
                    this.tipTimes++;
                }
                else
                {
                    this.tipTimes=0;
                }
                this.tipCardValues = this.cardFind.results[this.tipTimes];
            }
        }
        return true;
    },

    //检测选中牌是否可出 ---------------
    checkUpCard:function()
    {
        if(this.upCardValues.length ==0)
        {
            return false;
        }

        var flag = true;
        if(DDZModel.gamePlayType != 1 && !DDZTips.firsttipFlag && DDZModel.spadeoutcart == 1 && DDZModel.maxPlayer == 3)
        {
            for(var k = 0;k<this.upCardValues.length;k++)
            {
                if(this.upCardValues[k] == 51)
                {
                    flag = false;
                    break;
                }
            }

            if(!DDZTips.firsttipFlag && flag)
            {
                return false;
            }
        }

        

        var cards = this.createCards(this.upCardValues);
        var type = libddz.cardsParse.getCardType(cards);

        if(type == libddz.CardsType.CARD_TYPE_ERROR)
        {
            cc.log("checkUpCard type:"+type);
            return false;
        }
        if(type == libddz.CardsType.CARD_TYPE_ONE && DDZModel.nextPlayisSingle == 1)
        {
            //这里要判断是不是出最大的牌
            var flag = false;
            var mycards = this.handCardValues;
            for(var j = 0;j<mycards.length;j++)
            {
                if(libddz.card.compareFace(mycards[j],cards[0]) == 1)
                {
                    flag = true;
                    break;
                }
            }
            if(flag == true)
            {
                return false;
            }
            
        }
        cc.log("checkUpCard checkUpCardcheckUpCard type:"+type);
        /*===添加其它的判断==*/
        if(type == libddz.CardsType.CARD_TYPE_FOURWITHTWO)
        {
            if(!this.checkGameOptionlist(DDZModel.player_option,CARDGAMETYPE.FOURWITHTWO))
            {
                cc.log("checkUpCard type:"+libddz.CardsType.CARD_TYPE_FOURWITHTWO);
                return false;
            }
        }

        if(type == libddz.CardsType.CARD_TYPE_FOURWITHTHREE)
        {
            if(!this.checkGameOptionlist(DDZModel.player_option,CARDGAMETYPE.FOURWITHTHREE))
            {
                cc.log("checkUpCard type:"+libddz.CardsType.CARD_TYPE_FOURWITHTHREE);
                return false;
            }
        }

        if(this.handCards[0].length != cards.length)
        {
            if (type == libddz.CardsType.CARD_TYPE_THREE || type == libddz.CardsType.CARD_TYPE_THREEWITHONE ||
            type == libddz.CardsType.CARD_TYPE_PLANEWITHONE || type == libddz.CardsType.CARD_TYPE_PLANEWHITHLACK)
            {
                cc.log("length-------no------length");
                return false;
            }
        }
        else
        {
            if(type == libddz.CardsType.CARD_TYPE_THREEWITHONE || type ==  libddz.CardsType.CARD_TYPE_THREE)
            {
                if(!this.checkGameOptionlist(DDZModel.player_option,CARDGAMETYPE.THREEWITHONE))
                {
                    cc.log("checkUpCard type:"+libddz.CardsType.CARD_TYPE_THREEWITHONE);
                    return false;
                }
            }

            if(type == libddz.CardsType.CARD_TYPE_PLANEWHITHLACK || type ==  libddz.CardsType.CARD_TYPE_PLANEWITHONE)
            {
                if(!this.checkGameOptionlist(DDZModel.player_option,CARDGAMETYPE.PLANEWITHLESS))
                {
                    cc.log("checkUpCard type:"+libddz.CardsType.CARD_TYPE_PLANEWHITHLACK);
                    return false;
                }
            }
        }


        if (type < libddz.CardsType.CARD_TYPE_FOURWITHONE && !DDZModel.bomb_can_split) {
             var bomb_card_val = [];
             var mycards = utils.clone(this.handCardValues);
             var targetCards = libddz.cards.create(mycards);  
            if (targetCards.card4.length > 0) {
                for (var i = 0; i < targetCards.card4.length; i += 4) {
                    bomb_card_val.push(targetCards.card4[i].face);
                }

                var leng = cards.length;
                for (var j = 0;j<leng;j++) {
                    if (this.checkGameOptionlist(bomb_card_val,cards[j].face)) {
                        return false;
                    }
                }
            }
        }

        

        if(DDZModel.outCardMode == DDZCardState.FOLLOW_CARD)
        {
            var ret = libddz.cardsParse.compareCards(cards,this.outCardValues);
            cc.log("checkUpCard ret:"+ret);
            return ret ==0;
        }
        return true;
    },

    checkGameOptionlist:function(oplist,op)
    {
        var flg = false;
        for(var i = 0 ;i<oplist.length;i++)
        {
            if(oplist[i]==op)
            {
                flg = true;
                break;
            }
        }
        return flg;
    },

    showsplitcardAni:function()
    {
        log("showsplitcardAni");

        // DDZTips.hideTime();
        log("===split_card_after_tstamp===split_card_after_tstamp="+DDZModel.split_card_after_tstamp)
        DDZTips.showSingleTime(DDZModel.split_card_after_tstamp);
        DDZTips.hideClockTO(false);

        DDZAction.showpdkSplitSureBtn(true);

        var startPos = DDZModel.getPosBySeatID(0);
        var card = 0;
        var cardPos = DDZModel.table_type * 3;
        DDZCards.SplitFapai(startPos,card,cardPos);

        DDZAction.pdk_showsplitCardBtn(false);//隐藏按钮 翻牌  提示
        DDZTips.showSplitCardTip(1,true);

         DDZTips.showSilderVis(true);
        DDZTips.showSilder(true,50 - 2);
        DDZModel.sendPdkSplitCard(2,cardPos/2); //切牌 跟 当前选择牌的位置
    },

    hideAllsplitCardTip:function()
    {

        var leng = this.tempCards.length;
        for(var i =0;i<leng;i++)
        {
            this.tempCards[i].reset();
        }
        this.tempCards = [];
        DDZTips.showSplitCardTip(0,false);
        DDZTips.showSplitCardTip(1,false);
        DDZAction.pdk_showsplitCardBtn(false);
        DDZTips.hideTime();
        DDZTips.showSilderVis(false);
        DDZAction.showpdkSplitSureBtn(false);
        DDZModel.spliceCardstate = false;
    },

};

