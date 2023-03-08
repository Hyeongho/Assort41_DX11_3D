#include "DefaultSetting.h"
#include "../GameObject/Player.h"
#include "../GameObject/Weapon.h"
//#include "../GameObject/KingJellyfish.h"
//#include "../GameObject/Jellyfish.h"
#include "../GameObject/Npc/MrKrabs.h"
#include "../GameObject/Npc/Patric.h"
#include "../GameObject/Npc/Squidward.h"
#include "../GameObject/Npc/BusDriver.h"
#include "../GameObject/Bullet.h"
#include "../UI/PlayerUI.h"
#include "../UI/PauseUI.h"
#include "../UI/TitleSceneUI.h"
#include "../UI/DialogUI.h"
#include "../UI/InteractUI.h"
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
    CScene::CreateObjectCDO<CWeapon>("Weapon");
    CScene::CreateObjectCDO<CBullet>("Bullet");
    CScene::CreateObjectCDO<CMrKrabs>("MrKrabs");
    CScene::CreateObjectCDO<CSquidward>("Squidward");
    CScene::CreateObjectCDO<CBusDriver>("BusDriver");

    //CScene::CreateObjectCDO<CKingJellyfish>("CKingJellyfish");

    //CScene::CreateObjectCDO<CJellyfish>("Jellyfish");

    CScene::CreateUIWindowCDO<CPlayerUI>("PlayerUI");
    CScene::CreateUIWindowCDO<CPauseUI>("PauseUI");
    CScene::CreateUIWindowCDO<CTitleSceneUI>("TitleSceneUI");
    CScene::CreateUIWindowCDO<CDialogUI>("DialogUI");
    CScene::CreateUIWindowCDO<CInteractUI>("InteractUI");
}

void CDefaultSetting::LoadResource()
{
    LoadSpongebob();
    LoadPatrick();
    LoadSandy();

    LoadBuildings();
    LoadJellyfishFieldsObj();
    LoadRoboSponge();
    LoadKingJellyfish();
    LoadJellyfish();
    LoadTikis();

    LoadMrKrabs();
    LoadSquidward();
    LoadPatric_Npc();
    LoadTaxi();

    LoadSound();
    LoadPlayerSound();

    LoadParticle();
}

void CDefaultSetting::SetInput()
{
    // 캐릭터 키 하나로 통일 필요! - Move, LButton, RButton, Space

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
    CInput::GetInst()->AddBindKey("F4", VK_F4);     //에디터 위치변경함수
    CInput::GetInst()->AddBindKey("F5", VK_F5);     //에디터 일시정지함수
    CInput::GetInst()->AddBindKey("F6", VK_F6);
    CInput::GetInst()->AddBindKey("F7", VK_F7);
    CInput::GetInst()->AddBindKey("F8", VK_F8);
    CInput::GetInst()->AddBindKey("F9", VK_F9);
    CInput::GetInst()->AddBindKey("F10", VK_F10);
    CInput::GetInst()->AddBindKey("F11", VK_F11);
    CInput::GetInst()->AddBindKey("F12", VK_F12);

    // Arrow
    //겹치는 키
    CInput::GetInst()->AddBindKey("W", 'W');
    CInput::GetInst()->AddBindKey("S", 'S');
    CInput::GetInst()->AddBindKey("D", 'D');
    CInput::GetInst()->AddBindKey("A", 'A');
    CInput::GetInst()->AddBindKey("Space", VK_SPACE);
    CInput::GetInst()->AddBindKey("LClick", VK_LBUTTON);
    CInput::GetInst()->AddBindKey("RClick", VK_RBUTTON);

    //안겹치는 키
    CInput::GetInst()->AddBindKey("E", 'E');
    CInput::GetInst()->AddBindKey("Q", 'Q');
    CInput::GetInst()->AddBindKey("F", 'F');
    CInput::GetInst()->AddBindKey("Esc", VK_ESCAPE);
    CInput::GetInst()->AddBindKey("Tab", VK_TAB);

    //editor
    CInput::GetInst()->AddBindKey("UArrow", VK_UP);
    CInput::GetInst()->AddBindKey("DArrow", VK_DOWN);
    CInput::GetInst()->AddBindKey("LArrow", VK_LEFT);
    CInput::GetInst()->AddBindKey("RArrow", VK_RIGHT);

    CInput::GetInst()->AddBindKey("MClick", VK_MBUTTON);
    CInput::GetInst()->AddBindKey("Del", VK_DELETE);
    CInput::GetInst()->SetKeyCtrl("Del", true);
}

void CDefaultSetting::SetCollision()
{
    CCollisionManager::GetInst()->CreateChannel("Player", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->CreateChannel("PlayerAttack", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->CreateChannel("Monster", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->CreateChannel("MonsterAttack", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->CreateChannel("Wall", ECollision_Interaction::Collision);

    CCollisionManager::GetInst()->CreateProfile("Player", "Player", true);
    CCollisionManager::GetInst()->CreateProfile("PlayerAttack", "PlayerAttack", true);
    CCollisionManager::GetInst()->CreateProfile("Monster", "Monster", true);
    CCollisionManager::GetInst()->CreateProfile("MonsterAttack", "MonsterAttack", true);
    CCollisionManager::GetInst()->CreateProfile("Wall", "Wall", true);

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

    CCollisionManager::GetInst()->SetCollisionInteraction("Wall", "Wall", ECollision_Interaction::Ignore);
}

void CDefaultSetting::LoadSpongebob()
{
    CResourceManager* resourceManager = CResourceManager::GetInst();
    resourceManager->LoadMesh(nullptr, MeshType::Animation, "SpongebobWand", TEXT("Spongebob\\wand_bubble_wand.msh"));
    resourceManager->LoadSkeleton(nullptr, "SpongebobWandSkeleton", TEXT("Spongebob\\Spongebob_mesh.bne"), MESH_PATH);
    resourceManager->SetMeshSkeleton("SpongebobWand", "SpongebobWandSkeleton");

    resourceManager->LoadMesh(nullptr, MeshType::Animation, "Spongebob", TEXT("Spongebob\\Spongebob_mesh.msh"));
    resourceManager->LoadSkeleton(nullptr, "SpongebobSkeleton", TEXT("Spongebob\\Spongebob_mesh.bne"), MESH_PATH);
    resourceManager->SetMeshSkeleton("Spongebob", "SpongebobSkeleton");
    resourceManager->AddSocket("SpongebobSkeleton", "MiddleFinger3_R", "Weapon");
    resourceManager->LoadAnimationSequence("Spongebob_Idle", TEXT("Spongebob\\Anim_Spongebob_Idle.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_Walk", TEXT("Spongebob\\Anim_Spongebob_Walk.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_Attack", TEXT("Spongebob\\Anim_Spongebob_BubbleSpin.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_JumpDw", TEXT("Spongebob\\Anim_Spongebob_Jump_Dw.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_JumpUp", TEXT("Spongebob\\Anim_Spongebob_Jump_Up.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_BashStart", TEXT("Spongebob\\Anim_Spongebob_BubbleBash_Dw_Start.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_BashDw", TEXT("Spongebob\\Anim_Spongebob_Bubblebash_Dw.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_Bash", TEXT("Spongebob\\Anim_Spongebob_BubbleBash.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_Death", TEXT("Spongebob\\Anim_Spongebob_Death_02.fbx"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_Hit", TEXT("Spongebob\\Anim_Spongebob_Hit_01.fbx"), MESH_PATH);
//전용 모션
    resourceManager->LoadAnimationSequence("Spongebob_Bowl", TEXT("Spongebob\\Anim_Spongebob_Bubble_Bowl_Loop.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_BowlThrow", TEXT("Spongebob\\Anim_Spongebob_Bubble_Bowl_Throw.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_BounceStart", TEXT("Spongebob\\Anim_Spongebob_BubbleBounce_Start.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_BounceLoop", TEXT("Spongebob\\Anim_Spongebob_BubbleBounce_Loop.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_BounceLanding", TEXT("Spongebob\\Anim_Spongebob_BubbleBounce_Landing.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_MissileEnd", TEXT("Spongebob\\Anim_Spongebob_Cruise_Missile_Release_End.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_MissileLoop", TEXT("Spongebob\\Anim_Spongebob_Cruise_Missile_Release_Loop.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_MissileStart", TEXT("Spongebob\\Anim_Spongebob_Cruise_Missile_Release_Start.sqc"), MESH_PATH);

    resourceManager->LoadMesh(nullptr, MeshType::Animation, "SpongebobMissile", TEXT("Cruise_Missile\\SK_CruiseMissle.msh"));
    resourceManager->LoadSkeleton(nullptr, "SpongebobMissileSkeleton", TEXT("Cruise_Missile\\SK_CruiseMissle.bne"), MESH_PATH);
    resourceManager->SetMeshSkeleton("SpongebobMissile", "SpongebobMissileSkeleton");
    resourceManager->LoadAnimationSequence("SpongebobMissile_Idle", TEXT("Cruise_Missile\\Anim_Cruise_Missile_Idle.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("SpongebobMissile_Start", TEXT("Cruise_Missile\\Anim_Cruise_Missile_Start.sqc"), MESH_PATH);
}

void CDefaultSetting::LoadPatrick()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Patrick", TEXT("Patrick/Patrick11.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "PatrickSkeleton", TEXT("Patrick/Patrick11.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Patrick", "PatrickSkeleton");
    CResourceManager::GetInst()->AddSocket("PatrickSkeleton", "jt_hand_R", "Weapon");

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
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Lasso", TEXT("Sandy\\Lasso\\Lasso.fbx"));
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "LassoSkeleton", TEXT("Sandy\\Lasso\\Lasso.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Lasso", "LassoSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Lasso_Idle", TEXT("Sandy\\Lasso\\Anim_lasso_Pose_Straight_Rope.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Lasso_Start", TEXT("Sandy\\Lasso\\Anim_lasso_attack_start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Lasso_End", TEXT("Sandy\\Lasso\\Anim_lasso_attack_end.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Lasso_Copter", TEXT("Sandy\\Lasso\\Anim_lasso_copter.sqc"), MESH_PATH);

    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Sandy", TEXT("Sandy/Sandy_Idle.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "SandySkeleton", TEXT("Sandy/Sandy_Idle.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Sandy", "SandySkeleton");
    CResourceManager::GetInst()->AddSocket("SandySkeleton", "jt_lasso", "Weapon",Vector3(-30.f,100.f,-30.f));

    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Idle", TEXT("Sandy/Sandy_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Walk", TEXT("Sandy/Sandy_Walk.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Run", TEXT("Sandy/Sandy_Run.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_JumpDW", TEXT("Sandy/Sandy_JumpDW.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_JumpUp", TEXT("Sandy/Sandy_JumpUp.sqc"), MESH_PATH);
    //CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Jump_Landing_NonAdditive", TEXT("Sandy/Sandy_Jump_Landing_NonAdditive.sqc"), MESH_PATH);
    //CResourceManager::GetInst()->LoadAnimationSequence("Sandy_DoubleJump", TEXT("Sandy/Sandy_DoubleJump.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Karate_Chop", TEXT("Sandy/Sandy_Karate_Chop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Karate_Kick", TEXT("Sandy/Sandy_Karate_Kick.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Lasso_Start", TEXT("Sandy/Sandy_Lasso_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Death", TEXT("Sandy/Sandy_Death.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Hit", TEXT("Sandy/Anim_sandy_Hit_02.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Sandy_Swing_Loop", TEXT("Sandy/Anim_Sandy_Swing_Loop.sqc"), MESH_PATH);
}

void CDefaultSetting::LoadPlayerSound()
{
    // Sandy
    CResourceManager::GetInst()->LoadSound("Effect", "Sandy_Chop", false, "Sandy/SFX_SC_Chop.ogg", SOUND_PATH); // 주먹질
    CResourceManager::GetInst()->LoadSound("Effect", "Sandy_Kick", false, "Sandy/SFX_SC_Kick.ogg", SOUND_PATH); // 발차기
    CResourceManager::GetInst()->LoadSound("Effect", "Sandy_Jump", false, "Sandy/SFX_SC_Jump.ogg", SOUND_PATH); // 점프
    CResourceManager::GetInst()->LoadSound("Effect", "Sandy_DoubleJump", false, "Sandy/SFX_SC_DoubleJump.ogg", SOUND_PATH); // 이단 점프
    CResourceManager::GetInst()->LoadSound("Effect", "Sandy_LassoAttack", false, "Sandy/SFX_SC_LassoAttack_Throw.ogg", SOUND_PATH); // 올가미 공격
    CResourceManager::GetInst()->LoadSound("Effect", "Sandy_LassoAttack_End", false, "Sandy/SFX_SC_LassoAttack_End.ogg", SOUND_PATH); // 올가미 공격 끝
    CResourceManager::GetInst()->LoadSound("Effect", "Sandy_Walk", true, "Sandy/SFX_SC_Step_001.ogg", SOUND_PATH); // 걷기
    CResourceManager::GetInst()->LoadSound("Effect", "Sandy_Damage", false, "Sandy/SC_Ouch1.ogg", SOUND_PATH); // 다쳤을 때

    // Spongebob
    CResourceManager::GetInst()->LoadSound("Effect", "Spongebob_BubbleSpin", false, "Spongebob/SB_bubble_spin_rework_v2.ogg", SOUND_PATH); // 해파리채 공격
    CResourceManager::GetInst()->LoadSound("Effect", "Spongebob_BubbleBash", false, "Spongebob/SFX_SB_BubbleBash_V2.ogg", SOUND_PATH); // 해파리채 공격 강타
    CResourceManager::GetInst()->LoadSound("Effect", "Spongebob_Jump", false, "Spongebob/SB_jump_sngl.ogg", SOUND_PATH); // 점프
    CResourceManager::GetInst()->LoadSound("Effect", "Spongebob_DoubleJump", false, "Spongebob/SB_jump_dub.ogg", SOUND_PATH); // 이단 점프
    CResourceManager::GetInst()->LoadSound("Effect", "Spongebob_BubbleExplode", false, "Spongebob/SFX_SB_CruiseBubble_Explode.ogg", SOUND_PATH); // 미사일 명중
    CResourceManager::GetInst()->LoadSound("Effect", "Spongebob_BubbleLaunch", false, "Spongebob/SFX_SB_CruiseBubble_Launch.ogg", SOUND_PATH); // 미사일 발사
    CResourceManager::GetInst()->LoadSound("Effect", "Spongebob_BubbleLoop", false, "Spongebob/SFX_SB_CruiseBubble_Loop.ogg", SOUND_PATH); // 미사일 이동
    CResourceManager::GetInst()->LoadSound("Effect", "Spongebob_BubbleCharge", false, "Spongebob/SFX_SB_CruiseMissile_Charge.ogg", SOUND_PATH); // 미사일 충전
    CResourceManager::GetInst()->LoadSound("Effect", "Spongebob_WalkLeft", true, "Spongebob/SFX_SB_Run_L.ogg"); // 걷기 왼발
    CResourceManager::GetInst()->LoadSound("Effect", "Spongebob_WalkRight", true, "Spongebob/SFX_SB_Run_R.ogg"); // 걷기 오른발
    CResourceManager::GetInst()->LoadSound("Effect", "Spongebob_Damage", false, "Spongebob/SB_ouch1.ogg", SOUND_PATH); // 다쳤을 때
    CResourceManager::GetInst()->LoadSound("Effect", "Spongebob_Death", false, "Spongebob/SFX_SB_Death.ogg", SOUND_PATH); // 죽음
    CResourceManager::GetInst()->LoadSound("Effect", "Spongebob_Bowl_Charge", false, "Spongebob/SFX_SB_BubbleBowl_Charge.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "Spongebob_Bowl_Throw", false, "Spongebob/SFX_SB_BubbleBowl_Throw.ogg");
}

void CDefaultSetting::LoadSound()
{
    // 로딩 UI
    CResourceManager::GetInst()->LoadSound("Effect", "LoadingUI_First", false, "UI/SFX_SB_Spongball_Bubble_010.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "LoadingUI_Second", false, "UI/SFX_SB_Spongball_Bubble_008.ogg", SOUND_PATH); // first와 같이 재생
    CResourceManager::GetInst()->LoadSound("Effect", "LoadingUI", false, "UI/SFX_Bubbles_Add_001.ogg");

    // Bikini Bottom
    CResourceManager::GetInst()->LoadSound("BGM", "BikiniBottom", false, "Map/MUS_BikiniBottomTheme.ogg", SOUND_PATH);

    // Jellyfish Field
    CResourceManager::GetInst()->LoadSound("BGM", "JellyfishField", false, "Map/MUS_JellyfishFieldsTheme.ogg", SOUND_PATH);

    // BossStage BGM
    CResourceManager::GetInst()->LoadSound("BGM", "BossStage", false, "Map/MUS_BossFightTheme.ogg", SOUND_PATH);

    //Tile BGM
    CResourceManager::GetInst()->LoadSound("UI", "UI_Backward", false, "Sfx/SFX_UI_Backward_001.ogg");
    CResourceManager::GetInst()->LoadSound("BGM", "Title_Bgm", false, "Music/MUS_WavesSeagullsBg.ogg");

}

void CDefaultSetting::LoadBuildings()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "BikiniBottomRoad", TEXT("Buildings/BikiniBottom/BikiniBottomRoad.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "PineAppleHouse", TEXT("Buildings/BikiniBottom/PineAppleHouse.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "PatrickHouse", TEXT("Buildings/BikiniBottom/PatrickHouse.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "SquidHouse", TEXT("Buildings/BikiniBottom/SquidHouse.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "ChumBucket", TEXT("Buildings/BikiniBottom/ChumBucket.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Buliding_02", TEXT("Buildings/BikiniBottom/Buliding_02.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Buliding_05", TEXT("Buildings/BikiniBottom/Buliding_05.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "krustykrab", TEXT("Buildings/BikiniBottom/krustykrab.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "krustykrab_Enter", TEXT("Buildings/BikiniBottom/krustykrab_Enter.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "krustykrab_Shell", TEXT("Buildings/BikiniBottom/krustykrab_Shell.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "TaxiStop", TEXT("Buildings/BikiniBottom/TaxiStop.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "RedTree", TEXT("Buildings/BikiniBottom/RedTree.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "PurpleTree", TEXT("Buildings/BikiniBottom/PurpleTree.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Rock", TEXT("Buildings/BikiniBottom/Rock.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Rock2", TEXT("Buildings/BikiniBottom/Rock2.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Missile", TEXT("Buildings/BikiniBottom/Missile.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "InfoSign", TEXT("Buildings/BikiniBottom/InfoSign.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "DoubleTree", TEXT("Buildings/BikiniBottom/DoubleTree.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "CoconutTree", TEXT("Buildings/BikiniBottom/CoconutTree.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "SM_BB_FloatSign_01", TEXT("Buildings/BikiniBottom/SM_BB_FloatSign_01.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "fountain", TEXT("Buildings/BikiniBottom/fountain.msh"));
}

void CDefaultSetting::LoadJellyfishFieldsObj()
{
    // 해파리 동산 맵 메쉬
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "JellyfishfieldsSign", TEXT("Buildings/JellyfishField/JellyfishfieldsSign.fbx"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Water", TEXT("Buildings/JellyfishField/water.msh"));
    /*
    더블나무 : 빨강, 노랑, 보라
    작은 나무 : 빨강, 보라
    핑크 산호초(Seaflower)
    젤리나무(BouncingTree),
    조개(Clam)
    미사일,
    다리
    */
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "DoubleTree_Red", TEXT("Objects/JellyfishFields/DoubleTree_Red.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "DoubleTree_Yellow", TEXT("Objects/JellyfishFields/DoubleTree_Yellow.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "DoubleTree_Purple", TEXT("Objects/JellyfishFields/DoubleTree_Purple.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "SmallTree_Red", TEXT("Objects/JellyfishFields/SmallTree_Red.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "SmallTree_Purple", TEXT("Objects/JellyfishFields/SmallTree_Purple.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Seaflower", TEXT("Objects/JellyfishFields/Seaflower.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "BouncingTree", TEXT("Objects/JellyfishFields/BouncingTree.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Clam", TEXT("Objects/JellyfishFields/Clam.fbx"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Bridge", TEXT("Objects/JellyfishFields/Bridge.fbx"), MESH_PATH);
}

void CDefaultSetting::LoadEnemies()
{
}

void CDefaultSetting::LoadParticle()
{
    CParticle* Particle = nullptr;

    // 맵 바닥에서 올라오는 거품 파티클
    CResourceManager::GetInst()->CreateParticle("GroundBubble");

    Particle = CResourceManager::GetInst()->FindParticle("GroundBubble");

    Particle->SetMaterial("Bubble");

    Particle->SetParticleSpawnTime(0.8f);
    Particle->SetParticleStartMin(Vector3(-10.f, -10.f, -10.f));
    Particle->SetParticleStartMax(Vector3(10.f, 10.f, 10.f));
    Particle->SetParticleSpawnCountMax(1000);
    Particle->SetParticleScaleMin(Vector3(5.f, 5.f, 5.f));
    Particle->SetParticleScaleMax(Vector3(60.f, 60.f, 60.f));
    Particle->SetParticleLifeTimeMin(8.f);
    Particle->SetParticleLifeTimeMax(12.f);
    Particle->SetParticleColorMin(Vector4(0.5f, 0.f, 1.f, 0.5f));
    Particle->SetParticleColorMax(Vector4(0.7f, 0.f, 1.f, 0.7f));
    Particle->SetParticleSpeedMin(40.f);
    Particle->SetParticleSpeedMax(60.f);
    Particle->SetParticleMoveEnable(true);
    Particle->SetParticleGravityEnable(false);
    Particle->SetParticleMoveDir(Vector3(0.f, 1.f, 0.f));
    Particle->SetParticleMoveDirEnable(true);
    Particle->SetParticleMoveAngle(Vector3(0.f, 0.f, 5.f));
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

    CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_Idle", true, "KingJellyfish/SFX_Boss_JFK_Move.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_Charge", false, "KingJellyfish/KJ_Charge.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_Angry", false, "KingJellyfish/KJ_Taunt.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_Damage", false, "KingJellyfish/SFX_Boss_JFK_Grunt.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_SpawnJellyfish", false, "KingJellyfish/SFX_Boss_JFK_Birth_New.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_OnGround", false, "KingJellyfish/SFX_Boss_JFK_Land_001.ogg", SOUND_PATH);

    // 캐릭터 이펙트 사운드
    // CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_Pulse", false, "KingJellyfish/SFX_Boss_JFK_Pulse.ogg", SOUND_PATH);
    // CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_Shield_Down", false, "KingJellyfish/SFX_Boss_JFK_Shield_Down.ogg", SOUND_PATH);
    // CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_Shield_Up", false, "KingJellyfish/SFX_Boss_JFK_Shield_Up.ogg", SOUND_PATH);


}

void CDefaultSetting::LoadJellyfish()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Jellyfish", TEXT("Jellyfish/Jellyfish.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "JellyfishSkeleton", TEXT("Jellyfish/Jellyfish.bne"), MESH_PATH);

    CResourceManager::GetInst()->SetMeshSkeleton("Jellyfish", "JellyfishSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Jellyfish_Attack", TEXT("Jellyfish/Jellyfish_attack.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Jellyfish_Death", TEXT("Jellyfish/Jellyfish_death.sqc"), MESH_PATH);

    CResourceManager::GetInst()->LoadSound("Effect", "Jellyfish_Attack", false, "Jellyfish/SFX_Enemy_Jellyfish_Attack_Original.ogg", SOUND_PATH);
}

void CDefaultSetting::LoadTikis()
{    
    // Woods -> Normal
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Tiki_Woods", TEXT("Tikis/Tiki_Woods.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "Tiki_Woods_Skeleton", TEXT("Tikis/Tiki_Woods.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Tiki_Woods", "Tiki_Woods_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Tiki_Woods_Idle", TEXT("Tikis/Tiki_Woods_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Tiki_Woods_Die", TEXT("Tikis/Tiki_Woods_Die.sqc"), MESH_PATH);


    // Stone -> Unbreakerble
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Tiki_Stone", TEXT("Tikis/Tiki_Stone.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "Tiki_Stone_Skeleton", TEXT("Tikis/Tiki_Stone.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Tiki_Stone", "Tiki_Stone_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Tiki_Stone_Idle", TEXT("Tikis/Tiki_Stone_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Tiki_Stone_Die", TEXT("Tikis/Tiki_Stone_Die.sqc"), MESH_PATH);


    // Thunder -> Bomb
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Tiki_Thunder", TEXT("Tikis/Tiki_Thunder.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "Tiki_Thunder_Skeleton", TEXT("Tikis/Tiki_Thunder.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Tiki_Thunder", "Tiki_Thunder_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Tiki_Thunder_Idle", TEXT("Tikis/Tiki_Thunder_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Tiki_Thunder_Die", TEXT("Tikis/Tiki_Thunder_Die.sqc"), MESH_PATH);
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

void CDefaultSetting::LoadPatric_Npc()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Patric_Npc", TEXT("Patric_Npc/Patric.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "Patric_Npc_Skeleton", TEXT("Patric_Npc/Patric.bne"), MESH_PATH);

    CResourceManager::GetInst()->SetMeshSkeleton("Patric_Npc", "Patric_Npc_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Patric_Npc_Confused", TEXT("Patric_Npc/Patric_Npc_Confused.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patric_Npc_Default", TEXT("Patric_Npc/Patric_Npc_Default.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patric_Npc_Excited", TEXT("Patric_Npc/Patric_Npc_Excited.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patric_Npc_Scowl_Loop", TEXT("Patric_Npc/Patric_Npc_Scowl_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patric_Npc_Scowl_Start", TEXT("Patric_Npc/Patric_Npc_Scowl_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patric_Npc_Talk", TEXT("Patric_Npc/Patric_Npc_Talk.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patric_Npc_Thinking_Loop", TEXT("Patric_Npc/Patric_Npc_Thinking_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patric_Npc_Thinking_Start", TEXT("Patric_Npc/Patric_Npc_Thinking_Start.sqc"), MESH_PATH);
}

void CDefaultSetting::LoadTaxi()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Taxi_Driver", TEXT("Taxi/Taxi.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Taxi_Stop", TEXT("Taxi/TaxiStop.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Bus_Driver", TEXT("Taxi/Bus_Driver.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "Bus_Driver_Skeleton", TEXT("Taxi/Bus_Driver.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Bus_Driver", "Bus_Driver_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Bus_Driver_Drive", TEXT("Taxi/Bus_Driver_Drive.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Bus_Driver_Stop", TEXT("Taxi/Bus_Driver_Stop.sqc"), MESH_PATH);
}
