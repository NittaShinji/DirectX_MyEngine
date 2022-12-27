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
    
    //return pos;
    VSOutput output; //ピクセルシェーダーに渡す値
    output.svpos = mul(mat, pos);
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