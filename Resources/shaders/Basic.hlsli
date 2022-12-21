

cbuffer ConstBufferDateTransform : register(b0)
{
    matrix mat; //3D�ϊ��s��
}
cbuffer ConstBufferDateMaterial : register(b1)
{
	//�F(RGBA)
    float4 color;
};

//3D�ϊ��s��
//cbuffer cbuff1 : register(b1)
//{
//    float3 m_ambient : packoffset(c0); //�A���r�G���g�W��
//	float3 m_diffuse : packoffset(c1); //�f�B�t���[�Y�W��
//	float3 m_specular : packoffset(c2); //�X�y�L�����[�W��
//    float m_alpha : packoffset(c2.w);	//�A���t�@

//}

//���_�V�F�[�_�[�̏o�͍\����
//(���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����)
//struct VSOutput
//{
//	//�V�X�e���p���_���W
//    float4 svpos : SV_POSITION;
//	//�@���x�N�g��
//    float3 normal : NORMAL;
//	//uv�l
//    float2 uv : TEXCOORD;
//};
	
struct VSOutput
{
	//�V�X�e���p���_���W
    float4 svpos : SV_POSITION;
	////�@���x�N�g��
    float3 normal : NORMAL;
	////uv�l
    float2 uv : TEXCOORD;
};