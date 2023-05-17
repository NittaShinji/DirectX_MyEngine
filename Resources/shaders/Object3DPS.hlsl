#include"Object3D.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    ////テクスチャマッピング
    //float4 texcolor = float4(tex.Sample(smp, input.uv));
	
    ////return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha) * color;
    ////シェーディングによる色で描画
    //return input.color * texcolor;
    ////return float4(tex.Sample(smp, input.uv));

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
	
	//平行光源
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
    
    //シェーディングによる色で描画
    return shadeColor * texcolor;
	
    
}