#pragma once

#include "../../Ref.h"

struct TextureResourceInfo
{
	DirectX::ScratchImage* Image;
	ID3D11ShaderResourceView* SRV;
	unsigned int	Width;
	unsigned int	Height;
	TCHAR* FileName;
	char* PathName;

	TextureResourceInfo() :
		Image(nullptr),
		SRV(nullptr),
		Width(0),
		Height(0)
	{
		FileName = new TCHAR[MAX_PATH];
		PathName = new char[MAX_PATH];

		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);
		memset(PathName, 0, sizeof(char) * MAX_PATH);
	}

	~TextureResourceInfo()
	{
		SAFE_RELEASE(SRV);
		SAFE_DELETE_ARRAY(FileName);
		SAFE_DELETE_ARRAY(PathName);
		SAFE_DELETE(Image);
	}
};

class CTexture :
	public CRef
{
	friend class CTextureManager;

protected:
	CTexture();
	virtual ~CTexture();

protected:
	class CScene* m_Scene;
	std::vector<TextureResourceInfo*>	m_vecTextureInfo;
	EImageType	m_ImageType;
	ID3D11ShaderResourceView* m_ArraySRV;

public:
	EImageType GetImageType()	const
	{
		return m_ImageType;
	}

	ID3D11ShaderResourceView* GetResource(int Index = 0)	const
	{
		return m_vecTextureInfo[Index]->SRV;
	}

	unsigned int GetWidth(int Index = 0)	const
	{
		return m_vecTextureInfo[Index]->Width;
	}

	unsigned int GetHeight(int Index = 0)	const
	{
		return m_vecTextureInfo[Index]->Height;
	}

	TCHAR* GetFullPath(int index = 0)	const
	{
		return m_vecTextureInfo[index]->FileName;
	}

	int GetImageCount()	const
	{
		return (int)m_vecTextureInfo.size();
	}

public:
	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}


public:
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);
	bool LoadTextureArray(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureArrayFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);
	virtual void Render();

private:
	bool CreateResource(int Index);
	bool CreateResourceArray();

public:
	void SetShader(int Register, int ShaderBufferType, int Index);
	void ResetShader(int Register, int ShaderBufferType);


public:
	void Save(FILE* File);
	void Load(FILE* File);
};

