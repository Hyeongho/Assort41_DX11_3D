#include "AnimationSequence.h"
#include "../Mesh/FBXLoader.h"
#include "../../PathManager.h"
#include "../Shader/StructuredBuffer.h"

CAnimationSequence::CAnimationSequence()    :
    m_Scene(nullptr),
    m_Loop(false),
    m_StartTime(0.f),
    m_EndTime(0.f),
    m_TimeLength(0.f),
    m_FrameTime(0.f),
    m_PlayTime(1.f),
    m_PlayScale(1.f),
    m_StartFrame(0),
    m_EndFrame(0),
    m_FrameLength(0),
    m_FrameMode(0),
    m_ChangeFrame(0),
    m_End(false),
    m_FullPath{},
    m_KeyFrameBuffer(nullptr)
{
    SetTypeID<CAnimationSequence>();
}

CAnimationSequence::CAnimationSequence(const CAnimationSequence& Anim)
{
    *this = Anim;

    m_KeyFrameBuffer = nullptr;

    size_t  Size = m_vecKeyFrame.size();

    for (size_t i = 0; i < Size; ++i)
    {
        ++m_vecKeyFrame[i]->RefCount;
    }
}

CAnimationSequence::~CAnimationSequence()
{
    SAFE_DELETE(m_KeyFrameBuffer);

    size_t  Size = m_vecKeyFrame.size();

    for (size_t i = 0; i < Size; ++i)
    {
        --m_vecKeyFrame[i]->RefCount;

        if (m_vecKeyFrame[i]->RefCount == 0)
            delete m_vecKeyFrame[i];
    }

    m_vecKeyFrame.clear();
}

bool CAnimationSequence::IsSequenceEnd() const
{
    return m_End;
}

void CAnimationSequence::GetKeyFrame(
    std::vector<BoneKeyFrame*>& vecFrame)
{
    vecFrame = m_vecKeyFrame;
}

bool CAnimationSequence::LoadFbxAnimation(const char* FullPath,
    bool Loop)
{
    CFBXLoader  Loader;

    Loader.LoadFBX(FullPath, false);

    // 애니메이션 클립을 추가한다.
    const std::vector<FbxAnimationClip*>* pvecClip = Loader.GetClips();

    memset(m_FullPath, 0, MAX_PATH);
    strcpy_s(m_FullPath, FullPath);

    return CreateSequence(Loop, pvecClip->front());
}

bool CAnimationSequence::Save(const TCHAR* FileName, 
    const std::string& PathName)
{
    TCHAR	FullPath[MAX_PATH] = {};

    const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

    if (Path)
        lstrcpy(FullPath, Path->Path);

    lstrcat(FullPath, FileName);

    return SaveFullPath(FullPath);
}

bool CAnimationSequence::SaveMultibyte(const char* FileName,
    const std::string& PathName)
{
    char	FullPath[MAX_PATH] = {};

    const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

    if (Path)
        strcpy_s(FullPath, Path->PathMultibyte);

    strcat_s(FullPath, FileName);

    return SaveFullPathMultibyte(FullPath);
}

bool CAnimationSequence::SaveFullPath(const TCHAR* FullPath)
{
    char    FullPathMultibyte[MAX_PATH] = {};

    int Length = WideCharToMultiByte(CP_ACP, 0, FullPath,
        -1, 0, 0, 0, 0);
    WideCharToMultiByte(CP_ACP, 0, FullPath,
        -1, FullPathMultibyte, Length, 0, 0);

    return SaveFullPathMultibyte(FullPathMultibyte);
}

bool CAnimationSequence::SaveFullPathMultibyte(const char* FullPath)
{
    FILE* File = NULL;

    fopen_s(&File, FullPath, "wb");

    if (!File)
        return false;

    CRef::Save(File);

    fwrite(&m_Loop, sizeof(bool), 1, File);
    fwrite(&m_StartTime, sizeof(float), 1, File);
    fwrite(&m_EndTime, sizeof(float), 1, File);
    fwrite(&m_TimeLength, sizeof(float), 1, File);
    fwrite(&m_FrameTime, sizeof(float), 1, File);
    fwrite(&m_PlayTime, sizeof(float), 1, File);
    fwrite(&m_PlayScale, sizeof(float), 1, File);
    fwrite(&m_StartFrame, sizeof(int), 1, File);
    fwrite(&m_EndFrame, sizeof(int), 1, File);
    fwrite(&m_FrameLength, sizeof(int), 1, File);
    fwrite(&m_FrameMode, sizeof(int), 1, File);
    fwrite(&m_ChangeFrame, sizeof(int), 1, File);

    size_t	Count = m_vecKeyFrame.size();

    fwrite(&Count, sizeof(size_t), 1, File);

    for (size_t i = 0; i < Count; ++i)
    {
        fwrite(&m_vecKeyFrame[i]->BoneIndex, sizeof(int), 1,
            File);

        size_t	FrameCount = m_vecKeyFrame[i]->vecKeyFrame.size();

        fwrite(&FrameCount, sizeof(size_t), 1, File);

        for (size_t j = 0; j < FrameCount; ++j)
        {
            fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->Time, sizeof(double), 1, File);
            fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->Pos, sizeof(Vector3), 1, File);
            fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->Scale, sizeof(Vector3), 1, File);
            fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->Rot, sizeof(Vector4), 1, File);
        }
    }

    fclose(File);

    return true;
}

bool CAnimationSequence::Load(const TCHAR* FileName,
    const std::string& PathName)
{
    TCHAR	FullPath[MAX_PATH] = {};

    const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

    if (Path)
        lstrcpy(FullPath, Path->Path);

    lstrcat(FullPath, FileName);

    return LoadFullPath(FullPath);
}

bool CAnimationSequence::LoadMultibyte(const char* FileName, 
    const std::string& PathName)
{
    char	FullPath[MAX_PATH] = {};

    const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

    if (Path)
        strcpy_s(FullPath, Path->PathMultibyte);

    strcat_s(FullPath, FileName);

    return LoadFullPathMultibyte(FullPath);
}

bool CAnimationSequence::LoadFullPath(const TCHAR* FullPath)
{
    char    FullPathMultibyte[MAX_PATH] = {};

    int Length = WideCharToMultiByte(CP_ACP, 0, FullPath,
        -1, 0, 0, 0, 0);
    WideCharToMultiByte(CP_ACP, 0, FullPath,
        -1, FullPathMultibyte, Length, 0, 0);

    return LoadFullPathMultibyte(FullPathMultibyte);
}

bool CAnimationSequence::LoadFullPathMultibyte(
    const char* FullPath)
{
    FILE* File = NULL;

    fopen_s(&File, FullPath, "rb");

    if (!File)
        return false;

    CRef::Load(File);

    fread(&m_Loop, sizeof(bool), 1, File);
    fread(&m_StartTime, sizeof(float), 1, File);
    fread(&m_EndTime, sizeof(float), 1, File);
    fread(&m_TimeLength, sizeof(float), 1, File);
    fread(&m_FrameTime, sizeof(float), 1, File);
    fread(&m_PlayTime, sizeof(float), 1, File);
    fread(&m_PlayScale, sizeof(float), 1, File);
    fread(&m_StartFrame, sizeof(int), 1, File);
    fread(&m_EndFrame, sizeof(int), 1, File);
    fread(&m_FrameLength, sizeof(int), 1, File);
    fread(&m_FrameMode, sizeof(int), 1, File);
    fread(&m_ChangeFrame, sizeof(int), 1, File);

    size_t	Count = 0;

    fread(&Count, sizeof(size_t), 1, File);

    std::vector<AnimationFrameTransform>	vecFrameTrans;
    vecFrameTrans.resize(Count * m_FrameLength);

    for (size_t i = 0; i < Count; ++i)
    {
        BoneKeyFrame* pBoneKeyFrame = new BoneKeyFrame;
        m_vecKeyFrame.push_back(pBoneKeyFrame);

        fread(&pBoneKeyFrame->BoneIndex, sizeof(int), 1,
            File);

        size_t	BoneFrameCount = 0;

        fread(&BoneFrameCount, sizeof(size_t), 1, File);

        for (size_t j = 0; j < BoneFrameCount; ++j)
        {
            KeyFrame* pKeyFrame = new KeyFrame;
            pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);

            fread(&pKeyFrame->Time, sizeof(double), 1, File);
            fread(&pKeyFrame->Pos, sizeof(Vector3), 1, File);
            fread(&pKeyFrame->Scale, sizeof(Vector3), 1, File);
            fread(&pKeyFrame->Rot, sizeof(Vector4), 1, File);

            if (j < m_FrameLength)
            {
                AnimationFrameTransform	FrameTr = {};
                FrameTr.Scale = Vector4(pKeyFrame->Scale.x, pKeyFrame->Scale.y,
                    pKeyFrame->Scale.z, 1.f);
                FrameTr.Translate = Vector4(pKeyFrame->Pos.x, pKeyFrame->Pos.y,
                    pKeyFrame->Pos.z, 1.f);
                FrameTr.Rot = pKeyFrame->Rot;

                vecFrameTrans[i * m_FrameLength + j] = FrameTr;
            }
        }
    }

    m_KeyFrameBuffer = new CStructuredBuffer;

    m_KeyFrameBuffer->Init("KeyFrameBuffer", (unsigned int)sizeof(AnimationFrameTransform),
        (unsigned int)vecFrameTrans.size(), 15, true, (int)EShaderBufferType::Compute);

    m_KeyFrameBuffer->UpdateBuffer(&vecFrameTrans[0],
        (unsigned int)vecFrameTrans.size());

    fclose(File);

    return true;
}

void CAnimationSequence::SetPlayScale(float Scale)
{
    m_PlayScale = Scale;
}

void CAnimationSequence::SetPlayTime(float Time)
{
    m_PlayTime = Time;

    m_EndTime = m_PlayTime;
    m_TimeLength = m_PlayTime;

    m_FrameTime = m_PlayTime / m_FrameLength;

    size_t  Size = m_vecKeyFrame.size();

    for (size_t i = 0; i < Size; ++i)
    {
        BoneKeyFrame* Frame = m_vecKeyFrame[i];

        size_t  FrameCount = Frame->vecKeyFrame.size();

        for (size_t j = 0; j < FrameCount; ++j)
        {
            KeyFrame* pKeyFrame = Frame->vecKeyFrame[j];

            pKeyFrame->Time = j * m_FrameTime;
        }
    }
}

bool CAnimationSequence::CreateSequence(bool Loop, 
    const FbxAnimationClip* Clip)
{
    // 인자로 들어온 애니메이션 옵션정보를 설정한다.
    m_Loop = Loop;
    SetName(Clip->Name);

    m_ChangeFrame = 0;

    switch (Clip->TimeMode)
    {
    case FbxTime::eFrames24:
        m_FrameMode = 24;
        break;
    case FbxTime::eFrames30:
        m_FrameMode = 30;
        break;
    case FbxTime::eFrames60:
        m_FrameMode = 60;
        break;
    }

    // FbxAnimationClip에 있는 starttime 과 endtime 을 이용하여 keyframe 을 얻어온다.
    m_StartFrame = (int)Clip->StartTime.GetFrameCount(Clip->TimeMode);
    m_EndFrame = (int)Clip->EndTime.GetFrameCount(Clip->TimeMode);
    m_FrameLength = (int)(m_EndFrame - m_StartFrame + 1);

    // 시간 정보를 저장해준다.
    m_StartTime = 0.f;
    m_EndTime = m_PlayTime;
    m_TimeLength = m_PlayTime;

    m_FrameTime = m_PlayTime / m_FrameLength;

    std::vector<AnimationFrameTransform>	vecFrameTrans;
    vecFrameTrans.resize(Clip->vecBoneKeyFrame.size() * m_FrameLength);

    // 본 3개 키프레임 5개일 경우
    // 본0키0|본0키1|본0키2|본0키3|본0키4|본1키0|본1키1|본1키2|본1키3|본1키4|본2키0|본2키1|본2키2|본2키3|본2키4

    // 키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 위치, 크기, 회전정보로
    // 뽑아온다.
    for (size_t i = 0; i < Clip->vecBoneKeyFrame.size(); ++i)
    {
        BoneKeyFrame* pBoneKeyFrame = new BoneKeyFrame;

        pBoneKeyFrame->BoneIndex = Clip->vecBoneKeyFrame[i].BoneIndex;

        m_vecKeyFrame.push_back(pBoneKeyFrame);

        // 아래부터 키프레임 정보를 저장한다.
        pBoneKeyFrame->vecKeyFrame.reserve(Clip->vecBoneKeyFrame[i].vecKeyFrame.size());

        for (size_t j = 0; j < Clip->vecBoneKeyFrame[i].vecKeyFrame.size(); ++j)
        {
            KeyFrame* pKeyFrame = new KeyFrame;

            pKeyFrame->Time = j * m_FrameTime;

            // 현재 본의 키 프레임에 해당하는 행렬 정보를 얻어온다.
            FbxAMatrix		mat = Clip->vecBoneKeyFrame[i].vecKeyFrame[j].matTransform;

            FbxVector4		Pos, Scale;
            FbxQuaternion	Rot;

            // 행렬로부터 위치, 크기, 회전 정보를 얻어온다.
            Pos = mat.GetT();
            Scale = mat.GetS();
            Rot = mat.GetQ();

            pKeyFrame->Scale = Vector3((float)Scale.mData[0], (float)Scale.mData[1],
                (float)Scale.mData[2]);
            pKeyFrame->Pos = Vector3((float)Pos.mData[0], (float)Pos.mData[1],
                (float)Pos.mData[2]);
            pKeyFrame->Rot = Vector4((float)Rot.mData[0], (float)Rot.mData[1],
                (float)Rot.mData[2], (float)Rot.mData[3]);

            pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);

            if (j < m_FrameLength)
            {
                AnimationFrameTransform	FrameTr = {};
                FrameTr.Scale = Vector4(pKeyFrame->Scale.x, pKeyFrame->Scale.y,
                    pKeyFrame->Scale.z, 1.f);
                FrameTr.Translate = Vector4(pKeyFrame->Pos.x, pKeyFrame->Pos.y,
                    pKeyFrame->Pos.z, 1.f);
                FrameTr.Rot = pKeyFrame->Rot;

                vecFrameTrans[i * m_FrameLength + j] = FrameTr;
            }
        }
    }

    m_KeyFrameBuffer = new CStructuredBuffer;

    m_KeyFrameBuffer->Init("KeyFrameBuffer", (unsigned int)sizeof(AnimationFrameTransform),
        (unsigned int)vecFrameTrans.size(), 15, true, (int)EShaderBufferType::Compute);

    m_KeyFrameBuffer->UpdateBuffer(&vecFrameTrans[0],
        (unsigned int)vecFrameTrans.size());

    if (strlen(m_FullPath) != 0)
    {
        char	AnimName[MAX_PATH] = {};

        _splitpath_s(m_FullPath, 0, 0, 0, 0, AnimName, MAX_PATH, 0, 0);

        char	AnimPath[MAX_PATH] = {};
        strcpy_s(AnimPath, m_FullPath);

        int	iLength = (int)strlen(AnimPath);
        for (int i = iLength - 1; i >= 0; --i)
        {
            // aa/bb.exe 9개, 2번인덱스 3 ~ 8번까지 제거
            if (AnimPath[i] == '/' || AnimPath[i] == '\\')
            {
                memset(&AnimPath[i + 1], 0, sizeof(TCHAR) * (iLength - (i + 1)));
                break;
            }
        }

        char	AnimFullPath[MAX_PATH] = {};
        strcpy_s(AnimFullPath, AnimPath);
        strcat_s(AnimFullPath, AnimName);
        strcat_s(AnimFullPath, ".sqc");

        SaveFullPathMultibyte(AnimFullPath);
    }

    return true;
}

bool CAnimationSequence::CreateSequence(const std::string& Name,
    bool Loop, int StartFrame, int EndFrame, float PlayTime, 
    const std::vector<BoneKeyFrame*>& vecFrame)
{
    // 인자로 들어온 애니메이션 옵션정보를 설정한다.
    m_Loop = Loop;
    SetName(Name);

    m_ChangeFrame = 0;

    // FBXANIMATIONCLIP에 있는 starttime 과 endtime 을 이용하여 keyframe 을 얻어온다.
    m_StartFrame = 0;
    m_EndFrame = EndFrame - StartFrame;
    m_FrameLength = m_EndFrame - m_StartFrame + 1;

    m_PlayTime = PlayTime;
    m_FrameTime = PlayTime / m_FrameLength;

    // 시간 정보를 저장해준다.
    m_StartTime = 0.f;
    m_EndTime = PlayTime;
    m_TimeLength = PlayTime;

    std::vector<AnimationFrameTransform>	vecFrameTrans;
    vecFrameTrans.resize(vecFrame.size() * m_FrameLength);

    // 키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 위치, 크기, 회전정보로
    // 뽑아온다.
    for (size_t i = 0; i < vecFrame.size(); ++i)
    {
        BoneKeyFrame* pBoneKeyFrame = new BoneKeyFrame;

        pBoneKeyFrame->BoneIndex = vecFrame[i]->BoneIndex;

        m_vecKeyFrame.push_back(pBoneKeyFrame);

        // 아래부터 키프레임 정보를 저장한다.
        pBoneKeyFrame->vecKeyFrame.reserve(m_FrameLength);

        if (!vecFrame[i]->vecKeyFrame.empty())
        {
            for (size_t j = 0; j < m_FrameLength; ++j)
            {
                KeyFrame* pKeyFrame = new KeyFrame;

                pKeyFrame->Time = (j - StartFrame) * m_FrameTime;
                pKeyFrame->Scale = vecFrame[i]->vecKeyFrame[j]->Scale;
                pKeyFrame->Rot = vecFrame[i]->vecKeyFrame[j]->Rot;
                pKeyFrame->Pos = vecFrame[i]->vecKeyFrame[j]->Pos;

                pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);

                if (j < m_FrameLength)
                {
                    AnimationFrameTransform	FrameTr = {};
                    FrameTr.Scale = Vector4(pKeyFrame->Scale.x, pKeyFrame->Scale.y,
                        pKeyFrame->Scale.z, 1.f);
                    FrameTr.Translate = Vector4(pKeyFrame->Pos.x, pKeyFrame->Pos.y,
                        pKeyFrame->Pos.z, 1.f);
                    FrameTr.Rot = pKeyFrame->Rot;

                    vecFrameTrans[i * m_FrameLength + j] = FrameTr;
                }
            }
        }
    }

    m_KeyFrameBuffer = new CStructuredBuffer;

    m_KeyFrameBuffer->Init("KeyFrameBuffer", (unsigned int)sizeof(AnimationFrameTransform),
        (unsigned int)vecFrameTrans.size(), 15, true, (int)EShaderBufferType::Compute);

    m_KeyFrameBuffer->UpdateBuffer(&vecFrameTrans[0],
        (unsigned int)vecFrameTrans.size());

    return true;
}

bool CAnimationSequence::CreateSequence(const std::string& Name,
    const TCHAR* FileName, const std::string& PathName)
{
    TCHAR	FullPath[MAX_PATH] = {};

    const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

    if (Path)
        lstrcpy(FullPath, Path->Path);

    lstrcat(FullPath, FileName);

    return CreateSequenceFullPath(Name, FullPath);
}

bool CAnimationSequence::CreateSequenceFullPath(
    const std::string& Name, const TCHAR* FullPath)
{
    char    FullPathMultibyte[MAX_PATH] = {};

    int Length = WideCharToMultiByte(CP_ACP, 0, FullPath,
        -1, 0, 0, 0, 0);
    WideCharToMultiByte(CP_ACP, 0, FullPath,
        -1, FullPathMultibyte, Length, 0, 0);

    return CreateSequenceFullPathMultibyte(Name, FullPathMultibyte);
}

bool CAnimationSequence::CreateSequenceMultibyte(
    const std::string& Name, const char* FileName,
    const std::string& PathName)
{
    char	FullPath[MAX_PATH] = {};

    const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

    if (Path)
        strcpy_s(FullPath, Path->PathMultibyte);

    strcat_s(FullPath, FileName);

    return CreateSequenceFullPathMultibyte(Name, FullPath);
}

bool CAnimationSequence::CreateSequenceFullPathMultibyte(
    const std::string& Name, const char* FullPath)
{
    SetName(Name);

    char	Ext[_MAX_EXT] = {};

    _splitpath_s(FullPath, 0, 0, 0, 0, 0, 0, Ext, _MAX_EXT);

    _strupr_s(Ext);

    if (strcmp(Ext, ".FBX") == 0)
    {
        return LoadFbxAnimation(FullPath, m_Loop);
    }

    return LoadFullPathMultibyte(FullPath);
}

void CAnimationSequence::SetShader()
{
    m_KeyFrameBuffer->SetShader();
}

void CAnimationSequence::ResetShader()
{
    m_KeyFrameBuffer->ResetShader();
}

void CAnimationSequence::SetChangeShader()
{
    m_KeyFrameBuffer->SetShader(17, (int)EShaderBufferType::Compute);
}

void CAnimationSequence::ResetChangeShader()
{
    m_KeyFrameBuffer->ResetShader(17, (int)EShaderBufferType::Compute);
}

void CAnimationSequence::Clear()
{
}

void CAnimationSequence::AddNotify(const std::string& Name,
    float Time)
{
}

CAnimationSequence* CAnimationSequence::Clone()
{
    return new CAnimationSequence(*this);
}
