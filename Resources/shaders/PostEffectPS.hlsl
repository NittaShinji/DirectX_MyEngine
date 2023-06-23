#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);
    return float4(1.0f - texcolor.r, 1.0f - texcolor.g, 1.0f - texcolor.b,1);
    //return float4(1, 1, 1, 1);
}