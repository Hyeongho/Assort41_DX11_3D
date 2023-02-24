
#include "Share.fx"

struct VS_OUTPUT_LIGHTACC
{
	float4 Pos : SV_POSITION;
	float4 ProjPos : POSITION;
    uint InstanceID : TEXCOORD;
};

struct PS_OUTPUT_LIGHTACC
{
	float4 Diffuse : SV_TARGET;
	float4 Specular : SV_TARGET1;
	float4 Emissive : SV_TARGET2;
};

Texture2DMS<float4> g_GBuffer1Tex : register(t14);
Texture2DMS<float4> g_GBuffer2Tex : register(t15);
Texture2DMS<float4> g_GBuffer3Tex : register(t16);
Texture2DMS<float4> g_GBuffer4Tex : register(t17);

Texture2DMS<float4> g_LightDiffuseTex : register(t18);
Texture2DMS<float4> g_LightSpecularTex : register(t19);
Texture2DMS<float4> g_LightEmissiveTex : register(t20);

Texture2DMS<float4> g_ScreenTex : register(t21);
Texture2DMS<float4> g_ShadowMapTex : register(t22);

cbuffer ShadowCBuffer : register(b13)
{
    matrix  g_matShadowVP;
    matrix  g_matShadowInvVP;
    float2  g_ShadowResolution;
    float   g_ShadowBias;
    float   g_ShadowEmpty;
};

#define LightTypeDir 0
#define LightTypePoint 1
#define LightTypeSpot 2

struct LightInfo
{
    float4 LightColor;
    int LightLightType;
    float3 LightPos;
    float3 LightDir;
    float LightDistance;
    float LightAngleIn;
    float LightAngleOut;
    float LightAtt1;
    float LightAtt2;
    float LightAtt3;
    float LightIntensity;
    float2 LightEmpty;
};

StructuredBuffer<LightInfo> g_LightInfoArray : register(t41);

struct LightResult
{
    float3 Diffuse;
    float3 Specular;
    float3 Emissive;
};

LightResult ComputeLight(float3 ViewPos, float3 ViewNormal, float SpecularPower, uint InstanceID, float4 GBuffer4Color)
{
    LightResult result = (LightResult)0;

    float3 LightDir = (float3)0.f;
    float Attn = 1.f;

    LightInfo Info = g_LightInfoArray[InstanceID];

    if (Info.LightLightType == LightTypeDir)
    {
        LightDir = -Info.LightDir;
    }

    if (Info.LightLightType == LightTypePoint)
    {
        // ������ ��ġ���� ���� �ȼ��� ��ġ�� ����.
        LightDir = Info.LightPos - ViewPos;
        LightDir = normalize(LightDir);

        float Dist = distance(Info.LightPos, ViewPos);

        if (Dist > Info.LightDistance)
        {
            Attn = 0.f;
        }

        else
        {
            //Attn = 1.f / (Info.LightAtt1 + Info.LightAtt2 * Dist + Info.LightAtt3 * (Dist * Dist));
            Attn = 1.f - min(Dist / Info.LightDistance, 1.f);
        }
    }

    if (Info.LightLightType == LightTypeSpot)
    {
    }

    /*float   Intensity = max(0.f, dot(ViewNormal, LightDir)) *
        Info.LightIntensity;*/

    float Intensity = max(0.f, dot(ViewNormal, LightDir));

    float3 BaseColor = ConvertColor(GBuffer4Color.r).rgb;
    float3 AmbientColor = ConvertColor(GBuffer4Color.g).rgb;
    float3 SpecularColor = ConvertColor(GBuffer4Color.b).rgb;
    float3 EmissiveColor = ConvertColor(GBuffer4Color.a).rgb;

    float3 Diffuse = Info.LightColor.rgb * BaseColor * Intensity * Attn;
    float3 Ambient = Info.LightColor.rgb * AmbientColor * Intensity * Attn;

    result.Diffuse = Diffuse * Ambient;

    // ��������� ������ ī�޶� �ٶ󺸴� ���⺤�͸� �ϳ� ���Ѵ�.
    float3  View = -ViewPos;
    View = normalize(View);

    // �ݻ纤�͸� ���Ѵ�.
    float3 Reflect = 2.f * ViewNormal * dot(ViewNormal, LightDir) - LightDir;
    Reflect = normalize(Reflect);

    float SpecularIntensity = max(0.f, dot(View, Reflect)) * Info.LightIntensity;

    result.Specular = Info.LightColor.rgb * SpecularColor * pow(SpecularIntensity, SpecularPower) * Attn;

    result.Emissive = EmissiveColor * Info.LightIntensity;

    return result;
}

LightResult ComputeCelShaderLight(float3 ViewPos, float3 ViewNormal,
    float SpecularPower, uint InstanceID, float4 GBuffer4Color)
{
    LightResult result = (LightResult)0;

    float3  LightDir = (float3)0.f;
    float   Attn = 1.f;

    LightInfo   Info = g_LightInfoArray[InstanceID];

    if (Info.LightLightType == LightTypeDir)
    {
        LightDir = -Info.LightDir;
    }

    if (Info.LightLightType == LightTypePoint)
    {
        // ������ ��ġ���� ���� �ȼ��� ��ġ�� ����.
        LightDir = Info.LightPos - ViewPos;
        LightDir = normalize(LightDir);

        float Dist = distance(Info.LightPos, ViewPos);

        if (Dist > Info.LightDistance)
            Attn = 0.f;

        else
        {
            //Attn = 1.f / (Info.LightAtt1 + Info.LightAtt2 * Dist + Info.LightAtt3 * (Dist * Dist));
            Attn = 1.f - min(Dist / Info.LightDistance, 1.f);
        }
    }

    if (Info.LightLightType == LightTypeSpot)
    {
    }

    //float   Intensity = max(0.f, dot(ViewNormal, LightDir)) *
    //    Info.LightIntensity;

    float2  UV;
    UV.y = 0.1f;
    UV.x = max(0.f, dot(ViewNormal, LightDir));

    float   Intensity = 0.f;

    float4  TextureColor = g_LUTTexture.Sample(g_LinearSmp, UV);

    Intensity = TextureColor.r;

    //float   Intensity = max(0.f, dot(ViewNormal, LightDir));

    float3  BaseColor = ConvertColor(GBuffer4Color.r).rgb;
    float3  AmbientColor = ConvertColor(GBuffer4Color.g).rgb;
    float3  SpecularColor = ConvertColor(GBuffer4Color.b).rgb;
    float3  EmissiveColor = ConvertColor(GBuffer4Color.a).rgb;

    float3 Diffuse = Info.LightColor.rgb * BaseColor * Intensity * Attn;
    float3 Ambient = Info.LightColor.rgb * AmbientColor * Intensity * Attn;

    result.Diffuse = Diffuse * Ambient;

    // ��������� ������ ī�޶� �ٶ󺸴� ���⺤�͸� �ϳ� ���Ѵ�.
    float3  View = -ViewPos;
    View = normalize(View);

    // �ݻ纤�͸� ���Ѵ�.
    float3  Reflect = 2.f * ViewNormal * dot(ViewNormal, LightDir) -
        LightDir;
    Reflect = normalize(Reflect);

    float   SpecularIntensity = max(0.f, dot(View, Reflect)) *
        Info.LightIntensity;

    UV.y = 0.1f;
    UV.x = max(0.f, dot(View, Reflect));

    TextureColor = g_LUTTexture.Sample(g_LinearSmp, UV);

    SpecularIntensity = TextureColor.r;

    result.Specular = Info.LightColor.rgb * SpecularColor *
        pow(SpecularIntensity, SpecularPower) * Attn;

    result.Emissive = EmissiveColor * Info.LightIntensity;

    return result;
}

VS_OUTPUT_LIGHTACC LightAccVS(uint VertexID : SV_VertexID, uint InstanceID : SV_InstanceID)
{
	VS_OUTPUT_LIGHTACC output = (VS_OUTPUT_LIGHTACC)0;

    output.ProjPos = float4(g_NullPos[VertexID], 0.f, 1.f);
    output.Pos = output.ProjPos;

    output.InstanceID = InstanceID;

	return output;
}

PS_OUTPUT_LIGHTACC LightAccPS(VS_OUTPUT_LIGHTACC input)
{
	PS_OUTPUT_LIGHTACC	output = (PS_OUTPUT_LIGHTACC)0;

    // ���� ��ġ * ���� * �� * ���� = �������� ��ġ
    // ����������ġ / ��������w �� �ؼ� -1 ~ 1 ������ x, y ��ġ�� ���ϰ�
    // �� ���� 0 ~ 1 ���̷� ��ȯ�Ͽ� ȭ�鿡 ����Ѵ�.
    float2  UV;
    UV.x = input.ProjPos.x / input.ProjPos.w * 0.5f + 0.5f;
    UV.y = input.ProjPos.y / input.ProjPos.w * -0.5f + 0.5f;

    int2 LoadPos = (int2)0;

    LoadPos.x = (int)(UV.x * g_Resolution.x);
    LoadPos.y = (int)(UV.y * g_Resolution.y);

    float4 GBuffer3Color = g_GBuffer3Tex.Load(LoadPos, 0);

    if (GBuffer3Color.a == 0.f)
    {
        clip(-1);
    }

    float4 GBuffer2Color = g_GBuffer2Tex.Load(LoadPos, 0);
    float4 GBuffer4Color = g_GBuffer4Tex.Load(LoadPos, 0);

    // ������ �׷����� ���������� ��ġ�� �̿��ؼ� ��ü�� ��ġ�� ���س��� �Ѵ�.
    float4 ProjPos;

    // ���⿡ ������ ���� ��µǾ� �ִ� ��ü�� w�� �������� ���� ����Ȱ��̴�.
    ProjPos.x = (UV.x * 2.f - 1.f) * GBuffer3Color.g;
    ProjPos.y = (UV.y * -2.f + 1.f) * GBuffer3Color.g;
    ProjPos.z = GBuffer3Color.r * GBuffer3Color.g;
    ProjPos.w = GBuffer3Color.g;

    // �������� ���� Normal�� ������� �����Ƿ� �� ������ ��ġ��
    // �����ش�.
    float3 ViewPos;

    ViewPos.x = ProjPos.x / g_Proj11;
    ViewPos.y = ProjPos.y / g_Proj22;
    ViewPos.z = GBuffer3Color.g;

    float3 ViewNormal = GBuffer2Color.rgb;

    //ViewNormal = normalize(ViewNormal);

    LightResult result = ComputeLight(ViewPos, ViewNormal, GBuffer3Color.b, input.InstanceID, GBuffer4Color);

    output.Diffuse.rgb = result.Diffuse;
    output.Specular.rgb = result.Specular;
    output.Emissive.rgb = result.Emissive;

    output.Diffuse.a = 1.f;
    output.Specular.a = 1.f;
    output.Emissive.a = 1.f;

	return output;
}

struct VS_OUTPUT_SCREEN
{
    float4 Pos : SV_POSITION;
    float4 ProjPos : POSITION;
};

PS_OUTPUT_LIGHTACC CelShaderAccPS(VS_OUTPUT_LIGHTACC input)
{
    PS_OUTPUT_LIGHTACC	output = (PS_OUTPUT_LIGHTACC)0;

    // ���� ��ġ * ���� * �� * ���� = �������� ��ġ
    // ����������ġ / ��������w �� �ؼ� -1 ~ 1 ������ x, y ��ġ�� ���ϰ�
    // �� ���� 0 ~ 1 ���̷� ��ȯ�Ͽ� ȭ�鿡 ����Ѵ�.
    float2  UV;
    UV.x = input.ProjPos.x / input.ProjPos.w * 0.5f + 0.5f;
    UV.y = input.ProjPos.y / input.ProjPos.w * -0.5f + 0.5f;

    int2 LoadPos = (int2)0;

    LoadPos.x = (int)(UV.x * g_Resolution.x);
    LoadPos.y = (int)(UV.y * g_Resolution.y);

    float4 GBuffer1Color = g_GBuffer1Tex.Load(LoadPos, 0);

    if (GBuffer1Color.a == 0.f)
        clip(-1);

    float4 GBuffer3Color = g_GBuffer3Tex.Load(LoadPos, 0);
    float4 GBuffer2Color = g_GBuffer2Tex.Load(LoadPos, 0);
    float4 GBuffer4Color = g_GBuffer4Tex.Load(LoadPos, 0);

    // ������ �׷����� ���������� ��ġ�� �̿��ؼ� ��ü�� ��ġ�� ���س��� �Ѵ�.
    float4  ProjPos;

    // ���⿡ ������ ���� ��µǾ� �ִ� ��ü�� w�� �������� ���� ����Ȱ��̴�.
    ProjPos.x = (UV.x * 2.f - 1.f) * GBuffer3Color.g;
    ProjPos.y = (UV.y * -2.f + 1.f) * GBuffer3Color.g;
    ProjPos.z = GBuffer3Color.r * GBuffer3Color.g;
    ProjPos.w = GBuffer3Color.g;

    // �������� ���� Normal�� ������� �����Ƿ� �� ������ ��ġ��
    // �����ش�.
    float3 ViewPos;

    ViewPos.x = ProjPos.x / g_Proj11;
    ViewPos.y = ProjPos.y / g_Proj22;
    ViewPos.z = GBuffer3Color.g;

    float3  ViewNormal = GBuffer2Color.rgb;

    //ViewNormal = normalize(ViewNormal);

    LightResult result = ComputeCelShaderLight(ViewPos, ViewNormal,
        GBuffer3Color.b, input.InstanceID, GBuffer4Color);

    output.Diffuse.rgb = result.Diffuse;
    output.Specular.rgb = result.Specular;
    output.Emissive.rgb = result.Emissive;

    output.Diffuse.a = 1.f;
    output.Specular.a = 1.f;
    output.Emissive.a = 1.f;

    return output;
}

VS_OUTPUT_SCREEN ScreenVS(uint VertexID : SV_VertexID)
{
    VS_OUTPUT_SCREEN output = (VS_OUTPUT_SCREEN)0;

    output.ProjPos = float4(g_NullPos[VertexID], 0.f, 1.f);
    output.Pos = output.ProjPos;

    return output;
}

PS_OUTPUT_SINGLE ScreenPS(VS_OUTPUT_SCREEN input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

    // ���� ��ġ * ���� * �� * ���� = �������� ��ġ
    // ����������ġ / ��������w �� �ؼ� -1 ~ 1 ������ x, y ��ġ�� ���ϰ�
    // �� ���� 0 ~ 1 ���̷� ��ȯ�Ͽ� ȭ�鿡 ����Ѵ�.
    float2  UV;
    UV.x = input.ProjPos.x / input.ProjPos.w * 0.5f + 0.5f;
    UV.y = input.ProjPos.y / input.ProjPos.w * -0.5f + 0.5f;

    int2 LoadPos = (int2)0;

    LoadPos.x = (int)(UV.x * g_Resolution.x);
    LoadPos.y = (int)(UV.y * g_Resolution.y);

    float4 GBuffer1Color = g_GBuffer1Tex.Load(LoadPos, 0);

    if (GBuffer1Color.a == 0.f)
    {
        clip(-1);
    }

    float4 LightDiffuse = g_LightDiffuseTex.Load(LoadPos, 0);
    float4 LightSpecular = g_LightSpecularTex.Load(LoadPos, 0);
    float4 LightEmissive = g_LightEmissiveTex.Load(LoadPos, 0);

    // �׸��ڸ��� �����Ѵ�.
    float4 GBuffer3Color = g_GBuffer3Tex.Load(LoadPos, 0);

    float4  ProjPos;

    // ���⿡ ������ ���� ��µǾ� �ִ� ��ü�� w�� �������� ���� ����Ȱ��̴�.
    ProjPos.x = (UV.x * 2.f - 1.f) * GBuffer3Color.g;
    ProjPos.y = (UV.y * -2.f + 1.f) * GBuffer3Color.g;
    ProjPos.z = GBuffer3Color.r * GBuffer3Color.g;
    ProjPos.w = GBuffer3Color.g;

    // ������ ������ ��ü�� ���������� ��ġ�� ����������� ��ȯ�Ѵ�.
    float3  WorldPos = mul(ProjPos, g_matInvVP).xyz;

    // ������ ������ ��������� ��ġ�� ����������� ��ȯ�Ѵ�.
    float4  ShadowPos = mul(float4(WorldPos, 1.f), g_matShadowVP);

    float2  ShadowUV;
    ShadowUV.x = ShadowPos.x / ShadowPos.w * 0.5f + 0.5f;
    ShadowUV.y = ShadowPos.y / ShadowPos.w * -0.5f + 0.5f;

    int2 ShadowLoadPos = (int2)0;

    ShadowLoadPos.x = (int)(ShadowUV.x * g_ShadowResolution.x);
    ShadowLoadPos.y = (int)(ShadowUV.y * g_ShadowResolution.y);

    float4 ShadowMap = g_ShadowMapTex.Load(ShadowLoadPos, 0);

    if (ShadowMap.a > 0.f)
    {
        // ShadowPos.z / ShadowPos.w : ���� ����� ��ü��
        // ����������� ��ȯ�������� ����
        // ShadowMap.r : ���� �ȼ��� ��ġ�� �̿��Ͽ� �����������
        // ��ȯ������ �� ��ġ�� ��µ� ��ü�� ����
        if (ShadowMap.r * ShadowPos.w < ShadowPos.z - g_ShadowBias)
        {
            LightDiffuse.rgb *= 0.2f;
            LightSpecular.rgb *= 0.2f;
        }
    }

    output.Color.rgb = GBuffer1Color.rgb * LightDiffuse.rgb + GBuffer1Color.rgb * LightSpecular.rgb + GBuffer1Color.rgb * LightEmissive.rgb;

    output.Color.a = GBuffer1Color.a;

    return output;
}


VS_OUTPUT_SCREEN DeferredVS(uint VertexID : SV_VertexID)
{
    VS_OUTPUT_SCREEN output = (VS_OUTPUT_SCREEN)0;

    output.ProjPos = float4(g_NullPos[VertexID], 0.f, 1.f);
    output.Pos = output.ProjPos;

    return output;
}

PS_OUTPUT_SINGLE DeferredPS(VS_OUTPUT_SCREEN input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

    // ���� ��ġ * ���� * �� * ���� = �������� ��ġ
    // ����������ġ / ��������w �� �ؼ� -1 ~ 1 ������ x, y ��ġ�� ���ϰ�
    // �� ���� 0 ~ 1 ���̷� ��ȯ�Ͽ� ȭ�鿡 ����Ѵ�.
    float2  UV;
    UV.x = input.ProjPos.x / input.ProjPos.w * 0.5f + 0.5f;
    UV.y = input.ProjPos.y / input.ProjPos.w * -0.5f + 0.5f;

    int2 LoadPos = (int2)0;

    LoadPos.x = (int)(UV.x * g_Resolution.x);
    LoadPos.y = (int)(UV.y * g_Resolution.y);

    float4 ScreenColor = g_ScreenTex.Load(LoadPos, 0);

    if (ScreenColor.a == 0.f)
    {
        clip(-1);
    }

    output.Color = ScreenColor;

    return output;
}
