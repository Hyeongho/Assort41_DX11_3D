
#include "ConstantBufferData.h"
#include "../ResourceManager.h"

CConstantBufferData::CConstantBufferData()
{
}

CConstantBufferData::CConstantBufferData(const CConstantBufferData& Buffer)
{
	SetConstantBuffer("MS");

	m_Buffer = Buffer.m_Buffer;
}

CConstantBufferData::~CConstantBufferData()
{
}

void CConstantBufferData::SetConstantBuffer(const std::string& Name)
{
	m_Buffer = CResourceManager::GetInst()->FindConstantBuffer(Name);
}
