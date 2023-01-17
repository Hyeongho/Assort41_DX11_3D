
#include "ComputeShader.h"
#include "../../Device.h"
#include "../../PathManager.h"

CComputeShader::CComputeShader()	:
	m_CS(nullptr),
	m_CSBlob(nullptr)
{
	m_ShaderType = ShaderType::Compute;
}

CComputeShader::~CComputeShader()
{
	SAFE_RELEASE(m_CS);
	SAFE_RELEASE(m_CSBlob);
}

void CComputeShader::SetShader()
{
}

bool CComputeShader::LoadComputeShader(const char* EntryName, const TCHAR* FileName, const std::string& PathName)
{
	SAFE_RELEASE(m_CS);
	SAFE_RELEASE(m_CSBlob);

	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	TCHAR	FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		lstrcpy(FullPath, Info->Path);

	lstrcat(FullPath, FileName);

	ID3DBlob* ErrorBlob = nullptr;

	// ErrorBlob�� ������ �߻����� ��� ������ ������ �����ϱ� ���� �����̴�.
	if (FAILED(D3DCompileFromFile(FullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryName, "cs_5_0",
		Flag, 0, &m_CSBlob, &ErrorBlob)))
	{
		OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// ������ �������� �ڵ带 �̿��ؼ� VertexShader ��ü�� ������.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateComputeShader(
		m_CSBlob->GetBufferPointer(), m_CSBlob->GetBufferSize(),
		nullptr, &m_CS)))
		return false;

	return true;
}

void CComputeShader::Excute(unsigned int x, unsigned int y, 
	unsigned int z)
{
	CDevice::GetInst()->GetContext()->CSSetShader(m_CS, nullptr, 0);

	CDevice::GetInst()->GetContext()->Dispatch(x, y, z);
	//CDevice::GetInst()->GetContext()->CSSetShader(nullptr, nullptr, 0);
}
