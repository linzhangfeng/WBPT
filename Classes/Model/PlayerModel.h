#ifndef __PLAYER_MODEL_H__
#define __PLAYER_MODEL_H__

#include <string>
#include <stdint.h>
#include <vector>

class PlayerModel 
{
public:
	int					seatid;
    
    int                 occupied;
    int                 ready;
    int                 betting;
    int                 see;
    int                 role;
    int                 status;
    int                 bet;
    
    int                 uid;
	std::string			name;
	int					sex;
	std::string			avatar;
	std::string			birthday;
	std::string			zone;
    std::string         dsc;
	int					exp;
	int					rmb;
	long				money;
    int                 matchMoney;
	int					coin;
	int					total_board;
	int					total_win;
	int					pcount;
	int					vtime;
	int					vlevel;
    
    int                 redid;
    int                 redmoney;
    int                 isBig;
    
    int                 card_type; //牌型
    std::vector<int>         mydCard;
    std::vector<int>         spellCard;
    bool offline;       //是否掉线
    
    //新加道具
    int jinbiRound;//禁比轮数
    int fanbeiRound;//翻倍轮数
    int fanbeiNums;//翻倍数
    int huanpaiTimes;//已经使用换牌次数
    
    int jinbika;
    int fanbeika;
    int huanpaika;
    
    int myCard[5];
    
    long allBetMoney;//万人场 所有人在该玩家身上下的钱
    int myBetNums;//万人场 我在该玩家身上下的钱
    
    int eachBetMoney[4];//通杀场在4个位置的押注额
    int BjlBetMoney[5];//百家乐5个位置的押注金额
    
    //不利条件
    int isOut;
    int dangle;
    int cardType;
    //百家乐 start
    int baccarat_playCount; //百家乐庄家当前局数
    int baccarat_win;  //百家乐庄家当前盈利
    bool isRobort;      //是否为机器人
    int lifeNums;//玩家生命
    //百家乐 end
    
    //麻将 start
    int mj_cardNums; //手牌张数
    
    int mj_curScore;
    
    
    int isTuoGuan;
    
    int isTimeOut;
    
    float zhuan_surplustime; //转转麻将剩余倒计时
    //斗地主
    //掉线状态
    int isOffline;
    
    int mingpai_start;
    int base_double;
    int mingpai_play;
    int qiangguan;

public:
	PlayerModel();

	void reset();
    void prepra();
    void prepra2();
    
    void release(){delete this;}

};

#endif
