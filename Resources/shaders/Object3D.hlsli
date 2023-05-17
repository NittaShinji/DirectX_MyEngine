static const int DIRLIGHT_NUM = 3;

struct DirLight
{
    float3 lightv;
    float3 lightColor;
    uint active;
};

cbuffer ConstBufferDateTransform : register(b0)
{
    ////3D�ϊ��s��
    //matrix mat; 
    //�r���[�v���W�F�N�V�����s��
    matrix viewproj;
    //���[���h�s��
    matrix world;
    //�J�������W(���[���h�s��)
    float3 cameraPos;
}

cbuffer ConstBufferDateMaterial : register(b1)
{
	
    float3 m_ambient : packoffset(c0);  //�A���r�G���g�W��
    float3 m_diffuse : packoffset(c1);  //�f�B�t���[�Y�W��
    float3 m_specular : packoffset(c2); //�X�y�L�����[�W��
    float m_alpha : packoffset(c2.w);   //�A���t�@
};

cbuffer cbuff2 : register(b2)
{
    float3 ambientColor;
    DirLight dirLights[DIRLIGHT_NUM];
}

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
    //float3 normal : NORMAL;
    //���[���h���W
    float4 worldpos : POSITION;
    //�@��
    float3 normal : NORMAL;
	////uv�l
    float2 uv : TEXCOORD;
    
};