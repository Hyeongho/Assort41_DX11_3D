#pragma once

#include "../EngineInfo.h"
#include "../Resource/Animation/AnimationSequence2D.h"

struct Animation2DNotify
{
	std::string	Name;
	int		Frame;
	float	Time;
	bool	Call;
	std::function<void()>	Function;

	Animation2DNotify() :
		Frame(0),
		Time(0.f),
		Call(false)
	{
	}
};

class CAnimation2DData
{
	friend class CAnimation2D;

private:
	CAnimation2DData();
	CAnimation2DData(const CAnimation2DData& Anim);
	~CAnimation2DData();

private:
	class CAnimation2D* m_Owner;
	std::string	m_Name;
	std::string	m_SequenceName;
	CSharedPtr<CAnimationSequence2D>	m_Sequence;
	int		m_Frame;
	float	m_Time;
	float	m_FrameTime;
	float	m_PlayTime;
	float	m_PlayScale;
	bool	m_Loop;
	bool	m_Reverse;
	std::function<void()>	m_EndFunction;
	std::vector<Animation2DNotify*>	m_vecNotify;

public:
	void SetSequence(CAnimationSequence2D* Sequence);
	void Save(FILE* File);
	void Load(FILE* File);
	CAnimation2DData* Clone();

public:
	const std::string& GetName()	const
	{
		return m_Name;
	}

	int GetCurrentFrame()	const
	{
		return m_Frame;
	}

	float GetAnimationTime()	const
	{
		return m_Time;
	}

	class CAnimationSequence2D* GetAnimationSequence()	const
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
		Animation2DNotify* Notify = new Animation2DNotify;

		Notify->Name = Name;
		Notify->Frame = Frame;
		Notify->Time = Frame * m_FrameTime;
		Notify->Function = std::bind(Func, Obj);

		m_vecNotify.push_back(Notify);
	}

	template <typename T>
	void AddNotify(const std::string& Name, float Time, T* Obj, void(T::* Func)())
	{
		Animation2DNotify* Notify = new Animation2DNotify;

		Notify->Name = Name;
		Notify->Frame = Time / m_FrameTime;
		Notify->Time = Time;
		Notify->Function = std::bind(Func, Obj);

		m_vecNotify.push_back(Notify);
	}
};

