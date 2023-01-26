#pragma once
#include "EditorWidget.h"

class CEditorCursorPos : public CEditorWidget
{
private:
	friend class CEditorWindow;
	friend class CEditorGroup;
	template <typename T>
	friend class CEditorTreeItem;
	ImVec2			m_Pos;
protected:
	CEditorCursorPos();
	virtual ~CEditorCursorPos();
public:
	void SetPos(const ImVec2& pos)
	{
		m_Pos = pos;
	}
	void SetPos(const Vector2& pos)
	{
		m_Pos.x = pos.x;
		m_Pos.y = pos.y;
	}
	void SetPos(float x, float y)
	{
		m_Pos.x = x;
		m_Pos.y = y;
	}
	void SetPosX(float x)
	{
		m_Pos.x = x;
	}
	void SetPosY(float y)
	{
		m_Pos.y = y;
	}
	virtual bool Init();
	virtual void Render();
};

