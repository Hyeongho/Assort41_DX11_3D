#pragma once

#include "EngineInfo.h"

class CRef
{
public:
	CRef();
	CRef(const CRef& ref);
	virtual ~CRef();

protected:
	int		m_RefCount;
	std::string	m_Name;
	std::string	m_TypeName;
	size_t	m_TypeID;
	bool	m_Enable;	// 활성, 비활성
	bool	m_Active;	// 살아 있는지 죽었는지

public:
	void AddRef();
	int Release();


public:
	int GetRefCount()	const
	{
		return m_RefCount;
	}

	bool GetEnable()	const
	{
		return m_Enable;
	}

	bool GetActive()	const
	{
		return m_Active;
	}

	size_t GetTypeID()	const
	{
		return m_TypeID;
	}

	const std::string& GetTypeName()	const
	{
		return m_TypeName;
	}

	const std::string& GetName()	const
	{
		return m_Name;
	}

	void SetEnable(bool Enable)
	{
		m_Enable = Enable;
	}

	virtual void Destroy()
	{
		m_Active = false;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

	template <typename T>
	bool CheckTypeID()	const
	{
		return m_TypeID == typeid(T).hash_code();
	}

public:
	template <typename T>
	void SetTypeID()
	{
		// 타입 이름을 문자열로 얻어온다.
		m_TypeName = typeid(T).name();

		// 타입의 고유한 번호를 얻어온다.
		m_TypeID = typeid(T).hash_code();
	}

public:
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

