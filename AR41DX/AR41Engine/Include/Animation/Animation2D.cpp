#include "Animation2D.h"
#include "../Component/SpriteComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"

std::unordered_map<std::string, CAnimation2D*> CAnimation2D::m_mapAnimationCDO;

CAnimation2D::CAnimation2D()	:
	m_Owner(nullptr),
	m_Play(true)
{
	m_ClassName = "Animation2D";
}

CAnimation2D::CAnimation2D(const CAnimation2D& Anim)	:
	CRef(Anim)
{
	m_Play = Anim.m_Play;

	m_mapAnimation.clear();

	auto	iter = Anim.m_mapAnimation.begin();
	auto	iterEnd = Anim.m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		CAnimation2DData* Data = iter->second->Clone();

		if (Data->GetName() == Anim.m_CurAnimation->GetName())
		{
			m_CurAnimation = Data;
		}

		m_mapAnimation.insert(std::make_pair(iter->first, Data));
	}
}

CAnimation2D::~CAnimation2D()
{
	auto	iter = m_mapAnimation.begin();
	auto	iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

CScene* CAnimation2D::GetScene() const
{
	if (!m_Owner)
		return nullptr;

	return m_Owner->GetScene();
}

void CAnimation2D::Start()
{
	if (m_Owner && m_CurAnimation)
	{
		m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
	}
}

bool CAnimation2D::Init()
{
	return true;
}

void CAnimation2D::Update(float DeltaTime)
{
	if (!m_Play || !m_CurAnimation ||
		m_CurAnimation->m_Sequence->GetFrameCount() == 0)
		return;

	m_CurAnimation->m_Time += DeltaTime * m_CurAnimation->m_PlayScale;

	bool	AnimEnd = false;

	m_CurAnimation->m_FrameTime = m_CurAnimation->m_PlayTime / 
		m_CurAnimation->m_Sequence->GetFrameCount();

	if (m_CurAnimation->m_Time >= m_CurAnimation->m_FrameTime)
	{
		m_CurAnimation->m_Time -= m_CurAnimation->m_FrameTime;

		if (m_CurAnimation->m_Reverse)
		{
			--m_CurAnimation->m_Frame;

			if (m_CurAnimation->m_Frame < 0)
				AnimEnd = true;
		}

		else
		{
			++m_CurAnimation->m_Frame;

			if (m_CurAnimation->m_Frame ==
				m_CurAnimation->m_Sequence->GetFrameCount())
				AnimEnd = true;
		}
	}

	size_t	Size = m_CurAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (!m_CurAnimation->m_vecNotify[i]->Call &&
			m_CurAnimation->m_vecNotify[i]->Frame ==
			m_CurAnimation->m_Frame)
		{
			m_CurAnimation->m_vecNotify[i]->Call = true;
			m_CurAnimation->m_vecNotify[i]->Function();
		}
	}

	if (AnimEnd)
	{
		if (m_CurAnimation->m_Loop)
		{
			if (m_CurAnimation->m_Reverse)
				m_CurAnimation->m_Frame = m_CurAnimation->m_Sequence->GetFrameCount() - 1;

			else
				m_CurAnimation->m_Frame = 0;

			Size = m_CurAnimation->m_vecNotify.size();

			for (size_t i = 0; i < Size; ++i)
			{
				m_CurAnimation->m_vecNotify[i]->Call = false;
			}
		}

		else
		{
			if (m_CurAnimation->m_Reverse)
				m_CurAnimation->m_Frame = 0;

			else
				m_CurAnimation->m_Frame = m_CurAnimation->m_Sequence->GetFrameCount() - 1;
		}

		if (m_CurAnimation->m_EndFunction)
			m_CurAnimation->m_EndFunction();
	}
}

bool CAnimation2D::AddAnimation(const std::string& Name, 
	const std::string& SequenceName, float PlayTime, float PlayScale, 
	bool Loop, bool Reverse)
{
	CAnimation2DData* Anim = FindAnimation(Name);

	if (Anim)
		return false;

	CAnimationSequence2D* Sequence = nullptr;

	if (m_Owner->GetScene())
		Sequence = m_Owner->GetScene()->GetResource()->FindAnimationSequence2D(SequenceName);

	else
		Sequence = CResourceManager::GetInst()->FindAnimationSequence2D(SequenceName);

	Anim = new CAnimation2DData;

	Anim->m_Sequence = Sequence;
	Anim->m_SequenceName = SequenceName;
	Anim->m_Name = Name;
	Anim->m_PlayTime = PlayTime;
	Anim->m_PlayScale = PlayScale;
	Anim->m_Loop = Loop;
	Anim->m_Reverse = Reverse;
	Anim->m_FrameTime = PlayTime / Sequence->GetFrameCount();
	Anim->m_Owner = this;

	if (m_mapAnimation.empty())
	{
		m_CurAnimation = Anim;

		if (m_Owner)
		{
			m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
		}
	}

	m_mapAnimation.insert(std::make_pair(Name, Anim));

	return true;
}

bool CAnimation2D::AddAnimation(const std::string& Name, 
	CAnimationSequence2D* Sequence,
	float PlayTime, float PlayScale, bool Loop, bool Reverse)
{
	CAnimation2DData* Anim = FindAnimation(Name);

	if (Anim)
		return false;

	Anim = new CAnimation2DData;

	Anim->m_Sequence = Sequence;
	Anim->m_SequenceName = Sequence->GetName();
	Anim->m_Name = Name;
	Anim->m_PlayTime = PlayTime;
	Anim->m_PlayScale = PlayScale;
	Anim->m_Loop = Loop;
	Anim->m_Reverse = Reverse;
	Anim->m_FrameTime = PlayTime / Sequence->GetFrameCount();
	Anim->m_Owner = this;

	if (m_mapAnimation.empty())
	{
		m_CurAnimation = Anim;

		if (m_Owner)
		{
			m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
		}
	}

	m_mapAnimation.insert(std::make_pair(Name, Anim));

	return true;
}

void CAnimation2D::SetPlayTime(const std::string& Name, float PlayTime)
{
	CAnimation2DData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_PlayTime = PlayTime;
}

void CAnimation2D::SetPlayScale(const std::string& Name, float PlayScale)
{
	CAnimation2DData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_PlayScale = PlayScale;
}

void CAnimation2D::SetLoop(const std::string& Name, bool Loop)
{
	CAnimation2DData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_Loop = Loop;
}

void CAnimation2D::SetReverse(const std::string& Name, bool Reverse)
{
	CAnimation2DData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_Reverse = Reverse;
}

void CAnimation2D::SetCurrentAnimation(const std::string& Name)
{
	CAnimation2DData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	m_CurAnimation = Anim;

	m_CurAnimation->m_Frame = 0;
	m_CurAnimation->m_Time = 0.f;

	size_t	Size = m_CurAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurAnimation->m_vecNotify[i]->Call = false;
	}

	if (m_Owner)
	{
		m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
		m_Owner->SetTextureFrameIndex(0);
	}
}

void CAnimation2D::ChangeAnimation(const std::string& Name)
{
	if (m_CurAnimation->GetName() == Name)
		return;

	m_CurAnimation->m_Frame = 0;
	m_CurAnimation->m_Time = 0.f;

	size_t	Size = m_CurAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurAnimation->m_vecNotify[i]->Call = false;
	}

	m_CurAnimation = FindAnimation(Name);

	m_CurAnimation->m_Frame = 0;
	m_CurAnimation->m_Time = 0.f;

	if (m_Owner)
	{
		m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
		m_Owner->SetTextureFrameIndex(0);
	}
}

void CAnimation2D::Save(FILE* File)
{
	CRef::Save(File);

	int	Count = (int)m_mapAnimation.size();

	fwrite(&Count, 4, 1, File);

	auto	iter = m_mapAnimation.begin();
	auto	iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->Save(File);
	}

	int	Length = (int)m_CurAnimation->GetName().length();

	fwrite(&Length, 4, 1, File);
	fwrite(m_CurAnimation->GetName().c_str(), 1, Length, File);
}

void CAnimation2D::Load(FILE* File)
{
	CRef::Load(File);

	auto	iter = m_mapAnimation.begin();
	auto	iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	m_mapAnimation.clear();

	m_CurAnimation = nullptr;


	int	Count = 0;

	fread(&Count, 4, 1, File);

	for (int i = 0; i < Count; ++i)
	{
		CAnimation2DData* Data = new CAnimation2DData;

		Data->m_Owner = this;
		Data->Load(File);

		m_mapAnimation.insert(std::make_pair(Data->GetName(), Data));
	}

	int	Length = 0;
	char	CurName[256] = {};

	fread(&Length, 4, 1, File);
	fread(CurName, 1, Length, File);

	m_CurAnimation = FindAnimation(CurName);
}

CAnimation2D* CAnimation2D::Clone()
{
	return new CAnimation2D(*this);
}

void CAnimation2D::SetShader()
{
	if (!m_CurAnimation || !m_CurAnimation->m_Sequence ||
		!m_CurAnimation->m_Sequence->GetTexture())
		return;

	CAnimation2DConstantBuffer* Buffer = CResourceManager::GetInst()->GetAnim2DConstantBuffer();

	const Animation2DFrameData& FrameData = m_CurAnimation->m_Sequence->GetFrameData(m_CurAnimation->m_Frame);

	EAnimation2DType	Type = m_CurAnimation->m_Sequence->GetAnim2DType();

	if (Type == EAnimation2DType::Atlas)
	{
		Buffer->SetImageSize((float)m_CurAnimation->m_Sequence->GetTexture()->GetWidth(),
			(float)m_CurAnimation->m_Sequence->GetTexture()->GetHeight());
		Buffer->SetImageFrame(FrameData.Start, FrameData.End);
	}

	else if (Type == EAnimation2DType::Frame)
	{
		if (m_Owner)
		{
			m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
			m_Owner->SetTextureFrameIndex(m_CurAnimation->m_Frame);
		}

		else
		{
			m_CurAnimation->m_Sequence->GetTexture()->SetShader(0,
				(int)EShaderBufferType::Pixel, m_CurAnimation->m_Frame);
		}
	}

	else
	{
		Buffer->SetFrame(m_CurAnimation->m_Frame);
	}
	
	Buffer->SetImageType(m_CurAnimation->m_Sequence->GetAnim2DType());
	Buffer->SetAnim2DEnable(true);

	Buffer->UpdateBuffer();
}

CAnimation2DData* CAnimation2D::FindAnimation(const std::string& Name)
{
	auto	iter = m_mapAnimation.find(Name);

	if (iter == m_mapAnimation.end())
		return nullptr;

	return iter->second;
}
