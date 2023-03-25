#include "MaterialManager.h"

CMaterialManager::CMaterialManager()
{
}

CMaterialManager::~CMaterialManager()
{
}

bool CMaterialManager::Init()
{
	CMaterial* Material = CreateMaterial<CMaterial>("DefaultColor");

	Material->SetShader("SpriteColorShader");

	Material = CreateMaterial<CMaterial>("DefaultTexture");

	Material->SetShader("SpriteShader");

	Material->AddTexture(0, (int)EShaderBufferType::Pixel, "DefaultTexture",
		TEXT("teemo.png"));

	Material = CreateMaterial<CMaterial>("TileMap");

	std::vector<const TCHAR*>	vecFileNames;

	for (int i = 0; i <= 379; ++i)
	{
		TCHAR* FileName = new TCHAR[MAX_PATH];

		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(FileName, TEXT("Diablos_Lair_Floor_TRS/Diablos_Lair_Floor_%d.png"), i);

		vecFileNames.push_back(FileName);
	}

	Material->SetShader("TileMapShader");
	Material->AddTextureArray(10, (int)EShaderBufferType::Pixel, "DefaultTileIsometric",
		vecFileNames);
	Material->SetRenderState("DepthLessEqual");
	Material->SetRenderState("AlphaBlend");

	for (int i = 0; i <= 379; ++i)
	{
		SAFE_DELETE_ARRAY(vecFileNames[i]);
	}

	Material = CreateMaterial<CMaterial>("DefaultTileMapBack");

	Material->SetShader("TileMapBackShader");
	Material->SetRenderState("DepthLessEqual");



	Material = CreateMaterial<CMaterial>("Sky");

	Material->SetShader("SkyShader");

	Material->AddTexture(10, (int)EShaderBufferType::Pixel, "SkyTexture", TEXT("Sky/Sky0222.dds"));

	//Material->AddTexture(10, (int)EShaderBufferType::Pixel, "SkyTexture", TEXT("Sky.dds"));


	Material->SetRenderState("DepthLessEqual");
	Material->SetRenderState("FrontFaceCull");


	Material = CreateMaterial<CMaterial>("DefaultTerrain");

	Material->SetShader("TerrainShader");

	Material->AddTexture(0, (int)EShaderBufferType::Pixel, "DefaultTerrainTexture",
		TEXT("LandScape/ROCK_01+MOSS.bmp"));
	Material->AddTexture(1, (int)EShaderBufferType::Pixel, "DefaultTerrainNormalTexture",
		TEXT("LandScape/ROCK_01+MOSS_NRM.png"));
	Material->AddTexture(2, (int)EShaderBufferType::Pixel, "DefaultTerrainSpecularTexture",
		TEXT("LandScape/ROCK_01+MOSS_SPEC.png"));


	std::vector<const TCHAR*>	vecFileName;

	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cave_01.dds"));
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cliff05.dds"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Pebbles_01.dds"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large.dds"));

	Material->AddTextureArray(10, (int)EShaderBufferType::Pixel,
		"DefaultSplatDif", vecFileName);

	vecFileName.clear();
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cave_01_NRM.bmp"));
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cliff05_NRM.bmp"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Pebbles_01_NRM.bmp"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large_NRM.bmp"));

	Material->AddTextureArray(11, (int)EShaderBufferType::Pixel,
		"DefaultSplatNormal", vecFileName);

	vecFileName.clear();
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cave_01_SPEC.bmp"));
	vecFileName.push_back(TEXT("LandScape/BD_Terrain_Cliff05_SPEC.bmp"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Pebbles_01_SPEC.bmp"));
	vecFileName.push_back(TEXT("LandScape/Terrain_Cliff_15_Large_SPEC.bmp"));

	Material->AddTextureArray(12, (int)EShaderBufferType::Pixel,
		"DefaultSplatSpc", vecFileName);

	vecFileName.clear();
	vecFileName.push_back(TEXT("LandScape/RoadAlpha1.bmp"));
	vecFileName.push_back(TEXT("LandScape/WaterStonAlpha.bmp"));
	vecFileName.push_back(TEXT("LandScape/SandBaseAlpha.bmp"));
	vecFileName.push_back(TEXT("LandScape/WaterBaseAlpha.bmp"));

	Material->AddTextureArray(13, (int)EShaderBufferType::Pixel,
		"DefaultSplatAlpha", vecFileName);

	Material->EnableBump();
	Material->EnableSpecular();
	Material->SetSpecularPower(3.2f);

	Material = CreateMaterial<CMaterial>("BikiniCityTerrain");
	Material->SetShader("TerrainShader");
	Material->AddTexture(0, (int)EShaderBufferType::Pixel, "BikiniCity", TEXT("LandScape/T_BB_Sand_01_D.tga"));
	Material->SetEmissiveColor(1.f, 1.f, 1.f, 0.f);

	Material = CreateMaterial<CMaterial>("Billboard");

	Material->SetShader("BillboardShader");

	Material->AddTexture(0, (int)EShaderBufferType::Pixel, "DefaultTexture",
		TEXT("teemo.png"));


	Material = CreateMaterial<CMaterial>("DebugDecal");

	Material->SetShader("DecalDebugShader");
	Material->SetBaseColor(1.f, 1.f, 0.f, 1.f);


	Material = CreateMaterial<CMaterial>("DefaultDecal");

	Material->SetShader("DecalShader");
	Material->AddTexture(0, (int)EShaderBufferType::Pixel,
		"DefauleDecal", TEXT("Decal/free-blood-texture_COLOR.png"));
	Material->AddTexture(1, (int)EShaderBufferType::Pixel,
		"DefauleDecalNormal", TEXT("Decal/free-blood-texture_NRM.png"));
	Material->AddTexture(2, (int)EShaderBufferType::Pixel,
		"DefauleDecalSpecular", TEXT("Decal/free-blood-texture_SPEC.png"));

	Material->EnableBump();
	Material->EnableSpecular();

	Material = CreateMaterial<CMaterial>("Bubble");
	Material->AddTexture(0, (int)EShaderBufferType::Pixel, "Bubble", TEXT("Particle/Bubbles50px.png"));
	Material->SetShader("SpriteShader");

	//Material = CreateMaterial<CMaterial>("ShockWave");
	//Material->AddTexture(0, (int)EShaderBufferType::Pixel, "ShockWave", TEXT("Particle/T_ShockWave.tga"));
	//Material->SetShader("SpriteShader");

	// 흐르는 텍스쳐
	Material = CreateMaterial<CMaterial>("Effect");
	Material->AddTexture(0, (int)EShaderBufferType::Pixel, "Water", TEXT("Particle/T_Seafoam_BC.tga"));	
	Material->SetShader("TranslationShader");
	Material->SetRenderState("AlphaBlend");
	Material->SetOpacity(0.5f);
	return true;
}

CMaterial* CMaterialManager::FindMaterial(const std::string& Name)
{
	auto	iter = m_mapMaterial.find(Name);

	if (iter == m_mapMaterial.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CMaterialManager::ReleaseMaterial(const std::string& Name)
{
	auto	iter = m_mapMaterial.find(Name);

	if (iter != m_mapMaterial.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			m_mapMaterial.erase(iter);
	}
}
}
