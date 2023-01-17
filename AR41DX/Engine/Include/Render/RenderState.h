#pragma once

#include "../Ref.h"

class CRenderState	:
	public CRef
{
	friend class CRenderStateManager;

protected:
	CRenderState();
	virtual ~CRenderState();

protected:
	ID3D11DeviceChild* m_State;
	ID3D11DeviceChild* m_PrevState;
	ERenderStateType	m_Type;

public:
	ERenderStateType GetType()	const
	{
		return m_Type;
	}

public:
	virtual void SetState() = 0;
	virtual void ResetState() = 0;
};

