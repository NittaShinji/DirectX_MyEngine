 struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR; //�F
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR; //�F
};

cbuffer ConstBufferDateTransform : register(b0)
{
    //�r���[�v���W�F�N�V�����s��
    matrix viewproj;
    //���[���h�s��
    matrix world;
    //�J�������W(���[���h�s��)
    float3 cameraPos;
    //float4 color;
}

Texture2D swordTex : register(t0);
SamplerState swordSampler : register(s0);