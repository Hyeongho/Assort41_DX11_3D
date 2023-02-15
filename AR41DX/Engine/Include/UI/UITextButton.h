#pragma once

#include "UIWidget.h"
#include "../Resource/Font/Font.h"
#include "UIButton.h"
#include "UIText.h"

struct UIWidgetTextInfo
{
	TCHAR*		m_Text;
	int			m_TextCount;
	int			m_TextCapacity;

	float       m_FontSize;
	float       m_Opacity;
	Vector4     m_Color;
	ID2D1SolidColorBrush* m_Brush;

	Text_Align_H    m_AlignH;
	Text_Align_V    m_AlignV;

	// Shadow
	ID2D1SolidColorBrush* m_ShadowBrush;
	Vector4     m_ShadowColor;
	Vector2     m_ShadowOffset;


	UIWidgetTextInfo() :
		m_FontSize(10.f),
		m_Opacity(1.f),
		m_Color(Vector4::Black),
		m_Brush(nullptr),
		m_AlignH(Text_Align_H::Center),
		m_AlignV(Text_Align_V::Middle),
		m_ShadowBrush(nullptr),
		m_ShadowColor(Vector4::Black),
		m_ShadowOffset(1.1f, 1.1f)
	{
		m_Color.w = m_Opacity;

		m_TextCapacity = 32;
		m_TextCount = 0;

		m_Text = new TCHAR[m_TextCapacity];
	}
};

class CUITextButton :
	public CUIWidget
{
	friend class CUIWindow;
	friend class CScene;

protected:
	CUITextButton();
	CUITextButton(const CUITextButton& Button);
	virtual ~CUITextButton();

protected:
	CSharedPtr<CFont> m_Font;
	IDWriteTextLayout* m_Layout;
	ID2D1RenderTarget* m_RenderTarget;

protected:
	UIWidgetTextInfo            m_TextInfo[(int)EButtonState::Max];
	CSharedPtr<class CSound>    m_Sound[(int)EButtonEventState::Max];
	EButtonState                m_State;
	std::function<void()>       m_ClickCallback[(int)EButtonEventState::Max];
	bool						m_MouseHovered;

public:
	void SetFont(const std::string& Name);
	void SetText(EButtonState State, const TCHAR* Text, float Size, const Vector4& Color);

private:
	void CreateLayout(EButtonState State);

public:
	void ButtonEnable(bool Enable)
	{
		m_State = Enable ? EButtonState::Normal : EButtonState::Disable;
	}

	void SetSound(EButtonEventState State, class CSound* Sound);
	void SetSound(EButtonEventState State, const std::string& Name);
	bool SetSound(EButtonEventState State, const std::string& GroupName, const std::string& Name, bool Loop, const char* FileName, const std::string& PathName = SOUND_PATH);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CUITextButton* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	template <typename T>
	void SetCallback(EButtonEventState State, T* Obj, void(T::* Func)())
	{
		m_ClickCallback[(int)State] = std::bind(Func, Obj);
	}

};

