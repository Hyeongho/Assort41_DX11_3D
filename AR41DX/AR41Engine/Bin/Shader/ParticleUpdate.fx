#include "Share.fx"

cbuffer ParticleCBuffer : register(b10)
{
    uint g_ParticleSpawnEnable;  // 파티클 생성 여부
    float3 g_ParticleStartMin;     // 파티클이 생성될 영역의 Min
    float3 g_ParticleStartMax;     // 파티클이 생성될 영역의 Max
    uint g_ParticleSpawnCountMax;// 생성될 파티클의 최대 개수
    float3 g_ParticleScaleMin;     // 생성될 파티클 크기의 Min
    float g_ParticleLifeTimeMin;  // 파티클 생명의 최소 시간
    float3 g_ParticleScaleMax;     // 생성될 파티클 크기의 Max
    float g_ParticleLifeTimeMax;  // 파티클 생명의 최대 시간
    float4 g_ParticleColorMin;     // 생성될 파티클의 색상 Min
    float4 g_ParticleColorMax;     // 생성될 파티클의 색상 Max
    float g_ParticleSpeedMin;     // 파티클 이동 속도 Min
    float g_ParticleSpeedMax;     // 파티클 이동 속도 Max
    uint g_ParticleMoveEnable;   // 이동 할지 말지.
    uint g_ParticleGravity;      // 중력 적용 할지 말지.
    float3 g_ParticleMoveDir;      // 이동을 할 경우 기준이 될 방향.
    float g_ParticleEmpty1;
    float3 g_ParticleMoveAngle;    // 기준이 될 방향으로부터 회전할 최대 각도.
    float g_ParticleEmpty2;
};

#define GRAVITY 9.8f

struct ParticleInfo
{
    uint Enable;
    float3 WorldPos;
    float3 Dir;
    float Speed;
    float LifeTime;
    float LifeTimeMax;
    float FallTime;
    float FallStartY;
};

struct ParticleInfoShare
{
    uint SpawnEnable;

    float3 ScaleMin;
    float3 ScaleMax;

    float4 ColorMin;
    float4 ColorMax;

    uint GravityEnable;
};

RWStructuredBuffer<ParticleInfo> g_ParticleArray : register(u0);
RWStructuredBuffer<ParticleInfoShare> g_ParticleShare : register(u1);


[numthreads(64, 1, 1)]
void ParticleUpdate(int3 ThreadID : SV_DispatchThreadID)
{
    if (g_ParticleSpawnCountMax <= ThreadID.x)
    {
        return;
    }

    g_ParticleShare[0].SpawnEnable = g_ParticleSpawnEnable;

    g_ParticleShare[0].ScaleMin = g_ParticleScaleMin;
    g_ParticleShare[0].ScaleMax = g_ParticleScaleMax;

    g_ParticleShare[0].ColorMin = g_ParticleColorMin;
    g_ParticleShare[0].ColorMax = g_ParticleColorMax;

    g_ParticleShare[0].GravityEnable = g_ParticleGravity;

    // 파티클이 살아있는지 판단한다.
    // 비활성화 되어있을 경우
    if (g_ParticleArray[ThreadID.x].Enable == 0)
    {
        int SpawnEnable = g_ParticleShare[0].SpawnEnable;
        int Exchange = 0;

        if (g_ParticleShare[0].SpawnEnable == 1)
        {
            int InputValue = SpawnEnable - 1;

            // in, in, out 으로 인자가 구성되어 있다.
            InterlockedExchange(g_ParticleShare[0].SpawnEnable, InputValue, Exchange);

            if (Exchange == SpawnEnable)
            {
                g_ParticleArray[ThreadID.x].Enable = 1;
            }
        }

        if (g_ParticleArray[ThreadID.x].Enable == 0)
        {
            return;
        }

        // 살아나야 하는 파티클은 기본 정보들을 설정해준다.
        float Key = ThreadID.x / g_ParticleSpawnCountMax;

        float3 RandomPos = float3(Rand(Key), Rand(Key * 2.f), Rand(Key * 3.f));
    }

    // 활성화 되어 있을 경우
    else
    {
    }
}
