
#include "Animation2DData.h"
#include "Animation2D.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"

CAnimation2DData::CAnimation2DData()	:
	m_Frame(0),
	m_Time(0.f),
	m_FrameTime(0.f),
	m_PlayTime(0.f),
	m_PlayScale(0.f),
	m_Loop(false),
	m_Reverse(false)
{
}

CAnimation2DData::CAnimation2DData(const CAnimation2DData& Anim)
{
	m_Name = Anim.m_Name;
	m_SequenceName = Anim.m_SequenceName;
	m_Sequence = Anim.m_Sequence;

	m_Frame = Anim.m_Frame;
	m_Time = Anim.m_Time;
	m_FrameTime = Anim.m_FrameTime;
	m_PlayTime = Anim.m_PlayTime;
	m_PlayScale = Anim.m_PlayScale;
	m_Loop = Anim.m_Loop;
	m_Reverse = Anim.m_Reverse;
}

CAnimation2DData::~CAnimation2DData()
{
	size_t	Size = m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecNotify[i]);
	}
}

void CAnimation2DData::SetSequence(CAnimationSequence2D* Sequence)
{
	if (Sequence)
		m_SequenceName = Sequence->GetName();

	m_Sequence = Sequence;
}

void CAnimation2DData::Save(FILE* File)
{
	int	Length = (int)m_Name.length();
	fwrite(&Length, 4, 1, File);
	fwrite(m_Name.c_str(), 1, Length, File);

	Length = (int)m_SequenceName.length();
	fwrite(&Length, 4, 1, File);
	fwrite(m_SequenceName.c_str(), 1, Length, File);

	fwrite(&m_Frame, 4, 1, File);
	fwrite(&m_Time, 4, 1, File);
	fwrite(&m_FrameTime, 4, 1, File);
	fwrite(&m_PlayTime, 4, 1, File);
	fwrite(&m_PlayScale, 4, 1, File);

	fwrite(&m_Loop, 1, 1, File);
	fwrite(&m_Reverse, 1, 1, File);
}

void CAnimation2DData::Load(FILE* File)
{
	int	Length = 0;
	char	Name[256] = {};
	fread(&Length, 4, 1, File);
	fread(Name, 1, Length, File);

	m_Name = Name;

	Length = 0;
	char	SequenceName[256] = {};
	fread(&Length, 4, 1, File);
	fread(SequenceName, 1, Length, File);

	m_SequenceName = SequenceName;

	fread(&m_Frame, 4, 1, File);
	fread(&m_Time, 4, 1, File);
	fread(&m_FrameTime, 4, 1, File);
	fread(&m_PlayTime, 4, 1, File);
	fread(&m_PlayScale, 4, 1, File);

	fread(&m_Loop, 1, 1, File);
	fread(&m_Reverse, 1, 1, File);

	if (m_Owner->GetScene())
		m_Sequence = m_Owner->GetScene()->GetResource()->FindAnimationSequence2D(SequenceName);

	else
		m_Sequence = CResourceManager::GetInst()->FindAnimationSequence2D(SequenceName);
}

CAnimation2DData* CAnimation2DData::Clone()
{
	return new CAnimation2DData(*this);
}
