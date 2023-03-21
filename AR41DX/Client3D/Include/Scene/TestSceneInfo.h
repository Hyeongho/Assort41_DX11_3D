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
    float m_Time; // ���� �����ֱ� ����

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
};

