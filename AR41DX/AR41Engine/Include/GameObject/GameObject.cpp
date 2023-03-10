#include "GameObject.h"
#include "../Component/AnimationMeshComponent.h"
#include "../Component/CameraComponent.h"
#include "../Resource/Animation/Skeleton.h"
#include "../Resource/Animation/SkeletonSocket.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/CameraManager.h"
#include "../Input.h"
#include "../CollisionManager.h"

std::unordered_map<std::string, CGameObject*> CGameObject::m_mapObjectCDO;

CGameObject::CGameObject()  :
	m_Scene(nullptr),
	m_LifeTime(-1.f),
	m_ComponentSerialNumber(0),
	m_Start(false),
	m_FrustumCull(false),
	m_Radius(0.f)
{
	SetTypeID<CGameObject>();

	m_ObjectTypeName = "GameObject";
}

CGameObject::CGameObject(const CGameObject& Obj)    :
	CRef(Obj)
{
	m_ObjectTypeName = Obj.m_ObjectTypeName;
	m_LifeTime = Obj.m_LifeTime;
	m_ComponentSerialNumber = Obj.m_ComponentSerialNumber;
	m_Start = false;

	if (Obj.m_RootComponent)
	{
		m_RootComponent = Obj.m_RootComponent->Clone();

		m_RootComponent->SetOwner(this);

		m_RootComponent->AddOwner();
	}

	{
		m_vecObjectComponent.clear();

		auto	iter = Obj.m_vecObjectComponent.begin();
		auto	iterEnd = Obj.m_vecObjectComponent.end();

		for (; iter != iterEnd; ++iter)
		{
			CObjectComponent* Component = (*iter)->Clone();

			Component->SetOwner(this);

			m_vecObjectComponent.push_back(Component);
		}
	}
}

CGameObject::~CGameObject()
{
	if (m_RootComponent)
		m_RootComponent->Destroy();

	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Destroy();
	}
}

void CGameObject::SetScene(CScene* Scene)
{
	m_Scene = Scene;

	if (m_RootComponent)
		m_RootComponent->SetScene(Scene);

	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->SetScene(Scene);
	}
}

void CGameObject::Destroy()
{
	CRef::Destroy();

	if (m_RootComponent)
		m_RootComponent->Destroy();

	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Destroy();
	}
}

void CGameObject::AddChildToSocket(const std::string& SocketName,
	CGameObject* Obj)
{
	CAnimationMeshComponent* Mesh = nullptr;

	// 애니메이션메쉬 컴포넌트가 있는지 판단한다.
	auto	iter = m_SceneComponentList.begin();
	auto	iterEnd = m_SceneComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->CheckTypeID<CAnimationMeshComponent>())
		{
			Mesh = (CAnimationMeshComponent*)*iter;
			break;
		}
	}

	if (!Mesh)
		return;

	CSkeletonSocket* Socket = Mesh->GetSkeleton()->GetSocket(SocketName);

	if (!Socket)
		return;

	Obj->GetRootComponent()->SetSocket(Socket);
}

void CGameObject::GetAllComponentHierarchyName(std::vector<HierarchyName>& vecName)
{
	if (m_RootComponent)
	{
		HierarchyName	Names;

		Names.Name = m_RootComponent->GetName();
		Names.ClassName = m_RootComponent->GetComponentTypeName();
		Names.Component = m_RootComponent;
		Names.Parent = nullptr;

		vecName.push_back(Names);

		m_RootComponent->GetAllComponentHierarchyName(vecName);
	}
}

CComponent* CGameObject::FindComponent(const std::string& Name)
{
	auto    iter = m_SceneComponentList.begin();
	auto    iterEnd = m_SceneComponentList.end();
	
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return *iter;
	}

	auto    iter1 = m_vecObjectComponent.begin();
	auto    iter1End = m_vecObjectComponent.end();

	for (; iter1 != iter1End; ++iter1)
	{
		if ((*iter1)->GetName() == Name)
			return *iter1;
	}

	return nullptr;
}

void CGameObject::Start()
{
	m_Start = true;

	if (m_RootComponent)
	{
		m_PrevPos = GetWorldPos();
		m_RootComponent->Start();
	}

	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Start();
	}
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float DeltaTime)
{
	if (m_LifeTime > 0.f)
	{
		m_LifeTime -= DeltaTime;

		if (m_LifeTime <= 0.f)
		{
			Destroy();
			return;
		}
	}

	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Update(DeltaTime);
	}

	if (m_RootComponent)
		m_RootComponent->Update(DeltaTime);
}

void CGameObject::PostUpdate(float DeltaTime)
{
	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->PostUpdate(DeltaTime);
	}

	if (m_RootComponent)
	{
		m_Move = GetWorldPos() - m_PrevPos;
		m_PrevPos = GetWorldPos();
		m_RootComponent->PostUpdate(DeltaTime);
	}
	auto	iter = m_SceneComponentList.begin();
	auto	iterEnd = m_SceneComponentList.end();

	m_Min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_Max = Vector3(FLT_MIN, FLT_MIN, FLT_MIN);

	for (; iter != iterEnd; ++iter)
	{
		Vector3	Min = (*iter)->GetMeshSize() * (*iter)->GetWorldScale();

		if (m_Min.x > (*iter)->GetMin().x)
			m_Min.x = (*iter)->GetMin().x;

		if (m_Min.y > (*iter)->GetMin().y)
			m_Min.y = (*iter)->GetMin().y;

		if (m_Min.z > (*iter)->GetMin().z)
			m_Min.z = (*iter)->GetMin().z;

		if (m_Max.x < (*iter)->GetMax().x)
			m_Max.x = (*iter)->GetMax().x;

		if (m_Max.y < (*iter)->GetMax().y)
			m_Max.y = (*iter)->GetMax().y;

		if (m_Max.z < (*iter)->GetMax().z)
			m_Max.z = (*iter)->GetMax().z;
	}

	m_Center = (m_Min + m_Max) * 0.5f;
	m_Radius = (m_Max - m_Min).Length() * 0.5f;
}

CGameObject* CGameObject::Clone() const
{
	return new CGameObject(*this);
}

void CGameObject::Save(FILE* File)
{
	CRef::Save(File);

	fwrite(&m_LifeTime, 4, 1, File);

	{
		int	Length = (int)m_RootComponent->GetComponentTypeName().length();

		fwrite(&Length, 4, 1, File);
		fwrite(m_RootComponent->GetComponentTypeName().c_str(), 1, Length, File);

		m_RootComponent->Save(File);
	}

	{
		int	Count = (int)m_vecObjectComponent.size();

		fwrite(&Count, 4, 1, File);

		auto	iter = m_vecObjectComponent.begin();
		auto	iterEnd = m_vecObjectComponent.end();

		for (; iter != iterEnd; ++iter)
		{
			int	Length = (int)(*iter)->GetComponentTypeName().length();

			fwrite(&Length, 4, 1, File);
			fwrite((*iter)->GetComponentTypeName().c_str(), 1, Length, File);

			(*iter)->Save(File);
		}
	}
}

void CGameObject::Load(FILE* File)
{
	CRef::Load(File);

	fread(&m_LifeTime, 4, 1, File);

	{
		int	Length = 0;
		char	TypeName[256] = {};

		fread(&Length, 4, 1, File);
		fread(TypeName, 1, Length, File);

		if (!m_RootComponent)
		{
			CComponent* CDO = CComponent::FindCDO(TypeName);

			m_RootComponent = (CSceneComponent*)CDO->Clone();

			m_RootComponent->SetOwner(this);
			m_RootComponent->SetScene(m_Scene);

			m_RootComponent->Load(File);

			m_RootComponent->AddOwner();
		}

		else
		{
			m_RootComponent->SetOwner(this);
			m_RootComponent->SetScene(m_Scene);

			m_RootComponent->Load(File);
		}
	}

	{
		int	Count = 0;

		fread(&Count, 4, 1, File);

		bool	Empty = m_vecObjectComponent.empty();

		for (int i = 0; i < Count; ++i)
		{
			int	Length = 0;
			char	TypeName[256] = {};

			fread(&Length, 4, 1, File);
			fread(TypeName, 1, Length, File);

			if (Empty)
			{
				// CDO를 얻어온다.
				CComponent* CDO = CComponent::FindCDO(TypeName);

				CComponent* Component = CDO->Clone();

				Component->SetOwner(this);
				Component->SetScene(m_Scene);
				Component->Load(File);

				m_vecObjectComponent.push_back((CObjectComponent*)Component);
			}

			else
			{
				m_vecObjectComponent[i]->SetOwner(this);
				m_vecObjectComponent[i]->SetScene(m_Scene);
				m_vecObjectComponent[i]->Load(File);
			}
		}
	}
}

int CGameObject::InflictDamage(int damage)
{
	return -1;
}

void CGameObject::Reset()
{
}

void CGameObject::FrustumCull(CCameraComponent* Camera)
{
	bool	Cull = false;

	auto	iter = m_SceneComponentList.begin();
	auto	iterEnd = m_SceneComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (Camera->FrustumInSphere((*iter)->GetWorldPos(),
			(*iter)->GetRadius()))
			Cull = true;
	}

	m_FrustumCull = Cull;
}

bool CGameObject::Picking(PickingResult& result)
{
	Ray	ray = CInput::GetInst()->GetRay();

	// 월드공간으로 변환한다.
	CCameraComponent* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();

	Matrix	matView = Camera->GetViewMatrix();

	matView.Inverse();

	ray.Pos = ray.Pos.TransformCoord(matView);
	ray.Dir = ray.Dir.TransformNormal(matView);
	ray.Dir.Normalize();

	std::list<CSceneComponent*> SceneComponentList =
		m_SceneComponentList;

	SceneComponentList.sort(CGameObject::SortComponent);

	auto iter = SceneComponentList.begin();
	auto iterEnd = SceneComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		Vector3	Center = (*iter)->GetCenter();
		float	Radius = (*iter)->GetRadius();

		if (CCollisionManager::GetInst()->CollisionRayToSphere(result, ray, Center, Radius))
		{
			result.PickObject = this;
			result.PickComponent = *iter;
			return true;
		}
	}

	return false;
}

bool CGameObject::SortComponent(CSceneComponent* Src,
	CSceneComponent* Dest)
{
	Vector3	SrcCenter = Src->GetCenter();
	Vector3	DestCenter = Dest->GetCenter();

	CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	float	SrcDist = Camera->GetWorldPos().Distance(SrcCenter) - Src->GetRadius();
	float	DestDist = Camera->GetWorldPos().Distance(DestCenter) - Dest->GetRadius();

	return SrcDist < DestDist;
}

void CGameObject::SetInheritScale(bool Inherit)
{
	if (m_RootComponent)
		m_RootComponent->SetInheritScale(Inherit);
}

void CGameObject::SetInheritRotX(bool Inherit)
{
	if (m_RootComponent)
		m_RootComponent->SetInheritRotX(Inherit);
}

void CGameObject::SetInheritRotY(bool Inherit)
{
	if (m_RootComponent)
		m_RootComponent->SetInheritRotY(Inherit);
}

void CGameObject::SetInheritRotZ(bool Inherit)
{
	if (m_RootComponent)
		m_RootComponent->SetInheritRotZ(Inherit);
}

void CGameObject::SetInheritParentRotationPosX(bool Inherit)
{
	if (m_RootComponent)
		m_RootComponent->SetInheritParentRotationPosX(Inherit);
}

void CGameObject::SetInheritParentRotationPosY(bool Inherit)
{
	if (m_RootComponent)
		m_RootComponent->SetInheritParentRotationPosY(Inherit);
}

void CGameObject::SetInheritParentRotationPosZ(bool Inherit)
{
	if (m_RootComponent)
		m_RootComponent->SetInheritParentRotationPosZ(Inherit);
}

void CGameObject::InheritRotation(bool Current)
{
	if (m_RootComponent)
		m_RootComponent->InheritRotation(Current);
}

void CGameObject::InheritParentRotationPos()
{
	if (m_RootComponent)
		m_RootComponent->InheritParentRotationPos();
}

void CGameObject::InheritWorldRotation(bool Current)
{
	m_RootComponent->InheritWorldRotation(Current);
}

void CGameObject::InheritWorldParentRotationPos()
{
	m_RootComponent->InheritWorldParentRotationPos();
}

Vector3 CGameObject::GetRelativeScale() const
{
	return m_RootComponent->GetRelativeScale();
}

Vector3 CGameObject::GetRelativeRot() const
{
	return m_RootComponent->GetRelativeRot();
}

Vector3 CGameObject::GetRelativePos() const
{
	return m_RootComponent->GetRelativePos();
}

Vector3 CGameObject::GetRelativeAxis(AXIS Axis) const
{
	return m_RootComponent->GetRelativeAxis(Axis);
}

void CGameObject::SetRelativeScale(const Vector3& Scale)
{
	m_RootComponent->SetRelativeScale(Scale);
}

void CGameObject::SetRelativeScale(const Vector2& Scale)
{
	m_RootComponent->SetRelativeScale(Scale);
}

void CGameObject::SetRelativeScale(float x, float y)
{
	m_RootComponent->SetRelativeScale(x, y);
}

void CGameObject::SetRelativeScale(float x, float y, float z)
{
	m_RootComponent->SetRelativeScale(x, y, z);
}

void CGameObject::SetRelativeScaleX(float x)
{
	m_RootComponent->SetRelativeScaleX(x);
}

void CGameObject::SetRelativeScaleY(float y)
{
	m_RootComponent->SetRelativeScaleY(y);
}

void CGameObject::SetRelativeScaleZ(float z)
{
	m_RootComponent->SetRelativeScaleZ(z);
}

void CGameObject::SetRelativeRotation(const Vector3& Rot)
{
	m_RootComponent->SetRelativeRotation(Rot);
}

void CGameObject::SetRelativeRotation(const Vector2& Rot)
{
	m_RootComponent->SetRelativeRotation(Rot);
}

void CGameObject::SetRelativeRotation(float x, float y)
{
	m_RootComponent->SetRelativeRotation(x, y);
}

void CGameObject::SetRelativeRotation(float x, float y, float z)
{
	m_RootComponent->SetRelativeRotation(x, y, z);
}

void CGameObject::SetRelativeRotationX(float x)
{
	m_RootComponent->SetRelativeRotationX(x);
}

void CGameObject::SetRelativeRotationY(float y)
{
	m_RootComponent->SetRelativeRotationY(y);
}

void CGameObject::SetRelativeRotationZ(float z)
{
	m_RootComponent->SetRelativeRotationZ(z);
}

void CGameObject::SetRelativePosition(const Vector3& Pos)
{
	m_RootComponent->SetRelativePosition(Pos);
}

void CGameObject::SetRelativePosition(const Vector2& Pos)
{
	m_RootComponent->SetRelativePosition(Pos);
}

void CGameObject::SetRelativePosition(float x, float y)
{
	m_RootComponent->SetRelativePosition(x, y);
}

void CGameObject::SetRelativePosition(float x, float y, float z)
{
	m_RootComponent->SetRelativePosition(x, y, z);
}

void CGameObject::SetRelativePositionX(float x)
{
	m_RootComponent->SetRelativePositionX(x);
}

void CGameObject::SetRelativePositionY(float y)
{
	m_RootComponent->SetRelativePositionY(y);
}

void CGameObject::SetRelativePositionZ(float z)
{
	m_RootComponent->SetRelativePositionZ(z);
}

void CGameObject::AddRelativeScale(const Vector3& Scale)
{
	m_RootComponent->AddRelativeScale(Scale);
}

void CGameObject::AddRelativeScale(const Vector2& Scale)
{
	m_RootComponent->AddRelativeScale(Scale);
}

void CGameObject::AddRelativeScale(float x, float y)
{
	m_RootComponent->AddRelativeScale(x, y);
}

void CGameObject::AddRelativeScale(float x, float y, float z)
{
	m_RootComponent->AddRelativeScale(x, y, z);
}

void CGameObject::AddRelativeScaleX(float x)
{
	m_RootComponent->AddRelativeScaleX(x);
}

void CGameObject::AddRelativeScaleY(float y)
{
	m_RootComponent->AddRelativeScaleY(y);
}

void CGameObject::AddRelativeScaleZ(float z)
{
	m_RootComponent->AddRelativeScaleZ(z);
}

void CGameObject::AddRelativeRotation(const Vector3& Rot)
{
	m_RootComponent->AddRelativeRotation(Rot);
}

void CGameObject::AddRelativeRotation(const Vector2& Rot)
{
	m_RootComponent->AddRelativeRotation(Rot);
}

void CGameObject::AddRelativeRotation(float x, float y)
{
	m_RootComponent->AddRelativeRotation(x, y);
}

void CGameObject::AddRelativeRotation(float x, float y, float z)
{
	m_RootComponent->AddRelativeRotation(x, y, z);
}

void CGameObject::AddRelativeRotationX(float x)
{
	m_RootComponent->AddRelativeRotationX(x);
}

void CGameObject::AddRelativeRotationY(float y)
{
	m_RootComponent->AddRelativeRotationY(y);
}

void CGameObject::AddRelativeRotationZ(float z)
{
	m_RootComponent->AddRelativeRotationZ(z);
}

void CGameObject::AddRelativePosition(const Vector3& Pos)
{
	m_RootComponent->AddRelativePosition(Pos);
}

void CGameObject::AddRelativePosition(const Vector2& Pos)
{
	m_RootComponent->AddRelativePosition(Pos);
}

void CGameObject::AddRelativePosition(float x, float y)
{
	m_RootComponent->AddRelativePosition(x, y);
}

void CGameObject::AddRelativePosition(float x, float y, float z)
{
	m_RootComponent->AddRelativePosition(x, y, z);
}

void CGameObject::AddRelativePositionX(float x)
{
	m_RootComponent->AddRelativePositionX(x);
}

void CGameObject::AddRelativePositionY(float y)
{
	m_RootComponent->AddRelativePositionY(y);
}

void CGameObject::AddRelativePositionZ(float z)
{
	m_RootComponent->AddRelativePositionZ(z);
}

float CGameObject::GetRadius() const
{
	return m_RootComponent->GetRadius();
}

const Vector3& CGameObject::GetWorldScale() const
{
	return m_RootComponent->GetWorldScale();
}

const Vector3& CGameObject::GetWorldRot() const
{
	return m_RootComponent->GetWorldRot();
}

const Vector3& CGameObject::GetWorldPos() const
{
	return m_RootComponent->GetWorldPos();
}

const Vector3& CGameObject::GetWorldAxis(AXIS Axis) const
{
	return m_RootComponent->GetWorldAxis(Axis);
}

const Vector3& CGameObject::GetPivot() const
{
	return m_RootComponent->GetPivot();
}

const Vector3& CGameObject::GetMeshSize() const
{
	return m_RootComponent->GetMeshSize();
}

const Vector3& CGameObject::GetOffset() const
{
	return m_RootComponent->GetOffset();
}

const Matrix& CGameObject::GetWorldMatrix() const
{
	return m_RootComponent->GetWorldMatrix();
}

void CGameObject::SetPivot(const Vector3& Pivot)
{
	m_RootComponent->SetPivot(Pivot);
}

void CGameObject::SetPivot(const Vector2& Pivot)
{
	m_RootComponent->SetPivot(Pivot);
}

void CGameObject::SetPivot(float x, float y, float z)
{
	m_RootComponent->SetPivot(x, y, z);
}

void CGameObject::SetPivot(float x, float y)
{
	m_RootComponent->SetPivot(x, y);
}

void CGameObject::SetMeshSize(const Vector3& MeshSize)
{
	m_RootComponent->SetMeshSize(MeshSize);
}

void CGameObject::SetMeshSize(const Vector2& MeshSize)
{
	m_RootComponent->SetMeshSize(MeshSize);
}

void CGameObject::SetMeshSize(float x, float y, float z)
{
	m_RootComponent->SetMeshSize(x, y, z);
}

void CGameObject::SetMeshSize(float x, float y)
{
	m_RootComponent->SetMeshSize(x, y);
}

void CGameObject::SetWorldScale(const Vector3& Scale)
{
	m_RootComponent->SetWorldScale(Scale);
}

void CGameObject::SetWorldScale(const Vector2& Scale)
{
	m_RootComponent->SetWorldScale(Scale);
}

void CGameObject::SetWorldScale(float x, float y)
{
	m_RootComponent->SetWorldScale(x, y);
}

void CGameObject::SetWorldScale(float x, float y, float z)
{
	m_RootComponent->SetWorldScale(x, y, z);
}

void CGameObject::SetWorldScaleX(float x)
{
	m_RootComponent->SetWorldScaleX(x);
}

void CGameObject::SetWorldScaleY(float y)
{
	m_RootComponent->SetWorldScaleY(y);
}

void CGameObject::SetWorldScaleZ(float z)
{
	m_RootComponent->SetWorldScaleZ(z);
}

void CGameObject::SetWorldRotation(const Vector3& Rot)
{
	m_RootComponent->SetWorldRotation(Rot);
}

void CGameObject::SetWorldRotation(const Vector2& Rot)
{
	m_RootComponent->SetWorldRotation(Rot);
}

void CGameObject::SetWorldRotation(float x, float y)
{
	m_RootComponent->SetWorldRotation(x, y);
}

void CGameObject::SetWorldRotation(float x, float y, float z)
{
	m_RootComponent->SetWorldRotation(x, y, z);
}

void CGameObject::SetWorldRotationX(float x)
{
	m_RootComponent->SetWorldRotationX(x);
}

void CGameObject::SetWorldRotationY(float y)
{
	m_RootComponent->SetWorldRotationY(y);
}

void CGameObject::SetWorldRotationZ(float z)
{
	m_RootComponent->SetWorldRotationZ(z);
}

void CGameObject::SetWorldPosition(const Vector3& Pos)
{
	m_RootComponent->SetWorldPosition(Pos);
}

void CGameObject::SetWorldPosition(const Vector2& Pos)
{
	m_RootComponent->SetWorldPosition(Pos);
}

void CGameObject::SetWorldPosition(float x, float y)
{
	m_RootComponent->SetWorldPosition(x, y);
}

void CGameObject::SetWorldPosition(float x, float y, float z)
{
	m_RootComponent->SetWorldPosition(x, y, z);
}

void CGameObject::SetWorldPositionX(float x)
{
	m_RootComponent->SetWorldPositionX(x);
}

void CGameObject::SetWorldPositionY(float y)
{
	m_RootComponent->SetWorldPositionY(y);
}

void CGameObject::SetWorldPositionZ(float z)
{
	m_RootComponent->SetWorldPositionZ(z);
}

void CGameObject::SetOffset(const Vector3& Offset)
{
	m_RootComponent->SetOffset(Offset);
}

void CGameObject::SetOffset(const Vector2& Offset)
{
	m_RootComponent->SetOffset(Offset);
}

void CGameObject::SetOffsetX(float x)
{
	m_RootComponent->SetOffsetX(x);
}

void CGameObject::SetOffsetY(float y)
{
	m_RootComponent->SetOffsetY(y);
}

void CGameObject::SetOffsetZ(float z)
{
	m_RootComponent->SetOffsetZ(z);
}

void CGameObject::AddWorldScale(const Vector3& Scale)
{
	m_RootComponent->AddWorldScale(Scale);
}

void CGameObject::AddWorldScale(const Vector2& Scale)
{
	m_RootComponent->AddWorldScale(Scale);
}

void CGameObject::AddWorldScale(float x, float y)
{
	m_RootComponent->AddWorldScale(x, y);
}

void CGameObject::AddWorldScale(float x, float y, float z)
{
	m_RootComponent->AddWorldScale(x, y, z);
}

void CGameObject::AddWorldScaleX(float x)
{
	m_RootComponent->AddWorldScaleX(x);
}

void CGameObject::AddWorldScaleY(float y)
{
	m_RootComponent->AddWorldScaleY(y);
}

void CGameObject::AddWorldScaleZ(float z)
{
	m_RootComponent->AddWorldScaleZ(z);
}

void CGameObject::AddWorldRotation(const Vector3& Rot)
{
	m_RootComponent->AddWorldRotation(Rot);
}

void CGameObject::AddWorldRotation(const Vector2& Rot)
{
	m_RootComponent->AddWorldRotation(Rot);
}

void CGameObject::AddWorldRotation(float x, float y)
{
	m_RootComponent->AddWorldRotation(x, y);
}

void CGameObject::AddWorldRotation(float x, float y, float z)
{
	m_RootComponent->AddWorldRotation(x, y, z);
}

void CGameObject::AddWorldRotationX(float x)
{
	m_RootComponent->AddWorldRotationX(x);
}

void CGameObject::AddWorldRotationY(float y)
{
	m_RootComponent->AddWorldRotationY(y);
}

void CGameObject::AddWorldRotationZ(float z)
{
	m_RootComponent->AddWorldRotationZ(z);
}

void CGameObject::AddWorldPosition(const Vector3& Pos)
{
	m_RootComponent->AddWorldPosition(Pos);
}

void CGameObject::AddWorldPosition(const Vector2& Pos)
{
	m_RootComponent->AddWorldPosition(Pos);
}

void CGameObject::AddWorldPosition(float x, float y)
{
	m_RootComponent->AddWorldPosition(x, y);
}

void CGameObject::AddWorldPosition(float x, float y, float z)
{
	m_RootComponent->AddWorldPosition(x, y, z);
}

void CGameObject::AddWorldPositionX(float x)
{
	m_RootComponent->AddWorldPositionX(x);
}

void CGameObject::AddWorldPositionY(float y)
{
	m_RootComponent->AddWorldPositionY(y);
}

void CGameObject::AddWorldPositionZ(float z)
{
	m_RootComponent->AddWorldPositionZ(z);
}

void CGameObject::AddOffset(const Vector3& Offset)
{
	m_RootComponent->AddOffset(Offset);
}

void CGameObject::AddOffset(const Vector2& Offset)
{
	m_RootComponent->AddOffset(Offset);
}

void CGameObject::AddOffsetX(float x)
{
	m_RootComponent->AddOffsetX(x);
}

void CGameObject::AddOffsetY(float y)
{
	m_RootComponent->AddOffsetY(y);
}

void CGameObject::AddOffsetZ(float z)
{
	m_RootComponent->AddOffsetZ(z);
}
