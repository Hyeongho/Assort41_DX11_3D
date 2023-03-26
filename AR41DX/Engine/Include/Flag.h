#pragma once

enum AXIS2D
{
	AXIS2D_X,
	AXIS2D_Y,
	AXIS2D_MAX
};

enum AXIS
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_MAX
};

enum class MeshType : unsigned char
{
	Sprite,
	UI,
	Static,
	Animation
};

enum class BufferType : unsigned char
{
	Vertex,
	Index
};

enum class ShaderType : unsigned char
{
	Graphic,
	Compute
};

enum class ComponentType : unsigned char
{
	Scene,
	Object
};

enum class SceneComponentType : unsigned char
{
	Scene,
	Primitive
};

enum class EShaderBufferType : unsigned char
{
	Vertex = 0x1,
	Pixel = 0x2,
	Domain = 0x4,
	Hull = 0x8,
	Geometry = 0x10,
	Compute = 0x20,
	Graphic = Vertex | Pixel | Domain | Hull | Geometry,
	All = Vertex | Pixel | Domain | Hull | Geometry | Compute
};

enum class ECameraType : unsigned char
{
	Camera2D,
	Camera3D,
	CameraUI
};

enum class EImageType : unsigned char
{
	Atlas,
	Frame,
	Array,
	RenderTarget
};

enum class ESamplerType : unsigned char
{
	Point,
	Linear,
	Anisotropic
};

enum class ERenderStateType : unsigned char
{
	Blend,
	Rasterizer,
	DepthStencil,
	End
};

enum class EAnimation2DType : int
{
	None = -1,
	Atlas,
	Frame,
	Array
};

enum class ECollision_Channel
{
	Default,
	Mouse,
	Player,
	PlayerAttack,
	Monster,
	MonsterAttack,
	Ground,
	Wall,
	Collectible,
	Platform,
	Pufferfish,
	DetectArea,
	PlayerAttack,
	Detect,
	Pool,
	Custom8,
	Custom9,
	Custom10,
	Custom11,
	Custom12,
	Custom13,
	Custom14,
	Custom15,
	Custom16,
	Max
};

enum class ECollision_Interaction
{
	Ignore,
	Collision
};

enum class ECollider_Type
{
	Collider2D,
	Collider3D
};

enum class ECollider2D_Type
{
	Box2D,	// 회전없는 상자
	OBB2D,	// 회전되는 상자
	Sphere2D,	// 원
	Pixel	// 픽셀충돌용
};

enum class ECollider3D_Type
{
	Box,	// 회전없는 상자
	OBB,	// 회전되는 상자
	Sphere	// 원
};

enum class ECollision_Result
{
	Collision,
	Release,
	Max
};

enum class EPixelCollision_Type
{
	None,
	Color_Ignore,
	Color_Confirm,
	Alpha_Ignore,
	Alpha_Confirm
};

enum class EProgressBarTextureType
{
	Back,
	Bar,
	Max
};

enum class EProgressBarDir
{
	RightToLeft,
	LeftToRight,
	TopToBottom,
	BottomToTop
};

enum class ESceneComponentType
{
	Scene,
	Primitive,
	Sprite,
	Camera,
	TargetArm,
	Collider,
	Collider2D,
	Box2D,
	OBB2D,
	Sphere2D,
	Pixel,
	Collider3D,
	TileMap,
	Max
};

enum class ETileShape
{
	Rect,
	Isometric
};

enum class ETileOption
{
	None,
	Wall,
	End
};

enum class ELightType
{
	Direction,
	Point,
	Spot
};

enum class ENavNodeType
{
	None, 
	Open, 
	Close
};


enum class EMapList // 맵 전체 리스트
{
	Patric,				// 뚱이
	DutchMan_Grave,		// 바다도깨비의 묘지
	Sand_Mountain,		// 모래산
	Goo_Lagoon,			// 끈끈이 호수
	Dome,				// 포세이돔
	Krusty_Krab,		// 집게리아
	Chum_Bucketlab,		// 미끼식당 실험실
	Merma_Lair,			// 인어 은신처
	Bikini_Bottom,		// 비키니시티
	Down_Town,			// 비키니시티 도심
	Krabs,				// 집게사장
	Kelp_Forest,		// 켈프 숲
	Rock_Bottom,		// 메롱시티
	Jelly_Fish_Field,	// 해파리 동산
	Industrial_Park,	// 공업단지
	End
};

enum class ENpcList // Npc 전체 리스트
{
	MrKrabs,
	Squidward,
	Patric,
	BusDriver,
	InfoSign,
	End
};

enum class EItemList { // 아이템 전체 리스트
	GoldenSpatula,
	ShinyFlower,
	Sock,
	UnderWear,
	End
};

enum class EShaderType
{
	Default,
	CelShader
};