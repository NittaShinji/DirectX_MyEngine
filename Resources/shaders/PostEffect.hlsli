cbuffer ConstBufferDateMaterial : register(b0)
{

};

//3D�ϊ��s��
cbuffer ConstBufferDataTransform : register(b1)
{

}

//���_�V�F�[�_�[�̏o�͍\����
//(���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����)
struct VSOutput
{
	//�V�X�e���p���_���W
    float4 svpos : SV_POSITION;
	//uv�l
    float2 uv : TEXCOORD;
};