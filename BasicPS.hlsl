cbuffer ConstBufferDateMaterial : register(b0)
{
	float4 color; //F(RGBA)
};

float4 main() : SV_TARGET
{
	return color;
}