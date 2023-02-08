#include "Navigation3D.h"
#include "../Component/TerrainComponent.h"
#include "../Scene/NavigationMesh.h"

CNavigation3D::CNavigation3D() : m_Grid(true), m_LineRectCountX(0), m_LineRectCountY(0)
{
}

CNavigation3D::CNavigation3D(const CNavigation3D& Nav)
{
	m_vecCell.clear();

	size_t	Size = Nav.m_vecCell.size();

	for (size_t i = 0; i < Size; i++)
	{
		NavigationCell* Cell = new NavigationCell;

		*Cell = *Nav.m_vecCell[i];

		m_vecCell.push_back(Cell);
	}

	m_Grid = Nav.m_Grid;
	m_LineRectCountX = Nav.m_LineRectCountX;
	m_LineRectCountY = Nav.m_LineRectCountY;
}

CNavigation3D::~CNavigation3D()
{
	size_t	Size = m_vecCell.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecCell[i]);
	}
}

void CNavigation3D::CreateNavigation(CNavigationMesh* NavMesh)
{
	m_NavMesh = NavMesh;

	m_Grid = m_NavMesh->m_Grid;

	m_LineRectCountX = m_NavMesh->m_LineRectCountX;
	m_LineRectCountY = m_NavMesh->m_LineRectCountY;

	size_t	Size = m_NavMesh->m_vecCell.size();

	for (size_t i = 0; i < Size; i++)
	{
		NavigationCell* Cell = new NavigationCell;

		Cell->Enable = m_NavMesh->m_vecCell[i]->Enable;
		Cell->Index = m_NavMesh->m_vecCell[i]->Index;

		m_vecCell.push_back(Cell);
	}
}

bool CNavigation3D::FindPath(const Vector3& Start, const Vector3& End,
	std::list<Vector3>& PathList)
{
	PathList.clear();

	NavMeshInfo* StartCell = m_NavMesh->FindNavMeshInfo(Start);

	if (!StartCell)
	{
		return false;
	}

	NavMeshInfo* EndCell = m_NavMesh->FindNavMeshInfo(End);

	if (!EndCell)
	{
		return false;
	}

	if (!StartCell->Enable || !EndCell->Enable)
	{
		return false;
	}

	else if (StartCell == EndCell)
	{
		PathList.push_back(End);
		return true;
	}

	size_t Size = m_vecUseNode.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecUseNode[i]->NodeType = ENavNodeType::None;
		m_vecUseNode[i]->Cost = FLT_MAX;
		m_vecUseNode[i]->Dist = FLT_MAX;
		m_vecUseNode[i]->Total = FLT_MAX;
		m_vecUseNode[i]->Parent = nullptr;
	}

	m_vecUseNode.clear();

	NavigationCell* StartNode = m_vecCell[StartCell->Index];
	NavigationCell* EndNode = m_vecCell[EndCell->Index];

	StartNode->NodeType = ENavNodeType::Open;
	StartNode->Cost = 0.f;
	StartNode->Dist = StartCell->Center.Distance(End);
	StartNode->Total = StartNode->Dist;

	m_vecOpen.push_back(StartNode);

	m_vecUseNode.push_back(StartNode);

	while (!m_vecOpen.empty())
	{
		// 열린 목록에서 노드를 가져온다.
		NavigationCell* Node = m_vecOpen.back();
		m_vecOpen.pop_back();

		Node->NodeType = ENavNodeType::Close;

		// 현재 노드가 검사해야할 방향의 타일을 검사하여 코너를 열린목록에 넣어준다.
		if (FindNode(Node, StartNode, EndNode, End, PathList))
		{
			break;
		}

		// 열린 목록에 있는 노드를 비용이 큰 노드에서 작은 노드 순서로 정렬해준다.
		if (m_vecOpen.size() >= 2)
		{
			std::sort(m_vecOpen.begin(), m_vecOpen.end(), CNavigation3D::SortNode);
		}

	}

	m_vecOpen.clear();

	return !PathList.empty();
}

CNavigation3D* CNavigation3D::Clone()
{
	return new CNavigation3D(*this);
}

bool CNavigation3D::FindNode(NavigationCell* Node, NavigationCell* StartNode, NavigationCell* EndNode, const Vector3& End, std::list<Vector3>& PathList)
{
	NavMeshInfo* NodeInfo = m_NavMesh->GetNavMeshInfo(Node->Index);

	size_t AdjSize = NodeInfo->vecAdj.size();

	for (size_t i = 0; i < AdjSize; ++i)
	{
		if (NodeInfo->vecAdj[i].Index == -1)
		{
			continue;
		}

		int	AdjIndex = NodeInfo->vecAdj[i].Index;

		NavMeshInfo* AdjNode = m_NavMesh->GetNavMeshInfo(AdjIndex);

		if (!AdjNode->Enable)
		{
			continue;
		}

		else if (m_vecCell[AdjIndex]->NodeType == ENavNodeType::Close)
		{
			continue;
		}

		// 도착할 셀을 찾았을 경우
		if (EndNode->Index == AdjIndex)
		{
			m_vecUseNode.push_back(m_vecCell[AdjIndex]);
			PathList.push_back(End);

			std::stack<int> PathStack;

			NavigationCell* PathNode = EndNode;

			while (PathNode)
			{
				PathStack.push(PathNode->Index);
				PathNode = PathNode->Parent;
			}

			std::vector<int>	vecPathIndex;

			while (!PathStack.empty())
			{
				int	Index = PathStack.top();
				vecPathIndex.push_back(Index);

				PathStack.pop();
			}

			size_t	PathIndexCount = vecPathIndex.size();

			for (size_t j = 0; j < PathIndexCount - 1; j++)
			{
				NavMeshInfo* Info = m_NavMesh->GetNavMeshInfo(vecPathIndex[j]);

				int	EdgeIndex = -1;

				size_t	AdjSize1 = Info->vecAdj.size();

				for (size_t k = 0; k < AdjSize1; k++)
				{
					if (Info->vecAdj[k].Index == vecPathIndex[j + 1])
					{
						EdgeIndex = Info->vecAdj[k].EdgeIndex;
						break;
					}
				}

				if (EdgeIndex != -1)
				{
					PathList.push_front(Info->EdgeCenter[EdgeIndex]);
				}
			}

			return true;
		}

		Vector3 Line = AdjNode->Center - NodeInfo->Center;

		float Cost = Line.x * Line.x + Line.z * Line.z;

		Line = AdjNode->Center - End;

		float Dist = Line.x * Line.x + Line.z * Line.z;

		if (Cost < 0.f || Dist < 0.f)
		{
		}

		Cost = sqrtf(Cost);
		Dist = sqrtf(Dist);

		if (m_vecCell[AdjIndex]->NodeType == ENavNodeType::None)
		{
			m_vecUseNode.push_back(m_vecCell[AdjIndex]);

			m_vecCell[AdjIndex]->Cost = Cost + Node->Cost;
			m_vecCell[AdjIndex]->Dist = Dist;
			m_vecCell[AdjIndex]->Total = m_vecCell[AdjIndex]->Cost + m_vecCell[AdjIndex]->Dist;
			m_vecCell[AdjIndex]->Parent = Node;
			m_vecCell[AdjIndex]->ParentIndex = Node->Index;
			m_vecCell[AdjIndex]->NodeType = ENavNodeType::Open;

			m_vecOpen.push_back(m_vecCell[AdjIndex]);
		}

		else if (m_vecCell[AdjIndex]->Cost > Cost + Node->Cost)
		{
			m_vecCell[AdjIndex]->Cost = Cost + Node->Cost;
			m_vecCell[AdjIndex]->Dist = Dist;
			m_vecCell[AdjIndex]->Total = m_vecCell[AdjIndex]->Cost + m_vecCell[AdjIndex]->Dist;
			m_vecCell[AdjIndex]->Parent = Node;
			m_vecCell[AdjIndex]->ParentIndex = Node->Index;
		}
	}

	return false;
}

bool CNavigation3D::SortNode(NavigationCell* Src, NavigationCell* Dest)
{
	return Src->Total > Dest->Total;
}
