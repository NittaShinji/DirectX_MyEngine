#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float scale : TEXCOORD, float4 color : COLOR, float rotate : ROTATE)
{
	VSOutput output; // ピクセルシェーダーに渡す値
    output.pos = pos;
    output.scale = scale;
    output.color = color;
    output.rotate = rotate;
		
    return output;
}