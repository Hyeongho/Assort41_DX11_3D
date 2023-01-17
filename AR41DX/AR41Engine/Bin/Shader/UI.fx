
#include "Share.fx"

cbuffer UICBuffer : register(b10)
{
    float4 g_UITint;
    matrix g_UIWVP;
    float2 g_UIMeshSize;
    float2 g_UIPivot;
    int g_UITextureEnable;
    float g_UIOpacity;
    float2 g_UIEmpty;
};

cbuffer ProgressBarCBuffer : register(b11)
{
    int g_ProgressBarDir;
    float g_ProgressBarPercent;
    float2 g_ProgressBarEmpty;
};

struct VS_INPUT_UV
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_OUTPUT_UV
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};


// ���ڷ� VS_INPUT_COLOR�� ���� �� ����ü�� ������ �Է� �������Ϳ��� ����
// �����ͼ� ����ϹǷ� ���ڸ� ���� �־��� �ʿ䰡 ����.
VS_OUTPUT_UV UIVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    float2 Pos = input.Pos.xy - g_UIPivot * g_UIMeshSize;
    
    // mul : ��� ��. g_matWVP �� World * View * Proj �̹Ƿ� ������ ���⿡ ���ϰ� �Ǹ�
    // ���� �������� ��ȯ�� ������ ��ġ�� ���´�.
    output.Pos = mul(float4(Pos, input.Pos.z, 1.f), g_UIWVP);
    
    if (g_UITextureEnable)
        output.UV = UpdateAnimation2D(input.UV);
    
    else
        output.UV = input.UV;
    
    return output;
}

PS_OUTPUT_SINGLE UIPS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    if (g_UITextureEnable)
    {
        float4 TextureColor = g_BaseTexture.Sample(g_PointSmp, input.UV);
    
        output.Color.rgb = TextureColor.rgb * g_UITint.rgb;

        output.Color.a = TextureColor.a * g_UIOpacity;
    }
    
    else
    {
        output.Color.rgb = g_UITint.rgb;
        output.Color.a = g_UITint.a * g_UIOpacity;
        
    }
    
    return output;
}

#define ProgressBar_RightToLeft 0
#define ProgressBar_LeftToRight 1
#define ProgressBar_TopToBottom 2
#define ProgressBar_BottomToTop 3



float2 UpdateProgressBarAnimation2D(float2 UV)
{
    if (g_Anim2DEnable == 0)
        return UV;
    
    float2 Result = (float2) 0;
    
    if (g_Anim2DType == Anim2D_Atlas)
    {
        if (UV.x == 0.f)
            Result.x = g_Anim2DFrameStart.x / g_Anim2DImageWidth;
        else
            Result.x = g_Anim2DFrameEnd.x / g_Anim2DImageWidth;
    
        if (UV.y == 0.f)
            Result.y = g_Anim2DFrameStart.y / g_Anim2DImageHeight;
        else
            Result.y = g_Anim2DFrameEnd.y / g_Anim2DImageHeight;
    
        float Width = (g_Anim2DFrameEnd.x - g_Anim2DFrameStart.x) / g_Anim2DImageWidth;
        float Height = (g_Anim2DFrameEnd.y - g_Anim2DFrameStart.y) / g_Anim2DImageHeight;
        
        if (g_ProgressBarDir == ProgressBar_RightToLeft)
        {
            if (UV.x == 1.f)
                Result.x -= (Width * (1.f - g_ProgressBarPercent));
        }
        
        if (g_ProgressBarDir == ProgressBar_LeftToRight)
        {
            if (UV.x == 0.f)
                Result.x += (Width * (1.f - g_ProgressBarPercent));
        }
        
        if (g_ProgressBarDir == ProgressBar_TopToBottom)
        {
            if (UV.y == 0.f)
                Result.y += (Height * (1.f - g_ProgressBarPercent));
        }
        
        if (g_ProgressBarDir == ProgressBar_BottomToTop)
        {
            if (UV.y == 1.f)
                Result.y -= (Height * (1.f - g_ProgressBarPercent));
        }
    }
    
    else
    {
        Result = UV;
        
        // ������ -> ����
        if (g_ProgressBarDir == ProgressBar_RightToLeft)
        {
            if (Result.x == 1.f)
                Result.x = g_ProgressBarPercent;
        }
    
    
        // ���� -> ������
        if (g_ProgressBarDir == ProgressBar_LeftToRight)
        {
            if (Result.x == 0.f)
                Result.x = 1.f - g_ProgressBarPercent;
        }
    
    
        // �� -> �Ʒ�
        if (g_ProgressBarDir == ProgressBar_TopToBottom)
        {
            if (Result.y == 0.f)
                Result.y = 1.f - g_ProgressBarPercent;
        }
    
    
        // �Ʒ� -> ��
        if (g_ProgressBarDir == ProgressBar_BottomToTop)
        {
            if (Result.y == 1.f)
                Result.y = g_ProgressBarPercent;
        }
    }
        
    return Result;
}


VS_OUTPUT_UV ProgressBarVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    float2 Pos = input.Pos.xy;
    float2 UV = input.UV;
    
    
    // ������ -> ����
    if (g_ProgressBarDir == ProgressBar_RightToLeft)
    {
        if (Pos.x == 1.f)
            Pos.x = g_ProgressBarPercent;
    }
    
    
    // ���� -> ������
    if (g_ProgressBarDir == ProgressBar_LeftToRight)
    {
        if (Pos.x == 0.f)
            Pos.x = 1.f - g_ProgressBarPercent;
    }
    
    
    // �� -> �Ʒ�
    if (g_ProgressBarDir == ProgressBar_TopToBottom)
    {
        if (Pos.y == 1.f)
            Pos.y = g_ProgressBarPercent;
    }
    
    
    // �Ʒ� -> ��
    if (g_ProgressBarDir == ProgressBar_BottomToTop)
    {
        if (Pos.y == 0.f)
            Pos.y = 1.f - g_ProgressBarPercent;
    }
    
    Pos = Pos - g_UIPivot * g_UIMeshSize;
    
    // mul : ��� ��. g_matWVP �� World * View * Proj �̹Ƿ� ������ ���⿡ ���ϰ� �Ǹ�
    // ���� �������� ��ȯ�� ������ ��ġ�� ���´�.
    output.Pos = mul(float4(Pos, input.Pos.z, 1.f), g_UIWVP);
    
    if (g_UITextureEnable)
        output.UV = UpdateProgressBarAnimation2D(input.UV);
    else
        output.UV = input.UV;
    
    return output;
}
