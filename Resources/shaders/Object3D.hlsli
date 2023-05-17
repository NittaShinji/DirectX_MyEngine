static const int DIRLIGHT_NUM = 3;

struct DirLight
{
    float3 lightv;
    float3 lightColor;
    uint active;
};

cbuffer ConstBufferDateTransform : register(b0)
{
    ////3D変換行列
    //matrix mat; 
    //ビュープロジェクション行列
    matrix viewproj;
    //ワールド行列
    matrix world;
    //カメラ座標(ワールド行列)
    float3 cameraPos;
}

cbuffer ConstBufferDateMaterial : register(b1)
{
	
    float3 m_ambient : packoffset(c0);  //アンビエント係数
    float3 m_diffuse : packoffset(c1);  //ディフューズ係数
    float3 m_specular : packoffset(c2); //スペキュラー係数
    float m_alpha : packoffset(c2.w);   //アルファ
};

cbuffer cbuff2 : register(b2)
{
    float3 ambientColor;
    DirLight dirLights[DIRLIGHT_NUM];
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
//    float4 svpos : SV_POSITION;
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
    //float3 normal : NORMAL;
    //ワールド座標
    float4 worldpos : POSITION;
    //法線
    float3 normal : NORMAL;
	////uv値
    float2 uv : TEXCOORD;
    
};