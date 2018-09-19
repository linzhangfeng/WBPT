var cardAnalysis = cardAnalysis?cardAnalysis:{};

cardAnalysis.TwoCards = 
{

	cards_name:-1,				// 牌型名称
	cards_grade:-1,				// 牌型等级
	cards:[],					// 内部分析使用
	old_cards:[],	// 客户端传入的两张牌
	sort_cards:[], 	// 根据两张牌的等级降序排序，比牌的时候用

	clear:function()
	{ 
		this.cards_name = -1;
		this.cards_grade = -1; 
		this.cards = []; 
		this.old_cards = []; 
		this.sort_cards = [];
	},

	create:function()
	{
		var ret = utils.clone(cardAnalysis.TwoCards);
        ret.clear();
        return ret;
	},

	compare:function(two_cards)
	{
		if (this.sort_cards.length < 2 || two_cards.sort_cards.length < 2)
		{
			return cardAnalysis.CompareRusult.CMP_NULL;
		}
		// 根据牌型等级比较
		if (this.cards_grade > two_cards.cards_grade)
		{
			return cardAnalysis.CompareRusult.CMP_MAX;
		}
		if (this.cards_grade < two_cards.cards_grade)
		{
			return cardAnalysis.CompareRusult.CMP_MIN;
		}
		if (this.cards_grade >= CARD_GRADE_DIHEIGANG) // 天黑杠以上牌型不进行单牌比较
		{
			return cardAnalysis.CompareRusult.CMP_EQL;
		}
		// 牌型等级相等，比较单牌
		if (this.sort_cards[0].grade > two_cards.sort_cards[0].grade)
		{
			if (this.sort_cards[1].grade < two_cards.sort_cards[1].grade)
			{
				return cardAnalysis.CompareRusult.CMP_EQL;
			}
			return cardAnalysis.CompareRusult.CMP_MAX;
		}
		if (this.sort_cards[0].grade < two_cards.sort_cards[0].grade)
		{
			if (this.sort_cards[1].grade > two_cards.sort_cards[1].grade)
			{
				return cardAnalysis.CompareRusult.CMP_EQL;
			}
			return CMP_MIN;
		}
		if (this.sort_cards[0].grade == two_cards.sort_cards[0].grade)
		{
			if (this.sort_cards[1].grade > two_cards.sort_cards[1].grade)
			{
				return cardAnalysis.CompareRusult.CMP_MAX;
			}
			if (this.sort_cards[1].grade < two_cards.sort_cards[1].grade)
			{
				return cardAnalysis.CompareRusult.CMP_MIN;
			}
			if (this.sort_cards[1].grade == two_cards.sort_cards[1].grade)
			{
				return cardAnalysis.CompareRusult.CMP_EQL;
			}
		}	

		return cardAnalysis.CompareRusult.CMP_NULL;
	},
	

	operator:function(two_cards)
	{
		this.clear();
		this.cards_name = two_cards.cards_name;
		this.cards_grade = two_cards.cards_grade;

		for (var i = 0; i < two_cards.cards.length; ++i)
		{
			this.cards.push(two_cards.cards[i]);
		}
		for (var i = 0; i < two_cards.old_cards.length; ++i)
		{
			this.old_cards.push(two_cards.old_cards[i]);
		}
		for (var i = 0; i < two_cards.sort_cards.length; ++i)
		{
			this.sort_cards.push(two_cards.sort_cards[i]);
		}

		return true;
	},
};