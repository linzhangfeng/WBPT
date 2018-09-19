
var libddz = libddz?libddz:{};
//牌型
libddz.CardsType=
{
    CARD_TYPE_ERROR : 0,			// 错误类型
    CARD_TYPE_ONE : 1,			    // 单牌
    CARD_TYPE_ONELINE : 2,		    // 单连牌
    CARD_TYPE_TWO : 3,			    // 对子
    CARD_TYPE_TWOLINE : 4,		    // 连对		3个点数连续的
    CARD_TYPE_THREE : 5,			// 三张
    CARD_TYPE_THREELINE : 6,		// 三顺		2个以上连续点数
    CARD_TYPE_THREEWITHONE : 7,	    // 三带一
    CARD_TYPE_PLANEWITHONE : 8,  	// 飞机带单张
    CARD_TYPE_SOFT_BOMB : 9, 	// 炸弹 带癞子
    CARD_TYPE_HARD_BOMB : 10,	// 硬炸 不带癞子
    CARD_TYPE_ROCKET : 11, 	// 火箭
    CARD_TYPE_FIVE_BOMB : 12, 	//硬炸+癞子
    CARD_TYPE_PLANE_BOMB : 13, // 飞机硬炸
    CARD_TYPE_PLANE_LAIZI_BOMB : 14			// 飞机硬炸+癞子

};
libddz.cardsParse=
{
    //牌数量
    len:0,
    //牌类型
    type:0,
    //牌型起始点数
    face:0,
    //癞子值
    laizi:0,
    //飞机几连
    planeLen:0,

    //检测是否是顺子
    checkLine:function(cards,type,begin,end)
    {
        if(begin ==undefined )
        {
            begin = 0;
            end = cards.length;
        }
        if(cards.length < end)
        {
            return false;
        }

        cards.sort(function(v1,v2){
            return libddz.card.compare(v1,v2);
        });
        var  len = 1;
        var card = cards[begin];
        for (var i = type + begin; i < end; i += type)
        {
            if ((card.face + 1) == cards[i].face && cards[i].face != 15)
            { // 2 is not straight (Line)
                len++;
                card = cards[i];
            }
            else
            {
                return false;
            }
        }

        if (type == 1 && len > 4) // single straight
            return true;
        else if (type == 2 && len > 2) // double straight
            return true;
        else if (type == 3 && len > 1) // three straight
            return true;

        return false;
    },

    clear:function()
    {
        this.len=0;
        this.face=0;
        this.laizi=0;
        this.planeLen=0;
    },
    create:function(cards)
    {
        var ret =utils.clone(libddz.cardsParse);
        ret.clear();
        ret.parse(cards);
        return ret;
    },
    parse:function(cards)
    {
        this.type = libddz.CardsType.CARD_TYPE_ERROR;

        this.len = cards.len;

        if (this.len == 0)
        {
            return this.type;
        }
        if (cards.laizi) {
            this.laizi =0x4f;
            if ( this.len == 2)//判断对子
            {
                if(cards.card1.length == 1
                    && cards.card1[0].face != 16
                    && cards.card1[0].face != 17)
                {
                    this.face = cards.card1[0].face;
                    this.laizi =  this.face|0x40;
                    if(  this.face >13)
                    {
                        this.laizi =  (this.face-13)|0x40;
                    }
                    this.type = libddz.CardsType.CARD_TYPE_TWO;
                    return this.type;
                }
            }
            else if ( this.len == 3)//判断三张
            {
                if (cards.card2.length == 2)
                {
                    this.face = cards.card2[0].face;

                    this.laizi =  this.face|0x40;
                    if(  this.face >13)
                    {
                        this.laizi =  (this.face-13)|0x40;
                    }
                    this.type = libddz.CardsType.CARD_TYPE_THREE;
                    return  this.type;
                }
            }
            else if (this.len == 4)//判断炸弹或3带1
            {
                if (cards.card3.length == 3) {
                    this.face = cards.card3[0].face;
                    this.laizi = this.face | 0x40;
                    if(  this.face >13)
                    {
                        this.laizi =  (this.face-13)|0x40;
                    }
                    this.type = libddz.CardsType.CARD_TYPE_SOFT_BOMB;
                    return this.type;
                }
                else if (cards.card1.length == 1
                    && cards.card2.length == 2) {
                    this.face = cards.card2[0].face;
                    this.laizi = this.face | 0x40;
                    if(  this.face >13)
                    {
                        this.laizi =  (this.face-13)|0x40;
                    }
                    this.type = libddz.CardsType.CARD_TYPE_THREEWITHONE;
                    return this.type;
                }
            }
            else if (this.len == 5)//判断5炸
            {
                if (cards.card4.length == 4) {
                    this.face = cards.card4[0].face;
                    this.laizi = 0x4f;
                    this.type = libddz.CardsType.CARD_TYPE_FIVE_BOMB;
                    return this.type;
                }
            }
            if (this.len < 5)
            {
                return this.type;
            }
            //飞机癞子炸
            if (cards.card4.length == this.len-1&&this.checkLine(cards.card4, 4)) {
                this.face = cards.card4[0].face;
                this.laizi = 0x4f;
                this.type = libddz.CardsType.CARD_TYPE_PLANE_BOMB;
                return this.type;
            }

            //判断单顺
            if (this.len-1 == cards.card1.length)
            {
                var lz = false;
                var temp = cards.card1[0].face;
                var start = 1;
                if(temp == 11)
                {
                    temp = temp-1;
                    start = 0;
                    lz = true;
                    i = start;
                }
                if(temp <11)
                {
                    for(var i = start;i< cards.card1.length;i++)
                    {
                        var dis = cards.card1[i].face -temp;

                        if (dis == 1)
                        {
                            temp = cards.card1[i].face;
                        }
                        else
                        {
                            if(dis == 2&&!lz&&temp!=14)
                            {
                                var c =libddz.card.create(0x40|(temp+1));
                                var cs = utils.clone(cards.card1);
                                cs.push(c);

                                if (this.checkLine(cs, 1)) {
                                    this.face = cards.card1[0].face;
                                    this.laizi =(temp+1)| 0x40;
                                    if(  temp+1 >13)
                                    {
                                        this.laizi =  (temp+1-13)|0x40;
                                    }
                                    this.type = libddz.CardsType.CARD_TYPE_ONELINE;
                                    return this.type;
                                }
                            }
                            if(!lz&&cards.card1[0].face>3 && cards.card1[cards.card1.length-1 ].face == 14)
                            {
                                temp = cards.card1[0].face-1;
                                i = -1;
                                lz = true;
                                continue;
                            }
                            return this.type;
                        }
                    }
                    if(lz||temp >=14)
                    {
                        this.face = cards.card1[0].face-1;
                        this.laizi = this.face| 0x40;
                        if(  this.face >13)
                        {
                            this.laizi =  (this.face-13)|0x40;
                        }
                        if(this.face <3)
                        {
                            return libddz.CardsType.CARD_TYPE_ERROR;
                        }
                    }
                    else
                    {

                        this.face = cards.card1[0].face;
                        this.laizi =(temp+1)| 0x40;
                        if(  temp+1 >13)
                        {
                            this.laizi =  (temp+1-13)|0x40;
                        }
                    }
                    this.type = libddz.CardsType.CARD_TYPE_ONELINE;
                    return this.type;
                }
            }//判断双连
            if (1 == cards.card1.length
                && cards.card2.length == this.len-2 )
            {
                var cs = utils.clone(cards.card2);
                var c =utils.clone(cards.card1[0]);
                cs.push(c);
                c =libddz.card.create(0x40|cards.card1[0].face);
                cs.push(c);
                if (this.checkLine(cs, 2)) {
                    this.face = cs[0].face;
                    this.laizi = cards.card1[0].face| 0x40;
                    if(  cards.card1[0].face >13)
                    {
                        this.laizi =  (cards.card1[0].face-13)|0x40;
                    }
                    this.type = libddz.CardsType.CARD_TYPE_TWOLINE;
                    return this.type;
                }
            }
            //判断飞机带单张
            if ( this.len%4 == 0 )
            {
                if( cards.card2.length >=2&& cards.card3.length>=3)
                {
                    for(var i =0;i<cards.card2.length;i+=2)
                    {
                        var cs = utils.clone(cards.card3);
                        var c =utils.clone(cards.card2[i]);
                        cs.push(c);
                        c =utils.clone(cards.card2[i+1]);
                        cs.push(c);
                        c =libddz.card.create(0x40|cards.card2[i].face);
                        cs.push(c);
                        if (this.checkLine(cs, 3)) {
                            this.face = cs[0].face;
                            this.laizi =cards.card2[i].face| 0x40;
                            if( cards.card2[i].face >13)
                            {
                                this.laizi =  (cards.card2[i].face-13)|0x40;
                            }
                            this.type = libddz.CardsType.CARD_TYPE_PLANEWITHONE;
                        }
                    }
                    return this.type;
                }
            }
            //判断三连
            if ( this.len%3 == 0 )
            {
                if( cards.card2.length ==2&& cards.card3.length>=3)
                {
                    var cs = utils.clone(cards.card3);
                    var c =utils.clone(cards.card2[0]);
                    cs.push(c);
                    c =utils.clone(cards.card2[1]);
                    cs.push(c);
                    c =libddz.card.create(0x40|cards.card2[0].face);
                    cs.push(c);
                    if (this.checkLine(cs, 3)) {
                        this.face = cs[0].face;
                        this.laizi =cards.card2[0].face| 0x40;
                        if(  cards.card2[0].face >13)
                        {
                            this.laizi =  (cards.card2[0].face-13)|0x40;
                        }
                        this.type = libddz.CardsType.CARD_TYPE_THREELINE;
                    }
                }
            }
            return this.type;
        }

        if (this.len == 1)//判断单牌
        {
            this.face = cards.card1[0].face;
            this.type = libddz.CardsType.CARD_TYPE_ONE;
            return this.type;
        }
        else if ( this.len == 2)//判断对子或王炸
        {
            if (cards.card2.length == 2)
            {
                this.face = cards.card2[1].face;
                this.type = libddz.CardsType.CARD_TYPE_TWO;
                return this.type;
            }
            else if ( cards.card1.length == 2
                && cards.card1[0].face == 16
                && cards.card1[1].face == 17)
            {
                this.face = cards.card1[1].face;
                this.type = libddz.CardsType.CARD_TYPE_ROCKET;
                return this.type;
            }
        }
        else if ( this.len == 3)//判断三张
        {
            if (cards.card3.length == 3)
            {
                this.face = cards.card3[2].face;
                this.type = libddz.CardsType.CARD_TYPE_THREE;
                return  this.type;
            }
        }
        else if (this.len == 4)//判断炸弹或3带1
        {
            if (cards.card4.length == 4)
            {
                this.face = cards.card4[3].face;
                this.type = libddz.CardsType.CARD_TYPE_HARD_BOMB;
                return this.type;
            }
            else if (cards.card1.length == 1
                && cards.card3.length == 3)
            {
                this.face = cards.card3[0].face;
                this.type = libddz.CardsType.CARD_TYPE_THREEWITHONE;
                return this.type;
            }
        }

        if (this.len < 5)
        {
            return this.type;
        }
        //判断单顺
         if (this.len == cards.card1.length
            && cards.card1.length == cards.line1.length)
         {
            if (this.checkLine(cards.line1, 1)) {
                this.face = cards.card1[0].face;
                this.type = libddz.CardsType.CARD_TYPE_ONELINE;
                return this.type;
            }
        }
        //判断双连
          if (this.len == cards.card2.length
            && cards.card2.length == cards.line2.length)
          {
            if (this.checkLine(cards.line2, 2)) {
                this.face = cards.card2[0].face;
                this.type = libddz.CardsType.CARD_TYPE_TWOLINE;
                return this.type;
            }
        }


    //判断飞机

        //散牌数量
        var otherLen =0;
        if ( cards.line3.length != 0)
        {
            if (this.checkLine(cards.line3, 3))//纯3连
            {
                //散牌数量
                otherLen = cards.len - cards.line3.length;
                if (otherLen == 0)//3连
                {
                    this.face = cards.line3[0].face;
                    this.type = libddz.CardsType.CARD_TYPE_THREELINE;
                    return this.type;
                }
                else if (otherLen * 3 == cards.line3.length)//3带1
                {
                    this.face = cards.line3[0].face;
                    this.type = libddz.CardsType.CARD_TYPE_PLANEWITHONE;
                    return this.type;
                }
            }
            else if (this.checkLine(cards.line3, 3,3,cards.line3.length))//右边3连带1
            {
                otherLen = cards.len - cards.line3.length+3;
                if (otherLen * 3 == cards.line3.length-3)//3带1
                {
                    this.face = cards.line3[3].face;
                    this.type = libddz.CardsType.CARD_TYPE_PLANEWITHONE;
                    return this.type;
                }
            }
            else if (this.checkLine(cards.line3, 3,0,cards.line3.length-3))//左边3连带1
            {
                otherLen = cards.len - cards.line3.length+3;
                if (otherLen * 3 == cards.line3.length-3)//3带1
                {
                    this.face = cards.line3[0].face;
                    this.type = libddz.CardsType.CARD_TYPE_PLANEWITHONE;
                    return this.type;
                }
            }
        }
        if ( cards.card4.length == this.len &&this.checkLine(cards.card4, 4) )
        {
            this.face = cards.card4[0].face;
            this.type = libddz.CardsType.CARD_TYPE_PLANE_BOMB;
            return this.type;
        }

        return this.type;
    },
    //获取牌型
    getCardType:function(cs)
    {
        if(!cs||cs.length==0)
        {
            return libddz.CardsType.CARD_TYPE_ERROR;
        }
        var cards = libddz.cards.create(cs);
        var parse = libddz.cardsParse.create(cards);
        return parse.type;
    },
    //获取牌型
    getCardParse:function(cs)
    {
        if(!cs||cs.length==0)
        {
            return libddz.CardsType.CARD_TYPE_ERROR;
        }
        var cards = libddz.cards.create(cs);
        var parse = libddz.cardsParse.create(cards);
        return parse;
    },
    debug:function()
    {
      cc.log("type:"+this.type+" face:"+this.face);
    },
    //比较牌型大小   v1大为true，v2大为false，相等情况下有限v1大
    compare:function(v1,v2)
    {
        if(v1.type == libddz.CardsType.CARD_TYPE_ERROR || v2.type == libddz.CardsType.CARD_TYPE_ERROR)
        {
            return false;
        }
        if(v1.type == v2.type)
        {
            if(v1.len == v2.len
                && v1.face > v2.face)
            {
                return true;
            }
            if(v1.type == libddz.CardsType.CARD_TYPE_PLANE_BOMB||v1.type == libddz.CardsType.CARD_TYPE_PLANE_LAIZI_BOMB)
            {
                return v1.len> v2.len;
            }
        }
        else
        {
            if(v1.type >= libddz.CardsType.CARD_TYPE_SOFT_BOMB && v1.type <= libddz.CardsType.CARD_TYPE_FIVE_BOMB )
            {
                return v1.type >=v2.type;
            }
            if(v1.type == libddz.CardsType.CARD_TYPE_PLANE_BOMB || v1.type == libddz.CardsType.CARD_TYPE_PLANE_LAIZI_BOMB)
            {
                if(v1.type > v2.type)
                {
                    return true;
                }
                return v1.len> v2.len;
            }
        }
        return false;
    },
    //比较牌型大x小 cs1大为0，cs2大为1，c1错误为-1，c2错误为-2
    compareCards:function(cs1,cs2)
    {
        if (cs1.length == 0)
        {
            return -1;
        }

        if (cs2.length == 0)
        {
            return -2;
        }
        var cards1 = libddz.cards.create(cs1);
        var parse1 = this.create(cards1);
        if (parse1.type == libddz.CardsType.CARD_TYPE_ERROR)
        {
            return -1;
        }

        var cards2 = libddz.cards.create(cs2);
        var parse2 = this.create(cards2);
        if (parse2.type == libddz.CardsType.CARD_TYPE_ERROR)
        {
            return -2;
        }

        var res = this.compare(parse1,parse2);
        if (res)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
};
