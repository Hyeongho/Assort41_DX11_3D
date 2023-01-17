#pragma once
#include "Shader.h"
class CComputeShader :
    public CShader
{
    friend class CShaderManager;

protected:
    CComputeShader();
    virtual ~CComputeShader() = 0;

private:
    ID3D11ComputeShader* m_CS;
    ID3DBlob* m_CSBlob;

public:
    virtual bool Init() = 0;
    virtual void SetShader();

public:
    bool LoadComputeShader(const char* EntryName, const TCHAR* FileName,
        const std::string& PathName);
    void Excute(unsigned int x, unsigned int y, unsigned int z);
};

