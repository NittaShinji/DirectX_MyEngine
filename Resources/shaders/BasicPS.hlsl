#include"Basic.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

//float4 main(VSOutput input) : SV_TARGET
//{
//    //float3 light = normalize(float3(1, -1, 1)); // �E�����@�����̃��C�g
//    //float light_diffuse = saturate(dot(-light, input.normal)); //diffuse��[0,1]�͈̔͂�clamp����
//    //float3 shade_color;
    
//    //shade_color = m_ambient; // �A���r�G���g��
//    //shade_color += m_diffuse * light_diffuse; //�f�B�t���[�Y��
//    //float4 texcolor = float4(tex.Sample(smp, input.uv));
	
//    //return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha);
    
//    return float4(tex.Sample(smp, input.uv));

//}

//cbuffer ConstBufferDataMaterial : register(b0)
//{
//    float4 color; //�F(RGBA)
//}

float4 main(VSOutput input) : SV_TARGET
{
    //return color;
    //return float4(0.1f, 0.1f, 0.1f, 1.0f);
    return float4(tex.Sample(smp, input.uv));
    //return float4(input.uv, 0, 1);

}