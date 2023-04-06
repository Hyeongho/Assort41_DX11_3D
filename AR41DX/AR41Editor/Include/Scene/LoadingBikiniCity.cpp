#include "LoadingBikiniCity.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"
#include "../UI/LoadingUI.h"
#include "Thread/ThreadManager.h"
#include "Thread/LoadingThread.h"
#include "Thread/ThreadQueue.h"
#include "Thread/DataStream.h"
#include "Editor/EditorGUIManager.h"
#include "Component/ParticleComponent.h"
#include "Component/SpriteComponent.h"
//#include "Component/BillboardComponent.h"
//#include "Component/TileMapComponent.h"
#include "GameObject/GameObject.h"
#include "GameObject/SkySphere.h"

#ifdef __has_include
#	if __has_include("../Window/ObjectWindow.h")
#		include "../Window/ObjectWindow.h"
#	endif
#endif

CLoadingBikiniCity::CLoadingBikiniCity()
{
}

CLoadingBikiniCity::~CLoadingBikiniCity()
{
}

bool CLoadingBikiniCity::Init()
{
	return false;
}

void CLoadingBikiniCity::Update(float DeltaTime)
{
}

void CLoadingBikiniCity::SceneChangeComplete()
{
}
