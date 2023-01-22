#include "TerrainComponent.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../PathManager.h"
#include "../Resource/Shader/TerrainConstantBuffer.h"

CTerrainComponent::CTerrainComponent() : m_CountX(0), m_CountY(0)
{
	SetTypeID<CTerrainComponent>();

	m_ComponentTypeName = "TerrainComponent";

	m_CBuffer = new CTerrainConstantBuffer;

	m_CBuffer->Init();
}

CTerrainComponent::CTerrainComponent(const CTerrainComponent& component) : CPrimitiveComponent(component)
{
	m_CountX = component.m_CountX;
	m_CountY = component.m_CountY;

	m_Size = component.m_Size;
	m_CellSize = component.m_CellSize;
	
	m_CBuffer = m_CBuffer->Clone();
}

CTerrainComponent::~CTerrainComponent()
{
	SAFE_DELETE(m_CBuffer);
}

void CTerrainComponent::SetDetailLevel(float Level)
{
	m_CBuffer->SetDetailLevel(Level);
	m_CBuffer->UpdateBuffer();
}

void CTerrainComponent::SetSplatCount(int Count)
{
	m_CBuffer->SetSplatCount(Count);
	m_CBuffer->UpdateBuffer();
}

bool CTerrainComponent::SetMesh(const std::string& Name)
{
	return CPrimitiveComponent::SetMesh(Name);
}

bool CTerrainComponent::SetMesh(CMesh* Mesh)
{
	return CPrimitiveComponent::SetMesh(Mesh);
}

bool CTerrainComponent::SetMesh(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	if (m_Scene)
	{
		m_Scene->GetResource()->LoadMesh(MeshType::Static, Name, FileName, PathName);
		m_Mesh = m_Scene->GetResource()->FindMesh(Name);
	}

	else
	{
		CResourceManager::GetInst()->LoadMesh(m_Scene, MeshType::Static, Name, FileName, PathName);
		m_Mesh = CResourceManager::GetInst()->FindMesh(Name);
	}

	if (m_Mesh)
	{
		SetMeshSize(m_Mesh->GetMeshSize());
	}

	m_vecMaterial.clear();

	int SlotCount = m_Mesh->GetSlotCount();

	for (int i = 0; i < SlotCount; i++)
	{
		CMaterial* Material = m_Mesh->GetMaterial(i);

		m_vecMaterial.push_back(Material->Clone());
	}

	return true;
}

bool CTerrainComponent::SetMeshFullPath(const std::string& Name, const TCHAR* FullPath)
{
	if (m_Scene)
	{
		m_Scene->GetResource()->LoadMeshFullPath(MeshType::Static, Name, FullPath);
		m_Mesh = m_Scene->GetResource()->FindMesh(Name);
	}

	else
	{
		CResourceManager::GetInst()->LoadMeshFullPath(m_Scene, MeshType::Static, Name, FullPath);
		m_Mesh = CResourceManager::GetInst()->FindMesh(Name);
	}

	if (m_Mesh)
	{
		SetMeshSize(m_Mesh->GetMeshSize());
	}

	m_vecMaterial.clear();

	int SlotCount = m_Mesh->GetSlotCount();

	for (int i = 0; i < SlotCount; i++)
	{
		CMaterial* Material = m_Mesh->GetMaterial(i);

		m_vecMaterial.push_back(Material->Clone());
	}

	return true;
}

void CTerrainComponent::Start()
{
	CPrimitiveComponent::Start();

	m_CBuffer->UpdateBuffer();
}

bool CTerrainComponent::Init()
{
	if (!CPrimitiveComponent::Init())
	{
		return false;
	}

	return true;
}

void CTerrainComponent::Update(float DeltaTime)
{
	CPrimitiveComponent::Update(DeltaTime);
}

void CTerrainComponent::PostUpdate(float DeltaTime)
{
	CPrimitiveComponent::PostUpdate(DeltaTime);
}

void CTerrainComponent::Render()
{
	CPrimitiveComponent::Render();
}

CTerrainComponent* CTerrainComponent::Clone() const
{
	return new CTerrainComponent(*this);
}

void CTerrainComponent::Save(FILE* File)
{
	CPrimitiveComponent::Save(File);
}

void CTerrainComponent::Load(FILE* File)
{
	CPrimitiveComponent::Load(File);
}

void CTerrainComponent::CreateTerrain(int CountX, int CountY, float SizeX, float SizeY, const TCHAR* HeightMapName, const std::string& HeightMapPath)
{
	m_CountX = CountX;
	m_CountY = CountY;

	m_CellSize.x = SizeX;
	m_CellSize.y = SizeY;

	m_Size = m_CellSize * Vector2((float)CountX, (float)CountY);

	// HeightMap이 있을 경우 높이 정보를 얻어온다.
	std::vector<float>	vecY;
	float MaxY = 0.f;

	if (HeightMapName)
	{
		TCHAR FullPath[MAX_PATH] = {};

		const PathInfo* Path = CPathManager::GetInst()->FindPath(HeightMapPath);

		if (Path)
		{
			lstrcpy(FullPath, Path->Path);
		}

		lstrcat(FullPath, HeightMapName);

		char Ext[_MAX_EXT] = {};
		char FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

		int	Length = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultibyte, Length, nullptr, nullptr);

#else

		strcpy_s(FileNameMultibyte, FullPath);

#endif // UNICODE

		_splitpath_s(FullPathMultibyte, nullptr, 0, nullptr, 0, nullptr, 0, Ext, _MAX_EXT);
		_strupr_s(Ext);

		DirectX::ScratchImage Img;

		if (strcmp(Ext, ".DDS") == 0)
		{
			if (FAILED(DirectX::LoadFromDDSFile(FullPath, DirectX::DDS_FLAGS_NONE, nullptr,
				Img)))
			{
				return;
			}
		}

		else if (strcmp(Ext, ".TGA") == 0)
		{
			if (FAILED(DirectX::LoadFromTGAFile(FullPath, nullptr, Img)))
			{
				return;
			}
		}

		else
		{
			if (FAILED(DirectX::LoadFromWICFile(FullPath, DirectX::WIC_FLAGS_NONE, nullptr, Img)))
			{
				return;
			}
		}

		const DirectX::Image* PixelInfo = Img.GetImages();

		for (size_t i = 0; i < PixelInfo->height; i++)
		{
			for (size_t j = 0; j < PixelInfo->width; j++)
			{
				int	PixelIndex = (int)i * (int)PixelInfo->width * 4 + (int)j * 4;
				float Y = PixelInfo->pixels[PixelIndex];

				if (Y > MaxY)
				{
					MaxY = Y;
				}

				vecY.push_back(Y);
			}
		}
	}

	else
	{
		vecY.resize(m_CountX * m_CountY);
	}

	// 정점정보와 인덱스 정보를 구성한다.
	for (int i = 0; i < m_CountY; i++)
	{
		for (int j = 0; j < m_CountX; j++)
		{
			Vertex3DStatic	Vtx;
			Vtx.Pos = Vector3((float)j * m_CellSize.x, vecY[i * m_CountX + j], m_Size.y - (float)i * m_CellSize.y);
			//Vtx.Normal = Vector3(0.f, 1.f, 0.f);
			Vtx.UV = Vector2((float)j / (float)(m_CountX - 1), 1.f - (float)i / (float)(m_CountY - 1));

			m_vecVtx.push_back(Vtx);
		}
	}

	SetMeshSize(m_Size.x, MaxY, m_Size.y);

	m_vecFaceNormal.resize((m_CountX - 1) * (m_CountY - 1) * 2);

	int	TriIndex = 0;

	for (int i = 0; i < m_CountY - 1; i++)
	{
		for (int j = 0; j < m_CountX - 1; j++)
		{
			int Index = i * m_CountX + j;

			m_vecIndex.push_back(Index);
			m_vecIndex.push_back(Index + 1);
			m_vecIndex.push_back(Index + m_CountX + 1);

			Vector3	TrianglePos1, TrianglePos2, TrianglePos3;

			TrianglePos1 = m_vecVtx[Index].Pos;
			TrianglePos2 = m_vecVtx[Index + 1].Pos;
			TrianglePos3 = m_vecVtx[Index + m_CountX + 1].Pos;

			Vector3	Edge1, Edge2;

			Edge1 = TrianglePos2 - TrianglePos1;
			Edge2 = TrianglePos3 - TrianglePos1;

			Edge1.Normalize();
			Edge2.Normalize();

			m_vecFaceNormal[TriIndex] = Edge1.Cross(Edge2);
			m_vecFaceNormal[TriIndex].Normalize();

			TriIndex++;

			m_vecIndex.push_back(Index);
			m_vecIndex.push_back(Index + m_CountX + 1);
			m_vecIndex.push_back(Index + m_CountX);

			TrianglePos1 = m_vecVtx[Index].Pos;
			TrianglePos2 = m_vecVtx[Index + m_CountX + 1].Pos;
			TrianglePos3 = m_vecVtx[Index + m_CountX].Pos;

			Edge1 = TrianglePos2 - TrianglePos1;
			Edge2 = TrianglePos3 - TrianglePos1;

			Edge1.Normalize();
			Edge2.Normalize();

			m_vecFaceNormal[TriIndex] = Edge1.Cross(Edge2);
			m_vecFaceNormal[TriIndex].Normalize();

			++TriIndex;
		}
	}

	ComputeNormal();

	ComputeTangent();

	// 구해준 정보로 메쉬를 만든다.
	char	MeshName[256] = {};

#ifdef _WIN64

	sprintf_s(MeshName, "%s%lli", m_Name.c_str(), (__int64)this);

#else

	sprintf_s(MeshName, "%s%d", m_Name.c_str(), (int)this);

#endif // _WIN64

	m_Scene->GetResource()->CreateMesh(MeshType::Static, MeshName,
		&m_vecVtx[0], sizeof(Vertex3DStatic), (int)m_vecVtx.size(),
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		&m_vecIndex[0], 4, (int)m_vecIndex.size(),
		D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);

	m_Mesh = m_Scene->GetResource()->FindMesh(MeshName);

	AddMaterial("DefaultTerrain");
}

void CTerrainComponent::ComputeNormal()
{
	size_t	Size = m_vecFaceNormal.size();

	for (size_t i = 0; i < Size; i++)
	{
		unsigned int Idx0 = m_vecIndex[i * 3];
		unsigned int Idx1 = m_vecIndex[i * 3 + 1];
		unsigned int Idx2 = m_vecIndex[i * 3 + 2];

		m_vecVtx[Idx0].Normal += m_vecFaceNormal[i];
		m_vecVtx[Idx1].Normal += m_vecFaceNormal[i];
		m_vecVtx[Idx2].Normal += m_vecFaceNormal[i];
	}

	Size = m_vecVtx.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecVtx[i].Normal.Normalize();
	}
}

void CTerrainComponent::ComputeTangent()
{
	// 탄젠트 벡터 구함.
	for (size_t i = 0; i < m_vecFaceNormal.size(); ++i)
	{
		unsigned int	idx0 = m_vecIndex[i * 3];
		unsigned int	idx1 = m_vecIndex[i * 3 + 1];
		unsigned int	idx2 = m_vecIndex[i * 3 + 2];

		float	fVtx1[3], fVtx2[3];
		fVtx1[0] = m_vecVtx[idx1].Pos.x - m_vecVtx[idx0].Pos.x;
		fVtx1[1] = m_vecVtx[idx1].Pos.y - m_vecVtx[idx0].Pos.y;
		fVtx1[2] = m_vecVtx[idx1].Pos.z - m_vecVtx[idx0].Pos.z;

		fVtx2[0] = m_vecVtx[idx2].Pos.x - m_vecVtx[idx0].Pos.x;
		fVtx2[1] = m_vecVtx[idx2].Pos.y - m_vecVtx[idx0].Pos.y;
		fVtx2[2] = m_vecVtx[idx2].Pos.z - m_vecVtx[idx0].Pos.z;

		float	ftu[2], ftv[2];
		ftu[0] = m_vecVtx[idx1].UV.x - m_vecVtx[idx0].UV.x;
		ftv[0] = m_vecVtx[idx1].UV.y - m_vecVtx[idx0].UV.y;

		ftu[1] = m_vecVtx[idx2].UV.x - m_vecVtx[idx0].UV.x;
		ftv[1] = m_vecVtx[idx2].UV.y - m_vecVtx[idx0].UV.y;

		float	fDen = 1.f / (ftu[0] * ftv[1] - ftu[1] * ftv[0]);

		Vector3	Tangent;
		Tangent.x = (ftv[1] * fVtx1[0] - ftv[0] * fVtx2[0]) * fDen;
		Tangent.y = (ftv[1] * fVtx1[1] - ftv[0] * fVtx2[1]) * fDen;
		Tangent.z = (ftv[1] * fVtx1[2] - ftv[0] * fVtx2[2]) * fDen;

		Tangent.Normalize();

		m_vecVtx[idx0].Tangent = Tangent;
		m_vecVtx[idx1].Tangent = Tangent;
		m_vecVtx[idx2].Tangent = Tangent;

		m_vecVtx[idx0].Binormal = m_vecVtx[idx0].Normal.Cross(Tangent);
		m_vecVtx[idx1].Binormal = m_vecVtx[idx1].Normal.Cross(Tangent);
		m_vecVtx[idx2].Binormal = m_vecVtx[idx2].Normal.Cross(Tangent);

		m_vecVtx[idx0].Binormal.Normalize();
		m_vecVtx[idx1].Binormal.Normalize();
		m_vecVtx[idx2].Binormal.Normalize();
	}
}
