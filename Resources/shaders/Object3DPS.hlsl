#include"Object3D.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float3 light = normalize(float3(1, -1, 1)); // 右下奥　向きのライト
    float light_diffuse = saturate(dot(-light, input.normal)); //diffuseを[0,1]の範囲にclampする
    float3 shade_color;
    
    shade_color = m_ambient; // アンビエント項
    shade_color += m_diffuse * light_diffuse; //ディフューズ項
    float4 texcolor = float4(tex.Sample(smp, input.uv));
	
    return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha);
    
    //return float4(tex.Sample(smp, input.uv));

}

//cbuffer ConstBufferDataMaterial : register(b0)
//{
//    float4 color; //色(RGBA)
//}

//float4 main(VSOutput input) : SV_TARGET
//{
//    //return color;
//    //return float4(0.1f, 0.1f, 0.1f, 1.0f);
//    return float4(tex.Sample(smp, input.uv));
//    //return float4(input.uv, 0, 1);

//}