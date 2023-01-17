#pragma once

#include "../EngineInfo.h"
#include "../Resource/Animation/AnimationSequence.h"

struct AnimationNotify
{
	std::string	Name;
	int		Frame;
	float	Time;
	bool	Call;
	std::function<void()>	Function;

	AnimationNotify() :
		Frame(0),
		Time(0.f),
		Call(false)
	{
	}
};

class CAnimationData
{
	friend class CAnimation;

private:
	CAnimationData();
	CAnimationData(const CAnimationData& Anim);
	~CAnimationData();

private:
	class CAnimation* m_Owner;
	std::string	m_Name;
	std::string	m_SequenceName;
	CSharedPtr<CAnimationSequence>	m_Sequence;
	float	m_Time;
	float	m_FrameTime;
	float	m_PlayTime;
	float	m_PlayScale;
	bool	m_Loop;
	bool	m_Reverse;
	std::function<void()>	m_EndFunction;
	std::vector<AnimationNotify*>	m_vecNotify;

public:
	void SetSequence(CAnimationSequence* Sequence);
	void Save(FILE* File);
	void Load(FILE* File);
	CAnimationData* Clone();

public:
	const std::string& GetName()	const
	{
		return m_Name;
	}

	float GetAnimationTime()	const
	{
		return m_Time;
	}

	class CAnimationSequence* GetAnimationSequence()	const
	{
		return m_Sequence;
	}

public:
	template <typename T>
	void SetEndFunction(T* Obj, void(T::* Func)())
	{
		m_EndFunction = std::bind(Func, Obj);
	}

	template <typename T>
	void AddNotify(const std::string& Name, int Frame, T* Obj, void(T::* Func)())
	{
		AnimationNotify* Notify = new AnimationNotify;

		Notify->Name = Name;
		Notify->Frame = Frame;
		Notify->Time = Frame * m_FrameTime;
		Notify->Function = std::bind(Func, Obj);

		m_vecNotify.push_back(Notify);
	}

	template <typename T>
	void AddNotify(const std::string& Name, float Time, T* Obj, void(T::* Func)())
	{
		AnimationNotify* Notify = new AnimationNotify;

		Notify->Name = Name;
		Notify->Frame = Time / m_FrameTime;
		Notify->Time = Time;
		Notify->Function = std::bind(Func, Obj);

		m_vecNotify.push_back(Notify);
	}
};

