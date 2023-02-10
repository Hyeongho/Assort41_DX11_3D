#include "Share.fx"

cbuffer ParticleCBuffer : register(b10)
{
    uint g_ParticleSpawnEnable;  // ��ƼŬ ���� ����
    float3 g_ParticleStartMin;     // ��ƼŬ�� ������ ������ Min
    float3 g_ParticleStartMax;     // ��ƼŬ�� ������ ������ Max
    uint g_ParticleSpawnCountMax;// ������ ��ƼŬ�� �ִ� ����
    float3 g_ParticleScaleMin;     // ������ ��ƼŬ ũ���� Min
    float g_ParticleLifeTimeMin;  // ��ƼŬ ������ �ּ� �ð�
    float3 g_ParticleScaleMax;     // ������ ��ƼŬ ũ���� Max
    float g_ParticleLifeTimeMax;  // ��ƼŬ ������ �ִ� �ð�
    float4 g_ParticleColorMin;     // ������ ��ƼŬ�� ���� Min
    float4 g_ParticleColorMax;     // ������ ��ƼŬ�� ���� Max
    float g_ParticleSpeedMin;     // ��ƼŬ �̵� �ӵ� Min
    float g_ParticleSpeedMax;     // ��ƼŬ �̵� �ӵ� Max
    uint g_ParticleMoveEnable;   // �̵� ���� ����.
    uint g_ParticleGravity;      // �߷� ���� ���� ����.
    float3 g_ParticleMoveDir;      // �̵��� �� ��� ������ �� ����.
    float g_ParticleEmpty1;
    float3 g_ParticleMoveAngle;    // ������ �� �������κ��� ȸ���� �ִ� ����.
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

    // ��ƼŬ�� ����ִ��� �Ǵ��Ѵ�.
    // ��Ȱ��ȭ �Ǿ����� ���
    if (g_ParticleArray[ThreadID.x].Enable == 0)
    {
        int SpawnEnable = g_ParticleShare[0].SpawnEnable;
        int Exchange = 0;

        if (g_ParticleShare[0].SpawnEnable == 1)
        {
            int InputValue = SpawnEnable - 1;

            // in, in, out ���� ���ڰ� �����Ǿ� �ִ�.
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

        // ��Ƴ��� �ϴ� ��ƼŬ�� �⺻ �������� �������ش�.
        float Key = ThreadID.x / g_ParticleSpawnCountMax;

        float3 RandomPos = float3(Rand(Key), Rand(Key * 2.f), Rand(Key * 3.f));
    }

    // Ȱ��ȭ �Ǿ� ���� ���
    else
    {
    }
}
