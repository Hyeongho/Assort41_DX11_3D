#pragma once

#include "Engine.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "../DialogUI.h"

using namespace rapidxml;

struct DialogTextInfo {
	std::string DialogName;
	std::vector<std::string> vecTalker;
	std::vector<std::string> vecText;

	DialogTextInfo() :
		DialogName("")
	{}
};

class CXmlParser
{
private :
	std::vector<DialogTextInfo> m_vecDialTextInfo;

public :
	CXmlParser();
	~CXmlParser();

public :
	void init(const std::string& _strFile);

public :
	int GetDialogCount();
	std::string GetDialogInfoByIndex(int Idx, DialogInfo* Info) const;
};

