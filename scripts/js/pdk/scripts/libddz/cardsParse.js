
var libddz = libddz?libddz:{};
//牌型
libddz.CardsType=
{
    CARD_TYPE_ERROR : 0,            // 错误类型
    CARD_TYPE_ONE : 1,              // 单牌
    CARD_TYPE_ONELINE : 2,          // 单连牌
    CARD_TYPE_TWO : 3,              // 对子
    CARD_TYPE_TWOLINE : 4,          // 连对       3个点数连续的
    CARD_TYPE_THREE : 5,            // triple
    CARD_TYPE_THREELINE : 6,        // 三顺：点数相连的2个及以上的牌，可以从3连到A。
    CARD_TYPE_THREEWITHONE : 7,     // 最后一手没多余牌的情况下可以出三带一
    CARD_TYPE_THREEWITHTWO : 8,     // 3同张必须带2张其他牌，带的牌不要求同点数
    CARD_TYPE_PLANEWITHONE : 9,     // 飞机带羿，多个三带一,牌不够时出
    CARD_TYPE_PLANEWITHWING : 10,   // 飞机带翅，多个三带二
    CARD_TYPE_PLANEWHITHLACK : 11,      // 飞机带少或不带
    CARD_TYPE_FOURWITHONE : 12,     // 4个带一（牌不够时可以少带，带1-2张）
    CARD_TYPE_FOURWITHTWO : 13,     // 4个带二（牌不够时可以少带，带1-2张）
    CARD_TYPE_FOURWITHTHREE : 14,     // 4张牌也可以带3张其他牌，这时不算炸弹
    CARD_TYPE_BOMB : 15,            // 4个       炸弹

};
libddz.cardsParse=
{
    //牌数量
    len:0,
    //牌类型
    type:0,
    //牌型起始点数
    face:0,
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
        else if (type == 2 && len >= 2) // double straight
            return true;
        else if (type == 3 && len > 1) // three straight
            return true;

        return false;
    },

    create:function(cards)
    {
        var ret =utils.clone(libddz.cardsParse);
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

        if (this.len == 1)//判断单牌
        {
            this.face = cards.card1[0].face;
            this.type = libddz.CardsType.CARD_TYPE_ONE;
            return this.type;
        }
        else if ( this.len == 2)//判断对子或王炸
        {
            if (cards.line1.length == 1
                && cards.card2.length == 2)
            {
                this.face = cards.card2[1].face;
                this.type = libddz.CardsType.CARD_TYPE_TWO;
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
                this.type = libddz.CardsType.CARD_TYPE_BOMB;
                return this.type;
            }
            else if (cards.card1.length == 1
                && cards.card3.length == 3)
            {
                this.face = cards.card3[2].face;
                this.type = libddz.CardsType.CARD_TYPE_THREEWITHONE;
                return this.type;
            }
        }
        else if (this.len == 5)//判断4带1或3带2
        {
            log("+++++++++++++++++++++++++++++++++++++++++++");
            if (cards.card2.length == 2
                && cards.card3.length == 3)
            {
                this.face = cards.card3[2].face;
                this.type = libddz.CardsType.CARD_TYPE_THREEWITHTWO; // 333 44
                return this.type;                       
            }
            // else if (cards.card1.length == 1
            //     && cards.card4.length == 4) {
            //     this.face = cards.card4[3].face;
            //     this.type = libddz.CardsType.CARD_TYPE_FOURWITHONE; // 3333 4
            //     return this.type;
            // }
            else if (cards.card1.length == 2
                && cards.card3.length == 3) {
                this.face = cards.card3[2].face;
                this.type = libddz.CardsType.CARD_TYPE_THREEWITHTWO; // 333 4 5
                return this.type;
            }
        }
        else if (this.len == 6)//判断4带2单或4带1对
        {
            if (cards.card1.length == 2
                && cards.card4.length == 4)
            {
                this.face = cards.card4[3].face;
                this.type = libddz.CardsType.CARD_TYPE_FOURWITHTWO;
                return this.type;
            }
            else if (cards.card2.length == 2
                && cards.card4.length == 4) {
                this.face = cards.card4[3].face;
                this.type = libddz.CardsType.CARD_TYPE_FOURWITHTWO;
                return this.type;
            }
        }
        else if (this.len == 7)//判断4带2对
        {
             if (cards.card4.length == 4)
            {
                this.face = cards.card4[3].face;
                this.type = libddz.CardsType.CARD_TYPE_FOURWITHTHREE;// 3333 4 5 6
                return this.type;
            }
        }

        if (cards.card1.length == cards.line1.length&&cards.card2.length ==0
        &&cards.card3.length ==0&&cards.card4.length ==0) {
            if (this.checkLine(cards.line1, 1)) {
                this.face = cards.card1[0].face;
                this.type = libddz.CardsType.CARD_TYPE_ONELINE;
                return this.type;
            }
        }

        if (this.len == cards.card2.length
            && cards.card2.length == cards.line2.length) {
            if (this.checkLine(cards.line2, 2)) {
                this.face = cards.card2[0].face;
                this.type = libddz.CardsType.CARD_TYPE_TWOLINE;
                return this.type;
            }
        }

        if (this.len < 5)
        {
            return this.type;
        }


     var left_card_len;
     if (cards.card3.length == cards.line3.length
        && cards.card4.length == 0 && cards.card3.length != 0)
        {
            if (this.checkLine(cards.line3, 3))
            {
                left_card_len = cards.card1.length + cards.card2.length;
                if (left_card_len == 0)
                {
                    this.face = cards.card3[0].face;
                    this.type = libddz.CardsType.CARD_TYPE_THREELINE;
                    return this.type;

                }
                else if (left_card_len * 3 == cards.card3.length)
                {
                    this.face = cards.card3[0].face;
                    this.type = libddz.CardsType.CARD_TYPE_PLANEWITHONE;//555 666 8 9
                    return this.type;
                }
                //else if (card_stat.card1.size() == 0 && left_card_len * 3 == card_stat.card3.size() * 2)
                else if (left_card_len * 3 == cards.card3.length * 2)//20140819
                {
                    this.face = cards.card3[0].face;
                    this.type = libddz.CardsType.CARD_TYPE_PLANEWITHWING;//555 666 88 99
                    return this.type;
                }
            }

        }


        if (cards.card3.length != 0 && this.checkLine(cards.card3, 3))
        {
            left_card_len = cards.card1.length + cards.card2.length + cards.card4.length;
          
            if (left_card_len * 3 == cards.card3.length) {
  
                this.face = cards.card3[cards.card3.length - 1].face;
                this.type = libddz.CardsType.CARD_TYPE_PLANEWITHONE; // 333 444 555 666 9 9 9 9
                return this.type;
            }
            //else if (card_stat.card1.size() == 0 && left_card_len * 3 == card_stat.card3.size() * 2)
            else if (left_card_len * 3 == cards.card3.length * 2)//20140819
            {
                face = cards.card3[cards.card3.length - 1].face;
                type = libddz.CardsType.CARD_TYPE_PLANEWITHWING; // 333 444 99 99
                return type;
            }
            //in this situation:the last hands has not enough cards
            else if (left_card_len * 3 < cards.card3.length * 2)
            {
                this.face = cards.card3[cards.card3.length - 1].face;
                this.type = libddz.CardsType.CARD_TYPE_PLANEWHITHLACK; // 333 444 9;333 444 8 99;333444 7 8 9
                return this.type;
            }
        }


        if (cards.line3.length>=6)//bug 444 555 666 888 739  can not recognize
        {
            var straight_three_longest = [];
            var cnt = 0; // continues length
            var last_cnt = 0; //last time continues length
            var index = 0; // last time anti_continues length
            var i = 0; // cursor
            var temp = libddz.card;
            var flag = 0; // 0 continues,1 anti_continues

            for (var i = 0; i < cards.line3.length; i += 3)
            {
                if ((cards.line3[i].face - temp.face) != 1)
                {
                    if (cnt > last_cnt)
                    {
                        index = i;
                        last_cnt = cnt;
                    }
                    flag = 1;
                    cnt = 0;
                }
                else
                {
                    if (cards.line3[i].face!=15)
                    {
                        flag = 0;
                    }
                }
                cnt += 3;
                temp = cards.line3[i];
            }

            if (flag == 0)
            {
                if (cnt > last_cnt)
                {
                    index = i;
                    last_cnt = cnt;
                }
            }
            straight_three_longest = [];
            for (var i = (index - last_cnt); i < index; i++)
            {
                straight_three_longest.push(cards.line3[i]);
            }
            var len1=straight_three_longest.length;
            left_card_len = cards.card1.length + cards.card2.length + cards.card3.length + cards.card4.length;

            var len2 = left_card_len-len1;
            if ((len1*2)==(len2*3))
            {
                this.face = straight_three_longest[len1-1].face;
                this.type = libddz.CardsType.CARD_TYPE_PLANEWITHWING; // 333 444 6 7 3
                return this.type;
            }
            else if ((len1*2) > (len2*3)) {
                this.face = straight_three_longest[len1-1].face;
                this.type = libddz.CardsType.CARD_TYPE_PLANEWHITHLACK; // 333 444 6 7 3
                return this.type;
            }
        }

        /*
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
                else if (cards.card1.length == 0
                    && otherLen * 3 == cards.line3.length * 2)//3带2
                {
                    this.face = cards.card3[0].face;
                    this. type = libddz.CardsType.CARD_TYPE_PLANEWITHWING;
                    return  this.type;
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
        */
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
    debug:function()
    {
      cc.log("type:"+this.type+" face:"+this.face);
    },
    //比较牌型大小   v1大为true，v2大为false，相等情况下有限v1大
    compare:function(v1,v2)
    {
        v1.debug();
        v2.debug();
        if(v1.type == libddz.CardsType.CARD_TYPE_ERROR || v2.type == libddz.CardsType.CARD_TYPE_ERROR)
        {
            return false;
        }
        if(v1.type == libddz.CardsType.CARD_TYPE_ROCKET)
        {
            return true;
        }
        if(v2.type == libddz.CardsType.CARD_TYPE_ROCKET)
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
        }
        else
        {
            if(v1.type == libddz.CardsType.CARD_TYPE_BOMB)
            {
                return true;
            }
        }
        return false;
    },
    //比较牌型大x小 cs1大为0，cs2大为1，c1错误为-1，c2错误为-2
    compareCards:function(cs1,cs2)
    {
        for(var i in cs1)
        {
            cc.log("cs1:"+cs1[i]);
        }
        for(var i in cs2)
        {
            cc.log("cs2:"+cs2[i]);
        }
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
}
