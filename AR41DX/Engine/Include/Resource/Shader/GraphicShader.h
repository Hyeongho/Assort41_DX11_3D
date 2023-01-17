#pragma once
#include "Shader.h"
class CGraphicShader :
    public CShader
{
    friend class CShaderManager;

protected:
    CGraphicShader();
    virtual ~CGraphicShader() = 0;

private:
    ID3D11InputLayout* m_InputLayout;

    // D3D11_INPUT_ELEMENT_DESC 구조체 1개가 정점을 구성하는 요소 1개다.
    // 위치, 색상이라면 2개의 구조체가 작성되어 추가되어야 한다.
    std::vector<D3D11_INPUT_ELEMENT_DESC>   m_vecInputDesc;

    unsigned int    m_InputSize;

private:
    ID3D11VertexShader* m_VS;
    ID3DBlob* m_VSBlob; // VertexShader 코드를 가지고 있을 버퍼

    ID3D11PixelShader* m_PS;
    ID3DBlob* m_PSBlob;

    ID3D11HullShader* m_HS;
    ID3DBlob* m_HSBlob;

    ID3D11DomainShader* m_DS;
    ID3DBlob* m_DSBlob;

    ID3D11GeometryShader* m_GS;
    ID3DBlob* m_GSBlob;

public:
    virtual bool Init() = 0;
    virtual void SetShader();

public:
    void AddInputDesc(const char* SemanticName, unsigned int SemanticIndex,
        DXGI_FORMAT Fmt, unsigned int Slot, unsigned int Size,
        D3D11_INPUT_CLASSIFICATION SlotClass,
        unsigned int InstanceDataStepRate);
    bool CreateInputLayout();
    bool LoadVertexShader(const char* EntryName, const TCHAR* FileName,
        const std::string& PathName);
    bool LoadPixelShader(const char* EntryName, const TCHAR* FileName,
        const std::string& PathName);
    bool LoadHullShader(const char* EntryName, const TCHAR* FileName,
        const std::string& PathName);
    bool LoadDomainShader(const char* EntryName, const TCHAR* FileName,
        const std::string& PathName);
    bool LoadGeometryShader(const char* EntryName, const TCHAR* FileName,
        const std::string& PathName);
};

