#include "SoundManager.h"
#include "Sound.h"

CSoundManager::CSoundManager() :
	m_System(nullptr),
	m_MasterGroup(nullptr)
{
}

CSoundManager::~CSoundManager()
{
	m_mapSound.clear();

	auto	iter = m_mapChannelGroup.begin();
	auto	iterEnd = m_mapChannelGroup.end();

	for (; iter != iterEnd; iter++)
	{
		iter->second->release();
	}

	if (m_System)
	{
		m_System->release();
		m_System->close();
	}
}

bool CSoundManager::Init()
{
	// FMOD_RESULT : FMOD에서 제공하는 타입으로 FMOD의 기능을 사용할때 함수가 성공했는지 실패햇는지를
	// 판단하기 위한 값의 타입이다.
	FMOD_RESULT result = FMOD::System_Create(&m_System);

	if (result != FMOD_OK)
		return false;

	result = m_System->init(500, FMOD_INIT_NORMAL, nullptr);

	if (result != FMOD_OK)
		return false;

	result = m_System->getMasterChannelGroup(&m_MasterGroup);

	if (result != FMOD_OK)
		return false;

	m_mapChannelGroup.insert(std::make_pair("Master", m_MasterGroup));

	CreateSoundChannel("BGM");
	CreateSoundChannel("Effect");
	CreateSoundChannel("UI");

	return true;
}

void CSoundManager::Update()
{
	m_System->update();
}

bool CSoundManager::CreateSoundChannel(const std::string& Name)
{
	if (FindChannelGroup(Name))
		return false;

	FMOD::ChannelGroup* Group = nullptr;

	FMOD_RESULT	result = m_System->createChannelGroup(Name.c_str(), &Group);

	if (result != FMOD_OK)
		return false;

	// 생성한 그룹을 마스터 그룹에 추가해준다.
	m_MasterGroup->addGroup(Group, false);

	m_mapChannelGroup.insert(std::make_pair(Name, Group));

	return true;
}

bool CSoundManager::LoadSound(const std::string& GroupName, const std::string& Name,
	bool Loop, const char* FileName, const std::string& PathName)
{
	CSound* Sound = FindSound(Name);

	if (Sound)
		return true;

	FMOD::ChannelGroup* Group = FindChannelGroup(GroupName);

	if (!Group)
		return false;

	Sound = new CSound;

	Sound->SetName(Name);

	if (!Sound->LoadSound(m_System, Group, Loop, FileName, PathName))
	{
		SAFE_DELETE(Sound);
		return false;
	}

	Sound->SetGroupName(GroupName);

	m_mapSound.insert(std::make_pair(Name, Sound));

	return true;
}

bool CSoundManager::SetVolume(float Volume)
{
	m_MasterGroup->setVolume(Volume / 100.f);

	return true;
}

bool CSoundManager::SetVolume(const std::string& GroupName, float Volume)
{
	FMOD::ChannelGroup* Group = FindChannelGroup(GroupName);

	if (!Group)
		return false;

	Group->setVolume(Volume / 100.f);

	return true;
}

bool CSoundManager::SoundPlay(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Play();

	return true;
}

bool CSoundManager::SoundStop(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Stop();

	return true;
}

bool CSoundManager::SoundPause(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Pause();

	return true;
}

bool CSoundManager::SoundResume(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Resume();

	return true;
}

FMOD::ChannelGroup* CSoundManager::FindChannelGroup(const std::string& Name)
{
	auto	iter = m_mapChannelGroup.find(Name);

	if (iter == m_mapChannelGroup.end())
		return nullptr;

	return iter->second;
}

CSound* CSoundManager::FindSound(const std::string& Name)
{
	auto	iter = m_mapSound.find(Name);

	if (iter == m_mapSound.end())
		return nullptr;

	return iter->second;
}

void CSoundManager::ReleaseSound(const std::string& Name)
{
	auto	iter = m_mapSound.find(Name);

	if (iter == m_mapSound.end())
		return;

	if (iter->second->GetRefCount() == 1)
		m_mapSound.erase(iter);
}
