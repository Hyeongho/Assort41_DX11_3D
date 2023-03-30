#include "Mesh.h"
#include "../../Device.h"
#include "../Material/Material.h"
#include "../ResourceManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "FBXLoader.h"

CMesh::CMesh() :
	m_Min(FLT_MAX, FLT_MAX, FLT_MAX),
	m_Max(FLT_MIN, FLT_MIN, FLT_MIN),
	m_RenderCount(0)
{
}

CMesh::~CMesh()
{
	size_t	Size = m_vecContainer.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecContainer[i]);
	}
	Size = m_vecMeshSlot.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecMeshSlot[i]);
	}
}

void CMesh::AddRenderCount(bool Add)
{
	if (Add)
		++m_RenderCount;

	else
		--m_RenderCount;
}

void CMesh::SetMaterial(int Container, int Subset, const std::string& Name)
{
	CMaterial* Material = nullptr;

	if (m_Scene)
	{
		Material = m_Scene->GetResource()->FindMaterial(Name);
	}

	else
	{
		Material = CResourceManager::GetInst()->FindMaterial(Name);
	}

	m_vecContainer[Container]->vecSubset[Subset].Material = Material;
	m_vecContainer[Container]->vecSubset[Subset].Slot->Material = Material;
}

void CMesh::SetMaterial(int Container, int Subset, CMaterial* Material)
{
	m_vecContainer[Container]->vecSubset[Subset].Material = Material;
	m_vecContainer[Container]->vecSubset[Subset].Slot->Material = Material;
}

bool CMesh::CreateMesh(void* VtxData, int Size, int Count,
	D3D11_USAGE VtxUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
	void* IdxData, int IdxSize, int IdxCount, D3D11_USAGE IdxUsage,
	DXGI_FORMAT Fmt)
{
	MeshContainer* Container = new MeshContainer;

	Container->VB.Size = Size;
	Container->VB.Count = Count;
	Container->Primitive = Primitive;
	Container->VB.Data = new char[Size * Count];
	memcpy(Container->VB.Data, VtxData, Size * Count);

	if (!CreateBuffer(BufferType::Vertex, VtxData, Size, Count,
		VtxUsage, &Container->VB.Buffer))
		return false;

	m_vecContainer.push_back(Container);

	MeshSlot* Slot = new MeshSlot;

	m_vecMeshSlot.push_back(Slot);

	Slot->VB = &Container->VB;
	Slot->Primitive = Container->Primitive;

	if (IdxData != nullptr)
	{
		MeshSubset	Subset;
		Container->vecSubset.push_back(Subset);
		int Index = (int)Container->vecSubset.size() - 1;

		Container->vecSubset[Index].Slot = Slot;

		Slot->IB = &Container->vecSubset[Index].IB;

		Slot->IB->Size = IdxSize;
		Slot->IB->Count = IdxCount;
		Slot->IB->Fmt = Fmt;
		Slot->IB->Data = new char[IdxSize * IdxCount];
		memcpy(Slot->IB->Data, IdxData, IdxSize * IdxCount);

		if (!CreateBuffer(BufferType::Index, IdxData, IdxSize, IdxCount,
			IdxUsage, &Slot->IB->Buffer))
			return false;
	}

	return true;
}

bool CMesh::LoadMesh(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	return false;
}

bool CMesh::LoadMeshFullPath(const std::string& Name, const TCHAR* FullPath)
{
	return false;
}

bool CMesh::LoadMeshMultibyte(const std::string& Name,
	const char* FileName, const std::string& PathName)
{
	return false;
}

bool CMesh::LoadMeshMultibyteFullPath(const std::string& Name,
	const char* FullPath)
{
	return false;
}

void CMesh::Render()
{
	size_t	Size = m_vecMeshSlot.size();

	for (size_t i = 0; i < Size; ++i)
	{
		unsigned int	Stride = (unsigned int)m_vecMeshSlot[i]->VB->Size;
		unsigned int	Offset = 0;

		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[i]->Primitive);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1,
			&m_vecMeshSlot[i]->VB->Buffer, &Stride, &Offset);

		if (m_vecMeshSlot[i]->IB)
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(
				m_vecMeshSlot[i]->IB->Buffer, m_vecMeshSlot[i]->IB->Fmt,
				0);
			CDevice::GetInst()->GetContext()->DrawIndexed(
				m_vecMeshSlot[i]->IB->Count, 0, 0);
		}

		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(
				nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CDevice::GetInst()->GetContext()->Draw(
				m_vecMeshSlot[i]->VB->Count, 0);
		}
	}
}

void CMesh::Render(int SlotNumber)
{
	unsigned int	Stride = (unsigned int)m_vecMeshSlot[SlotNumber]->VB->Size;
	unsigned int	Offset = 0;

	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[SlotNumber]->Primitive);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1,
		&m_vecMeshSlot[SlotNumber]->VB->Buffer, &Stride, &Offset);

	if (m_vecMeshSlot[SlotNumber]->IB)
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(
			m_vecMeshSlot[SlotNumber]->IB->Buffer, m_vecMeshSlot[SlotNumber]->IB->Fmt,
			0);
		CDevice::GetInst()->GetContext()->DrawIndexed(
			m_vecMeshSlot[SlotNumber]->IB->Count, 0, 0);
	}

	else
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(
			nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CDevice::GetInst()->GetContext()->Draw(
			m_vecMeshSlot[SlotNumber]->VB->Count, 0);
	}
}

void CMesh::RenderInstancing(int Count)
{
	size_t	Size = m_vecMeshSlot.size();

	for (size_t i = 0; i < Size; ++i)
	{
		unsigned int	Stride = (unsigned int)m_vecMeshSlot[i]->VB->Size;
		unsigned int	Offset = 0;

		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[i]->Primitive);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1,
			&m_vecMeshSlot[i]->VB->Buffer, &Stride, &Offset);

		if (m_vecMeshSlot[i]->IB)
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(
				m_vecMeshSlot[i]->IB->Buffer, m_vecMeshSlot[i]->IB->Fmt,
				0);
			CDevice::GetInst()->GetContext()->DrawIndexedInstanced(
				m_vecMeshSlot[i]->IB->Count, Count, 0, 0, 0);
		}

		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(
				nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CDevice::GetInst()->GetContext()->DrawInstanced(
				m_vecMeshSlot[i]->VB->Count, Count, 0, 0);
		}
	}
}

void CMesh::RenderInstancing(int Count, int SlotNumber)
{
	unsigned int	Stride = (unsigned int)m_vecMeshSlot[SlotNumber]->VB->Size;
	unsigned int	Offset = 0;

	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[SlotNumber]->Primitive);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1,
		&m_vecMeshSlot[SlotNumber]->VB->Buffer, &Stride, &Offset);

	if (m_vecMeshSlot[SlotNumber]->IB)
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(
			m_vecMeshSlot[SlotNumber]->IB->Buffer, m_vecMeshSlot[SlotNumber]->IB->Fmt,
			0);
		CDevice::GetInst()->GetContext()->DrawIndexedInstanced(
			m_vecMeshSlot[SlotNumber]->IB->Count, Count, 0, 0, 0);
	}

	else
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(
			nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CDevice::GetInst()->GetContext()->DrawInstanced(
			m_vecMeshSlot[SlotNumber]->VB->Count, Count, 0, 0);
	}
}

bool CMesh::CreateBuffer(BufferType Type, void* Data, int Size,
	int Count, D3D11_USAGE Usage, ID3D11Buffer** Buffer)
{
	D3D11_BUFFER_DESC	Desc = {};

	Desc.ByteWidth = Size * Count;
	Desc.Usage = Usage;

	if (Type == BufferType::Vertex)
		Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	else
		Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	if (Usage == D3D11_USAGE_DYNAMIC)
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	else if (Usage == D3D11_USAGE_STAGING)
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	D3D11_SUBRESOURCE_DATA	BufferData = {};

	BufferData.pSysMem = Data;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc, &BufferData,
		Buffer)))
		return false;

	if (Type == BufferType::Vertex)
	{
		// 정점은 위치 12바이트가 항상 먼저 들어와 있을 것이다.
		char* VertexData = (char*)Data;

		for (int i = 0; i < Count; ++i)
		{
			Vector3* Pos = (Vector3*)VertexData;
			VertexData += Size;

			if (m_Min.x > Pos->x)
				m_Min.x = Pos->x;

			if (m_Min.y > Pos->y)
				m_Min.y = Pos->y;

			if (m_Min.z > Pos->z)
				m_Min.z = Pos->z;

			if (m_Max.x < Pos->x)
				m_Max.x = Pos->x;

			if (m_Max.y < Pos->y)
				m_Max.y = Pos->y;

			if (m_Max.z < Pos->z)
				m_Max.z = Pos->z;
		}
	}

	return true;
}

bool CMesh::ConvertFBX(CFBXLoader* Loader, const char* FullPath)
{
	const std::vector<std::vector<FbxMaterial*>>* vecMaterials = Loader->GetMaterials();
	const std::vector<FbxMeshContainer*>* vecContainers = Loader->GetContainer();

	auto    iter = vecContainers->begin();
	auto    iterEnd = vecContainers->end();

	std::vector<std::vector<bool>>  vecEmptyIndex;

	int ContainerIndex = 0;

	bool    BumpEnable = false;
	bool    AnimationEnable = false;

	for (; iter != iterEnd; iter++, ++ContainerIndex)
	{
		MeshContainer* Container = new MeshContainer;

		m_vecContainer.push_back(Container);

		std::vector<bool>    vecEmpty;
		vecEmptyIndex.push_back(vecEmpty);

		if ((*iter)->Bump)
			BumpEnable = true;

		if ((*iter)->Animation)
			AnimationEnable = true;

		std::vector<Vertex3D>   vecVtx;

		size_t  VtxCount = (*iter)->vecPos.size();

		vecVtx.resize(VtxCount);

		for (size_t i = 0; i < VtxCount; ++i)
		{
			Vertex3D    Vtx = {};

			Vtx.Pos = (*iter)->vecPos[i];
			Vtx.Normal = (*iter)->vecNormal[i];
			Vtx.UV = (*iter)->vecUV[i];

			if (BumpEnable)
			{
				if (!(*iter)->vecTangent.empty())
					Vtx.Tangent = (*iter)->vecTangent[i];

				if (!(*iter)->vecBinormal.empty())
					Vtx.Binormal = (*iter)->vecBinormal[i];
			}

			if (!(*iter)->vecBlendWeight.empty() && AnimationEnable)
			{
				Vtx.BlendWeight = (*iter)->vecBlendWeight[i];
				Vtx.BlendIndex = (*iter)->vecBlendIndex[i];
			}

			vecVtx[i] = Vtx;
		}

		// Mesh의 Vertex 생성
		if (!CreateBuffer(BufferType::Vertex,
			&vecVtx[0], sizeof(Vertex3D), (int)vecVtx.size(),
			D3D11_USAGE_DEFAULT, &Container->VB.Buffer))
			return false;

		Container->VB.Size = sizeof(Vertex3D);
		Container->VB.Count = (int)vecVtx.size();
		Container->VB.Data = new char[Container->VB.Size * Container->VB.Count];
		memcpy(Container->VB.Data, &vecVtx[0], Container->VB.Size * Container->VB.Count);

		Container->Primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		size_t  IdxCount = (*iter)->vecIndex.size();

		Container->vecSubset.reserve(IdxCount);

		for (size_t i = 0; i < IdxCount; ++i)
		{
			// 인덱스 버퍼를 채워줄 데이터가 없다면 해당 서브셋은 폐기한다.
			if ((*iter)->vecIndex[i].empty())
			{
				vecEmptyIndex[ContainerIndex].push_back(false);
				continue;
			}

			vecEmptyIndex[ContainerIndex].push_back(true);

			MeshSlot* Slot = new MeshSlot;

			m_vecMeshSlot.push_back(Slot);

			Slot->VB = &Container->VB;
			Slot->Primitive = Container->Primitive;

			MeshSubset	Subset = {};

			Container->vecSubset.emplace_back(Subset);

			if (!CreateBuffer(BufferType::Index,
				&(*iter)->vecIndex[i][0], sizeof(UINT),
				(int)(*iter)->vecIndex[i].size(),
				D3D11_USAGE_DEFAULT, &Container->vecSubset[i].IB.Buffer))
				return false;

			Container->vecSubset[i].IB.Size = sizeof(UINT);
			Container->vecSubset[i].IB.Count = (int)(*iter)->vecIndex[i].size();
			Container->vecSubset[i].IB.Fmt = DXGI_FORMAT_R32_UINT;
			Container->vecSubset[i].IB.Data = new char[Container->vecSubset[i].IB.Size * Container->vecSubset[i].IB.Count];

			memcpy(Container->vecSubset[i].IB.Data, &(*iter)->vecIndex[i][0],
				Container->vecSubset[i].IB.Size * Container->vecSubset[i].IB.Count);

			Slot->IB = &Container->vecSubset[i].IB;
			Container->vecSubset[i].Slot = Slot;
		}
	}


	// 재질 정보를 읽어온다.
	auto    iterM = vecMaterials->begin();
	auto    iterMEnd = vecMaterials->end();

	ContainerIndex = 0;

	for (; iterM != iterMEnd; iterM++, ++ContainerIndex)
	{
		// 서브셋 수만큼 반복한다.
		size_t  Size = (*iterM).size();

		for (size_t i = 0; i < Size; ++i)
		{
			if (!vecEmptyIndex[ContainerIndex][i])
				continue;

			FbxMaterial* Mtrl = (*iterM)[i];

			CMaterial* Material = new CMaterial;

			//kbj Material Name
			Material->SetName(Mtrl->Name);
			Material->SetBaseColor(Mtrl->BaseColor);
			Material->SetAmbientColor(Mtrl->AmbientColor);
			Material->SetSpecularColor(Mtrl->SpecularColor);
			Material->SetEmissiveColor(Mtrl->EmissiveColor);
			Material->SetSpecularPower(Mtrl->Shininess);

			Material->SetShader("MeshShader");
			//Kbj DummyTexture
			SetMaterial(ContainerIndex, (int)i, Material);
			// Texture
			char    FileName[MAX_PATH] = {};
			char    Ext[MAX_PATH] = {};
			_splitpath_s(Mtrl->DiffuseTexture.c_str(), 0, 0, 0, 0, FileName, MAX_PATH, Ext, MAX_PATH);
			TCHAR   FullPath[MAX_PATH] = {};
#ifdef UNICODE
			int PathLength = MultiByteToWideChar(CP_ACP, 0, Mtrl->DiffuseTexture.c_str(), -1, 0, 0);
			MultiByteToWideChar(CP_ACP, 0, Mtrl->DiffuseTexture.c_str(), -1, FullPath, PathLength);
#else
			strcpy_s(FullPath, Mtrl->DiffuseTexture.c_str());
#endif // UNICODE
			if (Ext[0] == '\0')
			{
				Material->AddTexture(0, (int)EShaderBufferType::Pixel, "none", TEXT("none.png"));
			}
			else
			{
				Material->AddTextureFullPath(0, (int)EShaderBufferType::Pixel, FileName, FullPath);
			}
			//
			if (!Mtrl->BumpTexture.empty())
			{
				Material->EnableBump();
				memset(FileName, 0, MAX_PATH);
				_splitpath_s(Mtrl->BumpTexture.c_str(), 0, 0, 0, 0, FileName, MAX_PATH, 0, 0);

				memset(FullPath, 0, sizeof(TCHAR) * MAX_PATH);

#ifdef UNICODE
				int PathLength = MultiByteToWideChar(CP_ACP, 0, Mtrl->BumpTexture.c_str(),
					-1, 0, 0);
				MultiByteToWideChar(CP_ACP, 0, Mtrl->BumpTexture.c_str(), -1, FullPath, PathLength);
#else
				strcpy_s(FullPath, Mtrl->BumpTexture.c_str());
#endif // UNICODE

				Material->AddTextureFullPath(1, (int)EShaderBufferType::Pixel,
					FileName, FullPath);
			}

			if (!Mtrl->SpecularTexture.empty())
			{
				memset(FileName, 0, MAX_PATH);
				_splitpath_s(Mtrl->SpecularTexture.c_str(), 0, 0, 0, 0, FileName, MAX_PATH, 0, 0);

				memset(FullPath, 0, sizeof(TCHAR) * MAX_PATH);

#ifdef UNICODE
				int PathLength = MultiByteToWideChar(CP_ACP, 0, Mtrl->SpecularTexture.c_str(),
					-1, 0, 0);
				MultiByteToWideChar(CP_ACP, 0, Mtrl->SpecularTexture.c_str(), -1, FullPath, PathLength);
#else
				strcpy_s(FullPath, Mtrl->SpecularTexture.c_str());
#endif // UNICODE

				Material->AddTextureFullPath(2, (int)EShaderBufferType::Pixel,
					FileName, FullPath);

				Material->EnableSpecular();
			}

			if (AnimationEnable)
				Material->EnableAnimation3D();
		}
	}


	// 자체포맷으로 저장해준다.
	char    MeshFullPath[MAX_PATH] = {};

	strcpy_s(MeshFullPath, FullPath);
	int PathLength = (int)strlen(FullPath);
	memcpy(&MeshFullPath[PathLength - 3], "msh", 3);

	SaveMeshFile(MeshFullPath);

	return true;
}

bool CMesh::SaveMeshFile(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "wb");

	if (!File)
		return false;

	SaveMesh(File);

	fclose(File);

	return true;
}

bool CMesh::LoadMeshFile(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "rb");

	if (!File)
		return false;

	LoadMesh(File);

	fclose(File);

	return true;
}

bool CMesh::SaveMesh(FILE* File)
{
	CRef::Save(File);

	fwrite(&m_MeshType, sizeof(MeshType), 1, File);

	fwrite(&m_Min, sizeof(Vector3), 1, File);
	fwrite(&m_Max, sizeof(Vector3), 1, File);

	int	ContainerCount = (int)m_vecContainer.size();

	fwrite(&ContainerCount, sizeof(int), 1, File);

	for (int i = 0; i < ContainerCount; ++i)
	{
		MeshContainer* Container = m_vecContainer[i];

		fwrite(&Container->Primitive,
			sizeof(D3D11_PRIMITIVE_TOPOLOGY), 1, File);

		fwrite(&Container->VB.Size, sizeof(int), 1, File);
		fwrite(&Container->VB.Count, sizeof(int), 1, File);

		fwrite(Container->VB.Data, Container->VB.Size,
			Container->VB.Count, File);

		int	SubsetCount = (int)Container->vecSubset.size();

		fwrite(&SubsetCount, sizeof(int), 1, File);

		for (int j = 0; j < SubsetCount; ++j)
		{
			fwrite(&Container->vecSubset[j].IB.Size,
				sizeof(int), 1, File);
			fwrite(&Container->vecSubset[j].IB.Count,
				sizeof(int), 1, File);
			fwrite(&Container->vecSubset[j].IB.Fmt,
				sizeof(DXGI_FORMAT), 1, File);

			fwrite(Container->vecSubset[j].IB.Data,
				Container->vecSubset[j].IB.Size,
				Container->vecSubset[j].IB.Count, File);

			Container->vecSubset[j].Material->Save(File);
		}
	}

	return true;
}

bool CMesh::LoadMesh(FILE* File)
{
	size_t	Size = m_vecContainer.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecContainer[i]);
	}

	Size = m_vecMeshSlot.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecMeshSlot[i]);
	}

	CRef::Load(File);

	fread(&m_MeshType, sizeof(MeshType), 1, File);

	fread(&m_Min, sizeof(Vector3), 1, File);
	fread(&m_Max, sizeof(Vector3), 1, File);

	int	ContainerCount = 0;

	fread(&ContainerCount, sizeof(int), 1, File);

	for (int i = 0; i < ContainerCount; ++i)
	{
		MeshContainer* Container = new MeshContainer;

		m_vecContainer.push_back(Container);

		fread(&Container->Primitive,
			sizeof(D3D11_PRIMITIVE_TOPOLOGY), 1, File);

		fread(&Container->VB.Size, sizeof(int), 1, File);
		fread(&Container->VB.Count, sizeof(int), 1, File);

		Container->VB.Data = new char[Container->VB.Size * Container->VB.Count];

		fread(Container->VB.Data, Container->VB.Size,
			Container->VB.Count, File);

		CreateBuffer(BufferType::Vertex, Container->VB.Data,
			Container->VB.Size, Container->VB.Count,
			D3D11_USAGE_DEFAULT, &Container->VB.Buffer);

		int	SubsetCount = 0;

		fread(&SubsetCount, sizeof(int), 1, File);

		Container->vecSubset.reserve(SubsetCount);

		for (int j = 0; j < SubsetCount; ++j)
		{
			MeshSlot* Slot = new MeshSlot;

			m_vecMeshSlot.push_back(Slot);

			Slot->VB = &Container->VB;
			Slot->Primitive = Container->Primitive;

			MeshSubset	Subset = {};

			Container->vecSubset.emplace_back(Subset);

			Container->vecSubset[j].Slot = Slot;

			fread(&Container->vecSubset[j].IB.Size,
				sizeof(int), 1, File);
			fread(&Container->vecSubset[j].IB.Count,
				sizeof(int), 1, File);
			fread(&Container->vecSubset[j].IB.Fmt,
				sizeof(DXGI_FORMAT), 1, File);

			Container->vecSubset[j].IB.Data =
				new char[Container->vecSubset[j].IB.Size *
				Container->vecSubset[j].IB.Count];

			fread(Container->vecSubset[j].IB.Data,
				Container->vecSubset[j].IB.Size,
				Container->vecSubset[j].IB.Count, File);

			if (!CreateBuffer(BufferType::Index,
				Container->vecSubset[j].IB.Data,
				Container->vecSubset[j].IB.Size,
				Container->vecSubset[j].IB.Count,
				D3D11_USAGE_DEFAULT, &Container->vecSubset[j].IB.Buffer))
				return false;

			Slot->IB = &Container->vecSubset[j].IB;

			CMaterial* Material = new CMaterial;

			Material->Load(File);

			if (Material)
			{
				if (Material->GetSpecularColor() == Vector4(0.f, 0.f, 0.f, 0.f))
				{
					Vector4 color = Material->GetBaseColor();
					color.w = 0.f;
					Material->SetEmissiveColor(color);
				}

				else
				{
					Material->SetEmissiveColor(1.f, 1.f, 1.f, 0.f);
				}

				SetMaterial(i, j, Material);
			}

		}
	}

	return true;
}
