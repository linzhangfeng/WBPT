
var libddz = libddz?libddz:{};
//牌型
libddz.cards =
{
    all_cards:[],
    //全是单牌
    card1:[],
    //全是对子
    card2:[],
    //全是三张
    card3:[],
    //全是四张
    card4:[],

    //可能的单连
    line1:[],
    //可能的双连
    line2:[],
    //可能的三连
    line3:[],

    //卡牌总数
    len:0,

    //从小到大排序卡牌
    sortUp:function(cards)
    {
        if(cards.length<2)
        {
            return;
        }
        cards.sort(function(v1,v2){
            return libddz.card.compare(v1,v2);
        });
    },
    //从大到小排序卡牌
    sortDown:function(cards)
    {
        if(cards.length<2)
        {
            return;
        }
        cards.sort(function(v1,v2){
            return libddz.card.compare(v2,v1);
        });
    },
    create:function(cards)
    {
        var ret =utils.clone(libddz.cards);
        ret.clear();
        ret.parse(cards);
        return ret;
    },
    clear:function()
    {
        this.card1 =[];
        this.card2 =[];
        this.card3=[];
        this.card4 =[];
        this.line1 =[];
        this.line2 =[];
        this.line3 =[];
        this.len =0;
        this.all_cards = [];
    },
    parse:function(cards)
    {

        this.clear();
        
        this.len = cards.length;
        if( 0 == this.len )
        {
            return -1;
        }
    
        this.sortUp(cards);  //升序
        this.all_cards = utils.clone(cards);

        var sameNum = 0;    //与temp牌值相同的个数，最多3个，加temp刚好四个
        var i = 0;
        var temp = cards[0];
        for (i = 1; i <= this.len; i++)
        {
            var end = false;
            if(i == this.len)
            {
                end = true;
            }
            if (!end && libddz.card.compareFace(temp,cards[i]) == 0)
            {
                sameNum++;
            }
            else
            {
                if (sameNum == 0)    //单牌
                {
                    this.card1.push(cards[i - 1]);
                    this.line1.push(cards[i - 1]);
                }
                else if (sameNum == 1) //对子
                {
                    this.card2.push(cards[i - 2]);
                    this.card2.push(cards[i - 1]);
                    this.line1.push(cards[i - 2]);
                    this.line2.push(cards[i - 2]);
                    this.line2.push(cards[i - 1]);
                }
                else if (sameNum == 2)//三张
                {
                    this.card3.push(cards[i - 3]);   //按花色降序排列
                    this.card3.push(cards[i - 2]);
                    this.card3.push(cards[i - 1]);
                    this.line1.push(cards[i - 3]);
                    this.line2.push(cards[i - 3]);
                    this.line2.push(cards[i - 2]);
                    this.line3.push(cards[i - 3]);
                    this.line3.push(cards[i - 2]);
                    this.line3.push(cards[i - 1]);
                }
                else if (sameNum == 3)//四张
                {
                    this.card4.push(cards[i - 4]);
                    this.card4.push(cards[i - 3]);
                    this.card4.push(cards[i - 2]);
                    this.card4.push(cards[i - 1]);
                    this.line1.push(cards[i - 4]);
                    this.line2.push(cards[i - 4]);
                    this.line2.push(cards[i - 3]);
                    this.line3.push(cards[i - 4]);
                    this.line3.push(cards[i - 3]);
                    this.line3.push(cards[i - 2]);
                }

                sameNum = 0;
                if(end)
                {
                    break;
                }
            }
            temp = cards[i];
        }
        return 0;
    }
}