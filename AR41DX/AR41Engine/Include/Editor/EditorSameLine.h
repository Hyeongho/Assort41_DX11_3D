#pragma once
#include "EditorWidget.h"
class CEditorSameLine :
    public CEditorWidget
{
	friend class CEditorWindow;
	friend class CEditorGroup;

	template <typename T>
	friend class CEditorTreeItem;

protected:
	CEditorSameLine();
	virtual ~CEditorSameLine();

private:
	// Offset은 현재 라인의 가장 왼쪽으로부터 어느정도의 위치에 표현할 것인지를 나타낸다.
	float	m_OffsetX;

	// 왼쪽의 위젯으로부터 얼마나 떨어질지를 결정한다.
	float	m_Spacing;

public:
	void SetOffsetX(float x)
	{
		m_OffsetX = x;
	}

	void SetSpacing(float Spacing)
	{
		m_Spacing = Spacing;
	}

public:
	virtual bool Init();
	virtual void Render();
};

