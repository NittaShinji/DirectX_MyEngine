#include"Basic.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = float4(tex.Sample(smp,input.uv));
	float3 light = normalize(float3(1,-1,1));// �E�����@�����̃��C�g
	float diffuse = saturate(dot(-light, input.normal)); //diffuse��[0,1]�͈̔͂�clamp����
	//float brightness = dot(-light, input.normal); // �����ւ̃x�N�g���Ɩ@���x�N�g���̓���
	float brightness = diffuse + 0.3f; // �A���r�G���g����0.3�Ƃ��Čv�Z

	return float4(texcolor.rgb * brightness, texcolor.a) * color; 

	//return float4(input.normal,1);	//RGB�����ꂼ��@����XYZ,A��1�ŏo��
	//return float4(tex.Sample(smp,input.uv)) * color;
	//return float4(1,1,1,1);
}