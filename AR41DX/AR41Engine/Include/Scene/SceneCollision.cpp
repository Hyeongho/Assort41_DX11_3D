#include "SceneCollision.h"
#include "CollisionSection2D.h"
#include "CollisionSection3D.h"
#include "../Device.h"
#include "../Component/Collider2D.h"
#include "../Component/Collider3D.h"
#include "../PathManager.h"
#include "../Input.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SceneViewport.h"
#include "../CollisionManager.h"
#include "CameraManager.h"
#include "../Component/CameraComponent.h"

CSceneCollision::CSceneCollision()	:
	m_CollisionWidget(false)
{
}

CSceneCollision::~CSceneCollision()
{
	size_t	Count = m_Section2D.vecSection.size();

	for (size_t i = 0; i < Count; ++i)
	{
		SAFE_DELETE(m_Section2D.vecSection[i]);
	}

	Count = m_Section3D.vecSection.size();

	for (size_t i = 0; i < Count; ++i)
	{
		SAFE_DELETE(m_Section3D.vecSection[i]);
	}

	auto	iter = m_mapPixelCollision.begin();
	auto	iterEnd = m_mapPixelCollision.end();

	for (; iter != iterEnd; iter++)
	{
		--iter->second->RefCount;

		if (iter->second->RefCount == 0)
		{
			SAFE_RELEASE(iter->second->SRV);
			SAFE_DELETE_ARRAY(iter->second->Pixel);
			SAFE_DELETE(iter->second);
		}
	}

	m_mapPixelCollision.clear();
}

void CSceneCollision::AddCollider(CCollider* Collider)
{
	m_ColliderList.push_back(Collider);
}

bool CSceneCollision::CollisionWidget()
{
	return m_CollisionWidget = m_Owner->GetViewport()->CollisionMouse();
}

bool CSceneCollision::Init()
{
	float Width = (float)CDevice::GetInst()->GetResolution().Width;
	float Height = (float)CDevice::GetInst()->GetResolution().Height;

	CreateSection2D(10, 10, Vector2(), Vector2(Width, Height));

	CreateSection3D(5, 5, 5, Vector3(), Vector3(1000.f, 1000.f, 1000.f));

	return true;
}

void CSceneCollision::Update(float DeltaTime)
{
	for (int i = 0; i < m_Section2D.Count; ++i)
	{
		m_Section2D.vecSection[i]->m_ColliderList.clear();
	}

	for (int i = 0; i < m_Section3D.Count; ++i)
	{
		m_Section3D.vecSection[i]->m_ColliderList.clear();
	}

	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			// 이전프레임에 충돌되고 있던 물체가 있다면 해당 물체들에게 충돌이 해제되었다는 통보를
			// 보내야 한다.
			(*iter)->SendPrevCollisionEnd();

			iter++;
			continue;
		}

		CheckSection(*iter);
		iter++;
	}

	// 현재 충돌영역이 겹치는지 판단한다. 이전프레임에 충돌되고 있던 물체와 충돌영역이 겹치는게 없다면
	// 현재 프레임에서는 충돌체크 자체를 아예 안하고 충돌 가능성도 0% 이기 때문에 충돌하던 물체가 떨어졌다는
	// 처리를 진행해주어야 한다.
	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; iter++)
	{
		if (!(*iter)->GetEnable())
			continue;

		(*iter)->CheckPrevCollisionColliderSection();
	}

	// 마우스와 먼저 충돌처리를 진행해야 한다.
	CollisionMouse(DeltaTime);


	// 충돌체끼리 충돌처리를 진행한다.
	// 모든 Section을 반복하며 충돌처리를 수행한다.
	for (int i = 0; i < m_Section2D.Count; ++i)
	{
		m_Section2D.vecSection[i]->Collision(DeltaTime);
	}

	for (int i = 0; i < m_Section3D.Count; ++i)
	{
		m_Section3D.vecSection[i]->Collision(DeltaTime);
	}

	// 충돌처리가 완료되었다면 충돌체를 Clear해준다.
	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; iter++)
	{
		if (!(*iter)->GetEnable())
			continue;

		(*iter)->ClearFrame();
	}
}

void CSceneCollision::CreateSection2D(int CountX, int CountY, const Vector2& WorldStart, const Vector2& SectionSize)
{
	size_t	Count = m_Section2D.vecSection.size();

	for (size_t i = 0; i < Count; ++i)
	{
		SAFE_DELETE(m_Section2D.vecSection[i]);
	}

	m_Section2D.CountX = CountX;
	m_Section2D.CountY = CountY;
	m_Section2D.Count = CountX * CountY;
	m_Section2D.WorldStart = WorldStart;
	m_Section2D.SectionSize = SectionSize;
	m_Section2D.WorldSize = SectionSize * Vector2((float)CountX, (float)CountY);

	for (int i = 0; i < CountY; ++i)
	{
		for (int j = 0; j < CountX; ++j)
		{
			CCollisionSection2D* Section = new CCollisionSection2D;

			Section->m_Min = WorldStart + SectionSize * Vector2((float)j, (float)i);
			Section->m_Max = Section->m_Min + SectionSize;
			Section->m_IndexX = j;
			Section->m_IndexY = i;
			Section->m_Index = i * CountX + j;

			m_Section2D.vecSection.push_back(Section);
		}
	}
}

void CSceneCollision::CreateSection3D(int CountX, int CountY, int CountZ, const Vector3& WorldStart, const Vector3& SectionSize)
{
	size_t	Count = m_Section3D.vecSection.size();

	for (size_t i = 0; i < Count; ++i)
	{
		SAFE_DELETE(m_Section3D.vecSection[i]);
	}

	m_Section3D.CountX = CountX;
	m_Section3D.CountY = CountY;
	m_Section3D.CountZ = CountZ;
	m_Section3D.Count = CountX * CountY * CountZ;
	m_Section3D.WorldStart = WorldStart;
	m_Section3D.SectionSize = SectionSize;
	m_Section3D.WorldSize = SectionSize * Vector3((float)CountX, (float)CountY, (float)CountZ);

	for (int i = 0; i < CountZ; ++i)
	{
		for (int j = 0; j < CountY; ++j)
		{
			for (int k = 0; k < CountX; ++k)
			{
				CCollisionSection3D* Section = new CCollisionSection3D;

				Section->m_Min = WorldStart + SectionSize * Vector3((float)k, (float)j, (float)i);
				Section->m_Max = Section->m_Min + SectionSize;
				Section->m_Center = (Section->m_Min + Section->m_Max) * 0.5f;
				Section->m_Radius = (Section->m_Max - Section->m_Min).Length() * 0.5f;
				Section->m_IndexX = k;
				Section->m_IndexY = j;
				Section->m_IndexZ = i;
				Section->m_Index = i * CountX * CountY + j * CountX + k;

				m_Section3D.vecSection.push_back(Section);
			}
		}
	}
}

void CSceneCollision::Save(FILE* File)
{
	/*	
	std::vector<class CCollisionSection3D*>	vecSection;
	int		CountX;
	int		CountY;
	int		CountZ;
	int		Count;
	Vector3	WorldStart;
	Vector3	SectionSize;
	Vector3	WorldSize;
	*/
	/*
	std::vector<class CCollisionSection2D*>	vecSection;
	int		CountX;
	int		CountY;
	int		Count;
	Vector2	WorldStart;
	Vector2	SectionSize;
	Vector2	WorldSize;
	*/
	/*	
	Section2D	m_Section2D;
	Section3D	m_Section3D;
	*/
}

void CSceneCollision::Load(FILE* File)
{
}

void CSceneCollision::CollisionMouse(float DeltaTime)
{
	if (m_MouseCollision)
	{
		if (!m_MouseCollision->GetActive())
		{
			m_MouseCollision->CallCollisionMouseCallback(ECollision_Result::Release);
			m_MouseCollision = nullptr;
		}
	}

	// UI와 마우스가 충돌이 없을 경우 월드물체와 충돌을 시작한다.
	bool	MouseWorldCollision = false;

	// UI와 마우스 충돌처리를 진행한다. 그 결과에 따라 다른 충돌처리를 할지 말지를 결정한다.
	//MouseUICollision = 

	if (!m_CollisionWidget)
	{
		// World위치를 얻어온다.
		Vector2	MouseWorldPos = CInput::GetInst()->GetMouseWorldPos();

		// 마우스가 2D Section에서 어느 영역에 있는지를 판단한다.
		Vector2	MouseIndex = MouseWorldPos;
		MouseIndex -= m_Section2D.WorldStart;

		int	IndexX = 0, IndexY = 0;

		IndexX = (int)(MouseIndex.x / m_Section2D.SectionSize.x);
		IndexY = (int)(MouseIndex.y / m_Section2D.SectionSize.y);

		IndexX = IndexX < 0 ? -1 : IndexX;
		IndexY = IndexY < 0 ? -1 : IndexY;

		IndexX = IndexX >= m_Section2D.CountX ? -1 : IndexX;
		IndexY = IndexY >= m_Section2D.CountY ? -1 : IndexY;

		if (IndexX != -1 && IndexY != -1)
		{
			CCollider* ColliderMouse = m_Section2D.vecSection[IndexY * m_Section2D.CountX + IndexX]->CollisionMouse(MouseWorldPos, DeltaTime);

			if (ColliderMouse)
			{
				if (ColliderMouse != m_MouseCollision)
					ColliderMouse->CallCollisionMouseCallback(ECollision_Result::Collision);

				if (m_MouseCollision && m_MouseCollision != ColliderMouse)
				{
					m_MouseCollision->CallCollisionMouseCallback(ECollision_Result::Release);
				}

				m_MouseCollision = ColliderMouse;
				MouseWorldCollision = true;
			}
		}
	}

	// 마우스와 UI가 충돌되었다면 기존에 충돌되고 있던 월드물체가 있을 경우 충돌 해제한다.
	else if(m_MouseCollision)
	{
		m_MouseCollision->CallCollisionMouseCallback(ECollision_Result::Release);
		m_MouseCollision = nullptr;
	}

	if (!MouseWorldCollision)
	{
		if (m_MouseCollision)
		{
			m_MouseCollision->CallCollisionMouseCallback(ECollision_Result::Release);
			m_MouseCollision = nullptr;
		}
	}
}

void CSceneCollision::CheckSection(CCollider* Collider)
{
	Vector3	Min = Collider->GetMin();
	Vector3	Max = Collider->GetMax();

	if (Collider->GetColliderType() == ECollider_Type::Collider2D)
	{
		Min -= Vector3(m_Section2D.WorldStart.x, m_Section2D.WorldStart.y, 0.f);
		Max -= Vector3(m_Section2D.WorldStart.x, m_Section2D.WorldStart.y, 0.f);

		int	IndexMinX, IndexMinY, IndexMaxX, IndexMaxY;

		IndexMinX = (int)(Min.x / m_Section2D.SectionSize.x);
		IndexMinY = (int)(Min.y / m_Section2D.SectionSize.y);

		IndexMaxX = (int)(Max.x / m_Section2D.SectionSize.x);
		IndexMaxY = (int)(Max.y / m_Section2D.SectionSize.y);

		IndexMinX = IndexMinX < 0 ? 0 : IndexMinX;
		IndexMinY = IndexMinY < 0 ? 0 : IndexMinY;

		IndexMaxX = IndexMaxX >= m_Section2D.CountX ? m_Section2D.CountX - 1 : IndexMaxX;
		IndexMaxY = IndexMaxY >= m_Section2D.CountY ? m_Section2D.CountY - 1 : IndexMaxY;

		for (int i = IndexMinY; i <= IndexMaxY; ++i)
		{
			for (int j = IndexMinX; j <= IndexMaxX; ++j)
			{
				int	Index = i * m_Section2D.CountX + j;

				m_Section2D.vecSection[Index]->AddCollider((CCollider2D*)Collider);
			}
		}
	}

	else
	{
		Min -= m_Section3D.WorldStart;
		Max -= m_Section3D.WorldStart;

		int	IndexMinX, IndexMinY, IndexMinZ, IndexMaxX, IndexMaxY, IndexMaxZ;

		IndexMinX = (int)(Min.x / m_Section3D.SectionSize.x);
		IndexMinY = (int)(Min.y / m_Section3D.SectionSize.y);
		IndexMinZ = (int)(Min.z / m_Section3D.SectionSize.z);

		IndexMaxX = (int)(Max.x / m_Section3D.SectionSize.x);
		IndexMaxY = (int)(Max.y / m_Section3D.SectionSize.y);
		IndexMaxZ = (int)(Max.z / m_Section3D.SectionSize.z);

		IndexMinX = IndexMinX < 0 ? 0 : IndexMinX;
		IndexMinY = IndexMinY < 0 ? 0 : IndexMinY;
		IndexMinZ = IndexMinZ < 0 ? 0 : IndexMinZ;

		IndexMaxX = IndexMaxX >= m_Section3D.CountX ? m_Section3D.CountX - 1 : IndexMaxX;
		IndexMaxY = IndexMaxY >= m_Section3D.CountY ? m_Section3D.CountY - 1 : IndexMaxY;
		IndexMaxZ = IndexMaxZ >= m_Section3D.CountZ ? m_Section3D.CountZ - 1 : IndexMaxZ;

		for (int i = IndexMinZ; i <= IndexMaxZ; ++i)
		{
			for (int j = IndexMinY; j <= IndexMaxY; ++j)
			{
				for (int k = IndexMinX; k <= IndexMaxX; ++k)
				{
					int	Index = i * m_Section3D.CountX * m_Section3D.CountY * + j * m_Section3D.CountX + k;

					m_Section3D.vecSection[Index]->AddCollider((CCollider3D*)Collider);
				}
			}
		}
	}
}

bool CSceneCollision::CreatePixelCollision(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	TCHAR	FullPath[MAX_PATH] = {};

	if (Path)
		lstrcpy(FullPath, Path->Path);
	lstrcat(FullPath, FileName);

	return CreatePixelCollisionFullPath(Name, FullPath);
}

bool CSceneCollision::CreatePixelCollisionFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	if (FindPixelCollision(Name))
		return true;

	// .dds, .tga, 나머지 포맷에 따라 로딩 함수가 다르다.
	TCHAR	_FileExt[_MAX_EXT] = {};

	_wsplitpath_s(FullPath, nullptr, 0, nullptr, 0, nullptr, 0, _FileExt, _MAX_EXT);

	char	FileExt[_MAX_EXT] = {};

#ifdef UNICODE

	int	ConvertLength = WideCharToMultiByte(CP_ACP, 0, _FileExt, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, _FileExt, -1, FileExt, ConvertLength, nullptr, nullptr);

#else

	strcpy_s(FileExt, _FileExt);

#endif // UNICODE

	// 확장자를 대문자로 만들어준다.
	_strupr_s(FileExt);

	DirectX::ScratchImage* Image = new DirectX::ScratchImage;

	if (strcmp(FileExt, ".DDS") == 0)
	{
		if (FAILED(DirectX::LoadFromDDSFile(FullPath, DirectX::DDS_FLAGS_NONE, nullptr,
			*Image)))
		{
			SAFE_DELETE(Image);
			return false;
		}
	}

	else if (strcmp(FileExt, ".TGA") == 0)
	{
		if (FAILED(DirectX::LoadFromTGAFile(FullPath, nullptr, *Image)))
		{
			SAFE_DELETE(Image);
			return false;
		}
	}

	else
	{
		if (FAILED(DirectX::LoadFromWICFile(FullPath, DirectX::WIC_FLAGS_NONE, nullptr,
			*Image)))
		{
			SAFE_DELETE(Image);
			return false;
		}
	}

	PixelInfo* Info = new PixelInfo;

	Info->RefCount = 1;

	Info->Width = (unsigned int)Image->GetImages()[0].width;
	Info->Height = (unsigned int)Image->GetImages()[0].height;

	Info->Pixel = new unsigned char[Image->GetPixelsSize()];

	memcpy(Info->Pixel, Image->GetPixels(), Image->GetPixelsSize());

	// bmp 파일은 메모리에 픽셀정보가 저장될때 상하반전되어 저장이 된다.
	if (strcmp(FileExt, ".BMP") == 0)
	{
		// 원래 순서대로 반전시켜서 저장해둔다.
		unsigned int LineSize = Info->Width * 4;

		unsigned char* Line = new unsigned char[LineSize];

		int	HalfHeight = Info->Height / 2;

		for (int i = 0; i < HalfHeight; ++i)
		{
			memcpy(Line, &Info->Pixel[i * LineSize], LineSize);
			memcpy(&Info->Pixel[i * LineSize], &Info->Pixel[(Info->Height - 1 - i) * LineSize], LineSize);
			memcpy(&Info->Pixel[(Info->Height - 1 - i) * LineSize], Line, LineSize);
		}

		SAFE_DELETE_ARRAY(Line);
	}

	if (FAILED(DirectX::CreateShaderResourceView(CDevice::GetInst()->GetDevice(),
		Image->GetImages(),
		Image->GetImageCount(),
		Image->GetMetadata(),
		&Info->SRV)))
		return false;

	m_mapPixelCollision.insert(std::make_pair(Name, Info));

	SAFE_DELETE(Image);

	return true;
}

bool CSceneCollision::CreatePixelCollisionMultibyte(const std::string& Name, 
	const char* FileName, const std::string& PathName)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	char	FullPath[MAX_PATH] = {};

	if (Path)
		strcpy_s(FullPath, Path->PathMultibyte);
	strcat_s(FullPath, FileName);

	return CreatePixelCollisionMultibyteFullPath(Name, FullPath);
}

bool CSceneCollision::CreatePixelCollisionMultibyteFullPath(const std::string& Name,
	const char* FullPath)
{
	TCHAR	WideCharFullPath[MAX_PATH] = {};

	int	Length = MultiByteToWideChar(CP_ACP, 0, FullPath, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, FullPath, -1, WideCharFullPath, Length);

	return CreatePixelCollisionFullPath(Name, WideCharFullPath);
}

PixelInfo* CSceneCollision::FindPixelCollision(const std::string& Name)
{
	auto	iter = m_mapPixelCollision.find(Name);

	if (iter == m_mapPixelCollision.end())
		return nullptr;

	return iter->second;
}

bool CSceneCollision::Picking(PickingResult& result)
{
	Ray	ray = CInput::GetInst()->GetRay();

	Matrix	matView = m_Owner->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();

	matView.Inverse();

	ray.ConvertSpace(matView);

	// 광선이 관통하는 구를 찾는다.
	size_t	SectionCount = m_Section3D.vecSection.size();

	std::list<int>	SectionList;

	for (size_t i = 0; i < SectionCount; ++i)
	{
		Vector3 Center = m_Section3D.vecSection[i]->m_Center;
		float Radius = m_Section3D.vecSection[i]->m_Radius;

		PickingResult	SectionResult;
		if (CCollisionManager::GetInst()->CollisionRayToSphere(SectionResult, ray, Center, Radius))
		{
			SectionList.push_back((int)i);
		}
	}

	SectionList.sort(CSceneCollision::SortPickginSection);

	auto iter = SectionList.begin();
	auto iterEnd = SectionList.end();

	for (; iter != iterEnd; iter++)
	{
		int	Idx = *iter;

		std::list<CSharedPtr<CCollider3D>>	List = m_Section3D.vecSection[Idx]->m_ColliderList;

		if (List.empty())
		{
			continue;
		}

		List.sort(CSceneCollision::SortColliderList);

		auto iter1 = List.begin();
		auto iter1End = List.end();

		for (; iter1 != iter1End; ++iter1)
		{
			if (!(*iter1)->GetActive() || !(*iter1)->GetEnable() || (*iter1)->GetFrustumCull())
			{
				continue;
			}

			if (CCollisionManager::GetInst()->CollisionRayToSphere(result, ray, (*iter1)->GetCenter(), (*iter1)->GetRadius()))
			{
				result.PickObject = (*iter1)->GetOwner();
				result.PickComponent = *iter1;
				return true;
			}
		}
	}

	return false;
}

bool CSceneCollision::SortPickginSection(int Src, int Dest)
{
	CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	Vector3	CameraPos = Camera->GetWorldPos();

	CSceneCollision* Collision = CSceneManager::GetInst()->GetScene()->GetCollisionManager();

	Vector3	SrcCenter = Collision->m_Section3D.vecSection[Src]->m_Center;
	Vector3	DestCenter = Collision->m_Section3D.vecSection[Dest]->m_Center;

	float	SrcDist = CameraPos.Distance(SrcCenter);
	float	DestDist = CameraPos.Distance(DestCenter);

	return SrcDist < DestDist;
}

bool CSceneCollision::SortColliderList(CSharedPtr<class CCollider3D> Src, CSharedPtr<class CCollider3D> Dest)
{
	Vector3	SrcCenter = Src->GetCenter();
	Vector3	DestCenter = Dest->GetCenter();

	CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	float	SrcDist = Camera->GetWorldPos().Distance(SrcCenter) - Src->GetRadius();
	float	DestDist = Camera->GetWorldPos().Distance(DestCenter) - Dest->GetRadius();

	return SrcDist < DestDist;
}
