//
//  ZJHModel.cpp
//  ZJH
//
//  Created by luochuanting on 13-6-21.
//
//

#include "PlayerModel.h"

PlayerModel::PlayerModel()
{
    reset();
}

void PlayerModel::reset()
{
    uid = -1;
	// seatid = 0;
    
    ready = 0;
    betting = 0;
    see = 0;
    role = 0;
    status = 0;
    bet = 0;
    
	name.clear();
	sex = 0;
	avatar.clear();
	birthday.clear();
	zone.clear();
	exp = 0;
	rmb = 0;
	money = 0;
	coin = 0;
	total_board = 0;
	total_win = 0;
	pcount = 0;
	vtime = 0;
	vlevel = 0;
    
    jinbiRound = 0;
    fanbeiRound = 0;
    
    jinbika = 0;
    fanbeika = 0;
    huanpaika = 0;
    
    myCard[0] = 0;
    myCard[1] = 0;
    myCard[2] = 0;
    myCard[3] = 0;
    myCard[4] = 0;
    
    mydCard.clear();
    spellCard.clear();
    
    allBetMoney = 0;
    myBetNums = 0;
    lifeNums =5;
    
    mj_cardNums = 0;
    
    memset(eachBetMoney, 0, sizeof(eachBetMoney));
    memset(BjlBetMoney, 0, sizeof(BjlBetMoney));
    
    isTuoGuan = 0;
    
    isOffline =0;
    
    zhuan_surplustime = 0.0f;
    
    mingpai_start = 1;
    base_double = 1;
    mingpai_play = 1;
    qiangguan = 1;
}

void PlayerModel::prepra()
{

    myCard[0] = 0;
    myCard[1] = 0;
    myCard[2] = 0;
    myCard[3] = 0;
    myCard[4] = 0;
    
    mydCard.clear();
    spellCard.clear();
    
    redid = -1;
    
    isTuoGuan = 0;
    isTimeOut = 0;
    
    see = 0;
}

void PlayerModel::prepra2()
{
    allBetMoney = 0;
    myBetNums = 0;
    redid = -1;
    betting = 0;
    
    memset(eachBetMoney, 0, sizeof(eachBetMoney));
    for (int i= 0 ; i< 5; i++) {
        BjlBetMoney[i] = 0;
    }
}
