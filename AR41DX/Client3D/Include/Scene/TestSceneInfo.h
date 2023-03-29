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

private:
    dtNavMeshCreateParams params;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

private:
    bool BuildNavMeshFromFBX(const char* filename, float cellSize, float cellHeight, float agentHeight,
        float agentRadius, float agentMaxClimb, float agentMaxSlope, dtNavMesh* navMesh);
    bool BuildNavMesh(const float* vertices, int numVerts, const int* triangles, int numTris, float cellSize, float cellHeight, float agentHeight, float agentRadius, float agentMaxClimb, float agentMaxSlope, dtNavMesh* navMesh);
};

