#include"Object3D.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL,float2 uv : TEXCOORD)
{ 
    //�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);
    
    //���_���王�_�ւ̕����x�N�g��
    float3 eyedir = normalize(cameraPos - wpos.xyz);
   
    //�����ˌ�
    float3 ambient = m_ambient;
	//�g�U���ˌ�
    float3 diffuse = dot(lightv, wnormal.xyz) * m_diffuse;
	 
	//���ʔ��ˌ��̌v�Z
    float3 eyeDir = normalize(cameraPos - wpos.xyz);
	
	//����x
    const float shininess = 4.0f;
	//���ˌ��x�N�g��
    float3 reflect = normalize(-lightv + 2 * dot(-lightv, wnormal.xyz) * wnormal.xyz);
	
	//���ʔ��ˌ�
    float3 specuar = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;
	
    VSOutput output; //�s�N�Z���V�F�[�_�[�ɓn���l
    
    output.svpos = mul(mul(viewproj, world), pos);
    output.color.rgb = (ambient + diffuse + specuar) * lightcolor;
    output.color.a = m_alpha;
    
    output.uv = uv;
    return output;
    
}