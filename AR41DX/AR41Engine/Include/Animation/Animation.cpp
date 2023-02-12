#include "Animation.h"
#include "../Component/AnimationMeshComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/AnimationUpdateConstantBuffer.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Resource/Animation/Skeleton.h"
#include "../Resource/Shader/AnimationUpdateShader.h"
#include "../Device.h"

std::unordered_map<std::string, CAnimation*> CAnimation::m_mapAnimationCDO;

CAnimation::CAnimation() :
	m_Owner(nullptr),
	m_Play(true),
	m_CurAnimation(nullptr),
	m_ChangeAnimation(nullptr),
	m_InstancingBoneBuffer(nullptr),
	m_GlobalTime(0.f),
	m_SequenceProgress(0.f),
	m_ChangeTimeAcc(0.f),
	m_ChangeTime(0.2f)
{
	m_ClassName = "Animation";
	SetTypeID<CAnimation>();
	m_AnimationUpdateCBuffer = new CAnimationUpdateConstantBuffer;
	m_OutputBuffer = new CStructuredBuffer;
	m_BoneBuffer = new CStructuredBuffer;
}

CAnimation::CAnimation(const CAnimation& Anim) :
	CRef(Anim)
{
	m_Play = Anim.m_Play;

	m_GlobalTime = 0.f;
	m_SequenceProgress = 0.f;
	m_ChangeTimeAcc = 0.f;
	m_ChangeTime = Anim.m_ChangeTime;

	m_AnimationUpdateShader = Anim.m_AnimationUpdateShader;

	if (Anim.m_AnimationUpdateCBuffer)
	{
		m_AnimationUpdateCBuffer = Anim.m_AnimationUpdateCBuffer->Clone();
	}


	if (Anim.m_OutputBuffer)
	{
		m_OutputBuffer = Anim.m_OutputBuffer->Clone();
	}

	if (Anim.m_BoneBuffer)
	{
		m_BoneBuffer = Anim.m_BoneBuffer->Clone();
	}

	m_Skeleton = Anim.m_Skeleton;

	m_mapAnimation.clear();

	auto iter = Anim.m_mapAnimation.begin();
	auto iterEnd = Anim.m_mapAnimation.end();

	for (; iter != iterEnd; iter++)
	{
		CAnimationData* Data = iter->second->Clone();

		if (Data->GetName() == Anim.m_CurAnimation->GetName())
		{
			m_CurAnimation = Data;
		}

		m_mapAnimation.insert(std::make_pair(iter->first, Data));
	}
}

CAnimation::~CAnimation()
{
	SAFE_DELETE(m_OutputBuffer);
	SAFE_DELETE(m_BoneBuffer);
	SAFE_DELETE(m_AnimationUpdateCBuffer);

	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE(iter->second);
	}
}

CScene* CAnimation::GetScene() const
{
	if (!m_Owner)
	{
		return nullptr;
	}

	return m_Owner->GetScene();
}

void CAnimation::SetSkeleton(CSkeleton* Skeleton)
{
	m_Skeleton = Skeleton;
}

void CAnimation::Start()
{
	m_AnimationUpdateShader = (CComputeShader*)CResourceManager::GetInst()->FindShader("AnimationUpdateShader");



	m_AnimationUpdateCBuffer->Init();

	m_AnimationUpdateCBuffer->SetBoneCount((int)m_Skeleton->GetBoneCount());

	m_vecBoneInfo.resize(m_Skeleton->GetBoneCount());



	m_OutputBuffer->Init("OutputBone", sizeof(Matrix), (unsigned int)m_Skeleton->GetBoneCount(), 0);



	m_BoneBuffer->Init("OutputBone", sizeof(OutputBoneInfo), (unsigned int)m_Skeleton->GetBoneCount(), 1,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
		D3D11_CPU_ACCESS_READ);
}

bool CAnimation::Init()
{
	return true;
}

void CAnimation::Update(float DeltaTime)
{
	if (!m_Play || !m_CurAnimation || !m_Skeleton || m_CurAnimation->m_Sequence->GetKeyFrameCount() == 0)
	{
		return;
	}

	m_GlobalTime += DeltaTime * m_CurAnimation->m_PlayScale;

	bool Change = false;
	bool ChangeEnd = false;
	bool AnimEnd = false;

	if (m_ChangeAnimation)
	{
		Change = true;
		m_ChangeTimeAcc += DeltaTime;

		if (m_ChangeTimeAcc >= m_ChangeTime)
		{
			m_ChangeTimeAcc = m_ChangeTime;
			ChangeEnd = true;
		}
	}

	else
	{
		m_SequenceProgress = m_GlobalTime / m_CurAnimation->m_PlayTime;

		if (m_GlobalTime >= m_CurAnimation->m_PlayTime)
		{
			//m_GlobalTime -= m_CurAnimation->m_PlayTime;
			m_GlobalTime = m_CurAnimation->m_PlayTime;

			AnimEnd = true;
		}

		m_AnimationUpdateCBuffer->SetChangeAnimation(false);
		m_AnimationUpdateCBuffer->SetChangeRatio(0.f);
		m_AnimationUpdateCBuffer->SetChangeFrameCount(0);

		float	AnimationTime = m_GlobalTime + m_CurAnimation->m_Sequence->m_StartTime;

		int	StartFrame = 0;
		int	EndFrame = m_CurAnimation->m_Sequence->m_FrameLength;

		int	FrameIndex = (int)(m_GlobalTime / m_CurAnimation->m_Sequence->m_FrameTime);
		int	NextFrameIndex = FrameIndex + 1;

		if (FrameIndex >= EndFrame)
		{
			FrameIndex = EndFrame - 1;
		}

		if (NextFrameIndex >= EndFrame)
		{
			NextFrameIndex = EndFrame - 1;
		}

		float Ratio = (m_GlobalTime - m_CurAnimation->m_Sequence->m_FrameTime * FrameIndex) / m_CurAnimation->m_Sequence->m_FrameTime;

		m_AnimationUpdateCBuffer->SetFrameCount(EndFrame);
		m_AnimationUpdateCBuffer->SetCurrentFrame(FrameIndex);
		m_AnimationUpdateCBuffer->SetNextFrame(NextFrameIndex);
		m_AnimationUpdateCBuffer->SetRatio(Ratio);

		size_t	Size = m_CurAnimation->m_vecNotify.size();

		for (size_t i = 0; i < Size; ++i)
		{
			if (!m_CurAnimation->m_vecNotify[i]->Call && m_CurAnimation->m_vecNotify[i]->Frame == FrameIndex)
			{
				m_CurAnimation->m_vecNotify[i]->Call = true;
				m_CurAnimation->m_vecNotify[i]->Function();
			}
		}

		if (AnimEnd)
		{
			if (m_CurAnimation->m_EndFunction)
			{
				m_CurAnimation->m_EndFunction();
			}

			if (m_CurAnimation->m_Loop)
			{
				m_GlobalTime = 0.f;
				size_t	Size = m_CurAnimation->m_vecNotify.size();

				for (size_t i = 0; i < Size; ++i)
				{
					m_CurAnimation->m_vecNotify[i]->Call = false;
				}
			}
		}
	}

	if (Change)
	{
		m_AnimationUpdateCBuffer->SetChangeRatio(m_ChangeTimeAcc / m_ChangeTime);
		m_AnimationUpdateCBuffer->SetChangeAnimation(true);
		m_AnimationUpdateCBuffer->SetChangeFrameCount(m_ChangeAnimation->m_Sequence->m_FrameLength);

		m_ChangeAnimation->m_Sequence->SetChangeShader();
	}

	m_AnimationUpdateCBuffer->SetRowIndex(m_Owner->GetInstanceID());

	m_OutputBuffer->SetShader();

	m_AnimationUpdateCBuffer->UpdateBuffer();

	m_Skeleton->SetShader();

	m_BoneBuffer->SetShader();

	m_InstancingBoneBuffer->SetShader();

	m_CurAnimation->m_Sequence->SetShader();

	unsigned int GroupX = (unsigned int)m_Skeleton->GetBoneCount() / 256 + 1;

	m_AnimationUpdateShader->Excute(GroupX, 1, 1);


	m_InstancingBoneBuffer->ResetShader();

	m_BoneBuffer->ResetShader();

	m_CurAnimation->m_Sequence->ResetShader();

	m_Skeleton->ResetShader();

	m_OutputBuffer->ResetShader();

	if (Change)
	{
		m_ChangeAnimation->m_Sequence->ResetChangeShader();
	}

	if (ChangeEnd)
	{
		m_CurAnimation = m_ChangeAnimation;
		m_ChangeAnimation = nullptr;
		m_ChangeTimeAcc = 0.f;
		m_GlobalTime = 0.f;
	}

	// 구조화 버퍼에 있는 본 정보를 DataBuffer로 복사한다.
	//CDevice::GetInst()->GetContext()->CopyResource(m_BoneDataBuffer, m_BoneBuffer->GetBuffer());

	D3D11_MAPPED_SUBRESOURCE	Map = {};

	CDevice::GetInst()->GetContext()->Map(m_BoneBuffer->GetBuffer(), 0, D3D11_MAP_READ, 0, &Map);

	memcpy(&m_vecBoneInfo[0], Map.pData, sizeof(OutputBoneInfo) * m_vecBoneInfo.size());;

	CDevice::GetInst()->GetContext()->Unmap(m_BoneBuffer->GetBuffer(), 0);


	m_Skeleton->Update(DeltaTime, m_vecBoneInfo, m_Owner);
}

bool CAnimation::AddAnimation(const std::string& Name,
	const std::string& SequenceName, float PlayTime, float PlayScale,
	bool Loop, bool Reverse)
{
	CAnimationData* Anim = FindAnimation(Name);

	if (Anim)
	{
		return false;
	}

	CAnimationSequence* Sequence = nullptr;

	if (m_Owner->GetScene())
	{
		Sequence = m_Owner->GetScene()->GetResource()->FindAnimationSequence(SequenceName);
	}

	else
	{
		Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);
	}

	Anim = new CAnimationData;

	Anim->m_Sequence = Sequence;
	Anim->m_SequenceName = SequenceName;
	Anim->m_Name = Name;
	Anim->m_PlayTime = PlayTime;
	Anim->m_PlayScale = PlayScale;
	Anim->m_Loop = Loop;
	Anim->m_Reverse = Reverse;
	Anim->m_FrameTime = PlayTime / Sequence->GetKeyFrameCount();
	Anim->m_Owner = this;

	if (m_mapAnimation.empty())
	{
		m_CurAnimation = Anim;
	}

	m_mapAnimation.insert(std::make_pair(Name, Anim));

	return true;
}

bool CAnimation::AddAnimation(const std::string& Name,
	CAnimationSequence* Sequence,
	float PlayTime, float PlayScale, bool Loop, bool Reverse)
{
	CAnimationData* Anim = FindAnimation(Name);

	if (Anim)
	{
		return false;
	}

	Anim = new CAnimationData;

	Anim->m_Sequence = Sequence;
	Anim->m_SequenceName = Sequence->GetName();
	Anim->m_Name = Name;
	Anim->m_PlayTime = PlayTime;
	Anim->m_PlayScale = PlayScale;
	Anim->m_Loop = Loop;
	Anim->m_Reverse = Reverse;
	Anim->m_FrameTime = PlayTime / Sequence->GetKeyFrameCount();
	Anim->m_Owner = this;

	if (m_mapAnimation.empty())
	{
		m_CurAnimation = Anim;
	}

	m_mapAnimation.insert(std::make_pair(Name, Anim));

	return true;
}

void CAnimation::SetPlayTime(const std::string& Name, float PlayTime)
{
	CAnimationData* Anim = FindAnimation(Name);

	if (!Anim)
	{
		return;
	}

	Anim->m_PlayTime = PlayTime;
}

void CAnimation::SetPlayScale(const std::string& Name, float PlayScale)
{
	CAnimationData* Anim = FindAnimation(Name);

	if (!Anim)
	{
		return;
	}

	Anim->m_PlayScale = PlayScale;
}

void CAnimation::SetLoop(const std::string& Name, bool Loop)
{
	CAnimationData* Anim = FindAnimation(Name);

	if (!Anim)
	{
		return;
	}

	Anim->m_Loop = Loop;
}

void CAnimation::SetReverse(const std::string& Name, bool Reverse)
{
	CAnimationData* Anim = FindAnimation(Name);

	if (!Anim)
	{
		return;
	}

	Anim->m_Reverse = Reverse;
}

void CAnimation::SetCurrentAnimation(const std::string& Name)
{
	CAnimationData* Anim = FindAnimation(Name);

	if (!Anim)
	{
		return;
	}

	m_CurAnimation = Anim;

	m_CurAnimation->m_Time = 0.f;
	m_GlobalTime = 0.f;

	size_t Size = m_CurAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurAnimation->m_vecNotify[i]->Call = false;
	}
}

void CAnimation::ChangeAnimation(const std::string& Name)
{
	if (m_CurAnimation->GetName() == Name)
	{
		return;
	}

	m_ChangeAnimation = FindAnimation(Name);

	if (!m_ChangeAnimation)
	{
		return;
	}

	size_t	Size = m_CurAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurAnimation->m_vecNotify[i]->Call = false;
	}

	m_CurAnimation->m_Time = 0.f;
}

void CAnimation::Save(FILE* File)
{
	CRef::Save(File);

	int	Count = (int)m_mapAnimation.size();

	fwrite(&Count, 4, 1, File);

	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; iter++)
	{
		iter->second->Save(File);
	}

	int	Length = (int)m_CurAnimation->GetName().length();

	fwrite(&Length, 4, 1, File);
	fwrite(m_CurAnimation->GetName().c_str(), 1, Length, File);
}

void CAnimation::Load(FILE* File)
{
	CRef::Load(File);

	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE(iter->second);
	}

	m_mapAnimation.clear();

	m_CurAnimation = nullptr;


	int	Count = 0;

	fread(&Count, 4, 1, File);

	for (int i = 0; i < Count; ++i)
	{
		CAnimationData* Data = new CAnimationData;

		Data->m_Owner = this;
		Data->Load(File);

		m_mapAnimation.insert(std::make_pair(Data->GetName(), Data));
	}

	int	Length = 0;
	char CurName[256] = {};

	fread(&Length, 4, 1, File);
	fread(CurName, 1, Length, File);

	m_CurAnimation = FindAnimation(CurName);
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::SetShader()
{
	m_OutputBuffer->SetShader(11, (int)EShaderBufferType::Vertex);
}

void CAnimation::ResetShader()
{
	m_OutputBuffer->ResetShader(11, (int)EShaderBufferType::Vertex);
}

CAnimationData* CAnimation::FindAnimation(const std::string& Name)
{
	auto iter = m_mapAnimation.find(Name);

	if (iter == m_mapAnimation.end())
	{
		return nullptr;
	}

	return iter->second;
}
