#ifndef __gex_AudioMgr_H__
#define __gex_AudioMgr_H__

#include "cocos2d.h"
typedef struct stAudioPlayInfo
{
	stAudioPlayInfo(const std::string& f, int i) :file(f), id(i){}
	std::string file;
	int id;
	bool operator == (const stAudioPlayInfo & a1)
	{
		return a1.file == file && a1.id == id;
	}
}stAudioPlayInfo;
class AudioMgr
{
private:
	AudioMgr(void);
	virtual ~AudioMgr(void);

	int m_bgMusic;
	float m_bgMusicVolume;
	float m_bgEffectVolume;
	std::vector<stAudioPlayInfo> m_effectLoops;
public:
	static AudioMgr *getInstance(void);
	static void destroyInstance(void);


	void end();

	void preloadBackgroundMusic(std::string pszFilePath);
	void playBackgroundMusic(std::string pszFilePath);
	void stopBackgroundMusic(bool bReleaseData = true);
	void pauseBackgroundMusic();
	void resumeBackgroundMusic();
	void setBackgroundMusicVolume(float volume);
	void setEffectsVolume(float volume);

	void preloadEffect(std::string pszFilePath);
	int playEffect(std::string pszFilePath, bool bLoop = false);
	void stopEffect(int id, bool bReleaseData = true);
	void stopAllEffects(bool bReleaseData = true);
	void pauseAllEffects();
	void resumeAllEffects();
	void setFinishCallback(int audioID, const std::function<void(int, const std::string&)>& callback);

	CC_SYNTHESIZE_READONLY(bool, m_isCanPlayBackgroundMusic, IsPlayMusic);
	void setIsPlayMusic(bool is);
	CC_SYNTHESIZE_READONLY(bool, m_isCanPlayEffect, IsPlayEffect);
	void setIsPlayEffect(bool is);

private:

	//当前播放的音乐
	std::string m_curMusic;
};
#endif