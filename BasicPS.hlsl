#include"Basic.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
	return float4(input.uv,0,1);
}