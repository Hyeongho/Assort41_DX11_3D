#pragma once

#include "../../Ref.h"

struct KeyFrame
{
    double  Time;
    Vector3 Pos;
    Vector3 Scale;
    Vector4 Rot;
};

struct BoneKeyFrame
{
    int     BoneIndex;
    std::vector<KeyFrame*>  vecKeyFrame;
    int     RefCount;

    BoneKeyFrame()  :
        RefCount(1)
    {
    }

    ~BoneKeyFrame()
    {
        size_t  Size = vecKeyFrame.size();
        
        for (size_t i = 0; i < Size; ++i)
        {
            SAFE_DELETE(vecKeyFrame[i]);
        }

        vecKeyFrame.clear();
    }
};

struct AnimationFrameTransform
{
    Vector4 Translate;
    Vector4 Scale;
    Vector4 Rot;
};

class CAnimationSequence :
    public CRef
{
    friend class CAnimationManager;
    friend class CResourceManager;
    friend class CSceneResource;
    friend class CAnimation;

private:
    CAnimationSequence();
    CAnimationSequence(const CAnimationSequence& Anim);
    ~CAnimationSequence();

private:
    class CScene* m_Scene;
    bool						m_Loop;
    float						m_StartTime;
    float						m_EndTime;
    float						m_TimeLength;
    float						m_FrameTime;
    float						m_PlayTime;
    float						m_PlayScale;
    int							m_StartFrame;
    int							m_EndFrame;
    int							m_FrameLength;
    int							m_FrameMode;
    int							m_ChangeFrame;
    bool						m_End;
    std::vector<BoneKeyFrame*>	m_vecKeyFrame;
    char						m_FullPath[MAX_PATH];
    class CStructuredBuffer*    m_KeyFrameBuffer;

public:
    int GetKeyFrameCount()	const
    {
        return m_FrameLength;
    }

public:
    void SetScene(class CScene* Scene)
    {
        m_Scene = Scene;
    }

public:
    bool IsSequenceEnd()	const;
    void GetKeyFrame(std::vector<BoneKeyFrame*>& vecFrame);
    bool LoadFbxAnimation(const char* FullPath, bool Loop = false);
    bool Save(const TCHAR* FileName, const std::string& PathName = MESH_PATH);
    bool SaveMultibyte(const char* FileName, const std::string& PathName = MESH_PATH);
    bool SaveFullPath(const TCHAR* FullPath);
    bool SaveFullPathMultibyte(const char* FullPath);
    bool Load(const TCHAR* FileName, const std::string& PathName = MESH_PATH);
    bool LoadMultibyte(const char* FileName, const std::string& PathName = MESH_PATH);
    bool LoadFullPath(const TCHAR* FullPath);
    bool LoadFullPathMultibyte(const char* FullPath);

public:
    void SetPlayScale(float Scale);
    void SetPlayTime(float Time);

public:
    bool CreateSequence(bool Loop, const struct FbxAnimationClip* Clip);
    bool CreateSequence(const std::string& Name, bool Loop,
        int StartFrame, int EndFrame, float PlayTime,
        const std::vector<BoneKeyFrame*>& vecFrame);
    bool CreateSequence(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = MESH_PATH);
    bool CreateSequenceFullPath(const std::string& Name,
        const TCHAR* FullPath);
    bool CreateSequenceMultibyte(const std::string& Name,
        const char* FileName,
        const std::string& PathName = MESH_PATH);
    bool CreateSequenceFullPathMultibyte(const std::string& Name,
        const char* FullPath);
    void SetShader();
    void ResetShader();
    void SetChangeShader();
    void ResetChangeShader();
    void Clear();
    void AddNotify(const std::string& Name, float Time);

public:
    CAnimationSequence* Clone();
};

