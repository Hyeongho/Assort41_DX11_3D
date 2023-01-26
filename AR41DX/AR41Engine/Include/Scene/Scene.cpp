#include "Scene.h"
#include "../GameObject/GameObject.h"
#include "../Input.h"
#include "../Component/SpriteComponent.h"
#include "../Component/CameraComponent.h"
#include "../Component/TargetArm.h"
#include "../Component/SceneComponent.h"
#include "../Component/ColliderBox2D.h"
#include "../Component/TileMapComponent.h"
#include "../Component/NavigationAgent.h"
#include "../Component/LightComponent.h"
#include "../Component/RigidBody.h"
#include "../Animation/Animation2D.h"
#include "../UI/UIButton.h"
#include "../UI/UIImage.h"
#include "../UI/UIWindow.h"
#include "../PathManager.h"
#include "../GameObject/SkySphere.h"

std::unordered_map<std::string, CSceneInfo*> CScene::m_mapSceneInfoCDO;

CScene::CScene()	:
	m_Change(false),
	m_Start(false)
{
	m_Name = "Scene";

	m_SceneInfo = new CSceneInfo;

	m_SceneInfo->m_Owner = this;

	m_SceneInfo->Init();

	m_Resource = new CSceneResource;

	m_Resource->m_Owner = this;

	m_Resource->Init();

	m_CameraManager = new CCameraManager;

	m_CameraManager->m_Owner = this;

	m_CameraManager->Init();

	m_CollisionManager = new CSceneCollision;

	m_CollisionManager->m_Owner = this;

	m_CollisionManager->Init();

	m_Viewport = new CSceneViewport;

	m_Viewport->m_Owner = this;

	m_Viewport->Init();

	m_NavManager = new CNavigationManager;

	m_NavManager->m_Owner = this;

	m_NavManager->Init();

	m_LightManager = new CLightManager;

	m_LightManager->m_Owner = this;

	m_LightManager->Init();
}

CScene::~CScene()
{
	CInput::GetInst()->ClearCallback(this);

	SAFE_DELETE(m_LightManager);
	SAFE_DELETE(m_NavManager);
	SAFE_DELETE(m_Viewport);
	SAFE_DELETE(m_CollisionManager);
	SAFE_DELETE(m_CameraManager);
	SAFE_DELETE(m_Resource);
	SAFE_DELETE(m_SceneInfo);
}

void CScene::CreateCDO()
{
	// ==================== SceneInfo ====================
	CSceneInfo* Info = new CSceneInfo;

	Info->Init();

	CScene::AddSceneInfoCDO("SceneInfo", Info);


	// ==================== GameObject ====================
	CGameObject* ObjCDO = new CGameObject;

	ObjCDO->Init();

	CGameObject::AddObjectCDO("GameObject", ObjCDO);


	// ==================== Component ====================
	CComponent* ComCDO = new CSceneComponent;

	ComCDO->Init();

	CComponent::AddComponentCDO("SceneComponent", ComCDO);

	ComCDO = new CSpriteComponent;

	ComCDO->Init();

	CComponent::AddComponentCDO("SpriteComponent", ComCDO);

	ComCDO = new CCameraComponent;

	ComCDO->Init();

	CComponent::AddComponentCDO("CameraComponent", ComCDO);

	ComCDO = new CTargetArm;

	ComCDO->Init();

	CComponent::AddComponentCDO("TargetArm", ComCDO);

	ComCDO = new CColliderBox2D;

	ComCDO->Init();

	CComponent::AddComponentCDO("Box2D", ComCDO);

	ComCDO = new CTileMapComponent;

	ComCDO->Init();

	CComponent::AddComponentCDO("TileMapComponent", ComCDO);

	ComCDO = new CNavigationAgent;

	ComCDO->Init();

	CComponent::AddComponentCDO("NavigationAgent", ComCDO);

	ComCDO = new CLightComponent;

	ComCDO->Init();

	CComponent::AddComponentCDO("LightComponent", ComCDO);

	ComCDO = new CRigidBody;

	ComCDO->Init();

	CComponent::AddComponentCDO("RigidBody", ComCDO);
	// ==================== Animation ====================
	CAnimation2D* AnimCDO = new CAnimation2D;

	AnimCDO->Init();

	CAnimation2D::AddAnimationCDO("Animation2D", AnimCDO);


	// ==================== UI ====================
	CUIWindow* UIWindowCDO = new CUIWindow;

	UIWindowCDO->Init();

	CUIWindow::AddUIWindowCDO("UIWindow", UIWindowCDO);

	CUIWidget* UIWidgetCDO = new CUIButton;

	UIWidgetCDO->Init();

	CUIWidget::AddUIWidgetCDO("UIButton", UIWidgetCDO);

	UIWidgetCDO = new CUIImage;

	UIWidgetCDO->Init();

	CUIWidget::AddUIWidgetCDO("UIImage", UIWidgetCDO);

}

void CScene::SetSkyTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	m_SkySphere->SetSkyTexture(Name, FileName, PathName);
}

void CScene::ClearSky()
{
	m_SkySphere = nullptr;
}

void CScene::Start()
{
	m_Start = true;

	m_SkySphere = new CSkySphere;

	m_SkySphere->SetName("Sky");
	m_SkySphere->SetScene(this);

	m_SkySphere->Init();

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->Start();
	}

	m_CameraManager->Start();

	m_Viewport->Start();

	m_LightManager->Start();
}

bool CScene::Init()
{


	return true;
}

void CScene::Update(float DeltaTime)
{
	if (m_SceneInfo)
	{
		m_SceneInfo->Update(DeltaTime);
	}

	m_SkySphere->Update(DeltaTime);

	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter)->Update(DeltaTime);
		iter++;
	}

	m_CameraManager->Update(DeltaTime);

	m_Viewport->Update(DeltaTime);
}

void CScene::PostUpdate(float DeltaTime)
{
	if (m_SceneInfo)
	{
		m_SceneInfo->PostUpdate(DeltaTime);
	}

	m_SkySphere->PostUpdate(DeltaTime);

	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter)->PostUpdate(DeltaTime);
		iter++;
	}

	m_CameraManager->PostUpdate(DeltaTime);

	m_Viewport->PostUpdate(DeltaTime);

	m_LightManager->Update(DeltaTime);
}

void CScene::Collision(float DeltaTime)
{
	m_CollisionManager->Update(DeltaTime);
}

void CScene::Save(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "wb");

	if (!File)
		return;

	// 이름 저장
	int	Length = (int)m_Name.length();

	fwrite(&Length, 4, 1, File);
	fwrite(m_Name.c_str(), 1, Length, File);

	// SceneInfo 저장
	m_SceneInfo->Save(File);
	m_CameraManager->Save(File);
	m_CollisionManager->Save(File);
	m_Viewport->Save(File);
	m_LightManager->Save(File);

	int	ObjCount = (int)m_ObjList.size();

	fwrite(&ObjCount, 4, 1, File);

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; iter++)
	{
		std::string	ClassTypeName = (*iter)->GetObjectTypeName();

		Length = (int)ClassTypeName.length();

		fwrite(&Length, 4, 1, File);
		fwrite(ClassTypeName.c_str(), 1, Length, File);

		(*iter)->Save(File);
	}

	fclose(File);
}

void CScene::Load(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "rb");

	if (!File)
		return;

	fseek(File, 0, SEEK_END);

	int	FileSize = (int)ftell(File);

	fseek(File, 0, SEEK_SET);

	int	LoadSize = 0;


	// 이름 저장
	int	Length = 0;
	char	Name[256] = {};

	fread(&Length, 4, 1, File);
	fread(Name, 1, Length, File);

	LoadSize += 4 + Length;

	if (m_LoadingCallback)
		m_LoadingCallback(LoadSize / (float)FileSize);

	m_Name = Name;

	// SceneInfo 저장
	Length = 0;
	char	SceneInfoName[256] = {};

	fread(&Length, 4, 1, File);
	fread(SceneInfoName, 1, Length, File);

	LoadSize += 4 + Length;

	if (m_LoadingCallback)
		m_LoadingCallback(LoadSize / (float)FileSize);

	SAFE_DELETE(m_SceneInfo);

	CSceneInfo* CDO = FindSceneInfoCDO(SceneInfoName);

	if (!CDO)
		CDO = FindSceneInfoCDO("SceneInfo");

	m_SceneInfo = CDO->Clone();

	m_SceneInfo->m_Owner = this;

	int	CurPos = (int)ftell(File);

	m_SceneInfo->Load(File);

	int	NextPos = (int)ftell(File);

	int	CurLoadSize = NextPos - CurPos;

	if (CurLoadSize > 0)
	{
		LoadSize += CurLoadSize;

		if (m_LoadingCallback)
			m_LoadingCallback(LoadSize / (float)FileSize);
	}

	CurPos = NextPos;

	m_CameraManager->m_Owner = this;
	m_CameraManager->Load(File);

	NextPos = (int)ftell(File);

	CurLoadSize = NextPos - CurPos;

	if (CurLoadSize > 0)
	{
		LoadSize += CurLoadSize;

		if (m_LoadingCallback)
			m_LoadingCallback(LoadSize / (float)FileSize);
	}

	CurPos = NextPos;

	m_CollisionManager->m_Owner = this;
	m_CollisionManager->Load(File);

	NextPos = (int)ftell(File);

	CurLoadSize = NextPos - CurPos;

	if (CurLoadSize > 0)
	{
		LoadSize += CurLoadSize;

		if (m_LoadingCallback)
			m_LoadingCallback(LoadSize / (float)FileSize);
	}

	CurPos = NextPos;

	m_Viewport->m_Owner = this;
	m_Viewport->Load(File);

	NextPos = (int)ftell(File);

	CurLoadSize = NextPos - CurPos;

	if (CurLoadSize > 0)
	{
		LoadSize += CurLoadSize;

		if (m_LoadingCallback)
			m_LoadingCallback(LoadSize / (float)FileSize);
	}

	CurPos = NextPos;

	m_LightManager->m_Owner = this;
	m_LightManager->Load(File);

	NextPos = (int)ftell(File);

	CurLoadSize = NextPos - CurPos;

	if (CurLoadSize > 0)
	{
		LoadSize += CurLoadSize;

		if (m_LoadingCallback)
			m_LoadingCallback(LoadSize / (float)FileSize);
	}

	CurPos = NextPos;

	int	ObjCount = 0;

	fread(&ObjCount, 4, 1, File);

	LoadSize += 4;

	NextPos += 4;

	if (m_LoadingCallback)
		m_LoadingCallback(LoadSize / (float)FileSize);

	CurPos = NextPos;

	for (int i = 0; i < ObjCount; ++i)
	{
		Length = 0;

		char	ObjClassTypeName[256] = {};

		fread(&Length, 4, 1, File);
		fread(ObjClassTypeName, 1, Length, File);


		LoadSize += 4 + Length;
		NextPos += 4 + Length;

		if (m_LoadingCallback)
			m_LoadingCallback(LoadSize / (float)FileSize);

		CurPos = NextPos;

		CGameObject* ObjCDO = CGameObject::FindCDO(ObjClassTypeName);

		CGameObject* NewObj = ObjCDO->Clone();

		NewObj->SetScene(this);

		NewObj->Load(File);

		NextPos = (int)ftell(File);

		CurLoadSize = NextPos - CurPos;

		if (CurLoadSize > 0)
		{
			LoadSize += CurLoadSize;

			if (m_LoadingCallback)
				m_LoadingCallback(LoadSize / (float)FileSize);
		}

		CurPos = NextPos;

		m_ObjList.push_back(NewObj);
	}

	m_SceneInfo->LoadComplete();

	fclose(File);
}

void CScene::Save(const char* FileName, const std::string& PathName)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	char	FullPath[MAX_PATH] = {};

	if (Path)
		strcpy_s(FullPath, Path->PathMultibyte);

	strcat_s(FullPath, FileName);

	Save(FullPath);
}

void CScene::Load(const char* FileName, const std::string& PathName)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	char	FullPath[MAX_PATH] = {};

	if (Path)
		strcpy_s(FullPath, Path->PathMultibyte);

	strcat_s(FullPath, FileName);

	Load(FullPath);
}

void CScene::GetAllGameObjectHierarchyName(std::vector<HierarchyObjectName>& vecName)
{
	auto    iter = m_ObjList.begin();
	auto    iterEnd = m_ObjList.end();

	for (; iter != iterEnd; iter++)
	{
		HierarchyObjectName	Names;

		//CGameObject* Parent = (*iter)->GetParent();

		Names.Name = (*iter)->GetName();
		Names.ClassName = (*iter)->GetObjectTypeName();
		Names.Obj= *iter;
		//Names.Parent = Parent;

		/*if (Parent)
		{
			Names.ParentName = Parent->GetName();
			Names.ParentClassName = Parent->GetComponentTypeName();
		}*/

		vecName.push_back(Names);
	}
}

CGameObject* CScene::FindObject(const std::string& Name)
{
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->GetName() == Name)
			return *iter;
	}

	return nullptr;
}
