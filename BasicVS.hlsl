#include"Basic.hlsli"

//P04_01_P25
VSOutput main(float4 pos : POSITION, float3 normal : NORMAL,float2 uv : TEXCOORD,float4 rgba : RGBA)
{
	VSOutput output; //ピクセルシェーダーに渡す値
	//output.svpos = pos;
	 
	//システム用頂点座標
	output.svpos = mul(mat, pos);//座標に3D変換行列を乗算
	//UV値
	output.uv = uv;
	//法線ベクトル
	output.normal = normal;
	//カラー
	output.rgba = rgba;

	return output;
}

//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos + float4( 0,0,1,1);
//}