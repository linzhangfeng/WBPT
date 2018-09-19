var cardAnalysis = cardAnalysis?cardAnalysis:{};
cardAnalysis.HandCards =
{

	tou_cards:null,												// 头牌
	wei_cards:null,												// 尾牌
	own_cards:[],									// 存放玩家4张手牌
	dijiuwang_select:0,


	clear:function()
	{
		this.tou_cards = cardAnalysis.TwoCards.create();
		this.wei_cards = cardAnalysis.TwoCards.create();
		this.own_cards = [];
		this.dijiuwang_select = 0;
	},

	create:function()
	{
		var ret = utils.clone(cardAnalysis.HandCards);
        ret.clear();
        return ret;
	},

	copy_cards:function()
	{
		this.clear()

		for (var i = 0; i < 4; ++i)
		{
			if (i < 2)
			{
				this.tou_cards.cards.push(this.own_cards[i]);
			}
			else
			{
				this.wei_cards.cards.push(this.own_cards[i]);
			}
		}

		this.tou_cards.old_cards = utils.clone(tou_cards.cards);
		this.sort_by_grade(tou_cards);

		this.wei_cards.old_cards = utils.clone(wei_cards.cards);
		this.sort_by_grade(wei_cards);
	},

	createAnalysisCards:function(cards,_dijiuwang_select)
	{

		this.clear();

		var tempCards = [];
		for(var i = 0;i<cards.length;i++)
		{
			var card = cardAnalysis.card.create(cards[i]);
			tempCards.push(card);
		}
		this.analysis(tempCards,_dijiuwang_select);
	},

	analysis:function(cards,_dijiuwang_select)
	{
		if (cards.length != 4)
		{
			return 101;
		}
		dijiuwang_select = _dijiuwang_select;

		// var temp_cards = cards;
		// cardAnalysis.card.sort_by_descending(temp_cards); 			// 降序排序

		// log("======length===="+cards.length);
		// for(var i =0;i < cards.length;i++)
		// {
		// 	log("======kkkkk===="+cards[i].value);
		// 	log("======kkkkk===="+cards[i].face);
		// }

		for (var i = 0; i < 4; i++)
		{
			if (i < 2)
			{
				this.tou_cards.cards.push(cards[i]);
			}
			else
			{
				this.wei_cards.cards.push(cards[i]);
			}
		}

		this.card_type_classify(this.tou_cards);
		this.card_type_classify(this.wei_cards);
		// this.judge_tou_wei();

		return 1;
	},

	card_type_teshu:function(cards)
	{
		if (cards.length < 3 || cards.length > 4)
		{
			return false;
		}

		this.clear();
		var cards_ten = []; 	// 存放10 
		var cards_six = []; 	// 存放6
		var card_J = false; 			// 是否有牌 J
		var card_B = false; 			// 是否有大王

		for (var i = 0; i < cards.length; ++i)
		{
			if (cards[i].face == cardAnalysis.Face.Ten)
			{
				cards_ten.push(cards[i]);
			}
			else if (cards[i].face == cardAnalysis.Face.Six)
			{
				cards_six.push(cards[i]);
			}
			else if (cards[i].face == cardAnalysis.Face.Jack)
			{
				card_J = true;
			}
			else if (cards[i].face == cardAnalysis.Face.Big)
			{
				card_B = true;
			}
		}

		if (card_J && cards_ten.length >= 2)
		{
			if (cards_ten.length >= 3)
			{
				this.tou_cards.cards_name = this.wei_cards.cards_name = cardAnalysis.CardName.CARD_NAME_SANHUASHI;
				this.tou_cards.cards_grade = this.wei_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_SANHUASHI;
				return true;
			}
			if ((cards_ten[0].suit + cards_ten[1].suit) % 2 == 1) // 方块+红桃 梅花+黑桃 和是偶数；其他组合都是奇数
			{
				this.tou_cards.cards_name = this.wei_cards.cards_name = cardAnalysis.CardName.CARD_NAME_SANHUASHI;
				this.tou_cards.cards_grade = this.wei_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_SANHUASHI;
				return true;
			}
		}

		if (card_B && cards_six.length >= 2)
		{
			if (cards_six.length >= 3)
			{
				this.tou_cards.cards_name = this.wei_cards.cards_name = cardAnalysis.CardName.CARD_NAME_SANHUALIU;
				this.tou_cards.cards_grade = this.wei_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_SANHUALIU;
				return true;
			}
			if ((cards_six[0].suit + cards_six[1].suit) % 2 == 1)
			{
				this.tou_cards.cards_name = this.wei_cards.cards_name = cardAnalysis.CardName.CARD_NAME_SANHUALIU;
				this.tou_cards.cards_grade = this.wei_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_SANHUALIU;
				return true;
			}
		}

		return false;
	},

	card_type_classify:function(two_cards)
	{
		if (two_cards.cards.length != 2)
		{
			return false;
		}

		two_cards.old_cards = two_cards.cards;

		log("======two_cards.cards[0]====="+two_cards.cards[0].face);
		log("======two_cards.cards[1]====="+two_cards.cards[1].face);

		this.sort_by_grade(two_cards);
		cardAnalysis.card.sort_by_descending(two_cards.cards); // 降序排序

		log("===sort===two_cards.cards[0]====="+two_cards.cards[0].face);
		log("====sort==two_cards.cards[1]====="+two_cards.cards[1].face);

		

		if (this.card_type_dingerhuang(two_cards))
		{
			return true;
		}
		if (this.card_type_duizi(two_cards))
		{
			return true;
		}
		if (this.card_type_wang(two_cards))
		{
			return true;
		}
		if (this.card_type_tiangang(two_cards))
		{
			return true;
		}
		if (this.card_type_digang(two_cards))
		{
			return true;
		}
		if (this.is_wang(two_cards))
		{
			return true;
		}

		var face_sum = 0;
		face_sum += two_cards.cards[0].face;
		face_sum += two_cards.cards[1].face;
		if (two_cards.cards[0].face == cardAnalysis.Face.Big || two_cards.cards[1].face == cardAnalysis.Face.Big)
		{
			face_sum -= 7; // 大王(13)当6点
		}
		face_sum %= 10;
		var ret = false;

		switch (face_sum) {
			case 9:
				ret = this.card_type_jiudian(two_cards);
				break;
			case 8:	
				ret = this.card_type_badian(two_cards);
				break;
			case 7:
				ret = this.card_type_qidian(two_cards);
				break;
			case 6:
				ret = this.card_type_liudian(two_cards);
				break;
			case 5:
				ret = this.card_type_wudian(two_cards);
				break;
			case 4:
				ret = this.card_type_sidian(two_cards);
				break;
			case 3:
				ret = this.card_type_sandian(two_cards);
				break;
			case 2:
				ret = this.card_type_erdian(two_cards);
				break;
			case 1:
				ret = this.card_type_yidian(two_cards);
				break;
			case 0:
				ret = this.card_type_lingdian(two_cards);
				break;
			default:
				ret = false;		
		}

		return ret;
	},

	sort_by_grade:function(two_cards)
	{
		if (two_cards.cards[0].grade >= two_cards.cards[1].grade)
		{
			two_cards.sort_cards = two_cards.cards;
		}
		else 
		{
			two_cards.sort_cards.push(two_cards.cards[1]);
			two_cards.sort_cards.push(two_cards.cards[0]);
		}

		return true;
	},

	card_type_dingerhuang:function(two_cards) // 王 + 三
	{
		if (two_cards.cards[0].face == cardAnalysis.Face.Big && two_cards.cards[1].face == cardAnalysis.Face.Three)
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DINGERHUANG;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DINGERHUANG;
			return true;
		}

		return false;
	},

	card_type_duizi:function(two_cards) 
	{
		if (two_cards.cards[0].face != two_cards.cards[1].face)
		{
			return false;
		}

		if (two_cards.cards[0].face == cardAnalysis.Face.Queen) // 一对Q
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIANPAIDUI;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIANPAIDUI;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Two) // 一对二
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DIPAIDUI;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DIPAIDUI;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Eight) // 一对红八
		{
			if (two_cards.cards[0].suit % 2 == 0 && two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_RENPAIDUI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_RENPAIDUI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Four) // 一对红四
		{
			if (two_cards.cards[0].suit % 2 == 0 && two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_HEPAIDUI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_HEPAIDUI;
				return true;
			}
		}
		if (two_cards.cards[0].suit % 2 == 1 && two_cards.cards[1].suit % 2 == 1) // 一对黑十 或黑六 或黑四
		{
			if (two_cards.cards[0].face == cardAnalysis.Face.Ten || two_cards.cards[0].face == cardAnalysis.Face.Six || two_cards.cards[0].face == cardAnalysis.Face.Four) 
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_ZHONGSANDUI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_ZHONGSANDUI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Jack) // 一对J
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_XIAYAODUI;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_XIAYAODUI;
			return true;
		}
		if (two_cards.cards[0].suit % 2 == 0 && two_cards.cards[1].suit % 2 == 0) // 一对红十 或红七 或红六
		{
			if (two_cards.cards[0].face == cardAnalysis.Face.Ten || two_cards.cards[0].face == cardAnalysis.Face.Seven || two_cards.cards[0].face == cardAnalysis.Face.Six) 
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_XIAYAODUI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_XIAYAODUI;
				return true;
			}
		}
		if (two_cards.cards[0].suit % 2 == 1 && two_cards.cards[1].suit % 2 == 1) // 一对黑五 或黑七 或黑八 或黑九
		{
			if (two_cards.cards[0].face == cardAnalysis.Face.Five || two_cards.cards[0].face == cardAnalysis.Face.Seven || two_cards.cards[0].face == cardAnalysis.Face.Eight || two_cards.cards[0].face == cardAnalysis.Face.Nine) 
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_SILAN;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_SILAN;
				return true;
			}
		}

		return false;
	},

	card_type_wang:function(two_cards)
	{
		if (two_cards.cards[0].face == cardAnalysis.Face.Queen && two_cards.cards[1].face == cardAnalysis.Face.Nine) // 所有的Q + 所有的九
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIANJIUWANG;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIANJIUWANG;
			return true;
		}
		if (this.dijiuwang_select == 2) // 地九王为大牌
		{
			if (two_cards.cards[0].face == cardAnalysis.Face.Nine && two_cards.cards[1].face == cardAnalysis.Face.Two) // 所有的九 + 所有的二
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DIJIUWANG_MAX;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DIJIUWANG_MAX;
				return true;
			}
		}

		return false;
	},

	card_type_tiangang:function(two_cards)
	{
		if (two_cards.cards[0].face == cardAnalysis.Face.Queen && two_cards.cards[1].face == cardAnalysis.Face.Eight)
		{
			if (two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIANHONGGANG;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIANHONGGANG;
				return true;
			}
			if (two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIANHEIGANG;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIANHEIGANG;
				return true;
			}
		}

		return false;
	},

	card_type_digang:function(two_cards)
	{
		if (two_cards.cards[0].face == cardAnalysis.Face.Eight && two_cards.cards[1].face == cardAnalysis.Face.Two)
		{
			if (two_cards.cards[0].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DIHONGGANG;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DIHONGGANG;
				return true;
			}
			if (two_cards.cards[0].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DIHEIGANG;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DIHEIGANG;
				return true;
			}
		}

		return false;
	},

	is_wang:function(two_cards)
	{
		if (two_cards.cards[0].face == cardAnalysis.Face.Big && two_cards.cards[1].face == cardAnalysis.Face.Queen)
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIANGUANBA;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIANGUANBA;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Big && two_cards.cards[1].face == cardAnalysis.Face.Two)
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DIGUANBA;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DIGUANBA;
			return true;
		}

		return false;
	},

	card_type_jiudian:function(two_cards)
	{
		if (two_cards.cards[0].face == cardAnalysis.Face.Queen && two_cards.cards[1].face == cardAnalysis.Face.Seven)
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIANGUANJIU;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIANGUANJIU;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Seven && two_cards.cards[1].face == cardAnalysis.Face.Two)
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DIGUANJIU;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DIGUANJIU;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Jack && two_cards.cards[1].face == cardAnalysis.Face.Eight)
		{
			if (two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_RENGUANJIU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_RENGUANJIU;
				return true;
			}
			else
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_WULONGJIU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_WULONGJIU;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Five && two_cards.cards[1].face == cardAnalysis.Face.Four)
		{
			if (two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_HEGUANJIU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_HEGUANJIU;
				return true;
			}
			else
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_BANWUJIU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_BANWUJIU;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Nine)
		{
			if (two_cards.cards[0].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_MEISHIJIU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_MEISHIJIU;
				return true;
			}
			else
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIAOSHIJIU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIAOSHIJIU;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Six && two_cards.cards[1].face == cardAnalysis.Face.Three)
		{
			if (two_cards.cards[0].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DINGCHANGJIU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DINGCHANGJIU;
				return true;
			}
			else
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DINGMAOJIU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DINGMAOJIU;
				return true;
			}
		}
		else
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_JIUDIAN;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_JIUDIAN;
			return true;
		}

		return false;
	},

	card_type_badian:function(two_cards)
	{

		if (two_cards.cards[0].face == cardAnalysis.Face.Queen && two_cards.cards[1].face == cardAnalysis.Face.Six)
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIANGUANBA;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIANGUANBA;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Six && two_cards.cards[1].face == cardAnalysis.Face.Two)
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DIGUANBA;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DIGUANBA;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Eight) // 所有的十 + 红八
		{
			if (two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_RENSHIBA;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_RENSHIBA;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Four && two_cards.cards[1].face == cardAnalysis.Face.Four)
		{
			if ((two_cards.cards[0].suit + two_cards.cards[1].suit) % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_HEGUANBA;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_HEGUANBA;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Eight) // 黑十 + 黑八
		{
			if (two_cards.cards[0].suit % 2 == 1 && two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_MEISHIBA;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_MEISHIBA;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Jack && two_cards.cards[1].face == cardAnalysis.Face.Seven)
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_FUQIBA;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_FUQIBA;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Eight) // 红十 + 黑八
		{
			if (two_cards.cards[0].suit % 2 == 0 && two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIAOSHIBA;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIAOSHIBA;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Five && two_cards.cards[1].face == cardAnalysis.Face.Three)
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DINGWUBA;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DINGWUBA;
			return true;
		}
		else
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_BADIAN;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_BADIAN;
			return true;
		}

		return true;
	},

	card_type_qidian:function(two_cards)
	{
		if (two_cards.cards[0].face == cardAnalysis.Face.Queen && two_cards.cards[1].face == cardAnalysis.Face.Five)
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIANGUANQI;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIANGUANQI;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Five && two_cards.cards[1].face == cardAnalysis.Face.Two)
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DIGUANQI;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DIGUANQI;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Nine && two_cards.cards[1].face == cardAnalysis.Face.Eight)
		{
			if (two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_SUZHOUQI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_SUZHOUQI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Four && two_cards.cards[1].face == cardAnalysis.Face.Three)
		{
			if (two_cards.cards[0].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DINGHEQI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DINGHEQI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Seven) // 黑十 + 所有的七
		{
			if (two_cards.cards[0].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_MEISHIQI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_MEISHIQI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Four && two_cards.cards[1].face == cardAnalysis.Face.Three)
		{
			if (two_cards.cards[0].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DINGBANQI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DINGBANQI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Jack && two_cards.cards[1].face == cardAnalysis.Face.Six)
		{
			if (two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_FUCHANGQI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_FUCHANGQI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Jack && two_cards.cards[1].face == cardAnalysis.Face.Six)
		{
			if (two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_FUMAOQI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_FUMAOQI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Seven) // 红十 + 所有的七
		{
			if (two_cards.cards[0].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIAOSHIQI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIAOSHIQI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Nine && two_cards.cards[1].face == cardAnalysis.Face.Eight)
		{
			if (two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_FEIZHOUQI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_FEIZHOUQI;
				return true;
			}
		}
		else
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_QIDIAN;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_QIDIAN;
			return true;
		}

		return false;
	},

	card_type_liudian:function(two_cards)
	{
		if (two_cards.cards[0].face == cardAnalysis.Face.Queen && two_cards.cards[1].face == cardAnalysis.Face.Four)
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIANGUANLIU;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIANGUANLIU;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Four && two_cards.cards[1].face == cardAnalysis.Face.Two)
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DIGUANLIU;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DIGUANLIU;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Eight && two_cards.cards[1].face == cardAnalysis.Face.Eight) // 黑八 + 红八
		{
			if ((two_cards.cards[0].suit + two_cards.cards[1].suit) % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_YUANBAOLIU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_YUANBAOLIU;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Six) // 黑十 + 所有的六
		{
			if (two_cards.cards[0].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_MEISHILIU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_MEISHILIU;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Six) // 红十 + 黑六
		{
			if (two_cards.cards[0].suit % 2 == 0 && two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_CHANGSANLIU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_CHANGSANLIU;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Jack && two_cards.cards[1].face == cardAnalysis.Face.Five) // 
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_FUWULIU;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_FUWULIU;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Six) // 红十 + 红六
		{
			if (two_cards.cards[0].suit % 2 == 0 && two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIAOSHILIU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIAOSHILIU;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Nine && two_cards.cards[1].face == cardAnalysis.Face.Seven) // 红七 + 所有的九
		{
			if (two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_GAOYAOLIU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_GAOYAOLIU;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Nine && two_cards.cards[1].face == cardAnalysis.Face.Seven) // 黑七 + 所有的九
		{
			if (two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_QIJIULIU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_QIJIULIU;
				return true;
			}
		}
		else
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_LIUDIAN;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_LIUDIAN;
			return true;
		}

		return false;
	},
	card_type_wudian:function(two_cards)
	{
		if (two_cards.cards[0].face == cardAnalysis.Face.Queen && two_cards.cards[1].face == cardAnalysis.Face.Three)
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIANDINGWU;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIANDINGWU;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Three && two_cards.cards[1].face == cardAnalysis.Face.Two)
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DIDINGWU;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DIDINGWU;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Eight && two_cards.cards[1].face == cardAnalysis.Face.Seven) // 红八 + 所有的七
		{
			if (two_cards.cards[0].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_RENSHIWU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_RENSHIWU;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Jack && two_cards.cards[1].face == cardAnalysis.Face.Four) // 所有的J + 红四
		{
			if (two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_HEFUWU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_HEFUWU;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Five) // 黑十 + 所有的五
		{
			if (two_cards.cards[0].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_MEISHIWU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_MEISHIWU;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Jack && two_cards.cards[1].face == cardAnalysis.Face.Four) // 所有的J + 黑四
		{
			if (two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_BANFUWU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_BANFUWU;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Nine && two_cards.cards[1].face == cardAnalysis.Face.Six) // 所有的九 + 黑六
		{
			if (two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_CHANGSANWU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_CHANGSANWU;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Five) // 红十 + 所有的五
		{
			if (two_cards.cards[0].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIAOSHIWU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIAOSHIWU;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Nine && two_cards.cards[1].face == cardAnalysis.Face.Six) // 所有的九 + 红六
		{
			if (two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_MAOJIUWU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_MAOJIUWU;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Eight && two_cards.cards[1].face == cardAnalysis.Face.Seven) // 黑八 + 红七
		{
			if (two_cards.cards[0].suit % 2 == 1 && two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_GAOYAOWU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_GAOYAOWU;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Eight && two_cards.cards[1].face == cardAnalysis.Face.Seven) // 黑八 + 黑七
		{
			if (two_cards.cards[0].suit % 2 == 1 && two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_QIBAWU;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_QIBAWU;
				return true;
			}
		}
		else
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_WUDIAN;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_WUDIAN;
			return true;
		}

		return false;
	},

	card_type_sidian:function(two_cards)
	{
		if (two_cards.cards[0].face == cardAnalysis.Face.Queen && two_cards.cards[1].face == cardAnalysis.Face.Two) // 所有的Q + 所有的二
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIANDIGONGZI;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIANDIGONGZI;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Eight && two_cards.cards[1].face == cardAnalysis.Face.Six) // 红八 + 所有的六
		{
			if (two_cards.cards[0].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_RENSHISI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_RENSHISI ;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Four) // 所有的十 + 红四
		{
			if (two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_HESHISI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_HESHISI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Four) // 黑十 + 黑四
		{
			if (two_cards.cards[0].suit % 2 == 1 && two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_MEISHISI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_MEISHISI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Eight && two_cards.cards[1].face == cardAnalysis.Face.Six) // 黑八 + 黑六
		{
			if (two_cards.cards[0].suit % 2 == 1 && two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_CHANGSHISI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_CHANGSHISI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Jack && two_cards.cards[1].face == cardAnalysis.Face.Three) // 所有的J + 所有的三
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DINGFUSI;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DINGFUSI;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Eight && two_cards.cards[1].face == cardAnalysis.Face.Six) // 黑八 + 红六
		{
			if (two_cards.cards[0].suit % 2 == 1 && two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_LIUBASI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_LIUBASI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Seven && two_cards.cards[1].face == cardAnalysis.Face.Seven) // 黑七 + 红七
		{
			if ((two_cards.cards[0].suit + two_cards.cards[1].suit) % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_SHUANGHUAQI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_SHUANGHUAQI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Nine && two_cards.cards[1].face == cardAnalysis.Face.Five) // 所有的九 + 所有的五
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_WUJIUSI;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_WUJIUSI;
			return true;
		}
		else
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_SIDIAN;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_SIDIAN;
			return true;
		}

		return false;
	},

	card_type_sandian:function(two_cards)
	{
		if (two_cards.cards[0].face == cardAnalysis.Face.Queen && two_cards.cards[1].face == cardAnalysis.Face.Jack) // 所有的Q + 所有的J
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIANFUSAN;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIANFUSAN;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Jack && two_cards.cards[1].face == cardAnalysis.Face.Two) // 所有的J + 所有的二
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DIFUSAN;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DIFUSAN;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Eight && two_cards.cards[1].face == cardAnalysis.Face.Five) // 红八 + 所有的五
		{
			if (two_cards.cards[0].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_RENSHISAN;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_RENSHISAN;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Nine && two_cards.cards[1].face == cardAnalysis.Face.Four) // 所有的九 + 红四
		{
			if (two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_HESHISAN;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_HESHISAN;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Three) // 黑十 + 所有的三
		{
			if (two_cards.cards[0].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_MEISHISAN;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_MEISHISAN;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Nine && two_cards.cards[1].face == cardAnalysis.Face.Four) // 所有的九 + 黑四
		{
			if (two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_SIJIUSAN;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_SIJIUSAN;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Seven && two_cards.cards[1].face == cardAnalysis.Face.Six) // 所有的七 + 黑六
		{
			if (two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_CHANGSHISAN;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_CHANGSHISAN;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Three) // 红十 + 所有的三
		{
			if (two_cards.cards[0].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIAOSHISAN;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIAOSHISAN;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Seven && two_cards.cards[1].face == cardAnalysis.Face.Six) // 所有的七 + 红六
		{
			if (two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_LIUQISAN;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_LIUQISAN;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Eight && two_cards.cards[1].face == cardAnalysis.Face.Five) // 黑八 + 所有的五
		{
			if (two_cards.cards[0].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_WUBASAN;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_WUBASAN;
				return true;
			}
		}
		else
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_SANDIAN;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_SANDIAN;
			return true;
		}
		
		return false;	
	},

	card_type_erdian:function(two_cards)
	{
		if (two_cards.cards[0].face == cardAnalysis.Face.Queen && two_cards.cards[1].face == cardAnalysis.Face.Ten) // 所有的Q + 所有的10
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_TIANSHIER;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_TIANSHIER;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Two) // 所有的10 + 所有的2
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DISHIER;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DISHIER;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Eight && two_cards.cards[1].face == cardAnalysis.Face.Four) // 红八 + 所有的四
		{
			if (two_cards.cards[0].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_RENSHIER;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_RENSHIER;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Eight && two_cards.cards[1].face == cardAnalysis.Face.Four) // 黑八 + 红四
		{
			if (two_cards.cards[0].suit % 2 == 1 && two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_HESHIER;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_HESHIER;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Eight && two_cards.cards[1].face == cardAnalysis.Face.Four) // 黑八 + 黑四
		{
			if (two_cards.cards[0].suit % 2 == 1 && two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_SIBAER;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_SIBAER;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Six && two_cards.cards[1].face == cardAnalysis.Face.Six) // 黑六 + 红六
		{
			if ((two_cards.cards[0].suit + two_cards.cards[1].suit) % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_ERLIUER;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_ERLIUER;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Seven && two_cards.cards[1].face == cardAnalysis.Face.Five) // 红七 + 所有的五
		{
			if (two_cards.cards[0].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_GAOYAOER;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_GAOYAOER;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Nine && two_cards.cards[1].face == cardAnalysis.Face.Three) // 所有的九 + 所有的三
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DINGJIUER;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DINGJIUER;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Seven && two_cards.cards[1].face == cardAnalysis.Face.Five) // 黑七 + 所有的五
		{
			if (two_cards.cards[0].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_WUQIER;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_WUQIER;
				return true;
			}
		}
		else
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_ERDIAN;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_ERDIAN;
			return true;
		}

		return false;
	},

	card_type_yidian:function(two_cards)
	{
		if (two_cards.cards[0].face == cardAnalysis.Face.Nine && two_cards.cards[1].face == cardAnalysis.Face.Two) // 所有的九 + 所有的二
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DIJIUWANG_MIN;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DIJIUWANG_MIN;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Eight && two_cards.cards[1].face == cardAnalysis.Face.Three) // 红八 + 所有的三
		{
			if (two_cards.cards[0].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_RENSHIYI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_RENSHIYI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Seven && two_cards.cards[1].face == cardAnalysis.Face.Four) // 黑七 + 红四
		{
			if (two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_HESHIYI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_HESHIYI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Jack && two_cards.cards[1].face == cardAnalysis.Face.Ten) // 所有的J + 黑十
		{
			if (two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_MEISHIYI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_MEISHIYI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Seven && two_cards.cards[1].face == cardAnalysis.Face.Four) // 所有的七 + 黑四
		{
			if (two_cards.cards[1].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_SIQIYI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_SIQIYI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Six && two_cards.cards[1].face == cardAnalysis.Face.Five) // 黑六 + 所有的五
		{
			if (two_cards.cards[0].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_CHANGSANYI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_CHANGSANYI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Jack && two_cards.cards[1].face == cardAnalysis.Face.Ten) // 所有的J + 红十
		{
			if (two_cards.cards[1].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_FUSHIYI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_FUSHIYI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Six && two_cards.cards[1].face == cardAnalysis.Face.Five) // 红六 + 所有的五
		{
			if (two_cards.cards[0].suit % 2 == 0)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_WULIUYI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_WULIUYI;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Eight && two_cards.cards[1].face == cardAnalysis.Face.Three) // 黑八 + 所有的三
		{
			if (two_cards.cards[0].suit % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DINGBAYI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_DINGBAYI;
				return true;
			}
		}
		else
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_YIDIAN;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_YIDIAN;
			return true;
		}
		
		return false;
	},

	card_type_lingdian:function(two_cards)
	{
		if (two_cards.cards[0].face == cardAnalysis.Face.Six && two_cards.cards[1].face == cardAnalysis.Face.Four) // 所有的六 + 所有的四
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_SILIUTING;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_LINGDIAN; // 所有零点牌型同等级 
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Ten && two_cards.cards[1].face == cardAnalysis.Face.Ten) // 黑十 + 红十
		{
			if ((two_cards.cards[0].suit + two_cards.cards[1].suit) % 2 == 1)
			{
				two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_SHUANGHUASHI;
				two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_LINGDIAN;
				return true;
			}
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Jack && two_cards.cards[1].face == cardAnalysis.Face.Nine) // 所有的J + 所有的九
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_FUJIUZHOU;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_LINGDIAN;
			return true;
		}
		if (two_cards.cards[0].face == cardAnalysis.Face.Seven && two_cards.cards[1].face == cardAnalysis.Face.Three) // 所有的七 + 所有的san
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_DINGQIZHOU;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_LINGDIAN;
			return true;
		}
		else
		{
			two_cards.cards_name = cardAnalysis.CardName.CARD_NAME_LINGDIAN;
			two_cards.cards_grade = cardAnalysis.CardGrade.CARD_GRADE_LINGDIAN;
			return true;
		}

		return false;
	},

	judge_tou_wei:function()
	{
		if (wei_cards.compare(tou_cards) == cardAnalysis.CompareRusult.CMP_MAX)
		{
			var temp_two_cards;
			temp_two_cards = utils.clone(tou_cards);

			tou_cards = [];
			tou_cards = utils.clone(wei_cards);

			wei_cards = [];
			wei_cards = utils.clone(temp_two_cards);
		}
	},

	getTouCardName:function()
	{
		return this.tou_cards.cards_name;
	},

	getWeiCardName:function()
	{
		return this.wei_cards.cards_name;
	},
};
