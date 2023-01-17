
#include "MeshManager.h"
#include "SpriteMesh.h"
#include "StaticMesh.h"
#include "AnimationMesh.h"

CMeshManager::CMeshManager()
{
}

CMeshManager::~CMeshManager()
{
}

bool CMeshManager::Init()
{
	// 기본 메쉬 생성 센터 중심, 좌하단 중심
	VertexColor	CenterColorMesh[4] =
	{
		VertexColor(Vector3(-0.5f, 0.5f, 0.f), Vector4::Red),
		VertexColor(Vector3(0.5f, 0.5f, 0.f), Vector4::Green),
		VertexColor(Vector3(-0.5f, -0.5f, 0.f), Vector4::Blue),
		VertexColor(Vector3(0.5f, -0.5f, 0.f), Vector4(1.f, 0.f, 1.f, 1.f))
	};

	unsigned int	RectIndex[6] = { 0, 1, 3, 0, 3, 2 };

	CreateMesh(nullptr, MeshType::Sprite, "CenterColorRect",
		CenterColorMesh, sizeof(VertexColor), 4, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, RectIndex,
		4, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT);

	CMesh* Mesh = FindMesh("CenterColorRect");

	Mesh->SetMaterial(0, 0, "DefaultColor");

	VertexColor	LBColorMesh[4] =
	{
		VertexColor(Vector3(0.f, 1.f, 0.f), Vector4::Red),
		VertexColor(Vector3(1.f, 1.f, 0.f), Vector4::Green),
		VertexColor(Vector3(0.f, 0.f, 0.f), Vector4::Blue),
		VertexColor(Vector3(1.f, 0.f, 0.f), Vector4(1.f, 0.f, 1.f, 1.f))
	};

	CreateMesh(nullptr, MeshType::Sprite, "LBColorRect",
		LBColorMesh, sizeof(VertexColor), 4, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, RectIndex,
		4, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT);

	Mesh = FindMesh("LBColorRect");

	Mesh->SetMaterial(0, 0, "DefaultColor");


	VertexUV	CenterUVMesh[4] =
	{
		VertexUV(Vector3(-0.5f, 0.5f, 0.f), Vector2(0.f, 0.f)),
		VertexUV(Vector3(0.5f, 0.5f, 0.f), Vector2(1.f, 0.f)),
		VertexUV(Vector3(-0.5f, -0.5f, 0.f), Vector2(0.f, 1.f)),
		VertexUV(Vector3(0.5f, -0.5f, 0.f), Vector2(1.f, 1.f))
	};

	CreateMesh(nullptr, MeshType::Sprite, "CenterUVRect",
		CenterUVMesh, sizeof(VertexUV), 4, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, RectIndex,
		4, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT);

	Mesh = FindMesh("CenterUVRect");

	Mesh->SetMaterial(0, 0, "DefaultTexture");

	VertexUV	LBUVMesh[4] =
	{
		VertexUV(Vector3(0.f, 1.f, 0.f), Vector2(0.f, 0.f)),
		VertexUV(Vector3(1.f, 1.f, 0.f), Vector2(1.f, 0.f)),
		VertexUV(Vector3(0.f, 0.f, 0.f), Vector2(0.f, 1.f)),
		VertexUV(Vector3(1.f, 0.f, 0.f), Vector2(1.f, 1.f))
	};

	CreateMesh(nullptr, MeshType::Sprite, "LBUVRect",
		LBUVMesh, sizeof(VertexUV), 4, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, RectIndex,
		4, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT);

	Mesh = FindMesh("LBUVRect");

	Mesh->SetMaterial(0, 0, "DefaultTexture");

	Vector3	Box2DLineMesh[5] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f),
		Vector3(-0.5f, 0.5f, 0.f)
	};

	CreateMesh(nullptr, MeshType::Sprite, "Box2DLineMesh",
		Box2DLineMesh, sizeof(Vector3), 5, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	Vector3	Sphere2DLineMesh[361];

	Sphere2DLineMesh[0].x = 1.f;
	Sphere2DLineMesh[0].y = 0.f;

	for (int i = 0; i < 360; ++i)
	{
		float Radian = (i + 1) * PI / 180.f;

		Sphere2DLineMesh[i + 1].x = cosf(Radian);
		Sphere2DLineMesh[i + 1].y = sinf(Radian);
	}

	Sphere2DLineMesh[360].x = 1.f;
	Sphere2DLineMesh[360].y = 0.f;

	CreateMesh(nullptr, MeshType::Sprite, "Sphere2DLineMesh",
		Sphere2DLineMesh, sizeof(Vector3), 361, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	return true;
}

bool CMeshManager::CreateMesh(CScene* Scene, MeshType Type, const std::string& Name,
	void* VtxData, int Size, int Count, D3D11_USAGE VtxUsage,
	D3D11_PRIMITIVE_TOPOLOGY Primitive, void* IdxData, int IdxSize, 
	int IdxCount, D3D11_USAGE IdxUsage, DXGI_FORMAT Fmt)
{
	if (FindMesh(Name))
		return true;

	CMesh* Mesh = nullptr;

	switch (Type)
	{
	case MeshType::Sprite:
		Mesh = new CSpriteMesh;
		break;
	case MeshType::UI:
		break;
	case MeshType::Static:
		Mesh = new CStaticMesh;
		break;
	case MeshType::Animation:
		Mesh = new CAnimationMesh;
		break;
	}

	Mesh->SetScene(Scene);
	Mesh->SetName(Name);

	if (!Mesh->CreateMesh(VtxData, Size, Count, VtxUsage, Primitive,
		IdxData, IdxSize, IdxCount, IdxUsage, Fmt))
	{
		SAFE_RELEASE(Mesh);
		return false;
	}

	m_mapMesh.insert(std::make_pair(Name, Mesh));

	return true;
}

bool CMeshManager::LoadMesh(CScene* Scene, MeshType Type, 
	const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	if (FindMesh(Name))
		return true;

	CMesh* Mesh = nullptr;

	switch (Type)
	{
	case MeshType::Sprite:
		Mesh = new CSpriteMesh;
		break;
	case MeshType::UI:
		break;
	case MeshType::Static:
		Mesh = new CStaticMesh;
		break;
	case MeshType::Animation:
		Mesh = new CAnimationMesh;
		break;
	}

	Mesh->SetScene(Scene);
	Mesh->SetName(Name);

	if (!Mesh->LoadMesh(Name, FileName, PathName))
	{
		SAFE_RELEASE(Mesh);
		return false;
	}

	m_mapMesh.insert(std::make_pair(Name, Mesh));

	return true;
}

bool CMeshManager::LoadMeshFullPath(CScene* Scene, MeshType Type, 
	const std::string& Name, const TCHAR* FullPath)
{
	if (FindMesh(Name))
		return true;

	CMesh* Mesh = nullptr;

	switch (Type)
	{
	case MeshType::Sprite:
		Mesh = new CSpriteMesh;
		break;
	case MeshType::UI:
		break;
	case MeshType::Static:
		Mesh = new CStaticMesh;
		break;
	case MeshType::Animation:
		Mesh = new CAnimationMesh;
		break;
	}

	Mesh->SetScene(Scene);
	Mesh->SetName(Name);

	if (!Mesh->LoadMeshFullPath(Name, FullPath))
	{
		SAFE_RELEASE(Mesh);
		return false;
	}

	m_mapMesh.insert(std::make_pair(Name, Mesh));

	return true;
}

bool CMeshManager::LoadMeshMultibyte(CScene* Scene, MeshType Type,
	const std::string& Name, const char* FileName, const std::string& PathName)
{
	if (FindMesh(Name))
		return true;

	CMesh* Mesh = nullptr;

	switch (Type)
	{
	case MeshType::Sprite:
		Mesh = new CSpriteMesh;
		break;
	case MeshType::UI:
		break;
	case MeshType::Static:
		Mesh = new CStaticMesh;
		break;
	case MeshType::Animation:
		Mesh = new CAnimationMesh;
		break;
	}

	Mesh->SetScene(Scene);
	Mesh->SetName(Name);

	if (!Mesh->LoadMeshMultibyte(Name, FileName, PathName))
	{
		SAFE_RELEASE(Mesh);
		return false;
	}

	m_mapMesh.insert(std::make_pair(Name, Mesh));

	return true;
}

bool CMeshManager::LoadMeshMultibyteFullPath(CScene* Scene, 
	MeshType Type, const std::string& Name, const char* FullPath)
{
	if (FindMesh(Name))
		return true;

	CMesh* Mesh = nullptr;

	switch (Type)
	{
	case MeshType::Sprite:
		Mesh = new CSpriteMesh;
		break;
	case MeshType::UI:
		break;
	case MeshType::Static:
		Mesh = new CStaticMesh;
		break;
	case MeshType::Animation:
		Mesh = new CAnimationMesh;
		break;
	}

	Mesh->SetScene(Scene);
	Mesh->SetName(Name);

	if (!Mesh->LoadMeshMultibyteFullPath(Name, FullPath))
	{
		SAFE_RELEASE(Mesh);
		return false;
	}

	m_mapMesh.insert(std::make_pair(Name, Mesh));

	return true;
}

CMesh* CMeshManager::FindMesh(const std::string& Name)
{
	auto	iter = m_mapMesh.find(Name);

	if (iter == m_mapMesh.end())
		return nullptr;

	return iter->second;
}

void CMeshManager::ReleaseMesh(const std::string& Name)
{
	auto	iter = m_mapMesh.find(Name);

	if (iter != m_mapMesh.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapMesh.erase(iter);
	}
}
