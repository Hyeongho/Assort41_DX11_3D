#include "BossMonster.h"

CBossMonster::CBossMonster()
{
    SetTypeID<CBossMonster>();

    m_ObjectTypeName = "BossMonster";
}

CBossMonster::CBossMonster(const CBossMonster& Obj)
    : CGameObject(Obj)
{
    m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
    m_Animation = Obj.m_Animation;

}

CBossMonster::~CBossMonster()
{
}

void CBossMonster::Start()
{
    CGameObject::Start();
}

bool CBossMonster::Init()
{
    CGameObject::Init();


    return true;
}

void CBossMonster::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CBossMonster::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CBossMonster* CBossMonster::Clone() const
{
    return new CBossMonster(*this);
}

void CBossMonster::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CBossMonster::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CBossMonster::SetBossData_MaxHp(int MaxHp)
{
}

void CBossMonster::SetBossData(const BossData& BossData)
{
}

void CBossMonster::CreateBossUI()
{
}

void CBossMonster::BossDamaged(int Damage)
{
}
