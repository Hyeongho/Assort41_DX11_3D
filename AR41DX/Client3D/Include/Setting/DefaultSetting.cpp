﻿#include "DefaultSetting.h"
#include "../GameObject/Player.h"
//#include "../GameObject/Patrick.h"
//#include "../GameObject/Sandy.h"
//#include "../GameObject/BikiniBottomBuildings.h"
//#include "../GameObject/KingJellyfish.h"
//#include "../GameObject/Jellyfish.h"
//#include "../GameObject/Monster.h"
//#include "../GameObject/Bullet.h"
//#include "../UI/StartSceneUI.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "CollisionManager.h"

CDefaultSetting::CDefaultSetting()
{
}

CDefaultSetting::~CDefaultSetting()
{
}

void CDefaultSetting::Init()
{
    LoadResource();

    CreateCDO();

    SetInput();

    SetCollision();
}

void CDefaultSetting::CreateCDO()
{
    CScene::CreateObjectCDO<CPlayer>("Player");

    //CScene::CreateObjectCDO<CBikiniBottomBuildings>("BikiniBottomBuildings");

    //CScene::CreateObjectCDO<CPatrick>("Patrick");

    //CScene::CreateObjectCDO<CSandy>("Sandy");

    //CScene::CreateObjectCDO<CKingJellyfish>("KingJellyfish");

    //CScene::CreateObjectCDO<CJellyfish>("Jellyfish");

}

void CDefaultSetting::LoadResource()
{
    LoadSpongebob();
    LoadPatrick();
    LoadSandy();

    LoadBuildings();
    LoadKingJellyfish();
    LoadJellyfish();
}

void CDefaultSetting::SetInput()
{
    //겹치는 키
    CInput::GetInst()->AddBindKey("W", 'W');
    CInput::GetInst()->AddBindKey("S", 'S');
    CInput::GetInst()->AddBindKey("D", 'D');
    CInput::GetInst()->AddBindKey("A", 'A');
    CInput::GetInst()->AddBindKey("Space", VK_SPACE);
    CInput::GetInst()->AddBindKey("LClick", VK_LBUTTON);
    CInput::GetInst()->AddBindKey("RClick", VK_RBUTTON);

    //안겹치는 키
    CInput::GetInst()->AddBindKey("UArrow", VK_UP);
    CInput::GetInst()->AddBindKey("DArrow", VK_DOWN);
    CInput::GetInst()->AddBindKey("LArrow", VK_RIGHT);
    CInput::GetInst()->AddBindKey("RArrow", VK_LEFT);

    CInput::GetInst()->AddBindKey("E", 'E');
    CInput::GetInst()->AddBindKey("Q", 'Q');
    CInput::GetInst()->AddBindKey("F", 'F');
    CInput::GetInst()->AddBindKey("Esc", VK_ESCAPE);
    CInput::GetInst()->AddBindKey("Tab", VK_TAB);

    CInput::GetInst()->AddBindKey("F1", VK_F1);
    CInput::GetInst()->AddBindKey("F2", VK_F2);
    CInput::GetInst()->AddBindKey("F3", VK_F3);

    //editor
    CInput::GetInst()->AddBindKey("MClick", VK_MBUTTON);
    CInput::GetInst()->AddBindKey("Del", VK_DELETE);
    CInput::GetInst()->AddBindKey("ChangePos", VK_F4);

}

void CDefaultSetting::SetCollision()
{
    CCollisionManager::GetInst()->CreateChannel("Player", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->CreateChannel("PlayerAttack", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->CreateChannel("Monster", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->CreateChannel("MonsterAttack", ECollision_Interaction::Collision);

    CCollisionManager::GetInst()->CreateProfile("Player", "Player", true);
    CCollisionManager::GetInst()->CreateProfile("PlayerAttack", "PlayerAttack", true);
    CCollisionManager::GetInst()->CreateProfile("Monster", "Monster", true);
    CCollisionManager::GetInst()->CreateProfile("MonsterAttack", "MonsterAttack", true);

    CCollisionManager::GetInst()->SetCollisionInteraction("Player", "PlayerAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("Player", "Player", ECollision_Interaction::Ignore);

    CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "Player", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "MonsterAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "PlayerAttack", ECollision_Interaction::Ignore);

    CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "MonsterAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "Monster", ECollision_Interaction::Ignore);

    CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "Monster", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "MonsterAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "PlayerAttack", ECollision_Interaction::Ignore);
}

void CDefaultSetting::LoadSpongebob()
{
    CResourceManager* resourceManager = CResourceManager::GetInst();
    resourceManager->LoadMesh(nullptr, MeshType::Animation, "Spongebob", TEXT("Spongebob\\Spongebob_mesh.msh"), MESH_PATH);
    resourceManager->LoadSkeleton(nullptr, "SpongebobSkeleton", TEXT("Spongebob\\Spongebob_mesh.bne"), MESH_PATH);
    resourceManager->SetMeshSkeleton("Spongebob", "SpongebobSkeleton");
    //resourceManager->AddSocket("SpongebobSkeleton", "bone11", "Weapon");
    resourceManager->LoadAnimationSequence("Spongebob_Idle", TEXT("Spongebob\\Anim_Spongebob_Idle.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_Walk", TEXT("Spongebob\\Anim_Spongebob_Walk.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_Attack", TEXT("Spongebob\\Anim_Spongebob_BubbleSpin.sqc"), MESH_PATH);
}

void CDefaultSetting::LoadPatrick()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Patrick", TEXT("Patrick/Patrick11.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "PatrickSkeleton", TEXT("Patrick/Patrick11.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Patrick", "PatrickSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Idle", TEXT("Patrick/Patrick_Idle.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Walk", TEXT("Patrick/Patrick_Walk.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Run", TEXT("Patrick/Patrick_Run.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_DoubleJump", TEXT("Patrick/Patrick_DoubleJump.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Jump", TEXT("Patrick/Patrick_JumpUp.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_PickUp", TEXT("Patrick/Patrick_PickUp.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_PickUpIdle", TEXT("Patrick/Patrick_PickUpIdle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_PickUpWalk", TEXT("Patrick/Patrick_PickUpWalk.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Throw", TEXT("Patrick/Patrick_Throw.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Slam", TEXT("Patrick/Patrick_Slam.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_BellyAttack", TEXT("Patrick/Patrick_BellyAttack.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Death", TEXT("Patrick/Patrick_Death.sqc"), MESH_PATH);
}

void CDefaultSetting::LoadSandy()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Sandy", TEXT("Sandy/Sandy_Idle.fbx"), MESH_PATH);
    //CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Sandy_Walk", TEXT("Sandy/Sandy_Walk.fbx"), MESH_PATH);
    /*CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Sandy_Walk", TEXT("Sandy/Sandy_Walk.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Sandy_Run", TEXT("Sandy/Sandy_Run.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Sandy_JumpDW", TEXT("Sandy/Sandy_JumpDW.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Sandy_JumpUp", TEXT("Sandy/Sandy_JumpUp.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Sandy_Jump_Landing_NonAdditive", TEXT("Sandy/Sandy_Jump_Landing_NonAdditive.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Sandy_DoubleJump", TEXT("Sandy/Sandy_DoubleJump.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Sandy_Karate_Chop", TEXT("Sandy/Sandy_Karate_Chop.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Sandy_Karate_Kick", TEXT("Sandy/Sandy_Karate_Kick.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Sandy_Lasso_Start", TEXT("Sandy/Sandy_Lasso_Start.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Sandy_Death", TEXT("Sandy/Sandy_Death.fbx"), MESH_PATH);*/

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "SandySkeleton", TEXT("Sandy/Sandy_Idle.bne"), MESH_PATH);

    CResourceManager::GetInst()->SetMeshSkeleton("Sandy", "SandySkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Idle", TEXT("Sandy/Sandy_Idle.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Walk", TEXT("Sandy/Sandy_Walk.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Run", TEXT("Sandy/Sandy_Run.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_JumpDW", TEXT("Sandy/Sandy_JumpDW.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_JumpUp", TEXT("Sandy/Sandy_JumpUp.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Jump_Landing_NonAdditive", TEXT("Sandy/Sandy_Jump_Landing_NonAdditive.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_DoubleJump", TEXT("Sandy/Sandy_DoubleJump.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Karate_Chop", TEXT("Sandy/Sandy_Karate_Chop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Karate_Kick", TEXT("Sandy/Sandy_Karate_Kick.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Lasso_Start", TEXT("Sandy/Sandy_Lasso_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Death", TEXT("Sandy/Sandy_Death.sqc"), MESH_PATH);
}

void CDefaultSetting::LoadBuildings()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "BikiniBottomBuildings", TEXT("Buildings/BikiniBottom/BikiniBottomBuildings.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "BikiniBottomBuildingsSkeleton", TEXT("Buildings/BikiniBottom/BikiniBottomBuildings.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("BikiniBottomBuildings", "BikiniBottomBuildingsSkeleton");


}

void CDefaultSetting::LoadRoboSponge()
{
}

void CDefaultSetting::LoadKingJellyfish()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "KingJellyfish", TEXT("KingJellyfish/KingJellyfish.fbx"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "KingJellyfishSkeleton", TEXT("KingJellyfish/KingJellyfish.bne"), MESH_PATH);

    CResourceManager::GetInst()->SetMeshSkeleton("KingJellyfish", "KingJellyfishSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("KingJellyfish_Idle", TEXT("KingJellyfish/bossJellyKing_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("KingJellyfish_Angry", TEXT("KingJellyfish/bossJellyKing_angry.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("KingJellyfish_Attack", TEXT("KingJellyfish/bossJellyKing_attack.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("KingJellyfish_Damage", TEXT("KingJellyfish/bossJellyKing_damage.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("KingJellyfish_OnGround", TEXT("KingJellyfish/bossJellyKing_onGround.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("KingJellyfish_SpawnJellyfish", TEXT("KingJellyfish/bossJellyKing_spawnJellyfish.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("KingJellyfish_Victory", TEXT("KingJellyfish/bossJellyKing_victory.sqc"), MESH_PATH);
}

void CDefaultSetting::LoadJellyfish()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Jellyfish", TEXT("Jellyfish/Jellyfish.fbx"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "JellyfishSkeleton", TEXT("Jellyfish/Jellyfish.bne"), MESH_PATH);

    CResourceManager::GetInst()->SetMeshSkeleton("Jellyfish", "JellyfishSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Jellyfish_Attack", TEXT("Jellyfish/Jellyfish_attack.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Jellyfish_Death", TEXT("Jellyfish/Jellyfish_death.sqc"), MESH_PATH);
}
