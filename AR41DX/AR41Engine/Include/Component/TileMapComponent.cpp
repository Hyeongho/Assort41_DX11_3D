#include "TileMapComponent.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Shader/TileMapConstantBuffer.h"
#include "../Device.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "CameraComponent.h"
#include "../Scene/CameraManager.h"
#include "../Thread/ThreadManager.h"

CTileMapComponent::CTileMapComponent()	:
	m_CountX(0),
	m_CountY(0),
	m_Count(0),
	m_RenderCount(0),
	m_TileMapCBuffer(nullptr),
	m_TileInfoBuffer(nullptr),
	m_EditorMouseOnTile(nullptr),
	m_TileInfoCount(0)
{
	SetTypeID<CTileMapComponent>();

	m_ComponentTypeName = "TileMapComponent";

	m_TileTypeColor[(int)ETileOption::None] = Vector4::White;
	m_TileTypeColor[(int)ETileOption::Wall] = Vector4::Red;
}

CTileMapComponent::CTileMapComponent(const CTileMapComponent& component) :
	CPrimitiveComponent(component)
{
	m_TileMesh = component.m_TileMesh;

	m_TileInfoCount = component.m_TileInfoCount;

	if (component.m_TileMaterial)
		m_TileMaterial = component.m_TileMaterial->Clone();

	if (component.m_TileMapCBuffer)
		m_TileMapCBuffer = component.m_TileMapCBuffer->Clone();

	if (component.m_TileInfoBuffer)
		m_TileInfoBuffer = component.m_TileInfoBuffer->Clone();


	m_Shape = component.m_Shape;
	m_CountX = component.m_CountX;
	m_CountY = component.m_CountY;
	m_Count = component.m_Count;
	m_RenderCount = component.m_RenderCount;
	m_TileSize = component.m_TileSize;
	m_TileTypeColor[(int)ETileOption::None] = component.m_TileTypeColor[(int)ETileOption::None];
	m_TileTypeColor[(int)ETileOption::Wall] = component.m_TileTypeColor[(int)ETileOption::Wall];

	m_vecTile.clear();

	size_t	Size = component.m_vecTile.size();

	for (size_t i = 0; i < Size; ++i)
	{
		CTile* Tile = component.m_vecTile[i]->Clone();

		Tile->m_Owner = this;

		m_vecTile.push_back(Tile);
	}

	m_vecTileInfo.resize(m_vecTile.size());
}

CTileMapComponent::~CTileMapComponent()
{
	CThreadManager::GetInst()->DeleteNavigationThread(this);

	SAFE_DELETE(m_TileMapCBuffer);
	SAFE_DELETE(m_TileInfoBuffer);

	auto	iter = m_vecTile.begin();
	auto	iterEnd = m_vecTile.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	m_vecTile.clear();
}

void CTileMapComponent::SetEditorMouseOnTile(int Index)
{
	if (Index < 0 || Index >= m_Count)
		m_EditorMouseOnTile = nullptr;

	else
		m_EditorMouseOnTile = m_vecTile[Index];
}

void CTileMapComponent::SetTileMaterial(const std::string& Name)
{
	if (m_Scene)
		m_TileMaterial = m_Scene->GetResource()->FindMaterial(Name);

	else
		m_TileMaterial = CResourceManager::GetInst()->FindMaterial(Name);

	m_TileMapCBuffer->SetImageSize(Vector2((float)m_TileMaterial->GetTexture(0)->GetWidth(),
		(float)m_TileMaterial->GetTexture(0)->GetHeight()));
}

void CTileMapComponent::SetTileMaterial(CMaterial* Material)
{
	m_TileMaterial = Material;
}

void CTileMapComponent::SetTileTexture(CTexture* Texture)
{
	m_TileMaterial->SetTexture(0, 0, (int)EShaderBufferType::Pixel,
		Texture->GetName(), Texture);
}

void CTileMapComponent::SetTileTexture(const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	m_TileMaterial->SetTexture(0, 0, (int)EShaderBufferType::Pixel, 
		Name, FileName, PathName);
}

void CTileMapComponent::SetTileTextureFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	m_TileMaterial->SetTextureFullPath(0, 0, (int)EShaderBufferType::Pixel,
		Name, FullPath);
}

void CTileMapComponent::SetTileTextureArray(const std::string& Name,
	const std::vector<const TCHAR*>& vecFileName, 
	const std::string& PathName)
{
	m_TileMaterial->SetTextureArray(0, 0, (int)EShaderBufferType::Pixel,
		Name, vecFileName, PathName);
}

void CTileMapComponent::SetTileTextureArrayFullPath(
	const std::string& Name,
	const std::vector<const TCHAR*>& vecFullPath)
{
	m_TileMaterial->SetTextureArrayFullPath(0, 0, (int)EShaderBufferType::Pixel,
		Name, vecFullPath);
}

void CTileMapComponent::SetTileBackTexture(CTexture* Texture)
{
	m_TileBackTexture = Texture;

	if (m_TileBackTexture)
	{
		if (m_TileBackTexture->GetImageType() != EImageType::Array)
		{
			m_vecMaterial[0]->SetTexture(0, 0, (int)EShaderBufferType::Pixel,
				Texture->GetName(), Texture);
		}

		else
		{
			m_vecMaterial[0]->SetTexture(10, 0, (int)EShaderBufferType::Pixel,
				Texture->GetName(), Texture);
		}
	}
}

void CTileMapComponent::SetTileBackTexture(const std::string& Name, 
	const TCHAR* FileName, const std::string& PathName)
{
	CTexture* Texture = nullptr;

	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
			return;

		Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
			return;

		Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	m_TileBackTexture = Texture;

	if (m_TileBackTexture)
	{
		m_vecMaterial[0]->SetTexture(0, 0, (int)EShaderBufferType::Pixel,
			Texture->GetName(), Texture);
	}
}

void CTileMapComponent::SetTileBackTextureFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	CTexture* Texture = nullptr;

	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureFullPath(Name, FullPath))
			return;

		Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
			return;

		Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	m_TileBackTexture = Texture;

	if (m_TileBackTexture)
	{
		m_vecMaterial[0]->SetTexture(0, 0, (int)EShaderBufferType::Pixel,
			Texture->GetName(), Texture);
	}
}

void CTileMapComponent::SetTileBackTextureArray(const std::string& Name, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CTexture* Texture = nullptr;

	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureArray(Name, vecFileName, PathName))
			return;

		Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTextureArray(Name, vecFileName, PathName))
			return;

		Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	m_TileBackTexture = Texture;

	if (m_TileBackTexture)
	{
		m_vecMaterial[0]->SetTexture(0, 10, (int)EShaderBufferType::Pixel,
			Texture->GetName(), Texture);
	}
}

void CTileMapComponent::SetTileBackTextureArrayFullPath(const std::string& Name,
	const std::vector<const TCHAR*>& vecFullPath)
{
	CTexture* Texture = nullptr;

	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureArrayFullPath(Name, vecFullPath))
			return;

		Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTextureArrayFullPath(Name, vecFullPath))
			return;

		Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	m_TileBackTexture = Texture;

	if (m_TileBackTexture)
	{
		m_vecMaterial[0]->SetTexture(0, 10, (int)EShaderBufferType::Pixel,
			Texture->GetName(), Texture);
	}
}

void CTileMapComponent::SetTileBaseColor(const Vector4& Color)
{
	m_TileMaterial->SetBaseColor(Color);
}

void CTileMapComponent::CreateTile(ETileShape Shape, int CountX, 
	int CountY, const Vector2& TileSize)
{
	m_Shape = Shape;

	m_CountX = CountX;
	m_CountY = CountY;
	m_Count = m_CountX * m_CountY;

	m_TileSize = TileSize;

	auto	iter = m_vecTile.begin();
	auto	iterEnd = m_vecTile.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	m_vecTile.clear();

	m_vecTile.resize((size_t)(m_CountX * m_CountY));

	for (int i = 0; i < m_CountY; ++i)
	{
		for (int j = 0; j < m_CountX; ++j)
		{
			CTile* Tile = new CTile;

			Tile->m_Owner = this;

			int	Index = i * m_CountX + j;

			Tile->m_IndexX = j;
			Tile->m_IndexY = i;
			Tile->m_Index = Index;

			Tile->m_Size.x = m_TileSize.x;
			Tile->m_Size.y = m_TileSize.y;

			Tile->m_TileStart = m_TileStartFrame;
			Tile->m_TileEnd = m_TileEndFrame;

			Tile->m_Frame = 3;

			m_vecTile[Index] = Tile;
		}
	}

	int	RenderCountX = 0, RenderCountY = 0;

	switch (m_Shape)
	{
	case ETileShape::Rect:
		RenderCountX = (int)(CDevice::GetInst()->GetResolution().Width /
			m_TileSize.x) + 2;
		RenderCountY = (int)(CDevice::GetInst()->GetResolution().Height /
			m_TileSize.y) + 2;

		for (int i = 0; i < m_CountY; ++i)
		{
			for (int j = 0; j < m_CountX; ++j)
			{
				int	Index = i * m_CountX + j;

				m_vecTile[Index]->m_Pos.x = j * m_TileSize.x;
				m_vecTile[Index]->m_Pos.y = i * m_TileSize.y;
			}
		}
		break;
	case ETileShape::Isometric:
	{
		RenderCountX = (int)(CDevice::GetInst()->GetResolution().Width /
			m_TileSize.x) + 3;
		RenderCountY = (int)(CDevice::GetInst()->GetResolution().Height /
			m_TileSize.y) * 2 + 6;

		float	StartX = 0.f;

		for (int i = 0; i < m_CountY; ++i)
		{
			if (i % 2 == 0)
				StartX = 0.f;

			else
				StartX = m_TileSize.x * 0.5f;

			for (int j = 0; j < m_CountX; ++j)
			{
				int	Index = i * m_CountX + j;

				m_vecTile[Index]->m_Pos.x = StartX + j * m_TileSize.x;
				m_vecTile[Index]->m_Pos.y = i * m_TileSize.y * 0.5f;
			}
		}
	}
		break;
	}

	m_TileMapCBuffer->SetTileSize(m_TileSize);

	// 구조화 버퍼를 생성한다. 구조화버퍼에는 출력할 타일정보가 들어가야
	// 하기 때문에 출력되는 최대 타일 개수를 이용해서 생성한다.
	SAFE_DELETE(m_TileInfoBuffer);

	m_TileInfoCount = RenderCountX * RenderCountY;

	m_TileInfoBuffer = new CStructuredBuffer;

	m_TileInfoBuffer->Init("TileInfo", sizeof(TileInfo),
		m_TileInfoCount, 40, true,
		(int)EShaderBufferType::Vertex);

	m_vecTileInfo.resize((size_t)m_Count);

	for (int i = 0; i < m_Count; ++i)
	{
		m_vecTileInfo[i].TypeColor = Vector4(1.f, 1.f, 1.f, 1.f);
		m_vecTileInfo[i].Opacity = 1.f;
	}

	m_SceneName = m_Scene->GetName();

	// 타일이 생성되었기 때문에 해당 타일맵의 길을 찾아줄 내비게이션 스레드를
	// 생성해준다.
	CThreadManager::GetInst()->CreateNavigationThread(this);
}

int CTileMapComponent::GetTileIndexX(const Vector2& Pos)
{
	if (m_Shape == ETileShape::Rect)
	{
		float ConvertX = Pos.x - GetWorldPos().x;

		int	IndexX = (int)(ConvertX / m_TileSize.x);

		if (IndexX < 0 || IndexX >= m_CountX)
			return -1;

		return IndexX;
	}

	int	IndexY = GetTileIndexY(Pos);

	if (IndexY < 0 || IndexY >= m_CountY)
		return -1;

	int	IndexX = -1;

	float	ConvertX = Pos.x - GetWorldPos().x;

	if (IndexY % 2 == 0)
		IndexX = (int)(ConvertX / m_TileSize.x);

	else
		IndexX = (int)((ConvertX - m_TileSize.x * 0.5f) / m_TileSize.x);

	if (IndexX < 0 || IndexX >= m_CountX)
		return -1;

	return IndexX;
}

int CTileMapComponent::GetTileIndexX(const Vector3& Pos)
{
	return GetTileIndexX(Vector2(Pos.x, Pos.y));
}

int CTileMapComponent::GetTileIndexY(const Vector2& Pos)
{
	if (m_Shape == ETileShape::Rect)
	{
		float ConvertY = Pos.y - GetWorldPos().y;

		int	IndexY = (int)(ConvertY / m_TileSize.y);

		if (IndexY < 0 || IndexY >= m_CountY)
			return -1;

		return IndexY;
	}

	Vector2	TileMapPos(GetWorldPos().x, GetWorldPos().y);

	Vector2	ConvertPos = Pos - TileMapPos;

	float RatioX = ConvertPos.x / m_TileSize.x;
	float RatioY = ConvertPos.y / m_TileSize.y;

	int	IndexX = (int)RatioX;
	int	IndexY = (int)RatioY;

	if (IndexX < 0 || IndexX > m_CountX)
		return -1;

	// 정수 부분을 제거하여 소수점 부분만 남겨준다.
	// 소수점 부분을 이용하여 사각형의 어느 지점에 있는지를
	// 판단하기 위해서이다.
	RatioX -= (int)RatioX;
	RatioY -= (int)RatioY;
	
	// 사각형의 아래쪽 부분일 경우
	if (RatioY < 0.5f)
	{
		// 좌 하단 사각형의 좌 하단 삼각형일 경우가 된다.
		// RatioX가 0.5보다 크면 오른쪽 하단 사각형이 되는데
		// 이경우 0.5에서 빼면 음수가 나오므로 RatioY는 절대 이 값
		// 보다 작아질 수 없다.
		// 이 경우 우측 상단의 삼각형 안에 존재하는 점은 RatioY가
		// 절대 이 값보다 작을 수 없다. 좌측 하단의 삼각형을
		// 구하는 식이다.
		if (RatioY < 0.5f - RatioX)
		{
			// Y인덱스가 0이면 가장 아래쪽 줄이라서 좌 하단 삼각형은
			// 없다.
			if (IndexY == 0)
				return -1;

			else if (IndexX == 0)
				return -1;

			else
				return IndexY * 2 - 1;
		}

		// 오른쪽 사각형 안에서 우 하단 삼각형일 경우
		else if (RatioY < RatioX - 0.5f)
		{
			if (IndexY == 0)
				return -1;

			else
				return IndexY * 2 - 1;
		}

		else
			return IndexY * 2;
	}

	// 사각형의 위쪽 부분일 경우
	else if (RatioY > 0.5f)
	{
		// 좌 상단 사각형의 좌 상단 삼각형일 경우
		if (RatioY - 0.5f > RatioX)
		{
			if (IndexX == 0)
				return -1;

			else if (IndexY * 2 + 1 >= m_CountY)
				return -1;

			else
				return IndexY * 2 + 1;
		}

		// 우 상단 사각형의 우 상단 삼각형일 경우
		else if (RatioY - 0.5f > 1.f - RatioX)
		{
			if (IndexX >= m_CountX)
				return -1;

			else if (IndexY * 2 + 1 >= m_CountY)
				return -1;

			else
				return IndexY * 2 + 1;
		}

		else
			return IndexY * 2;
	}

	// 가운데
	else
		return IndexY * 2;

	return -1;
}

int CTileMapComponent::GetTileIndexY(const Vector3& Pos)
{
	return GetTileIndexY(Vector2(Pos.x, Pos.y));
}

int CTileMapComponent::GetTileIndex(const Vector2& Pos)
{
	int	IndexX = GetTileIndexX(Pos);
	int	IndexY = GetTileIndexY(Pos);

	if (IndexX == -1 || IndexY == -1)
		return -1;

	return IndexY * m_CountX + IndexX;
}

int CTileMapComponent::GetTileIndex(const Vector3& Pos)
{
	return GetTileIndex(Vector2(Pos.x, Pos.y));
}

CTile* CTileMapComponent::GetTile(const Vector2& Pos)
{
	int	Index = GetTileIndex(Pos);

	if (Index == -1)
		return nullptr;

	return m_vecTile[Index];
}

CTile* CTileMapComponent::GetTile(const Vector3& Pos)
{
	int	Index = GetTileIndex(Pos);

	if (Index == -1)
		return nullptr;

	return m_vecTile[Index];
}

CTile* CTileMapComponent::GetTile(int X, int Y)
{
	if (X < 0 || X >= m_CountX || Y < 0 || Y >= m_CountY)
		return nullptr;

	return m_vecTile[Y * m_CountX + X];
}

CTile* CTileMapComponent::GetTile(int Index)
{
	if (Index < 0 || Index >= m_Count)
		return nullptr;

	return m_vecTile[Index];
}

int CTileMapComponent::GetTileRenderIndexX(const Vector3& Pos)
{
	if (m_Shape == ETileShape::Rect)
	{
		float ConvertX = Pos.x - GetWorldPos().x;

		int	IndexX = (int)(ConvertX / m_TileSize.x);

		if (IndexX < 0)
			return 0;

		else if (IndexX >= m_CountX)
			return m_CountX - 1;

		return IndexX;
	}

	int	IndexY = GetTileRenderIndexY(Pos);

	if (IndexY < 0)
		IndexY = 0;

	else if (IndexY >= m_CountY)
		IndexY = m_CountY - 1;

	int	IndexX = -1;

	float	ConvertX = Pos.x - GetWorldPos().x;

	if (IndexY % 2 == 0)
		IndexX = (int)(ConvertX / m_TileSize.x);

	else
		IndexX = (int)((ConvertX - m_TileSize.x * 0.5f) / m_TileSize.x);

	if (IndexX < 0)
		return 0;

	else if (IndexX >= m_CountX)
		return m_CountX - 1;

	return IndexX;
}

int CTileMapComponent::GetTileRenderIndexY(const Vector3& Pos)
{
	if (m_Shape == ETileShape::Rect)
	{
		float ConvertY = Pos.y - GetWorldPos().y;

		int	IndexY = (int)(ConvertY / m_TileSize.y);

		if (IndexY < 0)
			return 0;

		else if (IndexY >= m_CountY)
			return m_CountY - 1;

		return IndexY;
	}

	Vector2	TileMapPos(GetWorldPos().x, GetWorldPos().y);

	Vector2	ConvertPos = Vector2(Pos.x, Pos.y) - TileMapPos;

	float RatioX = ConvertPos.x / m_TileSize.x;
	float RatioY = ConvertPos.y / m_TileSize.y;

	int	IndexX = (int)RatioX;
	int	IndexY = (int)RatioY;

	if (IndexX < 0)
		IndexX = 0;

	else if (IndexX > m_CountX)
		IndexX = m_CountX - 1;

	// 정수 부분을 제거하여 소수점 부분만 남겨준다.
	// 소수점 부분을 이용하여 사각형의 어느 지점에 있는지를
	// 판단하기 위해서이다.
	RatioX -= (int)RatioX;
	RatioY -= (int)RatioY;

	// 사각형의 아래쪽 부분일 경우
	if (RatioY < 0.5f)
	{
		// 좌 하단 사각형의 좌 하단 삼각형일 경우가 된다.
		// RatioX가 0.5보다 크면 오른쪽 하단 사각형이 되는데
		// 이경우 0.5에서 빼면 음수가 나오므로 RatioY는 절대 이 값
		// 보다 작아질 수 없다.
		// 이 경우 우측 상단의 삼각형 안에 존재하는 점은 RatioY가
		// 절대 이 값보다 작을 수 없다. 좌측 하단의 삼각형을
		// 구하는 식이다.
		if (RatioY < 0.5f - RatioX)
		{
			// Y인덱스가 0이면 가장 아래쪽 줄이라서 좌 하단 삼각형은
			// 없다.
			if (IndexY == 0)
				return 0;

			else if (IndexX == 0)
			{
				if (IndexY < 0)
					return 0;

				else if (IndexY >= m_CountY)
					return m_CountY - 1;
					
				else
					return IndexY * 2 + 1;
			}

			else
				return IndexY * 2 - 1;
		}

		// 오른쪽 사각형 안에서 우 하단 삼각형일 경우
		else if (RatioY < RatioX - 0.5f)
		{
			if (IndexY == 0)
				return 0;

			else
				return IndexY * 2 - 1;
		}

		else
			return IndexY * 2;
	}

	// 사각형의 위쪽 부분일 경우
	else if (RatioY > 0.5f)
	{
		// 좌 상단 사각형의 좌 상단 삼각형일 경우
		if (RatioY - 0.5f > RatioX)
		{
			if (IndexX == 0)
			{
				if (IndexY < 0)
					return 0;

				else if (IndexY >= m_CountY)
					return m_CountY - 1;
			}

			if (IndexY * 2 + 1 >= m_CountY)
				return m_CountY - 1;

			else
				return IndexY * 2 + 1;
		}

		// 우 상단 사각형의 우 상단 삼각형일 경우
		else if (RatioY - 0.5f > 1.f - RatioX)
		{
			if (IndexX >= m_CountX)
			{
				if (IndexY < 0)
					return 0;

				else if (IndexY >= m_CountY)
					return m_CountY - 1;
			}

			if (IndexY * 2 + 1 >= m_CountY)
				return  m_CountY - 1;

			else
				return IndexY * 2 + 1;
		}

		else
			return IndexY * 2;
	}

	// 가운데
	else
		return IndexY * 2;

	return -1;
}

void CTileMapComponent::ChangeTileFrame(const Vector2& Pos, int Frame)
{
	CTile* Tile = GetTile(Pos);

	if (!Tile)
		return;

	Tile->m_TileStart = m_vecTileFrame[Frame].Start;
	Tile->m_TileEnd = m_vecTileFrame[Frame].End;

	Tile->m_Frame = Frame;
}

void CTileMapComponent::ChangeTileOption(const Vector2& Pos, ETileOption Option)
{
	CTile* Tile = GetTile(Pos);

	if (!Tile)
		return;

	Tile->m_TileOption = Option;
}

void CTileMapComponent::Start()
{
	CPrimitiveComponent::Start();
}

bool CTileMapComponent::Init()
{
	if (!CPrimitiveComponent::Init())
		return false;

	m_Transform->Set2D(true);

	// 배경용 메쉬 등록
	SetMesh("LBUVRect");

	m_TileMesh = m_Mesh;

	m_Shape = ETileShape::Rect;

	m_TileMapCBuffer = new CTileMapConstantBuffer;

	m_TileMapCBuffer->Init();

	m_TileMapCBuffer->SetStart(Vector2(0.f, 0.f));
	m_TileMapCBuffer->SetEnd(Vector2(160.f, 80.f));
	m_TileMapCBuffer->SetFrame(0);

	SetTileMaterial("TileMap");

	for (int i = 0; i <= 379; ++i)
	{
		Animation2DFrameData	Data;

		Data.Start = Vector2(0.f, 0.f);
		Data.End = Vector2(160.f, 80.f);

		m_vecTileFrame.push_back(Data);
	}

	m_TileStartFrame = Vector2(0.f, 0.f);
	m_TileEndFrame = Vector2(160.f, 80.f);

	m_vecMaterial.clear();

	AddMaterial("DefaultTileMapBack");

	//CreateTile(ETileShape::Isometric, 100, 100, Vector2(160.f, 80.f));

	return true;
}

void CTileMapComponent::Update(float DeltaTime)
{
	CPrimitiveComponent::Update(DeltaTime);

	size_t	Size = m_vecTile.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecTile[i]->Update(DeltaTime);
	}
}

void CTileMapComponent::PostUpdate(float DeltaTime)
{
	CPrimitiveComponent::PostUpdate(DeltaTime);

	if (!m_vecTile.empty())
	{
		CCameraComponent* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();

		Resolution	RS = CDevice::GetInst()->GetResolution();

		Vector3	LB = Camera->GetWorldPos();
		Vector3	RT = LB + Vector3((float)RS.Width, (float)RS.Height, 0.f);

		int	StartX, StartY, EndX, EndY;

		StartX = GetTileRenderIndexX(LB);
		StartY = GetTileRenderIndexY(LB);

		EndX = GetTileRenderIndexX(RT);
		EndY = GetTileRenderIndexY(RT);

		if (m_Shape == ETileShape::Isometric)
		{
			--StartX;
			--StartY;
			--StartY;

			++EndX;
			++EndY;
			++EndY;

			StartX = StartX < 0 ? 0 : StartX;
			StartY = StartY < 0 ? 0 : StartY;

			EndX = EndX >= m_CountX ? m_CountX - 1 : EndX;
			EndY = EndY >= m_CountY ? m_CountY - 1 : EndY;
		}

		/*TCHAR	Text[256] = {};
		wsprintf(Text, TEXT("StartX : %d StartY : %d EndX : %d EndY : %d\n"),
			StartX, StartY, EndX, EndY);

		OutputDebugString(Text);*/

		Matrix	matView, matProj;
		matView = Camera->GetViewMatrix();
		matProj = Camera->GetProjMatrix();

		m_RenderCount = 0;

		for (int i = StartY; i <= EndY; ++i)
		{
			for (int j = StartX; j <= EndX; ++j)
			{
				int	Index = i * m_CountX + j;

				m_vecTile[Index]->UpdateTransform(DeltaTime);

				m_vecTileInfo[m_RenderCount].matWVP = m_vecTile[Index]->m_matWorld *
					matView * matProj;
				m_vecTileInfo[m_RenderCount].matWVP.Transpose();

				if (!m_vecTile[Index]->FrameEmpty())
				{
					m_vecTileInfo[m_RenderCount].Start = m_vecTile[Index]->GetFrameData().Start;
					m_vecTileInfo[m_RenderCount].End = m_vecTile[Index]->GetFrameData().End;
				}

				else
				{
					m_vecTileInfo[m_RenderCount].Start = m_vecTile[Index]->m_TileStart;
					m_vecTileInfo[m_RenderCount].End = m_vecTile[Index]->m_TileEnd;
				}

				m_vecTileInfo[m_RenderCount].Opacity = m_vecTile[Index]->m_Opacity;
				m_vecTileInfo[m_RenderCount].AnimationType = (int)m_vecTile[Index]->m_Anim2DType;
				m_vecTileInfo[m_RenderCount].Frame = m_vecTile[Index]->m_Frame;
				m_vecTileInfo[m_RenderCount].TypeColor = m_TileTypeColor[(int)m_vecTile[Index]->m_TileOption];

				if (m_EditorMouseOnTile == m_vecTile[Index])
					m_vecTileInfo[m_RenderCount].TypeColor = Vector4(0.f, 1.f, 0.f, 1.f);

				++m_RenderCount;
			}
		}

		// 위에서 만들어준 타일 정보를 구조화 버퍼에 갱신해준다.
		m_TileInfoBuffer->UpdateBuffer(&m_vecTileInfo[0],
			m_RenderCount);
	}
}

void CTileMapComponent::Render()
{
	//CPrimitiveComponent::Render();
	CSceneComponent::Render();

	if (m_TileBackTexture)
	{
		m_vecMaterial[0]->SetMaterial();

		m_Mesh->Render();

		m_vecMaterial[0]->ResetMaterial();
	}


	if (m_RenderCount > 0 && m_TileMaterial)
	{
		// 구조화버퍼의 내용을 Shader로 보내준다.
		m_TileInfoBuffer->SetShader();

		// TileMap 상수버퍼를 Shader로 보내준다.
		m_TileMapCBuffer->UpdateBuffer();

		// 재질 지정
		m_TileMaterial->SetMaterial();


		m_TileMesh->RenderInstancing(m_RenderCount);



		m_TileMaterial->ResetMaterial();


		m_TileInfoBuffer->ResetShader();
	}
}

CTileMapComponent* CTileMapComponent::Clone() const
{
	return new CTileMapComponent(*this);
}

void CTileMapComponent::Save(FILE* File)
{
	CPrimitiveComponent::Save(File);

	int	Count = (int)m_vecTile.size();

	fwrite(&Count, sizeof(int), 1, File);

	for (int i = 0; i < Count; ++i)
	{
		m_vecTile[i]->Save(File);
	}

	bool	TileMaterial = false;

	if (m_TileMaterial)
		TileMaterial = true;

	fwrite(&TileMaterial, sizeof(bool), 1, File);

	if (TileMaterial)
		m_TileMaterial->Save(File);

	bool BackTexture = false;

	if (m_TileBackTexture)
		BackTexture = true;

	fwrite(&BackTexture, sizeof(bool), 1, File);

	if (BackTexture)
	{
		int	Length = (int)m_TileBackTexture->GetName().length();
		fwrite(&Length, sizeof(int), 1, File);
		fwrite(m_TileBackTexture->GetName().c_str(), sizeof(char), Length, File);
	}

	fwrite(&m_Shape, sizeof(ETileShape), 1, File);

	fwrite(&m_CountX, sizeof(int), 1, File);
	fwrite(&m_CountY, sizeof(int), 1, File);
	fwrite(&m_Count, sizeof(int), 1, File);
	fwrite(&m_TileInfoCount, sizeof(int), 1, File);

	fwrite(&m_TileSize, sizeof(Vector2), 1, File);
	fwrite(&m_TileStartFrame, sizeof(Vector2), 1, File);
	fwrite(&m_TileEndFrame, sizeof(Vector2), 1, File);

	int	FrameCount = (int)m_vecTileFrame.size();

	fwrite(&FrameCount, sizeof(int), 1, File);
	
	for (int i = 0; i < FrameCount; ++i)
	{
		fwrite(&m_vecTileFrame[i], sizeof(Animation2DFrameData), 1, File);
	}
}

void CTileMapComponent::Load(FILE* File)
{
	CPrimitiveComponent::Load(File);

	int	Count = 0;

	fread(&Count, sizeof(int), 1, File);

	size_t	Size = m_vecTile.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecTile[i]);
	}

	m_vecTile.clear();

	for (int i = 0; i < Count; ++i)
	{
		CTile* Tile = new CTile;

		Tile->m_Owner = this;

		Tile->Load(File);

		m_vecTile.push_back(Tile);
	}

	bool	TileMaterial = false;

	fread(&TileMaterial, sizeof(bool), 1, File);

	if (TileMaterial)
	{
		CMaterial* Material = m_Mesh->GetMaterial(0);

		m_TileMaterial = Material->Clone();

		m_TileMaterial->SetScene(m_Scene);

		m_TileMaterial->Load(File);
	}

	bool BackTexture = false;

	fread(&BackTexture, sizeof(bool), 1, File);

	if (BackTexture)
	{
		int	Length = 0;
		char	TexName[256] = {};
		fread(&Length, sizeof(int), 1, File);
		fread(TexName, sizeof(char), Length, File);

		if (m_Scene)
			m_TileBackTexture = m_Scene->GetResource()->FindTexture(TexName);

		else
			m_TileBackTexture = CResourceManager::GetInst()->FindTexture(TexName);
	}

	fread(&m_Shape, sizeof(ETileShape), 1, File);

	fread(&m_CountX, sizeof(int), 1, File);
	fread(&m_CountY, sizeof(int), 1, File);
	fread(&m_Count, sizeof(int), 1, File);
	fread(&m_TileInfoCount, sizeof(int), 1, File);

	fread(&m_TileSize, sizeof(Vector2), 1, File);
	fread(&m_TileStartFrame, sizeof(Vector2), 1, File);
	fread(&m_TileEndFrame, sizeof(Vector2), 1, File);

	int	FrameCount = 0;

	fread(&FrameCount, sizeof(int), 1, File);

	if (FrameCount > 0)
		m_vecTileFrame.resize((size_t)FrameCount);

	for (int i = 0; i < FrameCount; ++i)
	{
		fread(&m_vecTileFrame[i], sizeof(Animation2DFrameData), 1, File);
	}

	m_TileMapCBuffer->SetStart(m_TileStartFrame);
	m_TileMapCBuffer->SetEnd(m_TileEndFrame);
	m_TileMapCBuffer->SetFrame(0);

	SAFE_DELETE(m_TileInfoBuffer);

	m_TileInfoBuffer = new CStructuredBuffer;

	m_TileInfoBuffer->Init("TileInfo", sizeof(TileInfo),
		m_TileInfoCount, 40, true,
		(int)EShaderBufferType::Vertex);

	m_vecTileInfo.resize((size_t)m_Count);

	for (int i = 0; i < m_Count; ++i)
	{
		m_vecTileInfo[i].TypeColor = Vector4(1.f, 1.f, 1.f, 1.f);
		m_vecTileInfo[i].Opacity = 1.f;
	}

	m_SceneName = m_Scene->GetName();

	// 타일이 생성되었기 때문에 해당 타일맵의 길을 찾아줄 내비게이션 스레드를
	// 생성해준다.
	CThreadManager::GetInst()->CreateNavigationThread(this);
}

