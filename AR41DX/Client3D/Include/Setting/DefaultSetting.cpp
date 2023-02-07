#include "DefaultSetting.h"
#include "../GameObject/Player.h"
#include "../GameObject/Patrick.h"
#include "../GameObject/Sandy.h"
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
    //CScene::CreateObjectCDO<CPlayer>("Player");
    CScene::CreateObjectCDO<CPatrick>("Patrick");
    CScene::CreateObjectCDO<CSandy>("Sandy");
}

void CDefaultSetting::LoadResource()
{
    LoadPatrick();
    LoadSandy();

    /*
   CResourceManager::GetInst()->AddSocket("PlayerSkeleton",
        "bone11", "Weapon", Vector3(0.f, -60.f, 0.f), Vector3(0.f, 0.f, 90.f));



    CResourceManager::GetInst()->LoadAnimationSequence("PlayerIdle",
        TEXT("sqc_Idle.sqc"), MESH_PATH);

    CResourceManager::GetInst()->LoadAnimationSequence("PlayerWalk",
        TEXT("sqc_Battle_Walk.sqc"), MESH_PATH);

    CResourceManager::GetInst()->LoadAnimationSequence("PlayerAttack",
        TEXT("sqc_Battle_OverHeadSlash.sqc"), MESH_PATH);*/
}

void CDefaultSetting::SetInput()
{
    // 캐릭터 키 하나로 통일 필요! - Move, LButton, RButton, Space
    // Patrick
    CInput::GetInst()->AddBindKey("PatrickMoveRight", 'D');
    CInput::GetInst()->AddBindKey("PatrickMoveLeft", 'A');

    CInput::GetInst()->AddBindKey("PatrickMoveFront", 'W');
    CInput::GetInst()->AddBindKey("PatrickMoveBack", 'S');

    CInput::GetInst()->AddBindKey("PatrickJump", VK_SPACE);

    CInput::GetInst()->AddBindKey("PatrickLButton", VK_LBUTTON);

    CInput::GetInst()->AddBindKey("RButton", VK_RBUTTON);

    CInput::GetInst()->AddBindKey("Push", VK_LBUTTON);
    CInput::GetInst()->AddBindKey("SlamDown", VK_RBUTTON);
    CInput::GetInst()->AddBindKey("PickUp", VK_RBUTTON);
    CInput::GetInst()->AddBindKey("Throw", VK_RBUTTON);
    CInput::GetInst()->AddBindKey("CameraRotationKey", VK_RBUTTON);


    // Sandy
    CInput::GetInst()->AddBindKey("SandyMoveRight", 'D');
    CInput::GetInst()->AddBindKey("SandyMoveLeft", 'A');

    CInput::GetInst()->AddBindKey("SandyMoveFront", 'W');
    CInput::GetInst()->AddBindKey("SandyMoveBack", 'S');

    CInput::GetInst()->AddBindKey("SandyJump", VK_SPACE);

    CInput::GetInst()->AddBindKey("RButton", VK_RBUTTON);

    CInput::GetInst()->AddBindKey("MClick", VK_MBUTTON);
    CInput::GetInst()->AddBindKey("Del", VK_DELETE);
    CInput::GetInst()->AddBindKey("ChangePos", VK_F1);

    CInput::GetInst()->AddBindKey("SandyLButton", VK_LBUTTON);

    // Mouse Ű ���
    CInput::GetInst()->AddBindKey("LButton", VK_LBUTTON);
    CInput::GetInst()->AddBindKey("RButton", VK_RBUTTON);


    // Arrow Ű ���
    CInput::GetInst()->AddBindKey("UArrow", VK_UP);
    CInput::GetInst()->AddBindKey("DArrow", VK_DOWN);
    CInput::GetInst()->AddBindKey("LArrow", VK_LEFT);
    CInput::GetInst()->AddBindKey("RArrow", VK_RIGHT);

    CInput::GetInst()->AddBindKey("W", 'W');
    CInput::GetInst()->AddBindKey("S", 'S');
    CInput::GetInst()->AddBindKey("A", 'A');
    CInput::GetInst()->AddBindKey("D", 'D');


    // ETC Ű ���
    CInput::GetInst()->AddBindKey("Esc", VK_ESCAPE);
    CInput::GetInst()->AddBindKey("Enter", VK_RETURN);
    CInput::GetInst()->AddBindKey("Space", VK_SPACE);

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

void CDefaultSetting::LoadPatrick()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Patrick", TEXT("Patrick/Patrick11.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "PatrickSkeleton", TEXT("Patrick/Patrick11.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Patrick", "PatrickSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Idle", TEXT("Patrick/Patrick_Idle.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Walk", TEXT("Patrick/Patrick_Walk.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Run", TEXT("Patrick/Patrick_Run.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_DoubleJump", TEXT("Patrick/Patrick_DoubleJump.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Jump", TEXT("Patrick/Patrick_JumpUp.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_PickUp", TEXT("Patrick/Patrick_PickUp.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_PickUpIdle", TEXT("Patrick/Patrick_PickUpIdle.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_PickUpWalk", TEXT("Patrick/Patrick_PickUpWalk.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Throw", TEXT("Patrick/Patrick_Throw.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Slam", TEXT("Patrick/Patrick_Slam.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_BellyAttack", TEXT("Patrick/Patrick_BellyAttack.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Death", TEXT("Patrick/Patrick_Death.fbx"), MESH_PATH);
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

    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Idle", TEXT("Sandy/Sandy_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Walk", TEXT("Sandy/Sandy_Walk.sqc"), MESH_PATH);
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
