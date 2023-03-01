#include "Dead.h"
#include "../GameObject/Fodder.h"
#include "../GameObject/Hammer.h"
#include "Scene/Scene.h"

CDead::CDead()
{
}

CDead::~CDead()
{
}

bool CDead::Run()
{
    return true;
}

bool CDead::Run(CGameObject* Object)
{
    if (Object->GetObjectTypeName() == "Fodder")
    {
    }

    if (Object->GetObjectTypeName() == "Hammer")
    {
    }

    return true;
}
