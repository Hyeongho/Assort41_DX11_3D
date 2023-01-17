#include "AnimationData.h"
#include "Animation.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"

CAnimationData::CAnimationData() :
	m_Time(0.f),
	m_FrameTime(0.f),
	m_PlayTime(1.f),
	m_PlayScale(1.f),
	m_Loop(false),
	m_Reverse(false)
{
}

CAnimationData::CAnimationData(const CAnimationData& Anim)
{
	m_Name = Anim.m_Name;
	m_SequenceName = Anim.m_SequenceName;
	m_Sequence = Anim.m_Sequence;

	m_Time = Anim.m_Time;
	m_FrameTime = Anim.m_FrameTime;
	m_PlayTime = Anim.m_PlayTime;
	m_PlayScale = Anim.m_PlayScale;
	m_Loop = Anim.m_Loop;
	m_Reverse = Anim.m_Reverse;
}

CAnimationData::~CAnimationData()
{
	size_t	Size = m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecNotify[i]);
	}
}

void CAnimationData::SetSequence(CAnimationSequence* Sequence)
{
	if (Sequence)
		m_SequenceName = Sequence->GetName();

	m_Sequence = Sequence;
}

void CAnimationData::Save(FILE* File)
{
	int	Length = (int)m_Name.length();
	fwrite(&Length, 4, 1, File);
	fwrite(m_Name.c_str(), 1, Length, File);

	Length = (int)m_SequenceName.length();
	fwrite(&Length, 4, 1, File);
	fwrite(m_SequenceName.c_str(), 1, Length, File);

	fwrite(&m_Time, 4, 1, File);
	fwrite(&m_FrameTime, 4, 1, File);
	fwrite(&m_PlayTime, 4, 1, File);
	fwrite(&m_PlayScale, 4, 1, File);

	fwrite(&m_Loop, 1, 1, File);
	fwrite(&m_Reverse, 1, 1, File);
}

void CAnimationData::Load(FILE* File)
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

	fread(&m_Time, 4, 1, File);
	fread(&m_FrameTime, 4, 1, File);
	fread(&m_PlayTime, 4, 1, File);
	fread(&m_PlayScale, 4, 1, File);

	fread(&m_Loop, 1, 1, File);
	fread(&m_Reverse, 1, 1, File);

	if (m_Owner->GetScene())
		m_Sequence = m_Owner->GetScene()->GetResource()->FindAnimationSequence(SequenceName);

	else
		m_Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);
}

CAnimationData* CAnimationData::Clone()
{
    return new CAnimationData(*this);
}
