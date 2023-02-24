
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

	unsigned int RectIndex[6] = { 0, 1, 3, 0, 3, 2 };

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


	VertexUV CenterUVMesh[4] =
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

	VertexUV LBUVMesh[4] =
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

	for (int i = 0; i < 360; i++)
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

	std::vector<Vertex3D> vecSphere;
	std::vector<Vector3> vecSpherePos;
	std::vector<int> vecSphereIndex;

	CreateSphere(vecSphere, vecSphereIndex, 1.f, 10);

	size_t	SphereSize = vecSphere.size();
	vecSpherePos.resize(SphereSize);

	for (size_t i = 0; i < SphereSize; i++)
	{
		vecSpherePos[i] = vecSphere[i].Pos;
	}

	CreateMesh(nullptr, MeshType::Static, "SpherePos",
		&vecSpherePos[0], sizeof(Vector3), (int)SphereSize,
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		&vecSphereIndex[0], sizeof(int), (int)vecSphereIndex.size(),
		D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);

	Vector3 Point;

	CreateMesh(nullptr, MeshType::Static, "ParticlePoint",
		&Point, sizeof(Vector3), 1, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	Vector3	CubePos[8] =
	{
		Vector3(-0.5f, 0.5f, -0.5f),
		Vector3(0.5f, 0.5f, -0.5f),
		Vector3(-0.5f, -0.5f, -0.5f),
		Vector3(0.5f, -0.5f, -0.5f),

		Vector3(-0.5f, 0.5f, 0.5f),
		Vector3(0.5f, 0.5f, 0.5f),
		Vector3(-0.5f, -0.5f, 0.5f),
		Vector3(0.5f, -0.5f, 0.5f)
	};

	int	CubeLineIndex[24] =
	{
		0, 1, 1, 3, 2, 3, 0, 2, 4, 0, 5, 1, 6, 2, 7, 3, 4, 5, 5, 7, 7, 6, 6, 4
	};


	CreateMesh(nullptr, MeshType::Static, "CubeLinePos",
		&CubePos[0], sizeof(Vector3), 8,
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		&CubeLineIndex[0], sizeof(int), 24,
		D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);

	int	CubeIndex[36] =
	{
		0, 1, 3, 0, 3, 2,	// Near
		1, 5, 7, 1, 7, 3,	// Right
		5, 4, 6, 5, 6, 7,	// Far
		4, 0, 2, 4, 2, 6,	// Left
		4, 5, 1, 4, 1, 0,	// Top
		2, 3, 7, 2, 7, 6	// Bottom
	};


	CreateMesh(nullptr, MeshType::Static, "CubePos",
		&CubePos[0], sizeof(Vector3), 8,
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		&CubeIndex[0], sizeof(int), 36,
		D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);

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
	auto iter = m_mapMesh.find(Name);

	if (iter == m_mapMesh.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CMeshManager::ReleaseMesh(const std::string& Name)
{
	auto	iter = m_mapMesh.find(Name);

	if (iter != m_mapMesh.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			m_mapMesh.erase(iter);
		}
	}
}

bool CMeshManager::CreateSphere(std::vector<Vertex3D>& vecVertex, std::vector<int>& vecIndex, float Radius,
	unsigned int SubDivision)
{
	// Put a cap on the number of subdivisions.
	SubDivision = min(SubDivision, 5);

	// Approximate a sphere by tessellating an icosahedron.
	const float X = 0.525731f;
	const float Z = 0.850651f;

	Vector3 pos[12] =
	{
		Vector3(-X, 0.0f, Z),  Vector3(X, 0.0f, Z),
		Vector3(-X, 0.0f, -Z), Vector3(X, 0.0f, -Z),
		Vector3(0.0f, Z, X),   Vector3(0.0f, Z, -X),
		Vector3(0.0f, -Z, X),  Vector3(0.0f, -Z, -X),
		Vector3(Z, X, 0.0f),   Vector3(-Z, X, 0.0f),
		Vector3(Z, -X, 0.0f),  Vector3(-Z, -X, 0.0f)
	};

	DWORD k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	vecVertex.resize(12);
	vecIndex.resize(60);

	for (UINT i = 0; i < 12; i++)
	{
		vecVertex[i].Pos = pos[i];
	}

	for (UINT i = 0; i < 60; i++)
	{
		vecIndex[i] = k[i];
	}

	for (UINT i = 0; i < SubDivision; i++)
	{
		Subdivide(vecVertex, vecIndex);
	}

	// Project vertices onto sphere and scale.
	for (UINT i = 0; i < vecVertex.size(); i++)
	{
		// Project onto unit sphere.
		Vector3	vN = vecVertex[i].Pos;
		vN.Normalize();

		// Project onto sphere.
		Vector3 p = vN * Radius;

		vecVertex[i].Pos = p;
		// Normal이 있을 경우 따로 저장한다.

		// Derive texture coordinates from spherical coordinates.
		float theta = AngleFromXY(vecVertex[i].Pos.x, vecVertex[i].Pos.z);

		float phi = acosf(vecVertex[i].Pos.y / Radius);

		vecVertex[i].UV.x = theta / DirectX::XM_2PI;
		vecVertex[i].UV.y = phi / DirectX::XM_PI;

		// Partial derivative of P with respect to theta
		vecVertex[i].Tangent.x = -Radius * sinf(phi) * sinf(theta);
		vecVertex[i].Tangent.y = 0.0f;
		vecVertex[i].Tangent.z = Radius * sinf(phi) * cosf(theta);

		vecVertex[i].Binormal = vecVertex[i].Normal.Cross(vecVertex[i].Tangent);
		vecVertex[i].Binormal.Normalize();
	}

	return true;
}

void CMeshManager::Subdivide(std::vector<Vertex3D>& vecVertices, std::vector<int>& vecIndices)
{
	// Save a copy of the input geometry.
	std::vector<Vertex3D> vecCopyVertex = vecVertices;
	std::vector<int> vecCopyIndex = vecIndices;

	vecVertices.resize(0);
	vecIndices.resize(0);

	//       v1
	//       *
	//      / \
					//     /   \
	//  m0*-----*m1
//   / \   / \
	//  /   \ /   \
	// *-----*-----*
// v0    m2     v2

	UINT numTris = (UINT)(vecCopyIndex.size() / 3);

	for (UINT i = 0; i < numTris; i++)
	{
		Vertex3D v0 = vecCopyVertex[vecCopyIndex[i * 3 + 0]];
		Vertex3D v1 = vecCopyVertex[vecCopyIndex[i * 3 + 1]];
		Vertex3D v2 = vecCopyVertex[vecCopyIndex[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		Vertex3D m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.

		m0.Pos = Vector3( 0.5f * (v0.Pos.x + v1.Pos.x), 0.5f * (v0.Pos.y + v1.Pos.y), 0.5f * (v0.Pos.z + v1.Pos.z));

		m1.Pos = Vector3( 0.5f * (v1.Pos.x + v2.Pos.x), 0.5f * (v1.Pos.y + v2.Pos.y), 0.5f * (v1.Pos.z + v2.Pos.z));

		m2.Pos = Vector3( 0.5f * (v0.Pos.x + v2.Pos.x), 0.5f * (v0.Pos.y + v2.Pos.y), 0.5f * (v0.Pos.z + v2.Pos.z));

		//
		// Add new geometry.
		//

		vecVertices.push_back(v0); // 0
		vecVertices.push_back(v1); // 1
		vecVertices.push_back(v2); // 2
		vecVertices.push_back(m0); // 3
		vecVertices.push_back(m1); // 4
		vecVertices.push_back(m2); // 5

		vecIndices.push_back(i * 6 + 0);
		vecIndices.push_back(i * 6 + 3);
		vecIndices.push_back(i * 6 + 5);

		vecIndices.push_back(i * 6 + 3);
		vecIndices.push_back(i * 6 + 4);
		vecIndices.push_back(i * 6 + 5);

		vecIndices.push_back(i * 6 + 5);
		vecIndices.push_back(i * 6 + 4);
		vecIndices.push_back(i * 6 + 2);

		vecIndices.push_back(i * 6 + 3);
		vecIndices.push_back(i * 6 + 1);
		vecIndices.push_back(i * 6 + 4);
	}
}

float CMeshManager::AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
		{
			theta += 2.0f * PI; // in [0, 2*pi).
		} 
	}

	// Quadrant II or III
	else
	{
		theta = atanf(y / x) + PI; // in [0, 2*pi).
	} 

	return theta;
}