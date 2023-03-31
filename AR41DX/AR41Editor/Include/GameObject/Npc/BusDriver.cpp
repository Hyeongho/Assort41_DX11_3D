#include "BusDriver.h"

#include "Input.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../../Scene/JellyfishFieldSceneInfo.h"
#include "../../Scene/BikiniCitySceneInfo.h"
#include "../../Scene/LoadingSceneInfo.h"
#include "../../UI/DialogUI.h"
#include "../Object/BB/BusStop.h"
#include "../Player.h"

CBusDriver::CBusDriver()
{
    SetTypeID<CBusDriver>();

    m_ObjectTypeName = "BusDriver";

    m_DialogCount = 0;
    m_NpcType = ENpcList::BusDriver;
    m_NpcMapPos = EMapList::Bikini_Bottom;
    m_EnableDialog = false;
    m_NpcMeshType = MeshType::Animation;

    m_BusStart = false;
    m_BusState = EBusState::End;
    m_PurposeScene = EMapList::End;
}

CBusDriver::CBusDriver(const CBusDriver& Obj)
    : CNpc(Obj)
{
    m_AnimMesh = (CAnimationMeshComponent*)FindComponent("Mesh");
    m_Animation = Obj.m_Animation;

    m_DialogCount = Obj.m_DialogCount;
    m_NpcType = Obj.m_NpcType;
    m_NpcMapPos = Obj.m_NpcMapPos;
    m_EnableDialog = Obj.m_EnableDialog;
    m_NpcMeshType = Obj.m_NpcMeshType;

    m_BusStart = Obj.m_BusStart;
    m_BusState = Obj.m_BusState;
    m_PurposeScene = Obj.m_PurposeScene;
}

CBusDriver::~CBusDriver()
{
}

void CBusDriver::Start()
{
    CNpc::Start();

#ifdef _DEBUG
    //CInput::GetInst()->AddBindFunction<CBusDriver>("F1", Input_Type::Up, this, &CBusDriver::DebugKeyF1, m_Scene);
    //CInput::GetInst()->AddBindFunction<CBusDriver>("F2", Input_Type::Up, this, &CBusDriver::DebugKeyF2, m_Scene);
    CInput::GetInst()->AddBindFunction<CBusDriver>("F3", Input_Type::Up, this, &CBusDriver::DebugKeyF3, m_Scene);
#endif // DEBUG

    CInput::GetInst()->AddBindFunction<CBusDriver>("F", Input_Type::Up, this, &CBusDriver::MoveFromBusStop, m_Scene);

    CreateAnim();
}

bool CBusDriver::Init()
{
    CNpc::Init();

    m_AnimMesh->SetMesh("Bus_Driver");

    Vector3 ColSize = m_AnimMesh->GetMeshSize();

    if (m_AnimMesh->GetMeshSize().x >= m_AnimMesh->GetMeshSize().z)
        ColSize.z = m_AnimMesh->GetMeshSize().x;
    else
        ColSize.x = m_AnimMesh->GetMeshSize().z;

    m_Collider->SetBoxHalfSize(ColSize / 2.f);
    m_Collider->SetRelativePositionY(ColSize.y / 2.f);

    CreateAnim();

    return true;
}

void CBusDriver::Update(float DeltaTime)
{
    CNpc::Update(DeltaTime);

    if (m_BusStart) {
        if (m_BusState == EBusState::Stop) {
            // Update 중지 처리
            m_BusStart = false;

            // 애니메이션 변경
            ChangeAnim_Stop();

            return;
        }

        // 현재는 X이동만 가능하게끔 작업.
        //AddWorldPositionX(-1500.f * g_DeltaTime);
        AddWorldPositionZ(3000.f * g_DeltaTime);

        Vector3 DestCenter = m_Scene->FindObject("BusStop")->GetCenter();

        if (m_BusState == EBusState::MoveToBusStop) {
            // 생성 직후의 Pos에 따른 문제의 예외처리
            if (m_Center == Vector3(0.f, 0.f, 0.f))
                return;

            // 현재는 X이동만 가능하게끔 작업.
            //if (m_Center.x <= DestCenter.x)
            if (m_Center.z >= DestCenter.z)
                m_BusState = EBusState::Stop;
        }

        if (m_BusState == EBusState::MoveFromBusStop) {
            if (m_Center.Distance(DestCenter) >= 2000.f) {
                ChangeScene();
            }
        }
    }
}

void CBusDriver::PostUpdate(float DeltaTime)
{
    CNpc::PostUpdate(DeltaTime);
}

CBusDriver* CBusDriver::Clone() const
{
    return new CBusDriver(*this);
}

void CBusDriver::Save(FILE* File)
{
    CNpc::Save(File);
}

void CBusDriver::Load(FILE* File)
{
    CNpc::Load(File);
}

void CBusDriver::MoveToBusStop(const Vector3& BusStopPos, const Vector3& BusStartPos)
{
    // Update 처리
    m_BusStart = true;

    // BusState 변경
    m_BusState = EBusState::MoveToBusStop;

    // 애니메이션 변경
    ChangeAnim_Drive();

    // 시작점 배치
    SetWorldPosition(BusStartPos);

    // BusStop을 바라보도록 설정
    float Degree = atan2(BusStartPos.z - BusStopPos.z, BusStartPos.x - BusStopPos.x);
    Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;
    Degree -= 90.f;

    SetWorldRotationY(Degree);
}

void CBusDriver::MoveFromBusStop()
{
    // Update 처리
    m_BusStart = true;

    // BusState 변경
    m_BusState = EBusState::MoveFromBusStop;

    // 애니메이션 변경
    ChangeAnim_Drive();

    // 버스 이동 컷씬 실행
    BusMoveCutScene();
}

void CBusDriver::CreateAnim()
{
    if (m_Animation)
        return;


    m_Animation = m_AnimMesh->SetAnimation<CAnimation>("BusDriverAnimation");

    m_Animation->AddAnimation("Bus_Driver_Drive", "Bus_Driver_Drive", 1.f, 1.f, true);
    m_Animation->AddAnimation("Bus_Driver_Stop", "Bus_Driver_Stop", 1.f, 1.f, false);

    m_Animation->SetCurrentAnimation("Bus_Driver_Drive");
}

void CBusDriver::ChangeAnim_Stop()
{
    if (m_Animation)
        return;

    if (!m_Animation->FindAnimation("Bus_Driver_Stop"))
        return;

    m_Animation->ChangeAnimation("Bus_Driver_Stop");
}

void CBusDriver::ChangeAnim_Drive()
{
    if (m_Animation)
        return;

    if (!m_Animation->FindAnimation("Bus_Driver_Drive"))
        return;

    m_Animation->ChangeAnimation("Bus_Driver_Drive");
}

void CBusDriver::BusMoveCutScene()
{
    // 카메라를 버스 정류소로 이동시킨다.
    // 버스가 멀어지는걸 관찰시키고, 버스가 특정 거리만큼 이동되면 장면을 전환.
    
    // 버스 정류소(BusStop)의 타겟암으로 카메라를 이동.
    CBusStop* BusStop = (CBusStop*)m_Scene->FindObject("BusStop");
    BusStop->CutSceneStart();
    
    // 플레이어의 움직임을 제한.
    CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();
    // Player->SetStop();
}

void CBusDriver::ChangeScene()
{
    //if (m_PurposeScene == EMapList::End)
    //    return;

    //return;

    // 맵 변경
    CSceneManager::GetInst()->CreateNextScene();
    CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "JellyFish.scn");

    //// 설정된 목적지별로 다른 씬 로딩
    //if (m_PurposeScene == EMapList::Jelly_Fish_Field)
    //    CSceneManager::GetInst()->CreateSceneInfo<CJellyfishFieldSceneInfo>(false);
    //else if (m_PurposeScene == EMapList::Bikini_Bottom)
    //    CSceneManager::GetInst()->CreateSceneInfo<CBikiniCitySceneInfo>(false);
}

void CBusDriver::DebugKeyF1()
{
    ChangeAnim_Stop();
}

void CBusDriver::DebugKeyF2()
{
    ChangeAnim_Drive();
}

void CBusDriver::DebugKeyF3()
{
    MoveFromBusStop();
}
