#include "TitleSceneInfo.h"

#include "Scene/Scene.h"
#include "../UI/TitleSceneUI.h"
#include "../UI/Fade.h"

CTitleSceneInfo::CTitleSceneInfo()
{
	m_ClassTypeName = "TitleSceneInfo";
}

CTitleSceneInfo::~CTitleSceneInfo()
{
}

void CTitleSceneInfo::Start()
{
	m_Fade->SetState(EFade_State::FadeIn_Start);
}

bool CTitleSceneInfo::Init()
{
	CSceneInfo::Init();

	// Sound Load
	m_Owner->GetResource()->LoadSound("UI", "UI_Backward", false, "Sfx/SFX_UI_Backward_001.ogg");
	m_Owner->GetResource()->LoadSound("BGM", "Title_Bgm", false, "Music/MUS_WavesSeagullsBg.ogg");

	CSound* Sound = m_Owner->GetResource()->FindSound("Title_Bgm");

	if (Sound)
		Sound->Play();

	m_Owner->GetViewport()->CreateUIWindow<CTitleSceneUI>("TitleSceneUI");
	m_Fade = m_Owner->GetViewport()->CreateUIWindow<CFade>("Fade");

	// Background Charactor


	return true;
}
