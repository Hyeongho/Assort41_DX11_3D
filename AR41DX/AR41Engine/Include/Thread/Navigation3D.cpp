#include "Navigation3D.h"
#include "../Component/TerrainComponent.h"

CNavigation3D::CNavigation3D() :
	m_SectionArray(nullptr),
	m_Terrain(nullptr),
	m_Grid(true),
	m_LineRectCountX(0),
	m_LineRectCountY(0),
	m_SectionX(0),
	m_SectionY(0),
	m_Min(10000000.f, 10000000.f, 10000000.f),
	m_Max(-10000000.f, -10000000.f, -10000000.f)
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

	m_CellSize = Nav.m_CellSize;
	m_Terrain = Nav.m_Terrain;
	m_Min = Nav.m_Min;
	m_Max = Nav.m_Max;
	m_Grid = Nav.m_Grid;
	m_LineRectCountX = Nav.m_LineRectCountX;
	m_LineRectCountY = Nav.m_LineRectCountY;

	m_SectionX = Nav.m_SectionX;
	m_SectionY = Nav.m_SectionY;
	m_SectionSize = Nav.m_SectionSize;

	if (Nav.m_SectionArray)
	{
		m_SectionArray = new NavigationSection[m_SectionX * m_SectionY];

		for (int i = 0; i < m_SectionX * m_SectionY; i++)
		{
			m_SectionArray[i] = Nav.m_SectionArray[i];
		}
	}
}

CNavigation3D::~CNavigation3D()
{
	size_t	Size = m_vecCell.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecCell[i]);
	}

	SAFE_DELETE_ARRAY(m_SectionArray);
}

void CNavigation3D::CreateNavigation(CTerrainComponent* Terrain)
{
	m_Terrain = Terrain;

	m_Grid = m_Terrain->GetGrid();

	m_LineRectCountX = m_Terrain->GetCountX() - 1;
	m_LineRectCountY = m_Terrain->GetCountY() - 1;

	m_CellSize = m_Terrain->GetCellSize();

	int	FaceCount = m_Terrain->GetFaceCount();

	for (int i = 0; i < FaceCount; i++)
	{
		unsigned int	FaceIndex[3] = {};

		m_Terrain->GetFaceIndex(FaceIndex, i);

		NavigationCell* Cell = new NavigationCell;

		for (int j = 0; j < 3; ++j)
		{
			Cell->Pos[j] = m_Terrain->GetVertexPos(FaceIndex[j]);
		}

		Cell->Edge[0] = Cell->Pos[1] - Cell->Pos[0];
		Cell->EdgeCenter[0] = (Cell->Pos[1] + Cell->Pos[0]) / 2.f;

		Cell->Edge[1] = Cell->Pos[2] - Cell->Pos[1];
		Cell->EdgeCenter[1] = (Cell->Pos[2] + Cell->Pos[1]) / 2.f;

		Cell->Edge[2] = Cell->Pos[2] - Cell->Pos[0];
		Cell->EdgeCenter[2] = (Cell->Pos[2] + Cell->Pos[0]) / 2.f;

		Cell->Center = (Cell->Pos[0] + Cell->Pos[1] + Cell->Pos[2]) / 3.f;

		Cell->Index = (int)m_vecCell.size();

		for (int j = 0; j < 3; j++)
		{
			if (m_Min.x > Cell->Pos[j].x)
			{
				m_Min.x = Cell->Pos[j].x;
			}

			if (m_Min.y > Cell->Pos[j].y)
			{
				m_Min.y = Cell->Pos[j].y;
			}

			if (m_Min.z > Cell->Pos[j].z)
			{
				m_Min.z = Cell->Pos[j].z;
			}

			if (m_Max.x < Cell->Pos[j].x)
			{
				m_Max.x = Cell->Pos[j].x;
			}

			if (m_Max.y < Cell->Pos[j].y)
			{
				m_Max.y = Cell->Pos[j].y;
			}

			if (m_Max.z < Cell->Pos[j].z)
			{
				m_Max.z = Cell->Pos[j].z;
			}

			if (Cell->Min.x > Cell->Pos[j].x)
			{
				Cell->Min.x = Cell->Pos[j].x;
			}

			if (Cell->Min.y > Cell->Pos[j].y)
			{
				Cell->Min.y = Cell->Pos[j].y;
			}

			if (Cell->Min.z > Cell->Pos[j].z)
			{
				Cell->Min.z = Cell->Pos[j].z;
			}

			if (Cell->Max.x < Cell->Pos[j].x)
			{
				Cell->Max.x = Cell->Pos[j].x;
			}

			if (Cell->Max.y < Cell->Pos[j].y)
			{
				Cell->Max.y = Cell->Pos[j].y;
			}

			if (Cell->Max.z < Cell->Pos[j].z)
			{
				Cell->Max.z = Cell->Pos[j].z;
			}
		}

		Vector3	Dir1 = Cell->Edge[0];
		Vector3	Dir2 = Cell->Edge[2];

		Dir1.Normalize();
		Dir2.Normalize();

		Vector3	FaceNormal = Dir1.Cross(Dir2);
		FaceNormal.Normalize();

		float Angle = Vector3::Axis[AXIS_Y].Angle(FaceNormal);

		Cell->Angle = Angle;

		if (Angle < 45.f)
		{
			Cell->Enable = false;
		}

		else if (Angle >= 50.f)
		{
			float	MinY = Cell->Pos[0].y;
			float	MaxY = Cell->Pos[0].y;

			MinY = MinY > Cell->Pos[1].y ? Cell->Pos[1].y : MinY;
			MinY = MinY > Cell->Pos[2].y ? Cell->Pos[2].y : MinY;

			MaxY = MaxY < Cell->Pos[1].y ? Cell->Pos[1].y : MaxY;
			MaxY = MaxY < Cell->Pos[2].y ? Cell->Pos[2].y : MaxY;

			float	Height = MaxY - MinY;

			if (Height >= 30.f)
			{
				Cell->Enable = false;
			}
		}

		m_vecCell.push_back(Cell);
	}

	if (m_Grid)
	{
		CreateAdjGrid();
	}

	else
	{
		CreateAdj();
	}

	CreateSection();
}

void CNavigation3D::CreateAdjGrid()
{
	size_t	Size = m_vecCell.size();

	for (size_t i = 0; i < Size; i += 2)
	{
		NavAdjInfo	Info = {};

		// ���� �ﰢ�� �������� ����.
		// ���� �簢���� �� �ϴ� �ﰢ��, ������ �簢���� �� �ϴ� �ﰢ��,
		// �ڱ� �ڽ� �簢���� �� �ϴ� �ﰢ���̴�.
		// 
		// ���� �� �ϴ� �ﰢ�� �ε���
		int	Index = (int)i - m_LineRectCountX * 2 + 1;
		if (Index >= 0 && Index < (int)Size)
		{
			Info.Index = Index;
			Info.EdgeIndex = 0;
			m_vecCell[i]->vecAdj.push_back(Info);
		}

		// �ڱ��ڽ� �簢���� ���� �ϴ� �ﰢ���̴�.
		Index = (int)i + 1;

		Info.Index = Index;
		Info.EdgeIndex = 2;
		m_vecCell[i]->vecAdj.push_back(Info);

		// ������ �簢���� ���� �ϴ� �ﰢ���̴�.
		Index = (int)i + 3;

		// ������ �� �簢������ �Ǵ��Ѵ�.
		if (i / 2 % m_LineRectCountX != m_LineRectCountX - 1)
		{
			Info.Index = Index;
			Info.EdgeIndex = 1;
			m_vecCell[i]->vecAdj.push_back(Info);
		}

		// ���ϴ� �ﰢ�� �������� ����
		// ���� �簢���� ����, �ڱ��ڽ� �簢���� ����,
		// �Ʒ��� �簢���� ���� �ﰢ���̴�.

		// �ڱ��ڽ� �簢���� ����
		Index = (int)i;

		Info.Index = Index;
		Info.EdgeIndex = 0;
		m_vecCell[i + 1]->vecAdj.push_back(Info);

		// ���� �簢���� ����
		if ((int)i / 2 % m_LineRectCountX != 0)
		{
			// i�� ���� �ﰢ���� �ε����̹Ƿ� 2�� ���ش�.
			Index = (int)i - 2;
			Info.Index = Index;
			Info.EdgeIndex = 2;
			m_vecCell[i + 1]->vecAdj.push_back(Info);
		}

		// �Ʒ��� �簢���� ����.
		Index = (int)i + m_LineRectCountX * 2;
		if (Index < (int)Size)
		{
			Info.Index = Index;
			Info.EdgeIndex = 1;
			m_vecCell[i + 1]->vecAdj.push_back(Info);
		}
	}
}

void CNavigation3D::CreateAdj()
{
}

void CNavigation3D::CreateSection()
{
	m_SectionX = 5;
	m_SectionY = 5;

	Vector3	Length = m_Max - m_Min;

	m_SectionSize = Length / Vector3((float)m_SectionX, 1.f, (float)m_SectionY);

	SAFE_DELETE_ARRAY(m_SectionArray);

	m_SectionArray = new NavigationSection[m_SectionX * m_SectionY];

	for (int i = 0; i < m_SectionY; i++)
	{
		for (int j = 0; j < m_SectionX; j++)
		{
			int	Index = i * m_SectionX + j;

			m_SectionArray[Index].Size = m_SectionSize;
			m_SectionArray[Index].Min = m_Min + m_SectionSize * Vector3((float)j, 0.f, (float)i);
			m_SectionArray[Index].Max = m_Min + m_SectionSize * Vector3((float)(j + 1), 0.f, (float)(i + 1));
		}
	}

	size_t	Size = m_vecCell.size();

	// �ִ� 4���� ������ ���Ե� �� �ִ�.
	for (size_t i = 0; i < Size; i++)
	{
		int	MinX = 1000000, MinY = 1000000, MaxX = -1000000, MaxY = -1000000;

		for (int j = 0; j < 3; j++)
		{
			Vector3	Pos = m_vecCell[i]->Pos[j] - m_Min;

			Pos /= m_SectionSize;

			int	x = (int)Pos.x;
			int	y = (int)Pos.z;

			if (MinX > x)
			{
				MinX = x;
			}

			if (MaxX > x)
			{
				MaxX = x;
			}

			if (MinY < y)
			{
				MinY = y;
			}

			if (MaxY > y)
			{
				MaxY = y;
			}
		}

		MinX = MinX < 0 ? 0 : MinX;
		MinY = MinY < 0 ? 0 : MinY;
		MaxX = MaxX >= m_SectionX ? m_SectionX - 1 : MaxX;
		MaxY = MaxY >= m_SectionY ? m_SectionY - 1 : MaxY;

		for (int j = MinY; j <= MaxY; j++)
		{
			for (int k = MinX; k <= MaxX; k++)
			{
				int	Index = j * m_SectionX + k;

				m_SectionArray[Index].Add(m_vecCell[i]);
			}
		}
	}
}

float CNavigation3D::GetHeight(const Vector3& Pos)
{
	if (!m_Grid)
	{
		return 0.f;
	}

	Vector3	ConvertPos = Pos - m_Min;

	float	MaxHeight = m_Max.z - m_Min.z;

	ConvertPos.z = MaxHeight - ConvertPos.z;

	ConvertPos.x /= m_CellSize.x;
	ConvertPos.z /= m_CellSize.y;

	float	RatioX = ConvertPos.x - (int)ConvertPos.x;
	float	RatioY = ConvertPos.z - (int)ConvertPos.z;

	int	Index = -1;

	int	IndexX = (int)ConvertPos.x;
	int	IndexY = (int)ConvertPos.z;

	// ���� �ﰢ�� �ε����� ���Ѵ�.
	Index = IndexY * m_LineRectCountX * 2 + IndexX * 2;

	// ���ϴ� �ﰢ���� ������ ���
	if (RatioX < RatioY)
	{
		Index++;
	}

	NavigationCell* Cell = m_vecCell[Index];

	// ���ϴ�
	if (RatioX < RatioY)
	{
		return Cell->Pos[0].y + (Cell->Pos[1].y - Cell->Pos[2].y) * RatioX + (Cell->Pos[2].y - Cell->Pos[0].y) * RatioY;
	}

	// ����
	return Cell->Pos[0].y + (Cell->Pos[1].y - Cell->Pos[0].y) * RatioX + (Cell->Pos[2].y - Cell->Pos[1].y) * RatioY;
}

bool CNavigation3D::FindPath(const Vector3& Start, const Vector3& End,
	std::list<Vector3>& PathList)
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

	//for (size_t i = 0; i < Size; i++)
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

	//for (int i = 0; i < (int)ENodeDir1::End; i++)
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

CNavigation3D* CNavigation3D::Clone()
{
	return new CNavigation3D(*this);
}

//bool CNavigation3D::FindNode(NavNode1* Node, NavNode1* EndNode, const Vector2& End,
//	std::list<Vector2>& PathList)
//{
//	auto	iter = Node->SearchDirList.begin();
//	auto	iterEnd = Node->SearchDirList.end();
//
//	for (; iter != iterEnd; iter++)
//	{
//		NavNode1* Corner = GetCorner(*iter, Node, EndNode, End);
//
//		if (!Corner)
//			continue;
//
//		// ã���� ��尡 ���� ����� ��θ� ������ش�.
//		if (Corner == EndNode)
//		{
//			m_vecUseNode.push_back(Corner);
//			PathList.push_back(End);
//
//			NavNode1* PathNode = Node;
//
//			while (PathNode)
//			{
//				PathList.push_front(PathNode->Center);
//				PathNode = PathNode->Parent;
//			}
//
//			PathList.pop_front();
//
//			return true;
//		}
//
//		// �̵� ����� �����ش�.
//		float Cost = 0.f;
//
//		switch (*iter)
//		{
//		case ENodeDir1::T:
//		case ENodeDir1::B:
//			Cost = Node->Cost + Node->Size.y;
//			break;
//		case ENodeDir1::R:
//		case ENodeDir1::L:
//			Cost = Node->Cost + Node->Size.x;
//			break;
//		case ENodeDir1::RT:
//		case ENodeDir1::RB:
//		case ENodeDir1::LB:
//		case ENodeDir1::LT:
//			Cost = Node->Cost + Node->Center.Distance(Corner->Center);
//			break;
//		}
//
//		// ã���� ��尡 ������Ͽ� �� �ִ� ����� ����� ���Ͽ� ���� �������
//		// ��ü�Ѵ�.
//		if (Corner->NodeType == ENavNodeType1::Open)
//		{
//			if (Corner->Cost > Cost)
//			{
//				Corner->Cost = Cost;
//				Corner->Total = Corner->Cost + Corner->Dist;
//				Corner->Parent = Node;
//
//				// ������ ������ �־��ش�.
//				AddDir(*iter, Corner);
//			}
//		}
//
//		else
//		{
//			Corner->NodeType = ENavNodeType1::Open;
//			Corner->Parent = Node;
//			Corner->Cost = Cost;
//			Corner->Dist = Corner->Center.Distance(End);
//			Corner->Total = Corner->Cost + Corner->Dist;
//
//			m_vecOpen.push_back(Corner);
//
//			m_vecUseNode.push_back(Corner);
//
//			AddDir(*iter, Corner);
//		}
//	}
//
//	return false;
//}
//
//NavNode1* CNavigation3D::GetCorner(ENodeDir1 Dir, NavNode1* Node, NavNode1* EndNode,
//	const Vector2& End)
//{
//	switch (m_Shape)
//	{
//	case ETileShape::Rect:
//		switch (Dir)
//		{
//		case ENodeDir1::T:
//			return GetCornerRectTop(Node, EndNode);
//		case ENodeDir1::RT:
//			return GetCornerRectRT(Node, EndNode);
//		case ENodeDir1::R:
//			return GetCornerRectRight(Node, EndNode);
//		case ENodeDir1::RB:
//			return GetCornerRectRB(Node, EndNode);
//		case ENodeDir1::B:
//			return GetCornerRectBottom(Node, EndNode);
//		case ENodeDir1::LB:
//			return GetCornerRectLB(Node, EndNode);
//		case ENodeDir1::L:
//			return GetCornerRectLeft(Node, EndNode);
//		case ENodeDir1::LT:
//			return GetCornerRectLT(Node, EndNode);
//		}
//		break;
//	case ETileShape::Isometric:
//		switch (Dir)
//		{
//		case ENodeDir1::T:
//			return GetCornerIsometricTop(Node, EndNode);
//		case ENodeDir1::RT:
//			return GetCornerIsometricRT(Node, EndNode);
//		case ENodeDir1::R:
//			return GetCornerIsometricRight(Node, EndNode);
//		case ENodeDir1::RB:
//			return GetCornerIsometricRB(Node, EndNode);
//		case ENodeDir1::B:
//			return GetCornerIsometricBottom(Node, EndNode);
//		case ENodeDir1::LB:
//			return GetCornerIsometricLB(Node, EndNode);
//		case ENodeDir1::L:
//			return GetCornerIsometricLeft(Node, EndNode);
//		case ENodeDir1::LT:
//			return GetCornerIsometricLT(Node, EndNode);
//		}
//		break;
//	}
//
//	return nullptr;
//}
//
//NavNode1* CNavigation3D::GetCornerRectTop(NavNode1* Node, NavNode1* EndNode)
//{
//	// ��ĭ�� ���� �ö󰡸� �������� �����ְ� ������ ���� �շ����� ��쳪
//	// ������ �����ְ� ���� ���� �շ������� �ش� ���� �ڳʰ� �ȴ�.
//	int	IndexY = Node->IndexY;
//
//	while (true)
//	{
//		i++ndexY;
//
//		if (IndexY >= m_CountY)
//			return nullptr;
//
//		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + Node->IndexX];
//
//		if (CornerNode == EndNode)
//			return CornerNode;
//
//		else if (CornerNode->NodeType == ENavNodeType1::Close)
//			return nullptr;
//
//		else if (CornerNode->TileOption == ETileOption::Wall)
//			return nullptr;
//
//		int	CornerX = Node->IndexX + 1;
//		int	CornerY = IndexY;
//
//		if (CornerX < m_CountX && CornerY + 1 < m_CountY)
//		{
//			// �������� ���̰� ������ ���� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(CornerY + 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		CornerX = Node->IndexX - 1;
//		CornerY = IndexY;
//
//		if (CornerX >= 0 && CornerY + 1 < m_CountY)
//		{
//			// ������ ���̰� ���� ���� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(CornerY + 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//	}
//
//	return nullptr;
//}
//
//NavNode1* CNavigation3D::GetCornerRectBottom(NavNode1* Node, NavNode1* EndNode)
//{
//	// ��ĭ�� �Ʒ��� �������� �������� �����ְ� ������ �Ʒ��� �շ����� ��쳪
//	// ������ �����ְ� ���� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
//	int	IndexY = Node->IndexY;
//
//	while (true)
//	{
//		--IndexY;
//
//		if (IndexY < 0)
//			return nullptr;
//
//		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + Node->IndexX];
//
//		if (CornerNode == EndNode)
//			return CornerNode;
//
//		else if (CornerNode->NodeType == ENavNodeType1::Close)
//			return nullptr;
//
//		else if (CornerNode->TileOption == ETileOption::Wall)
//			return nullptr;
//
//		int	CornerX = Node->IndexX + 1;
//		int	CornerY = IndexY;
//
//		if (CornerX < m_CountX && CornerY - 1 >= 0)
//		{
//			// �������� ���̰� ������ �Ʒ��� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(CornerY - 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		CornerX = Node->IndexX - 1;
//		CornerY = IndexY;
//
//		if (CornerX >= 0 && CornerY - 1 >= 0)
//		{
//			// ������ ���̰� ���� �Ʒ��� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(CornerY - 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//	}
//
//	return nullptr;
//}
//
//NavNode1* CNavigation3D::GetCornerRectLeft(NavNode1* Node, NavNode1* EndNode)
//{
//	// ��ĭ�� �������� ���� ���� �����ְ� ���� ���� �շ����� ��쳪
//	// �Ʒ��� �����ְ� ���� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
//	int	IndexX = Node->IndexX;
//
//	while (true)
//	{
//		--IndexX;
//
//		if (IndexX < 0)
//			return nullptr;
//
//		NavNode1* CornerNode = m_vecNode[Node->IndexY * m_CountX + IndexX];
//
//		if (CornerNode == EndNode)
//			return CornerNode;
//
//		else if (CornerNode->NodeType == ENavNodeType1::Close)
//			return nullptr;
//
//		else if (CornerNode->TileOption == ETileOption::Wall)
//			return nullptr;
//
//		int	CornerX = IndexX;
//		int	CornerY = Node->IndexY + 1;
//
//		if (CornerX - 1 >= 0 && CornerY < m_CountY)
//		{
//			// ������ ���̰� ���� ���� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[CornerY * m_CountX + (CornerX - 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		CornerX = IndexX;
//		CornerY = Node->IndexY - 1;
//
//		if (CornerX - 1 >= 0 && CornerY >= 0)
//		{
//			// �Ʒ����� ���̰� ���� �Ʒ��� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[CornerY * m_CountX + (CornerX - 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//	}
//
//	return nullptr;
//}
//
//NavNode1* CNavigation3D::GetCornerRectRight(NavNode1* Node, NavNode1* EndNode)
//{
//	// ��ĭ�� ���������� ���� ���� �����ְ� ������ ���� �շ����� ��쳪
//	// �Ʒ��� �����ְ� ������ �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
//	int	IndexX = Node->IndexX;
//
//	while (true)
//	{
//		i++ndexX;
//
//		if (IndexX >= m_CountX)
//			return nullptr;
//
//		NavNode1* CornerNode = m_vecNode[Node->IndexY * m_CountX + IndexX];
//
//		if (CornerNode == EndNode)
//			return CornerNode;
//
//		else if (CornerNode->NodeType == ENavNodeType1::Close)
//			return nullptr;
//
//		else if (CornerNode->TileOption == ETileOption::Wall)
//			return nullptr;
//
//		int	CornerX = IndexX;
//		int	CornerY = Node->IndexY + 1;
//
//		if (CornerX + 1 < m_CountX && CornerY < m_CountY)
//		{
//			// ������ ���̰� ������ ���� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[CornerY * m_CountX + (CornerX + 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		CornerX = IndexX;
//		CornerY = Node->IndexY - 1;
//
//		if (CornerX + 1 < m_CountX && CornerY >= 0)
//		{
//			// �Ʒ����� ���̰� ������ �Ʒ��� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[CornerY * m_CountX + (CornerX + 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//	}
//
//	return nullptr;
//}
//
//NavNode1* CNavigation3D::GetCornerRectLT(NavNode1* Node, NavNode1* EndNode)
//{
//	// ��ĭ�� ���� ���� ���� �������� �����ְ� ������ ���� �շ����� ��쳪
//	// �Ʒ��� �����ְ� ���� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
//	int	IndexX = Node->IndexX;
//	int	IndexY = Node->IndexY;
//
//	while (true)
//	{
//		--IndexX;
//		i++ndexY;
//
//		if (IndexX < 0 || IndexY >= m_CountY)
//			return nullptr;
//
//		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];
//
//		if (CornerNode == EndNode)
//			return CornerNode;
//
//		else if (CornerNode->NodeType == ENavNodeType1::Close)
//			return nullptr;
//
//		else if (CornerNode->TileOption == ETileOption::Wall)
//			return nullptr;
//
//		int	CornerX = IndexX;
//		int	CornerY = IndexY - 1;
//
//		if (CornerX - 1 >= 0 && CornerY >= 0)
//		{
//			// �Ʒ����� ���̰� ���� �Ʒ��� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[CornerY * m_CountX + (CornerX - 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		CornerX = IndexX + 1;
//		CornerY = IndexY;
//
//		if (CornerX < m_CountX && CornerY + 1 < m_CountY)
//		{
//			// �������� ���̰� ������ ���� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(CornerY + 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		// �밢�� �ڳʰ� �ƴ϶�� ���� ���� ������ �ǹǷ� ����, �׸��� ��������
//		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
//		// �Ѵ�.
//		NavNode1* Find = GetCornerRectTop(CornerNode, EndNode);
//
//		// �������� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
//		if (Find)
//			return CornerNode;
//
//		Find = GetCornerRectLeft(CornerNode, EndNode);
//
//		if (Find)
//			return CornerNode;
//	}
//
//	return nullptr;
//}
//
//NavNode1* CNavigation3D::GetCornerRectRT(NavNode1* Node, NavNode1* EndNode)
//{
//	// ��ĭ�� ������ ���� ���� ������ �����ְ� ���� ���� �շ����� ��쳪
//	// �Ʒ��� �����ְ� ������ �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
//	int	IndexX = Node->IndexX;
//	int	IndexY = Node->IndexY;
//
//	while (true)
//	{
//		i++ndexX;
//		i++ndexY;
//
//		if (IndexX >= m_CountX || IndexY >= m_CountY)
//			return nullptr;
//
//		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];
//
//		if (CornerNode == EndNode)
//			return CornerNode;
//
//		else if (CornerNode->NodeType == ENavNodeType1::Close)
//			return nullptr;
//
//		else if (CornerNode->TileOption == ETileOption::Wall)
//			return nullptr;
//
//		int	CornerX = IndexX;
//		int	CornerY = IndexY - 1;
//
//		if (CornerX + 1 < m_CountX && CornerY >= 0)
//		{
//			// �Ʒ����� ���̰� ������ �Ʒ��� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[CornerY * m_CountX + (CornerX + 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		CornerX = IndexX - 1;
//		CornerY = IndexY;
//
//		if (CornerX >= 0 && CornerY + 1 < m_CountY)
//		{
//			// ������ ���̰� ���� ���� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(CornerY + 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		// �밢�� �ڳʰ� �ƴ϶�� ������ ���� ������ �ǹǷ� ������, �׸��� ��������
//		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
//		// �Ѵ�.
//		NavNode1* Find = GetCornerRectTop(CornerNode, EndNode);
//
//		// �������� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
//		if (Find)
//			return CornerNode;
//
//		Find = GetCornerRectRight(CornerNode, EndNode);
//
//		if (Find)
//			return CornerNode;
//	}
//
//	return nullptr;
//}
//
//NavNode1* CNavigation3D::GetCornerRectLB(NavNode1* Node, NavNode1* EndNode)
//{
//	// ��ĭ�� ���� �Ʒ��� ���� �������� �����ְ� ������ �Ʒ��� �շ����� ��쳪
//	// ���� �����ְ� ���� ���� �շ������� �ش� ���� �ڳʰ� �ȴ�.
//	int	IndexX = Node->IndexX;
//	int	IndexY = Node->IndexY;
//
//	while (true)
//	{
//		--IndexX;
//		--IndexY;
//
//		if (IndexX < 0 || IndexY < 0)
//			return nullptr;
//
//		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];
//
//		if (CornerNode == EndNode)
//			return CornerNode;
//
//		else if (CornerNode->NodeType == ENavNodeType1::Close)
//			return nullptr;
//
//		else if (CornerNode->TileOption == ETileOption::Wall)
//			return nullptr;
//
//		int	CornerX = IndexX;
//		int	CornerY = IndexY + 1;
//
//		if (CornerX - 1 >= 0 && CornerY < m_CountY)
//		{
//			// ������ ���̰� ���� ���� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[CornerY * m_CountX + (CornerX - 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		CornerX = IndexX + 1;
//		CornerY = IndexY;
//
//		if (CornerX < m_CountX && CornerY - 1 >= 0)
//		{
//			// �������� ���̰� ������ �Ʒ��� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(CornerY - 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		// �밢�� �ڳʰ� �ƴ϶�� ���� �Ʒ��� ������ �ǹǷ� ����, �׸��� �Ʒ�������
//		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
//		// �Ѵ�.
//		NavNode1* Find = GetCornerRectBottom(CornerNode, EndNode);
//
//		// �Ʒ������� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
//		if (Find)
//			return CornerNode;
//
//		Find = GetCornerRectLeft(CornerNode, EndNode);
//
//		if (Find)
//			return CornerNode;
//	}
//
//	return nullptr;
//}
//
//NavNode1* CNavigation3D::GetCornerRectRB(NavNode1* Node, NavNode1* EndNode)
//{
//	// ��ĭ�� ������ �Ʒ��� ���� ������ �����ְ� ���� �Ʒ��� �շ����� ��쳪
//	// ���� �����ְ� ������ ���� �շ������� �ش� ���� �ڳʰ� �ȴ�.
//	int	IndexX = Node->IndexX;
//	int	IndexY = Node->IndexY;
//
//	while (true)
//	{
//		i++ndexX;
//		--IndexY;
//
//		if (IndexX >= m_CountX || IndexY < 0)
//			return nullptr;
//
//		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];
//
//		if (CornerNode == EndNode)
//			return CornerNode;
//
//		else if (CornerNode->NodeType == ENavNodeType1::Close)
//			return nullptr;
//
//		else if (CornerNode->TileOption == ETileOption::Wall)
//			return nullptr;
//
//		int	CornerX = IndexX;
//		int	CornerY = IndexY + 1;
//
//		if (CornerX + 1 < m_CountX && CornerY < m_CountY)
//		{
//			// ������ ���̰� ������ ���� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[CornerY * m_CountX + (CornerX + 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		CornerX = IndexX - 1;
//		CornerY = IndexY;
//
//		if (CornerX >= 0 && CornerY - 1 >= 0)
//		{
//			// ������ ���̰� ���� �Ʒ��� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(CornerY - 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		// �밢�� �ڳʰ� �ƴ϶�� ������ �Ʒ��� ������ �ǹǷ� ������, �׸��� �Ʒ�������
//		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
//		// �Ѵ�.
//		NavNode1* Find = GetCornerRectBottom(CornerNode, EndNode);
//
//		// �Ʒ������� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
//		if (Find)
//			return CornerNode;
//
//		Find = GetCornerRectRight(CornerNode, EndNode);
//
//		if (Find)
//			return CornerNode;
//	}
//
//	return nullptr;
//}
//
//NavNode1* CNavigation3D::GetCornerIsometricTop(NavNode1* Node, NavNode1* EndNode)
//{
//	// ��ĭ�� ���� ���� ���� �Ʒ��� �����ְ� ������ �շ����� ��쳪
//	// ������ �Ʒ��� �����ְ� �������� �շ������� �ش� ���� �ڳʰ� �ȴ�.
//	int	IndexX = Node->IndexX;
//	int	IndexY = Node->IndexY;
//
//	while (true)
//	{
//		IndexY += 2;
//
//		if (IndexY >= m_CountY)
//			return nullptr;
//
//		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];
//
//		if (CornerNode == EndNode)
//			return CornerNode;
//
//		else if (CornerNode->NodeType == ENavNodeType1::Close)
//			return nullptr;
//
//		else if (CornerNode->TileOption == ETileOption::Wall)
//			return nullptr;
//
//		int	CornerX = IndexX;
//
//		if (IndexY % 2 == 0)
//			CornerX = IndexX - 1;
//
//		int	CornerY = IndexY - 1;
//
//		if (IndexX - 1 >= 0)
//		{
//			// ���� �Ʒ��� ���̰� ���� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[IndexY * m_CountX + (IndexX - 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		CornerX = IndexX;
//
//		if (IndexY % 2 == 1)
//			CornerX = IndexX + 1;
//
//		if (IndexX + 1 < m_CountX)
//		{
//			// ������ �Ʒ��� ���̰� �������� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[IndexY * m_CountX + (IndexX + 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		// �밢�� �ڳʰ� �ƴ϶�� ���� ������ �ǹǷ� ������ ��, �׸��� ���� ����
//		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
//		// �Ѵ�.
//		NavNode1* Find = GetCornerIsometricRT(CornerNode, EndNode);
//
//		// ������ ���� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
//		if (Find)
//			return CornerNode;
//
//		Find = GetCornerIsometricLT(CornerNode, EndNode);
//
//		if (Find)
//			return CornerNode;
//	}
//
//	return nullptr;
//}
//
//NavNode1* CNavigation3D::GetCornerIsometricBottom(NavNode1* Node, NavNode1* EndNode)
//{
//	// ��ĭ�� �Ʒ��� ���� ���� ���� �����ְ� ������ �շ����� ��쳪
//	// ������ ���� �����ְ� �������� �շ������� �ش� ���� �ڳʰ� �ȴ�.
//	int	IndexX = Node->IndexX;
//	int	IndexY = Node->IndexY;
//
//	while (true)
//	{
//		IndexY -= 2;
//
//		if (IndexY < 0)
//			return nullptr;
//
//		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];
//
//		if (CornerNode == EndNode)
//			return CornerNode;
//
//		else if (CornerNode->NodeType == ENavNodeType1::Close)
//			return nullptr;
//
//		else if (CornerNode->TileOption == ETileOption::Wall)
//			return nullptr;
//
//		int	CornerX = IndexX;
//
//		if (IndexY % 2 == 0)
//			CornerX = IndexX - 1;
//
//		int	CornerY = IndexY + 1;
//
//		if (IndexX - 1 >= 0)
//		{
//			// ���� ���� ���̰� ���� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[IndexY * m_CountX + (IndexX - 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		CornerX = IndexX;
//
//		if (IndexY % 2 == 1)
//			CornerX = IndexX + 1;
//
//		if (IndexX + 1 < m_CountX)
//		{
//			// ������ ���� ���̰� �������� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[IndexY * m_CountX + (IndexX + 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		// �밢�� �ڳʰ� �ƴ϶�� �Ʒ��� ������ �ǹǷ� ������ �Ʒ�, �׸��� ���� �Ʒ���
//		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
//		// �Ѵ�.
//		NavNode1* Find = GetCornerIsometricRB(CornerNode, EndNode);
//
//		// ������ �Ʒ��� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
//		if (Find)
//			return CornerNode;
//
//		Find = GetCornerIsometricLB(CornerNode, EndNode);
//
//		if (Find)
//			return CornerNode;
//	}
//
//	return nullptr;
//}
//
//NavNode1* CNavigation3D::GetCornerIsometricLeft(NavNode1* Node, NavNode1* EndNode)
//{
//	// ��ĭ�� �������� ���� ������ ���� �����ְ� ���� �շ����� ��쳪
//	// ������ �Ʒ��� �����ְ� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
//	int	IndexX = Node->IndexX;
//	int	IndexY = Node->IndexY;
//
//	while (true)
//	{
//		--IndexX;
//
//		if (IndexX < 0)
//			return nullptr;
//
//		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];
//
//		if (CornerNode == EndNode)
//			return CornerNode;
//
//		else if (CornerNode->NodeType == ENavNodeType1::Close)
//			return nullptr;
//
//		else if (CornerNode->TileOption == ETileOption::Wall)
//			return nullptr;
//
//		int	CornerX = IndexX;
//
//		if (IndexY % 2 == 1)
//			CornerX = IndexX + 1;
//
//		int	CornerY = IndexY + 1;
//
//		if (CornerX < m_CountX && IndexY + 2 < m_CountY)
//		{
//			// ������ ���� ���̰� �� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(IndexY + 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		CornerX = IndexX;
//
//		if (IndexY % 2 == 1)
//			CornerX = IndexX + 1;
//
//		CornerY = IndexY - 1;
//
//		if (CornerX < m_CountX && IndexY - 2 >= 0)
//		{
//			// ������ �Ʒ��� ���̰� �Ʒ��� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(IndexY - 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		// �밢�� �ڳʰ� �ƴ϶�� �������� ������ �ǹǷ� ���� ��, �׸��� ���� �Ʒ���
//		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
//		// �Ѵ�.
//		NavNode1* Find = GetCornerIsometricLT(CornerNode, EndNode);
//
//		// ���� ���� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
//		if (Find)
//			return CornerNode;
//
//		Find = GetCornerIsometricLB(CornerNode, EndNode);
//
//		if (Find)
//			return CornerNode;
//	}
//
//	return nullptr;
//}
//
//NavNode1* CNavigation3D::GetCornerIsometricRight(NavNode1* Node, NavNode1* EndNode)
//{
//	// ��ĭ�� ���������� ���� ���� ���� �����ְ� ���� �շ����� ��쳪
//	// ���� �Ʒ��� �����ְ� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
//	int	IndexX = Node->IndexX;
//	int	IndexY = Node->IndexY;
//
//	while (true)
//	{
//		i++ndexX;
//
//		if (IndexX >= m_CountX)
//			return nullptr;
//
//		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];
//
//		if (CornerNode == EndNode)
//			return CornerNode;
//
//		else if (CornerNode->NodeType == ENavNodeType1::Close)
//			return nullptr;
//
//		else if (CornerNode->TileOption == ETileOption::Wall)
//			return nullptr;
//
//		int	CornerX = IndexX;
//
//		if (IndexY % 2 == 0)
//			CornerX = IndexX - 1;
//
//		int	CornerY = IndexY + 1;
//
//		if (CornerX >= 0 && IndexY + 2 < m_CountY)
//		{
//			// ���� ���� ���̰� �� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(IndexY + 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		CornerX = IndexX;
//
//		if (IndexY % 2 == 0)
//			CornerX = IndexX - 1;
//
//		CornerY = IndexY - 1;
//
//		if (CornerX >= 0 && IndexY - 2 >= 0)
//		{
//			// ���� �Ʒ��� ���̰� �Ʒ��� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(IndexY - 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		// �밢�� �ڳʰ� �ƴ϶�� ���������� ������ �ǹǷ� ������ ��, �׸��� ������ �Ʒ���
//		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
//		// �Ѵ�.
//		NavNode1* Find = GetCornerIsometricRT(CornerNode, EndNode);
//
//		// ������ ���� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
//		if (Find)
//			return CornerNode;
//
//		Find = GetCornerIsometricRB(CornerNode, EndNode);
//
//		if (Find)
//			return CornerNode;
//	}
//
//	return nullptr;
//}
//
//NavNode1* CNavigation3D::GetCornerIsometricLT(NavNode1* Node, NavNode1* EndNode)
//{
//	// ��ĭ�� ���� ���� �ö󰡸� ������ ���� �����ְ� ���� �շ����� ��쳪
//	// ���� �Ʒ��� �����ְ� ������ �շ������� �ش� ���� �ڳʰ� �ȴ�.
//	int	IndexX = Node->IndexX;
//	int	IndexY = Node->IndexY;
//
//	while (true)
//	{
//		if (IndexY % 2 == 0)
//			--IndexX;
//
//		i++ndexY;
//
//		if (IndexY >= m_CountY || IndexX < 0)
//			return nullptr;
//
//		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];
//
//		if (CornerNode == EndNode)
//			return CornerNode;
//
//		else if (CornerNode->NodeType == ENavNodeType1::Close)
//			return nullptr;
//
//		else if (CornerNode->TileOption == ETileOption::Wall)
//			return nullptr;
//
//		int	CornerX = IndexX;
//		int	CornerY = IndexY + 1;
//
//		if (IndexY % 2 == 1)
//			++CornerX;
//
//		if (CornerX < m_CountX && IndexY + 2 < m_CountY)
//		{
//			// ������ ���� ���̰� ���� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(IndexY + 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		CornerX = IndexX;
//
//		if (IndexY % 2 == 0)
//			--CornerX;
//
//		CornerY = IndexY - 1;
//
//		if (IndexX - 1 >= 0 && CornerY >= 0)
//		{
//			// ���� �Ʒ��� ���̰� ������ �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[IndexY * m_CountX + (IndexX - 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//	}
//
//	return nullptr;
//}
//
//NavNode1* CNavigation3D::GetCornerIsometricRT(NavNode1* Node, NavNode1* EndNode)
//{
//	// ��ĭ�� ������ ���� �ö󰡸� ���� ���� �����ְ� ���� �շ����� ��쳪
//	// ������ �Ʒ��� �����ְ� �������� �շ������� �ش� ���� �ڳʰ� �ȴ�.
//	int	IndexX = Node->IndexX;
//	int	IndexY = Node->IndexY;
//
//	while (true)
//	{
//		if (IndexY % 2 == 1)
//			i++ndexX;
//
//		i++ndexY;
//
//		if (IndexY >= m_CountY || IndexX >= m_CountX)
//			return nullptr;
//
//		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];
//
//		if (CornerNode == EndNode)
//			return CornerNode;
//
//		else if (CornerNode->NodeType == ENavNodeType1::Close)
//			return nullptr;
//
//		else if (CornerNode->TileOption == ETileOption::Wall)
//			return nullptr;
//
//		int	CornerX = IndexX;
//		int	CornerY = IndexY + 1;
//
//		if (IndexY % 2 == 0)
//			--CornerX;
//
//		if (CornerX >= 0 && IndexY + 2 < m_CountY)
//		{
//			// ���� ���� ���̰� ���� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(IndexY + 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		CornerX = IndexX;
//
//		if (IndexY % 2 == 1)
//			++CornerX;
//
//		CornerY = IndexY - 1;
//
//		if (CornerX < m_CountX && CornerY >= 0)
//		{
//			// ������ �Ʒ��� ���̰� �������� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[IndexY * m_CountX + (IndexX + 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//	}
//
//	return nullptr;
//}
//
//NavNode1* CNavigation3D::GetCornerIsometricLB(NavNode1* Node, NavNode1* EndNode)
//{
//	// ��ĭ�� ���� �Ʒ��� �������� ���� ���� �����ְ� ������ �շ����� ��쳪
//	// ������ �Ʒ��� �����ְ� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
//	int	IndexX = Node->IndexX;
//	int	IndexY = Node->IndexY;
//
//	while (true)
//	{
//		if (IndexY % 2 == 0)
//			--IndexX;
//
//		--IndexY;
//
//		if (IndexY < 0 || IndexX < 0)
//			return nullptr;
//
//		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];
//
//		if (CornerNode == EndNode)
//			return CornerNode;
//
//		else if (CornerNode->NodeType == ENavNodeType1::Close)
//			return nullptr;
//
//		else if (CornerNode->TileOption == ETileOption::Wall)
//			return nullptr;
//
//		int	CornerX = IndexX;
//		int	CornerY = IndexY + 1;
//
//		if (IndexY % 2 == 0)
//			--CornerX;
//
//		if (IndexX - 1 >= 0 && CornerY < m_CountY)
//		{
//			// ���� ���� ���̰� ������ �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[IndexY * m_CountX + (IndexX - 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		CornerX = IndexX;
//
//		if (IndexY % 2 == 1)
//			++CornerX;
//
//		CornerY = IndexY - 1;
//
//		if (CornerX < m_CountX && IndexY - 2 >= 0)
//		{
//			// ������ �Ʒ��� ���̰� �Ʒ��� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(IndexY - 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//	}
//
//	return nullptr;
//}
//
//NavNode1* CNavigation3D::GetCornerIsometricRB(NavNode1* Node, NavNode1* EndNode)
//{
//	// ��ĭ�� ������ �Ʒ��� �������� ������ ���� �����ְ� �������� �շ����� ��쳪
//	// ���� �Ʒ��� �����ְ� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
//	int	IndexX = Node->IndexX;
//	int	IndexY = Node->IndexY;
//
//	while (true)
//	{
//		if (IndexY % 2 == 1)
//			i++ndexX;
//
//		--IndexY;
//
//		if (IndexY < 0 || IndexX >= m_CountX)
//			return nullptr;
//
//		NavNode1* CornerNode = m_vecNode[IndexY * m_CountX + IndexX];
//
//		if (CornerNode == EndNode)
//			return CornerNode;
//
//		else if (CornerNode->NodeType == ENavNodeType1::Close)
//			return nullptr;
//
//		else if (CornerNode->TileOption == ETileOption::Wall)
//			return nullptr;
//
//		int	CornerX = IndexX;
//		int	CornerY = IndexY + 1;
//
//		if (IndexY % 2 == 1)
//			++CornerX;
//
//		if (IndexX + 1 < m_CountX && CornerY < m_CountY)
//		{
//			// ������ ���� ���̰� �������� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[IndexY * m_CountX + (IndexX + 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		CornerX = IndexX;
//
//		if (IndexY % 2 == 0)
//			--CornerX;
//
//		CornerY = IndexY - 1;
//
//		if (CornerX >= 0 && IndexY - 2 >= 0)
//		{
//			// ���� �Ʒ��� ���̰� �Ʒ��� �Ϲ��� ���
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(IndexY - 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//	}
//
//	return nullptr;
//}
//
//void CNavigation3D::AddDir(ENodeDir1 Dir, NavNode1* Node)
//{
//	Node->SearchDirList.clear();
//
//	switch (m_Shape)
//	{
//	case ETileShape::Rect:
//		switch (Dir)
//		{
//		case ENodeDir1::T:
//			Node->SearchDirList.push_back(ENodeDir1::T);
//			Node->SearchDirList.push_back(ENodeDir1::LT);
//			Node->SearchDirList.push_back(ENodeDir1::RT);
//			break;
//		case ENodeDir1::RT:
//			Node->SearchDirList.push_back(ENodeDir1::RT);
//			Node->SearchDirList.push_back(ENodeDir1::T);
//			Node->SearchDirList.push_back(ENodeDir1::R);
//			Node->SearchDirList.push_back(ENodeDir1::LT);
//			Node->SearchDirList.push_back(ENodeDir1::RB);
//			break;
//		case ENodeDir1::R:
//			Node->SearchDirList.push_back(ENodeDir1::R);
//			Node->SearchDirList.push_back(ENodeDir1::RT);
//			Node->SearchDirList.push_back(ENodeDir1::RB);
//			break;
//		case ENodeDir1::RB:
//			Node->SearchDirList.push_back(ENodeDir1::RB);
//			Node->SearchDirList.push_back(ENodeDir1::R);
//			Node->SearchDirList.push_back(ENodeDir1::B);
//			Node->SearchDirList.push_back(ENodeDir1::RT);
//			Node->SearchDirList.push_back(ENodeDir1::LB);
//			break;
//		case ENodeDir1::B:
//			Node->SearchDirList.push_back(ENodeDir1::B);
//			Node->SearchDirList.push_back(ENodeDir1::LB);
//			Node->SearchDirList.push_back(ENodeDir1::RB);
//			break;
//		case ENodeDir1::LB:
//			Node->SearchDirList.push_back(ENodeDir1::LB);
//			Node->SearchDirList.push_back(ENodeDir1::L);
//			Node->SearchDirList.push_back(ENodeDir1::B);
//			Node->SearchDirList.push_back(ENodeDir1::LT);
//			Node->SearchDirList.push_back(ENodeDir1::RB);
//			break;
//		case ENodeDir1::L:
//			Node->SearchDirList.push_back(ENodeDir1::L);
//			Node->SearchDirList.push_back(ENodeDir1::LT);
//			Node->SearchDirList.push_back(ENodeDir1::LB);
//			break;
//		case ENodeDir1::LT:
//			Node->SearchDirList.push_back(ENodeDir1::LT);
//			Node->SearchDirList.push_back(ENodeDir1::T);
//			Node->SearchDirList.push_back(ENodeDir1::L);
//			Node->SearchDirList.push_back(ENodeDir1::RT);
//			Node->SearchDirList.push_back(ENodeDir1::LB);
//			break;
//		}
//		break;
//	case ETileShape::Isometric:
//		switch (Dir)
//		{
//		case ENodeDir1::T:
//			Node->SearchDirList.push_back(ENodeDir1::T);
//			Node->SearchDirList.push_back(ENodeDir1::RT);
//			Node->SearchDirList.push_back(ENodeDir1::LT);
//			Node->SearchDirList.push_back(ENodeDir1::R);
//			Node->SearchDirList.push_back(ENodeDir1::L);
//			break;
//		case ENodeDir1::RT:
//			Node->SearchDirList.push_back(ENodeDir1::RT);
//			Node->SearchDirList.push_back(ENodeDir1::T);
//			Node->SearchDirList.push_back(ENodeDir1::R);
//			break;
//		case ENodeDir1::R:
//			Node->SearchDirList.push_back(ENodeDir1::R);
//			Node->SearchDirList.push_back(ENodeDir1::RT);
//			Node->SearchDirList.push_back(ENodeDir1::RB);
//			Node->SearchDirList.push_back(ENodeDir1::T);
//			Node->SearchDirList.push_back(ENodeDir1::B);
//			break;
//		case ENodeDir1::RB:
//			Node->SearchDirList.push_back(ENodeDir1::RB);
//			Node->SearchDirList.push_back(ENodeDir1::R);
//			Node->SearchDirList.push_back(ENodeDir1::B);
//			break;
//		case ENodeDir1::B:
//			Node->SearchDirList.push_back(ENodeDir1::B);
//			Node->SearchDirList.push_back(ENodeDir1::RB);
//			Node->SearchDirList.push_back(ENodeDir1::LB);
//			Node->SearchDirList.push_back(ENodeDir1::R);
//			Node->SearchDirList.push_back(ENodeDir1::L);
//			break;
//		case ENodeDir1::LB:
//			Node->SearchDirList.push_back(ENodeDir1::LB);
//			Node->SearchDirList.push_back(ENodeDir1::L);
//			Node->SearchDirList.push_back(ENodeDir1::B);
//			break;
//		case ENodeDir1::L:
//			Node->SearchDirList.push_back(ENodeDir1::L);
//			Node->SearchDirList.push_back(ENodeDir1::LT);
//			Node->SearchDirList.push_back(ENodeDir1::LB);
//			Node->SearchDirList.push_back(ENodeDir1::T);
//			Node->SearchDirList.push_back(ENodeDir1::B);
//			break;
//		case ENodeDir1::LT:
//			Node->SearchDirList.push_back(ENodeDir1::LT);
//			Node->SearchDirList.push_back(ENodeDir1::L);
//			Node->SearchDirList.push_back(ENodeDir1::T);
//			break;
//		}
//		break;
//	}
//}
//
//bool CNavigation3D::SortNode(NavNode1* Src, NavNode1* Dest)
//{
//	return Src->Total > Dest->Total;
//}
