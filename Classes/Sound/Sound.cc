#include "ZJH.h"
#include "Sound.h"
#include <dirent.h>
using namespace std;

Sound *sound;

Sound::Sound()
{
    curLoad = "";
    allFile.clear();
}

Sound::~Sound()
{
    
}

Sound* Sound::getInstance()
{
    if (sound == NULL) {
        sound = new Sound();
    }
    
    return sound;
}

void Sound::PlayBgSound(const char *fileName ,bool ishall,bool isRoom)
{
    if(ZJHModel::getInstance()->musicOn == 1)
    {
       // SimpleAudioEngine::getInstance()->preloadBackgroundMusic(fileName);
        SimpleAudioEngine::getInstance()->playBackgroundMusic(fileName, true);
    }
}

void Sound::StopBgSound()
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void Sound::playEffect(const char *fileName)
{
    this->playEffect(fileName, false);
}

int Sound::playEffect(const char *fileName, bool isLoop, bool isbtn)
{
    if(ZJHModel::getInstance()->isPause == 1)
    {
        return 0;
    }
    
    if(ZJHModel::getInstance()->soundOn == 0)
    {
        return 0;
    }
    
    int relsut = 0;
    //SimpleAudioEngine::getInstance()->preloadEffect(fileName);
    relsut = SimpleAudioEngine::getInstance()->playEffect(fileName, isLoop);
    return relsut;
}

void Sound::playDNGameEffect(int index, int sex)
{
    string fileName;
    if (sex != SEX_MAN && sex != SEX_FEMALE) {
        sex = SEX_MAN;
    }
    switch(index)
    {
        case SOUND_GLOAL_DEAL:
            fileName = "Sound/dn/sound_fapai.mp3";
            break;
        case play_uptable:
            fileName = "Sound/dn/getin.mp3";
            break;
        case play_outtable:
            fileName = "Sound/dn/getout.mp3";
            break;
        case bet_overtime:
            fileName = "Sound/dn/overtime.mp3";
            break;
        case show_card:
            fileName = "Sound/dn/kaipai.mp3";
            break;
        case ding_zang:
        {
            if(sex == 1)
            {
                fileName = "Sound/dn/zang/nan.mp3";
            }else{
                fileName = "Sound/dn/zang/nv.mp3";
            }
        }
            break;
        
    }
    this->playEffect(fileName.c_str());
}

void Sound::playDNGameCardTypeEffect(int type, int sex)
{
    string fileName;
    if (sex != SEX_MAN && sex != SEX_FEMALE) {
        sex = SEX_MAN;
    }
    switch(type)
    {
        case 0:
            if(sex == 1)
            {
                fileName = "Sound/dn/card/1/0.mp3";
            }else{
                fileName = "Sound/dn/card/2/0.mp3";
            }
            break;
        case 1:
            if(sex == 1)
            {
                fileName = "Sound/dn/card/1/1.mp3";
            }else{
                fileName = "Sound/dn/card/2/1.mp3";
            }
            break;
        case 2:
            if(sex == 1)
            {
                fileName = "Sound/dn/card/1/2.mp3";
            }else{
                fileName = "Sound/dn/card/2/2.mp3";
            }
            break;
        case 3:
            if(sex == 1)
            {
                fileName = "Sound/dn/card/1/3.mp3";
            }else{
                fileName = "Sound/dn/card/2/3.mp3";
            }
            break;
        case 4:
        {
            if(sex == 1)
            {
                fileName = "Sound/dn/card/1/4.mp3";
            }else{
                fileName = "Sound/dn/card/2/4.mp3";
            }
        }
            break;
        case 5:
        {
            if(sex == 1)
            {
                fileName = "Sound/dn/card/1/5.mp3";
            }else{
                fileName = "Sound/dn/card/2/5.mp3";
            }
        }
            break;
        case 6:
        {
            if(sex == 1)
            {
                fileName = "Sound/dn/card/1/6.mp3";
            }else{
                fileName = "Sound/dn/card/2/6.mp3";
            }
        }
            break;
        case 7:
        {
            if(sex == 1)
            {
                fileName = "Sound/dn/card/1/7.mp3";
            }else{
                fileName = "Sound/dn/card/2/7.mp3";
            }
        }
            break;
        case 8:
        {
            if(sex == 1)
            {
                fileName = "Sound/dn/card/1/8.mp3";
            }else{
                fileName = "Sound/dn/card/2/8.mp3";
            }
        }
            break;
        case 9:
        {
            if(sex == 1)
            {
                fileName = "Sound/dn/card/1/9.mp3";
            }else{
                fileName = "Sound/dn/card/2/9.mp3";
            }
        }
            break;
        case 10:
        {
            if(sex == 1)
            {
                fileName = "Sound/dn/card/1/10.mp3";
            }else{
                fileName = "Sound/dn/card/2/10.mp3";
            }
        }
            break;
        case 11:
        {
            if(sex == 1)
            {
                fileName = "Sound/dn/card/1/11.mp3";
            }else{
                fileName = "Sound/dn/card/2/11.mp3";
            }
        }
            break;
        case 12:
        {
            if(sex == 1)
            {
                fileName = "Sound/dn/card/1/12.mp3";
            }else{
                fileName = "Sound/dn/card/2/12.mp3";
            }
        }
            break;
        case 13:
        {
            if(sex == 1)
            {
                fileName = "Sound/dn/card/1/13.mp3";
            }else{
                fileName = "Sound/dn/card/2/13.mp3";
            }
        }
            break;
            
    }
    this->playEffect(fileName.c_str());
}


void Sound::playGameEffect(int index, int sex)
{
    string fileName;
    if (sex != SEX_MAN && sex != SEX_FEMALE) {
        sex = SEX_MAN;
    }
    int flag = random(0, 100) % 2;
    switch (index)
    {
        case SOUND_PLAYER_ADD:
        {
            if (sex == SEX_MAN)
            {
                fileName = "Sound/zjh/m_add.mp3";
            }
            else
            {
                if (flag) {
                    fileName = "Sound/zjh/f_add1.mp3";
                }
                else
                {
                    fileName = "Sound/zjh/f_add2.mp3";
                }
            }
        }
            break;
            
        case SOUND_PLAYER_ALLIN:
        {
            if (sex == SEX_MAN)
            {
                if (flag)
                {
                    fileName = "Sound/zjh/m_allin1.mp3";
                }
                else
                {
                    fileName = "Sound/zjh/m_allin2.mp3";
                }
            }
            else
            {
                if (flag)
                {
                    fileName = "Sound/zjh/f_allin1.mp3";
                }
                else
                {
                    fileName = "Sound/zjh/f_allin2.mp3";
                }
            }
        }
            break;
            
        case SOUND_PLAYER_CMP:
        {
            if (sex == SEX_MAN)
            {
                float ramdon = CCRANDOM_0_1();
                if(ramdon >= 0.5f)
                {
                    fileName = "Sound/zjh/m_cmp.mp3";
                }
                else
                {
                    fileName = "Sound/zjh/m_cmp2.mp3";
                }
            }
            else
            {
                float ramdon = CCRANDOM_0_1();
                if(ramdon >= 0.5f)
                {
                    fileName = "Sound/zjh/f_cmp.mp3";
                }
                else
                {
                    fileName = "Sound/zjh/f_cmp2.mp3";
                }
            }
        }
            break;
            
        case SOUND_PLAYER_FOLLOW_ONE:
        case SOUND_PLAYER_FOLLOW_TWO:
        case SOUND_PLAYER_FOLLOW_THREE:
        {
            if (sex == SEX_MAN)
                fileName = "Sound/zjh/m_follow.mp3";
            else
                fileName = "Sound/zjh/f_follow.mp3";
        }
            break;
            
        case SOUND_PLAYER_GIVEUP:
        {
            if (sex == SEX_MAN)
            {
                fileName = "Sound/zjh/m_giveup.mp3";
            }
            else
            {
                fileName = "Sound/zjh/f_giveup.mp3";
            }
        }
            break;
            
        case SOUND_PLAYER_READY:
        {
            if (sex == SEX_MAN)
                fileName = "Sound/zjh/m_ready.mp3";
            else
                fileName = "Sound/zjh/f_ready.mp3";
        }
            break;
            
        case SOUND_PLAYER_WATCH:
        {
            if (sex == SEX_MAN)
                fileName = "Sound/zjh/m_watch.mp3";
            else
                fileName = "Sound/zjh/f_watch.mp3";
        }
            break;
            
        case SOUND_GLOAL_ADDCHIP:
        {
            fileName = "Sound/zjh/g_addchip.mp3";
        }
            break;
            
        case SOUND_GLOAL_ALARM:
        {
            fileName = "Sound/zjh/g_alarm.mp3";
        }
            break;
            
        case SOUND_GLOAL_CMP_ANIM:
        {
            fileName = "Sound/zjh/g_cmp_anim.mp3";
        }
            break;
            
        case SOUND_GLOAL_CMP_FAIL:
        {
            fileName = "Sound/zjh/g_cmp_fail.mp3";
        }
            break;
            
        case SOUND_GLOAL_CMP_FIRE:
        {
            fileName = "Sound/zjh/g_cmp_fire.mp3";
        }
            break;
            
        case SOUND_GLOAL_CMP_LIGHT:
        {
            fileName = "Sound/zjh/g_cmp_light.mp3";
        }
            break;
            
        case SOUND_GLOAL_CMP_SUC:
        {
            fileName = "Sound/zjh/g_cmp_suc.mp3";
        }
            break;
            
        case SOUND_GLOAL_DEAL:
        {
            fileName = "Sound/zjh/sound_fapai.mp3";
        }
            break;
            
        case SOUND_GLOAL_EGG:
        {
            fileName = "Sound/zjh/g_egg.mp3";
        }
            break;
            
        case SOUND_GLOAL_FLOP:
        {
            fileName = "Sound/zjh/g_flop.mp3";
        }
            break;
            
        case SOUND_GLOAL_GET_CHIP:
        {
            fileName = "Sound/zjh/g_get_chip.mp3";
        }
            break;
            
        case SOUND_GLOAL_GIVEUP:
        {
            fileName = "Sound/zjh/g_giveup.mp3";
        }
            break;
            
        case SOUND_GLOAL_MAX_HAND:
        {
            fileName = "Sound/zjh/g_max_hand.mp3";
        }
            break;
            
        case SOUND_GLOAL_RECVGIFT:
        {
            fileName = "Sound/zjh/g_recvgift.mp3";
        }
            break;
            
        case SOUND_GLOAL_STATIC_GOLD:
        {
            fileName = "Sound/zjh/g_static_gold.mp3";
        }
            break;
            
        case SOUND_GLOAL_WIN:
        {
            fileName = "Sound/zjh/g_win.mp3";
        }
            break;
        
        case SOUND_GLAOL_CLICK:
        {
            fileName = "Sound/zjh/click.mp3";
        }
            break;
        
        case SOUND_GLAOL_CLOSE:
        {
            fileName = "Sound/zjh/sound_guanbi.mp3";
        }
            break;
            
        case slot_fial:
        {
            fileName = "Sound/zjh/sound_laohujibuzhongjiang.mp3";
        }
            break;
            
        case slot_suc:
        {
            fileName = "Sound/zjh/sound_zhongjiang.mp3";
        }
            break;
            
        case slot_run:
        {
            fileName = "Sound/zjh/slot_run.mp3";
        }
            break;
            
        case fire:
        {
            fileName = "Sound/zjh/fire.mp3";
        }
            break;
            
        case bigwheel_run:
        {
            fileName = "Sound/zjh/bigwheel.mp3";
        }
            break;
            
        case add_something:
        {
            fileName = "Sound/zjh/sound_huodejinbi.mp3";
        }
            break;
            
        case g_tick_time:
        {
            fileName = "Sound/zjh/sound_tishichupai.mp3";
        }
            break;
        case bjl_banker_win:
        {
            fileName = "Sound/zjh/bjl_banker_winner.mp3";
        }
            break;
        case bjl_he_win:
        {
            fileName = "Sound/zjh/bjl_he_winner.mp3";
        }
            
            break;
        case bjl_xian_win:
        {
            fileName = "Sound/zjh/bjl_xian_win.mp3";
        }
            break;
            
        case SOUND_BJL_WIN:
        {
            fileName = "Sound/zjh/bjle_win.mp3";
        }
            break;
            
        case SOUND_BJL_LOST:
        {
            fileName = "Sound/zjh/bjl_lost.mp3";
        }
            break;
        default:
            break;
    }
    
    this->playEffect(fileName.c_str());
}

void Sound::preLoadSound()
{
//    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Room/sound/tablemusic.mp3");
//    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Sound/ddz/game_bgm.mp3");
//    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Sound/zjh/game_bgm.mp3");
    //SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Sound/hall_bg.mp3");
}

void filesearch(string path,vector<string> &result)
{
    DIR* dirp;
    struct dirent* direntp;
    dirp = opendir(FileUtils::getInstance()->fullPathForFilename(path).c_str());
    string tmp = ".mp3";
    if( dirp != NULL )
    {
        for(;;)
        {
            direntp = readdir( dirp );
            if( direntp == NULL )
                break;
            string s = path + "/" + direntp->d_name;
            if(s.find(tmp) != -1)
            {
                result.push_back(s);
            }
            else if (string(direntp->d_name) != "" && string(direntp->d_name)[0] != '.')
            {
                filesearch(s,result);
            }
        }
        closedir( dirp );
    }
}

void Sound::loadEffect(float dt)
{
    if (curIndex >= allFile.size())
    {
        cocos2d::Director::getInstance()->getScheduler()->unschedule(schedule_selector(Sound::loadEffect),this);
        return;
    }
    
    log("Sound::preloadEffect[%s]",allFile[curIndex].c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(allFile[curIndex].c_str());
    
    curIndex++;
}

void Sound::preLoadEffect(const char* key)
{
    if (curLoad == key)return;
    for (int i = 0; i < allFile.size(); i ++)
    {
        SimpleAudioEngine::getInstance()->unloadEffect(allFile[i].c_str());
    }
    
    curLoad = key;
    curIndex = 0;
    filesearch(curLoad,allFile);
    
    cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(Sound::loadEffect),this,0.0,false);
}