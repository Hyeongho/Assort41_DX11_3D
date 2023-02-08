#pragma once

#include "../Component/TerrainComponent.h"

struct NavAdjInfo
{
	int	Index;
	int	EdgeIndex;
};

struct NavMeshInfo
{
	// 셀을 구성하는 위치 정보들은 1개만 있으면 되기 때문에
	// 공유할 수 있는 클래스에 1개만 만들어두고 모든 네비게이션이 공유한다.
	bool Enable;
	Vector3	Pos[3];
	Vector3	Min;
	Vector3	Max;
	Vector3	Edge[3];
	Vector3	EdgeCenter[3];
	Vector3	Center;
	std::vector<NavAdjInfo>	vecAdj;
	float Angle;
	int Index;

	NavMeshInfo()
	{
		Enable = true;
		Angle = 0.f;
		Index = 0;

		Min = Vector3(10000000.f, 10000000.f, 10000000.f);
		Max = Vector3(-10000000.f, -10000000.f, -10000000.f);
	}
};

struct NavigationSection
{
	Vector3 Size;
	std::vector<NavMeshInfo*> vecCell;
	Vector3 Min;
	Vector3 Max;

	void Add(NavMeshInfo* Cell)
	{
		vecCell.push_back(Cell);
	}
};

class CNavigationMesh
{
	friend class CNavigation3DThread;
	friend class CNavigationManager3D;
	friend class CNavigation3D;

private:
	CNavigationMesh();
	CNavigationMesh(const CNavigationMesh& Nav);
	~CNavigationMesh();

private:
	class CNavigationManager3D* m_Mgr;
	std::vector<NavMeshInfo*> m_vecCell;
	std::vector<NavMeshInfo*> m_vecCellFind;
	class CTerrainComponent* m_Terrain;
	Vector3 m_Min;
	Vector3 m_Max;
	bool m_Grid;
	int m_LineRectCountX;
	int m_LineRectCountY;
	NavigationSection* m_SectionArray;
	int m_SectionX;
	int m_SectionY;
	Vector3 m_SectionSize;
	Vector2 m_CellSize;

public:
	class CScene* GetScene() const;
	std::string GetSceneName() const;
	std::string GetComponentName() const;

public:
	void CreateNavigation(class CTerrainComponent* Terrain);

private:
	void CreateAdjGrid();
	void CreateAdj();
	bool CheckOnEdge(int Src, int Dest, const Vector3& Origin1, const Vector3& Origin2, const Vector3& Edge, float EdgeLength, int Edge1, int Edge2);
	float ccw(const Vector2& v1, const Vector2& v2);
	float ccw(const Vector2& v1, const Vector2& v2, const Vector2& v3);
	void CreateSection();

public:
	float GetHeight(const Vector3& Pos);

public:
	NavMeshInfo* FindNavMeshInfo(const Vector3& Pos);
	int FindNavMeshInfoIndex(const Vector3& Pos);
	NavMeshInfo* GetNavMeshInfo(int Index);
	bool GetPickingPos(Vector3& Result, const Ray& ray);

private:
	bool RayIntersectTriangle(Vector3& ImpactPoint, float& Dist,
		const Vector3& Pos, const Vector3& Dir,
		const Vector3& CellPos0, const Vector3& CellPos1,
		const Vector3& CellPos2);

private:
	static bool SortCamera(NavMeshInfo* Src, NavMeshInfo* Dest);
};

