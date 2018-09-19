#ifndef _SOUND_H_
#define _SOUND_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;

enum {
    SOUND_PLAYER_ADD = 0,
    SOUND_PLAYER_ALLIN,
    SOUND_PLAYER_CMP,
    SOUND_PLAYER_FOLLOW_ONE,
    SOUND_PLAYER_FOLLOW_TWO,
    SOUND_PLAYER_FOLLOW_THREE,
    SOUND_PLAYER_GIVEUP,
    SOUND_PLAYER_READY,
    SOUND_PLAYER_WATCH,
    SOUND_GLOAL_ADDCHIP,
    SOUND_GLOAL_ALARM,
    SOUND_GLOAL_CMP_ANIM,
    SOUND_GLOAL_CMP_FAIL,
    SOUND_GLOAL_CMP_FIRE,
    SOUND_GLOAL_CMP_LIGHT,
    SOUND_GLOAL_CMP_SUC,
    SOUND_GLOAL_DEAL,
    SOUND_GLOAL_EGG,
    SOUND_GLOAL_FLOP,
    SOUND_GLOAL_GET_CHIP,
    SOUND_GLOAL_GIVEUP,
    SOUND_GLOAL_MAX_HAND,
    SOUND_GLOAL_RECVGIFT,
    SOUND_GLOAL_STATIC_GOLD,
    SOUND_GLOAL_WIN,
    SOUND_GLAOL_CLICK,
    SOUND_GLAOL_CLOSE,
    slot_fial,
    slot_run,
    slot_suc,
    fire,
    bigwheel_run,
    add_something,
    g_tick_time,
    red_call,
    red_player_add,
    red_fold,
    red_allin,
    red_banker_allin,
    red_banker_xiazhu,
    red_banker_start_bet_tip,
    red_before_fold,
    red_ready_rob_red,
    red_my_select_red,
    red_balance,
    red_my_open_big,
    red_my_open_small,
    red_show,
    red_open_big,
    red_open_small,
    red_tuozhuai_start,
    red_tuozhuai_end,
    red_select_banker_start,
    red_select_banker_end,
    bjl_banker_win,
    bjl_he_win,
    bjl_xian_win,
    SOUND_BJL_WIN,
    SOUND_BJL_LOST,
    
    
    play_uptable,
    play_outtable,
    bet_overtime,
    show_card,
    ding_zang,
};

class Sound : public Ref
{
public:
	Sound();
	~Sound();
    /** Return the shared instance **/
    static Sound *getInstance();
    void playEffect(const char *fileName);
    int playEffect(const char *fileName, bool isLoop, bool nothalleffect=false);
    void PlayBgSound(const char *fileName,bool ishall = false,bool isRoom=true);
    void StopBgSound();
    void playGameEffect(int index, int sex);
    void preLoadSound();
    void loadEffect(float dt);
    void preLoadEffect(const char* key);
    
    void playDNGameEffect(int index, int sex);
    void playDNGameCardTypeEffect(int type, int sex);
public:
    std::string curLoad;
    int curIndex;
    vector<std::string> allFile;
};

#endif //_SOUND_H_
