cbuffer cbuff0 : register(b0)
{
    matrix mat;          // ３Ｄ変換行列
    matrix matBillboard; //ビルボード行列
};
// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{	
    float4 pos : POSITION; //頂点座標
    float scale : TEXCOORD; //スケール
    float4 color : COLOR;
    float rotate : ROTATE;
};

struct GSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};