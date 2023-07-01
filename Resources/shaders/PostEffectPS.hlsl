#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);

SamplerState smp : register(s0);

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
    //色反転
    //float4 colortex0 = float4(1 - tex0.Sample(smp, input.uv).rgb,1);
    float4 colortex0 = float4(tex0.Sample(smp, input.uv).rgb,1);
    
   
    //ガウシアンブラー
    float totalWeight = 0, _Sigma = 0.005, _StepWidth = 0.001;
    float4 colortex1 = float4(0, 0, 0, 0);

    for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
    {
        for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
        {
            float2 pickUV = input.uv + float2(px, py);
            float weight = Gaussian(input.uv, pickUV, _Sigma);
            colortex1 += tex1.Sample(smp, pickUV) * weight;
            totalWeight += weight;
        }
    }

    //縞模様で表示
    colortex1.rgb = colortex1.rgb / totalWeight;
    colortex1.a = 1;
    
    float4 color = colortex0;
    //if (fmod(input.uv.y, 0.1f) < 0.05f)
    //{
    //    color = colortex1;
    //}
    
    return float4(color.rgb, 1);
    
    
}