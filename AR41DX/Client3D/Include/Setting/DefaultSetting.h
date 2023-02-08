#pragma once
#include "EngineSetting.h"
class CDefaultSetting :
    public CEngineSetting
{
public:
    CDefaultSetting();
    virtual ~CDefaultSetting();

public:
    virtual void Init();

private:
    void CreateCDO();
    void LoadResource();
    void SetInput();
    void SetCollision();

    void LoadPatrick();
    void LoadSandy();
    void LoadRoboSponge();
    void LoadKingJellyfish();

    void LoadJellyfish();
};

