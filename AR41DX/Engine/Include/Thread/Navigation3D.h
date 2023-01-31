#pragma once

#include "../Component/TerrainComponent.h"

enum class ENodeDir1
{
	T,
	RT,
	R,
	RB,
	B,
	LB,
	L,
	LT,
	End
};

enum class ENavNodeType1
{
	None,
	Open,
	Close
};

struct NavNode1
{
	NavNode1* Parent;
	ENavNodeType1 NodeType;
	ETileOption TileOption;
	Vector2 Pos;
	Vector2 Size;
	Vector2 Center;
	int IndexX;
	int IndexY;
	int Index;
	float Cost;
	float Dist;
	float Total;
	std::list<ENodeDir1> SearchDirList;

	NavNode1() :
		Parent(nullptr),
		NodeType(ENavNodeType1::None),
		TileOption(ETileOption::None),
		IndexX(-1),
		IndexY(-1),
		Index(-1),
		Cost(FLT_MAX),
		Dist(FLT_MAX),
		Total(FLT_MAX)
	{
	}
};

class CNavigation3D
{
	friend class CNavigation3DThread;

private:
	CNavigation3D();
	~CNavigation3D();

private:
	ETileShape m_Shape;
	std::vector<NavNode1*> m_vecNode;
	int m_CountX;
	int m_CountY;
	Vector2	m_TileSize;
	class CTerrainComponent* m_Terrain;
	std::vector<NavNode1*> m_vecOpen;
	std::vector<NavNode1*> m_vecUseNode;

public:
	void CreateNavigation(class CTerrainComponent* Terrain);
	bool FindPath(const Vector2& Start, const Vector2& End, std::list<Vector2>& PathList);

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

