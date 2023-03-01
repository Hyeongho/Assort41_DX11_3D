#include "Sequence.h"

bool CSequence::Run()
{
	std::vector<CNode*> Children = GetChildren();

	if (Children.empty())
	{
		return false;
	}

	auto iter = Children.begin();
	auto iterEnd = Children.end();

	for (; iter != iterEnd; iter++)
	{
		if (!(*iter)->Run())
		{
			return false;
		}
	}

	return true;
}

bool CSequence::Run(CGameObject* Object)
{
	if (!Object)
	{
		return false;
	}

	std::vector<CNode*> Children = GetChildren();

	if (Children.empty())
	{
		return false;
	}

	auto iter = Children.begin();
	auto iterEnd = Children.end();

	for (; iter != iterEnd; iter++)
	{
		if (!(*iter)->Run(Object))
		{
			return false;
		}
	}

	return true;
}
