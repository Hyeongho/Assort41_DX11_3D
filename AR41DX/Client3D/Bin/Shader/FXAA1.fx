#include "Share.fx"


//Texture2D<float4> tex : register(t10);
Texture2D tex : register(t10);
SamplerState clamp : register(s3);


struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    float4	ProjPos : POSITION; //
};

VS_OUT VSmain(uint id : SV_VertexID)
{
    VS_OUT vso;
    vso.UV = float2((id << 1) & 2, id & 2);
    //vso.Pos = float4(vso.UV * float2(2.0f, -2.0f) +
        //float2(-1.0f, 1.0f), 0.0f, 1.0f);

    vso.ProjPos = float4(g_NullPos[id], 0.f, 1.f);
    vso.Pos = vso.ProjPos;

    return vso;
}

cbuffer FXAACBuffer : register(b14)
{
    float4 rcpFrame;
};


#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
#define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
#define FXAA_SEARCH_STEPS        32
#define FXAA_SEARCH_ACCELERATION 1
#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
#define FXAA_SUBPIX              1
#define FXAA_SUBPIX_FASTER       0
#define FXAA_SUBPIX_CAP          (3.0/4.0)
#define FXAA_SUBPIX_TRIM         (1.0/4.0)
#define FXAA_SUBPIX_TRIM_SCALE (1.0/(1.0 - FXAA_SUBPIX_TRIM))


float4 FxaaTexOff(Texture2D tex, float2 Pos, int2 off)
{
    return tex.SampleLevel(clamp, Pos.xy, 0.0, off.xy);
}

float FxaaLuma(float3 rgb)
{
    return rgb.y * (0.587 / 0.299) + rgb.x;
}

float3 FxaaFilterReturn(float3 rgb)
{
    return rgb;
}

float4 FxaaTexGrad(Texture2D tex, float2 Pos, float2 grad)
{
    return tex.SampleGrad(clamp, Pos.xy, grad, grad);
}

float3 FxaaLerp3(float3 a, float3 b, float amountOfA)
{
    return (float3(-amountOfA, 0, 0) * b) +
        ((a * float3(amountOfA, 0, 0)) + b);
}

float4 FxaaTexLod0(Texture2 tex, float4 Pos)
{
    return tex.SampleLevel(clamp, Pos.xyzw, 0.0);
}

struct PS_IN
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    float4 ProjPos : POSITION;
};

float4 PSmain(PS_IN input) : SV_Target
{
    //==

    VS_OUT	vso = (VS_OUT)0;

    float2  UV;

    //vso.ProjPos = float4(g_NullPos[id], 0.f, 1.f);
    vso.Pos = vso.ProjPos;

    UV.x = input.ProjPos.x / input.ProjPos.w * 0.5f + 0.5f;
    UV.y = input.ProjPos.y / input.ProjPos.w * -0.5f + 0.5f;

    int2 LoadPos = (int2)0;

    LoadPos.x = (int)(UV.x * g_Resolution.x);
    LoadPos.y = (int)(UV.y * g_Resolution.y);

    float4 TexColor = tex.Load(LoadPos, 0);

    if (TexColor.a == 0.f)
        clip(-1);

    float4  ProjPos;

    // 여기에 구성한 값은 출력되어 있는 물체의 w로 나누어준 값이 저장된것이다.
    ProjPos.x = (UV.x * 2.f - 1.f) * TexColor.g;
    ProjPos.y = (UV.y * -2.f + 1.f) * TexColor.g;
    ProjPos.z = TexColor.r * TexColor.g;
    ProjPos.w = TexColor.g;

    // Normal은 뷰공간에 있으므로 뷰 공간의 위치를
    // 구해준다.
    float3 ViewPos;

    ViewPos.x = ProjPos.x / g_Proj11;
    ViewPos.y = ProjPos.y / g_Proj22;
    ViewPos.z = TexColor.g;

    float3  ViewNormal = TexColor.rgb;
    //==

    //SEARCH MAP
    float3 rgbN = FxaaTexOff(tex, input.UV.xy, int2(0, -1)).xyz;
    float3 rgbW = FxaaTexOff(tex, input.UV.xy, int2(-1, 0)).xyz;
    float3 rgbM = FxaaTexOff(tex, input.UV.xy, int2(0, 0)).xyz;
    float3 rgbE = FxaaTexOff(tex, input.UV.xy, int2(1, 0)).xyz;
    float3 rgbS = FxaaTexOff(tex, input.UV.xy, int2(0, 1)).xyz;
    float lumaN = FxaaLuma(rgbN);
    float lumaW = FxaaLuma(rgbW);
    float lumaM = FxaaLuma(rgbM);
    float lumaE = FxaaLuma(rgbE);
    float lumaS = FxaaLuma(rgbS);
    float rangeMin = min(lumaM, min(min(lumaN, lumaW), min(lumaS, lumaE)));
    float rangeMax = max(lumaM, max(max(lumaN, lumaW), max(lumaS, lumaE)));
    float range = rangeMax - rangeMin;
    if (range < max(FXAA_EDGE_THRESHOLD_MIN, rangeMax * FXAA_EDGE_THRESHOLD))
    {
        return float4(FxaaFilterReturn(rgbM), 1.0f);
    }
    float3 rgbL = rgbN + rgbW + rgbM + rgbE + rgbS;

    //COMPUTE LOWPASS
    #if FXAA_SUBPIX != 0
        float lumaL = (lumaN + lumaW + lumaE + lumaS) * 0.25;
        float rangeL = abs(lumaL - lumaM);
    #endif
    #if FXAA_SUBPIX == 1
        float blendL = max(0.0,
            (rangeL / range) - FXAA_SUBPIX_TRIM) * FXAA_SUBPIX_TRIM_SCALE;
        blendL = min(FXAA_SUBPIX_CAP, blendL);
    #endif


        //CHOOSE VERTICAL OR HORIZONTAL SEARCH
        float3 rgbNW = FxaaTexOff(tex, input.UV.xy, int2(-1, -1)).xyz;
        float3 rgbNE = FxaaTexOff(tex, input.UV.xy, int2(1, -1)).xyz;
        float3 rgbSW = FxaaTexOff(tex, input.UV.xy, int2(-1, 1)).xyz;
        float3 rgbSE = FxaaTexOff(tex, input.UV.xy, int2(1, 1)).xyz;
        #if (FXAA_SUBPIX_FASTER == 0) && (FXAA_SUBPIX > 0)
            rgbL += (rgbNW + rgbNE + rgbSW + rgbSE);
            rgbL *= float3(1.0 / 9.0,0,0);
        #endif
        float lumaNW = FxaaLuma(rgbNW);
        float lumaNE = FxaaLuma(rgbNE);
        float lumaSW = FxaaLuma(rgbSW);
        float lumaSE = FxaaLuma(rgbSE);
        float edgeVert =
            abs((0.25 * lumaNW) + (-0.5 * lumaN) + (0.25 * lumaNE)) +
            abs((0.50 * lumaW) + (-1.0 * lumaM) + (0.50 * lumaE)) +
            abs((0.25 * lumaSW) + (-0.5 * lumaS) + (0.25 * lumaSE));
        float edgeHorz =
            abs((0.25 * lumaNW) + (-0.5 * lumaW) + (0.25 * lumaSW)) +
            abs((0.50 * lumaN) + (-1.0 * lumaM) + (0.50 * lumaS)) +
            abs((0.25 * lumaNE) + (-0.5 * lumaE) + (0.25 * lumaSE));
        bool horzSpan = edgeHorz >= edgeVert;
        float lengthSign = horzSpan ? -rcpFrame.y : -rcpFrame.x;
        if (!horzSpan)
            lumaN = lumaW;
        if (!horzSpan)
            lumaS = lumaE;
        float gradientN = abs(lumaN - lumaM);
        float gradientS = abs(lumaS - lumaM);
        lumaN = (lumaN + lumaM) * 0.5;
        lumaS = (lumaS + lumaM) * 0.5;


        //CHOOSE SIDE OF PIXEL WHERE GRADIENT IS HIGHEST
        bool pairN = gradientN >= gradientS;
        if (!pairN)
            lumaN = lumaS;
        if (!pairN)
            gradientN = gradientS;
        if (!pairN)
            lengthSign *= -1.0;
        float2 PosN;
        PosN.x = input.UV.x + (horzSpan ? 0.0 : lengthSign * 0.5);
        PosN.y = input.UV.y + (horzSpan ? lengthSign * 0.5 : 0.0);

        //CHOOSE SEARCH LIMITING VALUES
        gradientN *= FXAA_SEARCH_THRESHOLD;

        //SEARCH IN BOTH DIRECTIONS UNTIL FIND LUMA PAIR AVERAGE IS OUT OF RANGE
        float2 PosP = PosN;
        float2 offNP = horzSpan ?
            float2(rcpFrame.x, 0.0) :
            float2(0.0f, rcpFrame.y);
        float lumaEndN = lumaN;
        float lumaEndP = lumaN;
        bool doneN = false;
        bool doneP = false;
        #if FXAA_SEARCH_ACCELERATION == 1
            PosN += offNP * float2(-1.0, -1.0);
            PosP += offNP * float2(1.0, 1.0);
        #endif
        for (int i = 0; i < FXAA_SEARCH_STEPS; i++)
        {
        #if FXAA_SEARCH_ACCELERATION == 1
            if (!doneN)
                lumaEndN =
                    FxaaLuma(FxaaTexLod0(tex, PosN.xy).xyz);
            if (!doneP)
                lumaEndP =
                    FxaaLuma(FxaaTexLod0(tex, PosP.xy).xyz);
        #endif
            doneN = doneN || (abs(lumaEndN - lumaN) >= gradientN);
            doneP = doneP || (abs(lumaEndP - lumaN) >= gradientN);
            if (doneN && doneP)
                break;
            if (!doneN)
                PosN -= offNP;
            if (!doneP)
                PosP += offNP;
        }


        //HANDLE IF CENTER IS ON PosITIVE OR NEGATIVE SIDE
        float dstN = horzSpan ? input.UV.x - PosN.x : input.UV.y - PosN.y;
        float dstP = horzSpan ? PosP.x - input.UV.x : PosP.y - input.UV.y;
        bool directionN = dstN < dstP;
        lumaEndN = directionN ? lumaEndN : lumaEndP;

        //CHECK IF PIXEL IS IN SECTION OF SPAN WHICH GETS NO FILTERING   
        if (((lumaM - lumaN) < 0.0) == ((lumaEndN - lumaN) < 0.0))
            lengthSign = 0.0;

        float spanLength = (dstP + dstN);
        dstN = directionN ? dstN : dstP;
        float subPixelOffset = (0.5 + (dstN * (-1.0 / spanLength))) * lengthSign;
        float3 rgbF = FxaaTexLod0(tex, float2(
        input.UV.x + (horzSpan ? 0.0 : subPixelOffset),
        input.UV.y + (horzSpan ? subPixelOffset : 0.0))).xyz;
       return float4(FxaaFilterReturn(FxaaLerp3(rgbL, rgbF, blendL)), 1.0f);
}