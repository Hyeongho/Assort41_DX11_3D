#include "BossMonster.h"

CBossMonster::CBossMonster()
{
}

CBossMonster::CBossMonster(const CBossMonster& Obj)
    : CGameObject(Obj)
{
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
