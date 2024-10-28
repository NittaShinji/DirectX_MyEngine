#include "TrailHF.hlsli"

//float4 main() : SV_TARGET
//{
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(PS_IN input) : SV_TARGET0
{
    //float4 color = return float4(tex.Sample(smp, input.uv)) * color;
    //return color;
    
    return float4(tex.Sample(smp, input.uv)) * input.color;
}