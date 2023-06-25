#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
   
    //const float xSize = 1280.0f;
    //const float ySize = 720.0f;
    
    //float pixelU = 1.0f / xSize;
    //float pixelV = 1.0f / ySize;
    //float4 texcolor = { 0, 0, 0, 0 };
    
    //const uint xMath = 2;
    //const uint yMath = 2;
    
    ////c•ûŒü‚ÉˆÚ“®‚µ‚È‚ª‚çŒJ‚è•Ô‚·ˆ×‚Ìfor•¶
    //for (uint i = 0; i < xMath; i++)
    //{
    //    //‰¡•ûŒü‚ÉˆÚ“®‚µ‚È‚ª‚çŒJ‚è•Ô‚·ˆ×‚Ìfor•¶
    //    for (uint j = 0; j < yMath; j++)
    //    {
    //        float4 leftUpColor = tex.Sample(smp, input.uv + float2(-pixelU + i, -pixelV + j));
    //        texcolor += leftUpColor;
    //        float4 midUpColor = tex.Sample(smp, input.uv + float2(0 + i, -pixelV + j));
    //        texcolor += midUpColor;
    //        float4 rightUpColor = tex.Sample(smp, input.uv + float2(pixelU + i, -pixelV + j));
    //        texcolor += rightUpColor;
    
    //        float4 leftMidColor = tex.Sample(smp, input.uv + float2(-pixelU + i, 0 + j));
    //        texcolor += leftMidColor;
    //        float4 midMidColor = tex.Sample(smp, input.uv + float2(0 + i, 0 + j));
    //        texcolor += midMidColor;
    //        float4 rightMidColor = tex.Sample(smp, input.uv + float2(pixelU + i, 0 + j));
    //        texcolor += rightMidColor;
    
    //        float4 leftRightColor = tex.Sample(smp, input.uv + float2(-pixelU + i, pixelV + j));
    //        texcolor += leftRightColor;
    //        float4 midRightColor = tex.Sample(smp, input.uv + float2(0 + i, pixelV + j));
    //        texcolor += midRightColor;
    //        float4 rightRightColor = tex.Sample(smp, input.uv + float2(pixelU + i, pixelV + j));
    //        texcolor += rightRightColor;
    //    }
    //}
    
    //texcolor = texcolor.rgb / (xMath * yMath), 1);
    
    float4 texcolor = tex.Sample(smp, input.uv + float2(0.0f, 0.0f));
    return float4(texcolor.rgb * 2.0f,1);
    //return float4(1, 1, 1, 1);
}