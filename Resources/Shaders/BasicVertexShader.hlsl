#include "BasicShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	//output.svpos = mul(mat, pos);
	//output.normal = normal;
	//output.uv = uv;
    output.pos = pos;
		
    return output;
}