#pragma once
#include "Collider2D.h"
class CColliderPixel :
    public CCollider2D
{
    friend class CGameObject;
    friend class CScene;

protected:
    CColliderPixel();
    CColliderPixel(const CColliderPixel& component);
    virtual ~CColliderPixel();

protected:
    PixelInfo*   m_Info;
    CSharedPtr<class CMesh> m_PixelMesh;
    CSharedPtr<class CShader> m_PixelShader;

public:
    const PixelInfo& GetInfo()  const
    {
        return *m_Info;
    }

public:
    void SetInfo(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetInfoFullPath(const std::string& Name, const TCHAR* FullPath);
    void SetInfoMultibyte(const std::string& Name, const char* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetInfoMultibyteFullPath(const std::string& Name, const char* FullPath);
    void SetPixelColorCollisionType(EPixelCollision_Type Type);
    void SetPixelAlphaCollisionType(EPixelCollision_Type Type);
    void SetPixelColor(unsigned char r, unsigned char g, unsigned char b);
    void SetPixelAlpha(unsigned char a);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CColliderPixel* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
    virtual bool Collision(CCollider* Dest);
    virtual bool CollisionMouse(const Vector2& MouseWorldPos);
};

