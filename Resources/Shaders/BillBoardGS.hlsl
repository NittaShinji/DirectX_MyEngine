#include "BillBoard.hlsli"

//四角形の頂点数
static const uint vnum = 4;

static const float2 uv_array[vnum] =
{
    float2(0, 1),
	float2(0, 0),
	float2(1, 1),
	float2(1, 0)
};

//センターからのオフセット
static const float4 offset_array[vnum] =
{
	float4(-0.5f, -0.5f, 0, 0),
	float4(-0.5f, +0.5f, 0, 0),
	float4(+0.5f, -0.5f, 0, 0),
	float4(+0.5f, +0.5f, 0, 0),
};
	
[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)

{
    GSOutput element;
	//4点分回す
    for (uint i = 0; i < vnum; i++)
    {
		//中心からのオフセットをスケーリング+ビルボード回転(モデル座標)
        float4 offset = offset_array[i] * input[0].scale;
		
        float x = offset.x;
        float y = offset.y;
        offset.x = cos(input[0].rotate) * x - sin(input[0].rotate) * y;
        offset.y = sin(input[0].rotate) * x + cos(input[0].rotate) * y;
        offset = mul(matBillboard, offset);
		
		//オフセット分ずらす(ワールド座標)
        element.svpos = input[0].pos + offset;
		//ビュー、射影変換
        element.svpos = mul(mat, element.svpos);
        element.uv = uv_array[i];
        element.color = input[0].color;
        output.Append(element);
		
		////中心からのオフセットをビルボード回転(モデル座標)
  //      float4 offset = mul(matBillboard, offset_array[i]);
		////オフセット分ずらす(ワールド座標)
  //      element.svpos = input[0].pos + offset;
		
		//////ワールド座標ベースでずらす
		////ビュー、射影変換
  //      element.svpos = mul(mat, element.svpos);
  //      element.uv = uv_array[i];
  //      output.Append(element);
    }
}

