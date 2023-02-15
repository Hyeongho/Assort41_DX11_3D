#include "DefaultSetting.h"
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

    LoadMrKrabs();
    LoadSquidward();

    LoadRoboSponge();
    LoadKingJellyfish();
    LoadJellyfish();
    
    
    LoadBuildings();
}

void CDefaultSetting::SetInput()
{
    // 캐릭터 키 하나로 통일 필요! - Move, LButton, RButton, Space

    CInput::GetInst()->AddBindKey("MoveRight", 'D');
    CInput::GetInst()->AddBindKey("MoveLeft", 'A');

    CInput::GetInst()->AddBindKey("MoveFront", 'W');
    CInput::GetInst()->AddBindKey("MoveBack", 'S');

    CInput::GetInst()->AddBindKey("Jump", VK_SPACE);

    CInput::GetInst()->AddBindKey("LButton", VK_LBUTTON);
    CInput::GetInst()->AddBindKey("RButton", VK_RBUTTON);

    CInput::GetInst()->AddBindKey("E", 'E');
    CInput::GetInst()->AddBindKey("Q", 'Q');
    CInput::GetInst()->AddBindKey("F", 'F');

    CInput::GetInst()->AddBindKey("Spongebob", VK_F1);
    CInput::GetInst()->AddBindKey("Patrick", VK_F2);
    CInput::GetInst()->AddBindKey("Sandy", VK_F3);

    // Num, FNum
    CInput::GetInst()->AddBindKey("1", '1');
    CInput::GetInst()->AddBindKey("2", '2');
    CInput::GetInst()->AddBindKey("3", '3');
    CInput::GetInst()->AddBindKey("4", '4');
    CInput::GetInst()->AddBindKey("5", '5');
    CInput::GetInst()->AddBindKey("6", '6');
    CInput::GetInst()->AddBindKey("7", '7');
    CInput::GetInst()->AddBindKey("8", '8');
    CInput::GetInst()->AddBindKey("9", '9');
    CInput::GetInst()->AddBindKey("0", '0');
    
    CInput::GetInst()->AddBindKey("F1", VK_F1);
    CInput::GetInst()->AddBindKey("F2", VK_F2);
    CInput::GetInst()->AddBindKey("F3", VK_F3);
    CInput::GetInst()->AddBindKey("F4", VK_F4);
    CInput::GetInst()->AddBindKey("F5", VK_F5);
    CInput::GetInst()->AddBindKey("F6", VK_F6);
    CInput::GetInst()->AddBindKey("F7", VK_F7);
    CInput::GetInst()->AddBindKey("F8", VK_F8);
    CInput::GetInst()->AddBindKey("F9", VK_F9);
    CInput::GetInst()->AddBindKey("F10", VK_F10);
    CInput::GetInst()->AddBindKey("F11", VK_F11);
    CInput::GetInst()->AddBindKey("F12", VK_F12);

    // Arrow
    CInput::GetInst()->AddBindKey("UArrow", VK_UP);
    CInput::GetInst()->AddBindKey("DArrow", VK_DOWN);
    CInput::GetInst()->AddBindKey("LArrow", VK_LEFT);
    CInput::GetInst()->AddBindKey("RArrow", VK_RIGHT);

    // ETC,  UI
    CInput::GetInst()->AddBindKey("Esc", VK_ESCAPE);
    CInput::GetInst()->AddBindKey("IngameUI", VK_TAB);
    CInput::GetInst()->AddBindKey("Enter", VK_RETURN);

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
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Patrick", TEXT("Patrick/Patrick11.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "PatrickSkeleton", TEXT("Patrick/Patrick11.bne"), MESH_PATH);

    CResourceManager::GetInst()->SetMeshSkeleton("Patrick", "PatrickSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Idle", TEXT("Patrick/Patrick_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Walk", TEXT("Patrick/Patrick_Walk.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Run", TEXT("Patrick/Patrick_Run.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_DoubleJump", TEXT("Patrick/Patrick_DoubleJump.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Jump", TEXT("Patrick/Patrick_JumpUp.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_PickUp", TEXT("Patrick/Patrick_PickUp.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_PickUpIdle", TEXT("Patrick/Patrick_PickUpIdle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_PickUpWalk", TEXT("Patrick/Patrick_PickUpWalk.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Throw", TEXT("Patrick/Patrick_Throw.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Slam", TEXT("Patrick/Patrick_Slam.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_BellyAttack", TEXT("Patrick/Patrick_BellyAttack.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patrick_Death", TEXT("Patrick/Patrick_Death.sqc"), MESH_PATH);
}

void CDefaultSetting::LoadSandy()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Sandy", TEXT("Sandy/Sandy_Idle.msh"), MESH_PATH);

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

void CDefaultSetting::LoadBuildings()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "BikiniBottomBuildings", TEXT("Buildings/BikiniBottom/BikiniBottomBuildings.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "BikiniBottomBuildingsSkeleton", TEXT("Buildings/BikiniBottom/BikiniBottomBuildings.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("BikiniBottomBuildings", "BikiniBottomBuildingsSkeleton");


}

void CDefaultSetting::LoadRoboSponge()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Robo_Sponge", TEXT("Robo_Sponge/Robo_Sponge.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "Robo_Sponge_Skeleton", TEXT("Robo_Sponge/Robo_Sponge.bne"), MESH_PATH);

    CResourceManager::GetInst()->SetMeshSkeleton("Robo_Sponge", "Robo_Sponge_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Horiz_L", TEXT("Robo_Sponge/Robo_Sponge_Attack_Horiz_L.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Horiz_L_Pose", TEXT("Robo_Sponge/Robo_Sponge_Attack_Horiz_L_Pose.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Horiz_R", TEXT("Robo_Sponge/Robo_Sponge_Attack_Horiz_R.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Horiz_R_Pose", TEXT("Robo_Sponge/Robo_Sponge_Attack_Horiz_R_Pose.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Vertic_L_Loop", TEXT("Robo_Sponge/Robo_Sponge_Attack_Vertic_L_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Vertic_L_Start", TEXT("Robo_Sponge/Robo_Sponge_Attack_Vertic_L_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Vertic_R_Loop", TEXT("Robo_Sponge/Robo_Sponge_Attack_Vertic_R_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Vertic_R_Start", TEXT("Robo_Sponge/Robo_Sponge_Attack_Vertic_R_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Words_Loop", TEXT("Robo_Sponge/Robo_Sponge_Attack_Words_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Words_Start", TEXT("Robo_Sponge/Robo_Sponge_Attack_Words_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Hit1", TEXT("Robo_Sponge/Robo_Sponge_Hit1.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Hit2", TEXT("Robo_Sponge/Robo_Sponge_Hit2.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Hit2_Pose", TEXT("Robo_Sponge/Robo_Sponge_Hit2_Pose.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Idle", TEXT("Robo_Sponge/Robo_Sponge_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Vertic_L_Hold", TEXT("Robo_Sponge/Robo_Sponge_Vertic_L_Hold.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Vertic_R_Hold", TEXT("Robo_Sponge/Robo_Sponge_Vertic_R_Hold.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Victory", TEXT("Robo_Sponge/Robo_Sponge_Victory.sqc"), MESH_PATH);

    //Robo_Sponge_Attack_Vertic_L_Loop
}

void CDefaultSetting::LoadKingJellyfish()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "KingJellyfish", TEXT("KingJellyfish/KingJellyfish.msh"), MESH_PATH);

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
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Jellyfish", TEXT("Jellyfish/Jellyfish.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "JellyfishSkeleton", TEXT("Jellyfish/Jellyfish.bne"), MESH_PATH);

    CResourceManager::GetInst()->SetMeshSkeleton("Jellyfish", "JellyfishSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Jellyfish_Attack", TEXT("Jellyfish/Jellyfish_attack.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Jellyfish_Death", TEXT("Jellyfish/Jellyfish_death.sqc"), MESH_PATH);
}

void CDefaultSetting::LoadMrKrabs()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "MrKrabs", TEXT("MrKrabs/MrKrabs.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "MrKrabs_Skeleton", TEXT("MrKrabs/MrKrabs.bne"), MESH_PATH);

    CResourceManager::GetInst()->SetMeshSkeleton("MrKrabs", "MrKrabs_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("MrKrabs_Angry_Loop", TEXT("MrKrabs/MrKrabs_Angry_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("MrKrabs_Angry_Start", TEXT("MrKrabs/MrKrabs_Angry_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("MrKrabs_Deceptive_Loop", TEXT("MrKrabs/MrKrabs_Deceptive_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("MrKrabs_Deceptive_Start", TEXT("MrKrabs/MrKrabs_Deceptive_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("MrKrabs_Greedy_Loop", TEXT("MrKrabs/MrKrabs_Greedy_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("MrKrabs_Greedy_Start", TEXT("MrKrabs/MrKrabs_Greedy_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("MrKrabs_Idle", TEXT("MrKrabs/MrKrabs_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("MrKrabs_Laughing", TEXT("MrKrabs/MrKrabs_Laughing.sqc"), MESH_PATH);
}

void CDefaultSetting::LoadSquidward()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Squidward", TEXT("Squidward/Squidward.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "Squidward_Skeleton", TEXT("Squidward/Squidward.bne"), MESH_PATH);

    CResourceManager::GetInst()->SetMeshSkeleton("Squidward", "Squidward_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Angry_Loop", TEXT("Squidward/Squidward_Angry_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Angry_Start", TEXT("Squidward/Squidward_Angry_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Annoyed_Loop", TEXT("Squidward/Squidward_Annoyed_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Annoyed_Start", TEXT("Squidward/Squidward_Annoyed_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Happy_Loop", TEXT("Squidward/Squidward_Happy_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Happy_Start", TEXT("Squidward/Squidward_Happy_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Hurt_Loop", TEXT("Squidward/Squidward_Hurt_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Hurt_Start", TEXT("Squidward/Squidward_Hurt_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Idle", TEXT("Squidward/Squidward_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Sarcastic_Loop", TEXT("Squidward/Squidward_Sarcastic_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Sarcastic_Start", TEXT("Squidward/Squidward_Sarcastic_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Talk", TEXT("Squidward/Squidward_Talk.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Talk_Idle", TEXT("Squidward/Squidward_Talk_Idle.sqc"), MESH_PATH);
}
