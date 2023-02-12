#pragma once

#include "ConstantBufferData.h"

class CParticleConstantBuffer :
    public CConstantBufferData
{
public:
	CParticleConstantBuffer();
	CParticleConstantBuffer(const CParticleConstantBuffer& Buffer);
	virtual ~CParticleConstantBuffer();

private:
	ParticleCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CParticleConstantBuffer* Clone();

public:
	void SetParticleEnable(bool Enable)
	{
		m_BufferData.ParticleSpawnEnable = Enable ? 1 : 0;
	}

	void SetParticleStartMin(const Vector3& Min)
	{
		m_BufferData.ParticleStartMin = Min;
	}

	void SetParticleStartMax(const Vector3& Max)
	{
		m_BufferData.ParticleStartMax = Max;
	}

	void SetParticleSpawnCountMax(unsigned int Count)
	{
		m_BufferData.ParticleSpawnCountMax = Count;
	}

	void SetParticleScaleMin(const Vector3& Min)
	{
		m_BufferData.ParticleScaleMin = Min;
	}

	void SetParticleScaleMax(const Vector3& Max)
	{
		m_BufferData.ParticleScaleMax = Max;
	}

	void SetParticleLifeTimeMin(float Min)
	{
		m_BufferData.ParticleLifeTimeMin = Min;
	}

	void SetParticleLifeTimeMax(float Max)
	{
		m_BufferData.ParticleLifeTimeMax = Max;
	}

	void SetParticleColorMin(const Vector4& Min)
	{
		m_BufferData.ParticleColorMin = Min;
	}

	void SetParticleColorMax(const Vector4& Max)
	{
		m_BufferData.ParticleColorMax = Max;
	}

	void SetParticleSpeedMin(float Min)
	{
		m_BufferData.ParticleSpeedMin = Min;
	}

	void SetParticleSpeedMax(float Max)
	{
		m_BufferData.ParticleSpeedMax = Max;
	}

	void SetParticleMoveEnable(bool Enable)
	{
		m_BufferData.ParticleMoveEnable = Enable ? 1 : 0;
	}

	void SetParticleGravityEnable(bool Enable)
	{
		m_BufferData.ParticleGravity = Enable ? 1 : 0;
	}

	void SetParticleMoveDir(const Vector3& MoveDir)
	{
		m_BufferData.ParticleMoveDir = MoveDir;
	}

	void SetParticleMoveDirEnable(bool Enable)
	{
		m_BufferData.ParticleMoveDirEnable = Enable ? 1 : 0;
	}

	void SetParticleMoveAngle(const Vector3& MoveAngle)
	{
		m_BufferData.ParticleMoveAngle = MoveAngle;
	}
};

