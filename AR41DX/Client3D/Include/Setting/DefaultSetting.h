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

    // Objects
    void LoadBuildings();
    void LoadJellyfishFields();

    // Boss
    void LoadRoboSponge();
    void LoadKingJellyfish();

    // Enemies
    void LoadEnemies();
    void LoadJellyfish();
    
    // NPC
    void LoadMrKrabs();
    void LoadSquidward();

};

