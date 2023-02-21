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

    void LoadSpongebob();
    void LoadPatrick();
    void LoadSandy();

    void LoadPlayerSound();
    void LoadSound();

    void LoadBuildings();
    void LoadRoboSponge();
    void LoadKingJellyfish();
    void LoadJellyfish();
    void LoadMrKrabs();
    void LoadSquidward();

    void LoadEnemies();

    void LoadParticle();
};

