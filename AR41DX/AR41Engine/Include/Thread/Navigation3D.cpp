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

		// 우상단 삼각형 인접정보 구성.
		// 위쪽 사각형의 좌 하단 삼각형, 오른쪽 사각형의 좌 하단 삼각형,
		// 자기 자신 사각형의 좌 하단 삼각형이다.
		// 
		// 위쪽 좌 하단 삼각형 인덱스
		int	Index = (int)i - m_LineRectCountX * 2 + 1;
		if (Index >= 0 && Index < (int)Size)
		{
			Info.Index = Index;
			Info.EdgeIndex = 0;
			m_vecCell[i]->vecAdj.push_back(Info);
		}

		// 자기자신 사각형의 왼쪽 하단 삼각형이다.
		Index = (int)i + 1;

		Info.Index = Index;
		Info.EdgeIndex = 2;
		m_vecCell[i]->vecAdj.push_back(Info);

		// 오른쪽 사각형의 왼쪽 하단 삼각형이다.
		Index = (int)i + 3;

		// 오른쪽 끝 사각형인지 판단한다.
		if (i / 2 % m_LineRectCountX != m_LineRectCountX - 1)
		{
			Info.Index = Index;
			Info.EdgeIndex = 1;
			m_vecCell[i]->vecAdj.push_back(Info);
		}

		// 좌하단 삼각형 인접정보 구성
		// 왼쪽 사각형의 우상단, 자기자신 사각형의 우상단,
		// 아래쪽 사각형의 우상단 삼각형이다.

		// 자기자신 사각형의 우상단
		Index = (int)i;

		Info.Index = Index;
		Info.EdgeIndex = 0;
		m_vecCell[i + 1]->vecAdj.push_back(Info);

		// 왼쪽 사각형의 우상단
		if ((int)i / 2 % m_LineRectCountX != 0)
		{
			// i는 우상단 삼각형의 인덱스이므로 2를 빼준다.
			Index = (int)i - 2;
			Info.Index = Index;
			Info.EdgeIndex = 2;
			m_vecCell[i + 1]->vecAdj.push_back(Info);
		}

		// 아래쪽 사각형의 우상단.
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

	// 최대 4개의 영역에 포함될 수 있다.
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

	// 우상단 삼각형 인덱스를 구한다.
	Index = IndexY * m_LineRectCountX * 2 + IndexX * 2;

	// 좌하단 삼각형에 존재할 경우
	if (RatioX < RatioY)
	{
		Index++;
	}

	NavigationCell* Cell = m_vecCell[Index];

	// 좌하단
	if (RatioX < RatioY)
	{
		return Cell->Pos[0].y + (Cell->Pos[1].y - Cell->Pos[2].y) * RatioX + (Cell->Pos[2].y - Cell->Pos[0].y) * RatioY;
	}

	// 우상단
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
	//	// 열린 목록에서 노드를 가져온다.
	//	NavNode1* Node = m_vecOpen.back();
	//	m_vecOpen.pop_back();

	//	Node->NodeType = ENavNodeType1::Close;

	//	// 현재 노드가 검사해야할 방향의 타일을 검사하여 코너를 열린목록에 넣어준다.
	//	if (FindNode(Node, EndNode, End, PathList))
	//		break;

	//	// 열린 목록에 있는 노드를 비용이 큰 노드에서 작은 노드 순서로 정렬해준다.
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
//		// 찾아준 노드가 도착 노드라면 경로를 만들어준다.
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
//		// 이동 비용을 구해준다.
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
//		// 찾아준 노드가 열린목록에 들어가 있는 노드라면 비용을 비교하여 작은 비용으로
//		// 교체한다.
//		if (Corner->NodeType == ENavNodeType1::Open)
//		{
//			if (Corner->Cost > Cost)
//			{
//				Corner->Cost = Cost;
//				Corner->Total = Corner->Cost + Corner->Dist;
//				Corner->Parent = Node;
//
//				// 조사할 방향을 넣어준다.
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
//	// 한칸씩 위로 올라가며 오른쪽이 막혀있고 오른쪽 위가 뚫려있을 경우나
//	// 왼쪽이 막혀있고 왼쪽 위가 뚫려있으면 해당 노드는 코너가 된다.
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
//			// 오른쪽이 벽이고 오른쪽 위가 일반일 경우
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
//			// 왼쪽이 벽이고 왼쪽 위가 일반일 경우
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
//	// 한칸씩 아래로 내려가며 오른쪽이 막혀있고 오른쪽 아래가 뚫려있을 경우나
//	// 왼쪽이 막혀있고 왼쪽 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
//			// 오른쪽이 벽이고 오른쪽 아래가 일반일 경우
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
//			// 왼쪽이 벽이고 왼쪽 아래가 일반일 경우
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
//	// 한칸씩 왼쪽으로 가며 위가 막혀있고 왼쪽 위가 뚫려있을 경우나
//	// 아래가 막혀있고 왼쪽 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
//			// 위쪽이 벽이고 왼쪽 위가 일반일 경우
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
//			// 아래쪽이 벽이고 왼쪽 아래가 일반일 경우
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
//	// 한칸씩 오른쪽으로 가며 위가 막혀있고 오른쪽 위가 뚫려있을 경우나
//	// 아래가 막혀있고 오른쪽 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
//			// 위쪽이 벽이고 오른쪽 위가 일반일 경우
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
//			// 아래쪽이 벽이고 오른쪽 아래가 일반일 경우
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
//	// 한칸씩 왼쪽 위로 가며 오른쪽이 막혀있고 오른쪽 위가 뚫려있을 경우나
//	// 아래가 막혀있고 왼쪽 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
//			// 아래쪽이 벽이고 왼쪽 아래가 일반일 경우
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
//			// 오른쪽이 벽이고 오른쪽 위가 일반일 경우
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(CornerY + 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		// 대각선 코너가 아니라면 왼쪽 위로 진행이 되므로 왼쪽, 그리고 위쪽으로
//		// 코너가 있는지를 판단하여 코너가 있다면 현재 노드를 코너드로 판단하도록
//		// 한다.
//		NavNode1* Find = GetCornerRectTop(CornerNode, EndNode);
//
//		// 위쪽으로 검사하던 중 코너가 있다면 이 노드를 코너로 인식한다.
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
//	// 한칸씩 오른쪽 위로 가며 왼쪽이 막혀있고 왼쪽 위가 뚫려있을 경우나
//	// 아래가 막혀있고 오른쪽 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
//			// 아래쪽이 벽이고 오른쪽 아래가 일반일 경우
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
//			// 왼쪽이 벽이고 왼쪽 위가 일반일 경우
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(CornerY + 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		// 대각선 코너가 아니라면 오른쪽 위로 진행이 되므로 오른쪽, 그리고 위쪽으로
//		// 코너가 있는지를 판단하여 코너가 있다면 현재 노드를 코너드로 판단하도록
//		// 한다.
//		NavNode1* Find = GetCornerRectTop(CornerNode, EndNode);
//
//		// 위쪽으로 검사하던 중 코너가 있다면 이 노드를 코너로 인식한다.
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
//	// 한칸씩 왼쪽 아래로 가며 오른쪽이 막혀있고 오른쪽 아래가 뚫려있을 경우나
//	// 위가 막혀있고 왼쪽 위가 뚫려있으면 해당 노드는 코너가 된다.
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
//			// 위쪽이 벽이고 왼쪽 위가 일반일 경우
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
//			// 오른쪽이 벽이고 오른쪽 아래가 일반일 경우
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(CornerY - 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		// 대각선 코너가 아니라면 왼쪽 아래로 진행이 되므로 왼쪽, 그리고 아래쪽으로
//		// 코너가 있는지를 판단하여 코너가 있다면 현재 노드를 코너드로 판단하도록
//		// 한다.
//		NavNode1* Find = GetCornerRectBottom(CornerNode, EndNode);
//
//		// 아래쪽으로 검사하던 중 코너가 있다면 이 노드를 코너로 인식한다.
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
//	// 한칸씩 오른쪽 아래로 가며 왼쪽이 막혀있고 왼쪽 아래가 뚫려있을 경우나
//	// 위가 막혀있고 오른쪽 위가 뚫려있으면 해당 노드는 코너가 된다.
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
//			// 위쪽이 벽이고 오른쪽 위가 일반일 경우
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
//			// 왼쪽이 벽이고 왼쪽 아래가 일반일 경우
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(CornerY - 1) * m_CountX + CornerX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		// 대각선 코너가 아니라면 오른쪽 아래로 진행이 되므로 오른쪽, 그리고 아래쪽으로
//		// 코너가 있는지를 판단하여 코너가 있다면 현재 노드를 코너드로 판단하도록
//		// 한다.
//		NavNode1* Find = GetCornerRectBottom(CornerNode, EndNode);
//
//		// 아래쪽으로 검사하던 중 코너가 있다면 이 노드를 코너로 인식한다.
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
//	// 한칸씩 위로 가며 왼쪽 아래가 막혀있고 왼쪽이 뚫려있을 경우나
//	// 오른쪽 아래가 막혀있고 오른쪽이 뚫려있으면 해당 노드는 코너가 된다.
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
//			// 왼쪽 아래가 벽이고 왼쪽 일반일 경우
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
//			// 오른쪽 아래가 벽이고 오른쪽이 일반일 경우
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[IndexY * m_CountX + (IndexX + 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		// 대각선 코너가 아니라면 위로 진행이 되므로 오른쪽 위, 그리고 왼쪽 위로
//		// 코너가 있는지를 판단하여 코너가 있다면 현재 노드를 코너드로 판단하도록
//		// 한다.
//		NavNode1* Find = GetCornerIsometricRT(CornerNode, EndNode);
//
//		// 오른쪽 위로 검사하던 중 코너가 있다면 이 노드를 코너로 인식한다.
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
//	// 한칸씩 아래로 가며 왼쪽 위가 막혀있고 왼쪽이 뚫려있을 경우나
//	// 오른쪽 위가 막혀있고 오른쪽이 뚫려있으면 해당 노드는 코너가 된다.
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
//			// 왼쪽 위가 벽이고 왼쪽 일반일 경우
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
//			// 오른쪽 위가 벽이고 오른쪽이 일반일 경우
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[IndexY * m_CountX + (IndexX + 1)]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		// 대각선 코너가 아니라면 아래로 진행이 되므로 오른쪽 아래, 그리고 왼쪽 아래로
//		// 코너가 있는지를 판단하여 코너가 있다면 현재 노드를 코너드로 판단하도록
//		// 한다.
//		NavNode1* Find = GetCornerIsometricRB(CornerNode, EndNode);
//
//		// 오른쪽 아래로 검사하던 중 코너가 있다면 이 노드를 코너로 인식한다.
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
//	// 한칸씩 왼쪽으로 가며 오른쪽 위가 막혀있고 위가 뚫려있을 경우나
//	// 오른쪽 아래가 막혀있고 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
//			// 오른쪽 위가 벽이고 위 일반일 경우
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
//			// 오른쪽 아래가 벽이고 아래가 일반일 경우
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(IndexY - 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		// 대각선 코너가 아니라면 왼쪽으로 진행이 되므로 왼쪽 위, 그리고 왼쪽 아래로
//		// 코너가 있는지를 판단하여 코너가 있다면 현재 노드를 코너드로 판단하도록
//		// 한다.
//		NavNode1* Find = GetCornerIsometricLT(CornerNode, EndNode);
//
//		// 왼쪽 위로 검사하던 중 코너가 있다면 이 노드를 코너로 인식한다.
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
//	// 한칸씩 오른쪽으로 가며 왼쪽 위가 막혀있고 위가 뚫려있을 경우나
//	// 왼쪽 아래가 막혀있고 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
//			// 왼쪽 위가 벽이고 위 일반일 경우
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
//			// 왼쪽 아래가 벽이고 아래가 일반일 경우
//			if (m_vecNode[CornerY * m_CountX + CornerX]->TileOption == ETileOption::Wall &&
//				m_vecNode[(IndexY - 2) * m_CountX + IndexX]->TileOption == ETileOption::None)
//				return CornerNode;
//		}
//
//		// 대각선 코너가 아니라면 오른쪽으로 진행이 되므로 오른쪽 위, 그리고 오른쪽 아래로
//		// 코너가 있는지를 판단하여 코너가 있다면 현재 노드를 코너드로 판단하도록
//		// 한다.
//		NavNode1* Find = GetCornerIsometricRT(CornerNode, EndNode);
//
//		// 오른쪽 위로 검사하던 중 코너가 있다면 이 노드를 코너로 인식한다.
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
//	// 한칸씩 왼쪽 위로 올라가며 오른쪽 위가 막혀있고 위가 뚫려있을 경우나
//	// 왼쪽 아래가 막혀있고 왼쪽이 뚫려있으면 해당 노드는 코너가 된다.
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
//			// 오른쪽 위가 벽이고 위가 일반일 경우
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
//			// 왼쪽 아래가 벽이고 왼쪽이 일반일 경우
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
//	// 한칸씩 오른쪽 위로 올라가며 왼쪽 위가 막혀있고 위가 뚫려있을 경우나
//	// 오른쪽 아래가 막혀있고 오른쪽이 뚫려있으면 해당 노드는 코너가 된다.
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
//			// 왼쪽 위가 벽이고 위가 일반일 경우
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
//			// 오른쪽 아래가 벽이고 오른쪽이 일반일 경우
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
//	// 한칸씩 왼쪽 아래로 내려가며 왼쪽 위가 막혀있고 왼쪽이 뚫려있을 경우나
//	// 오른쪽 아래가 막혀있고 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
//			// 왼쪽 위가 벽이고 왼쪽이 일반일 경우
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
//			// 오른쪽 아래가 벽이고 아래가 일반일 경우
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
//	// 한칸씩 오른쪽 아래로 내려가며 오른쪽 위가 막혀있고 오른쪽이 뚫려있을 경우나
//	// 왼쪽 아래가 막혀있고 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
//			// 오른쪽 위가 벽이고 오른쪽이 일반일 경우
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
//			// 왼쪽 아래가 벽이고 아래가 일반일 경우
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
