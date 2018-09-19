

var libddz = libddz?libddz:{};


/**
 * suit  0  方块  1 梅花   2  红桃    3黑桃
 *
 0x01, 0x11, 0x21, 0x31,		//A 14
 0x02, 0x12, 0x22, 0x32,		//2 15
 0x03, 0x13, 0x23, 0x33,		//3 3
 0x04, 0x14, 0x24, 0x34,		//4 4
 0x05, 0x15, 0x25, 0x35,		//5 5
 0x06, 0x16, 0x26, 0x36,		//6 6
 0x07, 0x17, 0x27, 0x37,		//7 7
 0x08, 0x18, 0x28, 0x38,		//8 8
 0x09, 0x19, 0x29, 0x39,		//9 9
 0x0A, 0x1A, 0x2A, 0x3A,		//10 10
 0x0B, 0x1B, 0x2B, 0x3B,		//J 11
 0x0C, 0x1C, 0x2C, 0x3C,		//Q 12
 0x0D, 0x1D, 0x2D, 0x3D,		//K 13
 0x0E,						//小王
 0x0F						//大王
 * @author luochuanting
 */


//牌点数
libddz.Face ={
    THREE:3,
    FOUR:4,
    FIVE:5,
    SIX:6,
    SEVEN:7,
    EIGHT:8,
    NINE:9,
    TEN:10,
    JACK:11,
    QUEEN:12,
    KING:13,
    ACE:14,
    TWO:15,
    SMALL:16,
    BIG:17,

    FIRST:3,
    LAST:17,
};
//牌色
libddz.Suit=
{
    DIAMONDS:0,
    CLUBS:1,
    HEARTS:2,
    SPADES:3,

    FIRST:0,
    LAST:3,
};
//卡牌
libddz.card =
{
    face:3,
    suit:0,
    value:3,

    //比较a与b卡牌大小，a大则为1，b大则为-1，相等为0
    compare:function(a,b)
    {
        if(a.face> b.face)
        {
            return 1;
        }
        else if(a.face < b.face)
        {
            return -1;
        }
        else
        {
            if(a.suit> b.suit)
            {
                return 1;
            }
            else if(a.suit < b.suit)
            {
                return -1;
            }
        }
        return 0;
    },
    //比较a与b卡牌点数大小，a大则为1，b大则为-1，相等为0
    compareFace:function(a,b)
    {
        if(a.face> b.face)
        {
            return 1;
        }
        else if(a.face < b.face)
        {
            return -1;
        }
        return 0;
    },
    parse:function(val)
    {
        this.value = val;
        this.face = val&15;
        this.suit = val>>4;
        if(this.face<3)
        {
            this.face +=13;
        }
        else if(this.face > 13)
        {
            this.face +=2;
        }
    },
    create:function(val)
    {
        var card =utils.clone(libddz.card);
        card.parse(val);
        return card;
    }
};