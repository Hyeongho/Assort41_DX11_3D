#include "GraphicShader.h"
#include "../../Device.h"
#include "../../PathManager.h"

CGraphicShader::CGraphicShader() :
	m_VS(nullptr),
	m_VSBlob(nullptr),
	m_PS(nullptr),
	m_PSBlob(nullptr),
	m_HS(nullptr),
	m_HSBlob(nullptr),
	m_DS(nullptr),
	m_DSBlob(nullptr),
	m_GS(nullptr),
	m_GSBlob(nullptr),
	m_InputSize(0),
	m_InputLayout(nullptr)
{
}

CGraphicShader::~CGraphicShader()
{
	SAFE_RELEASE(m_InputLayout);

	SAFE_RELEASE(m_VS);
	SAFE_RELEASE(m_PS);
	SAFE_RELEASE(m_HS);
	SAFE_RELEASE(m_DS);
	SAFE_RELEASE(m_GS);

	SAFE_RELEASE(m_VSBlob);
	SAFE_RELEASE(m_PSBlob);
	SAFE_RELEASE(m_HSBlob);
	SAFE_RELEASE(m_DSBlob);
	SAFE_RELEASE(m_GSBlob);
}

void CGraphicShader::SetShader()
{
	CDevice::GetInst()->GetContext()->VSSetShader(m_VS, nullptr, 0);
	CDevice::GetInst()->GetContext()->PSSetShader(m_PS, nullptr, 0);
	CDevice::GetInst()->GetContext()->HSSetShader(m_HS, nullptr, 0);
	CDevice::GetInst()->GetContext()->DSSetShader(m_DS, nullptr, 0);
	CDevice::GetInst()->GetContext()->GSSetShader(m_GS, nullptr, 0);

	CDevice::GetInst()->GetContext()->IASetInputLayout(m_InputLayout);
}

void CGraphicShader::AddInputDesc(const char* SemanticName, 
	unsigned int SemanticIndex, DXGI_FORMAT Fmt, 
	unsigned int Slot, unsigned int Size, 
	D3D11_INPUT_CLASSIFICATION SlotClass, 
	unsigned int InstanceDataStepRate)
{
	D3D11_INPUT_ELEMENT_DESC	Desc = {};

	Desc.SemanticName = SemanticName;
	Desc.SemanticIndex = SemanticIndex;
	Desc.Format = Fmt;
	Desc.InputSlot = Slot;
	Desc.InputSlotClass = SlotClass;
	Desc.InstanceDataStepRate = InstanceDataStepRate;

	Desc.AlignedByteOffset = m_InputSize;

	m_InputSize += Size;

	m_vecInputDesc.push_back(Desc);
}

bool CGraphicShader::CreateInputLayout()
{
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateInputLayout(
		&m_vecInputDesc[0], (UINT)m_vecInputDesc.size(),
		m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(),
		&m_InputLayout)))
		return false;

	return true;
}

bool CGraphicShader::LoadVertexShader(const char* EntryName,
	const TCHAR* FileName, const std::string& PathName)
{
	SAFE_RELEASE(m_VS);
	SAFE_RELEASE(m_VSBlob);

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

	// ErrorBlob은 에러가 발생했을 경우 에러의 내용을 저장하기 위한 버퍼이다.
	if (FAILED(D3DCompileFromFile(FullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryName, "vs_5_0",
		Flag, 0, &m_VSBlob, &ErrorBlob)))
	{
		OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// 위에서 컴파일한 코드를 이용해서 VertexShader 객체를 만들어낸다.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateVertexShader(
		m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(),
		nullptr, &m_VS)))
		return false;

	return true;
}

bool CGraphicShader::LoadPixelShader(const char* EntryName,
	const TCHAR* FileName, const std::string& PathName)
{
	SAFE_RELEASE(m_PS);
	SAFE_RELEASE(m_PSBlob);

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

	// ErrorBlob은 에러가 발생했을 경우 에러의 내용을 저장하기 위한 버퍼이다.
	if (FAILED(D3DCompileFromFile(FullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryName, "ps_5_0",
		Flag, 0, &m_PSBlob, &ErrorBlob)))
	{
		OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// 위에서 컴파일한 코드를 이용해서 VertexShader 객체를 만들어낸다.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreatePixelShader(
		m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize(),
		nullptr, &m_PS)))
		return false;

	return true;
}

bool CGraphicShader::LoadHullShader(const char* EntryName, 
	const TCHAR* FileName, const std::string& PathName)
{
	SAFE_RELEASE(m_HS);
	SAFE_RELEASE(m_HSBlob);

	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	TCHAR FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		lstrcpy(FullPath, Info->Path);

	lstrcat(FullPath, FileName);

	ID3DBlob* ErrorBlob = nullptr;

	// ErrorBlob은 에러가 발생했을 경우 에러의 내용을 저장하기 위한 버퍼이다.
	if (FAILED(D3DCompileFromFile(FullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryName, "hs_5_0",
		Flag, 0, &m_HSBlob, &ErrorBlob)))
	{
		OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// 위에서 컴파일한 코드를 이용해서 VertexShader 객체를 만들어낸다.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateHullShader(
		m_HSBlob->GetBufferPointer(), m_HSBlob->GetBufferSize(),
		nullptr, &m_HS)))
		return false;

	return true;
}

bool CGraphicShader::LoadDomainShader(const char* EntryName,
	const TCHAR* FileName, const std::string& PathName)
{
	SAFE_RELEASE(m_DS);
	SAFE_RELEASE(m_DSBlob);

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

	// ErrorBlob은 에러가 발생했을 경우 에러의 내용을 저장하기 위한 버퍼이다.
	if (FAILED(D3DCompileFromFile(FullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryName, "ds_5_0",
		Flag, 0, &m_DSBlob, &ErrorBlob)))
	{
		OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// 위에서 컴파일한 코드를 이용해서 VertexShader 객체를 만들어낸다.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateDomainShader(
		m_DSBlob->GetBufferPointer(), m_DSBlob->GetBufferSize(),
		nullptr, &m_DS)))
		return false;

	return true;
}

bool CGraphicShader::LoadGeometryShader(const char* EntryName, 
	const TCHAR* FileName, const std::string& PathName)
{
	SAFE_RELEASE(m_GS);
	SAFE_RELEASE(m_GSBlob);

	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	TCHAR FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		lstrcpy(FullPath, Info->Path);

	lstrcat(FullPath, FileName);

	ID3DBlob* ErrorBlob = nullptr;

	// ErrorBlob은 에러가 발생했을 경우 에러의 내용을 저장하기 위한 버퍼이다.
	if (FAILED(D3DCompileFromFile(FullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryName, "gs_5_0",
		Flag, 0, &m_GSBlob, &ErrorBlob)))
	{
		OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// 위에서 컴파일한 코드를 이용해서 VertexShader 객체를 만들어낸다.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateGeometryShader(
		m_GSBlob->GetBufferPointer(), m_GSBlob->GetBufferSize(),
		nullptr, &m_GS)))
		return false;

	return true;
}
