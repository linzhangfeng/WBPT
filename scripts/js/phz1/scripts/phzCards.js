
var INVAIL_CARD =255;
var PHZCard0 = cc.Node.extend(
    {
        reset:function()
        {
            this.setRotation(0);
            this.setOpacity(255);
            this.state=0;
            this.setVisible(false);
            this.setState(0);
            this.curPos =cc.p(0,0);
            this.beganPos =cc.p(0,0);
            this.beganTime = 0;
            this.zorder =0;
            this.clickState = false;
            this.value=INVAIL_CARD;
            this.setClick(false);
            this.clickNode.setVisible(false);
            this.getUI("dun").setVisible(false);
            this.getUI("out").setVisible(false);
            this.stopAllActions();
        },
        ctor:function ()
        {
            this._super();
            var json = ccs.load(PackName+"/res/card0.json");
            this.uiNode =json.node;
            this.addChild(this.uiNode);
            this.setContentSize(this.uiNode.getContentSize());
            this.setAnchorPoint(cc.p(0.5,0.5));
            this.clickNode = this.getUI("click");
            this.clikCB=null;
            this.clik2CB=null;
            this.endCB=null;
            this.moveCB=null;
            this.setCascadeOpacityEnabled(true);
            this.reset();

        },
        hit:function(pos)
        {
            return this.getUI("btn").hitTest(pos,cc.Camera.getVisitingCamera(),null);
        },
        setClick:function(is)
        {
            this.clickState =is;
            this.clickNode.setVisible(is);
            if(is)
            {
                this.setScale(1.2);
            }
            else{
                this.setScale(1);
            }
        },
        showDun:function(is)
        {
            this.getUI("dun").setVisible(is);
        },
        showOut:function(is)
        {
            this.getUI("out").setVisible(is);
        },
        getUI:function(key)
        {
            return  utils.findNode(this.uiNode,key);
        },
        showUI:function(key ,is)
        {
            this.getUI(key).setVisible(is);
        },
        //设置牌点数
        setValue:function(value)
        {
            if(!value ||  this.value == value) return;

            this.value = value;
            if(INVAIL_CARD != value)
            {
                this.getUI("face0").loadTexture("phz/card_b_"+utils.toHex2(value)+".png",1);
                this.getUI("face1").loadTexture("phz/card_b_"+utils.toHex2(value)+".png",1);
            }
        },
        setState:function(s)
        {
            this.state=s;
            this.getUI("bg0").setVisible(false);
            this.getUI("bg1").setVisible(false);
            this.getUI("mask").setVisible(false);
            if(s == 0)
            {
                this.getUI("bg0").setVisible(true);
            }
            else if(s==1)
            {
                this.getUI("bg1").setVisible(true);
            }
            else if(s==2)
            {
                this.getUI("bg0").setVisible(true);
                this.getUI("mask").setVisible(true);
            }
        }

    });

var PHZCard1 = cc.Node.extend(
    {
        ctor:function ()
        {
            this._super();
            var json = ccs.load(PackName+"/res/card1.json");
            this.uiNode =json.node;
            this.addChild(this.uiNode);
            this.setContentSize(this.uiNode.getContentSize());
            this.setState(0);
            this.setCascadeOpacityEnabled(true);
            this.reset();
        },

        getUI:function(key)
        {
            return  utils.findNode(this.uiNode,key);
        },
        showUI:function(key ,is)
        {
            this.getUI(key).setVisible(is);
        },
        //设置牌点数
        setValue:function(value)
        {
            if(!value ||  this.value == value) return;

            this.value = value;
            if(INVAIL_CARD != value)
            {
                this.getUI("face").loadTexture("phz/card_s_"+utils.toHex2(value)+".png",1);
            }
        },
        reset:function()
        {
            this.value =INVAIL_CARD;
            this.setRotation(0);
            this.setOpacity(255);
            this.setVisible(false);
            this.state=0;
            this.stopAllActions();
        },
        setState:function(s)
        {
            this.state=s;
            this.getUI("bg0").setVisible(false);
            this.getUI("bg1").setVisible(false);
            this.getUI("mask").setVisible(false);
            if(s == 0)
            {
                this.getUI("bg0").setVisible(true);
            }
            else if(s==1)
            {
                this.getUI("bg1").setVisible(true);
            }
            else if(s==2)
            {
                this.getUI("bg0").setVisible(true);
                this.getUI("mask").setVisible(true);
            }
        }

    });
var PHZCard2 = cc.Node.extend(
    {
        reset:function()
        {
            this.setRotation(0);
            this.setOpacity(255);
            this.state=0;
            this.setVisible(false);
            this.setState(0);
            this.curPos =cc.p(0,0);
            this.beganPos =cc.p(0,0);
            this.beganTime = 0;
            this.zorder =0;
            this.clickState = false;
            this.value=INVAIL_CARD;
            this.setClick(false);
            this.clickNode.setVisible(false);
            this.stopAllActions();
            this.ableTouch = true;
            this.moveState =false;
            this.isBegan = false;
        },
        ctor:function ()
        {
            this._super();
            var json = ccs.load(PackName+"/res/card2.json");
            this.uiNode =json.node;
            this.addChild(this.uiNode);
            this.setContentSize(this.uiNode.getContentSize());
            this.setAnchorPoint(cc.p(0.5,0.5));
            this.clickNode = this.getUI("click");
            this.clikCB=null;
            this.clik2CB=null;
            this.endCB=null;
            this.moveCB=null;
            this.setCascadeOpacityEnabled(true);
            this.reset();
            var r =  cc.director.getWinSize().width/80;
            r*=r;

            var self = this;
            this.getUI("btn").addTouchEventListener(function(sender,type){
                switch (type) {
                    case ccui.Widget.TOUCH_BEGAN:
                    {
                        if (self.state != 0 || PHZModel.state != PHZState.GAME_PLAYING||!self.ableTouch) {
                            return;
                        }

                        self.zorder = self.getLocalZOrder();
                        self.curPos = self.getPosition();
                        self.beganPos = sender.getTouchBeganPosition();
                        if (self.clikCB) {
                            self.clikCB(self);
                        }
                        if ( utils.getTimeStamp() - self.beganTime < 500) {

                            if (self.clik2CB) {
                                self.clik2CB(self);
                            }
                        }
                        self.moveState =false;
                        self.beganTime = utils.getTimeStamp();
                        self.isBegan = true;
                    }
                        break;
                    case ccui.Widget.TOUCH_MOVED:
                    {
                        if( self.state !=0|| PHZModel.state != PHZState.GAME_PLAYING||!self.ableTouch||!self.isBegan){
                            return ;
                        }
                        var p1 = sender.getTouchMovePosition();
                        var pos =utils.clone(self.curPos);
                        var dis =utils.subPP(p1,self.beganPos);
                        if(dis.x*dis.x+dis.y*dis.y >r)
                        {
                            self.moveState =true;
                        }
                        if( !self.moveState)
                        {
                            return;
                        }
                        pos.x += dis.x;
                        pos.y += dis.y;
                        self.setPosition(self.getParent().convertToNodeSpace(pos));
                        self.setLocalZOrder(1000);
                        self.clickNode.setVisible(false);
                        if(PHZModel.needOutCard)
                        {
                            PHZCards.showOutLine(true);
                        }
                        if(self.moveCB)
                        {
                            self.moveCB(self);
                        }
                    }
                        break;
                    case ccui.Widget.TOUCH_ENDED:
                    case ccui.Widget.TOUCH_CANCELED:
                    {
                        PHZCards.actionCardMove.setVisible(false);
                        if(self.state !=0|| PHZModel.state != PHZState.GAME_PLAYING||!self.ableTouch||!self.isBegan||!self.isVisible()){
                            return ;
                        }
                        self.isBegan = false;
                        self.clickNode.setVisible(self.clickState);

                        self.setClick(self.clickState);
                        PHZCards.showOutLine(false);
                        if(self.endCB)
                        {
                            self.endCB(self,sender.getTouchEndPosition());
                        }
                        /*self.setLocalZOrder(self.zorder);
                         self.setPosition(self.curPos);*/
                    }
                        break;
                }
            });
        },
        hit:function(pos)
        {
            return this.getUI("btn").hitTest(pos,cc.Camera.getVisitingCamera(),null);
        },
        setClick:function(is)
        {
            this.clickState =is;
            this.clickNode.setVisible(is);
          /*  if(is)
            {
                this.setScale(1.2);
            }
            else{
                this.setScale(1);
            }*/
        },
        getUI:function(key)
        {
            return  utils.findNode(this.uiNode,key);
        },
        showUI:function(key ,is)
        {
            this.getUI(key).setVisible(is);
        },
        //设置牌点数
        setValue:function(value)
        {
            if(!value ||  this.value == value) return;

            this.value = value;
            if(INVAIL_CARD != value)
            {
                this.getUI("face").loadTexture("phz/card_b_"+utils.toHex2(value)+".png",1);
            }
        },
        setState:function(s)
        {
            this.state=s;
            this.getUI("bg0").setVisible(false);
            this.getUI("mask").setVisible(false);
            if(s == 0)
            {
                this.getUI("bg0").setVisible(true);
            }
            else if(s==2)
            {
                this.getUI("bg0").setVisible(true);
                this.getUI("mask").setVisible(true);
            }
        }

    });
var PHZChiCards = cc.Node.extend(
    {
        ctor:function ()
        {
            this._super();
            var json = ccs.load(PackName+"/res/chiCards.json");
            this.uiNode =json.node;
            this.addChild(this.uiNode);
            this.setContentSize(this.uiNode.getContentSize());
            this.setAnchorPoint(cc.p(0.5,0.5));

            this.click = this.getUI("click");
            var self = this;
           this.getUI("btn").addClickEventListener(function(sender,type){

                if(self.clickCB)
                {
                    self.clickCB(self.clickCBTarget,self.kind);
                }
            });
            this.reset();
        },
        reset:function()
        {
            this.setScale(0.6);
            this.setClick(false);
            this.kind =0;
            this.cards = [];
            this.stopAllActions();
        },
        getUI:function(key)
        {
            return  utils.findNode(this.uiNode,key);
        },
        showUI:function(key ,is)
        {
            this.getUI(key).setVisible(is);
        },
        setClick:function(is)
        {
            this.click.setVisible(is);
            this.getUI("btn").setEnabled(!is);
        },
        setCards:function(cards)
        {
            for(var i in cards)
            {
                if(i>=3)
                {
                    break;
                }
                this.getUI("face"+i).loadTexture("phz/card_b_"+utils.toHex2(cards[i])+".png",1);
            }
        }

    });

var PHZCards =
{
    uiNode: null,
    uiAction: null,
    cardPool0: [],
    cardPool1: [],
    cardPool2:[],
    outLine: null,
    newbie: null,

    dunCard: null,
    dunPool: [],

    handCards: [],

    videoHandCardsVal: [],
    videoHandCards: [],

    showCards: [],
    historyCards: [],

    handCardsVal: [],
    showCardsVal: [],
    historyCardsVal: [],

    publicCard: [],
    actionCard: [],
    outY: 0,
    chiVal: [],
    init: function () {
        if (this.uiNode == null) {
            return;
        }
        this.getUI("dunNum").ignoreContentAdaptWithSize(true);
        this.getUI("dunNum").setLocalZOrder(101);
        this.dunCard = this.getUI("dunCard");

        this.ting = this.getUI("ting");
        this.tingCards = [];
        this.tingCards.push(this.ting.getChildByName("txt"));
        this.ting.setLocalZOrder(101);
        this.dunPos = this.getUI("dunNum").getPosition();
        for (var i = 22; i >= 0; --i) {
            var card = this.newDunCard();
            card.setVisible(true);
            card.setPosition(this.dunCard.getPosition().x, this.dunCard.getPosition().y - 1 * i);
            card.setLocalZOrder(100 - i);
        }
        this.dunPool.push(this.dunCard);
        this.newbie = utils.findNode(this.uiNode.getParent(), "newbie");
        this.outLine = utils.findNode(this.uiNode.getParent(), "outLine");
        this.outY = this.outLine.getPositionY();
        for (var i = 0; i < 5; ++i) {
            this.newCard0();
            this.newCard1();
            this.newCard2();
        }

        this.actionCardMove = new PHZCard2();
        this.uiNode.addChild( this.actionCardMove );
        for (var i = 0; i < 4; ++i) {
            this.publicCard.push(new PHZCard0());
            this.uiNode.addChild(this.publicCard[i], 100);
        }
        for (var i = 0; i < 4; ++i) {
            this.newActionCard();
        }

        this.reset();

    },
    reset: function () {
        this.ting.setVisible(false);
        for (var i in this.publicCard) {
            this.publicCard[i].reset();
            this.publicCard[i].setVisible(false);
        }
        for (var i in this.actionCard) {
            this.actionCard[i].reset();
        }
        this.actionCardMove.setVisible(false);
        this.showUI("dunBg", false);
        this.showNewbie(false);
        this.outLine.setVisible(false);
        for (var i in this.cardPool0) {
            this.cardPool0[i].reset();
        }
        for (var i in this.cardPool1) {
            this.cardPool1[i].reset();
        }
        for (var i in this.cardPool2) {
            this.cardPool2[i].reset();
        }
        this.handCards = [];
        for (var i = 0; i < 4; ++i) {
            this.showCards[i] = [];
            this.historyCards[i] = [];
            this.historyCardsVal[i] = [];
            this.showCardsVal[i] = [];
            this.videoHandCardsVal[i] = [];
            this.videoHandCards[i] = [];
        }
    },
    getUI: function (key) {
        return utils.findNode(this.uiNode, key);
    },
    showUI: function (key, is) {
        this.getUI(key).setVisible(is);
    },
    showNewbie: function (is) {
        this.newbie.setVisible(is);
        this.uiAction.gotoFrameAndPause(0);
        if (is) {
            this.uiAction.play("newbie", true);
        }
    },
    showOutLine: function (is) {
        this.outLine.setVisible(is);
    },

    onCardClick: function (card) {
        for (var i in PHZCards.handCards) {
            var d = PHZCards.handCards[i];
            if (d != card) {
                d.setClick(false);
            }
            else {
                d.setClick(!d.clickState);
            }
        }
        if (card.clickState) {
            PHZCards.clickCard = card;
        }
        else {
            PHZCards.clickCard = null;
        }
    },
    checkOut:function(card,isV)
    {
            var is = true;
            if(is)
            {
                var ds =[];
                for(var i in PHZCards.handCardsVal)
                {
                    for(var j in PHZCards.handCardsVal[i]) {
                        if (PHZCards.handCardsVal[i][j] != card.value) {
                            ds.push(PHZCards.handCardsVal[i][j]);
                        }
                    }

                }
                if(!(ds.length == 2 && ds[0]==ds[1]))
                {
                    is = false;
                }
            }
        if(is)
        {
            for(var j in PHZCards.showCardsVal[0])
            {
                var kind = PHZCards.showCardsVal[0][j]["weave_kind"];
                if(kind ==PHZAck.ACK_CHI )
                {
                    is = false;
                    break;
                }
            }
        }
        if(is)
        {
            var box = new PHZTipBox();
            box.card = card;
            PHZCards.uiNode.getParent().addChild(box, 1000);
            card.setLocalZOrder(card.zorder);
            card.setPosition(card.curPos);
            return false;
        }
        if(isV)
        {
            PlatformHelper.vibrate();
        }
        PHZCards.clickCard = card;
        PHZModel.sendOutCard(card.value, card.row, card.col);
        this.preOutMyCard(card);
        return true;
    },
    onCardClick2: function (card) {
        if (PHZModel.needOutCard) {
            PHZCards.clickCard = card;

            PHZCards.checkOut(card,true);
        }
    },
    onCardClickEnd: function (card, pos) {

        if(PHZCards.actionCardMove)
        {
            PHZCards.actionCardMove.setVisible(false);
        }
        if ( card.getParent().convertToNodeSpace(pos).y > PHZCards.outY) {

            if(PHZModel.needOutCard)
            {
                PHZCards.checkOut(card);
            }
            else
            {
                PHZTips.showMyTip("还没有到您出牌");

                card.setLocalZOrder(card.zorder);
                if(card.moveState)
                {
                    card.ableTouch = false;
                    card.runAction(cc.sequence(cc.moveTo(0.2,card.curPos),cc.callFunc(function(){ card.ableTouch=true;})));
                }
                else
                {
                    card.runAction(cc.moveTo(0.2,card.curPos));
                }
            }
        }
        else {
            var cur = card.getPosition();
            card.setLocalZOrder(card.zorder);
            card.setPosition(card.curPos);
            var base = JSON.stringify(PHZCards.handCardsVal);
            var toRow = -1;
            var toCol = -1;
            //检测是否移动到手牌中
            for (var i in PHZCards.handCards) {
                var d = PHZCards.handCards[i];
                if (d.hit(pos)) {
                    toRow = d.row;
                    toCol = d.col;
                    break;
                }
            }
            var cPos = card.getParent().convertToNodeSpace(pos);
            if (toRow == -1) {
                //检测是否添加到手牌最上方
                for (var i = PHZCards.handCards.length - 1; i >= 0; --i) {
                    var d = PHZCards.handCards[i];
                    var w = d.getContentSize().width;
                    var start = d.getPosition().x - w * d.getAnchorPoint().x;
                    var end = start + w;
                    if (cPos.x >= start && cPos.x <= end) {
                        toRow = d.row;
                        toCol = d.col;
                        if (d != card) {
                            toRow += 1;
                            if (d.row > 3) {
                                toRow = 4;
                            }
                        }
                        break;
                    }
                }
            }
            if (toRow == -1) {
                toRow = 0;
                toCol = -1;
                //检测是否添加到手牌左右边
                if (PHZCards.handCards.length > 0) {
                    var d = PHZCards.handCards[0];
                    var w = d.getContentSize().width;
                    var start = d.getPosition().x - w * d.getAnchorPoint().x;
                    if (start < cPos.x) {
                        toCol = PHZCards.handCardsVal.length;
                    }
                }

            }
            if (!(PHZCards.handCardsVal.length >= 11 && ( toCol == -1 || toCol == PHZCards.handCardsVal.length))) {
                var isB = PHZCards.moveHandCard(card.row, card.col, toRow, toCol);
                card.setPosition(cur);
                if (JSON.stringify(PHZCards.handCardsVal) != base) {
                    PHZCards.moveHandCards(card,isB);
                    PHZModel.sendChangeCard(PHZCards.handCardsVal);
                }
                else
                {
                    if(card.moveState)
                    {
                        card.ableTouch = false;
                        card.runAction(cc.sequence(cc.moveTo(0.2,card.curPos),cc.callFunc(function(){ card.ableTouch=true;})));
                    }
                    else
                    {
                        card.runAction(cc.moveTo(0.2,card.curPos));
                    }
                }
            }
            else
            {
                card.setPosition(cur);
                if(card.moveState)
                {
                    card.ableTouch = false;
                    card.runAction(cc.sequence(cc.moveTo(0.2,card.curPos),cc.callFunc(function(){ card.ableTouch=true;})));
                }
                else
                {
                    card.runAction(cc.moveTo(0.2,card.curPos));
                }
            }
        }
    },
    onCardClickMove:function(card)
    {
        //检测是否移动到手牌中
        /*for (var i in PHZCards.handCards) {
            var d = PHZCards.handCards[i];
            if (d!=card &&d.hit(pos)) {
                cc.log(i);
                card.setLocalZOrder(d.getLocalZOrder());
                return;
            }
        }
        card.setLocalZOrder(0);*/

        var temp =   PHZCards.actionCardMove ;
        temp.setPosition( card.curPos);
        temp.setOpacity(150);
        temp.setVisible(true);
        temp.setLocalZOrder(card.zorder);
        temp.setValue(card.value);
    },
    moveHandCard: function (row, col, toRow, toCol) {
        if (row == toRow && col == toCol) {
            return false;
        }
        var dat = this.handCardsVal[col][row];
        if (toCol == -1) {
            this.handCardsVal[col].splice(row, 1);
            if (this.handCardsVal[col].length == 0) {
                this.handCardsVal.splice(col, 1);
            }
            for (var i in this.handCardsVal[col]) {
                this.handCardsVal[col][i].row = Number(i);
            }
            this.handCardsVal.splice(0, 0, []);
            this.handCardsVal[0].push(dat);

            if(this.handCardsVal.length>1)
            {
                return this.handCardsVal[1][0] ==dat;
            }
            return false;
        }
        else if (toCol == PHZCards.handCardsVal.length) {
            this.handCardsVal.push([]);
            this.handCardsVal[toCol].push(dat);
            this.handCardsVal[col].splice(row, 1);
            if (this.handCardsVal[col].length == 0) {
                this.handCardsVal.splice(col, 1);
            }
            return false;
        }
        var cards = this.handCardsVal[toCol];
        if (cards && cards.length == 3 && cards[0] == cards[1] && cards[1] == cards[2]) {
            return false;
        }
        if (col != toCol) {
            if (cards.length == 4) {
                return false;
            }
            this.handCardsVal[toCol].splice(toRow, 0, dat);
            this.handCardsVal[col].splice(row, 1);

            var isN = true;
            var is = false;
            if(toRow+1<this.handCardsVal[toCol].length)
            {
                isN = false;
                is = this.handCardsVal[toCol][toRow+1] ==dat;
            }
            if (this.handCardsVal[col].length == 0) {
                this.handCardsVal.splice(col, 1);

                if(isN)
                {
                    if(toCol > col)
                    {
                        if(toCol < this.handCardsVal.length)
                        {
                            is = this.handCardsVal[toCol][0] ==dat;
                        }
                    }
                    else if(toCol+1 < this.handCardsVal.length)
                    {
                        is = this.handCardsVal[toCol+1][0] ==dat;
                    }
                }
            }
            else
            {
                if(isN)
                {
                    if(toCol+1 < this.handCardsVal.length)
                    {
                        is = this.handCardsVal[toCol+1][0] ==dat;
                    }
                }
            }
            return is;
        }
        else {
            if (toRow > row) {
                this.handCardsVal[col].splice(toRow + 1, 0, dat);
                this.handCardsVal[col].splice(row, 1);

                if(toRow + 1>=this.handCardsVal[col].length )
                {
                    if(this.handCardsVal.length>col+1)
                    {
                        return this.handCardsVal[col+1][0] ==dat;
                    }
                }
                return this.handCardsVal[col][toRow+1] ==dat;
            }
            else {
                this.handCardsVal[col].splice(toRow, 0, dat);
                this.handCardsVal[col].splice(row + 1, 1);

                return this.handCardsVal[col][toRow+1] ==dat;
            }

        }
        return false;
    },
    newChiCards: function (kind, cards) {
        var ret = new PHZChiCards();
        ret.kind = kind;
        ret.setCards(cards);
        return ret;
    },
    newCard0: function () {
        for (var i in this.cardPool0) {
            if (!this.cardPool0[i].isVisible()) {
                this.cardPool0[i].reset();
                return this.cardPool0[i];
            }
        }

        var card = new PHZCard0();
        this.uiNode.addChild(card);
        this.cardPool0.push(card);
        return card;
    },
    newActionCard: function () {
        for (var i in this.actionCard) {
            if (!this.actionCard[i].isVisible()) {
                this.actionCard[i].reset();
                return this.actionCard[i];
            }
        }
        var card = new PHZCard0();
        this.uiNode.addChild(card, 150);
        this.actionCard.push(card);
        return card;
    },
    newCard1: function () {
        for (var i in this.cardPool1) {
            if (!this.cardPool1[i].isVisible()) {
                this.cardPool1[i].reset();
                return this.cardPool1[i];
            }
        }

        var card = new PHZCard1();
        this.uiNode.addChild(card);
        this.cardPool1.push(card);
        return card;
    },

    newCard2: function () {
        for (var i in this.cardPool2) {
            if (!this.cardPool2[i].isVisible()) {
                this.cardPool2[i].reset();
                return this.cardPool2[i];
            }
        }

        var card = new PHZCard2();
        card.clikCB = this.onCardClick;
        card.clik2CB = this.onCardClick2;
        card.endCB = this.onCardClickEnd;
        card.moveCB = this.onCardClickMove;
        this.uiNode.addChild(card);
        this.cardPool2.push(card);
        return card;
    },

    newDunCard: function () {
        for (var i in this.dunPool) {
            if (!this.dunPool[i].isVisible()) {
                return this.dunPool[i];
            }
        }

        var card = new cc.Sprite(this.dunCard.getSpriteFrame());
        this.getUI("dunBg").addChild(card);
        this.dunPool.push(card);
        return card;
    },
    showDunCards: function (num) {
         for(var i in this.dunPool)
         {
            this.dunPool[i].setVisible(false);
         }
        if (num < 0) {
            this.showUI("dunBg", false);
            return;
        }

        for(var i  =0;i<num;++i)
        {
            this.dunPool[i].setVisible(true);
        }
        this.showUI("dunBg", true);
        if(num ==0)
        {
            this.getUI("dunNum").setString("" );
        }
        else{
            this.getUI("dunNum").setString(num);
        }
        this.getUI("dunNum").setPositionY(this.dunPos.y-1*(23-num));
    },

    getHandCardsPos: function (sxl, xl, yl) {
        var ret = [];
        var start = cc.p(640 - xl * 82 * 0.5, 48);
        for (var i = sxl; i < xl; ++i) {
            ret[i] = [];
            var x = start.x + 82 * i;
            for (var j = 0; j < yl; ++j) {
                ret[i][j] = cc.p(x, start.y + 75 * j);
            }
        }
        return ret;
    },
    moveHandCards:function(c,isB)
    {
        var vals = [];
        for (var i in this.handCardsVal) {
            var cards = this.handCardsVal[i];
            for (var j in cards) {
                vals.push(cards[j]);
            }
        }
        for(var i = 0;i<vals.length;++i)
        {
            if(this.handCards[i] == c)
            {
                this.handCards.splice(i,1);
                break;
            }
        }
        var isOver = false;
        for(var i = 0;i<this.handCards.length;++i)
        {
            if(this.handCards[i].value != vals[i])
            {
                isOver = true;

                if(i>0&&isB&&this.handCards[i-1].value== c.value)
                {
                    this.handCards.splice(i-1,0,c);
                }
                else
                {
                    this.handCards.splice(i,0,c);
                }
                break;
            }
        }
        if(!isOver)
        {
            if(isB&&this.handCards[this.handCards.length-1].value== c.value)
            {
                this.handCards.splice(this.handCards.length-1,0,c);
            }
            else
            {
                this.handCards.push(c);
            }
        }

        var n= 0;
        var poses = this.getHandCardsPos(0, this.handCardsVal.length, 4);
        for (var i in this.handCardsVal) {
            var cards = this.handCardsVal[i];
            var isKan = (cards.length == 3 && cards[0] == cards[1] && cards[1] == cards[2])||
                (cards.length == 4&& cards[0] == cards[1] && cards[1] == cards[2]&& cards[2] == cards[3]);

            for (var j in cards) {
                var card = this.handCards[n];
                card.setVisible(true);
                card.setLocalZOrder(i * 10 + 4 - j);
                card.col = Number(i);
                card.row = Number(j);
                if (isKan) {
                    card.setState(2);
                }
                else {
                    card.setState(0);
                }
                n++;
                (function(card,p){
                    card.runAction(cc.sequence(cc.moveTo(0.2,p),cc.callFunc(function(){ card.setClick(false);})));
                })(card,poses[i][j]);
            }
        }
    },
    showHandCards: function () {
        for (var i  in this.handCards) {
            this.handCards[i].reset();
        }
        this.handCards = [];

        var poses = this.getHandCardsPos(0, this.handCardsVal.length, 4);
        for (var i in this.handCardsVal) {
            var cards = this.handCardsVal[i];
            var isKan = (cards.length == 3 && cards[0] == cards[1] && cards[1] == cards[2])||
                (cards.length == 4&& cards[0] == cards[1] && cards[1] == cards[2]&& cards[2] == cards[3]);

            for (var j in cards) {
                var card = this.newCard2();
                card.setPosition(poses[i][j]);
                card.setVisible(true);
                this.handCards.push(card);
                card.setValue(cards[j]);
                card.setLocalZOrder(i * 10 + 4 - j);
                card.col = Number(i);
                card.row = Number(j);
                if (isKan) {
                    card.setState(2);
                }
                else {
                    card.setState(0);
                }
            }
        }
    },

    getShowCardsPos: function (pos, sxl, xl, yl) {
        var ret = [];
        var start = cc.p(120, 10);
        var dx = 1;
        var dy =1;
        switch (pos) {
            case 1:
                dx = -1;
                start.x = 1260 -  40;
                start.y = 340;
                break;
            case 2:
                dx = 1;
                dy = -1;
                start.x = 680;
                start.y = 710-40;
                break;
            case 3:
                start.x = 20;
                start.y = 340;
                break;
        }
        for (var i = sxl; i < xl; ++i) {
            ret[i] = [];
            var x = start.x + 40 * i*dx;
            for (var j = 0; j < yl; ++j) {
                ret[i][j] = cc.p(x, start.y + 38 * j*dy);
            }
        }
        return ret;
    },
    showShowCards: function (pos,hideTi) {
        for (var i  in  this.showCards[pos]) {
            this.showCards[pos][i].reset();
        }
        this.showCards[pos] = [];


        var poses = this.getShowCardsPos(pos,0, this.showCardsVal[pos].length, 4);
        var self = this;
        var func = function (i, j, cardPos,card_list,count,type) {
            var card = self.newCard1();
            card.setPosition(cardPos);
            card.pos = cardPos;
            card.setVisible(true);
            self.showCards[pos].push(card);

            if(type == PHZAck.ACK_TI)
            {
                if(!hideTi && ((j==count - 1 && pos!=2)||(j==0 && pos==2)) )
                {
                    card.setState(0);
                    card.setValue(card_list[0]);
                }
                else {
                    card.setState(1);
                }
            }
            else if (type == PHZAck.ACK_WEI || type == PHZAck.ACK_CW) {
                if((card_list[0] != INVAIL_CARD || type == PHZAck.ACK_CW) && ((j==count - 1 && pos!=2)||(j==0 && pos==2)) )
                {
                    card.setState(0);
                    card.setValue(card_list[0]);
                }
                else {
                    card.setState(1);
                }
            }
            else {
                if (type == PHZAck.ACK_CHI&&((j==2 && pos!=2)||(j==0 && pos==2)))
                {
                    card.setState(2);
                }
                else
                {
                    card.setState(0);
                }
                card.setValue(card_list[j]);
            }
        };
        for (var i in this.showCardsVal[pos]) {
            var weaves = this.showCardsVal[pos][i];
            var type = weaves["weave_kind"];
            var count = weaves["card_list"].length;
            for (var j  in weaves["card_list"]) {
                func(i, j, poses[i][j],weaves["card_list"],count,type);
            }
        }
    },
    showAckCards: function (pos, cs,val) {
        var poses = [cc.p(320, 10), cc.p(1100, 340), cc.p(800, 710-40), cc.p(280, 340)];
        var card = null;
        for (var i = 0; i < 4; ++i) {
            if (this.publicCard[i].isVisible()&&(this.publicCard[i].value == val||this.publicCard[i].value ==INVAIL_CARD) ) {
                this.publicCard[i].setVisible(false);
                var dat =  this.getOutCardPos(i);
                var p=dat.p;
                var r =dat.r;
                card = PHZCards.newActionCard();
                card.setPosition(p);
                card.setValue(this.publicCard[i].value);
                card.setState(this.publicCard[i].state);
                card.setVisible(true);
                card.setScale(1);
                card.setRotation(r);
                break;
            }
        }
        var dt = 0;
        var p1 = cc.p(0, 0);
        if (card) {
            dt = 0.3;
        }
        var len = 0;
        for (var i in this.showCardsVal[pos]) {

            for(var z in cs)
            {
                if(cs[z] ==i)
                {
                    for (var j = 0; j < this.showCardsVal[pos][i]["card_list"].length; ++j)
                    {
                        var d = len+j;

                        var c = this.showCards[pos][d];
                        c.setOpacity(0);

                        if (card && (card.value == c.value || (card.state == c.state && c.state == 1))) {

                            if(pos==2)
                            {
                                p1 =utils.addPP( poses[pos],cc.p(0,-38*j));
                            }
                            else
                            {
                                p1 =utils.addPP( poses[pos],cc.p(0,38*j));
                            }
                        }
                        if(pos==2)
                        {
                            c.setPosition(utils.addPP( poses[pos],cc.p(0,-38*j)));
                        }
                        else
                        {
                            c.setPosition(utils.addPP( poses[pos],cc.p(0,38*j)));
                        }
                        c.stopAllActions();
                        c.runAction(cc.sequence(cc.delayTime(dt), cc.fadeIn(0), cc.moveTo(0.3, c.pos)));
                    }
                    break;
                }
            }
            len += this.showCardsVal[pos][i]["card_list"].length;
            dt += 0.1;
        }
        if (card) {
            card.stopAllActions();
            card.runAction(cc.sequence(cc.spawn(cc.scaleTo(0.3, 0.2), cc.moveTo(0.3, p1)), cc.hide()));
        }
    },
    showMyHandCard:function()
    {
        var p1= cc.p(640,430);
        var dt = 0;
        for (var i  in  this.handCards) {
           var c =  this.handCards[i];
            var p = c.getPosition();
            c.setPosition(p1);
            c.setScale(0);
            c.runAction(cc.sequence(cc.delayTime(dt),cc.spawn(cc.callFunc(function(){
                Sound.getInstance().playEffect(utils.format("%1/res/sound/send_card.mp3",PackName));
            }),cc.scaleTo(0.3, 1), cc.moveTo(0.3, p))));
            dt+=0.1;
            /*c.setState(1);
            (function(c)
            {
                c.runAction(cc.sequence(cc.spawn(cc.scaleTo(0.3, 1), cc.moveTo(0.3, c.getPosition())),cc.callFunc(function(){
                    c.setState(0);
                })));
            })(c);*/
        }
    },
    getHistoryCardsPos: function (pos, sxl, xl) {
        var ret = [];
        var start = cc.p(20, 290);
        var dx = 1;
        var dy = -1;
        var mx = 20;
        switch (pos) {
            case 2:
                dx = -1;
                mx = 5;
                start.x = 460;
                start.y = 710-40;
                break;
            case 1:
                dx = -1;
                start.x = 1220;
                start.y = 290;
                break;
            case 0:
                dy = 1;
                dx = -1;
                start.x = 1220;
                start.y = 10;
                break;
        }
        for (var i = sxl; i < xl; ++i) {
            var di = i%mx;
            var dj = Math.floor(i/mx);
            var x = start.x + 40 * di*dx;
            ret[i] = cc.p(x, start.y + 38 * dj*dy);
        }
        return ret;
    },
    getHistoryCardsPosLast: function (pos) {
        var len = this.historyCardsVal[pos];
        var poses = this.getHistoryCardsPos(pos, len, len + 1);
        return poses[len];
    },
    showHistoryCards: function (pos) {
        if(this.historyCardsVal[pos] == undefined)
        {
            return;
        }
        for (var i  in  this.historyCards[pos]) {
            this.historyCards[pos][i].reset();
        }
        this.historyCards[pos] = [];

        var poses = this.getHistoryCardsPos(pos, 0, this.historyCardsVal[pos].length);
        var len = this.historyCardsVal[pos].length;
        var self = this;
        var func = function (i, cardPos) {
            var card = self.newCard1();
            card.setPosition(cardPos);
            card.setValue(self.historyCardsVal[pos][i]);
            card.setVisible(true);
            if (len - 1 == i) {
                card.setState(0);
            }
            else {
                card.setState(2);
            }
            self.historyCards[pos].push(card);
        };
        for (var i in this.historyCardsVal[pos]) {
            func(i, poses[i]);
        }
    },
    showInHistoryCard: function (pos, c) {

        for (var i in this.publicCard) {
            this.publicCard[i].reset();
        }
        var poses = this.getHistoryCardsPos(pos, 0, Math.max(this.historyCardsVal[pos].length, 1));
        var p1 = poses[0];
        if (pos != 2) {
            p1 = poses[poses.length - 1];
        }
        p1.x += 20;
        var dat =  this.getOutCardPos(pos);
        var p=dat.p;
        var r =dat.r;
        var card  = PHZCards.newActionCard();
        card.setPosition(p);
        card.setValue(c);
        card.setState(0);
        card.setVisible(true);
        card.setScale(1);
        card.setRotation(r);
        card.runAction(cc.sequence(cc.spawn(cc.scaleTo(0.3, 0.2), cc.moveTo(0.3, p1)), cc.hide()));
    },
    showOutHistoryCard: function (from,to, c)
    {
        var fCard = null;
        var tCard= null;
        for(var i in this.historyCards[from])
        {
            if(c == this.historyCards[from][i].value)
            {
                fCard =this.historyCards[from][i];
                break;
            }
        }
        for(var i in this.showCards[to])
        {
            if(this.showCards[to][i].value == c)
            {
                tCard = this.showCards[to][i];
                break;
            }
        }
        if(fCard ==null||tCard==null)
        {
            return;
        }
        var card = this.newCard1();
        card.setPosition(fCard.getPosition());
        card.setValue(c);
        card.setState(0);
        card.setVisible(true);
        card.runAction(cc.sequence( cc.moveTo(0.3, tCard.getPosition()), cc.hide()));
        fCard.setOpacity(0);

    },
    //吃牌信息
    pushChiCards: function (val) {
        this.chiVal = val;
        var func = function(dat)
        {
            for(var i in dat) {
                var data = dat[i];
                var card = data["center_card"];
                for (var j in data["cards"]) {
                    if (data["cards"][j] == card) {
                        data["cards"].splice(j, 1);
                        data["cards"].push(card);
                        break;
                    }
                }
                if (data.hasOwnProperty("second_info")) {
                    func(data["second_info"]);
                }
                if (data.hasOwnProperty("third_info")) {
                    func(data["third_info"]);
                }
            }
        };
        func(this.chiVal);
    },
    //  添加展示区牌
    pushShowCards: function (pos, val) {
        this.showCardsVal[pos] = val;
        var myPos = PHZModel.getPosBySeatID(PHZModel.seatID);
        for (var i in  this.showCardsVal[pos]) {
            var data = this.showCardsVal[pos][i];
            if (data["weave_kind"] == PHZAck.ACK_CHI) {
                var card = data["center_card"];
                for (var j in data["card_list"]) {
                    if (data["card_list"][j] == card) {
                        data["card_list"].splice(j, 1);
                        data["card_list"].push( card);

                        if(pos ==2)
                        {
                            data["card_list"].reverse();
                        }
                        break;
                    }
                }

            }
           else if (data["weave_kind"] == PHZAck.ACK_WEI) {
               if(!data["card_list"])
                {
                    data["card_list"] = [];
                    data["card_list"].push(INVAIL_CARD);
                    data["card_list"].push(INVAIL_CARD);
                    data["card_list"].push(INVAIL_CARD);
                }
            }

        }
    },

    //  添加展示区非吃牌
    pushShowCards1: function (pos, card, type, isNoHand,sID) {
        if (isNoHand && (type == PHZAck.ACK_PAO || type == PHZAck.ACK_TI)) {
            for (var i in this.showCardsVal[pos]) {
                var cards = this.showCardsVal[pos][i]["card_list"];
                if (cards.length == 3) {
                    if (cards[0] == cards[1] && cards[1] == cards[2]) {
                         if (cards[0] == card ) {
                            this.showCardsVal[pos][i]["weave_kind"] = type;
                            cards[0] = card;
                            cards[1] = card;
                            cards[2] = card;
                            cards.push(card);
                            return i;
                        }
                    }
                }
                else  if (cards.length == 2&& cards[1] == cards[0]) {
                   if (cards[0] == card ) {
                        this.showCardsVal[pos][i]["weave_kind"] = type;
                        cards[0] = card;
                        cards[1] = card;
                        cards.push(card);
                        cards.push(card);
                        return i;
                    }
                }
            }
            for (var i in this.showCardsVal[pos]) {
                var cards = this.showCardsVal[pos][i]["card_list"];
                if (cards.length == 3) {
                    if (cards[0] == cards[1] && cards[1] == cards[2]) {
                      if (cards[0] == INVAIL_CARD) {
                            this.showCardsVal[pos][i]["weave_kind"] = type;
                            cards[0] = card;
                            cards[1] = card;
                            cards[2] = card;
                            cards.push(card);
                            return i;
                        }
                    }
                }
            }
        }
        var data = {};
        data["card_list"] = [];
        data["weave_kind"] = type;
        data["center_card"] = card;
        var m = 3;
        if (type == PHZAck.ACK_PAO || type == PHZAck.ACK_TI) {
            m = 4;
        }
        else if (sID!=-1&&type == PHZAck.ACK_PENG) {
            m = 2;
        }
        for (var i = 0; i < m; ++i) {
            data["card_list"].push(card);
        }
        this.showCardsVal[pos].push(data);
        return  this.showCardsVal[pos].length-1;
    },
    //  添加展示区吃牌
    pushShowCards2: function (pos, cards, card) {
        var data = {};
        data["card_list"] = [];
        data["weave_kind"] = PHZAck.ACK_CHI;
        data["center_card"] = card;
        for (var i in cards) {
            if (cards[i] == card) {
                cards.splice(i, 1);
                cards.push(card);
                break;
            }
        }
        if(pos ==2)
        {
            cards.reverse();
        }
        data["card_list"] = cards;
        this.showCardsVal[pos].push(data);
        return  this.showCardsVal[pos].length-1;
    },
    //  添加展示区碰牌
    pushShowCards3: function (pos,  card) {

        for (var i in this.showCardsVal[pos]) {
            var cards = this.showCardsVal[pos][i]["card_list"];
            if (this.showCardsVal[pos][i]["weave_kind"] == PHZAck.ACK_PENG
            && cards.length == 2 && card == cards[0]&& cards[0] == cards[1])
            {
                cards.push(card);
                break;
            }
        }
    },
    isBaKuai: function (pos) {
        var n =0;
        for (var i in this.showCardsVal[pos])
        {
            var type = this.showCardsVal[pos][i]["weave_kind"]
            if (type == PHZAck.ACK_TI||PHZAck.ACK_PAO == type)
            {
                n++;
            }
        }
        return n == 2;
    },

    pushHistoryCards:function(pos,card)
    {
        this.historyCardsVal[pos].push(card);
    },

    popHistoryCards:function(pos,card)
    {
        for(var i in this.historyCardsVal[pos])
        {
            if(card == this.historyCardsVal[pos][i])
            {
                this.historyCardsVal[pos].splice(i, 1);
                break;
            }
        }
    },
    getOutCardPos:function(pos)
    {
        var p = cc.p(640,320);
        var r = 90;
        switch (pos)
        {
            case 3:
                r = 0;
                p.x =300;
                p.y =480;
                break;
            case 2:
                p.x =640;
                p.y =530;
                break;
            case 1:
                r = 0;
                p.x =1280-300;
                p.y =480;
                break;
        }
        return {p:p,r:r};
    },
    showOutCard:function(pos,val,isMove,isDun)
    {
        PHZCards.hideOutCard();
        this.publicCard[pos].reset();
        var p1 = cc.p(640,430);
        var dat =  this.getOutCardPos(pos);
        var p=dat.p;
        var r =dat.r;
        this.publicCard[pos].setPosition(p1);
        this.publicCard[pos].setVisible(true);
        this.publicCard[pos].showDun(isDun);
        this.publicCard[pos].showOut(!isDun);
        this.publicCard[pos].setState(1);
        this.publicCard[pos].setRotation(r);
        if(isMove)
        {
            this.publicCard[pos].setScale(0.1);
            if(val != INVAIL_CARD)
            {
                this.publicCard[pos].setValue(val);
                this.publicCard[pos].setState(0);
                /*this.publicCard[pos].runAction(cc.sequence(cc.spawn(cc.scaleTo(0.2,1),cc.moveTo(0.2,p)),cc.rotateTo(0.1,r),cc.orbitCamera(0.15,1,0,-180,90,0,0),
                    cc.callFunc(function(){
                        this.publicCard[pos].setValue(val);
                        this.publicCard[pos].setState(0);
                        //this.publicCard[pos].setScaleX(-1);
                    }.bind(this)),cc.orbitCamera(0.15,1,0,-90,90,0,0)));*/
            }
            this.publicCard[pos].runAction(cc.spawn(cc.scaleTo(0.2,1),cc.moveTo(0.2,p)));
        }
        else
        {
            this.publicCard[pos].setScale(1);
            this.publicCard[pos].setPosition(p);
            if(val != INVAIL_CARD)
            {
                this.publicCard[pos].setValue(val);
                this.publicCard[pos].setState(0);
            }
        }
    },
    preOutMyCard:function(card)
    {
        var d =0;
        for(var i in this.handCards)
        {
            if(this.handCards[i] == card )
            {
                d = Number(i);
                this.handCards.splice(i,1);
                break;
            }
        }
        var d1 =0;
        for(var i in this.handCardsVal)
        {
            var is = false;
            for(var j in this.handCardsVal[i])
            {
                if(d1 == d)
                {
                    is = true;
                    this.handCardsVal[i].splice(j,1);

                    if (this.handCardsVal[i].length == 0) {
                        this.handCardsVal.splice(i, 1);
                    }
                    break;
                }
                d1++;
            }
            if(is)
            {
                break;
            }
        }

        this.uiNode.runAction(cc.sequence(cc.delayTime(0.2),cc.callFunc(function(){
            PHZCards.showHandCards();
        })));
        card.ableTouch=false;
        this.showMyOutCard(0,card);

    },
    showMyOutCard:function(pos,card,v)
    {
        PHZCards.hideOutCard();
        this.publicCard[pos].reset();

        var p1 =  cc.p(640,0);
        var val = v;
        if(card)
        {
            card.setVisible(false);
            p1 = card.getPosition();
            val = card.value;
        }
        else
        {
            switch (pos)
            {
                case 2:
                    p1.x =550;
                    p1.y =670;
                    break;
                case 3:
                    p1.x =63;
                    p1.y =560;
                    break;
                case 1:
                    p1.x =1280-63;
                    p1.y =560;
                    break;
            }
        }
        var dat =  this.getOutCardPos(pos);
        var p=dat.p;
        var r =dat.r;
        this.publicCard[pos].showOut(true);
        this.publicCard[pos].setValue(val);
        this.publicCard[pos].setState(0);
        this.publicCard[pos].setVisible(true);
        this.publicCard[pos].setRotation(r);

        if(PHZModel.fastShow)
        {
            this.publicCard[pos].setScale(1);
            this.publicCard[pos].setPosition(p);
        }
        else
        {
            this.publicCard[pos].setScale(0.5);
            this.publicCard[pos].setPosition(p1);
            this.publicCard[pos].runAction(cc.spawn(cc.scaleTo(0.2,1),cc.moveTo(0.2,p)));
        }
    },
    hideOutCard:function(pos)
    {
        if(pos)
        {
            this.publicCard[pos].setVisible(false);
        }
        else
        {
            for(var i in this.publicCard)
            {
                this.publicCard[i].setVisible(false);
            }
        }
    },
    showPublicCard:function(pos,val)
    {
        var card = this.newActionCard();
        card.setState(1);
        card.setVisible(true);
        card.showDun(true);
        var p1= cc.p(640,430);
        var dat =  this.getOutCardPos(pos);
        var p=dat.p;
        var r =dat.r;
        var p2 = cc.p(63,66);
        switch (pos)
        {
            case 2:
                p2.x =550;
                p2.y =670;
                break;
            case 3:
                p2.x =63;
                p2.y =560;
                break;
            case 1:
                p2.x =1280-63;
                p2.y =560;
                break;
        }
        if(pos==0)
        {
            for(var i in this.handCards)
            {
                var c = this.handCards[i];
                if(c.value==val)
                {
                    p2 = c.getPosition();
                    break;
                }
            }
        }
        card.setPosition(p1);
        card.setScale(0.1);
        card.setRotation(r);

        if(val == INVAIL_CARD)
        {
        }
        else{
            card.setValue(val);
            card.setState(0);
          /*  card.runAction(cc.sequence(cc.spawn(cc.scaleTo(0.2,1),cc.moveTo(0.2,p)),cc.rotateTo(0.1,r),cc.orbitCamera(0.15,1,0,-180,90,0,0),
                cc.callFunc(function(){
                    card.setValue(val);
                    card.setState(0);
                }.bind(this)),cc.orbitCamera(0.15,1,0,-90,90,0,0),cc.delayTime(0.8),cc.moveTo(0.5,p2),cc.hide()));*/
        }
        card.runAction(cc.sequence(cc.spawn(cc.scaleTo(0.2,1),cc.moveTo(0.2,p)),cc.delayTime(0.5),cc.moveTo(0.3,p2),cc.hide()));
    },

    newTingCard:function(val)
    {
        var t = null;
        if(val>=1&&val<=10)
        {
            val = val == 10 ?0:val;
            t = new cc.LabelAtlas(val,PackName+"/res/font/card_s_nums.png",22,23,'0');
        }
        else
        {
            val = val-16;
            val = val == 10 ?0:val;
            t = new cc.LabelAtlas(val,PackName+"/res/font/card_b_nums.png",22,23,'0');
        }
        t.setAnchorPoint(cc.p(0.5,0.5));

        this.ting.addChild(t);
        return t;
    },
    hideTing:function()
    {
        this.ting.setVisible(false);
    },
    showTing:function(val)
    {
        for( var i  in this.tingCards)
        {
            if(i ==0)
            {
                continue;
            }
            this.tingCards[i].removeFromParent(true);
        }
        this.tingCards=[];
        this.tingCards.push( this.ting.getChildByName("txt"));
        var mess  ="";
        for(var i  in val)
        {
            var c = this.newTingCard(val[i]);
            this.tingCards.push(c);
        }
        this.ting.setVisible(true);
        utils.sortNodes(this.tingCards, this.ting.getContentSize().width*0.5, this.ting.getContentSize().height*0.5,5);
    },

    showAckCardsBack: function (pos,val) {
        var poses = [cc.p(320, 10), cc.p(1100, 340), cc.p(800, 710-40), cc.p(280, 340)];
        var card = null;
        for (var i = 0; i < 4; ++i) {
            if (this.publicCard[i].isVisible()&&(this.publicCard[i].value == val||this.publicCard[i].value ==INVAIL_CARD) ) {
                this.publicCard[i].setVisible(false);
                if(PHZModel.fastShow)
                {
                    break;
                }
                var p = this.getOutCardPos(i);
                card = PHZCards.newActionCard();
                card.setPosition(p);
                card.setValue(this.publicCard[i].value);
                card.setState(this.publicCard[i].state);
                card.setVisible(true);
                card.setScale(1);
                break;
            }
        }
        if (card) {
            card.stopAllActions();

            card.runAction(cc.sequence(cc.spawn(cc.scaleTo(0.3, 0.2), cc.moveTo(0.3, poses[pos])), cc.hide()));
        }
    },
    getVideoHandCardsPos: function (pos, xl, yl) {
        var ret = [];

        if(pos == 3)
        {
            var start = cc.p(180, 400);
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
            var start = cc.p(640-38*xl*0.5, 550);
            for (var i = 0; i < xl; ++i) {
                ret[i] = [];
                var x = start.x +38 * i;
                for (var j = 0; j < yl; ++j) {
                    ret[i][j] = cc.p(x, start.y - 38 * j);
                }
            }
        }
        else if(pos == 1)
        {
            var start = cc.p(1060, 400);
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
    showVideoHandCards: function (pos) {

        if(pos ==0)
        {
            this.handCardsVal = this.videoHandCardsVal[0];
            this.showHandCards();
            return;
        }

        for (var i  in this.videoHandCards[pos]) {
            this.videoHandCards[pos][i].reset();
        }
        this.videoHandCards[pos] = [];
        var poses = this.getVideoHandCardsPos(pos, this.videoHandCardsVal[pos].length, 4);
        for (var i in this.videoHandCardsVal[pos]) {
            var cards = this.videoHandCardsVal[pos][i];

            for (var j in cards) {
                var card = this.newCard1();
                card.setLocalZOrder(i * 10 + 4 - j+100);
                card.setPosition(poses[i][j]);
                card.setVisible(true);
                card.setValue(cards[j]);
                card.setState(0);
                this.videoHandCards[pos].push(card);
            }
        }

    }

};

