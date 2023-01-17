
#include "Share.fx"

/*
입력레지스터, 출력레지스터, 상수레지스터, 임시레지스터
VertexShader에서의 레지스터

레지스터는 이름 + 번호로 만들어지게 된다.
예를 들어 POSITION 레지스터라면 POSITION0 ~ 15 번까지 이용이 가능하다.
0번일 경우 번호 생략이 가능해서 POSITION 이라고 하면 0번 레지스터를 의미한다.

입력레지스터 : 출력하는 메쉬의 정점정보가 들어가 있게 된다.
메쉬를 출력할때 정점이 어떤 구조로 만들어진 정점인지를 지정한 후에 출력을
하게 된다.

출력레지스터 : Shader에서 연산을 마무리한 후에 결과값을 저장해두기 위한
레지스터이다. VertexShader의 경우 연산된 투영공간에서의 위치 정보를 넣어준다던지
아니면 다른 Shader에서 필요로 하는 추가적인 정보들도 출력레지스터 넣어두면
뒤의 Shader들이 가져가서 사용할 수 있다.

상수레지스터 : C++코드가 가지고 있는 정보를 Shader에 넘겨줄때 사용한다.

임시레지스터 : C++연산처럼 연산할때 필요한 레지스터이다.
*/

// float : 실수타입 float2 : x, y를 가지고 있는 Vector2, float3, float4 다 됨.
// int : 정수타입 int2 : x, y를 int타입으로 가지고 있는 Vector2 타입
// Semantic : 레지스터를 지정한다.

// 아래처럼 지정하면 입력레지스터의 POSITION0, COLOR0 레지스터에 들어있는
// 값을 가져와서 사용하겠다는 것이다.
struct VS_INPUT_COLOR
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
};

// SV : System Value로 사용되는 값으로 출력레지스터에서 사용할 수 있다.
// 출력레지스터에 아래처럼 SV_POSITION 을 넣으면 최종 투영공간에서의 위치를
// 지정하는것으로 이 값을 이용해서 화면의 어디에 출력할지를 결정한다.
// SV 값은 다른 Shader에서 절대로 건드리면 안된다.
struct VS_OUTPUT_COLOR
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
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


// 인자로 VS_INPUT_COLOR를 쓰면 저 구조체에 지정된 입력 레지스터에서 값을
// 가져와서 사용하므로 인자를 따로 넣어줄 필요가 없다.
VS_OUTPUT_COLOR SpriteColorVS(VS_INPUT_COLOR input)
{
    VS_OUTPUT_COLOR output = (VS_OUTPUT_COLOR) 0;

    float3  Pos = input.Pos - g_Pivot * g_MeshSize;
    
    // mul : 행렬 곱. g_matWVP 는 World * View * Proj 이므로 정점을 여기에 곱하게 되면
    // 투영 공간으로 변환된 정점의 위치가 나온다.
    output.Pos = mul(float4(Pos, 1.f), g_matWVP);
    output.Color = input.Color;
    
    return output;
}

PS_OUTPUT_SINGLE SpriteColorPS(VS_OUTPUT_COLOR input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;
    
    output.Color = input.Color;
    
    return output;
}

// 인자로 VS_INPUT_COLOR를 쓰면 저 구조체에 지정된 입력 레지스터에서 값을
// 가져와서 사용하므로 인자를 따로 넣어줄 필요가 없다.
VS_OUTPUT_UV SpriteVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    float3 Pos = input.Pos - g_Pivot * g_MeshSize;
    
    // mul : 행렬 곱. g_matWVP 는 World * View * Proj 이므로 정점을 여기에 곱하게 되면
    // 투영 공간으로 변환된 정점의 위치가 나온다.
    output.Pos = mul(float4(Pos, 1.f), g_matWVP);
    output.UV = UpdateAnimation2D(input.UV);
    
    return output;
}

PS_OUTPUT_SINGLE SpritePS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    float4  TextureColor = g_BaseTexture.Sample(g_PointSmp, input.UV);
    
    output.Color.rgb = TextureColor.rgb * g_MtrlBaseColor.rgb;

    output.Color.a = TextureColor.a * g_MtrlOpacity;
    
    return output;
}

