
var DDZCard = cc.Node.extend(
    {
        uiNode: null,
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
            this.value=-1;
            this.num=0;
            this.reset();
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

            if(is)
            {
                this.setPositionY(this.getPositionY()+15);
            }
            else{

                var suit = this.value>>4;
                if(suit == libddz.Suit.LAIZI)
                {
                    this.setValue(0x4f);
                }
                this.setPositionY(this.getPositionY()-15);
            }
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
        },

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
    mpPos:-1,

    cardFind:null,
    tipCardValues:[],
    tipNum:0,
    tipTimes:-1,

    cardW:138,
    cardH:180,
    dx:56,
    dy:180,
    my:97,
    ocardW:70,
    ocardH:92,
    odx:30,
    ody:45,
    touchBegin:{},
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
            this.showCardValues[i]=[];

        }
        for(var i =0; i < 4;++i)
        {
            this.publicCards[i] =  this.getUI("card"+i);
        }
        var self=this;
        self.touchBegin = cc.p(0,0);
        var listener = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches:false,
            onTouchBegan: function (touch, event) {
                if(DDZModel.isTuoguan == 1 || DDZModel.seatID == -1 || DDZModel.state != DDZState.PLAYING|| DDZModel.mustAll)
                {
                    return false;
                }
                if(!touch.getLocation)
                {
                    return false;
                }
                self.touchBegin = touch.getLocation();
                self.checkTouch(touch);
                return true;
            },
            onTouchMoved: function (touch, event) {
                if(DDZModel.isTuoguan == 1|| DDZModel.seatID == -1|| DDZModel.state != DDZState.PLAYING|| DDZModel.mustAll)
                {
                    return;
                }
                self.checkTouch(touch);
            },
            onTouchEnded: function (touch, event) {
                if(DDZModel.isTuoguan == 1|| DDZModel.seatID == -1|| DDZModel.state != DDZState.PLAYING|| DDZModel.mustAll)
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
            this.showCardValues[i]=[];
        }
        for(var i =0; i < 4;++i)
        {
            this.publicCards[i].setVisible(false);
        }
        this.handCardValues=[];
        this.outCardValues=[];
        this.upCardValues=[];
        this.touchBegin = cc.p(0,0);
        this.setDiZhuPos(-1);
        this.setMingPaiPos(-1);

        this.cardFind.clear();
        this.tipCardValues =[];
        this.tipNum=0;
        this.tipTimes=-1;
        DDZCards.showTuoGuan(false);
    },

    findCard:function(cards,val)
    {
        var suit = val>>4;
        for(var i in cards)
        {
            if(cards[i].value == val)
            {
                return i;
            }
            var s = cards[i].value>>4;
            if((suit ==libddz.Suit.LAIZI &&suit ==s))
            {
                return i;
            }
        }
        return -1;
    },
    deleteCard:function(cards,val)
    {
        var suit = val>>4;
        for(var i in cards)
        {
            var s = cards[i].value>>4;
            if(cards[i].value == val||(suit ==libddz.Suit.LAIZI &&suit ==s))
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
        libddz.cards.sortDown( this.showCardValues[pos]);
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
    },

    checkCardUp:function()
    {
        var needAuto = false;
        if(!DDZModel.isMySeatID(DDZModel.lastSeatID))
        {
            var type = libddz.cardsParse.getCardType(this.outCardValues);
            if( type == libddz.CardsType.CARD_TYPE_TWO
                ||type == libddz.CardsType.CARD_TYPE_THREE )//单张、对子、三张自动处理
            {
                var choose =[];
                var chooseCard =[];
                for(var i  in this.handCards[0])
                {
                    var card  = this.handCards[0][i];
                    if(card.isChoose&&!card.isUp)
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
                var is = ct== libddz.CardsType.CARD_TYPE_SOFT_BOMB
                    || ct== libddz.CardsType.CARD_TYPE_HARD_BOMB
                    || ct== libddz.CardsType.CARD_TYPE_FIVE_BOMB
                    || ct== libddz.CardsType.CARD_TYPE_PLANE_BOMB
                    || ct== libddz.CardsType.CARD_TYPE_PLANE_LAIZI_BOMB
                    ||libddz.CardsType.CARD_TYPE_ROCKET ==ct;
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

        if(!needAuto)
        {
            this.upCardValues =[];
            for(var i  in this.handCards[0])
            {
                var card  = this.handCards[0][i];
                if(card.isChoose)
                {
                    card.setUp(!card.isUp);
                }
                card.setChoose(false);
                if(card.isUp)
                {
                    this.upCardValues.push(card.value);
                }
            }
        }
        DDZAction.checkOutCard();
    },

    showUpCard:function(cs)
    {
        this.initUpCard();
        for(var i  in cs) {
            if(cs[i]&&cs[i].value>0)
            {
                this.upCardValues.push(cs[i].value) ;
            }
        }
        for(var i  in this.handCards[0])
        {
            var card  = this.handCards[0][i];
            for(var j  in this.upCardValues)
            {
                var suit = this.upCardValues[j]>>4;
                var suit1 = card.value>>4;
                if(this.upCardValues[j] == card.value || (suit == libddz.Suit.LAIZI&& suit1 == libddz.Suit.LAIZI))
                {
                    card.setValue(this.upCardValues[j]);
                    card.setUp(true);
                    break;
                }
            }
        }
        DDZAction.checkOutCard();
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
            for (var i = 0 ; i < num; i ++)
            {
                ret.push(cc.p(bx + dx * i, this.my ));
            }
        }
        else if(pos == 1)
        {

            var line_nums = num >10? 9:(num - 1);
            var bx = size.width - 90 - (dx * line_nums + cardw);

            for (var i = 0 ; i < num; i ++)
            {
               var di = i % 10;
                var dj = Math.floor(i/ 10);
                ret.push(cc.p(bx + dx * di, 530 - dy*dj ));
            }
        }
        else if(pos == 2)
        {
            var bx = 160;
            for (var i = 0 ; i < num; i ++)
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
    fapai:function(startPos,cardValue,cardPos)
    {
        this.resetCards();

        if (DDZModel.isMyInTable())
        {
            cardPos =cardPos%3;
            DDZModel.isFaPai = true;
            var dt = 0;
            var cards =[];
            for (var i = 0 ; i < 54; i ++)//牌组叠起
            {
                var card =this.newCard();
                if(i == cardPos+3)
                {
                    card.setValue(cardValue);
                }
                else
                {
                    card.setValue(0);
                }
                card.setScale(0.5);

                card.setPosition(160+0.1*i,380+0.1*i);
                card.setVisible(true);
                card.setLocalZOrder(i);
                card.runAction(cc.sequence(cc.delayTime(dt),cc.show()));
                dt+=0.007;
                cards.push(card);
            }

            dt+=0.1;

            for (var i = 53 ; i >= 0; --i)//牌组平移
            {
                var w = 18*i-0.1*i;
                var card  = cards[i];
                card.setVisible(false);
                card.runAction(cc.sequence(cc.delayTime(dt),cc.moveBy(w/10000, cc.p(w,-0.1*i))));
            }
            dt+=0.2+18*53/10000;
            var size = cc.director.getWinSize();

            var poses ={};
            poses[0] = this.handCardPos(0,17,this.cardW,this.dx,this.dy);
            poses[1] = this.handCardPos(1,17,this.ocardW,this.odx,this.ody);
            poses[2] = this.handCardPos(2,17,this.ocardW,this.odx,this.ody);
            var self =this;
            for (var i = 0 ; i < 51; i ++)//牌组分牌
            {
                var card = cards[53-i];

                var sound_cl = cc.callFunc(function(){
                    Sound.getInstance().playEffect(PackName+"/res/sound/effect/sendpoker.mp3");
                });

                var di = (startPos+i)%3;
                var dj =Math.floor(i/3);

               (function(card,di,dj) {

                   if(di ==1)
                   {
                       cc.pAddIn(poses[di][9],cc.p(-0.3,0.3));
                       poses[di][dj] = poses[di][9];
                   }
                   else if(di == 2)
                   {
                       cc.pAddIn(poses[di][0],cc.p(-0.3,0.3));
                       poses[di][dj] = poses[di][0];
                   }
                    card.runAction(cc.sequence(cc.delayTime(dt), sound_cl, cc.callFunc(function () {
                        card.setLocalZOrder(50);
                    }),cc.moveBy(0.01, cc.p(10, 0)),cc.moveTo(0.17, poses[di][dj]),cc.callFunc(function(){
                        card.setLocalZOrder(dj);
                        if(di == 0)
                        {
                            utils.scaleToSize(card,self.cardW,self.cardH);
                        }
                        else
                        {
                            utils.scaleToSize(card,self.ocardW,self.ocardH);
                        }
                    })));
                })(card,di,dj);
                dt+=0.02;
                this.handCards[di].push(card);
            }
            dt+=0.2;


            var cl = cc.callFunc(function(){

                for(var i = 0 ; i < 3; i ++)//牌组分牌
                {
                    cards[i].setVisible(false);
                }
                DDZCards.showPublicCard(0, 0);
                DDZCards.showPublicCard(1, 0);
                DDZCards.showPublicCard(2, 0);
                DDZCards.showPublicCard(3, 0);
                DDZTips.setRatio(1);
                DDZModel.isFaPai = false;
            });

            this.uiNode.runAction(cc.sequence(cc.delayTime(dt + 0.2),cl));
        }
        else
        {
            this.showFaPaiCard(startPos,cardValue,cardPos);
            DDZTips.setRatio(1);
        }
    },
    //发牌
    showFaPaiCard:function(startPos,cardValue,cardPos)
    {
        this.resetCards();

        var poses ={};
        poses[0] = this.handCardPos(0,17,this.cardW,this.dx,this.dy);
        poses[1] = this.handCardPos(1,1,this.ocardW,this.odx,this.ody);
        poses[2] = this.handCardPos(2,1,this.ocardW,this.odx,this.ody);

        cardPos =cardPos%3;
        for(var i = 0; i < 51;++i)
        {
            var card =this.newCard();
            var di = (startPos+i)%3;
            var dj =Math.floor(i/3);

            card.setVisible(true);
            card.setLocalZOrder(i);
            if(i !=50- cardPos)
            {
                card.setValue(0);
            }
            else
            {
                card.setValue(cardValue);
            }
            if(di == 0)
            {
                card.setPosition(poses[di][dj]);
                utils.scaleToSize(card,this.cardW,this.cardH);
            }
            else
            {
                cc.pAddIn(poses[di][0],cc.p(-0.3,0.3));
                card.setPosition(poses[di][0]);
                utils.scaleToSize(card,this.ocardW,this.ocardH);
            }
            this.handCards[di].push(card);
        }

    },

    //展示手牌
    showHandCards:function(pos,num)
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
        else  if(DDZModel.isOnVideo|| this.mpPos == pos)
        {
            values = this.showCardValues[pos];
            l =values.length;

            if(l ==0)
            {
                l =  num;
            }
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
                }
                if(num<=2)
                {
                    card.setValue(-1);
                    card.setNum(num);
                }
                else
                {
                    card.setValue(0);
                }
            }
            else
            {
                card.setPosition(poses[i]);
                card.setValue(values[i].value);
            }
            this.handCards[pos].push(card);
            if(this.dzPos == pos)
            {
                card.setDizhu(i==l-1);
            }
            if(this.mpPos == pos &&  pos == 0)
            {
                card.setMingpai(i==l-1);
            }

        }

        if(DDZModel.isMyInTable() && pos == 0) {
            var w = this.handCards[0].length * this.dx + this.cardW - this.dx;
            if (w < 200) {
                w = 200;
            }
            this.tuoguanBg.setContentSize(cc.size(w, this.cardH));
            this.qxtuoguanBtn.setPosition(w * 0.5,this.my);
            this.tuoguanBg.setPosition(cc.director.getWinSize().width * 0.5,this.my);
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
        }

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
            var line_nums = (l - 1);
            obx = size.width / 2 - this.odx * line_nums / 2;
            oby = 230;

        }

        for(var i  in this.outCards[pos])
        {
            this.outCards[pos][i].reset();
        }
        this.outCards[pos] =[];

        if(DDZModel.isMyInTable()&&pos == 0)//我方主动出牌时
        {
            oby = 250;
            for(var i= 0,len= this.handCards[0].length;i<len;++i)
            {
                var card = this.handCards[0][i];
                var index =this.findCard(this.outCardValues,card.value);
                if(index > -1)
                {
                    if(this.isLaiZi(card.value))
                    {
                        card.setValue(this.outCardValues[index].value);
                    }

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
                card.setMingpai(false);
                if(this.dzPos == pos)
                {
                    card.setDizhu(i==l-1);
                }
                (function(z,c)
                {
                    var  func = cc.callFunc(function () {
                        c.setLocalZOrder(z);
                    });
                    c.runAction(cc.spawn(cc.moveTo(0.17, cc.p(obx + i * self.odx, oby))
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
                card.setLocalZOrder(i);
                card.setPosition(obx + i * this.odx , oby );
                utils.scaleToSize(card,this.ocardW,this.ocardH);

                card.setValue(values[i].value);
                this.outCards[pos].push(card);
                if(this.dzPos == pos)
                {
                    card.setDizhu(i==l-1);
                }
            }
        }
    },
    //展示公共牌
    showPublicCard:function(pos,value)
    {
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

    },
    //初始化选中的牌
    initUpCard:function()
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
        this.mpPos = pos;
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
    //提示出牌
    tipCard:function()
    {
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
                if(this.tipTimes < this.cardFind.results.length -1)
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
    checkOneBS:function()
    {
        if(!libddz.cardsFind.splitRocket)
        {
            var b =false;
            var s =false;
            for(var i in this.handCardValues) {

                if(this.handCardValues[i].value == 14)
                {
                    s =true;
                }
                if(this.handCardValues[i].value == 15)
                {
                    b =true;
                }
            }
            if(b&&s)
            {
                return false;
            }
        }
        return true;
    },
    //检测选中牌是否可出
    checkUpCard:function()
    {
        if(this.upCardValues.length ==0)
        {
            return false;
        }

        var cards = this.createCards(this.upCardValues);
        var parse = libddz.cardsParse.getCardParse(cards);
        this.checkLaiZi(parse);
        if(parse.type == libddz.CardsType.CARD_TYPE_ERROR)
        {
            return false;
        }
        if(parse.type == libddz.CardsType.CARD_TYPE_ONE && parse.face >= libddz.Face.SMALL &&!this.checkOneBS())
        {
            return false;
        }
        if(DDZModel.outCardMode == DDZCardState.FOLLOW_CARD)
        {
            var ret = libddz.cardsParse.compareCards(cards,this.outCardValues);
            cc.log("checkUpCard ret:"+ret);
            return ret ==0;
        }
        return true;
    },
    isLaiZi:function(val)
    {
        var suit = val>>4;
        return suit == libddz.Suit.LAIZI;
    },
    checkLaiZi:function(parse)
    {
        for(var i in this.handCards[0])
        {
            var suit = this.handCards[0][i].value>>4;
            if(suit == libddz.Suit.LAIZI)
            {
                if(parse.laizi != 0)
                {
                    this.handCards[0][i].setValue(parse.laizi);

                }
                else
                {
                    this.handCards[0][i].setValue(0x4f);
                }
                break;
            }
        }
    },
    initLaiZi:function()
    {
        for(var i in this.handCards[0])
        {
            var suit = this.handCards[0][i].value>>4;
            if(suit == libddz.Suit.LAIZI)
            {
                this.handCards[0][i].setValue(0x4f);
            }
        }
    }


};

