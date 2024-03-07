cbuffer ConstBufferDateTransform : register(b0)
{
    matrix mat; //3D�ϊ��s��
}
cbuffer ConstBufferDateMaterial : register(b1)
{
	//�F(RGBA)
    float4 color;
};

struct VSOutput
{
	//�V�X�e���p���_���W
    float4 svpos : SV_POSITION;
	////�@���x�N�g��
    float3 normal : NORMAL;
	////uv�l
    float2 uv : TEXCOORD;
};