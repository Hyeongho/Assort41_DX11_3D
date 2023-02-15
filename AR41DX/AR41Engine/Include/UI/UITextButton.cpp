#include "UITextButton.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"
#include "../Device.h"


CUITextButton::CUITextButton() :
    m_Layout(nullptr),
    m_RenderTarget(nullptr),
    m_State(EButtonState::Normal),
    m_MouseHovered(false)
{
    m_WidgetTypeName = "UITextButton";
}

CUITextButton::CUITextButton(const CUITextButton& Button) :
    CUIWidget(Button)
{
    m_Font = Button.m_Font;
    m_Layout = nullptr;
    m_RenderTarget = Button.m_RenderTarget;

    m_State = Button.m_State;
    m_MouseHovered = false;

    for (int i = 0; i < (int)EButtonState::Max; ++i)
    {
        m_TextInfo[i] = Button.m_TextInfo[i];
    }

    for (int i = 0; i < (int)EButtonEventState::Max; ++i)
    {
        m_Sound[i] = Button.m_Sound[i];
    }
}

CUITextButton::~CUITextButton()
{
    SAFE_RELEASE(m_Layout);

    for (UIWidgetTextInfo iter : m_TextInfo) {
        SAFE_DELETE_ARRAY(iter.m_Text);
    }
}

void CUITextButton::SetFont(const std::string& Name)
{
    if (m_Scene)
    {
        m_Font = m_Scene->GetResource()->FindFont(Name);
    }

    else
    {
        m_Font = CResourceManager::GetInst()->FindFont(Name);
    }

}

void CUITextButton::SetText(EButtonState State, const TCHAR* Text, float Size, const Vector4& Color)
{
    m_TextInfo[(int)State];

    int Length = lstrlen(Text);

    if (Length + 1 > m_TextInfo[(int)State].m_TextCapacity)
    {
        m_TextInfo[(int)State].m_TextCapacity = Length + 1;

        SAFE_DELETE_ARRAY(m_TextInfo[(int)State].m_Text);

        m_TextInfo[(int)State].m_Text = new TCHAR[m_TextInfo[(int)State].m_TextCapacity];

        memset(m_TextInfo[(int)State].m_Text, 0, sizeof(TCHAR) * m_TextInfo[(int)State].m_TextCapacity);
    }

    lstrcpy(m_TextInfo[(int)State].m_Text, Text);
    m_TextInfo[(int)State].m_TextCount = Length;

    m_TextInfo[(int)State].m_FontSize = Size;
    m_TextInfo[(int)State].m_Color = Color;


    if (m_Scene)
    {
        m_Scene->GetResource()->CreateFontColor(Color);
        m_TextInfo[(int)State].m_Brush = m_Scene->GetResource()->FindFontColor(Color);
        m_TextInfo[(int)State].m_ShadowBrush = m_Scene->GetResource()->FindFontColor(m_TextInfo[(int)State].m_ShadowColor);
    }
    else
    {
        CResourceManager::GetInst()->CreateFontColor(Color);
        m_TextInfo[(int)State].m_Brush = CResourceManager::GetInst()->FindFontColor(Color);
        m_TextInfo[(int)State].m_ShadowBrush = CResourceManager::GetInst()->FindFontColor(m_TextInfo[(int)State].m_ShadowColor);
    }

    CreateLayout(State);
}

void CUITextButton::CreateLayout(EButtonState State)
{
    SAFE_RELEASE(m_Layout);

    m_Layout = m_Font->CreateLayout(m_TextInfo[(int)State].m_Text, m_Size.x, m_Size.y);

    DWRITE_TEXT_RANGE   Range = {};

    Range.startPosition = 0;
    Range.length = lstrlen(m_TextInfo[(int)State].m_Text);

    m_Layout->SetFontSize(m_TextInfo[(int)State].m_FontSize, Range);

    switch (m_TextInfo[(int)State].m_AlignH)
    {
    case Text_Align_H::Left:
        m_Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        break;
    case Text_Align_H::Center:
        m_Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        break;
    case Text_Align_H::Right:
        m_Layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
        break;
    }

    switch (m_TextInfo[(int)State].m_AlignV)
    {
    case Text_Align_V::Top:
        m_Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        break;
    case Text_Align_V::Middle:
        m_Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        break;
    case Text_Align_V::Bottom:
        m_Layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
        break;
    }
}


void CUITextButton::SetSound(EButtonEventState State, CSound* Sound)
{
    m_Sound[(int)State] = Sound;
}

void CUITextButton::SetSound(EButtonEventState State, const std::string& Name)
{
    if (m_Scene)
        m_Sound[(int)State] = m_Scene->GetResource()->FindSound(Name);

    else
        m_Sound[(int)State] = CResourceManager::GetInst()->FindSound(Name);
}

bool CUITextButton::SetSound(EButtonEventState State, const std::string& GroupName, const std::string& Name, bool Loop, const char* FileName, const std::string& PathName)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadSound(GroupName, Name, Loop, FileName, PathName))
            return false;

        m_Sound[(int)State] = m_Scene->GetResource()->FindSound(Name);
    }

    else
    {
        if (!CResourceManager::GetInst()->LoadSound(GroupName, Name, Loop, FileName, PathName))
            return false;

        m_Sound[(int)State] = CResourceManager::GetInst()->FindSound(Name);
    }

    return true;
}

void CUITextButton::Start()
{
    CUIWidget::Start();
}

bool CUITextButton::Init()
{
    if (!CUIWidget::Init())
        return false;

    m_RenderTarget = CDevice::GetInst()->Get2DTarget();

    SetFont("Default");


    if (m_Scene)
    {
        m_Scene->GetResource()->CreateFontColor(Vector4::Black);
        m_TextInfo[(int)m_State].m_Brush = m_Scene->GetResource()->FindFontColor(Vector4::Black);
        m_TextInfo[(int)m_State].m_ShadowBrush = m_Scene->GetResource()->FindFontColor(Vector4::Black);
    }
    else
    {
        CResourceManager::GetInst()->CreateFontColor(Vector4::Black);
        m_TextInfo[(int)m_State].m_Brush = CResourceManager::GetInst()->FindFontColor(Vector4::Black);
        m_TextInfo[(int)m_State].m_ShadowBrush = CResourceManager::GetInst()->FindFontColor(Vector4::Black);
    }

    return true;
}

void CUITextButton::Update(float DeltaTime)
{
    CUIWidget::Update(DeltaTime);

    if (m_State != EButtonState::Disable)
    {
        // 마우스가 버튼 위에 올라왔는지 체크한다.
        Vector2 MousePos = CInput::GetInst()->GetMousePos();

        bool    MouseHovered = true;

        if (m_RenderPos.x > MousePos.x)
            MouseHovered = false;

        else if (m_RenderPos.y > MousePos.y)
            MouseHovered = false;

        else if (m_RenderPos.x + m_Size.x < MousePos.x)
            MouseHovered = false;

        else if (m_RenderPos.y + m_Size.y < MousePos.y)
            MouseHovered = false;


        if (MouseHovered)
        {
            if (CInput::GetInst()->GetMouseLDown())
            {
                m_State = EButtonState::Click;
                CreateLayout(m_State);
            }

            else if (m_State == EButtonState::Click && CInput::GetInst()->GetMouseLUp())
            {
                if (m_Sound[(int)EButtonEventState::Click])
                    m_Sound[(int)EButtonEventState::Click]->Play();

                if (m_ClickCallback[(int)EButtonEventState::Click])
                    m_ClickCallback[(int)EButtonEventState::Click]();

                m_State = EButtonState::Hovered;
                CreateLayout(m_State);
            }

            else if (m_State == EButtonState::Click && CInput::GetInst()->GetMouseLPush())
                m_State = EButtonState::Click;

            else
            {
                if (m_MouseHovered != MouseHovered)
                {
                    if (m_Sound[(int)EButtonEventState::Hovered])
                        m_Sound[(int)EButtonEventState::Hovered]->Play();
                }

                if (m_ClickCallback[(int)EButtonEventState::Hovered])
                    m_ClickCallback[(int)EButtonEventState::Hovered]();

                m_State = EButtonState::Hovered;
                CreateLayout(m_State);
            }

            m_MouseHovered = true;
        }

        else
        {
            m_MouseHovered = false;

            m_State = EButtonState::Normal;
            CreateLayout(m_State);
        }
    }
}

void CUITextButton::PostUpdate(float DeltaTime)
{
    CUIWidget::PostUpdate(DeltaTime);
}

void CUITextButton::Render()
{
    m_RenderTarget->BeginDraw();

    Resolution RS = CDevice::GetInst()->GetResolution();

    // Text
    D2D1_POINT_2F   Point;
    Point.x = m_RenderPos.x;
    Point.y = RS.Height - m_RenderPos.y - m_Size.y;

    // Shadow
    {
        D2D1_POINT_2F   ShadowPoint = Point;

        ShadowPoint.x += m_TextInfo[(int)m_State].m_ShadowOffset.x;
        ShadowPoint.y += m_TextInfo[(int)m_State].m_ShadowOffset.y;

        m_TextInfo[(int)m_State].m_ShadowBrush->SetOpacity(1.f);

        m_RenderTarget->DrawTextLayout(ShadowPoint, m_Layout, m_TextInfo[(int)m_State].m_ShadowBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
    }

    m_RenderTarget->DrawTextLayout(Point, m_Layout, m_TextInfo[(int)m_State].m_Brush, D2D1_DRAW_TEXT_OPTIONS_NONE);

    m_RenderTarget->EndDraw();
}

CUITextButton* CUITextButton::Clone()
{
    return new CUITextButton(*this);
}

void CUITextButton::Save(FILE* File)
{
}

void CUITextButton::Load(FILE* File)
{
}
