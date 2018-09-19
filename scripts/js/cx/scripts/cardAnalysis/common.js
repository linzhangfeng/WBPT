// suit  0  方块  1 梅花   2  红桃    3黑桃
var cardAnalysis = cardAnalysis?cardAnalysis:{};

cardAnalysis.Face =  
{
	Two : 2,
	Three : 3,
	Four : 4,
	Five : 5,	
	Six : 6,
	Seven : 7,
	Eight : 8,
	Nine : 9,
	Ten : 10,
	Jack : 11,
	Queen : 12,
	Big : 13, 			// 大王
	
	FirstFace : 2,	// 2
	LastFace : 13, 		// 13
};

cardAnalysis.Suit = 
{
	Diamonds : 0,		// 方块
	Clubs : 1,				// 梅花
	Hearts : 2,				// 红桃
	Spades : 3,				// 黑桃
	
	FirstSuit : 0,
	LastSuit : 3,
};

/*
 * 单牌等级
 */
cardAnalysis.OneCardGrade =  
{
	XIAOPAI : 0,		// 小牌
	FUPAI : 1,				// 斧牌
	MEIPAI : 2,				// 梅牌
	HEPAI : 3,				// 和牌
	RENPAI : 4,				// 人牌
	DIPAI : 5,				// 地牌
	TIANPAI	: 6,			// 天牌
} ;

/*
 * 比牌结果
 */
cardAnalysis.CompareRusult = 
{
	CMP_NULL : -1, 		// 空
	CMP_EQL : 0, 		// 等于
	CMP_MAX : 1, 		// 大于
	CMP_MIN : 3, 		// 小于
	
} ;
