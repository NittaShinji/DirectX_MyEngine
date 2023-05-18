

cbuffer ConstBufferDateTransform : register(b0)
{
    ////�r���[�v���W�F�N�V�����s��
    matrix viewproj;
    //���[���h�s��
    matrix world;
    ////�J�������W(���[���h�s��)
    float3 cameraPos;
}

cbuffer ConstBufferDateMaterial : register(b1)
{
    float3 m_ambient : packoffset(c0);  //�A���r�G���g�W��
    float3 m_diffuse : packoffset(c1);  //�f�B�t���[�Y�W��
    float3 m_specular : packoffset(c2); //�X�y�L�����[�W��
    float m_alpha : packoffset(c2.w);   //�A���t�@
};

cbuffer cbuffer2 : register(b2)
{
    float3 lightv;
    float3 lightcolor;
}
	
struct VSOutput
{
    //�V�X�e���p���_���W
    float4 svpos : SV_POSITION;
    //�F
    float4 color : COLOR;
	////uv�l
    float2 uv : TEXCOORD;
};