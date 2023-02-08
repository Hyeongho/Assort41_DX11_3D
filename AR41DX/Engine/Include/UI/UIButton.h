#pragma once

#include "UIWidget.h"

enum class EButtonState
{
    Normal,
    Hovered,
    Click,
    Disable,
    Max
};

enum class EButtonEventState
{
    Hovered,
    Click,
    Max
};

class CUIButton :
    public CUIWidget
{
    friend class CUIWindow;
    friend class CScene;

protected:
    CUIButton();
    CUIButton(const CUIButton& Button);
    virtual ~CUIButton();

protected:
    UIWidgetImageInfo m_TextureInfo[(int)EButtonState::Max];
    CSharedPtr<class CSound> m_Sound[(int)EButtonEventState::Max];
    EButtonState m_State;
    std::function<void()> m_ClickCallback[(int)EButtonEventState::Max];
    bool m_MouseHovered;

public:
    UIWidgetImageInfo GetTextureInfo(int index)	const
    {
        return m_TextureInfo[index];
    }
    EButtonState GetState()	const
    {
        return m_State;
    }
    bool GetButtonEnable() const
    {
        return m_State != EButtonState::Disable;
    }
    void ButtonEnable(bool Enable)
    {
        m_State = Enable ? EButtonState::Normal : EButtonState::Disable;
    }
    void SetTexture(EButtonState State, CTexture* Texture);
    bool SetTexture(EButtonState State, const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetTextureFullPath(EButtonState State, const std::string& Name, const TCHAR* FullPath);
    bool SetTexture(EButtonState State, const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetTextureFullPath(EButtonState State, const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);
    void SetImageTint(EButtonState State, const Vector4& Tint);
    void SetImageTint(EButtonState State, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void AddFrameData(EButtonState State, const Vector2& Start, const Vector2& End);
    void SetPlayTime(EButtonState State, float PlayTime);
    void SetPlayScale(EButtonState State, float PlayScale);
    void SetSound(EButtonEventState State, class CSound* Sound);
    void SetSound(EButtonEventState State, const std::string& Name);
    bool SetSound(EButtonEventState State, const std::string& GroupName, const std::string& Name, bool Loop,
        const char* FileName, const std::string& PathName = SOUND_PATH);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CUIButton* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    template <typename T>
    void SetCallback(EButtonEventState State, T* Obj, void(T::* Func)())
    {
        m_ClickCallback[(int)State] = std::bind(Func, Obj);
    }
};

