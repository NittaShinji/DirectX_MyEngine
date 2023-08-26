#include"Object3D.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};

PSOutput main(GSOutput input)
{
    PSOutput output;
    
    //テクスチャマッピング
    float4 texcolor = tex.Sample(smp, input.uv);
	
	//光沢度
    const float shininess = 4.0f;
	
	//頂点から視点への方向ベクトル
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	
	//環境反射光
    float3 ambient = m_ambient;
	
	//シェーディングによる色
    float4 shadeColor = float4(ambientColor * ambient, m_alpha);
	
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
			//ライトに向かうベクトルと法線の内積
            float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
			//反射光ベクトル
            float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			//拡散反射光
            float3 diffuse = dotlightnormal * m_diffuse;
			//鏡面反射光の計算
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
			//全て加算する
            shadeColor.rgb += (diffuse + specular) * dirLights[i].lightColor;
        }
    }
	
    //陰影とテクスチャの色を合成
    output.target0 = shadeColor * texcolor;
    
    output.target1 = float4((shadeColor * texcolor).rgb, 1);
    
	//シェーディングによる色で描画
    return output;
}