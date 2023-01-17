#include "SceneCollision.h"
#include "CollisionSection2D.h"
#include "CollisionSection3D.h"
#include "../Device.h"
#include "../Component/Collider2D.h"
#include "../Component/Collider3D.h"
#include "../PathManager.h"
#include "../Input.h"
#include "Scene.h"
#include "SceneViewport.h"

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

	for (; iter != iterEnd; ++iter)
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

	//CreateSection3D(5, 5, 5, Vector3(), Vector3(Width, Height));

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

	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

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
			// ���������ӿ� �浹�ǰ� �ִ� ��ü�� �ִٸ� �ش� ��ü�鿡�� �浹�� �����Ǿ��ٴ� �뺸��
			// ������ �Ѵ�.
			(*iter)->SendPrevCollisionEnd();

			++iter;
			continue;
		}

		CheckSection(*iter);
		++iter;
	}

	// ���� �浹������ ��ġ���� �Ǵ��Ѵ�. ���������ӿ� �浹�ǰ� �ִ� ��ü�� �浹������ ��ġ�°� ���ٸ�
	// ���� �����ӿ����� �浹üũ ��ü�� �ƿ� ���ϰ� �浹 ���ɼ��� 0% �̱� ������ �浹�ϴ� ��ü�� �������ٴ�
	// ó���� �������־�� �Ѵ�.
	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->GetEnable())
			continue;

		(*iter)->CheckPrevCollisionColliderSection();
	}

	// ���콺�� ���� �浹ó���� �����ؾ� �Ѵ�.
	CollisionMouse(DeltaTime);


	// �浹ü���� �浹ó���� �����Ѵ�.
	// ��� Section�� �ݺ��ϸ� �浹ó���� �����Ѵ�.
	for (int i = 0; i < m_Section2D.Count; ++i)
	{
		m_Section2D.vecSection[i]->Collision(DeltaTime);
	}

	for (int i = 0; i < m_Section3D.Count; ++i)
	{
		m_Section3D.vecSection[i]->Collision(DeltaTime);
	}

	// �浹ó���� �Ϸ�Ǿ��ٸ� �浹ü�� Clear���ش�.
	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
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

	// UI�� ���콺�� �浹�� ���� ��� ���幰ü�� �浹�� �����Ѵ�.
	bool	MouseWorldCollision = false;

	// UI�� ���콺 �浹ó���� �����Ѵ�. �� ����� ���� �ٸ� �浹ó���� ���� ������ �����Ѵ�.
	//MouseUICollision = 

	if (!m_CollisionWidget)
	{
		// World��ġ�� ���´�.
		Vector2	MouseWorldPos = CInput::GetInst()->GetMouseWorldPos();

		// ���콺�� 2D Section���� ��� ������ �ִ����� �Ǵ��Ѵ�.
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

	// ���콺�� UI�� �浹�Ǿ��ٸ� ������ �浹�ǰ� �ִ� ���幰ü�� ���� ��� �浹 �����Ѵ�.
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

	// .dds, .tga, ������ ���˿� ���� �ε� �Լ��� �ٸ���.
	TCHAR	_FileExt[_MAX_EXT] = {};

	_wsplitpath_s(FullPath, nullptr, 0, nullptr, 0, nullptr, 0, _FileExt, _MAX_EXT);

	char	FileExt[_MAX_EXT] = {};

#ifdef UNICODE

	int	ConvertLength = WideCharToMultiByte(CP_ACP, 0, _FileExt, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, _FileExt, -1, FileExt, ConvertLength, nullptr, nullptr);

#else

	strcpy_s(FileExt, _FileExt);

#endif // UNICODE

	// Ȯ���ڸ� �빮�ڷ� ������ش�.
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

	// bmp ������ �޸𸮿� �ȼ������� ����ɶ� ���Ϲ����Ǿ� ������ �ȴ�.
	if (strcmp(FileExt, ".BMP") == 0)
	{
		// ���� ������� �������Ѽ� �����صд�.
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
