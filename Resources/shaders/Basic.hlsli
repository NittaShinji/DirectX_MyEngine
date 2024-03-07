cbuffer ConstBufferDateTransform : register(b0)
{
    matrix mat; //3D変換行列
}
cbuffer ConstBufferDateMaterial : register(b1)
{
	//色(RGBA)
    float4 color;
};

struct VSOutput
{
	//システム用頂点座標
    float4 svpos : SV_POSITION;
	////法線ベクトル
    float3 normal : NORMAL;
	////uv値
    float2 uv : TEXCOORD;
};