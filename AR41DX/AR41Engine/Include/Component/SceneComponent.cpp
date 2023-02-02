#include "SceneComponent.h"
#include "../GameObject/GameObject.h"
#include "../Resource/Animation/SkeletonSocket.h"
#include "../Scene/Scene.h"

CSceneComponent::CSceneComponent()	:
	m_Parent(nullptr),
	m_Socket(nullptr),
	m_LayerName("Default"),
	m_SceneComponentType(SceneComponentType::Scene)
{
	SetTypeID<CSceneComponent>();

	m_Transform = new CTransform;

	m_Transform->Init();
	m_Transform->m_Owner = this;

	m_ComponentType = ComponentType::Scene;

	m_ComponentTypeName = "SceneComponent";
}

CSceneComponent::CSceneComponent(const CSceneComponent& component)	:
	CComponent(component)
{
	m_Socket = nullptr;

	m_SceneComponentType = component.m_SceneComponentType;

	m_LayerName = component.m_LayerName;

	m_Transform = component.m_Transform->Clone();

	m_Transform->m_Parent = nullptr;
	m_Transform->m_vecChild.clear();

	m_Transform->m_Owner = this;

	m_Parent = nullptr;
	m_vecChild.clear();

	size_t	Size = component.m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		CSceneComponent* ChildCom = component.m_vecChild[i]->Clone();

		AddChild(ChildCom);
	}
}

CSceneComponent::~CSceneComponent()
{
	SAFE_DELETE(m_Transform);
}

void CSceneComponent::AddOwner()
{
	m_Owner->AddSceneComponent(this);

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->AddOwner();
	}
}

void CSceneComponent::SetSocket(class CSkeletonSocket* Socket)
{
	m_Socket = Socket;

	m_Transform->SetSocket(m_Socket);
}

void CSceneComponent::SetScene(CScene* Scene)
{
	CComponent::SetScene(Scene);

	m_Transform->m_Scene = Scene;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->SetScene(Scene);
	}
}

void CSceneComponent::SetOwner(CGameObject* Owner)
{
	CComponent::SetOwner(Owner);

	m_Transform->m_Object = Owner;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->SetOwner(Owner);
	}
}

void CSceneComponent::AddChild(CSceneComponent* Child, const std::string& SocketName)
{
	Child->m_Parent = this;
	Child->m_ParentName = m_Name;

	m_vecChild.push_back(Child);
	m_vecChildName.push_back(Child->GetName());

	Child->m_Transform->m_Parent = m_Transform;

	m_Transform->m_vecChild.push_back(Child->m_Transform);

	//m_Owner->AddSceneComponent(Child);
}

void CSceneComponent::AddChild(CGameObject* Child, const std::string& SocketName)
{
	CSceneComponent* ChildComponent = Child->GetRootComponent();

	ChildComponent->m_Parent = this;
	ChildComponent->m_ParentName = m_Name;

	m_vecChild.push_back(ChildComponent);
	m_vecChildName.push_back(ChildComponent->GetName());

	ChildComponent->m_Transform->m_Parent = m_Transform;

	m_Transform->m_vecChild.push_back(ChildComponent->m_Transform);

	m_Owner->AddSceneComponent(ChildComponent);
}

bool CSceneComponent::DeleteChild(CSceneComponent* Child)
{
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		if (m_vecChild[i] == Child)
		{
			auto iter = m_vecChild.begin() + i;
			auto iterName = m_vecChildName.begin() + i;

			(*iter)->m_Parent = nullptr;
			(*iter)->m_ParentName = "";

			m_Owner->DeleteSceneComponent(*iter);

			m_vecChild.erase(iter);
			m_vecChildName.erase(iterName);

			auto iterTr = m_Transform->m_vecChild.begin() + i;

			(*iterTr)->m_Parent = nullptr;

			m_Transform->m_vecChild.erase(iterTr);

			return true;
		}
	}

	return false;
}

bool CSceneComponent::DeleteChild(const std::string& Name)
{
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		if (m_vecChild[i]->GetName() == Name)
		{
			auto iter = m_vecChild.begin() + i;
			auto iterName = m_vecChildName.begin() + i;

			(*iter)->m_Parent = nullptr;
			(*iter)->m_ParentName = "";

			m_Owner->DeleteSceneComponent(*iter);

			m_vecChild.erase(iter);
			m_vecChildName.erase(iterName);

			auto iterTr = m_Transform->m_vecChild.begin() + i;

			(*iterTr)->m_Parent = nullptr;

			m_Transform->m_vecChild.erase(iterTr);

			return true;
		}
	}

	return false;
}

CSceneComponent* CSceneComponent::FindComponent(const std::string& Name)
{
	if (m_Name == Name)
	{
		return this;
	}

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		CSceneComponent* Find = m_vecChild[i]->FindComponent(Name);

		if (Find)
		{
			return Find;
		}
	}

	return nullptr;
}

void CSceneComponent::GetAllComponentHierarchyName(std::vector<HierarchyName>& vecName)
{
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		HierarchyName	Names;

		CSceneComponent* Parent = m_vecChild[i]->GetParent();

		Names.Name = m_vecChild[i]->GetName();
		Names.ClassName = m_vecChild[i]->GetComponentTypeName();
		Names.Component = m_vecChild[i];
		Names.Parent = Parent;

		if (Parent)
		{
			Names.ParentName = Parent->GetName();
			Names.ParentClassName = Parent->GetComponentTypeName();
		}

		vecName.push_back(Names);
	}

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->GetAllComponentHierarchyName(vecName);
	}
}

void CSceneComponent::Destroy()
{
	CComponent::Destroy();

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->Destroy();
	}
}

void CSceneComponent::Start()
{
	CComponent::Start();

	m_Transform->Start();

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->Start();
	}
}

bool CSceneComponent::Init()
{
	if (!CComponent::Init())
	{
		return false;
	}

	if (m_Scene)
	{
		m_SceneName = m_Scene->GetName();
	}

	return true;
}

void CSceneComponent::Update(float DeltaTime)
{
	CComponent::Update(DeltaTime);

	m_Transform->Update(DeltaTime);

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->Update(DeltaTime);
	}
}

void CSceneComponent::PostUpdate(float DeltaTime)
{
	CComponent::PostUpdate(DeltaTime);

	m_Transform->PostUpdate(DeltaTime);

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->PostUpdate(DeltaTime);
	}
}

void CSceneComponent::Render()
{
	CComponent::Render();

	m_Transform->SetTransform();
}

CSceneComponent* CSceneComponent::Clone() const
{
	return new CSceneComponent(*this);
}

void CSceneComponent::Save(FILE* File)
{
	CComponent::Save(File);

	int	Length = (int)m_LayerName.length();

	fwrite(&Length, 4, 1, File);
	fwrite(m_LayerName.c_str(), 1, Length, File);

	int	ChildCount = (int)m_vecChild.size();


	fwrite(&ChildCount, 4, 1, File);

	for (int i = 0; i < ChildCount; ++i)
	{
		Length = (int)m_vecChild[i]->GetComponentTypeName().length();

		fwrite(&Length, 4, 1, File);
		fwrite(m_vecChild[i]->GetComponentTypeName().c_str(), 1, Length, File);

		m_vecChild[i]->Save(File);
	}

	m_Transform->Save(File);
}

void CSceneComponent::Load(FILE* File)
{
	CComponent::Load(File);

	int	Length = 0;
	char	LayerName[256] = {};

	fread(&Length, 4, 1, File);
	fread(LayerName, 1, Length, File);

	m_LayerName = LayerName;

	int	ChildCount = 0;
	fread(&ChildCount, 4, 1, File);

	if (m_vecChild.empty())
	{
		for (int i = 0; i < ChildCount; ++i)
		{
			Length = 0;
			char	TypeName[256] = {};

			fread(&Length, 4, 1, File);
			fread(TypeName, 1, Length, File);

			CComponent* CDO = CComponent::FindCDO(TypeName);

			CSceneComponent* Component = (CSceneComponent*)CDO->Clone();

			Component->SetOwner(m_Owner);
			Component->SetScene(m_Scene);

			AddChild(Component);

			Component->Load(File);
		}
	}

	else
	{
		for (int i = 0; i < ChildCount; ++i)
		{
			Length = 0;
			char	TypeName[256] = {};

			fread(&Length, 4, 1, File);
			fread(TypeName, 1, Length, File);

			m_vecChild[i]->SetScene(m_Scene);

			m_vecChild[i]->Load(File);
		}
	}

	m_Transform->Load(File);
}

void CSceneComponent::SetInheritScale(bool Inherit)
{
	m_Transform->SetInheritScale(Inherit);
}

void CSceneComponent::SetInheritRotX(bool Inherit)
{
	m_Transform->SetInheritRotX(Inherit);
}

void CSceneComponent::SetInheritRotY(bool Inherit)
{
	m_Transform->SetInheritRotY(Inherit);
}

void CSceneComponent::SetInheritRotZ(bool Inherit)
{
	m_Transform->SetInheritRotZ(Inherit);
}

void CSceneComponent::SetInheritParentRotationPosX(bool Inherit)
{
	m_Transform->SetInheritParentRotationPosX(Inherit);
}

void CSceneComponent::SetInheritParentRotationPosY(bool Inherit)
{
	m_Transform->SetInheritParentRotationPosY(Inherit);
}

void CSceneComponent::SetInheritParentRotationPosZ(bool Inherit)
{
	m_Transform->SetInheritParentRotationPosZ(Inherit);
}

void CSceneComponent::InheritRotation(bool Current)
{
	m_Transform->InheritRotation(Current);
}

void CSceneComponent::InheritParentRotationPos()
{
	m_Transform->InheritParentRotationPos();
}

void CSceneComponent::InheritWorldRotation(bool Current)
{
	m_Transform->InheritWorldRotation(Current);
}

void CSceneComponent::InheritWorldParentRotationPos()
{
	m_Transform->InheritWorldParentRotationPos();
}

Vector3 CSceneComponent::GetRelativeScale() const
{
	return m_Transform->GetRelativeScale();
}

Vector3 CSceneComponent::GetRelativeRot() const
{
	return m_Transform->GetRelativeRot();
}

Vector3 CSceneComponent::GetRelativePos() const
{
	return m_Transform->GetRelativePos();
}

Vector3 CSceneComponent::GetRelativeAxis(AXIS Axis) const
{
	return m_Transform->GetRelativeAxis(Axis);
}

void CSceneComponent::SetRelativeScale(const Vector3& Scale)
{
	m_Transform->SetRelativeScale(Scale);
}

void CSceneComponent::SetRelativeScale(const Vector2& Scale)
{
	m_Transform->SetRelativeScale(Scale);
}

void CSceneComponent::SetRelativeScale(float x, float y)
{
	m_Transform->SetRelativeScale(x, y);
}

void CSceneComponent::SetRelativeScale(float x, float y, float z)
{
	m_Transform->SetRelativeScale(x, y, z);
}

void CSceneComponent::SetRelativeScaleX(float x)
{
	m_Transform->SetRelativeScaleX(x);
}

void CSceneComponent::SetRelativeScaleY(float y)
{
	m_Transform->SetRelativeScaleY(y);
}

void CSceneComponent::SetRelativeScaleZ(float z)
{
	m_Transform->SetRelativeScaleZ(z);
}

void CSceneComponent::SetRelativeRotation(const Vector3& Rot)
{
	m_Transform->SetRelativeRotation(Rot);
}

void CSceneComponent::SetRelativeRotation(const Vector2& Rot)
{
	m_Transform->SetRelativeRotation(Rot);
}

void CSceneComponent::SetRelativeRotation(float x, float y)
{
	m_Transform->SetRelativeRotation(x, y);
}

void CSceneComponent::SetRelativeRotation(float x, float y, float z)
{
	m_Transform->SetRelativeRotation(x, y, z);
}

void CSceneComponent::SetRelativeRotationX(float x)
{
	m_Transform->SetRelativeRotationX(x);
}

void CSceneComponent::SetRelativeRotationY(float y)
{
	m_Transform->SetRelativeRotationY(y);
}

void CSceneComponent::SetRelativeRotationZ(float z)
{
	m_Transform->SetRelativeRotationZ(z);
}

void CSceneComponent::SetRelativePosition(const Vector3& Pos)
{
	m_Transform->SetRelativePosition(Pos);
}

void CSceneComponent::SetRelativePosition(const Vector2& Pos)
{
	m_Transform->SetRelativePosition(Pos);
}

void CSceneComponent::SetRelativePosition(float x, float y)
{
	m_Transform->SetRelativePosition(x, y);
}

void CSceneComponent::SetRelativePosition(float x, float y, float z)
{
	m_Transform->SetRelativePosition(x, y, z);
}

void CSceneComponent::SetRelativePositionX(float x)
{
	m_Transform->SetRelativePositionX(x);
}

void CSceneComponent::SetRelativePositionY(float y)
{
	m_Transform->SetRelativePositionY(y);
}

void CSceneComponent::SetRelativePositionZ(float z)
{
	m_Transform->SetRelativePositionZ(z);
}

void CSceneComponent::AddRelativeScale(const Vector3& Scale)
{
	m_Transform->AddRelativeScale(Scale);
}

void CSceneComponent::AddRelativeScale(const Vector2& Scale)
{
	m_Transform->AddRelativeScale(Scale);
}

void CSceneComponent::AddRelativeScale(float x, float y)
{
	m_Transform->AddRelativeScale(x, y);
}

void CSceneComponent::AddRelativeScale(float x, float y, float z)
{
	m_Transform->AddRelativeScale(x, y, z);
}

void CSceneComponent::AddRelativeScaleX(float x)
{
	m_Transform->AddRelativeScaleX(x);
}

void CSceneComponent::AddRelativeScaleY(float y)
{
	m_Transform->AddRelativeScaleY(y);
}

void CSceneComponent::AddRelativeScaleZ(float z)
{
	m_Transform->AddRelativeScaleZ(z);
}

void CSceneComponent::AddRelativeRotation(const Vector3& Rot)
{
	m_Transform->AddRelativeRotation(Rot);
}

void CSceneComponent::AddRelativeRotation(const Vector2& Rot)
{
	m_Transform->AddRelativeRotation(Rot);
}

void CSceneComponent::AddRelativeRotation(float x, float y)
{
	m_Transform->AddRelativeRotation(x, y);
}

void CSceneComponent::AddRelativeRotation(float x, float y, float z)
{
	m_Transform->AddRelativeRotation(x, y, z);
}

void CSceneComponent::AddRelativeRotationX(float x)
{
	m_Transform->AddRelativeRotationX(x);
}

void CSceneComponent::AddRelativeRotationY(float y)
{
	m_Transform->AddRelativeRotationY(y);
}

void CSceneComponent::AddRelativeRotationZ(float z)
{
	m_Transform->AddRelativeRotationZ(z);
}

void CSceneComponent::AddRelativePosition(const Vector3& Pos)
{
	m_Transform->AddRelativePosition(Pos);
}

void CSceneComponent::AddRelativePosition(const Vector2& Pos)
{
	m_Transform->AddRelativePosition(Pos);
}

void CSceneComponent::AddRelativePosition(float x, float y)
{
	m_Transform->AddRelativePosition(x, y);
}

void CSceneComponent::AddRelativePosition(float x, float y, float z)
{
	m_Transform->AddRelativePosition(x, y, z);
}

void CSceneComponent::AddRelativePositionX(float x)
{
	m_Transform->AddRelativePositionX(x);
}

void CSceneComponent::AddRelativePositionY(float y)
{
	m_Transform->AddRelativePositionY(y);
}

void CSceneComponent::AddRelativePositionZ(float z)
{
	m_Transform->AddRelativePositionZ(z);
}

const Vector3& CSceneComponent::GetWorldScale() const
{
	return m_Transform->GetWorldScale();
}

const Vector3& CSceneComponent::GetWorldRot() const
{
	return m_Transform->GetWorldRot();
}

const Vector3& CSceneComponent::GetWorldPos() const
{
	return m_Transform->GetWorldPos();
}

const Vector3& CSceneComponent::GetWorldAxis(AXIS Axis) const
{
	return m_Transform->GetWorldAxis(Axis);
}

const Vector3& CSceneComponent::GetPivot() const
{
	return m_Transform->GetPivot();
}

const Vector3& CSceneComponent::GetMeshSize() const
{
	return m_Transform->GetMeshSize();
}

const Vector3& CSceneComponent::GetOffset() const
{
	return m_Transform->GetOffset();
}

const Matrix& CSceneComponent::GetWorldMatrix() const
{
	return m_Transform->GetWorldMatrix();
}

void CSceneComponent::SetPivot(const Vector3& Pivot)
{
	m_Transform->SetPivot(Pivot);
}

void CSceneComponent::SetPivot(const Vector2& Pivot)
{
	m_Transform->SetPivot(Pivot);
}

void CSceneComponent::SetPivot(float x, float y, float z)
{
	m_Transform->SetPivot(x, y, z);
}

void CSceneComponent::SetPivot(float x, float y)
{
	m_Transform->SetPivot(x, y);
}

void CSceneComponent::SetMeshSize(const Vector3& MeshSize)
{
	m_Transform->SetMeshSize(MeshSize);
}

void CSceneComponent::SetMeshSize(const Vector2& MeshSize)
{
	m_Transform->SetMeshSize(MeshSize);
}

void CSceneComponent::SetMeshSize(float x, float y, float z)
{
	m_Transform->SetMeshSize(x, y, z);
}

void CSceneComponent::SetMeshSize(float x, float y)
{
	m_Transform->SetMeshSize(x, y);
}

void CSceneComponent::SetWorldScale(const Vector3& Scale)
{
	m_Transform->SetWorldScale(Scale);
}

void CSceneComponent::SetWorldScale(const Vector2& Scale)
{
	m_Transform->SetWorldScale(Scale);
}

void CSceneComponent::SetWorldScale(float x, float y)
{
	m_Transform->SetWorldScale(x, y);
}

void CSceneComponent::SetWorldScale(float x, float y, float z)
{
	m_Transform->SetWorldScale(x, y, z);
}

void CSceneComponent::SetWorldScaleX(float x)
{
	m_Transform->SetWorldScaleX(x);
}

void CSceneComponent::SetWorldScaleY(float y)
{
	m_Transform->SetWorldScaleY(y);
}

void CSceneComponent::SetWorldScaleZ(float z)
{
	m_Transform->SetWorldScaleZ(z);
}

void CSceneComponent::SetWorldRotation(const Vector3& Rot)
{
	m_Transform->SetWorldRotation(Rot);
}

void CSceneComponent::SetWorldRotation(const Vector2& Rot)
{
	m_Transform->SetWorldRotation(Rot);
}

void CSceneComponent::SetWorldRotation(float x, float y)
{
	m_Transform->SetWorldRotation(x, y);
}

void CSceneComponent::SetWorldRotation(float x, float y, float z)
{
	m_Transform->SetWorldRotation(x, y, z);
}

void CSceneComponent::SetWorldRotationX(float x)
{
	m_Transform->SetWorldRotationX(x);
}

void CSceneComponent::SetWorldRotationY(float y)
{
	m_Transform->SetWorldRotationY(y);
}

void CSceneComponent::SetWorldRotationZ(float z)
{
	m_Transform->SetWorldRotationZ(z);
}

void CSceneComponent::SetWorldPosition(const Vector3& Pos)
{
	m_Transform->SetWorldPosition(Pos);
}

void CSceneComponent::SetWorldPosition(const Vector2& Pos)
{
	m_Transform->SetWorldPosition(Pos);
}

void CSceneComponent::SetWorldPosition(float x, float y)
{
	m_Transform->SetWorldPosition(x, y);
}

void CSceneComponent::SetWorldPosition(float x, float y, float z)
{
	m_Transform->SetWorldPosition(x, y, z);
}

void CSceneComponent::SetWorldPositionX(float x)
{
	m_Transform->SetWorldPositionX(x);
}

void CSceneComponent::SetWorldPositionY(float y)
{
	m_Transform->SetWorldPositionY(y);
}

void CSceneComponent::SetWorldPositionZ(float z)
{
	m_Transform->SetWorldPositionZ(z);
}

void CSceneComponent::SetOffset(const Vector3& Offset)
{
	m_Transform->SetOffset(Offset);
}

void CSceneComponent::SetOffset(const Vector2& Offset)
{
	m_Transform->SetOffset(Offset);
}

void CSceneComponent::SetOffsetX(float x)
{
	m_Transform->SetOffsetX(x);
}

void CSceneComponent::SetOffsetY(float y)
{
	m_Transform->SetOffsetY(y);
}

void CSceneComponent::SetOffsetZ(float z)
{
	m_Transform->SetOffsetZ(z);
}

void CSceneComponent::AddWorldScale(const Vector3& Scale)
{
	m_Transform->AddWorldScale(Scale);
}

void CSceneComponent::AddWorldScale(const Vector2& Scale)
{
	m_Transform->AddWorldScale(Scale);
}

void CSceneComponent::AddWorldScale(float x, float y)
{
	m_Transform->AddWorldScale(x, y);
}

void CSceneComponent::AddWorldScale(float x, float y, float z)
{
	m_Transform->AddWorldScale(x, y, z);
}

void CSceneComponent::AddWorldScaleX(float x)
{
	m_Transform->AddWorldScaleX(x);
}

void CSceneComponent::AddWorldScaleY(float y)
{
	m_Transform->AddWorldScaleY(y);
}

void CSceneComponent::AddWorldScaleZ(float z)
{
	m_Transform->AddWorldScaleZ(z);
}

void CSceneComponent::AddWorldRotation(const Vector3& Rot)
{
	m_Transform->AddWorldRotation(Rot);
}

void CSceneComponent::AddWorldRotation(const Vector2& Rot)
{
	m_Transform->AddWorldRotation(Rot);
}

void CSceneComponent::AddWorldRotation(float x, float y)
{
	m_Transform->AddWorldRotation(x, y);
}

void CSceneComponent::AddWorldRotation(float x, float y, float z)
{
	m_Transform->AddWorldRotation(x, y, z);
}

void CSceneComponent::AddWorldRotationX(float x)
{
	m_Transform->AddWorldRotationX(x);
}

void CSceneComponent::AddWorldRotationY(float y)
{
	m_Transform->AddWorldRotationY(y);
}

void CSceneComponent::AddWorldRotationZ(float z)
{
	m_Transform->AddWorldRotationZ(z);
}

void CSceneComponent::AddWorldPosition(const Vector3& Pos)
{
	m_Transform->AddWorldPosition(Pos);
}

void CSceneComponent::AddWorldPosition(const Vector2& Pos)
{
	m_Transform->AddWorldPosition(Pos);
}

void CSceneComponent::AddWorldPosition(float x, float y)
{
	m_Transform->AddWorldPosition(x, y);
}

void CSceneComponent::AddWorldPosition(float x, float y, float z)
{
	m_Transform->AddWorldPosition(x, y, z);
}

void CSceneComponent::AddWorldPositionX(float x)
{
	m_Transform->AddWorldPositionX(x);
}

void CSceneComponent::AddWorldPositionY(float y)
{
	m_Transform->AddWorldPositionY(y);
}

void CSceneComponent::AddWorldPositionZ(float z)
{
	m_Transform->AddWorldPositionZ(z);
}

void CSceneComponent::AddOffset(const Vector3& Offset)
{
	m_Transform->AddOffset(Offset);
}

void CSceneComponent::AddOffset(const Vector2& Offset)
{
	m_Transform->AddOffset(Offset);
}

void CSceneComponent::AddOffsetX(float x)
{
	m_Transform->AddOffsetX(x);
}

void CSceneComponent::AddOffsetY(float y)
{
	m_Transform->AddOffsetY(y);
}

void CSceneComponent::AddOffsetZ(float z)
{
	m_Transform->AddOffsetZ(z);
}
