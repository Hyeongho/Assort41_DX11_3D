
#include "Share.fx"

struct VS_INPUT_UV
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
    uint InstanceID : SV_InstanceID;
};

struct VS_OUTPUT_UV
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    float2 OriginUV : TEXCOORD1;
    float4 Color : COLOR;
    float Opacity : TEXCOORD2;
    int TextureType : TEXCOORD3;
    int Frame : TEXCOORD4;
};

cbuffer TileMapCBuffer : register(b10)
{
    float2 g_TileImageSize;
    float2 g_TileStart;
    float2 g_TileEnd;
    float2 g_TileSize;
    matrix g_TilematWVP;
    int    g_TileFrame;
    float3 g_TileEmpty;
};


struct TileInfo
{
    matrix matWVP;
    float2 Start;
    float2 End;
    float4 TypeColor;
    float Opacity;
    int AnimationType;
    int Frame;
    float Empty;
};

StructuredBuffer<TileInfo> g_TileInfoArray : register(t40);

Texture2DArray g_TileTextureArray : register(t10);

float2 UpdateTileAnimation2D(float2 UV, int InstanceID)
{
    float2 Result = (float2) 0;
    
    if (g_MtrlTextureType == Anim2D_Atlas)
    {
        if (UV.x == 0.f)
            Result.x = g_TileInfoArray[InstanceID].Start.x / g_MtrlTextureWidth;
        else
            Result.x = g_TileInfoArray[InstanceID].End.x / g_MtrlTextureWidth;
    
        if (UV.y == 0.f)
            Result.y = g_TileInfoArray[InstanceID].Start.y / g_MtrlTextureHeight;
        else
            Result.y = g_TileInfoArray[InstanceID].End.y / g_MtrlTextureHeight;
    }
    
    else
        Result = UV;
        
    return Result;
}


VS_OUTPUT_UV TileMapVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;
    
    // mul : ��� ��. g_matWVP �� World * View * Proj �̹Ƿ� ������ ���⿡ ���ϰ� �Ǹ�
    // ���� �������� ��ȯ�� ������ ��ġ�� ���´�.
    // ������ w���� ������� z���� ���´�.
    // ���������� ��� ����ؼ� �Ѱ��ְ� �ȼ������ �ϱ� ���� ���� ���������� x, y, z, w��
    // ���������� w�� ��� �������ش�.
    // �̷��� ���ִ� ������ �ָ� �ִ� ��ü�� �۰�, ������ �ִ� ��ü�� ũ�� ǥ���� �Ǿ� �ϴµ�
    // ���������� w���� ������� z�� ���Ƿ� �� ���� ��ü�� ī�޶�κ��� ������ �ִ� �Ÿ���
    // �ǹ��ϹǷ� �� ������ �������ְ� �ȴٸ� �Ÿ��� �ָ� �ּ��� ū ������ �������ְ� �Ǳ� ������
    // ��������� ���� ���� ������ �Ǿ� ��ü�� �۰� ����� �Ǵ� ����̴�.
    output.Pos = mul(float4(input.Pos, 1.f), g_TileInfoArray[input.InstanceID].matWVP).xyww;
    
    output.UV = UpdateTileAnimation2D(input.UV, input.InstanceID);
    
    output.OriginUV = input.UV;
    output.Color = g_TileInfoArray[input.InstanceID].TypeColor;
    output.Opacity = g_TileInfoArray[input.InstanceID].Opacity;
    output.TextureType = g_MtrlTextureType;
    output.Frame = g_TileInfoArray[input.InstanceID].Frame;
    
    return output;
}

PS_OUTPUT_SINGLE TileMapPS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    float4 TextureColor = (float4) 0.f;
    
    if(input.TextureType != 2)
        TextureColor = g_BaseTexture.Sample(g_PointSmp, input.UV);
    
    else
        TextureColor = g_TileTextureArray.Sample(g_PointSmp, float3(input.UV, input.Frame));
    
    output.Color.rgb = TextureColor.rgb * g_MtrlBaseColor.rgb * input.Color.rgb;

    output.Color.a = TextureColor.a * input.Opacity;
    
    return output;
}



struct VS_INPUT_BACK
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_OUTPUT_BACK
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

VS_OUTPUT_BACK BackVS(VS_INPUT_BACK input)
{
    VS_OUTPUT_BACK output = (VS_OUTPUT_BACK) 0;

    float3 Pos = input.Pos - g_Pivot * g_MeshSize;
    
    // mul : ��� ��. g_matWVP �� World * View * Proj �̹Ƿ� ������ ���⿡ ���ϰ� �Ǹ�
    // ���� �������� ��ȯ�� ������ ��ġ�� ���´�.
    output.Pos = mul(float4(Pos, 1.f), g_matWVP).xyww;
    output.UV = input.UV;
    
    return output;
}

PS_OUTPUT_SINGLE BackPS(VS_OUTPUT_BACK input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    float4 TextureColor = g_BaseTexture.Sample(g_PointSmp, input.UV);
    
    output.Color.rgb = TextureColor.rgb * g_MtrlBaseColor.rgb;

    output.Color.a = TextureColor.a * g_MtrlOpacity;
    
    return output;
}
