#include"Basic.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//return float4(input.normal,1);	//RGB‚ğ‚»‚ê‚¼‚ê–@ü‚ÌXYZ,A‚ğ1‚Åo—Í
	return float4(tex.Sample(smp,input.uv)) * color;
	//return float4(1,1,1,1);
}