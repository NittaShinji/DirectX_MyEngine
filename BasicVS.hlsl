#include"Basic.hlsli"

//P04_01_P25
VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output; //ピクセルシェーダーに渡す値
	//output.svpos = pos;
	 
	//システム用頂点座標
	output.svpos = mul(mat, pos);//座標に3D変換行列を乗算
	//UV値
	output.uv = uv;
	return output;
}

//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos + float4( 0,0,1,1);
//}