#pragma once

#include "Scene/SceneInfo.h"

class CTestSceneInfo :
    public CSceneInfo
{
public:
    CTestSceneInfo();
    ~CTestSceneInfo();

private:
    class CPlayerUI* m_PlayerUI;

private:
    float m_Time; // 복어 스폰주기 변수

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
};

