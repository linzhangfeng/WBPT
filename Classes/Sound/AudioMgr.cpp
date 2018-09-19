#include "AudioMgr.h"
#include "AudioEngine.h"
using namespace cocos2d;
using namespace experimental;
static AudioMgr* s_pAudioMgr = NULL;

AudioMgr *AudioMgr::getInstance(void)
{
	if (s_pAudioMgr == NULL)
	{
		s_pAudioMgr = new AudioMgr();
		AudioEngine::lazyInit();
	}
	return s_pAudioMgr;
}

void AudioMgr::destroyInstance(void)
{
	if (s_pAudioMgr != NULL)
	{
		delete s_pAudioMgr;
		s_pAudioMgr = NULL;
	}
}

AudioMgr::AudioMgr(void)
:m_isCanPlayBackgroundMusic(true)
, m_isCanPlayEffect(true)
, m_curMusic("")
, m_bgMusic(AudioEngine::INVALID_AUDIO_ID)
, m_bgMusicVolume(1.f)
, m_bgEffectVolume(1.f)
{
}

AudioMgr::~AudioMgr(void)
{
	end();
}

void AudioMgr::end()
{
	AudioEngine::end();
}

void AudioMgr::playBackgroundMusic(std::string pszFilePath)
{
	if (m_curMusic == pszFilePath || pszFilePath=="")
	{
		return;
	}
	m_curMusic = pszFilePath;
	stopBackgroundMusic(false);
	if (!m_isCanPlayBackgroundMusic)//不能播放则返回
	{
		return;
	}
	m_bgMusic = AudioEngine::play2d(pszFilePath, true, m_bgMusicVolume);
}

void AudioMgr::preloadBackgroundMusic(std::string pszFilePath)
{
	AudioEngine::preload(pszFilePath);
}

void AudioMgr::stopBackgroundMusic(bool bReleaseData)
{
	if (bReleaseData)
	{
		m_curMusic = "";
	}
	if (m_bgMusic == AudioEngine::INVALID_AUDIO_ID) return;
	AudioEngine::stop(m_bgMusic);
	m_bgMusic = AudioEngine::INVALID_AUDIO_ID;
}

void AudioMgr::pauseBackgroundMusic()
{
	if (m_bgMusic == AudioEngine::INVALID_AUDIO_ID) return;
	AudioEngine::pause(m_bgMusic);
}

void AudioMgr::resumeBackgroundMusic()
{
	if (!m_isCanPlayBackgroundMusic || m_bgMusic == AudioEngine::INVALID_AUDIO_ID)
	{
		return;
	}
	AudioEngine::resume(m_bgMusic);
}
void AudioMgr::setBackgroundMusicVolume(float volume)
{
	m_bgMusicVolume = volume;
	if (m_bgMusic == AudioEngine::INVALID_AUDIO_ID) return;
	AudioEngine::setVolume(m_bgMusic, volume);
}

void AudioMgr::setEffectsVolume(float volume)
{
	m_bgEffectVolume = volume;
}

int AudioMgr::playEffect(std::string pszFilePath, bool bLoop)
{
	stAudioPlayInfo info(pszFilePath, AudioEngine::INVALID_AUDIO_ID );

	if (!m_isCanPlayEffect)
	{
		if (bLoop)
		{
			m_effectLoops.push_back(info);
		}
		return AudioEngine::INVALID_AUDIO_ID;
	}
	info.id = AudioEngine::play2d(pszFilePath.c_str(), bLoop, m_bgEffectVolume);
	if (bLoop)
	{
		m_effectLoops.push_back(info);
	}
	return info.id;
}
void AudioMgr::setFinishCallback(int audioID, const std::function<void(int, const std::string&)>& callback)
{
	AudioEngine::setFinishCallback( audioID,  callback);
}

void AudioMgr::preloadEffect(std::string pszFilePath)
{
	AudioEngine::preload(pszFilePath);
}

void AudioMgr::stopEffect(int id, bool bReleaseData)
{
	AudioEngine::stop(id);
	for (size_t i = 0; i < m_effectLoops.size();++i)
	{
		stAudioPlayInfo& d = m_effectLoops[i];
		if (d.id == id)
		{
			if (bReleaseData)
			{
				m_effectLoops.erase(std::find(m_effectLoops.begin(), m_effectLoops.end(), d));
			}
			else
			{
				d.id = AudioEngine::INVALID_AUDIO_ID;
			}
			return;
		}
	}
}
void AudioMgr::stopAllEffects(bool bReleaseData)
{
	auto itEnd = AudioEngine::_audioIDInfoMap.end();
	if (AudioEngine::_audioIDInfoMap.find(m_bgMusic) == itEnd)
	{
		AudioEngine::stopAll();
	}
	else
	{
		auto infos = AudioEngine::_audioIDInfoMap;
		infos.erase(infos.find(m_bgMusic));
		itEnd = infos.end();
		for (auto it = infos.begin(); it != itEnd; ++it)
		{
			AudioEngine::stop(it->first);
		}
	}
	for (size_t i = 0; i < m_effectLoops.size(); )
	{
		//AudioEngine::stop(m_effectLoops.at(i).id);
		if (bReleaseData)
		{
			m_effectLoops.erase(m_effectLoops.begin());
		}
		else
		{
			m_effectLoops[i++].id = AudioEngine::INVALID_AUDIO_ID;
		}
	}
}
void AudioMgr::pauseAllEffects()
{
	auto itEnd = AudioEngine::_audioIDInfoMap.end();
	if (AudioEngine::_audioIDInfoMap.find(m_bgMusic) == itEnd)
	{
		AudioEngine::pauseAll();
	}
	else
	{
		auto infos = AudioEngine::_audioIDInfoMap;
		infos.erase(infos.find(m_bgMusic));
		itEnd = infos.end();
		for (auto it = infos.begin(); it != itEnd; ++it)
		{
			AudioEngine::pause(it->first);
		}
	}
}
void AudioMgr::resumeAllEffects()
{
	auto itEnd = AudioEngine::_audioIDInfoMap.end();
	if (AudioEngine::_audioIDInfoMap.find(m_bgMusic) == itEnd)
	{
		AudioEngine::resumeAll();
	}
	else
	{
		auto infos = AudioEngine::_audioIDInfoMap;
		infos.erase(infos.find(m_bgMusic));
		itEnd = infos.end();
		for (auto it = infos.begin(); it != itEnd; ++it)
		{
			AudioEngine::resume(it->first);
		}
	}
}
void AudioMgr::setIsPlayMusic(bool is)
{
	if (m_isCanPlayBackgroundMusic != is)
	{
		m_isCanPlayBackgroundMusic = is;
		if (!is)
		{
			stopBackgroundMusic(false);
		}
		else
		{
			m_bgMusic = AudioEngine::play2d(m_curMusic, true, m_bgMusicVolume);
		}
	}
}

void AudioMgr::setIsPlayEffect(bool is)
{
	if (m_isCanPlayEffect != is)
	{
		m_isCanPlayEffect = is;
		if (!is)
		{
			stopAllEffects(false);
		}
		else
		{
			for (size_t i = 0; i < m_effectLoops.size(); ++i)
			{
				stAudioPlayInfo& d = m_effectLoops[i];
				if (d.id == AudioEngine::INVALID_AUDIO_ID)
				{
					d.id = AudioEngine::play2d(d.file, true, m_bgEffectVolume);
				}
			}
		}
	}
}
