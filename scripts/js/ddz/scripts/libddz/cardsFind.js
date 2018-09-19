
var libddz = libddz?libddz:{};

libddz.cardsFind={
    results:[],

    clear:function()
    {
        this.results = [];
    },
    create:function()
    {
        var ret =utils.clone(libddz.cardsFind);
        ret.clear();
        return ret;
    },
    //查找targetCards中大于tempCards的牌
    find:function(tempCardsParse,tempCards,targetCards)
    {
        this.clear();
        if (tempCardsParse.type != libddz.CardsType.CARD_TYPE_ERROR)
        {
            if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_ROCKET){
                return 0;
            }
            if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_ONE){
                this.findOne(tempCardsParse, tempCards, targetCards);
                this.findTwo(tempCardsParse, tempCards, targetCards);
                this.findThree(tempCardsParse, tempCards, targetCards);
            }
            else if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_TWO){
                this.findTwo(tempCardsParse, tempCards, targetCards);
                this.findThree(tempCardsParse, tempCards, targetCards);
            }
            else if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_THREE){
                this.findThree(tempCardsParse, tempCards, targetCards);
            }
            else if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_ONELINE){
                this.findOneLine(tempCardsParse, tempCards, targetCards);
            }
            else if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_TWOLINE){
                this.findTwoLine(tempCardsParse, tempCards, targetCards);
            }
            else if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_THREELINE){
                this.findThreeLine(tempCardsParse, tempCards, targetCards);
            }
            else if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_THREEWITHONE){
                this.findThreeWithOne(tempCardsParse, tempCards, targetCards);
            }
            else if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_THREEWITHTWO){
                this.findThreeWithTwo(tempCardsParse, tempCards, targetCards);
            }
            else if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_PLANEWITHONE){
                this.findPlaneWithOne(tempCardsParse, tempCards, targetCards);
            }
            else if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_PLANEWITHWING){
                this.findPlaneWithWing(tempCardsParse, tempCards, targetCards);
            }
            else if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_FOURWITHONE){
                this.findFourWithOne(tempCardsParse, tempCards, targetCards);
            }
            else if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_FOURWITHTWO){
                this.findFourWithTwo(tempCardsParse, tempCards, targetCards);
            }
            else if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_FOURWITHONESINGLE) {
                this.findFourWithOneSingle(tempCardsParse, tempCards, targetCards);
            }
            else if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_FOURWITHONEPAIR) {
                this.findFourWithOnePair(tempCardsParse, tempCards, targetCards);
            }
            this.findBomb(tempCardsParse, tempCards, targetCards);
            this.findRocket(tempCardsParse, tempCards, targetCards);
        }

        return 0;
    },
    //在单张查找tempCards
    findOne:function(tempCardsParse,tempCards,targetCards)
    {
        if(tempCardsParse.type == libddz.CardsType.CARD_TYPE_ONE)
        {
            for(var i in targetCards.card1)
            {
                if(targetCards.card1[i].face > tempCardsParse.face)
                {
                    var d =[];
                    d.push(targetCards.card1[i]);
                    this.results.push(d);
                }
            }
        }
    },
    //查找在对子中查找tempCards
    findTwo:function(tempCardsParse,tempCards,targetCards)
    {
        if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_ONE)
        {
            for (var i = 0; i < targetCards.card2.length; i += 2)
            {
                if (targetCards.card2[i].face > tempCardsParse.face)
                {
                    var d =[];
                    d.push(targetCards.card2[i]);
                    this.results.push(d);
                }
            }
        }
        else if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_TWO)
        {
            for (var i = 0; i < targetCards.card2.length; i += 2)
            {
                if (targetCards.card2[i].face > tempCardsParse.face)
                {
                    var d =[];
                    d.push(targetCards.card2[i]);
                    d.push(targetCards.card2[i+1]);
                    this.results.push(d);
                }
            }
        }
    },
    //查找在3张中查找tempCards
    findThree:function(tempCardsParse,tempCards,targetCards)
    {
        if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_ONE)
        {
            for (var i = 0; i < targetCards.card3.length; i += 3)
            {
                if (targetCards.card3[i].face > tempCardsParse.face)
                {
                    var d =[];
                    d.push(targetCards.card3[i]);
                    this.results.push(d);
                }
            }
        }
        else if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_TWO)
        {
            for (var i = 0; i < targetCards.card3.length; i += 3)
            {
                if (targetCards.card3[i].face > tempCardsParse.face)
                {
                    var d =[];
                    d.push(targetCards.card3[i]);
                    d.push(targetCards.card3[i+1]);
                    this.results.push(d);
                }
            }
        }
        else if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_THREE)
        {
            for (var i = 0; i < targetCards.card3.length; i += 3)
            {
                if (targetCards.card3[i].face > tempCardsParse.face)
                {
                    var d =[];
                    d.push(targetCards.card3[i]);
                    d.push(targetCards.card3[i+1]);
                    d.push(targetCards.card3[i+2]);
                    this.results.push(d);
                }
            }
        }
    },
    //查找单顺
    findOneLine:function(tempCardsParse,tempCards,targetCards)
    {
        var count = targetCards.line1.length - tempCards.line1.length;
        for (var i = 0; i <= count; i++)
        {
            if (targetCards.line1[i].face > tempCardsParse.face)
            {
                var end = i + tempCardsParse.len;
                if (tempCardsParse.checkLine(targetCards.line1, 1, i, end))
                {
                    var d =[];
                    for (var j = i; j < end; j++)
                    {
                        d.push(targetCards.line1[j]);
                    }
                    this.results.push(d);
                }
            }
        }

    },
    //查找双顺
    findTwoLine:function(tempCardsParse,tempCards,targetCards)
    {
        var count = targetCards.line2.length - tempCards.line2.length;
        for (var i = 0; i <= count; i+=2)
        {
            if (targetCards.line2[i].face > tempCardsParse.face)
            {
                var end = i + tempCardsParse.len;
                if (tempCardsParse.checkLine(targetCards.line2, 2, i, end))
                {
                    var d =[];
                    for (var j = i; j < end; j++)
                    {
                        d.push(targetCards.line2[j]);
                    }
                    this.results.push(d);
                }
            }
        }

    },
    //查找三顺
    findThreeLine:function(tempCardsParse,tempCards,targetCards)
    {
        var count = targetCards.line3.length - tempCards.line3.length;
        for (var i = 0; i <= count; i+=2)
        {
            if (targetCards.line3[i].face > tempCardsParse.face)
            {
                var end = i + tempCardsParse.len;
                if (tempCardsParse.checkLine(targetCards.line3, 2, i, end))
                {
                    var d =[];
                    for (var j = i; j < end; j++)
                    {
                        d.push(targetCards.line3[j]);
                    }
                    this.results.push(d);
                }
            }
        }

    },
    //查找三带1
    findThreeWithOne:function(tempCardsParse,tempCards,targetCards)
    {
        if (targetCards.len < 4)
        {
            return;
        }

        for (var i = 0; i < targetCards.card3.length; i += 3)
        {
            if (targetCards.card3[i].face > tempCardsParse.face)
            {
                var d =[];
                d.push(targetCards.card3[i]);
                d.push(targetCards.card3[i+1]);
                d.push(targetCards.card3[i+2]);
                if (targetCards.card1.length > 0)
                {
                    d.push(targetCards.card1[0]);
                }
                else
                {
                    for (var j = 0; j < targetCards.line1.length; j++)
                    {
                        if (targetCards.line1[j].face != d[0].face) {
                            d.push(targetCards.line1[j]);
                            break;
                        }
                    }
                }
                this.results.push(d);
            }
        }

    },
    //查找三带1对
    findThreeWithTwo:function(tempCardsParse,tempCards,targetCards)
    {
        if (targetCards.len < 5)
        {
            return;
        }

        for (var i = 0; i < targetCards.card3.length; i += 3)
        {
            if (targetCards.card3[i].face > tempCardsParse.face)
            {
                var d =[];
                d.push(targetCards.card3[i]);
                d.push(targetCards.card3[i+1]);
                d.push(targetCards.card3[i+2]);
                if (targetCards.card2.length > 0)
                {
                    d.push(targetCards.card2[0]);
                    d.push(targetCards.card2[1]);
                }
                else
                {
                    for (var j = 0; j < targetCards.line2.length; j++)
                    {
                        if (targetCards.line2[j].face != d[0].face) {
                            d.push(targetCards.line2[j]);
                            d.push(targetCards.line2[j+1]);
                            break;
                        }
                    }
                }
                this.results.push(d);
            }
        }
    },
    //查找三带1飞机
    findPlaneWithOne:function(tempCardsParse,tempCards,targetCards)
    {
        if (targetCards.len < 8)
        {
            return;
        }

        var count = targetCards.line3.length - tempCards.line3.length;
        for (var i = 0; i <= count; i += 3) {
            if (targetCards.line3[i].face > tempCardsParse.face) {
                var end = i + tempCards.card3.length;
                if (tempCardsParse.checkLine(targetCards.line3, 3, i, end)) {
                    var cards = [];
                    for (var j = i; j < end; j++) {
                        cards.push(targetCards.line3[j]);
                    }
                    for (var j = 0; j < targetCards.card1.length; j++) {
                        cards.push(targetCards.card1[j]);
                        if (cards.length == tempCardsParse.len) {
                            break;
                        }
                    }

                    if (cards.length == tempCardsParse.len) {
                        this.results.push(cards);
                        continue;
                    }

                    var flag = 0;
                    for (var j = 0; j < targetCards.line1.length; j++) {
                        flag = 0;
                        for (var k = 0; k < cards.length; k++) {
                            if (cards[k].face == targetCards.line1[j].face) {
                                flag = 1;
                                break;
                            }
                        }

                        if (flag == 1) {
                            continue;
                        }

                        cards.push(targetCards.line1[j]);
                        if (cards.length == tempCardsParse.len) {
                            break;
                        }
                    }

                    if (cards.length == tempCardsParse.len) {
                        this.results.push(cards);
                        continue;
                    }
                }
            }
        }
    },
    //查找三带1对飞机
    findPlaneWithWing:function(tempCardsParse,tempCards,targetCards)
    {
        if (targetCards.len < 10)
        {
            return;
        }
        var count = targetCards.line3.length - tempCards.line3.length;
        for (var i = 0; i <= count; i += 3)
        {
            if (targetCards.line3[i].face > tempCardsParse.face)
            {
                var  end = i + tempCards.card3.length;
                if (tempCardsParse.checkLine(targetCards.line3, 3, i, end))
                {
                    var cards = [];
                    for (var j = i; j < end; j++)
                    {
                        cards.push(targetCards.line3[j]);
                    }
                    for (var j = 0; j < targetCards.card2.length; j += 2)
                    {
                        cards.push(targetCards.card2[j]);
                        cards.push(targetCards.card2[j + 1]);
                        if (cards.length == tempCardsParse.len)
                        {
                            break;
                        }
                    }

                    if (cards.length == tempCardsParse.len)
                    {
                        this.results.push(cards);
                        continue;
                    }

                    var flag = 0;
                    for (var j = 0; j < targetCards.line2.length; j += 2)
                    {
                        flag = 0;
                        for (var k = 0; k < cards.length; k++)
                        {
                            if (cards[k].face == targetCards.line2[j].face)
                            {
                                flag = 1;
                                break;
                            }
                        }

                        if (flag == 1)
                        {
                            continue;
                        }

                        cards.push(targetCards.line2[j]);
                        cards.push(targetCards.line2[j + 1]);
                        if (cards.length == tempCardsParse.len)
                        {
                            break;
                        }
                    }

                    if (cards.length == tempCardsParse.len)
                    {
                        this.results.push(cards);
                        continue;
                    }
                }
            }
        }
    },


    //查找4带2单张
    findFourWithOne:function(tempCardsParse,tempCards,targetCards)
    {
        if (targetCards.len < 6)
        {
            return;
        }

        for (var i = 0; i < targetCards.card4.length; i += 4)
        {
            if (targetCards.card4[i].face > tempCardsParse.face)
            {
                var cards=[];
                cards.push(targetCards.card4[i]);
                cards.push(targetCards.card4[i + 1]);
                cards.push(targetCards.card4[i + 2]);
                cards.push(targetCards.card4[i + 3]);

                for (var j = 0; j < targetCards.card1.length; j++)
                {
                    cards.push(targetCards.card1[j]);
                    if (cards.length == tempCardsParse.len)
                    {
                        break;
                    }
                }

                if (cards.length == tempCardsParse.len)
                {
                    this.results.push(cards);
                    continue;
                }

                var flag = 0;
                for (var j = 0; j < targetCards.line1.length; j++)
                {
                    flag = 0;
                    for (var k = 0; k < cards.length; k++)
                    {
                        if (cards[k].face == targetCards.line1[j].face)
                        {
                            flag = 1;
                            break;
                        }
                    }

                    if (flag == 1)
                    {
                        continue;
                    }

                    cards.push(targetCards.line1[j]);
                    if (cards.length == tempCardsParse.len)
                    {
                        break;
                    }
                }

                if (cards.length == tempCardsParse.len)
                {
                    this.results.push(cards);
                    continue;
                }
            }
        }
    },

    //查找4带2对
    findFourWithTwo:function(tempCardsParse,tempCards,targetCards)
    {
        if (targetCards.len < 8)
        {
            return;
        }

        for (var i = 0; i < targetCards.card4.length; i += 4)
        {
            if (targetCards.card4[i].face > tempCardsParse.face)
            {
                var cards=[];
                cards.push(targetCards.card4[i]);
                cards.push(targetCards.card4[i + 1]);
                cards.push(targetCards.card4[i + 2]);
                cards.push(targetCards.card4[i + 3]);

                for (var j = 0; j < targetCards.card2.length; j += 2)
                {
                    cards.push(targetCards.card2[j]);
                    cards.push(targetCards.card2[j + 1]);
                    if (cards.length == tempCardsParse.len)
                    {
                        break;
                    }
                }

                if (cards.length == tempCardsParse.len)
                {
                    this.results.push(cards);
                    continue;
                }

                var flag = 0;
                for (var j = 0; j < targetCards.line2.length; j += 2)
                {
                    flag = 0;
                    for (var k = 0; k < cards.length; k++)
                    {
                        if (cards[k].face == targetCards.line2[j].face)
                        {
                            flag = 1;
                            break;
                        }
                    }

                    if (flag == 1)
                    {
                        continue;
                    }

                    cards.push(targetCards.line2[j]);
                    cards.push(targetCards.line2[j + 1]);
                    if (cards.length == tempCardsParse.len)
                    {
                        break;
                    }
                }

                if (cards.length == tempCardsParse.len)
                {
                    this.results.push(cards);
                    continue;
                }
            }
        }
    },

    //查找4带1单张
    findFourWithOneSingle:function(tempCardsParse,tempCards,targetCards)
    {
        if (targetCards.len < 5){
            return;
        }
        for (var i = 0; i < targetCards.card4.length; i += 4){
            if (targetCards.card4[i].face > tempCardsParse.face){
                var cards=[];
                cards.push(targetCards.card4[i]);
                cards.push(targetCards.card4[i + 1]);
                cards.push(targetCards.card4[i + 2]);
                cards.push(targetCards.card4[i + 3]);
                for (var j = 0; j < targetCards.card1.length; j++){
                    cards.push(targetCards.card2[j]);
                    if (cards.length == tempCardsParse.len){
                        break;
                    }
                }
                if (cards.length == tempCardsParse.len){
                    this.results.push(cards);
                    continue;
                }
                var flag = 0;
                for (var j = 0; j < targetCards.line1.length; j++){
                    flag = 0;
                    for (var k = 0; k < cards.length; k++){
                        if (cards[k].face == targetCards.line1[j].face){
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 1){
                        continue;
                    }
                    cards.push(targetCards.line1[j]);
                    if (cards.length == tempCardsParse.len){
                        break;
                    }
                }
                if (cards.length == tempCardsParse.len){
                    this.results.push(cards);
                    continue;
                }
            }
        }
    },

    //查找4带1对
    findFourWithOnePair:function(tempCardsParse,tempCards,targetCards)
    {
        if (targetCards.len < 6){
            return;
        }
        for (var i = 0; i < targetCards.card4.length; i += 4){
            if (targetCards.card4[i].face > tempCardsParse.face){
                var cards=[];
                cards.push(targetCards.card4[i]);
                cards.push(targetCards.card4[i + 1]);
                cards.push(targetCards.card4[i + 2]);
                cards.push(targetCards.card4[i + 3]);
                for (var j = 0; j < targetCards.card2.length; j += 2){
                    cards.push(targetCards.card2[j]);
                    cards.push(targetCards.card2[j + 1]);
                    if (cards.length == tempCardsParse.len){
                        break;
                    }
                }
                if (cards.length == tempCardsParse.len){
                    this.results.push(cards);
                    continue;
                }
                var flag = 0;
                for (var j = 0; j < targetCards.line2.length; j += 2){
                    flag = 0;
                    for (var k = 0; k < cards.length; k++){
                        if (cards[k].face == targetCards.line2[j].face){
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 1){
                        continue;
                    }
                    cards.push(targetCards.line2[j]);
                    cards.push(targetCards.line2[j + 1]);
                    if (cards.length == tempCardsParse.len){
                        break;
                    }
                }
                if (cards.length == tempCardsParse.len){
                    this.results.push(cards);
                    continue;
                }
            }
        }
    },

    //查找炸弹
    findBomb:function(tempCardsParse,tempCards,targetCards)
    {
        if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_BOMB)
        {
            for (var i = 0; i < targetCards.card4.length; i += 4)
            {
                if (targetCards.card4[i].face > tempCardsParse.face)
                {
                    var cards=[];
                    cards.push(targetCards.card4[i]);
                    cards.push(targetCards.card4[i + 1]);
                    cards.push(targetCards.card4[i + 2]);
                    cards.push(targetCards.card4[i + 3]);
                    this.results.push(cards);
                }
            }
        }
        else
        {
            for (var i = 0; i < targetCards.card4.length; i += 4)
            {
                var cards=[];
                cards.push(targetCards.card4[i]);
                cards.push(targetCards.card4[i + 1]);
                cards.push(targetCards.card4[i + 2]);
                cards.push(targetCards.card4[i + 3]);
                this.results.push(cards);
            }
        }
    },

    //查找炸弹
    findRocket:function(tempCardsParse,tempCards,targetCards)
    {
        var len = targetCards.card1.length;
        if (len >= 2)
        {
            if (targetCards.card1[len - 2].face == 16
                && targetCards.card1[len - 1].face == 17)
            {
                var cards=[];
                cards.push(targetCards.card1[len - 2]);
                cards.push(targetCards.card1[len - 1]);
                this.results.push(cards);
            }
        }
    },
    //提示比tempCs大的牌 ,返回提示数量
    tip:function(tempCs,targetCs)
    {
        if (tempCs.length == 0)
        {
            return -1;
        }

        if (targetCs.length == 0)
        {
            return -2;
        }

        this.clear();

        var tempCards = libddz.cards.create(tempCs);
        var tempCardsParse = libddz.cardsParse.create(tempCards);
        if (tempCardsParse.type == libddz.CardsType.CARD_TYPE_ERROR)
        {
            return -1;
        }

        var targetCards = libddz.cards.create(targetCs);

        this.find(tempCardsParse, tempCards, targetCards);

        return this.results.length;
    },
    //提示targetCs 中可出的牌
    robot:function(targetCs)
    {
        this.clear();
        var targetCards = libddz.cards.create(targetCs);
        var line = this.getRobotLine(targetCards);
        if(line.length>0)
        {
            this.results.push(line);
            return true;
        }
        var d =[];
        var parse = targetCards;
        if(parse.card3.length>0)
        {
            d.push(parse.card3[0]);
            d.push(parse.card3[1]);
            d.push(parse.card3[2]);
            if(parse.card1.length>0)
            {
                if(parse.card1[0].face != libddz.Face.SMALL &&parse.card1[0].face != libddz.Face.BIG)
                {
                    d.push(parse.card1[0]);
                    this.results.push(d);
                    return true;
                }
            }
            if(parse.card2.length>0)
            {
                d.push(parse.card2[0]);
                d.push(parse.card2[1]);
                this.results.push(d);
            }
            return true;
        }
        else if(parse.card2.length>0)
        {
            d.push(parse.card2[0]);
            d.push(parse.card2[1]);
            this.results.push(d);
            return true;
        }
        else if(parse.card1.length == 2)
        {
            if(parse.card1[0].face == libddz.Face.SMALL &&parse.card1[1].face == libddz.Face.BIG)
            {
                d.push(parse.card1[0]);
            }
            d.push(parse.card1[1]);
            this.results.push(d);
            return true;
        }
        else if(parse.card1.length>0)
        {
            d.push(parse.card1[0]);
            this.results.push(d);
            return true;
        }
        else if(parse.card4.length>0)
        {
            d.push(parse.card1[0]);
            d.push(parse.card1[1]);
            d.push(parse.card1[2]);
            d.push(parse.card1[3]);
            this.results.push(d);
            return true;
        }
        return false;
    },
    //查找最长的顺
    getRobotLine:function(targetCards)
    {
        var line1 = this.getLineMax(1,targetCards.line1);
        var line2 = this.getLineMax(2,targetCards.line2);
        var line3 = this.getLineMax(3,targetCards.line3);
        if(line3.length>=line2.length&&line3.length>=line1.length&&line2.length>=6)
        {
            return line3;
        }
        if(line2.length>=line1.length&&line2.length>=line3.length&&line2.length>=6)
        {
            return line2;
        }
        if(line1.length>=line2.length&&line1.length>=line3.length&&line1.length>=5)
        {
            return line1;
        }
        return [];
    },
    //获取最长的顺
    getLineMax:function(type,targetCs)
    {
        var lastIndex =0;
        var lastNum =0;
        var lineNum =0;
        var temp=libddz.card.create(15);
        var flag =0;
        var i=0;
        for(;i<targetCs.length;i+=type)
        {
            if(targetCs[i].face >libddz.Face.ACE)
            {
                break;
            }
            if(targetCs[i].face -temp.face !=1)
            {
                if(lineNum>lastNum)
                {
                    lastIndex =i;
                    lastNum = lineNum;
                }
                flag =1;
                lineNum =0;
            }
            else
            {
                flag =0;
            }
            lineNum+=type;
            temp = targetCs[i];
        }
        if(flag ==0&&lineNum>lastNum)
        {
            lastIndex =i;
            lastNum = lineNum;
        }
        var ret=[];
        for(var i = lastIndex-lastNum;i<lastIndex;i++)
        {
            ret.push(targetCs[i]);
        }
        return ret;
    }

};