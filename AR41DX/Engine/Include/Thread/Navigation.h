#pragma once

#include "../Component/TileMapComponent.h"

enum class ENodeDir
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

enum class ENavNodeType
{
	None,
	Open,
	Close
};

struct NavNode
{
	NavNode* Parent;
	ENavNodeType NodeType;
	ETileOption	 TileOption;
	Vector2 Pos;
	Vector2 Size;
	Vector2 Center;
	int IndexX;
	int IndexY;
	int Index;
	float Cost;
	float Dist;
	float Total;
	std::list<ENodeDir>	SearchDirList;

	NavNode()	:
		Parent(nullptr),
		NodeType(ENavNodeType::None),
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

class CNavigation
{
	friend class CNavigation2DThread;

private:
	CNavigation();
	~CNavigation();

private:
	ETileShape	m_Shape;
	std::vector<NavNode*> m_vecNode;
	int m_CountX;
	int m_CountY;
	Vector2	m_TileSize;
	class CTileMapComponent* m_TileMap;
	std::vector<NavNode*> m_vecOpen;
	std::vector<NavNode*> m_vecUseNode;

public:
	void CreateNavigation(class CTileMapComponent* TileMap);
	bool FindPath(const Vector2& Start, const Vector2& End,
		std::list<Vector2>& PathList);

private:
	bool FindNode(NavNode* Node, NavNode* EndNode, const Vector2& End, std::list<Vector2>& PathList);
	NavNode* GetCorner(ENodeDir Dir, NavNode* Node, NavNode* EndNode, const Vector2& End);

	NavNode* GetCornerRectTop(NavNode* Node, NavNode* EndNode);
	NavNode* GetCornerRectBottom(NavNode* Node, NavNode* EndNode);
	NavNode* GetCornerRectLeft(NavNode* Node, NavNode* EndNode);
	NavNode* GetCornerRectRight(NavNode* Node, NavNode* EndNode);
	NavNode* GetCornerRectLT(NavNode* Node, NavNode* EndNode);
	NavNode* GetCornerRectRT(NavNode* Node, NavNode* EndNode);
	NavNode* GetCornerRectLB(NavNode* Node, NavNode* EndNode);
	NavNode* GetCornerRectRB(NavNode* Node, NavNode* EndNode);

	NavNode* GetCornerIsometricTop(NavNode* Node, NavNode* EndNode);
	NavNode* GetCornerIsometricBottom(NavNode* Node, NavNode* EndNode);
	NavNode* GetCornerIsometricLeft(NavNode* Node, NavNode* EndNode);
	NavNode* GetCornerIsometricRight(NavNode* Node, NavNode* EndNode);
	NavNode* GetCornerIsometricLT(NavNode* Node, NavNode* EndNode);
	NavNode* GetCornerIsometricRT(NavNode* Node, NavNode* EndNode);
	NavNode* GetCornerIsometricLB(NavNode* Node, NavNode* EndNode);
	NavNode* GetCornerIsometricRB(NavNode* Node, NavNode* EndNode);

	void AddDir(ENodeDir Dir, NavNode* Node);

private:
	static bool SortNode(NavNode* Src, NavNode* Dest);
};

