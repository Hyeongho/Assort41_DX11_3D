#include "Navigation3D.h"
#include "../Component/TerrainComponent.h"

CNavigation3D::CNavigation3D()
{
}

CNavigation3D::~CNavigation3D()
{
	size_t	Size = m_vecNode.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecNode[i]);
	}
}

void CNavigation3D::CreateNavigation(CTerrainComponent* Terrain)
{
	m_Terrain = Terrain;

	/*m_Shape = m_Terrain->GetShape();
	m_CountX = m_Terrain->GetCountX();
	m_CountY = m_Terrain->GetCountY();

	m_TileSize = m_Terrain->GetTileSize();

	int	Count = m_CountX * m_CountY;

	for (int i = 0; i < Count; ++i)
	{
		NavNode1* Node = new NavNode1;

		Node->TileOption = m_Terrain->GetTile(i)->GetTileOption();
		Node->Pos = m_Terrain->GetTile(i)->GetPos();
		Node->Size = m_Terrain->GetTileSize();
		Node->Center = Node->Pos + Node->Size * 0.5f;
		Node->IndexX = m_Terrain->GetTile(i)->GetIndexX();
		Node->IndexY = m_Terrain->GetTile(i)->GetIndexY();
		Node->Index = m_Terrain->GetTile(i)->GetIndex();

		m_vecNode.push_back(Node);
	}*/
}

bool CNavigation3D::FindPath(const Vector2& Start, const Vector2& End,
	std::list<Vector2>& PathList)
{
	//PathList.clear();

	//int	StartIndex = m_Terrain->GetTileIndex(Start);

	//if (StartIndex == -1)
	//	return false;

	//int EndIndex = m_Terrain->GetTileIndex(End);

	//if (EndIndex == -1)
	//	return false;

	//NavNode1* StartNode = m_vecNode[StartIndex];
	//NavNode1* EndNode = m_vecNode[EndIndex];

	//if (EndNode->TileOption == ETileOption::Wall)
	//	return false;

	//else if (EndNode == StartNode)
	//{
	//	PathList.push_back(End);
	//	return true;
	//}

	//size_t Size = m_vecUseNode.size();

	//for (size_t i = 0; i < Size; ++i)
	//{
	//	m_vecUseNode[i]->NodeType = ENavNodeType1::None;
	//	m_vecUseNode[i]->Cost = FLT_MAX;
	//	m_vecUseNode[i]->Dist = FLT_MAX;
	//	m_vecUseNode[i]->Total = FLT_MAX;
	//	m_vecUseNode[i]->Parent = nullptr;
	//	m_vecUseNode[i]->SearchDirList.clear();
	//}

	//m_vecUseNode.clear();

	//StartNode->NodeType = ENavNodeType1::Open;
	//StartNode->Cost = 0.f;
	//StartNode->Dist = StartNode->Center.Distance(End);
	//StartNode->Total = StartNode->Dist;

	//for (int i = 0; i < (int)ENodeDir1::End; ++i)
	//{
	//	StartNode->SearchDirList.push_back((ENodeDir1)i);
	//}

	//m_vecOpen.push_back(StartNode);

	//m_vecUseNode.push_back(StartNode);

	//while (!m_vecOpen.empty())
	//{
	//	// ���� ��Ͽ��� ��带 �����´�.
	//	NavNode1* Node = m_vecOpen.back();
	//	m_vecOpen.pop_back();

	//	Node->NodeType = ENavNodeType1::Close;

	//	// ���� ��尡 �˻��ؾ��� ������ Ÿ���� �˻��Ͽ� �ڳʸ� ������Ͽ� �־��ش�.
	//	if (FindNode(Node, EndNode, End, PathList))
	//		break;

	//	// ���� ��Ͽ� �ִ� ��带 ����� ū ��忡�� ���� ��� ������ �������ش�.
	//	if (m_vecOpen.size() >= 2)
	//		std::sort(m_vecOpen.begin(), m_vecOpen.end(), CNavigation3D::SortNode);

	//}

	//m_vecOpen.clear();

	//return !PathList.empty();

	return true;
}

bool CNavigation3D::FindNode(NavNode1* Node, NavNode1* EndNode, const Vector2& End,
	std::list<Vector2>& PathList)
{
	auto iter = Node->SearchDirList.begin();
	auto iterEnd = Node->SearchDirList.end();

	for (; iter != iterEnd; iter++)
	{
		NavNode1* Corner = GetCorner(*iter, Node, EndNode, End);

		if (!Corner)
			continue;

		// ã���� ��尡 ���� ����� ��θ� ������ش�.
		if (Corner == EndNode)
		{
			m_vecUseNode.push_back(Corner);
			PathList.push_back(End);

			NavNode1* PathNode = Node;

			while (PathNode)
			{
				PathList.push_front(PathNode->Center);
				PathNode = PathNode->Parent;
			}

			PathList.pop_front();

			return true;
		}

		// �̵� ����� �����ش�.
		float Cost = 0.f;

		switch (*iter)
		{
		case ENodeDir1::T:
		case ENodeDir1::B:
			Cost = Node->Cost + Node->Size.y;
			break;
		case ENodeDir1::R:
		case ENodeDir1::L:
			Cost = Node->Cost + Node->Size.x;
			break;
		case ENodeDir1::RT:
		case ENodeDir1::RB:
		case ENodeDir1::LB:
		case ENodeDir1::LT:
			Cost = Node->Cost + Node->Center.Distance(Corner->Center);
			break;
		}

		// ã���� ��尡 ������Ͽ� �� �ִ� ����� ����� ���Ͽ� ���� �������
		// ��ü�Ѵ�.
		if (Corner->NodeType == ENavNodeType1::Open)
		{
			if (Corner->Cost > Cost)
			{
				Corner->Cost = Cost;
				Corner->Total = Corner->Cost + Corner->Dist;
				Corner->Parent = Node;

				// ������ ������ �־��ش�.
				AddDir(*iter, Corner);
			}
		}

		else
		{
			Corner->NodeType = ENavNodeType1::Open;
			Corner->Parent = Node;
			Corner->Cost = Cost;
			Corner->Dist = Corner->Center.Distance(End);
			Corner->Total = Corner->Cost + Corner->Dist;

			m_vecOpen.push_back(Corner);

			m_vecUseNode.push_back(Corner);

			AddDir(*iter, Corner);
		}
	}

	return false;
}

NavNode1* CNavigation3D::GetCorner(ENodeDir1 Dir, NavNode1* Node, NavNode1* EndNode,
	const Vector2& End)
{
	switch (m_Shape)
	{
	case ETileShape::Rect:
		switch (Dir)
		{
		case ENodeDir1::T:
			return GetCornerRectTop(Node, EndNode);
		case ENodeDir1::RT:
			return GetCornerRectRT(Node, EndNode);
		case ENodeDir1::R:
			return GetCornerRectRight(Node, EndNode);
		case ENodeDir1::RB:
			return GetCornerRectRB(Node, EndNode);
		case ENodeDir1::B:
			return GetCornerRectBottom(Node, EndNode);
		case ENodeDir1::LB:
			return GetCornerRectLB(Node, EndNode);
		case ENodeDir1::L:
			return GetCornerRectLeft(Node, EndNode);
		case ENodeDir1::LT:
			return GetCornerRectLT(Node, EndNode);
		}
		break;
	case ETileShape::Isometric:
		switch (Dir)
		{
		case ENodeDir1::T:
			return GetCornerIsometricTop(Node, EndNode);
		case ENodeDir1::RT:
			return GetCornerIsometricRT(Node, EndNode);
		case ENodeDir1::R:
			return GetCornerIsometricRight(Node, EndNode);
		case ENodeDir1::RB:
			return GetCornerIsometricRB(Node, EndNode);
		case ENodeDir1::B:
			return GetCornerIsometricBottom(Node, EndNode);
		case ENodeDir1::LB:
			return GetCornerIsometricLB(Node, EndNode);
		case ENodeDir1::L:
			return GetCornerIsometricLeft(Node, EndNode);
		case ENodeDir1::LT:
			return GetCornerIsometricLT(Node, EndNode);
		}
		break;
	}

	return nullptr;
}

NavNode1* CNavigation3D::GetCornerRectTop(NavNode1* Node, NavNode1* EndNode)
{
	// ��ĭ�� ���� �ö󰡸� �������� �����ְ� ������ ���� �շ����� ��쳪
	// ������ �����ְ� ���� ���� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	IndexY = Node->IndexY;

	while (true)
	{
		++IndexY;

		if (IndexY >= m_CountY)
			return nullptr;

		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + Node->IndexX];

		if (CornerNode == EndNode)
			return CornerNode;

		else if (CornerNode->NodeType == ENavNodeType1::Close)
			return nullptr;

		else if (CornerNode->TileOption == ETileOption::Wall)
			return nullptr;

		int	CornerX = Node->IndexX + 1;
		int	CornerY = IndexY;

		if (CornerX < m_CountX && CornerY + 1 < m_CountY)
		{
			// �������� ���̰� ������ ���� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[(CornerY + 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
				return CornerNode;
		}

		CornerX = Node->IndexX - 1;
		CornerY = IndexY;

		if (CornerX >= 0 && CornerY + 1 < m_CountY)
		{
			// ������ ���̰� ���� ���� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[(CornerY + 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
				return CornerNode;
		}
	}

	return nullptr;
}

NavNode1* CNavigation3D::GetCornerRectBottom(NavNode1* Node, NavNode1* EndNode)
{
	// ��ĭ�� �Ʒ��� �������� �������� �����ְ� ������ �Ʒ��� �շ����� ��쳪
	// ������ �����ְ� ���� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	IndexY = Node->IndexY;

	while (true)
	{
		--IndexY;

		if (IndexY < 0)
			return nullptr;

		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + Node->IndexX];

		if (CornerNode == EndNode)
			return CornerNode;

		else if (CornerNode->NodeType == ENavNodeType1::Close)
			return nullptr;

		else if (CornerNode->TileOption == ETileOption::Wall)
			return nullptr;

		int	CornerX = Node->IndexX + 1;
		int	CornerY = IndexY;

		if (CornerX < m_CountX && CornerY - 1 >= 0)
		{
			// �������� ���̰� ������ �Ʒ��� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[(CornerY - 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
				return CornerNode;
		}

		CornerX = Node->IndexX - 1;
		CornerY = IndexY;

		if (CornerX >= 0 && CornerY - 1 >= 0)
		{
			// ������ ���̰� ���� �Ʒ��� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[(CornerY - 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
				return CornerNode;
		}
	}

	return nullptr;
}

NavNode1* CNavigation3D::GetCornerRectLeft(NavNode1* Node, NavNode1* EndNode)
{
	// ��ĭ�� �������� ���� ���� �����ְ� ���� ���� �շ����� ��쳪
	// �Ʒ��� �����ְ� ���� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	IndexX = Node->IndexX;

	while (true)
	{
		--IndexX;

		if (IndexX < 0)
			return nullptr;

		NavNode1* CornerNode = m_vecNode[Node->IndexY * m_CountX + IndexX];

		if (CornerNode == EndNode)
			return CornerNode;

		else if (CornerNode->NodeType == ENavNodeType1::Close)
			return nullptr;

		else if (CornerNode->TileOption == ETileOption::Wall)
			return nullptr;

		int	CornerX = IndexX;
		int	CornerY = Node->IndexY + 1;

		if (CornerX - 1 >= 0 && CornerY < m_CountY)
		{
			// ������ ���̰� ���� ���� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[CornerY * m_CountX + (CornerX - 1)]->TileOption == ETileOption::None)
				return CornerNode;
		}

		CornerX = IndexX;
		CornerY = Node->IndexY - 1;

		if (CornerX - 1 >= 0 && CornerY >= 0)
		{
			// �Ʒ����� ���̰� ���� �Ʒ��� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[CornerY * m_CountX + (CornerX - 1)]->TileOption == ETileOption::None)
				return CornerNode;
		}
	}

	return nullptr;
}

NavNode1* CNavigation3D::GetCornerRectRight(NavNode1* Node, NavNode1* EndNode)
{
	// ��ĭ�� ���������� ���� ���� �����ְ� ������ ���� �շ����� ��쳪
	// �Ʒ��� �����ְ� ������ �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	IndexX = Node->IndexX;

	while (true)
	{
		++IndexX;

		if (IndexX >= m_CountX)
			return nullptr;

		NavNode1* CornerNode = m_vecNode[Node->IndexY * m_CountX + IndexX];

		if (CornerNode == EndNode)
			return CornerNode;

		else if (CornerNode->NodeType == ENavNodeType1::Close)
			return nullptr;

		else if (CornerNode->TileOption == ETileOption::Wall)
			return nullptr;

		int	CornerX = IndexX;
		int	CornerY = Node->IndexY + 1;

		if (CornerX + 1 < m_CountX && CornerY < m_CountY)
		{
			// ������ ���̰� ������ ���� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[CornerY * m_CountX + (CornerX + 1)]->TileOption == ETileOption::None)
				return CornerNode;
		}

		CornerX = IndexX;
		CornerY = Node->IndexY - 1;

		if (CornerX + 1 < m_CountX && CornerY >= 0)
		{
			// �Ʒ����� ���̰� ������ �Ʒ��� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[CornerY * m_CountX + (CornerX + 1)]->TileOption == ETileOption::None)
				return CornerNode;
		}
	}

	return nullptr;
}

NavNode1* CNavigation3D::GetCornerRectLT(NavNode1* Node, NavNode1* EndNode)
{
	// ��ĭ�� ���� ���� ���� �������� �����ְ� ������ ���� �շ����� ��쳪
	// �Ʒ��� �����ְ� ���� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		--IndexX;
		++IndexY;

		if (IndexX < 0 || IndexY >= m_CountY)
			return nullptr;

		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];

		if (CornerNode == EndNode)
			return CornerNode;

		else if (CornerNode->NodeType == ENavNodeType1::Close)
			return nullptr;

		else if (CornerNode->TileOption == ETileOption::Wall)
			return nullptr;

		int	CornerX = IndexX;
		int	CornerY = IndexY - 1;

		if (CornerX - 1 >= 0 && CornerY >= 0)
		{
			// �Ʒ����� ���̰� ���� �Ʒ��� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[CornerY * m_CountX + (CornerX - 1)]->TileOption == ETileOption::None)
				return CornerNode;
		}

		CornerX = IndexX + 1;
		CornerY = IndexY;

		if (CornerX < m_CountX && CornerY + 1 < m_CountY)
		{
			// �������� ���̰� ������ ���� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[(CornerY + 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
				return CornerNode;
		}

		// �밢�� �ڳʰ� �ƴ϶�� ���� ���� ������ �ǹǷ� ����, �׸��� ��������
		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
		// �Ѵ�.
		NavNode1* Find = GetCornerRectTop(CornerNode, EndNode);

		// �������� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
		if (Find)
			return CornerNode;

		Find = GetCornerRectLeft(CornerNode, EndNode);

		if (Find)
			return CornerNode;
	}

	return nullptr;
}

NavNode1* CNavigation3D::GetCornerRectRT(NavNode1* Node, NavNode1* EndNode)
{
	// ��ĭ�� ������ ���� ���� ������ �����ְ� ���� ���� �շ����� ��쳪
	// �Ʒ��� �����ְ� ������ �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		++IndexX;
		++IndexY;

		if (IndexX >= m_CountX || IndexY >= m_CountY)
			return nullptr;

		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];

		if (CornerNode == EndNode)
			return CornerNode;

		else if (CornerNode->NodeType == ENavNodeType1::Close)
			return nullptr;

		else if (CornerNode->TileOption == ETileOption::Wall)
			return nullptr;

		int	CornerX = IndexX;
		int	CornerY = IndexY - 1;

		if (CornerX + 1 < m_CountX && CornerY >= 0)
		{
			// �Ʒ����� ���̰� ������ �Ʒ��� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[CornerY * m_CountX + (CornerX + 1)]->TileOption == ETileOption::None)
				return CornerNode;
		}

		CornerX = IndexX - 1;
		CornerY = IndexY;

		if (CornerX >= 0 && CornerY + 1 < m_CountY)
		{
			// ������ ���̰� ���� ���� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[(CornerY + 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
				return CornerNode;
		}

		// �밢�� �ڳʰ� �ƴ϶�� ������ ���� ������ �ǹǷ� ������, �׸��� ��������
		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
		// �Ѵ�.
		NavNode1* Find = GetCornerRectTop(CornerNode, EndNode);

		// �������� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
		if (Find)
			return CornerNode;

		Find = GetCornerRectRight(CornerNode, EndNode);

		if (Find)
			return CornerNode;
	}

	return nullptr;
}

NavNode1* CNavigation3D::GetCornerRectLB(NavNode1* Node, NavNode1* EndNode)
{
	// ��ĭ�� ���� �Ʒ��� ���� �������� �����ְ� ������ �Ʒ��� �շ����� ��쳪
	// ���� �����ְ� ���� ���� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		--IndexX;
		--IndexY;

		if (IndexX < 0 || IndexY < 0)
			return nullptr;

		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];

		if (CornerNode == EndNode)
			return CornerNode;

		else if (CornerNode->NodeType == ENavNodeType1::Close)
			return nullptr;

		else if (CornerNode->TileOption == ETileOption::Wall)
			return nullptr;

		int	CornerX = IndexX;
		int	CornerY = IndexY + 1;

		if (CornerX - 1 >= 0 && CornerY < m_CountY)
		{
			// ������ ���̰� ���� ���� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[CornerY * m_CountX + (CornerX - 1)]->TileOption == ETileOption::None)
				return CornerNode;
		}

		CornerX = IndexX + 1;
		CornerY = IndexY;

		if (CornerX < m_CountX && CornerY - 1 >= 0)
		{
			// �������� ���̰� ������ �Ʒ��� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[(CornerY - 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
				return CornerNode;
		}

		// �밢�� �ڳʰ� �ƴ϶�� ���� �Ʒ��� ������ �ǹǷ� ����, �׸��� �Ʒ�������
		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
		// �Ѵ�.
		NavNode1* Find = GetCornerRectBottom(CornerNode, EndNode);

		// �Ʒ������� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
		if (Find)
			return CornerNode;

		Find = GetCornerRectLeft(CornerNode, EndNode);

		if (Find)
			return CornerNode;
	}

	return nullptr;
}

NavNode1* CNavigation3D::GetCornerRectRB(NavNode1* Node, NavNode1* EndNode)
{
	// ��ĭ�� ������ �Ʒ��� ���� ������ �����ְ� ���� �Ʒ��� �շ����� ��쳪
	// ���� �����ְ� ������ ���� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		++IndexX;
		--IndexY;

		if (IndexX >= m_CountX || IndexY < 0)
			return nullptr;

		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];

		if (CornerNode == EndNode)
			return CornerNode;

		else if (CornerNode->NodeType == ENavNodeType1::Close)
			return nullptr;

		else if (CornerNode->TileOption == ETileOption::Wall)
			return nullptr;

		int	CornerX = IndexX;
		int	CornerY = IndexY + 1;

		if (CornerX + 1 < m_CountX && CornerY < m_CountY)
		{
			// ������ ���̰� ������ ���� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[CornerY * m_CountX + (CornerX + 1)]->TileOption == ETileOption::None)
				return CornerNode;
		}

		CornerX = IndexX - 1;
		CornerY = IndexY;

		if (CornerX >= 0 && CornerY - 1 >= 0)
		{
			// ������ ���̰� ���� �Ʒ��� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[(CornerY - 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
				return CornerNode;
		}

		// �밢�� �ڳʰ� �ƴ϶�� ������ �Ʒ��� ������ �ǹǷ� ������, �׸��� �Ʒ�������
		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
		// �Ѵ�.
		NavNode1* Find = GetCornerRectBottom(CornerNode, EndNode);

		// �Ʒ������� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
		if (Find)
			return CornerNode;

		Find = GetCornerRectRight(CornerNode, EndNode);

		if (Find)
			return CornerNode;
	}

	return nullptr;
}

NavNode1* CNavigation3D::GetCornerIsometricTop(NavNode1* Node, NavNode1* EndNode)
{
	// ��ĭ�� ���� ���� ���� �Ʒ��� �����ְ� ������ �շ����� ��쳪
	// ������ �Ʒ��� �����ְ� �������� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		IndexY += 2;

		if (IndexY >= m_CountY)
			return nullptr;

		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];

		if (CornerNode == EndNode)
			return CornerNode;

		else if (CornerNode->NodeType == ENavNodeType1::Close)
			return nullptr;

		else if (CornerNode->TileOption == ETileOption::Wall)
			return nullptr;

		int	CornerX = IndexX;

		if (IndexY % 2 == 0)
			CornerX = IndexX - 1;

		int	CornerY = IndexY - 1;

		if (IndexX - 1 >= 0)
		{
			// ���� �Ʒ��� ���̰� ���� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[IndexY * m_CountX + (IndexX - 1)]->TileOption == ETileOption::None)
				return CornerNode;
		}

		CornerX = IndexX;

		if (IndexY % 2 == 1)
			CornerX = IndexX + 1;

		if (IndexX + 1 < m_CountX)
		{
			// ������ �Ʒ��� ���̰� �������� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[IndexY * m_CountX + (IndexX + 1)]->TileOption == ETileOption::None)
				return CornerNode;
		}

		// �밢�� �ڳʰ� �ƴ϶�� ���� ������ �ǹǷ� ������ ��, �׸��� ���� ����
		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
		// �Ѵ�.
		NavNode1* Find = GetCornerIsometricRT(CornerNode, EndNode);

		// ������ ���� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
		if (Find)
			return CornerNode;

		Find = GetCornerIsometricLT(CornerNode, EndNode);

		if (Find)
			return CornerNode;
	}

	return nullptr;
}

NavNode1* CNavigation3D::GetCornerIsometricBottom(NavNode1* Node, NavNode1* EndNode)
{
	// ��ĭ�� �Ʒ��� ���� ���� ���� �����ְ� ������ �շ����� ��쳪
	// ������ ���� �����ְ� �������� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		IndexY -= 2;

		if (IndexY < 0)
			return nullptr;

		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];

		if (CornerNode == EndNode)
			return CornerNode;

		else if (CornerNode->NodeType == ENavNodeType1::Close)
			return nullptr;

		else if (CornerNode->TileOption == ETileOption::Wall)
			return nullptr;

		int	CornerX = IndexX;

		if (IndexY % 2 == 0)
			CornerX = IndexX - 1;

		int	CornerY = IndexY + 1;

		if (IndexX - 1 >= 0)
		{
			// ���� ���� ���̰� ���� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[IndexY * m_CountX + (IndexX - 1)]->TileOption == ETileOption::None)
				return CornerNode;
		}

		CornerX = IndexX;

		if (IndexY % 2 == 1)
			CornerX = IndexX + 1;

		if (IndexX + 1 < m_CountX)
		{
			// ������ ���� ���̰� �������� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[IndexY * m_CountX + (IndexX + 1)]->TileOption == ETileOption::None)
				return CornerNode;
		}

		// �밢�� �ڳʰ� �ƴ϶�� �Ʒ��� ������ �ǹǷ� ������ �Ʒ�, �׸��� ���� �Ʒ���
		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
		// �Ѵ�.
		NavNode1* Find = GetCornerIsometricRB(CornerNode, EndNode);

		// ������ �Ʒ��� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
		if (Find)
			return CornerNode;

		Find = GetCornerIsometricLB(CornerNode, EndNode);

		if (Find)
			return CornerNode;
	}

	return nullptr;
}

NavNode1* CNavigation3D::GetCornerIsometricLeft(NavNode1* Node, NavNode1* EndNode)
{
	// ��ĭ�� �������� ���� ������ ���� �����ְ� ���� �շ����� ��쳪
	// ������ �Ʒ��� �����ְ� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		--IndexX;

		if (IndexX < 0)
			return nullptr;

		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];

		if (CornerNode == EndNode)
			return CornerNode;

		else if (CornerNode->NodeType == ENavNodeType1::Close)
			return nullptr;

		else if (CornerNode->TileOption == ETileOption::Wall)
			return nullptr;

		int	CornerX = IndexX;

		if (IndexY % 2 == 1)
			CornerX = IndexX + 1;

		int	CornerY = IndexY + 1;

		if (CornerX < m_CountX && IndexY + 2 < m_CountY)
		{
			// ������ ���� ���̰� �� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[(IndexY + 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
				return CornerNode;
		}

		CornerX = IndexX;

		if (IndexY % 2 == 1)
			CornerX = IndexX + 1;

		CornerY = IndexY - 1;

		if (CornerX < m_CountX && IndexY - 2 >= 0)
		{
			// ������ �Ʒ��� ���̰� �Ʒ��� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[(IndexY - 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
				return CornerNode;
		}

		// �밢�� �ڳʰ� �ƴ϶�� �������� ������ �ǹǷ� ���� ��, �׸��� ���� �Ʒ���
		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
		// �Ѵ�.
		NavNode1* Find = GetCornerIsometricLT(CornerNode, EndNode);

		// ���� ���� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
		if (Find)
			return CornerNode;

		Find = GetCornerIsometricLB(CornerNode, EndNode);

		if (Find)
			return CornerNode;
	}

	return nullptr;
}

NavNode1* CNavigation3D::GetCornerIsometricRight(NavNode1* Node, NavNode1* EndNode)
{
	// ��ĭ�� ���������� ���� ���� ���� �����ְ� ���� �շ����� ��쳪
	// ���� �Ʒ��� �����ְ� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		++IndexX;

		if (IndexX >= m_CountX)
			return nullptr;

		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];

		if (CornerNode == EndNode)
			return CornerNode;

		else if (CornerNode->NodeType == ENavNodeType1::Close)
			return nullptr;

		else if (CornerNode->TileOption == ETileOption::Wall)
			return nullptr;

		int	CornerX = IndexX;

		if (IndexY % 2 == 0)
			CornerX = IndexX - 1;

		int	CornerY = IndexY + 1;

		if (CornerX >= 0 && IndexY + 2 < m_CountY)
		{
			// ���� ���� ���̰� �� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[(IndexY + 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
				return CornerNode;
		}

		CornerX = IndexX;

		if (IndexY % 2 == 0)
			CornerX = IndexX - 1;

		CornerY = IndexY - 1;

		if (CornerX >= 0 && IndexY - 2 >= 0)
		{
			// ���� �Ʒ��� ���̰� �Ʒ��� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[(IndexY - 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
				return CornerNode;
		}

		// �밢�� �ڳʰ� �ƴ϶�� ���������� ������ �ǹǷ� ������ ��, �׸��� ������ �Ʒ���
		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
		// �Ѵ�.
		NavNode1* Find = GetCornerIsometricRT(CornerNode, EndNode);

		// ������ ���� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
		if (Find)
			return CornerNode;

		Find = GetCornerIsometricRB(CornerNode, EndNode);

		if (Find)
			return CornerNode;
	}

	return nullptr;
}

NavNode1* CNavigation3D::GetCornerIsometricLT(NavNode1* Node, NavNode1* EndNode)
{
	// ��ĭ�� ���� ���� �ö󰡸� ������ ���� �����ְ� ���� �շ����� ��쳪
	// ���� �Ʒ��� �����ְ� ������ �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		if (IndexY % 2 == 0)
			--IndexX;

		++IndexY;

		if (IndexY >= m_CountY || IndexX < 0)
			return nullptr;

		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];

		if (CornerNode == EndNode)
			return CornerNode;

		else if (CornerNode->NodeType == ENavNodeType1::Close)
			return nullptr;

		else if (CornerNode->TileOption == ETileOption::Wall)
			return nullptr;

		int	CornerX = IndexX;
		int	CornerY = IndexY + 1;

		if (IndexY % 2 == 1)
			++CornerX;

		if (CornerX < m_CountX && IndexY + 2 < m_CountY)
		{
			// ������ ���� ���̰� ���� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[(IndexY + 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
				return CornerNode;
		}

		CornerX = IndexX;

		if (IndexY % 2 == 0)
			--CornerX;

		CornerY = IndexY - 1;

		if (IndexX - 1 >= 0 && CornerY >= 0)
		{
			// ���� �Ʒ��� ���̰� ������ �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[IndexY * m_CountX + (IndexX - 1)]->TileOption == ETileOption::None)
				return CornerNode;
		}
	}

	return nullptr;
}

NavNode1* CNavigation3D::GetCornerIsometricRT(NavNode1* Node, NavNode1* EndNode)
{
	// ��ĭ�� ������ ���� �ö󰡸� ���� ���� �����ְ� ���� �շ����� ��쳪
	// ������ �Ʒ��� �����ְ� �������� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		if (IndexY % 2 == 1)
			++IndexX;

		++IndexY;

		if (IndexY >= m_CountY || IndexX >= m_CountX)
			return nullptr;

		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];

		if (CornerNode == EndNode)
			return CornerNode;

		else if (CornerNode->NodeType == ENavNodeType1::Close)
			return nullptr;

		else if (CornerNode->TileOption == ETileOption::Wall)
			return nullptr;

		int	CornerX = IndexX;
		int	CornerY = IndexY + 1;

		if (IndexY % 2 == 0)
			--CornerX;

		if (CornerX >= 0 && IndexY + 2 < m_CountY)
		{
			// ���� ���� ���̰� ���� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[(IndexY + 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
				return CornerNode;
		}

		CornerX = IndexX;

		if (IndexY % 2 == 1)
			++CornerX;

		CornerY = IndexY - 1;

		if (CornerX < m_CountX && CornerY >= 0)
		{
			// ������ �Ʒ��� ���̰� �������� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[IndexY * m_CountX + (IndexX + 1)]->TileOption == ETileOption::None)
				return CornerNode;
		}
	}

	return nullptr;
}

NavNode1* CNavigation3D::GetCornerIsometricLB(NavNode1* Node, NavNode1* EndNode)
{
	// ��ĭ�� ���� �Ʒ��� �������� ���� ���� �����ְ� ������ �շ����� ��쳪
	// ������ �Ʒ��� �����ְ� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		if (IndexY % 2 == 0)
			--IndexX;

		--IndexY;

		if (IndexY < 0 || IndexX < 0)
			return nullptr;

		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];

		if (CornerNode == EndNode)
			return CornerNode;

		else if (CornerNode->NodeType == ENavNodeType1::Close)
			return nullptr;

		else if (CornerNode->TileOption == ETileOption::Wall)
			return nullptr;

		int	CornerX = IndexX;
		int	CornerY = IndexY + 1;

		if (IndexY % 2 == 0)
			--CornerX;

		if (IndexX - 1 >= 0 && CornerY < m_CountY)
		{
			// ���� ���� ���̰� ������ �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[IndexY * m_CountX + (IndexX - 1)]->TileOption == ETileOption::None)
				return CornerNode;
		}

		CornerX = IndexX;

		if (IndexY % 2 == 1)
			++CornerX;

		CornerY = IndexY - 1;

		if (CornerX < m_CountX && IndexY - 2 >= 0)
		{
			// ������ �Ʒ��� ���̰� �Ʒ��� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[(IndexY - 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
				return CornerNode;
		}
	}

	return nullptr;
}

NavNode1* CNavigation3D::GetCornerIsometricRB(NavNode1* Node, NavNode1* EndNode)
{
	// ��ĭ�� ������ �Ʒ��� �������� ������ ���� �����ְ� �������� �շ����� ��쳪
	// ���� �Ʒ��� �����ְ� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	IndexX = Node->IndexX;
	int	IndexY = Node->IndexY;

	while (true)
	{
		if (IndexY % 2 == 1)
			++IndexX;

		--IndexY;

		if (IndexY < 0 || IndexX >= m_CountX)
			return nullptr;

		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];

		if (CornerNode == EndNode)
			return CornerNode;

		else if (CornerNode->NodeType == ENavNodeType1::Close)
			return nullptr;

		else if (CornerNode->TileOption == ETileOption::Wall)
			return nullptr;

		int	CornerX = IndexX;
		int	CornerY = IndexY + 1;

		if (IndexY % 2 == 1)
			++CornerX;

		if (IndexX + 1 < m_CountX && CornerY < m_CountY)
		{
			// ������ ���� ���̰� �������� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[IndexY * m_CountX + (IndexX + 1)]->TileOption == ETileOption::None)
				return CornerNode;
		}

		CornerX = IndexX;

		if (IndexY % 2 == 0)
			--CornerX;

		CornerY = IndexY - 1;

		if (CornerX >= 0 && IndexY - 2 >= 0)
		{
			// ���� �Ʒ��� ���̰� �Ʒ��� �Ϲ��� ���
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
				m_vecNode[(IndexY - 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
				return CornerNode;
		}
	}

	return nullptr;
}

void CNavigation3D::AddDir(ENodeDir1 Dir, NavNode1* Node)
{
	Node->SearchDirList.clear();

	switch (m_Shape)
	{
	case ETileShape::Rect:
		switch (Dir)
		{
		case ENodeDir1::T:
			Node->SearchDirList.push_back(ENodeDir1::T);
			Node->SearchDirList.push_back(ENodeDir1::LT);
			Node->SearchDirList.push_back(ENodeDir1::RT);
			break;
		case ENodeDir1::RT:
			Node->SearchDirList.push_back(ENodeDir1::RT);
			Node->SearchDirList.push_back(ENodeDir1::T);
			Node->SearchDirList.push_back(ENodeDir1::R);
			Node->SearchDirList.push_back(ENodeDir1::LT);
			Node->SearchDirList.push_back(ENodeDir1::RB);
			break;
		case ENodeDir1::R:
			Node->SearchDirList.push_back(ENodeDir1::R);
			Node->SearchDirList.push_back(ENodeDir1::RT);
			Node->SearchDirList.push_back(ENodeDir1::RB);
			break;
		case ENodeDir1::RB:
			Node->SearchDirList.push_back(ENodeDir1::RB);
			Node->SearchDirList.push_back(ENodeDir1::R);
			Node->SearchDirList.push_back(ENodeDir1::B);
			Node->SearchDirList.push_back(ENodeDir1::RT);
			Node->SearchDirList.push_back(ENodeDir1::LB);
			break;
		case ENodeDir1::B:
			Node->SearchDirList.push_back(ENodeDir1::B);
			Node->SearchDirList.push_back(ENodeDir1::LB);
			Node->SearchDirList.push_back(ENodeDir1::RB);
			break;
		case ENodeDir1::LB:
			Node->SearchDirList.push_back(ENodeDir1::LB);
			Node->SearchDirList.push_back(ENodeDir1::L);
			Node->SearchDirList.push_back(ENodeDir1::B);
			Node->SearchDirList.push_back(ENodeDir1::LT);
			Node->SearchDirList.push_back(ENodeDir1::RB);
			break;
		case ENodeDir1::L:
			Node->SearchDirList.push_back(ENodeDir1::L);
			Node->SearchDirList.push_back(ENodeDir1::LT);
			Node->SearchDirList.push_back(ENodeDir1::LB);
			break;
		case ENodeDir1::LT:
			Node->SearchDirList.push_back(ENodeDir1::LT);
			Node->SearchDirList.push_back(ENodeDir1::T);
			Node->SearchDirList.push_back(ENodeDir1::L);
			Node->SearchDirList.push_back(ENodeDir1::RT);
			Node->SearchDirList.push_back(ENodeDir1::LB);
			break;
		}
		break;
	case ETileShape::Isometric:
		switch (Dir)
		{
		case ENodeDir1::T:
			Node->SearchDirList.push_back(ENodeDir1::T);
			Node->SearchDirList.push_back(ENodeDir1::RT);
			Node->SearchDirList.push_back(ENodeDir1::LT);
			Node->SearchDirList.push_back(ENodeDir1::R);
			Node->SearchDirList.push_back(ENodeDir1::L);
			break;
		case ENodeDir1::RT:
			Node->SearchDirList.push_back(ENodeDir1::RT);
			Node->SearchDirList.push_back(ENodeDir1::T);
			Node->SearchDirList.push_back(ENodeDir1::R);
			break;
		case ENodeDir1::R:
			Node->SearchDirList.push_back(ENodeDir1::R);
			Node->SearchDirList.push_back(ENodeDir1::RT);
			Node->SearchDirList.push_back(ENodeDir1::RB);
			Node->SearchDirList.push_back(ENodeDir1::T);
			Node->SearchDirList.push_back(ENodeDir1::B);
			break;
		case ENodeDir1::RB:
			Node->SearchDirList.push_back(ENodeDir1::RB);
			Node->SearchDirList.push_back(ENodeDir1::R);
			Node->SearchDirList.push_back(ENodeDir1::B);
			break;
		case ENodeDir1::B:
			Node->SearchDirList.push_back(ENodeDir1::B);
			Node->SearchDirList.push_back(ENodeDir1::RB);
			Node->SearchDirList.push_back(ENodeDir1::LB);
			Node->SearchDirList.push_back(ENodeDir1::R);
			Node->SearchDirList.push_back(ENodeDir1::L);
			break;
		case ENodeDir1::LB:
			Node->SearchDirList.push_back(ENodeDir1::LB);
			Node->SearchDirList.push_back(ENodeDir1::L);
			Node->SearchDirList.push_back(ENodeDir1::B);
			break;
		case ENodeDir1::L:
			Node->SearchDirList.push_back(ENodeDir1::L);
			Node->SearchDirList.push_back(ENodeDir1::LT);
			Node->SearchDirList.push_back(ENodeDir1::LB);
			Node->SearchDirList.push_back(ENodeDir1::T);
			Node->SearchDirList.push_back(ENodeDir1::B);
			break;
		case ENodeDir1::LT:
			Node->SearchDirList.push_back(ENodeDir1::LT);
			Node->SearchDirList.push_back(ENodeDir1::L);
			Node->SearchDirList.push_back(ENodeDir1::T);
			break;
		}
		break;
	}
}

bool CNavigation3D::SortNode(NavNode1* Src, NavNode1* Dest)
{
	return Src->Total > Dest->Total;
}
