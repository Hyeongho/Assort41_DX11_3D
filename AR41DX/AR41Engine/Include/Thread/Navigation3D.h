#pragma once

#include "../Component/TerrainComponent.h"

struct NavAdjInfo
{
	int	Index;
	int	EdgeIndex;
};

struct NavigationCell
{
	bool Enable;
	ENavNodeType NodeType;
	Vector3	Pos[3];
	Vector3	Edge[3];
	Vector3	EdgeCenter[3];
	std::vector<NavAdjInfo>	vecAdj;
	int Index;
	int ParentIndex;
	float Cost;
	float Dist;
	float Total;

	void Clear()
	{
		NodeType = ENavNodeType::None;
		ParentIndex = -1;
		Cost = -1.f;
		Dist = -1.f;
		Total = -1.f;
	}

	NavigationCell()
	{
		NodeType = ENavNodeType::None;
		ParentIndex = -1;
		Index = -1;
		Cost = -1.f;
		Dist = -1.f;
		Total = -1.f;
		Enable = true;
	}
};

struct NavigationSection
{
	Vector3 Size;
	std::vector<NavigationCell*> vecCell;
	Vector3 Min;
	Vector3 Max;

	void Add(NavigationCell* Cell)
	{
		vecCell.push_back(Cell);
	}
};

class CNavigation3D
{
	friend class CNavigation3DThread;
	friend class CNavigationManager3D;

private:
	CNavigation3D();
	~CNavigation3D();

private:
	std::vector<NavigationCell*>	m_vecCell;
	class CTerrainComponent* m_Terrain;
	std::vector<NavigationCell*>	m_vecOpen;
	std::vector<NavigationCell*>	m_vecUseNode;
	Vector3		m_Min;
	Vector3		m_Max;
	bool		m_Grid;
	int			m_LineRectCount;
	NavigationSection* m_SectionArray;
	int			m_SectionX;
	int			m_SectionZ;
	Vector3		m_SectionSize;
	Vector3		m_CellSize;

public:
	void CreateNavigation(class CTerrainComponent* Terrain);
	bool FindPath(const Vector2& Start, const Vector3& End, std::list<Vector2>& PathList);

private:
	bool FindNode(NavNode1* Node, NavNode1* EndNode, const Vector2& End, std::list<Vector2>& PathList);
	NavNode1* GetCorner(ENodeDir1 Dir, NavNode1* Node, NavNode1* EndNode, const Vector2& End);

	NavNode1* GetCornerRectTop(NavNode1* Node, NavNode1* EndNode);
	NavNode1* GetCornerRectBottom(NavNode1* Node, NavNode1* EndNode);
	NavNode1* GetCornerRectLeft(NavNode1* Node, NavNode1* EndNode);
	NavNode1* GetCornerRectRight(NavNode1* Node, NavNode1* EndNode);
	NavNode1* GetCornerRectLT(NavNode1* Node, NavNode1* EndNode);
	NavNode1* GetCornerRectRT(NavNode1* Node, NavNode1* EndNode);
	NavNode1* GetCornerRectLB(NavNode1* Node, NavNode1* EndNode);
	NavNode1* GetCornerRectRB(NavNode1* Node, NavNode1* EndNode);

	NavNode1* GetCornerIsometricTop(NavNode1* Node, NavNode1* EndNode);
	NavNode1* GetCornerIsometricBottom(NavNode1* Node, NavNode1* EndNode);
	NavNode1* GetCornerIsometricLeft(NavNode1* Node, NavNode1* EndNode);
	NavNode1* GetCornerIsometricRight(NavNode1* Node, NavNode1* EndNode);
	NavNode1* GetCornerIsometricLT(NavNode1* Node, NavNode1* EndNode);
	NavNode1* GetCornerIsometricRT(NavNode1* Node, NavNode1* EndNode);
	NavNode1* GetCornerIsometricLB(NavNode1* Node, NavNode1* EndNode);
	NavNode1* GetCornerIsometricRB(NavNode1* Node, NavNode1* EndNode);

	void AddDir(ENodeDir1 Dir, NavNode1* Node);

private:
	static bool SortNode(NavNode1* Src, NavNode1* Dest);
};

