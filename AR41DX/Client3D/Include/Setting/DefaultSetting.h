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

    // Sound
    void LoadPlayerSound();
    void LoadSound();

    // Player
    void LoadSpongebob();
    void LoadPatrick();
    void LoadSandy();

    // Monster
    void LoadRoboSponge();
    void LoadKingJellyfish();
    void LoadJellyfish();
    void LoadTikis();
    void LoadRobots();

    // NPC
    void LoadMrKrabs();
    void LoadSquidward();
    void LoadPatric_Npc();
    void LoadTaxi();


    // Objects
    void LoadBuildings();
    void LoadJellyfishFieldsObj();

    void LoadParticle();
};

