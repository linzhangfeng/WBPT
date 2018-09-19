
var cardAnalysis = cardAnalysis?cardAnalysis:{};

cardAnalysis.card = {

	face:0,
	suit:0,
	
	value:0,
	grade:0,
	
	Card:[],

	face_symbols:["2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"],

	suit_symbols:["d", "c", "h", "s"],
	
	// bool operator <  (const Card &c) const{ return (face < c.face); };
	// bool operator >  (const Card &c) const { return (face > c.face); };
	// bool operator == (const Card &c) const { return (face == c.face); };

	clear:function()
	{
		this.face = this.suit = this.value = 0;
		this.Card = [];
		this.grade = 0;
	},

	create:function(val)
	{
		var ret = utils.clone(cardAnalysis.card);
        ret.clear();

		ret.value = val;
		ret.face = ret.value & 0xF;
		ret.suit = ret.value >> 4;
		ret.make_grade();

        return ret;
	},

	make_grade:function()
	{
		if (this.face == cardAnalysis.Face.Queen)
		{
			this.grade = cardAnalysis.OneCardGrade.TIANPAI;
		}
		else if (this.face == cardAnalysis.Face.Two)
		{
			this.grade = cardAnalysis.OneCardGrade.DIPAI;
		}
		else if (this.face == cardAnalysis.Face.Eight && this.suit % 2 == 0)
		{
			this.grade = cardAnalysis.OneCardGrade.RENPAI;
		}
		else if (this.face == cardAnalysis.Face.Four && this.suit % 2 == 0)
		{
			this.grade = cardAnalysis.OneCardGrade.HEPAI;
		}
		else if ((this.face == cardAnalysis.Face.Ten && this.suit % 2 == 1) || (this.face == cardAnalysis.Face.Four) || (this.face == cardAnalysis.Face.Six && this.suit % 2 == 1))
		{
			grade = cardAnalysis.OneCardGrade.MEIPAI;
		}
		else if ((this.face == cardAnalysis.Face.Jack) || (this.face == cardAnalysis.Face.Ten) || (this.face == cardAnalysis.Face.Six) || (this.face == cardAnalysis.Face.Seven && this.suit % 2 == 0))
		{
			this.grade = cardAnalysis.OneCardGrade.FUPAI;
		}
		else 
		{
			this.grade = cardAnalysis.OneCardGrade.XIAOPAI;
		}
	},

	set_value:function(val)
	{
		this.value = val;
		
		this.face = value & 0xF;
		this.suit = value >> 4;
		if (this.face < 2)
			this.face += 13;	
		this.make_grade();
	},

	get_card:function()
	{
		var card;
		card.append(1, face_symbols[face - 2]);
		card.append(1, suit_symbols[suit]);
		
		return card;
	},

	
	compare:function(a,b)
	{
		if (a.face < b.face)
		{
			return 1;
		}
		else if (a.face > b.face)
		{
			return -1;
		}
		else if (a.face == b.face)
		{
			if (a.suit < b.suit)
			{
				return 1;
			}
			else if (a.suit > b.suit)
			{
				return -1;
			}	
		}
	
		return 0;
	},

	lesser_callback:function(a,b)
	{
		if (this.compare(a, b) == -1)
			return true;
		else
			return false;
	},
	
	greater_callback:function(a,b)
	{
		if (this.compare(a, b) == 1)
			return true;
		else
			return false;
	},
	
	sort_by_ascending:function(v)
	{
		if(v.length<2)
        {
            return;
        }
        var self = this;
        v.sort(function(v1,v2){
            return self.lesser_callback(v1,v2);
        });
	},
	
	sort_by_descending:function(v)
	{
		if(v.length<2)
        {
            return;
        }
        var self = this;
        v.sort(function(v1,v2){
            return self.greater_callback(v1,v2);
        });
	},

};