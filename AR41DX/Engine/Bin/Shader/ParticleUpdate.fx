#include "Share.fx"

cbuffer ParticleCBuffer : register(b8)
{
    uint g_ParticleSpawnEnable; // ��ƼŬ ���� ����
    float3 g_ParticleStartMin; // ��ƼŬ�� ������ ������ Min
    float3 g_ParticleStartMax; // ��ƼŬ�� ������ ������ Max
    uint g_ParticleSpawnCountMax; // ������ ��ƼŬ�� �ִ� ����
    float3 g_ParticleScaleMin; // ������ ��ƼŬ ũ���� Min
    float g_ParticleLifeTimeMin; // ��ƼŬ ������ �ּ� �ð�
    float3 g_ParticleScaleMax; // ������ ��ƼŬ ũ���� Max
    float g_ParticleLifeTimeMax; // ��ƼŬ ������ �ִ� �ð�
    float4 g_ParticleColorMin; // ������ ��ƼŬ�� ���� Min
    float4 g_ParticleColorMax; // ������ ��ƼŬ�� ���� Max
    float g_ParticleSpeedMin; // ��ƼŬ �̵� �ӵ� Min
    float g_ParticleSpeedMax; // ��ƼŬ �̵� �ӵ� Max
    uint g_ParticleMoveEnable; // �̵� ���� ����.
    uint g_ParticleGravity; // �߷� ���� ���� ����.
    float3 g_ParticleMoveDir; // �̵��� �� ��� ������ �� ����.
    uint g_ParticleMoveDirEnable; // �̵� ������ ���� ������ ���� �����Ѵ�.
    float3 g_ParticleMoveAngle; // ������ �� �������κ��� ȸ���� �ִ� ����.
    float g_ParticleEmpty2;
};

#define GRAVITY 98.f

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

float3x3 ComputeRotationMatrix(float3 Angle)
{
    float3 ConvertAngle;
    ConvertAngle.x = DegreeToRadian(Angle.x);
    ConvertAngle.y = DegreeToRadian(Angle.y);
    ConvertAngle.z = DegreeToRadian(Angle.z);

    float3x3 matRotX, matRotY, matRotZ, matRot;

    matRotX._11 = 1.f;
    matRotX._21 = 0.f;
    matRotX._31 = 0.f;

    matRotX._12 = 0.f;
    matRotX._22 = cos(ConvertAngle.x);
    matRotX._32 = -sin(ConvertAngle.x);

    matRotX._13 = 0.f;
    matRotX._23 = sin(ConvertAngle.x);
    matRotX._33 = cos(ConvertAngle.x);

    matRotY._11 = cos(ConvertAngle.y);
    matRotY._21 = 0.f;
    matRotY._31 = -sin(ConvertAngle.y);

    matRotY._12 = 0.f;
    matRotY._22 = 1.f;
    matRotY._32 = 0.f;

    matRotY._13 = sin(ConvertAngle.y);
    matRotY._23 = 0.f;
    matRotY._33 = cos(ConvertAngle.y);

    matRotZ._11 = cos(ConvertAngle.z);
    matRotZ._21 = -sin(ConvertAngle.z);
    matRotZ._31 = 0.f;

    matRotZ._12 = sin(ConvertAngle.z);
    matRotZ._22 = cos(ConvertAngle.z);
    matRotZ._32 = 0.f;

    matRotZ._13 = 0.f;
    matRotZ._23 = 0.f;
    matRotZ._33 = 1.f;

    matRot = mul(matRotX, matRotY);
    matRot = mul(matRot, matRotZ);

    return matRot;
}


[numthreads(64, 1, 1)]
void ParticleUpdateCS(int3 ThreadID : SV_DispatchThreadID)
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

        //float3 RandomPos = float3(Rand(Key), Rand(Key * 2.f), Rand(Key * 3.f));

        int Index = ThreadID.x;
        float3 RandomPos = float3(Rand(Index), Rand(Index), Rand(Index));

        float3 StartRange = g_ParticleStartMax - g_ParticleStartMin;

        float3 WorldPos = g_ParticleStartMin + StartRange * RandomPos;

        g_ParticleArray[ThreadID.x].WorldPos = WorldPos;

        g_ParticleArray[ThreadID.x].FallTime = 0.f;
        g_ParticleArray[ThreadID.x].FallStartY = WorldPos.y;

        g_ParticleArray[ThreadID.x].LifeTime = 0.f;
        g_ParticleArray[ThreadID.x].LifeTimeMax = RandomPos.x * (g_ParticleLifeTimeMax - g_ParticleLifeTimeMin) + g_ParticleLifeTimeMin;

        if (g_ParticleMoveEnable == 1)
        {
            if (g_ParticleMoveDirEnable == 1)
            {
                float3  RandAngle = g_ParticleMoveAngle * (RandomPos * 2.f - 1.f);

                float3x3    matRot = ComputeRotationMatrix(RandAngle);

                float3  Dir = normalize(mul(g_ParticleMoveDir, matRot));

                g_ParticleArray[ThreadID.x].Dir = Dir;
            }

            else
            {
                float3 Center = g_ParticleStartMin + StartRange * 0.5f;

                g_ParticleArray[ThreadID.x].Dir = normalize(WorldPos - Center);
            }

            g_ParticleArray[ThreadID.x].Speed = RandomPos.x * (g_ParticleSpeedMax - g_ParticleSpeedMin) + g_ParticleSpeedMin;
        }
    }

    // Ȱ��ȭ �Ǿ� ���� ���
    else
    {
        if (g_ParticleArray[ThreadID.x].LifeTimeMax != 0.f)
        {
            g_ParticleArray[ThreadID.x].LifeTime += g_GlobalDeltaTime;

            if (g_ParticleArray[ThreadID.x].LifeTime >= g_ParticleArray[ThreadID.x].LifeTimeMax)
                g_ParticleArray[ThreadID.x].Enable = 0;
        }

        float3  MovePos = (float3)0.f;

        // �����̴� ��ƼŬ�̶��.
        if (g_ParticleMoveEnable == 1)
        {
            MovePos = g_ParticleArray[ThreadID.x].Dir * g_ParticleArray[ThreadID.x].Speed * g_GlobalDeltaTime;
        }

        // �߷� ������ �� ���� �ƴ� ���� ������.
        if (g_ParticleShare[0].GravityEnable == 1)
        {
            g_ParticleArray[ThreadID.x].FallTime += g_GlobalDeltaTime;

            float Velocity = 0.f;

            if (g_ParticleArray[ThreadID.x].Dir.y > 0.f)
            {
                Velocity = g_ParticleArray[ThreadID.x].Speed * g_ParticleArray[ThreadID.x].FallTime * 0.5f;
            }

            g_ParticleArray[ThreadID.x].WorldPos.x += MovePos.x;
            g_ParticleArray[ThreadID.x].WorldPos.z += MovePos.z;

            g_ParticleArray[ThreadID.x].WorldPos.y =
                g_ParticleArray[ThreadID.x].FallStartY +
                Velocity - 0.5f * GRAVITY * g_ParticleArray[ThreadID.x].FallTime * g_ParticleArray[ThreadID.x].FallTime;
        }

        else
        {
            g_ParticleArray[ThreadID.x].WorldPos += MovePos;
        }
    }
}