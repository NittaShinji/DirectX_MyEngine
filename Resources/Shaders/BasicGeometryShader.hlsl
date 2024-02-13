#include "BasicShaderHeader.hlsli"

//[maxvertexcount(3)]
//void main(
//	point VSOutput input[1] : SV_POSITION,
//	inout TriangleStream< GSOutput > output
//)
//{
//	GSOutput element;
//	element.normal = input[0].normal;
//	element.uv = input[0].uv;

//	element.svpos = input[0].svpos;
//	output.Append(element);

//	element.svpos = input[0].svpos + float4(10.0f,10.0f,0,0);
//	output.Append(element);

//	element.svpos = input[0].svpos + float4(10.0f, 0, 0,0);
//	output.Append(element);

//}
	
//�l�p�`�̒��_��
static const uint vnum = 4;

static const float2 uv_array[vnum] =
{
    float2(0, 1),
	float2(0, 0),
	float2(1, 1),
	float2(1, 0)
};

//�Z���^�[����̃I�t�Z�b�g
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
	//4�_����
    for (uint i = 0; i < vnum; i++)
    {
		//���[���h���W�x�[�X�ł��炷
        element.svpos = input[0].pos + offset_array[i];
		//�r���[�A�ˉe�ϊ�
        element.svpos = mul(mat, element.svpos);
        //element.uv = float2(0.5f, 0.5f);
        element.uv = uv_array[i];
        output.Append(element);

    }
	
}

