#include "BillBoard.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(GSOutput input) : SV_TARGET
{
    //return tex.Sample(smp, input.uv);
    return float4(tex.Sample(smp, input.uv)) * input.color;
}