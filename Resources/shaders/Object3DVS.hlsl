#include"Object3D.hlsli"

//P04_01_P25
//VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
//{
//    VSOutput output; //ピクセルシェーダーに渡す値
//	//output.svpos = pos;
	 
//	//システム用頂点座標
//    output.svpos = mul(mat, pos); //座標に3D変換行列を乗算
//	//UV値
//    output.uv = uv;
//    output.normal = normal;
//    return output;
//}

//float4 main() : SV_TARGET
//{
//	return float4(1.0f,1.0f,1.0f,1.0f);
//}

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL,float2 uv : TEXCOORD)
{
 //   //法線にワールド行列によるスケーリング・回転を適用
 //   float4 wnormal = normalize(mul(world, float4(normal, 0)));
 //   float4 wpos = mul(world, pos);
	
	////環境反射光
 //   float3 ambient = m_ambient;
	////拡散反射光
 //   float3 diffuse = dot(lightv, wnormal.xyz) * m_diffuse;
	////視点座標
 //   float3 eyeDir = normalize(cameraPos - wpos.xyz);
	
	////光沢度
 //   const float shininess = 4.0f;
	////反射光ベクトル
 //   float3 reflect = normalize(-lightv + 2 * dot(-lightv, wnormal.xyz) * wnormal.xyz);
	////鏡面反射光
 //   float3 specuar = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;

 //   VSOutput output; //ピクセルシェーダーに渡す値
 //   output.svpos = mul(mul(viewproj, world), pos);
 //   output.color.rgb = (ambient + diffuse + specuar) * lightcolor;
 //   output.color.a = m_alpha;
 //   output.uv = uv;
 //   return output;
    
    VSOutput output; //ピクセルシェーダーに渡す値
    
    //matrix mat = world * viewproj;
    //ここでmatを使っている
    output.svpos = mul(mul(viewproj, world), pos);
    output.normal = normal;
    output.uv = uv;
    return output;
    
}
    //return pos + float4(0, 0, 1, 1);
    
//}

//float4 main(float4 pos : POSITION) : SV_POSITION
//{
    
//    return mul(mat, pos);
//    //VSOutput output; //ピクセルシェーダーに渡す値
//    //output.svpos = mul(mat, pos);
//    //output.normal = normal;
//    //output.uv = uv;
//    //return output;
    
//    //return pos + float4(0, 0, 1, 1);
    
//}