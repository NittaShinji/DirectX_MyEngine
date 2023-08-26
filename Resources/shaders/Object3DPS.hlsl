#include"Object3D.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};

PSOutput main(GSOutput input)
{
    PSOutput output;
    
    //�e�N�X�`���}�b�s���O
    float4 texcolor = tex.Sample(smp, input.uv);
	
	//����x
    const float shininess = 4.0f;
	
	//���_���王�_�ւ̕����x�N�g��
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	
	//�����ˌ�
    float3 ambient = m_ambient;
	
	//�V�F�[�f�B���O�ɂ��F
    float4 shadeColor = float4(ambientColor * ambient, m_alpha);
	
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
			//���C�g�Ɍ������x�N�g���Ɩ@���̓���
            float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
			//���ˌ��x�N�g��
            float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			//�g�U���ˌ�
            float3 diffuse = dotlightnormal * m_diffuse;
			//���ʔ��ˌ��̌v�Z
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
			//�S�ĉ��Z����
            shadeColor.rgb += (diffuse + specular) * dirLights[i].lightColor;
        }
    }
	
    //�A�e�ƃe�N�X�`���̐F������
    output.target0 = shadeColor * texcolor;
    
    output.target1 = float4((shadeColor * texcolor).rgb, 1);
    
	//�V�F�[�f�B���O�ɂ��F�ŕ`��
    return output;
}