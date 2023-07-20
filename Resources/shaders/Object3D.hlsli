static const int DIRLIGHT_NUM = 3;

struct DirLight
{
    float3 lightv;
    float3 lightColor;
    uint active;
};

cbuffer ConstBufferDateTransform : register(b0)
{
    //ビュープロジェクション行列
    matrix viewproj;
    //ワールド行列
    matrix world;
    //カメラ座標(ワールド行列)
    float3 cameraPos;
    //float4 color;
}

cbuffer ConstBufferDateMaterial : register(b1)
{
    float3 m_ambient : packoffset(c0);  //アンビエント係数
    float3 m_diffuse : packoffset(c1);  //ディフューズ係数
    float3 m_specular : packoffset(c2); //スペキュラー係数
    float m_alpha : packoffset(c2.w);   //アルファ
};

cbuffer cbuffer2 : register(b2)
{
    float3 ambientColor;
    DirLight dirLights[DIRLIGHT_NUM]; //ライトの色(RGB)
}
	
struct VSOutput
{
    //システム用頂点座標
    float4 svpos : SV_POSITION;
    // 法線ベクトル
    float3 normal : NORMAL; 
    //ワールド座標
    float4 worldpos : POSITION; 
	////uv値
    float2 uv : TEXCOORD;
};