#pragma once

#define	DIRECTINPUT_VERSION	0x0800

#include <Windows.h>
#include <vector>
#include <list>
#include <unordered_map>
#include <crtdbg.h>
#include <typeinfo>
#include <string>
#include <functional>
#include <algorithm>
#include <stack>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <filesystem>
#include <dwrite_3.h>
#include <d2d1.h>
#include <TCHAR.H>
#include <process.h>

#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "SharedPtr.h"
#include "Resource/Texture/DirectXTex.h"
#include "fmod.hpp"

#pragma comment(lib, "../Bin/fmod64_vc.lib")


extern float g_DeltaTime;

#define	ROOT_PATH		"Root"
#define	SHADER_PATH		"Shader"
#define	TEXTURE_PATH	"Texture"
#define	SOUND_PATH		"Sound"
#define	FONT_PATH		"Font"
#define	ANIMATION2D_PATH		"Animation2D"
#define	ANIMATION_PATH		"Animation"
#define	SCENE_PATH		"Scene"
#define	SAVE_PATH		"Save"
#define	MESH_PATH		"Mesh"

#define	DECLARE_SINGLE(Type)	\
private:\
	static Type*	m_Inst;\
public:\
	static Type* GetInst()\
	{\
		if (!m_Inst)\
			m_Inst = new Type;\
		return m_Inst;\
	}\
	static void DestroyInst()\
	{\
		if(m_Inst)\
		{\
			delete m_Inst;\
			m_Inst = nullptr;\
		}\
	}\
private:\
	Type();\
	~Type();

#define	DEFINITION_SINGLE(Type)	Type* Type::m_Inst = nullptr;

#define	SAFE_DELETE(p)	if(p)	{ delete p; p = nullptr; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = nullptr; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = nullptr;}

struct Resolution
{
	unsigned int	Width;
	unsigned int	Height;
};

// 위치, 색상 정보를 가지고 있는 정점.
struct VertexColor
{
	Vector3	Pos;
	Vector4	Color;

	VertexColor()
	{
	}

	VertexColor(const Vector3& _Pos, const Vector4& _Color) :
		Pos(_Pos),
		Color(_Color)
	{
	}
};

struct VertexBuffer
{
	ID3D11Buffer* Buffer;
	int		Size;	// 정점 1개의 크기
	int		Count;	// 정점 개수
	void* Data;		// 정점 정보

	VertexBuffer() :
		Buffer(nullptr),
		Size(0),
		Count(0),
		Data(nullptr)
	{
	}

	~VertexBuffer()
	{
		SAFE_DELETE_ARRAY(Data);
		SAFE_RELEASE(Buffer);
	}
};

struct IndexBuffer
{
	ID3D11Buffer* Buffer;
	int		Size;		// 인덱스 1개의 크기
	int		Count;		// 인덱스 개수
	DXGI_FORMAT	Fmt;	// 인덱스 포멧
	void* Data;			// 인덱스 정보

	IndexBuffer() : Buffer(nullptr), Size(0), Count(0), Fmt(DXGI_FORMAT_UNKNOWN), Data(nullptr)
	{
	}

	~IndexBuffer()
	{
		SAFE_DELETE_ARRAY(Data);
		SAFE_RELEASE(Buffer);
	}
};

struct TransformCBuffer
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;
	Matrix matWVP;
	Matrix matWV;
	Matrix matInvWorld;
	Matrix matInvView;
	Matrix matInvProj;
	Matrix matInvWVP;
	Matrix matInvVP;
	Vector3 Pivot;
	float	Proj11;
	Vector3 MeshSize;
	float	Proj22;
};

struct GlobalCBuffer
{
	float	DeltaTime;
	float	AccTime;
	Vector2	Resolution;
};

// 위치, 색상 정보를 가지고 있는 정점.
struct VertexUV
{
	Vector3	Pos;
	Vector2	UV;

	VertexUV()
	{
	}

	VertexUV(const Vector3& _Pos, const Vector2& _UV) : Pos(_Pos), UV(_UV)
	{
	}
};

struct Vertex3D
{
	Vector3	Pos;
	Vector3	Normal;
	Vector2	UV;
	Vector3	Tangent;
	Vector3	Binormal;
	Vector4	BlendWeight;
	Vector4	BlendIndex;
};

struct Vertex3DStatic
{
	Vector3	Pos;
	Vector3	Normal;
	Vector2	UV;
	Vector3	Tangent;
	Vector3	Binormal;
};

struct MaterialCBuffer
{
	Vector4 BaseColor;
	Vector4 AmbientColor;
	Vector4 SpecularColor;
	Vector4 EmissiveColor;
	float Opacity;
	int	TextureType;
	float TextureWidth;
	float TextureHeight;
	int Animation3DEnable;
	Vector3	Empty;
};

struct HierarchyName
{
	std::string	Name;
	std::string	ParentName;
	std::string	ClassName;
	std::string	ParentClassName;
	class CComponent* Component;
	class CComponent* Parent;
};

struct HierarchyObjectName
{
	std::string	Name;
	std::string	ParentName;
	std::string	ClassName;
	std::string	ParentClassName;
	class CGameObject* Obj;
	class CGameObject* Parent;
};

struct Animation2DFrameData
{
	Vector2	Start;
	Vector2	End;
};

struct Animation2DCBuffer
{
	float Anim2DImageWidth;
	float Anim2DImageHeight;
	Vector2 Anim2DFrameStart;
	Vector2 Anim2DFrameEnd;
	int Anim2DType;
	int Anim2DEnable;
	int	Anim2DFrame;
	Vector3 Anim2DEmpty;
};

struct CollisionChannel
{
	std::string			Name;
	ECollision_Channel	Channel;
	ECollision_Interaction	Interaction;
};

struct CollisionProfile
{
	std::string							Name;
	CollisionChannel*					Channel;
	bool								Enable;
	std::vector<ECollision_Interaction>	vecCollisionInteraction;

	CollisionProfile() :
		Enable(true)
	{
	}
};

struct CollisionResult
{
	class CCollider* Src;
	class CCollider* Dest;
	Vector3			HitPoint;

	CollisionResult() :
		Src(nullptr),
		Dest(nullptr)
	{
	}
};

struct ColliderCBuffer
{
	Vector4	Color;
	Matrix	matWVP;
};

struct Box2DInfo
{
	float	Left;
	float	Bottom;
	float	Right;
	float	Top;
};

struct Sphere2DInfo
{
	Vector2	Center;
	float	Radius;
};

struct OBB2DInfo
{
	Vector2	Center;
	Vector2	Axis[AXIS2D_MAX];
	float	Length[AXIS2D_MAX];
};

struct PixelInfo
{
	Box2DInfo	Box2D;
	unsigned char* Pixel;
	unsigned int	Width;
	unsigned int	Height;
	EPixelCollision_Type	PixelColorCollisionType;
	EPixelCollision_Type	PixelAlphaCollisionType;
	unsigned char	TypeColor[4];
	ID3D11ShaderResourceView* SRV;
	std::string	Name;
	TCHAR		FileName[MAX_PATH];
	char		PathName[MAX_PATH];
	int			RefCount;

	PixelInfo()	:
		FileName{},
		PathName{},
		RefCount(0),
		Pixel(nullptr),
		Box2D{},
		Width(0),
		Height(0),
		PixelColorCollisionType(EPixelCollision_Type::Alpha_Confirm),
		PixelAlphaCollisionType(EPixelCollision_Type::None),
		TypeColor{}
	{
	}
};

struct UICBuffer
{
	Vector4 UITint;
	Matrix	UIWVP;
	Vector2 UIMeshSize;
	Vector2 UIPivot;
	int		UITextureEnable;
	float	UIOpacity;
	Vector2	UIEmpty;
};

struct UIProgressBarCBuffer
{
	int		BarDir;
	float	Percent;
	Vector2	Empty;
};

struct ThreadSyncData
{
	int	Header;
	int	Size;
	unsigned char Data[1024];
};

struct TileMapCBuffer
{
	Vector2	ImageSize;
	Vector2	Start;
	Vector2	End;
	Vector2	TileSize;
	Matrix	matWVP;
	int		Frame;
	Vector3	Empty;
};

struct TileInfo
{
	Matrix	matWVP;
	Vector2	Start;
	Vector2	End;
	Vector4	TypeColor;
	float	Opacity;
	int		AnimationType;
	int		Frame;
	float	Empty;
};

struct LightCBuffer
{
	Vector4	Color;
	int		LightType;
	Vector3	Pos;
	Vector3	Dir;
	float	Distance;
	float	AngleIn;
	float	AngleOut;
	float	Att1;
	float	Att2;
	float	Att3;
	float	Intensity;
	Vector2	Empty;

	LightCBuffer() :
		Intensity(1.f),
		Att1(0.f),
		Att2(0.f),
		Att3(1.f),
		AngleIn(20.f),
		AngleOut(35.f),
		Distance(1000.f)
	{
	}
};

struct AnimationCBuffer
{
	int		BoneCount;
	int		CurrentFrame;
	int		NextFrame;
	float	Ratio;
	int		FrameCount;
	int		RowIndex;
	int		ChangeAnimation;
	float	ChangeRatio;
	int		ChangeFrameCount;
	Vector3	Empty;
};

struct InstancingBufferInfo
{
	Matrix  matWVP;
	Matrix	matWV;
	Vector4 BaseColor;
	Vector4 AmbientColor;
	Vector4 SpecularColor;
	Vector4 EmissiveColor;
	float Opacity;
	int Animation3D;
	Vector2	Empty;
};

struct InstancingCBuffer
{
	int	BoneCount;
	Vector3	Empty;
};

struct OutputBoneInfo
{
	Matrix matBone;
	Vector3 Pos;
	float Empty1;
	Vector3 Scale;
	float Empty2;
	Vector4 Rot;
};

struct TerrainCBuffer
{
	float DetailLevel;
	int SplatCount;
	Vector2 Empty;
};

struct Ray
{
	Vector3	Pos;
	Vector3	Dir;
};

//김범중 UI관련 구조체
struct HierarchyWindowName
{
	class CUIWindow* window;
	class CUIWindow* parent;
	std::string	name;
	std::string	parentName;
	std::string	className;
	std::string	parentClassName;
	HierarchyWindowName()
		: window(nullptr)
		, parent(nullptr)
	{
	}
};

struct HierarchyWidgetName
{
	class CUIWidget* widget;
	class CUIWidget* parent;
	std::string	name;
	std::string	parentName;
	std::string	className;
	std::string	parentClassName;
	HierarchyWidgetName()
		: widget(nullptr)
		, parent(nullptr)
	{
	}
};
//