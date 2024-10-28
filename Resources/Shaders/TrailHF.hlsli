 struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR; //色
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR; //色
};

cbuffer ConstBufferDateTransform : register(b0)
{
    //ビュープロジェクション行列
    matrix viewproj;
    //ワールド行列
    matrix world;
    //カメラ座標(ワールド行列)
    float3 cameraPos;
    //float4 color;
}

Texture2D swordTex : register(t0);
SamplerState swordSampler : register(s0);