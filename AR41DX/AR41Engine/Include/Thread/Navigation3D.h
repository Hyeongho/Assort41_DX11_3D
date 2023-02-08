#pragma once

#include "../Component/TerrainComponent.h"

struct NavigationCell
{
	// ���� ã�� ���� ������ �� �׺���̼Ǹ��� �ٸ��� ����Ǿ�� �ϱ� ������
	// ������ ������ش�.
	NavigationCell* Parent;
	bool Enable;
	ENavNodeType NodeType;
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
		Parent = nullptr;
	}

	NavigationCell()
	{
		Parent = nullptr;
		NodeType = ENavNodeType::None;
		ParentIndex = -1;
		Index = -1;
		Cost = -1.f;
		Dist = -1.f;
		Total = -1.f;
	}
};

class CNavigation3D
{
	friend class CNavigation3DThread;
	friend class CNavigationManager3D;

private:
	CNavigation3D();
	CNavigation3D(const CNavigation3D& Nav);
	~CNavigation3D();

private:
	class CNavigationMesh* m_NavMesh;
	std::vector<NavigationCell*> m_vecCell;
	std::vector<NavigationCell*> m_vecOpen;
	std::vector<NavigationCell*> m_vecUseNode;
	bool m_Grid;
	int m_LineRectCountX;
	int m_LineRectCountY;

public:
	void CreateNavigation(class CNavigationMesh* NavMesh);
	bool FindPath(const Vector3& Start, const Vector3& End, std::list<Vector3>& PathList);

private:
	bool FindNode(NavigationCell* Node, NavigationCell* StartNode, NavigationCell* EndNode, const Vector3& End, std::list<Vector3>& PathList);

private:
	static bool SortNode(NavigationCell* Src, NavigationCell* Dest);

public:
	CNavigation3D* Clone();
};

