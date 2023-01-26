#pragma once
#include "EditorWidget.h"
class CEditorCheckBox :
    public CEditorWidget
{
	friend class CEditorWindow;
	friend class CEditorGroup;

	template <typename T>
	friend class CEditorTreeItem;

protected:
	CEditorCheckBox();
	virtual ~CEditorCheckBox();

private:
	std::function<void(bool)>	m_CheckCallback;
	bool					m_Check;

public:
	bool GetCheck()	const
	{
		return m_Check;
	}
	void SetCheck(bool b)
	{
		m_Check = b;
	}
public:
	virtual bool Init();
	virtual void Render();

public:
	template <typename T>
	void SetCheckCallback(T* Obj, void(T::* Func)(bool))
	{
		m_CheckCallback = std::bind(Func, Obj);
	}
};

