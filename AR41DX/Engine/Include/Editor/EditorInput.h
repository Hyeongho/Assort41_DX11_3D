#pragma once

#include "EditorWidget.h"

enum class EImGuiInputType
{
	String,
	Int,
	Float
};

class CEditorInput :
    public CEditorWidget
{
	friend class CEditorWindow;
	friend class CEditorGroup;

	template <typename T>
	friend class CEditorTreeItem;

protected:
	CEditorInput();
	virtual ~CEditorInput();

private:
	char		m_Text[1024];
	wchar_t		m_wText[1024];
	char		m_TextUTF8[1024];

	char		m_HintText[1024];
	wchar_t		m_HintwText[1024];
	char		m_HintTextUTF8[1024];
	bool			m_MultiLine;
	ImGuiInputTextFlags	m_Flag;

	EImGuiInputType	m_InputType;

	int				m_ValueInt;
	float			m_ValueFloat;
	std::function<void()>	m_InputCallback;

public:
	const char* GetText()	const
	{
		return m_Text;
	}

	const wchar_t* GetWText()	const
	{
		return m_wText;
	}

	const char* GetTextUTF8()	const
	{
		return m_TextUTF8;
	}

	bool GetMultiLine()	const
	{
		return m_MultiLine;
	}

	EImGuiInputType GetInputType()	const
	{
		return m_InputType;
	}

	int GetInt()	const
	{
		return m_ValueInt;
	}

	float GetFloat()	const
	{
		return m_ValueFloat;
	}


public:
	void SetText(const char* Text);
	void SetHintText(const char* Text);
	void SetInt(int Value);
	void SetFloat(float Value);
	void SetMultiLine(bool MultiLine);
	void AddFlag(ImGuiInputTextFlags_ Flag);
	void ReadOnly(bool Enable);
	void PasswordMode(bool Enable);
	void SetInputType(EImGuiInputType Type);
	bool Empty()	const;

public:
	virtual bool Init();
	virtual void Render();

public:
	template <typename T>
	void SetInputCallback(T* Obj, void(T::* Func)())
	{
		m_InputCallback = std::bind(Func, Obj);
	}
};

