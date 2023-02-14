#pragma once

#include "../../EngineInfo.h"

class CSoundManager
{
	friend class CResourceManager;

private:
	CSoundManager();
	~CSoundManager();

private:
	std::unordered_map<std::string, CSharedPtr<class CSound>>	m_mapSound;
	FMOD::System* m_System;
	FMOD::ChannelGroup* m_MasterGroup;
	std::unordered_map<std::string, FMOD::ChannelGroup*>	m_mapChannelGroup;

public:
	bool Init();
	void Update();
	bool CreateSoundChannel(const std::string& Name);
	bool LoadSound(const std::string& GroupName, const std::string& Name,
		bool Loop, const char* FileName, const std::string& PathName = SOUND_PATH);
	bool SetVolume(float Volume);
	bool SetVolume(const std::string& GroupName, float Volume);
	bool SoundPlay(const std::string& Name);
	bool SoundStop(const std::string& Name);
	bool SoundPause(const std::string& Name);
	bool SoundResume(const std::string& Name);


	FMOD::ChannelGroup* FindChannelGroup(const std::string& Name);
	class CSound* FindSound(const std::string& Name);
	void ReleaseSound(const std::string& Name);
};

