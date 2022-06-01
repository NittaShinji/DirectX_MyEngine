#include"Basic.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output; //ピクセルシェーダーに渡す値
	output.svpos = pos;
	output.uv = uv;
	return output;
}

//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos + float4( 0,0,1,1);
//}