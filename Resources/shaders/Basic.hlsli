//cbuffer ConstBufferDateMaterial : register(b0)
//{
//	//色(RGBA)
//	float4 color;
//};

cbuffer ConstBufferDateTransform : register(b0)
{
    matrix mat; //3D変換行列
}

//3D変換行列
//cbuffer cbuff1 : register(b1)
//{
//    float3 m_ambient : packoffset(c0); //アンビエント係数
//	float3 m_diffuse : packoffset(c1); //ディフューズ係数
//	float3 m_specular : packoffset(c2); //スペキュラー係数
//    float m_alpha : packoffset(c2.w);	//アルファ

//}

//頂点シェーダーの出力構造体
//(頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体)
//struct VSOutput
//{
//	//システム用頂点座標
//	float4 svpos : SV_POSITION;	
//	//法線ベクトル
//    float3 normal : NORMAL;
//	//uv値
//    float2 uv : TEXCOORD;
//};
	
struct VSOutput
{
	//システム用頂点座標
	float4 svpos : SV_POSITION;	
	////法線ベクトル
 //   float3 normal : NORMAL;
	////uv値
 //   float2 uv : TEXCOORD;
};