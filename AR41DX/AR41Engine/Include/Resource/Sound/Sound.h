#pragma once

#include "../../Ref.h"

class CSound :
    public CRef
{
    friend class CSoundManager;

private:
    CSound();
    ~CSound();

private:
    FMOD::System* m_System;
    FMOD::Sound* m_Sound;
    FMOD::ChannelGroup* m_Group;
    FMOD::Channel* m_Channel;
    std::string     m_GroupName;
    bool        m_Play;
    bool        m_Loop;
    bool        m_Pause;
    char        m_FileName[MAX_PATH];
    char        m_PathName[MAX_PATH];

public:
    const std::string& GetGroupName()   const
    {
        return m_GroupName;
    }

    bool GetPlay()   const
    {
        return m_Play;
    }

    bool GetLoop()  const
    {
        return m_Loop;
    }

public:
    void SetGroupName(const std::string& GroupName)
    {
        m_GroupName = GroupName;
    }

public:
    bool LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group, bool Loop,
        const char* FileName, const std::string& PathName = SOUND_PATH);
    void Play();
    void Stop();
    void Pause();
    void Resume();


public:
    void Save(FILE* File);
    void Load(FILE* File);
};

