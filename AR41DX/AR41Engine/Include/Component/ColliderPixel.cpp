#include "ColliderPixel.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/CameraManager.h"
#include "../Scene/SceneCollision.h"
#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"
#include "../Engine.h"
#include "../Resource/ResourceManager.h"
#include "CameraComponent.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "../CollisionManager.h"
#include "ColliderBox2D.h"
#include "ColliderOBB2D.h"
#include "ColliderSphere2D.h"
#include "../Device.h"

CColliderPixel::CColliderPixel()	:
	m_Info(nullptr)
{
	SetTypeID<CColliderPixel>();

	m_ComponentTypeName = "ColliderPixel";
	m_Collider2DType = ECollider2D_Type::Pixel;
}

CColliderPixel::CColliderPixel(const CColliderPixel& component) :
	CCollider2D(component)
{
	m_Info = component.m_Info;

	if (m_Info)
		++m_Info->RefCount;

	m_PixelMesh = component.m_PixelMesh;
	m_PixelShader = component.m_PixelShader;
}

CColliderPixel::~CColliderPixel()
{
	if (m_Info)
	{
		--m_Info->RefCount;

		if (m_Info->RefCount == 0)
		{
			SAFE_RELEASE(m_Info->SRV);
			SAFE_DELETE_ARRAY(m_Info->Pixel);
			SAFE_DELETE(m_Info);
		}
	}
}

void CColliderPixel::Start()
{
	CCollider2D::Start();
}

bool CColliderPixel::Init()
{
	if (!CCollider2D::Init())
		return false;


	if (CEngine::GetEditorMode())
	{
		m_Mesh = CResourceManager::GetInst()->FindMesh("Box2DLineMesh");
		m_PixelMesh = CResourceManager::GetInst()->FindMesh("CenterUVRect");
		m_PixelShader = CResourceManager::GetInst()->FindShader("ColliderImageShader");
	}

	return true;
}

void CColliderPixel::Update(float DeltaTime)
{
	CCollider2D::Update(DeltaTime);
}

void CColliderPixel::PostUpdate(float DeltaTime)
{
	CCollider2D::PostUpdate(DeltaTime);

	Vector2	Size = Vector2((float)m_Info->Width, (float)m_Info->Height);
	Size.x *= GetWorldScale().x;
	Size.y *= GetWorldScale().y;

	Vector3	Min, Max;

	Min.x = GetWorldPos().x - Size.x * 0.5f;
	Min.y = GetWorldPos().y - Size.y * 0.5f;

	Max.x = Min.x + Size.x;
	Max.y = Min.y + Size.y;

	m_Info->Box2D.Left = Min.x;
	m_Info->Box2D.Bottom = Min.y;
	m_Info->Box2D.Right = Max.x;
	m_Info->Box2D.Top = Max.y;

	Min.x -= GetWorldPos().x;
	Min.y -= GetWorldPos().y;

	Max.x -= GetWorldPos().x;
	Max.y -= GetWorldPos().y;

	m_Transform->SetMin(Min);
	m_Transform->SetMax(Max);
}

void CColliderPixel::Render()
{
	CCollider2D::Render();

	Matrix	matScale, matTranslate, matWorld;

	Matrix	matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	Matrix	matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();

	Vector3	Scale = Vector3((float)m_Info->Width, (float)m_Info->Height, 1.f);
	Scale.x *= GetWorldScale().x;
	Scale.y *= GetWorldScale().y;
	Scale.z = GetWorldScale().z;

	matScale.Scaling(Scale);
	matTranslate.Translation(GetWorldPos());

	matWorld = matScale * matTranslate;

	CColliderConstantBuffer* Buffer = CResourceManager::GetInst()->GetColliderCBuffer();

	Buffer->SetColor(m_Color);
	Buffer->SetWVP(matWorld * matView * matProj);

	Buffer->UpdateBuffer();

	CDevice::GetInst()->GetContext()->PSSetShaderResources(0, 1, &m_Info->SRV);

	m_PixelShader->SetShader();

	m_PixelMesh->Render();



	m_Shader->SetShader();

	m_Mesh->Render();
}

CColliderPixel* CColliderPixel::Clone() const
{
	return new CColliderPixel(*this);
}

void CColliderPixel::Save(FILE* File)
{
	CCollider2D::Save(File);

	bool	PixelInfoEnable = m_Info ? true : false;

	fwrite(&PixelInfoEnable, sizeof(bool), 1, File);

	if (PixelInfoEnable)
	{
		int	Length = (int)m_Info->Name.length();
		fwrite(&Length, sizeof(int), 1, File);
		fwrite(m_Info->Name.c_str(), 1, Length, File);

		fwrite(m_Info->FileName, sizeof(TCHAR), MAX_PATH, File);
		fwrite(m_Info->PathName, sizeof(char), MAX_PATH, File);

		fwrite(&m_Info->Box2D, sizeof(Box2DInfo), 1, File);
		fwrite(&m_Info->PixelColorCollisionType, sizeof(EPixelCollision_Type), 1, File);
		fwrite(&m_Info->PixelAlphaCollisionType, sizeof(EPixelCollision_Type), 1, File);
		fwrite(m_Info->TypeColor, sizeof(unsigned char), 4, File);
	}
}

void CColliderPixel::Load(FILE* File)
{
	CCollider2D::Load(File);

	bool	PixelInfoEnable = false;

	fread(&PixelInfoEnable, sizeof(bool), 1, File);

	if (PixelInfoEnable)
	{
		if (m_Info)
		{
			--m_Info->RefCount;

			if (m_Info->RefCount == 0)
			{
				SAFE_RELEASE(m_Info->SRV);
				SAFE_DELETE_ARRAY(m_Info->Pixel);
				SAFE_DELETE(m_Info);
			}
		}

		TCHAR	FileName[MAX_PATH] = {};
		char	PathName[MAX_PATH] = {};
		int	Length = 0;
		char	Name[MAX_PATH] = {};

		fread(&Length, sizeof(int), 1, File);
		fread(Name, 1, Length, File);

		fread(FileName, sizeof(TCHAR), MAX_PATH, File);
		fread(PathName, sizeof(char), MAX_PATH, File);

		SetInfo(Name, FileName, PathName);

		fread(&m_Info->Box2D, sizeof(Box2DInfo), 1, File);
		fread(&m_Info->PixelColorCollisionType, sizeof(EPixelCollision_Type), 1, File);
		fread(&m_Info->PixelAlphaCollisionType, sizeof(EPixelCollision_Type), 1, File);
		fread(m_Info->TypeColor, sizeof(unsigned char), 4, File);
	}
}

bool CColliderPixel::Collision(CCollider* Dest)
{
	Vector2	HitPoint;
	bool	Result = false;

	switch (((CCollider2D*)Dest)->GetCollider2DType())
	{
	case ECollider2D_Type::Box2D:
		Result = CCollisionManager::GetInst()->CollisionBox2DToPixel(HitPoint, (CColliderBox2D*)Dest, this);
		break;
	case ECollider2D_Type::OBB2D:
		Result = CCollisionManager::GetInst()->CollisionOBB2DToPixel(HitPoint, (CColliderOBB2D*)Dest, this);
		break;
	case ECollider2D_Type::Sphere2D:
		Result = CCollisionManager::GetInst()->CollisionSphere2DToPixel(HitPoint, (CColliderSphere2D*)Dest, this);
		break;
	case ECollider2D_Type::Pixel:
		Result = CCollisionManager::GetInst()->CollisionPixelToPixel(HitPoint, this, (CColliderPixel*)Dest);
		break;
	}

	m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);

	return Result;
}

bool CColliderPixel::CollisionMouse(const Vector2& MouseWorldPos)
{
	Vector2	HitPoint;

	m_MouseCollision = CCollisionManager::GetInst()->CollisionPointToPixel(HitPoint, MouseWorldPos,
		*m_Info);

	m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);

	return m_MouseCollision;
}

void CColliderPixel::SetInfo(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	if (!m_Scene->GetCollisionManager()->CreatePixelCollision(Name, FileName, PathName))
		return;

	if (m_Info)
	{
		--m_Info->RefCount;

		if (m_Info->RefCount == 0)
		{
			SAFE_RELEASE(m_Info->SRV);
			SAFE_DELETE_ARRAY(m_Info->Pixel);
			SAFE_DELETE(m_Info);
		}
	}

	m_Info = m_Scene->GetCollisionManager()->FindPixelCollision(Name);
	++m_Info->RefCount;

	m_Info->Name = Name;
	lstrcpy(m_Info->FileName, FileName);
	strcpy_s(m_Info->PathName, PathName.c_str());
}

void CColliderPixel::SetInfoFullPath(const std::string& Name, const TCHAR* FullPath)
{
	if (!m_Scene->GetCollisionManager()->CreatePixelCollisionFullPath(Name, FullPath))
		return;

	if (m_Info)
	{
		--m_Info->RefCount;

		if (m_Info->RefCount == 0)
		{
			SAFE_RELEASE(m_Info->SRV);
			SAFE_DELETE_ARRAY(m_Info->Pixel);
			SAFE_DELETE(m_Info);
		}
	}

	m_Info = m_Scene->GetCollisionManager()->FindPixelCollision(Name);
	++m_Info->RefCount;

	m_Info->Name = Name;

	int	PathLength = lstrlen(FullPath);

	for (int i = PathLength - 1; i >= 0; --i)
	{
		// D:\41th\DirectX\Client\Bin\Texture\Test.png
		if ((FullPath[i] == '\\' || FullPath[i] == '/') && i >= 4)
		{
			if ((FullPath[i - 1] == 'n' || FullPath[i - 1] == 'N') &&
				(FullPath[i - 2] == 'i' || FullPath[i - 2] == 'I') &&
				(FullPath[i - 3] == 'b' || FullPath[i - 3] == 'B') &&
				(FullPath[i - 4] == '\\' || FullPath[i - 4] == '/'))
			{
				lstrcpy(m_Info->FileName, &FullPath[i + 1]);
				break;
			}
		}
	}

	strcpy_s(m_Info->PathName, strlen(ROOT_PATH) + 1, ROOT_PATH);
}

void CColliderPixel::SetInfoMultibyte(const std::string& Name, const char* FileName, const std::string& PathName)
{
	if (!m_Scene->GetCollisionManager()->CreatePixelCollisionMultibyte(Name, FileName, PathName))
		return;

	if (m_Info)
	{
		--m_Info->RefCount;

		if (m_Info->RefCount == 0)
		{
			SAFE_RELEASE(m_Info->SRV);
			SAFE_DELETE_ARRAY(m_Info->Pixel);
			SAFE_DELETE(m_Info);
		}
	}

	m_Info = m_Scene->GetCollisionManager()->FindPixelCollision(Name);
	++m_Info->RefCount;

	m_Info->Name = Name;

	TCHAR	ConvertFileName[MAX_PATH] = {};

#ifdef UNICODE

	int	ConvertLength = MultiByteToWideChar(CP_ACP, 0, FileName, -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, FileName, -1, ConvertFileName, ConvertLength);

#else

	strcpy_s(ConvertFileName, FileName);

#endif // UNICODE


	lstrcpy(m_Info->FileName, ConvertFileName);
	strcpy_s(m_Info->PathName, PathName.c_str());
}

void CColliderPixel::SetInfoMultibyteFullPath(const std::string& Name, const char* FullPath)
{
	if (!m_Scene->GetCollisionManager()->CreatePixelCollisionMultibyteFullPath(Name, FullPath))
		return;

	if (m_Info)
	{
		--m_Info->RefCount;

		if (m_Info->RefCount == 0)
		{
			SAFE_RELEASE(m_Info->SRV);
			SAFE_DELETE_ARRAY(m_Info->Pixel);
			SAFE_DELETE(m_Info);
		}
	}

	m_Info = m_Scene->GetCollisionManager()->FindPixelCollision(Name);
	++m_Info->RefCount;

	m_Info->Name = Name;

	TCHAR	ConvertFullPath[MAX_PATH] = {};

#ifdef UNICODE

	int	ConvertLength = MultiByteToWideChar(CP_ACP, 0, FullPath, -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, FullPath, -1, ConvertFullPath, ConvertLength);

#else

	strcpy_s(ConvertFullPath, FullPath);

#endif // UNICODE

	int	PathLength = lstrlen(ConvertFullPath);

	for (int i = PathLength - 1; i >= 0; --i)
	{
		// D:\41th\DirectX\Client\Bin\Texture\Test.png
		if ((FullPath[i] == '\\' || FullPath[i] == '/') && i >= 4)
		{
			if ((FullPath[i - 1] == 'n' || FullPath[i - 1] == 'N') &&
				(FullPath[i - 2] == 'i' || FullPath[i - 2] == 'I') &&
				(FullPath[i - 3] == 'b' || FullPath[i - 3] == 'B') &&
				(FullPath[i - 4] == '\\' || FullPath[i - 4] == '/'))
			{
				lstrcpy(m_Info->FileName, &ConvertFullPath[i + 1]);
				break;
			}
		}
	}

	strcpy_s(m_Info->PathName, strlen(ROOT_PATH) + 1, ROOT_PATH);
}

void CColliderPixel::SetPixelColorCollisionType(EPixelCollision_Type Type)
{
	if (!m_Info)
		return;

	m_Info->PixelColorCollisionType = Type;
}

void CColliderPixel::SetPixelAlphaCollisionType(EPixelCollision_Type Type)
{
	if (!m_Info)
		return;

	m_Info->PixelAlphaCollisionType = Type;
}

void CColliderPixel::SetPixelColor(unsigned char r, unsigned char g, unsigned char b)
{
	if (!m_Info)
		return;

	m_Info->TypeColor[0] = r;
	m_Info->TypeColor[1] = g;
	m_Info->TypeColor[2] = b;
}

void CColliderPixel::SetPixelAlpha(unsigned char a)
{
	if (!m_Info)
		return;

	m_Info->TypeColor[3] = a;
}
