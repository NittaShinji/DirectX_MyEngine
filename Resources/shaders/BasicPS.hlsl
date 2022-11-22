#include"Basic.hlsli"

//Texture2D<float4> tex : register(t0);
//SamplerState smp : register(s0);

//float4 main(VSOutput input) : SV_TARGET
//{
//	float4 texcolor = float4(tex.Sample(smp,input.uv));
//	float3 light = normalize(float3(1,-1,1));// 右下奥　向きのライト
//	float diffuse = saturate(dot(-light, input.normal)); //diffuseを[0,1]の範囲にclampする
//	//float brightness = dot(-light, input.normal); // 光源へのベクトルと法線ベクトルの内積
//	float brightness = diffuse + 0.3f; // アンビエント項を0.3として計算

//	return float4(texcolor.rgb * brightness, texcolor.a) * color; 

//	//return float4(input.normal,1);	//RGBをそれぞれ法線のXYZ,Aを1で出力
//	//return float4(tex.Sample(smp,input.uv)) * color;
//	//return float4(1,1,1,1);
//}

float4 main() : SV_Target
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);

}