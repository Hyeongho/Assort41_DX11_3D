#include "CXmlParser.h"

#include <fstream>
#include "PathManager.h"


CXmlParser::CXmlParser()
{

}

CXmlParser::~CXmlParser()
{

}

void CXmlParser::init(const std::string& _strFile)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(DIALOG_PATH);
	std::wstring wPath = Path->Path;
	
	std::string strFilePath(wPath.begin(), wPath.end());
	strFilePath.append(_strFile);

	std::basic_ifstream<char> xmlFile(strFilePath);
	xmlFile.seekg(0, std::ios::end);
	unsigned int size = xmlFile.tellg();
	xmlFile.seekg(0);

	std::vector<char> xmlData(size + 1);
	xmlData[size] = 0;
	xmlFile.read(&xmlData.front(), (std::streamsize)size);

	xml_document<char> xmlDoc;
	xmlDoc.parse<parse_default>(&xmlData.front());

	xml_node<char>* pRootNode = xmlDoc.first_node();
	xml_attribute<char>* attr;

	for (xml_node<char>* item = pRootNode->first_node(); item; item = item->next_sibling()) {
		if (strcmp("Dialog", item->name()) == 0) {
			DialogTextInfo Info;


			for (attr = item->first_attribute(); attr; attr = attr->next_attribute()) {
				if (strcmp("Name", attr->name()) == 0) {
					Info.DialogName = attr->value();
				}
			}

			for (xml_node<char>* subItem = item->first_node(); subItem; subItem = subItem->next_sibling()) {
				for (xml_node<char>* contextItem = subItem->first_node(); contextItem; contextItem = contextItem->next_sibling()) {
					if (strcmp("Talker", contextItem->name()) == 0) {
						char* value = contextItem->value();

						int rq_cch = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value, static_cast<int>(strlen(value)), nullptr , 0);

						std::wstring wValue;
						wValue.clear();
						wValue.resize(rq_cch);

						MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value,
							static_cast<int>(strlen(value)),
							const_cast<wchar_t*>(wValue.c_str()),
							static_cast<int>(wValue.size()));

						Info.vecTalker.push_back(wValue);
					}

					if (strcmp("Text", contextItem->name()) == 0) {
						char* value = contextItem->value();

						int rq_cch = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value, static_cast<int>(strlen(value)), nullptr, 0);

						std::wstring wValue;
						wValue.clear();
						wValue.resize(rq_cch);

						MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value,
							static_cast<int>(strlen(value)),
							const_cast<wchar_t*>(wValue.c_str()),
							static_cast<int>(wValue.size()));

						Info.vecText.push_back(wValue);
					}

					if (strcmp("Anim", contextItem->name()) == 0) {
						char* value = contextItem->value();

						int rq_cch = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value, static_cast<int>(strlen(value)), nullptr, 0);

						std::wstring wValue;
						wValue.clear();
						wValue.resize(rq_cch);

						MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value,
							static_cast<int>(strlen(value)),
							const_cast<wchar_t*>(wValue.c_str()),
							static_cast<int>(wValue.size()));

						Info.vecAnim.push_back(wValue);
					}
				}
			}

			m_vecDialTextInfo.push_back(Info);
		}

	}

	xmlFile.close();
}

int CXmlParser::GetDialogCount()
{
	return static_cast<int>(m_vecDialTextInfo.size());
}

std::string CXmlParser::GetDialogInfoByIndex(int Idx, DialogInfo* Info) const
{

	size_t MaxSize = m_vecDialTextInfo[Idx].vecTalker.size();

	Info->TextIdx = 0;
	Info->TextMaxIdx = static_cast<int>(MaxSize);

	for (size_t size = 0; size < MaxSize; size++) {
		Info->vecTalker.push_back(m_vecDialTextInfo[Idx].vecTalker[size]);
		Info->vecText.push_back(m_vecDialTextInfo[Idx].vecText[size]);
		Info->vecAnim.push_back(m_vecDialTextInfo[Idx].vecAnim[size]);
	}


	return m_vecDialTextInfo[Idx].DialogName;
}
