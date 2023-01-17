#pragma once
#include "ConstantBufferData.h"
class CTransformConstantBuffer :
    public CConstantBufferData
{
public:
	CTransformConstantBuffer();
	CTransformConstantBuffer(const CTransformConstantBuffer& Buffer);
	virtual ~CTransformConstantBuffer();

private:
	TransformCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CTransformConstantBuffer* Clone();

public:
	void SetWorldMatrix(const Matrix& matWorld)
	{
		m_BufferData.matWorld = matWorld;
	}

	void SetViewMatrix(const Matrix& matView)
	{
		m_BufferData.matView = matView;
	}

	void SetProjMatrix(const Matrix& matProj)
	{
		m_BufferData.matProj = matProj;
		m_BufferData.Proj11 = matProj._11;
		m_BufferData.Proj22 = matProj._22;
	}

	void SetPivot(const Vector3& Pivot)
	{
		m_BufferData.Pivot = Pivot;
	}

	void SetMeshSize(const Vector3& MeshSize)
	{
		m_BufferData.MeshSize = MeshSize;
	}
};

