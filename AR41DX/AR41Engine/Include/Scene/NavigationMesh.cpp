#include "NavigationMesh.h"
#include "Scene.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "../Component/CameraComponent.h"
#include "NavigationManager3D.h"
#include "../Thread/ThreadManager.h"

CNavigationMesh::CNavigationMesh() :
	m_SectionArray(nullptr),
	m_Terrain(nullptr),
	m_SectionX(0),
	m_SectionY(0),
	m_Min(10000000.f, 10000000.f, 10000000.f),
	m_Max(-10000000.f, -10000000.f, -10000000.f)
{
}

CNavigationMesh::CNavigationMesh(const CNavigationMesh& Nav)
{
	m_vecCell.clear();

	size_t	Size = Nav.m_vecCell.size();

	for (size_t i = 0; i < Size; i++)
	{
		NavMeshInfo* Cell = new NavMeshInfo;

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
			m_SectionArray[i].vecCell = m_vecCell;
		}
	}
}

CNavigationMesh::~CNavigationMesh()
{
	size_t Size = m_vecCell.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecCell[i]);
	}

	SAFE_DELETE_ARRAY(m_SectionArray);
	CThreadManager::GetInst()->DeleteNavigationThread(this);
}

CScene* CNavigationMesh::GetScene() const
{
	return m_Mgr->GetScene();
}

std::string CNavigationMesh::GetSceneName() const
{
	return m_Mgr->GetScene()->GetName();
}

std::string CNavigationMesh::GetComponentName() const
{
	return m_Terrain->GetName();
}

void CNavigationMesh::CreateNavigation(CTerrainComponent* Terrain)
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

		NavMeshInfo* Cell = new NavMeshInfo;

		for (int j = 0; j < 3; j++)
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

		if (Angle >= 50.f)
		{
			float MinY = Cell->Pos[0].y;
			float MaxY = Cell->Pos[0].y;

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
		m_vecCellFind.push_back(Cell);
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

void CNavigationMesh::CreateAdjGrid()
{
	size_t Size = m_vecCell.size();

	for (size_t i = 0; i < Size; i += 2)
	{
		NavAdjInfo Info = {};

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

void CNavigationMesh::CreateAdj()
{
	size_t	Size = m_vecCell.size();

	FILE* File = NULL;

	fopen_s(&File, "NaviAdj.txt", "wt");

	Vector3	Origin[2][3];
	Vector3	EdgeDir[2][3];
	float Length[2][3];

	for (size_t i = 0; i < Size - 1; i++)
	{
		Vector3	Src[3];
		Vector3	SrcOrigin[3];
		float	SrcLength[3] = {};
		for (int k = 0; k < 3; k++)
		{
			EdgeDir[0][k] = m_vecCell[i]->Edge[k];

			Length[0][k] = EdgeDir[0][k].Length();

			EdgeDir[0][k].Normalize();
		}

		Origin[0][0] = m_vecCell[i]->Pos[0];
		Origin[0][1] = m_vecCell[i]->Pos[1];
		Origin[0][2] = m_vecCell[i]->Pos[2];

		for (size_t j = i + 1; j < Size; j++)
		{
			// 삼각형의 3개의 엣지들을 서로 내적하여 평행한지 비교한다.
			Vector3	Dest[3];
			Vector3	DestOrigin[3];
			int	PosIdx[3][2] = { 1, 0, 2, 1, 2, 0 };
			bool	Insert = false;

			Origin[1][0] = m_vecCell[j]->Pos[0];
			Origin[1][1] = m_vecCell[j]->Pos[1];
			Origin[1][2] = m_vecCell[j]->Pos[2];

			for (int k = 0; k < 3; k++)
			{
				EdgeDir[1][k] = m_vecCell[j]->Edge[k];

				Length[1][k] = EdgeDir[1][k].Length();

				EdgeDir[1][k].Normalize();
			}

			for (int k = 0; k < 3; k++)
			{
				if (Insert)
					break;

				for (int l = 0; l < 3; l++)
				{
					// A삼각형과 B삼각형의 엣지를 구성하는 두 정점이 같을 경우
					// 이 삼각형은 무조건 붙어있는 삼각형이다.
					if ((Origin[0][PosIdx[k][0]] == Origin[1][PosIdx[l][0]] ||
						Origin[0][PosIdx[k][0]] == Origin[1][PosIdx[l][1]]) &&
						(Origin[0][PosIdx[k][1]] == Origin[1][PosIdx[l][0]] ||
							Origin[0][PosIdx[k][1]] == Origin[1][PosIdx[l][1]]))
					{
						NavAdjInfo	Info = {};
						Info.Index = (int)j;
						Info.EdgeIndex = (int)k;
						m_vecCell[i]->vecAdj.push_back(Info);

						Info.Index = (int)i;
						Info.EdgeIndex = (int)l;
						m_vecCell[j]->vecAdj.push_back(Info);
						Insert = true;
						break;
					}

					// 두 엣지에 대해서 상대 엣지를 구성하는 2개의 점이 대상 엣지
					// 위에 둘다 존재하는지를 판단한다. 둘다 존재한다면
					// 무조건 붙어있는 것이다.
					// (B1 - A1, B2 - A1), (B1 - A2, B2 - A2)
					// 내적을 통해서 빼준 벡터의 크기가 그대로 나온다면
					// 직선상에 존재하는 것이다. 단, 둘다 크기가 나올 경우는
					// 무조건 존재하는 것이고 둘중 하나만 나온다면 크기가 0이라면
					// 하나의 점만 직선위에 존재하고 두 점 위치가 같다는 것이므로
					// 해당 삼각형은 인접 삼각형이 아니다.
					// B1 - A1 처리
					if (CheckOnEdge((int)i, (int)j, Origin[1][PosIdx[l][0]],
						Origin[0][PosIdx[k][0]], EdgeDir[0][k], Length[0][k], k, l))
					{
						Insert = true;
						break;
					}

					// B2 - A1 처리
					else if (CheckOnEdge((int)i, (int)j, Origin[1][PosIdx[l][1]],
						Origin[0][PosIdx[k][0]], EdgeDir[0][k], Length[0][k], k, l))
					{
						Insert = true;
						break;
					}

					// B1 - A2 처리
					else if (CheckOnEdge((int)i, (int)j, Origin[1][PosIdx[l][0]],
						Origin[0][PosIdx[k][1]], EdgeDir[0][k] * -1.f, Length[0][k], k, l))
					{
						Insert = true;
						break;
					}

					// B2 - A2 처리
					else if (CheckOnEdge((int)i, (int)j, Origin[1][PosIdx[l][1]],
						Origin[0][PosIdx[k][1]], EdgeDir[0][k] * -1.f, Length[0][k], k, l))
					{
						Insert = true;
						break;
					}
				}
			}
		}
	}

	for (size_t i = 0; i < Size; i++)
	{
		char Text[256] = {};

		sprintf_s(Text, "%d : ", (int)i);

		for (size_t j = 0; j < m_vecCell[i]->vecAdj.size(); j++)
		{
			char AddText[256] = {};

			sprintf_s(AddText, "%d ", m_vecCell[i]->vecAdj[j].Index);

			strcat_s(Text, AddText);
		}

		strcat_s(Text, "\n");

		fputs(Text, File);
	}

	fclose(File);
}

bool CNavigationMesh::CheckOnEdge(int Src, int Dest,
	const Vector3& Origin1, const Vector3& Origin2,
	const Vector3& Edge, float EdgeLength, int Edge1, int Edge2)
{
	if ((int)(Origin2.x * 100) == (int)(Origin1.x * 100) && (int)(Origin2.y * 100) == (int)(Origin1.y * 100) &&
		(int)(Origin2.z * 100) == (int)(Origin1.z * 100))
	{
		return false;
	}

	Vector3	Result = Origin1 - Origin2;

	float fDist = Result.Length();

	// 위에서 구해준 거리를 이용해서 원점으로부터 엣지 방향으로 지정된 거리만큼 이동된 위치에 대상 점이 있는지를
	// 판단한다.
	Result = Origin2 + Edge * fDist;

	if ((int)(Result.x * 100) == (int)(Origin1.x * 100) && (int)(Result.y * 100) == (int)(Origin1.y * 100) &&
		(int)(Result.z * 100) == (int)(Origin1.z * 100))
	{
		Result -= Origin2;
		if (Result.Length() < EdgeLength)
		{
			NavAdjInfo	Info = {};
			Info.Index = Dest;
			Info.EdgeIndex = Edge1;
			m_vecCell[Src]->vecAdj.push_back(Info);

			Info.Index = Src;
			Info.EdgeIndex = Edge2;
			m_vecCell[Dest]->vecAdj.push_back(Info);
			return true;
		}
	}

	return false;
}

float CNavigationMesh::ccw(const Vector2& v1, const Vector2& v2)
{
	return 0.0f;
}

float CNavigationMesh::ccw(const Vector2& v1, const Vector2& v2, const Vector2& v3)
{
	return 0.0f;
}

void CNavigationMesh::CreateSection()
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

float CNavigationMesh::GetHeight(const Vector3& Pos)
{
	if (!m_Grid)
	{
		return 0.f;
	}

	Vector3	ConvertPos = Pos - m_Min;

	float MaxHeight = m_Max.z - m_Min.z;

	ConvertPos.z = MaxHeight - ConvertPos.z;

	ConvertPos.x /= m_CellSize.x;
	ConvertPos.z /= m_CellSize.y;

	float RatioX = ConvertPos.x - (int)ConvertPos.x;
	float RatioY = ConvertPos.z - (int)ConvertPos.z;

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

	NavMeshInfo* Cell = m_vecCell[Index];

	// 좌하단
	if (RatioX < RatioY)
	{
		return Cell->Pos[0].y + (Cell->Pos[1].y - Cell->Pos[2].y) * RatioX + (Cell->Pos[2].y - Cell->Pos[0].y) * RatioY;
	}

	// 우상단
	return Cell->Pos[0].y + (Cell->Pos[1].y - Cell->Pos[0].y) * RatioX + (Cell->Pos[2].y - Cell->Pos[1].y) * RatioY;
}

NavMeshInfo* CNavigationMesh::FindNavMeshInfo(const Vector3& Pos)
{
	int	Index = FindNavMeshInfoIndex(Pos);

	if (Index == -1)
	{
		return nullptr;
	}

	return m_vecCell[Index];
}

int CNavigationMesh::FindNavMeshInfoIndex(const Vector3& Pos)
{
	if (!m_Grid)
	{
		return -1;
	}

	Vector3	ConvertPos = Pos - m_Min;

	float MaxHeight = m_Max.z - m_Min.z;

	ConvertPos.z = MaxHeight - ConvertPos.z;

	ConvertPos.x /= m_CellSize.x;
	ConvertPos.z /= m_CellSize.y;

	int	IndexX = (int)ConvertPos.x;
	int	IndexY = (int)ConvertPos.z;

	if (IndexX < 0 || IndexX >= m_LineRectCountX)
	{
		return -1;
	}

	else if (IndexY < 0 || IndexY >= m_LineRectCountY)
	{
		return -1;
	}

	float RatioX = ConvertPos.x - (int)ConvertPos.x;
	float RatioY = ConvertPos.z - (int)ConvertPos.z;

	if (RatioX >= RatioY)
	{
		return IndexY * m_LineRectCountX * 2 + IndexX * 2;
	}

	return IndexY * m_LineRectCountX * 2 + IndexX * 2 + 1;
}

NavMeshInfo* CNavigationMesh::GetNavMeshInfo(int Index)
{
	if (Index < 0 || Index >= (int)m_vecCell.size())
	{
		return nullptr;
	}

	return m_vecCell[Index];
}

bool CNavigationMesh::GetPickingPos(Vector3& Result, const Ray& ray)
{
	// 1. 광선이 관통하는 섹션 목록을 구한다.
	// 2. 카메라의 가까운 순서대로 섹션 목록을 정렬한다.
	// 3. 섹션 안에 있는 셀과 충돌처리를 진행하여 관통하는 삼각형을 구한다.
	// 이때, 셀은 카메라와 가까운 순서대로 정렬하여 검사한다.
	// 단, DDT 를 적용했을 경우 위는 무시한다.
	// 이거 꼭 만들어보세요.
	std::sort(m_vecCellFind.begin(), m_vecCellFind.end(), CNavigationMesh::SortCamera);

	size_t Size = m_vecCellFind.size();

	for (size_t i = 0; i < Size; i++)
	{
		NavMeshInfo* Cell = m_vecCellFind[i];
		float Dist = 0.f;

		if (RayIntersectTriangle(Result, Dist, ray.Pos, ray.Dir, Cell->Pos[0], Cell->Pos[1], Cell->Pos[2]))
		{
			return true;
		}
	}

	return true;
}

bool CNavigationMesh::RayIntersectTriangle(Vector3& ImpactPoint,
	float& Dist, const Vector3& Pos, const Vector3& Dir,
	const Vector3& CellPos0, const Vector3& CellPos1,
	const Vector3& CellPos2)
{
	Vector3 e1, e2, h, s, q;
	float a, f, u, v;

	e1 = CellPos1 - CellPos0;
	e2 = CellPos2 - CellPos0;
	h = Dir.Cross(e2);
	a = e1.Dot(h);

	if (a > -0.00001 && a < 0.00001)
	{
		return false;
	}

	f = 1.f / a;
	s = Pos - CellPos0;

	u = f * s.Dot(h);

	if (u < 0.f || u > 1.f)
	{
		return false;
	}

	q = s.Cross(e1);

	v = f * Dir.Dot(q);

	if (v < 0.f || u + v > 1.f)
	{
		return false;
	}

	Dist = f * e2.Dot(q);

	if (Dist > 0.00001)
	{
		ImpactPoint = Pos + Dir * Dist;
		return true;
	}

	return false;
}

bool CNavigationMesh::SortCamera(NavMeshInfo* Src, NavMeshInfo* Dest)
{
	Vector3	CamPos = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->GetWorldPos();

	float SrcDist = CamPos.Distance(Src->Center);
	float DestDist = CamPos.Distance(Dest->Center);

	return SrcDist < DestDist;
}
