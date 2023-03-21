#include "TitleSceneInfo.h"

#include "Scene/Scene.h"
#include "../UI/TitleSceneUI.h"

CTitleSceneInfo::CTitleSceneInfo()
{
	m_ClassTypeName = "TitleSceneInfo";
}

CTitleSceneInfo::~CTitleSceneInfo()
{
}

bool CTitleSceneInfo::Init()
{
	CSceneInfo::Init();

	// Sound Load
	m_Owner->GetResource()->LoadSound("UI", "UI_Backward", false, "Sfx/UI/SFX_UI_Backward_001.ogg");
	m_Owner->GetResource()->LoadSound("BGM", "Title_Bgm", false, "Music/MUS_WavesSeagullsBg.ogg");
	
	CSound* Sound = m_Owner->GetResource()->FindSound("Title_Bgm");

	if (Sound)
		Sound->Play();

	m_Owner->GetViewport()->CreateUIWindow<CTitleSceneUI>("TitleSceneUI");


	// Background Charactor


    return true;
}
