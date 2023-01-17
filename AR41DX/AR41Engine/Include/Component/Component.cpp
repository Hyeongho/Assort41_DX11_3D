
#include "Component.h"

std::unordered_map<std::string, CComponent*> CComponent::m_mapComponent;

CComponent::CComponent()	:
	m_Scene(nullptr),
	m_Owner(nullptr),
	m_SerialNumber(0)
{
	SetTypeID<CComponent>();

	m_ComponentTypeName = "Component";
}

CComponent::CComponent(const CComponent& Obj)	:
	CRef(Obj)
{
	m_ComponentType = Obj.m_ComponentType;
	m_ComponentTypeName = Obj.m_ComponentTypeName;
	m_SerialNumber = Obj.m_SerialNumber;
}

CComponent::~CComponent()
{
}

void CComponent::Destroy()
{
	CRef::Destroy();
}

void CComponent::Start()
{
}

bool CComponent::Init()
{
	return true;
}

void CComponent::Update(float DeltaTime)
{
}

void CComponent::PostUpdate(float DeltaTime)
{
}

void CComponent::Render()
{
}

CComponent* CComponent::Clone() const
{
	return nullptr;
}

void CComponent::Save(FILE* File)
{
	CRef::Save(File);

	int	Length = (int)m_ComponentTypeName.length();

	fwrite(&Length, 4, 1, File);
	fwrite(m_ComponentTypeName.c_str(), 1, Length, File);

	fwrite(&m_ComponentType, sizeof(ComponentType), 1, File);
	fwrite(&m_SerialNumber, sizeof(int), 1, File);
}

void CComponent::Load(FILE* File)
{
	CRef::Load(File);

	int	Length = 0;
	char	TypeName[256] = {};

	fread(&Length, 4, 1, File);
	fread(TypeName, 1, Length, File);

	m_ComponentTypeName = TypeName;

	fread(&m_ComponentType, sizeof(ComponentType), 1, File);
	fread(&m_SerialNumber, sizeof(int), 1, File);
}
